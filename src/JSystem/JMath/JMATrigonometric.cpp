#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JMath/JMATrigonometric.h"
#include <cmath>
#include "global.h"

static f32 dummy() {
    return 0.0f;
}

namespace JMath {
template<typename T>
struct TAngleConstant_;

inline f64 getConst2() {
    return 9.765625E-4;
}

TSinCosTable<13, f32> sincosTable_ ATTRIBUTE_ALIGN(32);

TAtanTable<1024, f32> atanTable_ ATTRIBUTE_ALIGN(32);

TAsinAcosTable<1024, f32> asinAcosTable_ ATTRIBUTE_ALIGN(32);

}  // namespace JMath
