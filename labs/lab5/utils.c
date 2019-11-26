#include <lcom/lcf.h>
#include <stdint.h>

unsigned sys_inb_counter = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  // Stores the lsb of the value on the given address
  // val    - input 2 byte integer
  // lsb    - address of memory location to be updated with val's LSB
  // Return - 0 upon success and non-zero otherwise
  *lsb = val;                                                         // Just stores the value on the address
  return 0;                                                           // Successful operation
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  // Stores the msb of the value on the given address
  // val    - input 2 byte integer
  // msb    - address of memory location to be updated with val's MSB
  // Return - 0 upon success and non-zero otherwise
  val >>= 8;                                                          // Changes value to represent just the msb
  *msb = val;                                                         // Stores the value on the address
  return 0;                                                           // Successful operation
}

int (util_sys_inb)(int port, uint8_t *value) {
  // Loads the value from the port to the given address
  // port   -	the input port that is to be read
  // value  -	address of 8-bit variable to be update with the value read
  // Return - 0 upon success and non-zero otherwise
  u32_t val;                                                          // Creates a new variable to load the value from the port
  int res = sys_inb(port, &val);                                      // Loads the value from the port to the variable
  sys_inb_counter++;                                                  // Increments sys_inb_counter
  *value = (uint8_t) val;                                             // Stores the value on the given address
  return res;                                                         // Success of the sys_inb function
}

int (util_sys_in2b)(int port, uint16_t *value) {
  // Loads the value from the port to the given address
  // port   -	the input port that is to be read
  // value  -	address of 8-bit variable to be update with the value read
  // Return - 0 upon success and non-zero otherwise
  u32_t val;                                                          // Creates a new variable to load the value from the port
  int res = sys_inb(port, &val);                                      // Loads the value from the port to the variable
  *value = (uint16_t) val;                                            // Stores the value on the given address
  return res;                                                         // Success of the sys_inb function
}
