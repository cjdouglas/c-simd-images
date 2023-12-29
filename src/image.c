#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define FORCE_CHANNELS 3

simd_image global_simd_image;

void simd_image_load(const char* path) {
  simd_image_destroy();
  global_simd_image.data =
      stbi_load(path, &global_simd_image.width, &global_simd_image.height,
                &global_simd_image.channels, FORCE_CHANNELS);
}

const simd_image* simd_image_current() { return &global_simd_image; }

void simd_image_destroy() {
  if (global_simd_image.data) {
    stbi_image_free(global_simd_image.data);
    global_simd_image.data = NULL;
  }
}
