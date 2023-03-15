#include "test.h"

int main(void) {

  uint32_t canvas[WIDTH * HEIGHT];

  zaitoon_fill(canvas, WIDTH, HEIGHT, BACKGROUNG);

  // top border
  zaitoon_draw_line(canvas, WIDTH, HEIGHT, 0, 0, WIDTH - 1, 0, FOREGROUNG);
  // left border
  zaitoon_draw_line(canvas, WIDTH, HEIGHT, 0, 0, 0, HEIGHT - 1, FOREGROUNG);
  // bottom border
  zaitoon_draw_line(canvas, WIDTH, HEIGHT, WIDTH - 1, HEIGHT - 1, 0, HEIGHT - 1,
                    FOREGROUNG);
  // right border
  zaitoon_draw_line(canvas, WIDTH, HEIGHT, WIDTH - 1, HEIGHT - 1, WIDTH - 1, 0,
                    FOREGROUNG);
  // diagonal '\'
  zaitoon_draw_line(canvas, WIDTH, HEIGHT, 0, 0, WIDTH - 1, HEIGHT - 1,
                    FOREGROUNG);
  // diagonal '/'
  zaitoon_draw_line(canvas, WIDTH, HEIGHT, WIDTH - 1, 0, 0, HEIGHT - 1,
                    FOREGROUNG);


  const char *file_name = "line.ppm";
  Errno err = zaitoon_save_to_ppm(canvas, WIDTH, HEIGHT, file_name);
  if (err){
    printf("failed to save file %s : %s\n", file_name, strerror(errno));
	return 1;
  }

  return 0;
}
