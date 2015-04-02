/*
 ============================================================================
 Name        : lab7a_2.c
 Author      : Vu Nguyen
 Version     : v0.1
 Copyright   : GPLv2
 Description :
 Date		 : 27.02.2014

Phase 2. (The new program receives the alarm)
Show yourself that the alarm is inherited by the new program created by exec. You can do
this by
1. Requesting alarm in the child (to be delivered after 5 sec)
2. Starting a new program in the child (exec)
3. Installing signal handler in the new program. Signal handler only displays "Signal
received".
4. Observe that the signal is delivered to the new program in the child.

 ============================================================================
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


int main(void) {
	pid_t childPid;

    childPid = fork();
    if (childPid < 0) {
    	perror("fork error");
    	exit(EXIT_FAILURE);
    }
    if (childPid == 0) {
    	printf("this is child process\n");
    	alarm(5);
    	printf("Alarm is set to 5 seconds and is ticking now\n");
    	execl("./child.exe", "child.exe", NULL);
    	perror("execl"); //error checking since execXY does not return
    	exit(EXIT_FAILURE);
    }

    while (1) {                        /* loop forever and wait    */
    	pause();                      /* for Ctrl-C to come       */
    }

    return EXIT_SUCCESS;
}
