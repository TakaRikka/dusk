/**
 * c_bg_s_shdw_draw.cpp
 *
 */

#include "SSystem/SComponent/c_bg_s_shdw_draw.h"

cBgS_ShdwDraw::cBgS_ShdwDraw() {}

cBgS_ShdwDraw::~cBgS_ShdwDraw() {}

void cBgS_ShdwDraw::Set(cXyz& bound_min, cXyz& bound_max) {
    mM3dGAab.Set(&bound_min, &bound_max);
}

void cBgS_ShdwDraw::SetCallback(cBgS_ShdwDraw_Callback callback) {
    mCallbackFun = callback;
}
