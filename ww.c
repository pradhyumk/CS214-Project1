#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define isDirectory 0;

// int isdir(char *name) {
// 	struct stat data;
	
// 	int err = stat(name, &data);
	
// 	// should confirm err == 0
// 	if (err) {
// 		perror(name);  // print error message
// 		return 0;
// 	}
	
// 	if (S_ISDIR(data.st_mode)) {
// 		// S_ISDIR macro is true if the st_mode says the file is a directory
// 		// S_ISREG macro is true if the st_mode says the file is a regular file
//         #undef isDirectory
//         #define isDirectory 1

// 		return 1;
// 	} 

// 	return 0;
// }


int lastCharacter(int start, int width, int fd, char* buffer){

    return 0;
}

void ww(int width, int fd_in, char* buffer) {
    lastCharacter(0, width, fd_in, buffer);
}

int main(int argc, char **argv) {
    printf("Filename: %s\n", argv[2]);
	
	int fd = open(argv[2], O_RDONLY);
	printf("fd: %d\n", fd);

	unsigned int width = atoi(argv[1]);
	printf("Width: %d\n", width);

	char data;

	ww(width, fd, &data);

    char c;

    while (read(fd, &c, 1) == 1){
        putchar(c);
	}
	
	printf("\n");

	close(fd);
}