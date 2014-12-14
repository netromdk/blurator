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

set_blurator_links(${LIBNAME})

# Add test cases.
add_blurator_test(
  TestFaceDetectorValid
  ${LIBNAME}
  TestFaceDetectorValid.cpp
  )
