#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAMath.h"
#include "JSystem/JMath/JMATrigonometric.h"
#include "JSystem/J2DGraph/J2DAnimation.h"

// Probably some local function needed to change float literal order
static f32 floatDummyFunc() {
    return 1.0f;
}

void JPAGetDirMtx(JGeometry::TVec3<f32> const& param_0, f32 (*param_1)[4]) {
    JGeometry::TVec3<float> local_78(param_0.y, -param_0.x, 0.0f);
    f32 len = local_78.length();

    if (len <= JGeometry::TUtil<f32>::epsilon()) {
        local_78.zero();
    } else {
        local_78.scale(1.0f / len);
    }
    f32 xsquared = local_78.x * local_78.x;
    f32 ysquared = local_78.y * local_78.y;
    f32 xlen = local_78.x * len;
    f32 ylen = local_78.y * len;
    f32 fVar5 = (1.0f - param_0.z) * (local_78.x * local_78.y);
    param_1[0][0] = xsquared + param_0.z * (1.0f - xsquared);
    param_1[0][1] = fVar5;
    param_1[0][2] = -ylen;
    param_1[0][3] = 0.0f;
    param_1[1][0] = fVar5;
    param_1[1][1] = ysquared + param_0.z * (1.0f - ysquared);
    param_1[1][2] = xlen;
    param_1[1][3] = 0.0f;
    param_1[2][0] = ylen;
    param_1[2][1] = -xlen;
    param_1[2][2] = param_0.z;
    param_1[2][3] = 0.0f;
}

void JPAGetYZRotateMtx(s16 angleY, s16 angleZ, f32 (*param_2)[4]) {
    f32 cosy = JMASCos(angleY);
    f32 cosz = JMASCos(angleZ);
    f32 siny = JMASSin(angleY);
    f32 sinz = JMASSin(angleZ);
    param_2[0][0] = (cosy * cosz);
    param_2[0][1] = -sinz;
    param_2[0][2] = (siny * cosz);
    param_2[1][0] = (cosy * sinz);
    param_2[1][1] = cosz;
    param_2[1][2] = (siny * sinz);
    param_2[2][0] = -siny;
    param_2[2][2] = cosy;
    param_2[2][3] = 0.0f;
    param_2[2][1] = 0.0f;
    param_2[1][3] = 0.0f;
    param_2[0][3] = 0.0f;
}

void JPAGetXYZRotateMtx(s16 x, s16 y, s16 z, Mtx mtx) {
    f32 cosx = JMASCos(x);
    f32 cosy = JMASCos(y);
    f32 cosz = JMASCos(z);
    f32 sinx = JMASSin(x);
    f32 siny = JMASSin(y);
    f32 sinz = JMASSin(z);
    mtx[0][0] = cosy * cosz;
    mtx[1][0] = cosy * sinz;
    mtx[2][0] = -siny;
    mtx[2][1] = sinx * cosy;
    mtx[2][2] = cosx * cosy;
    f32 cosxsinz = cosx * sinz;
    f32 sinxcosz = sinx * cosz;
    mtx[0][1] = sinxcosz * siny - cosxsinz;
    mtx[1][2] = cosxsinz * siny - sinxcosz;
    f32 sinxsinz = sinx * sinz;
    f32 cosxcosz = cosx * cosz;
    mtx[0][2] = sinxsinz + cosxcosz * siny;
    mtx[1][1] = cosxcosz + sinxsinz * siny;
    mtx[2][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[0][3] = 0.0f;
}

void JPASetRMtxTVecfromMtx(f32 const (*param_0)[4], f32 (*param_1)[4],
                               JGeometry::TVec3<f32>* param_2) {
    JGeometry::TVec3<f32> dummy;
    JPASetRMtxSTVecfromMtx(param_0, param_1, &dummy, param_2);
}

void JPASetRMtxSTVecfromMtx(f32 const (*param_0)[4], f32 (*param_1)[4],
                                JGeometry::TVec3<f32>* param_2, JGeometry::TVec3<f32>* param_3) {
    JGeometry::TVec3<float> aTStack_54;
    aTStack_54.set(param_0[0][0], param_0[1][0], param_0[2][0]);
    param_2->x = aTStack_54.length();
    aTStack_54.set(param_0[0][1], param_0[1][1], param_0[2][1]);
    param_2->y = aTStack_54.length();
    aTStack_54.set(param_0[0][2], param_0[1][2], param_0[2][2]);
    param_2->z = aTStack_54.length();
    MTXIdentity(param_1);
    if (param_2->x != 0.0f) {
        f32 fVar5 = 1.0f / param_2->x;
        param_1[0][0] = param_0[0][0] * fVar5;
        param_1[1][0] = param_0[1][0] * fVar5;
        param_1[2][0] = param_0[2][0] * fVar5;
    }
    if (param_2->y != 0.0f) {
        f32 fVar5 = 1.0f / param_2->y;
        param_1[0][1] = param_0[0][1] * fVar5;
        param_1[1][1] = param_0[1][1] * fVar5;
        param_1[2][1] = param_0[2][1] * fVar5;
    }
    if (param_2->z != 0.0f) {
        f32 fVar5 = 1.0f / param_2->z;
        param_1[0][2] = param_0[0][2] * fVar5;
        param_1[1][2] = param_0[1][2] * fVar5;
        param_1[2][2] = param_0[2][2] * fVar5;
    }
    param_3->set(param_0[0][3], param_0[1][3], param_0[2][3]);
}

f32 JPACalcKeyAnmValue(f32 param_0, u16 param_1, f32 const* param_2) {
    if (param_0 < param_2[0]) {
        return param_2[1];
    }
    int ind = param_1 - 1;
    if (param_2[ind * 4] <= param_0) {
        return param_2[ind * 4 + 1];
    }
    int x = param_1;
    while (x > 1) {
        u32 uVar3 = x / 2;
        if (param_0 >= param_2[uVar3 * 4]) {
            param_2 += uVar3 * 4;
            x -= uVar3;
        } else {
            x = uVar3;
        }
    }
    return JMAHermiteInterpolation(param_0, param_2[0], param_2[1],
                                            param_2[3], param_2[4],
                                            param_2[5], param_2[6]);
}
