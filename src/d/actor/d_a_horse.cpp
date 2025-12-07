/**
 * @file d_a_horse.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_hozelda.h"
#include "d/actor/d_a_cow.h"
#include "d/actor/d_a_tag_hstop.h"
#include "d/d_demo.h"
#include "d/d_meter2_info.h"
#include "m_Do/m_Do_controller_pad.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
#include <cmath.h>

#define ANM_HS_BACK_WALK           6
#define ANM_HS_WALK_START          7
#define ANM_HS_EXCITEMENT          8
#define ANM_HS_EXCITEMENT_2        9
#define ANM_HS_JUMP_END            10
#define ANM_HS_JUMP_MIDDLE         11
#define ANM_HS_JUMP_START          12
#define ANM_HS_JUMP_START_CLIFT    13
#define ANM_HS_LASH                14
#define ANM_HS_NECK_WAITBACK_LEFT  15
#define ANM_HS_NECK_WAITBACK_RIGHT 16
#define ANM_HS_RUN_DASH            17
#define ANM_HS_RUN_FAST            18
#define ANM_HS_RUN_SLOW            19
#define ANM_HS_STAND               20
#define ANM_HS_STOP_SLOW           21
#define ANM_HS_STOP_STAND          22
#define ANM_HS_TURN_LEFT           23
#define ANM_HS_TURN_LEFT_START     24
#define ANM_HS_TURN_RIGHT          25
#define ANM_HS_TURN_RIGHT_START    26
#define ANM_HS_WAIT_01             27
#define ANM_HS_WAIT_01B            28
#define ANM_HS_WAIT_01B_CONNECT    29
#define ANM_HS_WAIT_02             30
#define ANM_HS_WAIT_03             31
#define ANM_HS_WAIT_04             32
#define ANM_HS_WAIT_05             33
#define ANM_HS_WALK_FAST           34
#define ANM_HS_WALK_SLOW           35

static void strippedFunc()
{
    extern void F(f32*);
    f32 v[3] = {0.0f};
    F(v);
}

static char const l_arcName[] = "Horse";

const u16 daHorse_c::m_footJointTable[] = {
    3, 7, 27, 31,
};

const f32 daHorse_c::m_callLimitDistance2 = 640000.0f;

const daHorse_hio_c1 daHorse_hio_c0::m = {
    600,
    50,
    45,
    7,
    14,
    8,
    90,
    210,
    15,
    7,
    13,
    8,
    3,
    45,
    42.0f,
    1.0f,
    0.8f,
    1.5f,
    1.2f,
    1.5f,
    1.0f,
    0.08f,
    0.45f,
    0.9f,
    1.1f,
    5.0f,
    1.4f,
    0.4f,
    2.0f,
    6.5f,
    1.0f,
    0.5f,
    1.0f,
    1.1f,
    5.0f,
    45.0f,
    1.7f,
    2.5f,
    1.1f,
    5.0f,
    57.0f,
    50.0f,
    31.0f,
    2.0f,
    12.0f,
    1.7f,
    4.0f,
    1.0f,
    0.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    9.0f,
    40.0f,
    350.0f,
    150.0f,
    100.0f,
    1000.0f,
    50.0f,
    40.0f,
    1.3f,
    0.5f,
    4.0f,
    0.7f,
    2.0f,
    3.0f,
    0.3f,
    7.0f,
    6.0f,
    1.0f,
    2.0f,
    2.0f,
    9.0f,
    125.0f,
    0.0f,
    35.0f,
    1.1f,
    1.3f,
    15.0f,
};

void daHorseRein_c::setReinPos(int param_0) {
    cXyz* var_r27 = &field_0x0[0][param_0] + 1;

    cXyz spE0 = *field_0x0[0] - *var_r27;
    spE0.normalizeZP();

    f32 var_f31 = (f32)param_0 / (f32)field_0x8[0];
    f32 var_f30 = var_f31 * var_f31;

    spE0 *= (1.0f - var_f30) * 3.0f;

    spE0.y += -4.5f;
    spE0.x += var_f30 * field_0x10;
    spE0.z += var_f30 * field_0x14;

    cXyz* var_r28 = &field_0x0[0][param_0];
    cXyz* var_r25 = &field_0x0[1][param_0];
    cXyz spD4(*var_r28);

    cXyz spC8 = ((*var_r28 - *var_r27) + *var_r25) + spE0;

    if (field_0x18 > 0.0f) {
        cXyz spBC = field_0x1c - *var_r27;
        spBC.normalizeZP();

        spC8 += spBC * field_0x18;
        field_0x18 -= 1.1764706f;
    }

    spC8.normalizeZP();
    *var_r28 = *var_r27 + (spC8 * 5.5f);

    spC8 = *var_r28 - *field_0x0[0];

    f32 var_f29 = spC8.abs();
    f32 var_f28 = (f32)param_0 * 5.5f * 1.2f;
    if (var_f29 > var_f28) {
        spC8 *= var_f28 / var_f29;
        *var_r28 = ((*field_0x0[0] + spC8) * (1.0f - var_f31)) + (*var_r28 * var_f31);
    }

    *var_r25 = (*var_r28 - spD4) * 0.2f;
}

void daHorseRein_c::setReinPosPart(int param_0) {
    field_0x8[0] = param_0 + 1;
    field_0x0[1][param_0] = cXyz::Zero;

    cXyz* var_r25 = &field_0x0[0][param_0];

    int var_r28 = field_0x8[1] - 1;
    cXyz* var_r27 = &field_0x0[0][var_r28];
    cXyz* var_r26 = &field_0x0[1][var_r28];
    int i;
    for (i = var_r28; i > param_0; i--, var_r27--, var_r26--) {
        *var_r27 = *var_r25;
        *var_r26 = cXyz::Zero;
    }
    param_0--;
    for (i = param_0; i > 0; i--) {
        setReinPos(i);
    }
}

static f32 l_autoUpHeight = 50.0f;

static dCcD_SrcCyl l_cylSrc = {
    {
        {0x0, {{AT_TYPE_HORSE, 0x1, 0x12}, {0xd8fbfdff, 0x9}, 0x79}}, // mObj
        {dCcD_SE_HARD_BODY, 0x1, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x226}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        50.0f, // mRadius
        230.0f // mHeight
    } // mCyl
};

static dCcD_SrcSph l_sphSrc = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x9}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x226}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
    } // mSphAttr
};

void daHorse_c::coHitCallbackBoarJump(fopAc_ac_c* i_hitActor) {
    if (!dComIfGp_event_runCheck() && 
        !checkEndResetStateFlg0(daHorse_ERFLG0(ERFLG0_UNK_4 | ERFLG0_UNK_2 | ERFLG0_UNK_1)) &&
        fopAcM_GetName(i_hitActor) == PROC_E_WB &&
        ((fopEn_enemy_c*)i_hitActor)->checkDownFlg())
    {
        f32 sin_y = cM_ssin(current.angle.y);
        f32 cos_y = cM_scos(current.angle.y);
        f32 jump_chk_dist = (m_hio->m.boar_jump_horizontal + m_hio->m.boar_jump_horizontal) + 100.0f;

        cXyz start(current.pos);
        cXyz end(start.x + (jump_chk_dist * sin_y), start.y, start.z + (jump_chk_dist * cos_y));

        int i;
        for (i = 0; i < 3; i++) {
            start.y = current.pos.y + m_acchcir[i].GetWallH();
            end.y = start.y;
            m_linechk.Set(&start, &end, this);

            if (dComIfG_Bgsp().LineCross(&m_linechk)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(m_linechk, &plane);

                if (cBgW_CheckBWall(plane.mNormal.y)) {
                    break;
                }
            }
        }

        if (i == 3) {
            cXyz chk_pos;
            chk_pos.y = current.pos.y + 200.0f;
            
            while (jump_chk_dist > 0.0f) {
                chk_pos.x = current.pos.x + (jump_chk_dist * sin_y);
                chk_pos.z = current.pos.z + (jump_chk_dist * cos_y);

                if (!fopAcM_gc_c::gndCheck(&chk_pos) || fopAcM_gc_c::getHorseNoEntry()) {
                    break;
                }

                jump_chk_dist -= 25.0f;
            }
        }

        if (jump_chk_dist <= 0.0f) {
            onEndResetStateFlg0(ERFLG0_UNK_20);
            field_0x1768 = m_hio->m.boar_jump_horizontal;
            field_0x176c = m_hio->m.boar_jump_height + current.pos.y;
            field_0x1770 = field_0x1768;
        } else if (m_procID == PROC_MOVE_e && speedF > 5.0f) {
            setBoarHit(this, 0);
        }
    }
}

void daHorse_c::coHitCallbackBoarHit(fopAc_ac_c* i_hitActor, dCcD_GObjInf* i_hitGObj) {
    if (!dComIfGp_event_runCheck() &&
        ((m_procID == PROC_MOVE_e && speedF > 5.0f) || checkStateFlg0(daHorse_FLG0(FLG0_UNK_200000 | FLG0_UNK_100000))) &&
        checkStateFlg0(FLG0_UNK_1) &&
        fopAcM_GetName(i_hitActor) == PROC_E_WB &&
        !((fopEn_enemy_c*)i_hitActor)->checkDownFlg())
    {
        if (checkStateFlg0(daHorse_FLG0(FLG0_UNK_200000 | FLG0_UNK_100000))) {
            m_cowHitAngle = fopAcM_searchActorAngleY(i_hitActor, this);
            if (m_cowHitAngle > 0x5000) {
                m_cowHitAngle = 0x5000;
            } else if (m_cowHitAngle < -0x5000) {
                m_cowHitAngle = -0x5000;
            } else if (abs(m_cowHitAngle) < 0x3000) {
                if (m_cowHitAngle > 0) {
                    m_cowHitAngle = 0x3000;
                } else {
                    m_cowHitAngle = -0x3000;
                }
            }

            m_cowHit = 5;
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));
            if (checkStateFlg0(FLG0_UNK_100000)) {
                procLargeDamageInit();
            }
        } else {
            setBoarHit(i_hitActor, i_hitGObj->GetStts()->GetWeightUc() == 0xFE);
        }
    }
}

void daHorse_c::coHitCallbackCowHit(fopAc_ac_c* i_hitActor) {
    if ((fopAcM_GetName(i_hitActor) == PROC_COW) && ((daCow_c*)i_hitActor)->isAngry()) {
        ((daCow_c*)i_hitActor)->setAngryHit();

        if (!dComIfGp_event_runCheck() && checkStateFlg0(FLG0_UNK_1)) {
            m_cowHitAngle = fopAcM_searchActorAngleY(i_hitActor, this);
            if ((s16)(m_cowHitAngle - shape_angle.y) >= 0) {
                field_0x1702 = -0x500;
            } else {
                field_0x1702 = 0x500;
            }

            m_cowHit = 5;
        }
    } else if (fopAcM_GetName(i_hitActor) == PROC_ALINK) {
        if (daAlink_getAlinkActorClass()->checkSlideMode()) {
            onEndResetStateFlg0(ERFLG0_UNK_800);
        }
    }
}

static void daHorse_coHitCallbackBoarJump(fopAc_ac_c* i_coActorA, dCcD_GObjInf* i_coObjInfA,
                                          fopAc_ac_c* i_coActorB, dCcD_GObjInf* i_coObjInfB) {
    (void)i_coObjInfA;
    (void)i_coObjInfB;
    ((daHorse_c*)i_coActorA)->coHitCallbackBoarJump(i_coActorB);
}

static void daHorse_coHitCallbackCowHit(fopAc_ac_c* i_coActorA, dCcD_GObjInf* i_coObjInfA,
                                        fopAc_ac_c* i_coActorB, dCcD_GObjInf* i_coObjInfB) {
    (void)i_coObjInfA;
    (void)i_coObjInfB;
    ((daHorse_c*)i_coActorA)->coHitCallbackCowHit(i_coActorB);
}

static void daHorse_coHitCallbackAll(fopAc_ac_c* i_coActorA, dCcD_GObjInf* i_coObjInfA,
                                     fopAc_ac_c* i_coActorB, dCcD_GObjInf* i_coObjInfB) {
    (void)i_coObjInfA;
    daHorse_c* a_this = (daHorse_c*)i_coActorA;

    a_this->coHitCallbackBoarJump(i_coActorB);
    a_this->coHitCallbackBoarHit(i_coActorB, i_coObjInfB);
    a_this->coHitCallbackCowHit(i_coActorB);
}

static void* daHorse_searchEnemy(fopAc_ac_c* i_actor, void* i_data) {
    daHorse_c* horse_p = dComIfGp_getHorseActor();
    f32 search_dist = *(f32*)i_data;

    if (fopAcM_GetGroup(i_actor) == fopAc_ENEMY_e && fopAcM_GetName(i_actor) != PROC_E_WS && horse_p->current.pos.abs2XZ(i_actor->current.pos) < search_dist * search_dist) {
        return i_actor;
    }

    return NULL;
}

int daHorse_c::modelCallBack(int i_jntNo) {
    if (i_jntNo == 0) {
        if (m_procID == PROC_TOOL_DEMO_e) {
            J3DTransformInfo* var_r24 = m_oldFrame->getOldFrameTransInfo(i_jntNo);
            var_r24->mTranslate.x = 0.0f;
            var_r24->mTranslate.z = 0.0f;

            MtxP m = m_model->getAnmMtx(i_jntNo);
            m[0][3] = current.pos.x;
            m[2][3] = current.pos.z;
            cMtx_copy(m, J3DSys::mCurrentMtx);
        }
    } else if (i_jntNo == 3 || i_jntNo == 7 || i_jntNo == 11 || i_jntNo == 22 || i_jntNo == 24 || i_jntNo == 27 || i_jntNo == 31) {
        if (i_jntNo == 11) {
            MtxP m = m_model->getAnmMtx(11);
            mDoMtx_stack_c::transS(m[0][3], m[1][3], m[2][3]);
            mDoMtx_stack_c::YrotM(field_0x16f0);
            mDoMtx_stack_c::transM(-m[0][3], -m[1][3], -m[2][3]);
            mDoMtx_stack_c::concat(m);
        } else {
            mDoMtx_stack_c::copy(m_model->getAnmMtx(i_jntNo));
        }

        if (i_jntNo == 27 || i_jntNo == 31 || i_jntNo == 22 || i_jntNo == 24) {
            mDoMtx_stack_c::ZrotM((shape_angle.x >> 1));
        } else {
            mDoMtx_stack_c::ZrotM(-(shape_angle.x >> 1));
        }

        m_model->setAnmMtx(i_jntNo, mDoMtx_stack_c::get());
        mDoMtx_copy(m_model->getAnmMtx(i_jntNo), J3DSys::mCurrentMtx);
    } else if ((!checkEndResetStateFlg0(ERFLG0_UNK_40) && (i_jntNo == 35 || i_jntNo == 36 || i_jntNo == 37)) || i_jntNo == 2 || i_jntNo == 26) {
        Quaternion sp20;
        Quaternion sp10;
        BOOL spC;
        s16 sp8;
        if (i_jntNo == 2 || i_jntNo == 26) {
            sp8 = -field_0x16fa;
            spC = FALSE;
        } else {
            sp8 = field_0x16d4[i_jntNo - 35];
            spC = TRUE;
        }

        JMAEulerToQuat(0, sp8, 0, &sp10);
        mDoMtx_QuatConcat(m_oldFrame->getOldFrameQuaternion(i_jntNo), &sp10, &sp20);

        J3DTransformInfo* var_r27 = m_oldFrame->getOldFrameTransInfo(i_jntNo);
        mDoMtx_stack_c::transS(var_r27->mTranslate.x, var_r27->mTranslate.y, var_r27->mTranslate.z);
        mDoMtx_stack_c::quatM(m_oldFrame->getOldFrameQuaternion(i_jntNo));
        mDoMtx_stack_c::inverse();

        MtxP m = m_model->getAnmMtx(i_jntNo);
        cMtx_concat(m, mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        MTXQuat(m, &sp20);
        m[0][3] = var_r27->mTranslate.x;
        m[1][3] = var_r27->mTranslate.y;
        m[2][3] = var_r27->mTranslate.z;
        cMtx_concat(J3DSys::mCurrentMtx, m, J3DSys::mCurrentMtx);
        cMtx_copy(J3DSys::mCurrentMtx, m);

        if (spC) {
            *m_oldFrame->getOldFrameQuaternion(i_jntNo) = sp20;
        }
    } else if (i_jntNo == 34) {
        setFootMatrix();
    }

    if (m_anmRatio[2].getAnmTransform() != NULL) {
        if (i_jntNo == 10) {
            m_anmRatio[2].setRatio(1.0f);
        } else if (i_jntNo == 20) {
            m_anmRatio[2].setRatio(0.0f);
        }
    }

    return 1;
}

static int daHorse_modelCallBack(J3DJoint* i_joint, int param_1) {
    J3DJoint* joint = i_joint;
    int jnt_no = joint->getJntNo();
    daHorse_c* a_this = (daHorse_c*)j3dSys.getModel()->getUserArea();

    if (param_1 == 0) {
        a_this->modelCallBack(jnt_no);
    }

    return 1;
}

static void* daHorse_searchSingleBoar(fopAc_ac_c* i_actor, void* i_data) {
    if (fopAcM_GetName(i_actor) == PROC_E_WB) {
        fopAc_ac_c** ppActor = (fopAc_ac_c**)i_data;
        *ppActor = i_actor;
    }

    return NULL;
}

int daHorse_c::createHeap() {
    m_modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 0x26);
    JUT_ASSERT(0x487, m_modelData != NULL);

    m_model = mDoExt_J3DModel__create(m_modelData, 0x80000, 0x11020084);
    if (m_model == NULL) {
        return 0;
    }

    J3DTransformInfo* transInfoBuf = new J3DTransformInfo[38];
    if (transInfoBuf == NULL) {
        return 0;
    }

    Quaternion* quatBuf = new Quaternion[38];
    if (quatBuf == NULL) {
        return 0;
    }

    m_oldFrame = new mDoExt_MtxCalcOldFrame(transInfoBuf, quatBuf);
    if (m_oldFrame == NULL) {
        return 0;
    }
    
    m_mtxcalc = new mDoExt_MtxCalcAnmBlendTblOld(m_oldFrame, 3, m_anmRatio);
    if (m_mtxcalc == NULL) {
        return 0;
    }

    if (!m_btp.init(m_modelData, (J3DAnmTexPattern*)dComIfG_getObjectRes(l_arcName, 0x29), FALSE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
        return 0;
    }

    m_hio = new HORSE_HIO_CLASS();
    if (m_hio == NULL) {
        return 0;
    }

    ResTIMG* texImg = (ResTIMG*)dComIfG_getObjectRes(l_arcName, 0x2C);
    JUT_ASSERT(0x4B6, texImg != NULL);

    if (!m_reinLine.init(1, 75, texImg, 0)) {
        return 0;
    }

    m_rein[0].field_0x8[1] = 35;
    m_rein[1].field_0x8[1] = 35;
    m_rein[2].field_0x8[1] = 5;

    daHorseRein_c* rein_p = m_rein;
    for (int i = 0; i < 3; i++, rein_p++) {
        rein_p->field_0x0[0] = new cXyz[rein_p->field_0x8[1]];
        if (rein_p->field_0x0[0] == NULL) {
            return 0;
        }

        rein_p->field_0x0[1] = new cXyz[rein_p->field_0x8[1]];
        if (rein_p->field_0x0[1] == NULL) {
            return 0;
        }
    }

    return 1;
}

static int daHorse_createHeap(fopAc_ac_c* i_this) {
    return ((daHorse_c*)i_this)->createHeap();
}

BOOL daHorse_c::checkEnding() {
    return (daAlink_c::checkStageName("F_SP108") && dComIfG_play_c::getLayerNo(0) == 4) ||
           (daAlink_c::checkStageName("F_SP104") && dComIfG_play_c::getLayerNo(0) == 12 && dComIfGp_roomControl_getStayNo() == 1);
}

#if DEBUG
int l_debugMode;
#else
#define l_debugMode 0
#endif
extern int g_horsePosInit;

int daHorse_c::create() {
    fopAcM_ct(this, daHorse_c);

    if (checkEnding()) {
        onStateFlg0(FLG0_UNK_8000);
    }

    if (!checkStateFlg0(FLG0_UNK_8000) &&
           /* Cutscene - Cutscene - attacked by monsters at Ordon spring */
        (((dComIfGs_isEventBit(dSv_event_flag_c::M_008)
            /* Main Event - Epona rescued flag */
        && !dComIfGs_isEventBit(dSv_event_flag_c::M_023))
        && (!daAlink_c::checkStageName("F_SP109")
            /* Death Mountain - First saw Goron cutscene on mountain path */
        || !dComIfGs_isEventBit(dSv_event_flag_c::F_0066)))
           /* General use - When on (while changing scenes) stage name is not shown */
        || dComIfGs_isTmpBit(dSv_event_tmp_flag_c::NO_TELOP)))
    {
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(&m_phase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (daAlink_getAlinkActorClass() == NULL) {
            return cPhs_INIT_e;
        }

        if (dComIfGp_getHorseActor() != NULL) {
            return cPhs_ERROR_e;
        }

        int room_no;
        if (((fopAcM_GetParam(this) >> 8) & 0xF) == 1) {
            room_no = fopAcM_GetRoomNo(this);
        } else {
            room_no = -1;
        }

        m_checkSpecialWallHit = &daHorse_c::checkSpecialWallHitSubstance;
        m_setReinPosHand = &daHorse_c::setReinPosHandSubstance;
        m_setReinPosNormal = &daHorse_c::setReinPosNormalSubstance;
        m_callHorse = &daHorse_c::callHorseSubstance;
        m_setHorsePosAngle = &daHorse_c::setHorsePosAndAngleSubstance;
        m_onRideFlg = &daHorse_c::onRideFlgSubstance;
        m_offRideFlg = &daHorse_c::offRideFlgSubstance;

        if (daAlink_getAlinkActorClass()->checkHorseStart() || checkStateFlg0(FLG0_UNK_8000) ||
            (DEBUG && g_horsePosInit) ||
            strcmp(dComIfGs_getHorseRestartStageName(), "") == 0
            /* dSv_event_flag_c::M_002 - Cutscene - [cutscene: 2] Met with Ilia (brings horse to
               spring) */
            || (dComIfGs_isEventBit(0x1580)
                /* dSv_event_flag_c::M_023 - Main Event - Epona rescued flag */
                && !dComIfGs_isEventBit(0x601)))
        {
#if DEBUG
            g_horsePosInit = 0;
#endif
        } else {
            if (daAlink_c::checkStageName(dComIfGs_getHorseRestartStageName()) &&
                (room_no == -1 || fopAcM_GetRoomNo(this) == dComIfGs_getHorseRestartRoomNo()))
            {
                current.pos = dComIfGs_getHorseRestartPos();
                old.pos = current.pos;
                shape_angle.y = dComIfGs_getHorseRestartAngleY();
                current.angle.y = shape_angle.y;
                fopAcM_SetRoomNo(this, dComIfGs_getHorseRestartRoomNo());
            } else {
                onStateFlg0(FLG0_NO_DRAW_WAIT);
            }
        }

        if (!fopAcM_entrySolidHeap(this, daHorse_createHeap, 0x6E60)) {
            return cPhs_ERROR_e;
        }

        if (daAlink_c::checkStageName("F_SP00") || daAlink_c::checkStageName("F_SP103") || daAlink_c::checkStageName("F_SP104") || daAlink_c::checkStageName("F_SP108") || daAlink_c::checkStageName("F_SP113")) {
            onStateFlg0(FLG0_UNK_2000);
            m_normalMaxSpeedF = m_hio->m.kakariko_max_speed;
            m_lashAddSpeed = m_hio->m.kakariko_add_lash_speed;
        } else {
            m_normalMaxSpeedF = m_hio->m.max_speed;
            m_lashAddSpeed = m_hio->m.add_lash_speed;
        }

        field_0x16c2 = m_hio->m.max_turn;
        m_flowID = shape_angle.z;
        if (m_flowID == 0) {
            m_flowID = 5000;
        }

        shape_angle.z = 0;
        current.angle.z = 0;

        m_path = dPath_GetRoomPath(fopAcM_GetParam(this) & 0xFF, room_no);
        if (m_path != NULL) {
            m_rodeoPath = dPath_GetNextRoomPath(m_path, room_no);
        }

        m_rodeoPoint = 1;

#if DEBUG
        m_hio->id = mDoHIO_createChild("馬", m_hio);
#endif

        m_modelData->getJointNodePointer(0)->setMtxCalc(m_mtxcalc);
        fopAcM_SetMtx(this, m_model->getBaseTRMtx());
        
        m_cc_stts.Init(0xFE, 0xFF, this);
        m_acchcir[0].SetWall(l_autoUpHeight + 0.1f, 60.0f);
        m_acchcir[1].SetWall(150.0f, 60.0f);
        m_acchcir[2].SetWall(240.0f, 60.0f);
        m_acch.Set(this, 3, m_acchcir);
        m_acch.SetWaterCheckOffset(500.0f);

        field_0x1764 = m_hio->m.walk_to_fastwalk_rate * m_normalMaxSpeedF;
        m_lashMaxSpeedF = m_normalMaxSpeedF + m_lashAddSpeed;

        attention_info.distances[fopAc_attn_ETC_e] = 14;
        attention_info.distances[fopAc_attn_SPEAK_e] = 38;

        m_sound.init(&current.pos, &eyePos, 6, 1);
        
        resetBasAnime();
        m_anmRatio[2].setAnmTransform(NULL);
        m_anmRatio[2].setRatio(0.0f);
        m_anmIdx[2] = 0xFFFF;

        field_0x17dc.init(&m_acch, 60.0f, 200.0f);
        gravity = -5.0f;
        field_0x16ce = 0xD000;
        field_0x16d0 = 0xF000;

        dCcD_Cyl* cyl_p = m_tgco_cyl;
        for (int i = 0; i < 3; i++, cyl_p++) {
            cyl_p->Set(l_cylSrc);
            cyl_p->SetStts(&m_cc_stts);
        }

        m_tgco_cyl[1].SetR(35.0f);

        m_head_sph.Set(l_sphSrc);
        m_head_sph.SetStts(&m_cc_stts);

        setServiceWaitTimer();

        m_tgco_cyl[0].SetCoHitCallback(daHorse_coHitCallbackAll);
        m_tgco_cyl[1].SetCoHitCallback(daHorse_coHitCallbackCowHit);
        m_tgco_cyl[2].SetCoHitCallback(daHorse_coHitCallbackCowHit);

        m_boar_cyl.Set(l_cylSrc);
        m_boar_cyl.SetStts(&m_cc_stts);
        m_boar_cyl.SetCoHitCallback(daHorse_coHitCallbackBoarJump);
        m_boar_cyl.SetCoVsGrp(0x20);
        m_boar_cyl.OnCoNoCrrBit();
        m_boar_cyl.OnCoNoCoHitInfSet();
        m_boar_cyl.OffTgSetBit();
        m_boar_cyl.SetR(55.0f);
        
        m_at_cyl.Set(l_cylSrc);
        m_at_cyl.SetStts(&m_cc_stts);
        m_at_cyl.OffTgSetBit();
        m_at_cyl.OffCoSetBit();
        m_at_cyl.OnAtSetBit();
        m_at_cyl.SetH(150.0f);
        m_at_cyl.SetR(60.0f);

        m_procID = PROC_NONE_e;
        m_demoStaffId = -1;
        this->model = m_model;

        procWaitInit();

        m_model->setUserArea((uintptr_t)this);
        for (u16 i = 0; i < 38; i++) {
            m_modelData->getJointNodePointer(i)->setCallBack(daHorse_modelCallBack);
        }

        setMatrix();
        m_model->calc();
        setBodyPart();
        field_0x17b8 = m_bodyEyePos;

        m_acch.CrrPos(dComIfG_Bgsp());
        setRoomInfo(1);

        dComIfGp_setHorseActor(this);
        field_0x16e8 = shape_angle.y;

        cXyz* sp2C;
        cXyz* sp28;
        int j;
        int i;
        daHorseRein_c* rein_p = m_rein;
        for (i = 0; i < 3; i++, rein_p++) {
            sp2C = rein_p->field_0x0[0];
            sp28 = rein_p->field_0x0[1];

            for (j = 0; j < rein_p->field_0x8[1]; j++, sp2C++, sp28++) {
                *sp2C = cXyz::Zero;
                *sp28 = cXyz::Zero;
            }
        }

        field_0x170e = shape_angle.y;
        field_0x1710 = field_0x170e;
        setReinPosNormal();
#if DEBUG
        l_debugMode = 0;
#endif
        m_lashCnt = 6;
        field_0x170a = shape_angle.y;

        if (daAlink_c::checkStageName("F_SP102") || daAlink_c::checkStageName("D_MN09B") || daAlink_c::checkStageName("F_SP123")) {
            onStateFlg0(FLG0_UNK_4000000);
        }

        fopAcM_setStageLayer(this);
    }

    return phase_state;
}

