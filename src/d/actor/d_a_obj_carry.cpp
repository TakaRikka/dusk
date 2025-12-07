/**
 * @file d_a_obj_carry.cpp
 * @brief Actor - Various carriable objects
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_carry.h"
#include "d/actor/d_a_player.h"
#include <cmath.h>
#include "d/d_com_inf_game.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_obj_swBallA.h"
#include "d/actor/d_a_obj_swBallB.h"
#include "d/actor/d_a_tag_setBall.h"
#include "d/actor/d_a_mirror.h"
#include "d/actor/d_a_obj_burnbox.h"
#include "d/d_tresure.h"
#include "d/d_s_play.h"
#include "d/d_lib.h"
#include "d/d_debug_viewer.h"
#include "f_op/f_op_kankyo_mng.h"

static const cM3dGCylS l_cyl_info[] = {
    { 0.0f, 0.0f, 0.0f, 30.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 50.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 37.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 40.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 50.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 30.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 24.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 30.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 40.0f, 80.0f },
    { 0.0f, 0.0f, 0.0f, 40.0f, 80.0f },
    { 0.0f, 0.0f, 0.0f, 50.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 40.0f, 80.0f },
    { 0.0f, 0.0f, 0.0f, 30.0f, 100.0f },
    { 0.0f, 0.0f, 0.0f, 50.0f, 100.0f },
};

const daObjCarry_dt_t daObjCarry_c::mData[] = {
    // TYPE_TSUBO
    {
        27.0f,
        36.0f,
        30.0f,
        80.0f,
        0.6f,
        0.6f,
        -6.0f,
        4.0f,
        0.4f,
        25.0f,
        0.0f,
        28.0f,
        1.0f,
        42,
        0,
        200,
        2,
        0.0f,
        28.0f,
        0.0f,
        48.0f,
        30.0f,
        30.0f,
        48.0f,
        250.0f,
        0.0f,
        JA_SE_OBJ_BREAK_CHINA_S,
        JA_SE_OBJ_PUT_POT_S,
        Z2SE_OBJ_PUT_WATER_S,
        JA_SE_OBJ_FALL_MAGMA_S,
        Z2SE_OBJ_FALL_WATER_S,
        0xD,
        40.0f,
        0x50,
        0x2D,
        0x2D,
        10.0f,
        0.1f,
        1.0f,
        1.0f,
        0xA00,
    },
    // TYPE_OOTSUBO
    {
        22.0f,
        43.0f,
        30.0f,
        80.0f,
        0.6f,
        0.6f,
        -6.5f,
        3.0f,
        0.4f,
        35.0f,
        0.0f,
        50.0f,
        1.0f,
        42,
        4,
        200,
        3,
        0.0f,
        48.0f,
        0.0f,
        68.0f,
        50.0f,
        50.0f,
        50.0f,
        250.0f,
        0.0f,
        JA_SE_OBJ_BREAK_CHINA_L,
        JA_SE_OBJ_PUT_POT_L,
        Z2SE_OBJ_PUT_WATER_M,
        JA_SE_OBJ_FALL_MAGMA_M,
        Z2SE_OBJ_FALL_WATER_M,
        0xD,
        50.0f,
        0x50,
        0x14,
        0x14,
        10.0f,
        0.1f,
        1.5f,
        1.5f,
        0xA00,
    },
    // TYPE_KIBAKO
    {
        27.0f,
        36.0f,
        30.0f,
        80.0f,
        0.6f,
        0.6f,
        -6.0f,
        8.0f,
        0.4f,
        30.0f,
        0.0f,
        37.5f,
        0.5f,
        42,
        1,
        200,
        2,
        0.0f,
        28.0f,
        0.0f,
        50.0f,
        37.5f,
        30.0f,
        50.0f,
        500.0f,
        0.0f,
        Z2SE_OBJ_BREAK_WOOD_S,
        Z2SE_OBJ_PUT_WBOX_S,
        Z2SE_OBJ_PUT_WATER_S,
        JA_SE_OBJ_FALL_MAGMA_S,
        Z2SE_OBJ_FALL_WATER_S,
        0xB,
        87.0f,
        0x59,
        0x2D,
        0x2D,
        10.0f,
        0.1f,
        1.5f,
        1.0f,
        0xA00,
    },
    // TYPE_IRON_BALL
    {
        25.0f,
        30.0f,
        30.0f,
        80.0f,
        0.1f,
        0.55f,
        -6.0f,
        2.5f,
        0.4f,
        0.0f,
        35.0f,
        0.0f,
        1.0f,
        42,
        8,
        200,
        3,
        0.0f,
        0.0f,
        0.0f,
        40.0f,
        0.0f,
        30.0f,
        40.0f,
        1000000.0f,
        50.0f,
        Z2SE_OBJ_BREAK_WOOD_S,
        Z2SE_OBJ_IRONBALL_HIT,
        Z2SE_OBJ_PUT_WATER_M,
        JA_SE_OBJ_FALL_MAGMA_S,
        Z2SE_OBJ_FALL_WATER_S,
        0xB,
        40.0f,
        0x2,
        0x8C,
        0x8C,
        50.0f,
        0.1f,
        1.5f,
        1.5f,
        0x1000,
    },
    // TYPE_TARU
    {
        22.0f,
        43.0f,
        30.0f,
        80.0f,
        0.6f,
        0.6f,
        -6.5f,
        10.0f,
        0.4f,
        40.0f,
        0.0f,
        50.0f,
        1.0f,
        42,
        4,
        200,
        3,
        0.0f,
        48.0f,
        0.0f,
        68.0f,
        50.0f,
        50.0f,
        50.0f,
        500.0f,
        0.0f,
        Z2SE_OBJ_BREAK_BARREL,
        Z2SE_OBJ_PUT_WBOX_S,
        Z2SE_OBJ_PUT_WATER_M,
        JA_SE_OBJ_FALL_MAGMA_M,
        Z2SE_OBJ_FALL_WATER_M,
        0xD,
        50.0f,
        0x59,
        0x28,
        0x28,
        10.0f,
        0.1f,
        2.0f,
        1.5f,
        0xA00,
    },
    // TYPE_DOKURO
    {
        27.0f,
        36.0f,
        30.0f,
        80.0f,
        0.6f,
        0.6f,
        -6.0f,
        4.0f,
        0.4f,
        10.0f,
        0.0f,
        28.0f,
        1.0f,
        42,
        0,
        200,
        2,
        0.0f,
        28.0f,
        0.0f,
        48.0f,
        30.0f,
        30.0f,
        48.0f,
        250.0f,
        0.0f,
        Z2SE_OBJ_SKULL_BREAK,
        Z2SE_OBJ_SKULL_PUT,
        Z2SE_OBJ_PUT_WATER_S,
        JA_SE_OBJ_FALL_MAGMA_S,
        Z2SE_OBJ_FALL_WATER_S,
        0xD,
        40.0f,
        0x59,
        0x32,
        0x32,
        10.0f,
        0.1f,
        1.0f,
        1.0f,
        0xA00,
    },
    // TYPE_BOKKURI
    {
        0.0f,
        100.0f,
        30.0f,
        80.0f,
        0.6f,
        0.3f,
        -6.5f,
        8.0f,
        0.4f,
        30.0f,
        0.0f,
        50.0f,
        0.6f,
        42,
        0,
        200,
        2,
        0.0f,
        48.0f,
        0.0f,
        60.0f,
        45.0f,
        30.0f,
        60.0f,
        250.0f,
        0.0f,
        Z2SE_OBJ_BOKKURI_BURST,
        Z2SE_OBJ_BOKKURI_PUT,
        Z2SE_OBJ_PUT_WATER_S,
        JA_SE_OBJ_FALL_MAGMA_M,
        Z2SE_OBJ_FALL_WATER_M,
        0xD,
        45.0f,
        0x50,
        0x1E,
        0x1E,
        10.0f,
        0.15f,
        1.5f,
        1.5f,
        0xA00,
    },
    // TYPE_TSUBO_2
    {
        27.0f,
        36.0f,
        30.0f,
        80.0f,
        0.6f,
        0.6f,
        -6.0f,
        4.0f,
        0.4f,
        25.0f,
        0.0f,
        28.0f,
        1.0f,
        42,
        0,
        200,
        2,
        0.0f,
        28.0f,
        0.0f,
        48.0f,
        30.0f,
        30.0f,
        48.0f,
        250.0f,
        0.0f,
        JA_SE_OBJ_BREAK_CHINA_S,
        JA_SE_OBJ_PUT_POT_S,
        Z2SE_OBJ_PUT_WATER_S,
        JA_SE_OBJ_FALL_MAGMA_S,
        Z2SE_OBJ_FALL_WATER_S,
        0xD,
        40.0f,
        0x50,
        0x2D,
        0x2D,
        10.0f,
        0.1f,
        1.0f,
        1.0f,
        0xA00,
    },
    // TYPE_BALL_S
    {
        27.0f,
        36.0f,
        30.0f,
        80.0f,
        0.7f,
        0.7f,
        -6.0f,
        2.5f,
        0.4f,
        0.0f,
        40.0f,
        0.0f,
        1.0f,
        45,
        0,
        200,
        3,
        0.0f,
        0.0f,
        0.0f,
        60.0f,
        40.0f,
        50.0f,
        60.0f,
        1000000.0f,
        50.0f,
        Z2SE_OBJ_BREAK_WOOD_S,
        Z2SE_OBJ_L8_L_BALL_BOUND,
        Z2SE_OBJ_PUT_WATER_M,
        JA_SE_OBJ_FALL_MAGMA_S,
        Z2SE_OBJ_FALL_WATER_S,
        0xB,
        40.0f,
        0x2,
        0x4B,
        0x6E,
        10.0f,
        0.015f,
        1.5f,
        1.5f,
        0x1000,
    },
    // TYPE_BALL_S_2
    {
        27.0f,
        36.0f,
        30.0f,
        80.0f,
        0.7f,
        0.7f,
        -6.0f,
        2.5f,
        0.4f,
        0.0f,
        40.0f,
        0.0f,
        1.0f,
        45,
        0,
        200,
        3,
        0.0f,
        0.0f,
        0.0f,
        100.0f,
        40.0f,
        50.0f,
        40.0f,
        1000000.0f,
        50.0f,
        Z2SE_OBJ_BREAK_WOOD_S,
        Z2SE_OBJ_L8_L_BALL_BOUND,
        Z2SE_OBJ_PUT_WATER_M,
        JA_SE_OBJ_FALL_MAGMA_S,
        Z2SE_OBJ_FALL_WATER_S,
        0xB,
        40.0f,
        0x2,
        0x4B,
        0x6E,
        10.0f,
        0.015f,
        2.0f,
        2.0f,
        0x1000,
    },
    // TYPE_AOTSUBO
    {
        22.0f,
        43.0f,
        30.0f,
        80.0f,
        0.6f,
        0.6f,
        -6.5f,
        3.0f,
        0.4f,
        35.0f,
        0.0f,
        50.0f,
        1.0f,
        42,
        4,
        200,
        3,
        0.0f,
        48.0f,
        0.0f,
        68.0f,
        50.0f,
        50.0f,
        68.0f,
        250.0f,
        0.0f,
        JA_SE_OBJ_BREAK_CHINA_L,
        JA_SE_OBJ_PUT_POT_L,
        Z2SE_OBJ_PUT_WATER_M,
        JA_SE_OBJ_FALL_MAGMA_M,
        Z2SE_OBJ_FALL_WATER_M,
        0xD,
        50.0f,
        0x50,
        0x14,
        0x14,
        10.0f,
        0.1f,
        1.5f,
        1.5f,
        0xA00,
    },
    // TYPE_LV8_BALL
    {
        27.0f,
        36.0f,
        30.0f,
        80.0f,
        0.7f,
        0.7f,
        -6.0f,
        2.5f,
        0.4f,
        0.0f,
        40.0f,
        0.0f,
        1.0f,
        45,
        0,
        200,
        3,
        0.0f,
        0.0f,
        0.0f,
        60.0f,
        40.0f,
        50.0f,
        60.0f,
        1000000.0f,
        50.0f,
        Z2SE_OBJ_BREAK_WOOD_S,
        Z2SE_OBJ_L8_L_BALL_BOUND,
        Z2SE_OBJ_PUT_WATER_M,
        JA_SE_OBJ_FALL_MAGMA_S,
        Z2SE_OBJ_FALL_WATER_S,
        0xB,
        40.0f,
        0x2,
        0x4B,
        0x6E,
        10.0f,
        0.015f,
        1.5f,
        1.5f,
        0x1000,
    },
    // TYPE_TSUBO_S
    {
        27.0f,
        36.0f,
        30.0f,
        80.0f,
        0.6f,
        0.6f,
        -6.0f,
        4.0f,
        0.4f,
        25.0f,
        0.0f,
        28.0f,
        1.0f,
        42,
        0,
        200,
        2,
        0.0f,
        28.0f,
        0.0f,
        48.0f,
        30.0f,
        30.0f,
        48.0f,
        250.0f,
        0.0f,
        JA_SE_OBJ_BREAK_CHINA_S,
        JA_SE_OBJ_PUT_POT_S,
        Z2SE_OBJ_PUT_WATER_S,
        JA_SE_OBJ_FALL_MAGMA_S,
        Z2SE_OBJ_FALL_WATER_S,
        0xD,
        40.0f,
        0x50,
        0x2D,
        0x2D,
        10.0f,
        0.1f,
        1.0f,
        1.0f,
        0xA00,
    },
    // TYPE_TSUBO_B
    {
        22.0f,
        43.0f,
        30.0f,
        80.0f,
        0.6f,
        0.6f,
        -6.5f,
        3.0f,
        0.4f,
        35.0f,
        0.0f,
        50.0f,
        1.0f,
        42,
        4,
        200,
        3,
        0.0f,
        48.0f,
        0.0f,
        68.0f,
        50.0f,
        50.0f,
        50.0f,
        250.0f,
        0.0f,
        JA_SE_OBJ_BREAK_CHINA_L,
        JA_SE_OBJ_PUT_POT_L,
        Z2SE_OBJ_PUT_WATER_M,
        JA_SE_OBJ_FALL_MAGMA_M,
        Z2SE_OBJ_FALL_WATER_M,
        0xD,
        50.0f,
        0x50,
        0x14,
        0x14,
        10.0f,
        0.1f,
        1.5f,
        1.5f,
        0xA00,
    },
};

const static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{AT_TYPE_THROW_OBJ, 0x2, 0x1f}, {0xd8fbfdff, 0x1f}, 0x79}},  // mObj
        {dCcD_SE_THROW_OBJ, 0x1, 0x0, 0x0, 0x1},                            // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                                 // mGObjTg
        {0x0},                                                              // mGObjCo
    },                                                                      // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        10.0f,               // mRadius
        50.0f                // mHeight
    }  // mCyl
};

const static dCcD_SrcSph l_light_at_sph_src = {
    {
        {0x0, {{AT_TYPE_LANTERN_SWING, 0x0, 0x11}, {0x0, 0x10}, 0x0}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, dCcD_MTRL_LIGHT, 0x0},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                             // mGObjTg
        {0x0},                                                          // mGObjCo
    },                                                                  // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 500.0f}  // mSph
    }  // mSphAttr
};

const static dCcD_SrcCps l_atCpsSrc = {
    {
        {0x0, {{AT_TYPE_IRON_BALL, 0x64, 0x1f}, {0x0, 0x10}, 0x0}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1},                          // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                          // mGObjTg
        {0x0},                                                       // mGObjCo
    },                                                               // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 40.0f},  // mCps
    }  // mCpsAttr
};

const static dCcD_SrcCyl l_tg_cyl = {
    {
        {0x0, {{0x0, 0x0, 0x1e}, {0x200, 0x11}, 0x0}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1},             // mGObjAt
        {dCcD_SE_WOLF_KICK, 0x0, 0x0, 0x0, 0x4},        // mGObjTg
        {0x0},                                          // mGObjCo
    },                                                  // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        40.0f,               // mRadius
        60.0f                // mHeight
    }  // mCyl
};

static const GXColor l_light_color = {0xFF, 0xFF, 0xFF, 0xFF};

static char* l_arcName[] = {
    "J_tubo_00", "J_tubo_01", "Kkiba_00",  "Y_ironbal", "J_taru00", "J_doku00", "Obj_bkl",
    "K_tubo02",  "Obj_ballS", "Obj_ballS", "D_aotubo0", "Obj_tama", "O_tuboS",  "O_tuboB",
};

static char* l_bmdName[] = {
    "J_tubo_00.bmd",       "J_tubo_01.bmd",       "J_hako_00.bmd",  "Yironball.bmd",
    "J_taru_00.bmd",       "J_doku_00.bmd",       "K_hb00.bmd",     "K_tubo02.bmd",
    "LV8_OBJ_HikariS.bmd", "LV8_OBJ_HikariS.bmd", "D_aotubo00.bmd", "LV8_tama.bmd",
    "O_tuboS_LV8.bmd",     "O_tuboB_LV8.bmd",
};

static Vec kibako_jv_offset[] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 150.0f, 0.0f},
};

static Vec taru_jv_offset[] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 100.0f, 0.0f},
};

// box joint col data
static dJntColData_c kibako_jc_data = {1, 1, 0, 45.0f, kibako_jv_offset};

static dJntColData_c taru_jc_data = {1, 1, 0, 45.0f, taru_jv_offset};

static f32 bound(cXyz* param_0, const cBgS_PolyInfo& param_1, f32 param_2) {
    cM3dGPla plane;

    bool var_r29 = dComIfG_Bgsp().GetTriPla(param_1, &plane);
    if (var_r29) {
        cXyz pos;
        f32 abs = param_0->absXZ();

        VECReflect(param_0, &plane.mNormal, &pos);
        *param_0 = (pos * abs) * param_2;
        return param_0->absXZ();
    } else {
        return 0.0f;
    }
}

static void TgHitCallBackBase(fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf,
                              fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf) {}

static void TgHitCallBackBokkuri(fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf,
                                 fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf) {
    daObjCarry_c* objCarry = (daObjCarry_c*)i_tgActor;
    if (objCarry != NULL && i_atActor != NULL && i_atObjInf->ChkAtType(AT_TYPE_SHIELD_ATTACK) &&
        fopAcM_IsActor(i_atActor) && fopAcM_GetName(i_atActor) == PROC_ALINK)
    {
        objCarry->field_0xd5c = 6000.0f + cM_rndF(2000.0f);
        objCarry->field_0xd60 = 4000.0f + cM_rndF(1500.0f);
        objCarry->field_0xd68 = 4000;
    }
}

static void CoHitCallBackBase(fopAc_ac_c* i_coActorA, dCcD_GObjInf* i_coObjInfA,
                              fopAc_ac_c* i_coActorB, dCcD_GObjInf* i_coObjInfB) {}

static void CoHitCallBackBokkuri(fopAc_ac_c* i_coActorA, dCcD_GObjInf* i_coObjInfA,
                                 fopAc_ac_c* i_coActorB, dCcD_GObjInf* i_coObjInfB) {
    daObjCarry_c* objCarry = (daObjCarry_c*)i_coActorA;

    if (objCarry != NULL && i_coActorB != NULL) {
        cXyz* cc_move_p = objCarry->mStts.GetCCMoveP();
        if (cc_move_p != NULL) {
            if (std::fabs(cc_move_p->x) > 0.5f && std::fabs(cc_move_p->z) > 0.5f) {
                objCarry->field_0xd5c = 800.0f + cM_rndF(400.0f);
                objCarry->field_0xd60 = 500.0f + cM_rndF(300.0f);

                if (objCarry->field_0xdb0 != 0) {
                    objCarry->field_0xd5c *= 0.5f;
                    objCarry->field_0xd60 *= 0.5f;
                }
            }
        }
    }
}

static void* s_swA_sub(void* i_actor, void* i_data) {
    daObjSwBallA_c* swball = (daObjSwBallA_c*)i_actor;
    daObjCarry_c* objCarry = (daObjCarry_c*)i_data;

    if (fopAcM_IsActor(swball) && fopAcM_GetName(swball) == PROC_Obj_SwBallA && swball != NULL &&
        objCarry != NULL)
    {
        if (swball->getSwbit() == objCarry->getSwbit2()) {
            return i_actor;
        }
    }

    return NULL;
}

static int l_sw_id;

static void* s_sw_sub(void* i_actor, void* i_data) {
    daObjSwBallB_c* swball = (daObjSwBallB_c*)i_actor;

    if (fopAcM_IsActor(swball) && fopAcM_GetName(swball) == PROC_Obj_SwBallB && swball != NULL &&
        swball->getID() == l_sw_id)
    {
        return i_actor;
    }

    return NULL;
}

static void* s_setA_sub(void* i_actor, void* i_data) {
    daTagSetBall_c* tagball = (daTagSetBall_c*)i_actor;

    if (fopAcM_IsActor(tagball) && fopAcM_GetName(tagball) == PROC_Tag_SetBall && tagball &&
        i_data != NULL && tagball->getType() == 0xF)
    {
        return i_actor;
    }

    return NULL;
}

static void* s_setB_sub(void* i_actor, void* i_data) {
    daTagSetBall_c* tagball = (daTagSetBall_c*)i_actor;

    if (fopAcM_IsActor(tagball) && fopAcM_GetName(tagball) == PROC_Tag_SetBall && tagball &&
        i_data != NULL && tagball->getType() == 0)
    {
        return i_actor;
    }

    return NULL;
}

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daObjCarry_c*)i_this)->CreateHeap();
}

const daObjCarry_dt_t& daObjCarry_c::data() {
    return mData[mType];
}

char* daObjCarry_c::getArcName() {
    return l_arcName[mType];
}

char* daObjCarry_c::getBmdName() {
    return l_bmdName[mType];
}

BOOL daObjCarry_c::checkFlag(u8 i_flag) {
    return cLib_checkBit<u8>(data().m_flags, i_flag);
}

void daObjCarry_c::initBaseMtx() {
    mRotAxis = cXyz::Zero;
    mRotation = 0;
    field_0xd3c = ZeroQuat;
    field_0xd4c = field_0xd3c;
    setBaseMtx();
}

void daObjCarry_c::setBaseMtx() {
    Mtx rot_mtx;

    if (mRotAxis.isZero()) {
        mDoMtx_identity(rot_mtx);
    } else {
        MTXRotAxisRad(rot_mtx, &mRotAxis, cM_s2rad(mRotation));
    }

    if (mType == TYPE_BOKKURI) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y + data().field_0x28, current.pos.z);
        mDoMtx_stack_c::transM(0.0f, scale.x * data().field_0x2c, 0.0f);
        mDoMtx_stack_c::concat(rot_mtx);
        mDoMtx_stack_c::transM(0.0f, scale.x * -data().field_0x2c, 0.0f);
        mDoMtx_stack_c::ZXYrotM(shape_angle.x + field_0xd64, shape_angle.y + field_0xd6a, shape_angle.z + field_0xd66);
        mDoMtx_stack_c::transM(0.0f, scale.x * 35.0f * field_0xd84, field_0xd84 * -25.0f);
        mDoMtx_stack_c::XrotM(field_0xd84 * 16384.0f);
    } else if (mType == TYPE_KIBAKO || mType == TYPE_TARU || mType == TYPE_DOKURO) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y + data().field_0x28, current.pos.z);
        mDoMtx_stack_c::transM(0.0f, data().field_0x2c, 0.0f);
        mDoMtx_stack_c::concat(rot_mtx);
        mDoMtx_stack_c::transM(0.0f, -data().field_0x2c, 0.0f);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
    } else {
        mDoMtx_quatMultiply(&field_0xd4c, &field_0xd3c, &field_0xd4c);
        mDoMtx_stack_c::transS(current.pos.x, data().field_0x28 + current.pos.y + field_0xddc, current.pos.z);
        mDoMtx_stack_c::transM(0.0f, data().field_0x2c, 0.0f);
        mDoMtx_stack_c::quatM(&field_0xd4c);
        mDoMtx_stack_c::transM(0.0, -data().field_0x2c, 0.0);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        field_0xd3c = field_0xd4c;
    }

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModel->setBaseScale(scale);
}

int daObjCarry_c::preInit() {
    fopAcM_ct(this, daObjCarry_c);

    if (!mInitParams) {
        mItemNo = home.angle.x;
        field_0xd18 = home.angle.z;
        home.angle.z = 0;
        home.angle.x = 0;
        current.angle.z = 0;
        current.angle.x = 0;
        shape_angle.z = 0;
        shape_angle.x = 0;
        mOnMoveBG = checkOnMoveBg();
        mInitParams = true;
    }

    mType = getType_private();
    return 1;
}

daObjCarry_c::daObjCarry_c() {}

BOOL daObjCarry_c::checkBreakWolfAttack() {
    if (mType == TYPE_TSUBO   ||
        mType == TYPE_TSUBO_2 ||
        mType == TYPE_OOTSUBO ||
        mType == TYPE_KIBAKO  ||
        mType == TYPE_DOKURO  ||
        mType == TYPE_AOTSUBO ||
        mType == TYPE_TSUBO_S ||
        mType == TYPE_TSUBO_B)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL daObjCarry_c::checkCarryBoomerang() {
    if (mType == TYPE_TSUBO   ||
        mType == TYPE_TSUBO_2 ||
        mType == TYPE_OOTSUBO ||
        mType == TYPE_KIBAKO  ||
        mType == TYPE_DOKURO  ||
        mType == TYPE_BOKKURI ||
        mType == TYPE_AOTSUBO ||
        mType == TYPE_TSUBO_S ||
        mType == TYPE_TSUBO_B)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL daObjCarry_c::checkCarryHookshot() {
    if (mType == TYPE_BALL_S   ||
        mType == TYPE_BALL_S_2 ||
        mType == TYPE_LV8_BALL)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL daObjCarry_c::checkCarryWolf() {
    if (mType == TYPE_TSUBO   ||
        mType == TYPE_DOKURO  ||
        mType == TYPE_TSUBO_2 ||
        mType == TYPE_TSUBO_S)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL daObjCarry_c::checkCarryOneHand() {
    if (mType == TYPE_TSUBO   ||
        mType == TYPE_DOKURO  ||
        mType == TYPE_TSUBO_2 ||
        mType == TYPE_BOKKURI ||
        mType == TYPE_TSUBO_S)
    {
        return TRUE;
    }

    return FALSE;
}

int daObjCarry_c::Create() {
    scale.set(data().scale, data().scale, data().scale);

    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    
    mAcchCir.SetWall(data().m_height, data().m_radius);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), &current.angle, &shape_angle);
    mAcch.SetWtrChkMode(2);
    mAcch.ClrRoofNone();
    mAcch.SetRoofCrrHeight(80.0f);

    mStts.Init(data().m_colliderWeight, 0xFF, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);

    mCyl.cM3dGCyl::Set(l_cyl_info[mType]);
    mCyl.SetAtAtp(data().m_cyl_atp);
    setTgHitCallBack();
    setCoHitCallBack();

    if (mType != TYPE_BOKKURI) {
        if (mType == TYPE_BALL_S) {
            mLightAtSph.Set(l_light_at_sph_src);
            mLightAtSph.SetStts(&mStts);
        } else if (mType == TYPE_BALL_S_2) {
            mLightAtSph.Set(l_light_at_sph_src);
            mLightAtSph.SetStts(&mStts);
        } else if (prm_chk_type_ironball()) {
            mAtCps.Set(l_atCpsSrc);
            mAtCps.SetStts(&mStts);
        } else if (mType == TYPE_TSUBO || mType == TYPE_OOTSUBO || mType == TYPE_TSUBO_2 || mType == TYPE_AOTSUBO) {
            mTgCyl.Set(l_tg_cyl);
            mTgCyl.SetStts(&mStts);
        }
    }

    fopAcM_SetCullSize(this, fopAc_CULLSPHERE_CUSTOM_e);
    fopAcM_setCullSizeSphere(this, data().m_cullsph_min_x, data().m_cullsph_min_y, data().m_cullsph_min_z, mpModel->getModelData()->getJointNodePointer(0)->getRadius() * data().scale);

    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    attention_info.distances[fopAc_attn_CARRY_e] = data().m_carry_attn_dist;

    if (checkFlag(4)) {
        fopAcM_OnCarryType(this, fopAcM_CARRY_TYPE_1);
    } else if (checkFlag(8)) {
        fopAcM_OnCarryType(this, fopAcM_CARRY_HEAVY);
    }

    field_0xcec = current.pos.y;
    fopAcM_SetGravity(this, data().m_gravity);
    field_0xd70 = cM_rndF(100.0f);
    field_0xd04 = field_0xd70;

    if (checkBreakWolfAttack()) {
        mCyl.SetTgType(mCyl.GetTgType() | 0x20000000);
    }

    u32 tg_type = mCyl.GetTgType();
    if (!checkCarryBoomerang()) {
        mCyl.SetTgType(tg_type & ~0x10000);
    }

    if (checkCarryHookshot()) {
        fopAcM_OnStatus(this, 0x80000);
        mCyl.SetTgType(mCyl.GetTgType() | 0x4000);
    }

    if (checkCarryWolf()) {
        fopAcM_OnCarryType(this, fopAcM_CARRY_LIGHT);
    }

    if (checkCarryOneHand()) {
        fopAcM_OnCarryType(this, fopAcM_CARRY_SIDE);
    }

    if (mType == TYPE_TSUBO || mType == TYPE_OOTSUBO || mType == TYPE_TSUBO_2 || mType == TYPE_AOTSUBO) {
        mCyl.SetTgType(mCyl.GetTgType() | 0x2000000);
    }

    if (mType != TYPE_TSUBO && mType != TYPE_TSUBO_2 && mType != TYPE_TSUBO_S && mType != TYPE_OOTSUBO && mType != TYPE_AOTSUBO && mType != TYPE_TSUBO_B) {
        mCyl.OnTgNoSlingHitInfSet();
    }

    if (!CreateInitCall()) {
        return 0;
    }

    field_0xdaf = 1;

    if (mMode == MODE_DB_DROP) {
        field_0xcf3 = 0;
        field_0xdb1 = 0;
    } else {
        field_0xcf3 = 10;
        field_0xdb1 = 1;
    }

    cXyz gnd_chk_pos(current.pos);
    gnd_chk_pos.y += 10.0f;

    if (fopAcM_gc_c::gndCheck(&gnd_chk_pos)) {
        if (std::fabs(fopAcM_gc_c::getGroundY() - current.pos.y) < 30.0f) {
            home.pos.y = fopAcM_gc_c::getGroundY();
            current.pos.y = home.pos.y;
            old.pos.y = home.pos.y;
        } else {
            // "Lift OBJ: not grounded on BG. \n"
            OS_REPORT("\x1B[43;30m持ち上げＯＢＪ：ＢＧに接地して配置されていません。\n");
            OS_REPORT("type<%d>pos<%.2f,%.2f,%.2f>\n", mType, home.pos.x, home.pos.y, home.pos.z);
            // "BG Height: %.2f\n"
            OS_REPORT("ＢＧ高さ:%.2f\n\x1b[m", fopAcM_gc_c::getGroundY());
        }
    }

    mAcch.CrrPos(dComIfG_Bgsp());
    initBaseMtx();
    return 1;
}

int daObjCarry_c::CreateInit_tsubo() {
    if (!checkCrashRoll()) {
        mCanCrashRoll = true;
    }

    mode_init_wait();

    if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0) {
        field_0xe25 = 1;
    }

    return 1;
}

int daObjCarry_c::CreateInit_ootubo() {
    mode_init_wait();
    return 1;
}

int daObjCarry_c::CreateInit_kibako() {
    if (!mJntCol.init(this, &kibako_jc_data, mpModel, 1)) {
        return 0;
    }

    mode_init_wait();
    return 1;
}

int daObjCarry_c::CreateInit_ironball() {
    if (chkSaveFlag()) {
        u8 l_saveID = getSaveID();

        current.pos = getPos(l_saveID);
        old.pos = current.pos;
        attention_info.position = current.pos;
        eyePos = current.pos;

        fopAcM_SetRoomNo(this, getRoomNo(l_saveID));
        tevStr.room_no = fopAcM_GetRoomNo(this);

        if (chkSttsFlag(l_saveID, 1)) {
            mDraw = true;
        }
    }

    mAcch.SetLink();
    mAcch.ClrObj();

    mode_init_wait();

    mCyl.SetAtType(mCyl.GetAtType() | AT_TYPE_IRON_BALL);
    mCyl.SetAtSe(6);
    return 1;
}

int daObjCarry_c::CreateInit_taru() {
    if (!mJntCol.init(this, &taru_jc_data, mpModel, 1)) {
        return 0;
    }

    mode_init_wait();
    return 1;
}

int daObjCarry_c::CreateInit_dokuro() {
    mode_init_wait();
    return fopAcM_isSwitch(this, getSwbit()) == FALSE ? TRUE : FALSE;
}

int daObjCarry_c::CreateInit_bokkuri() {
    mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_0);
    mCyl.OnTgNoHitMark();

    field_0xd76 = 0;
    field_0xd79 = 3;
    field_0xd7a = 100;

    mSound.init(&current.pos, 1);

    switch (getSetType()) {
    case 0:
        mode_init_wait();
        break;
    case 1:
        field_0xd76 = 255;
        speedF = 15.0f;
        speed.y = 20.0f;
        scale.set(1.0f, 1.0f, 1.0f);
        mode_init_dbDrop(1);
        break;
    case 2:
        fopAcM_OnCarryType(this, fopAcM_CARRY_TYPE_1);
        field_0xdb0 = 1;
        field_0xd7a = 0;
        mode_init_wait();
        break;
    case 3:
        fopAcM_OnCarryType(this, fopAcM_CARRY_TYPE_1);
        field_0xd76 = 255;
        field_0xd7a = 0;
        mode_init_growth();
        break;
    default:
        mode_init_wait();
    }

    return 1;
}

int daObjCarry_c::CreateInit_LightBall() {
    mSound.init(&current.pos, 1);

    if (dComIfGp_roomControl_getStayNo()) {
        fopAcM_OnStatus(this, 0x2000000);
    }

    mDalkmistInf.mPos = current.pos;
    mDalkmistInf.field_0xc = 750.0f;

    dKy_dalkmist_inf_set(&mDalkmistInf);
    field_0xe0c = 1;
    fopAcM_OnStatus(this, 0x20000);

    mCyl.SetTgSe(9);
    mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
    mCyl.OffTgNoHitMark();
    mCyl.OnTgShieldHit();
    mCyl.SetAtSe(1);

    mode_init_wait();
    return 1;
}

int daObjCarry_c::CreateInit_Lv8Ball() {
    mSound.init(&current.pos, 1);

    mCyl.SetTgSe(9);
    mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
    mCyl.OffTgNoHitMark();
    mCyl.OnTgShieldHit();

    if (fopAcM_isSwitch(this, getSwbit())) {
        mode_init_fit();
    } else {
        mode_init_wait();
    }

    return 1;
}

int daObjCarry_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(getArcName(), getBmdName());
    JUT_ASSERT(2813, modelData != NULL);

    u32 mdl_flags = prm_chk_type_ironball() ? J3DMdlFlag_None : J3DMdlFlag_DifferedDLBuffer;
    mpModel = mDoExt_J3DModel__create(modelData, mdl_flags, 0x11000084);
    if (mpModel == 0) {
        return 0;
    }

    return 1;
}

int daObjCarry_c::create() {
    preInit();

    if (!(DEBUG && prm_chk_type_lightball() &&
          strcmp(dComIfGp_getStartStageName(), "T_ENE02") == 0)) {
        int create_phase;
        create_phase = cPhs_NEXT_e;

        switch (mType) {
        case TYPE_BALL_S:
            create_phase = (cPhs__Step)checkCreate_LightBallA();
            break;
        case TYPE_BALL_S_2:
            create_phase = (cPhs__Step)checkCreate_LightBallB();
            break;
        case TYPE_LV8_BALL:
            create_phase = (cPhs__Step)checkCreate_Lv8Ball();
            break;
        }

        if (create_phase != cPhs_NEXT_e) {
            return create_phase;
        }
    }

    int phase_state = dComIfG_resLoad(&mPhaseReq, getArcName());
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, data().m_heapSize)) {
            return cPhs_ERROR_e;
        }

        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }

    return phase_state;
}

int daObjCarry_c::checkCreate_LightBallA() {
    fopAc_ac_c* var_r29 = NULL;

    if (daPy_py_c::checkCarryStartLightBallA()) {
        /* Palace of Twilight - Palace of Twilight use 1 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0311);
        /* Palace of Twilight - Palace of Twilight use 3 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0313);
        /* Palace of Twilight - Palace of Twilight use 5 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0315);
        /* Palace of Twilight - Palace of Twilight use 7 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0317);
        /* Palace of Twilight - Palace of Twilight use 9 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0319);

        if (dComIfGp_roomControl_getStayNo() == 51) {
            /* Palace of Twilight - Palace of Twilight use 5 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0315);
        } else if (dComIfGp_roomControl_getStayNo() == 52) {
            /* Palace of Twilight - Palace of Twilight use 9 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0319);
        } else if (dComIfGp_roomControl_getStayNo() == 0) {
            /* Palace of Twilight - Palace of Twilight use 1 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0311);
        } else if (dComIfGp_roomControl_getStayNo() == 1 || dComIfGp_roomControl_getStayNo() == 2) {
            /* Palace of Twilight - Palace of Twilight use 3 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0313);
        } else {
            /* Palace of Twilight - Palace of Twilight use 7 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0317);
        }

        fopAcM_setCarryNow(this, 1);

        if (dComIfGp_roomControl_getStayNo() == 0) {
            var_r29 = (fopAc_ac_c*)fpcM_Search(s_setA_sub, this);
            if (var_r29 == NULL) {
                return cPhs_INIT_e;
            }
        }
                /* Palace of Twilight - Palace of Twilight use 1 */
    } else if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0311)
                   /* Palace of Twilight - Palace of Twilight use 3 */
               && !dComIfGs_isEventBit(dSv_event_flag_c::F_0313)
                   /* Palace of Twilight - Palace of Twilight use 5 */
               && !dComIfGs_isEventBit(dSv_event_flag_c::F_0315)
                   /* Palace of Twilight - Palace of Twilight use 7 */
               && !dComIfGs_isEventBit(dSv_event_flag_c::F_0317)
                   /* Palace of Twilight - Palace of Twilight use 9 */
               && !dComIfGs_isEventBit(dSv_event_flag_c::F_0319))
    {
        if (dComIfGp_roomControl_getStayNo() != 51) {
                 /* Palace of Twilight - Palace of Twilight use 1 */
            if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0311)) {
                resetIconPosForLightBallA();
            }

            // "Light Ball A: delete since ball and Link aren't in same place<0>\n"
            OS_REPORT("\x1B[33m光球Ａ：玉とリンクの位置が違うので消します<0>\n\x1B[m");
            return cPhs_ERROR_e;
        }
                /* Palace of Twilight - Palace of Twilight use 1 */
    } else if ((dComIfGs_isEventBit(dSv_event_flag_c::F_0311)
                && dComIfGp_roomControl_getStayNo()) ||
                /* Palace of Twilight - Palace of Twilight use 3 */
               (dComIfGs_isEventBit(dSv_event_flag_c::F_0313)
                && dComIfGp_roomControl_getStayNo() != 1 && dComIfGp_roomControl_getStayNo() != 2) ||
                /* Palace of Twilight - Palace of Twilight use 5 */
               (dComIfGs_isEventBit(dSv_event_flag_c::F_0315)
                && dComIfGp_roomControl_getStayNo() != 51) ||
                /* Palace of Twilight - Palace of Twilight use 9 */
               (dComIfGs_isEventBit(dSv_event_flag_c::F_0319)
                && dComIfGp_roomControl_getStayNo() != 52) ||
                /* Palace of Twilight - Palace of Twilight use 7 */
               (dComIfGs_isEventBit(dSv_event_flag_c::F_0317)
                && dComIfGp_roomControl_getStayNo() != 4 && dComIfGp_roomControl_getStayNo() != 5))
    {
             /* Palace of Twilight - Palace of Twilight use 1 */
        if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0311)) {
            resetIconPosForLightBallA();
        } else {
            setIconPosForLightBallAAtR00();
        }

        // "Light Ball A: delete since ball and Link aren't in same place<1>\n"
        OS_REPORT("\x1B[33m光球Ａ：玉とリンクの位置が違うので消します<1>\n\x1B[m");
        return cPhs_ERROR_e;
    } else {
        if (dComIfGp_roomControl_getStayNo() == 0) {
            if (fopAcM_isSwitch(this, 63)) {
                return cPhs_ERROR_e;
            }
            
            l_sw_id = -1;

            for (int i = 0; i < 6; i++) {
                static const u16 l_event_bitA[] = {
                    dSv_event_flag_c::F_0551, /* Palace of Twilight - Palace of Twilight control use 1 */
                    dSv_event_flag_c::F_0552, /* Palace of Twilight - Palace of Twilight control use 2 */
                    dSv_event_flag_c::F_0553, /* Palace of Twilight - Palace of Twilight control use 3 */
                    dSv_event_flag_c::F_0554, /* Palace of Twilight - Palace of Twilight control use 4 */
                    dSv_event_flag_c::F_0555, /* Palace of Twilight - Palace of Twilight control use 5 */
                    dSv_event_flag_c::F_0556, /* Palace of Twilight - Palace of Twilight control use 6 */
                };

                if (dComIfGs_isEventBit(l_event_bitA[i])) {
                    l_sw_id = i;
                    break;
                }
            }

            if (l_sw_id != -1) {
                var_r29 = (fopAc_ac_c*)fpcM_Search(s_sw_sub, this);
                // "Light Ball A: Set to Switch OBJ<%d> position!\n"
                OS_REPORT("光球Ａ：スイッチＯＢＪ<%d>の位置へセットします！\n", l_sw_id);
            } else {
                var_r29 = (fopAc_ac_c*)fpcM_Search(s_setA_sub, this);
                // "Light Ball A: Set to Reset Tag position!\n"
                OS_REPORT("光球Ａ：再セットタグの位置へセットします！\n");
            }

            if (var_r29 == NULL) {
                return cPhs_INIT_e;
            }
        } else if (dComIfGp_roomControl_getStayNo() != 51) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetRoomNo(this, dComIfGp_roomControl_getStayNo());
        tevStr.room_no = fopAcM_GetRoomNo(this);
    }

    if (var_r29 != NULL) {
        home.pos = var_r29->home.pos;
        current.pos = home.pos;
        old.pos = home.pos;
        eyePos = home.pos;
        attention_info.position = home.pos;
    }

    return cPhs_NEXT_e;
}

