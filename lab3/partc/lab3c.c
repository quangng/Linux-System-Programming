/*
 ============================================================================
 Name        : lab3c.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Part C. (Contents of st_mode member: file type and access rights)

Write a program that finds out the file type of the file that is passed as a command line
parameter and displays the file type in plain text to the screen. (You have to test only three
file types: Regular file, Directory file and Character special file (for example the file
/dev/ttyS0)). Run the program three times, so that you can see all three possible results.
 *
In addition to finding out the type of the file, the program also finds out the access rigths
of the file as one number and displays it to the screen in the familiar octal format. For
example in the format 0740, if owner has all rights, group has read access right and others
have no rights to access the file. The basic idea here is that you should mask out from the
st_mode the bit field of 12 bits that contains access rights and then display that bit field as
an octal number.

Hint. In addtion to the access right bits the member st_mode contains only the file type bit
field. There is a mask for the file type bit field (predefined constant S_IFMT). Use that
mask first to construct a new mask for the access right bits field and use it to mask out the
access right bits from the st_mode.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void findFileTypes(const char *pathname);
void findFileAccessRights(const char *pathname);

/*
 * Application
 */
int main(int argc, char *argv[]) {
	const char *pathToFile;

	if (argc != 2) {
		printf("Usage: %s path_to_filename\n", argv[0]);
		exit(EXIT_FAILURE);
	} else {
		pathToFile = argv[1];
		findFileTypes(pathToFile);
		findFileAccessRights(pathToFile);
	}

	return EXIT_SUCCESS;
}

/*
 * Implementation of operation functions
 */
void findFileTypes(const char *pathname) {
	int ret = -1;
	struct stat buff;

   	ret = stat(pathname, &buff);
   	if (ret == -1)
   		perror("stat() error");
   	else {
   		if (S_ISDIR(buff.st_mode))
   			printf("%s is a directory.\n", pathname);
   		else if (S_ISCHR(buff.st_mode))
   			printf("%s is a character special file.\n", pathname);
   		else if (S_ISREG(buff.st_mode))
   			printf("%s is a regular file.\n", pathname);
   		else
   			printf("%s belongs to other file types.\n", pathname);
   	}
}

void findFileAccessRights(const char *pathname) {
	int ret = -1;
	struct stat buff;

	ret = stat(pathname, &buff);
	if (ret == -1)
		perror("stat() error");
	else
		printf("Access right bits in octal format %o\n", buff.st_mode);
}
