/*
 ============================================================================
 Name        : lab9c_fiforeader.c
 Author      : Vu Nguyen 
 Version     : 2.0
 Copyright   : Your copyright notice
 Description : FIFO reader. Read from FIFO and display what has been
 read immediately to stdout until reading character '6'
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(void) {
	int fifo_fd = -1;
	char chr;
	int retval = -1;

	// Reader creates a fifo
	if (mkfifo("/tmp/lab9cfifo", O_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP) != 0) {
		if (errno != EEXIST) {
			perror("Creating the fifo. Error");
			exit(EXIT_FAILURE);
		}
	}
	printf("FIFO has been created \n");

	// Reader opens fifo for reading
	if ((fifo_fd = open("/tmp/lab9cfifo", O_RDONLY)) < 0) {
		perror("Opening the fifo for reading. Error");
		exit(EXIT_FAILURE);
	}
	printf("FIFO has been opened for reading\n");
	printf("Read end fd: %d\n", fifo_fd);
	
	while (read(fifo_fd, &chr, 1) > 0) {
		printf("%c\n", chr);
		if (chr == '6')
			break;
	}

	return EXIT_SUCCESS;
}