BOOL daObjCarry_c::checkCreate_LightBallB() {
    fopAc_ac_c* var_r29 = NULL;

    if (daPy_py_c::checkCarryStartLightBallB()) {
        /* Palace of Twilight - Palace of Twilight use 2 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0312);
        /* Palace of Twilight - Palace of Twilight use 4 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0314);
        /* Palace of Twilight - Palace of Twilight use 6 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0316);
        /* Palace of Twilight - Palace of Twilight use 8 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0318);
        /* Palace of Twilight - Palace of Twilight use 10 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0320);

        if (dComIfGp_roomControl_getStayNo() == 51) {
            /* Palace of Twilight - Palace of Twilight use 6 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0316);
        } else if (dComIfGp_roomControl_getStayNo() == 52) {
            /* Palace of Twilight - Palace of Twilight use 10 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0320);
        } else if (dComIfGp_roomControl_getStayNo() == 0) {
            /* Palace of Twilight - Palace of Twilight use 2 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0312);
        } else if (dComIfGp_roomControl_getStayNo() == 1 || dComIfGp_roomControl_getStayNo() == 2) {
            /* Palace of Twilight - Palace of Twilight use 4 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0314);
        } else {
            /* Palace of Twilight - Palace of Twilight use 8 */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0318);
        }

        fopAcM_setCarryNow(this, 1);

        if (dComIfGp_roomControl_getStayNo() == 0) {
            var_r29 = (fopAc_ac_c*)fpcM_Search(s_setB_sub, this);
            if (var_r29 == NULL) {
                return cPhs_INIT_e;
            }
        }
                /* Palace of Twilight - Palace of Twilight use 2 */
    } else if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0312)
                   /* Palace of Twilight - Palace of Twilight use 4 */
               && !dComIfGs_isEventBit(dSv_event_flag_c::F_0314)
                   /* Palace of Twilight - Palace of Twilight use 6 */
               && !dComIfGs_isEventBit(dSv_event_flag_c::F_0316)
                   /* Palace of Twilight - Palace of Twilight use 8 */
               && !dComIfGs_isEventBit(dSv_event_flag_c::F_0318)
                   /* Palace of Twilight - Palace of Twilight use 10 */
               && !dComIfGs_isEventBit(dSv_event_flag_c::F_0320))
    {
        if (dComIfGp_roomControl_getStayNo() != 52) {
                 /* Castle Town - Showed reciept to town doctor */
            if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0321)) {
                resetIconPosForLightBallB();
            }

            // "Light Ball B: delete since ball and Link aren't in same place<2>\n"
            OS_REPORT("\x1B[33m光球Ｂ：玉とリンクの位置が違うので消します<2>\n\x1B[m");
            return cPhs_ERROR_e;
        }
                /* Palace of Twilight - Palace of Twilight use 2 */
    } else if ((dComIfGs_isEventBit(dSv_event_flag_c::F_0312)
               && dComIfGp_roomControl_getStayNo()) ||
                /* Palace of Twilight - Palace of Twilight use 4 */
               (dComIfGs_isEventBit(dSv_event_flag_c::F_0314)
               && dComIfGp_roomControl_getStayNo() != 1 && dComIfGp_roomControl_getStayNo() != 2) ||
                /* Palace of Twilight - Palace of Twilight use 6 */
               (dComIfGs_isEventBit(dSv_event_flag_c::F_0316)
               && dComIfGp_roomControl_getStayNo() != 51) ||
                /* Palace of Twilight - Palace of Twilight use 10 */
               (dComIfGs_isEventBit(dSv_event_flag_c::F_0320)
               && dComIfGp_roomControl_getStayNo() != 52) ||
                /* Palace of Twilight - Palace of Twilight use 8 */
               (dComIfGs_isEventBit(dSv_event_flag_c::F_0318)
               && dComIfGp_roomControl_getStayNo() != 4 && dComIfGp_roomControl_getStayNo() != 5))
    {
             /* Palace of Twilight - Palace of Twilight use 2 */
        if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0312)) {
            resetIconPosForLightBallB();
        } else {
            setIconPosForLightBallBAtR00();
        }

        // "Light Ball B: delete since ball and Link aren't in same place<3>\n"
        OS_REPORT("\x1B[33m光球Ｂ：玉とリンクの位置が違うので消します<3>\n\x1B[m");
        return cPhs_ERROR_e;
    } else {
        if (dComIfGp_roomControl_getStayNo() == 0) {
            if (fopAcM_isSwitch(this, 63)) {
                return cPhs_ERROR_e;
            }
            
            l_sw_id = -1;

            for (int i = 0; i < 6; i++) {
                static const u16 l_event_bitB[] = {
                    dSv_event_flag_c::F_0557, /* Palace of Twilight - Palace of Twilight control use 7 */
                    dSv_event_flag_c::F_0558, /* Palace of Twilight - Palace of Twilight control use 8 */
                    dSv_event_flag_c::F_0559, /* Palace of Twilight - Palace of Twilight control use 9 */
                    dSv_event_flag_c::F_0560, /* Palace of Twilight - Palace of Twilight control use 10 */
                    dSv_event_flag_c::F_0561, /* Palace of Twilight - Palace of Twilight control use 11 */
                    dSv_event_flag_c::F_0562, /* Palace of Twilight - Palace of Twilight control use 12 */
                };

                if (dComIfGs_isEventBit(l_event_bitB[i])) {
                    l_sw_id = i;
                    break;
                }
            }

            if (l_sw_id != -1) {
                var_r29 = (fopAc_ac_c*)fpcM_Search(s_sw_sub, this);
                // "Light Ball B: Set to Switch OBJ<%d> position!\n"
                OS_REPORT("光球Ｂ：スイッチＯＢＪ<%d>の位置へセットします！\n", l_sw_id);
            } else {
                var_r29 = (fopAc_ac_c*)fpcM_Search(s_setB_sub, this);
               // "Light Ball B: Set to Reset Tag position!\n"
                OS_REPORT("光球Ｂ：再セットタグの位置へセットします！\n");
            }

            if (var_r29 == NULL) {
                return cPhs_INIT_e;
            }
        } else if (dComIfGp_roomControl_getStayNo() != 52) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetRoomNo(this, dComIfGp_roomControl_getStayNo());
        tevStr.room_no = fopAcM_GetRoomNo(this);
    }

    if (var_r29 != NULL) {
        home.pos = var_r29->home.pos;
        current.pos = home.pos;
        old.pos = home.pos;
        eyePos = home.pos;
        attention_info.position = home.pos;
    }

    return cPhs_NEXT_e;
}

