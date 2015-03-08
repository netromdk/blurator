include(compilation)
include(deps)

if (APPLE)
  include(mac)
endif()

if (LINUX)
  include(linux)
endif()

if (WIN32)
  include(win)
endif()