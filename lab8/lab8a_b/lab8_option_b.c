/*
 ============================================================================
 Name        : lab8a_option_b.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

 In this exercise you use a so called multi sensor simulator. Multi sensor simulator
simulates a measuring system having many sensors (in this case 10 sensors). Each sensor is
measuring a temperature in a certain point in some industrial process. Temperatures can
vary between 0 and 500. The multi sensor simulator creates the measurement values. Each
sensor sends it values to their own descriptor so that you have 10 sensor descriptors in
your program.

Each sensor makes it’s own decision, when they send a new value to the descriptor. The
delay between two values from the same sensor can vary from very short time (about a
few milliseconds) to 10 seconds. Your program needs to receive each value as fast as
possible after it has been sent from the sensor. All sensors send the same number of
values. So it can be big differences between sensors how long it takes for them to send all
values. Number of values per sensor is indicated with the parameter when StartSimulator
function is called.

When sensor has sent all values, reading the file descriptor of that sensor causes end of file
situation. The termination of the program must be based on this fact and not on the
number of values received.

To help to analyze the response times the sensors send a sending time with the value. This
means that you can verify how much time has elapsed between sending and receiving the
value. Sensors send the data to the file descriptor as a structure that is defined in the
following way:
typedef struct {
	struct timespec moment;
	int value;
} Tmeas;

Immediately after you have read this kind of structure from the sensor file descriptor you
can read the current time from the computer and calculate the time difference that
indicates the delay of your read. Use the function diff_timespec for that purpose (see
MultiSensorSimulator.h). Calculate and display the sum of all delays because it gives a
good overall figure how real time your application works. Sum of delays can be calculated
with the function increment_timespec (see MultiSensorSimulator.h).

Note: 	Struct timespec has two fields, tv_sec and tv_nsec that
		increment_timespace is used to increment the time value.

It is easy to use the multi sensor simulator. Only thing you need to do is to include the file
MultiSensorSimulator.h to your source code and like object file MultiSensorSimulatorEdu.o to
your object code, if you are working with Edunix computer. The files are given at Tuubi.

If the source file of your application were myapplication.c, then you can compile and link
it in the Edunix in the following way:
	gcc -o myapplication.exe myapplication.c MultiSensorSimulatorEdu.o -lrt

In the source code you only need to call function StartSimulator as follows:
	int sensorDescriptors[10];
	StartSimulator(sensorDescriptors, 5); // Use 5 or 10
	// Generates 5 values per descriptor
After doing that you can read measurement values from sensors. For example, you could
read one measurement value from the first sensor and display it to the screen in the
following way:
	Tmeas measurement;
	read(sensorDescriptors[0], &measurement, sizeof(Tmeas));
	printf("Measurement value was %d\n", measurement.value);

Option B. (Asynchronous i/o, "new" Posix way)
Solve the same problem using asynchronous i/o.

Remark 1. Use new style of asynchronous i/o. This means that you need to use aio_xxxx
functions.

Reference: http://www.ibm.com/developerworks/library/l-async/

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "MultiSensorSimulator.h"


// Global variables
struct aiocb aiocb[10];
Tmeas measurement;
int eofCounter = 0;
int count = 0;
struct timespec sumOfDelays;
struct timespec delay;
struct timespec timeNow;
int eofArray[10];

// Function prototypes
static void aioHandler(int sigNum);
void aioSetup(int *fd_array);

int main(void) {
	int i = 0;
	int sensorDescriptors[10];
	struct sigaction sig_act;


	memset(&sumOfDelays, 0, sizeof(sumOfDelays));
	memset(&delay, 0, sizeof(delay));
	memset(&timeNow, 0, sizeof(timeNow));
	for (i = 0; i < 10; i++)
		eofArray[i] = 1;

	// Begin sensor simulator
	if (StartSimulator(sensorDescriptors, 5) < 0) {
		fprintf(stderr, "Error: Cannot start sensor simulator\n");
		exit(EXIT_FAILURE);
	}

	// Install signal handler
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = 0;
	sig_act.sa_handler = aioHandler;
	if (sigaction( SIGIO, &sig_act, NULL) < 0) {
		fprintf(stderr, "Error: Cannot install sigaction");
		exit(EXIT_FAILURE);
	}
	// Set up asynchronous I/O
	aioSetup(sensorDescriptors);

	// Initiate I/O read
	for (i = 0; i < 10; i++)
		aio_read(&aiocb[i]);

	// Wait until all values from sensors have been read
	do {
		pause();
	} while (eofCounter != 10);

	// Display the total delay period in seconds to stdout
	printf("Sum of delay %d seconds %lf seconds\n", sumOfDelays.tv_sec, sumOfDelays.tv_nsec * 1e-9);

	return EXIT_SUCCESS;
}

void aioSetup(int *fd_array) {
	int i = 0;

	// Fill control block structure
	for (i = 0; i < 10; i++) {
		// Set up the AIO request
		memset(&aiocb, 0, sizeof(aiocb));
		aiocb[i].aio_fildes = fd_array[i];
		aiocb[i].aio_buf = &measurement;
		aiocb[i].aio_nbytes = sizeof(Tmeas);

		// Link the AIO request with the Signal Handler
		aiocb[i].aio_sigevent.sigev_notify = SIGEV_SIGNAL;
		aiocb[i].aio_sigevent.sigev_signo = SIGIO;
		aiocb[i].aio_sigevent.sigev_value.sival_ptr = &aiocb[i];
	}
}

static void aioHandler(int sigNum) {
	int i = 0;
	int r = 0;
	int n = 0;

	if (sigNum == SIGIO) {
		/* We have read something, but we wanted to read from 10 I/O channels
		 * So check which I/O returns, and check for error also
		 */
		for (i = 0; i < 10; i++) {
			if (eofArray[i] != 0) {
				// Check for error
				if ((r = aio_error(&aiocb[i])) != 0) {
					printf("%s\n", strerror(r));
					exit(EXIT_FAILURE);
				} else {
					n = aio_return(&aiocb[i]);
				}
				// Check for end-of-file
				if (n == 0) {	// EOF encountered
					eofCounter++;
					eofArray[i] = 0;
				}
				// Calculate delays
				if (clock_gettime(CLOCK_REALTIME, &timeNow) == -1) {
					perror("clock gettime");
					exit(EXIT_FAILURE);
				}
				delay = diff_timespec(&measurement.moment, &timeNow);
				increment_timespec(&sumOfDelays, &delay);
			}
		}

		// Start new I/O reads
		for (i = 0; i < 10; i++) {
			// Don't read I/O channel which has reached EOF condition
			if (eofArray[i] != 0)
				aio_read(&aiocb[i]);
		}
	}

	return;
}
