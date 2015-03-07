cmake_minimum_required(VERSION 3.0.0)
project(blurator)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  )

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Find current version from source code.
file(
  STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/src/Version.h"
  VERSION_DATA REGEX "define.+_VERSION.+[0-9]+$"
  )
set(VERSION_NUMBER "")
foreach (ver IN LISTS VERSION_DATA)
  string(REGEX MATCH "[0-9]+" number ${ver})
  set(VERSION_NUMBER "${VERSION_NUMBER}${number}.")
endforeach()
string(LENGTH ${VERSION_NUMBER} length)
math(EXPR length "${length}-1")
string(SUBSTRING ${VERSION_NUMBER} 0 ${length} VERSION_NUMBER)

include(platform)
include(target)

add_subdirectory(src)

if (NOT ENABLE_TESTING)
  set(ENABLE_TESTING "NO")
endif()

if (${ENABLE_TESTING} MATCHES "YES")
  include(testing)
  enable_testing()
  add_subdirectory(test)
endif()

# Distribution
include(python)
add_custom_target(
  dist
  DEPENDS blurator
  COMMENT "Creating distribution"
  )
if (APPLE)
  set(ARCH "mac64")
elseif (WIN32)
  set(ARCH "win64")
elseif (LINUX)
  set(ARCH "linux64")
endif()
set(DIST_DIR "${BIN_DIR}/${PROJECT_NAME}_${ARCH}_${VERSION_NUMBER}")
set(ZIP_FILE "${PROJECT_NAME}_${ARCH}_${VERSION_NUMBER}.zip")
set(ZIP_DIST "${BIN_DIR}/${ZIP_FILE}")
if (APPLE OR LINUX)
  add_custom_command(
    TARGET dist
    POST_BUILD
    COMMAND scripts/makedist.sh
      ${BIN_DIR} ${DIST_DIR} ${ZIP_DIST} ${PYTHON_EXECUTABLE} ${QT_PLUGINS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

message(STATUS "--------------------------------------------------")
message(STATUS "BUILD: ${CMAKE_BUILD_TYPE}")
message(STATUS "VERSION: ${VERSION_NUMBER}")
message(STATUS "Building tests: ${ENABLE_TESTING}")
message(STATUS "Using C++11 features")
message(STATUS "Qt plugins: ${QT_PLUGINS}")
message(STATUS "Python: ${PYTHON_EXECUTABLE}")
message(STATUS "--------------------------------------------------")
