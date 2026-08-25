#include "dusk/transfer/server.hpp"

#include "dusk/transfer/http_parse.hpp"
#include "dusk/transfer/uploader_page.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <ctime>
#include <charconv>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

namespace dusk::transfer {
namespace {

constexpr std::size_t kSocketBufferBytes = 64 * 1024;

std::uint64_t to_u64(std::string_view text, bool& ok) noexcept {
    std::uint64_t value = 0;
    const char* begin = text.data();
    const char* end = begin + text.size();
    const auto res = std::from_chars(begin, end, value);
    ok = (res.ec == std::errc{} && res.ptr == end && !text.empty());
    return value;
}

// A single send() may write only part of the buffer, so every response goes through this.
bool send_all(int fd, std::string_view data) noexcept {
    std::size_t sent = 0;
    while (sent < data.size()) {
        const ssize_t n = ::send(fd, data.data() + sent, data.size() - sent, 0);
        if (n <= 0) {
            return false;
        }
        sent += static_cast<std::size_t>(n);
    }
    return true;
}

void send_response(int fd, int status, std::string_view reason, std::string_view contentType,
                   std::string_view body) noexcept {
    std::string head = "HTTP/1.1 " + std::to_string(status) + " " + std::string{reason} + "\r\n";
    head += "Content-Type: " + std::string{contentType} + "\r\n";
    head += "Content-Length: " + std::to_string(body.size()) + "\r\n";
    head += "Cache-Control: no-store\r\n";
    head += "Connection: close\r\n\r\n";
    if (send_all(fd, head)) {
        send_all(fd, body);
    }
}

void send_json(int fd, int status, std::string_view reason, const std::string& json) noexcept {
    send_response(fd, status, reason, "application/json", json);
}

std::string json_escape(std::string_view in) {
    std::string out;
    out.reserve(in.size() + 8);
    for (const char c : in) {
        switch (c) {
        case '"':  out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\n': out += "\\n";  break;
        case '\r': out += "\\r";  break;
        case '\t': out += "\\t";  break;
        default:
            if (static_cast<unsigned char>(c) < 0x20) {
                out += ' ';
            } else {
                out += c;
            }
        }
    }
    return out;
}

std::int64_t file_mtime_unix(const fs::path& path) noexcept {
    struct ::stat st {};
    if (::stat(path.c_str(), &st) != 0) {
        return 0;
    }
    return static_cast<std::int64_t>(st.st_mtime);
}

}  // namespace

struct Server::Impl {
    ServerConfig config;
    Validator validator;
    fs::path stagingDir;

    int listenFd = -1;
    std::uint16_t port = 0;
    std::atomic<bool> running{false};
    std::thread thread;

    mutable std::mutex mutex;
    UploadState upload;
    Progress prog;

    explicit Impl(ServerConfig cfg, Validator v)
        : config(std::move(cfg)), validator(std::move(v)),
          stagingDir(config.discsDir / ".incoming") {}

    fs::path staging_path(std::string_view id) const { return stagingDir / std::string{id}; }

    // Staged files live in purgeable storage like everything else, and an upload that is never
    // finalized would otherwise sit there forever. Swept on start, when nothing can be in flight.
    void sweep_staging() {
        std::error_code ec;
        const auto now = static_cast<std::int64_t>(::time(nullptr));
        for (fs::directory_iterator it{stagingDir, ec}, end; !ec && it != end; it.increment(ec)) {
            std::error_code fileEc;
            if (!it->is_regular_file(fileEc)) {
                continue;
            }
            if (is_stale(file_mtime_unix(it->path()), now)) {
                fs::remove(it->path(), fileEc);
            }
        }
    }

    void set_phase(Phase phase, std::string message = {}) {
        std::lock_guard<std::mutex> lock(mutex);
        prog.phase = phase;
        prog.message = std::move(message);
    }

    void handle_root(int fd) {
        std::string page{kUploaderPage};
        const std::string token = "%%ACCEPTED_GAME_IDS%%";
        for (std::size_t at = page.find(token); at != std::string::npos;
             at = page.find(token, at + config.acceptedGameIds.size())) {
            page.replace(at, token.size(), config.acceptedGameIds);
        }
        send_response(fd, 200, "OK", "text/html; charset=utf-8", page);
    }

