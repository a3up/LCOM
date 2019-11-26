#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <lcom/utils.h>

#include <stdint.h>

#include "i8254.h"
int hook_id = TIMER0_IRQ;                                             // Initializing hook_id
uint16_t counter = 0;                                                 // Initializing the counter

enum timer_init (timer_st_get_init_mode)(uint8_t st){
  // Returns the initialization mode from a status
  // st     - status read via the read-back command
  st &= BIT(4)|BIT(5);                                                // Applying a mask to the status
  switch (st) {                                                       // Reading the initialization mode
    case TIMER_LSB:                                                   // LSB mode
      return LSB_only;                                                // Returns the right value
    case TIMER_MSB:                                                   // MSB mode
      return MSB_only;                                                // Returns the right value
    case TIMER_LSB_MSB:                                               // LSB followed by MSB mode
      return MSB_after_LSB;                                           // Returns the right value
    default:                                                          // Invalid mode
      return INVAL_val;                                               // Returns the right value
  }
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  // Reads the input timer configuration (status) via read-back command
  // timer  - Timer whose configuration to read (Ranges from 0 to 2)
  // st     - Address of memory position to be filled with the timer config
  // Return - 0 (success), 1 (Invalid timer), 2 (Read-back command not successful) or 3 (Loading not successful)
  if (timer > 2)                                                      // Timer must be 0, 1 or 2
    return 1;                                                         // Invalid timer error
  uint8_t command = TIMER_RB_CMD|TIMER_RB_COUNT_|TIMER_RB_SEL(timer); // Read-back command format
  if(sys_outb(TIMER_CTRL, command))                                   // Stores the read-back command on the control
    return 2;                                                         // Read-back command not successful error
  if(util_sys_inb(TIMER_0 + timer, st))                               // Loads the configuration from the timer to the given address
    return 3;                                                         // Configuration loading not successful error
  return 0;                                                           // Successful operation
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  // Displays, in a human friendly way, the specified field of a timer status, which was read via the read-back command
  // timer  -	timer whose configuration should be displayed (Ranges from 0 to 2)
  // st     -	status read via the read-back command
  // field  -	status field to display in human friendly way
  // Return - 0 (success), 1 (Invalid timer) or 2 (timer_print_config call not successful)
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
      st &= BIT(3)|BIT(2)|BIT(1);                                     // Applying a mask to the status
      val.count_mode = st >> 1;                                       // Changes val to the counting mode
      if (val.count_mode > 5)                                         // Operating mode exceptions
        val.count_mode -= 4;                                          // Handling the exceptions
      break;
    case tsf_base:                                                    // Timer counting base mode
      st &= BIT(0);                                                   // Applying a mask to the status
      val.bcd = (st == TIMER_BCD);                                    // Changes val to true if BCD and false if Binary
  }
  if(timer_print_config(timer, field, val))                           // Calls the timer_print_config function with the right parameter
    return 2;                                                         // timer_print_config call not successful
  return 0;                                                           // Successful operation
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  // Changes the operating frequency of a timer
  // timer  -	Timer to configure. (Ranges from 0 to 2)
  // freq   -	Timer operating frequency
  // Return - 0 (success), 1 (Invalid timer), 2 (Invalid frequency), 3 (Status loading not successful),
  //          4 (Read-back command not successful), 5 (Invalid ini_val), 6 (util_get not successful) or 7 (Storing ini_val not successful)
  if (timer > 2)                                                      // Timer must be 0, 1 or 2
    return 1;                                                         // Invalid timer error
  if (freq > TIMER_FREQ || freq == 0)                                 // The frequency is higher than the clock frequency or zero
    return 2;                                                         // Frequency higher than acceptable or zero
  uint8_t st;                                                         // Declaring a variable to load the timer's status
  if(timer_get_conf(timer, &st))                                      // Loading the timer's status
    return 3;                                                         // Status loading not successful
  uint8_t word =  (timer << 6 | TIMER_LSB_MSB | (st & 0x0f));         // Creating the control word
  if(sys_outb(TIMER_CTRL, word))                                      // Stores the read-back command on the control
    return 4;                                                         // Read-back command not successful error
  uint32_t ini_val = TIMER_FREQ/freq;                                 // Calculating the value to store on the timer
  uint8_t wrt_val;                                                    // Variable to load the bytes of the ini_val
  if (ini_val > 0xffff)                                               // Initial value higher than the representable in 2 bytes
    return 5;                                                         // Initial value overflow error
  if (util_get_LSB(ini_val, &wrt_val))                                // Getting the LSB of the initial value
    return 6;                                                         // util_get error
  if (sys_outb(TIMER_0 + timer, wrt_val))                             // Storing the initial value in LSB mode
    return 7;                                                         // Storing on port error
  if (util_get_MSB(ini_val, &wrt_val))                                // Getting the MSB of the initial value
    return 6;                                                         // util_get error
  if (sys_outb(TIMER_0 + timer, wrt_val))                             // Storing the initial value in MSB mode
    return 7;                                                         // Storing on port error
  return 0;                                                           // Successful operation
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  // Subscribes and enables Timer 0 interrupts.
  // bit_no -	address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
  // Return - 0 upon success and non-zero otherwise
  *bit_no = hook_id;                                                  // Initializing bit_no with the value that will be the third argument of sys_irqsetpolicy
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);        // Calling the function to subscribe a notification
}

int (timer_unsubscribe_int)() {
  // Unsubscribes Timer 0 interrupts.
  // Return - 0 upon success and non-zero otherwise
  return sys_irqrmpolicy(&hook_id);                                   // Calling the function to unsubscribe a notification
}

void (timer_int_handler)() {
  // Just increments the counter
  counter++;                                                          // Incrementing counter
}
