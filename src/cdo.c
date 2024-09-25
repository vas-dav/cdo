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
  const char* project_path = extract_value_from_arg(args, ARG_PROJECT);
  LOG_INFO("Project full path: %s", project_path);
  return 0;
}
