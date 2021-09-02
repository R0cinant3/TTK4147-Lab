#include "res_estimation.h"

void res_gettime(){
    int ns_max = 500;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);

    //User-defined
    struct timespec timer1, timer2;

    for(int i = 0; i < 10*1000*1000; i++){
        //User-defined
        clock_gettime(CLOCK_MONOTONIC, &timer2);
        sched_yield();
        clock_gettime(CLOCK_MONOTONIC, &timer1);
        int ns = timer1.tv_nsec-timer2.tv_nsec;
        // printf("%d\n", ns);

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}