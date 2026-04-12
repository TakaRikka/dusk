#include "dusk/logging.h"
#include <cstdio>
#include <cstdlib>

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

aurora::Module DuskLog("dusk");