static int daHorse_Create(fopAc_ac_c* i_this) {
    daHorse_c* this_horse = (daHorse_c*)i_this;
    fpc_ProcID unused = fopAcM_GetID(i_this);
    return this_horse->create();
}

void daHorse_c::setBasAnime(int param_0) {
    daPy_frameCtrl_c* frame_ctrl = &m_frameCtrl[param_0];

    if (((mDoExt_transAnmBas*)m_anmRatio[param_0].getAnmTransform())->getBas() == NULL) {
        resetBasAnime();
    } else if (field_0x16f6 == m_anmIdx[param_0] && field_0x175c * frame_ctrl->getRate() >= 0.0f) {
        int unused;
        field_0x1144 = frame_ctrl;
    } else {
        field_0x1140 = ((mDoExt_transAnmBas*)m_anmRatio[param_0].getAnmTransform())->getBas();
        field_0x1144 = frame_ctrl;
        field_0x16f6 = m_anmIdx[param_0];
        field_0x175c = frame_ctrl->getRate();

        f32 var_f31;
        if (field_0x1144->getAttribute() == 2) {
            var_f31 = field_0x1144->getFrame() - field_0x1144->getRate();

            if (var_f31 <= (f32)field_0x1144->getStart()) {
                var_f31 = (f32)field_0x1144->getEnd() - var_f31;
            } else if (var_f31 >= (f32)field_0x1144->getEnd()) {
                var_f31 -= (f32)field_0x1144->getEnd();
            }
        } else {
            var_f31 = 0.0f;
        }

        m_sound.initAnime(field_0x1140, field_0x1144->getRate() >= 0.0f, field_0x1144->getLoop(),
                          var_f31);
    }
}

void daHorse_c::resetBasAnime() {
    field_0x16f6 = 0xFFFF;
    field_0x1144 = NULL;
}

int daHorse_c::setDoubleAnime(f32 i_ratio, f32 i_anmSpeedA, f32 i_anmSpeedB, u16 i_anmIdxA, u16 i_anmIdxB,
                              f32 i_morf) {
    J3DAnmTransform* temp_r26 = m_anmRatio[0].getAnmTransform();
    if (i_morf < 0.0f && (m_anmIdx[0] != ANM_HS_RUN_DASH || i_anmIdxA != ANM_HS_RUN_SLOW) && (m_anmIdx[0] != i_anmIdxA || m_anmIdx[1] != i_anmIdxB)) {
        i_morf = 3.0f;
    }

    if (i_anmIdxA == ANM_HS_TURN_LEFT || i_anmIdxA == ANM_HS_TURN_RIGHT) {
        i_morf = -1.0f;
    } else if (i_anmIdxA != ANM_HS_TURN_RIGHT_START && i_anmIdxA != ANM_HS_TURN_LEFT_START) {
        if (m_anmIdx[0] == ANM_HS_TURN_LEFT) {
            setNeckAnime(ANM_HS_NECK_WAITBACK_LEFT, 1.0f, 0.0f, -1);
            i_morf = 8.0f;
        } else if (m_anmIdx[0] == ANM_HS_TURN_RIGHT) {
            setNeckAnime(ANM_HS_NECK_WAITBACK_RIGHT, 1.0f, 0.0f, -1);
            i_morf = 8.0f;
        }
    }

    f32 var_f28;
    if (temp_r26 != NULL && m_anmRatio[1].getAnmTransform() != NULL) {
        var_f28 = temp_r26->getFrame() / temp_r26->getFrameMax();
    } else {
        var_f28 = 0.0f;
    }

    J3DAnmTransform* bckA = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, i_anmIdxA);
    J3DAnmTransform* bckB = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, i_anmIdxB);
    m_anmIdx[0] = i_anmIdxA;
    m_anmIdx[1] = i_anmIdxB;

    m_anmRatio[0].setRatio(1.0f - i_ratio);
    m_anmRatio[1].setRatio(i_ratio);

    f32 frame_maxA = bckA->getFrameMax();
    f32 frame_maxB = bckB->getFrameMax();
    f32 temp_f25 = 1.0f / frame_maxA;
    f32 speed = i_anmSpeedA + (i_ratio * (((i_anmSpeedB * frame_maxA) / frame_maxB) - i_anmSpeedA));

    m_frameCtrl[0].setFrameCtrl(bckA->getAttribute(), 0, frame_maxA, speed, var_f28 * frame_maxA);
    bckA->setFrame(m_frameCtrl[0].getFrame());

    m_frameCtrl[1].setFrameCtrl(bckB->getAttribute(), 0, frame_maxB, temp_f25 * (speed * frame_maxB), var_f28 * frame_maxB);
    bckB->setFrame(m_frameCtrl[1].getFrame());

    m_anmRatio[0].setAnmTransform(bckA);
    m_anmRatio[1].setAnmTransform(bckB);

    if (i_morf >= 0.0f) {
        m_oldFrame->initOldFrameMorf(i_morf, 0, 38);
        m_morf_frame = i_morf;
    }

    if (m_procID == PROC_WAIT_e || i_ratio < 0.5f) {
        setBasAnime(0);
    } else {
        setBasAnime(1);
    }

    return 1;
}

int daHorse_c::setSingleAnime(u16 i_anmIdx, f32 i_speed, f32 i_startF, s16 i_endF, f32 i_morf,
                              BOOL i_isDemoAnm) {
    J3DAnmTransform* bck;
    if (i_isDemoAnm) {
        if (i_anmIdx & 0x8000) {
            JUT_ASSERT(1748, FALSE);
        }

        bck = (J3DAnmTransform*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), i_anmIdx);
#if PLATFORM_GCN || PLATFORM_WII
        i_anmIdx |= 0x8000;
#else
        i_anmIdx |= (u16)0x8000;
#endif
    } else {
        bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, i_anmIdx);
    }

    m_anmIdx[0] = i_anmIdx;
    m_anmIdx[1] = 0xFFFF;

    m_anmRatio[0].setRatio(1.0f);
    m_anmRatio[1].setRatio(0.0f);
    m_anmRatio[0].setAnmTransform(bck);
    m_anmRatio[1].setAnmTransform(NULL);

    s16 end_frame;
    if (i_endF < 0) {
        end_frame = bck->getFrameMax();
    } else {
        end_frame = i_endF;
    }

    f32 frame;
    if (i_speed < 0.0f) {
        frame = end_frame;
    } else {
        frame = i_startF;
    }

    m_frameCtrl[0].setFrameCtrl(bck->getAttribute(), i_startF, end_frame, i_speed, frame);
    bck->setFrame(frame);

    if (i_morf >= 0.0f) {
        m_oldFrame->initOldFrameMorf(i_morf, 0, 38);
        m_morf_frame = i_morf;
    }

    setBasAnime(0);
    return 1;
}

void daHorse_c::animePlay() {
    if (m_frameCtrl[2].getRate() < 0.1f) {
        resetNeckAnime();
    }

    if (daAlink_getAlinkActorClass()->checkHorseGetItem()) {
        m_frameCtrl[0].setFrame(0);
    } else {
        m_frameCtrl[0].updateFrame();
    }

    m_anmRatio[0].getAnmTransform()->setFrame(m_frameCtrl[0].getFrame());

    if (m_anmRatio[1].getAnmTransform() != NULL) {
        if (daAlink_getAlinkActorClass()->checkHorseGetItem()) {
            m_frameCtrl[1].setFrame(0);
        } else {
            m_frameCtrl[1].updateFrame();
        }

        m_anmRatio[1].getAnmTransform()->setFrame(m_frameCtrl[1].getFrame());
    }

    if (m_anmRatio[2].getAnmTransform() != NULL) {
        m_frameCtrl[2].updateFrame();
        if (m_anmIdx[2] == ANM_HS_WAIT_03 && m_frameCtrl[2].checkPass(18)) {
            m_sound.startCreatureVoice(Z2SE_HS_V_BURURU, m_reverb);
        }

        m_anmRatio[2].getAnmTransform()->setFrame(m_frameCtrl[2].getFrame());
    }

    if (m_btpFrame != 0) {
        m_btpFrame++;
        if (m_btpFrame > m_btp.getBtpAnm()->getFrameMax()) {
            m_btpFrame = 0;
        }
    } else if (cM_rnd() < 0.012f) {
        m_btpFrame++;
    }
}

int daHorse_c::checkDemoAction() {
    if (checkStateFlg0(FLG0_CALL_HORSE)) {
        setStickCallMove();
        offStateFlg0(FLG0_CALL_HORSE);
        onStateFlg0(FLG0_UNK_10000000);

        shape_angle.y = cLib_targetAngleY(&current.pos, &daAlink_getAlinkActorClass()->current.pos);
        current.angle.y = shape_angle.y;
        field_0x1728 = 0;
        field_0x16b8 = 0;
        m_callMoveTimer = 450;
        speedF = m_normalMaxSpeedF;
        return procMoveInit();
    }

    if (dComIfGp_getEvent().isOrderOK()) {
        return 0;
    }

    JUT_ASSERT(0x752, (m_demoMode < DEMO_LAST_e) || (m_demoMode == DEMO_NEW_ANM0_e));

    if (m_demoMode == DEMO_NEW_ANM0_e) {
        onStateFlg0(FLG0_UNK_80000);
        if (m_procID == PROC_TOOL_DEMO_e) {
            return 1;
        }

        return procToolDemoInit();
    }

    if (m_demoMode == 7) {
        onStateFlg0(FLG0_UNK_80000);
        if (m_procID == PROC_JUMP_e || m_procID == PROC_LAND_e) {
            return 1;
        }

        return procJumpInit(0);
    }

    if (m_demoMode == 16) {
        onStateFlg0(FLG0_UNK_80000);
        if (m_procID == PROC_STOP_e) {
            return 1;
        }

        if (m_procID == PROC_MOVE_e && checkStateFlg0(FLG0_UNK_2)) {
            return procStopInit();
        }

        if (m_procID == PROC_WAIT_e) {
            dComIfGp_evmng_cutEnd(m_demoStaffId);
        }

        return 1;
    }

    if (m_demoMode == 8 || m_demoMode == 14) {
        onStateFlg0(FLG0_UNK_80000);
        if (m_procID == PROC_STOP_e) {
            return 1;
        }

        return procStopInit();
    }

    if (m_demoMode == 9 || m_demoMode == 15) {
        onStateFlg0(FLG0_UNK_80000);
        if (m_procID == PROC_TURN_e) {
            return 1;
        }

        return procTurnInit(0);
    }

    if (m_demoMode == 5) {
        int temp_r3 = procWaitInit();
        if (temp_r3 != 0) {
            m_oldFrame->initOldFrameMorf(0.0f, 0, 38);
        }

        return temp_r3;
    }

    if (checkStateFlg0(FLG0_UNK_80000) && (m_demoMode == 1 || m_demoMode == 2 || m_demoMode == 10 || m_demoMode == 3 || m_demoMode == 11 || m_demoMode == 4)) {
        offStateFlg0(FLG0_UNK_80000);
        if (fabsf(speedF) < 0.05f) {
            return procWaitInit();
        }

        return procMoveInit();
    }

    return 0;
}

void daHorse_c::setStickRodeoMove() {
    if (m_rodeoPath != NULL) {
        Vec* path_pnt_pos = &m_rodeoPath->m_points[m_rodeoPoint].m_position;

        cXyz pos_vec(path_pnt_pos->x - current.pos.x, path_pnt_pos->y - current.pos.y, path_pnt_pos->z - current.pos.z);
        f32 dist_to_pnt = pos_vec.absXZ();

        m_padStickValue = 1.0f;
        onEndResetStateFlg0(ERFLG0_MOVE_ACCEPT);

        BOOL var_r27;
        if (dist_to_pnt < 400.0f && field_0x16bc == 0) {
            if (((!checkStateFlg0(FLG0_UNK_1) && m_rodeoPath->m_points[m_rodeoPoint].mArg0 != 1) || m_rodeoPointCnt >= 15) && m_procID == PROC_MOVE_e) {
                onEndResetStateFlg0(ERFLG0_UNK_200);
                if (!checkStateFlg0(FLG0_UNK_1)) {
                    field_0x16bc = cM_rndF(3.0f);
                }
            }

            m_rodeoPoint++;
            if (m_rodeoPath->m_num == m_rodeoPoint) {
                m_rodeoPoint = 0;
            }

            path_pnt_pos = &m_rodeoPath->m_points[m_rodeoPoint].m_position;
            pos_vec.set(path_pnt_pos->x - current.pos.x, path_pnt_pos->y - current.pos.y, path_pnt_pos->z - current.pos.z);

            var_r27 = 1;
            if (checkStateFlg0(FLG0_UNK_1)) {
                m_rodeoPointCnt++;
            }
        } else {
            if (dist_to_pnt < 400.0f && m_procID != PROC_STOP_e && m_procID != PROC_TURN_e && field_0x16bc != 0) {
                field_0x16bc = 0;
            }
            var_r27 = 0;
        }

        m_padStickAngleY = pos_vec.atan2sX_Z();

        if (m_procID != PROC_TURN_e && cLib_distanceAngleS(m_padStickAngleY, shape_angle.y) > 0x5000) {
            m_rodeoPoint++;
            if (m_rodeoPath->m_num == m_rodeoPoint) {
                m_rodeoPoint = 0;
            }

            path_pnt_pos = &m_rodeoPath->m_points[m_rodeoPoint].m_position;
            pos_vec.set(path_pnt_pos->x - current.pos.x, path_pnt_pos->y - current.pos.y, path_pnt_pos->z - current.pos.z);
            m_padStickAngleY = pos_vec.atan2sX_Z();
            var_r27 = 1;
            field_0x16bc = 0;
        }

        if (var_r27) {
            if ((s16)(m_padStickAngleY - shape_angle.y) > 0) {
                offStateFlg0(FLG0_RODEO_LEFT);
            } else {
                onStateFlg0(FLG0_RODEO_LEFT);
            }
        }
    } else {
        m_padStickAngleY = 0;
        m_padStickValue = 0.0f;
    }
}

void daHorse_c::setStickCallMove() {
    m_padStickValue = 1.0f;

    u32 mode = 3;
    setDemoMoveData(&mode, &daAlink_getAlinkActorClass()->current.pos);
    m_padStickAngleY = m_demoMoveAngle;

    if (m_callMoveTimer != 0) {
        m_callMoveTimer--;
    }
}

void daHorse_c::setDemoMoveData(u32* i_mode, cXyz const* i_pos) {
    cXyz pos_vec;
    pos_vec = *i_pos - current.pos;

    if (field_0x1728 != 0 && (*i_mode == 4 || *i_mode == 11 || *i_mode == 3)) {
        m_demoMoveAngle = pos_vec.atan2sX_Z();
        if (abs((s16)(m_demoMoveAngle - shape_angle.y)) > 0x4000 && m_demoStaffId != -1) {
            dComIfGp_evmng_cutEnd(m_demoStaffId);
        }
        return;
    }
    f32 temp_f29 = SQUARE(20.0f);
    f32 temp_f28 = SQUARE(40.0f);
    f32 temp_f30 = SQUARE(100.0f);

    f32 dist_xz2 = pos_vec.abs2XZ();
    if (dist_xz2 < temp_f29 || (dist_xz2 < temp_f30 && fabsf(speedF) <= 0.05f)) {
        *i_mode = 1;
        speedF = 0.0f;
        m_padStickValue = 0.0f;
    } else if (((*i_mode == 2 || *i_mode == 10) && dist_xz2 < temp_f28) || dist_xz2 < temp_f30) {
        if (*i_mode == 10) {
            *i_mode = 10;
        } else {
            *i_mode = 2;
        }
    }

    if ((0.5f * (speedF * speedF)) / m_hio->m.stopping_deceleration > JMAFastSqrt(dist_xz2)) {
        m_padStickValue = 0.0f;
    }
    s16 s = pos_vec.atan2sX_Z();
    m_demoMoveAngle = s;
}

