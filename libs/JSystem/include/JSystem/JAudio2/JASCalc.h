#ifndef JASCALC_H
#define JASCALC_H

#include <types.h>
#include <cstring>
#include <limits>

/**
 * @ingroup jsystem-jaudio
 *
 */
struct JASCalc {
    static void imixcopy(const s16*, const s16*, s16*, u32);
#if TARGET_PC
    static void bcopyfast(const void* src, void* dest, u32 size) {
        std::memcpy(dest, src, size);
    }
#if TARGET_ANDROID
    static void _bcopy(const void* src, void* dest, u32 size) {
#else
    static void bcopy(const void* src, void* dest, u32 size) {
#endif
        std::memcpy(dest, src, size);
    }
    static void bzerofast(void* dest, u32 size) {
        std::memset(dest, 0, size);
    }
#if TARGET_ANDROID
    static void _bzero(void* dest, u32 size) {
#else
    static void bzero(void* dest, u32 size) {
#endif
        std::memset(dest, 0, size);
    }
#else
    static void bcopyfast(const void* src, void* dest, u32 size);
    static void bcopy(const void* src, void* dest, u32 size);
    static void bzerofast(void* dest, u32 size);
    static void bzero(void* dest, u32 size);
#endif
    static f32 pow2(f32);

    template <typename A, typename B>
    static A clamp(B x);

    static f32 clamp01(f32 i_value) {
        if (i_value <= 0.0f) {
            return 0.0f;
        }
        if (i_value >= 1.0f) {
            return 1.0f;
        }
        return i_value;
    }

    f32 fake1();
    f32 fake2(s32 x);
    f32 fake3();

#if AVOID_UB
    static DUSK_GAME_DATA const s16 CUTOFF_TO_IIR_TABLE[129][4];
#else
    static const s16 CUTOFF_TO_IIR_TABLE[128][4];
#endif
};

template <typename A, typename B>
A JASCalc::clamp(B x) {
    if (x <= std::numeric_limits<A>::min())
        return std::numeric_limits<A>::min();
    if (x >= std::numeric_limits<A>::max())
        return std::numeric_limits<A>::max();
    return x;
}

#endif /* JASCALC_H */
