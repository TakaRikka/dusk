#include "dusk/logging.h"
#include "dusk/config.h"
#include <cstdio>
#include <cstdlib>

void aurora_log_callback(AuroraLogLevel level, const char* module, const char* message,
                         unsigned int len) {
    if (level < duskConfig.logLevel) return;

    const char* levelStr = "??";
    FILE* out = stdout;
    switch (level) {
    case LOG_DEBUG:
        levelStr = "DEBUG";
        break;
    case LOG_INFO:
        levelStr = "INFO";
        break;
    case LOG_WARNING:
        levelStr = "WARNING";
        break;
    case LOG_ERROR:
        levelStr = "ERROR";
        out = stderr;
        break;
    case LOG_FATAL:
        levelStr = "FATAL";
        out = stderr;
        break;
    }
    fprintf(out, "[%s | %s] %s\n", levelStr, module, message);
    if (level == LOG_FATAL) {
        fflush(out);
        abort();
    }
}
