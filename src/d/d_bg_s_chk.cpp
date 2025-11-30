/**
 * d_bg_s_chk.cpp
 * BG Collision Check Base
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_s_chk.h"

dBgS_Chk::dBgS_Chk() {}

dBgS_Chk::~dBgS_Chk() {}

cBgS_PolyPassChk* dBgS_Chk::GetPolyPassChkInfo() {
    return this;
}

cBgS_GrpPassChk* dBgS_Chk::GetGrpPassChkInfo() {
    return this;
}
