#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	char chr = '1';

	while (1) 
		write(STDOUT_FILENO, &chr, 1);
		
	return EXIT_SUCCESS;
}