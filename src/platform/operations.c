#include "operations.h"
#include "../tools/alloc_wrappers.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

const char* convert_relative_to_full_path(const char* relative_path) {
    LOG_DEBUG("Entering convert_relative_to_full_path function");

    char* cwd;
    cwd = getcwd(NULL, 0);
    if (cwd == NULL) {
        LOG_ERROR_EXIT("Can't get current working directory. ERRNO: %d; %s",
                        errno, strerror(errno));
    }
    LOG_DEBUG("Current working directory: %s", cwd);

    if (relative_path == NULL) {
        LOG_WARNING("No relative_path provided, using current working directory.");
        LOG_DEBUG("Returning current working directory as full path");
        return cwd;
    }
    
    const size_t relative_path_len = strlen(relative_path);
    const size_t cwd_len = strlen(cwd);
    LOG_DEBUG("Relative path length: %lu, Current working directory length: %lu", relative_path_len, cwd_len);

    const size_t full_path_length = cwd_len + relative_path_len + 2; // +2 for '/' and '\0'
    char* full_path = cdo_calloc(full_path_length, sizeof(char));
    const int written_bytes = snprintf(full_path, 
                                       full_path_length,
                                       "%s/%s",
                                       cwd,
                                       relative_path);
    if (written_bytes != full_path_length - 1) { // -1 because \0 is not counted
        LOG_ERROR_EXIT("Full path is broken. Should have length = %lu, actual = %d",
                        full_path_length, written_bytes);
    }
    LOG_DEBUG("Returning full path: %s", full_path);

    free(cwd);
    return full_path;
}


const char* get_platform_name_from_project(const char* project_path) {
    return NULL;
}
