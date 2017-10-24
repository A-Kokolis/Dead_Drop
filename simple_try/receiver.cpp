
#include"util.hpp"

using namespace std;


char communication_buffer[CACHE_SIZE];
uint64_t comm_set_addresses[3][8];

bitset<8> ToBits(unsigned char byte)
{
    return bitset<8>(byte);
}


void wait_to_recv(uint64_t comm_set_addresses[][8]){
	uint32_t time_passed;
	int i;
	uint64_t hits=0, misses=0, counter = 0;

	while(1){
      counter++;
			if(counter%REPETITION_NUM==0){
				//cout<<"misses = "<<misses<<endl;
				counter=0;
				misses=0;
			}

			if(misses > WAIT_THR){
				cout<<"break misses = "<<misses<<endl;
				break;
			}
			//for(i=0;i<8;i++){
			  //Read this set until the sender notifies that it is gonna send a bit
				time_passed=measure_one_block_access_time(comm_set_addresses[0][0]);
				if(time_passed>HIT_TIME){
					//cout<<"time_passed = "<<time_passed<<endl;
					misses++;
				}
			//}
			//sleep(0.00001);

		}

}


void signal_readyTorecv(uint64_t comm_set_addresses[][8]){
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
		if(counter%(REPETITION_NUM)==0){
		// 	//cout<<"misses = "<<misses<<endl;
		// 	//counter=0;
		// 	//misses=0;
		 	break;
		 }
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
				time_passed=measure_one_block_access_time(comm_set_addresses[2][2]);
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

		return result;
}

int main(int argc, char **argv)
{
	// Put your covert channel setup code here
	int i,j,bitcount=0;
	uint32_t time_passed;
  struct timeval tts, ttf;
	bitset<8> msg;

	establish_covert_channel(communication_buffer, comm_set_addresses);

	for(i=0; i<4; i++){
		for(j=0; j< 8; j++){
			 printf("i,j: %d,%d address = %x ", i,j, comm_set_addresses[i][j]);
			 printf("index = %d\n",((uint64_t) comm_set_addresses[i][j]>>6) % 64);
			//  time_passed = measure_one_block_access_time(comm_set_addresses[i][j]);
			//  cout<<"time passed = "<<time_passed<<endl;
			//  time_passed = measure_one_block_access_time(comm_set_addresses[i][j]);
			//  cout<<"time passed = "<<time_passed<<endl;
		}
	}



	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	bool listening = true;
	while (listening) {
		//printf("Waiting for a new bit signal\n");
		wait_to_recv(comm_set_addresses);
		//printf("Signal that I am ready to recv\n");

		//signal_readyTorecv(comm_set_addresses);
		//sleep(10);
		//printf("Going to recv bit\n");
    gettimeofday(&tts,NULL);
		i = recv_bit(comm_set_addresses);
    gettimeofday(&ttf,NULL);
    printf("Time to receive a bit= %lf\n", (ttf.tv_sec-tts.tv_sec)+(ttf.tv_usec-tts.tv_usec)*0.000001);
		//printf("I received %d\n",i);
		//sleep(10);
		msg.set(7-bitcount,i);
		bitcount++;
		if(bitcount==8){
			bitcount=0;
			printf("letter is %c\n",msg.to_ulong());
		}

		// Put your covert channel code here

	}

	printf("Receiver finished.\n");

	return 0;
}
