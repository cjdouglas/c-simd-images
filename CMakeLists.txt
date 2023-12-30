cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project("simd")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES src/main.c src/context.c src/image.c src/overlay.c src/renderer.c
            src/shader.c)

add_executable(simd_run ${SOURCES})

include(FetchDependencies)
include(CompileFlags)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_include_directories(simd_run SYSTEM PUBLIC external)
target_include_directories(simd_run PUBLIC include)
target_link_libraries(simd_run glfw glad PkgConfig::GTK4)
apply_compile_flags(simd_run)
