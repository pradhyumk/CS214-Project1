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


int wordLength(int index, strbuff_t *sb){
	
	char *buffer = sb->data;

	if (isspace(buffer[index]))	{
		return -1;
	}

	int counter = index;
	int length = 0;

	while (counter <  sb->used-1){
		
		if (isspace(buffer[counter])) {
			break;
		}
		
		length++;
		counter++;
	}
	
    return length;
}


int checkEndWhitespace(int index, strbuff_t *sb){
	char *buffer = sb->data;
	int i = index;
	if (i < sb->used - 1 && isspace(buffer[i]))	{
		while (i < sb->used - 1) {
			if (!isspace(buffer[i])) {
				return -1;
			}
			i++;
		}
	}
	else {
		return -1;
	}

	return 0;
}

void ww(int width, int fd_in, int fd_out, strbuff_t *sb) {
	int i = 0;	// array index
	int characterUsed = 0; // Characters  in the current line
	int longWord = 0;

	while (i < sb->used-1) { // going through every character one by one
		// printf("Character: %c\n", sb->data[i]);

		if (isspace(sb->data[i])) {	// checking for 2 or more lines at the start AND if the \n is not at the beggining for a sentence
			int newLine = 0;
			int printed = 0; // to make sure we only print one para break

			if (sb->data[i] == '\n') {
				newLine = 1;	
			}

			i++; // start from next character
			
			while (i < sb->used-1 && isspace(sb->data[i])) {
				if (sb->data[i] == '\n' && printed == 0 && newLine == 1) { 	// saw two \n and printing a paragraph break
					if (checkEndWhitespace(i, sb) == 0) {
						break;
					}
		
					write(fd_out, "\n\n", 2);
					characterUsed = 0;
					printed = 1;
					i++;
					newLine++;
				} else if (isspace(sb->data[i])) {
					i++;
				} 
			}

			if (printed == 0) {
				int nextword = wordLength(i, sb);	// gettting the length of the word to see if it fits; it it fits, we print the space
				//printf("Char: %c | Next Start Index: %d | NextWordLength: %d | Width - CharUsed: %d\n",sb->data[i], i, nextword, width - characterUsed);
				
				if (nextword+1 <= width - characterUsed  && characterUsed != 0){
					write(fd_out, " ", 1);
					characterUsed++;
				} else if (i != sb->used - 1  && characterUsed != 0){
					write(fd_out, "\n", 1);
					// printf("Entered!\n");
					characterUsed = 0;
					longWord = 1;
				}
			}
		} else if (!isspace(sb->data[i])){ // character
			// printf("Passing in: %c\n", sb->data[i]);
			int wordLen = wordLength(i, sb);
			int tempCharacterUsed = characterUsed;
			
			if (characterUsed != 0 && wordLen > width - characterUsed) { // if the word fits, print it
				if (longWord == 0) {	// came across a word that is longer that the width and havent printed a new line yet
					write(fd_out, "\n", 1);
				}
			} 
			// printf("Index: %d | %c\n", i, sb->data[i]);
			while (i < sb->used-1 && !isspace(sb->data[i])) { //starts at the current index and prints everything until it hits a whitespace
				write(fd_out, &sb->data[i], 1);
				characterUsed++;
				i++;
			}

			if (sb->data[i] == '\0') {
				break;
			}

			if (wordLen > width - tempCharacterUsed && (sb->data[i] != '\n' && sb->data[i+1] != '\n')) { // if the word we just printed was too big, print a new line after that reset 
				characterUsed = 0;
				write(fd_out, "\n", 1);
				// printf("Entered1!\n");
				longWord = 0;
			}

			if (checkEndWhitespace(i, sb) == 0) {
				break;
			}
		} else {
			// int newLine = 0;
			i++;
		}
	}

	write(fd_out, "\n", 1);
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
	int fd_in;
	int fd_out;
	strbuff_t sb;
	unsigned int width;
	
	if (argc == 3) { // either passed in a file or directory
		width = atoi(argv[1]);
		
		struct stat file_stat;
		if (stat(argv[2], &file_stat) != 0){
			return EXIT_FAILURE;
		}

		int dir_check = S_ISDIR(file_stat.st_mode); // 0 is File, 1 is Directory
		if (dir_check == 0) { // just a regular file  was passed in
			fd_in = open(argv[2], O_RDONLY);
			fd_out = 1;
			
			charArray(fd_in, &sb);
			ww(width, fd_in, fd_out, &sb);
			
		} else if (dir_check == 1) {	// a directory was passed in
			DIR *dirp = opendir(argv[2]);  
			struct dirent *de;

			while ((de = readdir(dirp))) {

   			}
		
		}		
	} else if (argc == 2) {
		fd_in = 0;
		fd_out = 1;
		width = atoi(argv[1]);
		
		// char c[] = "Please enter your text: \n";
		// write(1, c, sizeof(c)-1);

		charArray(fd_in, &sb);
		ww(width, fd_in, fd_out, &sb);

	} else {
		return EXIT_FAILURE;
	}
	
	// printf("\n---------- Document Array ----------\n");
	// printf("%s\n", sb.data);
	// printf("------------------------------------\n");
	// printf("Width: %d\n", width);
	// printf("Document length: %lu\n", sb.used);

	sb_destroy(sb.data);

	return EXIT_SUCCESS;
}
