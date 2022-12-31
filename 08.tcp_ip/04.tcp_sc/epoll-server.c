#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 10000 

#define MAX_CON (1200)

static struct epoll_event *events;

int main(int argc, char *argv[]) {
  fd_set master;
  fd_set read_fds;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  int fdmax;
  int server_sfd;
  int newfd;
  char buf[1024];
  int nbytes;
  int addrlen;
  int yes;
  int epfd = -1;
  int res = -1;
  struct epoll_event ev;
  int i=0;
  int index = 0;
  int client_fd = -1;

  int SnumOfConnection = 0;
  time_t Sstart, Send;

  if((server_sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Server-socket() error lol!");
    exit(1);
  }

  if(setsockopt(server_sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("Server-setsockopt() error lol!");
    exit(1);
  }
	
	memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);
  // memset(&(server_addr.sin_zero), '\0', 8);
	
  if(bind(server_sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("Server-bind() error lol!");
    exit(1);
  }
  if(listen(server_sfd, 10) == -1) {
    perror("Server-listen() error lol!");
    exit(1);
  }
	
  fdmax = server_sfd; /* so far, it's this one*/

  events = calloc(MAX_CON, sizeof(struct epoll_event));
	
  if ((epfd = epoll_create(MAX_CON)) == -1) {
    perror("epoll_create");
    exit(1);
  }
	
  ev.events = EPOLLIN;
  ev.data.fd = fdmax;
	
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, fdmax, &ev) < 0) {
    perror("epoll_ctl");
    exit(1);
  }
	
  //time(&start);
  while(1) {
    res = epoll_wait(epfd, events, MAX_CON, -1);
    client_fd = events[index].data.fd;
		printf("CLIENT_FD=%d\n", client_fd);

    for (index = 0; index < MAX_CON; index++) {
      if(client_fd == server_sfd) {
        addrlen = sizeof(client_addr);
        if((newfd = accept(server_sfd, (struct sockaddr *)&client_addr, &addrlen)) == -1){
                perror("Server-accept() error lol!");
        }else{
          // printf("Server-accept() is OK...\n");
          ev.events = EPOLLIN;
          ev.data.fd = newfd;
          if (epoll_ctl(epfd, EPOLL_CTL_ADD, newfd, &ev) < 0) {
            perror("epoll_ctl");
            exit(1);
          }
        }
        break;
      }else{
        if (events[index].events & EPOLLHUP) {
          if (epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, &ev) < 0){
                  perror("epoll_ctl");
          }
          close(client_fd);
          break;
        }
        if(events[index].events & EPOLLIN) {
          if((nbytes = recv(client_fd, buf, sizeof(buf), 0)) <= 0) {
            if(nbytes == 0){
              // printf("socket %d hung up\n", client_fd);
            }else{
              printf("recv() error lol! %d", client_fd);
              perror("");
            }
					 
            if(epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, &ev) < 0) {
              perror("epoll_ctl");
            }
            close(client_fd);
          }else{
            if(send(client_fd, buf, nbytes, 0) == -1)
              perror("send() error lol!");
          }
          break;
        }
      }
    }
  }
  return 0;
}