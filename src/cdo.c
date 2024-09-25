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
  struct ArgList* args = parse_args(argc, argv);
  for (size_t i = 0; i < args->size; i++) {
    printf("Arg: %s, param: %s\n", args->args[i].name, args->args[i].value);
  }
  return 0;
}
