/*
 ============================================================================
 Name        : lab7b_extra_1.c
 Author      : Vu Nguyen
 Version     : v0.1
 Copyright   : GPLv2
 Description :
 Date		 : 27.02.2014

Exercise 7b (Extra, Child processes run their own program
files and signals are used to inform the parent, 0.25p)
Use the file excer7B_starting_point.c as a starting point. It helps you to understand the
problem description.

Phase 1
Write a program that creates five children. All children start to run a separate program
file, which is the same for all children. The parent process waits for the children, but
waiting is now based on the signal SIGCHLD so that signal handler increments the global
variable n that indicates how many children have terminated. The parent remains in the
waiting loop as long as the n is less 5. There is a pause in the loop to save processor power.
Each time pause returns (signal is delivered, child has terminated), the parent displays a
message “Child terminated x”, where x is the current value on n. When all children are
terminated, the parent comes out from the loop and displays the contents of the file that
children have written. The parent itself terminates after that.

Remark. Use for loop to fork five children (instead of repeating the code).

The children write n times the string xxxx to the file. The number of writes n and the string
xxxx can be different for each child, because they are passed as parameters for each child.
In the parent program use string array to store strings. Initialize the string array with the
strings “AAAA”, “BBBB”, etc. The file descriptor is also passed as a parameter.

Parent process opens the file before the fork using system call open, so that children
inherit the opened file descriptor and can use it without opening. It is necessary to pass the
file descriptor number to the new program. This is done using command line parameters.
Do not use library functions for file operations, please!

In the test run use the value 5 for parameter n so that the first child writes text “AAAA” 5
times to the file. The second child writes text lines “BBBB” 5 times to the file, and so on.

Run the program as many times that you see two different problems that can happen.
What are they? What are the reasons for these problems?

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// Number of children
#define N 5

// Global variable that indicates how many children have terminated
int n = 0;

// Write signal handler for signal SIGCHLD here.
// Signal handler increments n.
static void SIGCHLDSignalHandler (int sig) {
	//signal(sig, SIG_IGN);					// Prevent another signal from interrupting while signal handler is executing
	n++;
	//signal(SIGCHLD, SIGCHLDSignalHandler);	// Reinstall the signal handler
}

int main(void)
{
	int fd = -1;
	char fd_str;
	int childPid = -1;
	int i = 0;
	char *str[] = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE"};
	char chr;

    // Install signal handler
    // Open the file data.txt
	signal(SIGCHLD, SIGCHLDSignalHandler);
	fd = open("/tmp/data.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	sprintf(&fd_str, "%d", fd);


    // Create N children that start to run a separate
    // executable file child.exe. Pass the file descriptor,
    // the number of writes and the string that the child
    // program writes to the file.
	for (i = 0; i < N; i++) {
		childPid = fork();
		if (childPid < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (childPid == 0) {
			execl("./child.exe", "child.exe", &fd_str, str[i], "5", NULL);
			perror("execl");		//error checking since execl never returns
			exit(EXIT_FAILURE);
		}
	}
    // Parent process waits for the children in the following loop.
    // Signal causes pause to return
    while (n < N) {
        pause();
        printf("Child term %d\n", n);
    }
    // Parent process displays the contents from the file
	printf("Displaying contents of file /tmp/data.txt\n");
	lseek(fd, 0L, SEEK_SET);
	while (read(fd, &chr, 1) != 0)
		write(STDOUT_FILENO, &chr, 1);

	printf("\n");
	exit(EXIT_SUCCESS);


    return 0;
}

