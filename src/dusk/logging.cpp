#include "dusk/logging.h"
#include <cstdio>
#include <cstdlib>

#include "tracy/Tracy.hpp"

#if ANDROID
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

static bool IsForStubLog(const char* message) {
    std::string_view msg_view(message);

    for (auto& fragment : StubFragments) {
        if (msg_view.find(fragment) != ""sv.npos) {
            return true;
        }
    }

    return false;
}

#if ANDROID
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
#endif


aurora::Module DuskLog("dusk");
