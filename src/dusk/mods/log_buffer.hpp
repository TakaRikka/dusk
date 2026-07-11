#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>

#include "mods/svc/log.h"

namespace dusk::mods::log {

constexpr size_t k_capacity = 2048;

enum class Source : uint8_t {
    Loader,
    Mod,
};

struct Line {
    uint64_t seq = 0;  // monotonic, never reset (survives clear)
    int64_t timeMs = 0;
    LogLevel level = LOG_LEVEL_INFO;
    uint16_t modIndex = 0;  // index into ids()
    Source source = Source::Loader;
    std::string message;
};

struct Range {
    uint64_t firstSeq = 0;  // oldest retained entry
    uint64_t nextSeq = 0;   // one past the newest entry
};

// Appends to the mod log buffer and emits through aurora logging with the mod ID as the
// module tag. Console/file output honors the configured log level; the buffer captures
// every level. Thread-safe; mods may log from worker threads.
void emit(Source source, const std::string& modId, LogLevel level, const std::string& message);

// Appends entries with seq >= sinceSeq to `out` and returns the retained range,
// so callers diffing by seq can detect both new entries and truncation.
Range copy_since(uint64_t sinceSeq, std::vector<Line>& out);

// Append-only intern table of mod ids; indices are stable for the session.
std::vector<std::string> ids();

void clear();

// Formatting helper for loader messages.
template <typename... T>
void write(const std::string& modId, LogLevel level, fmt::format_string<T...> format, T&&... args) {
    emit(Source::Loader, modId, level, fmt::format(format, std::forward<T>(args)...));
}

}  // namespace dusk::mods::log
