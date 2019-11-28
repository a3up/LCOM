#pragma once

#include "macros/timer.h"

int timer_get_status(uint8_t timer, uint8_t *status);

void timer_reset_counter();

unsigned timer_get_counter();
