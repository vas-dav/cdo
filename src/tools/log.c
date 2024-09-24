#include "log.h"

char* cdo_log_enable_color = NULL;
char* cdo_log_enable_debug = NULL;

void init_log(void) {
    cdo_log_enable_color = getenv(LOG_COLORED_OUTPUT_ENV);
    cdo_log_enable_debug = getenv(LOG_DEBUG_MESSAGES_ENV);
    LOG_DEBUG("Log module initialised.");
    if (!cdo_log_enable_color) {
        LOG_DEBUG("%s env not defined.", LOG_COLORED_OUTPUT_ENV);
    }
    if (!cdo_log_enable_debug) {
        LOG_DEBUG("%s env not defined", LOG_DEBUG_MESSAGES_ENV);
    }
}