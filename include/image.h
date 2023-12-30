#ifndef _SI_IMAGE_H
#define _SI_IMAGE_H

typedef struct {
  unsigned char* data;
  int width;
  int height;
  int channels;
} si_image;

void si_image_load(const char* path);
void si_image_swap_buffers();
const si_image* si_image_active();
const si_image* si_image_inactive();
void si_image_destroy();

#endif  // _SI_IMAGE_H
