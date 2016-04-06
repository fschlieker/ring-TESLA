#include <stdio.h>
#include <string.h>

#include "../crypto_sign.h"
#include "../params.h"
#include "../api.h"
#include "../ntt.h"
#define MLEN 100

#define NTESTS 1000

unsigned char sk[CRYPTO_SECRETKEYBYTES];
unsigned char pk[CRYPTO_PUBLICKEYBYTES];
unsigned char sksav[CRYPTO_SECRETKEYBYTES];
unsigned char pksav[CRYPTO_PUBLICKEYBYTES];

unsigned char sk2[CRYPTO_SECRETKEYBYTES];
unsigned char pk2[CRYPTO_PUBLICKEYBYTES];

unsigned char mi[MLEN];
unsigned char mo[MLEN+CRYPTO_BYTES];
unsigned char sm[MLEN+CRYPTO_BYTES];
unsigned long long smlen;
unsigned long long mlen;

extern poly poly_a1;
extern poly poly_a2;
unsigned char Atsav[PARAM_N*PARAM_M];

extern unsigned long long rejwctr;
extern unsigned long long rejyzctr;
/*extern unsigned long long ctr;*/

int main()
{
  double rejw=.0,rejyz=.0;
	int r;
  unsigned long long i,j;
  FILE *urandom = fopen("/dev/urandom", "r");

    crypto_sign_keypair(pk, sk);
/*printf("%.2f,",1.0/((double)ctr));fflush(stdout);*/
  memcpy(sksav,sk,CRYPTO_SECRETKEYBYTES);
  memcpy(pksav,pk,CRYPTO_PUBLICKEYBYTES);
  
  /*memcpy(Atsav,matrix_At,PARAM_N*PARAM_M);*/

  for(i=0; i<NTESTS; i++)
  {
    for(j=0;j<MLEN;j++) 
      mi[j] = fgetc(urandom);
/*printf("qq");fflush(stdout);*/
    /* Test if valid signatures are verified as correct */
    crypto_sign(sm, &smlen, mi, MLEN, sk);    
 /*printf("gg");fflush(stdout);*/
 r = crypto_sign_open(mo, &mlen, sm, smlen, pk);
    printf("%d\n",r);
    if (rejyzctr != 0)
	        rejw+=rejwctr/rejyzctr;
        else
		            rejw+=rejwctr;
	    rejyz+=rejyzctr;
    /*printf("rej w:  %llu\n",rejwctr);*/
   /*printf("rej yz: %llu\n",rejyzctr);*/
    r = memcmp(mi,mo,MLEN);
    printf("%d\n",r);

    r = memcmp(sksav,sk,CRYPTO_SECRETKEYBYTES);
    printf("%d\n",r);
    r = memcmp(pksav,pk,CRYPTO_PUBLICKEYBYTES);
    printf("%d\n",r);
    /*r = memcmp(Atsav,matrix_At,PARAM_N*PARAM_M);*/
    /*printf("%d\n",r);*/

    /* Test with modified signature/message */
    crypto_sign(sm, &smlen, mi, MLEN, sk);    
    j = fgetc(urandom) % (MLEN+CRYPTO_BYTES);
    sm[j] ^= 1; //Change something in the signature somewhere
    r = crypto_sign_open(mo, &mlen, sm, smlen, pk);
	/*if (r==0)*/
	/*printf("%d-%lu \t %d\n",r+1,(j-100)%24,MLEN);*/
	/*else*/
	printf("%d\n",r+1);
    r = memcmp(sksav,sk,CRYPTO_SECRETKEYBYTES);
    printf("%d\n",r);
    r = memcmp(pksav,pk,CRYPTO_PUBLICKEYBYTES);
    printf("%d\n",r);
    /*r = memcmp(Atsav,matrix_At,PARAM_N*PARAM_M);*/
    /*printf("%d\n",r);*/
  }

  /*
  crypto_sign_keypair(pk, sk);
  crypto_sign_keypair(pk2, sk2);
  crypto_sign(sm, &smlen, mi, MLEN, sk);    
  r = crypto_sign_open(mo, &mlen, sm, smlen, pk2);
  printf("%d\n",r+1);
  */
printf("%.2f,",1/((rejw/NTESTS)+1));
  printf("%.2f",1/((rejyz/NTESTS)+1));
  fclose(urandom);
 
  return 0;
}


