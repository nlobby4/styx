#include "response.h"
#include "config.util.h"
#include "connection.util.h"
#include "errlog.h"
#include "globals.h"
#include "request.h"
#include <aio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

static struct timespec spec = { .tv_sec = 0, .tv_nsec = 10000 };
static connection_state state = { .code = NOT_PROCESSED,
                                  .current_request = 100,
                                  .max_requests = 100,
                                  .timeout = { .tv_sec = 1, .tv_usec = 0 },
                                  .keep_alive = true };
static size_t message_length;
static char resp_buf[BUFSIZ];
static char *body_start;

pthread_mutex_t mutex;

static const char *error_html
    = "<!DOCTYPE html>\n"
      "<html lang=\"en\">\n"
      "<head>\n"
      "\t<meta charset=\"UTF-8\">\n"
      "\t<meta name=\"viewport\" content=\"width=device-width, "
      "initial-scale=1.0\">\n"
      "\t<title>%d %s</title>\n"
      "</head>\n"
      "<body>\n"
      "\t<h1>An error has occured: %d %s</h1>\n"
      "</body>\n"
      "</html>\n";

static void *
connect_to_server (void *msg)
{
  nanosleep (&spec, NULL);
  cr_assert (connect (test_client, (struct sockaddr *)&server_addr,
                      sizeof (server_addr))
             >= 0);
  cr_assert (send (test_client, msg, message_length, 0) >= -1);
  ssize_t bytes_received = recv (test_client, resp_buf, BUFSIZ - 1, 0);
  cr_assert (bytes_received >= 0);
  nanosleep (&spec, NULL);
  recv (test_client, (resp_buf + bytes_received), BUFSIZ - bytes_received - 1,
        MSG_DONTWAIT);
  pthread_mutex_lock (&mutex);
  body_start = strstr (resp_buf, "\r\n\r\n");
  cr_assert_not_null (body_start);
  body_start += 4;
  cr_assert (*body_start != '\0');
  pthread_mutex_unlock (&mutex);
  running = 0;
  close (test_client);
  return msg;
}

TestSuite (connects, .fini = teardown_config, .timeout = 1);

Test (connects, get_test_js)
{
  cr_redirect_stdout ();
  char msg[] = "GET /test.js HTTP/1.1\r\n"
               "Connection: close\r\n"
               "\r\n";
  const char test_js[] = "console.log(\"test\");\n";
  message_length = sizeof (msg);
  pthread_mutex_init (&mutex, NULL);
  setup_connection (8091, msg, connect_to_server);
  header_data *data = request (bufs, &state);
  cr_assert_not_null (data);
  response (bufs, data, &state);
  cr_assert (state.code == OK);
  cr_assert (!state.keep_alive);
  while (body_start == NULL)
    usleep (5);
  cr_assert_not_null (strstr (resp_buf, "HTTP/1.1 200 Ok\r\n"));
  cr_assert_not_null (strstr (resp_buf, "Content-Type: text/javascript\r\n"));
  char content_length_buf[128] = { '\0' };
  sprintf (content_length_buf, "Content-Length: %lu\r\n",
           sizeof (test_js) - 1);
  cr_assert_not_null (strstr (resp_buf, content_length_buf));
  pthread_mutex_lock (&mutex);
  cr_assert_str_eq (body_start, test_js);
  pthread_mutex_unlock (&mutex);
  free_data (data);
  pthread_mutex_destroy (&mutex);
  teardown_connection ();
}

Test (connects, get_index_html)
{
  cr_redirect_stdout ();
  char msg[] = "GET / HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n";
  char index_html[512] = { '\0' };
  FILE *file = fopen ("tests/static/index.html", "r");
  cr_assert_not_null (file);
  fread (index_html, 511, 1, file);
  fclose (file);
  message_length = sizeof (msg);
  pthread_mutex_init (&mutex, NULL);
  setup_connection (8092, msg, connect_to_server);
  header_data *data = request (bufs, &state);
  cr_assert_not_null (data);
  response (bufs, data, &state);
  cr_assert (state.code == OK);
  cr_assert (state.keep_alive);
  while (body_start == NULL)
    usleep (5);
  cr_assert_not_null (strstr (resp_buf, "HTTP/1.1 200 Ok\r\n"));
  cr_assert_not_null (strstr (resp_buf, "Content-Type: text/html\r\n"));
  char content_length_buf[128] = { '\0' };
  sprintf (content_length_buf, "Content-Length: %lu\r\n", strlen (index_html));
  cr_assert_not_null (strstr (resp_buf, content_length_buf));
  pthread_mutex_lock (&mutex);
  cr_assert_str_eq (body_start, index_html);
  pthread_mutex_unlock (&mutex);
  free_data (data);
  pthread_mutex_destroy (&mutex);
  teardown_connection ();
}

