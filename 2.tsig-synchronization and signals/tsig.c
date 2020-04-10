#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

const unsigned int NUM_CHILD = 10;

int main(int argc, char **argv)
{
	for(int i = 0; i < NUM_CHILD; i++) {
		int pid =	fork();
	
		if(pid < 0) {
			printf("Failed to create process");
			return 1;
		}
		else if(pid > 0) {
			printf("Loop iter count: %d\n", i);
			printf("Parent process id: %d\n", pid);
		}
		else {
			printf("Child process number: %d\n", i);
			return 0;
		}

		sleep(1);	//1s
	}

	return 0;
}
