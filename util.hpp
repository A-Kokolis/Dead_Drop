
// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions
// (e.g., rand()).  You may not use system().
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <inttypes.h>
#include <time.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t
#define CYCLES uint32_t
#define SLEEP_TIME 0.00001

// L3 cache specs
#define L3_SIZE 8000000
#define L3_WAYS 16
#define CACHE_LINE_SIZE 64
#define L3_SETS L3_SIZE / (L3_WAYS * CACHE_LINE_SIZE)
#define HIT_TIME 50

#define SET_NUM 8
#define SET_JUMP 1024
#define REPETITION_NUM 100

CYCLES measure_one_block_access_time(ADDR_PTR addr);

#endif
