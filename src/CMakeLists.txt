add_executable(
  ${PROJECT_NAME}
  main.cpp

  Util.h
  Util.cpp

  Face.h
  Face.cpp

  FaceDetector.h
  FaceDetector.cpp
  )

qt5_use_modules(
  ${PROJECT_NAME}
  Core
  Gui
  )

target_link_libraries(
  ${PROJECT_NAME}
  opencv_core
  opencv_highgui
  opencv_objdetect
  opencv_imgproc
  )
