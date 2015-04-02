/*
 ============================================================================
 Name        : lab5b_phase1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
Part B (Two processes)
Phase 1 (Child process and exit status)
You have already solved the "real-time problem" in chat program in two different ways.
The first method was polling and the second was dividing the tasks in separate processes.
In the latter solution you needed to start the chat application by entering two program
names from the command line. This is not an acceptable way if we had several inputs to
respond meaning several separate programs to start.
This problem can now be solved so that we write one program and that program creates
another process (or more if we had more input to respond to).
Write once again the chat application using this method. The original process (the parent
process) creates a child process that takes care of reading the input from the chat fellow
descriptor. The parent process itself reads input from the keyboard. The application still
needs to work in real-time, so that it gives immediate answer to each input.
Add the following new features to the application:
1. The child process, in addition to sending characters from the chat fellow directly to
the screen, writes them also to the file. When end of file has been received from the
chat fellow, the child process terminates.
2. When the user enters Q or q from the keyboard the parent process stops reading the
keyboard and starts to wait for the child. When the child process has terminated,the parent process displays the contents from the file (written by the child). The
output is formatted as lines each containing 7 characters. See remark 1 below.
Remark 1. Use system calls in the file handling and do the output of the file in generic way
so that there is no in advance assumption about the size of the file. The output always
consists of lines containing 7 characters. The last line of course can have less than 7
characters.
Remark 2. Remember that the child process inherits the opened file descriptors. For this
reason, open the file only once in the parent process for writing and reading. You need to
open it before the fork Remember also that the file table entry (and thus the file offset) is
common for the parent and for the child.
Only the child process uses the chat fellow descriptor. So open it only in the child process.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void) {
	pid_t childPid = -1;
	int status;
	int fellow_desc = -1;
	int file_flags;
	int result = 0;
	char fellow_chr;
	char keyboard_chr;

	fellow_desc = OpenChatFellow();
	childPid = fork();
	if (childPid > 0) { //this is parent process
		/* read characters from the terminal
		 * if q or Q is entered, stop reading from the terminal
		 * wait for child process to terminate
		 * display "Child terminated"
		 */
		//change STDOUT_FILENO file descriptor from blocked mode to unblocked mode
		file_flags = fcntl(STDOUT_FILENO, F_GETFL);
		file_flags = file_flags | O_NONBLOCK;
		fcntl(STDOUT_FILENO, F_SETFL, file_flags);

		do {
			result = read(STDIN_FILENO, &keyboard_chr, 1);
			if (result != -1)
					write(STDOUT_FILENO, &keyboard_chr, 1);
		} while (keyboard_chr != 'Q' && keyboard_chr != 'q');

		printf("\nWait for child process to terminate.\n");
		wait(&status); //wait for child process to terminate
		printf("\nChild terminated.\n");
		close(STDOUT_FILENO);
	} else if (childPid == 0) { //this is child process
		/* read the inputs from the chat fellow
		 * display the input character to the terminal
		 * write the characters read to a file
		 * terminate if eof is encountered
		 */
		int fd = -1;

		fd = open("/tmp/file.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
		if (fd == -1 ) {
			perror("Open");
			exit(EXIT_FAILURE);
		}

		do {
			result = read(fellow_desc, &fellow_chr, 1);
			if (result != -1) {
				write(STDOUT_FILENO, &fellow_chr, 1);
				write(fd, &fellow_chr, 1);
			}
		} while (fellow_chr != 'z');

		close(fd);
		close(fellow_desc);
		exit(EXIT_SUCCESS);
	}else
		perror("fork() error");

	return EXIT_SUCCESS;
}
