#ifndef NW4HBM_MATH_TRIANGULAR_H
#define NW4HBM_MATH_TRIANGULAR_H

#include <cmath>
#include <cmath>

#include <revolution/types.h>

#include "constants.h"

namespace nw4hbm {
    namespace math {
        f32 SinFIdx(f32 fidx);
        f32 CosFIdx(f32 fidx);
        void SinCosFIdx(f32* s, f32* c, f32 fidx);

        f32 AtanFIdx(f32 x);
        f32 Atan2FIdx(f32 y, f32 x);

        inline f32 TanFIdx(f32 fidx) {
            // They were just like "Ah fuck it we already got too many tables" haha lol
            return std::tanf(fidx * convert::FIdx2Rad);
        }

        inline f32 CosRad(f32 rad) {
            return CosFIdx(rad * convert::Rad2FIdx);
        }

        inline f32 SinDeg(f32 deg) {
            return SinFIdx(deg * convert::Deg2FIdx);
        }

        inline f32 CosDeg(f32 deg) {
            return CosFIdx(deg * convert::Deg2FIdx);
        }

        inline f32 TanDeg(f32 deg) {
            return TanFIdx(deg * convert::Deg2FIdx);
        }

        inline f32 Atan2Deg(f32 y, f32 x) {
            return Atan2FIdx(y, x) * convert::FIdx2Deg;
        }
    }  // namespace math
}  // namespace nw4hbm

#endif  // NW4HBM_MATH_TRIANGULAR_H
