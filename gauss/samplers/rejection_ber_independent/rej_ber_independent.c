#include <stdio.h>
#include "../../rnd/random.h"

//The table for the Bernoulli sampler
#include "rej_ber_independent_table.data"

//Used for testing

static int32_t sample_rejection_independent_time();
static  uint32_t reject_counter =0;

int32_t gauss_max_val(){
  return MAX_GAUSS_VAL;
}

int32_t gauss_sigma(){
  return SIGMA;
}


int32_t sample_gauss(){
  //returns a sample
  
  int32_t val;
  uint8_t rnd;
 
  while(1){
    //obtain a sample from positive half of Gaussian
    val = sample_rejection_independent_time();
    rnd = get_random_8();

    //Check for a zero value and reject half of them
    if (val == 0){
      if (((rnd>>1) &1) == 0){
	//restart sampling procedure
	continue;
      }else{
	return val;
      }
    }
    
    //Sample a sign
    if (((rnd &1)==1)){
      return -val;
    }
    else{
      return val;
    }    
  }
  
}



static int32_t sample_rejection_independent_time() {
  //Bernoulli rejection sampling which can be made independent time- 
  //Reads out the whole table in case of a sucessfull sampling
  //Rejections if stuff does not work
  uint32_t val =0,x;
  uint32_t j, accept_mask;
  int32_t i;
  uint8_t r,reject;
  uint16_t smaller;
  uint16_t larger;
  
  
  //Use break an continue to escape from this loop
  while(1){

    val=0;
    i=0;
    //sample a candidate
    while (val < MAX_GAUSS_VAL){
      val |= ((uint32_t)get_random_8())<<(8*i);
      i++;
    }
    //mask the candidate and reject when necessary
    val = val & ((1<<MAX_GAUSS_LOG)-1);
    if (val >= MAX_GAUSS_VAL){
      continue;
    }
    
    //Check the table if we can accept this value
    //In case of a rejection we just abort
    //Otherwise the whole table has to be checked
    accept_mask = 0;
    reject = 0;
    x = val * val; //Have to evaluate exp(x^2/f). Keep val for later use (in case of success).
    for(j=0; j<MAX_BER_BYTES; j++){
      //From zero to one would be naive - we want to weed out the large values quickly, thus
      //we operate from max downto 0. Thus we can reject very early
      for(i=MAX_BER_ENTRIES-1; i>=0; i--){       
	//Sample a uniform byte

	  r = get_random_8();
	  
	  //We check wether the random value is smaller than the one in the table
	  smaller = 0;
	  larger = 0;
	  if (r < ber_table[i][j]){
	    smaller = 1;
	  }
	  if (r > ber_table[i][j]){
	    larger = 1;
	  }

	  //If the random value r is (a) smaller and (b) we havent accepted it so far,
	  //then we mark the index as accepted
	  if ((smaller == 1) && (((accept_mask >> i)&1) == 0)){
	    accept_mask |= (1<<i);
	  }

	  //When the sampled value is larger, we check
	  //a) is the bit set in x. If not we do not care and do no need to reject
	  //b) has the index already been accepted? If yes the larger value does not matter
	  //   as the comparision has already been evaluated
	  if ((larger == 1) && (((x >> i)&1)==1) && ((accept_mask >> i)&1) == 0 ){
	    //Break from the loop and restart again
	    reject = 1;
	    reject_counter++;
	    break;
	  } 	
      }
      if (reject == 1){
	break;
      }
    }
    
    if (reject == 0){
      //loop is finished - no reject
      //break the loop and output the value
      break;
    }
  } 

  return val;
}




int32_t get_rejections(){
  return reject_counter;
}

void clear_rejections(){
  reject_counter = 0;
}
