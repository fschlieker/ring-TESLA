#include "ntt.h"

void ntt_transform(poly,const poly);

static void mul_coefficients(poly  x, poly  c);

static double fmodq(double x)
{
  double c = x * PARAM_QINV;
  c = round(c);
  c *= PARAM_Q;
  return x-c;
}

static void mul_coefficients(poly  x, poly  c)
{
    unsigned int i;

    for(i = 0; i < PARAM_N; i++) {
        x[i] = fmodq(x[i] * c[i]);
    }
}

void poly_mul_fixed(poly  result, const poly  x, const poly  a)
{
    unsigned int i;

    if (result != x)
        for(i = 0; i < PARAM_N; i++)
            SETCOEFF(result[i], x[i]);


    mul_coefficients(result, psis);

    ntt_transform(result, omegas);


    for(i = 0; i < PARAM_N; i++)
    {
        result[i]= fmodq(result[i]* a[i]);
    }

    ntt_transform(result, omegas_inv);
    
    mul_coefficients(result, psis_inv);

}


void poly_add(poly  result, const poly  x, const poly  y)
{
    unsigned int i;

    for(i = 0; i < PARAM_N; i++)
    {
        result[i] = fmodq(x[i] + y[i]);
    }
}
void poly_sub(poly  result, const poly  x, const poly  y)
{
    unsigned int i;

    for(i = 0; i < PARAM_N; i++)
    {
        result[i] = fmodq(x[i] - y[i]);
    }
}
