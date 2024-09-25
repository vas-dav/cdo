#include "base.h"
#include "../tools/buffer.h"
#include "../tools/log.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const char* SUPPORTED_PLATFORM_NAMES [] = {
    "github.com",
    "gitlab.com"
};

PlaformId identify_platfrom_from_project(const char* project_path) {
    if (project_path == NULL) {
       LOG_ERROR_EXIT("No path supplied for project.");
    }
    Buffer* git_config_path_buffer = buffer_create(strlen(project_path));
    buffer_append(git_config_path_buffer, project_path);
    buffer_append(git_config_path_buffer, PATH_TO_GIT_CONFIG_RELATIVE_TO_PROJ);
    LOG_DEBUG("Constructed git config file path: %s", git_config_path_buffer->data);

    FILE* git_config_fp = fopen(git_config_path_buffer->data, "r");
    if (git_config_fp == NULL) {
        LOG_ERROR_EXIT("Could not open file %s. ERRNO %d; %s",
                        git_config_path_buffer->data,
                        errno,
                        strerror(errno));
    }
    struct stat statbuf;
    if (stat(git_config_path_buffer->data, &statbuf) == -1) {
        LOG_ERROR_EXIT("Could not stat file %s.ERRNO %d; %s",
                        git_config_path_buffer->data,
                        errno,
                        strerror(errno));
    }
    off_t file_size = statbuf.st_size;
    buffer_free(git_config_path_buffer);
    // TODO: Remove magic number from creating buffer for git config line
    Buffer* line_buffer = buffer_create(128);
    for (off_t i = 0; i < file_size; i++) {
        unsigned char current_char = fgetc(git_config_fp);
        if (current_char == '\n') {
            buffer_append_c(line_buffer, '\0');
            LOG_INFO("%s", line_buffer->data);
            line_buffer->size = 0;
        } else {
            buffer_append_c(line_buffer, current_char);
        }
    }
    fclose(git_config_fp);
    buffer_free(line_buffer);
    return PL_UNRECOGNIZED;
}