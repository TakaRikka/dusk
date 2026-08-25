#ifndef DUSK_TRANSFER_SERVER_HPP
#define DUSK_TRANSFER_SERVER_HPP

// A minimal HTTP/1.1 server that receives one disc image at a time and publishes it into the
// directory disc discovery scans. Validation is injected rather than called directly so this file
// stays free of dusk::iso and can be exercised on the host.

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

#include "dusk/transfer/upload_core.hpp"

namespace dusk::transfer {

struct ValidateOutcome {
    bool ok = false;
    // Kept when we could not determine the file is wrong (an I/O failure or a cancel), so the user
    // can retry validation without re-uploading. Discarded on a definitive rejection.
    bool keepStaging = false;
    std::string reason;
};

using Validator = std::function<ValidateOutcome(const std::filesystem::path&)>;

enum class Phase : std::uint8_t { Idle, Receiving, Validating, Failed, Published };

struct Progress {
    Phase phase = Phase::Idle;
    std::uint64_t received = 0;
    std::uint64_t total = 0;
    std::string message;
    std::string publishedPath;
};

struct ServerConfig {
    std::filesystem::path discsDir;      // <data dir>/discs
    std::uint16_t portFirst = 8080;
    std::uint16_t portLast = 8090;
    // JSON array literal of accepted game ids, served to the uploader page so the browser can
    // pre-flight a raw .iso without duplicating the catalog.
    std::string acceptedGameIds = "[]";
};

class Server {
public:
    Server(ServerConfig config, Validator validator);
    ~Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    // Binds the first free port in [portFirst, portLast] and starts serving. False if none is free.
    bool start();
    void stop();

    std::uint16_t port() const noexcept;
    Progress progress() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace dusk::transfer

#endif  // DUSK_TRANSFER_SERVER_HPP
