#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

/*
	This example is to demonstrate how to detect lost of the socket connection 
	by using signal SIGPIPE which will be sent to the application when send() is failed 
	when socket is closed by the client
*/

int server_sfd;
int client_sfd;

void handler(int signo) {
	int ret;
	int status;
	
	printf("Server detected the connection lost by receiving signal(%d)\n", signo);
	close(client_sfd);	
	close(server_sfd);	
	printf("Sockets closed -----\n");
	
}

int main(int argc, char *argv[]) {
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t sock_size;

	char message[]="Welcome to Linux Network Programming!";

	int yes = 1;
	
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	signal(SIGPIPE, handler);
	
	server_sfd=socket(PF_INET, SOCK_STREAM, 0);
	if(server_sfd == -1){
		perror("socket() error!!");
		exit(1);
	}

	if(setsockopt(server_sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		perror("setsockopt() error!!");
		exit(1);
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(atoi(argv[1]));

	if(bind(server_sfd, (struct sockaddr*) &server_addr, sizeof(server_addr))==-1 ) {
		perror("bind() error!!");
		exit(1);
	}
	
	if(listen(server_sfd, 5)==-1) {
		perror("listen() error!!");
		exit(1);
	}
	
	sock_size=sizeof(client_addr);  
	client_sfd=accept(server_sfd, (struct sockaddr*)&client_addr,&sock_size);
	printf("Connected to the client port --> %d\n", htons(client_addr.sin_port));
	if(client_sfd==-1){
		perror("accept() error!!");
		exit(1);
	}
	
	for(int i=0; i<10; i++){
		send(client_sfd, message, sizeof(message), 0);
		//write(client_sfd, message, sizeof(message));
		sleep(1);
	}
	
	return 0;
}

/*
* When server is tring to send a packet and the connection is closed by client,
* the kernel may detect it and deliver a signal of SIGPIPE to the server process. 
*/
