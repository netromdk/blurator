# Copies files matching the glob from dir to dst.
function(COPY_FILE_GLOB dir glob dst)
  file(GLOB FILES "${dir}/*")
  foreach (f ${FILES})
    if (${f} MATCHES ${glob})
      file(
        COPY "${f}"
        DESTINATION ${dst}
        )
    endif()
  endforeach()
endfunction()