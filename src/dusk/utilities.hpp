#pragma once

#include <cstddef>
#include <cstdint>

namespace dusk::utils {
uint32_t CRC32(const void* data, size_t size);
}