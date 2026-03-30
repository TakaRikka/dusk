/**
 * c_m3d_g_sph.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_sph.h"
#include "SSystem/SComponent/c_m3d.h"

#include "JSystem/JUtility/JUTAssert.h"

void cM3dGSph::SetC(const cXyz& p) {
    JUT_ASSERT(19, !isnan(p.x));
    JUT_ASSERT(20, !isnan(p.y));
    JUT_ASSERT(21, !isnan(p.z));

    JUT_ASSERT(24, -1.0e32f < p.x && p.x < 1.0e32f && -1.0e32f < p.y && p.y < 1.0e32f && -1.0e32f < p.z && p.z < 1.0e32f);

    mCenter = p;
}

void cM3dGSph::Set(const cXyz& center, f32 radius) {
    SetC(center);
    SetR(radius);
}

void cM3dGSph::Set(const cM3dGSphS& other) {
    SetC(cXyz(other.mCenter));
    SetR(other.mRadius);
}

void cM3dGSph::SetR(f32 r) {
    JUT_ASSERT(54, !isnan(r));
    JUT_ASSERT(55, -1.0e32f < r && r < 1.0e32f);
    mRadius = r;
}

bool cM3dGSph::cross(const cM3dGSph* other, cXyz* out) const {
    return cM3d_Cross_SphSph(other, this, out);
}

bool cM3dGSph::cross(const cM3dGCyl* cyl, cXyz* out) const {
    return cM3d_Cross_CylSph(cyl, this, out);
}

void cM3dGSph::GetMinMaxCube(cXyz& min, cXyz& max) const {
    min.x = mCenter.x - mRadius;
    min.y = mCenter.y - mRadius;
    min.z = mCenter.z - mRadius;
    max.x = mCenter.x + mRadius;
    max.y = mCenter.y + mRadius;
    max.z = mCenter.z + mRadius;
}
