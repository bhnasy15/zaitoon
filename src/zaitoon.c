#include "zaitoon.h"

Errno zaitoon_save_to_ppm(uint32_t *canvas, size_t canvas_width,
                          size_t canvas_height, const char *file_name) {
    Errno result = 0;
    FILE *file_path = NULL;
    {
        file_path = fopen(file_name, "wb");
        if (file_path == NULL)
            _return_defer(-1);

        fprintf(file_path, "P6\n%zu %zu\n255\n", canvas_width,
                canvas_height);
        if (ferror(file_path))
            _return_defer(errno);

        uint8_t bytes[canvas_width * canvas_height * 3];
        for (size_t i = 0; i < canvas_width * canvas_height; i++) {
            // 0xRRGGBBAA
            bytes[i * 3 + 0] = (canvas[i] >> 8 * 3) & 0xFF; // RR
            bytes[i * 3 + 1] = (canvas[i] >> 8 * 2) & 0xFF; // GG
            bytes[i * 3 + 2] = (canvas[i] >> 8 * 1) & 0xFF; // BB
        }

        fwrite(bytes, sizeof(bytes), 1, file_path);
        if (ferror(file_path))
            _return_defer(errno);
    }

defer:
    if (file_path)
        fclose(file_path);
    return result;
}

uint32_t zaitoon_mix_colors(uint32_t foreground, uint32_t background) {
    uint8_t foreground_raw[4] = {0};
    uint8_t background_raw[4] = {0};
    uint32_t rtn = 0x00000000;

    for (size_t i = 0; i < 4; i++) {
        foreground_raw[i] = (foreground >> i * 8) & 0xFF;
        background_raw[i] = (background >> i * 8) & 0xFF;
    }

    float ratio = 1 - foreground_raw[0] / 255.0;

    for (size_t i = 1; i < 4; i++) {

        int adder = (background_raw[i] - foreground_raw[i]) * ratio;

        int rtn_raw = foreground_raw[i] + adder;

        // clamp
        rtn_raw = rtn_raw < 0 ? 0 : rtn_raw > 255 ? 255 : rtn_raw;

        rtn |= ((uint32_t)rtn_raw << 8 * i);
    }
    return rtn;
}

void zaitoon_fill(uint32_t *canvas, size_t canvas_width,
                  size_t canvas_height, uint32_t color) {
    for (size_t i = 0; i < canvas_width * canvas_height; i++) {
        canvas[i] = color;
    }
}

void zaitoon_draw_rect(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x, int y, int rect_width,
                       int rect_height, uint32_t color) {
    int x2 = x + rect_width;
    int y2 = y + rect_height;
    // top
    zaitoon_draw_line(canvas, canvas_width, canvas_height, x, y, x2, y,
                      color);
    // bottom
    zaitoon_draw_line(canvas, canvas_width, canvas_height, x, y2, x2, y2,
                      color);
    // right
    zaitoon_draw_line(canvas, canvas_width, canvas_height, x2, y, x2, y2,
                      color);
    // left
    zaitoon_draw_line(canvas, canvas_width, canvas_height, x, y, x, y2,
                      color);
}

void zaitoon_fill_rect(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x, int y, int rect_width,
                       int rect_height, uint32_t color) {
    int start_y = rect_height > 0 ? y : y + rect_height;
    int start_x = rect_width > 0 ? x : x + rect_width;
    for (size_t j = 0; j < (size_t)abs(rect_height); ++j) {
        int p_y = start_y + j;
        if (0 <= p_y && (size_t)p_y < canvas_height) {
            for (size_t i = 0; i < (size_t)abs(rect_width); i++) {
                int p_x = start_x + i;
                if (0 <= p_x && (size_t)p_x < canvas_width) {
                    canvas[p_y * canvas_width + p_x] = zaitoon_mix_colors(
                        color, canvas[p_y * canvas_width + p_x]);
                }
            }
        }
    }
}

