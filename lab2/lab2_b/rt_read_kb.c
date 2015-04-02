/*
 ============================================================================
 Name        : rt_read_kb.c
 Author      : Vu Nguyen (1005157)
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
	char keyboard_chr;
	int result = -1;

	while(1) {
		result = read(STDIN_FILENO, &keyboard_chr, 1);
		if (result != -1)
			write(STDOUT_FILENO, &keyboard_chr, 1);
	}

	return EXIT_SUCCESS;
}
