#ifndef DUSK_ISO_VALIDATE_HPP
#define DUSK_ISO_VALIDATE_HPP

namespace dusk::iso {
    enum class ValidationError : u8 {
        Success = 0,
        IOError,
        InvalidImage,
        WrongGame,
        WrongVersion,
        ExecutableMismatch,
        Unknown
    };

    ValidationError validate(const char* path);
}

#endif  // DUSK_ISO_VALIDATE_HPP
