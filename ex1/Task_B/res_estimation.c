#include "res_estimation.h"


void res_rdtsc(){
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);

    //User-defined
    uint64_t timer1, timer2;

    for(int i = 0; i < 10*1000*1000; i++){
        //User-defined
        timer1 = __rdtsc();
        timer2 = __rdtsc();
        int ns = (timer2 - timer1) * 1000 / CPU_FREQ_MHZ;
        
        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}

void res_gettime(){
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);

    //User-defined
    struct timespec timer1, timer2;

    for(int i = 0; i < 10*1000*1000; i++){
        //User-defined
        clock_gettime(CLOCK_MONOTONIC, &timer1);
        clock_gettime(CLOCK_MONOTONIC, &timer2);
        int ns = timer2.tv_nsec-timer1.tv_nsec;

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}

void res_times(){
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);

    //User-defined
    struct tms cpu_timer;
    clock_t timer1, timer2;
    int clocks_per_sec = sysconf(_SC_CLK_TCK);

    for(int i = 0; i < 10*1000*1000; i++){
        
        //User-defined
        timer1 = times(&cpu_timer);
        timer2 = times(&cpu_timer);
        int ns = (timer2-timer1)*RATIO_NANO_TO_SECOND/clocks_per_sec;
        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}