Test (connects, not_found)
{
  cr_redirect_stdout ();
  char msg[] = "GET /test.css HTTP/1.1\r\n"
               "Connection: Keep-Alive\r\n\r\n";
  message_length = sizeof (msg);
  char expect_error[512];
  sprintf (expect_error, error_html, NOT_FOUND, "Not Found", NOT_FOUND,
           "Not Found");
  pthread_mutex_init (&mutex, NULL);
  setup_connection (8093, msg, connect_to_server);
  header_data *data = request (bufs, &state);
  cr_assert_not_null (data);
  response (bufs, data, &state);
  cr_assert (state.code == NOT_FOUND);
  cr_assert (state.keep_alive);
  while (body_start == NULL)
    usleep (5);
  cr_assert_not_null (strstr (resp_buf, "HTTP/1.1 404 Not Found\r\n"));
  cr_assert_not_null (strstr (resp_buf, "Content-Type: text/html\r\n"));
  pthread_mutex_lock (&mutex);
  cr_assert_str_eq (body_start, expect_error);
  pthread_mutex_unlock (&mutex);
  free_data (data);
  pthread_mutex_destroy (&mutex);
  teardown_connection ();
}

Test (connects, bad_request)
{
  cr_redirect_stdout ();
  char msg[] = "GET /test.css HTTP/\r\n"
               "Connection: Keep-Alive\r\n\r\n";
  message_length = sizeof (msg);
  char expect_error[512];
  sprintf (expect_error, error_html, BAD_REQUEST, "Bad Request", BAD_REQUEST,
           "Bad Request");
  pthread_mutex_init (&mutex, NULL);
  setup_connection (8094, msg, connect_to_server);
  header_data *data = request (bufs, &state);
  cr_assert_null (data);
  response (bufs, data, &state);
  cr_assert (state.code == BAD_REQUEST);
  cr_assert (!state.keep_alive);
  while (body_start == NULL)
    usleep (5);
  cr_assert_not_null (strstr (resp_buf, "HTTP/1.1 400 Bad Request\r\n"));
  cr_assert_not_null (strstr (resp_buf, "Content-Type: text/html\r\n"));
  pthread_mutex_lock (&mutex);
  cr_assert_str_eq (body_start, expect_error);
  pthread_mutex_unlock (&mutex);
  free_data (data);
  pthread_mutex_destroy (&mutex);
  teardown_connection ();
}

Test (connects, insufficient_storage)
{
  cr_redirect_stdout ();
  char msg[] = " /test.js HTTP/1.1\r\n"
               "Connection: Keep-Alive\r\n\r\n";
  message_length = sizeof (msg);
  char expect_error[512];
  sprintf (expect_error, error_html, INTERNAL_SERVER_ERROR,
           "Internal Server Error", INTERNAL_SERVER_ERROR,
           "Internal Server Error");
  pthread_mutex_init (&mutex, NULL);
  setup_connection (8095, msg, connect_to_server);
  header_data *data = request (bufs, &state);
  cr_assert_null (data);
  // emulates something bad happening
  state.code = INTERNAL_SERVER_ERROR;
  response (bufs, data, &state);
  cr_assert (!state.keep_alive);
  while (body_start == NULL)
    usleep (5);
  cr_assert_not_null (
      strstr (resp_buf, "HTTP/1.1 500 Internal Server Error\r\n"));
  cr_assert_not_null (strstr (resp_buf, "Content-Type: text/html\r\n"));
  pthread_mutex_lock (&mutex);
  cr_assert_str_eq (body_start, expect_error);
  pthread_mutex_unlock (&mutex);
  free_data (data);
  pthread_mutex_destroy (&mutex);
  teardown_connection ();
}