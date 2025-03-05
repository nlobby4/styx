/**
 * @file state.h
 * @author Alexsander d. S. Tenne
 * @brief Small header file containing the data structures needed to track the
 * state of an established HTTP-connection.
 * @version 1.0.0
 * @date 2025-03-05
 */
#ifndef _STATE_H
#define _STATE_H

#include <stdbool.h>
#include <sys/types.h>

typedef enum
{
  CLOSE = -2,
  NOT_PROCESSED = -1,
  OK = 200,
  BAD_REQUEST = 400,
  NOT_FOUND = 404,
  CONTENT_TOO_LARGE = 413,
  REQU_HEAD_FIELDS_TOO_LARGE = 431,
  INTERNAL_SERVER_ERROR = 500,
  NOT_IMPLEMENTED = 501,
  INSUFFICIENT_STORAGE = 507,
} status;
typedef struct
{
  struct timeval timeout;
  bool keep_alive;
  int max_requests;
  status code;
  int current_request;
} connection_state;

#endif