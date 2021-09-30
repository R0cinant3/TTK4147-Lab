#define _GNU_SOURCE
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
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

struct timespec waketime;
clock_gettime(CLOCK_REALTIME, &waketime);

void timespec_add(struct timespec *t, long us){
    t->tv_nsec += us*1000;
    if(t->tv_nsec > 1000000000){
        t->tv_nsec = t->tv_nsec - 1000000000;
        t->tv_sec += 1;
    }
}

struct timespec period = {.tv_sec = 0, .tv_nsec = 500*1000*1000};


void* comms_function(void * args){
    int channel_id = (int)args;
    set_cpu(1);
    while(1){
        if(!io_read(channel_id)){
            printf("Channel: %d\t...Is now communicating!\n", channel_id);
            io_write(channel_id, 0);
            usleep(PIN_LOW_TIME);
            io_write(channel_id, 1);
            waketime = timespec_add(waketime, period);
            clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
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
    pthread_t threads_troll[NUMBER_TROLL_THREADS];

    //Creating threads
    for(int = 0; i < NUMBER_THREADS; i++)
        pthread_create(&threads[i], NULL, &comms_function, (void *)(i+1));
    for(int i = 0; i < NUMBER_TROLL_THREADS; i++)
        pthread_create(&threads_troll[i], NULL, &troll_function, NULL);

    //Joining threads
    for(int i = 0; i < NUMBER_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    for(int i = 0; i < NUMBER_TROLL_THREADS; i++){
        pthread_join(threads_troll[i], NULL);
    }

    return 0;
}