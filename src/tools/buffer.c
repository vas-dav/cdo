#include "buffer.h"
#include "alloc_wrappers.h"
#include "log.h"

#include <string.h>
#include <stdio.h>

Buffer* buffer_create(size_t capacity) {
    Buffer* buffer = (Buffer*)cdo_malloc(sizeof(Buffer));
    buffer->data = (char*)cdo_malloc(capacity);
    buffer->size = 0;
    buffer->capacity = capacity;
    return buffer;
}

void buffer_append(Buffer* buffer, const char* str) {
    size_t len = strlen(str);
    if (buffer->size + len >= buffer->capacity) {
        buffer->capacity *= 2;
        buffer->data = (char*)cdo_realloc(buffer->data, buffer->capacity);
    }
    strcpy(buffer->data + buffer->size, str);
    buffer->size += len;
}

void buffer_flush(Buffer* buffer) {
    printf("%s", buffer->data);
    buffer_free(buffer);
}

void buffer_free(Buffer* buffer) {
    if (!buffer->data || !buffer) {
        LOG_WARNING("Trying to free NULL pointer");
    }
    cdo_free(buffer->data);
    buffer->data = NULL;
    cdo_free(buffer);
    buffer = NULL;
}


