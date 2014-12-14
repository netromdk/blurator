set(BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib")

file(
  MAKE_DIRECTORY
  ${BIN_DIR}
  ${LIB_DIR}
  )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE} ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE} ${LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_DIR})

macro(set_blurator_links target)
  qt5_use_modules(
    ${target}
    Core
    Gui
    )

  target_link_libraries(
    ${target}
    opencv_core
    opencv_highgui
    opencv_objdetect
    opencv_imgproc
    )
endmacro()
