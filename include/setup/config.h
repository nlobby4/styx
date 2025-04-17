/**
 * @file config.h
 * @author -
 * @brief Contains the data structures and functions needed to
 * parse the config JSON file and store its data.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _CONFIG_H
#define _CONFIG_H
#include <inttypes.h>
#include <sys/time.h>

/**
 * @brief Represents the server configuration.
 *
 * @note Defined as a pointer, since it's only dynamically allocated.
 */
typedef struct
{
  uint16_t port;       /**< Port number the server listens on. */
  char addr[16];       /**< IP address the server binds to. */
  long recv_header_sz; /**< Size of the buffer for receiving header buffer. */
  long recv_body_sz;   /**< Size of the buffer for receiving body buffer. */
  long resp_header_sz; /**< Size of the buffer for response header buffer. */
  long resp_body_sz;   /**< Size of the buffer for response body buffer. */
  time_t timeout_s;    /**< Timeout in seconds for pending clients. */
  int max_clients;     /**< Maximum number of clients that can connect
                          simultaneously. */
} *server_config;

/**
 * @brief Parses the configuration file and creates a server_config instance.
 *
 * @note server_config is dynamically allocated.
 *
 * @param file_name The name of the configuration file.
 * @return A pointer to the allocated server_config instance.
 */
server_config config_make (const char *file_name);

/**
 * @brief Frees the server_config instances memory and sets its pointer to
 * NULL.
 *
 * @param config A pointer to the pointer of the server_config instance to
 * destroy.
 */
void config_destroy (server_config *config);

#endif