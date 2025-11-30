/**
 * c_m3d_g_pla.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_pla.h"
#include "SSystem/SComponent/c_m3d.h"

cM3dGPla::cM3dGPla(const cXyz* normal, f32 d) {
    mNormal = *normal;
    mD = d;
}

bool cM3dGPla::crossInfLin(const cXyz& start, const cXyz& end, cXyz& out) const {
    f32 tmp1 = getPlaneFunc(&start);
    f32 tmp2 = getPlaneFunc(&end);
    if (cM3d_IsZero(tmp1 - tmp2)) {
        out = end;
        return false;
    }

    f32 var_f29 = tmp1 / (tmp1 - tmp2);
    cM3d_InDivPos2(&start, &end, var_f29, &out);
    return true;
}

void cM3dGPla::SetupNP0(const Vec& normal, const Vec& point) {
    mNormal = normal;
    PSVECNormalize(&mNormal, &mNormal);
    mD = -PSVECDotProduct(&mNormal, &point);
}

void cM3dGPla::SetupNP(const Vec& normal, const Vec& point) {
    mNormal = normal;
    mD = -PSVECDotProduct(&mNormal, &point);
}

bool cM3dGPla::getCrossY(const cXyz& point, f32* out) const {
    if (cM3d_IsZero(mNormal.y)) {
        return false;
    } else {
        *out = (-mNormal.x * point.x - mNormal.z * point.z - mD) / mNormal.y;
        return true;
    }
}

bool cM3dGPla::getCrossYLessD(const Vec& point, f32* out) const {
    if (cM3d_IsZero(mNormal.y)) {
        return false;
    } else {
        *out = (-mNormal.x * point.x - mNormal.z * point.z) / mNormal.y;
        return true;
    }
}

void cM3dGPla::Set(const cM3dGPla* other) {
    mNormal = *other->GetNP();
    mD = other->GetD();
}
