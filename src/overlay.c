#include "overlay.h"

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_FONT_BAKING
#define NK_IMPLEMENTATION
#include "nuklear.h"

//
#include <stdio.h>

struct nk_context ctx;
struct nk_font_atlas atlas;

void overlay_init() {
  nk_font_atlas_init_default(&atlas);
  nk_font_atlas_begin(&atlas);
  struct nk_font* font = nk_font_atlas_add_default(&atlas, 15, NULL);
  nk_font_atlas_end(&atlas, nk_handle_id(0), NULL);

  nk_init_default(&ctx, &font->handle);
  nk_style_set_font(&ctx, &font->handle);
  printf("ctx style font is null? = %d\n", ctx.style.font == NULL);
  printf("ctx style font width = %p\n", ctx.style.font->width);
  enum { EASY, HARD };
  int op = EASY;
  float value = 0.6f;

  if (nk_begin(&ctx, "Show", nk_rect(50, 50, 220, 220),
               NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
    /* fixed widget pixel width */
    nk_layout_row_static(&ctx, 30, 80, 1);
    if (nk_button_label(&ctx, "button")) {
      /* event handling */
    }

    /* fixed widget window ratio width */
    nk_layout_row_dynamic(&ctx, 30, 2);
    if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
    if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;

    /* custom widget pixel width */
    nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
    {
      nk_layout_row_push(&ctx, 50);
      nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
      nk_layout_row_push(&ctx, 110);
      nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
    }
    nk_layout_row_end(&ctx);
  }
  nk_end(&ctx);
}

void overlay_destroy() {
  nk_font_atlas_clear(&atlas);
  nk_free(&ctx);
}
