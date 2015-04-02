/*
 ============================================================================
 Name        : lab13extra.c
 Author      : Vu Nguyen <quangngmetro@gmail.com>
 Version     : 0.1
 Copyright   : GPL
 Description :

Exercise 13 (Extra, more about thread synchronization, 0.25p)
Write a program which has the main thread and one sub thread. The main thread displays
continuously digits 1 and the sub thread displays continuously digits 2 to the screen in a
synchronous way so that the digits alternate in the display (they appear one after another).
The output looks as follows
1212121212...

It is not allowed to use delays or busy loops in the program. The output starts with the
digit 1 and is finished when both threads have displayed their digits 100 000 times. When
the main thread has done its output it waits for (joins to) the sub thread displays the
message that all is done and exits.

Remark. The solution must be portable, in other words, it must be based on the POSIX
standard. For that reason it is not allowed for a thread that has not locked the mutex to
unlock the mutex.

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 1000000000000000000

int flag = 1;

pthread_mutex_t mutex;
pthread_cond_t condvar;


void *tf(void *param){
	int i = 0;

	while (i < N) {
		pthread_mutex_lock(&mutex);
		if (flag == 2) {
			write(STDOUT_FILENO, "2", 1);
			i++;
			flag = 1;
			pthread_cond_signal(&condvar);
		} else {
			pthread_cond_wait(&condvar, &mutex);
		}
		pthread_mutex_unlock(&mutex);
	}
	printf("\nSubthread written %d times \"2\" to terminal\n", i);

	pthread_exit(NULL);
}


int main(void) {
	int i = 0;
	pthread_t ti_arr;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&condvar, NULL);


	if (pthread_create(&ti_arr, NULL, tf, NULL) != 0) {
		printf("Error in creating a thread\n");
		exit(EXIT_FAILURE);
	}

	while (i < N) {
		pthread_mutex_lock(&mutex);
		if (flag == 1) {
			write(STDOUT_FILENO, "1", 1);
			i++;
			flag = 2;
			pthread_cond_signal(&condvar);
		} else {
			pthread_cond_wait(&condvar, &mutex);
		}
		pthread_mutex_unlock(&mutex);
	}
	printf("\nMain thread written %d times \"1\" to terminal\n", i);

	pthread_join(ti_arr, NULL);
	printf("Done.\n");

	return EXIT_SUCCESS;
}
