//#define _GNU_SOURCE
#include <rtdk.h>
#include <pthread.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <sys/mman.h>
#include <unistd.h>

#define TASK1_ID 1
#define TASK2_ID 2
#define TASK3_ID 3

#define TASK1_PRIO 60
#define TASK2_PRIO 65
#define TASK3_PRIO 70

RT_SEM sem;



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

void task_func(void * args){
    set_cpu(1);
    struct TaskParameters taskparam = *(struct TaskParameters*)args;

    rt_sem_p(&sem, TM_INFINITE);
    rt_task_sleep(10);
    rt_printf("Task ID: %d\tPriority: %d\r\n", taskparam.id, taskparam.priority);
    rt_task_sleep(10);
    rt_sem_v(&sem);
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

	//Creating threads
	rt_task_create(&task1, "Task1", stack_size, TASK1_PRIO, mode);
	rt_task_create(&task2, "Task2", stack_size, TASK2_PRIO, mode);
	rt_task_create(&task3, "Task3", stack_size, TASK3_PRIO, mode);

	//Start threads
	rt_task_start(&task1, task_func,(&(struct TaskParameters){TASK1_ID, TASK1_PRIO}));
	rt_task_start(&task2, task_func,(&(struct TaskParameters){TASK2_ID, TASK2_PRIO}));
	rt_task_start(&task3, task_func,(&(struct TaskParameters){TASK3_ID, TASK3_PRIO}));

    rt_task_sleep(100*1000*1000);
    rt_printf("Broadcasting...\r\n");
    rt_sem_broadcast(&sem);
    rt_task_sleep(100*1000*1000);
    rt_printf("END...\r\n");

    rt_sem_delete(&sem);

    return 0;
}