void daHorse_c::setDemoData() {
    if (!dComIfGp_event_runCheck()) {
        offStateFlg0(FLG0_UNK_2000000);
    }

    if (dComIfGp_getEvent().isOrderOK()) {
        u32 unused;
        if (checkHorseDemoMode()) {
            u32 old_demoMode = m_demoMode;
            field_0x16b8 = 0;
            m_demoMode = 0;
            field_0x1728 = 0;
            m_demoStaffId = -1;
            m_demoStickR = 0.0f;

            offStateFlg0(FLG0_UNK_80000);

            if (checkStateFlg0(FLG0_UNK_400)) {
                offStateFlg0(FLG0_UNK_400);
                speedF = field_0x1794;
            }

            if (old_demoMode != 12 && old_demoMode != 15 && old_demoMode != 13) {
                if (fabsf(speedF) < 0.05f) {
                    procWaitInit();
                } else {
                    procMoveInit();
                }
            }
        }
    } else {
        m_lashAccelerationTime = 0;
        dDemo_actor_c* demo_actor_p = dDemo_c::getActor(demoActorID);
        cXyz* pos_p = NULL;
        u32 mode = 1;
        s16 angle;
        int* prm_p = NULL;
        m_demoStaffId = dComIfGp_evmng_getMyStaffId("Horse", this, 0);

        if (demo_actor_p != NULL) {
            demo_actor_p->setModel(m_model);
            m_demoStaffId = -1;

            if (field_0x16b8 != 1) {
                field_0x16b8 = 1;
                procWaitInit();
            }

            if (demo_actor_p->checkEnable(dDemo_actor_c::ENABLE_ANM_e)) {
                mode = demo_actor_p->getAnmId();
            }

            if (demo_actor_p->checkEnable(dDemo_actor_c::ENABLE_TRANS_e)) {
                pos_p = &demo_actor_p->getTrans();
            } else {
                pos_p = &current.pos;
            }

            if (demo_actor_p->checkEnable(dDemo_actor_c::ENABLE_ROTATE_e)) {
                angle = demo_actor_p->getRatate().y;
            } else {
                angle = shape_angle.y;
            }

            if (demo_actor_p->checkEnable(1)) {
                int sp20, sp1C, sp18;
                u16 sp8;
                while (demo_actor_p->getDemoIDData(&sp20, &sp1C, &sp18, &sp8, NULL) != 0) {
                    if (sp20 == 0 && sp1C == 0) {
                        if (sp18 == 2) {
                            if (sp8 != 0) {
                                onEndResetStateFlg0(ERFLG0_UNK_40);
                            }
                        } else if (sp18 == 3) {
                            if (sp8 != 0) {
                                onResetStateFlg0(RFLG0_UNK_1);
                            }
                        } else if (sp18 == 4) {
                            if (sp8 != 0) {
                                onResetStateFlg0(RFLG0_UNK_80);
                            }
                        } else if (sp18 == 5) {
                            if (sp8 != 0) {
                                onResetStateFlg0(RFLG0_UNK_100);
                            }
                        } else if (sp18 == 6 && sp8 != 0) {
                            onResetStateFlg0(RFLG0_UNK_200);
                        }
                    }
                }
            }
        } else {
            if (!checkHorseDemoMode()) {
                field_0x16b8 = 2;
            }

            if (m_demoStaffId != -1) {
                char* cutname = dComIfGp_getPEvtManager()->getMyNowCutName(m_demoStaffId);
                if (cutname != NULL) {
                    mode = ((cutname[0] - '0') * 100) + ((cutname[1] - '0') * 10) + (cutname[2] - '0');

                    pos_p = dComIfGp_evmng_getMyXyzP(m_demoStaffId, "pos");
                    if (pos_p == NULL) {
                        pos_p = dComIfGp_evmng_getGoal();
                    }

                    int* angle_p = dComIfGp_evmng_getMyIntegerP(m_demoStaffId, "angle");
                    if (angle_p != NULL) {
                        angle = *angle_p;
                    } else {
                        angle = shape_angle.y;
                    }

                    prm_p = dComIfGp_evmng_getMyIntegerP(m_demoStaffId, "prm0");

                    if (dComIfGp_evmng_getMyIntegerP(m_demoStaffId, "ride") != NULL && !checkStateFlg0(FLG0_UNK_1)) {
                        mode = 1;
                    }
                }
            }
        }

        if (field_0x16b8 == 1 || (field_0x16b8 == 2 && m_demoStaffId != -1)) {
            if (prm_p != NULL) {
                field_0x1728 = *prm_p;
            } else {
                field_0x1728 = 0;
            }

            if (mode == 5) {
                speedF = 0.0f;
                m_cc_stts.ClrCcMove();
                setHorsePosAndAngle(pos_p, angle);
                m_demoMoveAngle = angle;
            } else if (mode == 2 || mode == 10 || mode == 3 || mode == 11 || mode == 4) {
                setDemoMoveData(&mode, pos_p);
            } else if (field_0x16b8 == 2 && mode == 7) {
                f32* dis_p = dComIfGp_evmng_getMyFloatP(m_demoStaffId, "disXZ");
                if (dis_p != NULL) {
                    field_0x1768 = *dis_p;
                    field_0x1770 = field_0x1768;
                }

                dis_p = dComIfGp_evmng_getMyFloatP(m_demoStaffId, "disY");
                if (dis_p != NULL) {
                    field_0x176c = *dis_p;
                }
            }

            m_demoMode = mode;
            if (m_demoMode == 1) {
                speedF = 0.0f;
            }
        } else if (field_0x16b8 == 3) {
            if (m_demoMode == 14 && !checkStateFlg0(FLG0_UNK_400)) {
                if (checkStateFlg0(FLG0_UNK_2)) {
                    onStateFlg0(FLG0_UNK_400);
                    field_0x1794 = speedF;
                } else {
                    m_demoMode = 1;
                }

                speedF = 0.0f;
            }

            if (m_demoMode == 2 || m_demoMode == 10 || m_demoMode == 3 || m_demoMode == 11 || m_demoMode == 4) {
                setDemoMoveData(&m_demoMode, &m_demoPos0);
            }

            if (m_demoMode == 5) {
                speedF = 0.0f;
                m_cc_stts.ClrCcMove();
            } else if (m_demoMode == 1) {
                speedF = 0.0f;
            }
        } else if (checkStateFlg0(FLG0_RODEO_MODE)) {
            m_demoMode = 13;
        } else {
            if ((m_demoMode != 0 || mode != 1) && !checkStateFlg0(FLG0_UNK_10000000) && (m_procID != PROC_STOP_e || field_0x171c == 0)) {
                speedF = 0.0f;
            }

            m_demoMode = mode;
        }

        if (m_demoStaffId != -1 && (m_demoMode == 1 || m_demoMode == 6 || m_demoMode == 5)) {
            dComIfGp_evmng_cutEnd(m_demoStaffId);
        }
    }
}

void daHorse_c::acceptPlayerRide() {
    if (!checkStateFlg0(FLG0_UNK_1) && !daPy_py_c::checkNowWolf()) {
        int angle = fopAcM_seenPlayerAngleY(this);
        if (angle > 0x2800 && (!daAlink_getAlinkActorClass()->checkHorseZelda() || angle < 0x5800)) {
            attention_info.flags |= fopAc_AttnFlag_ETC_e;
        }
    }
}

void daHorse_c::setStickData() {
    s16 stick_angle;
    if (checkStateFlg0(daHorse_FLG0(FLG0_RODEO_MODE | FLG0_UNK_10000000))) {
        field_0x16c2 = 2000;
    } else {
        field_0x16c2 = m_hio->m.max_turn;
    }

    if (dComIfGp_event_runCheck()) {
        if (field_0x16b8 == 1 || field_0x16b8 == 3 || (field_0x16b8 == 2 && m_demoStaffId != -1)) {
            if (m_demoMode == 2 || m_demoMode == 10 || m_demoMode == 4 || m_demoMode == 11 || m_demoMode == 3) {
                m_padStickValue = 1.0f;
            } else if (m_demoMode == 6) {
                m_padStickValue = m_demoStickR;
            } else {
                m_padStickValue = 0.0f;
            }

            m_padStickAngleY = m_demoMoveAngle;
        } else if (checkStateFlg0(FLG0_RODEO_MODE)) {
            setStickRodeoMove();
        } else if (checkStateFlg0(FLG0_UNK_10000000)) {
            setStickCallMove();
        } else {
            m_padStickValue = 0.0f;
            m_padStickAngleY = 0;
        }
    } else if (checkStateFlg0(FLG0_UNK_10000000)) {
        setStickCallMove();
    } else if (checkStateFlg0(FLG0_RODEO_MODE)) {
        if (!checkStateFlg0(FLG0_UNK_400000)) {
            onStateFlg0(FLG0_UNK_400000);
            mDoAud_subBgmStart(Z2BGM_RODEO);
        }
        setStickRodeoMove();
    } else {
        if (checkStateFlg0(FLG0_UNK_1) && !daAlink_getAlinkActorClass()->checkHorseGetOffMode()) {
            if (!daAlink_getAlinkActorClass()->checkHorseHangMode()) {
                if (!checkStateFlg0(FLG0_PLAYER_BACK_RIDE_LASH)) {
                    m_padStickValue = mDoCPd_c::getStickValue(PAD_1);

                    stick_angle = mDoCPd_c::getStickAngle3D(PAD_1);
                    m_padStickAngleY = (dCam_getControledAngleY(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0))) + 0x10000 + stick_angle) - 0x8000;
                    return;
                }
            }
        }
    
        if (checkEndResetStateFlg0(ERFLG0_RIDE_RUN_FLG)) {
            m_padStickValue = 1.0f;
            m_padStickAngleY = shape_angle.y;
        } else {
            m_padStickValue = 0.0f;
            m_padStickAngleY = 0;
        }
    }
}

void daHorse_c::setTurnStartVibration() {}

void daHorse_c::setTurnVibration() {}

void daHorse_c::setMoveAnime(f32 i_morf) {
    daPy_frameCtrl_c* frame_ctrl = &m_frameCtrl[0];

    f32 run_anm_speed, fastrun_anm_speed;
    if (checkStateFlg0(FLG0_UNK_2000)) {
        run_anm_speed = m_hio->m.kakariko_run_anm_speed;
        fastrun_anm_speed = m_hio->m.kakariko_fastrun_anm_speed;
    } else {
        run_anm_speed = m_hio->m.run_anm_speed;
        fastrun_anm_speed = m_hio->m.fast_run_anm_speed;
    }

    if (m_anmIdx[0] == ANM_HS_RUN_DASH && !m_frameCtrl[0].checkAnmEnd()) {
        field_0x16b7 = 0;
        if (frame_ctrl->checkPass(10) || frame_ctrl->checkPass(34)) {
            field_0x16b6 |= (u8)4;
        }

        if (frame_ctrl->checkPass(12) || frame_ctrl->checkPass(35)) {
            field_0x16b6 |= (u8)8;
        } else if (frame_ctrl->checkPass(14)) {
            field_0x16b6 |= (u8)1;
        } else if (frame_ctrl->checkPass(19)) {
            field_0x16b6 |= (u8)2;
        }
        return;
    }

    f32 var_f31;
    f32 ratio;
    if (speedF >= 0.0f) {
        var_f31 = speedF * (1.0f / m_normalMaxSpeedF);
    } else {
        var_f31 = speedF * (1.0f / m_hio->m.max_backward_speed);
    }

    if (var_f31 < m_hio->m.walk_to_fastwalk_rate) {
        if (checkStateFlg0(FLG0_UNK_2)) {
            offStateFlg0(FLG0_UNK_2);
            i_morf = m_hio->m.walk_run_interpolation;
        }

        if (var_f31 < 0.0f && m_anmIdx[0] == ANM_HS_WALK_START && !m_frameCtrl[0].checkAnmEnd()) {
            return;
        }
        
        if (var_f31 < -m_hio->m.backward_idle_to_walk_rate) {
            setDoubleAnime(1.0f, m_hio->m.backwalk_anm_speed, m_hio->m.backwalk_anm_speed, ANM_HS_BACK_WALK, ANM_HS_BACK_WALK, i_morf);
        } else if (var_f31 < 0.0f) {
            ratio = 1.0f - ((var_f31 + m_hio->m.backward_idle_to_walk_rate) / m_hio->m.backward_idle_to_walk_rate);
            setDoubleAnime(ratio, m_hio->m.wait_anm_speed, m_hio->m.backwalk_anm_speed, ANM_HS_WAIT_01, ANM_HS_BACK_WALK, i_morf);
        } else if (var_f31 < m_hio->m.wait_to_walk_rate) {
            int sp8 = cLib_distanceAngleS(m_padStickAngleY, shape_angle.y);
            if (checkWaitTurn() && checkInputOnR() && sp8 >= 0x800 && sp8 <= 0x6000) {
                f32 anm_speed = 1.0f;
                u16 anm_idx;
                resetNeckAnime();

                if ((s16)(shape_angle.y - field_0x16e8) > 0) {
                    if (m_anmIdx[0] == ANM_HS_TURN_LEFT_START) {
                        if (m_frameCtrl[0].checkAnmEnd()) {
                            anm_idx = ANM_HS_TURN_LEFT;
                        } else {
                            setTurnStartVibration();
                            return;
                        }
                    } else if (m_anmIdx[0] == ANM_HS_TURN_LEFT) {
                        anm_idx = ANM_HS_TURN_LEFT;
                        anm_speed = 0.5f + ((0.5f / (f32)field_0x16c2) * (f32)abs((s16)(shape_angle.y - field_0x16e8)));
                        if (anm_speed > 1.0f) {
                            anm_speed = 1.0f;
                        }
                        setTurnVibration();
                    } else {
                        m_anmRatio[0].getAnmTransform()->setFrame(0);
                        anm_idx = ANM_HS_TURN_LEFT_START;
                    }
                } else if (m_anmIdx[0] == ANM_HS_TURN_RIGHT_START) {
                    if (m_frameCtrl[0].checkAnmEnd()) {
                        anm_idx = ANM_HS_TURN_RIGHT;
                    } else {
                        setTurnStartVibration();
                        return;
                    }
                } else {
                    if (m_anmIdx[0] == ANM_HS_TURN_RIGHT) {
                        anm_idx = ANM_HS_TURN_RIGHT;
                        anm_speed = 0.5f + ((0.5f / (f32)field_0x16c2) * (f32)abs((s16)(shape_angle.y - field_0x16e8)));
                        if (anm_speed > 1.0f) {
                            anm_speed = 1.0f;
                        }
                        setTurnVibration();
                    } else {
                        m_anmRatio[0].getAnmTransform()->setFrame(0);
                        anm_idx = ANM_HS_TURN_RIGHT_START;
                    }
                }

                setDoubleAnime(0.0f, anm_speed, anm_speed, anm_idx, anm_idx, i_morf);
            } else {
                ratio = var_f31 / m_hio->m.wait_to_walk_rate;
                if (m_procID != PROC_WAIT_e && speedF > 0.05f) {
                    ratio = m_hio->m.field_0xdc + (ratio * (1.0f - m_hio->m.field_0xdc));
                }
                setDoubleAnime(ratio, m_hio->m.wait_anm_speed, m_hio->m.walk_anm_speed, ANM_HS_WAIT_01, ANM_HS_WALK_SLOW, i_morf);
            }
        } else {
            ratio = (var_f31 - m_hio->m.wait_to_walk_rate) / (m_hio->m.walk_to_fastwalk_rate - m_hio->m.wait_to_walk_rate);
            setDoubleAnime(ratio, m_hio->m.walk_anm_speed, m_hio->m.fast_walk_anm_speed, ANM_HS_WALK_SLOW, ANM_HS_WALK_FAST, i_morf);
        }      
    } else if (var_f31 < m_hio->m.fastwalk_to_run_rate) {
        if (checkStateFlg0(FLG0_UNK_2) == 0) {
            setDoubleAnime(1.0f, m_hio->m.fast_walk_anm_speed, m_hio->m.fast_walk_anm_speed, ANM_HS_WALK_FAST, ANM_HS_WALK_FAST, i_morf);
        } else {
            setDoubleAnime(0.0f, run_anm_speed, run_anm_speed, ANM_HS_RUN_SLOW, ANM_HS_RUN_SLOW, i_morf);
        }
    } else {
        if (!checkStateFlg0(FLG0_UNK_2)) {
            onStateFlg0(FLG0_UNK_2);
            if (m_lashAccelerationTime == 0) {
                onResetStateFlg0(RFLG0_UNK_4);
                setSingleAnime(ANM_HS_RUN_DASH, m_hio->m.dash_anm_speed, 0.0f, -1, 3.0f, 0);
            }
            return;
        }

        if (var_f31 < m_hio->m.run_to_fastrun_rate) {
            ratio = 0.1f * ((var_f31 - m_hio->m.fastwalk_to_run_rate) / (m_hio->m.run_to_fastrun_rate - m_hio->m.fastwalk_to_run_rate));
        } else {
            ratio = 0.1f + ((var_f31 - m_hio->m.run_to_fastrun_rate) / ((1.0f + (m_lashAddSpeed / m_normalMaxSpeedF)) - m_hio->m.run_to_fastrun_rate));
            if (ratio > 1.0f) {
                ratio = 1.0f;
            }
        }

        setDoubleAnime(ratio, run_anm_speed, fastrun_anm_speed, ANM_HS_RUN_SLOW, ANM_HS_RUN_FAST, i_morf);
    }

    if (checkStateFlg0(FLG0_UNK_2)) {
        onResetStateFlg0(RFLG0_UNK_20);

        if (m_lashAccelerationTime != 0 || (checkStateFlg0(FLG0_RODEO_MODE) && checkStateFlg0(FLG0_UNK_1))) {
            m_at_cyl.SetAtSpl(dCcG_At_Spl_UNK_1);
            m_at_cyl.SetAtHitMark(3);
            m_at_cyl.SetAtAtp(2);
        } else {
            m_at_cyl.SetAtSpl(dCcG_At_Spl_UNK_0);
            m_at_cyl.SetAtHitMark(1);
            m_at_cyl.SetAtAtp(1);
        }
    }

    if (checkStateFlg0(FLG0_UNK_2)) {
        field_0x16b7 = 0;

        for (int i = 0; i < 4; i++) {
            static f32 const footEffectRate[4] = {
                0.27f, 0.4f, 0.9f, 0.17f
            };

            if (frame_ctrl->checkPass(footEffectRate[i] * frame_ctrl->getEnd())) {
#if PLATFORM_GCN || PLATFORM_WII
                field_0x16b6 |= (1 << i);
#else
                field_0x16b6 |= (u8)(1 << i);
#endif
            }
        }
    }
}

int daHorse_c::checkHorseNoMove(int param_0) {
    cXyz sp50;
    cXyz sp44;

    if (checkStateFlg0(FLG0_UNK_2000000)) {
        return 0;
    }

    if (checkPlayerHeavy()) {
        return 3;
    }

    s16 spA;
    if (daAlink_c::getMoveBGActorName(m_acch.m_gnd, 0) == PROC_OBJ_BRG) {
        spA = 0;
    } else if (m_procID == PROC_TURN_e && dComIfG_Bgsp().ChkPolySafe(m_acch.m_gnd)) {
        spA = fopAcM_getPolygonAngle(m_acch.m_gnd, shape_angle.y);
    } else {
        spA = field_0x1714;
    }

    f32 var_f29;
    f32 var_f28;
    f32 sp28;
    f32 var_f27;
    f32 var_f31;
    f32 var_f30;
    
    if (param_0 != 0) {
        if (checkEndResetStateFlg0(ERFLG0_UNK_200) || (checkStateFlg0(FLG0_UNK_100000) && (m_procID == PROC_MOVE_e || m_procID == PROC_TURN_e || m_procID == PROC_STOP_e) && ((current.pos.z < -33500.0f && abs(shape_angle.y) > 0x4000) || (current.pos.z > -20500.0f && abs(shape_angle.y) < 0x4000)))) {
            return 3;
        }

        var_f31 = fabsf(speedF) / m_normalMaxSpeedF;
        if (var_f31 > 1.0f) {
            var_f31 = 1.0f;
        } else if (m_procID == PROC_WAIT_e || m_procID == PROC_STOP_e) {
            var_f31 = 0.5f;
        }
        var_f31 *= var_f31;

        var_f31 = 200.0f + (300.0f * var_f31);
        var_f29 = cM_ssin(current.angle.y);
        var_f28 = cM_scos(current.angle.y);
        sp28 = -var_f31 * cM_ssin(spA);
        var_f27 = var_f31 * cM_scos(spA);
    } else {
        var_f29 = -cM_ssin(current.angle.y);
        var_f28 = -cM_scos(current.angle.y);
        sp28 = 200.0f * cM_ssin(spA);
        var_f27 = 200.0f * cM_scos(spA);
    }

    daAlink_c* player = daAlink_getAlinkActorClass();
    cXyz start(current.pos.x, current.pos.y + m_acchcir[0].GetWallH(), current.pos.z);

    if (daAlink_c::getMoveBGActorName(m_acch.m_gnd, 0) == PROC_OBJ_BRG) {
        start.y = 200.0f + current.pos.y;
    }

    cXyz end(start.x + (var_f27 * var_f29), start.y + sp28, start.z + (var_f27 * var_f28));
    int rt = 0;
    m_linechk.Set(&start, &end, this);

    onStateFlg0(FLG0_UNK_100);
    // Fakematch: Debug shows that dBgS::LineCross is called here, but the instruction order on
    // retail only matches if cBgS::LineCross is called instead.
    // An alternative way to match the instruction order is by changing dBgS::LineCross's return
    // type to BOOL while keeping cBgS::LineCross's return type as bool, but that breaks other
    // functions that call dBgS::LineCross.
    BOOL line_cross = dComIfG_Bgsp().cBgS::LineCross(&m_linechk);
    offStateFlg0(FLG0_UNK_100);

    if (line_cross) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(m_linechk, &plane);
        sp50 = m_linechk.GetCross();

        if (cBgW_CheckBWall(plane.mNormal.y) && ((param_0 != 0 && cLib_distanceAngleS(plane.mNormal.atan2sX_Z(), shape_angle.y) > 0x6000) || (param_0 == 0 && cLib_distanceAngleS(plane.mNormal.atan2sX_Z(), shape_angle.y) < 0x2000))) {
            m_linechk.Set(&start, &end, this);
            if (m_procID == PROC_MOVE_e && param_0 != 0 && !dComIfG_Bgsp().LineCross(&m_linechk)) {
                if (sp50.abs2(start) < 23716.0f) {
                    return 4;
                }
                return 0;
            }

            if (start.abs(sp50) < 300.0f) {
                return 2;
            }
            rt = 1;
        }
    }

    if (m_procID == PROC_TURN_e) {
        var_f30 = 0.0f;
    } else {
        var_f30 = 50.0f;
    }

    sp50.y = 200.0f + current.pos.y;

    f32 sp18;
    if (param_0 != 0 && speedF > (0.5f * m_normalMaxSpeedF)) {
        sp18 = 500.0;
    } else {
        sp18 = 200.0f;
    }

    while (var_f30 < sp18) {
        sp50.x = current.pos.x + (var_f30 * var_f29);
        sp50.z = current.pos.z + (var_f30 * var_f28);

        if (fopAcM_gc_c::gndCheck(&sp50)) {
            cM3dGPla sp5C;
            fopAcM_gc_c::getTriPla(&sp5C);

            s16 sp8 = cM_atan2s(sp5C.mNormal.absXZ(), sp5C.mNormal.y);
            f32 sp14 = cM_deg2s(m_hio->m.floor_angle_limit);

            if (!checkStateFlg0(FLG0_UNK_1)) {
                for (int i = 0; i < m_scnChg_num; i++) {
                    if (m_scnChg_buffer[i]->checkArea(&sp50)) {
                        if (var_f30 <= 101.0f) {
                            if (param_0 != 0 && !player->checkHorseGetOffMode()) {
                                onStateFlg0(FLG0_UNK_40000);
                                field_0x1704 = 30;
                            }
                            return 2;
                        }
                        return 1;
                    }
                }
            }

            if (fopAcM_gc_c::getHorseNoEntry() ||
                (!checkStateFlg0(FLG0_UNK_1) && dComIfG_Bgsp().GetExitId(*fopAcM_gc_c::getGroundCheck()) != 0x3F) ||
                (sp8 > sp14 && daAlink_c::getMoveBGActorName(*fopAcM_gc_c::getGroundCheck(), 1) != PROC_OBJ_BRG && dComIfG_Bgsp().GetSpecialCode(*fopAcM_gc_c::getGroundCheck()) != 2))
            {
                if (var_f30 <= 101.0f) {
                    if (sp8 > sp14 && param_0 != 0) {
                        onStateFlg0(FLG0_UNK_40000);
                        field_0x1704 = 30;
                    }
                    return 2;
                }
                return 1;
            }

            if (fopAcM_wt_c::waterCheck(&sp50)) {
                if (fopAcM_wt_c::getWaterY() - fopAcM_gc_c::getGroundY() > m_hio->m.water_depth_limit) {
                    if (var_f30 <= 101.0f) {
                        if (param_0 != 0) {
                            onStateFlg0(FLG0_UNK_40000);
                            field_0x1704 = 30;
                        }
                        return 2;
                    }
                    return 1;
                }
            }
        }

        if (daTagHstop_c::getTop() != NULL) {
            for (daTagHstop_c* hstop = daTagHstop_c::getTop(); hstop != NULL; hstop = hstop->getNext()) {
                if (hstop->getActiveFlg()) {
                    fpoAcM_relativePos(hstop, &sp50, &sp44);

                    if (sp44.y >= -200.0f && sp44.y <= (400.0f + hstop->scale.y) && fabsf(sp44.x) <= hstop->scale.x && fabsf(sp44.z) <= hstop->scale.z) {
                        if (param_0 != 0 && speedF > 0.0f && m_procID == PROC_MOVE_e) {
                            onStateFlg0(FLG0_UNK_200);
                            hstop->onTagStop();
                        }

                        if (var_f30 <= 101.0f) {
                            return 2;
                        }
                        return 1;
                    }
                }
            }
        }

        var_f30 += 25.0f;
    }

    return rt;    
}

