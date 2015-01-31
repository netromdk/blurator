# Static library for test cases.
include_directories(../src/blurator)

set(LIBNAME "testlib")
add_library(
  ${LIBNAME}
  STATIC

  Test.h
  Test.cpp

  ../src/blurator/Util.h
  ../src/blurator/Util.cpp

  ../src/blurator/Detector.h
  ../src/blurator/Detector.cpp

  ../src/blurator/res.qrc
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
  TestDetectorValid
  ${LIBNAME}
  TestDetectorValid.cpp
  )

add_blurator_test(
  TestFaceDetectorEmptyImage
  ${LIBNAME}
  TestFaceDetectorEmptyImage.cpp
  )
