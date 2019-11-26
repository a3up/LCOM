#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdbool.h>
#include "macros/timer.h"

enum timer_init_mode {
    INVALID_ACCESS_TYPE, LSB, MSB, LSB_followed_by_MSB
};

enum timer_base {
    BINARY, DECIMAL
};

struct timer_info {
    bool output;
    bool null_count;
    enum timer_init_mode init_mode;
    uint8_t prog_mode;
    enum timer_base base;
};

int timer_get_info(uint8_t timer, struct timer_info *info) {
    if (timer > 2)
        return 1;
    if (write_command(RB_COMMAND | RB_COUNT | RB_TIMER(timer)))
        return 2;
    uint32_t status_byte;
    if (sys_inb(TIMER_0 + timer, &status_byte))
        return 3;
    info->output     = status_byte & ST_OUTPUT;
    info->null_count = status_byte & ST_NULL_COUNT;
    info->init_mode  = status_byte & ST_INIT_MODE;
    info->prog_mode  = status_byte & ST_PROG_MODE;
    info->base       = status_byte & ST_BCD;
}

enum timer_init (timer_st_get_init_mode)(uint8_t st) {
    st &= ST_INIT_MODE;
    switch (st) {
        case LSB_MODE:
            return LSB_only;
        case MSB_MODE:
            return MSB_only;
        case LSB_MSB_MODE:
            return MSB_after_LSB;
        default:
            return INVAL_val;
    }
}

int write_command(uint8_t command) {
    return sys_outb(TIMER_CTRL, command);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
    if (timer > 2)
        return 1;
    if (write_command(RB_COMMAND | RB_COUNT | RB_TIMER(timer)))
        return 2;
    unsigned aux;
    if (sys_inb(TIMER_0 + timer, &aux))
        return 3;
    *st = aux;
    return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
    union timer_status_field_val val;                                   // Declaring a variable to give the field argument to timer_print_config
    if (timer > 2)                                                      // Timer must be 0, 1 or 2
        return 1;                                                         // Invalid timer error
    switch (field) {                                                    // Decision based on which field to display
        case tsf_all:                                                     // Timer configuration/status mode
            val.byte = st;                                                  // Changes val to the entire status
            break;
        case tsf_initial:                                                 // Timer initialization mode
            val.in_mode = timer_st_get_init_mode(st);                       // Changes val to the initialization mode
            break;
        case tsf_mode:                                                    // Timer counting mode
            st &= BIT(3) | BIT(2) | BIT(1);                                     // Applying a mask to the status
            val.count_mode = st >> 1;                                       // Changes val to the counting mode
            if (val.count_mode > 5)                                         // Operating mode exceptions
                val.count_mode -= 4;                                          // Handling the exceptions
            break;
        case tsf_base:                                                    // Timer counting base mode
            st &= BIT(0);                                                   // Applying a mask to the status
            val.bcd = (st ==
                       ST_BCD);                                    // Changes val to true if BCD and false if Binary
    }
    if (timer_print_config(timer, field,
                           val))                           // Calls the timer_print_config function with the right parameter
        return 2;                                                         // timer_print_config call not successful
    return 0;                                                           // Successful operation
}
