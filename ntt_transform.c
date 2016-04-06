/*
 * File taken from: lattisigns512-20130329/ntt_transform.c
 * Author: Gim Güneysu, Tobias Oder, Thomas Pöppelmann, Peter Schwabe
 * Public Domain
 */


#include "ntt.h"
#include <x86intrin.h>
#include <stdio.h>
static void bitrev(poly r)
{
  double t;
#include "bitrev.code"
}


static const double _neg2[4] = {1.,-1.,1.,-1.};
static const double _neg4[4] = {1.,1.,-1.,-1.};

void ntt_transform(poly out, const poly o)
{ 
  int s, pos = 0, offset;
  __m256d vt,vo0,vo10,vo11,vo20,vo21,vo22,vo23,vc,vp,vpinv,neg2,neg4;
  __m256d vx0,vx1,vx2,vx3,vx4,vx5,vx6,vx7;
  
  vpinv = _mm256_set_pd(PARAM_QINV, PARAM_QINV, PARAM_QINV, PARAM_QINV);
  vp    = _mm256_set_pd(PARAM_Q, PARAM_Q, PARAM_Q, PARAM_Q);

  bitrev(out);
  vo10 = _mm256_load_pd(o+pos);
  vo20 = _mm256_load_pd(o+pos+4);
  neg2 = _mm256_load_pd(_neg2);
  neg4 = _mm256_load_pd(_neg4);
                                  
  // m = 2, m = 4, m = 8 (3 levels merged)
  for(s = 0; s<PARAM_N; s+=8)
  {
    // No multiplication with omega required, respective value is 1
    vx0 = _mm256_load_pd(out+s);
      vc = _mm256_mul_pd(vx0, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx0 = _mm256_sub_pd(vx0,vc);
   vt = _mm256_mul_pd(vx0,neg2);
    vx0 = _mm256_hadd_pd(vx0,vt);
    vc = _mm256_mul_pd(vx0, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx0 = _mm256_sub_pd(vx0,vc);
 
    vx1 = _mm256_load_pd(out+s+4);
     vc = _mm256_mul_pd(vx1, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx1 = _mm256_sub_pd(vx1,vc);
    vt = _mm256_mul_pd(vx1,neg2);
    vx1 = _mm256_hadd_pd(vx1,vt);
    vc = _mm256_mul_pd(vx1, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx1 = _mm256_sub_pd(vx1,vc);
 
    vx0 = _mm256_mul_pd(vx0, vo10);
    vc = _mm256_mul_pd(vx0, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx0 = _mm256_sub_pd(vx0,vc);
    vt = _mm256_permute2f128_pd (vx0, vx0, 0x01); // now contains x2,x3,x0,x1
    vx0 = _mm256_mul_pd(vx0, neg4);
    vx0 = _mm256_add_pd(vx0, vt);
    vc = _mm256_mul_pd(vx0, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx0 = _mm256_sub_pd(vx0,vc);
 
    vx1 = _mm256_mul_pd(vx1, vo10);
    vc = _mm256_mul_pd(vx1, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx1 = _mm256_sub_pd(vx1,vc);
    vt = _mm256_permute2f128_pd (vx1, vx1, 0x01); // now contains x2,x3,x0,x1
    vx1 = _mm256_mul_pd(vx1, neg4);
    vx1 = _mm256_add_pd(vx1, vt);
    vc = _mm256_mul_pd(vx1, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx1 = _mm256_sub_pd(vx1,vc);
 
    vt = _mm256_mul_pd(vx1, vo20);
    vc = _mm256_mul_pd(vt, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vt = _mm256_sub_pd(vt,vc);
    vx1 = _mm256_sub_pd(vx0, vt);
     vc = _mm256_mul_pd(vx1, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx1 = _mm256_sub_pd(vx1,vc);
    _mm256_store_pd(out+s+4, vx1);

    vx0 = _mm256_add_pd(vx0, vt);
    vc = _mm256_mul_pd(vx0, vpinv);
    vc = _mm256_round_pd(vc,0x08);
    vc = _mm256_mul_pd(vc, vp);
    vx0 = _mm256_sub_pd(vx0,vc);
   _mm256_store_pd(out+s+0, vx0);
  }
  
  pos += 8;

// m = 16, m = 32, m = 64 (3 levels merged)
  for(offset = 0; offset < 8; offset+=4)
  {
     vo0 = _mm256_load_pd(o+pos+offset);
    vo10 = _mm256_load_pd(o+pos+offset+8);
    vo11 = _mm256_load_pd(o+pos+offset+16);

    for(s = 0; s<PARAM_N; s+=64)
    {
     vx1 = _mm256_load_pd(out+offset+s+8);
      vc = _mm256_mul_pd(vx1, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx1 = _mm256_sub_pd(vx1,vc);
      vt = _mm256_mul_pd(vx1, vo0);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx0 = _mm256_load_pd(out+offset+s+0);
     vx1 = _mm256_sub_pd(vx0, vt);
      vc = _mm256_mul_pd(vx1, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx1 = _mm256_sub_pd(vx1,vc);
     vx0 = _mm256_add_pd(vx0, vt);
      vc = _mm256_mul_pd(vx0, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx0 = _mm256_sub_pd(vx0,vc);
      
     vx3 = _mm256_load_pd(out+offset+s+24);
	  vc = _mm256_mul_pd(vx3, vpinv);
	  vc = _mm256_round_pd(vc,0x08);
	  vc = _mm256_mul_pd(vc, vp);
     vx3 = _mm256_sub_pd(vx3,vc);
	  vt = _mm256_mul_pd(vx3, vo0);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx2 = _mm256_load_pd(out+offset+s+16);
     vx3 = _mm256_sub_pd(vx2, vt);
      vc = _mm256_mul_pd(vx3, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx3 = _mm256_sub_pd(vx3,vc);
     vx2 = _mm256_add_pd(vx2, vt);
      vc = _mm256_mul_pd(vx2, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx2 = _mm256_sub_pd(vx2,vc);
      
     vx5 = _mm256_load_pd(out+offset+s+40);
      vc = _mm256_mul_pd(vx5, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx5 = _mm256_sub_pd(vx5,vc);
      vt = _mm256_mul_pd(vx5, vo0);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx4 = _mm256_load_pd(out+offset+s+32);
     vx5 = _mm256_sub_pd(vx4, vt);
      vc = _mm256_mul_pd(vx5, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx5 = _mm256_sub_pd(vx5,vc);
     vx4 = _mm256_add_pd(vx4, vt);
      vc = _mm256_mul_pd(vx4, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx4 = _mm256_sub_pd(vx4,vc);

     vx7 = _mm256_load_pd(out+offset+s+56);
      vc = _mm256_mul_pd(vx7, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx7 = _mm256_sub_pd(vx7,vc);
      vt = _mm256_mul_pd(vx7, vo0);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx6 = _mm256_load_pd(out+offset+s+48);
     vx7 = _mm256_sub_pd(vx6, vt);
      vc = _mm256_mul_pd(vx7, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx7 = _mm256_sub_pd(vx7,vc);
     vx6 = _mm256_add_pd(vx6, vt);
      vc = _mm256_mul_pd(vx6, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx6 = _mm256_sub_pd(vx6,vc);

     vt = _mm256_mul_pd(vx2, vo10);
     vc = _mm256_mul_pd(vt, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
     vt = _mm256_sub_pd(vt,vc);
    vx2 = _mm256_sub_pd(vx0, vt);
     vc = _mm256_mul_pd(vx2, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx2 = _mm256_sub_pd(vx2,vc);
    vx0 = _mm256_add_pd(vx0, vt);
     vc = _mm256_mul_pd(vx0, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx0 = _mm256_sub_pd(vx0,vc);
     
     vt = _mm256_mul_pd(vx6, vo10);
     vc = _mm256_mul_pd(vt, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
     vt = _mm256_sub_pd(vt,vc);
    vx6 = _mm256_sub_pd(vx4, vt);
     vc = _mm256_mul_pd(vx6, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx6 = _mm256_sub_pd(vx6,vc);
    vx4 = _mm256_add_pd(vx4, vt);
     vc = _mm256_mul_pd(vx4, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx4 = _mm256_sub_pd(vx4,vc);
   
     vt = _mm256_mul_pd(vx3, vo11);
     vc = _mm256_mul_pd(vt, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
     vt = _mm256_sub_pd(vt,vc);
    vx3 = _mm256_sub_pd(vx1, vt);
     vc = _mm256_mul_pd(vx3, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx3 = _mm256_sub_pd(vx3,vc);
    vx1 = _mm256_add_pd(vx1, vt);
     vc = _mm256_mul_pd(vx1, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx1 = _mm256_sub_pd(vx1,vc);

     vt = _mm256_mul_pd(vx7, vo11);
     vc = _mm256_mul_pd(vt, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
     vt = _mm256_sub_pd(vt,vc);
    vx7 = _mm256_sub_pd(vx5, vt);
     vc = _mm256_mul_pd(vx7, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx7 = _mm256_sub_pd(vx7,vc);
    vx5 = _mm256_add_pd(vx5, vt);
     vc = _mm256_mul_pd(vx5, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx5 = _mm256_sub_pd(vx5,vc);
   
   vo20 = _mm256_load_pd(o+pos+offset+24);
     vt = _mm256_mul_pd(vx4, vo20);
     vc = _mm256_mul_pd(vt, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
     vt = _mm256_sub_pd(vt,vc);
    vx4 = _mm256_sub_pd(vx0, vt);
     vc = _mm256_mul_pd(vx4, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx4 = _mm256_sub_pd(vx4,vc);
    _mm256_store_pd(out+offset+s+32, vx4);
    vx0 = _mm256_add_pd(vx0, vt);
     vc = _mm256_mul_pd(vx0, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx0 = _mm256_sub_pd(vx0,vc);
    _mm256_store_pd(out+offset+s+0, vx0);

   vo21 = _mm256_load_pd(o+pos+offset+32);
     vt = _mm256_mul_pd(vx5, vo21);
     vc = _mm256_mul_pd(vt, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
     vt = _mm256_sub_pd(vt,vc);
    vx5 = _mm256_sub_pd(vx1, vt);
     vc = _mm256_mul_pd(vx5, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx5 = _mm256_sub_pd(vx5,vc);
    _mm256_store_pd(out+offset+s+40, vx5);
    vx1 = _mm256_add_pd(vx1, vt);
     vc = _mm256_mul_pd(vx1, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx1 = _mm256_sub_pd(vx1,vc);
    _mm256_store_pd(out+offset+s+8, vx1);

   vo22 = _mm256_load_pd(o+pos+offset+40);
     vt = _mm256_mul_pd(vx6, vo22);
     vc = _mm256_mul_pd(vt, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
     vt = _mm256_sub_pd(vt,vc);
    vx6 = _mm256_sub_pd(vx2, vt);
     vc = _mm256_mul_pd(vx6, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx6 = _mm256_sub_pd(vx6,vc);
    _mm256_store_pd(out+offset+s+48, vx6);
    vx2 = _mm256_add_pd(vx2, vt);
     vc = _mm256_mul_pd(vx2, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx2 = _mm256_sub_pd(vx2,vc);
    _mm256_store_pd(out+offset+s+16, vx2);

   vo23 = _mm256_load_pd(o+pos+offset+48);
     vt = _mm256_mul_pd(vx7, vo23);
     vc = _mm256_mul_pd(vt, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
     vt = _mm256_sub_pd(vt,vc);
    vx7 = _mm256_sub_pd(vx3, vt);
     vc = _mm256_mul_pd(vx7, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx7 = _mm256_sub_pd(vx7,vc);
    _mm256_store_pd(out+offset+s+56, vx7);
    vx3 = _mm256_add_pd(vx3, vt);
     vc = _mm256_mul_pd(vx3, vpinv);
     vc = _mm256_round_pd(vc,0x08);
     vc = _mm256_mul_pd(vc, vp);
    vx3 = _mm256_sub_pd(vx3,vc);
    _mm256_store_pd(out+offset+s+24, vx3);
    }
  }


  pos += 56;

  // m = 128, m=256, m=512 (3 levels merged)
  for(offset=0;offset<64;offset+=4)
  {
     vo0 = _mm256_load_pd(o+pos+offset);
    vo10 = _mm256_load_pd(o+pos+offset+64);
    vo11 = _mm256_load_pd(o+pos+offset+128);

    for(s = 0; s<PARAM_N; s+=512)
    {
     vx1 = _mm256_load_pd(out+offset+s+64);
      vt = _mm256_mul_pd(vx1, vo0);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx0 = _mm256_load_pd(out+offset+s+0);
     vx1 = _mm256_sub_pd(vx0, vt);
      vc = _mm256_mul_pd(vx1, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx1 = _mm256_sub_pd(vx1,vc);
     vx0 = _mm256_add_pd(vx0, vt);
      vc = _mm256_mul_pd(vx0, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx0 = _mm256_sub_pd(vx0,vc);
    
     vx3 = _mm256_load_pd(out+offset+s+192);
      vt = _mm256_mul_pd(vx3, vo0);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx2 = _mm256_load_pd(out+offset+s+128);
     vx3 = _mm256_sub_pd(vx2, vt);
      vc = _mm256_mul_pd(vx3, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx3 = _mm256_sub_pd(vx3,vc);
     vx2 = _mm256_add_pd(vx2, vt);
      vc = _mm256_mul_pd(vx2, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx2 = _mm256_sub_pd(vx2,vc);
    
     vx5 = _mm256_load_pd(out+offset+s+320);
      vt = _mm256_mul_pd(vx5, vo0);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx4 = _mm256_load_pd(out+offset+s+256);
     vx5 = _mm256_sub_pd(vx4, vt);
      vc = _mm256_mul_pd(vx5, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx5 = _mm256_sub_pd(vx5,vc);
     vx4 = _mm256_add_pd(vx4, vt);
      vc = _mm256_mul_pd(vx4, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx4 = _mm256_sub_pd(vx4,vc);
    
     vx7 = _mm256_load_pd(out+offset+s+448);
      vt = _mm256_mul_pd(vx7, vo0);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx6 = _mm256_load_pd(out+offset+s+384);
     vx7 = _mm256_sub_pd(vx6, vt);
      vc = _mm256_mul_pd(vx7, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx7 = _mm256_sub_pd(vx7,vc);
     vx6 = _mm256_add_pd(vx6, vt);
      vc = _mm256_mul_pd(vx6, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx6 = _mm256_sub_pd(vx6,vc);

      vt = _mm256_mul_pd(vx2, vo10);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx2 = _mm256_sub_pd(vx0, vt);
      vc = _mm256_mul_pd(vx2, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx2 = _mm256_sub_pd(vx2,vc);
     vx0 = _mm256_add_pd(vx0, vt);
      vc = _mm256_mul_pd(vx0, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx0 = _mm256_sub_pd(vx0,vc);
     
      vt = _mm256_mul_pd(vx3, vo11);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx3 = _mm256_sub_pd(vx1, vt);
      vc = _mm256_mul_pd(vx3, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx3 = _mm256_sub_pd(vx3,vc);
     vx1 = _mm256_add_pd(vx1, vt);
      vc = _mm256_mul_pd(vx1, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx1 = _mm256_sub_pd(vx1,vc);
     
      vt = _mm256_mul_pd(vx6, vo10);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx6 = _mm256_sub_pd(vx4, vt);
      vc = _mm256_mul_pd(vx6, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx6 = _mm256_sub_pd(vx6,vc);
     vx4 = _mm256_add_pd(vx4, vt);
      vc = _mm256_mul_pd(vx4, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx4 = _mm256_sub_pd(vx4,vc);
     
      vt = _mm256_mul_pd(vx7, vo11);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx7 = _mm256_sub_pd(vx5, vt);
      vc = _mm256_mul_pd(vx7, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx7 = _mm256_sub_pd(vx7,vc);
     vx5 = _mm256_add_pd(vx5, vt);
      vc = _mm256_mul_pd(vx5, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx5 = _mm256_sub_pd(vx5,vc);
    
    vo20 = _mm256_load_pd(o+pos+offset+192);
      vt = _mm256_mul_pd(vx4, vo20);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx4 = _mm256_sub_pd(vx0, vt);
      vc = _mm256_mul_pd(vx4, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx4 = _mm256_sub_pd(vx4,vc);
     _mm256_store_pd(out+offset+s+256, vx4);
     vx0 = _mm256_add_pd(vx0, vt);
      vc = _mm256_mul_pd(vx0, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx0 = _mm256_sub_pd(vx0,vc);
     _mm256_store_pd(out+offset+s+0, vx0);

    vo21 = _mm256_load_pd(o+pos+offset+256);
      vt = _mm256_mul_pd(vx5, vo21);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx5 = _mm256_sub_pd(vx1, vt);
      vc = _mm256_mul_pd(vx5, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx5 = _mm256_sub_pd(vx5,vc);
     _mm256_store_pd(out+offset+s+320, vx5);
     vx1 = _mm256_add_pd(vx1, vt);
      vc = _mm256_mul_pd(vx1, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx1 = _mm256_sub_pd(vx1,vc);
     _mm256_store_pd(out+offset+s+64, vx1);

    vo22 = _mm256_load_pd(o+pos+offset+320);
      vt = _mm256_mul_pd(vx6, vo22);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx6 = _mm256_sub_pd(vx2, vt);
      vc = _mm256_mul_pd(vx6, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx6 = _mm256_sub_pd(vx6,vc);
     _mm256_store_pd(out+offset+s+384, vx6);
     vx2 = _mm256_add_pd(vx2, vt);
      vc = _mm256_mul_pd(vx2, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx2 = _mm256_sub_pd(vx2,vc);
     _mm256_store_pd(out+offset+s+128, vx2);

    vo23 = _mm256_load_pd(o+pos+offset+384);
      vt = _mm256_mul_pd(vx7, vo23);
      vc = _mm256_mul_pd(vt, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
      vt = _mm256_sub_pd(vt,vc);
     vx7 = _mm256_sub_pd(vx3, vt);
      vc = _mm256_mul_pd(vx7, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx7 = _mm256_sub_pd(vx7,vc);
     _mm256_store_pd(out+offset+s+448, vx7);

     vx3 = _mm256_add_pd(vx3, vt);
      vc = _mm256_mul_pd(vx3, vpinv);
      vc = _mm256_round_pd(vc,0x08);
      vc = _mm256_mul_pd(vc, vp);
     vx3 = _mm256_sub_pd(vx3,vc);
     _mm256_store_pd(out+offset+s+192, vx3);
    }
  }
}
