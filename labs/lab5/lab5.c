// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "interrupts.h"
#include "timer.h"
#include "kbd.h"
#include "graphics.h"
#include "draw.h"
#include "sprite.h"

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

int(video_test_init)(uint16_t mode, uint8_t delay) {
  init_graphics(mode);
  sleep(delay);
  vg_exit();
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  // Configuring Keyboard
  extern uint16_t last_scancode;
  uint8_t bitno = 0;
  if(kbd_subscribe_int(&bitno)) return 3;
  keyboard_irq_set = BIT(bitno);
  // Configuring Graphics
  init_graphics(mode);

  // Drawing
  vg_draw_rectangle(x, y, width, height, color);
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

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  // Configuring Keyboard
  extern uint16_t last_scancode;
  uint8_t bitno = 0;
  if(kbd_subscribe_int(&bitno)) return 3;
  keyboard_irq_set = BIT(bitno);
  // Configuring Graphics
  init_graphics(mode);
  // Drawing
  vg_draw_pattern(no_rectangles, first, step);
  // Waiting for the ESC breakcode
  do {
    if((r = driver_receive(ANY, &msg, &ipc_status)))   // Receiving the drive response
      continue;                                        // Restarts the do-while block
    if(notification_origin(msg, KEYBOARD)){            // If it was from keyboard
      kbc_ih();
    }
  }while(last_scancode != ESC_BREAKCODE);
  // Configuring Graphics
  vg_exit();
  // Configuring Keyboard
  kbd_unsubscribe_int();
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  // Configuring Keyboard
  extern uint16_t last_scancode;
  uint8_t bitno = 0;
  if(kbd_subscribe_int(&bitno)) return 3;
  keyboard_irq_set = BIT(bitno);
  // Configuring Graphics
  init_graphics(0x105);

  // Drawing
  vg_draw_sprite(xpm, x, y);
  // Waiting for the ESC breakcode
  do {
    if((r = driver_receive(ANY, &msg, &ipc_status)))   // Receiving the drive response
      continue;                                        // Restarts the do-while block
    if(notification_origin(msg, KEYBOARD)){            // If it was from keyboard
      kbc_ih();
    }
  }while(last_scancode != ESC_BREAKCODE);

  // Configuring Graphics
  vg_exit();
  // Configuring Keyboard
  kbd_unsubscribe_int();
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  //Configuring Timer
  extern uint16_t counter;
  uint8_t bitno = 0;
  if (timer_subscribe_int(&bitno)) return 1;
  timer_irq_set = BIT(bitno);
  if (timer_set_frequency(0, sys_hz()/fr_rate));
  // Configuring Keyboard
  extern uint16_t last_scancode;
  bitno = 0;
  if(kbd_subscribe_int(&bitno)) return 2;
  keyboard_irq_set = BIT(bitno);
  // Configuring Graphics
  extern xpm_image_t image;
  init_graphics(0x105);
  uint16_t x = xi;
  uint16_t y = yi;
  uint16_t speed_x = 0;
  uint16_t speed_y = 0;
  if (xi == xf)
    speed_y = speed / (abs(yf - yi)/(yf - yi));
  if (yi == yf)
    speed_x = speed / (abs(xf - xi)/(xf - xi));
  vg_draw_sprite(xpm, x, y);
  do {
    if((r = driver_receive(ANY, &msg, &ipc_status)))   // Receiving the drive response
      continue;                                        // Restarts the do-while block
    if(notification_origin(msg, KEYBOARD)){            // If it was from keyboard
      kbc_ih();
    }
    if(notification_origin(msg, TIMER)) {
      if(x == xf && y == yf){
        continue;
      }else{
        timer_int_handler();
        if(speed > 0){
          if (speed_y == 0) {
            vg_draw_rectangle(x - speed_x, y - speed_y, speed_x, image.height, 0);
          }
          if (speed_x == 0){
            vg_draw_rectangle(x - speed_x, y - speed_y, image.width, speed_y, 0);
          }
          vg_draw_sprite(xpm, x, y);
          x += MIN(speed_x, xf - x);
          y += MIN(speed_y, yf - y);
        }
        if(speed < 0 && counter == -speed * fr_rate){
          if (speed_y == 0) {
            vg_draw_rectangle(x - 1, y, 1, image.height, 0);
          }
          if (speed_x == 0){
            vg_draw_rectangle(x, y - 1, image.width, 1, 0);
          }
          vg_draw_sprite(xpm, x, y);
          x += MIN(speed_x, xf - x);
          y += MIN(speed_y, yf - y);
          counter = 0;
        }
      }
    }
  }while(last_scancode != ESC_BREAKCODE);

  // Configuring Graphics
  vg_exit();
  // Configuring Timer
  timer_unsubscribe_int();
  // Configuring Keyboard
  kbd_unsubscribe_int();

  return 0;
}

int(video_test_controller)() {
  if (vbe_get_controller_info()) return 1;
  return 0;
}
