#ifndef DUSK_ISO_VALIDATE_HPP
#define DUSK_ISO_VALIDATE_HPP

#include <xxh3.h>

namespace dusk::iso {
    struct KnownDisc;

    enum class ValidationError : u8 {
        Unknown = 0,
        IOError,
        InvalidImage,
        WrongGame,
        WrongVersion,
        HashMismatch,
        Success
    };

    struct VerificationStatus {
        size_t bytesRead = 0;
        size_t bytesTotal = 0;
        const KnownDisc* knownDisc = nullptr;
        bool shouldCancel = false;
    };

    ValidationError validate(const char* path, VerificationStatus& status);
    ValidationError validate(const char* path);
    bool isPal(const char* path);
}

#endif  // DUSK_ISO_VALIDATE_HPP
