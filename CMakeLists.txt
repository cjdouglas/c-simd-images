cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project("simd")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES src/main.c src/algorithm.c src/context.c src/image.c src/renderer.c
            src/shader.c)

add_executable(simd_run ${SOURCES})

include(FetchDependencies)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
target_include_directories(simd_run SYSTEM PUBLIC external)
target_include_directories(simd_run PUBLIC include)
target_link_libraries(simd_run glfw GLEW::glew OpenGL::GL)

include(CompileFlags)
apply_compile_flags(simd_run)
