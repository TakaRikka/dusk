#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio/jstudio-math.h"
#include "JSystem/JGeometry.h"
#include "JSystem/TPosition3.h"
#include "math.h"

void JStudio::math::getRotation_xyz(MtxP param_1, f32 x, f32 y, f32 z) {
    f32 cosx = i_cosf(DEG_TO_RAD(x));
    f32 sinx = i_sinf(DEG_TO_RAD(x));
    f32 cosy = i_cosf(DEG_TO_RAD(y));
    f32 siny = i_sinf(DEG_TO_RAD(y));
    f32 cosz = i_cosf(DEG_TO_RAD(z));
    f32 sinz = i_sinf(DEG_TO_RAD(z));
    f32 cosxcosz = cosx * cosz;
    f32 cosxsinz = cosx * sinz;
    f32 sinxcosz = sinx * cosz;
    f32 sinxsinz = sinx * sinz;

    param_1[0][0] = cosy * cosz;
    param_1[1][0] = cosy * sinz;
    param_1[2][0] = -siny;
    param_1[2][1] = sinx * cosy;
    param_1[2][2] = cosx * cosy;
    param_1[0][1] = ((sinxcosz * siny) - cosxsinz);
    param_1[0][2] = (sinxsinz + (cosxcosz * siny));
    param_1[1][1] = (cosxcosz + (sinxsinz * siny));
    param_1[1][2] = (((cosxsinz) * siny) - sinxcosz);
    param_1[0][3] = 0.0f;
    param_1[1][3] = 0.0f;
    param_1[2][3] = 0.0f;
}

void JStudio::math::getTransformation_SRxyzT(MtxP param_1, Vec const& param_2, Vec const& param_3,
                                             Vec const& param_4) {
    Mtx afStack_40;
    MTXScale(afStack_40, param_2.x, param_2.y, param_2.z);
    Mtx amStack_70;
    rotate_xyz(afStack_40, amStack_70, param_3);
    MTXTransApply(amStack_70, param_1, param_4.x, param_4.y, param_4.z);
}

void JStudio::math::getFromTransformation_SRxyzT(Vec* param_1, Vec* param_2, Vec* param_3,
                                                     CMtxP param_4) {
    getFromTransformation_S(param_1, param_4);
    getFromTransformation_T(param_3, param_4);
    JGeometry::TRotation3<JGeometry::SMatrix33C<double> > aTStack_88;
    JGeometry::TVec3<double> local_a0;
    double dVar9 = 0.0;
    double dVar8 = 0.0;
    double dVar7 = 0.0;
    if (0.0f != param_1->x) {
        dVar9 = 1.0 / param_1->x;
    }
    if (0.0f != param_1->y) {
        dVar8 = 1.0 / param_1->y;
    }
    if (0.0f != param_1->z) {
        dVar7 = 1.0 / param_1->z;
    }
    aTStack_88.set(param_4[0][0] * dVar9, param_4[0][1] * dVar8, param_4[0][2] * dVar7,
                   param_4[1][0] * dVar9, param_4[1][1] * dVar8, param_4[1][2] * dVar7,
                   param_4[2][0] * dVar9, param_4[2][1] * dVar8, param_4[2][2] * dVar7);
    aTStack_88.getEulerXYZ(&local_a0);
    local_a0 *= 57.29577951308232;
    param_2->x = local_a0.x;
    param_2->y = local_a0.y;
    param_2->z = local_a0.z;
}
