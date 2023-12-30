#include "image.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ALIGNMENT 16
#define CHANNELS 3

stbi_uc* raw_image;
si_image buf1;
si_image buf2;
bool buf_flag = true;

void si_image_load(const char* path) {
  si_image_destroy();
  int width, height, channels;
  raw_image = stbi_load(path, &width, &height, &channels, CHANNELS);
  if (!raw_image) {
    fprintf(stderr, "Could not load image\n");
    exit(-1);
  }

  buf1.width = width;
  buf1.height = height;
  buf1.channels = channels;
  buf2.width = width;
  buf2.height = height;
  buf2.channels = channels;

#ifndef _WIN32
  const size_t size = (size_t)(width * height * channels);
  printf("size = %lu\n", size);
  buf1.data = aligned_alloc(ALIGNMENT, size);
  if (!buf1.data) {
    fprintf(stderr, "Could not allocate buf1.data\n");
    exit(-1);
  }
  printf("Trying to copy buf1\n");
  memcpy(buf1.data, raw_image, size);

  buf2.data = aligned_alloc(ALIGNMENT, size);
  if (!buf2.data) {
    fprintf(stderr, "Could not allocate buf2.data\n");
    exit(-1);
  }
  printf("Trying to copy buf2\n");
  memcpy(buf2.data, raw_image, size);
#else
  // TODO: windows implementation
#endif
}

void si_image_swap_buffers() { buf_flag = !buf_flag; }

const si_image* si_image_active() { return buf_flag ? &buf1 : &buf2; }

const si_image* si_image_inactive() { return buf_flag ? &buf2 : &buf1; }

void si_image_destroy() {
  if (raw_image) {
    stbi_image_free(raw_image);
#ifndef _WIN32
    free(buf1.data);
    free(buf2.data);
#else
// TODO: windows implementation
#endif
    buf1.data = NULL;
    buf2.data = NULL;
  }
}
