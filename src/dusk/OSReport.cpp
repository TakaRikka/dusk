#include <memory>

#include "aurora/lib/logging.hpp"
#include "os_report.h"

aurora::Module Log("dusk::osReport");

bool dusk::OSReportReallyForceEnable = false;

u8 __OSReport_disable;

void OSReportDisable() {
    __OSReport_disable = true;
}

void OSReportEnable() {
    __OSReport_disable = false;
}

static bool checkEnabled() {
    return !__OSReport_disable || dusk::OSReportReallyForceEnable;
}

#ifndef va_copy
#define va_copy(d, s) ((d) = (s))
#endif

static std::string FormatToString(const char* msg, va_list list) {
    size_t size = (strlen(msg) * 2) + 50;
    std::string str;
    va_list ap;
    int attempts = 0;
    while (true) {
        str.resize(size);
        va_copy(ap, list);
        int n = vsnprintf(str.data(), size, msg, ap);
        va_end(ap);
        if (n > -1 && n < size) { 
            str.resize(n);
            break;
        }
        
        ++attempts;
        if (attempts >= 3) {
            if (n == -1) {
                str.clear();
            }
            break;
        }
        if (n > -1) {
            size = n + 1;
        } else {
            size *= 2;
        }
    }
    return str;
}

void OSReport(const char* fmt, ...) {
    if (!checkEnabled()) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    const auto str = FormatToString(fmt, args);
    va_end(args);

    Log.info("{}", str);
}

void OSPanic(const char* file, int line, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const auto str = FormatToString(fmt, args);
    va_end(args);

    Log.fatal("[{}:{}] {}", file, line, str);
}

void OSReport_Error(const char* fmt, ...) {
    if (!checkEnabled()) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    const auto str = FormatToString(fmt, args);
    va_end(args);

    Log.error("{}", str);
}

void OSReport_FatalError(const char* fmt, ...) {
    if (!checkEnabled()) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    const auto str = FormatToString(fmt, args);
    va_end(args);

    Log.fatal("{}", str);
}

void OSReport_Warning(const char* fmt, ...) {
    if (!checkEnabled()) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    const auto str = FormatToString(fmt, args);
    va_end(args);

    Log.warn("{}", str);
}

void OSReport_System(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    OSVAttention(fmt, args);
    va_end(args);
}

void OSVAttention(const char* fmt, va_list args) {
    if (!checkEnabled()) {
        return;
    }

    const auto str = FormatToString(fmt, args);

    Log.info("{}", str);
}

void OSAttention(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    OSVAttention(fmt, args);
    va_end(args);
}