#include "util.hpp"
#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {
  char *fill_memory;
  uint64_t *sets, time_passed;
  int i,j;

  // Allocate enough memory to cover the whole CACHE
  fill_memory = (char *)malloc(CACHE_SIZE*sizeof(char));
  sets = (uint64_t *)malloc(CACHE_SETS*sizeof(uint64_t));

  char text_buf[128];
  fgets(text_buf, sizeof(text_buf), stdin);
  for (uint64_t reps = 0; reps < REPETITION_NUM; reps++) {
  	//memset(fill_memory, 'a', sizeof(char)*CACHE_SIZE);
	for(i=0; i<SET_JUMP*8; i+= 64){
		for (j=0; j<SET_NUM*4096;j+=4096){
			//fill_memory[i+j] = 'a';
      			time_passed = measure_one_block_access_time((uint64_t)&fill_memory[i+j]);
		
		}
	}


	for(i=0; i<SET_JUMP*8; i+= 64){
		for (j=0; j<SET_NUM*4096;j+=4096){
      			time_passed = measure_one_block_access_time((uint64_t)&fill_memory[i+j]);
			//printf("address = %x\n",&fill_memory[i+j]);
			//printf("index = %d\n",((uint64_t)&fill_memory[i+j]>>6) % 64);	

			if(time_passed > HIT_TIME && time_passed < UPPER_MISS_TIME){
				printf("miss\n");
				//cout<<time_passed<<endl;
		      	}else{
				cout<<"hit"<<endl;;
		      }

            //sets[i] += (time_passed > HIT_TIME && time_passed< UPPER_MISS_TIME? 1 : 0);
		}
	}	
  }

  return 0;
}