BOOL daObjCarry_c::checkCreate_Lv8Ball() {
    if (!fopAcM_isSwitch(this, getSwbit())) {
        return cPhs_NEXT_e;
    }

    fopAc_ac_c* var_r29 = (fopAc_ac_c*)fpcM_Search(s_swA_sub, this);
    if (var_r29 == NULL) {
        return cPhs_INIT_e;
    }

    current.pos = var_r29->home.pos;
    old.pos = current.pos;
    attention_info.position = current.pos;
    eyePos = current.pos;
    return cPhs_NEXT_e;
}

void daObjCarry_c::resetIconPosForLightBallA() {
    cXyz pos(0.0f, 100.0f, -3930.0f);
    dTres_c::setIconPositionOfCarryLight(getTrboxBit(), &pos, 51);
}

void daObjCarry_c::setIconPosForLightBallAAtR00() {
    cXyz pos;

    if (fopAcM_isSwitch(this, 61)) {
        pos.set(0.0f, -210.0f, 11000.0f);
    } else if (fopAcM_isSwitch(this, 62)) {
        pos.set(500.0f, -210.0f, 11000.0f);
    } else {
        pos.set(-4700.0f, -590.0f, 11600.0f);
    }

    dTres_c::setIconPositionOfCarryLight(getTrboxBit(), &pos, 0);
}

void daObjCarry_c::setIconPosForLightBallBAtR00() {
    cXyz pos;

    if (fopAcM_isSwitch(this, 61)) {
        pos.set(0.0f, -210.0f, 11000.0f);
    } else if (fopAcM_isSwitch(this, 62)) {
        pos.set(500.0f, -210.0f, 11000.0f);
    } else {
        pos.set(5100.0f, -590.0f, 11600.0f);
    }

    dTres_c::setIconPositionOfCarryLight(getTrboxBit(), &pos, 0);
}

void daObjCarry_c::resetIconPosForLightBallB() {
    cXyz pos(0.0f, 100.0f, -3930.0f);
    dTres_c::setIconPositionOfCarryLight(getTrboxBit(), &pos, 52);
}

