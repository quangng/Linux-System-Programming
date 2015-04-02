/*
 ============================================================================
 Name        : lab3a_phase1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Excercise 3 (Handling files, file descriptors and flags, 1,0p)
Part A

Phase 1 (Concurrent writing data to the file; consequent writes)

Use system calls in file handling (not standard i/o-library functions) in phases 1, 2 and 3.

Write two programs that both write data to the same file exlog.txt. (You can imagine, that
the file is a kind of log file.)

You can make the both programs to open the file in similar way using O_CREAT flag in
open system call. This is so because if the file already happens to exist, open just opens the
existing file normally. Always remember, before the next test run, to delete the file
exlog.txt. Both programs enter a loop after opening the file. The first program writes four
characters "AAAA" to the file in the loop. The second program writes four characters
"BBBB" to the file in the loop. The loop is repeated 1 000 000 times.

Run these programs concurrently. Start them using the command ...$ progr1.exe&
progr2.exe [Enter].

Use command ...$ ls -l exlog.txt to see the size of the file. If all data were written, the size
should be 8 000 0000 bytes. What is the situation?

Remark. The starting procedure described above guarantees that programs are started
almost "simultaneously" and they are run concurrently. But you never can be sure which
one is started first. That's why it is recommended that you use O_CREAT flag in both
programs. To get correct results from the test runs remember always to delete the file
exlog.txt before the next run.

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define REPEATED_TIME 1000000

/*
int main(void) {
	int fd = -1;
	int i = 0;

	fd = open("/tmp/exlog.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		for (i = 0; i < REPEATED_TIME; i++)
			write(fd, "AAAA", 4);
	} else
		perror("Open error:");

	return EXIT_SUCCESS;
}
*/

int main(void) {
	int fd = -1;
	int i = 0;

	fd = open("/tmp/exlog.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		for (i = 0; i < REPEATED_TIME; i++)
			write(fd, "BBBB", 4);
	} else
		perror("Open error:");

	return EXIT_SUCCESS;
}
