
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
  establish_covert_channel(communication_buffer, comm_set_addresses);

	for(int i=0; i<8; i++){
		for(int j=0; j< 8; j++){
			 printf("i,j: %d,%d address = %x ",i,j, comm_set_addresses[i][j]);
			 printf("index = %d\n",((uint64_t) comm_set_addresses[i][j]>>6) % 64);
		}
	}

	printf("Please type a message.\n");
	bool sending = true;
	while (sending) {
		char text_buf[128];
		fgets(text_buf, sizeof(text_buf), stdin);
    printf("%d length = %d\n",text_buf[0], strlen(text_buf));
    send_msg(text_buf, comm_set_addresses);
    sleep(0.01);
    send_break(comm_set_addresses);
    sleep(0.1);
    receive_msg(comm_set_addresses);
	}

	printf("Sender finished.\n");

	return 0;
}
