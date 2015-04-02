#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define REPEATED_TIME 5

int main(int argc, char **argv) {
	int fd;
	int i;

	if (argc != 2) {
		printf("error");
		exit(EXIT_FAILURE);
	}

	fd = atoi(argv[1]);
	for (i = 0; i < REPEATED_TIME; i++) {
		write(fd, "AAAAA", 5);
		sleep(1);
	}

	close(fd);

	return EXIT_SUCCESS;
}
