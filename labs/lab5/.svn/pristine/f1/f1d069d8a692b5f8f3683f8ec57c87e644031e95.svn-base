#include <lcom/lcf.h>
#include "math.h"
#include "draw.h"

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  if(x >= vbe_info.XResolution || y >= vbe_info.YResolution)
    return 1;
  uint16_t bytes = ((vbe_info.BitsPerPixel + 1) / 8);
  uint8_t * pixel = video_mem;
  pixel += (x + vbe_info.XResolution * y) * bytes;
  for (char i = 0; i < bytes; i++)
    * (pixel + i) = color >> (i * 8);
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t width, uint32_t color) {
  for (int i = 0 ; i < width; i++)
    vg_draw_pixel(x + i, y, color);
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0 ; i < height; i++)
    vg_draw_hline(x, y + i, width, color);
  return 0;
}

int (vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step){
  uint16_t height = vbe_info.YResolution / no_rectangles;
  uint16_t width = vbe_info.XResolution / no_rectangles;
  for(uint16_t row = 0; row < no_rectangles; row++){
    for(uint16_t col = 0; col < no_rectangles; col++){
      uint32_t color = get_rectangle_color(row, col, no_rectangles, first, step);
      vg_draw_rectangle(col * width, row * height, width, height, color);
    }
  }
  return 0;
}

uint32_t get_rectangle_color(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (vbe_info.MemoryModel == INDEX_MEMORY_MODEL) {
    return (first + (row * no_rectangles + col) * step) % (1 << vbe_info.BitsPerPixel) & (INDEX_COLOR_MASK);
  } else if (vbe_info.MemoryModel == DIRECT_MEMORY_MODEL) {
    uint8_t red, green, blue;
    get_color_components(&red, &green, &blue, first);
    red = (red + col * step) % (1 << vbe_info.RedMaskSize);
	  green = (green + row * step) % (1 << vbe_info.GreenMaskSize);
	  blue = (blue + (col + row) * step) % (1 << vbe_info.BlueMaskSize);
    uint32_t joined_colors = (red << vbe_info.RedFieldPosition) | (green << vbe_info.GreenFieldPosition) | (blue << vbe_info.BlueFieldPosition);
    return joined_colors;
  }
  return 0;
}

void get_color_components(uint8_t *red, uint8_t *green, uint8_t *blue, uint32_t color) {
  *red = get_color_bits(color, vbe_info.RedMaskSize, vbe_info.RedFieldPosition);
  *green = get_color_bits(color, vbe_info.GreenMaskSize, vbe_info.GreenFieldPosition);
  *blue = get_color_bits(color, vbe_info.BlueMaskSize, vbe_info.BlueFieldPosition);
  return;
}

uint8_t get_color_bits(uint32_t color, uint8_t size, uint8_t pos) {
  uint8_t res;
  uint32_t mask;
  mask = BIT(size+1)-1;
  mask <<= pos;
  res = (color & mask) >> pos;
  return res;
}
