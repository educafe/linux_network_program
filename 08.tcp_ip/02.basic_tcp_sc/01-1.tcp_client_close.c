#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
	This example requites to work with 02-1.tcp_server_close.c to see if what happens
	when server close the socket while waiting for recv().
	To test it, server keeps hiting the "Enter" until the socket is closed
*/

int main(int argc, char* argv[]) {
	int sockfd;
	struct sockaddr_in sockaddr;
	char message[500];
	int bytes_recv;
	
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sockfd=socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family=AF_INET;
	sockaddr.sin_addr.s_addr=inet_addr(argv[1]);
	sockaddr.sin_port=htons(atoi(argv[2]));
		
	if(connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr))==-1) {
		perror("connect() error!!");
		exit(1);
	}
	for(int i=0; i<5; i++) {
		bzero(&message, sizeof(message));
		bytes_recv=recv(sockfd, message, sizeof(message), 0);
	//bytes_recv=read(sockfd, message, sizeof(message));
		if(bytes_recv==-1)	{
			perror("recv() error!!");
			exit(1);
	}
	if(bytes_recv == 0){
		printf("socket is closed by the other end!!!\n");
		break;
	}
		printf("Message from server: %s \n", message); 
	}
	
	close(sockfd);
	return 0;
}

/*
* while waiting for new message using recv, client may be able to detect 
* the lost of connection when a server close the socket.
*/
