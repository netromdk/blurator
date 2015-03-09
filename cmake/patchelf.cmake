find_program(PATCHELF_EXECUTABLE NAMES patchelf)

if (LINUX AND ${PATCHELF_EXECUTABLE} MATCHES "NOTFOUND")
  message("Could not find patchelf executable!")
  message(FATAL_ERROR "It is required for making Linux distribution!")
endif()