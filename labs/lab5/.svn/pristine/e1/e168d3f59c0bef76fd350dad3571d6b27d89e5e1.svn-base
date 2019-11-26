#include <lcom/lcf.h>
#include "kbd.h"
#include "utils.h"

int hook_id_kbd = IRQ1;                                       // Keyboard hook id
uint8_t status;                                               // Status byte
uint16_t last_scancode;                                       // Last scancode read

int kbd_command(uint8_t *command, uint8_t issue){
  // Writes a new command byte to the KBC.
  // Return 0 upon succes, 1 upon return error in out buffer, 2 upon error reading the command and 3 upon error writing the command
  if (sys_outb(KBC_CMD_REG, issue)) return 3;
  switch (issue) {
    case (WRITE_COMMAND):                                     // 0x60 command
      if (sys_outb(KBC_ARG_REG, *command)) return 3;
      break;
    case (READ_COMMAND):                                      // 0x20 command
      if (util_sys_inb(OUT_BUF, command)) return 2;
      break;
    case (CHECK_KBC):                                         // 0xAA command
      if (util_sys_inb(OUT_BUF, command)) return 2;
      return *command != KBC_SELF_TEST_OK;                    // Returns 0 if the return value in the out buff is 0x55 and 1 if it is 0xFC
    case (CHECK_KBD_IF):                                      // 0xAB command
      if (util_sys_inb(OUT_BUF, command)) return 2;
      return *command;                                        // Return 0 if OK, otherwise 1
    default:                                                  // 0xAD, 0xAE commands or others
      break;
  }
  return 0;
}

int kbd_subscribe_int(uint8_t *bit_no) {
  // Subscribes and enables keyboard interrupts.
  // bit_no -	address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
  // Return - 0 upon success, 1 upon error setting the interrupt policy
  *bit_no = hook_id_kbd;                                                            // Initializing bit_no with the value that will be the third argument of sys_irqsetpolicy
  return sys_irqsetpolicy(IRQ1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd);        // Calling the function to subscribe a notification
}

int kbd_unsubscribe_int(){
  // Unsubscribes Timer 0 interrupts.
  // Return - 0 upon success, 1 upon error removing the interrupt policy
  return sys_irqrmpolicy(&hook_id_kbd);                                             // Calling the function to unsubscribe interrupts
}

int kbd_read_scan(uint16_t *scancode) {
  // Reads a scancode from KBC output buffer
  // Return - 0 upon success and non-zero otherwise
  return util_sys_in2b(OUT_BUF, scancode);
}

int kbd_read_status(uint8_t *status){
  // Reads the KBC status from the status register
  // Return - 0 upon success and non-zero otherwise
    return util_sys_inb(STAT_REG, status);
}

int kbd_check_status(uint8_t *status){
  // Reads the KBC status and checks for errors
  // Return - 0 upon success, 1 upon error reading the status, 2 upon parity error or timeout error in the KBC
  if(kbd_read_status(status))
    return 1;
  if(*status & (PARITY_ERROR | TIMEOUT_ERROR))
    return 2;
  return 0;
}

int kbd_display_scancode(uint16_t *scancode){
  // Prints a scancode on screen. Handles 1 and 2 byte scancodes differently
  // Return - 0 upon success, 1 upon error printing the scancode
  bool mode = true;
  uint8_t bytes[2];
  uint8_t size = 1;
  if(*scancode & BIT(7))
    mode = false;
  bytes[0] = *scancode;                                             //
  bytes[1] = *scancode >> 8;                                        // Checking if scancode is a 2-byte scancode.
  if(bytes[1] == LONG_SCANCODE)                                     //
    size = 2;
  return kbd_print_scancode(mode, size, bytes);
}

void (kbc_ih)() {
  // Handles a keyboard interruption. Checks the KBC status for errors and if there are no errors,
  // reads a scancode to the global vairable last_scancode.
  // No return.
  if(kbd_check_status(&status))
    return;
  if(status & OUT_BUF_STATUS)
    kbd_read_scan(&last_scancode);
}

int kbd_check_out_buf(uint8_t *out_buf_st) {
  // Checks if the KBC output buffer is full and sets the LSBit of out_buf_st to 1 if so, and 0 if not.
  // Return 0 upon success and 1 upon error checking the whole status byte.
  if (kbd_check_status(out_buf_st))
    return 1;
  *out_buf_st &= OUT_BUF_STATUS;
  return 0;
}
