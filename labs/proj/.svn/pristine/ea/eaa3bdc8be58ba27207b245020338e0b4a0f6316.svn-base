#include <lcom/lcf.h>
#include "i8042.h"
#include "utils.h"
#include "kbc.h"
#include "mouse.h"

int mouse_hook_id = IRQ12;                          // Mouse hook id
uint8_t read_bytes = 0;
state_t state = INIT;
int32_t total_x = 0;

//typedef enum {LEFT, RIGHT, MIDDLE} button_t;

bool check_slope(struct packet p) {
  return (abs(p.delta_y/p.delta_x) >= 1);
}

bool check_tolerance(struct packet p, uint8_t tolerance) {
  return (abs(p.delta_x) <= tolerance && abs(p.delta_y) <= tolerance);
}

bool check_len(uint8_t x_len) {
  return total_x >= x_len;
}

void check_gesture(struct packet *pckt, uint8_t x_len, uint8_t tolerance) {
  total_x += pckt->delta_x;
  switch (state) {
    case INIT:
      if(pckt->lb && !pckt->mb && !pckt->rb)
        state = DRAWUP;
      else
        total_x = 0;
    break;
    case DRAWUP:
      if(pckt->mb || pckt->rb){
        state = INIT;
        total_x = 0;
      }else if(!pckt->lb){
        state = VERTEX;
        if(!check_len(x_len)){
          state = INIT;
        }
        total_x = 0;
      }else if(!check_slope(*pckt)){
        state = INIT;
        total_x = 0;
      }
    break;
    case VERTEX:
      if (!pckt->lb && !pckt->mb && pckt->rb) {
        state = DRAWDOWN;
      }else if(!check_tolerance(*pckt, tolerance)){
        state = INIT;
        total_x = 0;
      }else{
        total_x = 0;
      }
    break;
    case DRAWDOWN:
      if(pckt->lb || pckt->mb){
        state = INIT;
        total_x = 0;
      }else if(!pckt->rb){
        if(!check_len(x_len)){
          state = INIT;
          total_x = 0;
        }else{
          state = FINAL;
        }
      }else if(!check_slope(*pckt)){
        state = INIT;
        total_x = 0;
      }
    break;
    case FINAL:
    break;
  }
}

bool valid_packet(uint8_t bytes[3]){
  return (bytes[0] | SYNC_1);
}

uint16_t join_bytes(uint8_t a, uint8_t b){
  uint16_t res = b;
  if (a) {
    res -= 256;
  }
  return res;
}

void add_last_bytes(uint8_t byte){
    last_bytes[0] = last_bytes[1];
    last_bytes[1] = last_bytes[2];
    last_bytes[2] = byte;
    read_bytes++;
}

bool create_packet(struct packet *pckt, uint8_t bytes[3]){
  if(!valid_packet(bytes))
    return false;
  for(unsigned i = 0; i < 3; ++i){
      pckt->bytes[i] = bytes[i];
  }
  pckt->rb = (bytes[0] & R_BUTTON);
  pckt->mb = (bytes[0] & M_BUTTON);
  pckt->lb = (bytes[0] & L_BUTTON);
  pckt->delta_x = join_bytes(bytes[0] & MSB_X_DELTA, bytes[1]);
  pckt->delta_y = join_bytes(bytes[0] & MSB_Y_DELTA, bytes[2]);
  pckt->x_ov = bytes[0] & X_OVFL;
  pckt->y_ov = bytes[0] & Y_OVFL;
  return true;
}

void display_last_packet(){
  struct packet last_packet;
  create_packet(&last_packet, last_bytes);
  mouse_print_packet(&last_packet);
}

int mouse_subscribe_int(uint8_t *bit_no) {
  *bit_no = mouse_hook_id;
  return sys_irqsetpolicy(IRQ12, IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook_id);
}

int mouse_unsubscribe_int(){
  return sys_irqrmpolicy(&mouse_hook_id);                                             // Calling the function to unsubscribe interrupts
}

int read_byte(uint8_t *byte){
  if (!kbc_check_out_buf()) return 2;
  if (util_sys_inb(OUT_BUF, byte)) return 1;
  return 0;
}

void (mouse_ih)() {
  uint8_t byte;
  if(read_byte(&byte)) return;
  add_last_bytes(byte);
  if(read_bytes == 3){
    if(valid_packet(last_bytes))
      pckt_ready = true;
    read_bytes = 0;
  }
}
