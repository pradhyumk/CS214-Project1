#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


typedef struct {
    size_t length;
	size_t used;
    char *data;
} strbuff_t;

void sb_destroy(char *L) {
    free(L);
}

int sb_append(strbuff_t *sb, char item)
{
    if (sb->used == sb->length) {
	   size_t size = sb->length * 2;
	   char *p = realloc(sb->data, sizeof(char) * size);
	   if (!p) return 1;

	   sb->data = p;
	   sb->length = size;
    }

    sb->data[sb->used] = sb->data[sb->used-1];
    sb->data[sb->used-1] = item;

    ++sb->used;
    
    return 0;
}

/* String: hello    world
 * width: 3
 * create method that gets the length of the word before printing it
 */

int lastCharacter(int start, int width, char* buffer){
	int counter = 0;
	int ws = 0;
	int word = 0;
	int laststartIndex = start;
	
	while (counter < width) {
		
	}
	
	
    return 0;
}

void ww(int width, int fd_in, char* buffer) {
    lastCharacter(0, width, fd_in, buffer);
}

char* charArray(int fd) {
	char ch;
	strbuff_t sb;
	sb.data = malloc(sizeof(char));
	sb.data[0] = '\0';
	sb.length = 1;
	sb.used   = 1;

	while(read(fd, &ch, 1) != 0) {
		sb_append(&sb, ch);
	}

	if (fd > 2)
	{
		close(fd);
	}
	
	return sb.data;
}

int main(int argc, char **argv) {

	int fd = 0;
	char* data;
	
	if (argc == 3) {
		fd = open(argv[2], O_RDONLY); 
	} else if (argc == 2) {
		char c[] = "Please enter your text: \n";
		write(1, c, sizeof(c)-1);
	} else {
		return EXIT_FAILURE;
	}

	 
	data = charArray(fd);
	printf("Array: %s\n", data);

	unsigned int width = atoi(argv[1]);

	printf("Width: %d\n", width);


	printf("\n");

	sb_destroy(data);


	return EXIT_SUCCESS;
}