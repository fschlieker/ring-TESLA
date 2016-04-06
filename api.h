#include "params.h"
#include <stdint.h>

//Contains S and E
#define CRYPTO_SECRETKEYBYTES (sizeof(double)*PARAM_N+sizeof(double)*PARAM_N*2)

//Contains T
#define CRYPTO_PUBLICKEYBYTES (sizeof(double)*2*PARAM_N)

//Contains z (bounded by B) and a hash
#define CRYPTO_BYTES (3*PARAM_N+32)
