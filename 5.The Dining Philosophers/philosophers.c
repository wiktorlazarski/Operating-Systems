#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

const unsigned int STATE_MUTEX_KEY =  0x1111;

enum state {THINKING, HUNGRY, EATING};

void lock(int mutex);
void unlock(int mutex);

void philosopher(int philo_id);

void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void test(int philo_id);

void think() { printf("THINKING\n"); }
void eat() { printf("EATING\n"); }

int main(int argc, char *argv[])
{
	//mutex for state changes
	int mutex_id = semget(STATE_MUTEX_KEY, 1, 0600 | IPC_CREAT);
	if(mutex_id < 0) {
		perror("semget: mutex not created");
		return -1;
	}

	union mutex{
		int val;
	} mutex;

	mutex.val = 1;
	if(semctl(mutex_id, 0, SETVAL, mutex) < 0) {
		perror("semctl: mutex val failed to set");
		return -1;
	}
	
	//array of mutexes for grab/put away fork changes


	//philosophers' processes construction
	pid_t pid = fork();
	if(pid < 0) {
		perror("Failed to create child process");
		return -1;
	}
	else if(pid > 0) {
		//parent process
	}
	else {	//child/philosopher process
		printf("PHILOSOPHER\n");
		return 0;
	}

	int code;
	wait(&code);	
	return 0;
}

void philosopher(int philo_id) {
	while(true) {
		think();
		grab_forks(philo_id);
		eat();
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
	static struct sembuf p = {0, -1, SEM_UNDO};

	if(semop(sem, &p, 1) < 0){
		perror("semop unlock");
		exit(-1);
	}
}

void unlock(int sem) {
	static struct sembuf v = {0, +1, SEM_UNDO};

	if(semop(sem, &v, 1) < 0){
		perror("semop unlock");
		exit(-1);
	}
}
