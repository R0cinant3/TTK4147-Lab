#include <stdio.h>
#include "busy_waiting.h"
#include <unistd.h>
#include <sys/times.h>

void wait_for_duration(int seconds);
void wait_for_duration_times();

int main(int argc, char* argv[]){
    // sleep(1);
    // wait_for_duration(1);
    wait_for_duration_times();

    return 0;
}

void wait_for_duration(int seconds){
    struct timespec start_time = timespec_normalized(seconds,0);
    busy_wait(start_time);    
}

void wait_for_duration_times(){
    int clocks_per_second = sysconf(_SC_CLK_TCK);
    struct tms cpu_start_time, cpu_end_time;
    clock_t start_time = times(&cpu_start_time);
    clock_t end_time;
    while(end_time - start_time < clocks_per_second){
        for(int i = 0; i < 10000; i++){}
        end_time = times(&cpu_end_time);
    }
}
