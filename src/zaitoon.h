#ifndef ZAITOON_H
#define ZAITOON_H

#include <err.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _return_defer(err)                                                     \
  {                                                                            \
    result = err;                                                              \
    goto defer;                                                                \
  }

typedef int Errno;

Errno zaitoon_save_to_ppm(uint32_t *canvas, size_t canvas_width,
                          size_t canvas_height, const char *file_path);

uint32_t zaitoon_mix_colors(uint32_t foreground, uint32_t background);

void zaitoon_fill(uint32_t *canvas, size_t canvas_width, size_t canvas_height,
                  uint32_t color);

void zaitoon_draw_rect(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x, int y, int rect_width,
                       int rect_height, uint32_t color);
void zaitoon_fill_rect(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x, int y, int rect_width,
                       int rect_height, uint32_t color);

void zaitoon_draw_line(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x1, int y1, int x2, int y2,
                       uint32_t color);

void zaitoon_fill_ellipse(uint32_t *canvas, size_t canvas_width,
                         size_t canvas_height, int x, int y, size_t horz_radios,
                         size_t vert_radios, uint32_t color);
void zaitoon_draw_ellipse(uint32_t *canvas, size_t canvas_width,
                         size_t canvas_height, int x, int y, size_t horz_radios,
                         size_t vert_radios, uint32_t color);

void zaitoon_draw_trig(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x1, int y1, int x2, int y2,
                       int x3, int y3, uint32_t color);
void zaitoon_fill_trig(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x1, int y1, int x2, int y2,
                       int x3, int y3, uint32_t color);

#endif // !ZAITOON_H
