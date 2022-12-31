#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>

#define MAXBUF 256

int main() {
  char buffer[MAXBUF];
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int addrlen, n, i;
  int sockfd;
	struct pollfd pollfds[5];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sockfd,(struct sockaddr*)&addr ,sizeof(addr));
  listen (sockfd, 5);

  for (i=0; i<5; i++) {
    memset(&client, 0, sizeof (client));
    addrlen = sizeof(client);
    pollfds[i].fd = accept(sockfd,(struct sockaddr*)&client, &addrlen);
    pollfds[i].events = POLLIN;
  }
	
  sleep(1);
	
  while(1) {
  	puts("round again");
		poll(pollfds, 5, 5000);
		for(i=0; i<5; i++) {
			if (pollfds[i].revents & POLLIN){
				pollfds[i].revents = 0;
				memset(buffer, 0, MAXBUF);
				read(pollfds[i].fd, buffer, MAXBUF);
				puts(buffer);
			}
		}
  }
  return 0;
}