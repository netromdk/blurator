include(compilation)

set(EXTRA_LIBS)
include(deps)

if (APPLE)
  include(mac)
endif()
