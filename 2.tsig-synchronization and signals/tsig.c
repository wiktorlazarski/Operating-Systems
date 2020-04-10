/*
	Created by Wiktor Lazarski 10/04/2020
	Operating Systems - Laboratory 2
	Synchronization and signals.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

const unsigned int NUM_CHILD = 10;

void terminate(unsigned int last_created, pid_t *ids);

int main(int argc, char **argv)
{
	pid_t process_ids[NUM_CHILD];

	for(int i = 0; i < NUM_CHILD; i++) {
		 pid_t pid = fork();

		if(pid < 0) { //process not created
			printf("Failed to create process - terminateinating program");
			terminate(i - 1, process_ids);
			return 1;	
		}
		else if(pid > 0) { //parent process
			//store process id
			process_ids[i] = pid;
		}
		else { //child process
			pid_t child_pid = getpid();

			printf("New process created with pid: %d\n", child_pid);
			printf("Parent pid: %d\n", getppid());
			
			sleep(10); //10s
			
			printf("Child process %d finished execution\n", child_pid);
			return 0;
		}

		sleep(1);//1s delay between child process creation
	}

	int exits[NUM_CHILD];
	unsigned int loop_cnt = 0;
	while(1) {
		int code = 0;

		int child_pid = wait(&code);
		//-1 means that all children terminated
		if(child_pid == -1) {
			printf("No more child processes\n");
			break;
		}

		exits[loop_cnt] = code;
		loop_cnt++;	
	}
	
	//print exit codes
	printf("---- Received %d exit codes ----\n", loop_cnt);
	for(int i = 0; i < loop_cnt; i++){
		printf("%d\t", exits[i]);
	}
	
	printf("\n");
	return 0;
}

void terminate(unsigned int last_created, pid_t *ids) {
	for(int j = last_created; j >= 0; j--) {
		kill(ids[j], SIGTERM);
	}
}