int daObjCarry_c::execute() {
    mRecover = 0;

    if (prm_chk_type_ironball()) {
        dTres_c::setCarryIron(getTrboxBit(), &current.pos);
    } else if (prm_chk_type_lightball()) {
        dTres_c::setIconPositionOfCarryLight(getTrboxBit(), &current.pos, fopAcM_GetRoomNo(this));
        fopAcM_seStartLevel(this, Z2SE_OBJ_L8_L_BALL, 0);

        if (mAcch.ChkGroundHit() && !mAcch.ChkGroundLanding()) {
            cLib_chaseF(&field_0xe20, 1.0f, 0.05f + KREG_F(1));
        } else {
            cLib_chaseF(&field_0xe20, 2.0f, 0.05f + KREG_F(1));
        }

        GXColor color = (GXColor){0x5F, 0x5F, 0x5F, 0xFF};
        int var_r27 = dKy_BossLight_set(&current.pos, &color, field_0xe20 + KREG_F(0), 0);
        if (!var_r27) {
            OS_REPORT_ERROR("光球：ライト登録できませんでした\n");
        }
    }

    if ((prm_chk_type_ironball() && getRoomNo() != 0x3F) || (prm_chk_type_lightball() && getRoomNo() != 0x3F)) {
        if (fopAcM_GetRoomNo(this) == -1) {
            cXyz gnd_chk_pos(current.pos);
            gnd_chk_pos.y += 30.0f;

            if (fopAcM_gc_c::gndCheck(&gnd_chk_pos)) {
                fopAcM_SetRoomNo(this, fopAcM_gc_c::getRoomId());
                tevStr.room_no = fopAcM_GetRoomNo(this);
            }
        }

        int room_no = fopAcM_GetRoomNo(this);
        if (room_no == -1) {
            room_no = getRoomNo();
        }

        if (!dComIfGp_roomControl_checkRoomDisp(room_no)) {
            if (prm_chk_type_lightball()) {
                fopAcM_delete(this);
            }
            return 1;
        }

        f32 reset_dist = 10000.0f;
        if (prm_chk_type_ironball()) {
            reset_dist = 4000.0f;
        }

        if (home.pos.y - current.pos.y > reset_dist) {
            mReset = true;
        }
    }

    if (mType == TYPE_DOKURO && getSwbit() != 0xFF && !fopAcM_isSwitch(this, getSwbit())) {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        return 1;
    }

    if (mDeleteTimer != 0) {
        if (--mDeleteTimer == 0) {
            fopAcM_delete(this);
        }
        return 1;
    }

    if (mCtrl != 0) {
        if (field_0xdb9 == 0) {
            mode_init_wait();
        }

        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        calc_rot_call();
        setBaseMtx();
        field_0xdb9 = mCtrl;
        field_0xdec = current.pos;
        return 1;
    }

    if (field_0xdb9 != 0) {
        if (field_0xdea != 0) {
            speed.x = (speedF * cM_ssin(current.angle.y)) + (field_0xde4 * cM_ssin(field_0xde8));
            speed.y += field_0xde0;
            speed.z = (speedF * cM_scos(current.angle.y)) + (field_0xde4 * cM_scos(field_0xde8));
            speedF = std::sqrt((speed.x * speed.x) + (speed.z * speed.z));
            current.angle.y = cM_atan2s(speed.x, speed.z);

            field_0xde4 = 0.0f;
            field_0xde0 = 0.0f;
            field_0xde8 = 0;

            if (speedF > data().m_throwInitSpeedXZ && speed.y > data().m_throwInitSpeedY) {
                mode_init_drop(0);
            } else {
                mode_init_walk(0);
            }

            if (mpCannonActor != NULL && fopAcM_IsActor(mpCannonActor) && fopAcM_GetProfName(mpCannonActor) == PROC_Obj_Ytaihou) {
                mStts.ClrCcMove();
                mCyl.SetAtAtp(100);
                mAcch.SetIronBall();
                // "Fired from cannon! speedF<%f> speed.y<%f>\n"
                OS_REPORT("大砲から発射されました！speedF<%f>speed.y<%f>\n", speedF, speed.y);
                mpCannonActor = NULL;
                mCannon = true;
            }

            field_0xdea = 0;
        } else if (mType == TYPE_LV8_BALL || (prm_chk_type_lightball() && dComIfGp_roomControl_getStayNo() == 0)) {
            mode_init_fit();
        } else {
            mode_init_wait();
        }
    } else if (field_0xdec != current.pos && !fopAcM_checkCarryNow(this) && mMode != MODE_CARRY && !fopAcM_checkHookCarryNow(this) && mMode != MODE_HOOK_CARRY && mMode != MODE_WAIT && mMode != MODE_FIT) {
        mode_init_wait();
    }

    if (mReset) {
        mReset = false;
        current.pos = home.pos;
        current.pos.y -= 10.0f;
        eyePos = current.pos;
        old.pos = current.pos;
        field_0xdec = current.pos;
        speed.y = 0.0f;

        if (prm_chk_type_lightball()) {
            fopAcM_seStart(this, Z2SE_OBJ_L8_L_BALL_REVIVE, 0);
            scale.setall(0.0f);
            mode_init_resetLightBall();
        } else {
            if (prm_chk_type_ironball()) {
                if (fopAcM_GetRoomNo(this) != getRoomNo()) {
                    fopAcM_SetRoomNo(this, getRoomNo());
                    tevStr.room_no = fopAcM_GetRoomNo(this);
                }
            }
            mode_init_wait();
        }
    }

    field_0xd1c = speed;
    field_0xd04++;

    cc_damage_proc_call();
    mode_proc_call();
    bg_check();
    calc_rot_call();
    obj_execute_proc_call();

    mStts.Move();

    if (field_0xdae == 0) {
        mCyl.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mCyl);

        if (mCyl.ChkCoSPrm(1) && mMode == MODE_DROP) {
            mAcchCir.SetWall(0.5f * data().m_height, 0.5f * data().m_radius);
        } else {
            mAcchCir.SetWall(data().m_height, data().m_radius);
        }
    } else {
        mAcchCir.SetWall(data().m_height, data().m_radius);
    }

    if (mMode == MODE_WAIT || mMode == MODE_WALK || mMode == MODE_SINK || mMode == MODE_DROP) {
        dComIfG_Ccsp()->SetMass(&mCyl, 3);
    }

    attention_info.position.x = current.pos.x;
    attention_info.position.y = current.pos.y + data().field_0x48;
    attention_info.position.z = current.pos.z;
    eyePos = attention_info.position;

    field_0xda9 = fopAcM_checkCarryNow(this) != 0;
    field_0xdac = mCyl.ChkCoHit() != 0;
    field_0xdad = mAcch.ChkWaterIn() != 0;

    if (field_0xdaa != 0) {
        fopAcM_SetModel(this, mpModel);
    } else {
        fopAcM_SetModel(this, NULL);
    }

    field_0xdb9 = mCtrl;
    setBaseMtx();

    if (mType == TYPE_KIBAKO || mType == TYPE_TARU) {
        mpModel->calc();
    }

    field_0xdaf = 0;
    if (cLib_calcTimer<s8>(&field_0xe24) == 0) {
        mAcch.OffLineCheck();
    }

    field_0xdec = current.pos;

    if (fopAcM_checkCarryNow(this)) {
        fopAcM_OffStatus(this, 0x400);
    } else {
        fopAcM_OnStatus(this, 0x400);
    }

    return 1;
}

BOOL daObjCarry_c::checkCulling() {
    BOOL is_cull = FALSE;

    if ((prm_chk_type_ironball() && getRoomNo() != 0x3F) || (prm_chk_type_lightball() && getRoomNo() != 0x3F)) {
        int room_no = fopAcM_GetRoomNo(this);
        if (room_no == -1) {
            room_no = getRoomNo();
        }

        if (!dComIfGp_roomControl_checkRoomDisp(room_no)) {
            return TRUE;
        }
    }

    if (daPy_py_c::wolfGrabSubjectNoDraw(this)) {
        is_cull = TRUE;
    }

    if (mDraw) {
        is_cull = TRUE;
    }

    if (mType == TYPE_DOKURO && getSwbit() != 0xFF && !fopAcM_isSwitch(this, getSwbit())) {
        is_cull = TRUE;
    }

    if (KREG_S(2) == 10000) {
        is_cull = FALSE;
    }

    return is_cull;
}

int daObjCarry_c::draw() {
    if (checkCulling()) {
        return 1;
    }

    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    if (mType == TYPE_BOKKURI) {
        J3DModelData* modelData = mpModel->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* material = modelData->getMaterialNodePointer(i);
            J3DGXColor* tev_color = material->getTevKColor(1);
            tev_color->a = field_0xd76;
        }
    } else if (prm_chk_type_lightball() && dComIfGp_roomControl_getStayNo() == 0) {
        J3DModelData* modelData = mpModel->getModelData();
        JUTNameTab* nametab = modelData->getMaterialName();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* material = modelData->getMaterialNodePointer(i);
            J3DGXColor* tev_kcolor = material->getTevKColor(1);
            J3DGXColorS10* tev_color = material->getTevColor(1);

            if (strcmp(nametab->getName(i), "cc_ball") == 0) {
                tev_color->r = 0x82;
                tev_color->g = 0xC8;
                tev_color->b = 0xFA;
                tev_color->a = 0xFF;

                tev_kcolor->r = 0x00;
                tev_kcolor->g = 0x3C;
                tev_kcolor->b = 0x8C;
                tev_kcolor->a = 0xFF;
            } else if (strcmp(nametab->getName(i), "aa_hikari") == 0) {
                tev_kcolor->r = 0x8C;
                tev_kcolor->g = 0xD2;
                tev_kcolor->b = 0xFF;
                tev_kcolor->a = 0xFF;
            }
        }
    }

    mDoExt_modelUpdateDL(mpModel);

    if (mType == TYPE_IRON_BALL) {
        daMirror_c::entry(mpModel);
    }

    if (fopAcM_GetModel(this) == NULL) {
        f32 temp_f31 = scale.x * data().field_0x74;
        if (checkFlag(1)) {
            dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), temp_f31, mAcch.m_gnd, shape_angle.y, 1.0f, NULL);
        } else {
            dComIfGd_setSimpleShadow(&current.pos, KREG_F(0) + mAcch.GetGroundH(), temp_f31, mAcch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
        }
    }

    debugDraw();
    return 1;
}

void daObjCarry_c::debugDraw() {
#if DEBUG
    if (KREG_S(8) == 10000 && !mRotAxis.isZero()) {
        cXyz start(current.pos);
        start.y += 35.0f;

        cXyz end = current.pos + (mRotAxis.norm() * 100.0f);
        end.y += 35.0f;

        GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
        dDbVw_drawLineOpa(start, end, color, TRUE, 12);
    }

    if (KREG_S(8) == 20000) {
        cXyz start(daPy_getPlayerActorClass()->current.pos);
        cXyz end(home.pos);
        GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
        dDbVw_drawLineOpa(start, end, color, TRUE, 12);
    }

    if (KREG_S(8) == 20001) {
        cXyz start(daPy_getPlayerActorClass()->current.pos);
        cXyz end(home.pos);
        GXColor color = {0xFF, 0x00, 0x00, 0xFF};
        dDbVw_drawLineOpa(start, end, color, TRUE, 12);
    }

    if (KREG_S(7) == 4000) {
        GXColor color = {0xFF, 0x00, 0x00, 0xFF};
        cXyz sp80(0.0f, 0.0f, 100.0f);
        mDoMtx_stack_c::YrotS(current.angle.y);
        mDoMtx_stack_c::multVec(&sp80, &sp80);
        sp80 += current.pos;
        dDbVw_drawArrowOpa(current.pos, sp80, color, TRUE, 12);

        color = (GXColor){0x00, 0x00, 0xFF, 0xFF};
        sp80.set(0.0f, 0.0f, 100.0f);
        mDoMtx_stack_c::YrotS(shape_angle.y);
        mDoMtx_stack_c::multVec(&sp80, &sp80);
        sp80 += current.pos;
        dDbVw_drawArrowOpa(current.pos, sp80, color, TRUE, 12);
    }

    if (KREG_S(7) == 5000) {
        GXColor color = {0x00, 0x00, 0xFF, 0xFF};
        cXyz sp74(current.pos);
        cXyz sp68;

        int sp28;
        if (fopAcM_getWaterStream(&current.pos, mAcch.m_gnd, &sp68, &sp28, 0)) {
            cXyz sp5C = sp68 * 100.0f;
            sp5C += current.pos;
            dDbVw_drawArrowOpa(current.pos, sp5C, color, TRUE, 12);
        }
    }

    if (mType == TYPE_KIBAKO || mType == TYPE_TARU) {
        // mJntCol.debugDraw();
    }
#endif
}

int daObjCarry_c::_delete() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    fopAcM_offActor(this, fopAcM_GetSetId(this));

    if (mType == TYPE_BOKKURI || mType == TYPE_LV8_BALL || prm_chk_type_lightball()) {
        mSound.deleteObject();
    }

    if ((mType == TYPE_BALL_S || mType == TYPE_BALL_S_2) && field_0xe0c != 0) {
        dKy_dalkmist_inf_cut(&mDalkmistInf);
        field_0xe0c = 0;
    }

    if (mType == TYPE_BALL_S) {
             /* Palace of Twilight - Palace of Twilight use 1 */
        if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0311)
            || (player != NULL && player->getGrabActorID() != fopAcM_GetID(this))) {
            /* Palace of Twilight - Palace of Twilight use 3 */
            dComIfGs_offEventBit(dSv_event_flag_c::F_0313);
            /* Palace of Twilight - Palace of Twilight use 5 */
            dComIfGs_offEventBit(dSv_event_flag_c::F_0315);
            /* Palace of Twilight - Palace of Twilight use 7 */
            dComIfGs_offEventBit(dSv_event_flag_c::F_0317);
            /* Palace of Twilight - Palace of Twilight use 9 */
            dComIfGs_offEventBit(dSv_event_flag_c::F_0319);

                 /* Palace of Twilight - Palace of Twilight use 1 */
            if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0311)) {
                cXyz pos(0.0f, 100.0f, -3930.0f);
                dTres_c::setIconPositionOfCarryLight(getTrboxBit(), &pos, 51);
            }
        }

        if (fopAcM_isSwitch(this, 90)
                /* Palace of Twilight - Palace of Twilight use 1 */
            && !dComIfGs_isEventBit(dSv_event_flag_c::F_0311)
            && player != NULL) {
            if (player->getGrabActorID() != fopAcM_GetID(this)) {
                fopAcM_onSwitch(this, 170);
                // "Midna Hint Switch set!\n"
                OSReport_Error("ミドナヒント用スイッチ立てました！\n");
            }
        }
                                           /* Palace of Twilight - Palace of Twilight use 2 */
    } else if (mType == TYPE_BALL_S_2 && (!dComIfGs_isEventBit(dSv_event_flag_c::F_0312)
               || (player != NULL && player->getGrabActorID() != fopAcM_GetID(this)))) {
        /* Palace of Twilight - Palace of Twilight use 4 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0314);
        /* Palace of Twilight - Palace of Twilight use 6 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0316);
        /* Palace of Twilight - Palace of Twilight use 8 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0318);
        /* Palace of Twilight - Palace of Twilight use 10 */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0320);

             /* Palace of Twilight - Palace of Twilight use 2 */
        if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0312)) {
            cXyz pos(0.0f, 100.0f, -3930.0f);
            dTres_c::setIconPositionOfCarryLight(getTrboxBit(), &pos, 52);
        }
    }

    if (prm_chk_type_ironball()) {
        if (-G_CM3D_F_INF != mAcch.GetGroundH()) {
            savePos(getSaveID(), current.pos);
            setRoomNo(getSaveID(), fopAcM_GetRoomNo(this));
        } else {
            int room_no = fopAcM_GetRoomNo(this);
            if (room_no == -1) {
                room_no = getRoomNo();
            }

            if (!dComIfGp_roomControl_checkRoomDisp(room_no)) {
                savePos(getSaveID(), current.pos);
                setRoomNo(getSaveID(), room_no);
            }
        }

        if (!isDraw()) {
            onSttsFlag(getSaveID(), 1);
        } else {
            offSttsFlag(getSaveID(), 1);
        }
    }

    dComIfG_resDelete(&mPhaseReq, getArcName());
    return 1;
}

void daObjCarry_c::crr_pos() {
    if (field_0xd14 != 0) {
        mAcch.ClrRoofHit();
        mAcch.ClrWallHit();
        mAcch.ClrGroundLanding();
        mAcch.ClrGroundHit();
        mAcch.ClrWaterHit();
        mAcch.ClrWaterIn();
        return;
    }

    bool var_r29 = false;
    cXyz sp20(current.pos);
    if (mMode == MODE_CARRY || field_0xdb0 == 1) {
        var_r29 = true;
    }

    mAcch.CrrPos(dComIfG_Bgsp());

    if (field_0xe25 != 0) {
        cXyz sp14(current.pos.x, current.pos.y - 5000.0f, current.pos.z);
        mAcch.m_wtr.Set(sp14, 5000.0f + current.pos.y);

        if (dComIfG_Bgsp().WaterChk(&mAcch.m_wtr)) {
            mAcch.SetWaterHit();
            if (mAcch.m_wtr.GetHeight() > current.pos.y) {
                mAcch.SetWaterIn();
            }
        }
    }

    field_0xd08 = current.pos - sp20;

    if (dComIfG_Bgsp().ChkMoveBG_NoDABg(mAcch.m_gnd) && mOnMoveBG) {
        var_r29 = false;
        mOnMoveBG = false;
    }

    if (mType == TYPE_IRON_BALL) {
        if (mAcch.ChkGroundHit()) {
            switch (dComIfG_Bgsp().GetPolyAtt0(mAcch.m_gnd)) {
            case 13:
                field_0xddc = -20.0f;
                break;
            }
        } else {
            cLib_chaseF(&field_0xddc, 0.0f, 1.0f);
        }
    }

    if (var_r29) {
        if (field_0xdb0 != 0) {
            current.pos.x = sp20.x;
            current.pos.z = sp20.z;
        } else {
            current.pos = sp20;
        }
    }
}

int daObjCarry_c::check_sink(f32* param_0) {
    int var_r29 = 0;
    f32 temp_f31 = mAcch.m_wtr.GetHeight();

    *param_0 = 0.0f;

    if (mAcch.ChkWaterHit()) {
        if (temp_f31 > current.pos.y + (2.0f * data().field_0x2c)) {
            var_r29 = 2;
            *param_0 = 1.0f;
        } else if (temp_f31 > current.pos.y) {
            var_r29 = 1;
            *param_0 = std::fabs(temp_f31 - current.pos.y) / (2.0f * data().field_0x2c);
        }
    }

    return var_r29;
}

f32 daObjCarry_c::calc_gravity() {
    f32 obj_gravity = fopAcM_GetGravity(this);

    f32 sp8;
    int temp_r3 = check_sink(&sp8);
    switch (temp_r3) {
    case 0:
        obj_gravity = data().m_gravity;
        break;
    case 1:
    case 2:
        obj_gravity = data().m_gravity + (sp8 * data().m_Buoyancy);
        break;
    }

    return obj_gravity;
}

BOOL daObjCarry_c::checkRollAngle() {
    f32 roll_angle = data().m_rollAngle;

    cXyz gnd_chk_pos(current.pos);
    gnd_chk_pos.y += 50.0f;

    cM3dGPla plane;
    bool gnd_chk = fopAcM_gc_c::gndCheck(&gnd_chk_pos);
    bool valid_plane = fopAcM_gc_c::getTriPla(&plane);
    f32 ground_y = fopAcM_gc_c::getGroundY();

    if (gnd_chk && ground_y != -G_CM3D_F_INF && valid_plane) {
        f32 roll_threshold = cM_scos(cM_deg2s(roll_angle - 0.5f));
        cXyz normal(plane.mNormal);
        if (normal.y < roll_threshold) {
            return TRUE;
        }
    }

    return FALSE;
}

