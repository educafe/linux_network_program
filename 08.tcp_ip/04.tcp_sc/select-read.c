#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

int main(){
  int fd, bytes_read, ret;

  char buf[128];
  struct timeval tv;
  fd_set readfds;
	
  while ((fd = open("file01", O_RDONLY)) == -1){
		perror("open");
		sleep(1);
	} 
	
  FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	ret = select(fd+1, &readfds, NULL, NULL, NULL);
	if(ret == -1){
    perror("select");
    exit(1);
  }
	
  while(1){
    if(FD_ISSET(fd, &readfds)){
			memset(buf, 0, sizeof(buf));
      while((bytes_read = read(fd, buf, sizeof(buf))) > 0)
        printf("%s",buf);
    }
  }
}