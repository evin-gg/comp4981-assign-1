#include "../include/setup.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 8080
#define MAXCON 5

int main(void) {
  int serverfd;
  int clientfd;
  socklen_t host_addrlen;
  struct sockaddr_in host_addr;

  setup_socket(&serverfd);
  setup_address(&host_addr, &host_addrlen, PORT);

  if (bind(serverfd, (struct sockaddr *)&host_addr, host_addrlen) != 0) {
    perror("Bind");
    close(serverfd);
    return EXIT_FAILURE;
  }

  if (listen(serverfd, MAXCON) != 0) {
    perror("Listen");
    close(serverfd);
    return EXIT_FAILURE;
  }

  // while(1) {
  //   int clientfd = accept(serverfd, (struct sockaddr *)&host_addr, host_addrlen);
  //   if()
  // }

  printf("Success??\n");
  close(serverfd);
  return 0;
}
