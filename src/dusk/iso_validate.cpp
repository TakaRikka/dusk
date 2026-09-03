#include "iso_validate.hpp"

#include <borealis/disc.hpp>

#include <array>
#include <string_view>

#include "dusk/logging.h"
#include "dusk/settings.h"

namespace {

const char* verification_state_name(dusk::DiscVerificationState state) noexcept {
    switch (state) {
    case dusk::DiscVerificationState::Success:
        return "verified";
    case dusk::DiscVerificationState::HashMismatch:
        return "hash mismatch";
    case dusk::DiscVerificationState::Unknown:
    default:
        return "unknown";
    }
}

}  // namespace

namespace dusk::iso {
namespace {

constexpr auto AcceptedDiscs = std::to_array<borealis::disc::AcceptedDisc>({
    {
        .gameId = "GZ2E01",
        .expectedHash = borealis::disc::parse_xxh3_128("14e886f08e548a000afde98a3195e788"),
    },
    {
        .gameId = "GZ2J01",
        .expectedHash = borealis::disc::parse_xxh3_128("5967dc7a6a553652f4d2050aeef6f368"),
    },
    {
        .gameId = "GZ2P01",
        .expectedHash = borealis::disc::parse_xxh3_128("9ef597588b0035ca9e91b333fa9a8a7e"),
    },
    {
        .gameId = "RZDE01", .revision = 0,
        .expectedHash = borealis::disc::parse_xxh3_128("b3d91fbea59e5c66934d04c01566728e"),
    },
    {
        .gameId = "RZDE01", .revision = 2,
        .expectedHash = borealis::disc::parse_xxh3_128("c3ec420921a1b36d6ae43f576491d25c"),
    },
    {
        .gameId = "RZDJ01",
        .expectedHash = borealis::disc::parse_xxh3_128("d3866821c7fc6999e6e8bbef8b6875aa"),
    },
    {
        .gameId = "RZDP01",
        .expectedHash = borealis::disc::parse_xxh3_128("6095a924a57e5fb4294ac96fb85a09a1"),
    },
});

constexpr auto RecognizedGameIds = std::to_array<std::string_view>({"RZDK01"});

constexpr borealis::disc::Catalog DiscCatalog{
    .acceptedDiscs = AcceptedDiscs,
    .recognizedGameIds = RecognizedGameIds,
};

ValidationError validation_error(borealis::disc::Status status) noexcept {
    switch (status) {
    case borealis::disc::Status::Success:
        return ValidationError::Success;
    case borealis::disc::Status::IOError:
        return ValidationError::IOError;
    case borealis::disc::Status::InvalidImage:
        return ValidationError::InvalidImage;
    case borealis::disc::Status::UnknownGame:
        return ValidationError::WrongGame;
    case borealis::disc::Status::UnsupportedVersion:
        return ValidationError::WrongVersion;
    case borealis::disc::Status::Canceled:
        return ValidationError::Canceled;
    case borealis::disc::Status::HashMismatch:
        return ValidationError::HashMismatch;
    case borealis::disc::Status::Failed:
    default:
        return ValidationError::Unknown;
    }
}

Region region_from_game_id(std::string_view gameId) noexcept {
    if (gameId.size() >= 4) {
        switch (gameId[3]) {
        case 'P':
            return Region::Europe;
        case 'J':
            return Region::Japan;
        case 'K':
            return Region::Korea;
        default:
            break;
        }
    }
    return Region::NorthAmerica;
}

void update_info(const borealis::disc::Result& result, DiscInfo& info) noexcept {
    if (!result.metadata.gameId.empty()) {
        info.platform = result.metadata.platform;
        info.region = region_from_game_id(result.metadata.gameId);
        info.revision = result.metadata.revision;
    }
}

}  // namespace

ValidationError validate(const char* path, VerificationStatus& status, DiscInfo& info) {
    const auto result = borealis::disc::verify(
        path == nullptr ? std::string_view{} : std::string_view{path}, DiscCatalog, &status);
    update_info(result, info);
    return validation_error(result.status);
}

ValidationError inspect(const char* path, DiscInfo& info) {
    const auto result = borealis::disc::inspect(
        path == nullptr ? std::string_view{} : std::string_view{path}, DiscCatalog);
    update_info(result, info);
    return validation_error(result.status);
}

bool isPal(const char* path) {
    DiscInfo info{};
    return inspect(path, info) == ValidationError::Success && info.region == Region::Europe;
}

void log_verification_state(std::string_view path, DiscVerificationState state) {
    const std::string pathText = path.empty() ? "<none>" : std::string(path);
    DuskLog.info(
        "Disc verification status: {} (path: {})", verification_state_name(state), pathText);
}
}  // namespace dusk::iso
