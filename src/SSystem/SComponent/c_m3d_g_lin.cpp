/**
 * c_m3d_g_lin.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_lin.h"

cM3dGLin::cM3dGLin(const cXyz& start, const cXyz& end) {
    mStart = start;
    mEnd = end;
}

void cM3dGLin::SetStartEnd(const cXyz& start, const cXyz& end) {
    mStart = start;
    mEnd = end;
}

void cM3dGLin::SetStartEnd(const Vec& start, const Vec& end) {
    mStart.set(start);
    mEnd.set(end);
}

void cM3dGLin::CalcPos(Vec* out, f32 scale) const {
    cXyz tmp;
    PSVECSubtract(&mEnd, &mStart, &tmp);
    PSVECScale(&tmp, &tmp, scale);
    PSVECAdd(&tmp, &mStart, out);
}

void cM3dGLin::SetEnd(const cXyz& end) {
    mEnd = end;
}
