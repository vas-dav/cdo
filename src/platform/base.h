#ifndef __PLATFORM_BASE_H__
#define __PLATFORM_BASE_H__

#define PATH_TO_GIT_CONFIG_RELATIVE_TO_PROJ "/.git/config"

typedef enum PlaformId {
    PL_GITHUB,
    PL_GITLAB,
    PL_UNRECOGNIZED
} PlaformId;

extern const char* SUPPORTED_PLATFORM_NAMES [];

PlaformId identify_platfrom_from_project(const char* project_path, const char* remote);

#endif /*__PLATFORM_BASE_H__*/