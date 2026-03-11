#ifndef _SRC_DUSK_MATH_H_
#define _SRC_DUSK_MATH_H_

#include <cmath>
#include <array>
#include <limits>
#include <bit>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif
#define M_SQRT3 1.73205f

#define DEG_TO_RAD(degrees) (degrees * (M_PI / 180.0f))
#define RAD_TO_DEG(radians) (radians * (180.0f / M_PI))

inline float i_sinf(float x) { return sin(x); }
inline float i_cosf(float x) { return cos(x); }
inline float i_tanf(float x) { return tan(x); }
inline float i_acosf(float x) { return acos(x); }


// frsqrte matching courtesy of Geotale, with reference to https://achurch.org/cpu-tests/ppc750cl.s

struct BaseAndDec32 {
    uint32_t base;
    int32_t dec;
};

struct BaseAndDec64 {
    uint64_t base;
    int64_t dec;
};

union c32 {
    constexpr c32(const float p) {
        f = p;
    }

    constexpr c32(const uint32_t p) {
        u = p;
    }

    uint32_t u;
    float f;
};

union c64 {
    constexpr c64(const double p) {
        f = p;
    }

    constexpr c64(const uint64_t p) {
        u = p;
    }

    uint64_t u;
    double f;
};

static constexpr uint64_t EXPONENT_SHIFT_F64 = 52;
static constexpr uint64_t MANTISSA_MASK_F64 = 0x000fffffffffffffULL;
static constexpr uint64_t EXPONENT_MASK_F64 = 0x7ff0000000000000ULL;
static constexpr uint64_t SIGN_MASK_F64 = 0x8000000000000000ULL;

static constexpr std::array<BaseAndDec64, 32> RSQRTE_TABLE = {{
        {0x69fa000000000ULL, -0x15a0000000LL},
        {0x5f2e000000000ULL, -0x13cc000000LL},
        {0x554a000000000ULL, -0x1234000000LL},
        {0x4c30000000000ULL, -0x10d4000000LL},
        {0x43c8000000000ULL, -0x0f9c000000LL},
        {0x3bfc000000000ULL, -0x0e88000000LL},
        {0x34b8000000000ULL, -0x0d94000000LL},
        {0x2df0000000000ULL, -0x0cb8000000LL},
        {0x2794000000000ULL, -0x0bf0000000LL},
        {0x219c000000000ULL, -0x0b40000000LL},
        {0x1bfc000000000ULL, -0x0aa0000000LL},
        {0x16ae000000000ULL, -0x0a0c000000LL},
        {0x11a8000000000ULL, -0x0984000000LL},
        {0x0ce6000000000ULL, -0x090c000000LL},
        {0x0862000000000ULL, -0x0898000000LL},
        {0x0416000000000ULL, -0x082c000000LL},
        {0xffe8000000000ULL, -0x1e90000000LL},
        {0xf0a4000000000ULL, -0x1c00000000LL},
        {0xe2a8000000000ULL, -0x19c0000000LL},
        {0xd5c8000000000ULL, -0x17c8000000LL},
        {0xc9e4000000000ULL, -0x1610000000LL},
        {0xbedc000000000ULL, -0x1490000000LL},
        {0xb498000000000ULL, -0x1330000000LL},
        {0xab00000000000ULL, -0x11f8000000LL},
        {0xa204000000000ULL, -0x10e8000000LL},
        {0x9994000000000ULL, -0x0fe8000000LL},
        {0x91a0000000000ULL, -0x0f08000000LL},
        {0x8a1c000000000ULL, -0x0e38000000LL},
        {0x8304000000000ULL, -0x0d78000000LL},
        {0x7c48000000000ULL, -0x0cc8000000LL},
        {0x75e4000000000ULL, -0x0c28000000LL},
        {0x6fd0000000000ULL, -0x0b98000000LL},
}};

[[nodiscard]] static inline double frsqrte(const double val) {
    c64 bits(val);

    uint64_t mantissa = bits.u & MANTISSA_MASK_F64;
    int64_t exponent = bits.u & EXPONENT_MASK_F64;
    bool sign = (bits.u & SIGN_MASK_F64) != 0;

    // Handle 0 case
    if (mantissa == 0 && exponent == 0) {
        return std::copysign(std::numeric_limits<double>::infinity(), bits.f);
    }

    // Handle NaN-like
    if (exponent == EXPONENT_MASK_F64) {
        if (mantissa == 0) {
            return sign ? std::numeric_limits<double>::quiet_NaN() : 0.0;
        }

        return val;
    }

    // Handle negative inputs
    if (sign) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    if (exponent == 0) {
        // Shift so one bit goes to where the exponent would be,
        // then clear that bit to mimic a not-subnormal number!
        // Aka, if there are 12 leading zeroes, shift left once
        uint32_t shift = std::countl_zero(mantissa) - static_cast<uint32_t>(63 - EXPONENT_SHIFT_F64);

        mantissa <<= shift;
        mantissa &= MANTISSA_MASK_F64;
        // The shift is subtracted by 1 because denormals by default
        // are offset by 1 (exponent 0 doesn't have implied 1 bit)
        exponent -= static_cast<int64_t>(shift - 1) << EXPONENT_SHIFT_F64;
    }

    // In reality this doesn't get the full exponent -- Only the least significant bit
    // Only that's needed because square roots of higher exponent bits simply multiply the
    // result by 2!!
    uint32_t key = static_cast<uint32_t>((static_cast<uint64_t>(exponent) | mantissa) >> 37);
    uint64_t new_exp =
            (static_cast<uint64_t>((0xbfcLL << EXPONENT_SHIFT_F64) - exponent) >> 1) & EXPONENT_MASK_F64;

    // Remove the bits relating to anything higher than the LSB of the exponent
    const auto &entry = RSQRTE_TABLE[0x1f & (key >> 11)];

    // The result is given by an estimate then an adjustment based on the original
    // key that was computed
    uint64_t new_mantissa = static_cast<uint64_t>(entry.base + entry.dec * static_cast<int64_t>(key & 0x7ff));

    return c64(new_exp | new_mantissa).f;
}

#endif // _SRC_DUSK_MATH_H_
