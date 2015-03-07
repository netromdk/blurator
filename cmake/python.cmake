find_program(
  PYTHON_EXECUTABLE
  NAMES python3.4 python3.3 python3.2 python3.1 python3.0 python3
  )

if (${PYTHON_EXECUTABLE} MATCHES "NOTFOUND")
  message(FATAL_ERROR "Could not find python 3 executable!")
endif()
