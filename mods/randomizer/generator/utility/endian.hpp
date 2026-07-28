#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <bit>
#include <version>



namespace Utility::Endian
{
    enum struct Type {
        Big = 0,
        Little = 1
    };

#ifdef __cpp_lib_endian
    //use the c++20 api if possible
    constexpr Type target = std::endian::native == std::endian::big ? Type::Big : Type::Little;
    constexpr inline bool isBE() { return target == Type::Big; }
#else
    //do a runtime check otherwise
    Type getEndian();
    const Type target = getEndian();
    inline bool isBE() { return target == Type::Big; }
#endif

    uint64_t byteswap(const uint64_t& value);

    uint32_t byteswap(const uint32_t& value);

    uint32_t byteswap24(const uint32_t& value); //used in FST files

    uint16_t byteswap(const uint16_t& value);

    int64_t byteswap(const int64_t& value);

    int32_t byteswap(const int32_t& value);

    int16_t byteswap(const int16_t& value);

    [[deprecated("Platform may silently set NaN bits, bit_cast to uint32_t first if possible.")]] float byteswap(const float& value);

    [[deprecated("Platform may silently set NaN bits, bit_cast to uint64_t first if possible.")]] double byteswap(const double& value);

    char16_t byteswap(const char16_t& value);

    std::u16string byteswap(const std::u16string& value);

    template<typename T>
    concept CanByteswap = sizeof(T) > 1;

    template<typename T> requires CanByteswap<T> && (!std::is_enum_v<T>)
    constexpr T toPlatform(const Type& src, const T& value) {
        if (src != target) return byteswap(value);
        return value;
    }

    //for enums
    template<typename T, typename TBase = std::underlying_type_t<T>> requires CanByteswap<T> && std::is_enum_v<T>
    constexpr T toPlatform(const Type& src, const T& value) {
        if (src != target) return static_cast<T>(byteswap(static_cast<TBase>(value)));
        return value;
    }

    //doesn't work for enums
    template<typename T> requires CanByteswap<T> && (!std::is_enum_v<T>)
    constexpr void toPlatform_inplace(const Type& src, T& value) {
        if (src != target) value = byteswap(value);
    }

    //for enums
    template<typename T, typename TBase = std::underlying_type_t<T>> requires CanByteswap<T> && std::is_enum_v<T>
    constexpr void toPlatform_inplace(const Type& src, T& value) {
        if (src != target) value = static_cast<T>(byteswap(static_cast<TBase>(value)));
    }
}
