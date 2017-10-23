#include <stdio.h>
#include <iostream>
#include <bitset>
#include <unistd.h>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"

using namespace std;

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

// OCall implementations
void ocall_print(const char* str, uint64_t addr) {
    printf("%s %x index = %llu\n", str, addr, (addr>>6) % 64);
}

#define CACHE_SIZE 32768
#define SET_NUM 8
#define REPETITION_NUM 100
#define HIT_TIME 50
#define WAIT_THR 12

char communication_buffer[CACHE_SIZE];
uint64_t comm_set_addresses[4][8];

int main(int argc, char const *argv[]) {
    int i,j;
    int iter, chars, retry_flag;
    uint32_t time_passed;

    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }
    //int ptr;

    //sgx_status_t status = generate_random_number(global_eid, &ptr);

    establish_covert_channel(global_eid,communication_buffer, comm_set_addresses);


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

                   for(int i=0; i<10000;i++){
                      signal_readyTosend(global_eid, comm_set_addresses);
                    }

                    //printf("Sleeping\n");
                    sleep(2);


                   //printf("sleeping\n");
                   //sleep(10);
                  //  if(!retry_flag){
                  // 	 printf("Receiver did not get the signal or I missed one. Try again\n");
                  // 	 goto signal;
                  //  }
                  if(letter[i]){
                      printf("Sending 1\n");
                      for(int i=0; i<10000;i++){
                        send_Bit(global_eid, comm_set_addresses);
                      }
                      printf("Signal stop sending\n");
                      for(int i=0; i<10000;i++){
                        signal_stopSending(global_eid, comm_set_addresses);
                      }
                      //printf("Signal stop sending DONE\n");

                      //sleep(1);
                      //correction_flag = send_Bit(comm_set_addresses);

                  }else{
                    printf("Sending 0\n");
                    // for(int i=0; i<10000;i++){
                    //   signal_stopSending(global_eid, comm_set_addresses);
                    // }
                    //sleep(1);
                  }
              }

        }

      }


      // Put your covert channel code here

    printf("Sender finished.\n");

    // int ptr;
    // sgx_status_t status = generate_random_number(global_eid, &ptr);
    // std::cout << status << std::endl;
    // if (status != SGX_SUCCESS) {
    //     std::cout << "noob" << std::endl;
    // }
    // printf("Random number: %d\n", ptr);
    //
    // // Seal the random number
    // size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(ptr);
    // uint8_t* sealed_data = (uint8_t*)malloc(sealed_size);
    //
    // sgx_status_t ecall_status;
    // status = seal(global_eid, &ecall_status,
    //         (uint8_t*)&ptr, sizeof(ptr),
    //         (sgx_sealed_data_t*)sealed_data, sealed_size);
    //
    // if (!is_ecall_successful(status, "Sealing failed :(", ecall_status)) {
    //     return 1;
    // }
    //
    // int unsealed;
    // status = unseal(global_eid, &ecall_status,
    //         (sgx_sealed_data_t*)sealed_data, sealed_size,
    //         (uint8_t*)&unsealed, sizeof(unsealed));
    //
    // if (!is_ecall_successful(status, "Unsealing failed :(", ecall_status)) {
    //     return 1;
    // }
    //
    // std::cout << "Seal round trip success! Receive back " << unsealed << std::endl;

    return 0;
}
