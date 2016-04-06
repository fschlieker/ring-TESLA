#ifndef ORACLE_H
#define ORACLE_H

#include "params.h"
#include "ntt.h"

void random_oracle(unsigned char *c_bin,  poly v1, poly v2, const unsigned char *m, unsigned long long mlen);

#endif
