#ifdef DEBUG
#include "debug.h"
#endif
#include "handleconn.h"
#include "header.h"
#include "mem.h"
#include "request.h"
#include "response.h"
#include "types.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
handle_connection (message_buffers *bufs)
{
  close (server);
  status code = NOT_PROCESSED;
  header_data *request_data = request (bufs, &code);
  response (bufs, request_data, code);
  free_data (request_data);
  free_bufs (bufs);
  close (connection);
  exit (0);
}