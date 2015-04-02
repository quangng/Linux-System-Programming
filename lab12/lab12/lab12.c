/*
 ============================================================================
 Name        : lab12.c
 Author      : Vu Nguyen
 Version     : v0.1
 Copyright   : Your copyright notice
 Description :

Exercise 12 (Thread synchronization, 1p)
Phase 1. (Demonstrate the problem)
Write a program that has a main thread and 10 sub-threads. Each sub-thread increments
the common counter as fast as possible 1000 000 times (if there is no problem, let them
increment 10 000 0000 times). The threads also increment their own counter in the array in
the memory area of the main thread (counter array is a local variable in the main function).
The main thread waits until all sub threads have done their work. After that the main
thread displays the value of the counter, the list that tells how many times each thread has
incremented their own counters and also the value how many times the increment of
common counter has been missed. Observe how many increments have been lost. What is
the reason that some of the increments have been missed?

 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 10
#define N 1000000	// 1 million

int commonCounter = 0;

void *increment_counter(void *ptr);

int main(void) {
	int error = -1;
	int i = 0;
	pthread_t thread_ids[NTHREADS];
	int mainCounter[NTHREADS];

	// Initialize mainCounter array
	for (i = 0; i < NTHREADS; i++)
		mainCounter[i] = 0;

	// Create subthreads
	for (i = 0; i < NTHREADS; i++) {
		error = pthread_create(&thread_ids[i], NULL, increment_counter, &mainCounter[i]);
		if (error)
			fprintf(stderr, "Error: %s\n", strerror(error));
	}

	// Wait for all subthreads
	for (i = 0; i < NTHREADS; i++)
		pthread_join(thread_ids[i], NULL);

	// Display the values of common counter
	printf("Value of common counter: %d\n", commonCounter);


	// Display values of counters from main thread
	for (i = 0; i < NTHREADS; i++)
		printf("Value of main counter[i]=%d\n", mainCounter[i]);

	// Display number of losses of increments of common counter
	printf("Number of losses of increments from commonCounter: %d\n", N*NTHREADS - commonCounter);

	exit(0);

	return 0;
}


void *increment_counter(void *ptr) {
	int i = 0;

	while (i < N) {
		*(int *)ptr = *(int *)ptr + 1;
		commonCounter++;
		i++;
	}
}