void zaitoon_draw_line(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x1, int y1, int x2,
                       int y2, uint32_t color) {
    // clamp
    y1 = y1 < 0 ? 0 : (size_t)y1 > canvas_height ? canvas_height : y1;
    y2 = y2 < 0 ? 0 : (size_t)y2 > canvas_height ? canvas_height : y2;
    x1 = x1 < 0 ? 0 : (size_t)x1 > canvas_width ? canvas_width : x1;
    x2 = x2 < 0 ? 0 : (size_t)x2 > canvas_width ? canvas_width : x2;

    int y_min = y1 < y2 ? y1 : y2;
    int y_max = y1 > y2 ? y1 : y2;
    if (x1 == x2) {
        for (; y_min < y_max; ++y_min) {
			size_t index = y_min * canvas_width + x1;
            canvas[index] = zaitoon_mix_colors(
                color, canvas[index]);
        }
        return;
    }
    int x_min = x1 < x2 ? x1 : x2;
    int x_max = x1 > x2 ? x1 : x2;
    if (y1 == y2) {
        for (; x_min < x_max; ++x_min) {
			size_t index = y1 * canvas_width + x_min;
            canvas[index] = zaitoon_mix_colors(
                color, canvas[index]);
        }
        return;
    }

    int fake_width = x1 - x2;
    int fake_height = y1 - y2;
    size_t fake_size = abs(fake_width * fake_height);
    char *fake_canvas = calloc(fake_size, sizeof(char) );

	if(!fake_canvas){
		perror("fake_canvas memory error\n");
		exit(-1);
	}

	int fake_x1 = x1 -x_min;
	int fake_y1 = y1 -y_min;
    float slop = (float)fake_height / (float)fake_width;

	for(int i = 0; i < abs(fake_width); ++i){
		int y = slop * (float)(i - fake_x1) + fake_y1;
		fake_canvas[y * abs(fake_width) + i] = 1;
	}
	for(int j = 0; j < abs(fake_height); ++j){
		int x = (float)(j - fake_y1) / slop + fake_x1;
		fake_canvas[j * abs(fake_width) + x] = 1;
	}

    for (size_t i = 0; i < fake_size; i++) {
        if (!fake_canvas[i])
            continue;

        size_t y = i / abs(fake_width) + y_min;
        size_t x = i % abs(fake_width) + x_min;

        size_t index = y * canvas_width + x;
        canvas[index]= zaitoon_mix_colors(color, canvas[index]);
    }
	free(fake_canvas);
}

void zaitoon_draw_ellipse(uint32_t *canvas, size_t canvas_width,
                         size_t canvas_height, int x, int y,
                         size_t horz_radios, size_t vert_radios,
                         uint32_t color) {
    if (vert_radios == 0 || horz_radios == 0)
        return;

	
    const float A = horz_radios * horz_radios;
    const float B = vert_radios * vert_radios;
    const float CY = B / A;
    const float CX = A / B;
	size_t fake_size = canvas_width * canvas_height;
	uint32_t *fake_canvas = calloc(fake_size, sizeof(uint32_t));

    for (int i = -(int)horz_radios; i <= (int)horz_radios; ++i) {
        int p_x = x + i;
        if (0 <= p_x && (size_t)p_x < canvas_width) {
            const float I2 = i * i;
            int p_y1 = y + (sqrtf(B - (I2 * CY)));
            if (0 <= p_y1 && (size_t)p_y1 < canvas_height) {
                fake_canvas[p_y1 * canvas_width + p_x] = color;
            }

            int p_y2 = y - (sqrtf(B - (I2 * CY)));
            if (0 <= p_y2 && (size_t)p_y2 < canvas_height) {
                fake_canvas[p_y2 * canvas_width + p_x] = color;
            }
        }
    }

    for (int j = -(int)vert_radios; j <= (int)vert_radios; ++j) {
        int p_y = y + j;
        if (0 <= p_y && (size_t)p_y < canvas_height) {
            const float J2 = j * j;
            int p_x1 = x + (sqrtf(A - (J2 * CX)));
            if (0 <= p_x1 && (size_t)p_x1 < canvas_width) {
                fake_canvas[p_y * canvas_width + p_x1] = color;
            }

            int p_x2 = x - (sqrtf(A - (J2 * CX)));
            if (0 <= p_x2 && (size_t)p_x2 < canvas_width) {
                fake_canvas[p_y * canvas_width + p_x2] = color;
            }
        }
    }
    for (size_t i = 0; i < fake_size; i++) {
        if (!fake_canvas[i])
            continue;

        size_t y = i / canvas_width;
        size_t x = i % canvas_width;

        size_t index = y * canvas_width + x;
        canvas[index]= zaitoon_mix_colors(color, canvas[index]);
    }
	free(fake_canvas);
}

