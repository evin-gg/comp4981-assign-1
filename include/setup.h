#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

void setup_socket(int *fd);
void setup_address(struct sockaddr_in *addr, socklen_t *addrlen, in_port_t port);