void daObjCarry_c::mode_proc_call() {
    typedef int (daObjCarry_c::*mode_func)();
    static mode_func l_mode_func[] = {
        &daObjCarry_c::mode_proc_wait,
        &daObjCarry_c::mode_proc_walk,
        &daObjCarry_c::mode_proc_carry,
        &daObjCarry_c::mode_proc_drop,
        &daObjCarry_c::mode_proc_float,
        &daObjCarry_c::mode_proc_sink,
        &daObjCarry_c::mode_proc_yogan,
        &daObjCarry_c::mode_proc_magne,
        &daObjCarry_c::mode_proc_magneCarry,
        &daObjCarry_c::mode_proc_boomCarry,
        &daObjCarry_c::mode_proc_growth,
        &daObjCarry_c::mode_proc_dbDrop,
        &daObjCarry_c::mode_proc_hookCarry,
        &daObjCarry_c::mode_proc_end,
        &daObjCarry_c::mode_proc_fit,
        &daObjCarry_c::mode_proc_controled,
        &daObjCarry_c::mode_proc_resetLightBall,
    };

    if (field_0xdae == 0) {
        if (field_0xcf2 != 0x11) {
            if (field_0xcf2 == 3) {
                mode_init_drop(0);
            }
            field_0xcf2 = 0x11;
        }

        if (!fopAcM_checkCarryNow(this) || mCarryHookOK) {
            if (mCyl.ChkTgHit()) {
                cCcD_Obj* tg_hit_obj = mCyl.GetTgHitObj();
                if (tg_hit_obj != NULL && tg_hit_obj->ChkAtType(AT_TYPE_BOOMERANG)) {
                    mode_init_boomCarry();
                }
            } else if (fopAcM_checkHookCarryNow(this)) {
                if (mMode != MODE_HOOK_CARRY) {
                    mode_init_hookCarry();
                }

                if (fopAcM_checkCarryNow(this)) {
                    mRecover = 1;
                    fopAcM_cancelCarryNow(this);
                }

                if (prm_chk_type_lightball() && fopAcM_searchPlayerDistance(this) < 250.0f) {
                    daPy_py_c* player = daPy_getPlayerActorClass();
                    if (player->setForceGrab(this, 0, 1)) {
                        mode_init_carry();
                        fopAcM_cancelHookCarryNow(this);
                        field_0xda9 = 1;
                    }
                }
            }
        } else if (field_0xda9 == 0) {
            mode_init_carry();
        }

        (this->*l_mode_func[mMode])();

        crr_pos();

        if (mMode == MODE_BOOM_CARRY) {
            mBoomerangMove.bgCheckAfterOffset(&current.pos);
        }

        bg_damage_proc_call();
    }
}

void daObjCarry_c::mode_init_wait() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    field_0xda8 = 0;

    speedF = 0.0f;
    gravity = data().m_gravity;
    fopAcM_SetMaxFallSpeed(this, -100.0f);
    mStts.SetWeight(data().m_colliderWeight);

    if (prm_chk_type_ironball()) {
        mAcch.ClrIronBall();
    }

    mMode = MODE_WAIT;
}

int daObjCarry_c::mode_proc_wait() {
    bool do_crash_roll = false;
    bool var_r26 = true;
    bool is_roll_angle = checkRollAngle();

    if (field_0xdb0 != 0) {
        fopAcM_posMoveF(this, NULL);
    } else {
        if (mCanCrashRoll) {
            daPy_py_c* player = dComIfGp_getLinkPlayer();
            if (player->current.pos.absXZ(current.pos) < (500.0f + KREG_F(9)) && (player->checkFrontRollCrash() || player->checkWolfAttackReverse())) {
                do_crash_roll = true;
                mCanCrashRoll = false;
            }
        }

        mAcch.ClrMoveBGOnly();
        mAcch.ClrGrndNone();
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());

    if (mAcch.ChkGroundHit()) {
        field_0xcec = current.pos.y;
    }

    if (is_roll_angle && var_r26) {
        mode_init_walk(0);
        return 1;
    }

    if (do_crash_roll) {
        speedF = 30.0f + KREG_F(8);
        mode_init_walk(0);
        return 1;
    }

    if (0.0f != speedF) {
        mode_init_walk(0);
        return 1;
    }

    return 1;
}

void daObjCarry_c::mode_init_walk(u8 unused) {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    field_0xda8 = 0;
    mCanCrashRoll = false;

    mStts.SetWeight(data().m_colliderWeight);

    fopAcM_SetMaxFallSpeed(this, -100.0f);
    gravity = data().m_gravity;

    if (prm_chk_type_ironball()) {
        mAcch.ClrIronBall();
    }

    mMode = MODE_WALK;
}

int daObjCarry_c::mode_proc_walk() {
    bool gnd_hit = mAcch.ChkGroundHit() != 0;
    bool gnd_landing = mAcch.ChkGroundLanding() != 0;
    bool gnd_away = mAcch.ChkGroundAway() != 0;

    if (gnd_hit) {
        field_0xcec = current.pos.y;
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

        if (0.0f == speedF && 0.0f == speed.y) {
            mode_init_wait();
        }

        if (mType == TYPE_IRON_BALL) {
            if (speedF > 25.0f) {
                cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            } else {
                cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            }
        }
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }

    f32 var_f29 = data().m_rollAngle;

    cXyz sp40(current.pos);
    sp40.y += 50.0f;

    f32 speed_scale = 1.0f - data().m_friction;

    cM3dGPla plane;
    bool gnd_check = fopAcM_gc_c::gndCheck(&sp40);
    bool valid_plane = fopAcM_gc_c::getTriPla(&plane);
    bool sp8 = false;

    f32 var_f31 = data().m_slopeInfluence;
    f32 temp_f25 = fopAcM_gc_c::getGroundY();

    if (gnd_check && -G_CM3D_F_INF != temp_f25 && gnd_hit && !gnd_landing) {
        bool var_r27 = 1;
        if (fopAcM_gc_c::getPolyAtt0() == 3) {
            var_r27 = 0;
        }

        if (mType == TYPE_IRON_BALL && fopAcM_gc_c::getPolyAtt0() == 13) {
            var_f29 = 50.0f;
            speed_scale = 0.8f;
            var_f31 = 20.0f;
        }

        if (valid_plane && var_r27) {
            f32 temp_f30 = cM_scos(cM_deg2s(var_f29 - 0.5f));
            cXyz sp34(plane.mNormal);

            if (sp34.y < temp_f30) {
                cXyz sp28(sp34);
                s16 temp_r26 = cM_atan2s(sp28.x, sp28.z);

                var_f31 *= 1.0f - sp34.y;
                if (var_f31 < 0.1f) {
                    var_f31 = 0.1f;
                }

                cXyz sp1C(speedF * cM_ssin(current.angle.y), 0.0f, speedF * cM_scos(current.angle.y));
                cXyz sp10(var_f31 * cM_ssin(temp_r26), 0.0f, var_f31 * cM_scos(temp_r26));
                sp1C += sp10;
                speedF = std::sqrt((sp1C.x * sp1C.x) + (sp1C.z * sp1C.z));

                if (!prm_chk_type_lightball()) {
                    f32 temp_f1 = std::sqrt((sp34.x * sp34.x) + (sp34.z * sp34.z));
                    if (0.0f != temp_f1) {
                        f32 temp_f26 = (0.9f + KREG_F(17)) * std::fabs((sp34.y / temp_f1) * data().m_gravity);
                        if (temp_f26 < speedF) {
                            speedF = temp_f26;
                        }
                    }
                } else {
                    f32 temp_f24 = std::sqrt((sp34.x * sp34.x) + (sp34.z * sp34.z));
                    if (0.0f != temp_f30) {
                        speed.y = -speedF * (temp_f24 / temp_f30);
                    }
                }

                current.angle.y = cM_atan2s(sp1C.x, sp1C.z);
                sp8 = 1;
            }
        }
    }

    speedF *= speed_scale;
    if (mType != TYPE_IRON_BALL && mStts.GetCCMoveP()->abs2XZ() > 0.010000001f) {
        speedF *= 0.9f;
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    return 1;
}

void daObjCarry_c::mode_init_carry() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OffCoSPrmBit(0x10);

    field_0xda8 = 0;
    mCanCrashRoll = false;
    mStts.SetWeight(data().m_colliderWeight);

    fopAcM_SetMaxFallSpeed(this, -100.0f);
    speedF = 0.0f;

    current.angle.setall(0);

    if (mMode == MODE_BOOM_CARRY) {
        field_0xdaa = 1;
    } else {
        field_0xdaa = 0;
    }

    if (field_0xdb0 == 1) {
        field_0xdb0 = 0;
        field_0xd7a = 1;
        fopAcM_OffCarryType(this, fopAcM_CARRY_TYPE_1);
        mDoAud_seStart(Z2SE_OBJ_BOKKURI_PULLOUT, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    field_0xd68 = 0;

    if (prm_chk_type_ironball()) {
        mAcch.ClrIronBall();
    }

    if (prm_chk_type_lightball()) {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

        daPy_py_c* player = daPy_getPlayerActorClass();
        if (player->getGrabActorID() == fopAcM_GetID(this)) {
            if (mType == TYPE_BALL_S) {
                     /* Palace of Twilight - Palace of Twilight use 1 */
                if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0311)
                        /* Palace of Twilight - Palace of Twilight use 3 */
                    && !dComIfGs_isEventBit(dSv_event_flag_c::F_0313)
                        /* Palace of Twilight - Palace of Twilight use 5 */
                    && !dComIfGs_isEventBit(dSv_event_flag_c::F_0315)
                        /* Palace of Twilight - Palace of Twilight use 7 */
                    && !dComIfGs_isEventBit(dSv_event_flag_c::F_0317)
                        /* Palace of Twilight - Palace of Twilight use 9 */
                    && !dComIfGs_isEventBit(dSv_event_flag_c::F_0319)) {
                    /* Palace of Twilight - Palace of Twilight use 5 */
                    dComIfGs_onEventBit(dSv_event_flag_c::F_0315);
                }
                        /* Palace of Twilight - Palace of Twilight use 2 */
            } else if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0312)
                           /* Palace of Twilight - Palace of Twilight use 4 */
                       && !dComIfGs_isEventBit(dSv_event_flag_c::F_0314)
                           /* Palace of Twilight - Palace of Twilight use 6 */
                       && !dComIfGs_isEventBit(dSv_event_flag_c::F_0316)
                           /* Palace of Twilight - Palace of Twilight use 8 */
                       && !dComIfGs_isEventBit(dSv_event_flag_c::F_0318)
                           /* Palace of Twilight - Palace of Twilight use 10 */
                       && !dComIfGs_isEventBit(dSv_event_flag_c::F_0320)) {
                /* Palace of Twilight - Palace of Twilight use 10 */
                dComIfGs_onEventBit(dSv_event_flag_c::F_0320);
            }
        }
    }

    mMode = MODE_CARRY;
}

int daObjCarry_c::mode_proc_carry() {
    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass();

    if (field_0xdaa == 0) {
        if (player->getGrabActorID() == fopAcM_GetID(this) && player->getGrabUpStart()) {
            field_0xdaa = 1;
        }
    }

    if (!prm_chk_type_lightball()) {
        if (mCyl.ChkCoHit()) {
            cXyz* cc_move_p = mStts.GetCCMoveP();
            if (cc_move_p != NULL) {
                field_0xd08 += *cc_move_p;
            }
        }

        mStts.ClrCcMove();
        mCyl.ClrCoHit();
        player->setGrabCollisionOffset(field_0xd08.x, field_0xd08.z, NULL);
    }

    if (!fopAcM_checkCarryNow(this)) {
        bool temp_r26 = fopAcM_GetSpeedF(this) != 0.0f;

        if (0.0f != fopAcM_GetSpeedF(this)) {
            if (current.angle.x == 0) {
                if (daPy_py_c::checkNowWolf()) {
                    speedF = data().m_wolfThrowInitSpeedXZ;
                    speed.y = data().m_wolfThrowInitSpeedY;
                } else {
                    speedF = data().m_throwInitSpeedXZ;
                    speed.y = data().m_throwInitSpeedY;
                }
            } else {
                if (daPy_py_c::checkNowWolf()) {
                    speedF = data().m_wolfThrowInitSpeedXZ;
                } else {
                    speedF = data().m_throwInitSpeedXZ;
                }

                s16 temp_r0 = -current.angle.x;
                speed.y = (speedF * cM_ssin(temp_r0)) / cM_scos(temp_r0);
            }
    
            current.angle.x = 0;
        }

        field_0xe24 = 30;

        mAcch.OnLineCheck();

        if (chkWaterLineIn()) {
            if (chkSinkObj()) {
                mode_init_sink();
            } else {
                mode_init_float();
            }
        } else if (temp_r26) {
            mode_init_drop(0);
        } else {
            mode_init_walk(0);
        }

        field_0xdaa = 0;
    }

    field_0xcec = current.pos.y;
    return 1;
}

void daObjCarry_c::mode_init_drop(u8 param_0) {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OnAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);
    mCyl.OffAtSPrmBit(0xC);

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

    mStts.SetWeight(data().m_colliderWeight);
    field_0xda8 = 0;
    mCanCrashRoll = false;

    fopAcM_SetMaxFallSpeed(this, -100.0f);

    gravity = calc_gravity();
    if (mType == TYPE_BOKKURI && mMode == MODE_CARRY) {
        gravity = -2.0f;
    }

    if (prm_chk_type_ironball()) {
        mCyl.SetAtAtp(1);
    }

    mCannon = false;
    mMode = MODE_DROP;
    field_0xdb4 = 0;
}

int daObjCarry_c::mode_proc_drop() {
    if (field_0xdb4 == 0) {
        mAcch.OnLineCheck();
        field_0xdb4 = 1;
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());

    if (prm_chk_type_ironball() && mCyl.GetAtAtp() == 100) {
        mAtCps.cCcD_CpsAttr::Set(old.pos, current.pos, 40.0f);

        cXyz sp8 = old.pos - current.pos;
        mAtCps.SetAtVec(sp8);
        mAtCps.SetAtAtp(100);
        dComIfG_Ccsp()->Set(&mAtCps);
    }

    return 1;
}

void daObjCarry_c::mode_init_float() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

    field_0xda8 = 0;
    field_0xdc4 = 0;
    field_0xdc8 = 0.0f;

    if (speedF > 10.0f || speed.y < -20.0f) {
        field_0xd5c = 3000.0f;
        field_0xd60 = 3000.0f;
    }

    speed.y = -12.0f;
    speedF = cLib_minMaxLimit<f32>(speedF, -10.0f, 10.0f);

    mStts.SetWeight(KREG_S(2) + 100);

    field_0xd6c = fopAcM_GetSpeed_p(this)->y;

    if (prm_chk_type_ironball()) {
        mAcch.ClrIronBall();
    }

    mMode = MODE_FLOAT;
}

int daObjCarry_c::mode_proc_float() {
    field_0xdc4++;
    gravity = 0.0f;

    cXyz sp34;
    f32 speed_target;

    int spC = 0;
    if (fopAcM_getWaterStream(&current.pos, mAcch.m_gnd, &sp34, &spC, 0)) {
        s16 sp8 = cM_atan2s(sp34.x, sp34.z);

        cXyz sp28(speedF * cM_ssin(current.angle.y), 0.0f, speedF * cM_scos(current.angle.y));
        sp34.normalizeZP();

        sp34 *= 0.2f * spC;
        sp28 += sp34;

        current.angle.y = cM_atan2s(sp28.x, sp28.z);

        speed_target = 0.8f * spC;
        if (speed_target < 2.0f) {
            speed_target = 2.0f;
        }
    } else {
        speed_target = 0.0f;
    }

    cLib_addCalc(&speedF, speed_target, 0.05f, 0.1f, 0.01f);
    cLib_addCalc0(&speed.y, 1.0f, 0.2f);

    fopAcM_posMoveF(this, mStts.GetCCMoveP());

    cXyz sp1C(current.pos);
    int temp_r27 = fopAcM_getWaterY(&current.pos, &sp1C.y);
    f32 temp_f28 = field_0xdc8 * cM_ssin(field_0xdc4 * 1000);

    if (temp_r27 != 0) {
        current.pos.y = sp1C.y - data().field_0x24;
        cLib_addCalc2(&field_0xdc8, 0.5f, 0.05f, 1.0f);
        current.pos.y += temp_f28;
    }

    f32 var_f30 = 800.0f;
    f32 var_f29 = 800.0f;

    if (mCyl.ChkCoHit() && field_0xdac == 0) {
        var_f30 = 1200.0f;
        var_f29 = 1200.0f;
    }

    cLib_addCalc2(&field_0xd5c, var_f30, 0.03f, 100.0f);
    cLib_addCalc2(&field_0xd60, var_f29, 0.03f, 100.0f);

    field_0xd64 = field_0xd5c * cM_ssin(field_0xdc4 * 1200);
    field_0xd66 = field_0xd60 * cM_ssin(field_0xdc4 * 800);
    shape_angle.x = field_0xd64;
    shape_angle.z = field_0xd66;
    return 1;
}

void daObjCarry_c::mode_init_sink() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

    field_0xda8 = 0;
    mStts.SetWeight(data().m_colliderWeight);

    if (mMode == MODE_CARRY && 0.0f != speedF) {
        speedF = cLib_minMaxLimit<f32>(speedF, -5.0f - KREG_F(2), 5.0f + KREG_F(2));
        speed.y = cLib_minMaxLimit<f32>(speed.y, -15.0f - KREG_F(3), 5.0f + KREG_F(4));
    } else {
        speedF = cLib_minMaxLimit<f32>(speedF, -5.0f - KREG_F(2), 5.0f + KREG_F(2));
        speed.y = cLib_minMaxLimit<f32>(speed.y, -5.0f - KREG_F(3), 13.0f + KREG_F(4));
    }

    gravity = -0.25f + KREG_F(5);
    if (prm_chk_type_ironball()) {
        mAcch.ClrIronBall();
    }

    mMode = MODE_SINK;
}

