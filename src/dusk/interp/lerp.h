#pragma once

#include "SSystem/SComponent/c_angle.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"

#include <cmath>
#include <dolphin/mtx.h>

#ifdef __cplusplus
namespace dusk::interp {

inline s16 lerp(s16 lhs, s16 rhs, float step) {
    const f32 ra = S2RAD(lhs);
    const f32 d = remainderf(S2RAD(rhs) - ra, 2.0f * M_PI);
    return cAngle::Radian_to_SAngle(ra + d * step);
}

inline u16 lerp(u16 lhs, u16 rhs, float step) {
    return static_cast<u16>(lerp(static_cast<s16>(lhs), static_cast<s16>(rhs), step));
}

inline f32 lerp(f32 lhs, f32 rhs, float step) {
    return lhs + (rhs - lhs) * step;
}

inline u8 lerp(u8 lhs, u8 rhs, float step) {
    return static_cast<u8>(std::lround(lerp(static_cast<f32>(lhs), static_cast<f32>(rhs), step)));
}

inline void lerp(cXyz& out, const cXyz& lhs, const cXyz& rhs, float step) {
    out.x = lerp(lhs.x, rhs.x, step);
    out.y = lerp(lhs.y, rhs.y, step);
    out.z = lerp(lhs.z, rhs.z, step);
}

inline void lerp(csXyz& out, const csXyz& lhs, const csXyz& rhs, float step) {
    out.x = lerp(lhs.x, rhs.x, step);
    out.y = lerp(lhs.y, rhs.y, step);
    out.z = lerp(lhs.z, rhs.z, step);
}

inline void lerp(Mtx& out, const Mtx& lhs, const Mtx& rhs, float step) {
    for (size_t row = 0; row < 3; ++row) {
        for (size_t col = 0; col < 4; ++col) {
            const float l = lhs[row][col];
            out[row][col] = l + (rhs[row][col] - l) * step;
        }
    }
}

}  // namespace dusk::interp
#endif
