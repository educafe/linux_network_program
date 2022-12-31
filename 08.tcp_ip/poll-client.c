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

void child_process(void) {
  sleep(2);
  char msg[MAXBUF];
  struct sockaddr_in addr = {0};
  int n, sockfd, num=0;
  srandom(getpid());
  /* Create socket and connect to server */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = inet_addr("192.168.56.101");

  connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

  printf("child {%d} connected \n", getpid());
  while(1) {
    int sl = (random() % 10 ) +  1;
    num++;
    sleep(sl);
  	sprintf (msg, "Test message %d from client %d", num, getpid());
  	n = write(sockfd, msg, strlen(msg));	/* Send message */
		if (n == -1 )
			break;
  }
}

int main() {
  char buffer[MAXBUF];
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int addrlen, n, i;
  int sockfd;
	
  for(i=0; i<5; i++) {
  	if(fork() == 0)	{
  		child_process();
  		exit(0);
  	}
  }
	
  return 0;
}