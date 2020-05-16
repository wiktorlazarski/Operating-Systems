#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <signal.h>

#define N_PHILOSOPHERS 5
#define LEFT (i + N_PHILOSOPHERS - 1) % N_PHILOSOPHERS
#define RIGHT (i + 1) % N_PHILOSOPHERS

const key_t STATE_MUTEX_KEY =  0x1000;
const key_t PHILOSOPHERS_MUTEXES_KEY = 0x2000;
const key_t SHARED_MEMORY_KEY = 0x3000;

enum state {THINKING, HUNGRY, EATING};
struct shared_mem {
	enum state states[N_PHILOSOPHERS];
} *shm;

void lock(int semid, int idx);
void unlock(int semid, int idx);

void philosopher(int philo_id);

void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void test(int philo_id);

void think(int philo);
void eat(int philo);

void terminate(unsigned int last_created, pid_t *ids);

int main(int argc, char *argv[])
{
	//shared memory
	int shm_id = shmget(SHARED_MEMORY_KEY, sizeof(struct shared_mem), 0600 | IPC_CREAT);
	if(shm_id < 0) {
		perror("shmget: shared memory not created");
		return 1;
	}

	//attach memory
	shm = shmat(shm_id, NULL, 0);
	if(shm == (void*)-1) {
		perror("shmat: failed to attach");
		return 1;
	} 
	//init philosophers state
	for(int i = 0; i < N_PHILOSOPHERS; i++){
		shm->states[i] = THINKING;
	}

	//mutex for state changes
	int state_mutex = semget(STATE_MUTEX_KEY, 1, 0600 | IPC_CREAT);
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
	int philo_mutexes = semget(PHILOSOPHERS_MUTEXES_KEY, N_PHILOSOPHERS, 0600 | IPC_CREAT);
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

void philosopher(int id) {
	while(true) {
		think(id);
		grab_forks(id);
		eat(id);
		put_away_forks(id);
	}
}

void grab_forks(int left_fork_id) {
	int i = left_fork_id;
	int state_mutex = semget(STATE_MUTEX_KEY, 1, 0600);
	int philo_mutexes = semget(PHILOSOPHERS_MUTEXES_KEY, N_PHILOSOPHERS, 0600);
	if(state_mutex < 0 || philo_mutexes < 0) {
		perror("grab_forks: error");
		exit(1);
	}

	lock(state_mutex, 0);
	shm->states[i] = HUNGRY;
	test(i);
	unlock(state_mutex, 0);
	lock(philo_mutexes, i);
}

void put_away_forks(int left_fork_id) {
	int i = left_fork_id;

	int state_mutex = semget(STATE_MUTEX_KEY, 1, 0600);
	if(state_mutex < 0) {
		perror("put_away_forks: error");
		exit(1);
	}

	lock(state_mutex, 0);
	shm->states[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	unlock(state_mutex, 0);
}

void test(int philo_id) {
	int philo_mutexes = semget(PHILOSOPHERS_MUTEXES_KEY, N_PHILOSOPHERS, 0600);
	if(philo_mutexes < 0) {
		perror("test: error");
		exit(1);
	}

	bool is_hungry = shm->states[philo_id] == HUNGRY;
	bool left_eat = shm->states[philo_id] == EATING;
	bool right_eat = shm->states[philo_id] == EATING;

	if(is_hungry && !left_eat && !right_eat) {
		shm->states[philo_id] = EATING;
		unlock(philo_mutexes, philo_id);
	}
}

void lock(int semid, int idx) {
	struct sembuf p = {idx, -1, SEM_UNDO};

	if(semop(semid, &p, 1) < 0){
		perror("semop lock");
		exit(1);
	}
}

void unlock(int semid, int idx) {
	struct sembuf v = {idx, +1, SEM_UNDO};

	if(semop(semid, &v, 1) < 0){
		perror("semop unlock");
		exit(1);
	}
}

void think(int philo) { 
	printf("philosopher[%d]: THINKING\n", philo); 
	sleep(2);
}

void eat(int philo) { 
	printf("philosopher[%d]: EATING\n", philo); 
	sleep(3);
}

void terminate(unsigned int last_created, pid_t *ids) {
	for(int i = last_created; i >= 0; i--) {
		kill(ids[i], SIGTERM);
	}
}
