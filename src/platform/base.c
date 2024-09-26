#include "base.h"
#include "../tools/buffer.h"
#include "../tools/log.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const char* SUPPORTED_PLATFORM_NAMES [] = {
    "github.com",
    "gitlab.com"
};

static void throw_regex_error(int errcode, const regex_t *preg, const char* str) {
    char regex_error_buffer[REGEX_ERROR_STR_MAX_LEN] = {0};
    size_t written = regerror(errcode, preg, regex_error_buffer, REGEX_ERROR_STR_MAX_LEN);
    LOG_DEBUG("regex_error_str() errcode: %d, regerror ret: %lu", errcode, written);
    LOG_ERROR_EXIT("Regex compilation error for str: %s (%s)", str, regex_error_buffer);
}

static bool parse_regex_result(int value) {
    if (value == 0) {
        return true;
    } else if (value == REG_NOMATCH) {
        return false;
    } else {
        LOG_ERROR_EXIT("Regex matching failed");
    }
}

static Lines* get_git_remote_urls_from_lines(Lines* lines, const char* remote) {
    LOG_DEBUG("Entering function %s", __func__);
    char config_remote_line_rg [GIT_CONFIG_REMOTE_STRING_MAX_LEN] = {0};
    const int config_remote_line_written_bytes = snprintf(
        config_remote_line_rg,
        GIT_CONFIG_REMOTE_STRING_MAX_LEN,
        "\\[remote \"%s\"]",
        remote);
    if (config_remote_line_written_bytes < 0) {
        LOG_ERROR_EXIT("Failed generating remote string [remote \"value\"]. Value %s",
        remote);
    }
    RegexWrapper regexes [GIT_CFG_RG_COUNT] = {
        {.regex_pattern = config_remote_line_rg},
        {.regex_pattern = GIT_CONFIG_NEXT_SECTION_RG},
        {.regex_pattern = GIT_CONFIG_REMOTE_URL_RG}
    };
    for (size_t i = 0; i < GIT_CFG_RG_COUNT; i++) {
        int value = regcomp(&(regexes[i].regex_holder), regexes[i].regex_pattern, 0);
        if (value != 0) throw_regex_error(value, &(regexes[i].regex_holder), regexes[i].regex_pattern);
    }

    bool remote_section_found = false;
    bool inside_remote_section = false;
    Lines* git_remote_urls = lines_create(1);
    Buffer* remote_url_buffer = buffer_create(1);
    for (size_t i = 0; i < lines->size; i++) {
        if (remote_section_found) { 
            bool next_section_met = (parse_regex_result(regexec(
                &(regexes[GIT_CFG_RG_NEXT_SECTION].regex_holder),
                lines->lines[i]->data,
                0, NULL, 0)));
            if (!next_section_met) {
                inside_remote_section = true;
                LOG_DEBUG("inside_remote_section = true");
            } else {
                inside_remote_section = false;
                remote_section_found = false;
                LOG_DEBUG("inside_remote_section = false");
                LOG_DEBUG("remote_section_found = false");
            }
        }
        if (inside_remote_section) {
            bool url_found = (parse_regex_result(regexec(
                &(regexes[GIT_CFG_RG_REMOTE_URL].regex_holder),
                lines->lines[i]->data,
                0, NULL, 0)));
            if (url_found) {
                LOG_DEBUG("url_found: %s", lines->lines[i]->data);
                buffer_append(remote_url_buffer, lines->lines[i]->data);
                lines_append(git_remote_urls, remote_url_buffer);
                remote_url_buffer->size = 0;
                continue;
            }
            LOG_DEBUG("url not found inside remote. Line: %s", lines->lines[i]->data);
        }
        if (parse_regex_result(regexec(
                &(regexes[GIT_CFG_RG_USER_GIVEN_REMOTE].regex_holder),
                lines->lines[i]->data,
                0, NULL, 0))
            ) {
                remote_section_found = true;
                LOG_DEBUG("remote_section_found = true");
                continue;
        }
    }
    for (size_t i = 0; i < GIT_CFG_RG_COUNT; i++) {
        regfree(&(regexes[i].regex_holder));
    }
    buffer_free(remote_url_buffer);

    return git_remote_urls;
}

PlaformId identify_platfrom_from_project(const char* project_path, const char* remote) {
    LOG_DEBUG("Entering function %s", __func__);
    if (project_path == NULL) {
       LOG_ERROR_EXIT("No path supplied for project.");
    }
    Buffer* git_config_path_buffer = buffer_create(strlen(project_path));
    buffer_append(git_config_path_buffer, project_path);
    buffer_append(git_config_path_buffer, PATH_TO_GIT_CONFIG_RELATIVE_TO_PROJ);
    LOG_DEBUG("Constructed git config file path: %s", git_config_path_buffer->data);

    Lines* config_file_lines = read_file_lines(git_config_path_buffer->data);
    Lines* git_remote_urls = get_git_remote_urls_from_lines(config_file_lines, remote);
    //TODO: parse remote "origin url" to get platform
    lines_free(config_file_lines);
    lines_flush(git_remote_urls);
    buffer_free(git_config_path_buffer);
    return PL_UNRECOGNIZED;
}