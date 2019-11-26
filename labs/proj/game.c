#include <lcom/lcf.h>
#include "game.h"
#include "art/pieces.h"
#include "graphics.h"
#include "draw.h"
#include "sprite.h"
#include "chess.h"

#define SQUARE_SIZE 74
#define SQUARE_COLOR_1 14
#define SQUARE_COLOR_2 55
#define PIECE_SIZE 64
#define BLACK_COLOR (unsigned char) 0
#define WHITE_COLOR (unsigned char) 63

static int draw_piece(xpm_map_t xpm, uint16_t x, uint16_t y, unsigned char *colors) {
    xpm_load(xpm, XPM_INDEXED, &image);
    uint8_t *byte = image.bytes;
    for (uint16_t row = 0; row < image.height; row++) {
        for (uint16_t col = 0; col < image.width; col++) {
            if (*byte != 0)
                vg_draw_pixel(x + col, y + row, colors[*byte - 1]);
            byte++;
        }
    }
    return 0;
}

int draw_board() {
    unsigned char xi, yi;
    xi = (vbe_info.XResolution - 8 * SQUARE_SIZE) >> 1;
    yi = (vbe_info.YResolution - 8 * SQUARE_SIZE) >> 1;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            vg_draw_rectangle(xi + SQUARE_SIZE * i, yi + SQUARE_SIZE * j, SQUARE_SIZE, SQUARE_SIZE,
                              (i + j) % 2 ? SQUARE_COLOR_1 : SQUARE_COLOR_2);
    unsigned char padding = (SQUARE_SIZE - PIECE_SIZE) >> 1;
    struct position position;
    for(unsigned char row = 0; row < 8; row++)
        for(unsigned char col = 0; col < 8; col++){
            position.col = col;
            position.row = row;
            struct piece *piece = get_piece(&position);
            if(piece == NULL)
                continue;
            unsigned char colors[2];
            colors[0] = (piece->color) ? WHITE_COLOR : BLACK_COLOR;
            colors[1] = (piece->color) ? BLACK_COLOR : WHITE_COLOR;
            switch (piece->type){
                case PAWN:
                    draw_piece(pawn, xi + padding + col * SQUARE_SIZE, yi + padding + row * SQUARE_SIZE, colors);
                    break;
                case KNIGHT:
                    draw_piece(knight, xi + padding + col * SQUARE_SIZE, yi + padding + row * SQUARE_SIZE, colors);
                    break;
                case BISHOP:
                    draw_piece(bishop, xi + padding + col * SQUARE_SIZE, yi + padding + row * SQUARE_SIZE, colors);
                    break;
                case ROOK:
                    draw_piece(rook, xi + padding + col * SQUARE_SIZE, yi + padding + row * SQUARE_SIZE, colors);
                    break;
                case QUEEN:
                    draw_piece(queen, xi + padding + col * SQUARE_SIZE, yi + padding + row * SQUARE_SIZE, colors);
                    break;
                case KING:
                    draw_piece(king, xi + padding + col * SQUARE_SIZE, yi + padding + row * SQUARE_SIZE, colors);
                    break;
            }
        }
    return 0;
}
