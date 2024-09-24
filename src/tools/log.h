#ifndef __THREAD_COMMON_LOG_H_
#define __THREAD_COMMON_LOG_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_COLORED_OUTPUT_ENV "CDO_LOG_ENABLE_COLOR"
#define LOG_DEBUG_MESSAGES_ENV "CDO_LOG_ENABLE_DEBUG"

#include <assert.h>
#define INT_ASSERT(statement) assert(statement)


typedef enum LogLevel {
    LEVEL_INFO,
    LEVEL_DEBUG,
    LEVEL_WARNING,
    LEVEL_ERROR
} LogLevel;

static const char* coloredLogLevels[] = {
    "\x1b[34mINFO\x1b[0m",
    "\x1b[35mDEBUG\x1b[0m",
    "\x1b[33mWARNING\x1b[0m",
    "\x1b[31mERROR\x1b[0m"
};

static const char* plainLogLevels[] = {
    "INFO",
    "DEBUG",
    "WARNING",
    "ERROR"
};

extern char* cdo_log_enable_color;
extern char* cdo_log_enable_debug;
extern void init_log(void);

#define LOG_BUFFER_MAX_CAP 256
#define LOG_MESSAGE_MAX_CAP 150

#define _LOG_STREAMOUT(message, message_length)                 \
    INT_ASSERT(message_length > 0);                             \
    printf("%.*s", message_length, message)

static void create_log_line(const char * _status,
                                const char * _location,
                                const char * _func,
                                const size_t _line,
                                const char * _fmt, ...)
{
    va_list args;
    va_start(args, _fmt);
    char message [LOG_BUFFER_MAX_CAP] = {0};
    int message_len = vsnprintf(message, LOG_BUFFER_MAX_CAP, _fmt, args);
    va_end(args);
    char buffer [LOG_BUFFER_MAX_CAP] = {0};
    int buffer_len = snprintf(buffer, LOG_BUFFER_MAX_CAP,
                            "[%s] In [File: %s] [Func: %s] [Line: %zu] %.*s\n",
                            _status,
                            _location,
                            _func,
                            _line,
                            message_len,
                            message);
    _LOG_STREAMOUT(buffer, buffer_len);
    
}

static void create_log_line_simple(const char * _status,
                                const char * _fmt, ...)
{
    va_list args;
    va_start(args, _fmt);
    char message [LOG_BUFFER_MAX_CAP] = {0};
    int message_len = vsnprintf(message, LOG_BUFFER_MAX_CAP, _fmt, args);
    va_end(args);
    char buffer [LOG_BUFFER_MAX_CAP] = {0};
    int buffer_len = snprintf(buffer, LOG_BUFFER_MAX_CAP,
                            "[%s] %.*s\n",
                            _status,
                            message_len,
                            message);
    _LOG_STREAMOUT(buffer, buffer_len);
    
}

#define GET_LOG_SEVERITY_STR(SEVERITY)( \
    (cdo_log_enable_color) ? coloredLogLevels[SEVERITY] : plainLogLevels[SEVERITY] \
)

#define LOG_INFO( fmt, ...)                                        \
    create_log_line(GET_LOG_SEVERITY_STR(LEVEL_INFO), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);

#define LOG_WARNING(fmt, ...)                                        \
    create_log_line(GET_LOG_SEVERITY_STR(LEVEL_WARNING), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);

#define LOG_ERROR(fmt, ...)                                       \
    create_log_line(GET_LOG_SEVERITY_STR(LEVEL_ERROR), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__); \
    exit(1);

#define LOG_ERROR_USER(fmt, ...) \
    create_log_line_simple(GET_LOG_SEVERITY_STR(LEVEL_ERROR), fmt, ##__VA_ARGS__);

#define LOG_DEBUG(fmt, ...)                                        \
    create_log_line(GET_LOG_SEVERITY_STR(LEVEL_DEBUG), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);




#endif /* __THREAD_COMMON_LOG_H_ */
