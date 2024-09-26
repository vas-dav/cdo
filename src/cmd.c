#include "cmd.h"
#include "tools/log.h"
#include "tools/alloc_wrappers.h"
#include "tools/buffer.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static struct ArgList* supported_args;

static const char* PREDEFINED_ARG_NAMES [] = {
    "--project",
    "--help"
};

static const char* PREDEFINED_ARG_DESC [] = {
    "Path to the project to be used for looking up TODO's",
    "Show this message"
};

static const char* PREDEFINED_ARG_VALUES [] = {
    "Curernt Working Directory",
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

static struct CommandLineArg* renderArgById(ArgNameId name) {
    struct CommandLineArg* e = cdo_malloc(sizeof(struct CommandLineArg));
    e->name = NULL;
    e->description = NULL;
    e->value = NULL;
    e->id = name;
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

static struct CommandLineArg* renderArgByValues(
    const char* name,
    const char* description,
    const char* value,
    ArgNameId id) {
    struct CommandLineArg* e = cdo_malloc(sizeof(struct CommandLineArg));
    e->name = NULL;
    e->description = NULL;
    e->value = NULL;
    e->id = id;
    if (name) {
        e->name = cdo_malloc(strlen(name) + 1);
        strcpy(e->name, name);
    }
    if (description) {
        e->description = cdo_malloc(strlen(description) + 1);
        strcpy(e->description, description);
    }
    if (value) {
        e->value = cdo_malloc(strlen(value) + 1);
        strcpy(e->value, value);
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
            append_arg(supported_args, renderArgById(i));
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

const char* extract_value_from_arg(struct ArgList* list, ArgNameId id) {
    for (size_t i = 0; i < ARG_COUNT; i++) {
        if (list->args[i].id == id) {
            return list->args[i].value;
        }
    }
    return NULL;
}

void free_arg_list(struct ArgList* list) {
    for (size_t i = 0; i < list->size; i++) {
        cdo_free(list->args[i].name);
        cdo_free(list->args[i].description);
        cdo_free(list->args[i].value);
    }
    cdo_free(list->args);
    cdo_free(list);
    
}

static ArgNameId get_arg_name_id_from_str(const char* str) {
    for (int i = 0; i < ARG_COUNT; i++) {
        if (strcmp(str, PREDEFINED_ARG_NAMES[i]) == 0)
            return (ArgNameId)i;
    }
    return ARG_COUNT;
}

static bool validate_arg_param(const char* str) {
    for (int i = 0; i < ARG_COUNT; i++) {
        if (strcmp(str, PREDEFINED_ARG_NAMES[i]) == 0)
            return false;
    }
    return true;
}

struct ArgList* parse_args(const int argc, char* argv[]) {
    int initial_capacity = 1;
    if (argc > 1) {
        initial_capacity = argc / 2;
    }
    struct ArgList* user_args = create_arg_list(initial_capacity);
    for(int i = 1; i < argc; i++) {
        ArgNameId current_arg = get_arg_name_id_from_str(argv[i]);
        switch (current_arg)
        {
        case ARG_HELP:
            usage();
            break;
        case ARG_PROJECT:
            int next_iterator = (i + 1 < argc) ? i + 1 : i;
            if (!validate_arg_param(argv[next_iterator])) {
                LOG_ERROR("Argument '%s' requires a value.", PREDEFINED_ARG_NAMES[current_arg]);
                goto defer;
            }
            i = next_iterator;
            append_arg(
                user_args, 
                renderArgByValues(
                    PREDEFINED_ARG_NAMES[current_arg],
                    NULL,
                    argv[next_iterator],
                    current_arg
                )
            );  
            break;
        default:
            LOG_ERROR("Undefined argument '%s'", argv[i]);
            goto defer;
        }
    }
    return user_args;
    defer:
            usage();
            exit(1);
}