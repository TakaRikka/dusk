#include "dusk/logging.h"
#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <string>

#include "tracy/Tracy.hpp"

#if TARGET_ANDROID
#include "android/log.h"
#include <vector>
#include <sstream>
#endif

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
// On macOS, std::mutex becomes poisoned when its dtor is run.
// We use this to check if the LogState is destroyed before attempting to acquire it.
std::atomic g_logStateAlive(true);

struct LogState {
    std::mutex mutex;
    FILE* file = nullptr;
    std::string filePath;

    ~LogState() {
        CloseFile();
        g_logStateAlive.store(false, std::memory_order_release);
    }

    void CloseFile() {
        if (!g_logStateAlive.load(std::memory_order_acquire)) {
            return;
        }
        std::lock_guard lock(mutex);
        if (file != nullptr) {
            std::fflush(file);
            std::fclose(file);
            file = nullptr;
        }
    }
};
LogState g_logState;

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

#if TARGET_ANDROID
void aurora_log_callback(AuroraLogLevel level, const char* module, const char* message,
                         unsigned int len) {
    ZoneScoped;
    if (StubLogEnabled && level != LOG_FATAL && IsForStubLog(message)) {
        dusk::SendToStubLog(level, module, message);
        return;
    }

    int android_log_level = 0;
    switch (level) {
    case LOG_DEBUG:
        android_log_level = ANDROID_LOG_DEBUG;
        break;
    case LOG_INFO:
        android_log_level = ANDROID_LOG_INFO;
        break;
    case LOG_WARNING:
        android_log_level = ANDROID_LOG_WARN;
        break;
    case LOG_ERROR:
        android_log_level = ANDROID_LOG_ERROR;
        break;
    case LOG_FATAL:
        android_log_level = ANDROID_LOG_FATAL;
        break;
    }

    std::stringstream msgStream(message);
    std::string segment;
    while(std::getline(msgStream, segment)) {
        __android_log_print(android_log_level, module, "%s\n", segment.c_str());
    }

    if (level == LOG_FATAL) {
        abort();
    }
}
#else
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

    if (g_logStateAlive.load(std::memory_order_acquire)) {
        std::lock_guard lock(g_logState.mutex);
        if (g_logState.file != nullptr) {
            WriteLogLine(g_logState.file, levelStr, module, message, len);
        }
    }

    if (level == LOG_FATAL) {
        abort();
    }
}
#endif


aurora::Module DuskLog("dusk");

void dusk::InitializeFileLogging(const std::filesystem::path& configDir, AuroraLogLevel logLevel) {
    if (!g_logStateAlive.load(std::memory_order_acquire)) {
        return;
    }
    std::lock_guard lock(g_logState.mutex);
    if (g_logState.file != nullptr || configDir.empty()) {
        return;
    }

    std::error_code ec;
    const std::filesystem::path logsDir = configDir / "logs";
    std::filesystem::create_directories(logsDir, ec);
    if (ec) {
        std::fprintf(stderr, "[WARNING | dusk] Failed to create log directory '%s': %s\n",
                     logsDir.string().c_str(), ec.message().c_str());
        return;
    }

    const std::filesystem::path logPath = logsDir / MakeTimestampedLogName();
    g_logState.file = std::fopen(logPath.string().c_str(), "wb");
    if (g_logState.file == nullptr) {
        std::fprintf(stderr, "[WARNING | dusk] Failed to open log file '%s'\n",
                     logPath.string().c_str());
        return;
    }

    g_logState.filePath = logPath.string();
    aurora::g_config.logCallback = &aurora_log_callback;
    aurora::g_config.logLevel = logLevel;
    WriteLogLine(g_logState.file, "INFO", "dusk", "File logging initialized", 24);
}

void dusk::ShutdownFileLogging() {
    if (!g_logStateAlive.load(std::memory_order_acquire)) {
        return;
    }
    g_logState.CloseFile();
}

const char* dusk::GetLogFilePath() {
    if (!g_logStateAlive.load(std::memory_order_acquire)) {
        return nullptr;
    }
    std::lock_guard lock(g_logState.mutex);
    return g_logState.filePath.empty() ? nullptr : g_logState.filePath.c_str();
}
