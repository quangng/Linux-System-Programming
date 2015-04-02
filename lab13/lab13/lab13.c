/*
 ============================================================================
 Name        : lab13.c
 Author      : Vu Nguyen <quangngmetro@gmail.com>
 Version     : 0.1
 Copyright   : GPL
 Description :

Exercise 13 (More about thread synchronization, 1p)

Phase 1. (Demonstrate the problem)
Download the program counter_several_threads_pohja.c from the Tuubi. In the program
five threads increment the common counter until the value becomes 5 000 000. The
original requirement was that it must be guaranteed that no thread never can increment
the counter so that it exceeds the upper limit 5 000 000. The second requirement was that
when the main thread returns from the wait loop, the counter value must be exactly the
upper limit. Neither requirement is met as you can see when you run the program. The
program also uses busy loop to wait until counter becomes, what was required. This
means wasting processor resources.

Fix the problems of this program. The both outputs ate the end of the program must be 5
000 000 and the program cannot use busy loops or delays for the synchronization.

 ============================================================================
 */

// Five threads increment the common counter. They know the upper limit that is
// 5 000 000. The main thread waits until upper limit has been reached.
// There are three problems in this program.
// 1. The main thread wastes resources because it waits in the busy loop.
// 2. Threads at least sometimes do extra steps
// 3. The main thread cannot detect the situation when counter becomes exactly
//    MAXCOUNT.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define N 5
#define MAXCOUNT 5000000

int counter = 0;  // Common counter
pthread_mutex_t mutex;
pthread_cond_t condvar;

void *tf(void *param){
	int i = 0;

	while (counter < MAXCOUNT) {
		pthread_mutex_lock(&mutex);
		if (counter == MAXCOUNT)
			pthread_cond_signal(&condvar);
		else
			counter++;
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);

}

int main(void) {
	int i;
	pthread_t ti_arr[N];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&condvar, NULL);


	for (i = 0 ; i < N ; i++) {
		if (pthread_create(ti_arr+i, NULL, tf, NULL) != 0) {
			printf("Error in creating a thread %d\n", i);
			exit (0);
		}
	}

	pthread_mutex_lock(&mutex);
	while (counter < MAXCOUNT) {
		pthread_cond_wait(&condvar, &mutex);
	}
	pthread_mutex_unlock(&mutex);

	printf("Counter is after the while loop %d\n", counter);

	for ( i = 0 ; i < N ; i++) {
		pthread_join(ti_arr[i], NULL);
	}

	printf("Counter is when all sub threads have finnished %d\n", counter);

	return 0;
}
