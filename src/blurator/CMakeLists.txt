include_directories(../core)

add_executable(
  ${PROJECT_NAME}
  main.cpp

  Detector.h
  Detector.cpp

  res.qrc
  )

target_link_libraries(
  ${PROJECT_NAME}
  core
  )

set_blurator_links(${PROJECT_NAME})