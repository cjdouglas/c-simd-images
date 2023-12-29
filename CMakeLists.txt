cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project("simd")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES src/main.c src/context.c src/renderer.c src/shader.c)

add_executable(simd_run ${SOURCES})

include(FetchDependencies)

target_include_directories(simd_run PUBLIC include)
target_link_libraries(simd_run glfw glad)
target_compile_options(simd_run PRIVATE -Wall -Wextra -Werror)
