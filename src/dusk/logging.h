#pragma once

#include <aurora/aurora.h>
#include <aurora/lib/logging.hpp>

#include <filesystem>

void aurora_log_callback(AuroraLogLevel level, const char* module, const char* message, unsigned int len);

namespace dusk {
    void InitializeFileLogging(const std::filesystem::path& configDir, AuroraLogLevel logLevel);
    void ShutdownFileLogging();
    const char* GetLogFilePath();
    int GetLogFileDescriptor();
    void SendToStubLog(AuroraLogLevel level, const char* module, const char* message);
}

extern bool StubLogEnabled;

extern aurora::Module DuskLog;

#ifndef NDEBUG
#define STUB_LOG() DuskLog.debug("{} is a stub", __FUNCTION__)
#else
#define STUB_LOG()
#endif

#if TARGET_PC
#define STUB_RET(...) \
    STUB_LOG(); \
    return __VA_ARGS__;

#else
#define STUB_RET() (void)0
#endif
