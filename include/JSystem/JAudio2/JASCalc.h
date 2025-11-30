#ifndef JASCALC_H
#define JASCALC_H

#include "dolphin/types.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASCalc {
    static void imixcopy(const s16*, const s16*, s16*, u32);
    static void bcopyfast(const void* src, void* dest, u32 size);
    static void bcopy(const void* src, void* dest, u32 size);
    static void bzerofast(void* dest, u32 size);
    static void bzero(void* dest, u32 size);
    static f32 pow2(f32);

    // Could not make it work as inline - specialization is in JASCalc.cpp
    template <typename A, typename B>
    static A clamp(B x); /* {
        if (std::numeric_limits<A>::min() >= x)
            return std::numeric_limits<A>::min();
        if (x >= std::numeric_limits<A>::max())
            return std::numeric_limits<A>::max();
        return x;
    } */

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
    f32 fake2(long x);
    f32 fake3();

    static const s16 CUTOFF_TO_IIR_TABLE[128][4];
};

#endif /* JASCALC_H */
