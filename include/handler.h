#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 8912
#define DATE_SIZE 29
// #define CHAR_COUNT 41

void *handle_request(void *arg);
void parse_request(char **method, char **path, char **protocol, char *buffer);
void trim_protocol(char *protocol);
int open_requested_file(int *fd, const char *path);

void construct_response(int clientfd, const char *status, const char *body, const char *mime, size_t body_len);
void construct_get_response405(int clientfd);
void construct_get_response404(int clientfd);
void construct_get_response403(int clientfd);
void construct_get_response200(int clientfd, const char *mime, int filefd);

void get_time(char * timestr, ssize_t size);
int can_read_file(const char *file_path);
int file_exists(const char *file_path);

const char* get_mime_type(const char *file_path);