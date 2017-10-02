
#include"util.hpp"
#include <unistd.h>

#define set_number 8
#define set_jump 1024
#define repetition_num 100


void establish_covert_channel(char *communication_buffer){
	 int i,iter;

	 //Write to specific sets in the LLC cache to figure out the cache sets that we are
	 //gonna be using for the communication
	 for(iter=0; iter<repetition_num; iter++){
	 	for(i=0; i<set_number; i+= set_jump){

			communication_buffer[i] = 'a';	
	  	}
		sleep(0.000001);
	 }


}





int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	int iter,i,j;

	char *communication_buffer;

	communication_buffer = (char*)malloc(sizeof(char)*8000000);	

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


