# Adds the test to ctest.
# "name" is the name of the test.
# "libname" is the static testing library to link against.
# "sources" is the sources to compile the test from.
macro(add_blurator_test name libname sources)
  add_executable(
    ${name}
    ${sources}
    )

  target_link_libraries(
    ${name}
    ${libname}
    )

  add_test(
    NAME ${name}
    COMMAND ${name}
    )
endmacro()
