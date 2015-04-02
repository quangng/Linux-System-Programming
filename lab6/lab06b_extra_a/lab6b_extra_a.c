/*
 ============================================================================
 Name        : lab6b_extra_a.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Part A (Chain of processes)
Write a program that contains 6 processes. The process that is started from the command
line (the main process of the system) first creates one child process. This child process also
creates a child. The recently created child again creates a child and so on. This means that
each process in the system has one child process except the process that was created last
(this is the 5th child in the chain).

Each process that has created a child starts to wait for its child immediately after creating
it. When a child has terminated, the parent process takes the exit value of it’s child, sleeps
one second, displays the exit value, adds one to the exit value and returns this value as its
own exit value to its parent. The child that has been created lastly only sleeps for a second
and returns zero as an exit value to its parent. The whole program terminates of course
when all processes have terminated.

Hint. You need again the macro WEXITSTATUS.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>

int main(void) {
	pid_t pid1, pid2, pid3, pid4, pid5;
	int status;

	pid1 = fork();
	if (pid1 < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid1 > 0) { //this is the fourth child
		//wait for the 5th child, take exit value of the 5th child, sleep 1 second
		//display the exit value, increment this exit value by 1, exit
		int myExitStatus;

		waitpid(pid1, &status, 0);
		if (WIFEXITED(status)) {
			printf("The 1st child %d exits with exit status: %d\n",pid1, WEXITSTATUS(status));
		} else
			printf("The 1st child did not terminate with exit\n");
		myExitStatus= WEXITSTATUS(status);
		sleep(1);
		printf("All child processes have terminated. Main parent process is terminating now\n");
		exit(myExitStatus);
	}

	pid2 = fork();
	if (pid2 < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid2 > 0) { //this is the fourth child
		//wait for the 5th child, take exit value of the 5th child, sleep 1 second
		//display the exit value, increment this exit value by 1, exit
		int myExitStatus;

		waitpid(pid2, &status, 0);
		if (WIFEXITED(status)) {
			printf("The 2nd child %d exits with exit status: %d\n",pid2, WEXITSTATUS(status));
		} else
			printf("The 2nd child did not terminate with exit\n");
		myExitStatus = WEXITSTATUS(status) + 1;
		sleep(1);
		exit(myExitStatus);
	}

	pid3 = fork();
	if (pid3 < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid3 > 0) { //this is the fourth child
		//wait for the 5th child, take exit value of the 5th child, sleep 1 second
		//display the exit value, increment this exit value by 1, exit
		int myExitStatus;

		waitpid(pid3, &status, 0);
		if (WIFEXITED(status)) {
			printf("The 3rd child %d exits with exit status: %d\n",pid3, WEXITSTATUS(status));
		} else
			printf("The 3rd child did not terminate with exit\n");
		myExitStatus = WEXITSTATUS(status) + 1;
		sleep(1);
		exit(myExitStatus);
	}

	pid4 = fork();
	if (pid4 < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid4 > 0) { //this is the fourth child
		//wait for the 4th child, take exit value of the 5th child, sleep 1 second
		//display the exit value, increment this exit value by 1, exit
		int myExitStatus;

		waitpid(pid4, &status, 0);
		if (WIFEXITED(status)) {
			printf("The 4th child %d exits with exit status: %d\n",pid4, WEXITSTATUS(status));
		} else
			printf("The 4th child did not terminate with exit\n");
		myExitStatus = WEXITSTATUS(status) + 1;
		sleep(1);
		exit(myExitStatus);
	}

	pid5 = fork();
	if (pid5 < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid5 > 0) { //this is the fourth child
		//wait for the 5th child, take exit value of the 5th child, sleep 1 second
		//display the exit value, increment this exit value by 1, exit
		int myExitStatus;

		waitpid(pid5, &status, 0);
		if (WIFEXITED(status)) {
			printf("The 5th child %d exits with exit status: %d\n",pid5, WEXITSTATUS(status));
		} else
			printf("The 5th child did not terminate with exit\n");
		myExitStatus = WEXITSTATUS(status) + 1;
		sleep(1);
		exit(myExitStatus);
	}

	//here comes the code for the last child, the 5th child
	sleep(1);
	exit(0);

	return EXIT_SUCCESS;
}
