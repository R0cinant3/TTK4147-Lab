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
#define TASK3_ID 3

#define TASK1_PRIO 60
#define TASK2_PRIO 65
#define TASK3_PRIO 70

#define TASK1_RT_TIME 0
#define TASK2_RT_TIME 1
#define TASK3_RT_TIME 2

#define TASK1_BUSY_TIME 3
#define TASK2_BUSY_TIME 5
#define TASK3_BUSY_TIME 2

RT_SEM sem;
RT_MUTEX mutex;

typedef struct TaskParameters{
    uint8_t id;
    uint8_t priority;
    RTIME rt_time;
    unsigned long busy_time;
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
    switch(taskparam.id){
        case 1: 
            // rt_sem_p(&sem, TM_INFINITE);
            rt_printf("Task ID: %d\tPriority: %d\r\n", taskparam.id, taskparam.priority);
            rt_mutex_acquire(&mutex, TM_INFINITE);
            busy_wait_us(taskparam.busy_time);
            rt_mutex_release(&mutex);
            // rt_sem_v(&sem);
            break;
        case 2:
            rt_printf("Task ID: %d\tPriority: %d\r\n", taskparam.id, taskparam.priority);
            rt_task_sleep(taskparam.rt_time);
            busy_wait_us(taskparam.busy_time);
            break;
        case 3:
            rt_printf("Task ID: %d\tPriority: %d\r\n", taskparam.id, taskparam.priority);
            rt_task_sleep(taskparam.rt_time);
            rt_mutex_acquire(&mutex, TM_INFINITE);
            // rt_sem_p(&sem, TM_INFINITE);
            busy_wait_us(taskparam.busy_time);
            rt_mutex_release(&mutex);
            // rt_sem_v(&sem);
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
    RT_TASK task3;

    rt_mutex_create(&mutex_low,"Low Mutex");
    rt_mutex_create(&mutex_high,"High Mutex");
 
	//Creating threads
	rt_task_create(&task1, "Task1", stack_size, TASK1_PRIO, mode);
	rt_task_create(&task2, "Task2", stack_size, TASK2_PRIO, mode);
	rt_task_create(&task3, "Task3", stack_size, TASK3_PRIO, mode);

	//Start threads
	rt_task_start(&task1, task_func,(&(struct TaskParameters){TASK1_ID, TASK1_PRIO, TASK1_RT_TIME, TASK1_BUSY_TIME}));
	rt_task_start(&task2, task_func,(&(struct TaskParameters){TASK2_ID, TASK2_PRIO, TASK2_RT_TIME, TASK2_BUSY_TIME}));
	rt_task_start(&task3, task_func,(&(struct TaskParameters){TASK3_ID, TASK3_PRIO, TASK3_RT_TIME, TASK3_BUSY_TIME}));

    rt_task_sleep(100*1000*1000);
    rt_printf("Broadcasting...\r\n");
    rt_sem_broadcast(&sem);
    rt_task_sleep(100*1000*1000);
    rt_printf("END...\r\n");

    rt_sem_delete(&sem);


    return 0;
}

