#include "cmd.h"
#include "tools/log.h"
#include "tools/alloc_wrappers.h"
#include "tools/buffer.h"

#include <stdlib.h>
#include <string.h>

static struct ArgList* supported_args;

static const char* PREDEFINED_ARG_NAMES [] = {
    "project",
    "help"
};

static const char* PREDEFINED_ARG_DESC [] = {
    "Path to the project to be used for looking up TODO's",
    "Show this message"
};

static const char* PREDEFINED_ARG_VALUES [] = {
    "Curernt WD",
    NULL
};

void usage(void) {
    Buffer * usage_lines = buffer_create(DEFAULT_USAGE_LINE_LEN);
    buffer_append(usage_lines, "Usage: cdo [OPTION]...\n");
    buffer_append(usage_lines, "Parse TODO's of the PROJECT.\n\n");
    for (int i = 0; i < ARG_COUNT; i++) {
        buffer_append(usage_lines, supported_args->args[i].name);
        buffer_append(usage_lines, " - ");
        buffer_append(usage_lines, supported_args->args[i].description);
        if (supported_args->args[i].value) {
            buffer_append(usage_lines, " [DEFAULT = ");
            buffer_append(usage_lines, supported_args->args[i].value);
            buffer_append(usage_lines, "]");
        }
        buffer_append(usage_lines, "\n");
    }
    buffer_append(usage_lines, "\n");
    buffer_flush(usage_lines);
}

static struct CommandLineArg* renderArg(ArgName name) {
    struct CommandLineArg* e = cdo_malloc(sizeof(struct CommandLineArg));
    e->name = NULL;
    e->description = NULL;
    e->value = NULL;
    if (PREDEFINED_ARG_NAMES[name]) {
        e->name = cdo_malloc(strlen(PREDEFINED_ARG_NAMES[name]) + 1);
        strcpy(e->name, PREDEFINED_ARG_NAMES[name]);
    }
    if (PREDEFINED_ARG_DESC[name]) {
        e->description = cdo_malloc(strlen(PREDEFINED_ARG_DESC[name]) + 1);
        strcpy(e->description, PREDEFINED_ARG_DESC[name]);
    }
    if (PREDEFINED_ARG_VALUES[name]) {
        e->value = cdo_malloc(strlen(PREDEFINED_ARG_VALUES[name]) + 1);
        strcpy(e->value, PREDEFINED_ARG_VALUES[name]);
    }
    return e;
}

void init_cmd(void) {
    supported_args = create_arg_list(ARG_COUNT);
    for (int i = 0; i < ARG_COUNT; i++) {
        switch (i)
        {
        case ARG_PROJECT:
        case ARG_HELP:
            append_arg(supported_args, renderArg(i));
            break;
        default:
            LOG_ERROR_EXIT("Undefined argument iterator %d", i);
        }
    }
    LOG_DEBUG("Cmd module initialised.");
}

void exit_cmd(void) {
    free_arg_list(supported_args);
}

struct ArgList* create_arg_list(size_t capacity) {
    struct ArgList* list = NULL;
    list = (struct ArgList*) cdo_malloc(sizeof(struct ArgList));
    list->args = (struct CommandLineArg*) cdo_malloc(capacity * sizeof(struct CommandLineArg));
    list->size = 0;
    list->capacity = capacity;
    return list;
}

void append_arg(struct ArgList* list, struct CommandLineArg* arg) {
    if (list->size + 1 >= list->capacity) {
        list->capacity *= 2;
        list->args =
        (struct CommandLineArg*)
        cdo_realloc(
            list->args,
            list->capacity * sizeof(struct CommandLineArg)
        );
    }
    memcpy(list->args + list->size, arg, sizeof(struct CommandLineArg));
    cdo_free(arg);
    list->size += 1;
}

void free_arg_list(struct ArgList* list) {
    for (size_t i = 0; i < ARG_COUNT; i++) {
        cdo_free(list->args[i].name);
        cdo_free(list->args[i].description);
        cdo_free(list->args[i].value);
    }
    cdo_free(list->args);
    cdo_free(list);
    
}

struct CommandLineArg* parse_args(const int argc, char* argv[]) {
    if (argc < 2) {
        LOG_ERROR("At least 1 argument required.");
        usage();
        exit(1);
    }
    return NULL;
}