/*
 ============================================================================
 Name        : lab8a_option_a.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Exercise 8a (Multiplexed i/o and asynchronous i/o, 1p)

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

Option A. (Multiplexed i/o)
Use multiplexed i/o solve the problem above.

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>
#include <string.h>
#include "MultiSensorSimulator.h"

#define BILLION	1E9

int findMaxFileDescriptor(int *fd_array, int array_size);

int main(void) {
	int retval = -1;
	fd_set rfds;
	int fdmax = 0;
	int n = 0;
	int i = 0;
	int k = 0;
	struct timespec sumOfDelays;
	struct timespec delay;
	struct timespec timeNow;
	int sensorDescriptors[10];
	Tmeas measurement;
	int eofArray[10];
	int eofCount = 0;
	int counter = 0;
	
	// Initialize struct timespec variables
	memset(&sumOfDelays, 0, sizeof(sumOfDelays));
	for (i = 0; i < 10; i++) {
		eofArray[i] = 1;
	}
	
	// Specify 5 values per sensor
	if (StartSimulator(sensorDescriptors, 5) < 0) {
		fprintf(stderr, "Error: Cannot initialize sensor simulator\n");
		exit(EXIT_FAILURE);
	}
	fdmax = findMaxFileDescriptor(sensorDescriptors, 10);
		
	do {
		FD_ZERO(&rfds);
		for (i = 0; i < 10; i++) {
			if (eofArray[i] != 0) 	// Add file descriptors that haven't reached EOF to the set 
				FD_SET(sensorDescriptors[i], &rfds);
		}
		n = select(fdmax+1, &rfds, NULL, NULL, NULL);
		if (n > 0) {
			for (i = 0; i < 10; i++) {
				if (FD_ISSET(sensorDescriptors[i], &rfds)) {
					k++;
					/*
					 *	Sensor descriptor is ready, but it is also ready on EOF
					 *	So check for EOF condition, if EOF, then don't read it
					 */
					if (eofArray[i] == 0) 	// 0 indicates EOF condition
						break;
					else {
						retval = read(sensorDescriptors[i], &measurement, sizeof(Tmeas));
						
						if (retval == 0) {
							eofArray[i] = 0;
							eofCount++;
						} else {
							counter++;
						}
						if (clock_gettime(CLOCK_REALTIME, &timeNow) == -1) {
							perror("clock gettime");
							exit(EXIT_FAILURE);
						}
						delay = diff_timespec(&measurement.moment, &timeNow);
						increment_timespec(&sumOfDelays, &delay);
					}
					
				}
				// All ready file descriptors have been processed
				// So it's not necessary for looping to the end of loop
				if (k == n)
					break;
			}
			k = 0;
		}
	} while (eofCount != 10);
	
	// Display the total delay
	printf("Sum of delay %ld s %ld ns\n", sumOfDelays.tv_sec, sumOfDelays.tv_nsec);
	printf("counter %d\n", counter);

	return EXIT_SUCCESS;
}



int findMaxFileDescriptor(int *fd_array, int array_size) {
	int max = 0;
	int i = 0;

	max = fd_array[0];
	for (i = 0; i < array_size; i++) {
		if (fd_array[i] > max)
			max = fd_array[i];
	}

	return max;
}


// file descriptor is ready on end of file.

