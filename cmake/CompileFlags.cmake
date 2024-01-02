function(apply_compile_flags target)
  if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
      ${target}
      PRIVATE -Wall
              -Wextra
              -Werror
              -Wshadow
              -Wconversion
              -Wsign-conversion
              -Wdouble-promotion
              -Wimplicit-fallthrough
              -Wno-unused-parameter
              -Wno-unused-variable)
  elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    message(WARNING "MSVC still under development")
  else()
    message(
      WARNING
        "Non standard compiler detected, no additional compilation flags specified"
    )
  endif()
endfunction()
