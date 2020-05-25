#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SUCCESS 0
#define FAILED 1

#define RW_N_ARGS 3
#define MMAP_N_ARGS 4
#define SRC_FILE_IDX 1
#define DST_FILE_IDX 2

void help();
int copy(const char *src, const char *dst);
int m_copy(const char *src, const char *dst);

const char** load_params(int argc, char* argv[], int required); 

int main(int argc, char *argv[])
{
	int opt;
	const char **files;
	while((opt = getopt(argc, argv, ":mh")) != -1){
		switch(opt){
			case 'h':
				help();
				return 0;
			case 'm':
				files = load_params(argc, argv, MMAP_N_ARGS);
				return m_copy(files[SRC_FILE_IDX], files[DST_FILE_IDX]);
			case '?':
				printf("ERROR: unknown operation: %c\n", optopt);
				exit(FAILED);
		}
	}
	
	files = load_params(argc, argv, RW_N_ARGS);
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

const char** load_params(int argc, char* argv[], int required){
	if(argc < required){
		printf("ERROR: Not enought parameters specified\n");
		exit(FAILED);
	}

	static const char *retv[2];

	int src_idx = required - 2;
	int dst_idx = required - 1;

	retv[SRC_FILE_IDX] = argv[src_idx];
	retv[DST_FILE_IDX] = argv[dst_idx];
	return retv;
}
