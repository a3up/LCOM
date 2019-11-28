#pragma once

int kbc_get_status();

int kbc_write_command(uint32_t command);

int kbc_read_return(uint16_t *data);
