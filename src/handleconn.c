#include "types.h"
#include "mem.h"
#include "header.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "debug.h"
#include <unistd.h>

void handle_connection(file_descriptor server, file_descriptor connection, message_buffers *bufs)
{
    int status = 0;
    ssize_t bytes_read = 0;
    char *header_end = NULL;
    close(server);
    allocate_bufs(bufs);
    bytes_read = recv(connection, bufs->recv.head.payload, bufs->recv.head.size - 1, 0);

    if (bytes_read == -1)
    {
        free_bufs(bufs);
        exit_error("recv error");
    }
    header_end = strstr(bufs->recv.head.payload, "\r\n\r\n");
    if (header_end == NULL)
    {
        free_bufs(bufs);
        exit_error("request header size %ld exceeded", bufs->recv.head.size);
    }
    char *body_start = header_end + 4;
    size_t body_len = 0;
    for (char *ptr = body_start; *ptr; ++ptr)
        ++body_len;
    memmove(bufs->recv.body.payload, body_start, body_len);
    *body_start = '\0';
    header_data *data = parse(bufs->recv.head.payload);
    if (data == NULL)
        status = 400;
    size_t content_length = 0;
    for (header temp = data->header_list.head; temp != NULL; temp = temp->next)
    {
        if (!strcmp("Content-Length", temp->key))
        {
            content_length = strtoul(temp->value, NULL, 10);
            break;
        }
    }
    if (content_length > bufs->recv.body.size - 1)
    {
        status = 400;
    }
    else if (content_length == 0)
    {
        free(bufs->recv.body.payload);
        bufs->recv.body.payload = NULL;
    }
    else if (content_length < body_len)
        bufs->recv.body.payload[content_length] = '\0';
    else if (content_length > body_len)
    {
        bytes_read = recv(connection, (bufs->recv.body.payload + body_len), content_length - body_len, 0);
        if (bytes_read == -1)
        {
            free_bufs(bufs);
            exit_error("recv error with body");
        }
    }
    print_data(data);
    const char *http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, World!";
    send(connection, http_response, strlen(http_response), 0);
    free_data(data);
    free_bufs(bufs);
    close(connection);
    exit(0);
}