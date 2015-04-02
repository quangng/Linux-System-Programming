/*
 ============================================================================
 Name        : rt_read_cf.c
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
	int fellow_desc = 0;
	int result = -1;
	char fellow_chr;

	fellow_desc = OpenChatFellow();
	while(1) {
		result = read(fellow_desc, &fellow_chr, 1);
		if (result != -1)
			write(STDOUT_FILENO, &fellow_chr, 1);
	}

	return EXIT_SUCCESS;
}
