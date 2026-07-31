#include "utilities.hpp"

#include <array>

namespace dusk::utils {
constexpr std::array<uint32_t, 256> generate_crc32_table() {
    std::array<uint32_t, 256> table{};
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t ch = i;
        for (size_t j = 0; j < 8; ++j) {
            ch = (ch & 1) != 0 ? 0xEDB88320 ^ ch >> 1 : ch >> 1;
        }
        table[i] = ch;
    }
    return table;
}

constexpr std::array<uint32_t, 256> kCrc32Table = generate_crc32_table();

// CRC-32 (IEEE, reflected)
uint32_t CRC32(const void* data, size_t size) {
    const auto* bytes = static_cast<const uint8_t*>(data);
    uint32_t crc = ~0u;
    for (size_t i = 0; i < size; ++i) {
        crc = crc >> 8 ^ kCrc32Table[static_cast<uint8_t>(crc ^ bytes[i])];
    }
    return ~crc;
}
}