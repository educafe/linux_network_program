

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int server_sfd, client_sfd;
	
	if(argc < 2){
		fprintf(stderr, "Usage : %s <port-number>\n", argv[0]);
		exit(1);
	}
	
    printf("Creating socket...\n");
    
    server_sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sfd == -1) {
        perror("socket failed");
        return 1;
    }
	
	printf("LISTEN_SOCKET = %d\n", server_sfd);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(atoi(argv[1]));

  printf("Binding socket to local address...\n");
  if (bind(server_sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
    perror("bind failed");
    return 1;
  }
  
  printf("Listening...\n");
  if (listen(server_sfd, 10) < 0) {
    perror("listen failed");
    return 1;
  }

  fd_set master;
  FD_ZERO(&master);
  FD_SET(server_sfd, &master);
  int max_socket = server_sfd;
	
	// printf("FD_ARRAY = %#lx\n", master.__fds_bits[0]);

  printf("Waiting for connections...\n");


  while(1) {
    fd_set reads;
    reads = master;
    if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
      perror("select failed");
      return 1;
    }

    for(int i=1; i <= max_socket; ++i) {
      if (FD_ISSET(i, &reads)) {
        if (i == server_sfd) {
					int sin_size = sizeof(struct sockaddr_in);
          client_sfd = accept(server_sfd, (struct sockaddr*)&client_addr, &sin_size);
          if (client_sfd == -1) {
            perror("accept failed");
            return 1;
          }
					
					// printf("CLIENT_SOCKET = %d\n", client_sfd);
          FD_SET(client_sfd, &master);
					// printf("FD_ARRAY = %#lx\n", master.__fds_bits[0]);
          if (client_sfd > max_socket)
            max_socket = client_sfd;

				  printf("New connection from %s and port #%d\n", inet_ntoa(client_addr.sin_addr), 
									ntohs(client_addr.sin_port));

        } else {
          char buf[1024];
          int bytes_recv = recv(i, buf, 1024, 0);
          if (bytes_recv < 1) {
            FD_CLR(i, &master);
            close(i);
            continue;
          }
				 
          int j;
          for (j = 1; j <= max_socket; ++j) {
            if (FD_ISSET(j, &master)) {
                if (j == server_sfd || j == i)
                    continue;
                else
                    send(j, buf, bytes_recv, 0);
            }
          }
        }
      } //if FD_ISSET
    } //for i to max_socket
  } //while(1)

  printf("Closing listening socket...\n");
  close(server_sfd);
 
  printf("Finished.\n");
 
  return 0;
}
