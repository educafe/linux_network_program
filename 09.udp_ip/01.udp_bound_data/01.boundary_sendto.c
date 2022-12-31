#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHUNK_SIZE 15

int main(int argc, char *argv[]) {
	int sockfd;
	char msg1[]="Hello world!";
	char msg2[]="Good morning";
	char msg3[]="How are you";
	struct to_addr_in to_addr;
	socklen_t sock_size;
	
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	if((sockfd=socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&to_addr, 0, sizeof(to_addr));
	to_addr.sin_family=AF_INET;
	to_addr.sin_addr.s_addr=inet_addr(argv[1]);
	to_addr.sin_port=htons(atoi(argv[2]));
	
	sendto(sockfd, msg1, CHUNK_SIZE, 0, (struct to_addr*)&to_addr, sizeof(to_addr));
	sendto(sockfd, msg2, CHUNK_SIZE, 0, (struct to_addr*)&to_addr, sizeof(to_addr));
	sendto(sockfd, msg3, CHUNK_SIZE, 0, (struct to_addr*)&to_addr, sizeof(to_addr));
	
	close(sockfd);
	return 0;
}
