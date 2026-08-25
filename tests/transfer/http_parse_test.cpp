#include "dusk/transfer/http_parse.hpp"

#include <cstdio>
#include <string>

using namespace dusk::transfer;

#define CHECK(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); return 1; } } while (0)

int main() {
    // A complete GET with a query string.
    {
        const std::string raw = "GET /status?name=tp.iso&size=1234 HTTP/1.1\r\nHost: x\r\n\r\n";
        auto r = parse_request_head(raw);
        CHECK(r.status == ParseStatus::Ok);
        CHECK(r.head.method == "GET");
        CHECK(r.head.path == "/status");
        CHECK(query_value(r.head, "name") == "tp.iso");
        CHECK(query_value(r.head, "size") == "1234");
        CHECK(query_value(r.head, "absent").empty());
        CHECK(r.head.contentLength == 0);
        CHECK(r.head.headLength == raw.size());
    }

    // Percent-decoding: filenames routinely contain spaces and brackets.
    {
        auto r = parse_request_head("GET /status?name=The%20Game%20%28Europe%29.iso HTTP/1.1\r\n\r\n");
        CHECK(r.status == ParseStatus::Ok);
        CHECK(query_value(r.head, "name") == "The Game (Europe).iso");
    }

    // A split buffer must report Incomplete, never a partial parse.
    {
        auto r = parse_request_head("POST /chunk?id=ab HTTP/1.1\r\nContent-Length: 10\r\n");
        CHECK(r.status == ParseStatus::Incomplete);
    }

    // Content-Length is read case-insensitively and drives body framing.
    {
        const std::string raw = "POST /chunk?id=ab&offset=0 HTTP/1.1\r\ncOnTeNt-LeNgTh: 4096\r\n\r\n";
        auto r = parse_request_head(raw);
        CHECK(r.status == ParseStatus::Ok);
        CHECK(r.head.contentLength == 4096);
        CHECK(r.head.headLength == raw.size());
    }

    // Chunked request bodies are valid HTTP that we deliberately do not implement. Accepting the
    // head and then reading the body as raw bytes would silently corrupt the upload.
    {
        auto r = parse_request_head("POST /chunk HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n");
        CHECK(r.status == ParseStatus::Unsupported);
    }

    // Methods we do not serve.
    {
        auto r = parse_request_head("DELETE /chunk HTTP/1.1\r\n\r\n");
        CHECK(r.status == ParseStatus::Unsupported);
    }

    // Garbage request line.
    {
        auto r = parse_request_head("not-http-at-all\r\n\r\n");
        CHECK(r.status == ParseStatus::Malformed);
    }

    // An unterminated head past the cap is refused rather than buffered forever.
    {
        std::string big = "GET /";
        big.append(kMaxHeadBytes + 16, 'a');
        auto r = parse_request_head(big);
        CHECK(r.status == ParseStatus::TooLarge);
    }

    // A body may already be sitting in the buffer; headLength must point at its first byte.
    {
        const std::string raw = "POST /chunk?id=ab&offset=0 HTTP/1.1\r\nContent-Length: 3\r\n\r\nabc";
        auto r = parse_request_head(raw);
        CHECK(r.status == ParseStatus::Ok);
        CHECK(raw.substr(r.head.headLength) == "abc");
    }

    std::puts("http_parse_test OK");
    return 0;
}
