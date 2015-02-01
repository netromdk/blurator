SET(NAME core)

add_library(
  ${NAME}

  Util.h
  Util.cpp
  )

set_blurator_links(${NAME})