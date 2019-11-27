#pragma once

enum dispositive {
    TIMER, KEYBOARD, MOUSE, GRAPHICS
};

int request_message();

bool received_message(enum dispositive dispositive);

int subscribe_ints();

int unsubscribe_ints();
