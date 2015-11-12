// Author: Josh Johnson
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>


void handleSIG(){
	//fprintf(stderr, "Received SIGUSR1 in child.\n");
	int zero = 0;
	write(1, &zero, sizeof(int));
}

void handleTERM(){
	//fprintf(stderr, "Child exiting...\n");
	exit(1);
}

int main(int argc, char* argv[]){
	FILE* fp;
	char* fileName;
	int input;
	pid_t pid = getpid();

	signal(SIGUSR1, handleSIG);
	signal(SIGTERM, handleTERM);

	while(1){
		//fprintf(stderr, "Waiting for next read in child #%s...\n", argv[2]);
		read(0, &input, sizeof(int));
		input += 1;
		sleep(1);
		//printf("%d", input);
		write(1, &input, sizeof(int));
		fprintf(stderr, "%s %s: pid = %d, value = %d\n", argv[1], argv[2], pid, input);
	}

	//fprintf(stderr, "%s %s: pid = %d (%d arguments passed)\n", argv[1], argv[2], getpid(), argc);
}
