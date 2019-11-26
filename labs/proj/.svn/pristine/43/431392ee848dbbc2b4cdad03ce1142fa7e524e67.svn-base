#include <lcom/lcf.h>
#include "sprite.h"

int vg_draw_sprite(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_load(xpm, XPM_INDEXED, &image);
    uint8_t *byte = image.bytes;
    for (uint16_t row = 0; row < image.height; row++) {
        for (uint16_t col = 0; col < image.width; col++) {
            if (*byte != 0)
                vg_draw_pixel(x + col, y + row, *byte);
            byte++;
        }
    }
    return 0;
}
