#ifndef DUSK_LOGGING_H
#define DUSK_LOGGING_H

#include <aurora/aurora.h>

#define STUB_LOG(v) puts(__FUNCTION__ " is a stub")

void aurora_log_callback(AuroraLogLevel level, const char* module, const char* message, unsigned int len);

#define DUSK_DEBUG()
#endif
