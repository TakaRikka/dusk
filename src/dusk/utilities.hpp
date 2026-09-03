#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#ifdef crc32
// miniz defines crc32 as an alias.
#undef crc32
#endif

namespace dusk::utils {
std::string base64_encode(const std::vector<uint8_t>& data);
bool base64_decode(const std::string& text, std::vector<uint8_t>& out);
uint32_t crc32(const void* data, size_t size);

struct PaneCache {
    uint64_t tag;
    float origTransX;
    float origTransY;
    bool cached;
};
}  // namespace dusk::utils
