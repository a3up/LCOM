#pragma once

#include "macros/mouse.h"

// void mouse_int_handler();

int mouse_subscribe_int(uint16_t *bit_no);

int mouse_unsubscribe_int();

uint32_t mouse_get_last_packet();

uint8_t mouse_get_packet_counter();
