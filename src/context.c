#include "context.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "algorithm.h"
#include "renderer.h"
#include "utils.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"
// Nuklear definitions and includes

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

struct si_image_context {
  GLFWwindow* window;
  int width;
  int height;
};

struct si_overlay_context {
  struct nk_context* ctx;
  GLFWwindow* window;
  int width;
  int height;
};

struct window_context {
  GLFWwindow* window;
  int width;
  int height;
};

struct window_context texture_window;
struct window_context nuklear_window;
bool should_exit = false;

struct si_image_context image_context;
struct si_overlay_context overlay_context;

static void handle_input(GLFWwindow* window) {
  if (glfwWindowShouldClose(window)) {
    should_exit = true;
  }

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    should_exit = true;
  }
}

static void resize_callback(GLFWwindow* window, int w, int h) {
  if (window == texture_window.window) {
    texture_window.width = w;
    texture_window.height = h;
  } else {
    nuklear_window.width = w;
    nuklear_window.height = h;
  }
}

static int window_create(struct window_context* context, const char* title,
                         int width, int height) {
  GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) {
    return 0;
  }
  context->window = window;
  context->width = width;
  context->height = height;
  return 1;
}

void si_context_start() {
  if (!glfwInit()) {
    fprintf(stderr, "GLFW failed to init\n");
    exit(1);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  if (!window_create(&texture_window, "SIMD Image Algorithms", 800, 800)) {
    fprintf(stderr, "Could not create texture window\n");
    exit(1);
  }

  if (!window_create(&nuklear_window, "Effects", 200, 600)) {
    fprintf(stderr, "Could not create nuklear window\n");
    exit(1);
  }

  glfwSetFramebufferSizeCallback(texture_window.window, resize_callback);
  glfwSetFramebufferSizeCallback(nuklear_window.window, resize_callback);

  glewExperimental = GL_TRUE;
  glfwMakeContextCurrent(texture_window.window);
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "GLEW failed to init\n");
    exit(1);
  }
  renderer_init();

  glfwMakeContextCurrent(nuklear_window.window);
  glewInit();

  struct nk_glfw glfw = {0};
  struct nk_context* ctx =
      nk_glfw3_init(&glfw, nuklear_window.window, NK_GLFW3_INSTALL_CALLBACKS);
  {
    struct nk_font_atlas* atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas);
    struct nk_font* font =
        nk_font_atlas_add_from_file(atlas, "../fonts/DroidSans.ttf", 18, 0);
    nk_glfw3_font_stash_end(&glfw);
    nk_style_set_font(ctx, &font->handle);
  }

  float brightness_value = 0.0F;
  while (!should_exit) {
    // Texture window rendering
    handle_input(texture_window.window);
    glfwMakeContextCurrent(texture_window.window);
    GL_CHECK(glViewport(0, 0, texture_window.width, texture_window.height));
    renderer_draw();
    glfwSwapBuffers(texture_window.window);

    // Nuklear window rendering
    handle_input(nuklear_window.window);
    glfwMakeContextCurrent(nuklear_window.window);
    nk_glfw3_new_frame(&glfw);
    if (nk_begin(ctx, "Apply Effects",
                 nk_rect(0, 0, (float)nuklear_window.width,
                         (float)nuklear_window.height),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
      nk_layout_row_dynamic(ctx, 30, 1);
      nk_slider_float(ctx, -1.0F, &brightness_value, 1.0F, 0.1F);
      nk_layout_row_dynamic(ctx, 30, 1);
      if (nk_button_label(ctx, "Apply")) {
        printf("Apply clicked\n");
      }
    }
    nk_end(ctx);

    GL_CHECK(glViewport(0, 0, nuklear_window.width, nuklear_window.height));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER,
                    MAX_ELEMENT_BUFFER);
    glfwSwapBuffers(nuklear_window.window);

    glfwPollEvents();
  }

  renderer_destroy();
  glfwDestroyWindow(texture_window.window);
  glfwDestroyWindow(nuklear_window.window);
  glfwTerminate();
}
