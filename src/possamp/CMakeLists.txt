SET(NAME possamp)

add_executable(
  ${NAME}
  main.cpp

  Util.h
  Util.cpp

  MainWindow.h
  MainWindow.cpp  
  )

qt5_use_modules(
  ${NAME}
  Core
  Gui
  Widgets
  )