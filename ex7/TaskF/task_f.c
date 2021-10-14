#include <pthread.h>
#include <stdio.h>

#define NUMBER_THREADS 2
#define SIZE    50*1000*1000
pthread_mutex_t mtx1;
pthread_mutex_t mtx2;

long glob_iter = 0;

// Note the argument and return types: void*
void* incrementingThreadFunction(){
    for (int j = 0; j < SIZE; j++) {
        //Start of critical section
        pthread_mutex_lock(&mtx1);
        pthread_mutex_lock(&mtx2);
        // printf("Increment is running!\n");

        glob_iter++;
        //End of critical section
        pthread_mutex_unlock(&mtx1);
        pthread_mutex_unlock(&mtx2);
    }
    return NULL;
}

void* decrementingThreadFunction(){
    for (int j = 0; j < SIZE; j++) {
        //Start of critical section
        pthread_mutex_lock(&mtx1);
        pthread_mutex_lock(&mtx2);

        // printf("Decrement is running!\n");
        glob_iter--;
        //End of critical section
        pthread_mutex_unlock(&mtx1);
        pthread_mutex_unlock(&mtx2);

    }
    return NULL;
}
int main(){
    pthread_t incrementingThread, decrementingThread;
    pthread_mutex_init(&mtx1, NULL);
    pthread_mutex_init(&mtx2, NULL);

    pthread_create(&incrementingThread, NULL, incrementingThreadFunction, NULL);
    pthread_create(&decrementingThread, NULL, decrementingThreadFunction, NULL);

    pthread_join(incrementingThread, NULL);
    pthread_join(decrementingThread, NULL);

    pthread_mutex_destroy(&mtx1);
    pthread_mutex_destroy(&mtx2);

    printf("The magic number is: %ld\n", glob_iter);
    return 0;
}