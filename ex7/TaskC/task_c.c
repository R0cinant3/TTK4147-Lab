#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define NUMBER_THREADS 2

/*
    real    0m14,512s
    user    0m18,523s
    sys     0m10,337s
*/


long glob_iter = 0;
sem_t mutex;

// Note the argument and return types: void*
void* fn(void* args){
    int thread_id = (int)args;
    long local_iter = 0;
    for(;local_iter < 50*1000*1000; local_iter++){
        sem_wait(&mutex);
        glob_iter++; //Issue: Data race between threads
        sem_post(&mutex);
    }
    printf("Local Iterator %d:\t%ld\n", thread_id, local_iter);
    printf("Global Iterator:\t%ld\n", glob_iter);
    return NULL;
}
int main(){
    pthread_t thread_handles[NUMBER_THREADS];
    sem_init(&mutex, 0, 1);

    for(int i = 0; i < NUMBER_THREADS; i++)
        pthread_create(&thread_handles[i], NULL, fn, (void *)(i+1));

    for(int i = 0; i < NUMBER_THREADS; i++)
        pthread_join(thread_handles[i], NULL);
    
    sem_destroy(&mutex);
    
    return 0;
}