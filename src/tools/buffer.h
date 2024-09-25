#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdlib.h>

typedef struct {
    char* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer* buffer_create(size_t capacity);
void buffer_append(Buffer* buffer, const char* str);
void buffer_append_c(Buffer* buffer, char c);
void buffer_flush(Buffer* buffer);
void buffer_free(Buffer* buffer);

#endif /*__BUFFER_H__*/
