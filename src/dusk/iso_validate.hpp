#ifndef DUSK_ISO_VALIDATE_HPP
#define DUSK_ISO_VALIDATE_HPP

#include <xxh3.h>

namespace dusk::iso {
    struct KnownDisc;

    enum class ValidationError : u8 {
        Success = 0,
        IOError,
        InvalidImage,
        WrongGame,
        WrongVersion,
        Cancelled,
        DiscHashMismatch,
        Unknown
    };

    struct VerificationStatus {
        size_t bytesRead = 0;
        size_t bytesTotal = 0;
        const KnownDisc* knownDisc = nullptr;
        bool shouldCancel = false;
    };

    ValidationError validate(const char* path, VerificationStatus& status);
    bool isPal(const char* path);
}

#endif  // DUSK_ISO_VALIDATE_HPP
