#include "dusk/logging.h"
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <string>

#include "tracy/Tracy.hpp"

bool StubLogEnabled = true;

using namespace std::literals::string_view_literals;

// MSVC is broken and seemingly miscompiles std::string_view::npos without this.
// I wish I was joking.
constexpr size_t npos = std::string_view::npos;

static constexpr std::string_view StubFragments[] = {
    "is a stub"sv,
    "Unimplemented: BP register"sv,
    "Unhandled BP register"sv,
    "Unhandled XF register"sv,
    "but selective updates are not implemented"sv,
};

namespace {
std::mutex g_logMutex;
FILE* g_logFile = nullptr;
std::string g_logFilePath;

const char* LogLevelString(AuroraLogLevel level) {
    switch (level) {
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_INFO:
        return "INFO";
    case LOG_WARNING:
        return "WARNING";
    case LOG_ERROR:
        return "ERROR";
    case LOG_FATAL:
        return "FATAL";
    }

    return "??";
}

FILE* LogStreamForLevel(AuroraLogLevel level) {
    return level >= LOG_ERROR ? stderr : stdout;
}

std::string MakeTimestampedLogName() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
#if _WIN32
    localtime_s(&localTime, &nowTime);
#else
    localtime_r(&nowTime, &localTime);
#endif

    std::array<char, 32> buffer{};
    std::strftime(buffer.data(), buffer.size(), "dusk-%Y%m%d-%H%M%S.log", &localTime);
    return buffer.data();
}

void WriteLogLine(FILE* out, const char* levelStr, const char* module, const char* message, unsigned int len) {
    if (out == nullptr) {
        return;
    }

    std::fprintf(out, "[%s | %s] ", levelStr, module);
    std::fwrite(message, 1, len, out);
    std::fputc('\n', out);
    std::fflush(out);
}
}  // namespace

static bool IsForStubLog(const char* message) {
    std::string_view msg_view(message);

    for (auto& fragment : StubFragments) {
        if (msg_view.find(fragment) != ""sv.npos) {
            return true;
        }
    }

    return false;
}

void aurora_log_callback(AuroraLogLevel level, const char* module, const char* message,
                         unsigned int len) {
    ZoneScoped;
    if (StubLogEnabled && level != LOG_FATAL && IsForStubLog(message)) {
        dusk::SendToStubLog(level, module, message);
        return;
    }

    if (module == nullptr) {
        module = "";
    }

    const char* levelStr = LogLevelString(level);
    FILE* out = LogStreamForLevel(level);
    WriteLogLine(out, levelStr, module, message, len);

    {
        std::lock_guard lock(g_logMutex);
        if (g_logFile != nullptr) {
            WriteLogLine(g_logFile, levelStr, module, message, len);
        }
    }

    if (level == LOG_FATAL) {
        abort();
    }
}

aurora::Module DuskLog("dusk");

void dusk::InitializeFileLogging(const char* configDir, AuroraLogLevel logLevel) {
    std::lock_guard lock(g_logMutex);
    if (g_logFile != nullptr || configDir == nullptr) {
        return;
    }

    std::error_code ec;
    const std::filesystem::path logsDir = std::filesystem::path(configDir) / "logs";
    std::filesystem::create_directories(logsDir, ec);
    if (ec) {
        std::fprintf(stderr, "[WARNING | dusk] Failed to create log directory '%s': %s\n",
                     logsDir.string().c_str(), ec.message().c_str());
        return;
    }

    const std::filesystem::path logPath = logsDir / MakeTimestampedLogName();
    g_logFile = std::fopen(logPath.string().c_str(), "wb");
    if (g_logFile == nullptr) {
        std::fprintf(stderr, "[WARNING | dusk] Failed to open log file '%s'\n",
                     logPath.string().c_str());
        return;
    }

    g_logFilePath = logPath.string();
    aurora::g_config.logCallback = &aurora_log_callback;
    aurora::g_config.logLevel = logLevel;
    WriteLogLine(g_logFile, "INFO", "dusk", "File logging initialized", 24);
}

void dusk::ShutdownFileLogging() {
    std::lock_guard lock(g_logMutex);
    if (g_logFile == nullptr) {
        return;
    }

    std::fflush(g_logFile);
    std::fclose(g_logFile);
    g_logFile = nullptr;
}

const char* dusk::GetLogFilePath() {
    std::lock_guard lock(g_logMutex);
    return g_logFilePath.empty() ? nullptr : g_logFilePath.c_str();
}
