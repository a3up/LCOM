#include "arrow.h"
#include "mouse.h"

int print_mouse() {
    void *video_mem_copy;
    unsigned bufferSize = vbe_info.YResolution * vbe_info.XResolution * vbe_info.BitsPerPixel / 8;
    memcpy(video_mem_copy, video_mem, bufferSize);

    struct packet mouse_packet;
    create_packet(&mouse_packet, last_bytes);
    x_arrow += vbe_info.delta_x;
    y_arrow += vbe_info.delta_y;

    vg_draw_sprite(arrow, x_arrow, y_arrow);
    memcpy(video_mem, video_mem_copy, bufferSize);
    return 0;
}


