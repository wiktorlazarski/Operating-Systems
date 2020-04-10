#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

const unsigned int NUM_CHILD = 10;

int main(int argc, char **argv)
{
	int process_ids[NUM_CHILD];

	for(int i = 0; i < NUM_CHILD; i++) {
		int pid = fork();

		if(pid < 0) {
			printf("Failed to create process");
			//kills all the children processes
			for(int j = i - 1; j > 0; j--) {
				kill(process_ids[j], SIGTERM);
			}
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
