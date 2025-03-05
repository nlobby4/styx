#include "cleanup.h"
#include "config.util.h"
#include "globals.h"
#include "pthread.h"
#include "setup.h"
#include <arpa/inet.h>
#include <criterion/criterion.h>
#include <unistd.h>

file_descriptor test_client = -1;
struct sockaddr_in server_addr;
message_buffers *bufs = NULL;

static pthread_t thread;

void
setup_connection (int port, char *msg, void *(*connect_to_server) (void *))
{
  static const int enable = 1;
  static sockaddr_in_p server_sock;
  static socklen_t socklen = sizeof (*server_sock);
  running = 1;
  setup_config (port);
  server_sock = make_ipv4 (config);
  cr_assert ((test_client = socket (AF_INET, SOCK_STREAM, 0)) >= 0);
  cr_assert_not_null (config);
  bufs = setup_buffers (config);
  allocate_bufs (bufs);
  interval.tv_sec = config->timeout_s;
  interval.tv_usec = 0;
  cr_assert ((server = socket (AF_INET, SOCK_STREAM, 0)) >= 0);
  cr_assert (
      setsockopt (server, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof (int))
      >= 0);
  cr_assert (
      bind (server, (struct sockaddr *)server_sock, sizeof (*server_sock))
      >= 0);

  cr_assert (listen (server, config->max_clients) >= 0);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons (config->port);
  inet_pton (AF_INET, config->addr, &server_addr.sin_addr);
  pthread_create (&thread, NULL, connect_to_server, msg);
  cr_assert (
      (connection = accept (server, (struct sockaddr *)server_sock, &socklen))
      != -1);
}

void
teardown_connection (void)
{
  cleanup (bufs);
  pthread_join (thread, NULL);
}