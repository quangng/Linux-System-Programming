/*
 ============================================================================
 Name        : lab11_extra.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Exercise 11 (Extra, Statistical analysis of the real-time signal delivery delay, 0.25p)
Modify your program in such a way that you generate multiple signals (e.g. 10000 signals)
in random intervals (delay between signals is n×1ms, where n=1,...,32) and then
analyze from that set the mean, standard deviation, minimum and maximum delay values.

You should get output like this:
Real time clock resolution is: 0 s 1 ns
10000 signals received
Mean 13.9us, standard deviation 40.9us, min 5us, max 3064us

Note: In order to calculate square root (sqrt() function) you need to use the switch -lm
when compling with the gcc.
Test initially with the smaller value, but run the final test run with 10000 signals in order to improve the
statistical reliability.

 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>

// Macros
#define N 10000

// Global variables
struct timespec childTimeNow;
double sigDeliveryDelay[N];
int n = 0;

// Forward declaration of functions
void sigactionHandler(int signo, siginfo_t *info, void *context);
double findMaxArr(double *array, int num_elements);
double findMinArr(double *array, int num_elements);
double findAverageArr(double *array, int num_elements);
double findSDArr(double *array, int num_elements);


int main(void) {
	pid_t childPid = -1;
	int pipeArr[2];
	int retval = -1;
	struct timespec real_time_res, parentNow, childNow;
	double maxDelay = 0, minDelay = 0, averageDelay = 0, standardDeviation = 0;
	sigset_t mask, oldmask, zeromask;
	struct sigaction sigact;
	union sigval value;
	int delayPeriod = 0;
	int i = 0;
	time_t tik, tak;

	// Get current time tik
	if (time(&tik) < 0) {
		perror("time");
		exit(EXIT_FAILURE);
	}

	// Get clock resolution
	clock_getres(CLOCK_REALTIME, &real_time_res);
	printf("Real time clock resolution is: %ld s %ld ns\n", real_time_res.tv_sec, real_time_res.tv_nsec);
	// Create a pipe for parent-child communication
	if (pipe(pipeArr) < 0) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	// Install signal handler
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;	// use real-time signal handler
	sigact.sa_sigaction = sigactionHandler;
	sigaction(SIGRTMAX, &sigact, NULL);
	// Block SIGRTMAX signal to prevent race condition
	sigemptyset(&mask); sigemptyset(&zeromask);
	sigaddset(&mask, SIGRTMAX);
	sigprocmask(SIG_BLOCK, &mask, &oldmask);
	// Initialize variables to zero
	memset(&childTimeNow, 0, sizeof(struct timespec));
	memset(&parentNow, 0, sizeof(struct timespec));
	memset(&childNow, 0, sizeof(struct timespec));
	value.sival_int = 0;
	// Initialize random seed
	srand(time(NULL));

	childPid = fork();
	if (childPid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (childPid > 0) {
		close(pipeArr[0]);
		do {
			// Get current time ti
			if (clock_gettime(CLOCK_REALTIME, &parentNow) == -1) {
				perror("clock_getttime");
				exit(EXIT_FAILURE);
			}
			// Send a real time signal to child process using sigqueue
			if (sigqueue(childPid, SIGRTMAX, value) < 0) {
				perror("sigqueue");
				exit(EXIT_FAILURE);
			}
			// If real time signal is sent successfully, send time ti to child process via pipe
			if (write(pipeArr[1], &parentNow, sizeof(struct timespec)) < 0) {
				perror("write");
				exit(EXIT_FAILURE);
			}
			i++;
			// Delay for some time from 1 to 32 ms
			delayPeriod = rand() % 32 + 1;
			usleep(delayPeriod*1000);
		} while (i < N);
		close(pipeArr[1]);
		wait(NULL);
		exit(EXIT_SUCCESS);
	} else {
		sigprocmask(SIG_SETMASK, &oldmask, NULL);
		while(n < 1)
			sigsuspend(&zeromask);
		i = 0;

		while (n < N) {
			retval = read(pipeArr[0], &childNow, sizeof(struct timespec));
			if (retval < 0) {
				perror("read");
				exit(EXIT_FAILURE);
			}
			// Calculate delay
			sigDeliveryDelay[i] = (double)((childTimeNow.tv_sec - childNow.tv_sec)*1E9 + (childTimeNow.tv_nsec - childNow.tv_nsec));
			i++;
			// Wait for real time signal from parent
			sigsuspend(&zeromask);
		}
		// n = N and i = N-1, store the last delay
		retval = read(pipeArr[0], &childNow, sizeof(struct timespec));
		if (retval < 0) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		sigDeliveryDelay[i] = (double)((childTimeNow.tv_sec - childNow.tv_sec)*1E9 + (childTimeNow.tv_nsec - childNow.tv_nsec));
		
		printf("%d real-time signals received\n", n);
		
		
		// Calculate min delay
		minDelay = findMinArr(sigDeliveryDelay, N);
		printf("Min: %lf us\n", minDelay/1000);
		// Calculate max delay
		maxDelay = findMaxArr(sigDeliveryDelay, N);
		printf("Max:  %lf us\n", maxDelay/1000);
		// Calculate average delay
		averageDelay = findAverageArr(sigDeliveryDelay, N);
		printf("Mean: %lf us\n", averageDelay/1000);
		// Calculate standard deviation
		standardDeviation = findSDArr(sigDeliveryDelay, N);
		printf("SD:   %lf us\n", standardDeviation/1000);

		if (time(&tak) < 0) {
			perror("time");
			exit(EXIT_FAILURE);
		}
		printf("It took %ld seconds to finish running the program\n", tak-tik);
		exit(EXIT_SUCCESS);
	}

	return EXIT_SUCCESS;
}



void sigactionHandler(int signo, siginfo_t *info, void *context) {
	if (info->si_signo == SIGRTMAX && info->si_code == SI_QUEUE) {
		n++;
		clock_gettime(CLOCK_REALTIME, &childTimeNow);
	}
	return;
}

double findMaxArr(double *array, int num_elements) {
	int i = 0;
	double max = 0;

	for (i = 0; i < num_elements; i++) {
		if (array[i] > max) 
			max = array[i];
    }

	return max;
}

double findMinArr(double *array, int num_elements) {
	int i = 0;
	double min = array[0];

	for (i = 0; i < num_elements; i++) {
		if (array[i] < min) 
			min = array[i];
    }
	
	return min;
}

double findAverageArr(double *array, int num_elements) {
	int i = 0;
	double average = 0;

	for (i = 0; i < num_elements; i++) 
		average += array[i];
		
	average /= num_elements;

	return average;
}

double findSDArr(double *array, int num_elements) {
	int i = 0;
	double tmp = 0, tmp1 = 0, tmp2 = 0;
	double sd = 0;
	double average = 0;

	average = findAverageArr(sigDeliveryDelay, N);
	for (i = 0; i < num_elements; i++) {
		tmp1 = array[i] - average;
		tmp2 = tmp1 * tmp1;
		tmp += tmp2;
	}
	sd = sqrt(tmp / N);

	return (sd);
}

