#pragma once

#include "macros/kbc.h"

int kbc_get_status();

int kbc_write_command(uint32_t command);

int kbc_read_return(uint16_t *data);

int kbc_write_mouse_byte(uint8_t byte);
