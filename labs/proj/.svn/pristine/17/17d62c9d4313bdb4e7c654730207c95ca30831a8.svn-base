#pragma once

struct position {
    unsigned char col;
    unsigned char row;
};

enum color {
    BLACK, WHITE
};

enum type {
    KING = 'K', QUEEN = 'Q', ROOK = 'R', BISHOP = 'B', KNIGHT = 'N', PAWN = 'P'
};

struct piece {
    enum color color;
    enum type type;
};

enum status {
    PLAYING, CHECKMATE, STALEMATE, REPETITION, RESIGN, TO_BEGIN
};

int make_move(const char *notation);

void construct_game();

void destruct_game();

struct piece *get_piece(const struct position *position);

enum status get_status();

char *get_final_message();
