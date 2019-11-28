#pragma once

enum dispositive {
    TIMER = 1, KEYBOARD = 2, MOUSE = 4, GRAPHICS = 8
};

int request_message();

bool received_message(enum dispositive dispositive);

int subscribe_ints(uint8_t dispositives);

int unsubscribe_ints(uint8_t dispositives);