BOOL daHorse_c::checkTurnPlayerState() {
    daAlink_c* player = daAlink_getAlinkActorClass();
    return player->checkHorseBackInput() && !player->checkHorseWaitLashAnime() && (!player->checkHorseWalkStartAnm() || player->getBaseAnimeFrame() < 1.0f);
}

int daHorse_c::setSpeedAndAngle() {
    int var_r28 = checkHorseNoMove(1);
    if (var_r28 == 3) {
        if (checkStateFlg0(FLG0_UNK_2)) {
            return 3;
        }
        return 1;
    }

    if (var_r28 == 4) {
        return 5;
    }

    if (var_r28 == 2 && m_procID == PROC_MOVE_e) {
        if (speedF >= 0.0f) {
            if (speedF <= m_hio->m.cliff_rise_rate) {
                onStateFlg0(FLG0_UNK_40000000);
            }
            return 4;
        }
    }

    daAlink_c* player = daAlink_getAlinkActorClass();

    if (player->checkHorseSubjectivity()
#if PLATFORM_GCN
        && checkStateFlg0(FLG0_UNK_1)
#endif
    )
    {
        if (speedF < 0.0f) {
            var_r28 = checkHorseNoMove(0);
            if (var_r28 == 2) {
                speedF = 0.0f;
            } else if (var_r28 == 1) {
                cLib_chaseF(&speedF, 0.0f, m_hio->m.stopping_deceleration);
            }
        } else if (m_lashAccelerationTime == 0 && speedF > m_normalMaxSpeedF) {
            cLib_chaseF(&speedF, m_normalMaxSpeedF, m_hio->m.stopping_deceleration);
        }

        if (checkStateFlg0(FLG0_UNK_200000)) {
            if (abs(current.angle.y) < 0x4000) {
                cLib_addCalcAngleS(&current.angle.y, 0, 2, 0x2000, 0x100);
            } else {
                cLib_addCalcAngleS(&current.angle.y, -0x8000, 2, 0x2000, 0x100);
            }
            cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 2, 0x2000, 0x100);
        }

        return 0;
    }

    s16 spA;
    if (checkInputOnR()) {
        spA = m_padStickAngleY - shape_angle.y;
    } else {
        spA = 0;
    }

    int var_r27;
    if (abs(spA) > 0x6000 && !checkStateFlg0(FLG0_UNK_10000000)) {
        var_r27 = 1;
    } else if (spA >= 0x3000) {
        var_r27 = 2;
    } else if (spA <= -0x3000) {
        var_r27 = 3;
    } else {
        var_r27 = 0;
    }

    if (checkInputOnR()) {
        if (checkStateFlg0(FLG0_UNK_40000) && (m_anmIdx[2] != ANM_HS_WAIT_03 || m_frameCtrl[2].getFrame() >= 18.0f)) {
            if (var_r27 == 1) {
                cLib_addCalcAngleS(&current.angle.y, m_padStickAngleY + 0x8000, 5, field_0x16c2, m_hio->m.min_turn);
            } else {
                cLib_addCalcAngleS(&current.angle.y, m_padStickAngleY, 5, field_0x16c2, m_hio->m.min_turn);
            }

            if (!checkStateFlg0(daHorse_FLG0(FLG0_UNK_100000 | FLG0_UNK_200000))) {
                shape_angle.y = current.angle.y;
            }
        } else if (var_r27 != 1) {
            if (checkStateFlg0(FLG0_UNK_10000000) || (!player->checkHorseWalkStartAnm() && (!player->checkHorseWaitLashAnime() || checkEndResetStateFlg0(ERFLG0_MOVE_ACCEPT)))) {
                if (speedF <= 0.05f) {
                    f32 var_f28 = m_padStickValue;
                    if (var_f28 < 0.5f) {
                        var_f28 = 0.5f;
                    }

                    if (!checkStateFlg0(daHorse_FLG0(FLG0_UNK_100000 | FLG0_UNK_200000))) {
                        cLib_addCalcAngleS(&current.angle.y, m_padStickAngleY, 5, field_0x16c2 * var_f28, m_hio->m.min_turn * var_f28);
                    }
                } else {
                    cLib_addCalcAngleS(&current.angle.y, m_padStickAngleY, 5, field_0x16c2, m_hio->m.min_turn);
                }

                if (!checkStateFlg0(daHorse_FLG0(FLG0_UNK_100000 | FLG0_UNK_200000))) {
                    shape_angle.y = current.angle.y;
                }
            }
        } else {
            if (checkStateFlg0(FLG0_UNK_2) && player->checkHorseBackInput()) {
                return 3;
            }

            cLib_addCalcAngleS(&current.angle.y, (m_padStickAngleY + 0x8000), 5, field_0x16c2, m_hio->m.min_turn);

            if (!checkStateFlg0(daHorse_FLG0(FLG0_UNK_100000 | FLG0_UNK_200000))) {
                shape_angle.y = current.angle.y;
            }
        }
    }

    if (m_demoMode == 6) {
        if (var_r28 != 0 && speedF > 0.0f) {
            cLib_chaseF(&speedF, 0.0f, m_hio->m.stopping_deceleration);
        }
        return 0;
    }

    if (checkStateFlg0(FLG0_UNK_1)) {
        dAttention_c* attention = dComIfGp_getAttention();
        if (attention->GetLockonList(0) != NULL && attention->LockonTruth() && fopAcM_searchActorDistanceXZ2(this, attention->GetLockonList(0)->getActor()) > 1000000.0f) {
            cLib_addCalcAngleS(&current.angle.y, fopAcM_searchActorAngleY(this, attention->GetLockonList(0)->getActor()), 5, field_0x16c2, m_hio->m.min_turn);
            if (!checkStateFlg0(daHorse_FLG0(FLG0_UNK_100000 | FLG0_UNK_200000))) {
                shape_angle.y = current.angle.y;
            }
        }
    }

    if (field_0x1702 != 0) {
        shape_angle.y += field_0x1702;
        current.angle.y = shape_angle.y;
    }

    if (checkStateFlg0(daHorse_FLG0(FLG0_UNK_100000 | FLG0_UNK_200000))) {
        fopAc_ac_c* boar_p = NULL;
        fopAcIt_Executor((fopAcIt_ExecutorFunc)daHorse_searchSingleBoar, &boar_p);

        if (checkInputOnR() && boar_p != NULL && fopAcM_seenActorAngleY(this, boar_p) < 0x4000 && boar_p->current.pos.abs2XZ(current.pos) < 2250000.0f) {
            s16 sp8;
            if (abs((s16)(shape_angle.y - current.angle.y)) > 0x5000) {
                sp8 = field_0x170a + ((s16)((current.angle.y + 0x8000) - field_0x170a) / 4);
            } else {
                sp8 = field_0x170a + ((s16)(current.angle.y - field_0x170a) / 4);
            }

            cLib_addCalcAngleS(&shape_angle.y, sp8, 5, 0x2000, 0x100);
        } else {
            if (current.angle.y < 0x4000 && current.angle.y > 0x1800) {
                current.angle.y = 0x1800;
            } else if (current.angle.y > -0x4000 && current.angle.y < -0x1800) {
                current.angle.y = -0x1800;
            } else if (current.angle.y < 0x6800 && current.angle.y >= 0x4000) {
                current.angle.y = 0x6800;
            } else if (current.angle.y > -0x6800 && current.angle.y <= -0x4000) {
                current.angle.y = -0x6800;
            }

            cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 5, 0x2000, 0x100);
        }
    }

    cLib_chaseAngleS(&field_0x1702, 0, 0xC0);

    f32 var_f30 = cM_scos(spA);
    if (checkStateFlg0(FLG0_UNK_2)) {
        if (var_f30 < 0.7f) {
            var_f30 = 0.7f;
        }
    } else if (speedF < 0.05f && var_r27 != 0) {
        var_f30 = 0.0f;
    } else if (var_f30 < 0.0f) {
        var_f30 = 0.0f;
    }

    if (var_r28 != 0 && speedF > 0.0f) {
        cLib_chaseF(&speedF, 0.0f, m_hio->m.stopping_deceleration);
    } else if (checkStateFlg0(FLG0_UNK_40000)) {
        if (m_anmIdx[2] != ANM_HS_WAIT_03 || m_frameCtrl[2].getFrame() >= 18.0f) {
            cLib_chaseF(&speedF, -m_hio->m.max_backward_speed, m_hio->m.max_backward_acceleration);

            field_0x1704--;
            if (field_0x1704 == 0) {
                offStateFlg0(FLG0_UNK_40000);
            } else if (checkHorseNoMove(0) == 2) {
                offStateFlg0(FLG0_UNK_40000);
                field_0x1704 = 0;
                speedF = 0.0f;
            }
        }
    } else if (checkInputOnR() && m_lashAccelerationTime == 0 && var_r27 == 1) {
        if (speedF > 0.0f) {
            cLib_chaseF(&speedF, 0.0f, m_hio->m.stopping_deceleration);
        } else if (!checkStateFlg0(FLG0_UNK_8) && (checkTurnInput() || checkStateFlg0(daHorse_FLG0(FLG0_UNK_100000 | FLG0_UNK_200000))) && checkTurnPlayerState() && speedF > (0.2f * -m_hio->m.max_backward_speed)) {
            return 1;
        } else {
            int spC = checkHorseNoMove(0);
            if (spC == 0) {
                if (!checkStateFlg0(daHorse_FLG0(FLG0_UNK_100000 | FLG0_UNK_200000))) {
                    cLib_chaseF(&speedF, -m_hio->m.max_backward_speed, m_hio->m.max_backward_acceleration);
                }
            } else if (spC == 1) {
                cLib_chaseF(&speedF, 0.0f, m_hio->m.stopping_deceleration);
            } else {
                speedF = 0.0f;
            }
        }
    } else if (m_cowHit != 0) {
        if (!checkStateFlg0(daHorse_FLG0(FLG0_UNK_100000 | FLG0_UNK_200000))) {
            cLib_chaseF(&speedF, 0.5f * m_normalMaxSpeedF, m_hio->m.lash_acceleration);
        }
    } else {
        f32 var_f31;
        f32 var_f29;
        if (var_r28 != 0) {
            var_f31 = 0.0f;
            var_f29 = m_hio->m.deceleration;
        } else if (m_lashAccelerationTime != 0 || (checkStateFlg0(FLG0_RODEO_MODE) && checkStateFlg0(FLG0_UNK_1)) || m_demoMode == 4) {
            var_f31 = m_lashMaxSpeedF;
            var_f29 = m_hio->m.lash_acceleration;
            if (speedF < m_hio->m.min_lash_speed) {
                speedF = m_hio->m.min_lash_speed;
            }
        } else {
            if (player->checkHorseHangMode()) {
                var_f31 = speedF;
                if (m_lashAccelerationTime == 0 && speedF > m_normalMaxSpeedF) {
                    var_f31 = m_normalMaxSpeedF;
                }
            } else {
                var_f31 = var_f30 * (m_normalMaxSpeedF * (m_padStickValue * m_padStickValue));
            }

            var_f29 = m_hio->m.acceleration;
            if (checkStateFlg0(FLG0_UNK_4) && abs(field_0x16ea) < 0x4000) {
                var_f31 *= 1.0f - (0.8f * cM_scos(field_0x16ea));
            }
        }

        if (dComIfG_Bgsp().ChkPolySafe(m_acch.m_gnd) && dComIfG_Bgsp().GetGroundCode(m_acch.m_gnd) == 11 && var_f31 > (m_normalMaxSpeedF + (0.5f * m_lashAddSpeed))) {
            var_f31 = m_normalMaxSpeedF + (0.5f * m_lashAddSpeed);
        }
    
        if (var_f31 > fabsf(speedF)) {
            cLib_chaseF(&speedF, var_f31, var_f29);
        } else if (checkStateFlg0(FLG0_UNK_4) || (!dComIfGp_event_runCheck() && !daAlink_getAlinkActorClass()->checkHorseRide() && !checkStateFlg0(daHorse_FLG0(FLG0_RODEO_MODE | FLG0_UNK_10000000)) && m_procID == PROC_MOVE_e)) {
            if (checkStateFlg0(FLG0_UNK_2) && !checkStateFlg0(FLG0_UNK_4)) {
                return 3;
            }
    
            cLib_chaseF(&speedF, var_f31, m_hio->m.stopping_deceleration);
        } else {
            if (dComIfGp_event_runCheck() || checkStateFlg0(FLG0_UNK_10000000)) {
                var_f29 = m_hio->m.stopping_deceleration;
            } else {
                var_f29 = m_hio->m.deceleration;
            }
    
            cLib_chaseF(&speedF, var_f31, var_f29);
        }
    }

    if (checkInputOnR() && m_procID == PROC_MOVE_e && var_r28 == 0 && var_r27 == 0 && speedF > 0.05f && speedF < m_hio->m.walk_min_speed) {
        speedF = m_hio->m.walk_min_speed;
    }

    return 0;
}

BOOL daHorse_c::checkWaitTurn() const {
    return m_procID == PROC_WAIT_e && shape_angle.y != field_0x16e8;
}

void daHorse_c::setRoomInfo(int param_0) {
    int room_no;
    if (-G_CM3D_F_INF != m_acch.GetGroundH() && (checkStateFlg0(FLG0_UNK_1) || m_procID == PROC_LARGE_DAMAGE_e || m_procID == PROC_JUMP_e || current.pos.y - m_acch.GetGroundH() < 500.0f)) {
        room_no = dComIfG_Bgsp().GetRoomId(m_acch.m_gnd);
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(m_acch.m_gnd);

        if (m_acch.ChkWaterHit() && m_acch.m_wtr.GetHeight() > current.pos.y) {
            m_poly_sound = dKy_pol_sound_get(&m_acch.m_wtr);
        } else if (m_acch.ChkGroundHit()) {
            m_poly_sound = dKy_pol_sound_get(&m_acch.m_gnd);
        } else {
            m_poly_sound = 0;
        }

        if (m_acch.ChkGroundHit() && m_procID != PROC_LARGE_DAMAGE_e && m_procID != PROC_JUMP_e) {
            savePos();
        }

        onStateFlg0(FLG0_UNK_4000);
    } else {
        room_no = fopAcM_GetRoomNo(this);
        m_poly_sound = 0;
        savePos();
        offStateFlg0(FLG0_UNK_4000);
    }

    if (param_0 != 0 || room_no != fopAcM_GetRoomNo(this)) {
        tevStr.room_no = room_no;
        if (tevStr.room_no == -1) {
            tevStr.room_no = dComIfGp_roomControl_getStayNo();
        }

        m_reverb = dComIfGp_getReverb(room_no);
        m_cc_stts.SetRoomId(room_no);
        fopAcM_SetRoomNo(this, room_no);
    }
}

static cXyz l_frontFootOffset(23.5f, -20.0f, 0.0f);

static cXyz l_backFootOffset(25.5f, 12.0f, 0.0f);

void daHorse_c::setBodyPart() {
    static cXyz localEyePos(25.0f, -15.0f, 0.0f);

    mDoMtx_multVec(m_model->getAnmMtx(15), &localEyePos, &eyePos);
    mDoMtx_multVecZero(m_model->getAnmMtx(0), &m_bodyEyePos);
}

void daHorse_c::setMatrix() {
    s16 var_r29 = 0;
    s16 var_r27 = 0;
    f32 temp_f31 = m_normalMaxSpeedF * m_hio->m.fastwalk_to_run_rate;

    if (m_procID == PROC_MOVE_e && speedF > temp_f31) {
        f32 var_f30;
        if (speedF <= m_normalMaxSpeedF) {
            var_f30 = (0.3f * (speedF - temp_f31)) / (m_normalMaxSpeedF - temp_f31);
        } else {
            var_f30 = 0.5f + ((0.5f * speedF) / m_lashMaxSpeedF);
        }
        
        var_r29 = 4.0f * var_f30 * ((500.0f / (f32)field_0x16c2) * (f32)(s16)(field_0x16e8 - shape_angle.y));
        var_r27 = -var_r29;
    }

    if (checkStateFlg0(FLG0_UNK_1) && checkStateFlg0(FLG0_RODEO_MODE) && m_procID == PROC_MOVE_e) {
        if (checkStateFlg0(FLG0_RODEO_LEFT)) {
            if (var_r29 < 1500) {
                var_r29 = 1500;
            }
        } else if (var_r29 > -1500) {
            var_r29 = -1500;
        }
    }

    cLib_addCalcAngleS(&shape_angle.z, var_r29, 4, 1000, 100);

    if (var_r27 == 0) {
        cLib_addCalcAngleS(&field_0x16fa, 0, 2, 4000, 1000);
    } else {
        cLib_addCalcAngleS(&field_0x16fa, var_r27, 4, 1200, 200);
    }

    setNeckAngle();

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    m_model->setBaseTRMtx(mDoMtx_stack_c::get());
#if PLATFORM_SHIELD
    m_model->setBaseScale(scale);
#endif

    if (daPy_py_c::checkNowWolf()) {
        attention_info.position.set(current.pos.x + (140.0f * cM_ssin(shape_angle.y)), 200.0f + current.pos.y, current.pos.z + (140.0f * cM_scos(shape_angle.y)));
    } else {
        attention_info.position.set(current.pos.x, 200.0f + current.pos.y, current.pos.z);
    }    
}

void daHorse_c::setDashEffect(u32* i_emitterID) {
    camera_class* camera_p = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz* eye_p = fopCamM_GetEye_p(camera_p);

    if (eye_p->abs(current.pos) > 1200.0f) {
        return;
    }
    s16 var_r28 = shape_angle.y - fopCamM_GetAngleY(camera_p);
    f32 temp_f31 = (200.0f * std::abs(cM_ssin(var_r28))) + (700.0f * std::abs(cM_scos(var_r28)));
    f32 temp_f30 = temp_f31 * cM_scos(fopCamM_GetAngleX(camera_p));

#if PLATFORM_WII || PLATFORM_SHIELD
    Vec pos;
#else
    Vec pos = {0.0f, 0.0f, 0.0f};
#endif
    pos.x = eye_p->x + (temp_f30 * cM_ssin(fopCamM_GetAngleY(camera_p)));
    pos.y = eye_p->y + (temp_f31 * cM_ssin(-fopCamM_GetAngleX(camera_p)));
    f32 v = eye_p->z + (temp_f30 * cM_scos(fopCamM_GetAngleY(camera_p)));
    pos.z = v;

    SVec angle = {0, shape_angle.y, 0};

    *i_emitterID = dComIfGp_particle_set(*i_emitterID, 0x8657, (cXyz*)&pos, &tevStr, (csXyz*)&angle,
                                         NULL, 0xFF, NULL, -1, 0, 0, 0);
}

