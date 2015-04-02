/*
 ============================================================================
 Name        : rt_lab2b.c
 Author      : Vu Nguyen (1005157)
 Version     :
 Copyright   : 
 Description : 
Excercise 2a (Chat program solution using Polling, 0,5p)

We discussed about the polling in the class in the context of simple chat program. Do it
now in practice. You can use a function OpenChatFellow that returns a file descriptor that
simulates a serial line or network connection from a chat fellow. The simulated fellow
writes characters a,b,c, ..., z one at a time in one second intervals.
Your program needs to give immediate answer to both inputs: your own keyboard input
and the input from the chat fellow.
Write a program that continuously runs in a loop. It displays always immeatedly each
character weather it comes from your keyboard or from the simulated chat fellow. The
program terminates when the user presses Q key.

Hint 1.
You can read from the keyboard using file descriptor 0 (STDIN_FILENO). You can read
from the chat fellow using the file descriptor that has been returned by the function
OpenChatFellow. Function OpenChatFellow is used in the following way;
prototype is: int OpenChatFellow();
it is called this way:
int fellow_desc;
fellow_desc = OpenChatFellow();
then you can read a character from the fellow this way:
char chr_fellow;
read(fellow_desc, &chr_fellow, 1);
it is good practice to close the descriptor at the end of the program: close(fellow_desc);
You also need to link the object file to your executable. Use the following command:
gcc ex2.c InputGenEdu.o -oex2.exe in Edunix
And copy the file InputGenWir.o or InputGenEdu.o to your working directory.
Both descriptors are initially in blocked mode, so that read blocks if data is not ready.

Hint 2.
You can change the file descriptor from the blocked mode to the unblocked mode in the
following way:
int file_flags;
file_flags = fcntl(file_descriptor, F_GETFL); // read current file flags
file_flags = file_flags | O_NONBLOCK; // add O_NONOBLOCK bit
fcntl(file_descriptor, F_SETFL, file_flags); // write new flags back

After that the read function does not block anymore. The read function call
char chr;
result = read(file_descriptor, &chr, 1);
always returns and behaves as follows: If the read operation succeeds (data is available),
the read function returns 1 ( number of bytes that were read). If data is not ready, the
function returns -1 (error) and the error number errno value is EAGAIN.

Hint 3.
Keyboard input comes through terminal driver. Terminal driver uses buffering and
echoing by default. To get immediate response to the keyboard input, set the terminal in
non canonical mode (in unbuffered mode) by entering command stty -icanon from
the terminal before you start your program.
When you run (test) you program, make sure that the keyboard input appears twice on
the screen, because the first output is echo from the terminal driver. If you see the
keyboard input only once, it means that your program does not display it at all.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
	int fellow_desc = 0;
	int file_flags;
	char fellow_chr;
	char keyboard_chr;
	int result = 0;

	fellow_desc = OpenChatFellow();
	//change fellow_desc file descriptor from blocked mode to unblocked mode
	file_flags = fcntl(fellow_desc, F_GETFL);
	file_flags = file_flags | O_NONBLOCK;
	fcntl(fellow_desc, F_SETFL, file_flags);

	//change STDOUT_FILENO file descriptor from blocked mode to unblocked mode
	file_flags = fcntl(STDOUT_FILENO, F_GETFL);
	file_flags = file_flags | O_NONBLOCK;
	fcntl(STDOUT_FILENO, F_SETFL, file_flags);


	do {
		result = read(fellow_desc, &fellow_chr, 1);
		if (result != -1)
			write(STDOUT_FILENO, &fellow_chr, 1);

		result = read(STDIN_FILENO, &keyboard_chr, 1);
		if (result != -1)
			write(STDOUT_FILENO, &keyboard_chr, 1);
	} while (keyboard_chr != 'Q');

	close(fellow_desc);
	close(STDOUT_FILENO);
	return EXIT_SUCCESS;
}


/*
 * NOTE: to test the program, use the command stty -icanon to change the keyboard driver to unbuffered mode so that we can
 * get immediate response on the terminal.
 */
