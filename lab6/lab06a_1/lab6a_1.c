/*
 ============================================================================
 Name        : lab6a_1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Exercise 6a (Chain of processes, 1p)
Part A (Avoiding zombies in the server)
Phase 1
The following program simulates a server. The function fgets reads a command (that
represents connection request from a client). If the command is "n" like next client, the
server creates a child process to serve the client. The server cannot serve the client itself,
because it may take a long time and the server needs to respond immediately to the
possible new connection requests. For the same reason, the server cannot wait for the child
(and block) either. That’s why zombies are left in the system always when a client has
been served.

Compile and run the program below. Enter n for example 5 times. Then use another
terminal and enter the command ps -u "your username" –a to see that all children
are still there as zombies, even if they have been terminated.

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>


int main (void) {
	pid_t pid;
	char buf[20];

	printf("Enter input (n = next, other input terminates):");
	fgets(buf, 20, stdin);
	while(buf[0] == 'n') {
		pid = fork();
		if (pid < 0) {
			perror("Fork:");
			exit(1);
		}
		if (pid == 0) {
			sleep(5); // This represents something real work
			exit(0);
			// that is done for the client
		}
	printf("Enter input (n = next, other input terminates):");
	fgets(buf, 20, stdin);
	}
}
 ============================================================================
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>


int main (void) {
	pid_t pid;
	char buf[20];

	printf("Enter input (n = next, other input terminates):");
	fgets(buf, 20, stdin);
	while(buf[0] == 'n') {
		pid = fork();
		if (pid < 0) {
			perror("Fork:");
			exit(1);
		}
		if (pid == 0) {
			sleep(5); // This represents something real work
			exit(0);
			// that is done for the client
		}
	printf("Enter input (n = next, other input terminates):");
	fgets(buf, 20, stdin);
	}

	return 0;
}