void daHorse_c::setEffect() {
    JPABaseEmitter* sp50;
    int sp4C;
    if (checkEndResetStateFlg0(ERFLG0_UNK_400)) {
        setDashEffect(&field_0x1738[field_0x16bd]);
        sp4C = field_0x16bd;
        field_0x16bd ^= (u8)1;
    } else {
        sp4C = -1;
    }

    f32 var_f31 = 2.0f * (speedF / m_normalMaxSpeedF);
    if (var_f31 > 1.0f) {
        var_f31 = 1.0f;
    }

    u8 sp21 = 255.0f * var_f31;

    for (int i = 0; i < 2; i++) {
        if (i != sp4C) {
            sp50 = dComIfGp_particle_getEmitter(field_0x1738[i]);
            if (sp50 != NULL && !sp50->isEnableDeleteEmitter()) {
                setDashEffect(&field_0x1738[i]);
                if (sp50->getGlobalAlpha() > sp21) {
                    sp50->setGlobalAlpha(sp21);
                }
            }
        }
    }

    u32 sp44 = 0;
    if (field_0x16b7 == 2) {
        sp44 |= 3;
    }

    u32 sp40 = 0;
    if (field_0x16b8 == 1) {
        sp40 |= 0x20000;
    }

    if (m_acch.ChkGroundHit()) {
        field_0x17dc.setEffectFour(&tevStr,
                                   &current.pos,
                                   sp44,
                                   sp40,
                                   NULL,
                                   (field_0x16b6 & 1) ? &m_footData[0].field_0xc : NULL,
                                   (field_0x16b6 & 2) ? &m_footData[1].field_0xc : NULL,
                                   (field_0x16b6 & 4) ? &m_footData[2].field_0xc : NULL,
                                   (field_0x16b6 & 8) ? &m_footData[3].field_0xc : NULL,
                                   &current.angle,
                                   NULL,
                                   fopAcM_GetRoomNo(this),
                                   1.0f,
                                   speedF);
    } else {
        field_0x17dc.setEffectFour(&tevStr,
                                   &current.pos,
                                   sp44,
                                   sp40,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   &current.angle,
                                   NULL,
                                   fopAcM_GetRoomNo(this),
                                   1.0f,
                                   speedF);
    }

    static cXyz runScale(2.0f, 2.0f, 2.0f);
    static cXyz landScale(1.5f, 1.5f, 1.5f);
    static cXyz grassRunScale(1.8f, 1.8f, 1.8f);
    static cXyz waterDirection(0.0f, 1.0f, -0.75f);
    
    int j;
    int i;
    int k;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            u8 sp20 = field_0x17dc.getTypeFour(i, j);
            if (sp20 == 0) {
                sp50 = field_0x17dc.getEmitterFour(i, j, 0);
                if (sp50 != NULL) {
                    if (field_0x16b7 == 2) {
                        sp50->setGlobalScale(landScale);
                    } else {
                        sp50->setGlobalScale(runScale);
                        if (field_0x16b7 == 0) {
                            sp50->setRate(3.0f);
                        } else {
                            sp50->setRate(0.75f);
                        }
                    }
                }
            } else if (sp20 == 1) {
                sp50 = field_0x17dc.getEmitterFour(i, j, 0);
                if (sp50 != NULL) {
                    if (field_0x16b7 == 2) {
                        sp50->setGlobalScale(landScale);
                    } else {
                        sp50->setGlobalScale(grassRunScale);
                        if (field_0x16b7 == 0) {
                            sp50->setRate(4.0f);
                        } else {
                            sp50->setRate(0.5f);
                        }
                    }
                }
            } else if (sp20 == 2) {
                for (k = 0; k < 4; k++) {
                    sp50 = field_0x17dc.getEmitterFour(i, j, k);
                    if (sp50 != NULL) {
                        if (field_0x16b7 == 2) {
                            sp50->setGlobalScale(landScale);
                        } else {
                            sp50->setGlobalScale(this->scale);
                            if (field_0x16b7 == 1) {
                                sp50->setRateStep(3);
                            }

                            if (k != 0) {
                                sp50->setDirection(waterDirection);
                            }
                        }
                    }
                }
            }
        }
    }
}

void daHorse_c::setCollision() {
    m_cc_stts.Move();
    f32 sin_y = cM_ssin(shape_angle.y);
    f32 cos_y = cM_scos(shape_angle.y);

    cXyz center(current.pos.x + (170.0f * sin_y), current.pos.y, current.pos.z + (170.0f * cos_y));
    m_boar_cyl.SetC(center);

    center.set(current.pos.x + (83.0f * sin_y), current.pos.y, current.pos.z + (83.0f * cos_y));

    sin_y *= 80.0f;
    cos_y *= 80.0f;

    for (int i = 0; i < 3; i++) {
        m_tgco_cyl[i].SetC(center);
        center.x -= sin_y;
        center.z -= cos_y;
    }

    static const Vec localHeadPos = {20.0f, 0.0f, 0.0f};
    mDoMtx_multVec(m_model->getAnmMtx(15), &localHeadPos, &center);
    m_head_sph.SetC(center);

    dComIfG_Ccsp()->Set(&m_tgco_cyl[0]);
    dComIfG_Ccsp()->Set(&m_tgco_cyl[1]);
    dComIfG_Ccsp()->Set(&m_tgco_cyl[2]);
    dComIfG_Ccsp()->SetMass(&m_tgco_cyl[0], 1);
    dComIfG_Ccsp()->SetMass(&m_tgco_cyl[2], 1);
    dComIfG_Ccsp()->Set(&m_head_sph);
    dComIfG_Ccsp()->Set(&m_boar_cyl);

    center.y = current.pos.y;
    m_at_cyl.MoveCAt(center);

    if (checkResetStateFlg0(RFLG0_UNK_20)) {
        dComIfG_Ccsp()->Set(&m_at_cyl);
        dComIfG_Ccsp()->SetMass(&m_at_cyl, 1);
    } else {
        m_at_cyl.ResetAtHit();
    }
}

void daHorse_c::autoGroundHit() {
    if (checkStateFlg0(FLG0_UNK_10) && m_procID != PROC_LARGE_DAMAGE_e && m_procID != PROC_JUMP_e) {
        f32 dist_to_ground = m_acch.GetGroundH() - current.pos.y;
        if (!m_acch.ChkGroundHit() && dist_to_ground < 0.0f && dist_to_ground >= -l_autoUpHeight) {
            current.pos.y = m_acch.GetGroundH();
            m_acch.SetGroundHit();
            speed.y = 0.0f;
        }
    }
}

void daHorse_c::setTailAngle() {
    s16* var_r30 = field_0x16d4;
    s16* var_r29 = field_0x16e0;
    s16 var_r27 = shape_angle.y - field_0x16e8;

    for (int i = 0; i < 3; i++, var_r30++, var_r29++) {
        if (checkResetStateFlg0(RFLG0_UNK_1) || m_procID == PROC_TOOL_DEMO_e) {
            *var_r30 = 0;
            *var_r29 = 0;
        } else {
            *var_r30 -= var_r27;
            s16 sp8 = *var_r30;

            cLib_addCalcAngleS(var_r30, 0, 5, 400, 35);
            *var_r30 += *var_r29;
            if (*var_r30 > 0x1000) {
                *var_r30 = 0x1000;
            } else if (*var_r30 < -0x1000) {
                *var_r30 = -0x1000;
            }

            var_r27 = *var_r30 - sp8;
            *var_r29 = 0.5f * var_r27;

            if (checkEndResetStateFlg0(ERFLG0_UNK_40)) {
                *var_r29 = 0;
            }
        }
    }
}

void daHorse_c::setNeckAngle() {
    int var_r27 = 0;
    if (eventInfo.checkCommandTalk()) {
        m_aimNeckAngleY = fopAcM_searchActorAngleY(this, daAlink_getAlinkActorClass()) - shape_angle.y;
    } else if (field_0x1702 != 0) {
        cLib_addCalcAngleS(&field_0x16f0, field_0x1702 * 3, 3, 0x400, 0x100);
        return;
    } else if (field_0x16fa != 0) {
        field_0x16f0 = field_0x16fa;
        return;
    } else if (!checkStateFlg0(FLG0_UNK_1) && !checkResetStateFlg0(RFLG0_ENEMY_SEARCH) && m_procID == PROC_WAIT_e && m_anmIdx[0] == ANM_HS_WAIT_01) {
        var_r27 = 1;

        if (current.pos.abs2XZ(daAlink_getAlinkActorClass()->current.pos) < 250000.0f) {
            daPy_py_c::setLookPos(&eyePos);

            s16 var_r26 = fopAcM_searchActorAngleY(this, daAlink_getAlinkActorClass()) - shape_angle.y;
            if (abs(var_r26) < 0x2000) {
                m_aimNeckAngleY = var_r26;
            } else {
                m_aimNeckAngleY = 0;
            }
        } else {
            m_aimNeckAngleY = 0;
        }
    } else {
        m_aimNeckAngleY = 0;
    }

    s16 var_r29;
    s16 var_r28;
    if (var_r27 != 0) {
        var_r29 = 0x100;
        var_r28 = 0x40;
    } else if (m_aimNeckAngleY == 0 || (m_aimNeckAngleY * field_0x16f0) < 0) {
        var_r29 = 0x1000;
        var_r28 = 0x100;
    } else {
        var_r29 = 0x200;
        var_r28 = 0x40;
    }

    cLib_addCalcAngleS(&field_0x16f0, m_aimNeckAngleY, 3, var_r29, var_r28);
}

void daHorse_c::copyFootMatrix() {
    daHorseFootData_c* footdata_p = m_footData;
    for (int i = 0; i < 4; i++, footdata_p++) {
        for (int j = 0; j < 4; j++) {
            cMtx_copy(m_model->getAnmMtx(j + m_footJointTable[i]), footdata_p->field_0x24[j]);
        }
    }
}

int daHorse_c::setFootMatrix() {
    daHorseFootData_c* footdata_p = m_footData;
    copyFootMatrix();

    if (!m_oldFrame->getOldFrameFlg()) {
        return 0;
    }

    footdata_p = m_footData;
    int i;
    Quaternion* quat_p;
    J3DTransformInfo* transInfo_p;
    int spC = 1;
    u16 joint_no;
    for (i = 0; i < 4; i++, footdata_p++) {
        joint_no = m_footJointTable[i];

        mDoMtx_stack_c::ZrotS(footdata_p->field_0x4[0] * spC);
        mDoMtx_stack_c::revConcat(m_model->getAnmMtx(joint_no));
        m_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
        joint_no++;

        transInfo_p = m_oldFrame->getOldFrameTransInfo(joint_no);
        quat_p = m_oldFrame->getOldFrameQuaternion(joint_no);

        mDoMtx_stack_c::transM(transInfo_p->mTranslate.x, transInfo_p->mTranslate.y, transInfo_p->mTranslate.z);
        mDoMtx_stack_c::quatM(quat_p);
        mDoMtx_stack_c::ZrotM(spC * (footdata_p->field_0x4[1] - footdata_p->field_0x4[0]));
        mDoMtx_stack_c::scaleM(transInfo_p->mScale.x, transInfo_p->mScale.y, transInfo_p->mScale.z);
        m_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
        joint_no++;
        transInfo_p++;
        quat_p++;

        mDoMtx_stack_c::transM(transInfo_p->mTranslate.x, transInfo_p->mTranslate.y, transInfo_p->mTranslate.z);
        mDoMtx_stack_c::quatM(quat_p);
        mDoMtx_stack_c::ZrotM(spC * (footdata_p->field_0x4[2] - footdata_p->field_0x4[1]));
        mDoMtx_stack_c::scaleM(transInfo_p->mScale.x, transInfo_p->mScale.y, transInfo_p->mScale.z);
        m_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
        joint_no++;
        transInfo_p++;
        quat_p++;

        mDoMtx_stack_c::transM(transInfo_p->mTranslate.x, transInfo_p->mTranslate.y, transInfo_p->mTranslate.z);
        mDoMtx_stack_c::quatM(quat_p);
        mDoMtx_stack_c::scaleM(transInfo_p->mScale.x, transInfo_p->mScale.y, transInfo_p->mScale.z);

        if (i >= 2) {
            mDoMtx_stack_c::ZrotM(spC * (footdata_p->field_0x4[3] - footdata_p->field_0x4[2]));
        } else {
            mDoMtx_stack_c::ZrotM(footdata_p->field_0x4[3]);
        }

        m_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());

        if (i == 1) {
            spC = -1;
        }
    }

    return 1;
}

int daHorse_c::setLegAngle(f32 param_0, int param_1, int param_2, s16* param_3) {
    if (fabsf(param_0) < 0.1f || ((param_1 == 0 || param_1 == 1) && param_2 == 0)) {
        return 0;
    }

    cXyz spC0[4];
    cXyz spB0;
    cXyz spA4;
    cXyz sp98;
    cXyz sp8C;
    cXyz sp80;
    cXyz sp74;
    cXyz sp68;
    cXyz sp5C;
    int i;
    for (i = 0; i < 4; i++) {
        mDoMtx_multVecZero(m_footData[param_1].field_0x24[i], &spB0);
        mDoMtx_stack_c::multVec(&spB0, &spC0[i]);
        spC0[i].x = 0.0f;
    }

    cXyz* sp1C;
    cXyz* sp18;
    cXyz* sp14;
    int sp10;
    f32 var_f31;
    f32 temp_f30;
    f32 var_f29;
    f32 temp_f28;
    f32 var_f27;
    f32 temp_f26;
    f32 temp_f25;

    if (param_1 == 0 || param_1 == 1) {
        sp10 = 0;
        var_f27 = 0.005f;
    } else {
        sp10 = 1;
        var_f27 = 0.05f;
    }
   
    for (i = 0; i < 2; i++) {
        sp1C = &spC0[i];
        sp18 = &spC0[i + 1];
        sp14 = &spC0[i + 2];

        spA4 = *sp18 - *sp1C;
        sp98 = *sp14 - *sp18;

        if (i == 0) {
            sp14->y += param_0 * var_f27;
        } else {
            sp14->y += param_0 * (1.0f - var_f27);
        }

        if (sp14->y >= sp1C->y) {
            return 0;
        }

        sp74 = *sp14 - *sp1C;

        var_f29 = sp74.abs2();
        if (cM3d_IsZero(var_f29)) {
            return 0;
        }

        temp_f26 = spA4.abs2();
        temp_f25 = sp98.abs2();
        if (JMAFastSqrt(temp_f26) + JMAFastSqrt(temp_f25) <= JMAFastSqrt(var_f29)) {
            return 0;
        }

        temp_f28 = ((var_f29 + temp_f26) - temp_f25) / (2.0f * var_f29);
        sp68.x = 0.0f;
        sp68.y = sp1C->y + (temp_f28 * sp74.y);
        sp68.z = sp1C->z + (temp_f28 * sp74.z);

        var_f31 = temp_f26 - (temp_f28 * (var_f29 * temp_f28));
        if (var_f31 < 0.0f) {
            var_f31 = 0.0f;
        }

        var_f31 = JMAFastSqrt(var_f31);
        if (param_1 == 0 || param_1 == 1) {
            if (i == 0) {
                sp5C.set(0.0f, -sp74.z, sp74.y);
            } else {
                sp5C.set(0.0f, sp74.z, -sp74.y);
            }
        } else if (i == 0) {
            sp5C.set(0.0f, sp74.z, -sp74.y);
        } else {
            sp5C.set(0.0f, -sp74.z, sp74.y);
        }

        temp_f30 = sp5C.abs();
        if (cM3d_IsZero(temp_f30)) {
            return 0;
        }

        temp_f30 = var_f31 / temp_f30;
    
        sp18->set(0.0f, sp68.y + (temp_f30 * sp5C.y), sp68.z + (temp_f30 * sp5C.z));

        sp8C = *sp18 - *sp1C;
        sp80 = *sp14 - *sp18;

        param_3[i] += (s16)(cM_atan2s(spA4.y, spA4.z) - cM_atan2s(sp8C.y, sp8C.z));
        // i don't like this, but it matches debug and release, param_3[i+1] does not match debug
        (param_3 + 1)[i] += (s16)(cM_atan2s(sp98.y, sp98.z) - cM_atan2s(sp80.y, sp80.z));

        if (i == 0) {
            spC0[3].y += param_0 * var_f27;
        }
    }

    return 1;
}

void daHorse_c::footBgCheck() {
    int i;
    daHorseFootData_c* footdata_p;
    s16 spA0[4][4];
    cXyz sp94;
    cXyz sp88;

    int sp24 = 4;
    u32 sp20;
    int j;

    MtxP sp18 = m_model->getBaseTRMtx();
    sp20 = fabsf(speedF) < 0.05f && m_procID != PROC_TURN_e;

    footdata_p = m_footData;
    mDoMtx_multVec(m_model->getAnmMtx(6), &l_frontFootOffset, &m_footData[0].field_0xc);
    mDoMtx_multVec(m_model->getAnmMtx(0xA), &l_frontFootOffset, &m_footData[1].field_0xc);
    mDoMtx_multVec(m_model->getAnmMtx(0x1E), &l_backFootOffset, &m_footData[2].field_0xc);
    mDoMtx_multVec(m_model->getAnmMtx(0x22), &l_backFootOffset, &m_footData[3].field_0xc);

    cXyz sp7C = current.pos - old.pos;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            spA0[i][j] = 0;
        }
    }

    for (i = 0; i < 4; i++, footdata_p++) {
        sp88 = footdata_p->field_0xc - footdata_p->field_0x18;
        footdata_p->field_0xc += sp7C;

        if (m_anmIdx[0] == ANM_HS_WAIT_02) {
            footdata_p->field_0xc = footdata_p->field_0x18;
        } else if (sp88.abs2XZ() < 100.0f && sp20 != 0) {
            if (footdata_p->field_0x1 != 0) {
                footdata_p->field_0x1--;
            } else {
                footdata_p->field_0xc = footdata_p->field_0x18 + sp7C;
            }
        } else {
            footdata_p->field_0x1 = 5;
        }

        footdata_p->field_0x18 = footdata_p->field_0xc;
        footdata_p->field_0xc.y += 120.0f;

        if (fopAcM_gc_c::gndCheck(&footdata_p->field_0xc) && footdata_p->field_0xc.y - fopAcM_gc_c::getGroundY() < 240.0f) {
            spA0[i][3] = fopAcM_gc_c::getGroundAngleDirection(shape_angle.y);
            if (spA0[i][3] < 0) {
                spA0[i][3] = 0;
            }
            footdata_p->field_0xc.y = fopAcM_gc_c::getGroundY();
        } else {
            footdata_p->field_0xc.y = current.pos.y;
        }
    }

    f32 temp_f31;

    if (!m_acch.ChkGroundHit() || m_procID == PROC_TOOL_DEMO_e) {
        sp24 = 4;
    } else {
        f32 var_f30 = 1000.0f + current.pos.y;

        if (checkResetStateFlg0(RFLG0_TURN_STAND)) {
            i = 2;
        } else {
            i = 0;
        }

        for (; i < 4; i++) {
            if (var_f30 > m_footData[i].field_0xc.y) {
                var_f30 = m_footData[i].field_0xc.y;
                sp24 = i;
            }
        }
    }

    int sp14;
    if (m_anmIdx[0] != ANM_HS_WAIT_02 && fabsf(m_footData[0].field_0xc.y - m_footData[1].field_0xc.y) > 30.0f) {
        sp14 = 1;
    } else {
        sp14 = 0;
    }

    if (m_procID == PROC_JUMP_e || m_procID == PROC_TOOL_DEMO_e) {
        field_0x1714 = 0;
    } else {
        cXyz sp70 = ((m_footData[0].field_0xc + m_footData[1].field_0xc) * 0.5f) - ((m_footData[2].field_0xc + m_footData[3].field_0xc) * 0.5f);
        field_0x1714 = sp70.atan2sY_XZ();
    }

    s16 spA;
    if (checkResetStateFlg0(RFLG0_TURN_STAND)) {
        spA = 0;
    } else {
        spA = field_0x1714;
        if (spA > 0x1C72) {
            spA = 0x1C72;
        } else if (spA < -0x1C72) {
            spA = -0x1C72;
        }
    }

    cLib_addCalcAngleS(&shape_angle.x, spA, 10, 2000, 182);

    if (sp24 != 4) {
        mDoMtx_stack_c::YrotS(-field_0x16e8);
        mDoMtx_stack_c::transM(-old.pos.x, -old.pos.y, -old.pos.z);
        setLegAngle(m_footData[sp24].field_0xc.y - sp18[1][3], sp24, sp14, spA0[sp24]);

        for (i = 0; i < 4; i++) {
            if (i != sp24) {
                temp_f31 = m_footData[i].field_0xc.y - sp18[1][3];
                if (temp_f31 > 0.0f || sp20 != 0) {
                    setLegAngle(temp_f31, i, sp14, spA0[i]);
                }
            }
        }
    }

    footdata_p = m_footData;
    for (i = 0; i < 4; i++, footdata_p++) {
        for (j = 0; j < 4; j++) {
            cLib_addCalcAngleS(&footdata_p->field_0x4[j], spA0[i][j], 2, 0x1800, 0x10);
        }
    }
}

void daHorse_c::setReinPosMoveInit(int param_0) {
    static cXyz reinLeftStart(63.0f, 17.0f, 11.0f);
    static cXyz reinRightStart(63.0f, 17.0f, -11.0f);
    static cXyz localNeckLeft(10.0f, 10.0f, 35.0f);
    static cXyz localNeckRight(10.0f, 10.0f, -35.0f);

    mDoMtx_multVec(m_model->getAnmMtx(0xF), &reinLeftStart, m_rein[0].field_0x0[0]);
    mDoMtx_multVec(m_model->getAnmMtx(0xF), &reinRightStart, m_rein[1].field_0x0[0]);

    s16 spC = field_0x170e + 0x4000;
    f32 var_f31 = 10.0f;
    f32 var_f30;
    f32 temp_f28 = cM_ssin(spC);
    f32 temp_f27 = cM_scos(spC);
    f32 temp_f29 = fabsf((f32)field_0x1712 / 8192.0f);

    if (shape_angle.x < 0) {
        var_f31 += 0.0005f * -shape_angle.x;
    }

    if (param_0 != 3) {
        var_f31 += 1.0f;
    }

    var_f30 = var_f31;
    m_rein[0].field_0x18 = 0.0f;
    m_rein[1].field_0x18 = 0.0f;

    if (field_0x1712 > 0) {
        mDoMtx_multVec(m_model->getAnmMtx(0xB), &localNeckRight, &m_rein[1].field_0x1c);
        m_rein[1].field_0x18 = 20.0f * temp_f29;
        var_f30 += 10.0f * temp_f29;
    } else if (field_0x1712 < 0) {
        mDoMtx_multVec(m_model->getAnmMtx(0xB), &localNeckLeft, &m_rein[0].field_0x1c);
        m_rein[0].field_0x18 = 20.0f * temp_f29;
        var_f31 += 10.0f * temp_f29;
    }

    m_rein[0].field_0x10 = var_f31 * temp_f28;
    m_rein[0].field_0x14 = var_f31 * temp_f27;
    m_rein[1].field_0x10 = -var_f30 * temp_f28;
    m_rein[1].field_0x14 = -var_f30 * temp_f27;

    if ((current.pos.abs2(old.pos) < 1.0f && field_0x170e - field_0x1710 == 0) || checkResetStateFlg0(RFLG0_UNK_1)) {
        return;
    }

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(field_0x170e - field_0x1710);
    mDoMtx_stack_c::transM(-old.pos.x, -old.pos.y, -old.pos.z);
    cXyz sp24;

    cXyz* sp20;
    int j, i;
    daHorseRein_c* rein_p = m_rein;
    
    for (i = 0; i < 3; i++, rein_p++) {
        sp20 = rein_p->field_0x0[0] + 1;
        for (j = 1; j < rein_p->field_0x8[1]; j++, sp20++) {
            sp24 = *sp20;
            mDoMtx_stack_c::multVec(&sp24, sp20);
        }
    }
}

