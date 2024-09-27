#ifndef __PLATFORM_BASE_H__
#define __PLATFORM_BASE_H__

#include <regex.h>

#define PATH_TO_GIT_CONFIG_RELATIVE_TO_PROJ "/.git/config"
#define REGEX_ERROR_STR_MAX_LEN 512
#define GIT_CONFIG_REMOTE_STRING_MAX_LEN 256
#define GIT_CONFIG_NEXT_SECTION_RG "\\[.*"
#define GIT_CONFIG_REMOTE_URL_RG ".*url.*"

typedef enum PlaformId {
    PL_GITHUB,
    PL_GITLAB,
    PL_UNRECOGNIZED
} PlaformId;

typedef struct PlatformIdList {
    PlaformId* ids;
    size_t size;
} PlatformIdList;

typedef enum GitConfigRegexes {
    GIT_CFG_RG_USER_GIVEN_REMOTE,
    GIT_CFG_RG_NEXT_SECTION,
    GIT_CFG_RG_REMOTE_URL,
    GIT_CFG_RG_COUNT
} GitConfigRegexes;

typedef struct RegexWrapper {
    const char* regex_pattern;
    regex_t regex_holder;
} RegexWrapper;

extern const char* SUPPORTED_PLATFORM_NAMES [];

PlatformIdList* identify_platfroms_from_project(const char* project_path, const char* remote);

#endif /*__PLATFORM_BASE_H__*/