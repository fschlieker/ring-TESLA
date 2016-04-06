#include <stdlib.h>
#include <stdint.h>
#include "../../fastrandombytes.h"

void get_random_256(uint32_t * buf){
  //Write 256 bits/32 bytes of randomness into the buffer
  
  //use c rand() function as placeholder for testing on PC
  //int i;
  //2Dfor(i=0; i<8; i++){
  //buf[i] = (uint32_t) rand();
  //}

  fastrandombytes( (unsigned char*) buf, 8*4); 

}


#define BUF_VARS 128
static int32_t buf_cnt = -1;
static uint8_t buf[BUF_VARS];

void clear_buffer(){
  buf_cnt = -1;
}

uint8_t get_random_8(void){
  //Write 256 bits/32 bytes of randomness into the buffer
  //use c rand() function as placeholder for testing on PC

  //Maintain an internal buffer which contains random numbers. User can request bytes from this buffer. 
  //In case the internal buffer is deepleted, we have to resample uniform bits again.
  //TODO 
  //return (uint8_t) (rand() & 0xFF);
  //printf("%x",val);
  
  if (buf_cnt < 0){
    fastrandombytes(buf, BUF_VARS);   
    buf_cnt = BUF_VARS-1;
  }
  
 return buf[buf_cnt--];
}



/*
uint32_t getRandom(void)
{
  uint32_t r;
  
   // while(!(RNG->SR & RNG_FLAG_DRDY));
    //return RNG->DR;
  
  // XXX: TODO
  randombytes((unsigned char *)&r, 4);
  return r;
}





void getRandomPoly(int32_t * poly)
{
  int i;
    for(i = 0; i<paramN; i++)
    {
      do{
        poly[i] = getRandom()&0x3FFF;
      }while(poly[i] >= paramQ);
    }  
}
*/