void daHorse_c::copyReinPos() {
    int i;
    cXyz* pos_p = m_reinLine.getPos(0);
    daHorseRein_c* rein = &m_rein[0];
    
    field_0x1204 = rein->field_0x8[0];

    for (i = 0; i < rein->field_0x8[0]; i++, pos_p++) {
        *pos_p = rein->field_0x0[0][i];
    }
    rein = &m_rein[2];
    if (rein->field_0x8[0] > 1) {
        field_0x1204 += rein->field_0x8[0];
        for (i = 0; i < rein->field_0x8[0]; i++, pos_p++) {
            *pos_p = rein->field_0x0[0][i];
        }
    }

    rein = &m_rein[1];
    field_0x1204 += rein->field_0x8[0];
    for (i = rein->field_0x8[0] - 1; i >= 0; i--, pos_p++) {
        *pos_p = rein->field_0x0[0][i];
    }
}

void daHorse_c::setReinPosHandSubstance(int param_0) {
    static const Vec zeldaLocalLeft = {3.0f, -0.5f, -5.0f};
    static const Vec zeldaLocalRight = {3.0f, -0.5f, 5.0f};

    setReinPosMoveInit(param_0);

    daAlink_c* player_p = daAlink_getAlinkActorClass();
    int var_r29 = (-field_0x1712 * 5) / 0x2000;
    if (param_0 != 3) {
        var_r29 *= 2;
    }
    cXyz* temp_r26;
    int var_r25;
    int var_r24;
    if (param_0 & 1) {
        var_r25 = var_r29 + 20;
    } else {
        var_r25 = var_r29 + 24;
    }

    if (param_0 & 2) {
        var_r24 = 20 - var_r29;
    } else {
        var_r24 = 24 - var_r29;
    }

    if (param_0 & 4) {
        daHoZelda_c* hozelda_p = (daHoZelda_c*)m_zeldaActorKeep.getActor();
        mDoMtx_multVec(hozelda_p->getRightFingerMtx(), &zeldaLocalLeft, &m_rein[0].field_0x0[0][var_r25]);
        mDoMtx_multVec(hozelda_p->getRightFingerMtx(), &zeldaLocalRight, &m_rein[1].field_0x0[0][var_r24]);
    } else {
        player_p->getHorseReinHandPos(&m_rein[0].field_0x0[0][var_r25], &m_rein[1].field_0x0[0][var_r24]);
    }

    m_rein[0].setReinPosPart(var_r25);
    m_rein[1].setReinPosPart(var_r24);

    if (param_0 & 4) {
        m_rein[2].setReinPosPart(0);
    } else {
        temp_r26 = m_rein[2].field_0x0[0];
        player_p->getHorseReinCenterLeftPos(temp_r26);

        if (param_0 == 3) {
            player_p->getHorseReinCenterRightPos(temp_r26 + 4);
            m_rein[2].setReinPosPart(4);
        } else {
            m_rein[2].setReinPosPart(0);
        }

        if (checkResetStateFlg0(RFLG0_UNK_1)) {
            for (int i = 0; i < 5; i++) {
                m_rein[0].setReinPosPart(var_r25);
                m_rein[1].setReinPosPart(var_r24);

                temp_r26 = m_rein[2].field_0x0[0];
                player_p->getHorseReinCenterLeftPos(temp_r26);

                if (param_0 == 3) {
                    player_p->getHorseReinCenterRightPos(temp_r26 + 4);
                    m_rein[2].setReinPosPart(4);
                } else {
                    m_rein[2].setReinPosPart(0);
                }
            }
        }
    }

    copyReinPos();
}

void daHorse_c::setReinPosNormalSubstance() {
    static cXyz saddleLeft(29.0f, -2.0f, 30.0f);
    static cXyz saddleRight(29.0f, 2.0f, 30.0f);

    if (!checkStateFlg0(FLG0_UNK_1) && getZeldaActor() != NULL) {
        if (((daHoZelda_c*)getZeldaActor())->checkSingleRide()) {
            offStateFlg0(FLG0_UNK_4000000);
            return;
        }
    }

    setReinPosMoveInit(0);

    mDoMtx_multVec(m_model->getAnmMtx(0x15), &saddleLeft, m_rein[0].field_0x0[0] + 24);
    m_rein[0].setReinPosPart(24);

    mDoMtx_multVec(m_model->getAnmMtx(0x15), &saddleRight, m_rein[1].field_0x0[0] + 24);
    m_rein[1].setReinPosPart(24);

    *m_rein[2].field_0x0[0] = m_rein[0].field_0x0[0][24];
    m_rein[2].setReinPosPart(0);

    if (checkResetStateFlg0(RFLG0_UNK_1)) {
        for (int i = 0; i < 5; i++) {
            m_rein[0].setReinPosPart(24);
            m_rein[1].setReinPosPart(24);
            *m_rein[2].field_0x0[0] = m_rein[0].field_0x0[0][24];
            m_rein[2].setReinPosPart(0);
        }
    }

    copyReinPos();
}

void daHorse_c::bgCheck() {
    if (m_procID != PROC_LARGE_DAMAGE_e) {
        static cXyz localCenterPos(0.0f, 100.0f, 0.0f);
        static cXyz localFrontPos(0.0f, 100.0f, 220.0f);
        static cXyz localBackPos(0.0f, 100.0f, -170.0f);

        cXyz line_start;
        cXyz line_end;
        cXyz* linecross_p = NULL;

        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::ZXYrotM(field_0x1714, shape_angle.y, 0);
        mDoMtx_stack_c::multVec(&localCenterPos, &line_start);
        mDoMtx_stack_c::multVec(&localFrontPos, &line_end);

        cM3dGPla plane;
        m_linechk.Set(&line_start, &line_end, this);

        if (dComIfG_Bgsp().LineCross(&m_linechk)) {
            dComIfG_Bgsp().GetTriPla(m_linechk, &plane);
            if (cBgW_CheckBWall(plane.mNormal.y)) {
                linecross_p = m_linechk.GetCrossP();
                onStateFlg0(FLG0_UNK_4);
                field_0x16ea = (current.angle.y + 0x8000) - plane.mNormal.atan2sX_Z();
            }
        }

        if (linecross_p == NULL) {
            offStateFlg0(FLG0_UNK_4);
            mDoMtx_stack_c::multVec(&localBackPos, &line_end);

            m_linechk.Set(&line_start, &line_end, this);
            if (dComIfG_Bgsp().LineCross(&m_linechk) != 0) {
                dComIfG_Bgsp().GetTriPla(m_linechk, &plane);
                if (cBgW_CheckBWall(plane.mNormal.y)) {
                    linecross_p = m_linechk.GetCrossP();
                }
            }
        }

        if (linecross_p != NULL) {
            line_end -= *linecross_p;

            f32 temp_f31 = -line_end.absXZ() * cM_scos(line_end.atan2sX_Z() - cM_atan2s(-plane.mNormal.x, -plane.mNormal.z));
            current.pos.x -= temp_f31 * plane.mNormal.x;
            current.pos.y -= line_end.y;
            current.pos.z -= temp_f31 * plane.mNormal.z;
        }
    }
}

bool daHorse_c::checkSpecialWallHitSubstance(const cXyz& param_0) const {
    if (m_procID == PROC_JUMP_e) {
        return 0;
    }

    if (checkStateFlg0(FLG0_UNK_100)) {
        return 1;
    }

    if (speedF < (0.7f * m_normalMaxSpeedF)) {
        return 1;
    }

    if (cLib_distanceAngleS(shape_angle.y, param_0.atan2sX_Z()) > 0x6000) {
        return 0;
    }

    return 1;
}

void daHorse_c::setServiceWaitTimer() {
    m_serviceWaitTimer = cM_rndF(80.0f) + 150.0f;
}

BOOL daHorse_c::checkServiceWaitAnime() {
    return m_anmIdx[0] == ANM_HS_WAIT_03 ||
           m_anmIdx[0] == ANM_HS_WAIT_04 ||
           m_anmIdx[0] == ANM_HS_WAIT_05 ||
           m_anmIdx[0] == ANM_HS_WAIT_01B ||
           m_anmIdx[0] == ANM_HS_WAIT_01B_CONNECT ||
           m_anmIdx[0] == ANM_HS_WAIT_02;
}

BOOL daHorse_c::checkTurnInput() {
    return m_padStickValue > 0.9f && (!dComIfGp_getAttention()->Lockon() || daAlink_getAlinkActorClass()->getAtnActor() != NULL);
}

BOOL daHorse_c::checkTgHitTurn() {
    if (!dComIfGp_event_runCheck() && !checkStateFlg0(FLG0_UNK_1)) {
        if (!daAlink_getAlinkActorClass()->checkHorseRideReady() &&
            (checkEndResetStateFlg0(ERFLG0_UNK_800) ||
             m_tgco_cyl[0].ChkTgHit() ||
             m_tgco_cyl[1].ChkTgHit() ||
             m_tgco_cyl[2].ChkTgHit() ||
             m_head_sph.ChkTgHit()))
        {
            return procTurnInit(1);
        }
    }

    return 0;
}

BOOL daHorse_c::checkTurnAfterFastMove(f32 param_0) {
    if (param_0 > field_0x177C) {
        if (checkTgHitTurn()) {
            return 1;
        }

        if (checkInputOnR()) {
            int temp_r29 = abs((s16)(m_padStickAngleY - shape_angle.y));
            if (field_0x16bc != 0) {
                field_0x16bc--;
                return procTurnInit(0);
            }

            if (temp_r29 < 0x2000 && !checkHorseNoMove(1)) {
                speedF = m_hio->m.fastwalk_to_run_rate * m_normalMaxSpeedF;
                return procMoveInit();
            }

            if (m_procID == PROC_TURN_e && field_0x1722 != 0) {
                return 0;
            }

            if (temp_r29 < 0x6000) {
                return procWaitInit();
            }

            if (daAlink_getAlinkActorClass()->checkHorseTurnMode()) {
                if (checkTurnInput() && !checkStateFlg0(FLG0_UNK_200)) {
                    return procTurnInit(0);
                }
                return procWaitInit();
            }
        }
    }

    return 0;
}

void daHorse_c::setNeckAnimeMorf() {
    u16 start_jnt_no;
    u16 end_jnt_no;
    if (m_oldFrame->getOldFrameRate() > 0.1f && m_oldFrame->getOldFrameStartJoint() == 0) {
        start_jnt_no = 0;
        end_jnt_no = 38;
    } else {
        start_jnt_no = 11;
        end_jnt_no = 21;
    }

    m_oldFrame->initOldFrameMorf(3.0f, start_jnt_no, end_jnt_no);
}

void daHorse_c::setNeckAnime(u16 i_anmIdx, f32 i_speed, f32 i_startF, s16 i_endF) {
    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, i_anmIdx);
    
    s16 endF;
    if (i_endF < 0) {
        endF = bck->getFrameMax();
    } else {
        endF = i_endF;
    }

    f32 frame;
    if (i_speed < 0.0f) {
        frame = endF;
    } else {
        frame = i_startF;
    }

    m_anmIdx[2] = i_anmIdx;
    m_anmRatio[2].setAnmTransform(bck);
    m_frameCtrl[2].setFrameCtrl(bck->getAttribute(), i_startF, endF, i_speed, frame);
    bck->setFrame(frame);
    setNeckAnimeMorf();
}

void daHorse_c::resetNeckAnime() {
    if (m_anmRatio[2].getAnmTransform() != NULL) {
        m_anmRatio[2].setAnmTransform(NULL);
        m_anmIdx[2] = 0xFFFF;
        setNeckAnimeMorf();
    }
}

void daHorse_c::setLashCnt() {
    if (checkEndResetStateFlg0(ERFLG0_UNK_8) && checkStateFlg0(FLG0_UNK_1)) {
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 1, cXyz(0.0f, 1.0f, 0.0f));

        if (checkStateFlg0(FLG0_PLAYER_BACK_RIDE_LASH) || m_lashCnt > 0) {
            mDoAud_seStart(Z2SE_WHIP_HORSE, NULL, 0, 0);

            if (!checkStateFlg0(FLG0_PLAYER_BACK_RIDE_LASH)) {
                m_lashCnt--;
                if (m_lashCnt == 0) {
                    m_lashRecoverTime = m_hio->m.full_spur_recovery_time;
                } else {
                    m_lashRecoverTime = m_hio->m.spur_recovery_time;
                }
            } else {
                offStateFlg0(FLG0_PLAYER_BACK_RIDE_LASH);
            }

            if (checkStateFlg0(FLG0_UNK_2000)) {
                m_lashAccelerationTime = m_hio->m.kakariko_lash_acceleration_time;
            } else {
                m_lashAccelerationTime = m_hio->m.lash_acceleration_time;
            }

            onResetStateFlg0(RFLG0_LASH_DASH_START);
            setNeckAnime(ANM_HS_LASH, 1.0f, 0.0f, -1);
            m_sound.startCreatureVoice(Z2SE_HS_V_RUSH, -1);
            onEndResetStateFlg0(ERFLG0_UNK_400);
        } else {
            m_sound.startCreatureVoice(Z2SE_HS_V_ANNOY, -1);
        }
    } else if (checkStateFlg0(FLG0_UNK_1)) {
        if (m_lashAccelerationTime > 0) {
            m_lashAccelerationTime--;
        }

        if (m_lashRecoverTime > 0) {
            m_lashRecoverTime--;
            if (m_lashRecoverTime == 0) {
                if (m_lashCnt == 0) {
                    m_lashCnt = 6;
                    if (!dComIfGp_event_runCheck()) {
                        mDoAud_seStart(Z2SE_WHIP_RECOVER_ALL, NULL, 0, 0);
                    }
                } else {
                    m_lashCnt++;
                    if (m_lashCnt < 6) {
                        m_lashRecoverTime = m_hio->m.continuous_spur_recovery_time;
                    }

                    if (!dComIfGp_event_runCheck()) {
                        mDoAud_seStart(Z2SE_WHIP_RECOVER, NULL, 0, 0);
                    }
                }
            }
        }
    } else {
        m_lashAccelerationTime = 0;
        m_lashRecoverTime = 0;
    }
}

void daHorse_c::setTalkModeWolf() {
    if (eventInfo.checkCommandTalk()) {
        if (!checkStateFlg0(FLG0_UNK_20000)) {
            m_msgflow.init(this, m_flowID, 0, NULL);
            onStateFlg0(FLG0_UNK_20000);
        } else {
            if (m_msgflow.doFlow(this, NULL, 0)) {
                dComIfGp_event_reset();
            }
        }
    } else {
        offStateFlg0(FLG0_UNK_20000);
    }
}

void daHorse_c::setBoarHit(fopAc_ac_c* param_0, int param_1) {
    int vibmode;
    s16 var_r30;
    if (param_1) {
        vibmode = VIBMODE_S_POWER2;
        var_r30 = 0x800;
    } else {
        vibmode = VIBMODE_S_POWER1;
        var_r30 = 0x500;
    }

    if (abs(field_0x1702) < 0x100) {
        dComIfGp_getVibration().StartShock(vibmode, 1, cXyz(0.0f, 1.0f, 0.0f));
        daAlink_getAlinkActorClass()->seStartOnlyReverb(Z2SE_HORSE_BODYHIT);
    }

    if ((s16)(fopAcM_searchActorAngleY(this, param_0) - shape_angle.y) >= 0) {
        field_0x1702 = -var_r30;
    } else {
        field_0x1702 = var_r30;
    }
}

void daHorse_c::savePos() {
    if (this->model != NULL && !checkStateFlg0(FLG0_UNK_8000) && !checkStateFlg0(FLG0_NO_DRAW_WAIT)) {
        dComIfGs_setHorseRestart(dComIfGp_getStartStageName(), current.pos, shape_angle.y, fopAcM_GetRoomNo(this));
    }
}

int daHorse_c::callHorseSubstance(cXyz const* i_pos) {
    int room_no = dComIfGp_roomControl_getStayNo();
    if (checkStateFlg0(FLG0_RODEO_MODE) ||
        (daAlink_c::checkStageName("F_SP108") && (room_no == 5 || room_no == 6 || room_no == 11 || room_no == 14)) ||
        (daAlink_c::checkStageName("F_SP110") && (room_no == 1 || room_no == 2 || room_no == 3)))
    {
        return 0;
    }

    cXyz pos_vec = current.pos - *i_pos;
    f32 dist_xz2 = pos_vec.abs2XZ();

    m_sound.startCreatureVoice(Z2SE_HS_V_CRY_UMAKUSA, m_reverb);

    int rt = 2;
    if (m_zeldaActorKeep.getActor() != NULL) {
        if (((daHoZelda_c*)m_zeldaActorKeep.getActor())->checkSingleRide()) {
            changeOriginalDemo();
            changeDemoMode(15, 0);
            return 4;
        }
    }

    if (m_path != NULL && (checkStateFlg0(FLG0_NO_DRAW_WAIT) || dist_xz2 > SQUARE(2000.0f))) {
        daAlink_c* player = daAlink_getAlinkActorClass();
        Vec* farthest_pos;
        Vec* path_pnt_pos;

        for (int i = 0; i < m_path->m_num; i++) {
            path_pnt_pos = &m_path->m_points[i].m_position;
            f32 x_dist = path_pnt_pos->x - i_pos->x;
            f32 z_dist = path_pnt_pos->z - i_pos->z;
            f32 farthest_sqdist;
            f32 sqdist = (x_dist * x_dist) + (z_dist * z_dist);

            if (i == 0 || (farthest_sqdist > sqdist && sqdist > SQUARE(2000.0f))) {
                farthest_sqdist = sqdist;
                farthest_pos = path_pnt_pos;
            }
        }

        cXyz pos(farthest_pos->x, farthest_pos->y, farthest_pos->z);
        setHorsePosAndAngle(&pos, shape_angle.y);
        rt = 1;
    } else if (dist_xz2 <= SQUARE(800.0f)) {
        return 3;
    }

    onStateFlg0(FLG0_CALL_HORSE);
    changeOriginalDemo();
    changeDemoMode(12, 0);
    return rt;
}

void daHorse_c::setHorsePosAndAngleSubstance(cXyz const* param_0, s16 param_1) {
    int i, j;

    cXyz sp10(current.pos);
    s16 var_r24 = shape_angle.y;

    if (param_0 != NULL) {
        current.pos = *param_0;
        old.pos = current.pos;
    }

    shape_angle.y = param_1;
    current.angle.y = param_1;
    field_0x16e8 = shape_angle.y;
    field_0x170e = param_1;
    field_0x1710 = param_1;

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y - var_r24);
    mDoMtx_stack_c::transM(-sp10.x, -sp10.y, -sp10.z);
    mDoMtx_stack_c::multVec(&field_0x17a0, &field_0x17a0);

    daHorseRein_c* rein_p = m_rein;
    for (i = 0; i < 3; i++, rein_p++) {
        cXyz* var_r28 = rein_p->field_0x0[0];
        for (j = 0; j < rein_p->field_0x8[1]; j++, var_r28++) {
            mDoMtx_stack_c::multVec(var_r28, var_r28);
        }
    }
}

BOOL daHorse_c::checkPlayerHeavy() {
    return !dComIfGp_event_runCheck() && checkStateFlg0(FLG0_UNK_1) && !checkStateFlg0(daHorse_FLG0(FLG0_UNK_200000 | FLG0_UNK_100000)) && daAlink_getAlinkActorClass()->checkBootsOrArmorHeavy();
}

void daHorse_c::setTgCoGrp(u32 i_tgGrp, u32 i_coGrp) {
    m_head_sph.SetTgGrp(i_tgGrp);

    dCcD_Cyl* cyl_p = m_tgco_cyl;
    for (int i = 0; i < 3; i++, cyl_p++) {
        cyl_p->SetTgGrp(i_tgGrp);
        cyl_p->SetCoVsGrp(i_coGrp);
    }
}

void daHorse_c::onRideFlgSubstance() {
    onStateFlg0(FLG0_UNK_1);
    m_rodeoPointCnt = 0;
    setTgCoGrp(4, 0x60);
}

void daHorse_c::offRideFlgSubstance() {
    offStateFlg0(FLG0_UNK_1);
    setTgCoGrp(8, 0x70);
}

int daHorse_c::procWaitInit() {
    if (m_procID == PROC_WAIT_e) {
        return 0;
    }

    offStateFlg0(daHorse_FLG0(FLG0_UNK_10000000 | FLG0_PLAYER_BACK_RIDE_LASH | FLG0_UNK_800 | FLG0_UNK_200));
    m_proc = &daHorse_c::procWait;
    m_procID = PROC_WAIT_e;
    speedF = 0.0f;
    field_0x1760 = 0.0f;

    if (!checkServiceWaitAnime()) {
        setMoveAnime(3.0f);
        setServiceWaitTimer();
    }

    return 1;
}