    void handle_status(int fd, const RequestHead& head) {
        const std::string name{query_value(head, "name")};
        bool ok = false;
        const std::uint64_t size = to_u64(query_value(head, "size"), ok);
        if (name.empty() || !ok || size == 0) {
            send_json(fd, 400, "Bad Request", R"({"error":"name and size are required"})");
            return;
        }
        const std::string id = derive_upload_id(name, size);

        std::lock_guard<std::mutex> lock(mutex);
        // A second browser must not be able to retarget an upload that is already under way; two
        // files interleaved into one staging path would produce a corrupt image with no symptom
        // until validation.
        if (upload.active && upload.id != id && upload.received > 0) {
            send_json(fd, 409, "Conflict", R"({"busy":true})");
            return;
        }

        std::error_code ec;
        const auto staged = staging_path(id);
        const auto existing = fs::file_size(staged, ec);
        const std::uint64_t received = ec ? 0ull : static_cast<std::uint64_t>(existing);

        upload.id = id;
        upload.name = name;
        upload.declaredSize = size;
        upload.received = received;
        upload.active = true;
        prog.phase = (received > 0) ? Phase::Receiving : Phase::Idle;
        prog.received = received;
        prog.total = size;

        send_json(fd, 200, "OK",
                  "{\"id\":\"" + id + "\",\"received\":" + std::to_string(received) +
                      ",\"size\":" + std::to_string(size) + ",\"active\":true}");
    }

    // Streams the request body onto the end of the staging file. `prefix` is whatever of the body
    // already arrived in the head buffer.
    void handle_chunk(int fd, const RequestHead& head, std::string_view prefix) {
        const std::string id{query_value(head, "id")};
        bool ok = false;
        const std::uint64_t offset = to_u64(query_value(head, "offset"), ok);
        if (id.empty() || !ok) {
            send_json(fd, 400, "Bad Request", R"({"error":"id and offset are required"})");
            return;
        }

        ChunkVerdict verdict;
        fs::path staged;
        std::uint64_t received = 0;
        {
            std::lock_guard<std::mutex> lock(mutex);
            verdict = judge_chunk(upload, id, offset, head.contentLength);
            staged = staging_path(upload.id);
            received = upload.received;
        }
        switch (verdict) {
        case ChunkVerdict::Accept:
            break;
        case ChunkVerdict::Overrun:
            send_json(fd, 400, "Bad Request", R"({"error":"chunk exceeds the declared size"})");
            return;
        case ChunkVerdict::OffsetMismatch:
        case ChunkVerdict::WrongUpload:
        case ChunkVerdict::Inactive:
        default:
            // Hand back the truth so the client re-syncs instead of guessing.
            send_json(fd, 409, "Conflict",
                      "{\"received\":" + std::to_string(received) + "}");
            return;
        }

        const int out = ::open(staged.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (out < 0) {
            send_json(fd, 500, "Internal Server Error", R"({"error":"cannot open staging file"})");
            return;
        }

        std::uint64_t written = 0;
        bool noSpace = false;
        bool failed = false;

        const auto write_all = [&](const char* data, std::size_t len) {
            std::size_t done = 0;
            while (done < len) {
                const ssize_t n = ::write(out, data + done, len - done);
                if (n < 0) {
                    if (errno == EINTR) {
                        continue;
                    }
                    noSpace = (errno == ENOSPC);
                    failed = true;
                    return false;
                }
                done += static_cast<std::size_t>(n);
            }
            return true;
        };

        if (!prefix.empty()) {
            const std::size_t take = std::min<std::size_t>(prefix.size(), head.contentLength);
            if (write_all(prefix.data(), take)) {
                written += take;
            }
        }

        std::vector<char> buffer(kSocketBufferBytes);
        while (!failed && written < head.contentLength) {
            const std::size_t want =
                std::min<std::uint64_t>(buffer.size(), head.contentLength - written);
            const ssize_t n = ::recv(fd, buffer.data(), want, 0);
            if (n <= 0) {
                // The client vanished mid-body. Keep what landed; /status will report it and the
                // client resumes from there.
                break;
            }
            if (!write_all(buffer.data(), static_cast<std::size_t>(n))) {
                break;
            }
            written += static_cast<std::uint64_t>(n);
        }
        ::close(out);

        {
            std::lock_guard<std::mutex> lock(mutex);
            upload.received += written;
            prog.received = upload.received;
            prog.total = upload.declaredSize;
            if (prog.phase == Phase::Idle) {
                prog.phase = Phase::Receiving;
            }
            received = upload.received;
        }

        if (noSpace) {
            set_phase(Phase::Failed, "Not enough space on the Apple TV.");
            send_json(fd, 507, "Insufficient Storage",
                      R"({"error":"not enough space on the device"})");
            return;
        }
        if (failed) {
            send_json(fd, 500, "Internal Server Error", R"({"error":"write failed"})");
            return;
        }
        if (written < head.contentLength) {
            send_json(fd, 400, "Bad Request",
                      "{\"received\":" + std::to_string(received) + "}");
            return;
        }
        send_json(fd, 200, "OK", "{\"received\":" + std::to_string(received) + "}");
    }

    void handle_finalize(int fd, const RequestHead& head) {
        const std::string id{query_value(head, "id")};
        std::string name;
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (!upload.active || upload.id != id) {
                send_json(fd, 409, "Conflict", R"({"ok":false,"reason":"no such upload"})");
                return;
            }
            name = upload.name;
        }

        set_phase(Phase::Validating, "Verifying the disc image...");
        const fs::path staged = staging_path(id);
        const ValidateOutcome outcome = validator(staged);

        std::error_code ec;
        if (!outcome.ok) {
            if (!outcome.keepStaging) {
                fs::remove(staged, ec);
                std::lock_guard<std::mutex> lock(mutex);
                upload = UploadState{};
            }
            set_phase(Phase::Failed, outcome.reason);
            send_json(fd, 200, "OK",
                      "{\"ok\":false,\"reason\":\"" + json_escape(outcome.reason) + "\"}");
            return;
        }

        const fs::path published = config.discsDir / sanitize_publish_name(name, id);
        fs::rename(staged, published, ec);
        if (ec) {
            set_phase(Phase::Failed, "Could not move the verified image into place.");
            send_json(fd, 500, "Internal Server Error",
                      R"({"ok":false,"reason":"could not publish the image"})");
            return;
        }

        {
            std::lock_guard<std::mutex> lock(mutex);
            upload = UploadState{};
            prog.phase = Phase::Published;
            prog.message = "Disc added.";
            prog.publishedPath = published.string();
        }
        send_json(fd, 200, "OK", R"({"ok":true,"reason":"verified"})");
    }

