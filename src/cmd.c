#include "cmd.h"
#include "tools/log.h"
#include "tools/buffer.h"

#include <stdlib.h>
#include <string.h>

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

void init_cmd(void) {
    return
}

void create_arg_list(struct ArgList* list, size_t capacity) {
    list = (struct ArgList*) malloc(sizeof(struct ArgList));
    list->args = (struct CommandLineArg*) malloc(capacity * sizeof(struct CommandLineArg));
    list->size = 0;
    list->capacity = capacity;
}

void append_arg(struct ArgList* list, struct CommandLineArg arg) {
    if (list->size + 1 >= list->capacity) {
        list->capacity *= 2;
        list->args =
        (struct CommandLineArg*)
        realloc(
            list->args,
            list->capacity * sizeof(struct CommandLineArg)
        );
    }
    memcpy(
        list->args + (list->size * sizeof(struct CommandLineArg)),
        &arg,
        sizeof(struct CommandLineArg)
    );
}

struct CommandLineArg* parse_args(const int argc, char* argv[]) {
    if (argc < 2) {
        LOG_ERROR_USER("At least 1 argument required.");
        usage();
        exit(1);
    }
    return NULL;
}