int daObjCarry_c::mode_proc_sink() {
    cXyz sp24;
    f32 speed_target;
    int sp8 = 0;
    if (fopAcM_getWaterStream(&current.pos, mAcch.m_gnd, &sp24, &sp8, 0)) {
        s16 var_r27 = cM_atan2s(sp24.x, sp24.z);

        cXyz sp18(speedF * cM_ssin(current.angle.y), 0.0f, speedF * cM_scos(current.angle.y));
        sp24.normalizeZP();
        sp18 += sp24;

        current.angle.y = cM_atan2s(sp18.x, sp18.z);
        speed_target = 0.8f * sp8;
        if (speed_target < 2.0f) {
            speed_target = 2.0f;
        }
    } else {
        speed_target = 0.0f;
    }

    cLib_addCalc(&speedF, speed_target, 0.05f, 0.1f, 0.01f);
    speed.y = cLib_minMaxLimit<f32>(speed.y, -15.0f - KREG_F(3), 13.0f + KREG_F(4));

    if (mAcch.ChkGroundHit()) {
        speedF *= 0.9f;
    }

    if (chkWaterLineIn()) {
        gravity = -0.25f + KREG_F(5);
    } else {
        gravity = data().m_gravity;
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());

    if (mAcch.ChkGroundHit()) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }

    field_0xcec = current.pos.y;
    return 1;
}

void daObjCarry_c::mode_init_yogan() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

    mStts.SetWeight(data().m_colliderWeight);

    fopAcM_SetGravity(this, -2.0f);
    fopAcM_SetMaxFallSpeed(this, -6.0f);

    if (prm_chk_type_ironball()) {
        mAcch.ClrIronBall();
    }

    mMode = MODE_YOGAN;
}

int daObjCarry_c::mode_proc_yogan() {
    bool temp_r30 = mAcch.m_wtr.GetHeight() > 200.0f + current.pos.y;
    bool gnd_landing = mAcch.ChkGroundLanding();

    speedF *= 0.65f;
    fopAcM_posMoveF(this, NULL);

    if (temp_r30 || gnd_landing) {
        fopAcM_delete(this);
    }

    return 1;
}

int daObjCarry_c::mode_proc_magne() {
    return 1;
}

int daObjCarry_c::mode_proc_magneCarry() {
    return 1;
}

void daObjCarry_c::mode_init_boomCarry() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OnCoSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCanCrashRoll = false;

    gravity = calc_gravity();
    fopAcM_SetMaxFallSpeed(this, -100.0f);

    mStts.SetWeight(data().m_colliderWeight);
    mCyl.GetTgHitGObj();
    mBoomerangMove.initOffset(&current.pos);

    if (field_0xdb0 == 1) {
        field_0xdb0 = 0;
        fopAcM_OffCarryType(this, fopAcM_CARRY_TYPE_1);
        field_0xd7a = 1;
    }

    if (prm_chk_type_ironball()) {
        mAcch.ClrIronBall();
    }

    mMode = MODE_BOOM_CARRY;
}

int daObjCarry_c::mode_proc_boomCarry() {
    if (!mBoomerangMove.posMove(&current.pos, &shape_angle.y, this, 0x1C00)) {
        if (fopAcM_checkCarryNow(this)) {
            mode_init_carry();
        } else {
            mode_init_wait();
        }
    }

    return 1;
}

void daObjCarry_c::mode_init_growth() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OnCoSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    field_0xdb0 = 1;

    scale.setall(0.0f);
    field_0xd79 = 2;
    mStts.SetWeight(data().m_colliderWeight);
    mMode = MODE_GROWTH;
}

int daObjCarry_c::mode_proc_growth() {
    f32 sp8 = scale.x;
    f32 temp_f31 = cLib_addCalc(&sp8, data().scale, 0.05f, 0.1f, 0.01f);
    scale.setall(sp8);

    if (0.0f == temp_f31) {
        field_0xcf3 = 10;
        field_0xdb1 = 1;
        mode_init_wait();
    }

    return 1;
}

int daObjCarry_c::mode_proc_end() {
    return 1;
}

void daObjCarry_c::mode_init_dbDrop(u8 param_0) {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);
    mCyl.OffTgSPrmBit(1);

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

    fopAcM_SetMaxFallSpeed(this, -100.0f);
    mStts.SetWeight(data().m_colliderWeight);
    gravity = calc_gravity();

    field_0xd75 = 1;
    field_0xd79 = 0;
    mMode = MODE_DB_DROP;
}

int daObjCarry_c::mode_proc_dbDrop() {
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    return 1;
}

void daObjCarry_c::mode_init_hookCarry() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OnCoSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCanCrashRoll = false;

    gravity = calc_gravity();
    fopAcM_SetMaxFallSpeed(this, -100.0f);
    mStts.SetWeight(data().m_colliderWeight);

    mCyl.GetTgHitGObj();
    mBoomerangMove.initOffset(&current.pos);

    if (prm_chk_type_ironball()) {
        mAcch.ClrIronBall();
    } else if (prm_chk_type_lightball() || mType == TYPE_LV8_BALL) {
        mSound.startCollisionSE(Z2SE_HIT_HOOKSHOT_STICK, 0, NULL);
    }

    mMode = MODE_HOOK_CARRY;
}

int daObjCarry_c::mode_proc_hookCarry() {
    if (!fopAcM_checkHookCarryNow(this)) {
        mode_init_wait();
    }

    return 1;
}

void daObjCarry_c::mode_init_fit() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OnTgSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    fopAcM_OffStatus(this, 0x80000);
    field_0xda8 = 0;

    speedF = 0.0f;
    gravity = data().m_gravity;
    fopAcM_SetMaxFallSpeed(this, -100.0f);
    mStts.SetWeight(0xFF);

    if (prm_chk_type_ironball()) {
        mAcch.ClrIronBall();
    }

    mMode = MODE_FIT;
}

int daObjCarry_c::mode_proc_fit() {
    fopAcM_posMoveF(this, NULL);
    return 1;
}

int daObjCarry_c::mode_proc_controled() {
    calc_rot_call();
    setBaseMtx();
    field_0xdb9 = mCtrl;
    field_0xdec = current.pos;
    return 1;
}

void daObjCarry_c::mode_init_resetLightBall() {
    mAcch.ClrMoveBGOnly();
    mAcch.ClrGrndNone();

    mCyl.OffAtSPrmBit(1);
    mCyl.OffTgSPrmBit(1);
    mCyl.OnCoSPrmBit(0x10);

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    field_0xda8 = 0;

    speedF = 0.0f;
    speed.setall(0.0f);

    mStts.ClrCcMove();

    gravity = data().m_gravity;
    fopAcM_SetMaxFallSpeed(this, -100.0f);
    mStts.SetWeight(data().m_colliderWeight);

    mMode = MODE_RESET_LIGHTBALL;
}

int daObjCarry_c::mode_proc_resetLightBall() {
    if (cLib_chaseF(&scale.x, 1.0f, 0.05f)) {
        field_0xcf3 = 10;
        field_0xdb1 = 1;
        mode_init_wait();
    }

    scale.y = scale.x;
    scale.z = scale.x;
    fopAcM_posMoveF(this, NULL);
    return 1;
}

BOOL daObjCarry_c::chkSinkAll() {
    return mAcch.m_wtr.GetHeight() > current.pos.y + data().field_0x2c * 2.0f;
}

BOOL daObjCarry_c::chkWaterLineIn() {
    return mAcch.m_wtr.GetHeight() > current.pos.y + data().field_0x24;
}

BOOL daObjCarry_c::chkSinkObj() {
    return data().m_gravity + data().m_Buoyancy <= 0.0f;
}

void daObjCarry_c::bg_check() {
    bool roof_hit = mAcch.ChkRoofHit();
    bool wall_hit = mAcch.ChkWallHit();
    bool gnd_landing = mAcch.ChkGroundLanding();
    bool gnd_hit = mAcch.ChkGroundHit();
    bool water_hit = mAcch.ChkWaterHit();
    bool water_in = mAcch.ChkWaterIn();

    if (wall_hit && field_0xe0d == 0) {
        cM3dGPla spA0;
        bool sp10 = dComIfG_Bgsp().GetTriPla(mAcchCir, &spA0);

        f32 var_f27 = 0.0f;
        if (sp10 != 0) {
            var_f27 = speed.inprod(spA0.mNormal);
        }

        if (var_f27 < 0.0f) {
            if (speed.abs() > 8.0f) {
                f32 sp30 = field_0xd1c.y;
                cXyz sp94(speed);
                cXyz sp88(speed);

                f32 var_f31 = data().field_0x14;
                if (mCannon) {
                    var_f31 = 0.05f;
                }

                switch (dComIfG_Bgsp().GetPolyAtt0(mAcchCir)) {
                case 13:
                    if (mType == TYPE_IRON_BALL && !mCannon) {
                        var_f31 = 0.2f;
                    }
                    /* fallthrough */
                case 8:
                    if (mType == TYPE_IRON_BALL) {
                        if (!mCannon) {
                            var_f31 = 0.2f;
                        }

                        cXyz size(1.0f + KREG_F(10), 1.0f + KREG_F(10), 1.0f + KREG_F(10));
                        dComIfGp_particle_set(0xAE3, &current.pos, &current.angle, &size);
                        mDoAud_seStart(Z2SE_OBJ_IRONBALL_HIT_ICE, &current.pos, (u32)std::fabs(speedF), 0);
                        // "Iron Ball: Ice Wall Hit SE<%d>\n"
                        OS_REPORT("鉄球：氷壁ヒットＳＥ<%d>\n", (u32)std::fabs(speedF));
                    }
                    break;
                default:
                    if (mType == TYPE_IRON_BALL) {
                        mDoAud_seStart(Z2SE_OBJ_IRONBALL_HIT, &current.pos, std::fabs(speedF), 0);
                        // "Iron Ball: Wall Hit SE<%d>\n"
                        OS_REPORT("鉄球：壁ヒットＳＥ<%d>\n", (u32)std::fabs(speedF));
                    }
                    break;
                }

                bound(&sp94, mAcchCir, var_f31);
                speed.set(sp94);
                speed.y = sp88.y;
                speedF = sp94.absXZ();
                current.angle.y = cM_atan2s(sp94.x, sp94.z);
            } else {
                speedF = 0.0f;
            }
        }
    }

    if (gnd_landing) {
        f32 temp_f30 = field_0xd1c.y;

        if (speed.abs() > 1.0f) {
            f32 var_f29 = data().field_0x14;
            f32 var_f28 = data().field_0x10;

            switch (dComIfG_Bgsp().GetPolyAtt0(mAcch.m_gnd)) {
            case 3:
                var_f29 = 0.1f;
                var_f28 = 0.1f;
                break;
            case 13:
                if (mType == TYPE_IRON_BALL) {
                    var_f29 = 0.2f;
                    var_f28 = 0.2f;

                    if (std::fabs(field_0xd1c.y) > 10.0f) {
                        cXyz sp70(0.9f + KREG_F(10), 0.9f + KREG_F(10), 0.9f + KREG_F(10));
                        dComIfGp_particle_setPolyColor(0x881F, mAcch.m_gnd, &current.pos, &tevStr, &current.angle, &sp70, 0, NULL, -1, NULL);
                        dComIfGp_particle_setPolyColor(0x8820, mAcch.m_gnd, &current.pos, &tevStr, &current.angle, &sp70, 0, NULL, -1, NULL);
                    }
                }
                break;
            default:
                if (mType == TYPE_IRON_BALL && std::fabs(field_0xd1c.y) > 10.0f) {
                    cXyz sp64(0.9f + KREG_F(10), 0.9f + KREG_F(10), 0.9f + KREG_F(10));
                    dComIfGp_particle_setPolyColor(0xE7, mAcch.m_gnd, &current.pos, &tevStr, &current.angle, &sp64, 0, NULL, -1, NULL);
                }
                break;
            }

            bound(&speed, mAcch.m_gnd, var_f29);
            speedF = std::sqrt((speed.x * speed.x) + (speed.z * speed.z));

            if (chkWaterLineIn()) {
                speed.y = 0.0f;
            } else {
                speed.y = -temp_f30 * var_f28;
            }

            if (std::fabs(speed.y) < 5.0f) {
                speed.y = 0.0f;
            }
        }

        if (mType == TYPE_IRON_BALL) {
            switch (dComIfG_Bgsp().GetPolyAtt0(mAcch.m_gnd)) {
            case 13: {
                u32 sp2C = std::sqrt((speedF * speedF) + (speed.y * speed.y));
                mDoAud_seStart(Z2SE_OBJ_IRONBALL_HIT_SNOW, &current.pos, std::fabs(temp_f30), 0);
                break;
            }
            case 8: {
                u32 sp2C = std::sqrt((speedF * speedF) + (speed.y * speed.y));
                mDoAud_seStart(Z2SE_OBJ_IRONBALL_HIT_ICE, &current.pos, std::fabs(temp_f30), 0);
                break;
            }
            default:
                if (!fopAcM_checkCarryNow(this)) {
                    mDoAud_seStart(Z2SE_OBJ_IRONBALL_HIT, &current.pos, std::fabs(temp_f30), 0);
                }
                break;
            }
        }
    }

    if (mMode == MODE_WAIT || mMode == MODE_WALK || mMode == MODE_DROP || mMode == MODE_DB_DROP || mMode == MODE_CARRY) {
        f32 water_height = mAcch.m_wtr.GetHeight();

        if (water_hit) {
            f32 sp28 = speed.y;

            int poly_att = dComIfG_Bgsp().GetPolyAtt0(mAcch.m_wtr);
            switch (poly_att) {
            default:
            case 7:
            case 11:
                if (chkWaterLineIn() && mMode != MODE_CARRY) {
                    if (chkSinkObj()) {
                        mode_init_sink();
                    } else {
                        mode_init_float();
                    }
                }

                if (water_in && field_0xdad == 0) {
                    cXyz pos(current.pos);
                    pos.y = water_height;

                    if (sp28 < -15.0f) {
                        fopKyM_createWpillar(&pos, data().field_0x88, 0);
                        se_fall_water();
                    } else {
                        daPy_py_c* player = daPy_getPlayerActorClass();
                        if (mMode != MODE_CARRY || (mMode == MODE_CARRY && player->speedF < 10.0f)) {
                            se_put_water();
                        }
                    }
                }
                break;
            case 6: 
                if (chkWaterLineIn()) {
                    mode_init_yogan();
                }

                if (water_in && field_0xdad == 0) {
                    cXyz pos(current.pos.x, water_height, current.pos.z);
                    cXyz size(data().field_0x8c, data().field_0x8c, data().field_0x8c);
                    dComIfGp_particle_set(0x8757, &pos, &tevStr, NULL, &size);
                    dComIfGp_particle_set(0x8758, &pos, &tevStr, NULL, &size);
                }
                break;
            }
        }

        if (gnd_landing && !chkWaterLineIn() && (mMode == MODE_DROP || mMode == MODE_DB_DROP)) {
            if (mType == TYPE_BOKKURI) {
                field_0xd5c = 4000.0f + cM_rndF(1000.0f);
                field_0xd60 = 3000.0f + cM_rndF(900.0f);
                field_0xd68 = 3000;
            }
            mode_init_walk(0);
        }
    } else if ((mMode == MODE_FLOAT || mMode == MODE_SINK) && ((gnd_hit && !chkWaterLineIn()) || !water_in)) {
        f32 sp1C = speedF;
        f32 sp18 = speed.y;
        mode_init_walk(0);

        if (water_in == 0) {
            speedF = sp1C;
            speed.y = sp18;
        }
    }

    if (gnd_hit) {
        if (field_0xcf3 != 0) {
            field_0xcf3--;
        } else if ((mMode == MODE_WALK || mMode == MODE_DROP || mMode == MODE_DB_DROP || mMode == MODE_WAIT || mMode == MODE_SINK) && field_0xdae == 0 && field_0xdb1 == 0 && field_0xcf3 == 0) {
            if (mType != TYPE_IRON_BALL) {
                se_put(NULL);
            }
            field_0xdb1 = 1;
        }
    } else if (field_0xdb0 == 0) {
        field_0xdb1 = 0;
    }

    cXyz sp34(current.pos);
    sp34.y = mAcch.m_wtr.GetHeight();

    if (water_hit && sp34.y > current.pos.y && (mMode == MODE_FLOAT || (mMode == MODE_SINK && !chkSinkAll()) || (mMode == MODE_WAIT && gnd_hit) || (mMode == MODE_WALK && gnd_hit) || (mMode == MODE_CARRY && water_in && !chkSinkAll()))) {
        fopAcM_effHamonSet(&field_0xdd4, &sp34, 1.0f, 0.1f);
    }

    if (mType == TYPE_IRON_BALL && roof_hit && gnd_hit) {
        // "Lift: pinched!\n"
        OS_REPORT("持ち上げ：挟まれました！\n");
        current.pos.y -= 50.0f;
    }

    field_0xe0d = wall_hit;
}

bool daObjCarry_c::check_bg_damage_proc_base() {
    bool gnd_landing = mAcch.ChkGroundLanding();
    bool wall_hit = mAcch.ChkWallHit();
    bool var_r30 = false;

    if (gnd_landing) {
        var_r30 = dComIfG_Bgsp().GetPolyAtt0(mAcch.m_gnd) != 3;
    }

    if (mMode == MODE_DROP) {
        if (wall_hit || var_r30) {
            return 1;
        }

        if (mType == TYPE_TARU) {
            if (strcmp(dComIfGp_getStartStageName(), "R_SP160") == 0 && fopAcM_GetRoomNo(this) == 4 && fopAcM_lc_c::lineCheck(&old.pos, &current.pos, this)) {
                return 1;
            }
        }
    }

    if (var_r30 != 0) {
        if (std::fabs(current.pos.y - field_0xcec) > data().field_0x54) {
            return 1;
        }
    }

    if (mMode == MODE_BOOM_CARRY && wall_hit) {
        return 1;
    }

    return 0;
}

bool daObjCarry_c::bg_damage_proc_kotubo() {
    bool rt = check_bg_damage_proc_base();
    if (rt) {
        obj_break(true, true, true);
        fopAcM_delete(this);
    }

    return rt;
}

bool daObjCarry_c::bg_damage_proc_ootubo() {
    return bg_damage_proc_kotubo();
}

bool daObjCarry_c::bg_damage_proc_kibako() {
    return bg_damage_proc_kotubo();
}

