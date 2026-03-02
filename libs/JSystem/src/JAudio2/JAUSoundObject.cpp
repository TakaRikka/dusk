#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUSoundObject.h"

#include "JSystem/JGeometry.h"

static void dummy(JGeometry::TVec3<f32>* vec_1, JGeometry::TVec3<f32>* vec_2) {
    *vec_1 = *vec_2;
}
