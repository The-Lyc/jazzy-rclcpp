cmake_minimum_required(VERSION 3.20)

project(rclcpp)

option(EXP_LATENCY "Enable experiment-latency in rclcpp" OFF)
option(EXP_SEP "set separate flag true" OFF)
option(TEST_NEW "Enable test new" OFF)
option(TEST_WHY "Enable test why" OFF)
option(EXP_QOS "Enable experiment-qos in rclcpp" OFF)

if(EXP_LATENCY)
  add_compile_definitions(EXP_LATENCY)
elseif(EXP_LATENCY STREQUAL "OFF")
  message(STATUS "EXP_LATENCY is set to OFF")
endif()

if(EXP_SEP)
  add_compile_definitions(EXP_SEP)
endif()

if(TEST_NEW)
  add_compile_definitions(TEST_NEW)
endif()

if(TEST_WHY)
  add_compile_definitions(TEST_WHY)
endif()

if(EXP_QOS)
  add_compile_definitions(EXP_QOS)
endif()

find_package(Threads REQUIRED)

find_package(ament_cmake_ros REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(libstatistics_collector REQUIRED)
find_package(rcl REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(rcl_logging_interface REQUIRED)
find_package(rcl_yaml_param_parser REQUIRED)
find_package(rcpputils REQUIRED)
find_package(rcutils REQUIRED)
find_package(rmw REQUIRED)
find_package(rosgraph_msgs REQUIRED)
find_package(rosidl_dynamic_typesupport REQUIRED)
find_package(rosidl_runtime_c REQUIRED)
find_package(rosidl_runtime_cpp REQUIRED)
find_package(rosidl_typesupport_c REQUIRED)
find_package(rosidl_typesupport_cpp REQUIRED)
find_package(statistics_msgs REQUIRED)
find_package(tracetools REQUIRED)

# TODO(wjwwood): remove this when gtest can build on its own, when using target_compile_features()
# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # About -Wno-sign-conversion: With Clang, -Wconversion implies -Wsign-conversion. There are a number of
  # implicit sign conversions in rclcpp and gtest.cc, see https://ci.ros2.org/job/ci_osx/9265/.
  # Hence disabling -Wsign-conversion for now until all those have eventually been fixed.
  # (from https://github.com/ros2/rclcpp/pull/1188#issuecomment-650229140)
  add_compile_options(-Wall -Wextra -Wconversion -Wno-sign-conversion -Wpedantic -Wnon-virtual-dtor -Woverloaded-virtual)
endif()

set(${PROJECT_NAME}_SRCS
  src/rclcpp/any_executable.cpp
  src/rclcpp/callback_group.cpp
  src/rclcpp/client.cpp
  src/rclcpp/clock.cpp
  src/rclcpp/context.cpp
  src/rclcpp/contexts/default_context.cpp
  src/rclcpp/create_generic_client.cpp
  src/rclcpp/detail/add_guard_condition_to_rcl_wait_set.cpp
  src/rclcpp/detail/resolve_intra_process_buffer_type.cpp
  src/rclcpp/detail/resolve_parameter_overrides.cpp
  src/rclcpp/detail/rmw_implementation_specific_payload.cpp
  src/rclcpp/detail/rmw_implementation_specific_publisher_payload.cpp
  src/rclcpp/detail/rmw_implementation_specific_subscription_payload.cpp
  src/rclcpp/detail/utilities.cpp
  src/rclcpp/duration.cpp
  src/rclcpp/dynamic_typesupport/dynamic_message.cpp
  src/rclcpp/dynamic_typesupport/dynamic_message_type.cpp
  src/rclcpp/dynamic_typesupport/dynamic_message_type_builder.cpp
  src/rclcpp/dynamic_typesupport/dynamic_message_type_support.cpp
  src/rclcpp/dynamic_typesupport/dynamic_serialization_support.cpp
  src/rclcpp/event.cpp
  src/rclcpp/exceptions/exceptions.cpp
  src/rclcpp/executable_list.cpp
  src/rclcpp/executor.cpp
  src/rclcpp/executor_options.cpp
  src/rclcpp/executors.cpp
  src/rclcpp/executors/executor_entities_collection.cpp
  src/rclcpp/executors/executor_entities_collector.cpp
  src/rclcpp/executors/executor_notify_waitable.cpp
  src/rclcpp/executors/multi_threaded_executor.cpp
  src/rclcpp/executors/single_threaded_executor.cpp
  src/rclcpp/executors/static_single_threaded_executor.cpp
  src/rclcpp/expand_topic_or_service_name.cpp
  src/rclcpp/experimental/executors/events_executor/events_executor.cpp
  src/rclcpp/experimental/timers_manager.cpp
  src/rclcpp/future_return_code.cpp
  src/rclcpp/generic_client.cpp
  src/rclcpp/generic_publisher.cpp
  src/rclcpp/generic_subscription.cpp
  src/rclcpp/graph_listener.cpp
  src/rclcpp/guard_condition.cpp
  src/rclcpp/init_options.cpp
  src/rclcpp/intra_process_manager.cpp
  src/rclcpp/logger.cpp
  src/rclcpp/logging_mutex.cpp
  src/rclcpp/memory_strategies.cpp
  src/rclcpp/memory_strategy.cpp
  src/rclcpp/message_info.cpp
  src/rclcpp/network_flow_endpoint.cpp
  src/rclcpp/node.cpp
  src/rclcpp/node_interfaces/node_base.cpp
  src/rclcpp/node_interfaces/node_clock.cpp
  src/rclcpp/node_interfaces/node_graph.cpp
  src/rclcpp/node_interfaces/node_logging.cpp
  src/rclcpp/node_interfaces/node_parameters.cpp
  src/rclcpp/node_interfaces/node_services.cpp
  src/rclcpp/node_interfaces/node_time_source.cpp
  src/rclcpp/node_interfaces/node_timers.cpp
  src/rclcpp/node_interfaces/node_topics.cpp
  src/rclcpp/node_interfaces/node_type_descriptions.cpp
  src/rclcpp/node_interfaces/node_waitables.cpp
  src/rclcpp/node_options.cpp
  src/rclcpp/parameter.cpp
  src/rclcpp/parameter_client.cpp
  src/rclcpp/parameter_event_handler.cpp
  src/rclcpp/parameter_events_filter.cpp
  src/rclcpp/parameter_map.cpp
  src/rclcpp/parameter_service.cpp
  src/rclcpp/parameter_value.cpp
  src/rclcpp/publisher_base.cpp
  src/rclcpp/qos.cpp
  src/rclcpp/event_handler.cpp
  src/rclcpp/qos_overriding_options.cpp
  src/rclcpp/rate.cpp
  src/rclcpp/serialization.cpp
  src/rclcpp/serialized_message.cpp
  src/rclcpp/service.cpp
  src/rclcpp/signal_handler.cpp
  src/rclcpp/subscription_base.cpp
  src/rclcpp/subscription_intra_process_base.cpp
  src/rclcpp/time.cpp
  src/rclcpp/time_source.cpp
  src/rclcpp/timer.cpp
  src/rclcpp/type_support.cpp
  src/rclcpp/typesupport_helpers.cpp
  src/rclcpp/utilities.cpp
  src/rclcpp/wait_set_policies/detail/write_preferring_read_write_lock.cpp
  src/rclcpp/waitable.cpp
)

# By default, without the settings below, find_package(Python3) will attempt
# to find the newest python version it can, and additionally will find the
# most specific version.  For instance, on a system that has
# /usr/bin/python3.10, /usr/bin/python3.11, and /usr/bin/python3, it will find
# /usr/bin/python3.11, even if /usr/bin/python3 points to /usr/bin/python3.10.
# The behavior we want is to prefer the "system" installed version unless the
# user specifically tells us othewise through the Python3_EXECUTABLE hint.
# Setting CMP0094 to NEW means that the search will stop after the first
# python version is found.  Setting Python3_FIND_UNVERSIONED_NAMES means that
# the search will prefer /usr/bin/python3 over /usr/bin/python3.11.  And that
# latter functionality is only available in CMake 3.20 or later, so we need
# at least that version.
cmake_policy(SET CMP0094 NEW)
set(Python3_FIND_UNVERSIONED_NAMES FIRST)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

# "watch" template for changes
configure_file(
  "resource/logging.hpp.em"
  "logging.hpp.em.watch"
  COPYONLY
)
# generate header with logging macros
set(python_code_logging
  "import em"
  "em.invoke(['-o', 'include/rclcpp/logging.hpp', '${CMAKE_CURRENT_SOURCE_DIR}/resource/logging.hpp.em'])")
string(REPLACE ";" "$<SEMICOLON>" python_code_logging "${python_code_logging}")
add_custom_command(OUTPUT include/rclcpp/logging.hpp
  COMMAND ${CMAKE_COMMAND} -E make_directory "include/rclcpp"
  COMMAND Python3::Interpreter ARGS -c "${python_code_logging}"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/logging.hpp.em.watch"
  COMMENT "Expanding logging.hpp.em"
  VERBATIM
)
list(APPEND ${PROJECT_NAME}_SRCS
  include/rclcpp/logging.hpp)

file(GLOB interface_files "include/rclcpp/node_interfaces/node_*_interface.hpp")
foreach(interface_file ${interface_files})
  get_filename_component(interface_name ${interface_file} NAME_WE)

  # "watch" template for changes
  configure_file(
    "resource/interface_traits.hpp.em"
    "${CMAKE_CURRENT_BINARY_DIR}/${interface_name}_traits.hpp.em.watch"
    COPYONLY
  )
  set(python_${interface_name}_traits
    "import em"
    "em.invoke(['-D', 'interface_name = \\'${interface_name}\\'', '-o', 'include/rclcpp/node_interfaces/${interface_name}_traits.hpp', '${CMAKE_CURRENT_SOURCE_DIR}/resource/interface_traits.hpp.em'])")
  string(REPLACE ";" "$<SEMICOLON>" python_${interface_name}_traits "${python_${interface_name}_traits}")
  add_custom_command(OUTPUT include/rclcpp/node_interfaces/${interface_name}_traits.hpp
    COMMAND ${CMAKE_COMMAND} -E make_directory "include/rclcpp/node_interfaces"
    COMMAND Python3::Interpreter ARGS -c "${python_${interface_name}_traits}"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${interface_name}_traits.hpp.em.watch"
    COMMENT "Expanding interface_traits.hpp.em into ${interface_name}_traits.hpp"
    VERBATIM
  )
  list(APPEND ${PROJECT_NAME}_SRCS
    include/rclcpp/node_interfaces/${interface_name}_traits.hpp)

  # "watch" template for changes
  configure_file(
    "resource/get_interface.hpp.em"
    "get_${interface_name}.hpp.em.watch"
    COPYONLY
  )
  set(python_get_${interface_name}
    "import em"
    "em.invoke(['-D', 'interface_name = \\'${interface_name}\\'', '-o', 'include/rclcpp/node_interfaces/get_${interface_name}.hpp', '${CMAKE_CURRENT_SOURCE_DIR}/resource/get_interface.hpp.em'])")
  string(REPLACE ";" "$<SEMICOLON>" python_get_${interface_name} "${python_get_${interface_name}}")
  add_custom_command(OUTPUT include/rclcpp/node_interfaces/get_${interface_name}.hpp
    COMMAND ${CMAKE_COMMAND} -E make_directory "include/rclcpp/node_interfaces"
    COMMAND Python3::Interpreter ARGS -c "${python_get_${interface_name}}"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/get_${interface_name}.hpp.em.watch"
    COMMENT "Expanding get_interface.hpp.em into get_${interface_file}.hpp"
    VERBATIM
  )
  list(APPEND ${PROJECT_NAME}_SRCS
    include/rclcpp/node_interfaces/get_${interface_name}.hpp)
endforeach()

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
# TODO(wjwwood): address all deprecation warnings and then remove this
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PUBLIC "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS")
endif()
target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>")
target_link_libraries(${PROJECT_NAME} PUBLIC
  ${builtin_interfaces_TARGETS}
  libstatistics_collector::libstatistics_collector
  rcl::rcl
  ${rcl_interfaces_TARGETS}
  rcl_yaml_param_parser::rcl_yaml_param_parser
  rcpputils::rcpputils
  rcutils::rcutils
  rmw::rmw
  ${rosgraph_msgs_TARGETS}
  rosidl_dynamic_typesupport::rosidl_dynamic_typesupport
  rosidl_runtime_c::rosidl_runtime_c
  rosidl_runtime_cpp::rosidl_runtime_cpp
  rosidl_typesupport_cpp::rosidl_typesupport_cpp
  ${statistics_msgs_TARGETS}
  tracetools::tracetools
  ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  ament_index_cpp::ament_index_cpp
  rcl_logging_interface::rcl_logging_interface
)

# Causes the visibility macros to use dllexport rather than dllimport,
# which is appropriate when building the dll but not consuming it.
target_compile_definitions(${PROJECT_NAME}
  PRIVATE "RCLCPP_BUILDING_LIBRARY")

install(
  TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Export old-style CMake variables
ament_export_include_directories("include/${PROJECT_NAME}")
ament_export_libraries(${PROJECT_NAME})

# Export modern CMake targets
ament_export_targets(${PROJECT_NAME})

ament_export_dependencies(
  builtin_interfaces
  libstatistics_collector
  rcl
  rcl_interfaces
  rcl_yaml_param_parser
  rcpputils
  rcutils
  rmw
  rosgraph_msgs
  rosidl_dynamic_typesupport
  rosidl_runtime_c
  rosidl_runtime_cpp
  rosidl_typesupport_cpp
  statistics_msgs
  tracetools
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()

  add_subdirectory(test)
endif()

ament_package()

install(
  DIRECTORY include/ ${CMAKE_CURRENT_BINARY_DIR}/include/
  DESTINATION include/${PROJECT_NAME}
)

if(TEST cppcheck)
  # must set the property after ament_package()
  set_tests_properties(cppcheck PROPERTIES TIMEOUT 1200)
endif()

if(TEST cpplint)
  set_tests_properties(cpplint PROPERTIES TIMEOUT 180)
endif()

ament_generate_version_header(${PROJECT_NAME})
