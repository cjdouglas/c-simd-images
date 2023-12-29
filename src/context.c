#include "context.h"

#include <glad/glad.h>
// Ensure glad is included above first

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define START_WIDTH 800
#define START_HEIGHT 600

typedef struct {
  GLFWwindow* window;
  int width;
  int height;
} simd_img_context;

simd_img_context global_context;

void _resize_callback(GLFWwindow* window, int w, int h) {
  glViewport(0, 0, w, h);
  global_context.width = w;
  global_context.height = h;
}

void _handle_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void simd_context_start() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  global_context.window =
      glfwCreateWindow(START_WIDTH, START_HEIGHT, "SIMD Images", NULL, NULL);
  global_context.width = START_WIDTH;
  global_context.height = START_HEIGHT;
  if (!global_context.window) {
    fprintf(stderr, "Failed to create window\n");
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(global_context.window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    exit(-1);
  }

  // TODO: GL_CHECK all gl calls
  glViewport(0, 0, global_context.width, global_context.height);
  glfwSetFramebufferSizeCallback(global_context.window, _resize_callback);
  while (!glfwWindowShouldClose(global_context.window)) {
    _handle_input(global_context.window);

    // Render pass
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(global_context.window);
    glfwPollEvents();
  }

  glfwTerminate();
}