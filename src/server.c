#include "../include/setup.h"
#include <netinet/in.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>

#define PORT 80

int main(void)
{
    socklen_t data;
    struct sockaddr_in host;
    char    host_address[INET_ADDRSTRLEN];



    find_address(&host.sin_addr.s_addr, host_address);

    find_port(&host, host_address);


    return EXIT_SUCCESS;
}