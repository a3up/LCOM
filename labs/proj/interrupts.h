#pragma once

enum dispositive {
   TIMER,
   KEYBOARD,
   MOUSE
 };

int r, ipc_status;
message msg;
uint32_t mouse_irq_set, timer_irq_set, keyboard_irq_set;

bool notification_origin(message msg, enum dispositive d);
