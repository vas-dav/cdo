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

    Lines* config_file_lines = read_file_lines(git_config_path_buffer->data);
    lines_flush(config_file_lines);
    buffer_free(git_config_path_buffer);
    return PL_UNRECOGNIZED;
}