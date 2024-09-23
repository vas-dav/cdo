#include <stdio.h>

#include "cmd.h"

int main (int argc, char *argv[]) {
  init_cmd();
  struct CommandLineArg* args = parse_args(argc, argv);
  return 0;
}
