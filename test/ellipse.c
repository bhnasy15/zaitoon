#include "test.h"

int main(void){
  uint32_t canvas[WIDTH * HEIGHT];

  zaitoon_fill(canvas, WIDTH, HEIGHT, BACKGROUNG);

  size_t w = 200;
  size_t h = 200;

  zaitoon_fill_ellipse(canvas, WIDTH, HEIGHT, 100, 100, w, h, FOREGROUNG);

  zaitoon_fill_ellipse(canvas, WIDTH, HEIGHT, WIDTH - 100, HEIGHT - 200, w / 2,
                      h, FOREGROUNG);

  zaitoon_draw_ellipse(canvas, WIDTH, HEIGHT, WIDTH / 2, HEIGHT / 2, w, h / 2,
                      FOREGROUNG);

  zaitoon_draw_ellipse(canvas, WIDTH, HEIGHT, WIDTH / 2, HEIGHT / 2, w / 2, h,
                      FOREGROUNG);

  zaitoon_draw_ellipse(canvas, WIDTH, HEIGHT, WIDTH / 2, HEIGHT / 2, w, h,
                      FOREGROUNG);

  const char *file_name = "ellipse.ppm";
  Errno err = zaitoon_save_to_ppm(canvas, WIDTH, HEIGHT, file_name);
  if (err) {
	 printf("failed to save file %s : %s\n", file_name, strerror(errno));
	 return 1;
  }

	return 0;
}

