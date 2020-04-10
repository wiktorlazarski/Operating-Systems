#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

const unsigned int NUM_CHILD = 3;

void kill_processes(unsigned int last_created, pid_t *ids);

int main(int argc, char **argv)
{
	pid_t process_ids[NUM_CHILD];

	for(int i = 0; i < NUM_CHILD; i++) {
		 pid_t pid = fork();

		if(pid < 0) {	//process not created
			printf("Failed to create process");
			kill_processes(i - 1, process_ids);
			exit(1);
		}
		else if(pid > 0) {	//parent process
			printf("New process id: %d\n", pid);
			//store process id
			process_ids[i] = pid;
		}
		else {	//child process
			printf("Child process number: %d\n", i);
			return 0;
		}

		sleep(1);	//1s
	}

	//print all children process ids
	for(int i = 0; i < NUM_CHILD; i++) {
		printf("%d\t=>\t%d\n", i, process_ids[i]);
	}

	return 0;
}

void kill_processes(unsigned int last_created, pid_t *ids) {
	for(int j = last_created; j > 0; j--) {
		retv = kill(ids[j], SIGTERM);
	}
}
