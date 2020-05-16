#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <signal.h>

typedef int semid;

const unsigned int N_PHILOSOPHERS = 5;
const key_t STATE_MUTEX_KEY =  0x1111;
const key_t PHILOSOPHERS_MUTEXES_KEY = 0x1112;

enum state {THINKING, HUNGRY, EATING};

void lock(int mutex);
void unlock(int mutex);

void philosopher(int philo_id);

void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void test(int philo_id);

void think(int philo);
void eat(int philo);

void terminate(unsigned int last_created, pid_t *ids);

int main(int argc, char *argv[])
{
	enum state philosophers[N_PHILOSOPHERS];

	//mutex for state changes
	semid state_mutex = semget(STATE_MUTEX_KEY, 1, 0600 | IPC_CREAT);
	if(state_mutex < 0) {
		perror("semget: state mutex not created");
		return 1;
	}
	
	union semaphore_un{
		int val;
		unsigned int *array;
	} sem_un;

	//init mutex counter
	sem_un.val = 1;
	if(semctl(state_mutex, 0, SETVAL, sem_un) < 0) {
		perror("semctl: state mutex value failed to set");
		return 1;
	}

	//mutexes for philosopher grab\put away forks
	semid philo_mutexes = semget(PHILOSOPHERS_MUTEXES_KEY, N_PHILOSOPHERS, 0600 | IPC_CREAT);
	if(philo_mutexes < 0) {
		perror("semget: philosophers mutexes not created");
		return 1;
	}

	//init mutexes couters
	unsigned int ones[N_PHILOSOPHERS];
	for(int i = 0; i < N_PHILOSOPHERS; i++) {
		ones[i] = 1;
	}
	sem_un.array = ones;
	if(semctl(philo_mutexes, N_PHILOSOPHERS, SETALL, sem_un) < 0) {
		perror("semctl: philo mutexes values failed to set");
		return 1;
	}

	//philosophers' processes construction
	pid_t process_ids[N_PHILOSOPHERS];
	for(int i = 0; i < N_PHILOSOPHERS; i++){
		pid_t pid = fork();
		
		if(pid < 0) { //process not created
			perror("Failed to create child process");
			terminate(i - 1, process_ids);
			return 1;
		}
		else if(pid > 0) { //parent process
			//store process id
			process_ids[i] = pid;
		}
		else {	//child\philosopher process
			philosopher(i);
			return 0;
		}
	}

	//waits for child processes to terminate
	for(int i = 0; i < N_PHILOSOPHERS; i++) {
		int code;
		wait(&code);	
	}
	
	return 0;
}

void philosopher(int philo_id) {
	while(true) {
		think(philo_id);
		grab_forks(philo_id);
		eat(philo_id);
		put_away_forks(philo_id);
	}
}

void grab_forks(int left_fork_id) {

}

void put_away_forks(int left_fork_id) {

}

void test(int philo_id) {

}

void lock(int sem) {
	struct sembuf p = {sem, -1, SEM_UNDO};

	if(semop(sem, &p, 1) < 0){
		perror("semop unlock");
		exit(1);
	}
}

void unlock(int sem) {
	struct sembuf v = {sem, +1, SEM_UNDO};

	if(semop(sem, &v, 1) < 0){
		perror("semop unlock");
		exit(1);
	}
}

void think(int philo) { 
	printf("philosopher[%d]: THINKING\n", philo); 
}

void eat(int philo) { 
	printf("philosopher[%d]: EATING\n", philo); 
}

void terminate(unsigned int last_created, pid_t *ids) {
	for(int i = last_created; i >= 0; i--) {
		kill(ids[i], SIGTERM);
	}
}
