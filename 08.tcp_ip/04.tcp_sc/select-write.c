#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
  int fd;
	
  fd = open("file01", O_CREAT | O_WRONLY | O_APPEND, 0666);
	// fd = open("file01", O_CREAT | O_WRONLY, 0666);

  if(fd < 0){
    perror("file open");
    exit(0);
  }
	
	fd_set readfds, writefds;
  FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 1000;
	
	while(1) {
    // FD_SET(fd, &writefds);
    FD_SET(0, &readfds);
	 
    if (select(fd+1, &readfds, &writefds, 0, &timeout) < 0) {
      perror("select failed");
      exit(1);
    }
	 
    if(FD_ISSET(0, &readfds)) {
      char buf[4096];
      if (!fgets(buf, 4096, stdin)) break;
			write(fd, buf, strlen(buf));
    }
		
 } //end while(1)

  close(fd);
  return 0;
}