bool daObjCarry_c::bg_damage_proc_ironball() {
    return false;
}

bool daObjCarry_c::bg_damage_proc_taru() {
    return bg_damage_proc_kotubo();
}

bool daObjCarry_c::bg_damage_proc_dokuro() {
    return bg_damage_proc_kotubo();
}

bool daObjCarry_c::bg_damage_proc_bokkuri() {
    bool rt = check_bg_damage_proc_base();
    if (field_0xd74 != 0) {
        rt = false;
    }

    if (mAcch.ChkWaterHit() && mAcch.ChkWaterIn()) {
        fopAcM_seStart(this, Z2SE_OBJ_BOKKURI_FALLWATER, 0);
        field_0xdb3 = 1;
        rt = true;
    }

    if (rt) {
        obj_break(true, true, true);
        fopAcM_delete(this);
    }

    return rt;
}

bool daObjCarry_c::bg_damage_proc_LightBall() {
    return false;
}

bool daObjCarry_c::bg_damage_proc_Lv8Ball() {
    return false;
}

void daObjCarry_c::obj_break(bool i_createItem, bool i_cancelCarry, bool i_doBreakEff) {
    int item_no = getItemNo();
    int item_bit = getItemBit();

    if (i_createItem) {
        if (getItemType() == 0) {
            csXyz angle(0, home.angle.y, 0);
            fopAcM_createItem(&current.pos, item_no, item_bit, fopAcM_GetHomeRoomNo(this), &angle, NULL, 0);
        } else {
            csXyz angle(0, home.angle.y, 0);
            fopAcM_createItemFromTable(&current.pos, item_no, item_bit, fopAcM_GetHomeRoomNo(this), &angle, 0, NULL, NULL, NULL, false);
        }
    }

    if (i_cancelCarry) {
        fopAcM_cancelCarryNow(this);
        fopAcM_cancelHookCarryNow(this);
    }

    if (i_doBreakEff) {
        eff_break_call();
        se_break(NULL);
    }
}

bool daObjCarry_c::check_cc_damage_proc_base(bool param_0) {
    bool rt = false;

    if (mCyl.ChkAtHit()) {
        mCyl.ClrAtHit();
        rt = true;
    } else if (mCyl.ChkTgHit()) {
        cCcD_Obj* tg_hit_obj = mCyl.GetTgHitObj();
        if (tg_hit_obj != NULL && !tg_hit_obj->ChkAtType(AT_TYPE_BOOMERANG)) {
            mCyl.ClrTgHit();
            rt = true;
        }
    }

    return rt;
}

bool daObjCarry_c::cc_damage_proc_kotubo() {
    bool rt = check_cc_damage_proc_base(false);
    if (rt) {
        obj_break(true, true, true);
        fopAcM_delete(this);
    }

    return rt;
}

bool daObjCarry_c::cc_damage_proc_ootubo() {
    return cc_damage_proc_kotubo();
}

bool daObjCarry_c::cc_damage_proc_kibako() {
    bool break_obj = false;
    bool do_break_eff = false;
    bool delete_obj = false;

    if (mCyl.ChkAtHit()) {
        break_obj = true;
        do_break_eff = true;
        delete_obj = true;
    } else if (mCyl.ChkTgHit()) {
        dCcD_GObjInf* tg_hit_gobj = mCyl.GetTgHitGObj();
        if (tg_hit_gobj != NULL) {
            if (!tg_hit_gobj->ChkAtType(AT_TYPE_LANTERN_SWING) && tg_hit_gobj->GetAtMtrl() == dCcD_MTRL_FIRE) {
                if (mAcch.ChkGroundHit()) {
                    u32 params;
                    daObjBurnBox_c::make_prm_burnBox(&params, 0);
                    fopAcM_createChild(PROC_Obj_BurnBox, fopAcM_GetID(this), params, &current.pos, fopAcM_GetRoomNo(this), &shape_angle, &scale, -1, NULL);

                    break_obj = true;
                    do_break_eff = false;
                    delete_obj = false;
                    mDeleteTimer = 5;
                } else {
                    break_obj = true;
                    do_break_eff = true;
                    delete_obj = true;
                }
            } else if (!tg_hit_gobj->ChkAtType(AT_TYPE_ARROW) && !tg_hit_gobj->ChkAtType(AT_TYPE_SHIELD_ATTACK) && !tg_hit_gobj->ChkAtType(AT_TYPE_BOOMERANG)) {
                break_obj = true;
                do_break_eff = true;
                delete_obj = true;
            }
        }
    }

    if (break_obj) {
        obj_break(true, true, do_break_eff);
    }

    if (delete_obj) {
        fopAcM_delete(this);
    }

    return break_obj;
}

bool daObjCarry_c::cc_damage_proc_ironball() {
    bool var_r26 = false;
    bool var_r27 = true;

    if (mAcch.ChkGroundHit() && dComIfG_Bgsp().ChkPolySafe(mAcch.m_gnd) && dComIfG_Bgsp().ChkMoveBG_NoDABg(mAcch.m_gnd) && dComIfG_Bgsp().GetActorPointer(mAcch.m_gnd)) {
        s16 name = fopAcM_GetName(dComIfG_Bgsp().GetActorPointer(mAcch.m_gnd));
        if (name == PROC_Obj_YIblltray) {
            var_r27 = false;
            mStts.ClrCcMove();
        }
    }

    if (mCyl.ChkTgHit() && var_r27) {
        dCcD_GObjInf* tg_hit_gobj = mCyl.GetTgHitGObj();
        if (tg_hit_gobj != NULL && tg_hit_gobj->ChkAtType(AT_TYPE_IRON_BALL) && tg_hit_gobj->GetAtAtp() != 1 && tg_hit_gobj->GetAtAtp() != 100) {
            cXyz* at_vec_p = tg_hit_gobj->GetAtVecP();
            if (at_vec_p != NULL) {
                speedF = 100.0f + KREG_F(4);
                current.angle.y = cM_atan2s(at_vec_p->x, at_vec_p->z);
            }
        }
    }

    return var_r26;
}

bool daObjCarry_c::cc_damage_proc_taru() {
    bool break_obj = false;
    bool do_break_eff = false;
    bool delete_obj = false;

    if (mCyl.ChkAtHit()) {
        mCyl.ClrAtHit();
        break_obj = true;
        do_break_eff = true;
        delete_obj = true;
    } else if (mCyl.ChkTgHit()) {
        dCcD_GObjInf* tg_hit_gobj = mCyl.GetTgHitGObj();
        if (tg_hit_gobj != NULL) {
            if (!tg_hit_gobj->ChkAtType(AT_TYPE_LANTERN_SWING) && tg_hit_gobj->GetAtMtrl() == dCcD_MTRL_FIRE) {
                if (mAcch.ChkGroundHit()) {
                    u32 params;
                    daObjBurnBox_c::make_prm_burnBox(&params, 2);
                    fopAcM_createChild(PROC_Obj_BurnBox, fopAcM_GetID(this), params, &current.pos, fopAcM_GetRoomNo(this), &shape_angle, &scale, -1, NULL);

                    break_obj = true;
                    do_break_eff = false;
                    delete_obj = false;
                    mDeleteTimer = 5;
                } else {
                    break_obj = true;
                    do_break_eff = true;
                    delete_obj = true;
                }
            } else if (!tg_hit_gobj->ChkAtType(AT_TYPE_ARROW) && !tg_hit_gobj->ChkAtType(AT_TYPE_SHIELD_ATTACK) && !tg_hit_gobj->ChkAtType(AT_TYPE_BOOMERANG)) {
                mCyl.ClrTgHit();
                break_obj = true;
                do_break_eff = true;
                delete_obj = true;
            }
        }
    }

    if (break_obj) {
        obj_break(true, true, do_break_eff);
    }

    if (delete_obj) {
        fopAcM_delete(this);
    }

    return break_obj;
}

bool daObjCarry_c::cc_damage_proc_dokuro() {
    return cc_damage_proc_kotubo();
}

bool daObjCarry_c::cc_damage_proc_bokkuri() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    bool break_obj = false;
    bool var_r26 = true;

    if (mCyl.ChkAtHit()) {
        break_obj = true;
        var_r26 = false;
    } else if (mCyl.ChkTgHit()) {
        cCcD_Obj* tg_hit_obj = mCyl.GetTgHitObj();
        if (tg_hit_obj != NULL) {
            if (!tg_hit_obj->ChkAtType(AT_TYPE_BOOMERANG) && !tg_hit_obj->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                break_obj = true;
            }

            if (tg_hit_obj->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
                if (player->speedF > 10.0f) {
                    var_r26 = false;
                }

                if (field_0xd75 != 0) {
                    fopAcM_seStart(this, Z2SE_OBJ_BOKKURI_HIT_ROLL, 0);
                }
            }

            if (tg_hit_obj->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                if (field_0xdb0 != 0) {
                    field_0xd7a = 0x10;
                    fopAcM_OffCarryType(this, fopAcM_CARRY_TYPE_1);
                }

                field_0xdb0 = 0;

                if (mMode == MODE_WAIT || mMode == MODE_WALK) {
                    cXyz sp8 = current.pos - player->current.pos;
                    speedF = 25.0f;
                    current.angle.y = cM_atan2s(sp8.x, sp8.z);
                    mode_init_walk(0);
                }

                fopAcM_seStart(this, Z2SE_OBJ_BOKKURI_HIT_ROLL, 0);
            }
        }
    }

    if (field_0xd75 != 0) {
        break_obj = false;
    }

    if (break_obj) {
        obj_break(true, true, true);
        fopAcM_delete(this);
    }

    return break_obj;
}

bool daObjCarry_c::cc_damage_proc_LightBall() {
    return false;
}

bool daObjCarry_c::cc_damage_proc_Lv8Ball() {
    return false;
}

void daObjCarry_c::eff_break_tuboBmd(u16 param_0, cXyz param_1) {
    J3DModelData* tubo_bmd = (J3DModelData*)dComIfG_getObjectRes("Always", 0x20);
    J3DAnmTexPattern* tubo_btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("Always", 0x42);
    
    JUT_ASSERT(6963, tubo_bmd != NULL);
    JUT_ASSERT(6964, tubo_btp != NULL);
    
    JPABaseEmitter* emitter = dComIfGp_particle_set(0x15C, &current.pos, NULL, NULL, 0xFF, &dPa_modelEcallBack::getEcallback(), fopAcM_GetRoomNo(this), NULL, NULL, &param_1);

    dPa_modelEcallBack::setModel(emitter, tubo_bmd, tevStr, 3, tubo_btp, param_0);

    dComIfGp_particle_set(0x15D, &current.pos, NULL, NULL, 0xFF, dPa_control_c::getTsuboSelectTexEcallBack(param_0), fopAcM_GetRoomNo(this), NULL, NULL, &param_1);
}

void daObjCarry_c::eff_break_kibakoBmd(cXyz i_size) {
    cXyz pos(current.pos);

    J3DModelData* kibako_bmd = (J3DModelData*)dComIfG_getObjectRes("Always", "BreakWoodBox.bmd");
    JUT_ASSERT(7005, kibako_bmd != NULL);
    
    JPABaseEmitter* emitter = dComIfGp_particle_set(0x82AF, &pos, NULL, NULL, 0xFF, &dPa_modelEcallBack::getEcallback(), fopAcM_GetRoomNo(this), NULL, NULL, &i_size);

    dPa_modelEcallBack::setModel(emitter, kibako_bmd, tevStr, 3, NULL, 0, 0);

    for (int i = 0; i < 3; i++) {
        static u16 const particle_id[] = {0x82AB, 0x82AC, 0x82AD};
        dComIfGp_particle_set(particle_id[i], &pos, NULL, &i_size, 0xFF, NULL, -1, NULL, NULL, NULL);
    }
}

void daObjCarry_c::eff_break_tsubo() {
    u16 var_r31 = 0;

    cXyz spC(cXyz(1.0f, 1.0f, 1.0f));
    eff_break_tuboBmd(var_r31, spC);
}

void daObjCarry_c::eff_break_ootubo() {
    u16 var_r31 = 1;

    cXyz spC(cXyz(1.5f, 1.5f, 1.5f));
    eff_break_tuboBmd(var_r31, spC);
}

void daObjCarry_c::eff_break_kibako() {
    cXyz size(scale);
    eff_break_kibakoBmd(size);
}

void daObjCarry_c::eff_break_ironball() {}

void daObjCarry_c::eff_break_taru() {
    cXyz size(0.75f, 0.75f, 0.75f);
    eff_break_kibakoBmd(size);
}

void daObjCarry_c::eff_break_dokuro() {
    u16 var_r31 = 2;

    cXyz spC(cXyz(1.0f, 1.0f, 1.0f));
    eff_break_tuboBmd(var_r31, spC);
}

