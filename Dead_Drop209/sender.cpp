
#include "util.hpp"
#include <unistd.h>
#include <string.h>
#include <bitset>
#include <iostream>

using namespace std;

//char communication_buffer[CACHE_SIZE];

void establish_covert_channel(char *communication_buffer){
	 int i,iter,j;
	 char text_buf[128];

	for(i=0; i<SET_JUMP*2; i+= 64*2){
	      for (j=0; j<SET_NUM*4096;j+=4096){
		   //for (j=0; j<SET_JUMP;j++){
			communication_buffer[i+j] = 'a';	
			printf("address = %x\n",&communication_buffer[i+j]);
			printf("index = %d\n",((uint64_t)&communication_buffer[i+j]>>6) % 64);
			printf("address = %x\n",&communication_buffer[i+j+2*8*64]);
			printf("index = %d\n",((uint64_t)&communication_buffer[i+j+2*8*64]>>6) % 64);
			printf("address = %x\n",&communication_buffer[i+j+4*8*64]);
			printf("index = %d\n",((uint64_t)&communication_buffer[i+j+4*8*64]>>6) % 64);


		   }	
			//printf("OK\n");
	  	}
	 printf("waiting to press enter\n");
       	 fgets(text_buf, sizeof(text_buf), stdin);


	 //Write to specific sets in the LLC cache to figure out the cache sets that we are
	 //gonna be using for the communication
	 for(iter=0; iter<REPETITION_NUM*200; iter++){
	 	for(i=0; i<SET_JUMP*2; i+= 64*2){
		   for (j=0; j<SET_NUM*4096;j+=4096){
		   //for (j=0; j<SET_JUMP;j++){
			communication_buffer[i+j] = 'a';	
			communication_buffer[i+j+ 2*8*64]='a';
			communication_buffer[i+j+ 4*8*64]='a';
			//printf("address = %x\n",&communication_buffer[i+j]);
		   }	
			//printf("OK\n");
	  	}
		//sleep(0.1);
	 }


}

bitset<8> ToBits(unsigned char byte)
{
    return bitset<8>(byte);
}

char communication_buffer[CACHE_SIZE];

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	int iter,i,j, chars;

	//char *communication_buffer;

	//communication_buffer = (char*)malloc(sizeof(char)*CACHE_SIZE);	

	establish_covert_channel(communication_buffer);

	printf("Please type a message.\n");
	
	bitset<8> letter;
	int bitcount=0;
	bool sending = true;
	while (sending) {
		char text_buf[128];
		printf("send me\n");
		fgets(text_buf, sizeof(text_buf), stdin);
		printf("%d length = %d\n",text_buf[0], strlen(text_buf));	
	
	for(chars = 0; chars< strlen(text_buf)-1; chars++){			
		 //printf("Letter = %c\n",text_buf[chars]);
		 letter = bitset<8>(text_buf[chars]);			
		 //cout<<"letter = "<<letter<<endl;
		 for(iter=0; iter<REPETITION_NUM*100; iter++){
		 	bitcount=7;
			for(i=0; i<SET_JUMP*2; i+= 64*2){
			   for (j=0; j<SET_NUM*4096;j+=4096){
				//cout<<letter[bitcount];	
				if(letter[bitcount]){
				  communication_buffer[i+j] = 'a';	
				  communication_buffer[i+j+ 2*8*64]='a';
				  communication_buffer[i+j+ 4*8*64]='a';
	   			 // printf("address = %x\n",&communication_buffer[i+j]);
				 // printf("index = %d\n",((uint64_t)&communication_buffer[i+j]>>6) % 64);
				 // printf("address = %x\n",&communication_buffer[i+j+2*8*64]);
				 // printf("index = %d\n",((uint64_t)&communication_buffer[i+j+2*8*64]>>6) % 64);
				 // printf("address = %x\n",&communication_buffer[i+j+4*8*64]);
				 // printf("index = %d\n",((uint64_t)&communication_buffer[i+j+4*8*64]>>6) % 64);

				}
			   }
			   bitcount--;	
			  //cout<<endl;
			  //cout<<endl;
			}
		 }
	}
		
		// Put your covert channel code here
	}

	printf("Sender finished.\n");

	return 0;
}


