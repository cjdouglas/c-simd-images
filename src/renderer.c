#include "renderer.h"

#include <glad/glad.h>

#include "shader.h"
#include "utils.h"

float tex_coords[16] = {
    -1.0F, 1.0F, 0.0F, 0.0F, -1.0F, -1.0F, 0.0F, 1.0F,
    1.0F,  1.0F, 1.0F, 0.0F, 1.0F,  -1.0F, 1.0F, 1.0F,
};

GLuint program;

GLuint tex;
GLuint vertex_buf;

GLint attr_pos;
GLint attr_tex_pos;
GLint unif_tex;

void renderer_init() {
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_CULL_FACE);

  GLuint vertex_shader = shader_create(GL_VERTEX_SHADER);
  GLuint fragment_shader = shader_create(GL_FRAGMENT_SHADER);
  GL_CHECK(program = glCreateProgram());
  GL_CHECK(glAttachShader(program, vertex_shader));
  GL_CHECK(glAttachShader(program, fragment_shader));
  GL_CHECK(glLinkProgram(program));

  // Get locations of shader variables
  GL_CHECK(attr_pos = glGetAttribLocation(program, "a_Position"));
  GL_CHECK(attr_tex_pos = glGetAttribLocation(program, "a_TexPosition"));
  GL_CHECK(unif_tex = glGetUniformLocation(program, "u_Texture"));

  // Generate vertex buffers and send data
  GL_CHECK(glGenBuffers(1, &vertex_buf));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertex_buf));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords,
                        GL_STATIC_DRAW));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GL_CHECK(glGenTextures(1, &tex));
}

void renderer_draw() {}

void renderer_destroy() {}
