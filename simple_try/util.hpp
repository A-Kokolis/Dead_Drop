
// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions
// (e.g., rand()).  You may not use system().
#include <stdlib.h>

#include <inttypes.h>
#include <time.h>

#include <unistd.h>
#include <string.h>
#include <bitset>
#include <iostream>
#include <stdint.h>
# include <sys/time.h>


#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t
#define CYCLES uint32_t

CYCLES measure_one_block_access_time(ADDR_PTR addr);

void clflush(ADDR_PTR addr);

//Cache Specifications
#define CACHE_SIZE 32768
#define SET_NUM 8
#define REPETITION_NUM 100
#define HIT_TIME 50
#define WAIT_THR 12



//We take 3 sets that we are gonna use for our implementation
//Sets 5, 17, 23, 27
void establish_covert_channel(char communication_buffer[], uint64_t comm_set_addresses[][8]);

#endif
