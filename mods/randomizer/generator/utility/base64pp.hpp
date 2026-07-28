#pragma once

#include "base64pp/base64pp.h"

// extra wrappers for convenience
inline std::string b64_encode(const std::string& asciiStr) {
    std::vector<uint8_t> permalinkBytes = {};
    for(const char& ch : asciiStr)
    {
        permalinkBytes.push_back(ch);
    }

    std::span<const uint8_t> span(permalinkBytes.begin(), permalinkBytes.end());

    return base64pp::encode(span);
}

inline std::string b64_decode(const std::string& b64Str) {
    const auto optional = base64pp::decode(b64Str);
    if(!optional.has_value())
    {
        // Return an empty string if there was an error
        return "";
    }

    const auto& bytes = optional.value();
    return {bytes.begin(), bytes.end()};
}