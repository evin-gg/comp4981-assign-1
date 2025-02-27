#include "../include/handler.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void *handle_request(void *arg)
{
    ssize_t valread;
    int     clientfd;
    int     requestedfd;

    const char *mime;

    char *method;
    char *path;
    char *protocol;

    char checkpath[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    int get;

    // not get by default
    get = 0;

    // Cast the FD from void
    clientfd = *(int *)arg;

    // Read in the request
    valread = read(clientfd, buffer, BUFFER_SIZE);
    if(valread < 0)
    {
        close(clientfd);
        exit(EXIT_FAILURE);
    }
    (void)valread;

    // parse the request from the read-in buffer
    parse_request(&method, &path, &protocol, &buffer[0]);

    // check the method
    if(strcmp("GET", method) == 0)
    {
        get = 1;
    }

    // throw 405
    if(strcmp("HEAD", method) != 0 && strcmp("GET", method) != 0)
    {
        construct_get_response405(clientfd);
        return NULL;
    }

    // make a code readable relative path
    snprintf(checkpath, sizeof(checkpath), ".%s", path);

    if(file_exists(checkpath) == -1)
    {
        // send 404 head or get accordingly
        if(get)
        {
            construct_get_response404(clientfd);
        }
        else
        {
            construct_response(clientfd, "404", NULL, "text/html", 0);
        }
        return NULL;
    }

    if(can_read_file(checkpath) == -1)
    {
        // figure out get or head
        // react accordingly (403)
        if(get)
        {
            construct_get_response403(clientfd);
        }
        else
        {
            construct_response(clientfd, "403", NULL, "text/html", 0);
        }
        return NULL;
    }

    if(open_requested_file(&requestedfd, checkpath) == -1)
    {
        // send 404 head or get accordingly
        if(get)
        {
            construct_get_response404(clientfd);
        }
        else
        {
            construct_response(clientfd, "404", NULL, "text/html", 0);
        }
        return NULL;
    }

    mime = get_mime_type(path);

    construct_get_response200(clientfd, mime, requestedfd);

    return NULL;
}

void parse_request(char **method, char **path, char **protocol, char *buffer)
{
    char *save;

    *method   = strtok_r(buffer, " ", &save);
    *path     = strtok_r(NULL, " ", &save);
    *protocol = strtok_r(NULL, " ", &save);
    if(*protocol)
    {
        trim_protocol(*protocol);
    }
}

void trim_protocol(char *protocol)
{
    for(int i = 0; protocol[i] != '\0'; i++)
    {
        if(protocol[i] == '\r' || protocol[i] == '\n')
        {
            protocol[i] = '\0';
            break;
        }
    }
}

int open_requested_file(int *fd, const char *path)
{
    *fd = open(path, O_RDONLY | O_CLOEXEC);
    if(*fd < 0)
    {
        return -1;
    }
    return 0;
}

void get_time(char *timestr, size_t size)
{
    time_t    rawtime;
    struct tm timeinfo;

    time(&rawtime);
    gmtime_r(&rawtime, &timeinfo);
    strftime(timestr, size, "%a %d %b %Y %H:%M:%S GMT", &timeinfo);
}

int can_read_file(const char *file_path)
{
    struct stat file_stat;

    if(stat(file_path, &file_stat) == 0 && (file_stat.st_mode & S_IRUSR))
    {
        return 0;
    }

    return -1;
}

int file_exists(const char *file_path)
{
    struct stat file_stat;

    if(stat(file_path, &file_stat) == 0)
    {
        return 0;
    }

    return -1;
}

const char *get_mime_type(const char *file_path)
{
    const char *ext = strrchr(file_path, '.');
    if(ext == NULL)
    {
        return "application/octet-stream";
    }

    if(strcmp(ext, ".html") == 0)
    {
        return "text/html";
    }

    if(strcmp(ext, ".css") == 0)
    {
        return "text/css";
    }

    if(strcmp(ext, ".js") == 0)
    {
        return "application/javascript";
    }

    if(strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
    {
        return "image/jpeg";
    }

    if(strcmp(ext, ".png") == 0)
    {
        return "image/png";
    }

    if(strcmp(ext, ".gif") == 0)
    {
        return "image/gif";
    }

    return "application/octet-stream";
}

void construct_response(int clientfd, const char *status, const char *body, const char *mime, size_t body_len)
{
    char response[BUFFER_SIZE];
    char time[DATE_SIZE];
    get_time(time, sizeof(time));

    snprintf(response,
             sizeof(response),
             "HTTP/1.1 %s\r\n"
             "Date: %s\r\n"
             "Server: Evin\r\n"
             "Connection: close\r\n"
             "Content-Length: %zu\r\n"
             "Content-Type: %s\r\n\r\n",
             status,
             time,
             body_len,
             mime);

    write(clientfd, response, strlen(response));

    if(body != NULL)
    {
        write(clientfd, body, body_len);
    }
}

void construct_get_response405(int clientfd)
{
    const char body[] = "<html><body><h1>405 Unknown Method</h1></body></html>";
    construct_response(clientfd, "404 Not Found", body, "text/html", strlen(body));
}

void construct_get_response404(int clientfd)
{
    const char body[] = "<html><body><h1>404 Not Found</h1></body></html>";
    construct_response(clientfd, "404 Not Found", body, "text/html", strlen(body));
}

void construct_get_response403(int clientfd)
{
    const char body[] = "<html><body><h1>403 Forbidden</h1></body></html>";
    construct_response(clientfd, "404 Forbidden", body, "text/html", strlen(body));
}

void construct_get_response200(int clientfd, const char *mime, int filefd)
{
    size_t  fileSize;
    char   *buffer;
    ssize_t bytesread;

    fileSize = find_content_length(filefd);

    buffer = (char *)malloc(sizeof(char) * fileSize);

    bytesread = read(filefd, buffer, fileSize);

    if(bytesread < 0)
    {
        construct_get_response404(clientfd);
        free(buffer);
    }
    else
    {
        construct_response(clientfd, "200 OK", buffer, mime, fileSize);
        free(buffer);
    }
}

size_t find_content_length(int fd)
{
    struct stat fileStat;

    if(fstat(fd, &fileStat) == 0)
    {
        return (size_t)fileStat.st_size;
    }
    return (size_t)-1;
}
