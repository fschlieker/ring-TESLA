#ifndef NTT_H
#define NTT_H
#include "params.h"
#include <math.h>

typedef double __attribute__ ((aligned (32))) poly[PARAM_N];

/* Macros and external variable declarations */
#define SETCOEFF(a, b) do{ a = b; }while(0)
#define SWAP(a, b) do{ double tmp = a; a = b; b = tmp; }while(0)
#define ADDMOD(a, b, c) do{ a = (b + c) % PARAM_Q; }while(0)
#define MULMOD(a, b, c) do{ a = (b * c) % PARAM_Q; }while(0)

extern double omegas[];
extern double omegas_inv[];

extern double psis[];
extern double psis_inv[];

/* Interface function prototypes */
void poly_mul(poly  result, const poly  x, const poly  y);
void poly_transform(poly  result, const poly  x);
void poly_mul_fixed(poly  result, const poly x, const poly  a);
void poly_add(poly  result, const poly  x, const poly  y);
void poly_sub(poly result, const poly x, const poly y);

#endif /* NTT_H */
