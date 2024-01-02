#include "context.h"

#include <glad/glad.h>
// Ensure glad is included above first

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "algorithm.h"
#include "renderer.h"
#include "utils.h"

#define WINDOW_SIZE 800

struct si_image_context {
  GLFWwindow* window;
  int width;
  int height;
};

struct si_image_context context;

void _resize_callback(GLFWwindow* window, int w, int h) {
  GL_CHECK(glViewport(0, 0, w, h));
  context.width = w;
  context.height = h;
}

void _handle_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    sse_apply_brightness(50);
  }
}

void si_context_start() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  context.window =
      glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "SIMD Images", NULL, NULL);
  context.width = WINDOW_SIZE;
  context.height = WINDOW_SIZE;
  if (!context.window) {
    fprintf(stderr, "Failed to create window\n");
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(context.window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    exit(-1);
  }

  const GLubyte* version_string = glGetString(GL_VERSION);
  if (version_string) {
    printf("OpenGL version: %s\n", version_string);
  }

  renderer_init();

  GL_CHECK(glViewport(0, 0, context.width, context.height));
  glfwSetFramebufferSizeCallback(context.window, _resize_callback);
  while (!glfwWindowShouldClose(context.window)) {
    _handle_input(context.window);

    // Render pass
    renderer_draw();

    glfwSwapBuffers(context.window);
    glfwPollEvents();
  }

  renderer_destroy();
  glfwTerminate();
}
