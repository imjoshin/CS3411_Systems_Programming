#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void sendint(int fd, int x) {

    int start = 0;
    int left = sizeof(int);
    x = htonl(x);
    while (left > 0) {
		int num = write(fd, &x+start, left);
		if (num < 0){
			perror("write");
			exit(1); 
		}else{
			left -= num;
			start += num;
		}
    }
}

int receiveint(int fd) {

    int x;
    int start = 0;
    int left = sizeof(int);
    while (left > 0) {
		int num = read(fd, &x+start, left);
		if (num < 0){
			perror("write");
			exit(1);
		}else if (num == 0){
			return 0;
		}else{
			left -= num;
			start += num;
		}
    }
    return ntohl(x);
}
