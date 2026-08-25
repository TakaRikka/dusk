#include "dusk/transfer/upload_core.hpp"

#include <algorithm>

namespace dusk::transfer {
namespace {

constexpr std::size_t kMaxStemBytes = 100;

bool allowed_name_char(char c) noexcept {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
           c == ' ' || c == '.' || c == '_' || c == '-';
}

}  // namespace

std::string derive_upload_id(std::string_view name, std::uint64_t size) {
    std::uint64_t hash = 1469598103934665603ull;  // FNV-1a 64 offset basis
    const auto mix = [&hash](unsigned char byte) {
        hash ^= byte;
        hash *= 1099511628211ull;  // FNV-1a 64 prime
    };
    for (const char c : name) {
        mix(static_cast<unsigned char>(c));
    }
    mix(0u);
    const std::string digits = std::to_string(size);
    for (const char c : digits) {
        mix(static_cast<unsigned char>(c));
    }

    static constexpr char kHex[] = "0123456789abcdef";
    std::string out(16, '0');
    for (int i = 15; i >= 0; --i) {
        out[static_cast<std::size_t>(i)] = kHex[hash & 0xFull];
        hash >>= 4;
    }
    return out;
}

ChunkVerdict judge_chunk(const UploadState& state, std::string_view id,
                         std::uint64_t offset, std::uint64_t length) noexcept {
    if (!state.active) {
        return ChunkVerdict::Inactive;
    }
    if (id != state.id) {
        return ChunkVerdict::WrongUpload;
    }
    if (offset != state.received) {
        return ChunkVerdict::OffsetMismatch;
    }
    if (length > state.declaredSize - state.received) {
        return ChunkVerdict::Overrun;
    }
    return ChunkVerdict::Accept;
}

bool is_stale(std::int64_t stagedAtUnix, std::int64_t nowUnix,
              std::int64_t maxAgeSeconds) noexcept {
    if (nowUnix <= stagedAtUnix) {
        // Clock skew, or a file stamped in the future. Never sweep on the strength of that.
        return false;
    }
    return (nowUnix - stagedAtUnix) > maxAgeSeconds;
}

std::string sanitize_publish_name(std::string_view rawName, std::string_view fallbackId) {
    const std::size_t slash = rawName.find_last_of("/\\");
    std::string_view base =
        (slash == std::string_view::npos) ? rawName : rawName.substr(slash + 1);

    std::string cleaned;
    cleaned.reserve(base.size());
    for (const char c : base) {
        cleaned.push_back(allowed_name_char(c) ? c : '_');
    }
    while (!cleaned.empty() && cleaned.front() == '.') {
        cleaned.erase(cleaned.begin());
    }

    std::string stem = cleaned;
    std::string ext;
    const std::size_t dot = cleaned.find_last_of('.');
    if (dot != std::string::npos) {
        stem = cleaned.substr(0, dot);
        ext = cleaned.substr(dot);
    }
    if (stem.size() > kMaxStemBytes) {
        stem.resize(kMaxStemBytes);
    }
    if (stem.empty()) {
        return std::string{fallbackId} + ext;
    }
    return stem + ext;
}

}  // namespace dusk::transfer
