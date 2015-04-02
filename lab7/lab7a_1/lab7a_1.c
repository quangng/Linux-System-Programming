/*
 ============================================================================
 Name        : lab7a_1.c
 Author      : Vu Nguyen
 Version     : v0.1
 Copyright   : GPLv2
 Description :
 Date		 : 27.02.2014

Exercise 7a (Signals, 1p)
Phase 1. (The child process does not inherit the alarm)
Show yourself first that the alarm is not inherited by the child process. You can do this by
1. Installing signal handler before the fork
2. Request alarm signal before the fork (to be delivered after 5 sec)
3. Observe that the signal is delivered only to the parent.

 ============================================================================
 */

#include <stdio.h>
#include <signal.h>	//included for signal
#include <stdlib.h>
#include <unistd.h>	//included for alarm()...


static void signalHandler(int signo);		//Why are these signal handlers global only to this file (static keyword)?
							//To protect these functions, we want them to be private in this file

int main(void) {
	pid_t childPid = -1;

	//install signal handler for SIGALRM
    if (signal(SIGALRM, signalHandler) == SIG_ERR) {
    	perror("signal");
	exit(EXIT_FAILURE);
    }


    //set alarm to deliver SIGALRM after 5 seconds
    alarm(5);
    printf("alarm set to 5 seconds and is ticking\n");

    childPid = fork();
	if (childPid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (childPid > 0) {
    	printf("This is parent, pid = %d\n", getpid());
    	signal(SIGALRM, signalHandler);
    } else {
    	printf("this is child, pid = %d\n", getpid());
    	signal(SIGALRM, signalHandler);
    }

    while (1) {
    	pause();	//Wait for Ctrl-C
    }

    return EXIT_SUCCESS;
}

void signalHandler(int signo) {
     signal(signo, SIG_IGN);		//prevent another signal from interrupting when signal handler is executing
     printf("SIGALRM delivered to process: %d\n", getpid());
     signal(SIGALRM, signalHandler);   	//reinstall the handler
}

