#include "test.h"

int main(void) {
  uint32_t canvas[WIDTH * HEIGHT];

  zaitoon_fill(canvas, WIDTH, HEIGHT, BACKGROUNG);

  size_t w = 200;
  size_t cols = WIDTH / w;
  size_t h = 200;
  size_t rows = HEIGHT / h;

  for (size_t j = 0; j < rows; ++j) {
    for (size_t i = 0; i < cols; i++) {
      bool full = (i + j) % 2;
      if (full)
        zaitoon_fill_rect(canvas, WIDTH, HEIGHT, i * w, j * h, w, h,
                          FOREGROUNG);
      if (!full)
        zaitoon_draw_rect(canvas, WIDTH, HEIGHT, (i * w) + (w / 4),
                          (j * h) + (h / 4), w / 2, h / 2, FOREGROUNG);
    }
  }

  const char *file_name = "rect.ppm";
  Errno err = zaitoon_save_to_ppm(canvas, WIDTH, HEIGHT, file_name);
  if (err){
    printf("failed to save file %s : %s\n", file_name, strerror(errno));
	return 1;
  }

  return 0;
}
