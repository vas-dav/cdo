#define CDO_LOG_IMPLEMENTATION

#include <stdio.h>

#include "cmd.h"
#include "tools/log.h"
#include "platform/operations.h"
#include "platform/base.h"

static void __attribute__((constructor)) init_cdo(void) {
  atexit(exit_cmd);
  init_log();
  init_cmd();
}

int main (int argc, char *argv[]) {
  struct ArgList* args = parse_args(argc, argv);
  const char* relative_project_path = extract_value_from_arg(args, ARG_PROJECT);
  const char* full_path = convert_relative_to_full_path(relative_project_path);
  LOG_INFO("Project full path: %s", full_path);
  identify_platfrom_from_project(full_path);
  return 0;
}
