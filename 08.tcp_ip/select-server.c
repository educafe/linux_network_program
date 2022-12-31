#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXBUF 256

int main() {
  char buffer[MAXBUF];
  int fds[5];
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  int sock_size, i, max=0;;
  int sockfd, commfd;
  fd_set rset;
	
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(3333);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  bind(sockfd,(struct sockaddr*)&server_addr ,sizeof(server_addr));
  listen (sockfd, 5); 

  for (i=0;i<5;i++) {
    memset(&client_addr, 0, sizeof(client_addr));
    sock_size = sizeof(client_addr);
    fds[i] = accept(sockfd,(struct sockaddr*)&client_addr, &sock_size);
    if(fds[i] > max)
    	max = fds[i];
  }
  
  while(1) {
		FD_ZERO(&rset);
  	for (i = 0; i< 5; i++ ) {
  		FD_SET(fds[i], &rset);
  	}

		puts("round again");
		select(max+1, &rset, NULL, NULL, NULL);

		for(i=0;i<5;i++) {
			if (FD_ISSET(fds[i], &rset)){
				memset(buffer, 0, MAXBUF);
				read(fds[i], buffer, MAXBUF);
				puts(buffer);
			}
		}	
  }
  return 0;
}