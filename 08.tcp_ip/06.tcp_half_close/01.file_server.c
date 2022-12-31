#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]){
	int server_sfd, client_sfd, bytes_recv;;
	FILE * fp;
	char buf[BUF_SIZE];
	
	struct sockaddr_in server_addr, client_addr;
	socklen_t sock_size;
	
	if(argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	fp=fopen("file_server.c", "rb"); 
	server_sfd=socket(PF_INET, SOCK_STREAM, 0);   
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(atoi(argv[1]));
	
	bind(server_sfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	listen(server_sfd, 5);
	
	sock_size=sizeof(client_addr);    
	client_sfd=accept(server_sfd, (struct sockaddr*)&client_addr, &sock_size);
	
	while(1) {
		bytes_recv=fread((void*)buf, 1, BUF_SIZE, fp);
		if(bytes_recv<BUF_SIZE){
			write(client_sfd, buf, bytes_recv);
			break;
		}
		write(client_sfd, buf, BUF_SIZE);
	}
	fclose(fp);
	
#ifdef HALF
	printf("Enter to close half socket...");
	getchar();
	shutdown(client_sfd, SHUT_WR);	
	while( bytes_recv=read(client_sfd, buf, BUF_SIZE) ){
		if(bytes_recv < 0) break;
		printf("Message from client(bytes_recv:%d): %s \n", bytes_recv, buf);
	}
#endif
	
#if 1
	printf("Enter to close socket...");
	getchar();
	close(client_sfd); 
	printf("Enter to quit...");
	getchar();
#endif
	close(server_sfd);
	return 0;
}




