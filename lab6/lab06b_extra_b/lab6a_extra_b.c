/*
 ============================================================================
 Name        : lab6a_extra_b.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Part B (The child process runs it's own program file)
Write a program that creates one child. The child process starts to run the program file of
its own. The parent process continues its working after it has created the child. The parent
is working in the loop as long as its child is working. In this program we simulate the
parent's work by displaying the text "Parent is working" once in the second (parent goes to
sleep for one second after displaying the text). When the child terminates, the parent firts
displays the message "Child terminated" and then the contents of the file, which the child
has written to the file. The parent itself terminates after that.

The child can write for example 5 times character set AAAAA to the file (25 characters
altogether). Put the child to sleep for one second after each write operation so that it is
easy to follow, how the whole system works.

Parent process opens the file before the fork, so that the child inherits the opened file
descriptor and can use it without opening again. It is necessary to pass the file
descriptor number to the new program. This is done using command line parameter.
Because file descriptor is integer and command line parameters are strings, the conversion
needs to be done.

Hint. Use non-blocking wait in the parent, because then it is possible to do the work at the
same time.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
	pid_t pidOfChild;
	int fd;
	char str;
	int file_flags;
	FILE *fp;

	fd = open("/tmp/file.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	sprintf(&str, "%d", fd);					// convert from integer to string
	file_flags = fcntl(fd, F_GETFL);            //
	file_flags = file_flags | FD_CLOEXEC;       // this bit is set, but child process
	fcntl(fd, F_SETFL, file_flags);             // still can open the opened file. Why???

	pidOfChild = fork();
	if (pidOfChild < 0) {
		perror("fork error");
		exit(EXIT_FAILURE);
	} else if (pidOfChild == 0) {
		execl("./child.exe", "child.exe", &str, NULL);
		perror("execl"); //error checking since execXY does not return
		exit(EXIT_FAILURE);
	} else {
		while (waitpid(pidOfChild, NULL, WNOHANG) <= 0) {
			printf("Parent is working\n");
			sleep(1);
		}
		printf("Child terminated\n");

		fp = fopen("/tmp/file.txt", "r");
		if (fp == NULL) {
			perror("Error while opening the file");
			exit(EXIT_FAILURE);
		}
		printf("Displaying contents of file /tmp/file.txt\n");
		while ((str = fgetc(fp)) != EOF)
			printf("%c", str);

		printf("\n");
		exit(EXIT_SUCCESS);
	}

	return EXIT_SUCCESS;
}
