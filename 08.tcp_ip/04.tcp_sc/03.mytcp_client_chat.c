#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in sockaddr;
	
  if (argc < 3) {
      fprintf(stderr, "Usage: %s <server-ip> <port #>\n", argv[0]);
      exit(1);
  }

  printf("Creating socket...\n");
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1){
      perror("socket failed");
      exit(1);
  }
	
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(argv[1]); 
	sockaddr.sin_port = htons(atoi(argv[2]));


  printf("Connecting...\n");
  if (connect(sockfd,(struct sockaddr *)&sockaddr, sizeof(struct sockaddr)) == -1) {
		perror("connect failed");
		exit(1);
	}

  printf("Connected.\n");
  printf("Now ready to chat. Enter message.\n");

  while(1) {

    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(sockfd, &reads);
    FD_SET(0, &reads);
	 
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;
	 
    if (select(sockfd+1, &reads, 0, 0, &timeout) < 0) {
        perror("select failed");
        exit(1);
    }
	 
    if (FD_ISSET(sockfd, &reads)){
      char buf[4096];
      int bytes_received = recv(sockfd, buf, 4096, 0);
      if (bytes_received < 1) {
        printf("Connection closed by peer.\n");
        break;
      }
			printf("FROM (%d) : %.*s", ntohs(sockaddr.sin_port), bytes_received, buf);
    }
		
    if(FD_ISSET(0, &reads)) {
      char buf[4096];
      if (!fgets(buf, 4096, stdin)) break;
        // printf("Sending: %s", buf);
      int bytes_sent = send(sockfd, buf, strlen(buf), 0);
			printf("Sending: (%d bytes) %s", bytes_sent, buf);
    }
  } //end while(1)

  printf("Closing socket...\n");
  close(sockfd);

  printf("Good Bye.\n");
  return 0;
}

