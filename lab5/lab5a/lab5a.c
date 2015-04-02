/*Excercise 5a (Process creation, 1,0p)
Part A (Fan of processes)
Write a program that first finds out how many processes can be created. The program
displays the maximum number of processes.
Next the program creates 5 child processes. The first child displays character A and waits
for one second after displaying the character. This is repeated 5 times. The second child
displays character B and wait for one second and repeats this 5 times. There is no other
output from the child processes (not even newlines) so that all characters are at the same
line. Make sure that characters go immediately to the screen so that they are not left in the
buffer.
The parent process waits for all children. Always when a child has terminated the parent
display the message "A child xxxx has terminated", where xxxx is the process id of the
child process. When all children have terminated, the parent terminates.
Run the program a few times and examine the output. Find the differences in the output.*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_CHILD_PROCESS 5

pid_t pidOfChildren[N_CHILD_PROCESS];
char chr[N_CHILD_PROCESS] = {'A', 'B', 'C', 'D', 'E'};

void childTask(pid_t childpid);

int main(void) {
	pid_t pid;
	int i = 0;

	printf("Maximum number of processes can be created by user is: %d\n", sysconf(_SC_CHILD_MAX));
	
	printf("This is parent a, pid is %d\n", (int)getpid());
	while (i < N_CHILD_PROCESS) {
		pid = fork();
		i++;
		if (pid < 0) {
			perror("fork");
			exit(1);
		} else if (pid == 0) {// child b,c,d...
			i--; //set array index to 0 for the first child created
			pidOfChildren[i] = getpid(); //save pid of each child to a global array
			for (i = 0; i < 5; i++) {
				childTask(getpid());
				sleep(1);
			}
			exit(0);
		} else {
			printf("parent has created child %d with child pid of %d\n", i, (int)pid);
		}
	}
	//parent a continues here
	while((pid = waitpid(-1, NULL, 0)) > 0) {
		printf("\nA child %d has terminated.", (int)pid);
	}
	printf("\nAll children have terminated. Parent is terminating now.\n");

	return 0;
}

void childTask(pid_t childpid) {
	int i;

	setvbuf(stdout, NULL, _IONBF, 0); //set ouput buffer to non-buffer mode

	for (i = 0; i < N_CHILD_PROCESS; i++) {
		if (childpid == pidOfChildren[i])
			printf("%c", chr[i]);
	}

}


/* Run the program a few times and find that the output is not always the same. Whys is that?
 * -> This is because whether the child process or the parent process runs first is dependent
 * on the scheduler. We do not know which one will run first after fork() is called.
 */