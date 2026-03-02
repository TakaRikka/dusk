#ifndef NW4HBM_MATH_EQUATION_H
#define NW4HBM_MATH_EQUATION_H

#include <revolution/types.h>

namespace nw4hbm {
    namespace math {
        int SolveEquation2(f32* root, f32 a, f32 b, f32 c);
        int SolveEquation3(f32* root, f32 a, f32 b, f32 c, f32 d);
        int SolveEquation4(f32* root, f32 a, f32 b, f32 c, f32 d, f32 e);
    }  // namespace math
}  // namespace nw4hbm

#endif  // NW4HBM_MATH_EQUATION_H
