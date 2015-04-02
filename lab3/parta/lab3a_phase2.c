/*
 ============================================================================
 Name        : lab3a_phase2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Phase 2. (Concurrent writing data to the file; consequent lseek and write)
Modify the program of phase 1 so that before each call of write function, you set the file
position pointer at the end of the file (using function lseek).
See how many outputs have now successfully written to the file.

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define REPEATED_TIME 100000

int main(void) {
	int fd = -1;
	int i = 0;

	fd = open("/tmp/exlog.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		for (i = 0; i < REPEATED_TIME; i++) {
			lseek(fd, 0, SEEK_END);
			write(fd, "AAAA", 4);
		}
	} else
		perror("Open error:");

	return EXIT_SUCCESS;
}


/*
int main(void) {
	int fd = -1;
	int i = 0;

	fd = open("/tmp/exlog.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		for (i = 0; i < REPEATED_TIME; i++) {
			lseek(fd, 0, SEEK_END);
			write(fd, "BBBB", 4);
		}
	} else
		perror("Open error:");

	return EXIT_SUCCESS;
}
*/
