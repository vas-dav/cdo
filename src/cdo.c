#define CDO_LOG_IMPLEMENTATION

#include <stdio.h>

#include "cmd.h"
#include "tools/log.h"

static void __attribute__((constructor)) init_cdo(void) {
  atexit(exit_cmd);
  init_log();
  init_cmd();
}

int main (int argc, char *argv[]) {
  struct CommandLineArg* args = parse_args(argc, argv);
  return 0;
}
