#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXBUFLEN 100

int main(void) {
	int sockfd, sock_size, bytes_recv;
	struct sockaddr_in to_addr;
	struct sockaddr_in from_addr;
	char buf[MAXBUFLEN];

	//UDP socket open
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 	{
		perror("socket");
		exit(1);
	}

	to_addr.sin_family = AF_INET;
	to_addr.sin_port = htons(10000);
	to_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(to_addr.sin_zero), '\0', 8);

	//server socket ip & port setting
	if(bind(sockfd, (struct sockaddr *)&to_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	sock_size = sizeof(struct sockaddr);
	do {
		//wait for rx data from client 
		if((bytes_recv = recvfrom(sockfd, buf, MAXBUFLEN, 0, 
													(struct sockaddr *)&from_addr, &sock_size)) == -1) {
			perror("recvfrom");
			exit(1);
		}
		printf("got packet from %s\n", inet_ntoa(from_addr.sin_addr));
		printf("packet is %d bytes long\n", bytes_recv);
		buf[bytes_recv] = '\0';
		printf("packet contains : %s", buf);
	} while(1);

	//socket close
	close(sockfd);
	return 0;
}

