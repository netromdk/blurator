include_directories(../src)

add_library(
  testlib
  STATIC

  ../src/FaceDetector.cpp
  ../src/Face.cpp
  ../src/Util.cpp
  )

qt5_use_modules(
  testlib
  Core
  Gui
  )

target_link_libraries(
  testlib
  opencv_core
  opencv_highgui
  opencv_objdetect
  opencv_imgproc
  )

set(NAME "TestFaceDetector")
add_executable(
  ${NAME}
  TestFaceDetector.cpp
  )

target_link_libraries(
  ${NAME}
  testlib
  )

add_test(
  NAME TestFaceDetector
  COMMAND ${NAME}
  )