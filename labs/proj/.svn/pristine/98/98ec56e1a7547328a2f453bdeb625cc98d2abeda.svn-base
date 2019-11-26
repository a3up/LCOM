#pragma once
#include "i8042.h"

int kbd_command(uint8_t *command, uint8_t issue);
  // Writes a new command byte to the KBC.
  // Return 0 upon succes, 1 upon error anouncing command to the KBC, 2 upon error writing the command

int kbd_subscribe_int(uint8_t *bit_no);
  // Subscribes and enables keyboard interrupts.
  // bit_no -	address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
  // Return - 0 upon success, 1 upon error setting the interrupt policy

int kbd_unsubscribe_int();
  // Unsubscribes Timer 0 interrupts.
  // Return - 0 upon success, 1 upon error removing the interrupt policy

int kbd_read_scan(uint16_t *scancode);
  // Reads a scancode from KBC output buffer
  // Return - 0 upon success and non-zero otherwise

int kbd_read_status(uint8_t *status);
  // Reads the KBC status from the status register
  // Return - 0 upon success and non-zero otherwise

int kbd_check_status(uint8_t *status);
  // Reads the KBC status and checks for errors
  // Return - 0 upon success, 1 upon error reading the status, 2 upon parity error or timeout error in the KBC

int kbd_display_scancode(uint16_t *scancode);
  // Prints a scancode on screen. Handles 1 and 2 byte scancodes differently
  // Return - 0 upon success, 1 upon error printing the scancode

void (kbc_ih)();
  // Handles a keyboard interruption. Checks the KBC status for errors and if there are no errors,
  // reads a scancode to the global vairable last_scancode.
  // No return.

int kbd_check_out_buf(uint8_t *out_buf_st);
  // Checks if the KBC output buffer is full and sets the LSBit of out_buf_st to 1 if so, and 0 if not.
  // Return 0 upon success and 1 upon error checking the whole status byte.
