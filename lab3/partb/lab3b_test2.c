/*
 ============================================================================
 Name        : lab3b_test2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Test 2.
Open the terminal using the access mode Read Only and without any other flags.
Call the print_flags function again and do all the same things as in test 1.
Do not close the terminal.

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

void print_flags(int filedescriptor);

int main(void) {
	int fd = -1;
	char chr;
	char buf[2];

	fd = open("/dev/pts/0", O_RDONLY);
	if (fd == -1)
		perror("Open error");
	else
		print_flags(fd);

	//read one character from the opened terminal
	printf("Please enter one character and then hit enter\n");
	if ((read(fd, &chr, 1)) == 1) {
		buf[0] = chr;
		buf[1] = '\0';
		printf("Read succeeded. Character was %s\n", buf);
	}
	else
		perror("Read fail");

	//write character 'A' to the terminal
	if ((write(fd, "A", 1)) == 1)
		printf("\nWrite succeeded.\n");
	else
		perror("Write failed");

	close(fd);

	return EXIT_SUCCESS;
}


void print_flags(int filedescriptor) {
	int accessMode, fileFlags;

	fileFlags = fcntl(filedescriptor, F_GETFL, 0);

	//Get access mode
	accessMode = fileFlags & O_ACCMODE;
	if (accessMode == O_RDONLY)
		printf("Access mode: Read Only\n");
	else if (accessMode == O_WRONLY)
		printf("Access mode: Write Only\n");
	else
		printf("Access mode: Read/Write\n");

	//Check O_NONBLOCK flag
	if (fileFlags & O_NONBLOCK)
		printf("O_NONBLOCK: ON\n");
	else
		printf("O_NONBLOCK: OFF\n");

	//check O_APPEND flag
	if (fileFlags & O_APPEND)
		printf("O_APPEND: ON\n");
	else
		printf("O_APPEND: OFF\n");
}
