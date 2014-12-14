add_executable(
  ${PROJECT_NAME}
  main.cpp

  Util.h
  Util.cpp

  Face.h
  Face.cpp

  FaceDetector.h
  FaceDetector.cpp

  ../res.qrc
  )

set_blurator_links(${PROJECT_NAME})