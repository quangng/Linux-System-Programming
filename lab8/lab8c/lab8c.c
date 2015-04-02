/*
 ============================================================================
 Name        : lab8c.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Exercise 8c (Extra exercise, 0.25p)
There were two problems in exercise 7B.
The first problem was that the program hung (got stuck) sometimes. You fixed that
problem by calling one of the wait functions in a loop in the signal handler. The second
problem left unresolved.
The second problem was that some outputs were missing. The reason for this was the race
condition. Sometimes the next signal was delivered before the program had reached the
pause again.

Use sigsuspend instead of pause to prevent the race condition. Now you have done
two fixes. But still the program at least sometimes misses the output. What is the reason?

What you need to do to make it work perfectly? Do the final modification and test it so
many times that you can be sure that it works in a reliable way. Is sigsuspend necessary
or useful in the final solution?

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
		printf("Child term %d\n", n);
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
	fd = open("./data.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
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
    }

    // Parent process displays the contents from the file
	printf("Displaying contents of file ./data.txt\n");
	lseek(fd, 0L, SEEK_SET);
	while (read(fd, &chr, 1) > 0)
		write(1, &chr, 1);

	printf("\n");
	close(fd);
	exit(EXIT_SUCCESS);


    return 0;
}
