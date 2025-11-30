/**
 * c_m3d_g_sph.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_sph.h"
#include "SSystem/SComponent/c_m3d.h"

void cM3dGSph::SetC(const cXyz& center) {
    mCenter = center;
}

void cM3dGSph::Set(const cXyz& center, f32 radius) {
    SetC(center);
    SetR(radius);
}

void cM3dGSph::Set(const cM3dGSphS& other) {
    SetC(cXyz(other.mCenter));
    SetR(other.mRadius);
}

void cM3dGSph::SetR(f32 radius) {
    mRadius = radius;
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
