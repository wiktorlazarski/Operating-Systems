#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SUCCESS 0
#define FAILED 1

void help();
int copy(const char *src, const char *dst);
int m_copy(const char *src, const char *dst);

int main(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, ":m:hx")) != -1){
		switch(opt){
			case 'h':
				help();
				return 0;
			case 'm':
				return m_copy("from", "to");
			case ':':
				printf("ERROR: source file required\n");	
				exit(FAILED);
			case '?':
				printf("ERROR: unknown operation: %c\n", optopt);
				exit(FAILED);
		}
	}
	
	return copy("from", "to");
}

void help(){
	printf("HELP!!!\n");
}

int copy(const char *src, const char *dst){
	printf("READ/WRITE coping\n");
	return SUCCESS;
}

int m_copy(const char *src, const char *dst){
	printf("MMAP coping\n");
	return SUCCESS;
}
