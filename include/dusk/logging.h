#ifndef DUSK_LOGGING_H
#define DUSK_LOGGING_H

#include <aurora/aurora.h>
#include <aurora/lib/logging.hpp>

void aurora_log_callback(AuroraLogLevel level, const char* module, const char* message, unsigned int len);

extern aurora::Module DuskLog;

#define STUB_LOG(v) DuskLog.debug(__FUNCTION__ " is a stub")

#endif
