/**
 * @file config.h
 * @author Alexsander d. S. Tenne
 * @brief Contains the data structures and functions needed to
 * parse the config json file and store its data.
 * @version 1.0.0
 * @date 2025-03-05
 */
#ifndef _CONFIG_H
#define _CONFIG_H
#include <inttypes.h>
#include <sys/time.h>

typedef struct
{
  uint16_t port;
  char addr[16];
  long recv_header_sz;
  long recv_body_sz;
  long resp_header_sz;
  long resp_body_sz;
  time_t timeout_s;
  int max_clients;
} *server_config;

server_config config_make (const char *file_name);
void config_destroy (server_config *config);

#endif