int daHorse_c::procWait() {
    if ((!dComIfGp_event_runCheck() && checkStateFlg0(FLG0_UNK_4000000)) || checkPlayerHeavy()) {
        return procTurnInit(0);
    }

    int sp18 = setSpeedAndAngle();
    field_0x1760 = speedF;

    if (!checkEndResetStateFlg0(ERFLG0_MOVE_ACCEPT) && !checkStateFlg0(FLG0_UNK_40000) && (m_anmIdx[0] != ANM_HS_WALK_START || !(m_frameCtrl[0].getFrame() >= 15.0f)) && !dComIfGp_event_runCheck()) {
        if (sp18 == 0 && speedF < 0.0f && m_anmIdx[0] != ANM_HS_WALK_START) {
            setSingleAnime(ANM_HS_WALK_START, 1.0f, 0.0f, -1, 3.0f, 0);
        }
        speedF = 0.0f;
    }

    if (m_anmIdx[0] == ANM_HS_WAIT_03 && m_frameCtrl[0].checkPass(18)) {
        m_sound.startCreatureVoice(Z2SE_HS_V_BURURU, m_reverb);
    }

    if (sp18 != 0) {
        return procTurnInit(0);
    }

    if (fabsf(speedF) >= 0.05f) {
        if (checkEndResetStateFlg0(ERFLG0_RIDE_RUN_FLG)) {
            speedF = m_hio->m.fastwalk_to_run_rate * m_normalMaxSpeedF;
        }
        return procMoveInit();
    }

    daAlink_c* player = daAlink_getAlinkActorClass();
    if (checkTgHitTurn()) {
        return 1;
    }

    f32 enemy_search_range = m_hio->m.enemy_search_range;
    if ((!checkInputOnR() || !checkStateFlg0(FLG0_UNK_1)) && fopAcIt_Judge((fopAcIt_JudgeFunc)daHorse_searchEnemy, &enemy_search_range) != NULL) {
        onResetStateFlg0(RFLG0_ENEMY_SEARCH);

        if (field_0x170c == 0 && !checkStateFlg0(daHorse_FLG0(FLG0_UNK_200000 | FLG0_UNK_100000)) && !dComIfGp_event_runCheck() && !player->checkHorseRideReady() && !player->checkHorseLieAnime() && !checkStateFlg0(FLG0_PLAYER_BACK_RIDE_LASH) && !checkStateFlg0(FLG0_UNK_1)) {
            return procTurnInit(1);
        }

        if (!dComIfGp_event_runCheck() && !player->checkHorseRideReady() && m_anmIdx[0] != ANM_HS_EXCITEMENT && m_anmIdx[0] != ANM_HS_EXCITEMENT_2) {
            if (cM_rnd() < 0.7f) {
                setSingleAnime(ANM_HS_EXCITEMENT_2, 1.0f, 0.0f, -1, 3.0f, 0);
                field_0x170c = 80.0f + cM_rndF(30.0f);
            } else {
                setSingleAnime(ANM_HS_EXCITEMENT, 1.0f, 0.0f, -1, 3.0f, 0);
                field_0x170c = 30.0f + cM_rndF(30.0f);
            }
        }
    } else if ((m_anmIdx[0] == ANM_HS_EXCITEMENT || m_anmIdx[0] == ANM_HS_EXCITEMENT_2) && !m_frameCtrl[0].checkPass(0.0f)) {
        onResetStateFlg0(RFLG0_ENEMY_SEARCH);
    }

    f32 temp_f30 = fabsf(m_frameCtrl[0].getRate());

    if (m_anmIdx[0] == ANM_HS_WALK_START) {
        setServiceWaitTimer();
        if (fabsf(field_0x1760) < 0.05f) {
            m_anmIdx[0] = 0xFFFF;
            setMoveAnime(3.0f);
        }
    } else if (temp_f30 < 0.01f || shape_angle.y != field_0x16e8 || field_0x16f0 != 0 || eventInfo.checkCommandTalk()) {
        int spC = 0;
        if (shape_angle.y == field_0x16e8 && field_0x16f0 == 0 && !eventInfo.checkCommandTalk()) {
            if (m_anmIdx[0] == ANM_HS_WAIT_04 || (m_anmIdx[0] == ANM_HS_WAIT_01B_CONNECT && m_frameCtrl[0].getFrame() > 1.0f)) {
                setSingleAnime(ANM_HS_WAIT_01B, 1.0f, 0.0f, -1, 3.0f, 0);
                m_frameCtrl[0].setAttribute(0);
                spC = 1;
            } else if (m_anmIdx[0] == ANM_HS_WAIT_01B) {
                setSingleAnime(ANM_HS_WAIT_01B_CONNECT, -1.0f, 0.0f, -1, 3.0f, 0);
                spC = 1;
            }
        }

        if (spC == 0) {
            f32 var_f29;
            if (temp_f30 < 0.01f) {
                var_f29 = 3.0f;
            } else {
                var_f29 = -1.0f;
            }

            if (!checkResetStateFlg0(RFLG0_ENEMY_SEARCH) || (checkInputOnR() && checkStateFlg0(FLG0_UNK_1))) {
                setMoveAnime(var_f29);
            }

            setServiceWaitTimer();
        }
    } else if (shape_angle.y == field_0x16e8 && !dComIfGp_event_runCheck() && fabsf(field_0x1760) < 0.05f && !checkResetStateFlg0(RFLG0_ENEMY_SEARCH) && m_anmRatio[2].getAnmTransform() == NULL) {
        if (m_serviceWaitTimer == 0) {
            if (!checkServiceWaitAnime()) {
                f32 rnd = cM_rnd();
                u16 anm_idx;
                if (rnd < 0.2f) {
                    anm_idx = ANM_HS_WAIT_02;
                } else if (rnd < 0.4f) {
                    anm_idx = ANM_HS_WAIT_03;
                } else if (rnd < 0.6f) {
                    anm_idx = ANM_HS_WAIT_04;
                } else if (rnd < 0.8f) {
                    anm_idx = ANM_HS_WAIT_05;
                } else {
                    anm_idx = ANM_HS_WAIT_01B_CONNECT;
                }

                setSingleAnime(anm_idx, 1.0f, 0.0f, -1, 6.0f, 0);
            }
        } else {
            m_serviceWaitTimer--;
            setMoveAnime(-1.0f);
        }
    } else {
        if (!checkResetStateFlg0(RFLG0_ENEMY_SEARCH) || (checkInputOnR() && checkStateFlg0(FLG0_UNK_1))) {
            setMoveAnime(-1.0f);
        }
        setServiceWaitTimer();
    }

    if (!checkStateFlg0(FLG0_UNK_1)) {
        if (daPy_py_c::checkNowWolf()) {
            attention_info.flags |= fopAc_AttnFlag_SPEAK_e;
            eventInfo.onCondition(1);
        } else if (m_procID == PROC_WAIT_e) {
            acceptPlayerRide();
        }

        if (checkStateFlg0(FLG0_RIDE_START_FLG)) {
            offStateFlg0(FLG0_RIDE_START_FLG);
            if (!checkServiceWaitAnime() || m_anmIdx[0] == ANM_HS_WAIT_01B || m_anmIdx[0] == ANM_HS_WAIT_04 || (m_anmIdx[0] == ANM_HS_WAIT_01B_CONNECT && m_frameCtrl[0].getRate() > 0.0f)) {
                setSingleAnime(ANM_HS_WAIT_02, 1.0f, 0.0f, -1, 3.0f, 0);
                m_serviceWaitTimer = 0;
            }
        }
    }

    return 1;
}

int daHorse_c::procMoveInit() {
    m_proc = &daHorse_c::procMove;
    m_procID = PROC_MOVE_e;
    offStateFlg0(daHorse_FLG0(FLG0_PLAYER_BACK_RIDE_LASH | FLG0_UNK_200));
    setMoveAnime(3.0f);
    field_0x171a = 0;
    return 1;
}

int daHorse_c::procMove() {
    int var_r30 = setSpeedAndAngle();
    if (var_r30 != 0) {
        offStateFlg0(FLG0_UNK_10000000);
    }

    if (checkStateFlg0(FLG0_UNK_800) && !daAlink_getAlinkActorClass()->checkHorseSubjectivity()) {
        offStateFlg0(FLG0_UNK_800);
        if (var_r30 == 0) {
            speedF = field_0x1798;
        }
    }

    if (m_demoMode == 2 && speedF > (m_hio->m.walk_to_fastwalk_rate * m_normalMaxSpeedF)) {
        speedF = m_hio->m.walk_to_fastwalk_rate * m_normalMaxSpeedF;
    } else if (m_demoMode == 10 && speedF > (m_hio->m.wait_to_walk_rate * m_normalMaxSpeedF)) {
        speedF = m_hio->m.wait_to_walk_rate * m_normalMaxSpeedF;
    } else if (m_demoMode == 11 && speedF > (m_hio->m.fastwalk_to_run_rate * m_normalMaxSpeedF)) {
        speedF = m_hio->m.fastwalk_to_run_rate * m_normalMaxSpeedF;
    }

    if (checkStateFlg0(daHorse_FLG0(FLG0_UNK_200000 | FLG0_UNK_100000)) && !m_acch.ChkGroundHit()) {
        field_0x1768 = 350.0f;
        field_0x176c = 0.0f;
        field_0x1770 = field_0x1768;
        current.pos.y = old.pos.y;
        procJumpInit(0);
        speed.y = 0.0f;
        setSingleAnime(ANM_HS_JUMP_END, 0.0f, 0.0f, -1, 8.0f, 0);
    } else if (!checkStateFlg0(FLG0_UNK_10000000) && checkEndResetStateFlg0(daHorse_ERFLG0(ERFLG0_UNK_20 | ERFLG0_UNK_1))) {
        procJumpInit(0);
    } else if (var_r30 == 5) {
        field_0x1768 = m_hio->m.cliff_jump_horizontal;
        field_0x176c = m_hio->m.cliff_jump_vertical + current.pos.y;
        field_0x1770 = field_0x1768;
        procJumpInit(1);
    } else if (var_r30 == 1) {
        procTurnInit(0);
    } else if (var_r30 == 3 || var_r30 == 4) {
        if (!checkStateFlg0(FLG0_UNK_2)) {
            resetNeckAnime();
            m_lashAccelerationTime = 0;
            offStateFlg0(daHorse_FLG0(FLG0_TURN_CANCEL_KEEP | FLG0_PLAYER_BACK_RIDE_LASH | FLG0_UNK_800 | FLG0_UNK_2));
            procWaitInit();
        } else {
            if (var_r30 == 4) {
                speedF = 0.0f;
            }
            procStopInit();
        }
    } else if (fabsf(speedF) < 0.05f) {
        if (checkStateFlg0(FLG0_UNK_200)) {
            procTurnInit(0);
        } else {
            procWaitInit();
        }
    } else if (checkStateFlg0(FLG0_UNK_10000000) && field_0x171a != 0 && (m_cc_stts.GetCCMoveP()->abs() > 1.0f || checkStateFlg0(FLG0_UNK_4) || m_acch.ChkWallHit() || m_callMoveTimer == 0 || daAlink_getAlinkActorClass()->current.pos.abs2XZ(current.pos) < 640000.0f)) {
        procStopInit();
        offStateFlg0(FLG0_UNK_10000000);
    } else {
        setMoveAnime(-1.0f);
    }

    if (m_procID == PROC_MOVE_e) {
        field_0x171a = 1;
    }

    return 1;
}

int daHorse_c::procStopInit() {
    m_proc = &daHorse_c::procStop;
    m_procID = PROC_STOP_e;
    resetNeckAnime();
    m_lashAccelerationTime = 0;
    offStateFlg0(daHorse_FLG0(FLG0_TURN_CANCEL_KEEP | FLG0_PLAYER_BACK_RIDE_LASH | FLG0_UNK_800 | FLG0_UNK_2));
    field_0x171a = 0;

    if (m_demoMode == 8 || m_demoMode == 16 || m_demoMode == 14 || checkStateFlg0(FLG0_UNK_40000000) || (speedF * (1.0f / m_normalMaxSpeedF)) < m_hio->m.run_to_fastrun_rate) {
        setSingleAnime(ANM_HS_STOP_SLOW, m_hio->m.faststop_anm_speed, 0.0f, -1, m_hio->m.faststop_interpolation, 0);
        field_0x177C = m_hio->m.fast_stop_cancel_frame;

        if (checkStateFlg0(FLG0_UNK_40000000)) {
            offStateFlg0(FLG0_UNK_40000000);
            field_0x171a = 1;
        }
    } else {
        setSingleAnime(ANM_HS_STOP_STAND, m_hio->m.faststop_stand_anm_speed, 0.0f, -1, m_hio->m.faststop_interpolation, 0);
        field_0x177C = m_hio->m.faststop_stand_cancel_frame;
    }

    field_0x16b7 = 1;
    field_0x171c = 0;

    if (checkStateFlg0(FLG0_UNK_10000000)) {
        field_0x1774 = 2.7f;
        field_0x171c = 1;
    } else if (speedF <= m_normalMaxSpeedF) {
        field_0x1774 = m_hio->m.faststop_deceleration_slow;
    } else {
        f32 var_f31 = (m_normalMaxSpeedF * m_normalMaxSpeedF) / (2.0f * m_hio->m.faststop_deceleration_slow);
        field_0x1774 = (speedF * speedF) / (2.f * var_f31);
        if (field_0x1774 > m_hio->m.faststop_deceleration) {
            field_0x1774 = m_hio->m.faststop_deceleration;
        }
    }

    return 1;
}

int daHorse_c::procStop() {
    struct unk_foot_eff_t {
        f32 _0;
        f32 _4;
        f32 _8;
    };

    static const unk_foot_eff_t stopFootEffectFrame[] = {
        {8, 11, 24},
        {11, 14, 27},
        {5, 8, 18},
        {8, 13, 23}
    };

    static const unk_foot_eff_t stopStandFootEffectFrame[] = {
        {7, 12, 24},
        {11, 19, 26},
        {5, 10, 18},
        {7, 18, 26}
    };

    const unk_foot_eff_t* var_r29;
    daPy_frameCtrl_c* frame_ctrl = &m_frameCtrl[0];
    int var_r25 = checkHorseNoMove(1);

    if (var_r25 == 2) {
        speedF = 0.0f;
    } else if (checkStateFlg0(FLG0_RODEO_MODE)) {
        cLib_chaseF(&speedF, 0.0f, 3.5f);
    } else {
        cLib_chaseF(&speedF, 0.0f, field_0x1774);
    }

    if (fabsf(speedF) < 1.0f) {
        acceptPlayerRide();
        if (checkStateFlg0(FLG0_RIDE_START_FLG)) {
            offStateFlg0(FLG0_RIDE_START_FLG);
            procWaitInit();
        }
    }

    f32 var_f30;
    if (frame_ctrl->checkAnmEnd() || checkEndResetStateFlg0(ERFLG0_CUT_TURN_CANCEL)) {
        if (m_demoMode == 8 || m_demoMode == 16) {
            dComIfGp_evmng_cutEnd(m_demoStaffId);
        } else {
            if (m_demoMode == 14) {
                m_demoMode = 1;
                return procWaitInit();
            } else if (checkEndResetStateFlg0(ERFLG0_CUT_TURN_CANCEL)) {
                return procMoveInit();
            } else if (field_0x171a != 0) {
                setNeckAnime(ANM_HS_WAIT_03, 1.0f, 0.0f, -1);
            }
            
            return procWaitInit();
        }
    } else {
        if (frame_ctrl->getFrame() > field_0x177C) {
            if (checkStateFlg0(FLG0_RODEO_MODE) || checkStateFlg0(FLG0_UNK_200)) {
                return procTurnInit(0);
            }
    
            if (var_r25 == 0 && checkTurnAfterFastMove(frame_ctrl->getFrame())) {
                return 1;
            }
        }
    
        if (m_anmIdx[0] == ANM_HS_STOP_STAND) {
            if (frame_ctrl->checkPass(59)) {
                field_0x16b6 = 1;
                field_0x16b7 = 2;
            }
            var_r29 = stopStandFootEffectFrame;
            var_f30 = 32.0f;
        } else {
            var_r29 = stopFootEffectFrame;
            var_f30 = 29.0f;
        }
    }

    f32 frame = frame_ctrl->getFrame();
    if (frame <= var_f30) {
        for (int i = 0; i < 4; i++, ++var_r29) { // needs to be pre-increment
            if ((frame >= var_r29->_0 && frame <= var_r29->_4) || frame >= var_r29->_8) {
#if PLATFORM_GCN || PLATFORM_WII
                field_0x16b6 |= (1 << i);
#else
                field_0x16b6 |= (u8)(1 << i);
#endif
            }
        }

        m_sound.startCreatureSoundLevel(Z2SE_FN_HORSE_SLIP, m_poly_sound, m_reverb);
    }

    return 1;
}

int daHorse_c::procTurnInit(int param_0) {
    if (checkEndResetStateFlg0(ERFLG0_RIDE_RUN_FLG)) {
        speedF = 0.0f;
        return 0;
    }

    m_proc = &daHorse_c::procTurn;
    m_procID = PROC_TURN_e;

    resetNeckAnime();
    setSingleAnime(ANM_HS_STAND, m_hio->m.stand_anm_speed, 0.0f, -1, m_hio->m.stand_interpolation, 0);

    field_0x1774 = 52.0f;
    field_0x1778 = 0.09817477f;
    field_0x177C = m_hio->m.stand_cancel_frame;
    field_0x1780 = 20.0f;
    field_0x1784 = 53.0f;
    speedF = 0.0f;

    field_0x171a = 0;
    field_0x171c = shape_angle.y;
    current.angle.y = shape_angle.y;
    offStateFlg0(daHorse_FLG0(FLG0_TURN_CANCEL_KEEP | FLG0_PLAYER_BACK_RIDE_LASH | FLG0_UNK_800 | FLG0_UNK_8));
    field_0x171e = shape_angle.y + 0x8000;

    if (!dComIfGp_event_runCheck() && !checkStateFlg0(FLG0_UNK_4000000)) {
        field_0x170a += (s16)(f32)0x8000;
    } else if (checkStateFlg0(FLG0_UNK_4000000)) {
        field_0x171e = shape_angle.y;
    }

    if (!dComIfGp_event_runCheck()) {
        offStateFlg0(FLG0_UNK_4000000);
    }

    field_0x1730 = param_0;
    if (field_0x1730 != 0) {
        field_0x171e = (f32)shape_angle.y + cM_rndFX(8192.0f);
    }

    field_0x1720 = 0;
    field_0x170c = 30.0f + cM_rndF(30.0f);
    field_0x16b7 = 2;

    acceptPlayerRide();
    m_at_cyl.SetAtSpl(dCcG_At_Spl_UNK_1);
    m_at_cyl.SetAtHitMark(3);
    m_at_cyl.SetAtAtp(2);

    if (checkInputOnR()) {
        field_0x1722 = 1;
    } else {
        field_0x1722 = 0;
    }

    return 1;
}

int daHorse_c::procTurn() {
    daPy_frameCtrl_c* frame_ctrl = &m_frameCtrl[0];

    if (!checkInputOnR()) {
        field_0x1722 = 0;
    }

    if (field_0x1730 != 0) {
        onResetStateFlg0(RFLG0_ENEMY_SEARCH);
    }

    int sp14 = checkHorseNoMove(1);

    if (m_anmIdx[0] == ANM_HS_STAND) {
        if (frame_ctrl->checkPass(8.0f) && !checkInputOnR()) {
            field_0x171a = 0;
        }

        if (frame_ctrl->checkPass(14.0f) && !checkEndResetStateFlg0(ERFLG0_CUT_TURN_CANCEL)) {
            m_sound.startCreatureVoice(Z2SE_HS_V_CRY, m_reverb);
        }
    }

    if (frame_ctrl->checkPass(field_0x1784) != 0) {
        field_0x16b6 = 1;
    }

    bool sp8 = frame_ctrl->getFrame() < field_0x1784;

    if (field_0x1720 != 0) {
        dComIfGp_evmng_cutEnd(m_demoStaffId);
        setMoveAnime(-1.0f);
    } else if (frame_ctrl->checkAnmEnd() || checkEndResetStateFlg0(ERFLG0_CUT_TURN_CANCEL)) {
        if (m_demoMode == 9) {
            dComIfGp_evmng_cutEnd(m_demoStaffId);
            setMoveAnime(3.0f);
            field_0x1720 = 1;
        } else {
            acceptPlayerRide();
            return procWaitInit();
        }
    } else {
        if (sp14 == 0) {
            if (checkTurnAfterFastMove(frame_ctrl->getFrame())) {
                if (sp8) {
                    field_0x16b6 = 1;
                    field_0x16b7 = 2;
                }
                return 1;
            }
        }
    
        if (frame_ctrl->getFrame() >= field_0x1780 && frame_ctrl->getFrame() < field_0x1774) {
            onResetStateFlg0(RFLG0_TURN_STAND);
            if (!checkStateFlg0(daHorse_FLG0(FLG0_UNK_200000 | FLG0_UNK_100000)) || frame_ctrl->getFrame() < field_0x1774 - 10.0f) {
                onResetStateFlg0(RFLG0_TURN_STAND_CAMERA);
            }

            if (checkInputOnR() || (checkStateFlg0(daHorse_FLG0(FLG0_UNK_200000 | FLG0_UNK_100000)) && !dComIfGp_event_runCheck()) || checkStateFlg0(FLG0_UNK_200) || field_0x1730 != 0) {
                f32 f31 = cM_fsin(field_0x1778 * (frame_ctrl->getFrame() - field_0x1780));
                s16 spE = 2000.0f * f31;
                s16 var_r27;
                if (checkStateFlg0(daHorse_FLG0(FLG0_UNK_200000 | FLG0_UNK_100000 | FLG0_UNK_200)) || field_0x1730 != 0) {
                    var_r27 = field_0x171e;
                } else {
                    var_r27 = m_padStickAngleY;
                }

                s16 var_r26 = var_r27 - shape_angle.y;
                if (var_r26 != 0 && (field_0x171a * var_r26) >= 0) {
                    if (field_0x171a == 0 && abs((s16)(shape_angle.y - field_0x171c)) > 1000) {
                        if (var_r26 > 0) {
                            field_0x171a = 1;
                        } else {
                            field_0x171a = -1;
                        }
                    }

                    cLib_chaseAngleS(&shape_angle.y, var_r27, spE);

                    cXyz sp24(current.pos.x - (60.0f * cM_ssin(current.angle.y)), current.pos.y, current.pos.z - (60.0f * cM_scos(current.angle.y)));
                    cXyz sp18 = current.pos - sp24;

                    mDoMtx_stack_c::transS(sp24.x, sp24.y, sp24.z);
                    mDoMtx_stack_c::YrotM(shape_angle.y - current.angle.y);
                    mDoMtx_stack_c::multVec(&sp18, &current.pos);
                    current.pos.y += 300.0f;

                    if (fopAcM_gc_c::gndCheck(&current.pos)) {
                        current.pos.y = fopAcM_gc_c::getGroundY();
                    } else {
                        current.pos = old.pos;
                    }

                    current.angle.y = shape_angle.y;
                    s16 spC = shape_angle.y;
                    s16 spA = shape_angle.x;

                    for (int i = 0; i < 8; i++) {
                        if (checkHorseNoMove(1) == 2) {
                            current.pos = old.pos;
                            break;
                        }

                        shape_angle.y += (s16)0x2000;
                        current.angle.y = shape_angle.y;
                    }

                    shape_angle.y = spC;
                    current.angle.y = shape_angle.y;
                    if (cLib_distanceAngleS(shape_angle.y, field_0x171c) >= 0x4000) {
                        offStateFlg0(FLG0_UNK_40000);
                    }
                }
            }
        }
    }

    acceptPlayerRide();
    if (checkStateFlg0(FLG0_RIDE_START_FLG)) {
        offStateFlg0(FLG0_RIDE_START_FLG);
        procWaitInit();
    } else if (frame_ctrl->getFrame() > field_0x1784 - 3.0f && frame_ctrl->getFrame() < 3.0f + field_0x1784) {
        onResetStateFlg0(RFLG0_UNK_20);
    }

    return 1;
}

