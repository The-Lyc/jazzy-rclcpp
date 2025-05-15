// Copyright 2023 iRobot Corporation.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__QOS_PROMISED_QUEUE_HPP_
#define RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__QOS_PROMISED_QUEUE_HPP_

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>
#include <iostream>
#include <chrono>
#include <ctime>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <set>

#include "rclcpp/experimental/executors/events_executor/events_queue.hpp"
#include <rcl/subscription.h>

typedef std::pair<size_t,rclcpp::experimental::executors::ExecutorEvent> event_pair_t;

struct Compare_event_pair
{
  bool operator()(const event_pair_t & lhs, const event_pair_t & rhs)
  {
    return lhs.first < rhs.first;
  }
};

namespace rclcpp
{
namespace experimental
{
namespace executors
{

/**
 * @brief This class implements an EventsQueue as a simple wrapper around a std::queue.
 * It does not perform any checks about the size of queue, which can grow
 * unbounded without being pruned.
 * The simplicity of this implementation makes it suitable for optimizing CPU usage.
 */
class QosPromisedQueue : public EventsQueue
{
public:
  RCLCPP_PUBLIC
  QosPromisedQueue()
  {
    #ifdef EXP_LATENCY
    start_time_ = std::chrono::steady_clock::now();
    std::cout<<"------------------create txt------------------"<<std::endl;
    // open file to store the latency
    outfile.open(file_name, std::ios::out | std::ios::app);
    if (!outfile.is_open()) {
      std::cerr << "Failed to open file: " << file_name << std::endl;
    }
    #endif
  }

  RCLCPP_PUBLIC
  ~QosPromisedQueue() override = default;

  /**
   * @brief enqueue event into the queue
   * Thread safe
   * @param event The event to enqueue into the queue
   */
  RCLCPP_PUBLIC
  void
  enqueue(const rclcpp::experimental::executors::ExecutorEvent & event) override
  {
    rclcpp::experimental::executors::ExecutorEvent single_event = event;

    single_event.num_events = 1;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      for (size_t ev = 0; ev < event.num_events; ev++) {
        size_t priority = 99;
        if(priority_.count(single_event.entity_key)){
          priority = priority_[single_event.entity_key];
        }
        event_queue_.push({priority,single_event});
        #ifdef EXP_LATENCY
        //std::cout<<"add an event to events queue:"<<event.type<<"***now key is:"<<event.entity_key<<std::endl;
        auto time_point_ = std::chrono::steady_clock::now();
        
        if(single_event.type == TIMER_EVENT){
          enqueue_time_[single_event.entity_key].push_back(time_point_);
        }
        #endif
      }
    }
    // std::cout<<"$$$$$ now size is:"<<this->size()<<std::endl;
    events_queue_cv_.notify_one();
  }

