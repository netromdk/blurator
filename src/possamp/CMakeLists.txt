SET(NAME possamp)

include_directories(../core)

add_executable(
  ${NAME}
  main.cpp

  ObjectManager.h
  ObjectManager.cpp

  MainWindow.h
  MainWindow.cpp  
  )

target_link_libraries(
  ${NAME}
  core
  )

qt5_use_modules(
  ${NAME}
  Core
  Gui
  Widgets
  )