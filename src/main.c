#include "cleanup.h"
#include "mem.h"
#include "run.h"
#include "setup.h"

int
main (int argc, char const **argv)
{
  message_buffers *bufs = setup (argc, argv);
  run (bufs);
  cleanup (bufs);
  return 0;
}
