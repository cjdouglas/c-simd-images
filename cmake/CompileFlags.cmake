function()
  if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")

  elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")

  else()
    message(
      WARNING
        "Non standard compiler detected, no additional compilation flags specified"
    )
  endif()
endfunction()
