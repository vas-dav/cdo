#ifndef __CMD_H__
#define __CMD_H__

#include <stdlib.h>

#define MAX_CMD_LINE_ARG_NAME_LEN 256
#define MAX_CMD_LINE_ARG_DESC_LEN 1024
#define DEFAULT_USAGE_LINE_LEN 256
#define SUPPORTED_ARGS_AMOUNT 2

typedef enum {
    ARG_PROJECT,
    ARG_HELP,
    ARG_COUNT
} ArgNameId;

struct CommandLineArg {
    char* name;
    char* description;
    char* value;
    ArgNameId id;
};

struct ArgList {
    struct CommandLineArg* args;
    size_t capacity;
    size_t size;
};

struct ArgList* parse_args(const int argc, char* argv[]);
void init_cmd(void);
void exit_cmd(void);
struct ArgList* create_arg_list(size_t capacity);
void append_arg(struct ArgList* list, struct CommandLineArg* arg);
void free_arg_list(struct ArgList* list);
void usage(void);
#endif /*__CMD_H__*/