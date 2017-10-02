
#include"util.hpp"
#include <unistd.h>

void establish_covert_channel(char *communication_buffer){
	 int i,iter;

	 //Write to specific sets in the LLC cache to figure out the cache sets that we are
	 //gonna be using for the communication
	 for(iter=0; iter<REPETITION_NUM; iter++){
	 	for(i=0; i<SET_NUM; i+= SET_JUMP){

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


