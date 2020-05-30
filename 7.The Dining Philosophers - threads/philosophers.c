#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define N_PHILOSOPHERS 5
#define LEFT (i + N_PHILOSOPHERS - 1) % N_PHILOSOPHERS
#define RIGHT (i + 1) % N_PHILOSOPHERS

void *philosopher(void *arg);

void grab_forks(int i);
void put_away_forks(int i);
void test(int i);

void think(int i);
void eat(int i);

enum state {THINKING, HUNGRY, EATING} states[N_PHILOSOPHERS];

int main(int argc, char *argv[])
{
	printf("Hello The Dining Philosophers - threads\n");

	//create threads
	pthread_t philo[N_PHILOSOPHERS];
	for(int i = 0; i < N_PHILOSOPHERS; i++){
		pthread_create(&philo[i], NULL, philosopher, (void *)&philo[i]);
	}
	
	sleep(60);	//60s runtime
	
	//terminate threads
	for(int i = 0; i < N_PHILOSOPHERS; i++){
		pthread_cancel(philo[i]);
		pthread_join(philo[i], NULL);
	}
	
	return 0;
}

void *philosopher(void *arg){
	int *philo_id = (int *)arg;
	while(true)
		printf("Sub philosopher[%d]\n", *philo_id);
}

void grab_forks(int i){
}

void put_away_forks(int i){
}

void test(int i){
}

void think(int i){
	printf("philosopher[%d]: EATING\n", i);
	fflush(stdout);
	sleep(3);
}

void eat(int i){
	printf("philosopher[%d]: THINKING\n", i);
	fflush(stdout);
	sleep(3);
}
