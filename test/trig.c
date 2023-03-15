#include "test.h"

int main(void){
  uint32_t canvas[WIDTH * HEIGHT];

  zaitoon_fill(canvas, WIDTH, HEIGHT, BACKGROUNG);

  zaitoon_draw_trig(canvas, WIDTH, HEIGHT, 10, HEIGHT - 19, 500, HEIGHT - 88,
                    WIDTH - 600, HEIGHT - 555, FOREGROUNG);
  zaitoon_fill_trig(canvas, WIDTH, HEIGHT, 10, 19, 500, 88, 600, 555,
                    FOREGROUNG);

  const char *file_name = "trig.ppm";
  Errno err = zaitoon_save_to_ppm(canvas, WIDTH, HEIGHT, file_name);
  if (err) {
	 printf("failed to save file %s : %s\n", file_name, strerror(errno));
	 return 1;
  }

	return 0;
}

