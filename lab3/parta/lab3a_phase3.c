/*
 ============================================================================
 Name        : lab3a_phase3.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Phase 3. (Atomic operation using O_APPEND flag)
In phase 2 you saw the problem that was caused by the fact that context switch can
sometimes happen between the lseek and write system calls.
Fix the program so that you add the flag O_APPEND in the open call of each program and
see that there is no problem any more. It is not necessary or usefull to use lseek system call
in this phase anymore.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define REPEATED_TIME 1000000

int main(void) {
	int fd = -1;
	int i = 0;

	fd = open("/tmp/exlog.txt", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		for (i = 0; i < REPEATED_TIME; i++)
			write(fd, "AAAA", 4);
	} else
		perror("Open error:");

	return EXIT_SUCCESS;
}


/*
int main(void) {
	int fd = -1;
	int i = 0;

	fd = open("/tmp/exlog.txt", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		for (i = 0; i < REPEATED_TIME; i++)
			write(fd, "BBBB", 4);
	} else
		perror("Open error:");

	return EXIT_SUCCESS;
}
*/
