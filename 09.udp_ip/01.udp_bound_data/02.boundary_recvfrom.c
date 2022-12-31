#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 10

int main(int argc, char *argv[]){
	int sockfd, bytes_recv;
	char message[BUF_SIZE];
	struct sockaddr_in to_addr, from_addr;
	socklen_t sock_size;

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
	
	sock_size=sizeof(from_addr);
	while(1) {
		sleep(2);
		memset(&message, 0, sizeof(message));
		// bytes_recv=recvfrom(sockfd, message, BUF_SIZE, MSG_DONTWAIT, \
														(struct sockaddr*)&from_addr, &sock_size);
		bytes_recv=recvfrom(sockfd, message, BUF_SIZE, 0, \
														(struct sockaddr*)&from_addr, &sock_size);
		printf("Message : %s\n", message);
	}		
	close(sockfd);	
	return 0;
}
