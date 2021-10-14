//#define _GNU_SOURCE
#include "io.h"
#include <rtdk.h>
#include <pthread.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <unistd.h>



#define NUMBER_TROLL_THREADS 10

RT_TASK a;
RT_TASK b;
RT_TASK c;

//	EX 6

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


void* troll_function(void * args){
	set_cpu(1);
    while(1){
        asm volatile("" ::: "memory");
    }
}

void comms_function(void * args){
	unsigned long duration = 100000000000; // 10 sec
	unsigned long endTime = rt_timer_read() + duration;


    long channel_id = (long)args;
    set_cpu(1);
    while(1){
        if(!io_read(channel_id)){
            // printf("Channel: %d\t...Is now communicating!\n");
        	io_write(channel_id, 0);
        	rt_timer_spin(5000);
        	io_write(channel_id, 1);
        }
		if(rt_timer_read() > endTime){
			rt_printf("time expired\n");
			rt_task_delete(NULL);		
		}
		if(rt_task_yield()){
			rt_printf("task failed to yield");
			rt_task_delete(NULL);
		}

    }
}



int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	//io_init();

	int prio = 75;	
	int stack_size = 0;
	int mode = T_CPU(1);
 
	//Declaring threads
	pthread_t threads_troll[NUMBER_TROLL_THREADS];

	//Creating threads
	rt_task_create(&a, "a", stack_size, prio, mode);
	rt_task_create(&b, "b", stack_size, prio, mode);
	rt_task_create(&c, "c", stack_size, prio, mode);

	for(int i = 0; i < NUMBER_TROLL_THREADS; i++){
		pthread_create(&threads_troll[i],NULL,(void*)troll_function,NULL);
	}

	//Start threads
	rt_task_start(&a, comms_function,(void*)1);
	rt_task_start(&b, comms_function,(void*)2);
	rt_task_start(&c, comms_function,(void*)3);


	//Troll threads
    	for(int i = 0; i < NUMBER_TROLL_THREADS; i++){
        pthread_join(threads_troll[i], NULL);
    	}
	

	while(1) {
		sleep(-1);
	}
return 0;
}
