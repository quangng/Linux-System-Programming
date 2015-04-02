/*
 ============================================================================
 Name        : lab7b_extra_2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Phase 2.
The first problem is that at least sometimes, the parent process does not come out from the
waiting loop. Fix this problem. You are not expected to correct another problem yet, but
you need to explain what it is and what the reason for it is.

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
	while (waitpid(-1, NULL, WNOHANG) > 0) {	//while loop to make sure that all signals won't be lost
		n++;
	}
}

int main(void)
{
	int fd = -1;
	char fd_str;
	int childPid = -1;
	int i = 0;
	char *str[] = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE"};
	char chr;
	sigset_t newmask, oldmask, zeromask;

	// Set our handler, save previous information
    // Open the file data.txt
	fd = open("/tmp/data.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	sprintf(&fd_str, "%d", fd);

	signal(SIGCHLD, SIGCHLDSignalHandler);

	// Initialize the signal sets
	sigemptyset(&newmask); sigemptyset(&zeromask);

	// Add the signal to the set
	sigaddset(&newmask, SIGCHLD);

	// Block SIGCHLD and save current signal mask in set variable 'oldmask'
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

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
			perror("execl");			//error checking since execl never returns
			exit(EXIT_FAILURE);
		}
	}
	// Resume to the original signal mask
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	while (n < N) {
		sigsuspend(&zeromask);			//return when signal handler returns
		printf("Child term %d\n", n);
    }

    // Parent process displays the contents from the file
	printf("Displaying contents of file /tmp/data.txt\n");
	lseek(fd, 0L, SEEK_SET);
	while (read(fd, &chr, 1) > 0)
		write(STDOUT_FILENO, &chr, 1);

	printf("\r\n");
	close(fd);
	exit(EXIT_SUCCESS);


    return 0;
}