void zaitoon_fill_ellipse(uint32_t *canvas, size_t canvas_width,
                         size_t canvas_height, int x, int y,
                         size_t horz_radios, size_t vert_radios,
                         uint32_t color) {
	size_t fake_size = canvas_width * canvas_height;
	uint32_t *fake_canvas = calloc(fake_size, sizeof(uint32_t));

    if (vert_radios == 0 || horz_radios == 0)
        return;

    const float A = horz_radios * horz_radios;
    const float B = vert_radios * vert_radios;
    const float CY = B / A;
    const float CX = A / B;

    for (int i = -(int)horz_radios; i <= (int)horz_radios; ++i) {
        int p_x = x + i;
        if (0 <= p_x && (size_t)p_x < canvas_width) {
            const float I2 = i * i;
            int p_y1 = y + (sqrtf(B - (I2 * CY)));
            if (0 <= p_y1 && (size_t)p_y1 < canvas_height) {
                    fake_canvas[p_y1 * canvas_width + p_x] = color;
            }

            int p_y2 = y - (sqrtf(B - (I2 * CY)));
            if (0 <= p_y2 && (size_t)p_y2 < canvas_height) {
                    fake_canvas[p_y2 * canvas_width + p_x] = color;
            }
        }
    }

    for (int j = -(int)vert_radios; j <= (int)vert_radios; ++j) {
        int p_y = y + j;
        if (0 <= p_y && (size_t)p_y < canvas_height) {
            const float J2 = j * j;
            int p_x1 = x + (sqrtf(A - (J2 * CX)));
            if (0 <= p_x1 && (size_t)p_x1 < canvas_width) {
                    fake_canvas[p_y * canvas_width + p_x1] = color;
            }

            int p_x2 = x - (sqrtf(A - (J2 * CX)));
            if (0 <= p_x2 && (size_t)p_x2 < canvas_width) {
                    fake_canvas[p_y * canvas_width + p_x2] = color;
            }

            for (; p_x2 <= p_x1; ++p_x2) {
                if (0 <= p_x2 && (size_t)p_x2 < canvas_width) {
                        fake_canvas[p_y * canvas_width + p_x2] = color;
                }
            }
        }
    }

    for (size_t i = 0; i < fake_size; i++) {
        if (!fake_canvas[i])
            continue;

        size_t y = i / canvas_width;
        size_t x = i % canvas_width;

        size_t index = y * canvas_width + x;
        canvas[index]= zaitoon_mix_colors(color, canvas[index]);
    }
	free(fake_canvas);
}

void zaitoon_draw_trig(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x1, int y1, int x2,
                       int y2, int x3, int y3, uint32_t color) {
    zaitoon_draw_line(canvas, canvas_width, canvas_height, x1, y1, x2, y2,
                      color);
    zaitoon_draw_line(canvas, canvas_width, canvas_height, x1, y1, x3, y3,
                      color);
    zaitoon_draw_line(canvas, canvas_width, canvas_height, x3, y3, x2, y2,
                      color);
}

void zaitoon_fill_trig(uint32_t *canvas, size_t canvas_width,
                       size_t canvas_height, int x1, int y1, int x2,
                       int y2, int x3, int y3, uint32_t color) {
    // 2 times are enough
	size_t fake_size = canvas_width * canvas_height;
	uint32_t *fake_canvas = calloc(fake_size, sizeof(uint32_t));
    int dx = x2 - x3;
    int dy = y2 - y3;
    int l = sqrtf((dx * dx) + (dy * dy));
    for (int i = 0; i <= l; ++i) {
        int new_x = (dx * i / l) + x3;
        int new_y = (dy * i / l) + y3;
        zaitoon_draw_line(fake_canvas, canvas_width, canvas_height, x1, y1,
                          new_x, new_y, color);
    }
    dx = x1 - x3;
    dy = y1 - y3;
    l = sqrtf((dx * dx) + (dy * dy));
    for (int i = 0; i <= l; ++i) {
        int new_x = (dx * i / l) + x3;
        int new_y = (dy * i / l) + y3;
        zaitoon_draw_line(fake_canvas, canvas_width, canvas_height, x2, y2,
                          new_x, new_y, color);
    }

    for (size_t i = 0; i < fake_size; i++) {
        if (!fake_canvas[i])
            continue;

        size_t y = i / canvas_width;
        size_t x = i % canvas_width;

        size_t index = y * canvas_width + x;
        canvas[index]= zaitoon_mix_colors(color, canvas[index]);
    }
	free(fake_canvas);
}
