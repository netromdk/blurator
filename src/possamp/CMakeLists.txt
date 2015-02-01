SET(NAME possamp)

add_executable(
  ${NAME}
  main.cpp

  MainWindow.h
  MainWindow.cpp  
  )

qt5_use_modules(
  ${NAME}
  Core
  Gui
  Widgets
  )