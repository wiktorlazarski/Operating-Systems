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
int philo_id[N_PHILOSOPHERS];

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s[N_PHILOSOPHERS];

int main(int argc, char *argv[])
{
	//init philosopher ids and state
	for(int i = 0; i < N_PHILOSOPHERS; i++){
		philo_id[i] = i;
		states[i] = THINKING;
	}

	//init philosopher mutexes
	for(int i = 0; i < N_PHILOSOPHERS; i++){
		if(pthread_mutex_init(&s[i], NULL) != 0){
			perror("failed to init mutex\n");
			exit(1);
		}
		//set init state to lock
		pthread_mutex_lock(&s[i]);
	}

	//create threads
	pthread_t philo_tid[N_PHILOSOPHERS];
	for(int i = 0; i < N_PHILOSOPHERS; i++){
		if(pthread_create(&philo_tid[i], NULL, philosopher, (void *)&philo_id[i]) != 0){
			perror("failed to create thread");
			exit(1);
		}
	}
	
	sleep(60);	//60s runtime
	
	//terminate threads
	for(int i = 0; i < N_PHILOSOPHERS; i++){
		pthread_cancel(philo_tid[i]);
		pthread_join(philo_tid[i], NULL);
	}

	//destroy philosopher mutexes
	for(int i = 0; i < N_PHILOSOPHERS; i++){
		pthread_mutex_destroy(&s[i]);
	}
	
	return 0;
}

void *philosopher(void *arg){
	int philo_id = *((int *)arg);
	while(true){
		think(philo_id);
		grab_forks(philo_id);
		eat(philo_id);
		put_away_forks(philo_id);
	}
}

void grab_forks(int i){
	pthread_mutex_lock(&m);
	states[i] = HUNGRY;
	test(i);
	pthread_mutex_unlock(&m);
	pthread_mutex_lock(&s[i]);
}

void put_away_forks(int i){
	pthread_mutex_lock(&m);
	states[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	pthread_mutex_unlock(&m);
}

void test(int i){
	bool is_hungry = states[i] == HUNGRY;
	bool left_eat = states[LEFT] == EATING;
	bool right_eat = states[RIGHT] == EATING;

	if(is_hungry && !left_eat && !right_eat){
		states[i] = EATING;
		pthread_mutex_unlock(&s[i]);
	}
}

void think(int i){
	printf("philosopher[%d]: THINKING\n", i);
	fflush(stdout);
	sleep(3);
}

void eat(int i){
	printf("philosopher[%d]: EATING\n", i);
	fflush(stdout);
	sleep(3);
}
