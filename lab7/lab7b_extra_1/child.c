/*
 * child.c
 *
 *  Created on: Mar 6, 2014
 *  Author: quangng
 *  Description: This program write characters passed as parameter from command line.
 *  The number of write is also passed as parameter from command line
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


//1: file descriptor
//2: data 
//3: number of writes

int main(int argc, char **argv) {
	int fd;
	int i;
	int nWrites;

	fd = atoi(argv[1]);
	nWrites = atoi(argv[3]);
	for (i = 0; i < nWrites; i++) {
		write(fd, argv[2], 4);
	}

	close(fd);

	return EXIT_SUCCESS;
}

