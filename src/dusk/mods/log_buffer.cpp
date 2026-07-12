#include "log_buffer.hpp"

#include <chrono>
#include <mutex>

#include "dusk/logging.h"

namespace dusk::mods::log {
namespace {

struct BufferState {
    std::mutex mutex;
    std::vector<Line> ring;
    size_t head = 0;
    size_t count = 0;
    uint64_t nextSeq = 0;
    std::vector<std::string> modIds;

    uint16_t intern(std::string_view modId) {
        for (size_t i = 0; i < modIds.size(); ++i) {
            if (modIds[i] == modId) {
                return static_cast<uint16_t>(i);
            }
        }
        modIds.emplace_back(modId);
        return static_cast<uint16_t>(modIds.size() - 1);
    }
};
BufferState g_buffer;

}  // namespace

void emit(Source source, const std::string& modId, LogLevel level, const std::string& message) {
    const auto timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
                            .count();

    {
        std::lock_guard lock{g_buffer.mutex};
        if (g_buffer.ring.empty()) {
            g_buffer.ring.resize(k_capacity);
        }
        auto& slot = g_buffer.ring[(g_buffer.head + g_buffer.count) % k_capacity];
        if (g_buffer.count == k_capacity) {
            g_buffer.head = (g_buffer.head + 1) % k_capacity;
        } else {
            ++g_buffer.count;
        }
        slot.seq = g_buffer.nextSeq++;
        slot.timeMs = timeMs;
        slot.level = level;
        slot.modIndex = g_buffer.intern(modId);
        slot.source = source;
        // assign() reuses the slot's capacity once the ring has wrapped
        slot.message.assign(message);
    }

    AuroraLogLevel auroraLevel = LOG_INFO;
    switch (level) {
    case LOG_LEVEL_TRACE:
    case LOG_LEVEL_DEBUG:
        auroraLevel = LOG_DEBUG;
        break;
    case LOG_LEVEL_INFO:
        auroraLevel = LOG_INFO;
        break;
    case LOG_LEVEL_WARN:
        auroraLevel = LOG_WARNING;
        break;
    case LOG_LEVEL_ERROR:
        auroraLevel = LOG_ERROR;
        break;
    }
    if (aurora::g_config.logLevel <= auroraLevel) {
        aurora::log_internal(auroraLevel, modId.c_str(), message.c_str(),
            static_cast<unsigned int>(message.length()));
    }
}

Range copy_since(uint64_t sinceSeq, std::vector<Line>& out) {
    std::lock_guard lock{g_buffer.mutex};
    const Range range{
        .firstSeq = g_buffer.nextSeq - g_buffer.count,
        .nextSeq = g_buffer.nextSeq,
    };
    for (uint64_t seq = std::max(sinceSeq, range.firstSeq); seq < range.nextSeq; ++seq) {
        out.push_back(g_buffer.ring[(g_buffer.head + (seq - range.firstSeq)) % k_capacity]);
    }
    return range;
}

std::vector<std::string> ids() {
    std::lock_guard lock{g_buffer.mutex};
    return g_buffer.modIds;
}

void clear() {
    std::lock_guard lock{g_buffer.mutex};
    g_buffer.head = 0;
    g_buffer.count = 0;
}

}  // namespace dusk::mods::log
