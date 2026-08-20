#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace dusk {
namespace detail {

template <size_t Size>
struct uint_of_size;

template <>
struct uint_of_size<1> {
    using type = uint8_t;
};

template <>
struct uint_of_size<2> {
    using type = uint16_t;
};

template <>
struct uint_of_size<4> {
    using type = uint32_t;
};

template <>
struct uint_of_size<8> {
    using type = uint64_t;
};

template <size_t Size>
using uint_of_size_t = uint_of_size<Size>::type;

template <typename T>
    requires(std::is_trivially_copyable_v<T>)
T unaligned_load(const void* source) noexcept {
    T value;
    std::memcpy(&value, source, sizeof(value));
    return value;
}

template <typename T>
    requires(std::is_trivially_copyable_v<T>)
void unaligned_store(void* destination, T value) noexcept {
    std::memcpy(destination, &value, sizeof(value));
}

}  // namespace detail

template <typename T>
    requires(std::is_unsigned_v<T>)
constexpr T bswap(T value) noexcept {
    if constexpr (sizeof(T) == 1) {
        return value;
    } else if constexpr (sizeof(T) == 2) {
        return static_cast<T>((value << 8) | (value >> 8));
    } else if constexpr (sizeof(T) == 4) {
        return static_cast<T>(((value & 0x000000ffU) << 24) | ((value & 0x0000ff00U) << 8) |
                              ((value & 0x00ff0000U) >> 8) | ((value & 0xff000000U) >> 24));
    } else {
        static_assert(sizeof(T) == 8);
        return static_cast<T>(
            ((value & 0x00000000000000ffULL) << 56) | ((value & 0x000000000000ff00ULL) << 40) |
            ((value & 0x0000000000ff0000ULL) << 24) | ((value & 0x00000000ff000000ULL) << 8) |
            ((value & 0x000000ff00000000ULL) >> 8) | ((value & 0x0000ff0000000000ULL) >> 24) |
            ((value & 0x00ff000000000000ULL) >> 40) | ((value & 0xff00000000000000ULL) >> 56));
    }
}

/// Reads an unaligned integral value in the specified byte order.
template <typename T>
    requires(std::is_integral_v<T> && !std::is_same_v<T, bool>)
T read_bits(const void* source, std::endian endian = std::endian::big) noexcept {
    using Bits = std::make_unsigned_t<T>;
    Bits value = detail::unaligned_load<Bits>(source);
    if constexpr (sizeof(Bits) > 1) {
        if (endian != std::endian::native) {
            value = bswap(value);
        }
    }
    return std::bit_cast<T>(value);
}

template <typename T>
    requires(std::is_integral_v<T> && !std::is_same_v<T, bool>)
constexpr T read_bits(const uint8_t* source, std::endian endian = std::endian::big) noexcept {
    if (!std::is_constant_evaluated()) {
        return read_bits<T>(static_cast<const void*>(source), endian);
    }
    using Bits = std::make_unsigned_t<T>;
    Bits value{};
    if (endian == std::endian::big) {
        for (size_t i = 0; i < sizeof(Bits); ++i) {
            value = static_cast<Bits>((value << 8) | source[i]);
        }
    } else {
        for (size_t i = 0; i < sizeof(Bits); ++i) {
            value |= static_cast<Bits>(source[i]) << (i * 8);
        }
    }
    return std::bit_cast<T>(value);
}

/// Reads an unaligned floating-point value in the specified byte order.
template <typename T>
    requires(
        std::is_floating_point_v<T> && requires { typename detail::uint_of_size_t<sizeof(T)>; })
T read_bits(const void* source, std::endian endian = std::endian::big) noexcept {
    using Bits = detail::uint_of_size_t<sizeof(T)>;
    return std::bit_cast<T>(read_bits<Bits>(source, endian));
}

template <typename T>
    requires(
        std::is_floating_point_v<T> && requires { typename detail::uint_of_size_t<sizeof(T)>; })
constexpr T read_bits(const uint8_t* source, std::endian endian = std::endian::big) noexcept {
    using Bits = detail::uint_of_size_t<sizeof(T)>;
    return std::bit_cast<T>(read_bits<Bits>(source, endian));
}

/// Writes an unaligned integral value in the specified byte order.
template <typename T>
    requires(std::is_integral_v<T> && !std::is_same_v<T, bool>)
void write_bits(void* destination, T value, std::endian endian = std::endian::big) noexcept {
    using Bits = std::make_unsigned_t<T>;
    Bits bits = std::bit_cast<Bits>(value);
    if constexpr (sizeof(Bits) > 1) {
        if (endian != std::endian::native) {
            bits = bswap(bits);
        }
    }
    detail::unaligned_store(destination, bits);
}

template <typename T>
    requires(std::is_integral_v<T> && !std::is_same_v<T, bool>)
constexpr void write_bits(
    uint8_t* destination, T value, std::endian endian = std::endian::big) noexcept {
    if (!std::is_constant_evaluated()) {
        write_bits(static_cast<void*>(destination), value, endian);
        return;
    }
    using Bits = std::make_unsigned_t<T>;
    const Bits bits = std::bit_cast<Bits>(value);
    if (endian == std::endian::big) {
        for (size_t i = 0; i < sizeof(Bits); ++i) {
            destination[sizeof(Bits) - i - 1] = static_cast<uint8_t>(bits >> (i * 8));
        }
    } else {
        for (size_t i = 0; i < sizeof(Bits); ++i) {
            destination[i] = static_cast<uint8_t>(bits >> (i * 8));
        }
    }
}

/// Writes an unaligned floating-point value in the specified byte order.
template <typename T>
    requires(
        std::is_floating_point_v<T> && requires { typename detail::uint_of_size_t<sizeof(T)>; })
void write_bits(void* destination, T value, std::endian endian = std::endian::big) noexcept {
    using Bits = detail::uint_of_size_t<sizeof(T)>;
    write_bits(destination, std::bit_cast<Bits>(value), endian);
}

template <typename T>
    requires(
        std::is_floating_point_v<T> && requires { typename detail::uint_of_size_t<sizeof(T)>; })
constexpr void write_bits(
    uint8_t* destination, T value, std::endian endian = std::endian::big) noexcept {
    using Bits = detail::uint_of_size_t<sizeof(T)>;
    write_bits(destination, std::bit_cast<Bits>(value), endian);
}

}  // namespace dusk
