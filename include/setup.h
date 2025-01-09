#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void setup_socket(int *fd);
void setup_address(struct sockaddr_in *addr, socklen_t *addrlen,
                   in_port_t port);
