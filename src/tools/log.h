#ifndef __THREAD_COMMON_LOG_H_
#define __THREAD_COMMON_LOG_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


/* ================= Settings ================== */
#define LOG_COLORED_OUTPUT
#define HIGH_PRIORITY_DEBUG
#define LOG_DEBUG_MESSAGES 0
/* ================= Settings ================== */

// internal debug defines
#ifdef HIGH_PRIORITY_DEBUG
#include <assert.h>
#define INT_ASSERT(statement) assert(statement)
#else
#define INT_ASSERT(statement) 
#endif

#ifdef LOG_COLORED_OUTPUT
#define C_INFO "\x1b[34mINFO\x1b[0m"
#define C_DEBUG "\x1b[35mDEBUG\x1b[0m"
#define C_DEBUG_ISR "\x1b[36mDEBUG_ISR\x1b[0m"
#define C_WARN "\x1b[33mWARNING\x1b[0m"
#define C_ERROR "\x1b[31mERROR\x1b[0m"
#else
#define C_INFO "INFO"
#define C_DEBUG "DEBUG"
#define C_DEBUG_ISR "DEBUG_ISR"
#define C_WARN "WARNING"
#define C_ERROR "ERROR"
#endif
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

#define LOG_INFO( fmt, ...)                                        \
    create_log_line(C_INFO, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);

#define LOG_WARNING(fmt, ...)                                        \
    create_log_line(C_WARN, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);

#define LOG_ERROR(fmt, ...)                                       \
    create_log_line(C_ERROR, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__); \
    exit(1);

#define LOG_ERROR_USER(fmt, ...) \
    create_log_line_simple(C_ERROR, fmt, ##__VA_ARGS__);

#if LOG_DEBUG_MESSAGES
#define LOG_DEBUG(fmt, ...)                                        \
    create_log_line(C_DEBUG, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);
#endif


#endif /* __THREAD_COMMON_LOG_H_ */
