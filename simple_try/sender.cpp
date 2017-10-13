
#include"util.hpp"

using namespace std;

char communication_buffer[CACHE_SIZE];
uint64_t comm_set_addresses[4][8];

bitset<8> ToBits(unsigned char byte)
{
    return bitset<8>(byte);
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

int wait_readyTorecv(uint64_t comm_set_addresses[][8]){
	uint32_t time_passed;
	int i;
	uint64_t hits=0, misses=0, counter = 0;

	while(1){
      counter++;
			if(counter%REPETITION_NUM==0){
				cout<<"misses = "<<misses<<endl;
				counter=0;
				misses=0;
				return 0;
			}

			if(misses > WAIT_THR){
				cout<<"break misses = "<<misses<<endl;
				return 1;
			}
			//for(i=0;i<8;i++){
			  //Read this set until the sender notifies that it is gonna send a bit
				time_passed=measure_one_block_access_time(comm_set_addresses[1][2]);
				if(time_passed>HIT_TIME){
					//cout<<"time_passed = "<<time_passed<<endl;
					misses++;
				}
			//}
			//sleep(0.00001);

		}
}

void send_Bit(uint64_t comm_set_addresses[][8]){
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

void signal_stopSending(uint64_t comm_set_addresses[][8]){
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

int main(int argc, char **argv)
{
	// Put your covert channel setup code here
	int iter,i,j, chars, retry_flag;
	uint32_t time_passed;

  establish_covert_channel(communication_buffer, comm_set_addresses);

	for(i=0; i<4; i++){
		for(j=0; j< 8; j++){
			 printf("i,j: %d,%d address = %x ",i,j, comm_set_addresses[i][j]);
			 printf("index = %d\n",((uint64_t) comm_set_addresses[i][j]>>6) % 64);
			//  time_passed = measure_one_block_access_time(comm_set_addresses[i][j]);
			//  cout<<"time passed = "<<time_passed<<endl;
			//  time_passed = measure_one_block_access_time(comm_set_addresses[i][j]);
			//  cout<<"time passed = "<<time_passed<<endl;
		}
	}

	printf("Please type a message.\n");

	bitset<8> letter;

	bool sending = true;
	while (sending) {
		char text_buf[128];
		fgets(text_buf, sizeof(text_buf), stdin);
    printf("%d length = %d\n",text_buf[0], strlen(text_buf));

		for(chars = 0; chars< strlen(text_buf)-1; chars++){
						 printf("Letter = %c\n",text_buf[chars]);
						 letter = bitset<8>(text_buf[chars]);
						 for(i=7; i>=0; i--){
							 signal:;
							 printf("Signal ready to send\n");
							 signal_readyTosend(comm_set_addresses);
							//  retry_flag = wait_readyTorecv(comm_set_addresses);
							//  printf("Receiver said that he is %d about receiving\n",retry_flag);
							//  sleep(10);
							//  if(!retry_flag){
							// 	 printf("Receiver did not get the signal or I missed one. Try again\n");
							// 	 goto signal;
							//  }
						 	if(letter[i]){
									printf("Sending 1\n");
									send_Bit(comm_set_addresses);
									printf("Signal stop sending\n");
									signal_stopSending(comm_set_addresses);
									printf("Signal stop sending DONE\n");

									//sleep(5);
									//correction_flag = send_Bit(comm_set_addresses);

							}else{
								printf("Sending 0\n");
								signal_stopSending(comm_set_addresses);

								//sleep(5);
							}
					  }

		}


		// Put your covert channel code here
	}

	printf("Sender finished.\n");

	return 0;
}
