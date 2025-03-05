#include "buf.h"
#include "errlog.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void
cleanup (message_buffers *bufs)
{
  NULL_CHECK (bufs, );
  int status;

  if (pid != 0)
    {
      close (server);
#ifdef TEST
      server = 0;
#endif
      wait (&status);
      puts ("\nClosing server...");
    }
  else
    {
      close (connection);
#ifdef TEST
      connection = 0;
#endif
      free_bufs (bufs);
    }
  fflush (NULL);
}