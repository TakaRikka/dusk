/**
 * d_bg_s_sph_chk.cpp
 * BG Collision Sphere Check
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_s_sph_chk.h"

dBgS_SphChk::dBgS_SphChk() {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());
    SetActorPid(0xFFFFFFFF);
    ClearPi();
    mCallback = NULL;
}

dBgS_SphChk::~dBgS_SphChk() {}
