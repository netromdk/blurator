# Qt 5
# Added some paths to help find the modules on some systems.
if (NOT WIN32)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "/usr/local/opt"
    "/usr/local/opt/qt5"
    "/usr/local/Qt-5.4.0"
    "/usr/local/Qt-5.3.2"
    "/usr/local/Qt-5.3.1"
    "/usr/local/Qt-5.3.0"
    "/usr/local/Qt-5.2.1")
else()
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "C:/qt-everywhere-opensource-src-5.4.0-w64/qtbase/lib/cmake")
endif()

set(CMAKE_AUTOMOC ON) # moc when necessary.
set(CMAKE_AUTORCC ON) # rcc when necessary.

# As moc files are generated in the binary dir, tell CMake to always
# look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

# Detect path to Qt plugins.
find_path(
  QT_PLUGINS "plugins"
  PATHS "${Qt5Core_DIR}/../../../"
  NO_DEFAULT_PATH
  )

if (${QT_PLUGINS} MATCHES "NOTFOUND")
  message(FATAL_ERROR "Could not find Qt plugins folder!")
else()
  SET(QT_PLUGINS "${QT_PLUGINS}/plugins/")
endif()

# Win: Qt requires ICU so require path to DLLs.
# Define NO_ICU to suppress this.
if (WIN32 AND (NOT NO_ICU) AND (NOT ICU_BIN_DIR))
  message("Could not find ICU!")
  message("Please define -DICU_BIN_DIR=<PATH>")
  message(FATAL_ERROR "Or suppress using -DNO_ICU=1")
endif()

# OpenCV
if (WIN32)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "C:/opencv/build")
endif()
find_package(OpenCV REQUIRED)
