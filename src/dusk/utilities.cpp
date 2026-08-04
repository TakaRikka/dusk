#include "utilities.hpp"

#include <array>

namespace dusk::utils {
namespace {

constexpr char kBase64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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

}  // namespace

std::string base64_encode(const std::vector<uint8_t>& data) {
    std::string out;
    out.reserve((data.size() + 2) / 3 * 4);
    for (size_t i = 0; i < data.size(); i += 3) {
        const uint32_t rest = data.size() - i;
        uint32_t chunk = data[i] << 16;
        if (rest > 1) {
            chunk |= data[i + 1] << 8;
        }
        if (rest > 2) {
            chunk |= data[i + 2];
        }
        out.push_back(kBase64Chars[chunk >> 18 & 0x3F]);
        out.push_back(kBase64Chars[chunk >> 12 & 0x3F]);
        out.push_back(rest > 1 ? kBase64Chars[chunk >> 6 & 0x3F] : '=');
        out.push_back(rest > 2 ? kBase64Chars[chunk & 0x3F] : '=');
    }
    return out;
}

bool base64_decode(const std::string& text, std::vector<uint8_t>& out) {
    if (text.size() % 4 != 0) {
        return false;
    }
    static const auto lookup = [] {
        std::array<int8_t, 256> table;
        table.fill(-1);
        for (int i = 0; i < 64; ++i) {
            table[static_cast<uint8_t>(kBase64Chars[i])] = static_cast<int8_t>(i);
        }
        return table;
    }();
    out.clear();
    out.reserve(text.size() / 4 * 3);
    for (size_t i = 0; i < text.size(); i += 4) {
        uint32_t chunk = 0;
        int pads = 0;
        for (size_t j = 0; j < 4; ++j) {
            const char c = text[i + j];
            if (c == '=' && i + 4 == text.size() && j >= 2) {
                ++pads;
                chunk <<= 6;
                continue;
            }
            const int8_t value = lookup[static_cast<uint8_t>(c)];
            if (value < 0 || pads != 0) {
                return false;
            }
            chunk = chunk << 6 | static_cast<uint32_t>(value);
        }
        out.push_back(chunk >> 16 & 0xFF);
        if (pads < 2) {
            out.push_back(chunk >> 8 & 0xFF);
        }
        if (pads < 1) {
            out.push_back(chunk & 0xFF);
        }
    }
    return true;
}

uint32_t crc32(const void* data, size_t size) {
    const auto* bytes = static_cast<const uint8_t*>(data);
    uint32_t crc = ~0u;
    for (size_t i = 0; i < size; ++i) {
        crc = crc >> 8 ^ kCrc32Table[static_cast<uint8_t>(crc ^ bytes[i])];
    }
    return ~crc;
}
}  // namespace dusk::utils