    void handle_connection(int fd) {
        std::string buffer;
        ParseResult parsed;
        std::vector<char> chunk(kSocketBufferBytes);
        for (;;) {
            parsed = parse_request_head(buffer);
            if (parsed.status != ParseStatus::Incomplete) {
                break;
            }
            if (buffer.size() > kMaxHeadBytes) {
                parsed.status = ParseStatus::TooLarge;
                break;
            }
            const ssize_t n = ::recv(fd, chunk.data(), chunk.size(), 0);
            if (n <= 0) {
                return;  // client went away before sending a complete head
            }
            buffer.append(chunk.data(), static_cast<std::size_t>(n));
        }

        switch (parsed.status) {
        case ParseStatus::Malformed:
            send_response(fd, 400, "Bad Request", "text/plain", "bad request");
            return;
        case ParseStatus::Unsupported:
            send_response(fd, 501, "Not Implemented", "text/plain", "unsupported request");
            return;
        case ParseStatus::TooLarge:
            send_response(fd, 431, "Request Header Fields Too Large", "text/plain", "head too large");
            return;
        default:
            break;
        }

        const RequestHead& head = parsed.head;
        const std::string_view body{buffer.data() + head.headLength,
                                    buffer.size() - head.headLength};

        if (head.method == "GET" && head.path == "/") {
            handle_root(fd);
        } else if (head.method == "GET" && head.path == "/status") {
            handle_status(fd, head);
        } else if (head.method == "POST" && head.path == "/chunk") {
            handle_chunk(fd, head, body);
        } else if (head.method == "POST" && head.path == "/finalize") {
            handle_finalize(fd, head);
        } else {
            send_response(fd, 404, "Not Found", "text/plain", "not found");
        }
    }

    void serve() {
        while (running.load(std::memory_order_relaxed)) {
            const int fd = ::accept(listenFd, nullptr, nullptr);
            if (fd < 0) {
                if (errno == EINTR) {
                    continue;
                }
                return;  // listener closed by stop()
            }
#ifdef SO_NOSIGPIPE
            // A browser closing mid-response would otherwise raise SIGPIPE and kill the app.
            int on = 1;
            ::setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
#endif
            handle_connection(fd);
            ::close(fd);
        }
    }
};

Server::Server(ServerConfig config, Validator validator)
    : impl_(std::make_unique<Impl>(std::move(config), std::move(validator))) {}

Server::~Server() { stop(); }

bool Server::start() {
    std::error_code ec;
    fs::create_directories(impl_->stagingDir, ec);
    if (ec) {
        return false;
    }
    impl_->sweep_staging();

    for (std::uint16_t port = impl_->config.portFirst; port <= impl_->config.portLast; ++port) {
        const int fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            continue;
        }
        int on = 1;
        ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);
        if (::bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0 ||
            ::listen(fd, 4) != 0) {
            ::close(fd);
            continue;
        }
        impl_->listenFd = fd;
        impl_->port = port;
        impl_->running.store(true, std::memory_order_relaxed);
        impl_->thread = std::thread([impl = impl_.get()] { impl->serve(); });
        return true;
    }
    return false;
}

void Server::stop() {
    if (!impl_ || !impl_->running.exchange(false)) {
        return;
    }
    if (impl_->listenFd >= 0) {
        // Break the blocking accept() so the thread can observe running == false.
        ::shutdown(impl_->listenFd, SHUT_RDWR);
        ::close(impl_->listenFd);
        impl_->listenFd = -1;
    }
    if (impl_->thread.joinable()) {
        impl_->thread.join();
    }
}

std::uint16_t Server::port() const noexcept { return impl_ ? impl_->port : 0; }

Progress Server::progress() const {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    return impl_->prog;
}

}  // namespace dusk::transfer
