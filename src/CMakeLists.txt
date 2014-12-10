add_executable(
  ${PROJECT_NAME}
  main.cpp

  Util.h
  Util.cpp)

QT5_USE_MODULES(${PROJECT_NAME} Core Gui)
TARGET_LINK_LIBRARIES(
  ${PROJECT_NAME}
  ${OpenCV_LIBS})