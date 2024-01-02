#include "algorithm.h"

#include <stddef.h>
#include <xmmintrin.h>

#include "image.h"
#include "renderer.h"

#define RGB_MIN 0
#define RGB_MAX 255

unsigned char rgb_clamp(int value) {
  if (value < RGB_MIN) {
    return RGB_MIN;
  } else if (value > RGB_MAX) {
    return RGB_MAX;
  } else {
    return (unsigned char)value;
  }
}

void apply_brightness(unsigned char strength) {
  struct si_image* image = si_image_inactive();

  const int n_components = image->width * image->height * image->channels;
  for (int i = 0; i < n_components; ++i) {
    image->data[i] = rgb_clamp((int)image->data[i] + strength);
  }

  si_image_swap_buffers();
  renderer_update_texture();
}

void sse_apply_brightness(unsigned char strength) {
  struct si_image* image = si_image_inactive();

  __m128i sv = _mm_set1_epi8((char)strength);
  const int n_components = image->width * image->height * image->channels;

  int i = 0;
  while (i < n_components) {
    __m128i vec = _mm_load_si128((__m128i*)&image->data[i]);
    __m128i res = _mm_adds_epu8(vec, sv);
    _mm_store_si128((__m128i*)&image->data[i], res);
    i += 16;
  }

  while (i < n_components) {
    unsigned int val = (unsigned int)image->data[i] + strength;
    image->data[i++] = val > 255 ? 255 : (unsigned char)val;
  }

  si_image_swap_buffers();
  renderer_update_texture();
}
