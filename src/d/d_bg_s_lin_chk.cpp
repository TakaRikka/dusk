/**
 * d_bg_s_lin_chk.cpp
 * BG Collision Line Check
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_s_lin_chk.h"
#include "f_op/f_op_actor_mng.h"

#define CHECK_FLOAT_RANGE(line, x) JUT_ASSERT(line, -1.0e32f < x && x < 1.0e32f);

dBgS_LinChk::dBgS_LinChk() {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());
}

dBgS_LinChk::~dBgS_LinChk() {}

void dBgS_LinChk::Set(cXyz const* pi_start, cXyz const* pi_end, fopAc_ac_c const* p_actor) {
    JUT_ASSERT(45, !isnan(pi_start->x));
    JUT_ASSERT(46, !isnan(pi_start->y));
    JUT_ASSERT(47, !isnan(pi_start->z));
    JUT_ASSERT(48, !isnan(pi_end->x));
    JUT_ASSERT(49, !isnan(pi_end->y));
    //! @bug Two asserts on pi_end->y instead of one on py_end->z
    JUT_ASSERT(50, !isnan(pi_end->y));

    JUT_ASSERT(54, -INF < pi_start->x && pi_start->x < INF);
    JUT_ASSERT(55, -INF < pi_start->y && pi_start->y < INF);
    JUT_ASSERT(56, -INF < pi_start->z && pi_start->z < INF);
    JUT_ASSERT(57, -INF < pi_end->x && pi_end->x < INF);
    JUT_ASSERT(58, -INF < pi_end->y && pi_end->y < INF);
    JUT_ASSERT(59, -INF < pi_end->z && pi_end->z < INF);

    JUT_ASSERT(61, pi_start->y != G_CM3D_F_INF);
    JUT_ASSERT(62, pi_start->y != -G_CM3D_F_INF);
    JUT_ASSERT(63, pi_end->y != G_CM3D_F_INF);
    JUT_ASSERT(64, pi_end->y != -G_CM3D_F_INF);

    u32 id;
    if (p_actor != NULL) {
        id = fopAcM_GetID(p_actor);
    } else {
        id = 0xFFFFFFFF;
    }
    Set2(pi_start, pi_end, id);
}

dBgS_CamLinChk::dBgS_CamLinChk() {
    SetCam();
}

dBgS_CamLinChk::~dBgS_CamLinChk() {}

dBgS_CamLinChk_NorWtr::dBgS_CamLinChk_NorWtr() {
    OnWaterGrp();
    OnNormalGrp();
}

dBgS_CamLinChk_NorWtr::~dBgS_CamLinChk_NorWtr() {}

dBgS_ObjLinChk::dBgS_ObjLinChk() {
    SetObj();
}

dBgS_ObjLinChk::~dBgS_ObjLinChk() {}

dBgS_LinkLinChk::dBgS_LinkLinChk() {
    SetLink();
}

dBgS_LinkLinChk::~dBgS_LinkLinChk() {}

dBgS_BombLinChk::dBgS_BombLinChk() {
    SetBomb();
}

dBgS_BombLinChk::~dBgS_BombLinChk() {}

dBgS_ArrowLinChk::dBgS_ArrowLinChk() {
    SetArrow();
    OnNormalGrp();
}

dBgS_ArrowLinChk::~dBgS_ArrowLinChk() {}

dBgS_BoomerangLinChk::dBgS_BoomerangLinChk() {
    SetBoomerang();
}

dBgS_BoomerangLinChk::~dBgS_BoomerangLinChk() {}

dBgS_RopeLinChk::dBgS_RopeLinChk() {
    SetRope();
}

dBgS_RopeLinChk::~dBgS_RopeLinChk() {}

dBgS_HorseLinChk::dBgS_HorseLinChk() {
    SetHorse();
}

dBgS_HorseLinChk::~dBgS_HorseLinChk() {}
