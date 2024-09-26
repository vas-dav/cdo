#include "log.h"

char* cdo_log_enable_color = NULL;
char* cdo_log_enable_debug = NULL;
char* cdo_log_enable_verbose = NULL;

void init_log(void) {
    LOG_DEBUG("Entering function %s", __func__);
    cdo_log_enable_color = getenv(LOG_COLORED_OUTPUT_ENV);
    cdo_log_enable_debug = getenv(LOG_DEBUG_MESSAGES_ENV);
    cdo_log_enable_verbose = getenv(LOG_VERBOSITY_ENV);
    LOG_DEBUG("Log module initialised.");
    if (!cdo_log_enable_color) {
        LOG_DEBUG("%s env not defined.", LOG_COLORED_OUTPUT_ENV);
    }
    if (!cdo_log_enable_debug) {
        LOG_DEBUG("%s env not defined", LOG_DEBUG_MESSAGES_ENV);
    }
    if (!cdo_log_enable_verbose) {
        LOG_DEBUG("%s env not defined", LOG_VERBOSITY_ENV);
    }
}