  /**
   * @brief waits for an event until timeout, gets a single event
   * Thread safe
   * @return true if event, false if timeout
   */
  RCLCPP_PUBLIC
  bool
  dequeue(
    rclcpp::experimental::executors::ExecutorEvent & event,
    std::chrono::nanoseconds timeout = std::chrono::nanoseconds::max()) override
  {
    std::unique_lock<std::mutex> lock(mutex_);

    // Initialize to true because it's only needed if we have a valid timeout
    bool has_data = true;
    if (timeout != std::chrono::nanoseconds::max()) {
      has_data =
        events_queue_cv_.wait_for(lock, timeout, [this]() {return !event_queue_.empty();});
    } else {
      events_queue_cv_.wait(lock, [this]() {return !event_queue_.empty();});
    }

    

    if (has_data) {
      event_pair_t event_pair = event_queue_.top();
      event = event_pair.second;
      event_queue_.pop();
      if(event.type == TIMER_EVENT){
        // only timers use tokens 
        if(!tokens_.count(event.entity_key)){
            // this timer is not in our system, so execute it directly
            return true;
        }else{
          size_t token = tokens_[event.entity_key];
          if(token > 1){
              // execute frequency is lower than token's release frequency of current queue
              if(!in_hf_queue_.count(event.entity_key)){
                  // this timer in low frequency queue and has >= 1 tokens -> under QoS!
                  // need to move all timers in high frequency queue to low frequency queue
                  // hf -> lf : if tokens >= 1, reserve one token in lf
                  //            if tokens == 0, keep tokens = 0
                  is_under_qos_[event.entity_key] = true;
                  for(auto it = tokens_.begin(); it != tokens_.end(); ++it){
                    auto timer_key = it->first;
                    // if this timer is not in high frequency queue, continue
                    if(!in_hf_queue_.count(timer_key)){
                      continue;
                    }
                    // if this timer is in high frequency queue, move it to low frequency queue
                    auto token_ = it->second;
                    if(token_ >= 1){
                      // this timer in high frequency queue and has >= 1 tokens
                      // need to move it to low frequency queue and reserve 1 token
                      it->second = 1;
                      in_hf_queue_[timer_key] = false;
                    }else{
                      // this timer in high frequency queue and has 0 tokens
                      // need to move it to low frequency queue and keep tokens = 0
                      it->second = 0;
                      in_hf_queue_[timer_key] = false;
                    }                        
                  }
              }else{
                  // this timer is in high frequency queue and has >= 1 tokens -> under hf!
                  // move it and, 
                  // timers those in high frequency queue and priority lower than this one, 
                  // to low frequency queue
                  size_t current_priority = event_pair.first;
                  for(auto it = tokens_.begin(); it != tokens_.end(); ++it){
                    auto timer_key = it->first;
                    if(!in_hf_queue_.count(timer_key) || priority_[timer_key] > current_priority){
                      // this timer is not in high frequency queue or has higher priority than current one
                      // so, keep it in high frequency queue
                      continue;
                    }
                    // move it to low frequency queue
                    auto token_ = it->second;
                    if(token_ >= 1){
                      // this timer in high frequency queue and has >= 1 tokens
                      // need to move it to low frequency queue and reserve one token
                      it->second = 1;
                      in_hf_queue_[timer_key] = false;
                    }else{
                      // this timer in high frequency queue and has 0 tokens
                      // need to move it to low frequency queue and keep tokens = 0
                      it->second = 0;
                      in_hf_queue_[timer_key] = false;
                    }                        
                  }
              }
          }else if(token == 1){
              // execute frequency equal to token's release frequency of current queue
              // this timer should be keep staying in current queue
              // so, take one token from it and dequeue it
              tokens_[event.entity_key]--;
              // check whether this timer was under QoS or not
              if(is_under_qos_[event.entity_key]){
                  is_under_qos_.erase(event.entity_key);
              }
              //return true;
          }else{
            // execute frequency is higher than token's release frequency of current queue
            if(!in_hf_queue_.count(event.entity_key)){
                // this timer is not in high frequency queue and has 0 tokens
                // check whether this timer has the highest priority in the queue
                size_t current_priority = event_pair.first;
                bool flag = true;
                for(auto it : tokens_){
                    auto timer_key = it.first;
                    if(!in_hf_queue_.count(timer_key)){
                        continue;
                    }
                    if(priority_[timer_key] > current_priority){
                        // this timer has higher priority than current one
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    // this timer has the highest priority in the queue
                    // additionally check whether there's still timers under QoS
                    // if no timer under QoS, dequeue this timer even if its tokens is 0
                    if(is_under_qos_.size()){
                        in_hf_queue_[event.entity_key] = true;
                        tokens_[event.entity_key] = 0;
                        //return true;
                    }
                }
                // else, either this timer is not the highest priority in the queue
                // or there are still timers under QoS, 
                // so we need to check
                // so we omit this timer for higher priority ones or under QoS ones
                return false;
            }else{
                // this timer is in high frequency queue and has 0 tokens
                // this is impossible, do nothing and skip
                return false;
            }
          }
        }
      }
      #ifdef EXP_LATENCY
      // add count for each event
      int index_ = cnt_[event.entity_key];
      cnt_[event.entity_key]++;
      // get actual execute time
      auto now_ = std::chrono::steady_clock::now();
      std::pair<int,int> pos;
      if(event.type == TIMER_EVENT){
        auto time_point_ = enqueue_time_[event.entity_key][index_];
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now_ - time_point_).count();
        // store the latency of current timer(connected with index_)
        latency_[event.entity_key].push_back(duration_ms);
        pos= timer_info_[event.entity_key];
      }
      else{
        if(!callback_to_timer_.count(event.entity_key)){
          return true;
        }
        // get the timer of this event
        auto timer_ = callback_to_timer_[event.entity_key];
        // get the enqueue time of this timer
        auto timer_enqueue_time_ = enqueue_time_[timer_][index_];
        // get delta time of this event
        auto delta_time_ = delta_[event.entity_key];
        // calculate the latency of this event
        auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(now_ - timer_enqueue_time_).count() - delta_time_;
        // store the latency of this event
        latency_[event.entity_key].push_back(latency);
        pos = callback_info_[event.entity_key];
      } 
      auto duration = now_ - start_time_;
      auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
      auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000;
      // output position and latency of this event
      outfile <<"record time:"<<milliseconds<<"."<<microseconds
              <<";position:"<<pos.first<<","<<pos.second
              <<";latency:"<<latency_[event.entity_key][latency_[event.entity_key].size()-1]<<std::endl;    
      #endif
      return true;
    }

    return false;
  }

  /**
   * @brief Test whether queue is empty
   * Thread safe
   * @return true if the queue's size is 0, false otherwise.
   */
  RCLCPP_PUBLIC
  bool
  empty() const override
  {
    std::unique_lock<std::mutex> lock(mutex_);
    return event_queue_.empty();
  }

