#ifndef __PLATFORM_OPERATIONS_H__
#define __PLATFORM_OPERATIONS_H__

#define PATH_TO_GIT_CONFIG_RELATIVE_TO_PROJ ".git/config"

const char* convert_relative_to_full_path(const char* relative_path);
const char* get_platform_name_from_project(const char* project_path);


#endif /*__PLATFORM_OPERATIONS_H__*/