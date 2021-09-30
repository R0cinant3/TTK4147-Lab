#define _GNU_SOURCE
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include "io.h"

#define PIN_LOW_TIME 5

int set_cpu(int cpu_number){
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void comms_function(void * args){
    int channel_id = (int)args;
    set_cpu(1);
    while(1){
        if(io_read(channel_id)){
            printf("Channel: %d\t...Is now communicating!\n");
            io_write(channel_id, 0);
            usleep(PIN_LOW_TIME);
            io_write(channel_id, 1);
        }
    }
}

void troll_function(void * args){
    while(1){
        asm volatile("" ::: "memory");
    }
}

int main(){
    io_init();

    //Declaring threads
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    pthread_t thread4;
    pthread_t thread5;
    pthread_t thread6;
    
    //Creating threads
    pthread_create(&thread1, NULL, &comms_function, (void *) 1);
    pthread_create(&thread2, NULL, &comms_function, (void *) 2);
    pthread_create(&thread3, NULL, &comms_function, (void *) 3);

    pthread_create(&thread4, NULL, &troll_function, NULL);
    pthread_create(&thread5, NULL, &troll_function, NULL);
    pthread_create(&thread6, NULL, &troll_function, NULL);

    //Joining threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);

    return 0;
}