#include "mem.h"
#include "config.h"
#include "globals.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#define ERROR_MSG(msg) ("\033[1;31mERROR:\033[0m " msg)

server_config *config = NULL;
file_descriptor server = 0, connection = 0;
volatile sig_atomic_t running = 1;

void
setup (void)
{
  config = load_config ("serverconfig.json");
}

void
teardown (void)
{
  free (config);
  config = NULL;
}

TestSuite (buffer_creation, .init = setup, .fini = teardown);

Test (buffer_creation, config_not_null)
{
  cr_redirect_stderr ();
  message_buffers *buffers = setup_buffers (NULL);
  cr_stderr_match_str (ERROR_MSG ("config cannot be null"));
  cr_expect (buffers == NULL, "Expected buffer to be NULL");
}