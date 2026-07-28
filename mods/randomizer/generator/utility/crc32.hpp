#pragma once

#include <array>

namespace randomizer::utility {

// Generates standard lookup table
constexpr std::array<uint32_t, 256> generate_crc32_table() {
    std::array<uint32_t, 256> table{};
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t ch = i;
        for (size_t j = 0; j < 8; ++j) {
            ch = (ch & 1) ? (0xEDB88320 ^ (ch >> 1)) : (ch >> 1);
        }
        table[i] = ch;
    }
    return table;
}

inline constexpr std::array<uint32_t, 256> crc32_table = generate_crc32_table();

inline uint32_t crc32(const void* data, size_t length, uint32_t previous_crc = 0) {
    const auto* bytes = static_cast<const uint8_t*>(data);
    uint32_t crc = ~previous_crc;

    for (size_t i = 0; i < length; ++i) {
        auto index = static_cast<uint8_t>(crc ^ bytes[i]);
        crc = (crc >> 8) ^ crc32_table[index];
    }

    return ~crc;
}

}