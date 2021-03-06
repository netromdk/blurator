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

  ../src/Detector.h
  ../src/Detector.cpp

  ../src/res.qrc
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
  TestFaceDetectorNullImage
  ${LIBNAME}
  TestFaceDetectorNullImage.cpp
  )

add_blurator_test(
  TestFaceDetectorEmptyImage
  ${LIBNAME}
  TestFaceDetectorEmptyImage.cpp
  )

add_blurator_test(
  TestPlateDetectorNullImage
  ${LIBNAME}
  TestPlateDetectorNullImage.cpp
  )

add_blurator_test(
  TestPlateDetectorEmptyImage
  ${LIBNAME}
  TestPlateDetectorEmptyImage.cpp
  )
