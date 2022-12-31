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

void child_process(void) {
  sleep(2);
  char msg[MAXBUF];
  struct sockaddr_in sockaddr = {0};
  int bytes_sent, sockfd, num=0;
  srandom(getpid());
  /* Create socket and connect to server */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(3333);
  sockaddr.sin_addr.s_addr = inet_addr("192.168.56.101");

  connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

  printf("child {%d} connected \n", getpid());
  while(1) {
    int sl = (random() % 10 ) +  1;
    num++;
    sleep(sl);
  	sprintf (msg, "Test message %d from client %d", num, getpid());
  	bytes_sent = write(sockfd, msg, strlen(msg));	/* Send message */
  }
}

int main() {
  int i=0;;
	
  for(i=0;i<5;i++) {
  	if(fork() == 0)	{
  		child_process();
  		exit(0);
  	}
  }
  return 0;
}