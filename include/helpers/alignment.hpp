#pragma once

namespace dusk::helpers::alignment {

/**
 * Read data from an address that may not be aligned properly.
 * @tparam T Type of data to read.
 * @param ptr Address to read from.
 * @return The copied value.
 */
template <typename T> requires std::is_trivially_copyable_v<T>
[[nodiscard]] constexpr T read_unaligned(u8 const* ptr) {
    T copy;
    memcpy(&copy, ptr, sizeof(T));
    return copy;
}

}
