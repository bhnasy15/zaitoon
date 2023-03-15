#include "test.h"

int main(void){

  uint32_t canvas[WIDTH * HEIGHT];

  zaitoon_fill(canvas, WIDTH, HEIGHT, 0xFFFFFFFF );

  zaitoon_fill_rect(canvas, WIDTH, HEIGHT, 0, 0,  WIDTH * 3 / 4, HEIGHT * 3 /4, 0x88880088);

  zaitoon_fill_rect(canvas, WIDTH, HEIGHT, WIDTH, HEIGHT, - WIDTH * 3 / 4, -HEIGHT * 3 /4, 0xFF000088);

  zaitoon_fill_ellipse(canvas, WIDTH, HEIGHT, WIDTH - 200, HEIGHT - 200, 100,
                      200, 0x00FF0088);

  zaitoon_fill_trig(canvas, WIDTH, HEIGHT, 10, 19, 500, 88, 600, 555,
                    0x0000FF88);

  zaitoon_draw_line(canvas, WIDTH, HEIGHT, 0, 0, WIDTH - 1, HEIGHT - 1,
                    0x0000FF88);

  zaitoon_draw_ellipse(canvas, WIDTH, HEIGHT, WIDTH / 2, HEIGHT / 2, 200, 200,
                      0x00FF0088);


  const char *file_name = "alpha.ppm";
  Errno err = zaitoon_save_to_ppm(canvas, WIDTH, HEIGHT, file_name);
  if (err){
    printf("failed to save file %s : %s\n", file_name, strerror(errno));
	return 1;
  }
	return 0;
}
