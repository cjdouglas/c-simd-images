#include "renderer.h"

#include <glad/glad.h>
#include <stdio.h>

#include "image.h"
#include "shader.h"
#include "utils.h"

const float tex_coords[16] = {
    -1.0f, 1.0f,  0.f, 0.f,  // Top left
    -1.0f, -1.0f, 0.f, 1.f,  // Bottom left
    1.0f,  1.0f,  1.f, 0.f,  // Top right
    1.0f,  -1.0f, 1.f, 1.f   // Bottom right
};

GLuint program;

GLuint tex;
GLuint vertex_buf;

GLint attr_pos;
GLint attr_tex_pos;
GLint unif_tex;

void renderer_init() {
  GL_CHECK(glEnable(GL_BLEND));
  GL_CHECK(glDisable(GL_CULL_FACE));
  GL_CHECK(glClearColor(0.0F, 0.0F, 0.0F, 1.0F));

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
  printf("%u\n", vertex_buf);
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertex_buf));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords,
                        GL_STATIC_DRAW));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GL_CHECK(glGenTextures(1, &tex));

  // TODO: move this elsewhere
  simd_image_load("/home/cdoug/Pictures/nebula.jpg");
  renderer_update_texture();
}

void renderer_update_texture() {
  const simd_image* image = simd_image_current();
  GL_CHECK(glActiveTexture(GL_TEXTURE0));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,
                        0, GL_RGB, GL_UNSIGNED_BYTE, image->data));
  GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                           GL_LINEAR_MIPMAP_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void renderer_draw() {
  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

  GL_CHECK(glUseProgram(program));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertex_buf));
  GL_CHECK(glActiveTexture(GL_TEXTURE0));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex));

  GL_CHECK(glUniform1i(unif_tex, 0));

  GL_CHECK(glEnableVertexAttribArray((GLuint)attr_pos));
  GL_CHECK(glVertexAttribPointer((GLuint)attr_pos, 2, GL_FLOAT, GL_FALSE,
                                 4 * sizeof(float), (const void*)0));
  GL_CHECK(glEnableVertexAttribArray((GLuint)attr_tex_pos));
  GL_CHECK(glVertexAttribPointer((GLuint)attr_tex_pos, 2, GL_FLOAT, GL_FALSE,
                                 4 * sizeof(float),
                                 (const void*)(2 * sizeof(float))));

  GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void renderer_destroy() { simd_image_destroy(); }
