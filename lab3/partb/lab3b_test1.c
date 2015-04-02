/*
 ============================================================================
 Name        : lab3b_test1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Part B. (File flags)

Write first the function print_flags that takes a file descriptor as a parameter. The function
reads file flags and finds out the access mode and the current settings of the file flags
O_NONBLOCK and O_APPEND. The function displays the findings in the following
way:

Access mode: Read/Write (or Read Only or Write Only according the situatiaon)
O_NONBLOCK: OFF (or OFF)
O_APPEND: OFF (or ON)

Write next the main function that has four sections each corresponding a certain test
phase.

Test 1.
Open the terminal using the access mode Read/Write and without any other flags.This is
done as follows
fd = open("/dev/tty", O_RDWR);

Use the print_flags function the to see the access mode and settings for flags
O_NONBLOCK and O_APPEND.

Next try to read one character from the opened terminal. Test the return value of read
function. If the read was successfull, display a message "Read succeeded. Character was
X" . If the read failed, display a message “Read failed: xyzxyzxyz”, where xyzxyzxyz tells
the error in plain text.

After reading try to write the character A to the terminal. Test the return value of write
function. If the wrwite was successfull, display a message "Write succeeded" . If the write
failed, display a message "Write failed: xyzxyzxyz", where xyzxyzxyz tells the error in
plain text.
Close the terminal.

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

	fd = open("/dev/pts/0", O_RDWR);
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
