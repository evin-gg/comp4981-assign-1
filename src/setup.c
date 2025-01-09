#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

void setup_socket(int *fd) {
    *fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*fd < 0) {
        perror("Socket creation");
        exit(EXIT_FAILURE);
    }
}

void setup_address(struct sockaddr_in *addr, socklen_t *addrlen, int port) {
    memset(addr, 0, sizeof(*addr));

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = htonl(INADDR_ANY);

    *addrlen = sizeof(*addr);
}
