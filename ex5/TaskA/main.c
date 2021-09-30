#define _GNU_SOURCE
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include "io.h"

#define PIN_LOW_TIME 5

void* comms_function(void * args){
    int channel_id = (int)args;
    while(1){
        if(!io_read(channel_id)){
            // printf("Channel: %d\t...Is now communicating!\n");
            io_write(channel_id, 0);
            usleep(PIN_LOW_TIME);
            io_write(channel_id, 1);
        }
    }
}

int main(){
    io_init();

    //Declaring threads
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    //Creating threads
    pthread_create(&thread1, NULL, &comms_function, (void *) 1);
    pthread_create(&thread2, NULL, &comms_function, (void *) 2);
    pthread_create(&thread3, NULL, &comms_function, (void *) 3);

    //Joining threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    return 0;
}