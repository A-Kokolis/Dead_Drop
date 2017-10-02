
#include"util.hpp"


#define set_number 8
#define set_jump 1024
#define repetition_num 100

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	int iter,i,j;

	char *communication_buffer;

	communication_buffer = (char*)malloc(sizeof(char)*8000000);	

	//Write to specific sets in the LLC cache to figure out the cache sets that we are
	//gonna be using for the communication
	for(i=0; i<set_number; i+= set_jump){

	  for(iter=0; iter<repetition_num; iter++){
		communication_buffer[i+j] = 'a';	
	  }
	}

	printf("Please type a message.\n");

	bool sending = true;
	while (sending) {
		char text_buf[128];
		fgets(text_buf, sizeof(text_buf), stdin);
		printf("%c\n",text_buf[0]);	
		// Put your covert channel code here
	}

	printf("Sender finished.\n");

	return 0;
}


