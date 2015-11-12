// Author: Josh Johnson
#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char* argv[]){
	int numChildren = atoi(argv[1]);
	char* string = argv[2];
	int i;
	char ch;
	char* strI = malloc(sizeof(char) * 3);
	pid_t forkID;
	pid_t* childID = malloc(sizeof(pid_t) * numChildren);
	char* argvs[] = {"link", "string", "strI", NULL};//{"link", string, strI, NULL};
	int readPrev;
	int firstLastPipe[2]; //represents the pipe between the first and last children
	int curPipe[2]; //represents the pipe between the current child and the next

	if(childID == NULL || strI == NULL){
		perror("Not enough memory.");
		exit(1);
	}
	//printf("Passed string: %s\n", string);
	argvs[1] = string;
	argvs[2] = strI;

	//printf("Parent pid: %d\n", getpid());
	pipe(firstLastPipe);

	for(i = 0; i < numChildren; i++){

		if(i != 0)
			readPrev = curPipe[0];

		pipe(curPipe);
		forkID = fork();

		//FORK ERROR
		if(forkID < 0){
			perror("parent: Can't fork");
			exit(1);

		//CHILD
		}else if (forkID == 0){
			snprintf(strI, 3, "%d", i);
			argvs[2] = strI;
			
			//FIRST CHILD
			if(i == 0){
				close(curPipe[0]); //close read for first child, as it is not needed

				//read
				close(0);
				dup(firstLastPipe[0]);
				close(firstLastPipe[0]);

				//write
				close(1);
				dup(curPipe[1]);
				close(curPipe[1]);

			//LAST CHILD
			}else if(i == numChildren - 1){
				close(curPipe[1]); //close write for last child, as it is not needed

				//read
				close(0);
				dup(readPrev);
				close(readPrev);

				//write
				close(1);
				dup(firstLastPipe[1]);
				close(firstLastPipe[1]);

			//MIDDLE CHILDREN
			}else{

				//read
				close(0);
				dup(readPrev);
				close(readPrev);

				//write
				close(1);
				dup(curPipe[1]);
				close(curPipe[1]);

			}
			
			//printf("Executing child #%d\n", i);
			execve("link", argvs, NULL);
			perror("child: Can't execve");
			exit(1);

		//PARENT
		}else{
			childID[i] = forkID;
			close(curPipe[1]);
		}
	}		

	//for(i = 0; i < numChildren; i++){
	//	printf("parent: childID[%d] = %d\n", i, childID[i]);
	//}


	printf("hit enter/return to start\n");
	//while( getchar() != '\n' );
	getchar();
	//printf("Sending SIGUSR1...\n");
	kill(childID[0], SIGUSR1);

	printf("hit enter/return to stop\n");
	getchar();

	for(i = 0; i < numChildren; i++){
		kill(childID[i], SIGTERM);
	}

	free(childID);
	free(strI);

}
