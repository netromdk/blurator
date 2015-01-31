add_executable(
  ${PROJECT_NAME}
  main.cpp

  Util.h
  Util.cpp

  Detector.h
  Detector.cpp

  res.qrc
  )

set_blurator_links(${PROJECT_NAME})