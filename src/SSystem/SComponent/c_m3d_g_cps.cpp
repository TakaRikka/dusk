/**
 * c_m3d_g_cps.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_cps.h"

cM3dGCps::cM3dGCps() {}

cM3dGCps::~cM3dGCps() {}

void cM3dGCps::Set(const cXyz& start, const cXyz& end, f32 radius) {
    SetStartEnd(start, end);
    SetR(radius);
}

void cM3dGCps::Set(const cM3dGCpsS& other) {
    SetStartEnd(other.mStart, other.mEnd);
    SetR(other.mRadius);
}

void cM3dGCps::SetCps(const cM3dGCps& other) {
    Set(other.GetStart(), other.GetEnd(), other.GetR());
}
