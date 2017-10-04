#include "util.hpp"
#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;

// helper function that prepares the channel
// for communication with the sender.
// To do this we fill the whole L3 cache with our dataset.
void prepare_channel(char *channel, uint32_t *sets);

// array shuflle function from:https://stackoverflow.com/questions/6127503/shuffle-array-in-c
void shuffle (uint64_t *array, uint64_t n);

int main(int argc, char **argv) {
  // Put your covert channel setup code here

  char *channel;
  uint32_t sets[L3_SETS];

  // Allocate enough memory to cover the whole L3
  channel = (char *)malloc(L3_SIZE);

  if (channel == NULL) {
    cerr << "Failed to allocate memory of size " << L3_SIZE << " bytes\n";
    return -1;
  }

  prepare_channel(channel, sets);

  printf("Please press enter.\n");

  char text_buf[2];
  // fgets(text_buf, sizeof(text_buf), stdin);

  printf("Receiver now listening.\n");

  uint64_t int_array[10];
  uint64_t addr;

  bool listening = true;
  // while (listening) {
  //  for (int i = 0; i < 10; i++) {

  //    addr = (uint64_t)&int_array[i];

  //   uint32_t time_passed = measure_one_block_access_time(addr);
  // cout << "time passed = " << time_passed << endl;
  // }
  // Put your covert channel code here
  //}

  printf("Receiver finished.\n");

  return 0;
}

void prepare_channel(char *channel, uint32_t *sets) {

  uint32_t time_passed=0, addrs=0;
  uint64_t addr=(uint64_t)&channel[0];
  uint64_t access[L3_SETS];

  // initiliaze the sets miss/hit history
  memset(sets, 0, L3_SETS * sizeof(uint32_t));
  srand(time(NULL));
  
  // generate all the address that will be probed
  for (uint64_t i=0; i < L3_SETS; i++){
  	access[i]=addr;
	addr += CACHE_LINE_SIZE;
  }
  
  // shuffle the array to avoid prefetching effects
  shuffle(access, L3_SETS);
  
  for (uint64_t reps = 0; reps < REPETITION_NUM; reps++) {
    // initialize the channel
    memset(channel, 1, L3_SIZE);

    // grab the beggining of the array
    addr = (uint64_t)&channel[0];
    // sleep to give time to the sender to write
    sleep(SLEEP_TIME);
    
    for (uint64_t i = 0; i < L3_SETS; i++) {
      // measure the access time for each line
      // indirection based access to avoid prefetching effects
      time_passed = measure_one_block_access_time(access[i]);
      // and mark the misses
      sets[i] += (time_passed > HIT_TIME ? 1 : 0);
    }
  }

  for (uint64_t i = 0; i < L3_SETS; i++) {
    cout << sets[i] << "\t";
  }
  // cout << "\n";
}

void shuffle(uint64_t *array, uint64_t n)
{
    if (n > 1) 
    {
        uint64_t i;
        for (i = 0; i < n - 1; i++) 
        {
          uint64_t j = i + rand() / (RAND_MAX / (n - i) + 1);
	  uint64_t t = array[j];
	  array[j] = array[i];
          array[i] = t;
        }
    }
}
