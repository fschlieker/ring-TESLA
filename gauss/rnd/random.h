#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
void get_random_256(uint32_t *buf);
uint8_t get_random_8(void);
void clear_buffer();

#endif
