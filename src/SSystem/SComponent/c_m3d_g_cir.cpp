/**
 * c_m3d_g_cir.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_cir.h"

cM3dGCir::cM3dGCir() {}

cM3dGCir::~cM3dGCir() {}

void cM3dGCir::Set(f32 pos_x, f32 pos_y, f32 pos_z, f32 radius) {
    cM2dGCir::Set(pos_x, pos_y, radius);
    mPosZ = pos_z;
}
