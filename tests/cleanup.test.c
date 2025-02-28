#include "cleanup.h"
#include "config.util.h"
#include "errlog.h"
#include "globals.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>

Test (_, cleanup_not_null)
{
  cr_redirect_stderr ();
  cleanup (NULL);
  cr_assert_stderr_eq_str (ERROR_MSG ("bufs cannot be NULL\n"));
}

Test (_, cleanup_server, .fini = teardown_config)
{
  cr_redirect_stdout ();
  pid = getpid ();
  server = -1;
  connection = -1;
  cr_assert ((server = socket (AF_INET, SOCK_STREAM, 0)) >= 0);
  setup_config (1);
  message_buffers *bufs = setup_buffers (config);
  cleanup (bufs);
  cr_assert (server == 0);
  cr_assert (connection == -1);
  cr_assert_stdout_eq_str ("\nClosing server...\n");
}

Test (_, cleanup_client, .fini = teardown_config)
{
  cr_redirect_stdout ();
  pid = 0;
  server = -1;
  connection = -1;
  cr_assert ((connection = socket (AF_INET, SOCK_STREAM, 0)) >= 0);
  setup_config (1);
  message_buffers *bufs = setup_buffers (config);
  allocate_bufs (bufs);
  cleanup (bufs);
  cr_assert (server == -1);
  cr_assert (connection == 0);
  cr_assert_stdout_neq_str ("\nClosing server...\n");
}