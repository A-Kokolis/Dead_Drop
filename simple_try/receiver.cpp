
#include"util.hpp"

using namespace std;


char communication_buffer[CACHE_SIZE];
uint64_t comm_set_addresses[8][8];

bitset<8> ToBits(unsigned char byte)
{
    return bitset<8>(byte);
}


int main(int argc, char **argv)
{
	// Put your covert channel setup code here
	int i,j,bitcount=0;
	uint32_t time_passed;
	bitset<8> msg;

	establish_covert_channel(communication_buffer, comm_set_addresses);

	for(i=0; i<8; i++){
		for(j=0; j< 8; j++){
			 printf("i,j: %d,%d address = %x ", i,j, comm_set_addresses[i][j]);
			 printf("index = %d\n",((uint64_t) comm_set_addresses[i][j]>>6) % 64);
		}
	}

	printf("Please press enter.\n");

	char text_buf[128];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");
  while(1){
  	receive_msg(comm_set_addresses);
    char text_buf[128];
    fgets(text_buf, sizeof(text_buf), stdin);
    send_msg(text_buf,comm_set_addresses);
    sleep(0.01);
    send_break(comm_set_addresses);
    sleep(0.1);
  }
	printf("Receiver finished.\n");

	return 0;
}
