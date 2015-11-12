#include <errno.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <poll.h>


char string[] = "aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz";
char sendS[]   = "....................................................";
int P1score = 0;
int P2score = 0;
int* waitB;

void play(int, int, int[]);
void sendString(int);
void sendint(int, int);
int receiveint(int);


int main() {
	waitB = (int *)malloc(sizeof(int));

	*waitB = 1;


 	// set up socket (in process)
    int socket_fd = socket (AF_INET, SOCK_STREAM, 0);

    // create destination
    struct sockaddr_in s_in;
    memset((char *) &s_in, sizeof(s_in),0); 
    s_in.sin_family = (short)AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    s_in.sin_port = htons((unsigned short)50002);

    // bind socket to a port
    int length = sizeof(s_in);
    int bret = bind(socket_fd, (struct sockaddr*)&s_in, length);
    if (bret < 0) { perror("bind"); exit(1); }

    // print port number
    getsockname(socket_fd, (struct sockaddr*)&s_in, &length);           //remove
    printf("port number %d assigned\n", ntohs(s_in.sin_port));

    // listen
    int lret = listen(socket_fd, 1);
    if (lret < 0) { perror("listen"); exit(1); }

	//accept 2 players
	struct sockaddr_in s_out;
	length = sizeof(s_out);
	int conn_fd1 = accept(socket_fd, (struct sockaddr*) &s_out, &length);
	if(conn_fd1 <0) { perror("accept"); exit(1); }

	int conn_fd2 = accept(socket_fd, (struct sockaddr*) &s_out, &length);
	if(conn_fd2 <0) { perror("accept"); exit(1); }


	//pipe to get scores and new new string
	int CtoP[2];
	int PtoC[2];
	pipe(CtoP);
	pipe(PtoC);

	int cpid;
	if((cpid = fork()) == 0) {
		close(CtoP[0]);
		close(PtoC[1]);
		int pipefd[2] = {PtoC[0], CtoP[1]};
		play(conn_fd1, 1, pipefd);
		

		close(CtoP[1]);	
		close(PtoC[0]);
		close(socket_fd);
		close(conn_fd1);
	} else {
		//parent
		close(PtoC[0]);
		close(CtoP[1]);
		int pipefd[2] = {CtoP[0], PtoC[1]};
		play(conn_fd2, 2, pipefd);

		close(conn_fd2);
		close(socket_fd);
		//clean up children
		int status, wret;
		while((wret = waitpid(-1,&status,WNOHANG)) != 0);
	}

}


void play(int conn_fd, int player, int pipefd[]) {
	int first;
	int second;
	bool done;

	sendString(conn_fd);
	sendint(conn_fd, 0);
	sendint(conn_fd, 0);

	while(P1score < 14 && P2score < 14 && (P1score + P2score) < 26) {
		first = receiveint(conn_fd);
		second = receiveint(conn_fd);

		while (poll(&(struct pollfd){ .fd = pipefd[0], .events = POLLIN }, 1, 0)==1) {
    		int err = read(pipefd[0], &sendS, sizeof(char) * 53);
			if(err <= 0) { perror("pipe read"); exit(1); }

			 err = read(pipefd[0], &P1score, sizeof(int));
			if(err <= 0) { perror("pipe read"); exit(1); }

			 err = read(pipefd[0], &P2score, sizeof(int));
			if(err <= 0) { perror("pipe read"); exit(1); }
		}

		if(string[first] == string[second] && sendS[first] != ' ' && first != second) {
			if(player == 1)
				P1score++;
			else
				P2score++;

			sendS[first] = ' ';
			sendS[second] = ' ';
		} else if (sendS[first] == '.') {
			sendS[first]  = string[first];
			sendS[second] = string[second];
		}
		
    	int err = write(pipefd[1], &sendS, sizeof(char) * 53);
		if(err <= 0) { perror("pipe read"); exit(1); }

		 err = write(pipefd[1], &P1score, sizeof(int));
		if(err <= 0) { perror("pipe read"); exit(1); }

		 err = write(pipefd[1], &P2score, sizeof(int));
		if(err <= 0) { perror("pipe read"); exit(1); }


		sendString(conn_fd);
		if(player == 1) {
			sendint(conn_fd, P1score);
			sendint(conn_fd, P2score);
		} else {
			sendint(conn_fd, P2score);
			sendint(conn_fd, P1score);
		}
	}
}


void sendString(int conn_fd) {
	int left = sizeof(char) * 53;
	int start = 0;

	while(left > 0) {
		int num = write(conn_fd, (void *)sendS+start, left);
		if(num < 0) { perror("write String"); exit(1); }
		else {
			left -= num;
			start += num;
		}
	}
}













