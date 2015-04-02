/*
 ============================================================================
 Name        : lab11.c
 Author      : Vu Nguyen
 Version     : 0.1
 Copyright   : Your copyright notice
 Description :

Exercise 11 (Measuring the real-time signal delivery delay, 1p)
Write a program which measures the real-time signal delivery delay from one process to
another. This information is useful when you are designing real-time systems using
signals for event handling.

First your program should display what is the resolution of the timers so that you know
what the measurement resolution is. Then you need to create a new process (e.g. child process)
that waits (using sigsuspend) for the signals to come with a signal handler. When the
child process receives a signal, a signal handler must acquire the current timer value
(using clock_gettime). Then you read from the pipe the signal sending time.1
Therefore, after the signal, children read the sending time from the pipe and calculated the
delivery delay time. This time is shown on the screen in us.

Parent process take the current time value (clock_gettime), and generates the signal
(using sigqueue). Note that you need to use special real-time signal, e.g. SIGRTMAX.
After sending the signal, parent process sends the signal sending time to the children
using a pipe.

Note: when you are using real-time signals, you need to use the -lrt option when
compiling with the gcc.

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


struct timespec childTimeNow;
int n = 0;

void sigactionHandler(int signo, siginfo_t *info, void *context);

int main(void) {
	pid_t childPid = -1;
	int pipeArr[2];
	int retval = -1;
	struct timespec real_time_res, parentNow, childNow;
	sigset_t mask, oldmask, zeromask;
	struct sigaction sigact;
	union sigval value;

	// Get clock resolution
	clock_getres(CLOCK_REALTIME, &real_time_res);
	printf("Real time clock resolution is: %ld s %ld ns\n", real_time_res.tv_sec, real_time_res.tv_nsec);
	// Create a pipe for parent-child communication
	if (pipe(pipeArr) < 0) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	// Install signal handler
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;	// use real-time signal handler
	sigact.sa_sigaction = sigactionHandler;
	sigaction(SIGRTMAX, &sigact, NULL);
	// Block SIGRTMAX signal to prevent race condition
	sigemptyset(&mask); sigemptyset(&zeromask);
	sigaddset(&mask, SIGRTMAX);
	sigprocmask(SIG_BLOCK, &mask, &oldmask);

	// Initialize variables to zero
	memset(&childTimeNow, 0, sizeof(struct timespec));
	memset(&parentNow, 0, sizeof(struct timespec));
	memset(&childNow, 0, sizeof(struct timespec));
	value.sival_int = 0;

	// Fork a child
	childPid = fork();
	if (childPid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (childPid > 0) {
		clock_gettime(CLOCK_REALTIME, &parentNow);
		if (sigqueue(childPid, SIGRTMAX, value) < 0) {
			perror("sigqueue");
			exit(EXIT_FAILURE);
		}
		close(pipeArr[0]);
		if (write(pipeArr[1], &parentNow, sizeof(struct timespec)) < 0) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		close(pipeArr[1]);
		wait(NULL);
		printf("Child terminated\n");
		exit(EXIT_SUCCESS);
	} else {
		sigprocmask(SIG_SETMASK, &oldmask, NULL);
		while(n < 1)
			sigsuspend(&zeromask);
		retval = read(pipeArr[0], &childNow, sizeof(struct timespec));
		if (retval < 0) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		else {
			// Calculate real-time signal delivery delay and display the delay in microsecond
			printf("Real time signal delivery delay: %ld s %ld us\n",
					(childTimeNow.tv_sec - childNow.tv_sec), (childTimeNow.tv_nsec - childNow.tv_nsec)/1000);
		}
		exit(EXIT_SUCCESS);
	}

	return EXIT_SUCCESS;
}



void sigactionHandler(int signo, siginfo_t *info, void *context) {
	// Caught real time signal sent from parent process
	// Get current time
	if (info->si_signo == SIGRTMAX && info->si_code == SI_QUEUE) {
		//printf("SIGRTMAX caught\n");
		n++;
		clock_gettime(CLOCK_REALTIME, &childTimeNow);
	}
	return;
}















