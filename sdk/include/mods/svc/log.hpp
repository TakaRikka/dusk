#pragma once

#if !defined(DUSK_BUILDING_GAME) && !defined(DUSK_MOD_FEATURE_FMT)
#error "mods/svc/log.hpp requires add_mod(... FEATURES fmt)"
#endif

#include <mods/svc/log.h>

#include <fmt/format.h>

#include <utility>

namespace mods::log {

template <typename... Args>
void write(LogLevel level, fmt::format_string<Args...> formatString, Args&&... args) {
    const auto message = fmt::format(formatString, std::forward<Args>(args)...);
    svc_log->write(mod_ctx, level, message.c_str());
}

template <typename... Args>
void trace(fmt::format_string<Args...> formatString, Args&&... args) {
    write(LOG_LEVEL_TRACE, formatString, std::forward<Args>(args)...);
}

template <typename... Args>
void debug(fmt::format_string<Args...> formatString, Args&&... args) {
    write(LOG_LEVEL_DEBUG, formatString, std::forward<Args>(args)...);
}

template <typename... Args>
void info(fmt::format_string<Args...> formatString, Args&&... args) {
    write(LOG_LEVEL_INFO, formatString, std::forward<Args>(args)...);
}

template <typename... Args>
void warn(fmt::format_string<Args...> formatString, Args&&... args) {
    write(LOG_LEVEL_WARN, formatString, std::forward<Args>(args)...);
}

template <typename... Args>
void error(fmt::format_string<Args...> formatString, Args&&... args) {
    write(LOG_LEVEL_ERROR, formatString, std::forward<Args>(args)...);
}

}  // namespace mods::log
