/**
 * @file state.h
 * @author Alexsander d. S. Tenne
 * @brief Small header file containing the data structures needed to track the
 * state of an established HTTP-connection.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _STATE_H
#define _STATE_H

#include <stdbool.h>
#include <sys/types.h>

/**
 * @brief Represents the status of an HTTP connection.
 */
typedef enum
{
  CLOSE = -2,                       /**< Connection should be closed. */
  NOT_PROCESSED = -1,               /**< Request has not been processed. */
  OK = 200,                         /**< Request was successful. */
  BAD_REQUEST = 400,                /**< Bad request. */
  NOT_FOUND = 404,                  /**< Resource not found. */
  CONTENT_TOO_LARGE = 413,          /**< Payload too large. */
  REQU_HEAD_FIELDS_TOO_LARGE = 431, /**< Request header fields too large. */
  INTERNAL_SERVER_ERROR = 500,      /**< Internal server error. */
  NOT_IMPLEMENTED = 501,            /**< Not implemented. */
  INSUFFICIENT_STORAGE = 507,       /**< Insufficient storage. */
} status;

/**
 * @brief Represents the state of an established HTTP connection.
 */
typedef struct
{
  struct timeval timeout; /**< Timeout for the connection. */
  bool keep_alive;  /**< Indicates if the connection should be kept alive. */
  int max_requests; /**< Maximum number of requests allowed. */
  status code;      /**< Status code of the connection. */
  int current_request; /**< Current request number. */
} connection_state;

#endif