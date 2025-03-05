/**
 * @file buf.h
 * @author Alexsander d. S. Tenne
 * @brief Contains the data structures and functions needed to
 * store the raw HTTP data.
 * @version 1.0.0
 * @date 2025-03-05
 */
#ifndef _BUF_H
#define _BUF_H
#include "config.h"
#include "global_types.h"

typedef struct
{
  char *payload;
  long size;
  size_t bytes_written;
} buffer;

typedef struct
{
  buffer head;
  buffer body;
} message;

typedef struct
{
  message recv;
  message resp;
} message_buffers;

message_buffers *setup_buffers (server_config config);
void free_bufs (message_buffers *bufs);
void allocate_bufs (message_buffers *bufs);
void clear_bufs (message_buffers *bufs);

#endif