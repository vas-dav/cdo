#include <stdio.h>

#include "cmd.h"

int main (int argc, char *argv[]) {
  struct CommandLineArg* args = parse_args(argc, argv);
  return 0;
}
