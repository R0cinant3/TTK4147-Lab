#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <string.h>


#define CPU_FREQ_MHZ 2599.992
#define RATIO_NANO_TO_SECOND 1000000000 

void res_rdtsc();
void res_gettime();
void res_times();
