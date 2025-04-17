/**
 * @file buf.h
 * @author -
 * @brief Contains the data structures and functions needed to
 * store the raw HTTP data.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _BUF_H
#define _BUF_H

#include "config.h"
#include "global_types.h"

/**
 * @brief Represents a buffer for storing raw HTTP data.
 */
typedef struct
{
  char *payload;        /**< Pointer to the buffer's payload. */
  long size;            /**< Size of the buffer. */
  size_t bytes_written; /**< Number of bytes written to the buffer. */
} buffer;

/**
 * @brief Represents an HTTP message with separate head and body buffers.
 */
typedef struct
{
  buffer head; /**< Buffer for the HTTP message head. */
  buffer body; /**< Buffer for the HTTP message body. */
} message;

/**
 * @brief Represents the buffers for receiving and responding to HTTP messages.
 */
typedef struct
{
  message recv; /**< Buffer for the received HTTP message. */
  message resp; /**< Buffer for the response HTTP message. */
} message_buffers;

/**
 * @brief Setup and return the message_buffers singleton. Does not allocate,
 * only sets assigns the buffer size values.
 *
 * @note Call this inside the parent process and only once.
 *
 * @param config A pointer to the server_config instance. Cannot be NULL.
 * @return message_buffers * A pointer to the message_buffers singleton
 * instance.
 */
message_buffers *setup_buffers (server_config config);

/**
 * @brief Frees up the payloads of all buffers, sets them to NULL and sets
 * bytes_written to 0. The message_buffers instance itself does NOT get freed,
 * since it's static.
 *
 * @note Only call this in a child process.
 *
 * @param bufs A pointer to the message_buffers instance. Cannot be NULL.
 */
void free_bufs (message_buffers *bufs);

/**
 * @brief Allocates memory for all buffer payloads, based on their size
 * property. Since this gets called in a child process, it copies the
 * message_buffers instance on write by calling this function.
 *
 * @note Only call this in a child process.
 *
 * @param bufs A pointer to the message_buffers instance. Cannot be NULL.
 */
void allocate_bufs (message_buffers *bufs);

/**
 * @brief Sets all payload strings to 0, clearing them so they can be reused by
 * the same connection.
 *
 * @note Only call this in a child process, after having already called
 * allocate_bufs.
 *
 * @param bufs A pointer to the message_buffers instance. Cannot be NULL.
 */
void clear_bufs (message_buffers *bufs);

#endif