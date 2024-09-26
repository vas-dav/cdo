#include "buffer.h"
#include "alloc_wrappers.h"
#include "log.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

Buffer* buffer_create(size_t capacity) {
    LOG_DEBUG("Entering function %s", __func__);
    Buffer* buffer = (Buffer*)cdo_malloc(sizeof(Buffer));
    buffer->data = (char*)cdo_malloc(capacity);
    buffer->size = 0;
    buffer->capacity = capacity;
    return buffer;
}

void buffer_append(Buffer* buffer, const char* str) {
    LOG_DEBUG("Entering function %s", __func__);
    size_t len = strlen(str);
    if (buffer->size + len >= buffer->capacity) {
        buffer->capacity = buffer->size + len;
        buffer->data = (char*)cdo_realloc(buffer->data, buffer->capacity);
    }
    strcpy(buffer->data + buffer->size, str);
    buffer->size += len;
}

void buffer_append_c(Buffer* buffer, char c) {
    LOG_DEBUG("Entering function %s", __func__);
    if (buffer->size + 1 >= buffer->capacity) {
        buffer->capacity *= 2;
        buffer->data = (char*)cdo_realloc(buffer->data, buffer->capacity);
    }
    *(buffer->data + buffer->size) = c;
    buffer->size++;
}

void buffer_flush(Buffer* buffer) {
    LOG_DEBUG("Entering function %s", __func__);
    printf("%s", buffer->data);
    buffer_free(buffer);
}

void buffer_free(Buffer* buffer) {
    LOG_DEBUG("Entering function %s", __func__);
    cdo_free(buffer->data);
    buffer->data = NULL;
    cdo_free(buffer);
    buffer = NULL;
}

Lines* lines_create(size_t capacity) {
    LOG_DEBUG("Entering function %s", __func__);
    Lines* _lines = (Lines*)cdo_malloc(sizeof(Lines));
    _lines->lines = (Buffer**)cdo_malloc(capacity * sizeof(Buffer*));
    _lines->size = 0;
    _lines->capacity = capacity;
    return _lines;
}

Lines* read_file_lines(const char* file_path) {
    LOG_DEBUG("Entering function %s", __func__);
    FILE* _file_pointer = fopen(file_path, "r");
    if (_file_pointer == NULL) {
        LOG_ERROR_EXIT("Could not open file %s. ERRNO %d; %s",
                        file_path,
                        errno,
                        strerror(errno));
    }
    struct stat statbuf;
    if (stat(file_path, &statbuf) == -1) {
        LOG_ERROR_EXIT("Could not stat file %s.ERRNO %d; %s",
                        file_path,
                        errno,
                        strerror(errno));
    }
    off_t file_size = statbuf.st_size;
    size_t line_numbers = 0;
    for (off_t i = 0; i < file_size; i++) {
        unsigned char current_char = fgetc(_file_pointer);
        if (current_char == '\n') line_numbers++;
    }
    LOG_DEBUG("file %s has %lu lines.", file_path, line_numbers);
    rewind(_file_pointer);

    Lines* file_lines = lines_create(line_numbers);
    // TODO: Remove magic number from creating line buffer
    Buffer* line_buffer = buffer_create(1);
    for (off_t i = 0; i < file_size; i++) {
        unsigned char current_char = fgetc(_file_pointer);
        if (current_char == '\n') {
            buffer_append_c(line_buffer, '\0');
            lines_append(file_lines, line_buffer);
            line_buffer->size = 0;
        } else {
            buffer_append_c(line_buffer, current_char);
        }
    }
    buffer_free(line_buffer);
    fclose(_file_pointer);

    return file_lines;
}

void lines_append(Lines* lines, Buffer* buffer) {
    LOG_DEBUG("Entering function %s", __func__);
    if (lines->size + 1 >= lines->capacity) {
        lines->capacity *= 2;
        lines->lines = (Buffer**)cdo_realloc(lines->lines, lines->capacity * sizeof(Buffer*));
    }
    Buffer* new_buffer = buffer_create(buffer->size + 1);
    new_buffer->size = buffer->size;
    memcpy(new_buffer->data, buffer->data, buffer->size + 1);
    lines->lines[lines->size] = new_buffer;
    lines->size++;
}

void lines_flush(Lines* lines) {
    LOG_DEBUG("Entering function %s", __func__);
    for(size_t i = 0; i < lines->size; i++) {
        printf("%s\n", lines->lines[i]->data);
        buffer_free(lines->lines[i]);
    }
    cdo_free(lines->lines);
    lines->lines = NULL;
    cdo_free(lines);
    lines = NULL;
}

void lines_free(Lines* lines) {
    LOG_DEBUG("Entering function %s", __func__);
    for(size_t i = 0; i < lines->size; i++) {
        buffer_free(lines->lines[i]);
    }
    cdo_free(lines->lines);
    lines->lines = NULL;
    cdo_free(lines);
    lines = NULL;
}
