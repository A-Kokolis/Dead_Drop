#include "util.hpp"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <bitset>
 
using namespace std;

// helper function that prepares the channel
// for communication with the sender.
// To do this we fill the whole L3 cache with our dataset.
void prepare_channel(char *channel, uint32_t sets[][2], uint32_t sets_used[][3]);

// helper function that receives an 8bit message
// over the communication channel
char receive_msg(uint32_t sets_used[][3]);

char receive_msg2(uint32_t sets_used[][3]);

// array shuflle function from:https://stackoverflow.com/questions/6127503/shuffle-array-in-c
void shuffle (uint64_t *array, uint64_t n);
// communication buffer
char channel[CACHE_SIZE];

int main(int argc, char **argv) {
  // Put your covert channel setup code here
  uint32_t sets[CACHE_SETS][2]; 
  uint32_t sets_used[CACHE_SETS][3];
  char text_buf[2];
  
  prepare_channel(channel, sets, sets_used);
  
  fgets(text_buf, sizeof(text_buf), stdin);

  for (uint32_t i = 0; i < MSG_NUM; i++){
  	printf("%c\n",receive_msg2(sets_used));
  }
  
  return 0;
}

void prepare_channel(char *channel, uint32_t sets[][2], uint32_t sets_used[][3]) {

  uint32_t time_passed=0;
  uint16_t index = 0;
  uint64_t addr=(uint64_t)&channel[0];
  uint64_t access[CACHE_SIZE/CACHE_LINE_SIZE];
  char text_buf[2];
  
  // initiliaze the sets miss/hit history
  //memset(sets, 0, CACHE_SETS * sizeof(uint32_t));
  for(uint32_t i=0; i < CACHE_SETS;i++){
	for(uint32_t j=0; j< 2; j++){
		sets[i][j]=(uint32_t)0;
	}
  }

  srand(time(NULL));
  
  // generate all the address that will be probed
  for (uint64_t i=0; i < CACHE_SIZE/CACHE_LINE_SIZE; i++){
	access[i]=addr;
	addr += CACHE_LINE_SIZE;
  }

  // shuffle the array to avoid prefetching effects
  shuffle(access, CACHE_SIZE/CACHE_LINE_SIZE);
  fgets(text_buf, sizeof(text_buf), stdin);
  
  for (uint64_t reps = 0; reps < REPETITION_NUM; reps++) {
    // initialize the channel
    for (uint64_t i = 0; i < CACHE_SIZE/CACHE_LINE_SIZE; i++) { 
      time_passed = measure_one_block_access_time(access[i]);
    }

    for (uint64_t i = 0; i < CACHE_SIZE/CACHE_LINE_SIZE; i++) {
      // measure the access time for each line
      // indirection based access to avoid prefetching effects
      time_passed = measure_one_block_access_time(access[i]);
      // and mark the misses
      index = (access[i] >> 6) % CACHE_LINE_SIZE;
      sets[index][0] += ( (time_passed > HIT_TIME && time_passed< UPPER_MISS_TIME) ? 1 : 0);
      sets[index][1] = access[i];
    }
  }
  // cleanup from here
  for(uint64_t i=0; i< CACHE_SETS; i++){
	sets_used[i][0]=sets[i][0];
	sets_used[i][1] = i;
	sets_used[i][2] = sets[i][1];
  }

  for (uint64_t i = 0; i < CACHE_SETS; i++) {
    cout << sets[i][0] <<" "<<sets_used[i][0]<<" "<<sets_used[i][1]<< " "<<sets_used[i][2] <<"\n";
  }
  cout << "\n";

  cout<<"Sorted\n\n"<<endl;

  qsort(sets_used, CACHE_SETS, sizeof(*sets_used),
        [](const void *arg1, const void *arg2)->int
        {
            int const *lhs = static_cast<int const*>(arg1);
            int const *rhs = static_cast<int const*>(arg2);
            return (lhs[0] > rhs[0]) ? -1
                :  ((rhs[0] > lhs[0]) ? 1
                :  (lhs[1] > rhs[1] ? -1
                :  ((rhs[1] > lhs[1] ? 1 : 0))));
        });

   for (uint64_t i = 0; i < CACHE_SETS; i++) {
    cout <<sets_used[i][0]<<" "<<sets_used[i][1] <<"\t";
   }
  cout << "\n";

    qsort(sets_used, 24, sizeof(*sets_used),
        [](const void *arg1, const void *arg2)->int
        {
            int const *lhs = static_cast<int const*>(arg1);
            int const *rhs = static_cast<int const*>(arg2);
            return lhs[1]-rhs[1];
        });
	cout << "\n";
    for (uint64_t i = 0; i < CACHE_SETS; i++) {
    	cout <<sets_used[i][0]<<" "<<sets_used[i][1]<< " " << sets_used[i][2] <<"\t";
    }
  cout << "\n";

}

