#pragma once
#include "graphics.h"

/* XPM */
static char *arrow[] = {
/* columns rows colors chars-per-pixel */
        "8 8 3",
        "  c 0",
        ". c 1",
        "X c 2",
/* pixels */
        "...     ",
        ".XX..   ",
        ".XXXX.  ",
        " .XXX.  ",
        " .XX.   ",
        "  .. .  ",
        "      . ",
        "       ."
};

unsigned x_arrow = vbe_info.XResolution / 2, y_arrow = vbe_info.YResolution / 2;

int print_mouse();