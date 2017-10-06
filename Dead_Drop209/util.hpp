
// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions
// (e.g., rand()).  You may not use system().
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>

#include <inttypes.h>
#include <time.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t
#define CYCLES uint32_t
#define SLEEP_TIME 0.00001

// L3 cache specs
//#define CACHE_SIZE 8388608
#define CACHE_SIZE 32768
#define CACHE_WAYS 8
#define CACHE_LINE_SIZE 64
#define CACHE_SETS CACHE_SIZE / (CACHE_WAYS * CACHE_LINE_SIZE)
#define HIT_TIME 50
#define HIT_THRESHOLD 1.1
#define UPPER_MISS_TIME 200

#define SET_NUM 8
#define SET_JUMP 512
#define REPETITION_NUM 100000
#define MSG_SIZE 8
#define MSG_NUM 1
CYCLES measure_one_block_access_time(ADDR_PTR addr);

#endif
