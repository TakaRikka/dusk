#ifndef DUSK_LOGGING_H
#define DUSK_LOGGING_H

#include <aurora/aurora.h>
#include <aurora/lib/logging.hpp>

void aurora_log_callback(AuroraLogLevel level, const char* module, const char* message, unsigned int len);

namespace dusk {
    void InitializeFileLogging(const char* configDir, AuroraLogLevel logLevel);
    void ShutdownFileLogging();
    const char* GetLogFilePath();
    void SendToStubLog(AuroraLogLevel level, const char* module, const char* message);
}

extern bool StubLogEnabled;

extern aurora::Module DuskLog;

#define STUB_LOG() DuskLog.debug("{} is a stub", __FUNCTION__)

#if TARGET_PC
#define STUB_RET(...) \
    STUB_LOG(); \
    return __VA_ARGS__;

#else
#define STUB_RET() (void)0
#endif

#endif
