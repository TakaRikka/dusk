#ifndef DUSK_TRANSFER_HTTP_PARSE_HPP
#define DUSK_TRANSFER_HTTP_PARSE_HPP

// Pure HTTP/1.1 request-head parsing. Deliberately strict and minimal: this server answers four
// endpoints on a LAN and never needs keep-alive negotiation, chunked request bodies, or content
// negotiation. Anything unexpected is rejected rather than interpreted.
//
// No sockets, no filesystem: tests/transfer builds this standalone on the host.

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace dusk::transfer {

// A request head larger than this is refused rather than buffered, so a hostile client cannot make
// the app allocate without bound before a single route has been matched.
inline constexpr std::size_t kMaxHeadBytes = 8192;

enum class ParseStatus : std::uint8_t {
    Incomplete,   // no CRLFCRLF yet; read more and call again
    Ok,
    Malformed,    // syntactically broken request line or headers
    Unsupported,  // valid HTTP we deliberately do not implement (e.g. chunked bodies)
    TooLarge,     // head exceeded kMaxHeadBytes without terminating
};

struct RequestHead {
    std::string method;
    std::string path;
    std::vector<std::pair<std::string, std::string>> query;
    std::uint64_t contentLength = 0;
    std::size_t headLength = 0;  // bytes consumed by the head; the body starts at this offset
};

struct ParseResult {
    ParseStatus status = ParseStatus::Incomplete;
    RequestHead head;
};

ParseResult parse_request_head(std::string_view buffer) noexcept;

// Returns an empty view when the key is absent. On repeated keys the first wins.
std::string_view query_value(const RequestHead& head, std::string_view key) noexcept;

}  // namespace dusk::transfer

#endif  // DUSK_TRANSFER_HTTP_PARSE_HPP
