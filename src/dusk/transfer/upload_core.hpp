#ifndef DUSK_TRANSFER_UPLOAD_CORE_HPP
#define DUSK_TRANSFER_UPLOAD_CORE_HPP

// Pure upload rules. No sockets, no filesystem, no clock: every input is passed in, so
// tests/transfer can build this standalone on the host and drive every branch deterministically.

#include <cstdint>
#include <string>
#include <string_view>

namespace dusk::transfer {

inline constexpr std::uint64_t kChunkBytes = 8ull * 1024 * 1024;
inline constexpr std::int64_t kStaleAfterSeconds = 7 * 24 * 60 * 60;

// FNV-1a over name + '\0' + decimal size, lowercase hex. Derived rather than server-assigned so a
// resume works from a cold start with no client-side state; hex-encoded so the result is always
// safe as a filename, which is what keeps a hostile `name` out of the filesystem entirely.
std::string derive_upload_id(std::string_view name, std::uint64_t size);

enum class ChunkVerdict : std::uint8_t {
    Accept,
    OffsetMismatch,  // not the next byte we expect
    Overrun,         // would write past the declared size
    Inactive,        // no upload in progress
    WrongUpload,     // id belongs to a different upload
};

struct UploadState {
    std::string id;
    std::string name;
    std::uint64_t declaredSize = 0;
    std::uint64_t received = 0;
    bool active = false;
};

// The load-bearing rule. A chunk whose response was lost in flight gets retried by the client; if
// the server appended it again the file would be silently corrupt. Requiring offset == received
// makes a replay a refusal instead, and the client re-syncs from /status.
ChunkVerdict judge_chunk(const UploadState& state, std::string_view id,
                         std::uint64_t offset, std::uint64_t length) noexcept;

bool is_stale(std::int64_t stagedAtUnix, std::int64_t nowUnix,
              std::int64_t maxAgeSeconds = kStaleAfterSeconds) noexcept;

// Directory components stripped, restricted to [A-Za-z0-9 ._-], leading dots removed, stem
// truncated to 100 characters, original extension preserved because discovery matches on it.
// Falls back to "<fallbackId><ext>" when nothing survives.
std::string sanitize_publish_name(std::string_view rawName, std::string_view fallbackId);

}  // namespace dusk::transfer

#endif  // DUSK_TRANSFER_UPLOAD_CORE_HPP
