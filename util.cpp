
#include "util.hpp"

// L3 cache size in bytes
#define L3_SIZE 8000000
#define L3_WAYS 16
#define CACHE_LINE_SIZE 64
#define L3_SETS L3_SIZE / (L3_WAYS * CACHE_LINE_SIZE)

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr) {
  CYCLES cycles;

  asm volatile("mov %1, %%r8\n\t"
               "lfence\n\t"
               "rdtsc\n\t"
               "mov %%eax, %%edi\n\t"
               "mov (%%r8), %%r8\n\t"
               "lfence\n\t"
               "rdtsc\n\t"
               "sub %%edi, %%eax\n\t"
               : "=a"(cycles) /*output*/
               : "r"(addr)
               : "r8", "edi");

  return cycles;
}
