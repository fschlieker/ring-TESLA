#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdint.h>
#include "params.h"
#include "ntt.h"

void sample_y(poly y);

void sample_gauss_poly(poly x);

void generate_c(uint32_t *pos_list, unsigned char *c_bin);

#endif
