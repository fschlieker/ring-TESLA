#include <stdint.h>
#include <stdio.h>
#include "params.h"
#include "crypto_hash_sha512.h"
#include "ntt.h"

static void compress_v(int32_t t[PARAM_N], double v[PARAM_N])
{
  int i;
  for(i=0;i<PARAM_N;i++){
    double c = v[i] * PARAM_QINV;
    c = floor(c) * PARAM_Q;
    t[i] = ((int64_t)(v[i]-c))>>PARAM_D;
  }
}



void random_oracle(unsigned char *c_bin,  poly v1, poly v2, const unsigned char *m, unsigned long long mlen)
{
  int32_t t1[PARAM_N],t2[PARAM_N];
  unsigned long long i;
  unsigned char buf[2*PARAM_N+mlen];
  compress_v(t1, v1);
  compress_v(t2, v2);
  for(i=0; i<PARAM_N; i++)
    buf[i] = t1[i];
  for(i=0; i<PARAM_N; i++)
    buf[i+PARAM_N] = t2[i];
  for(i=0; i<mlen; i++)
    buf[i+2*PARAM_N] = m[i];

  crypto_hash_sha512(c_bin, buf, mlen+2*PARAM_N);
}