char receive_msg2(uint32_t sets_used[][3]){
  int i,j, iter;
  uint32_t time_passed;

	for(i=0; i< 24; i++){
		sets_used[i][0] = 0;
    		cout <<sets_used[i][0]<<" "<<sets_used[i][1]<< " " << sets_used[i][2] <<"\t";
	}


	 for(iter=0; iter<REPETITION_NUM*100; iter++){
		for(i=0; i<24; i++){
		   for (j=0; j<SET_NUM*4096;j+=4096){
		        time_passed = measure_one_block_access_time( sets_used[i][2]+j);
			sets_used[i][0] += (time_passed> HIT_TIME && time_passed< UPPER_MISS_TIME? 1 : 0 );

		   }
		}
	}


	for(i=0; i< 24; i++){
    		cout <<sets_used[i][0]<<" "<<sets_used[i][1]<< " " << sets_used[i][2] <<"\t";
	}

}



char receive_msg(uint32_t sets_used[][3]){

  uint64_t max_miss=sets_used[MSG_SIZE*3][0];
  uint32_t vote=0, time_passed=0;
  bitset<8> msg;
    
  // initialize the access time of the sets we are gonna communicate over
  for(uint32_t i = 0; i < MSG_SIZE ; i++){
	for(uint32_t j = 0; j < 3*MSG_SIZE; j += MSG_SIZE){
		sets_used[i+j][0] = 0;
  	}
  }

  for (uint64_t reps = 0; reps < REPETITION_NUM*100; reps++) {
    // initialize the channel
    for(uint32_t i = 0; i < MSG_SIZE ; i++){
	for(uint32_t j = 0; j < 3*MSG_SIZE; j += MSG_SIZE){
		time_passed = measure_one_block_access_time( sets_used[i+j][2] );
  	}
    }
    
    // read "the bits" over the channel
    for(uint32_t i = 0; i < MSG_SIZE ; i++){
	for(uint32_t j = 0; j < 3*MSG_SIZE; j += MSG_SIZE){
		//printf("gmw = %lu\n",sets_used[i+j][0]);
		time_passed = measure_one_block_access_time( sets_used[i+j][2]);
		sets_used[i+j][0] += (time_passed> HIT_TIME && time_passed< UPPER_MISS_TIME? 1 : 0 );
  	}
    }
  }//END REPETITIONS

  // process the message using a 3-way voting technique
  for(uint32_t i = 0; i < MSG_SIZE ; i++){
	vote = 0;
	for(uint32_t j = 0; j < 3*MSG_SIZE; j += MSG_SIZE){
		vote += ( sets_used[i+j][0] > ((double)max_miss*HIT_THRESHOLD*2) ? 1 : 0 );
		printf("Set Hits: %u, Threshold: %f, Vote: %u\n", sets_used[i+j][0], ((double)max_miss*HIT_THRESHOLD), vote);
  	}
	msg.set(i, (vote>1 ? 1 : 0));
  }
   cout << "msg: " << msg << endl;
   return  static_cast<unsigned char> (msg.to_ulong());

}

void shuffle(uint64_t *array, uint64_t n){
	if (n > 1) {
        	uint64_t i;
       		for (i = 0; i < n - 1; i++) {
          		uint64_t j = i + rand() / (RAND_MAX / (n - i) + 1);
	  		uint64_t t = array[j];
	  		array[j] = array[i];
          		array[i] = t;
        	}
    }
}
