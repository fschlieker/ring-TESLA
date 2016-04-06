#include "ntt.h"
#include "immintrin.h"

__m256d vc,vt;
__m256d vx,vy;

__m256d vpinv = {PARAM_QINV, PARAM_QINV, PARAM_QINV, PARAM_QINV};
__m256d vp    = {PARAM_Q, PARAM_Q, PARAM_Q, PARAM_Q};

void ntt_transform(poly,const poly);

static void mul_coefficients(poly  x, poly  c);

static void mul_coefficients(poly  x, poly  c)
{
    unsigned int i;

    for(i = 0; i < PARAM_N; i+=4)
    {
        vx = _mm256_load_pd(x+i);
        vy = _mm256_load_pd(c+i);
        vt = _mm256_mul_pd(vx, vy);

        vc = _mm256_mul_pd(vt, vpinv);
        vc = _mm256_round_pd(vc,0x08);
        vt = _mm256_fnmadd_pd(vc,vp,vt);

        _mm256_store_pd(x+i, vt);
    }
}

void poly_mul_fixed(poly  result, const poly  x, const poly  a)
{
    unsigned int i;

    if (result != x)
        for(i = 0; i < PARAM_N; i++)
            result[i] = x[i];


    mul_coefficients(result, psis);

    ntt_transform(result, omegas);


    for(i = 0; i < PARAM_N; i+=4)
    {
        vx = _mm256_load_pd(result+i);
        vy = _mm256_load_pd(a+i);
        vt = _mm256_mul_pd(vx, vy);

        vc = _mm256_mul_pd(vt, vpinv);
        vc = _mm256_round_pd(vc,0x08);
        vt = _mm256_fnmadd_pd(vc,vp,vt);

        _mm256_store_pd(result+i, vt);
    }

    ntt_transform(result, omegas_inv);
    
    mul_coefficients(result, psis_inv);

}


void poly_add(poly  result, const poly  x, const poly  y)
{
    unsigned int i;

    for(i = 0; i < PARAM_N; i+=4)
    {
        vx = _mm256_load_pd(x+i);
        vy = _mm256_load_pd(y+i);
        vt = _mm256_add_pd(vx, vy);

        vc = _mm256_mul_pd(vt, vpinv);
        vc = _mm256_round_pd(vc,0x08);
        vt = _mm256_fnmadd_pd(vc,vp,vt);

        _mm256_store_pd(result+i, vt);
    }
}
void poly_sub(poly  result, const poly  x, const poly  y)
{
    unsigned int i;

    for(i = 0; i < PARAM_N; i+=4)
    {
        vx = _mm256_load_pd(x+i);
        vy = _mm256_load_pd(y+i);
        vt = _mm256_sub_pd(vx, vy);

        vc = _mm256_mul_pd(vt, vpinv);
        vc = _mm256_round_pd(vc,0x08);
        vt = _mm256_fnmadd_pd(vc,vp,vt);

        _mm256_store_pd(result+i, vt);
    }
}
