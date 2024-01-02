#ifndef _SI_IMAGE_H
#define _SI_IMAGE_H

struct si_image {
  unsigned char* data;
  int width;
  int height;
  int channels;
};

void si_image_load(const char* path);
void si_image_swap_buffers();
struct si_image* si_image_active();
struct si_image* si_image_inactive();
void si_image_destroy();

#endif  // _SI_IMAGE_H
