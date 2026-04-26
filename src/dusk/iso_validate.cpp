#include "iso_validate.hpp"

#include <nod.h>
#include <span>

#include "SDL3/SDL_iostream.h"

namespace dusk::iso {

constexpr const char* TP_GAME_IDS[] = {
    "GZ2E01", // GCN USA
    "GZ2P01", // GCN PAL
    "GZ2J01", // GCN JPN
    "RZDE01", // Wii USA
    "RZDP01", // Wii PAL
    "RZDJ01", // Wii JPN
    "RZDK01", // Wii KOR
};

constexpr const char* PAL_GAME_IDS[] = {
    "GZ2P01", // GCN PAL
    "RZDP01", // Wii PAL
};

constexpr const char* SUPPORTED_TP_GAME_IDS[] = {
    "GZ2E01", // GCN USA
    "GZ2P01", // GCN PAL
};

template <size_t N>
constexpr bool matches(const char (&id)[6], const char* const (&valid)[N]) {
    for (auto elem : valid) {
        if (strncmp(id, elem, 6) == 0) {
            return true;
        }
    }

    return false;
}

struct NodHandleWrapper {
    NodHandle* handle;

    NodHandleWrapper() : handle(nullptr) {
    }

    ~NodHandleWrapper() {
        if (handle != nullptr) {
            nod_free(handle);
            handle = nullptr;
        }
    }
};

static ValidationError convertNodError(NodResult result) {
    switch (result) {
    case NOD_RESULT_ERR_IO:
        return ValidationError::IOError;
    case NOD_RESULT_ERR_FORMAT:
        return ValidationError::InvalidImage;
    default:
        return ValidationError::Unknown;
    }
}

s64 StreamReadAt(void* user_data, u64 offset, void* out, size_t len) {
    if (len == 0) {
        return 0;
    }

    auto io = static_cast<SDL_IOStream*>(user_data);

    auto ret = SDL_SeekIO(io, static_cast<s64>(offset), SDL_IO_SEEK_SET);
    if (ret < 0) {
        return -1;
    }

    auto read = SDL_ReadIO(io, out, len);
    if (read == 0) {
        if (SDL_GetIOStatus(io) == SDL_IO_STATUS_EOF) {
            return 0;
        }

        return -1;
    }

    return static_cast<s64>(read);
}

s64 StreamLength(void* user_data) {
    auto io = static_cast<SDL_IOStream*>(user_data);
    return SDL_GetIOSize(io);
}

void StreamClose(void* user_data) {
    auto io = static_cast<SDL_IOStream*>(user_data);
    SDL_CloseIO(io);
}

ValidationError validate(const char* path) {
    NodHandleWrapper disc;

    const auto sdlStream = SDL_IOFromFile(path, "rb");
    const NodDiscStream nod_stream {
        .user_data = sdlStream,
        .read_at = StreamReadAt,
        .stream_len = StreamLength,
        .close = StreamClose,
    };

    auto result = nod_disc_open_stream(&nod_stream, nullptr, &disc.handle);
    if (disc.handle == nullptr || result != NOD_RESULT_OK) {
        return convertNodError(result);
    }

    NodDiscHeader header{};
    result = nod_disc_header(disc.handle, &header);
    if (result != NOD_RESULT_OK) {
        return convertNodError(result);
    }

    if (!matches(header.game_id, TP_GAME_IDS)) {
        return ValidationError::WrongGame;
    }

    if (!matches(header.game_id, SUPPORTED_TP_GAME_IDS)) {
        return ValidationError::WrongVersion;
    }

    return ValidationError::Success;
}
bool isPal(const char* path) {
    NodHandleWrapper disc;

    const auto sdlStream = SDL_IOFromFile(path, "rb");
    if (sdlStream == nullptr) {
        return false;
    }

    const NodDiscStream nod_stream{
        .user_data = sdlStream,
        .read_at = StreamReadAt,
        .stream_len = StreamLength,
        .close = StreamClose,
    };

    if (nod_disc_open_stream(&nod_stream, nullptr, &disc.handle) != NOD_RESULT_OK || disc.handle == nullptr) {
        return false;
    }

    NodDiscHeader header{};
    if (nod_disc_header(disc.handle, &header) != NOD_RESULT_OK) {
        return false;
    }

    return matches(header.game_id, PAL_GAME_IDS);
}
}  // namespace dusk::iso