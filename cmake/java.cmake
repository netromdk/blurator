find_program(JAR_EXECUTABLE NAMES jar)

if (WIN32 AND ${JAR_EXECUTABLE} MATCHES "NOTFOUND")
  message("Could not find java jar executable!")
  message(FATAL_ERROR "It is required for making Windows distribution!")
endif()