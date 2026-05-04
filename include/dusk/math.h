#ifndef _SRC_DUSK_MATH_H_
#define _SRC_DUSK_MATH_H_

#include <cmath>

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

#include <dolphin/ppc_math.h>

#endif // _SRC_DUSK_MATH_H_