int daHorse_c::procJumpInit(int param_0) {
    if (checkEndResetStateFlg0(ERFLG0_RIDE_RUN_FLG)) {
        return 0;
    }

    if (!checkStateFlg0(FLG0_UNK_1) && m_demoMode != 7) {
        return procStopInit();
    }

    m_proc = &daHorse_c::procJump;

    m_procID = PROC_JUMP_e;
    resetNeckAnime();

    field_0x1720 = param_0;

    if (param_0) {
        setSingleAnime(ANM_HS_JUMP_START_CLIFT, m_hio->m.cliff_jump_anm_speed, m_hio->m.cliff_jump_start_frame, m_hio->m.cliff_jump_end_frame, m_hio->m.cliff_jump_interpolation, 0);
    } else {
        setSingleAnime(ANM_HS_JUMP_START, m_hio->m.jump_anm_speed, m_hio->m.jump_start_frame, m_hio->m.jump_end_frame, m_hio->m.jump_interpolation, 0);
    }

    if (speedF < 0.75f * m_normalMaxSpeedF) {
        speedF = 0.75f * m_normalMaxSpeedF;
    }

    field_0x1774 = 0.8f * speedF;

    f32 temp_f31 = 20.0f + (field_0x176c - current.pos.y);
    speedF *= field_0x1768 / JMAFastSqrt((field_0x1768 * field_0x1768) + (temp_f31 * temp_f31));

    field_0x171a = field_0x1768 / speedF;
    if (field_0x171a < m_hio->m.min_jump_time) {
        field_0x171a = m_hio->m.min_jump_time;
        speedF = field_0x1768 / (f32)field_0x171a;
    }

    field_0x171e = field_0x1770 / speedF;
    if (field_0x171e < m_hio->m.min_jump_time) {
        field_0x171e = m_hio->m.min_jump_time;
    }

    field_0x177C = (-2.0f * temp_f31) / (f32)(field_0x171e * field_0x171e);
    gravity = (-2.0f * temp_f31) / (f32)(field_0x171a * field_0x171a);
    speed.y = -gravity * (f32)field_0x171a;

    offStateFlg0(daHorse_FLG0(FLG0_PLAYER_BACK_RIDE_LASH | FLG0_UNK_800 | FLG0_UNK_200 | FLG0_UNK_10));
    field_0x1702 = 0;
    field_0x171c = 0;
    field_0x17d0 = current.pos;
    field_0x1778 = 1.5f * (1.5f * (field_0x1768 * field_0x1768));
    field_0x1722 = 300;
    return 1;
}

int daHorse_c::procJump() {
    if (field_0x1722 != 0) {
        field_0x1722--;
    }

    if ((field_0x171c != 0) && ((speed.y < 0.0f && field_0x17d0.abs2XZ(current.pos) > field_0x1778) || field_0x1722 == 0) && m_acch.ChkGroundHit()) {
        return procLandInit(field_0x1774, field_0x1720);
    }

    field_0x171c = 1;

    if (m_anmIdx[0] == ANM_HS_JUMP_START_CLIFT) {
        if (m_frameCtrl[0].checkAnmEnd()) {
            setSingleAnime(ANM_HS_JUMP_MIDDLE, m_hio->m.cliff_air_anm_speed, m_hio->m.cliff_air_start_frame, m_hio->m.cliff_air_end_frame, m_hio->m.cliff_air_interpolation, 0);
        }
    } else if (m_anmIdx[0] == ANM_HS_JUMP_START) {
        if (m_frameCtrl[0].checkAnmEnd()) {
            int temp_r28 = (f32)field_0x171a - ((1.0f / m_hio->m.jump_anm_speed) * (f32)(m_frameCtrl[0].getEnd() - m_frameCtrl[0].getStart()));
            if (temp_r28 <= 0) {
                temp_r28 += field_0x171e;
                gravity = field_0x177C;

                if (temp_r28 <= 0) {
                    setSingleAnime(ANM_HS_JUMP_END, 0.0f, 0.0f, -1, field_0x171e, 0);
                } else {
                    setSingleAnime(ANM_HS_JUMP_MIDDLE, (m_hio->m.air_end_frame - 6.0f) / temp_r28, 6.0f, m_hio->m.air_end_frame, m_hio->m.air_interpolation, 0);
                }
            } else {
                setSingleAnime(ANM_HS_JUMP_MIDDLE, (6.0f - m_hio->m.air_start_frame) / temp_r28, m_hio->m.air_start_frame, 6, m_hio->m.air_interpolation, 0);
            }
        }
    } else if (m_anmIdx[0] == ANM_HS_JUMP_MIDDLE && field_0x1720 == 0 && m_frameCtrl[0].checkAnmEnd() && m_frameCtrl[0].getEnd() == 6) {
        setSingleAnime(ANM_HS_JUMP_MIDDLE, (1.0f + (m_hio->m.air_end_frame - 6.0f)) / field_0x171e, 7.0f, m_hio->m.air_end_frame, -1.0f, 0);
        gravity = field_0x177C;
    }

    return 1;
}

int daHorse_c::procLandInit(f32 i_speedF, BOOL param_1) {
    m_proc = &daHorse_c::procLand;
    m_procID = PROC_LAND_e;

    resetNeckAnime();
    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER2, 1, cXyz(0.0f, 1.0f, 0.0f));

    if (param_1) {
        setSingleAnime(ANM_HS_JUMP_END, m_hio->m.cliff_land_anm_speed, m_hio->m.cliff_land_start_frame, m_hio->m.cliff_land_end_frame, m_hio->m.cliff_land_interpolation, 0);
        field_0x177C = m_hio->m.cliff_land_cancel_frame;
    } else {
        setSingleAnime(ANM_HS_JUMP_END, m_hio->m.land_anm_speed, m_hio->m.land_start_frame, m_hio->m.land_end_frame, m_hio->m.land_interpolation, 0);
        field_0x177C = m_hio->m.land_cancel_frame;
    }

    speedF = i_speedF;
    if (speedF >= m_lashMaxSpeedF) {
        speedF = m_lashMaxSpeedF;
    }

    gravity = -5.0f;
    field_0x171a = 0;
    field_0x16b6 = 1;
    field_0x16b7 = 2;
    offStateFlg0(daHorse_FLG0(FLG0_PLAYER_BACK_RIDE_LASH | FLG0_UNK_800 | FLG0_UNK_200));
    return 1;
}

int daHorse_c::procLand() {
    daPy_frameCtrl_c* frame_ctrl = &m_frameCtrl[0];
    int temp_r3 = checkHorseNoMove(1);

    if (temp_r3 == 2) {
        speedF = 0.0f;
    } else if (temp_r3 != 0) {
        cLib_chaseF(&speedF, 0.0f, m_hio->m.stopping_deceleration);
    }

    if (field_0x171a != 0) {
        cLib_chaseF(&speedF, 0.0f, m_hio->m.stopping_deceleration);
        dComIfGp_evmng_cutEnd(m_demoStaffId);
        setMoveAnime(-1.0f);
    } else if (frame_ctrl->checkAnmEnd()) {
        if (m_demoMode == 7) {
            cLib_chaseF(&speedF, 0.0f, m_hio->m.stopping_deceleration);
            dComIfGp_evmng_cutEnd(m_demoStaffId);
            setMoveAnime(3.0f);
            field_0x171a = 1;
        } else {
            procMoveInit();
        }
    } else if (temp_r3 == 0) {
        checkTurnAfterFastMove(frame_ctrl->getFrame());
    }

    return 1;
}

int daHorse_c::procLargeDamageInit() {
    m_proc = &daHorse_c::procLargeDamage;
    m_procID = PROC_LARGE_DAMAGE_e;

    resetNeckAnime();
    setSingleAnime(ANM_HS_STAND, m_hio->m.stand_anm_speed, 0.0f, -1, m_hio->m.stand_interpolation, 0);
    
    current.angle.y = m_cowHitAngle;
    speedF = 60.0f;
    speed.y = 50.0f;
    gravity = -5.0f;
    offStateFlg0(daHorse_FLG0(FLG0_PLAYER_BACK_RIDE_LASH | FLG0_UNK_800 | FLG0_UNK_200));
    return 1;
}

int daHorse_c::procLargeDamage() {
    cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 5, 0x2000, 0x1000);
    if (m_acch.ChkWallHit()) {
        speedF = 0.0f;
    }

    return 1;
}

int daHorse_c::procToolDemoInit() {
    m_proc = &daHorse_c::procToolDemo;
    m_procID = PROC_TOOL_DEMO_e;

    speedF = 0.0f;
    speed.y = 0.0f;
    resetNeckAnime();
    offStateFlg0(daHorse_FLG0(FLG0_PLAYER_BACK_RIDE_LASH | FLG0_UNK_800 | FLG0_UNK_200));
    return 1;
}

int daHorse_c::procToolDemo() {
    dDemo_actor_c* demo_actor_p = dDemo_c::getActor(demoActorID);
    field_0x1730 = 0;

    if (demo_actor_p != NULL) {
        f32 anm_frame = 0.0f;

        if (demo_actor_p->checkEnable(2)) {
            current.pos = demo_actor_p->getTrans();
        }

        if (demo_actor_p->checkEnable(8)) {
            shape_angle = demo_actor_p->getRatate();
            current.angle = shape_angle;
        }

        if (demo_actor_p->checkEnable(0x40)) {
            anm_frame = demo_actor_p->getAnmFrame();
        }

        if (demo_actor_p->checkEnable(1) != 0) {
            int sp14, sp10, spC;
            u16 anm_idx;
            while (demo_actor_p->getDemoIDData(&sp14, &sp10, &spC, &anm_idx, NULL) != 0) {
                if (sp14 != 0 && sp14 == 2 && sp10 == 1 && spC == 2) {
                    setSingleAnime(anm_idx, 1.0f, 0.0f, -1, -1.0f, 1);
                }
            }
        }
    
        if (demo_actor_p->checkEnable(0x40)) {
            m_anmRatio[0].getAnmTransform()->setFrame(anm_frame);
            demo_actor_p->setAnmFrameMax(m_anmRatio[0].getAnmTransform()->getFrameMax());
            m_frameCtrl[0].setFrame(anm_frame);
        }
    } else {
        offStateFlg0(FLG0_UNK_80000);
        procWaitInit();
    }

    return 1;
}

void daHorse_c::searchSceneChangeArea(fopAc_ac_c* i_scnChg) {
    if (fopAcM_GetName(i_scnChg) == PROC_SCENE_EXIT) {
        if (m_scnChg_num == 50) {
            // "Exceeded buffer!!!!!\n"
            OS_PANIC(0x1CD6, "バッファ越え！！！！！\n");
#if PLATFORM_GCN || PLATFORM_WII
            m_scnChg_num = 49;
#endif
        }

        m_scnChg_buffer[m_scnChg_num] = (daScex_c*)i_scnChg;
        m_scnChg_num++;
    }
}

static void* daHorse_searchSceneChangeArea(fopAc_ac_c* i_actor, void* i_data) {
    (void)i_data;
    dComIfGp_getHorseActor()->searchSceneChangeArea(i_actor);
    return NULL;
}

int daHorse_c::execute() {
    m_scnChg_num = 0;
    fopAcIt_Executor((fopAcIt_ExecutorFunc)daHorse_searchSceneChangeArea, NULL);
    m_zeldaActorKeep.setActor();

    if (checkStateFlg0(FLG0_NO_DRAW_WAIT)) {
        if (checkStateFlg0(FLG0_CALL_HORSE)) {
            offStateFlg0(FLG0_NO_DRAW_WAIT);
        } else {
            return 1;
        } 
    }

    daAlink_c* player_p = daAlink_getAlinkActorClass();

#if PLATFORM_SHIELD || PLATFORM_WII
    l_autoUpHeight = m_hio->m.m0D0;
    m_acchcir[0].SetWall(l_autoUpHeight + 0.1f, 60.0f);
    if (checkStateFlg0(FLG0_UNK_2000)) {
        m_normalMaxSpeedF = m_hio->m.kakariko_max_speed;
        m_lashAddSpeed = m_hio->m.kakariko_add_lash_speed;
    } else {
        m_normalMaxSpeedF = m_hio->m.max_speed;
        m_lashAddSpeed = m_hio->m.add_lash_speed;
    }
    field_0x1764 = m_hio->m.walk_to_fastwalk_rate * m_normalMaxSpeedF;
    m_lashMaxSpeedF = m_normalMaxSpeedF + m_lashAddSpeed;
#endif

    if (player_p->checkBoarSingleBattleFirst()) {
        onStateFlg0(FLG0_UNK_100000);
    } else if (player_p->checkBoarSingleBattleSecond()) {
        onStateFlg0(FLG0_UNK_200000);
    }

    if (m_acch.ChkGroundHit()) {
        onStateFlg0(FLG0_UNK_10);
    } else {
        offStateFlg0(FLG0_UNK_10);
    }

    m_resetStateFlg0 = 0;
    attention_info.flags &= ~(fopAc_AttnFlag_ETC_e | fopAc_AttnFlag_SPEAK_e);
    field_0x16b6 = 0;
    field_0x16e8 = shape_angle.y;
    field_0x1710 = field_0x170e;

    setDemoData();
    setStickData();
    m_morf_frame = -1.0f;
    setLashCnt();
    setTalkModeWolf();

    if (checkStateFlg0(FLG0_UNK_1) && m_anmIdx[2] == 0xFFFF && (m_tgco_cyl[0].ChkTgHit() || m_tgco_cyl[1].ChkTgHit() || m_tgco_cyl[2].ChkTgHit() || m_head_sph.ChkTgHit())) {
        setNeckAnime(ANM_HS_LASH, 1.0f, 0.0f, -1);
        m_sound.startCreatureVoice(Z2SE_HS_V_RUSH, -1);
    }

    if (m_cowHit != 0) {
        m_cowHit--;
    }

    if (m_procID != PROC_TURN_e && field_0x170c != 0) {
        field_0x170c--;
    }

    if (checkStateFlg0(FLG0_UNK_1)) {
        field_0x16bc = 0;
    }

#if DEBUG
    if (checkStateFlg0(FLG0_UNK_1) && daAlink_c::checkDebugMoveInput()) {
        if (l_debugMode) {
            l_debugMode = 0;
        } else {
            l_debugMode = 1;
        }
    }
#endif

    if (l_debugMode) {
        f32 f31 = 50.0f;
        if (mDoCPd_c::getHoldLockR(PAD_1)) {
            f31 = 100.0f;
        }

        if (mDoCPd_c::getHoldY(PAD_1)) {
            current.pos.y += f31;
        } else if (mDoCPd_c::getHoldX(PAD_1)) {
            current.pos.y -= f31;
        }
        current.pos.x += f31 * m_padStickValue * cM_ssin(m_padStickAngleY);
        current.pos.z += f31 * m_padStickValue * cM_scos(m_padStickAngleY);
        setMatrix();
        m_model->calc();
        setBodyPart();
    } else {
        animePlay();
        checkDemoAction();

        (this->*m_proc)();

        cXyz old_pos(current.pos);
        f32 old_speedF = speedF;
        speedF *= cM_scos(shape_angle.x);

        if (m_procID == PROC_JUMP_e) {
            fopAcM_posMoveF(this, NULL);
        } else {
            fopAcM_posMoveF(this, m_cc_stts.GetCCMoveP());
            if (m_cc_stts.GetCCMoveP()->abs() > 0.0001f) {
                s16 old_shape_y = shape_angle.y;
                s16 old_angle_y = current.angle.y;

                for (int i = 0; i < 8; i++) {
                    if (checkHorseNoMove(1) == 2) {
                        current.pos -= *m_cc_stts.GetCCMoveP();
                        break;
                    }

                    shape_angle.y += (s16)0x2000;
                    current.angle.y = shape_angle.y;
                }

                shape_angle.y = old_shape_y;
                current.angle.y = old_angle_y;
            }
        }

        speedF = old_speedF;

        m_cc_stts.ClrCcMove();
        f32 old_speed_y = speed.y;

        bgCheck();
        m_acch.CrrPos(dComIfG_Bgsp());

        if (checkStateFlg0(FLG0_UNK_4000)) {
            autoGroundHit();
        } else {
            current.pos.y = old_pos.y;
            speed.y = 0.0f;
        }

        if (m_procID == PROC_TOOL_DEMO_e) {
            J3DTransformInfo sp70;
            m_mtxcalc->getAnm(0)->getTransform(0, &sp70);

            mDoMtx_stack_c::transS(old_pos);
            mDoMtx_stack_c::YrotM(shape_angle.y);

            Vec sp54 = {0.0f, 0.0f, 0.0f};
            sp54.x = sp70.mTranslate.x;
            f32 f28 = sp70.mTranslate.z;
            sp54.z = f28;

            mDoMtx_stack_c::multVec(&sp54, &current.pos);
            if (field_0x1730 != 0 && -G_CM3D_F_INF != m_acch.GetGroundH()) {
                current.pos.y = m_acch.GetGroundH();
            }
        } else if (m_procID == PROC_JUMP_e) {
            speed.y = old_speed_y;
        }

        if (checkStateFlg0(FLG0_UNK_1) && m_acch.ChkGroundHit() && checkStateFlg0(FLG0_UNK_2)) {
            if (!checkStateFlg0(FLG0_UNK_1000)) {
                dComIfGp_getVibration().StartQuake(VIBMODE_S_DOKUTT, 1, cXyz(0.0f, 1.0f, 0.0f));
                onStateFlg0(FLG0_UNK_1000);
            }
        } else if (checkStateFlg0(FLG0_UNK_1000)) {
            dComIfGp_getVibration().StopQuake(0x1F);
            offStateFlg0(FLG0_UNK_1000);
        }

        setRoomInfo(0);
        setMatrix();
        footBgCheck();
        setTailAngle();

        m_model->calc();
        setBodyPart();

        if (m_procID == PROC_TOOL_DEMO_e) {
            cXyz sp48;
            mDoMtx_multVec(m_model->getAnmMtx(0), &cXyz::BaseX, &sp48);
            field_0x170e = cM_atan2s(-sp48.x, -sp48.z);
        } else {
            field_0x170e = shape_angle.y;
        }

        cXyz sp3C;
        cXyz sp30;
        mDoMtx_multVecZero(m_model->getAnmMtx(0xB), &sp3C);
        mDoMtx_multVecZero(m_model->getAnmMtx(0x14), &sp30);
        sp30 -= sp3C;

        field_0x1712 = sp30.atan2sX_Z() - field_0x170e;
        if (field_0x1712 > 0x2000) {
            field_0x1712 = 0x2000;
        } else if (field_0x1712 < -0x2000) {
            field_0x1712 = -0x2000;
        }

        if (checkResetStateFlg0(RFLG0_UNK_1)) {
            mDoMtx_stack_c::transS(m_bodyEyePos.x, m_bodyEyePos.y, m_bodyEyePos.z);
            mDoMtx_stack_c::YrotM(field_0x170e - field_0x1710);
            mDoMtx_stack_c::transM(-field_0x17b8.x, -field_0x17b8.y, -field_0x17b8.z);

            daHorseRein_c* rein_p = m_rein;
            cXyz* var_r26;
            for (int i = 0; i < 3; i++, rein_p++) {
                var_r26 = rein_p->field_0x0[0];
                for (int j = 0; j < rein_p->field_0x8[1]; j++, var_r26++) {
                    mDoMtx_stack_c::multVec(var_r26, var_r26);
                }
            }
        }

        field_0x17b8 = m_bodyEyePos;

        m_sound.framework(m_poly_sound, m_reverb);

        if (field_0x1144 != NULL) {
            m_sound.updateAnime(field_0x1144->getFrame(), field_0x1144->getRate());
        }
    }
    setEffect();
    setCollision();

    if (!checkStateFlg0(FLG0_UNK_1)) {
        setReinPosNormal();
        m_sound.setLinkRiding(false);
    } else {
        m_sound.setLinkRiding(true);
    }

    if (!player_p->checkHorseRide()) {
        m_lashCnt = 6;
    }

    if (!checkInputOnR() && fabsf(speedF) < 0.05f) {
        offStateFlg0(FLG0_UNK_8);
    } else if (fabsf(speedF) > 0.2f * m_hio->m.max_backward_speed || (m_anmIdx[0] == ANM_HS_WALK_START && m_frameCtrl[0].getFrame() > 2.0f)) {
        onStateFlg0(FLG0_UNK_8);
    }

    m_endResetStateFlg0 = 0;

    if (checkResetStateFlg0(RFLG0_UNK_200) || player_p->checkHorseZelda()) {
        m_modelData->getMaterialNodePointer(5)->getShape()->hide();
    } else {
        m_modelData->getMaterialNodePointer(5)->getShape()->show();
    }

    dMeter2Info_setHorseLifeCount(m_lashCnt);
    return 1;
}

static int daHorse_Execute(daHorse_c* i_this) {
    return i_this->execute();
}

int daHorse_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    if (checkStateFlg0(FLG0_NO_DRAW_WAIT) || checkResetStateFlg0(RFLG0_UNK_80)) {
        return 1;
    }

    g_env_light.setLightTevColorType_MAJI(m_model, &tevStr);

    if (dComIfGp_getCameraAttentionStatus(0) & 0x40) {
        m_modelData->getMaterialNodePointer(2)->getShape()->hide();
    } else {
        m_modelData->getMaterialNodePointer(2)->getShape()->show();
    }

    m_btp.entry(m_modelData, m_btpFrame);
    mDoExt_modelEntryDL(m_model);

    cXyz shadow_pos(current.pos.x, 100.0f + current.pos.y, current.pos.z);
    m_shadowID = dComIfGd_setShadow(m_shadowID, 0, m_model, &shadow_pos, 1000.0f, 0.0f, current.pos.y, m_acch.GetGroundH(), m_acch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    
    if (!checkResetStateFlg0(RFLG0_UNK_100) && (!checkStateFlg0(FLG0_UNK_1) || !daAlink_getAlinkActorClass()->checkHorseSubjectivity())) {
        static GXColor reinLineColor = {0x00, 0x00, 0x00, 0xFF};
        m_reinLine.update(field_0x1204, 1.5f, reinLineColor, 0, &tevStr);
        dComIfGd_set3DlineMat(&m_reinLine);
    }

    return 1;
}

static int daHorse_Draw(daHorse_c* i_this) {
    return i_this->draw();
}

daHorse_c::~daHorse_c() {
#if DEBUG
    if (m_hio) {
        mDoHIO_deleteChild(m_hio->id);
    }
#endif
    m_sound.deleteObject();
    dComIfG_resDelete(&m_phase, l_arcName);

    if (dComIfGp_getHorseActor() == this) {
        dComIfGp_setHorseActor(NULL);
    }
}

static int daHorse_Delete(daHorse_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    i_this->~daHorse_c();
    return 1;
}

static actor_method_class l_daHorse_Method = {
    (process_method_func)daHorse_Create,
    (process_method_func)daHorse_Delete,
    (process_method_func)daHorse_Execute,
    0,
    (process_method_func)daHorse_Draw,
};

extern actor_process_profile_definition g_profile_HORSE = {
  fpcLy_CURRENT_e,        // mLayerID
  4,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_HORSE,             // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daHorse_c),      // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  90,                     // mPriority
  &l_daHorse_Method,      // sub_method
  0x00060000,             // mStatus
  fopAc_UNK_GROUP_5_e,    // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance;
