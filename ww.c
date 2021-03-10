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

	// h e l l o \0 _  _  _  _  
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


int wordLength(int index, strbuff_t *sb){
	
	char *buffer = sb->data;

	if (isspace(buffer[index]))	{
		return -1;
	}

	int counter = index;
	int length = 0;
	while (counter <  sb->used){
		
		if (isspace(buffer[counter])) {
			break;
		}
		
		length++;
		counter++;
	}
	
    return length;
}

// \n  \n\n     hello world --
//hello\n\t\t\nworld --
//hello\n\n\t\t\n\nworld --
//hello \n\t world --
//hello --
//hello      world

/* 
 * hello
 * 
 * world
 * hello  \n world (w=8)
 */


void ww(int width, int fd_in, strbuff_t *sb) {
	int i = 0;	// array index
	int characterUsed = 0; // Characters  in the current line
	int whiteSpace = 0;
	int wordLen;

	wordLen = wordLength(4, sb);
	printf("Length of the word is: %d\n", wordLen);

	while (i < sb->used) { // going through every character one by one
		if (isspace(sb->data[i]) && characterUsed != 0) {	// checking for 2 or more lines at the start AND if the \n is not at the beggining for a sentence

			int printed = 0; // to make sure we only print one para break

			if (sb->data[i] == '\n') {
				i++; // start from next character
				
				while (isspace(sb->data[i])) {
					if (sb->data[i] == '\n' && printed == 0) { 	// saw two \n and printing a paragraph break
						write(fd, "\n\n", 2);
						characterUsed = 0;
						printed = 1;
						i++;
					} else if (isspace(sb->data[i])) {
						i++;
					} 
				}

			} else {
				if (characterUsed == 0) { // if there is a whitepace at the begging of a line
					i++;
				} 
			}

			if (printed == 0) {
				int nextword = wordLength(i, &sb);	// gettting the length of the word to see if it fits; it it fits, we print the space

				if (nextword+1 <= width - characterUsed){
					write(fd, " ", 1);
					characterUsed++;
				} else {
					write(fd, "\n", 1);
					characterUsed = 0;
				}
			}
		} else if (!isspace(sb->data[i])){ // character


			// print white space

			while () { //starts at the current index and prints everything until it hits a whitespace

			}
		} else {
			i++;
		}
	}
}


strbuff_t* charArray(int fd, strbuff_t *sb) {
	char ch;
	sb->data = malloc(sizeof(char));
	sb->data[0] = '\0';
	sb->length = 1;
	sb->used   = 1;

	while(read(fd, &ch, 1) != 0) {
		sb_append(sb, ch);
	}

	if (fd > 2) {
		close(fd);
	}
	
	return sb;
}


int main(int argc, char **argv) {

	int fd = 0;
	strbuff_t sb;
	
	if (argc == 3) {
		fd = open(argv[2], O_RDONLY); 
	} else if (argc == 2) {
		char c[] = "Please enter your text: \n";
		write(1, c, sizeof(c)-1);
	} else {
		return EXIT_FAILURE;
	}

	unsigned int width = atoi(argv[1]);
	 
	charArray(fd, &sb);
	
	printf("---------- Document Array ----------\n");
	printf("%s\n", sb.data);
	printf("------------------------------------\n");
	printf("Width: %d\n", width);
	printf("Document length: %lu\n", sb.used);

	ww(width, fd, &sb);


	sb_destroy(sb.data);


	return EXIT_SUCCESS;
}


			// if ((i+1 < sb->used) && (sb->data[i+1] == '\n')) {
			// 	while (!ispace(sb->data[i+1])) { // print a new line until you see that next character is not a character
			// 		write(fd_in, )

			// 		i += 2;
			// 	}
			// }