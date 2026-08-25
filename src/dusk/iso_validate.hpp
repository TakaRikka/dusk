#ifndef DUSK_ISO_VALIDATE_HPP
#define DUSK_ISO_VALIDATE_HPP

#include "dusk/settings.h"
#include <borealis/disc.hpp>

#include <cstdint>
#include <string>
#include <string_view>

namespace dusk {
enum class DiscVerificationState : uint8_t;
}

namespace dusk::iso {

enum class ValidationError : uint8_t {
    Unknown = 0,
    IOError,
    InvalidImage,
    WrongGame,
    WrongVersion,
    Canceled,
    HashMismatch,
    Success
};

using Platform = borealis::disc::Platform;

enum class Region : uint8_t {
    NorthAmerica,
    Europe,
    Japan,
    Korea,
};

using VerificationStatus = borealis::disc::Progress;

struct DiscInfo {
    Platform platform = Platform::Unknown;
    Region region = Region::NorthAmerica;
    std::uint8_t revision = 0;
};

// The accepted disc ids as a JSON array literal, for the uploader page's pre-flight check.
// Derived from the same AcceptedDiscs table dusk::iso::validate uses, so the catalog stays
// single-sourced -- a disc the app accepts can never be one the page rejects.
std::string accepted_game_ids_json();

ValidationError inspect(const char* path, DiscInfo& info);
ValidationError validate(const char* path, VerificationStatus& status, DiscInfo& info);
bool isPal(const char* path);
void log_verification_state(std::string_view path, DiscVerificationState state);

}  // namespace dusk::iso

#endif  // DUSK_ISO_VALIDATE_HPP
