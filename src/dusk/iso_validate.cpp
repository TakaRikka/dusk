#include "iso_validate.hpp"

#include <SDL3/SDL_iostream.h>
#include <nod.h>
#include <xxhash.h>

#include <array>
#include <memory>
#include <stdexcept>
#include <string_view>

namespace {

constexpr uint8_t hex_nibble_to_u8(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    throw std::invalid_argument("invalid hex character");
}

constexpr uint64_t parse_u64_hex(std::string_view s) {
    if (s.size() != 16)
        throw std::invalid_argument("expected 16 hex chars for uint64");

    uint64_t value = 0;
    for (char c : s) {
        value = (value << 4) | hex_nibble_to_u8(c);
    }
    return value;
}

constexpr XXH128_hash_t parse_xxh128(std::string_view hex) {
    if (hex.size() != 32)
        throw std::invalid_argument("expected 32 hex chars for XXH128");

    return XXH128_hash_t{
        .low64 = parse_u64_hex(hex.substr(16, 16)),
        .high64 = parse_u64_hex(hex.substr(0, 16)),
    };
}

}  // namespace

namespace dusk::iso {

enum class Platform : u8 {
    GameCube,
    Wii,
};

enum class Region : u8 {
    NorthAmerica,
    Europe,
    Japan,
    Korea,
};

struct KnownDisc {
    std::string_view id;
    Platform platform;
    Region region;
    bool supported = false;
    XXH128_hash_t hash{};

    constexpr KnownDisc(std::string_view id, Platform platform, Region region)
        : id(id), platform(platform), region(region) {}
    constexpr KnownDisc(
        std::string_view id, Platform platform, Region region, const std::string_view hash)
        : id(id), platform(platform), region(region), supported(true), hash(parse_xxh128(hash)) {}
};

constexpr auto KNOWN_DISCS = std::to_array<KnownDisc>({
    {"GZ2E01", Platform::GameCube, Region::NorthAmerica, "14e886f08e548a000afde98a3195e788"},
    {"GZ2J01", Platform::GameCube, Region::Japan},
    {"GZ2P01", Platform::GameCube, Region::Europe, "9ef597588b0035ca9e91b333fa9a8a7e"},
    {"RZDE01", Platform::Wii, Region::NorthAmerica},
    {"RZDJ01", Platform::Wii, Region::Japan},
    {"RZDK01", Platform::Wii, Region::Korea},
    {"RZDP01", Platform::Wii, Region::Europe},
});

constexpr const KnownDisc* find_disc(std::string_view id) {
    for (const auto& disc : KNOWN_DISCS) {
        if (disc.id == id)
            return &disc;
    }
    return nullptr;
}

struct NodHandleWrapper {
    NodHandle* handle;

    NodHandleWrapper() : handle(nullptr) {}
    ~NodHandleWrapper() {
        if (handle != nullptr) {
            nod_free(handle);
            handle = nullptr;
        }
    }
};

static ValidationError convert_nod_error(NodResult result) {
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
    auto* io = static_cast<SDL_IOStream*>(user_data);
    const auto ret = SDL_SeekIO(io, static_cast<s64>(offset), SDL_IO_SEEK_SET);
    if (ret < 0) {
        return -1;
    }
    const auto read = SDL_ReadIO(io, out, len);
    if (read == 0) {
        if (SDL_GetIOStatus(io) == SDL_IO_STATUS_EOF) {
            return 0;
        }
        return -1;
    }
    return static_cast<s64>(read);
}

s64 StreamLength(void* user_data) {
    return SDL_GetIOSize(static_cast<SDL_IOStream*>(user_data));
}

void StreamClose(void* user_data) {
    SDL_CloseIO(static_cast<SDL_IOStream*>(user_data));
}

ValidationError verify_disc(NodHandle* disc, VerificationStatus& status) {
    std::unique_ptr<XXH3_state_t, decltype(&XXH3_freeState)> hashState(
        XXH3_createState(), XXH3_freeState);
    if (!hashState) {
        return ValidationError::Unknown;
    }
    XXH3_128bits_reset(hashState.get());

    while (true) {
        if (status.shouldCancel.load(std::memory_order_relaxed)) {
            return ValidationError::Canceled;
        }

        size_t bytesAvail;
        const auto buf = nod_buf_read(disc, &bytesAvail);
        if (!bytesAvail)
            break;

        XXH3_128bits_update(hashState.get(), buf, bytesAvail);

        status.bytesRead.fetch_add(bytesAvail, std::memory_order_relaxed);
        nod_buf_consume(disc, bytesAvail);
    }

    const auto hash = XXH3_128bits_digest(hashState.get());
    if (!XXH128_isEqual(hash, status.knownDisc->hash)) {
        return ValidationError::HashMismatch;
    }
    return ValidationError::Success;
}

ValidationError validate(const char* path, VerificationStatus& status) {
    const auto sdlStream = SDL_IOFromFile(path, "rb");
    if (sdlStream == nullptr) {
        return ValidationError::IOError;
    }

    NodHandleWrapper disc;
    const NodDiscStream nod_stream{
        .user_data = sdlStream,
        .read_at = StreamReadAt,
        .stream_len = StreamLength,
        .close = StreamClose,
    };
    auto result = nod_disc_open_stream(&nod_stream, nullptr, &disc.handle);
    if (disc.handle == nullptr || result != NOD_RESULT_OK) {
        return convert_nod_error(result);
    }

    status.bytesTotal.store(nod_disc_size(disc.handle), std::memory_order_relaxed);

    NodDiscHeader header{};
    result = nod_disc_header(disc.handle, &header);
    if (result != NOD_RESULT_OK) {
        return convert_nod_error(result);
    }

    const auto knownDisc = find_disc(std::string_view(header.game_id, 6));
    if (!knownDisc) {
        return ValidationError::WrongGame;
    }
    status.knownDisc = knownDisc;
    if (!knownDisc->supported) {
        return ValidationError::WrongVersion;
    }
    return verify_disc(disc.handle, status);
}

ValidationError inspect(const char* path, DiscInfo& info) {
    const auto sdlStream = SDL_IOFromFile(path, "rb");
    if (sdlStream == nullptr) {
        return ValidationError::IOError;
    }

    NodHandleWrapper disc;
    const NodDiscStream nod_stream{
        .user_data = sdlStream,
        .read_at = StreamReadAt,
        .stream_len = StreamLength,
        .close = StreamClose,
    };
    auto result = nod_disc_open_stream(&nod_stream, nullptr, &disc.handle);
    if (disc.handle == nullptr || result != NOD_RESULT_OK) {
        return convert_nod_error(result);
    }

    NodDiscHeader header{};
    result = nod_disc_header(disc.handle, &header);
    if (result != NOD_RESULT_OK) {
        return convert_nod_error(result);
    }

    const auto knownDisc = find_disc(std::string_view(header.game_id, 6));
    if (!knownDisc) {
        return ValidationError::WrongGame;
    }
    info.isPal = knownDisc->region == Region::Europe;
    if (!knownDisc->supported) {
        return ValidationError::WrongVersion;
    }
    return ValidationError::Success;
}

bool isPal(const char* path) {
    DiscInfo info{};
    return inspect(path, info) == ValidationError::Success && info.isPal;
}
}  // namespace dusk::iso
