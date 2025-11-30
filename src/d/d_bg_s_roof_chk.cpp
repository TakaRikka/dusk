/**
 * d_bg_s_roof_chk.cpp
 * BG Collision Roof Check
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_s_roof_chk.h"

dBgS_RoofChk::dBgS_RoofChk() {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());
    m_pos.x = 0.0f;
    m_pos.y = 0.0f;
    m_pos.z = 0.0f;
    SetActorPid(0xFFFFFFFF);
    field_0x48 = 0;
}

dBgS_RoofChk::~dBgS_RoofChk() {}

void dBgS_RoofChk::Init() {
    SetNowY(G_CM3D_F_INF);
    ClearPi();
}
