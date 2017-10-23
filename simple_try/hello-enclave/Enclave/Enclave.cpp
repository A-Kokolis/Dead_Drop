#include "Enclave_t.h"
#include <stdio.h>
#include <stdlib.h>

#define CACHE_SIZE 32768
#define SET_NUM 8
#define REPETITION_NUM 100
#define HIT_TIME 50
#define WAIT_THR 12
#define ADDR_PTR uint64_t
#define CYCLES uint32_t

int generate_random_number() {
    //ocall_print("Processing random number generation...");
    return 42;
}

void establish_covert_channel(char *communication_buffer, uint64_t comm_set_addresses[4][8]){
         int i,iter,j, set1=0, set2=0, set3=0, set4=0;
         char buffer[1024];

				 for (i=0; i< CACHE_SIZE;i++){
					 		if(((uint64_t)&communication_buffer[i]>>6) % 64 == 5 && set1<8){

								for(j=0;j<SET_NUM*4096; j+=4096){
									comm_set_addresses[0][set1] = (uint64_t)&communication_buffer[i+j];

									// printf("Address = %x\n",comm_set_addresses[0][set1]);
									// printf("Index = %d\n",((uint64_t)comm_set_addresses[0][set1]>>6) % 64);
                  //
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

void clflush(uint64_t addr)
{
    asm volatile ("clflush (%0)" :: "r"(addr));
    //ocall_print("",addr);

}

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

void signal_readyTosend(uint64_t comm_set_addresses[4][8]){
	int i;
	uint32_t time_passed;
	uint64_t hits=0, misses=0, counter = 0;

	while(1){

		for(i=0; i<8; i++){
			clflush(comm_set_addresses[2][i]);
			//time_passed = measure_one_block_access_time(comm_set_addresses[0][i]);
		}
		//sleep(0.0001);
		counter++;
		if(counter%(REPETITION_NUM)==0){
		// 	//cout<<"misses = "<<misses<<endl;
		// 	//counter=0;
		// 	//misses=0;
      //ocall_print("Reached that");
		 	break;
		 }
	}
}

void signal_stopSending(uint64_t comm_set_addresses[4][8]){
	int i;
	uint32_t time_passed;
	uint64_t hits=0, misses=0, counter = 0;

	while(1){

		for(i=0; i<8; i++){
			clflush(comm_set_addresses[3][i]);
			//time_passed = measure_one_block_access_time(comm_set_addresses[0][i]);
		}
		//sleep(0.0001);
		counter++;
		if(counter%REPETITION_NUM==0){
		// 	//cout<<"misses = "<<misses<<endl;
		// 	//counter=0;
		// 	//misses=0;
		 	break;
		 }
	}
}

void send_Bit(uint64_t comm_set_addresses[4][8]){
	int i;
	uint32_t time_passed;
	uint64_t hits=0, misses=0, counter = 0;

	while(1){

		for(i=0; i<8; i++){
			clflush(comm_set_addresses[2][i]);
			//time_passed = measure_one_block_access_time(comm_set_addresses[0][i]);
		}
		//sleep(0.0001);
		counter++;
		if(counter%REPETITION_NUM==0){
		// 	//cout<<"misses = "<<misses<<endl;
		// 	//counter=0;
		// 	//misses=0;
		 	break;
		 }
	}
}
