#include <errno.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>

#define h_addr h_addr_list[0]

// prototypes
void sendint(int, int);
int receiveint(int);
void recievestring(int);

char * recS[53];

/**
 * Get pairs of ints from user, send to server, get sum back and display.
 */
int main(int argc, char* argv[]) {

    if (argc != 3) {
	fprintf(stderr, "Usage: client hostname portnumber\n");
	exit(1);
    }

    // get port number
    unsigned short port = atoi(argv[2]);

    // set up socket to networked machine
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) { perror("socket"); exit(1); }

    // create socket address (of server)
    struct sockaddr_in dest;
    memset((char *) &dest, sizeof(dest),0);
    struct hostent* hostptr = gethostbyname(argv[1]);
    if (!hostptr) { perror("lookup error"); exit(1);}
    dest.sin_family = (short) AF_INET;
    memcpy((char *)&dest.sin_addr,(char *)hostptr->h_addr,hostptr->h_length);
    dest.sin_port = htons(port);

    // connect to the server
    int cret = connect(socket_fd,(struct sockaddr*)&dest, sizeof(dest));
    if (cret < 0) { perror("connect"); exit(1);}

    // get two numbers from the user
    int a, b;


	recievestring(socket_fd);
	int score1 = receiveint(socket_fd);
	int score2 = receiveint(socket_fd);
	printf("          111111111122222222223333333333444444444455\n");
	printf("0123456789012345678901234567890123456789012345678901\n");
	printf("%s\nyour score = %d, other score = %d\nenter 2 locations: ", recS, score1, score2);
	printf("enter two numbers: ");
    while (scanf("%d %d", &a, &b) != EOF) {
		if(a < 0 || a > 51 || b < 0 || b > 51) {
			printf("invalid location!!\nenter two locations: ");
			continue;
		}
		sendint(socket_fd,a);
		sendint(socket_fd,b);
		recievestring(socket_fd);
		int score1 = receiveint(socket_fd);
		int score2 = receiveint(socket_fd);
		printf("          111111111122222222223333333333444444444455\n");
		printf("0123456789012345678901234567890123456789012345678901\n");
		printf("%s\nyour score = %d, other score = %d\n", recS, score1, score2);
		if(score1 >= 14) {
			printf("\nYou Win!\n");
			exit(0);
		} else if(score2 >= 14) {
			printf("\nYou Lost...\n");
			exit(0);
		} else if(score1 + score2 >= 26) {
			printf("\nTie!\n");
			exit(0);
		}
		printf("enter two locations: ");
    }
    close(socket_fd);




} // end of main


void recievestring(int conn_fd) {
	int left = sizeof(char) * 53;
	int start = 0;

	while(left > 0) {
		int num = read(conn_fd, (void *)recS+start, left);
		if(num < 0) { perror("write String"); exit(1); }
		else {
			left -= num;
			start += num;
		}
	}
}






















