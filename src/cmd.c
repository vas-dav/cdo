#include "cmd.h"
#include "tools/log.h"
#include "tools/buffer.h"

#include <stdlib.h>

const struct CommandLineArg supported_args [] = {
    {
        "PROJECT",
        "Path to the project to be used for looking up TODO's",
        "Current Working directory",
        STRING
    },
    {}
};

void usage(void) {
    Buffer * usage_lines = buffer_create(DEFAULT_USAGE_LINE_LEN);
    buffer_append(usage_lines, "Usage: cdo [PROJECT] [OPTION]\n");
    buffer_append(usage_lines, "Parse TODO's of the PROJECT.\n\n");
    for (int i = 0; supported_args[i].type != EMPTY; i++) {
        buffer_append(usage_lines, supported_args[i].name);
        buffer_append(usage_lines, " - ");
        buffer_append(usage_lines, supported_args[i].description);
        buffer_append(usage_lines, " [DEFAULT = ");
        buffer_append(usage_lines, supported_args[i].value);
    }
    buffer_append(usage_lines, "\n]");
    buffer_flush(usage_lines);
}

struct CommandLineArg* parse_args(const int argc, char* argv[]) {
    if (argc < 2) {
        LOG_ERROR_USER("At least 1 argument required.");
        usage();
        exit(1);
    }
    return NULL;
}