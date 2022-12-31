#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
	int sockfd, bytes_recv;
	char message[BUF_SIZE];
	socklen_t sock_size;
	struct sockaddr_in to_addr, from_addr;

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
	
#ifdef CONNECTED
	if(connect(sockfd, (struct sockaddr*)&to_addr, sizeof(to_addr))==-1) {
		perror("connect() error!!");
		exit(1);
	}
#endif

	while(1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);     
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
			break;
#ifdef CONNECTED
		send(sockfd, message, strlen(message), 0);
		bytes_recv=recv(sockfd, message, BUF_SIZE, 0);
#else
		sendto(sockfd, message, strlen(message), 0, 
										(struct sockaddr*)&to_addr, sizeof(to_addr));
		sock_size=sizeof(from_addr);
		bytes_recv=recvfrom(sockfd, message, BUF_SIZE, 0, 
										(struct sockaddr*)&from_addr, &sock_size);
		message[bytes_recv]=0;
#endif
		message[bytes_recv]=0;
		printf("Message from server %s: %s", inet_ntoa(from_addr.sin_addr), message);
	}
	close(sockfd);
	return 0;
}