  /**
   * @brief Returns the number of elements in the queue.
   * Thread safe
   * @return the number of elements in the queue.
   */
  RCLCPP_PUBLIC
  size_t
  size() const override
  {
    std::unique_lock<std::mutex> lock(mutex_);
    return event_queue_.size();
  }

  #ifdef EXP_LATENCY
  /**
   * @brief register event
   * @param event_key The event to register
   * @param pos The position of the event
   * @param delta_time The delta time of the event
   */
  RCLCPP_PUBLIC
  void
  register_event(
    const void * timer_key,
    const void * event_key,
    [[maybe_unused]] bool is_critical,
    const std::pair<int, int> pos,
    const std::chrono::milliseconds::rep delta_time) override
  {
    // if this event is not a timer event, store its timer
    if(pos.first != 0){
      callback_to_timer_[event_key] = timer_key;
    }

    // store the position of this event
    if(timer_key == event_key)  timer_info_[event_key] = pos;
    else  callback_info_[event_key] = pos;
    // store the delta time of this event
    delta_[event_key] = delta_time;
  }
  #endif

  /**
   * @brief register event's priority
   * @param event_key The event to register
   * @param priority The priority of the event
   */
  RCLCPP_PUBLIC
  void
  register_priority(
    const void * event_key,
    size_t priority)
  {
    // store the priority of this event
    priority_[event_key] = priority;
  }
  
  /**
   * @brief add a token to the event, return true if this event is only used to produce token, 
   * so don't enqueue it
   * @param event_key The event to register
   */
  RCLCPP_PUBLIC
  bool
  add_token(
    const void * event_key)
  {
    size_t hf_ = hf_token_producers_.count(event_key);
    size_t lf_ = lf_token_producers_.count(event_key);
    // check whether this event is in our system
    if(!hf_ && !lf_){
      // this event is not in our system, so do nothing
      return false;
    }
    auto event_ = producer_to_event_[event_key];
    // check whether the queue we want to add is the queue timer is in
    // add token to this event
    if(lf_ && !in_hf_queue_[event_]){
      // It's a lf token and this event is in low frequency queue
      tokens_[event_]++;
    }else if(hf_ && in_hf_queue_[event_]){
      // It's a hf token and this event is in high frequency queue
      tokens_[event_]++;
    }

    if(lf_)   return true;
    else   return false;
  }

  RCLCPP_PUBLIC
  void
  register_token_producer(
    const void * producer_key,
    const void * consumer_key,
    bool is_hf_)
  {
    // store this producer to the right set
    if(is_hf_){
      hf_token_producers_.insert(producer_key);
    }else{
      lf_token_producers_.insert(producer_key);
    }
    // store the connection between the producer and the consumer
    producer_to_event_[producer_key] = consumer_key;
    // init the state of this consumer
    tokens_[consumer_key] = 0;
    in_hf_queue_[consumer_key] = true;
    is_under_qos_[consumer_key] = false;
  }

private:
  // The underlying queue implementation
  std::priority_queue<event_pair_t,std::vector<event_pair_t>,Compare_event_pair> event_queue_;
  // The tokens bucket
  std::map<const void *, size_t> tokens_;
  // The map used to store the priority of the events
  std::map<const void *, size_t> priority_;
  // To store whether this event is in high frequency queue or not
  std::map<const void *, bool> in_hf_queue_;
  // To store whether this event is under QoS or not
  std::map<const void *, bool> is_under_qos_;
  // The producers of the two token buckets
  std::set<const void *> hf_token_producers_;
  std::set<const void *> lf_token_producers_;
  // The connection between the event and the producer
  std::map<const void *, const void *> producer_to_event_;

  #ifdef EXP_LATENCY
  // The queue used to store the time points of the events
  // This is used to keep track of the time points of the events
  std::chrono::steady_clock::time_point start_time_;
  std::map<const void *, std::vector<std::chrono::steady_clock::time_point>> enqueue_time_;
  std::map<const void *, int> cnt_;
  std::map<const void *, int> type_;
  std::map<const void *, std::pair<int,int>> timer_info_;
  std::map<const void *, std::pair<int,int>> callback_info_;
  std::map<const void *, std::vector<std::chrono::milliseconds::rep>> latency_;
  std::map<const void *, const void *> callback_to_timer_;
  std::map<const void *, std::chrono::milliseconds::rep> delta_;

  std::ofstream outfile;
  std::string file_name = "/home/lyc/wkspace/exp4_timermanager_latency/latency.txt";
  #endif
  // Mutex to protect read/write access to the queue
  mutable std::mutex mutex_;
  // Mutex to protect read/write access to the priority
  mutable std::mutex mutex_priority_;
  // Variable used to notify when an event is added to the queue
  std::condition_variable events_queue_cv_;
};

}  // namespace executors
}  // namespace experimental
}  // namespace rclcpp

#endif  // RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__QOS_PROMISED_QUEUE_HPP_
