cmake_minimum_required(VERSION 3.0.0)
project(blurator)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE MinSizeRel) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
elseif (${CMAKE_BUILD_TYPE} MATCHES "Debug")
  set(CMAKE_BUILD_TYPE Debug) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -O0")
  add_definitions("-Wall")
endif()

# Versioning.
set(${PROJECT_NAME}_MAJOR_VERSION 0) 
set(${PROJECT_NAME}_MINOR_VERSION 0) 
set(${PROJECT_NAME}_BUILD_VERSION 1)

include(platform)
include(target)

add_subdirectory(src)

message(STATUS "--------------------------------------------------")
message(STATUS "BUILD: ${CMAKE_BUILD_TYPE}")
message(STATUS "VERSION: ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_BUILD_VERSION}")
message(STATUS "Using C++11 features")
message(STATUS "--------------------------------------------------")
