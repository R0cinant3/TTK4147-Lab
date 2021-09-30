#define _GNU_SOURCE
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include "io.h"

#define PIN_LOW_TIME 5
#define NUMBER_THREADS 3
#define NUMBER_TROLL_THREADS 10

int set_cpu(int cpu_number){
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void* comms_function(void * args){
    int channel_id = (int)args;
    set_cpu(1);
    while(1){
        if(!io_read(channel_id)){
            // printf("Channel: %d\t...Is now communicating!\n");
            io_write(channel_id, 0);
            usleep(PIN_LOW_TIME);
            io_write(channel_id, 1);
        }
    }
}

void* troll_function(void * args){
    while(1){
        asm volatile("" ::: "memory");
    }
}

int main(){
    io_init();

    //Declaring threads
    pthread_t threads[NUMBER_THREADS];
    pthread_t thread_troll[NUMBER_TROLL_THREADS];

    //Creating threads
    for(int = 0; i < NUMBER_THREADS; i++)
        pthread_create(&threads[i], NULL, &comms_function, (void *)(i+1));
    for(int i = 0; i < NUMBER_TROLL_THREADS; i++)
        pthread_create(&thread_troll[i], NULL, &troll_function, NULL);

    //Joining threads
    for(int i = 0; i < NUMBER_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    for(int i = 0; i < NUMBER_TROLL_THREADS; i++){
        pthread_join(thread_troll[i], NULL);
    }

    return 0;
}