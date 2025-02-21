#ifdef DEBUG
#include "debug.h"
#endif
#include "globals.h"
#include "header.h"
#include "mem.h"
#include "string.h"
#include "types.h"
#include <stdlib.h>

header_data *
request (message_buffers *bufs, status *code)
{
  ssize_t bytes_read = 0;
  char *header_end = NULL;
  allocate_bufs (bufs);
  bytes_read = recv (connection, bufs->recv.head.payload,
                     bufs->recv.head.size - 1, 0);

  if (bytes_read == -1)
    {
      warning ("recv error");
      *code = BAD_REQUEST;
      return NULL;
    }
  header_end = strstr (bufs->recv.head.payload, "\r\n\r\n");
  if (header_end == NULL)
    {
      warning ("request header size %ld exceeded", bufs->recv.head.size);
      *code = REQU_HEAD_FIELDS_TOO_LARGE;
      return NULL;
    }
  char *body_start = header_end + 4;
  size_t body_len = 0;
  for (char *ptr = body_start; *ptr; ++ptr)
    ++body_len;
  memmove (bufs->recv.body.payload, body_start, body_len);
  *body_start = '\0';
  header_data *data = parse (bufs->recv.head.payload);
#ifdef DEBUG
  print_data (data);
#endif
  if (data == NULL)
    {
      *code = BAD_REQUEST;
      return data;
    }
  if (!(strcmp ("GET", data->method) && strcmp ("HEAD", data->method)))
    {
      *code = NOT_IMPLEMENTED;
      return data;
    }
  size_t content_length = 0;
  for (header temp = data->header_list.head; temp != NULL; temp = temp->next)
    {
      if (!strcmp ("Content-Length", temp->key))
        {
          content_length = strtoul (temp->value, NULL, 10);
          break;
        }
    }
  if (content_length > (size_t)(bufs->recv.body.size - 1))
    {
      *code = CONTENT_TOO_LARGE;
      return data;
    }
  else if (content_length > body_len)
    {
      bytes_read = recv (connection, (bufs->recv.body.payload + body_len),
                         content_length - body_len, 0);
      if (bytes_read == -1)
        {
          *code = INTERNAL_SERVER_ERROR;
          warning ("recv error with body");
          return data;
        }
    }
  else
    {
      bufs->recv.body.payload[content_length] = '\0';
    }

  return data;
}