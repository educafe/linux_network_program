#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
	int sockfd;
	char message[BUF_SIZE+1];
	int bytes_recv;
	socklen_t sock_size;
	struct sockaddr_in to_addr, from_addr;
	struct sockaddr_in to_addr2;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	if((sockfd=socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&to_addr, 0, sizeof(to_addr));
	to_addr.sin_family=AF_INET;
	to_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	to_addr.sin_port=htons(atoi(argv[1]));
	
	if(bind(sockfd, (struct sockaddr*)&to_addr, sizeof(to_addr))==-1) {
		perror("bind() error!!");
		exit(1);
	}

	while(1) {
		sock_size=sizeof(from_addr);
		memset(&message, 0, sizeof(message));
		bytes_recv=recvfrom(sockfd, message, BUF_SIZE, 0, \
													(struct sockaddr*)&from_addr, &sock_size);
		printf("Received from %s : %s\n", inet_ntoa(from_addr.sin_addr), message);
		sendto(sockfd, message, bytes_recv, 0, \
													(struct sockaddr*)&from_addr, sock_size);
#ifdef MULTI	
		sendto(sockfd, message, strlen(message)+1, 0, \
													(struct sockaddr*)&to_addr2, sizeof(to_addr2));
#endif
	}	
	close(sockfd);
	return 0;
}
