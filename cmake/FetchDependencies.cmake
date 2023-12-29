include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.9)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
  FetchContent_Populate(glfw)

  set(GLFW_BUILD_EXAMPLES
      OFF
      CACHE INTERNAL "Build the GLFW example programs")
  set(GLFW_BUILD_TESTS
      OFF
      CACHE INTERNAL "Build the GLFW test programs")
  set(GLFW_BUILD_DOCS
      OFF
      CACHE INTERNAL "Build the GLFW documentation")
  set(GLFW_INSTALL
      OFF
      CACHE INTERNAL "Generate installation target")

  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
  FetchContent_Populate(glad)
  add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()
