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

typedef std::pair<const void *,size_t> timer_priority_t;

struct Compare_timer_priority_t
{
  bool operator()(const timer_priority_t & lhs, const timer_priority_t & rhs) const
  {
    return lhs.second > rhs.second;
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
        event_queue_.push(single_event);
        if(registered_events_.find(single_event.entity_key) != registered_events_.end())  registered_events_in_queue_++;
        #ifdef EXP_LATENCY
        //std::cout<<"add an event to events queue:"<<event.type<<"***now key is:"<<event.entity_key<<std::endl;
        auto time_point_ = std::chrono::steady_clock::now();
        
        if(single_event.type == TIMER_EVENT){
          enqueue_time_[single_event.entity_key].push_back(time_point_);
        }
        #endif
      }
    }
    events_queue_cv_.notify_one();
  }

  /**
   * @brief put this timer into buffer_, and notify
   * 
   * @param timer_key 
   * @return RCLCPP_PUBLIC void
   */
  RCLCPP_PUBLIC
  void
  increase_buffer(
    const void * timer_key
  ) override
  {
    std::unique_lock<std::mutex> lock(mutex_);
    buffer_[timer_key]++;
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
        events_queue_cv_.wait_for(lock, timeout, [this]() {
          if(!event_queue_.empty() || !buffer_empty_unsafe()) return true;
          else  return false;});
    } else {
      events_queue_cv_.wait(lock, [this]() {
        if(!event_queue_.empty() || !buffer_empty_unsafe()) return true;
        else  return false;});
    }

    if (has_data) {
      if(!get_count_in_queue()){
        auto next_timer = get_timer_from_buffer();
        if(next_timer != nullptr) {
          rclcpp::experimental::executors::ExecutorEvent new_event = {
          next_timer, nullptr, -1, ExecutorEventType::TIMER_EVENT, 1};
          event_queue_.push(new_event);
          registered_events_in_queue_++;
        } else {
          std::cout<<"!!!!!!!!!!!!get an empty next timer!!!!!!!!!!"<<std::endl;
        }
      }
      // get an event from event_queue_
      if(event_queue_.size()==0) {
        std::cout<<"!!!!!!!!!!!!get an empty event!!!!!!!!!!"<<std::endl;
      }
      event = event_queue_.front();
      event_queue_.pop();
      if(registered_events_.find(event.entity_key) != registered_events_.end()) registered_events_in_queue_--;
      
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
   * @brief Get the timer from buffer object
   * 
   * @return const void* 
   */
  const void *
  get_timer_from_buffer()
  {
    std::unique_lock<std::mutex> lock(mutex_tokens_);
    auto front = under_qos_set_.begin();
    if(front != under_qos_set_.end()){
      auto timer = front->first;
      if(hf_tokens_[timer]>0){
        consume_token_unsafe(timer);
      }
      if(lf_tokens_[timer]<0){
        // achieve QoS requirements, put this timer to under_origin_set
        under_qos_set_.erase(front);
        under_origin_set_.insert({timer,priority_[timer]});
      }
      // buffer --
      buffer_[timer]--;
      return timer;
    }

    front = under_origin_set_.begin();
    if(front != under_origin_set_.end()){
      auto timer = front->first;
      consume_token_unsafe(timer);
      buffer_[timer]--;
      return timer;
    }

    return nullptr;
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

  RCLCPP_PUBLIC
  size_t
  get_count_in_queue()
  {
    return registered_events_in_queue_;
  }

  /**
   * @brief 
   * 
   * @return RCLCPP_PUBLIC 
   */
  RCLCPP_PUBLIC
  bool
  buffer_empty_unsafe()
  {
    for(auto it : buffer_){
      if(it.second>0) return false;
    }
    return true;
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
   * @brief consume a token of this timer from both hf and lf token buckets
   * @param timer_key The timer key of the event
   */
  RCLCPP_PUBLIC
  void
  consume_token_unsafe(
    const void * timer_key
  )
  {
    hf_tokens_[timer_key]--;
    lf_tokens_[timer_key]--;
    auto timer_pair_ = std::make_pair(timer_key,priority_[timer_key]);
    
    if(under_qos_set_.find(timer_pair_)!=under_qos_set_.end()){
      // timer is in under_qos state
      if(lf_tokens_[timer_key]<=0){
        // need to remove from under qos state
        under_qos_set_.erase(timer_pair_);
        if(hf_tokens_[timer_key]>=1){
          // need to move to under origin state
          under_origin_set_.insert(timer_pair_);
        }
        // else, move to original state, do nothing
        // neither in under qos nor in under origin, is original state
      } 
    } else if(under_origin_set_.find(timer_pair_)!=under_origin_set_.end()) {
      // timer is in under origin state
      // only possible to move to original state or stay in under origin state
      if(hf_tokens_[timer_key]<=0){
        // need to move to original state
        under_origin_set_.erase(timer_pair_);
      }
    } else {
      // timer is in original state, it's impossible. skip
    }
  }
  
  /**
   * @brief add a token to the event, return true if this event is only used to produce token, 
   * so don't enqueue it
   * @param producer_key The producer key of the event
   */
  RCLCPP_PUBLIC
  bool
  produce_token(
    const void * producer_key) override
  {
    size_t hf_ = hf_token_producers_.count(producer_key);
    size_t lf_ = lf_token_producers_.count(producer_key);
    // check whether this event is in our system
    if(!hf_ && !lf_){
      // this event is not in our system, so do nothing
      return false;
    }
    auto timer_ = producer_to_timer_[producer_key];

    std::unique_lock<std::mutex> lock2(mutex_tokens_); 
    auto timer_pair_ = std::make_pair(timer_,priority_[timer_]);
    // check type of this token (hf token or lf token)
    // add token to this event
    if(lf_){
      // It's a lf token 
      // check token counts
      if(lf_tokens_[timer_]<0){
        // lf tokens have been all consumed so far.
        // A negative number indicates that this timer has met the QoS requirements 
        // and even consumed more HF tokens. 
        // Therefore, this LF token is used to compensate for the deviation.
        lf_tokens_[timer_] = 0;
        
        // timer is either in original state or under_origin state, 
        // because we haven't changed HF tokens, so timer needs to stay in its state
        // do nothing
      }else{
        // After producing this token, this timer no longer meets the QoS requirements.
        // So, we need to put this timer into under_qos_set
        lf_tokens_[timer_]++;
        if(under_origin_set_.find(timer_pair_) != under_origin_set_.end()){
          // this timer is in under_origin_set
          // so, we need to remove it from under_origin_set
          under_origin_set_.erase(timer_pair_);
          // put this timer into under_qos_set
          under_qos_set_.insert(timer_pair_);
        }   
      }
    }else if(hf_){
      // It's a HF token and add it to the HF token bucket
      hf_tokens_[timer_]++;
      // check LF token counts, if LF tokens have been all consumed so far(means this timer is in FULL state)
      // put this timer into under_origin_set
      if(lf_tokens_[timer_] <= 0){
        under_origin_set_.insert(timer_pair_);
      }
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
      hf_tokens_[consumer_key] = 0;
    }else{
      lf_token_producers_.insert(producer_key);
      lf_tokens_[consumer_key] = 0;
    }
    // store the connection between the producer and the consumer
    // consumer is just the timer connected with this producer
    producer_to_timer_[producer_key] = consumer_key;
    // init the buffer state of this consumer
    buffer_[consumer_key] = 0;
  }

  RCLCPP_PUBLIC
  void
  register_qos_event(
    const void * event_key
  )
  {
    registered_events_.insert(event_key);
  }

  RCLCPP_PUBLIC
  bool
  is_timer_in_system(
    const void * timer_key
  ) override
  {
    if(registered_events_.find(timer_key) != registered_events_.end())  return true;
    else  return false;
  }

private:
  // The timer buffer used to cache ready timers send by the timers manager and send a reasonable timer to executor
  std::priority_queue<timer_priority_t,std::vector<timer_priority_t>,Compare_timer_priority_t> timer_buffer_;
  // The underlying queue implementation
  std::queue<rclcpp::experimental::executors::ExecutorEvent> event_queue_;
  // The hf tokens bucket, key is timer, value is the number of tokens
  std::map<const void *, int> hf_tokens_;
  // The lf tokens bucket, key is timer, value is the number of tokens
  std::map<const void *, int> lf_tokens_;
  // The map used to store the priority of the timers
  std::map<const void *, size_t> priority_;
  // The producers of the two token buckets
  std::set<const void *> hf_token_producers_;
  std::set<const void *> lf_token_producers_;
  // The connection between the event and the producer
  std::map<const void *, const void *> producer_to_timer_;
  // The set storing the timers under QoS
  std::set<timer_priority_t,Compare_timer_priority_t> under_qos_set_;
  // The set storing the timers between original frequency and QoS frequency
  std::set<timer_priority_t,Compare_timer_priority_t> under_origin_set_;
  // The map used to store the whole timers' buffer
  std::map<const void *, size_t> buffer_;

  size_t registered_events_in_queue_ = 0;
  std::set<const void *> registered_events_;

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
  // Mutex to protect read/write access to the events queue
  mutable std::mutex mutex_;
  // Mutex to protect read/write access to the set
  mutable std::mutex mutex_set_;
  mutable std::mutex mutex_tokens_;
  // Variable used to notify when an event is added to the queue
  std::condition_variable events_queue_cv_;
};

}  // namespace executors
}  // namespace experimental
}  // namespace rclcpp

#endif  // RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__QOS_PROMISED_QUEUE_HPP_
