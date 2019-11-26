// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "interrupts.h"
#include "kbd.h"
#include "graphics.h"
#include "game.h"
#include "chess.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");
  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");
  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");
  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;
  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();
  return 0;
}

int (proj_main_loop)(int argc, char *argv[]){
    // Configuring Keyboard
    extern uint16_t last_scancode;
    uint8_t bitno = 0;
    if(kbd_subscribe_int(&bitno)) return 3;
    keyboard_irq_set = BIT(bitno);
    // Configuring Graphics
    init_graphics(0x105);

    // Drawing
    construct_game();
    draw_board();
    destruct_game();

    // Waiting for the ESC breakcode
    do {
        if((r = driver_receive(ANY, &msg, &ipc_status)))   // Receiving the drive response
            continue;                                        // Restarts the do-while block
        if(notification_origin(msg, KEYBOARD)){            // If it was from keyboard
            kbc_ih();
        }
    }while(last_scancode != ESC_BREAKCODE);
    // Configuring Keyboard
    kbd_unsubscribe_int();
    // Configuring Graphics
    vg_exit();
    return 0;
}
