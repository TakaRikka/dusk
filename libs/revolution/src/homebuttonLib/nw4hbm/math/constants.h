#ifndef NW4HBM_MATH_CONSTANTS_H
#define NW4HBM_MATH_CONSTANTS_H

#include <revolution/types.h>

namespace nw4hbm {
    namespace math {
        static f32 const Ln2 = 0.69314718056f;
        static f32 const Pi = 3.141592653589f;
        static f32 const Tau = 2.0f * Pi;

        namespace convert {
            static f32 const Deg2Rad = Pi / 180.0f;
            // Rad2Deg

            static f32 const Deg2FIdx = 256.0f / 360.0f;
            static f32 const FIdx2Deg = 360.0f / 256.0f;

            static f32 const Rad2FIdx = 256.0f / Tau;
            static f32 const FIdx2Rad = Tau / 256.0f;
        }  // namespace convert
    }  // namespace math
}  // namespace nw4hbm

#endif  // NW4HBM_MATH_CONSTANTS_H
