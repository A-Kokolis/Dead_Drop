#include "util.hpp"
#include <iostream>

using namespace std;

// helper function that prepares the channel
// for communication with the sender.
// To do this we fill the whole L3 cache with our dataset.
void prepare_channel(char *channel, uint32_t *sets);

int main(int argc, char **argv) {
  // Put your covert channel setup code here

  char *channel;
  uint32_t sets[L3_SETS];

  // Allocate enough memory to cover the whole L3
  channel = (char *)malloc(L3_SIZE);

  if (channel == NULL) {
    cerr << "Failed to allocate memory of size " << L3_SIZE << " bytes";
    return -1;
  }

  prepare_channel(channel, &sets[0]);

  printf("Please press enter.\n");

  char text_buf[2];
  fgets(text_buf, sizeof(text_buf), stdin);

  printf("Receiver now listening.\n");

  uint64_t int_array[10];
  uint64_t addr;

  bool listening = true;
  while (listening) {
    for (int i = 0; i < 10; i++) {

      addr = (uint64_t)&int_array[i];

      uint32_t time_passed = measure_one_block_access_time(addr);
      cout << "time passed = " << time_passed << endl;
    }
    // Put your covert channel code here
  }

  printf("Receiver finished.\n");

  return 0;
}

void prepare_channel(char *channel, uint32_t *sets) {

  uint32_t time_passed;
  // initialize the channel
  memset(channel, 1, L3_SIZE);
  uint64_t addr = (uint64_t)&channel[0];
  sleep(SLEEP_TIME);

  for (uint32_t i = 0; i < L3_SETS; i++) {
    time_passed = measure_one_block_access_time(addr);
    sets[i] += (time_passed > HIT_TIME ? 0 : 1);
    addr += CACHE_LINE_SIZE;
  }
}
