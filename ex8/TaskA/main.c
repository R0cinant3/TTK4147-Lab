//#define _GNU_SOURCE
#include <rtdk.h>
#include <pthread.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <unistd.h>


RT_SEM sem;

typedef struct TaskParameters{
    uint8_t id;
    uint8_t priority;
    RTIME rt_time; 
}TaskParameters;

typedef struct Task{
    RT_TASK task;
    TaskParameters task_param;
}Task;

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void task_func(void * args){
    set_cpu(1);
    TaskParameters taskparam = (TaskParameters)args;

    rt_sem_p(&sem, TM_INFINITE);
    rt_printf("Task ID: %d\tPriority: %d\r\n", taskparam.id, taskparam.priority);
    rt_task_sleep(taskparam.rt_time);
    rt_sem_v(&sem);
}

int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	int stack_size = 0;
	int mode = T_CPU(1);

    rt_task_shadow(NULL, "Main", 65, mode);

    rt_sem_create(&sem, "Semaphore", 0, S_PRIO);

    Task task1;
    Task task2;
    Task task3;

    task1.TaskParameters.id = 1;
    task2.TaskParameters.id = 2;
    task3.TaskParameters.id = 3;

    task1.TaskParameters.priority = 60;
    task2.TaskParameters.priority = 65;
    task3.TaskParameters.priority = 70;

    task1.TaskParameters.rt_time = 100*1000;
    task2.TaskParameters.rt_time = 100*1000;
    task3.TaskParameters.rt_time = 100*1000;
 
	//Creating threads
	rt_task_create(&task1.task, "Task1", stack_size, task1.TaskParameters.priority, mode);
	rt_task_create(&task1.task, "Task2", stack_size, task2.TaskParameters.priority, mode);
	rt_task_create(&task1.task, "Task3", stack_size, task3.TaskParameters.priority, mode);

	//Start threads
	rt_task_start(&task1.task, task_func,(void*)task1.TaskParameters);
	rt_task_start(&task1.task, task_func,(void*)task2.TaskParameters);
	rt_task_start(&task1.task, task_func,(void*)task3.TaskParameters);

    rt_task_sleep(100*1000*1000);
    rt_printf("Broadcasting...\r\n");
    rt_sem_broadcast(&sem);
    rt_task_sleep(100*1000*1000);
    rt_printf("END...\r\n");

    rt_sem_delete(&sem);

    return 0;
}

