#include "handleconn.h"
#include "globals.h"
#include "header.h"
#include "mem.h"
#include "request.h"
#include "response.h"
#include <unistd.h>

void
handle_connection (message_buffers *bufs)
{
  close (server);
  status code = NOT_PROCESSED;
  header_data *request_data = request (bufs, &code);
  // response (bufs, request_data, code);
  free_data (request_data);
}