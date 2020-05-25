#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define SUCCESS 0
#define FAILED 1

#define RW_N_ARGS 3
#define MMAP_N_ARGS 4
#define SRC_FILE_IDX 1
#define DST_FILE_IDX 2

#define OUTPUT_MODE 0700

int copy_read_write(int fd_from, int fd_to);
int copy_mmap(int fd_from, int fd_to);
void help();

const char** load_params(int argc, char* argv[], int required); 

int main(int argc, char *argv[])
{
	int opt, fd_in, fd_out, result;
	const char **files;

	while((opt = getopt(argc, argv, ":mh")) != -1){
		switch(opt){
			case 'h':
				help();
				return 0;
			case 'm':
				files = load_params(argc, argv, MMAP_N_ARGS);
				fd_in = open(files[SRC_FILE_IDX], O_RDONLY);
				if(fd_in < 0){
					printf("ERROR: failed to open src file");
					exit(FAILED);
				}

				fd_out = open(files[DST_FILE_IDX], O_RDWR | O_CREAT);
				if(fd_out < 0){
					printf("ERROR: failed to open dst file");
					exit(FAILED);
				}

				result = copy_mmap(fd_in, fd_out);
				close(fd_in);
				close(fd_out);

				return result;
			case '?':
				printf("ERROR: unknown operation: %c\n", optopt);
				exit(FAILED);
		}
	}
	
	files = load_params(argc, argv, RW_N_ARGS);
	
	fd_in = open(files[SRC_FILE_IDX], O_RDONLY);
	if(fd_in < 0){
		printf("ERROR: failed to open src file");
		exit(FAILED);
	}

	fd_out = open(files[DST_FILE_IDX], O_RDWR | O_CREAT);
	if(fd_out < 0){
		printf("ERROR: failed to open dst file");
		exit(FAILED);
	}
	
	result = copy_read_write(fd_in, fd_out);
	return result;
}

int copy_read_write(int fd_from, int fd_to){
	static const int BUFFER_SIZE = 4096;
  char buffer[BUFFER_SIZE];

	int read_cnt, write_cnt;
	while((read_cnt = read(fd_from, buffer, BUFFER_SIZE)) > 0){
		write_cnt = write(fd_to, buffer, read_cnt);
		if(write_cnt <= 0){
			printf("ERROR: RW copy write failed");
			return FAILED;
		}
	}
	
	return SUCCESS;
}

int copy_mmap(int fd_from, int fd_to){
	printf("MMAP copying\n");
	return SUCCESS;
}

const char** load_params(int argc, char* argv[], int required){
	if(argc < required){
		printf("ERROR: not enought parameters specified\n");
		exit(FAILED);
	}

	static const char *retv[2];

	int src_idx = required - 2;
	int dst_idx = required - 1;

	retv[SRC_FILE_IDX] = argv[src_idx];
	retv[DST_FILE_IDX] = argv[dst_idx];
	return retv;
}

void help(){
	printf("HELP!!!\n");
}
