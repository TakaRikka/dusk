#ifndef DUSK_ISO_VALIDATE_HPP
#define DUSK_ISO_VALIDATE_HPP

#include <atomic>

namespace dusk::iso {
struct KnownDisc;

enum class ValidationError : u8 {
    Unknown = 0,
    IOError,
    InvalidImage,
    WrongGame,
    WrongVersion,
    Canceled,
    HashMismatch,
    Success
};

struct VerificationStatus {
    std::atomic_size_t bytesRead = 0;
    std::atomic_size_t bytesTotal = 0;
    const KnownDisc* knownDisc = nullptr;
    std::atomic_bool shouldCancel = false;
};

struct DiscInfo {
    bool isPal = false;
};

ValidationError inspect(const char* path, DiscInfo& info);
ValidationError validate(const char* path, VerificationStatus& status);
bool isPal(const char* path);

}  // namespace dusk::iso

#endif  // DUSK_ISO_VALIDATE_HPP
