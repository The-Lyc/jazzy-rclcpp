# Copyright 2019 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Register an rclcpp component with the ament
# resource index and create an executable.
#
# usage: rclcpp_components_register_node(
#        <target> PLUGIN <component> EXECUTABLE <node>)
#
# :param target: the shared library target
# :type target: string
# :param PLUGIN: the plugin name
# :type PLUGIN: string
# :param EXECUTABLE: the node's executable name
# :type EXECUTABLE: string
# :param EXECUTOR: the C++ class name of the executor to use (blank uses SingleThreadedExecutor)
# :type EXECUTOR: string
# :param RESOURCE_INDEX: the ament resource index to register the components
# :type RESOURCE_INDEX: string
# :param NO_UNDEFINED_SYMBOLS: add linker flags to deny undefined symbols
# :type NO_UNDEFINED_SYMBOLS: option
#
macro(rclcpp_components_register_node target)
  cmake_parse_arguments(ARGS "NO_UNDEFINED_SYMBOLS" "PLUGIN;EXECUTABLE;EXECUTOR;RESOURCE_INDEX" "" ${ARGN})
  if(ARGS_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "rclcpp_components_register_node() called with unused "
      "arguments: ${ARGS_UNPARSED_ARGUMENTS}")
  endif()
  if("${ARGS_PLUGIN}" STREQUAL "")
    message(FATAL_ERROR "rclcpp_components_register_node macro requires a PLUGIN argument for target ${target}")
  endif()
  if("${ARGS_EXECUTABLE}" STREQUAL "")
    message(FATAL_ERROR "rclcpp_components_register_node macro requires a EXECUTABLE argument for target ${target}")
  endif()
  # default to rclcpp_components if not specified otherwise
  set(resource_index "rclcpp_components")
  if(NOT "${ARGS_RESOURCE_INDEX}" STREQUAL "")
    set(resource_index ${ARGS_RESOURCE_INDEX})
    message(STATUS "Setting component resource index to non-default value ${resource_index}")
  endif()

  # default to executor if not specified otherwise
  set(executor "SingleThreadedExecutor")
  if(NOT "${ARGS_EXECUTOR}" STREQUAL "")
    set(executor ${ARGS_EXECUTOR})
    message(STATUS "Setting executor non-default value ${executor}")
  endif()

  set(component ${ARGS_PLUGIN})
  set(node ${ARGS_EXECUTABLE})
  _rclcpp_components_register_package_hook()
  set(_path "lib")
  set(library_name "$<TARGET_FILE_NAME:${target}>")
  if(WIN32)
    set(_path "bin")
  endif()
  set(_RCLCPP_COMPONENTS_${resource_index}__NODES
    "${_RCLCPP_COMPONENTS_${resource_index}__NODES}${component};${_path}/$<TARGET_FILE_NAME:${target}>\n")
  list(APPEND _RCLCPP_COMPONENTS_PACKAGE_RESOURCE_INDICES ${resource_index})

  if(ARGS_NO_UNDEFINED_SYMBOLS AND WIN32)
    message(WARNING "NO_UNDEFINED_SYMBOLS is enabled for target \"${target}\", but this is unsupported on windows.")
  elseif(ARGS_NO_UNDEFINED_SYMBOLS AND NOT WIN32)
    check_cxx_compiler_flag("-Wl,--no-undefined" linker_supports_no_undefined)
    if(linker_supports_no_undefined)
      target_link_options("${target}" PRIVATE "-Wl,--no-undefined")
    else()
      message(WARNING "NO_UNDEFINED_SYMBOLS is enabled for target \"${target}\",\
                but the linker does not support the \"--no-undefined\" flag.")
    endif()

    check_cxx_compiler_flag("-Wl,--no-allow-shlib-undefined" linker_supports_no_allow_shlib_undefined)
    if(linker_supports_no_allow_shlib_undefined)
      target_link_options("${target}" PRIVATE "-Wl,--no-allow-shlib-undefined")
    else()
      message(WARNING "NO_UNDEFINED_SYMBOLS is enabled for target \"${target}\",\
                but the linker does not support the \"--no-allow-shlib-undefined\" flag.")
    endif()
  endif()

  configure_file(${rclcpp_components_NODE_TEMPLATE}
    ${PROJECT_BINARY_DIR}/rclcpp_components/node_main_configured_${node}.cpp.in)
  file(GENERATE OUTPUT ${PROJECT_BINARY_DIR}/rclcpp_components/node_main_${node}.cpp
    INPUT ${PROJECT_BINARY_DIR}/rclcpp_components/node_main_configured_${node}.cpp.in)
  add_executable(${node} ${PROJECT_BINARY_DIR}/rclcpp_components/node_main_${node}.cpp)
  target_link_libraries(${node}
    class_loader::class_loader
    rclcpp::rclcpp
    rclcpp_components::component
  )
  install(TARGETS
    ${node}
    DESTINATION lib/${PROJECT_NAME})
endmacro()
