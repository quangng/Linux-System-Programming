/*
 ============================================================================
 Name        : lab9c_fifowriter.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

void sigpipeHandler(int signo) {
	return;
}

int main(void) {
	int fifo_fd = -1;
	char chr = '0';

	// Install signal handler
	signal(SIGPIPE, sigpipeHandler);

	// Open the fifo for writing
	if ((fifo_fd = open("/tmp/lab9cfifo", O_WRONLY)) < 0) {
		perror("Opening the fifo for writing. Error");
		exit(EXIT_FAILURE);
	}
	printf("Fifo has been opened for writing \n");
	printf("Write end fd: %d\n", fifo_fd);

	do {
		if (write(fifo_fd, &chr, sizeof(chr)) == -1) {
			if (errno == EPIPE)
				break;
			else {
				perror("write");
				exit(EXIT_FAILURE);
			}
		}
		chr = chr + 1;
		if (chr == '9')
			chr = '0';
		sleep(1);
	} while (errno != EPIPE);

	printf("Need to terminate because no reader anymore\n");

	return EXIT_SUCCESS;
}

