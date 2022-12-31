#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]){
	int sockfd, bytes_sent, bytes_recv;
	FILE *fp;
	char buf[BUF_SIZE];
	struct sockaddr_in sockaddr;
	
	if(argc!=3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	fp=fopen("receive.dat", "wb");
	sockfd=socket(PF_INET, SOCK_STREAM, 0);   

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family=AF_INET;
	sockaddr.sin_addr.s_addr=inet_addr(argv[1]);
	sockaddr.sin_port=htons(atoi(argv[2]));

	connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
	
	while((bytes_recv=read(sockfd, buf, BUF_SIZE ))!=0)
		fwrite((void*)buf, 1, bytes_recv, fp);
	
	puts("Received file data");
	fclose(fp);

	printf("Enter to send message...");
	getchar();
	bytes_sent = write(sockfd, "Thank you~~", 12);
	if(bytes_sent <= 0) perror("write error");
	else printf("write ok(bytes_sent:%d)!!\n", bytes_sent);
	
	printf("Enter to send message...");
	getchar();
	bytes_sent = write(sockfd, "Thank you!!", 12);
	if(bytes_sent <= 0) perror("write error");
	else printf("write ok(bytes_sent:%d)!!\n", bytes_sent);

	close(sockfd);
	return 0;
}
