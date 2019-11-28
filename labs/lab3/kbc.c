#include <lcom/lcf.h>
#include "kbc.h"
#include "macros/kbc.h"
#include "macros/error.h"

static uint8_t status;

int kbc_get_status() {
    uint32_t aux;
    error(sys_inb(STATUS_REGISTER, &aux), "Error reading status from keyboard");
    status = (uint8_t) aux;
    error(status & PARITY_ERROR, "Parity error when reading status");
    error(status & TIMEOUT_ERROR, "Timeout error when reading status");
    return 0;
}

int kbc_write_command(uint32_t command) {
    for (char i = 0; i < MAX_TRIES; ++i) {
        if (kbc_get_status())
            continue;
        if (status & FULL_IN_BUFFER) {
            error(sys_outb(COMMAND_REGISTER, command), "Error writing command to register");
            return 0;
        }
        delay(DELAY_US);
    }
    error(true, "Max number of tries reached when trying to write command");
}

int kbc_read_return(uint16_t *data) {
    for (char i = 0; i < MAX_TRIES; ++i) {
        if (kbc_get_status())
            continue;
        if (status & FULL_OUT_BUFFER) {
            uint32_t aux;
            error(sys_inb(OUT_BUFFER, &aux), "Error reading output buffer");
            *data = (uint16_t) aux;
            return 0;
        }
        delay(DELAY_US);
    }
    error(true, "Max number of tries reached when trying to read output buffer");
}
