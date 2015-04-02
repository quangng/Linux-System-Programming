/*
 ============================================================================
 Name        : rt_lab2_extra.c
 Author      : Vu Nguyen (1005157)
 Version     :
 Copyright   : 
 Description :

Extra, not compulsory additional exercise (Using system calls and library
calls, 0,25p)
Write a program that displays the date and time in the format usually used in Finland.
That is in the format
dd.mm.yyyy hh:mm:ss

 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[]) {
    struct tm *mt;
    time_t now;
    char ftime[25];

    setenv("TZ", "std-2", 1); //the third argument can be any non-zero number
    tzset(); //set timezone
    if ((now = time(NULL)) == (time_t)-1) {
    	perror("time now");
    	exit(EXIT_FAILURE);
    }

    if ((mt = localtime(&now)) == NULL) {
    	perror("localtime");
    	exit(EXIT_FAILURE);
    }

    strftime(ftime,sizeof(ftime),"%d.%m.%Y %H:%M:%S",mt);
    printf("%s\n", ftime);

    return(EXIT_SUCCESS);
}

