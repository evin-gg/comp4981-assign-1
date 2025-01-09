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

  while (1) {
    ssize_t valread;
    int clientfd;
    clientfd = accept(serverfd, (struct sockaddr *)&host_addr, &host_addrlen);
    if (clientfd < 0) {
      perror("Client File Descriptor");
      continue;
    }

    valread = read(clientfd, buffer, BUFFER_SIZE);
    if (valread < 0) {
      close(clientfd);
      continue;
    }
    printf("%s", buffer);

    write(clientfd, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello\0", 52);
    close(clientfd);
  }

  printf("Success??\n");
  close(serverfd);
  return 0;
}