void daObjCarry_c::eff_break_bokkuri() {
    cXyz pos(current.pos);
    pos.y += 50.0f;

    J3DModelData* tubo_bmd = (J3DModelData*)dComIfG_getObjectRes("Always", 0x20);
    J3DAnmTexPattern* tubo_btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("Always", 0x42);
    
    JUT_ASSERT(7108, tubo_bmd != NULL);
    JUT_ASSERT(7109, tubo_btp != NULL);
    
    JPABaseEmitter* emitter = dComIfGp_particle_set(0x15C, &current.pos, NULL, NULL, 0xFF, &dPa_modelEcallBack::getEcallback(), fopAcM_GetRoomNo(this), NULL, NULL, &scale);

    dPa_modelEcallBack::setModel(emitter, tubo_bmd, tevStr, 3, tubo_btp, 5);

    for (int i = 0; i < 3; i++) {
        static u16 const particle_id[] = {0x8268, 0x8269, 0x826A};
        dComIfGp_particle_set(particle_id[i], &pos, NULL, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
    }
}

void daObjCarry_c::eff_break_kotubo2() {
    u16 var_r31 = 1;

    cXyz spC(cXyz(1.0f, 1.0f, 1.0f));
    eff_break_tuboBmd(var_r31, spC);
}

void daObjCarry_c::eff_break_LightBall() {}

void daObjCarry_c::eff_break_ootubo2() {
    u16 var_r31 = 0;

    cXyz spC(cXyz(1.5f, 1.5f, 1.5f));
    eff_break_tuboBmd(var_r31, spC);
}

void daObjCarry_c::eff_break_Lv8Ball() {}

void daObjCarry_c::eff_break_tsuboL8() {
    u16 var_r31 = 7;

    cXyz spC(cXyz(1.0f, 1.0f, 1.0f));
    eff_break_tuboBmd(var_r31, spC);
}

void daObjCarry_c::eff_break_ootuboL8() {
    u16 var_r31 = 7;

    cXyz spC(cXyz(1.5f, 1.5f, 1.5f));
    eff_break_tuboBmd(var_r31, spC);
}

void daObjCarry_c::se_break(cBgS_PolyInfo*) {
    if (field_0xdb3 == 0) {
        fopAcM_seStart(this, data().m_breakSound, 0);
    }
}

void daObjCarry_c::se_put(cBgS_PolyInfo*) {
    fopAcM_seStart(this, data().m_putSound, 0);
}

void daObjCarry_c::se_put_water() {
    fopAcM_seStart(this, data().m_putWaterSound, 0);
}

void daObjCarry_c::se_fall_water() {
    fopAcM_seStart(this, data().m_fallWaterSound, 0);
}

void daObjCarry_c::calc_rot_axis_base(u8 param_0) {
    field_0xd4c = ZeroQuat;

    if (fopAcM_checkCarryNow(this)) {
        if (field_0xd7b < 0) {
            mDoMtx_quatSlerp(&field_0xd3c, &ZeroQuat, &field_0xd3c, 0.25f * daPy_getLinkPlayerActorClass()->getBaseAnimeFrameRate());
        } else {
            field_0xd7b--;
        }
        return;
    }

    cXyz sp24 = current.pos - field_0xdec;
    cXyz sp18(sp24.x, 0.0f, sp24.z);

    bool temp_r29 = sp24.normalizeRS();
    bool temp_r28 = sp18.normalizeRS();

    bool var_r26 = 1;
    if (temp_r29 == 0 && temp_r28 == 0) {
        var_r26 = 0;
    }

    if (temp_r28) {
        mRotAxis = sp18;
        mDoMtx_stack_c::YrotS(0x4000);
        mDoMtx_stack_c::multVec(&mRotAxis, &mRotAxis);
    } else if (temp_r29) {
        mRotAxis = cXyz::BaseX;
    }

    if (mCtrl == 0) {
        if (mAcch.ChkGroundHit()) {
            mRotation = data().field_0x7c * (std::fabs(speedF) + std::fabs(speed.y));
        } else {
            mRotation = data().m_urnRotateFactor * (std::fabs(speedF) + std::fabs(speed.y));
        }

        field_0xd4c.x = mRotAxis.x * cM_ssin(mRotation);
        field_0xd4c.y = mRotAxis.y * cM_ssin(mRotation);
        field_0xd4c.z = mRotAxis.z * cM_ssin(mRotation);
        field_0xd4c.w = cM_scos(mRotation);
    } else {
        sp24 = current.pos - field_0xdec;
        current.angle.y = cM_atan2s(sp24.x, sp24.z);

        if (var_r26) {
            mRotation = std::fabs(sp24.abs()) * data().m_urnRotateFactor;

            field_0xd4c.x = mRotAxis.x * cM_ssin(mRotation);
            field_0xd4c.y = mRotAxis.y * cM_ssin(mRotation);
            field_0xd4c.z = mRotAxis.z * cM_ssin(mRotation);
            field_0xd4c.w = cM_scos(mRotation);
        }
    }

    field_0xd7b = param_0;
}

void daObjCarry_c::calc_rot_axis_tsubo() {
    cXyz sp20;
    int var_r30 = 0;

    // NOTE: only case 0 ever reached since var_r30 is never modified
    switch (var_r30) {
    case 0:
        calc_rot_axis_base(5);
        break;
    case 1:
        if (mMode == MODE_DROP || mMode == MODE_SINK || mMode == MODE_YOGAN) {
            calc_rot_axis_base(0);
        } else {
            mRotAxis = cXyz::Zero;
            mRotation = 0;
        }
        break;
    case 2:
        sp20 = current.pos - old.pos;
        if (!sp20.isZero()) {
            mRotAxis = cXyz::BaseX;
            mDoMtx_stack_c::YrotS(current.angle.y);
            mDoMtx_stack_c::multVec(&mRotAxis, &mRotAxis);
        }

        mRotation += (s16)(((0.5f * field_0xd70) + data().m_urnRotateFactor) * (std::fabs(speedF) + std::fabs(speed.y)));
        break;
    case 3:
        sp20 = current.pos - old.pos;

        if (!sp20.isZero()) {
            if (!mCyl.ChkCoHit()) {
                mRotAxis = sp20;
                mDoMtx_stack_c::YrotS(0x4000);
                mDoMtx_stack_c::multVec(&mRotAxis, &mRotAxis);
            }
        }

        mRotation += (s16)(((0.5f * field_0xd70) + data().m_urnRotateFactor) * (std::fabs(speedF) + std::fabs(speed.y)));
        break;
    }
}

void daObjCarry_c::calc_rot_axis_ootubo() {
    calc_rot_axis_base(10);
}

void daObjCarry_c::calc_rot_axis_kibako() {
    if (fopAcM_checkCarryNow(this)) {
        mRotAxis = cXyz::Zero;
        mRotation = 0;
    } else {
        mRotAxis = cXyz::BaseX;
        mDoMtx_stack_c::YrotS(current.angle.y);
        mDoMtx_stack_c::multVec(&mRotAxis, &mRotAxis);
        cXyz sp8 = current.pos - old.pos;

        if (mMode == MODE_DROP) {
            mRotation += (s16)(speedF * ((field_0xd70 * 0.5f) + data().m_urnRotateFactor));
        } else {
            mRotation = 0;
        }
    }
}

void daObjCarry_c::calc_rot_axis_ironball() {
    calc_rot_axis_base(10);
}

void daObjCarry_c::calc_rot_axis_taru() {
    calc_rot_axis_kibako();
}

void daObjCarry_c::calc_rot_axis_dokuro() {
    calc_rot_axis_base(5);
}

void daObjCarry_c::calc_rot_axis_bokkuri() {
    if (mMode == MODE_DROP || mMode == MODE_DB_DROP) {
        if (fopAcM_checkCarryNow(this)) {
            if (cLib_chaseS(&mRotation, 0, 0x2000)) {
                mRotAxis = cXyz::Zero;
            }
        } else {
            cXyz sp8 = current.pos - old.pos;
            if (!sp8.isZero()) {
                mRotAxis = cXyz::BaseX;
                mDoMtx_stack_c::YrotS(current.angle.y);
                mDoMtx_stack_c::multVec(&mRotAxis, &mRotAxis);
            }

            mRotation += (s16)(((0.5f * field_0xd70) + data().m_urnRotateFactor) * (std::fabs(speedF) + std::fabs(speed.y)));
        }
    } else if (mMode == MODE_WALK || mMode == MODE_WAIT) {
        mRotAxis = cXyz::Zero;
        mRotation = 0;
    } else {
        mRotAxis = cXyz::Zero;
        mRotation = 0;
        field_0xd60 = 0.0f;
        field_0xd5c = 0.0f;
        field_0xd68 = 0;
        field_0xd66 = 0;
        field_0xd64 = 0;
    }
}

void daObjCarry_c::calc_rot_axis_kotubo2() {
    calc_rot_axis_base(5);
}

void daObjCarry_c::calc_rot_axis_LightBall() {
    calc_rot_axis_base(10);
}

void daObjCarry_c::calc_rot_axis_Lv8Ball() {
    calc_rot_axis_base(10);
}

void daObjCarry_c::set_wind_power() {
    if (!fopAcM_checkCarryNow(this)) {
        if (mTgCyl.ChkTgHit()) {
            dCcD_GObjInf* tg_hit_gobj = mTgCyl.GetTgHitGObj();
            if (tg_hit_gobj != NULL && tg_hit_gobj->ChkAtType(AT_TYPE_LANTERN_SWING) && tg_hit_gobj->GetAtMtrl() == dCcD_MTRL_WIND) {
                cXyz* at_vec_p = tg_hit_gobj->GetAtVecP();

                s16 var_r26 = cM_atan2s(at_vec_p->x, at_vec_p->z);
                f32 var_f31 = at_vec_p->absXZ() * data().m_urnWindEffRatio;
                f32 temp_f30 = std::fabs(at_vec_p->y) *  data().m_urnWindEffRatio;

                fopAc_ac_c* temp_r28 = mTgCyl.GetTgHitAc();
                if (var_f31 < 1.0f && temp_r28 != NULL) {
                    var_r26 = cM_atan2s(current.pos.x - temp_r28->current.pos.x, current.pos.z - temp_r28->current.pos.z);
                    var_f31 += 2.0f;
                }

                cXyz sp14(speedF * cM_ssin(current.angle.y), speed.y, speedF * cM_scos(current.angle.y));
                cXyz sp8(var_f31 * cM_ssin(var_r26), temp_f30, var_f31 * cM_scos(var_r26));
                sp14 += sp8;

                speedF = std::sqrt((sp14.x * sp14.x) + (sp14.z * sp14.z));
                current.angle.y = cM_atan2s(sp14.x, sp14.z);
                speed.y += temp_f30;

                if (current.pos.y > old.pos.y) {
                    field_0xcec = current.pos.y;
                }
            }
        }

        mTgCyl.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mTgCyl);
    }
}

void daObjCarry_c::exec_proc_tsubo() {
    set_wind_power();
}

void daObjCarry_c::exec_proc_ootubo() {
    set_wind_power();
}

void daObjCarry_c::exec_proc_kibako() {}

void daObjCarry_c::exec_proc_ironball() {
    if (mAcch.GetGroundH() != -G_CM3D_F_INF) {
        savePos(getSaveID(), current.pos);
        setRoomNo(getSaveID(), fopAcM_GetRoomNo(this));
    }
}

void daObjCarry_c::exec_proc_taru() {}

void daObjCarry_c::exec_proc_dokuro() {}

void daObjCarry_c::exec_proc_bokkuri() {
    switch (field_0xd79) {
    case 0:
        if (mAcch.ChkGroundLanding()) {
            field_0xd79 = 1;
            field_0xd77 = 15;
        }
        break;
    case 1:
        if (field_0xd77 != 0) {
            field_0xd77--;
        }

        if (field_0xd77 == 0) {
            mSound.startSound(Z2SE_EN_DB_SHRINK, 0, -1);
            field_0xd79 = 2;
        }
        break;
    case 2:
        cLib_chaseUC(&field_0xd76, 0, 0xFF / 24);
        if (field_0xd76 == 0) {
            field_0xd75 = 0;
            field_0xd79 = 3;
        }
        break;
    case 3:
        break;
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (field_0xd7a != 0) {
        if (field_0xd7a < 100) {
            field_0xd7a++;
        }

        field_0xd84 = (field_0xd7a - 16) / 5.0f;
        if (field_0xd84 > 1.0f) {
            field_0xd84 = 1.0f;
        } else if (field_0xd84 < 0.0f) {
            field_0xd84 = 0.0f;
        }
    } else {
        field_0xd84 = 0.0f;
    }

    if (mMode == MODE_CARRY) {
        if (field_0xd7a > 16) {
            cLib_addCalc0(&field_0xd5c, 0.2f, 500.0f);
            cLib_addCalc0(&field_0xd60, 0.2f, 500.0f);
            cLib_addCalcAngleS2(&field_0xd6a, 0, 5, 4000);
        }
        field_0xd68 = 0;
    } else if (speedF < 10.0f) {
        cLib_addCalc0(&field_0xd5c, 0.04f, 200.0f);
        cLib_addCalc0(&field_0xd60, 0.04f, 200.0f);
        cLib_addCalcAngleS(&field_0xd68, 0, 30, 100, 10);
    } else if (mMode == MODE_WALK) {
        cLib_chaseS(&field_0xd68, 300.0f * speedF, 100);
    }

    field_0xd64 = field_0xd5c * cM_ssin(field_0xd04 * 2000);
    field_0xd66 = field_0xd60 * cM_ssin(field_0xd04 * 2000);
    field_0xd6a += field_0xd68;

    if (mMode != MODE_GROWTH) {
        f32 scale_target = data().scale;
        cLib_addCalc(&scale.x, scale_target, 0.1f, 0.2f, 0.1f);
        cLib_addCalc(&scale.y, scale_target, 0.1f, 0.2f, 0.1f);
        cLib_addCalc(&scale.z, scale_target, 0.1f, 0.2f, 0.1f);
    }
}

void daObjCarry_c::exec_proc_LightBall() {
    mDalkmistInf.mPos = current.pos;
    mLightAtSph.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mLightAtSph);

    if (mCyl.ChkTgHit() && mCyl.GetTgHitObj()) {
        mSound.startCollisionSE(mCyl.GetTgHitObjHitSeID(1), 2, NULL);
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daObjCarry_c::exec_proc_Lv8Ball() {
    if (mCyl.ChkTgHit() && mCyl.GetTgHitObj()) {
        mSound.startCollisionSE(mCyl.GetTgHitObjHitSeID(1), 2, NULL);
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daObjCarry_c::bg_damage_proc_call() {
    typedef bool (daObjCarry_c::*type_func)();
    static type_func l_type_func[] = {
        &daObjCarry_c::bg_damage_proc_kotubo,    &daObjCarry_c::bg_damage_proc_ootubo,
        &daObjCarry_c::bg_damage_proc_kibako,    &daObjCarry_c::bg_damage_proc_ironball,
        &daObjCarry_c::bg_damage_proc_taru,      &daObjCarry_c::bg_damage_proc_dokuro,
        &daObjCarry_c::bg_damage_proc_bokkuri,   &daObjCarry_c::bg_damage_proc_kotubo,
        &daObjCarry_c::bg_damage_proc_LightBall, &daObjCarry_c::bg_damage_proc_LightBall,
        &daObjCarry_c::bg_damage_proc_ootubo,    &daObjCarry_c::bg_damage_proc_Lv8Ball,
        &daObjCarry_c::bg_damage_proc_kotubo,    &daObjCarry_c::bg_damage_proc_ootubo,
    };

    if (field_0xdae == 0) {
        field_0xdae |= (this->*l_type_func[mType])();
    }
}

void daObjCarry_c::cc_damage_proc_call() {
    typedef bool (daObjCarry_c::*type_func)();
    static type_func l_type_func[] = {
        &daObjCarry_c::cc_damage_proc_kotubo,    &daObjCarry_c::cc_damage_proc_ootubo,
        &daObjCarry_c::cc_damage_proc_kibako,    &daObjCarry_c::cc_damage_proc_ironball,
        &daObjCarry_c::cc_damage_proc_taru,      &daObjCarry_c::cc_damage_proc_dokuro,
        &daObjCarry_c::cc_damage_proc_bokkuri,   &daObjCarry_c::cc_damage_proc_kotubo,
        &daObjCarry_c::cc_damage_proc_LightBall, &daObjCarry_c::cc_damage_proc_LightBall,
        &daObjCarry_c::cc_damage_proc_ootubo,    &daObjCarry_c::cc_damage_proc_Lv8Ball,
        &daObjCarry_c::cc_damage_proc_kotubo,    &daObjCarry_c::cc_damage_proc_ootubo,
    };

    if (field_0xdae == 0) {
        field_0xdae |= (this->*l_type_func[mType])();
    }
}

void daObjCarry_c::eff_break_call() {
    typedef void (daObjCarry_c::*type_func)();
    static type_func l_type_func[] = {
        &daObjCarry_c::eff_break_tsubo,     &daObjCarry_c::eff_break_ootubo,
        &daObjCarry_c::eff_break_kibako,    &daObjCarry_c::eff_break_ironball,
        &daObjCarry_c::eff_break_taru,      &daObjCarry_c::eff_break_dokuro,
        &daObjCarry_c::eff_break_bokkuri,   &daObjCarry_c::eff_break_kotubo2,
        &daObjCarry_c::eff_break_LightBall, &daObjCarry_c::eff_break_LightBall,
        &daObjCarry_c::eff_break_ootubo2,   &daObjCarry_c::eff_break_Lv8Ball,
        &daObjCarry_c::eff_break_tsuboL8,   &daObjCarry_c::eff_break_ootuboL8,
    };

    (this->*l_type_func[mType])();
}

void daObjCarry_c::calc_rot_call() {
    typedef void (daObjCarry_c::*type_func)();
    static type_func l_type_func[] = {
        &daObjCarry_c::calc_rot_axis_tsubo,     &daObjCarry_c::calc_rot_axis_ootubo,
        &daObjCarry_c::calc_rot_axis_kibako,    &daObjCarry_c::calc_rot_axis_ironball,
        &daObjCarry_c::calc_rot_axis_taru,      &daObjCarry_c::calc_rot_axis_dokuro,
        &daObjCarry_c::calc_rot_axis_bokkuri,   &daObjCarry_c::calc_rot_axis_kotubo2,
        &daObjCarry_c::calc_rot_axis_LightBall, &daObjCarry_c::calc_rot_axis_LightBall,
        &daObjCarry_c::calc_rot_axis_ootubo,    &daObjCarry_c::calc_rot_axis_Lv8Ball,
        &daObjCarry_c::calc_rot_axis_tsubo,     &daObjCarry_c::calc_rot_axis_ootubo,
    };

    (this->*l_type_func[mType])();
}

void daObjCarry_c::obj_execute_proc_call() {
    typedef void (daObjCarry_c::*type_func)();
    static type_func l_type_func[] = {
        &daObjCarry_c::exec_proc_tsubo,     &daObjCarry_c::exec_proc_ootubo,
        &daObjCarry_c::exec_proc_kibako,    &daObjCarry_c::exec_proc_ironball,
        &daObjCarry_c::exec_proc_taru,      &daObjCarry_c::exec_proc_dokuro,
        &daObjCarry_c::exec_proc_bokkuri,   &daObjCarry_c::exec_proc_tsubo,
        &daObjCarry_c::exec_proc_LightBall, &daObjCarry_c::exec_proc_LightBall,
        &daObjCarry_c::exec_proc_ootubo,    &daObjCarry_c::exec_proc_Lv8Ball,
        &daObjCarry_c::exec_proc_tsubo,     &daObjCarry_c::exec_proc_ootubo,
    };

    (this->*l_type_func[mType])();
}

void daObjCarry_c::setTgHitCallBack() {
    typedef void (*TgCallBack)(fopAc_ac_c*, dCcD_GObjInf*, fopAc_ac_c*, dCcD_GObjInf*);
    static TgCallBack l_callback[] = {
        TgHitCallBackBase,    TgHitCallBackBase, TgHitCallBackBase,
        TgHitCallBackBase,    TgHitCallBackBase, TgHitCallBackBase,
        TgHitCallBackBokkuri, TgHitCallBackBase, TgHitCallBackBase,
        TgHitCallBackBase,    TgHitCallBackBase, TgHitCallBackBase,
        TgHitCallBackBase,    TgHitCallBackBase,
    };

    mCyl.SetTgHitCallback(l_callback[mType]);
}

void daObjCarry_c::setCoHitCallBack() {
    typedef void (*CoCallBack)(fopAc_ac_c*, dCcD_GObjInf*, fopAc_ac_c*, dCcD_GObjInf*);
    static CoCallBack l_callback[] = {
        CoHitCallBackBase,    CoHitCallBackBase, CoHitCallBackBase,
        CoHitCallBackBase,    CoHitCallBackBase, CoHitCallBackBase,
        CoHitCallBackBokkuri, CoHitCallBackBase, CoHitCallBackBase,
        CoHitCallBackBase,    CoHitCallBackBase, CoHitCallBackBase,
        CoHitCallBackBase,    CoHitCallBackBase,
    };

    mCyl.SetCoHitCallback(l_callback[mType]);
}

int daObjCarry_c::CreateInitCall() {
    typedef int (daObjCarry_c::*CreateInitFunc)();
    static CreateInitFunc l_func[] = {
        &daObjCarry_c::CreateInit_tsubo,     &daObjCarry_c::CreateInit_ootubo,
        &daObjCarry_c::CreateInit_kibako,    &daObjCarry_c::CreateInit_ironball,
        &daObjCarry_c::CreateInit_taru,      &daObjCarry_c::CreateInit_dokuro,
        &daObjCarry_c::CreateInit_bokkuri,   &daObjCarry_c::CreateInit_tsubo,
        &daObjCarry_c::CreateInit_LightBall, &daObjCarry_c::CreateInit_LightBall,
        &daObjCarry_c::CreateInit_ootubo,    &daObjCarry_c::CreateInit_Lv8Ball,
        &daObjCarry_c::CreateInit_tsubo,     &daObjCarry_c::CreateInit_ootubo,
    };

    return (this->*l_func[mType])();
}

static int daObjCarry_Draw(daObjCarry_c* i_this) {
    return i_this->draw();
}

static int daObjCarry_Execute(daObjCarry_c* i_this) {
    return i_this->execute();
}

static int daObjCarry_Delete(daObjCarry_c* i_this) {
    return i_this->_delete();
}

static int daObjCarry_Create(fopAc_ac_c* i_this) {
    return ((daObjCarry_c*)i_this)->create();
}

static actor_method_class l_daObjCarry_Method = {
    (process_method_func)daObjCarry_Create, (process_method_func)daObjCarry_Delete,
    (process_method_func)daObjCarry_Execute, (process_method_func)NULL,
    (process_method_func)daObjCarry_Draw};

extern actor_process_profile_definition g_profile_Obj_Carry = {
    fpcLy_CURRENT_e,         // mLayerID
    8,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Carry,          // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjCarry_c),    // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    244,                     // mPriority
    &l_daObjCarry_Method,    // sub_method
    0x40500,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
