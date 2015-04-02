/*
 ============================================================================
 Name        : lab9b.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Exercise 9b (Extra exercise, Pipe, 0.25p)

Write a program that consists of six processes. The program that is started from the
command line is running as a main process of the system. This process then creates five
child processes. The child processes are “sisters”, because they all have the same parent
process. Now you need to construct a pipeline from sister to sister. The child that is
created last (the fifth child) is the beginning of the pipeline. The fifth child sends
information to the fourth child. The fourth child sends information to the third child and
so on. The first child has the end of the pipe line. When the processes have written the
information to the pipe, they close the write end of the pipe and terminate.

Children that read the pipe (1st, 2nd, 3rd and 4th child) read the pipe until end of file is
received.

Now we only carry a single character through the pipeline (from the last child to the first
child). The last (youngest) child process sends a character A to its older sister (and closes
the write end of the pipe and terminates). The sister that received the character increments
the ascii code of the character by one and forwards it to its big sister. The same procedure
continues until the first child has received the character. The first (oldest) child process
only displays the character that it has received from the second child process.

Remark 1. You have to use pipe to pass information between child processes.
Remark 2. When the parent process has created all children, it only waits until all children
have terminated.
Remark 3. To make the program behavior easier to follow, put a delay of half a second
between creations of children.

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


int main(void) {
	pid_t childPid;
	int retval = -1;
	int i = 0;
	//int n = 0;
	int fdArray[4][2];
	char chr;

	// Create 4 pipes for child processes to communicate
	for (i = 0; i < 4; i++) {
		if (pipe(fdArray[i]) == -1) {
			perror("pipe creation");
			exit(EXIT_FAILURE);
		}
	}

	// Create 5 child processes
	for (i = 0; i < 5; i++) {
		childPid = fork();
		if (childPid < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (childPid == 0) {	// This is child process
			if (i == 4) { 		// Child process 5th
				for (i = 0; i < 3; i++) {
					close(fdArray[i][0]);
					close(fdArray[i][1]);
				}
				close(fdArray[3][0]);
				if (write(fdArray[3][1], "A", 1) == -1) {
					perror("write");
					exit(EXIT_FAILURE);
				}
				close(fdArray[3][1]);
				exit(EXIT_SUCCESS);
			} else if (i == 3) {
				for (i = 0; i < 2; i++) {
					close(fdArray[i][0]);
					close(fdArray[i][1]);
				}
				close(fdArray[2][0]);
				close(fdArray[3][1]);
				while((retval = read(fdArray[3][0], &chr, 1)) != 0) {
					if (retval == -1) {
						perror("read");
						exit(EXIT_FAILURE);
					}
					chr = chr + 1;
					write(fdArray[2][1], &chr, 1);
				}
				close(fdArray[3][0]);
				close(fdArray[2][1]);
				exit(EXIT_SUCCESS);
			} else if (i == 2) {
				close(fdArray[0][0]);
				close(fdArray[0][1]);
				close(fdArray[1][0]);
				close(fdArray[2][1]);
				close(fdArray[3][0]);
				close(fdArray[3][1]);
				while((retval = read(fdArray[2][0], &chr, 1)) != 0) {
					if (retval == -1) {
						perror("read");
						exit(EXIT_FAILURE);
					}
					chr = chr + 1;
					write(fdArray[1][1], &chr, 1);
				}
				close(fdArray[1][1]);
				close(fdArray[2][0]);
				exit(EXIT_SUCCESS);
			} else if (i == 1) {
				for (i = 2; i < 4; i++) {
					close(fdArray[i][0]);
					close(fdArray[i][1]);
				}
				close(fdArray[0][0]);
				close(fdArray[1][1]);
				while ((retval = read(fdArray[1][0], &chr, 1)) != 0) {
					if (retval == -1) {
						perror("read");
						exit(EXIT_FAILURE);
					}
					chr = chr + 1;
					write(fdArray[0][1], &chr, 1);
				}
				close(fdArray[0][1]);
				close(fdArray[1][0]);
				exit(EXIT_SUCCESS);
			} else {
				for (i = 1; i < 4; i++) {
					close(fdArray[i][0]);
					close(fdArray[i][1]);
				}
				close(fdArray[0][1]);
				while((retval = read(fdArray[0][0], &chr, 1)) != 0) {
					if (retval == -1) {
						perror("read");
						exit(EXIT_FAILURE);
					}
					printf("The 1st child is displaying character to stdout from the pipeline\n");
					write(STDOUT_FILENO, &chr, 1);
					printf("\n");
				}
				close(fdArray[0][0]);
				exit(EXIT_SUCCESS);
			}
		}
		printf("Child created. pid = %d\n", childPid);
		usleep(500000);
	}
	// Parent waits for all child processes to terminate
	for (i = 0; i < 4; i++) {
		close(fdArray[i][0]);
		close(fdArray[i][1]);
	}
	while ((childPid = waitpid(-1, NULL, 0))) {
	   if (errno == ECHILD) {
	      break;
	   }
	}
	printf("All child processes have terminated\n");

	return EXIT_SUCCESS;
}
