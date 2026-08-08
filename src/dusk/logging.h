#pragma once

#include <borealis/cli.hpp>
#include <borealis/log.hpp>

#include <filesystem>
#include <string_view>

namespace dusk {
void InitializeLogging(
    const std::filesystem::path& cacheDir, const borealis::cli::StandardOptions& standard);
void SendToStubLog(borealis::LogLevel level, std::string_view module, std::string_view message);
}  // namespace dusk

extern bool StubLogEnabled;

inline constexpr borealis::Log DuskLog{"dusk"};

#ifndef NDEBUG
#define STUB_LOG() DuskLog.debug("{} is a stub", __FUNCTION__)
#else
#define STUB_LOG()
#endif

#if TARGET_PC
#define STUB_RET(...)                                                                              \
    STUB_LOG();                                                                                    \
    return __VA_ARGS__;

#else
#define STUB_RET() (void)0
#endif
