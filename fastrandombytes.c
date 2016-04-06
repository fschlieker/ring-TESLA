#include <stdio.h>
#include "crypto_stream.h"
#include "randombytes.h"
#include "gauss/rnd/random.h"

static int init = 0;
static unsigned char key[crypto_stream_KEYBYTES];
static unsigned char nonce[crypto_stream_NONCEBYTES] = {0};


void set_key(const unsigned char *data, unsigned long long datalen){ 
  //Set key to zero
  unsigned long long i;
  if (datalen > crypto_stream_KEYBYTES){
    printf("Seed too large");
  }

  clear_buffer();

  for(i=0; i<crypto_stream_KEYBYTES; i++){
    key[i]=0;
  }
  for(i=0; i<crypto_stream_NONCEBYTES; i++){
    nonce[i]=0;
  }  

  for(i=0; i<datalen; i++){
    key[i]=data[i];
  }

  init =1;
}

void set_zero_key(){
  int i;
  clear_buffer();

  for(i=0; i<crypto_stream_KEYBYTES; i++){
    key[i]=0;
  }  
  for(i=0; i<crypto_stream_NONCEBYTES; i++){
    nonce[i]=0;
  }  

  init = 1;
}


void get_random_key(){
  int i;
  
  clear_buffer();

  for(i=0; i<crypto_stream_NONCEBYTES; i++){
    nonce[i]=0;
  }  

  randombytes(key, crypto_stream_KEYBYTES);
  init = 1;
}

void fastrandombytes(unsigned char *r, unsigned long long rlen)
{
  unsigned long long n=0;
  int i;
  if(!init){
    randombytes(key, crypto_stream_KEYBYTES);
    init = 1;
  }
  crypto_stream(r,rlen,nonce,key);
  
  // Increase 64-bit counter (nonce)
  for(i=0;i<8;i++)
    n ^= ((unsigned long long)nonce[i]) << 8*i;
  n++;
  for(i=0;i<8;i++)
    nonce[i] = (n >> 8*i) & 0xff;
}
