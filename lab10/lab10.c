/*
 ============================================================================
 Name        : lab10.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :

Exercise 10 (Server with unix domain sockets, 1p)
Write a server that uses Unix domain sockets. The address of the server is server.sock.
Clients send a service request, which is an ascii string containing one character, one space
and integer number indicating a time interval in seconds. The request means that the
server needs to send the character ten times. Time interval between sending the characters is
the time in the request. For example, request "a 1" means server is supposed to send character 'a'
ten times in 1-second intervals

The requirements for the server are:
1. The server needs to serve all clients "simultaneously"
2. When the server has sent a character 10 times to the client and when the client has
read them, the next read must cause end of file.
3. The server is running continuously and you can start the testing program
harj_client.exe (described below) again and again.
4. No zombies are born when the server is running.

It is allowed to use whatever technique in the implementation.
Use the program harj_client.exe to test your server. This program generates four clients.
The request of the first client is a 1 (send character a 10 times in one second intervals). The
request of the second client is b 2 (send character b 10 times in two seconds intervals) etc.
Clients only write the results immediately to the screen without any spaces or newlines.
When end of file has been received, then client writes (eof). Use for example two terminal
windows and run your server in one and the test program in another.

Your server works correctly, if the output looks something like
cadbaabcadbaacbaadbacab(eof)cdbbcdbcbd(eof)ccdcd(eof)dd(eof)

and a is displayed once per second, b is displayed twice per second and so on. (eof) is
displayed in 10 seconds intervals.

Remark. Unix domain sockets are used. The principle of the server is exactly the same
even if the server were used over the network. The only difference is in the addresses, in
the definitions of the address structures and in the initialization of structure members. All
phases that are needed to create connections and to pass information are similar.


 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <sys/wait.h>


int main(void) {
	int serverSocket = -1, socketToClient = -1;
	struct sockaddr_un serverAddr;
	pid_t c1 = -1, c2 = -1;
	int r;
	char request[10];
	char chr;
	int delay = 0;
	int i = 0;

	// Step1: Create a socket for the server
	if ((serverSocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("Server: Error in creating a socket ");
		exit(EXIT_FAILURE);
	}

	// Step2: Bind the socket to the local address
	unlink("server.sock");	// Delete "server.sock" from the file system
	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path, "server.sock");
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		perror("Server: Error in binding the socket");
		exit(EXIT_FAILURE);
	}

	// Step3: Server starts listening
	if (listen (serverSocket, 5) == -1) {
		perror("Server: Error in listening a socket");
		exit(EXIT_FAILURE);
	}

	while(1) {
		// Wait for client request
		if ((socketToClient = accept(serverSocket, NULL, 0)) == -1) {
			perror("Server: Error in accepting a connection");
			exit(EXIT_FAILURE);
		}

		// New client request, server client by calling serveClient() function
		// Use double forking technique to prevent zombie problem
		c1 = fork();
		if (c1 < 0) {
			perror("Server: fork");
			exit(EXIT_FAILURE);
		}
		if (c1 == 0) {
			c2 = fork();
			if (c2 < 0) {
				perror("Server: fork");
				exit(EXIT_FAILURE);
			} else if (c2 > 0) {
				exit(EXIT_SUCCESS);
			} else {
				r = read(socketToClient, request, 5);
				if (r > 0) {
					request[r] = '\0';
					printf("Server: %s\n", request);
					printf("socket to client fd: %d\n", socketToClient);
					chr = request[0];
					delay = atoi(&request[2]);
					for (i = 0; i < 10; i++) {
						if (write(socketToClient, &chr, 1) == -1)
							perror("Server: Error in write");
						sleep(delay);
					}
					printf("Closing socket to client fd: %d\n", socketToClient);
					close(socketToClient);
				} else if (r < 0)
					perror("Server: Error in read");
				else
					printf("Server: Number of bytes read was zero %d\n", r);

				exit(EXIT_SUCCESS);
			}
		}
		close(socketToClient);	// Because file descriptor is duplicated for child process also, we close file descriptor from parent process
		wait(NULL);
	}

	return EXIT_SUCCESS;
}







