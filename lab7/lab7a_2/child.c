#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void signalHandler(int sig);

int main(void) {
	signal(SIGALRM, signalHandler);
	while(1);
	return EXIT_SUCCESS;
}

static void signalHandler(int sig) {
	printf("Signal received\n");
}