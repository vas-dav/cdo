#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

typedef struct {
    char* data;
    size_t size;
    size_t capacity;
} Buffer;

typedef struct {
    Buffer** lines;
    size_t size;
    size_t capacity;
} Lines;

Buffer* buffer_create(size_t capacity);
void buffer_append(Buffer* buffer, const char* str);
void buffer_append_c(Buffer* buffer, char c);
void buffer_flush(Buffer* buffer);
void buffer_free(Buffer* buffer);

Lines* lines_create(size_t capacity);
Lines* read_file_lines(const char* file_path);
void lines_append(Lines* lines, Buffer* buffer);
void lines_flush(Lines* lines);
void lines_free(Lines* lines);


#endif /*__BUFFER_H__*/
