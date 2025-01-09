#include "../include/setup.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
  
  int serverfd;
  int clientfd;
  socklen_t host_addrlen;
  struct sockaddr_in host_addr;
  char buffer[BUFFER_SIZE];

  setup_socket(&serverfd);
  setup_address(&host_addr, &host_addrlen, PORT);

  if (bind(serverfd, (struct sockaddr *)&host_addr, host_addrlen) != 0) {
    perror("Bind");
    close(serverfd);
    return EXIT_FAILURE;
  }

  if (listen(serverfd, SOMAXCONN) != 0) {
    perror("Listen");
    close(serverfd);
    return EXIT_FAILURE;
  }

  while(1) {
    int clientfd = accept(serverfd, (struct sockaddr *)&host_addr, &host_addrlen);
    if(clientfd < 0) {
      perror("Client File Descriptor");
      close(clientfd);
      continue;
    }
    
    int valread = read(clientfd, buffer, BUFFER_SIZE);
    printf("%s", buffer);

    write(clientfd, "hi", 3);
    close(clientfd);
  }

  printf("Success??\n");
  close(serverfd);
  return 0;
}
