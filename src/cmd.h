#ifndef __CMD_H__
#define __CMD_H__

#define MAX_CMD_LINE_ARG_NAME_LEN 256
#define MAX_CMD_LINE_ARG_DESC_LEN 1024
#define DEFAULT_USAGE_LINE_LEN 256
#define SUPPORTED_ARGS_AMOUNT 2

typedef enum {
    EMPTY,
    STRING
} CmdType;

struct CommandLineArg {
    char name[MAX_CMD_LINE_ARG_NAME_LEN];
    char description[MAX_CMD_LINE_ARG_DESC_LEN];
    char* value;
    CmdType type;
};

struct ArgList {
    struct CommandLineArg* args;
    size_t capacity;
    size_t size;
};

struct ArgList* supported_arg_list;

struct CommandLineArg* parse_args(const int argc, char* argv[]);
void init_cmd(void);
void create_arg_list(struct ArgList* list, size_t capacity);
void append_arg(struct ArgList* list, struct CommandLineArg arg);
void usage(void);

#endif /*__CMD_H__*/