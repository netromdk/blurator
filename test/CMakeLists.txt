# Static library for test cases.
include_directories(../src)

set(LIBNAME "testlib")
add_library(
  ${LIBNAME}
  SHARED

  Test.h
  Test.cpp

  ../src/Util.h
  ../src/Util.cpp

  ../src/Face.h
  ../src/Face.cpp

  ../src/FaceDetector.h
  ../src/FaceDetector.cpp

  ../res.qrc
  )

qt5_use_modules(
  ${LIBNAME}
  Core
  Gui
  )

target_link_libraries(
  ${LIBNAME}
  opencv_core
  opencv_highgui
  opencv_objdetect
  opencv_imgproc
  )

# Add test cases.
add_blurator_test(
  TestFaceDetectorValid
  ${LIBNAME}
  TestFaceDetectorValid.cpp
  )
