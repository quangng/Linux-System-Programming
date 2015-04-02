/*
 ============================================================================
 Name        : lab4_extra.c
 Author      : Vu Nguyen (1005157)
 Version     :
 Copyright   :
 Description :

Extra exercise 4b. (More examination of the i/o-library, 0,25p)
Find the file /usr/include/libio.h and the definition of struct _IO_FILE in that file. FILE is
only another name for this structure. The file libio.h is a header that is included into
header file stdio.h.

Open any text file using library functions fopen and read (fread) something from it. After
that, access the FILE structure and display the following information from that structure:
1) file descriptor, 2) the size of the file, 3) the size of the buffer and 4) the buffering mode.

These thing can be found and/or calculated in the following way:
file descriptor is in the field _fileno

the size of the file is calculated from two members as follows
	_IO_read_end - _IO_read_base

the size of the buffer is from two members as follows
	_IO_buf_end - _IO_buf_base

the procedure to find out the buffering mode is as follows
There is the field _flags and mask constants
	_IO_UNBUFFERED and _IO_LINE_BUF.
1) If the bit _IO_UNBUFFERED if ON in the _flags field the buffering mode is unbuffered
2) If the bit _IO_LINE_BUF if ON the buffering mode is linebuffered
3) If both of these bits are OFF, the buffering mode is fully buffered

Display the same information about stdout.

Remark. It is important to do one i/o operation (read or write) before members
_IO_buf_end and _IO_buf_base have correct values in the FILE structure.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int getFileDescriptor(FILE *stream);
int getFileSize(FILE *stream);		//get the size of file in bytes
int getFileBufSize(FILE *stream);	//get the size of buffer in bytes
void getFileBufMode(FILE *stream);
int fileSize(FILE *stream);


int main(void) {
	const char *filename = "/tmp/file.txt";
	char c;
	FILE *fp;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("fopen() failed");
		exit(EXIT_FAILURE);
	} else {
		printf("Displaying the contents of file...\n");
		while((c = fgetc(fp)) != EOF) {
			fputc(c, stdout);
		}

		//diplay i/o information about the text file
		printf("\nFile descriptor is 	%d\n",getFileDescriptor(fp));
		printf("File size is 		%d bytes\n", getFileSize(fp));
		printf("Size of file is 	%d bytes\n", fileSize(fp));
		printf("Size of buffer is 	%d bytes\n", getFileBufSize(fp));
		getFileBufMode(fp);

		//display i/o information about stdout
		printf("\n"); //put a character to stdout
		printf("File descriptor is 	%d\n",getFileDescriptor(stdout));
		printf("Size of buffer is 	%d bytes\n", getFileBufSize(stdout));
		getFileBufMode(stdout);
	}

	fclose(fp);
	return EXIT_SUCCESS;
}



int getFileDescriptor(FILE *stream) {
	return stream->_fileno;
}

int getFileSize(FILE *stream) {
	return (int)(stream->_IO_read_end - stream->_IO_read_base);
}

int getFileBufSize(FILE *stream) {
	return (int)(stream->_IO_buf_end - stream->_IO_buf_base);
}

void getFileBufMode(FILE *stream) {

	if (stream->_flags & _IO_UNBUFFERED)
		printf("Buffering mode is unbuffered\n");
	else if (stream->_flags & _IO_LINE_BUF)
		printf("Buffering mode is line buffered\n");
	else if ((stream->_flags & _IO_UNBUFFERED) && (stream->_flags & _IO_LINE_BUF))
		printf("undefined mode\n");
	else
		printf("Buffering mode is fully buffered\n");
}

int fileSize(FILE *stream) {
    int pos;
    int end;

    pos = ftell (stream);
    fseek (stream, 0, SEEK_END);
    end = ftell (stream);
    fseek (stream, pos, SEEK_SET);

    return end;
}
