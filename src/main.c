#include "../include/handler.h"
#include "../include/setup.h"
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

static void setup_signal_handler(void);
static void sigint_handler(int signum);

static volatile sig_atomic_t
    exit_flag = // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    0;

int main(void) {

  int serverfd;
  socklen_t host_addrlen;
  struct sockaddr_in host_addr;
  // char buffer[BUFFER_SIZE];
  pthread_t listenerThread;

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

  setup_signal_handler();
  while (!exit_flag) {
    // ssize_t valread;
    int clientfd;

    clientfd = accept(serverfd, (struct sockaddr *)&host_addr, &host_addrlen);
    if (clientfd < 0) {
      perror("Client File Descriptor");
      continue;
    }

    if (pthread_create(&listenerThread, NULL, handle_request,
                       (void *)&clientfd) != 0) {
      fprintf(stderr, "Error: Could not create thread");
    }
    pthread_join(listenerThread, NULL);

    // valread = read(clientfd, buffer, BUFFER_SIZE);
    // if (valread < 0) {
    //   close(clientfd);
    //   continue;
    // }
    // printf("%s", buffer);

    // write(clientfd, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello
    // World!",
    //       50);
    close(clientfd);
  }

  printf("Success??\n");
  close(serverfd);
  return 0;
}

static void setup_signal_handler(void) {
  struct sigaction sa;

  memset(&sa, 0, sizeof(sa));

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
  sa.sa_handler = sigint_handler;
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void sigint_handler(int signum) { exit_flag = 1; }

#pragma GCC diagnostic pop
