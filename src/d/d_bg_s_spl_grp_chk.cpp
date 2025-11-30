/**
 * d_bg_s_spl_grp_chk.cpp
 * BG Collision Special Group Check
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_s_spl_grp_chk.h"

void dBgS_SplGrpChk::Set(cXyz& ground, f32 roof) {
    m_ground = ground;
    m_roof = roof;
}

dBgS_SplGrpChk::dBgS_SplGrpChk() {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());
    OffNormalGrp();

    m_ground.x = 0.0f;
    m_ground.y = 0.0f;
    m_ground.z = 0.0f;
    m_roof = G_CM3D_F_INF;
    m_height = 0.0f;

    OffMoveBGOnly();
    OffIn();
    OffFind();
}

dBgS_SplGrpChk::~dBgS_SplGrpChk() {}

void dBgS_SplGrpChk::Init() {
    OffIn();
    OffFind();
    SetHeight(GetPosP().y);
    ClearPi();
}
