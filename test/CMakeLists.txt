# Static library for test cases.
include_directories(../src)

set(LIBNAME "testlib")
add_library(
  ${LIBNAME}
  STATIC

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
  TestImageConversion
  ${LIBNAME}
  TestImageConversion.cpp
  )

add_blurator_test(
  TestImageConversionAlpha
  ${LIBNAME}
  TestImageConversionAlpha.cpp
  )

add_blurator_test(
  TestImageConversionMat
  ${LIBNAME}
  TestImageConversionMat.cpp
  )

add_blurator_test(
  TestRectConversion
  ${LIBNAME}
  TestRectConversion.cpp
  )

add_blurator_test(
  TestFace
  ${LIBNAME}
  TestFace.cpp
  )

add_blurator_test(
  TestFaceDetectorValid
  ${LIBNAME}
  TestFaceDetectorValid.cpp
  )

add_blurator_test(
  TestFaceDetectorEmptyImage
  ${LIBNAME}
  TestFaceDetectorEmptyImage.cpp
  )
