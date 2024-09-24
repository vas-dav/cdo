#ifndef __ALLOC_WARPPERS_H__
#define __ALLOC_WARPPERS_H__

#include "log.h"

#include <stdlib.h>

#define FAILED_ALLOC_MESSAGE \
    LOG_ERROR("Allocation failed. Returned pointer is NULL.");

#define cdo_malloc(size) ((void*)({ \
    void *__ptr__ = malloc(size); \
    if (!__ptr__) { \
        FAILED_ALLOC_MESSAGE; \
    } \
    __ptr__; \
}))

#define cdo_free(ptr) do { \
    if (!ptr) { \
        LOG_WARNING("Freeing NULL pointer."); \
    } \
    free(ptr); \
} while (0)

#define cdo_calloc(nmemb, size) ((void*)({ \
    void *__ptr__ = calloc(nmemb, size); \
    if (!__ptr__) { \
        FAILED_ALLOC_MESSAGE; \
    } \
    __ptr__; \
}))

#define cdo_realloc(ptr, size) ((void*)({ \
    void *__ptr__ = realloc(ptr, size); \
    if (!__ptr__) { \
        FAILED_ALLOC_MESSAGE; \
    } \
    __ptr__; \
}))

#define cdo_reallocarray(ptr, nmemb, size) ((void*)({ \
    void *__ptr__ = reallocarray(ptr, nmemb, size); \
    if (!__ptr__) { \
        FAILED_ALLOC_MESSAGE; \
    } \
    __ptr__; \
}))




#endif /*__ALLOC_WARPPERS_H__*/ 