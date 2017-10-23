
#include "util.hpp"

using namespace std;

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

void find_set(char communication_buffer[], uint64_t comm_set_addresses[][8], int set, int index){
	int i,j,set1=0;

	for (i=0; i< CACHE_SIZE;i++){
			 if(((uint64_t)&communication_buffer[i]>>6) % 64 == set && set1<8){

				 for(j=0;j<SET_NUM*4096; j+=4096){
					 comm_set_addresses[index][set1] = (uint64_t)&communication_buffer[i+j];
					 // printf("Address = %x\n",comm_set_addresses[0][set1]);
					 // printf("Index = %d\n",((uint64_t)comm_set_addresses[0][set1]>>6) % 64);

					 // printf("address = %x\n",&communication_buffer[i+j]);
					 // printf("index = %d\n",((uint64_t)&communication_buffer[i+j]>>6) % 64);
					 set1++;
				 }
			 }

	}

}

void establish_covert_channel(char communication_buffer[], uint64_t comm_set_addresses[][8]){
         int i,iter,j, set1=0, set2=0, set3=0, set4=0;

					find_set(communication_buffer, comm_set_addresses, 5, 0);
					find_set(communication_buffer, comm_set_addresses, 17, 1);
					find_set(communication_buffer, comm_set_addresses, 23, 2);
					find_set(communication_buffer, comm_set_addresses, 27, 3);

					find_set(communication_buffer, comm_set_addresses, 11, 4);
					find_set(communication_buffer, comm_set_addresses, 13, 5);
					find_set(communication_buffer, comm_set_addresses, 19, 6);
					find_set(communication_buffer, comm_set_addresses, 31, 7);

}

void signal_readyTosend(uint64_t comm_set_addresses[][8]){
	int i;
	uint32_t time_passed;
	uint64_t hits=0, misses=0, counter = 0;

	while(1){

		for(i=0; i<8; i++){
			clflush(comm_set_addresses[0][i]);
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

void send_Bit(uint64_t comm_set_addresses[][8]){
	int i;
	uint32_t time_passed;
	uint64_t hits=0, misses=0, counter = 0;

	while(1){

		for(i=0; i<8; i++){
			clflush(comm_set_addresses[1][i]);
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

void signal_stopSending(uint64_t comm_set_addresses[][8]){
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


int wait_to_recv(uint64_t comm_set_addresses[][8]){
	uint32_t time_passed;
	int i;
	uint64_t hits=0, misses=0, misses2=0, counter = 0;
		while(1){
	      counter++;
				if(counter%REPETITION_NUM==0){
					//cout<<"misses = "<<misses<<endl;
					counter=0;
					misses=0;
					misses2=0;
				}

				if(misses > WAIT_THR){
					//cout<<"result = 1"<<endl;
					return 1;
				}
				if(misses2 > WAIT_THR){
					//cout<<"break misses = "<<misses<<endl;
					return 0;
				}
				//for(i=0;i<8;i++){
				  //Read this set until the sender notifies that it is gonna send a bit
					time_passed=measure_one_block_access_time(comm_set_addresses[0][2]);
					if(time_passed>HIT_TIME){
						//cout<<"time_passed = "<<time_passed<<endl;
						misses++;
					}

					time_passed=measure_one_block_access_time(comm_set_addresses[3][2]);
					if(time_passed>HIT_TIME){
						//cout<<"time_passed = "<<time_passed<<endl;
						misses2++;
					}

				//}
				//sleep(0.00001);

			}

}


//Here we read from one set the bit value until we identify that sender stopped sending
//the value in the other set
int recv_bit(uint64_t comm_set_addresses[][8]){
	uint32_t time_passed;
	int i, result=0;
	uint64_t hits=0, misses=0, counter = 0, misses2=0;

	while(1){
      counter++;
			if(counter%REPETITION_NUM==0){
				//cout<<"misses = "<<misses<<endl;
				counter=0;
				misses=0;
				misses2=0;
			}

			if(misses > WAIT_THR){
				//cout<<"result = 1"<<endl;
				result=1;
			}
			if(misses2 > WAIT_THR){
				//cout<<"break misses = "<<misses<<endl;
				break;
			}
			//for(i=0;i<8;i++){
			  //Read this set until the sender notifies that it is gonna send a bit
				time_passed=measure_one_block_access_time(comm_set_addresses[1][2]);
				if(time_passed>HIT_TIME){
					//cout<<"time_passed = "<<time_passed<<endl;
					misses++;
				}

				time_passed=measure_one_block_access_time(comm_set_addresses[2][2]);
				if(time_passed>HIT_TIME){
					//cout<<"time_passed = "<<time_passed<<endl;
					misses2++;
				}

			//}
			//sleep(0.00001);

		}

		return result;
}

void receive_msg(uint64_t comm_set_addresses[][8]){
  int i,bitcount=0, done = -1;
  bitset<8> msg;
  bool listening=true;
  while (listening) {
    //printf("Waiting for a new bit signal\n");
    done = wait_to_recv(comm_set_addresses);
		if (done == 0){
			printf("Done receiving message\n");
			return;
		}
		//printf("Signal that I am ready to recv\n");
    //printf("Going to recv bit\n");
    i = recv_bit(comm_set_addresses);
    //printf("I received %d\n",i);
    msg.set(7-bitcount,i);
    bitcount++;
    if(bitcount==8){
      bitcount=0;
      printf("letter is %c\n",msg.to_ulong());
    }
  }
}

void send_msg(char *text_buf, uint64_t comm_set_addresses[][8]){
  bitset<8> letter;
  for(int chars = 0; chars< strlen(text_buf)-1; chars++){
           printf("Letter = %c\n",text_buf[chars]);
           letter = bitset<8>(text_buf[chars]);
           for(int i=7; i>=0; i--){
             signal:;
             printf("Signal ready to send\n");
             signal_readyTosend(comm_set_addresses);

            if(letter[i]){
                printf("Sending 1\n");
                send_Bit(comm_set_addresses);
                printf("Signal stop sending\n");
                signal_stopSending(comm_set_addresses);
                printf("Signal stop sending DONE\n");
            }else{
              printf("Sending 0\n");
              signal_stopSending(comm_set_addresses);
            }
          }

  }
}

void send_break(uint64_t comm_set_addresses[][8]){
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
