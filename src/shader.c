#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define LOG_SIZE 128

const char* vertex_shader =
    "#version 330 core\n"
    "layout(location = 0) in vec2 a_Position;\n"
    "layout(location = 1) in vec2 a_TexPosition;\n"
    "out vec2 v_TexPosition;\n"
    "void main() {\n"
    "  v_TexPosition = a_TexPosition;\n"
    "  gl_Position = vec4(a_Position, 0.0F, 1.0F);\n"
    "}\0";

const char* fragment_shader =
    "#version 330 core\n"
    "uniform sampler2D u_Texture;\n"
    "in vec2 v_TexPosition;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = texture(u_Texture, v_TexPosition.xy);\n"
    "}\0";

GLuint shader_create(GLenum shader_type) {
  const char* shader_source;
  const char* shader_name;
  switch (shader_type) {
    case GL_VERTEX_SHADER:
      shader_source = vertex_shader;
      shader_name = "vertex";
      break;
    case GL_FRAGMENT_SHADER:
      shader_source = fragment_shader;
      shader_name = "fragment";
      break;
    default:
      fprintf(stderr, "Unsupported shader type passed\n");
      exit(-1);
  }

  GL_CHECK(GLuint shader_id = glCreateShader(shader_type));
  GL_CHECK(glShaderSource(shader_id, 1, &shader_source, NULL));
  GL_CHECK(glCompileShader(shader_id));

  int result;
  GL_CHECK(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    char log[LOG_SIZE + 1];
    int length;
    GL_CHECK(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length));
    if (length > LOG_SIZE) {
      fprintf(stderr, "Shader log exceeds allowed buffer\n");
      exit(-1);
    }
    GL_CHECK(glGetShaderInfoLog(shader_id, length, &length, log));
    log[LOG_SIZE] = '\0';

    fprintf(stderr, "Failed to compile %s shader: %s\n", shader_name, log);
    GL_CHECK(glDeleteShader(shader_id));
    exit(-1);
  }

  return shader_id;
}
