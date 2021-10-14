#include <pthread.h>
#include <stdio.h>

#define NUMBER_OF_PHILOSOPHERS 5

pthread_mutex_t forks[NUMBER_OF_PHILOSOPHERS];
pthread_t philosophers[NUMBER_OF_PHILOSOPHERS];

// Note the argument and return types: void*
void* pickup(int phil){
    int right = (phil + 1) % NUMBER_OF_PHILOSOPHERS;
	int left = (phil + NUMBER_OF_PHILOSOPHERS) % NUMBER_OF_PHILOSOPHERS;
    if(phil){ //The other phils are right handed
        pthread_mutex_lock(&forks[right]);
		printf("Philosopher %d picked up fork %d\n", phil, right);
		pthread_mutex_lock(&forks[left]);
		printf("Philosopher %d picked up fork %d\n", phil, left);
    }
    else{ //Making phil 0 left handed
        pthread_mutex_lock(&forks[left]);
		printf("Philosopher %d picked up fork %d\n", phil, left);
		pthread_mutex_lock(&forks[right]);
		printf("Philosopher %d picked up fork %d\n", phil, right);
    }

    return NULL;
}
void* putdown(int phil){
    printf("Philosopher %d put down the forks\n", phil);
	pthread_mutex_unlock(&forks[(phil + 1) % NUMBER_OF_PHILOSOPHERS]);
	pthread_mutex_unlock(&forks[(phil + NUMBER_OF_PHILOSOPHERS) % NUMBER_OF_PHILOSOPHERS]);
    return NULL;
}

void* eat(int phil){
	int eatTime = rand() % 2 + 1;
	printf("Philosopher %d will eat for %d seconds\n", phil, eatTime);
	sleep(eatTime);
    return NULL;
}

void* think(int phil){
	int sleepTime = rand() % 2 + 1;
	printf("Philosopher %d will think for %d seconds\n", phil, sleepTime);
	sleep(sleepTime);
    return NULL;
}

void *philosopher(void *phil) {
	while (1) {
		think(phil);
		pickup(phil);
		eat(phil);
		putdown(phil);
	}
}
int main(){
    int i;
	srand(time(NULL));
	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_mutex_init(&forks[i], NULL);
	}

    for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_create(&philosophers[i], NULL, philosopher, (void *)(i));
	}

	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_join(philosophers[i], NULL);
	}

    return 0;
}