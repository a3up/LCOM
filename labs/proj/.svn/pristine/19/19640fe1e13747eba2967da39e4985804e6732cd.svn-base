#pragma once

typedef enum {INIT, DRAWUP, VERTEX, DRAWDOWN, FINAL} state_t;
typedef enum {MOVE, RCLICK, MCLICK, LCLICK} ev_type_t;

uint8_t last_bytes[3];
bool pckt_ready = false;

void check_gesture(struct packet *pckt, uint8_t x_len, uint8_t tolerance);
bool valid_packet(uint8_t bytes[3]);
uint16_t join_bytes(uint8_t a, uint8_t b);
void add_last_bytes(uint8_t byte);
bool create_packet(struct packet *pckt, uint8_t bytes[3]);
void display_last_packet();
int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();
int read_byte();
void (mouse_ih)();
