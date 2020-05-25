#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

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

const char** load_paths(int argc, char* argv[], int required); 

int main(int argc, char *argv[])
{
	bool mmap_copy = false;
	int opt;
	while((opt = getopt(argc, argv, ":mh")) != -1){
		switch(opt){
			case 'h':
				help();
				return 0;
			case 'm':
				mmap_copy = true;	
				break;
			case '?':
				printf("ERROR: unknown operation: %c\n", optopt);
				exit(FAILED);
		}
	}
	
	size_t n_args = mmap_copy ? MMAP_N_ARGS : RW_N_ARGS;
	
	const char **files;
	files = load_paths(argc, argv, n_args);
	
	int src_file = open(files[SRC_FILE_IDX], O_RDONLY);
	if(src_file < 0){
		printf("ERROR: failed to open src file");
		exit(FAILED);
	}
	
	struct stat src_stat;
	if(fstat(src_file, &src_stat)){
		printf("ERROR: failed to load src mode information");
		exit(FAILED);
	}
	
	int dst_file = open(files[DST_FILE_IDX], O_RDWR | O_CREAT, src_stat.st_mode);
	if(dst_file < 0){
		printf("ERROR: failed to open dst file");
		exit(FAILED);
	}
	
	int result = mmap_copy ? copy_mmap(src_file, dst_file) : copy_read_write(src_file, dst_file);
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

const char** load_paths(int argc, char* argv[], int required){
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
