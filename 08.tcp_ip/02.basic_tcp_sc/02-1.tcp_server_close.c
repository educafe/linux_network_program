#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/*
	This example may work together with 01.tcp_client.c 
	and the client is waiting for server to close the socket fibytes_sent
*/

int main(int argc, char *argv[]) {
	int server_sfd;
	int client_sfd;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t client_addr_size;
	
	int bytes_sent;

	char message[]="Welcome to Linux Network Programming!";

	int yes = 1;
	
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	server_sfd=socket(PF_INET, SOCK_STREAM, 0);
	if(server_sfd == -1) {
		perror("socket() error!!");
		exit(1);
	}

#if 1
	if(setsockopt(server_sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt() error!!");
		exit(1);
	}
#endif
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(atoi(argv[1]));

	if(bind(server_sfd, (struct sockaddr*) &server_addr, sizeof(server_addr))==-1 )	{
		perror("bind() error!!");
		exit(1);
	}
	
	if(listen(server_sfd, 5)==-1)	{
		perror("listen() error!!");
		exit(1);
	}
	
	client_addr_size=sizeof(client_addr);  
	client_sfd=accept(server_sfd, (struct sockaddr*)&client_addr,&client_addr_size);
	printf("Connected to the client port --> %d\n", htons(client_addr.sin_port));
	if(client_sfd==-1) {
		perror("accept() error!!");
		exit(1);
	}
	
	bytes_sent = send(client_sfd, message, strlen(message), 0);
	printf("bytes_sent : %d\n", bytes_sent);
	//write(client_sfd, message, sizeof(message));
	if(bytes_sent == -1){
		printf("Socket is closed by the other end!!!\n");
		close(client_sfd);	
		close(server_sfd);
	}
	
	printf("Enter to send...");
	getchar();
	
	bytes_sent = send(client_sfd, message, sizeof(message), 0);
	printf("bytes_sent1 : %d\n", bytes_sent);
	//write(client_sfd, message, sizeof(message));
	if(bytes_sent == 0){
		printf("Socket1 is closed by the other end!!!\n");
		close(client_sfd);	
		close(server_sfd);
	}

	printf("Enter to send...");
	getchar();

	bytes_sent = send(client_sfd, message, sizeof(message), 0);
	printf("bytes_sent2 : %d\n", bytes_sent);
	//send() doesn't return 0 when the socket is not available due to close
	if(bytes_sent == -1){
		printf("Socket2 is closed by the other end!!!\n");
		close(client_sfd);	
		close(server_sfd);
	}
	
#if 1
	printf("Enter to close the socket!!!");
	getchar();
#endif
	close(client_sfd);	
	close(server_sfd);
	
	return 0;
}
