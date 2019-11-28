#pragma once

#include "macros/keyboard.h"

uint16_t keyboard_get_last_scancode();

void keyboard_int_handler();

int keyboard_subscribe_int(uint8_t *bit_no);

int keyboard_unsubscribe_int();

void keyboard_print_last_scancode();
