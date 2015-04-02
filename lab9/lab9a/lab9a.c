/*
 ============================================================================
 Name        : lab9a.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Exercise 9a (Pipe, 1p)
A process first creates a child process. The child process sends digits 0, 1, 2, 3,..., 9 in one
second intervals to the parent via a pipe. When all digits have been sent the child closes
the write end of the pipe and exits.

Parent process reads the pipe and always immediately displays the received digit to the
screen. The parent process terminates when it has read all characters from the pipe (the
parent does not know in advance how many characters the child process sends). The end
can be recognized because reading from an empty pipe returns 0 (end of file), if the pipe is
not anymore open for writing (this is the reason we close the pipe in the child when all
characters are written).

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	int i = 0;
	int fdArray[2];
	char chr;
	pid_t childPid = -1;
	int retval;

	if (pipe(fdArray) == -1) {
		perror("pipe creation");
		exit(EXIT_FAILURE);
	}

	childPid = fork();
	if (childPid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (childPid == 0) {
		close(fdArray[0]);	// This is to ensure child cannot accidentally read
		for (i = 0; i < 10; i++) {
			chr = '0' + i;
			if (write(fdArray[1], &chr, 1) == -1) {
				perror("write");
				exit(EXIT_FAILURE);
			}
			sleep(1);
		}
		close(fdArray[1]);
	} else {
		close(fdArray[1]);	// This is to ensure parent cannot accidentally write
		while((retval = read(fdArray[0], &chr, 1)) != 0) {
			write(STDOUT_FILENO, &chr, 1);
		}
		printf("Parent received EOF: %d\n", retval);
		close(fdArray[0]);
	}

	return EXIT_SUCCESS;
}
