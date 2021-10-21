//#define _GNU_SOURCE
#include <rtdk.h>
#include <pthread.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <native/mutex.h>

#include <sys/mman.h>
#include <unistd.h>

#define TASK1_ID 1
#define TASK2_ID 2

#define TASK1_PRIO 60
#define TASK2_PRIO 70

RT_SEM sem;
RT_MUTEX mutex_a;
RT_MUTEX mutex_b;

typedef struct TaskParameters{
    uint8_t id;
    uint8_t priority;
}TaskParameters;

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void busy_wait_us(unsigned long delay){
    for(; delay > 0; delay--){
        rt_timer_spin(1000);
    }
}

void task_func(void * args){
    set_cpu(1);
    struct TaskParameters taskparam = *(struct TaskParameters*)args;
    rt_printf("Task ID: %d\tPriority: %d\r\n", taskparam.id, taskparam.priority);
    switch(taskparam.id){
        case 1: 
            rt_mutex_acquire(&mutex_a, TM_INFINITE);
            busy_wait_us(3);
            rt_mutex_acquire(&mutex_b, TM_INFINITE);
            busy_wait_us(3);
            rt_mutex_release(&mutex_b);
            rt_mutex_release(&mutex_a);
            busy_wait_us(1);
            break;
        case 2:
            rt_task_sleep(1);
            rt_mutex_acquire(&mutex_b, TM_INFINITE);
            busy_wait_us(1);
            rt_mutex_acquire(&mutex_a, TM_INFINITE);
            busy_wait_us(2);
            rt_mutex_release(&mutex_a);
            rt_mutex_release(&mutex_b);
            busy_wait_us(1);
            break;
    }
}


int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	int stack_size = 0;
	int mode = T_CPU(1);

    rt_task_shadow(NULL, "Main", 65, mode);

    rt_sem_create(&sem, "Semaphore", 0, S_PRIO);

    RT_TASK task1;
    RT_TASK task2;

    rt_mutex_create(&mutex_a,"A Mutex");
    rt_mutex_create(&mutex_b,"B Mutex");
 
	//Creating threads
	rt_task_create(&task1, "Task1", stack_size, TASK1_PRIO, mode);
	rt_task_create(&task2, "Task2", stack_size, TASK2_PRIO, mode);

	//Start threads
	rt_task_start(&task1, task_func,(&(struct TaskParameters){TASK1_ID, TASK1_PRIO}));
	rt_task_start(&task2, task_func,(&(struct TaskParameters){TASK2_ID, TASK2_PRIO}));

    rt_task_sleep(100*1000*1000);
    rt_printf("Broadcasting...\r\n");
    rt_sem_broadcast(&sem);
    rt_task_sleep(100*1000*1000);
    rt_printf("END...\r\n");

    rt_sem_delete(&sem);


    return 0;
}

