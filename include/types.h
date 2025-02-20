#pragma once
#include <sys/socket.h>
typedef int file_descriptor;
typedef struct sockaddr_in *sockaddr_in_p;
typedef enum
{
  NOT_PROCESSED,
  INTERNAL_SERVER_ERROR = 500,
  CONTENT_TOO_LARGE = 413,
  REQU_HEAD_FIELDS_TOO_LARGE = 431,
  NOT_IMPLEMENTED = 501,
  NOT_FOUND = 404,
  BAD_REQUEST = 400,
  OK = 200,
} status;