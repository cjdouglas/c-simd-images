#ifndef _SIMD_IMG_UTILS_H
#define _SIMD_IMG_UTILS_H

#include <glad/glad.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>

#define ASSERT(x) \
  if (!(x)) raise(SIGKILL)

#define GL_CHECK(stmt) \
  _GL_CLEAR_ERROR();   \
  stmt;                \
  ASSERT(_glLogCall(#stmt, __FILE__, __LINE__))

#define _GL_CLEAR_ERROR()                 \
  {                                       \
    while (glGetError() != GL_NO_ERROR) { \
    }                                     \
  }

#define _GL_LOG_CALL(func) _glLogCall(func, __FILE__, __LINE__)

static inline bool _glLogCall(const char* func, const char* file, int line) {
  GLenum err = glGetError();
  bool error = false;
  while (err != GL_NO_ERROR) {
    fprintf(stderr, "[OpenGL Error] (%u): %s %s %d\n", err, func, file, line);
    err = glGetError();
    error = true;
  }
  return !error;
}

#endif  // _SIMD_IMG_UTILS_H
