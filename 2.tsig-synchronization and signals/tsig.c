/*
*	Created by Wiktor Lazarski 11/04/2020
*	Operating Systems - Laboratory 2
*	Synchronization and signals.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define WITH_SIGNALS

const unsigned int NUM_CHILD = 11;

void terminate(unsigned int last_created, pid_t *ids);

#ifdef WITH_SIGNALS
bool is_interrupted = false;
void parent_sigint_handler(int signal);
void child_sigterm_handler(int signal);
#endif

int main(int argc, char **argv)
{
	printf("---- Operating Systems Laboratory 'tsig' ----\n---- NUM_CHILD = %d ----\n", NUM_CHILD);

	#ifdef WITH_SIGNALS
	for(int sig = 1; sig < NSIG; sig++) {
		signal(sig, SIG_IGN);
	}
	signal(SIGCHLD, SIG_DFL);
	signal(SIGINT, parent_sigint_handler);
	#endif
	
	pid_t process_ids[NUM_CHILD];
	pid_t root_pid = getpid();

	for(int i = 0; i < NUM_CHILD; i++) {
		#ifdef WITH_SIGNALS
		//terminate child processes
		if(is_interrupted) {
			terminate(i - 1, process_ids);
			printf("parent[%d]: creation interrupted.\n", root_pid);
			break;
		}
		#endif
		
		pid_t pid = fork();

		if(pid < 0) { //process not created
			printf("parent[%d]: failed to create process - sending SIGTER, signal.", root_pid);
			terminate(i - 1, process_ids);
			return 1;	
		}
		else if(pid > 0) { //parent process
			//store process id
			process_ids[i] = pid;
		}
		else { //child process
			pid_t child_pid = getpid();
			
			#ifdef WITH_SIGNALS
			signal(SIGINT, SIG_IGN);
			signal(SIGTERM, child_sigterm_handler);
			#endif

			printf("child[%d]: new process created, ", child_pid);
			printf("parent pid: %d.\n",  getppid());
			
			sleep(10); //10s
			
			printf("child[%d]: process finished execution.\n", child_pid);
			return 0;
		}

		sleep(1);//1s delay between child process creation
	}

	//store information about exits code of a process
	struct exit {
		int code;
		pid_t id;
	} exits_tab[NUM_CHILD];
	
	unsigned int loop_cnt = 0;
	while(1) {
		int code = 0;

		int child_pid = wait(&code);
		//-1 means that all children terminated
		if(child_pid == -1) {
			printf("parent[%d]: no more child processes.\n", root_pid);
			break;
		}

		struct exit curr = {code, child_pid};
		exits_tab[loop_cnt] = curr;
		loop_cnt++;	
	}
	
	//print exit codes
	printf("\n---- Received %d exit codes ----\n", loop_cnt);
	for(int i = 0; i < loop_cnt; i++){
		printf("parent[%d]: child[%d] exits with code %d.\n", root_pid, exits_tab[i].id, exits_tab[i].code);
	}
	
	printf("\n");
	//restore default handlers
	for(int sig = 1; sig < NSIG; sig++) {
		signal(sig, SIG_DFL);
	}
	return 0;
}

void terminate(unsigned int last_created, pid_t *ids) {
	for(int j = last_created; j >= 0; j--) {
		kill(ids[j], SIGTERM);
	}
}

#ifdef WITH_SIGNALS
void parent_sigint_handler(int sig) {
	printf("parent[%d]: interrupt\n", getpid());
	is_interrupted = true;
}

void child_sigterm_handler(int signal) {
	printf("child[%d]: process termination.\n", getpid());
}
#endif
