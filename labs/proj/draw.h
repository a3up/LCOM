#pragma once
#include "graphics.h"

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t width, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);
uint32_t get_rectangle_color(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first, uint8_t step);
void (get_color_components)(uint8_t *red, uint8_t *green, uint8_t *blue, uint32_t color);
uint8_t get_color_bits(uint32_t color, uint8_t size, uint8_t pos);

