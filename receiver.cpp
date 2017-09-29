#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include"util.hpp"

using namespace std;

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	uint64_t int_array[10];
	uint64_t addr;
 
	bool listening = true;
	while (listening) {
		for (int i=0; i<10; i++){

			addr = (long long)&int_array[i];

			uint32_t time_passed = measure_one_block_access_time(addr);
			cout<< "time passed = "<<time_passed<<endl;

		}
		// Put your covert channel code here
	}

	printf("Receiver finished.\n");

	return 0;
}


