/**
 * c_m3d_g_tri.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_tri.h"
#include "SSystem/SComponent/c_m3d.h"
#include "JSystem/JUtility/JUTAssert.h"

bool cM3dGTri::cross(const cM3dGCyl* cyl, Vec* out) const {
    return cM3d_Cross_CylTri(cyl, this, out);
}

void cM3dGTri::setPos(const Vec* vtx_a, const Vec* vtx_b, const Vec* vtx_c) {
    mA = *vtx_a;
    mB = *vtx_b;
    mC = *vtx_c;
    setUp();

    JUT_ASSERT(99, !cM3d_IsZero(GetNP()->x) || !cM3d_IsZero(GetNP()->y) || !cM3d_IsZero(GetNP()->z));
}

void cM3dGTri::setBg(const Vec* vtx_a, const Vec* vtx_b, const Vec* vtx_c, const cM3dGPla* plane) {
    mA.x = vtx_a->x;
    mA.y = vtx_a->y;
    mA.z = vtx_a->z;

    mB.x = vtx_b->x;
    mB.y = vtx_b->y;
    mB.z = vtx_b->z;

    mC.x = vtx_c->x;
    mC.y = vtx_c->y;
    mC.z = vtx_c->z;

    Set(plane);
}

void cM3dGTri::set(const Vec* vtx_a, const Vec* vtx_b, const Vec* vtx_c, const Vec* normal) {
    mA.x = vtx_a->x;
    mA.y = vtx_a->y;
    mA.z = vtx_a->z;

    mB.x = vtx_b->x;
    mB.y = vtx_b->y;
    mB.z = vtx_b->z;

    mC.x = vtx_c->x;
    mC.y = vtx_c->y;
    mC.z = vtx_c->z;

    SetupNP(*normal, *vtx_a);
}
