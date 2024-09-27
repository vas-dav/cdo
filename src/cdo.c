#define CDO_LOG_IMPLEMENTATION

#include <stdio.h>

#include "cmd.h"
#include "platform/base.h"
#include "platform/operations.h"
#include "tools/alloc_wrappers.h"
#include "tools/log.h"

static void __attribute__((constructor)) init_cdo(void) {
	atexit(exit_cmd);
	init_log();
	init_cmd();
}

int main(int argc, char* argv[]) {
	LOG_DEBUG("Entering function %s", __func__);
	struct ArgList* args = parse_args(argc, argv);

	const char* project_path = convert_relative_to_full_path(extract_value_from_arg(args, ARG_PROJECT));
	LOG_INFO("Project full path: %s", project_path);
	PlatformIdList* project_platforms = identify_platfroms_from_project(
	    project_path,
	    extract_value_from_arg(args, ARG_REMOTE));

	cdo_free((void*)project_path);
	free_arg_list(args);
	return 0;
}
