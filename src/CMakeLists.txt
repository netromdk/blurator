add_subdirectory(core)
add_subdirectory(blurator)

if (${ENABLE_POSSAMP} MATCHES "YES")
  add_subdirectory(possamp)
endif()