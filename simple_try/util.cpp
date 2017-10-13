
#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
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

void clflush(ADDR_PTR addr)
{
    asm volatile ("clflush (%0)" :: "r"(addr));
}


void establish_covert_channel(char communication_buffer[], uint64_t comm_set_addresses[][8]){
         int i,iter,j, set1=0, set2=0, set3=0, set4=0;

				 for (i=0; i< CACHE_SIZE;i++){
					 		if(((uint64_t)&communication_buffer[i]>>6) % 64 == 5 && set1<8){

								for(j=0;j<SET_NUM*4096; j+=4096){
									comm_set_addresses[0][set1] = (uint64_t)&communication_buffer[i+j];
									// printf("Address = %x\n",comm_set_addresses[0][set1]);
									// printf("Index = %d\n",((uint64_t)comm_set_addresses[0][set1]>>6) % 64);

									// printf("address = %x\n",&communication_buffer[i+j]);
									// printf("index = %d\n",((uint64_t)&communication_buffer[i+j]>>6) % 64);
									set1++;
								}
							}


							if(((uint64_t)&communication_buffer[i]>>6) % 64 == 17 && set2<8){

								for(j=0;j<SET_NUM*4096; j+=4096){
									comm_set_addresses[1][set2] = (uint64_t)&communication_buffer[i+j];
									// printf("address = %x\n",&communication_buffer[i+j]);
									// printf("index = %d\n",((uint64_t)&communication_buffer[i+j]>>6) % 64);
									set2++;
								}
							}

							if(((uint64_t)&communication_buffer[i]>>6) % 64 == 23 && set3<8){

								for(j=0;j<SET_NUM*4096; j+=4096){
									comm_set_addresses[2][set3] = (uint64_t)&communication_buffer[i+j];
									// printf("address = %x\n",&communication_buffer[i+j]);
									// printf("index = %d\n",((uint64_t)&communication_buffer[i+j]>>6) % 64);
									set3++;
								}
							}

							if(((uint64_t)&communication_buffer[i]>>6) % 64 == 27 && set4<8){

								for(j=0;j<SET_NUM*4096; j+=4096){
									comm_set_addresses[3][set4] = (uint64_t)&communication_buffer[i+j];
									// printf("address = %x\n",&communication_buffer[i+j]);
									// printf("index = %d\n",((uint64_t)&communication_buffer[i+j]>>6) % 64);
									set4++;
								}
							}

				 }
}
