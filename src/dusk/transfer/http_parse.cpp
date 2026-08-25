#include "dusk/transfer/http_parse.hpp"

#include <algorithm>
#include <cctype>

namespace dusk::transfer {
namespace {

bool iequals(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size()) {
        return false;
    }
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

int hex_value(char c) noexcept {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Percent-decoding, with '+' meaning space as browsers encode query strings. An invalid escape is
// passed through verbatim rather than dropped, so a malformed name never silently becomes a
// different valid name.
std::string percent_decode(std::string_view in) {
    std::string out;
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '+') {
            out.push_back(' ');
        } else if (in[i] == '%' && i + 2 < in.size()) {
            const int hi = hex_value(in[i + 1]);
            const int lo = hex_value(in[i + 2]);
            if (hi >= 0 && lo >= 0) {
                out.push_back(static_cast<char>((hi << 4) | lo));
                i += 2;
            } else {
                out.push_back(in[i]);
            }
        } else {
            out.push_back(in[i]);
        }
    }
    return out;
}

void parse_query(std::string_view query, RequestHead& head) {
    while (!query.empty()) {
        const std::size_t amp = query.find('&');
        std::string_view pair = query.substr(0, amp);
        query = (amp == std::string_view::npos) ? std::string_view{} : query.substr(amp + 1);
        if (pair.empty()) {
            continue;
        }
        const std::size_t eq = pair.find('=');
        if (eq == std::string_view::npos) {
            head.query.emplace_back(percent_decode(pair), std::string{});
        } else {
            head.query.emplace_back(percent_decode(pair.substr(0, eq)),
                                    percent_decode(pair.substr(eq + 1)));
        }
    }
}

}  // namespace

ParseResult parse_request_head(std::string_view buffer) noexcept {
    ParseResult result;
    const std::size_t end = buffer.find("\r\n\r\n");
    if (end == std::string_view::npos) {
        result.status = (buffer.size() > kMaxHeadBytes) ? ParseStatus::TooLarge
                                                        : ParseStatus::Incomplete;
        return result;
    }
    if (end + 4 > kMaxHeadBytes) {
        result.status = ParseStatus::TooLarge;
        return result;
    }

    std::string_view head = buffer.substr(0, end);
    const std::size_t lineEnd = head.find("\r\n");
    std::string_view requestLine = head.substr(0, lineEnd);
    std::string_view rest = (lineEnd == std::string_view::npos)
        ? std::string_view{} : head.substr(lineEnd + 2);

    const std::size_t sp1 = requestLine.find(' ');
    if (sp1 == std::string_view::npos) {
        result.status = ParseStatus::Malformed;
        return result;
    }
    const std::size_t sp2 = requestLine.find(' ', sp1 + 1);
    if (sp2 == std::string_view::npos) {
        result.status = ParseStatus::Malformed;
        return result;
    }
    if (requestLine.substr(sp2 + 1).rfind("HTTP/1.", 0) != 0) {
        result.status = ParseStatus::Malformed;
        return result;
    }

    result.head.method = std::string{requestLine.substr(0, sp1)};
    if (result.head.method != "GET" && result.head.method != "POST") {
        result.status = ParseStatus::Unsupported;
        return result;
    }

    std::string_view target = requestLine.substr(sp1 + 1, sp2 - sp1 - 1);
    const std::size_t q = target.find('?');
    if (q == std::string_view::npos) {
        result.head.path = percent_decode(target);
    } else {
        result.head.path = percent_decode(target.substr(0, q));
        parse_query(target.substr(q + 1), result.head);
    }

    while (!rest.empty()) {
        const std::size_t nl = rest.find("\r\n");
        std::string_view line = rest.substr(0, nl);
        rest = (nl == std::string_view::npos) ? std::string_view{} : rest.substr(nl + 2);
        const std::size_t colon = line.find(':');
        if (colon == std::string_view::npos) {
            result.status = ParseStatus::Malformed;
            return result;
        }
        std::string_view name = line.substr(0, colon);
        std::string_view value = line.substr(colon + 1);
        while (!value.empty() && (value.front() == ' ' || value.front() == '\t')) {
            value.remove_prefix(1);
        }
        if (iequals(name, "Transfer-Encoding")) {
            // Valid HTTP we do not implement. Reading such a body as raw bytes would append the
            // chunk-size lines into the disc image.
            result.status = ParseStatus::Unsupported;
            return result;
        }
        if (iequals(name, "Content-Length")) {
            std::uint64_t n = 0;
            if (value.empty()) {
                result.status = ParseStatus::Malformed;
                return result;
            }
            for (const char c : value) {
                if (c < '0' || c > '9') {
                    result.status = ParseStatus::Malformed;
                    return result;
                }
                n = n * 10 + static_cast<std::uint64_t>(c - '0');
            }
            result.head.contentLength = n;
        }
    }

    result.head.headLength = end + 4;
    result.status = ParseStatus::Ok;
    return result;
}

std::string_view query_value(const RequestHead& head, std::string_view key) noexcept {
    for (const auto& [k, v] : head.query) {
        if (k == key) {
            return v;
        }
    }
    return {};
}

}  // namespace dusk::transfer
