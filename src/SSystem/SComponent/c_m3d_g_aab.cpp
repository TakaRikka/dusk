/**
 * c_m3d_g_aab.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_aab.h"

void cM3dGAab::Set(const cXyz* min, const cXyz* max) {
    mMin = *min;
    mMax = *max;
}

bool cM3dGAab::CrossY(const cXyz* other) const {
    if (mMin.x > other->x || mMax.x < other->x || mMin.z > other->z || mMax.z < other->z) {
        return false;
    } else {
        return true;
    }
}

bool cM3dGAab::UnderPlaneYUnder(f32 y) const {
    if (mMin.y < y) {
        return true;
    }

    return false;
}

bool cM3dGAab::TopPlaneYUnder(f32 y) const {
    if (mMax.y < y) {
        return true;
    }

    return false;
}

void cM3dGAab::ClearForMinMax() {
    mMin.x = mMin.y = mMin.z = G_CM3D_F_INF;
    mMax.x = mMax.y = mMax.z = -G_CM3D_F_INF;
}

void cM3dGAab::SetMinMax(const cXyz& pMinMax) {
    this->SetMin(pMinMax);
    this->SetMax(pMinMax);
}

void cM3dGAab::SetMinMax(const cM3dGAab& other) {
    this->SetMinMax(other.mMin);
    this->SetMinMax(other.mMax);
}

void cM3dGAab::SetMin(const cXyz& min) {
    if (mMin.x > min.x) {
        mMin.x = min.x;
    }
    if (mMin.y > min.y) {
        mMin.y = min.y;
    }
    if (mMin.z > min.z) {
        mMin.z = min.z;
    }
}

void cM3dGAab::SetMax(const cXyz& max) {
    if (mMax.x < max.x) {
        mMax.x = max.x;
    }
    if (mMax.y < max.y) {
        mMax.y = max.y;
    }
    if (mMax.z < max.z) {
        mMax.z = max.z;
    }
}

void cM3dGAab::CalcCenter(cXyz* out) const {
    PSVECAdd(&mMin, &mMax, out);
    PSVECScale(out, out, 0.5f);
}

void cM3dGAab::PlusR(f32 r) {
    mMin.x -= r;
    mMin.y -= r;
    mMin.z -= r;

    mMax.x += r;
    mMax.y += r;
    mMax.z += r;
}
