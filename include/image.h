#ifndef _SIMD_IMG_IMAGE_H
#define _SIMD_IMG_IMAGE_H

typedef struct {
  unsigned char* data;
  int width;
  int height;
  int channels;
} simd_image;

void simd_image_load(const char* path);
const simd_image* simd_image_current();
void simd_image_destroy();

#endif  // _SIMD_IMG_IMAGE_H
