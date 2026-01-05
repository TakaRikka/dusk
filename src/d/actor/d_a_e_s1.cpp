/**
* @file d_a_e_s1.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_s1.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

class daE_S1_HIO_c {
public:
    daE_S1_HIO_c();
    virtual ~daE_S1_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mBaseSize;
    /* 0x0C */ f32 mMoveSpeed;
    /* 0x10 */ u8 field_0x10[0x14 - 0x10];
    /* 0x14 */ f32 mDashSpeed;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ s16 mFallSEWaitTime;
    /* 0x1E */ s16 mAllDeadWaitTime;
    /* 0x20 */ f32 mReactionDist;
    /* 0x24 */ f32 mReactionAngle;
    /* 0x28 */ s16 mReactionTime;
    /* 0x2A */ u8 mInvincible;
};

#define ANM_ATTACK 5
#define ANM_ATTACK_02 6
#define ANM_DAMAGED 7
#define ANM_DASH_01 8
#define ANM_DASH_02 9
#define ANM_DEAD_02 10
#define ANM_DEAD_03 11
#define ANM_DEAD_04 12
#define ANM_DEADWAIT_02 13
#define ANM_DEADWAIT_03 14
#define ANM_DEADWAIT_04 15
#define ANM_DEADWAKE_02 16
#define ANM_DEADWAKE_03 17
#define ANM_DEADWAKE_04 18
#define ANM_DOWN 19
#define ANM_HANGED 20
#define ANM_HANG_DAMAGE 21
#define ANM_HANG_BRUSH 22
#define ANM_HANG_BRUSH2 23
#define ANM_HANG_WAIT 24
#define ANM_SHOUT 25
#define ANM_SHRINK 26
#define ANM_SHRINK_DOWN 27
#define ANM_STICK 28
#define ANM_WAIT_01 29
#define ANM_WAIT_02 30
#define ANM_WALK 31

enum daE_S1_ACTION {
    ACT_WAIT,
    ACT_ROOF,
    ACT_FIGHT_RUN,
    ACT_FIGHT,
    ACT_BIBIRI,
    ACT_DAMAGE,
    ACT_INVINCIBLE,  // does not exist anymore
    ACT_PATH,
    ACT_WOLFBITE,
    ACT_FAIL_WAIT,
    ACT_FAIL,
    ACT_SHOUT,
    ACT_WARP_APPEAR = 20,
};

daE_S1_HIO_c::daE_S1_HIO_c() {
    field_0x4 = -1;
    mBaseSize = 1.3f;
    mMoveSpeed = 4.0f;
    mDashSpeed = 20.0f;
    field_0x18 = 300.0f;
    mFallSEWaitTime = 20;
    mAllDeadWaitTime = 100;
    mReactionDist = 700.0f;
    mReactionAngle = 90.0f;
    mReactionTime = 20;
    mInvincible = 0;
}

static void anm_init(e_s1_class* i_this, int i_resNo, f32 i_morf, u8 i_attr, f32 i_speed) {
    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes("E_S2", i_resNo);
    i_this->mpMorf->setAnm(pbck, i_attr, i_morf, i_speed, 0.0f, -1.0f);
    i_this->mAnm = i_resNo;
}

static int daE_S1_Draw(e_s1_class* i_this) {
    if (i_this->field_0x306c != 0) {
        return 1;
    }

    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    J3DModel* model = i_this->mpMorf->getModel();

    g_env_light.settingTevStruct(2, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

    dComIfGd_setListDark();
    i_this->mpMorf->entryDL();

    if (i_this->mDrawShadow) {
        cXyz pos;
        pos.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f, a_this->current.pos.z);

        i_this->mShadowKey = dComIfGd_setShadow(
            i_this->mShadowKey, 1, model, &pos, BREG_F(19) + 1300.0f, 0.0f,
            a_this->current.pos.y, i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &i_this->tevStr,
            0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    GXColor line_color;
    line_color.r = JREG_S(0) + 5;
    line_color.g = JREG_S(1) + 10;
    line_color.b = JREG_S(2) + 10;
    line_color.a = 0xFF;

    i_this->mLineMat.update(16, line_color, &i_this->tevStr);
    dComIfGd_set3DlineMatDark(&i_this->mLineMat);

    dComIfGd_setList();
    return 1;
}

static BOOL pl_check(e_s1_class* i_this, f32 i_check_range) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 x_dist = player->current.pos.x - i_this->home.pos.x;
    f32 z_dist = player->current.pos.z - i_this->home.pos.z;

    if (JMAFastSqrt(x_dist * x_dist + z_dist * z_dist) < i_check_range) {
        s16 angle_dist = i_this->shape_angle.y - fopAcM_searchPlayerAngleY(i_this);
        if (angle_dist < 0x7000 && angle_dist > -0x7000 &&
            !fopAcM_otherBgCheck(i_this, dComIfGp_getPlayer(0)))
        {
            return true;
        }
    }

    return false;
}

static BOOL pl_at_check(e_s1_class* i_this, f32 i_check_range) {
    return fopAcM_searchPlayerDistance(i_this) < i_check_range;
}

static u8 l_no_fail;  // when enabled, enemy will never revive after defeated
static u8 hio_set;

static daE_S1_HIO_c l_HIO;

static BOOL path_check(e_s1_class* i_this) {
    static u8 check_index[255];
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    dBgS_LinChk linchk;
    cXyz start_pos;
    cXyz end_pos;

    start_pos = a_this->current.pos;
    start_pos.y += WREG_F(0) + 10.0f;

    dPnt* point = i_this->mpPath->m_points;
    for (int i = 0; i < i_this->mpPath->m_num; i++, point++) {
        if (i < 255) {
            end_pos.x = point->m_position.x;
            end_pos.y = point->m_position.y + 10.0f;
            end_pos.z = point->m_position.z;
            linchk.Set(&start_pos, &end_pos, a_this);

            if (!dComIfG_Bgsp().LineCross(&linchk)) {
                check_index[i] = 1;
            } else {
                check_index[i] = 0;
            }
        }
    }

    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 range = 100.0f;
    u8 found = false;

    for (int i = 0; i < 100; i++) {
        point = i_this->mpPath->m_points;
        for (int j = 0; j < i_this->mpPath->m_num; j++, point++) {
            if (j < 255 && check_index[j]) {
                x_dist = a_this->current.pos.x - point->m_position.x;
                y_dist = a_this->current.pos.y - point->m_position.y;
                z_dist = a_this->current.pos.z - point->m_position.z;

                if (JMAFastSqrt((z_dist * z_dist) + ((x_dist * x_dist) + (y_dist * y_dist))) <
                    range)
                {
                    i_this->mCurrentPathPointNo = j - i_this->mPathDirection;

                    if (i_this->mCurrentPathPointNo >= (i_this->mpPath->m_num & 0xFF)) {
                        i_this->mCurrentPathPointNo = i_this->mpPath->m_num;
                    } else if (i_this->mCurrentPathPointNo < 0) {
                        i_this->mCurrentPathPointNo = 0;
                    }

                    found = true;
                    break;
                }
            }
        }

        if (found) {
            break;
        }

        range += 50.0f;
    }

    if (!found) {
        return false;
    }

    if (cM_rndF(1.0f) < 0.5f) {
        i_this->mPathDirection = 1;
    } else {
        i_this->mPathDirection = -1;
    }

    return true;
}

static void path_check2(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    dBgS_LinChk linchk;
    cXyz start_pos;
    cXyz end_pos;

    start_pos = a_this->current.pos;
    start_pos.y += WREG_F(2) + 20.0f;

    end_pos = i_this->mPathTargetPos;
    end_pos.y += WREG_F(3) + 10.0f;

    linchk.Set(&start_pos, &end_pos, a_this);
    if (dComIfG_Bgsp().LineCross(&linchk)) {
        i_this->mAction = ACT_PATH;
        i_this->mMode = 0;
    }
}

static int same_id;

static int fail_id;

static void* s_last_sub(void* i_actor, void* i_data) {
    fopAc_ac_c* a_actor = (fopAc_ac_c*)i_actor;
    fopAc_ac_c* a_data = (fopAc_ac_c*)i_data;

    if (fopAcM_IsActor(a_actor) && fopAcM_GetName(a_actor) == PROC_E_S1) {
        e_s1_class* e_data = (e_s1_class*)a_data;
        e_s1_class* e_actor = (e_s1_class*)a_actor;

        if (e_actor != e_data && e_actor->mGroupID == e_data->mGroupID) {
            same_id++;

            if (e_actor->mAction == ACT_FAIL || e_actor->mAction == ACT_FAIL_WAIT) {
                fail_id++;
            }
        }
    }

    return NULL;
}

static void damage_check(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (i_this->mHitInvincibilityTimer == 0) {
        i_this->mCcStts.Move();

        for (int i = 0; i <= 2; i++) {
            if (l_HIO.mInvincible) {
                i_this->mCcSph[i].OnTgShield();
            } else {
                i_this->mCcSph[i].OffTgShield();
            }

            if (i_this->mCcSph[i].ChkTgHit()) {
                i_this->mHitInvincibilityTimer = 6;
                i_this->mAtInfo.mpCollider = i_this->mCcSph[i].GetTgHitObj();

                if (player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_LEFT && player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_RIGHT &&
                    i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK))
                {
                    if (!player->onWolfEnemyHangBite(a_this)) {
                        return;
                    }

                    OS_REPORT("S1 PL BITE HANG \n");
                    i_this->mAction = ACT_WOLFBITE;
                    i_this->mMode = 0;
                    i_this->mHitInvincibilityTimer = 1000;
                    i_this->mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x1F);
                    return;
                }

                at_power_check(&i_this->mAtInfo);
                OS_REPORT("E_S1 HIT POW %d\n", i_this->mAtInfo.mAttackPower);

                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                    i_this->mTimers[3] = 30;
                } else {
                    i_this->mTimers[3] = 0;
                }

                if (l_HIO.mInvincible) {
                    i_this->mAction = ACT_INVINCIBLE;

                    if (i_this->mAtInfo.mAttackPower < 20) {
                        i_this->mTimers[0] = 15;
                        i_this->field_0x6ac = TREG_F(0) + 30.0f;
                    } else {
                        i_this->mTimers[0] = 25;
                        i_this->field_0x6ac = TREG_F(1) + 65.0f;
                    }
                } else {
                    if (i_this->mAtInfo.mAttackPower >= 60 && daPy_py_c::checkNowWolf()) {
                        i_this->mAtInfo.field_0x18 = 36;
                    } else {
                        i_this->mAtInfo.field_0x18 = 0;
                    }

                    cc_at_check(a_this, &i_this->mAtInfo);

                    if (i_this->mAtInfo.mpCollider->ChkAtType(
                            AT_TYPE_MIDNA_LOCK | AT_TYPE_10000000 | AT_TYPE_WOLF_CUT_TURN |
                            AT_TYPE_WOLF_ATTACK))
                    {
                        i_this->mHitInvincibilityTimer = 20;
                    } else {
                        i_this->mHitInvincibilityTimer = 10;
                    }

                    if (i_this->mAtInfo.mAttackPower < 20) {
                        i_this->field_0x6ac = TREG_F(0) + 20.0f;
                    } else if (i_this->mAtInfo.mAttackPower >= 60 && daPy_py_c::checkNowWolf()) {
                        i_this->field_0x6ac = 0.0f;
                        a_this->health = 0;

                        cXyz offset;
                        cXyz pos;

                        MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(2), *calc_mtx);

                        offset.set(0.0f, 0.0f, 0.0f);
                        MtxPosition(&offset, &pos);

                        cXyz size(l_HIO.mBaseSize, l_HIO.mBaseSize, l_HIO.mBaseSize);
                        csXyz angle(player->shape_angle);
                        angle.y -= 0x8000;

                        dComIfGp_particle_set(0x8248, &pos, &angle, &size);
                        dComIfGp_particle_set(0x8249, &pos, &angle, &size);
                        dComIfGp_particle_set(0x824A, &a_this->current.pos, &angle, &size);
                    } else {
                        i_this->field_0x6ac = TREG_F(1) + 60.0f;
                    }

                    if (!l_no_fail) {
                        if (i_this->mAtInfo.mAttackPower >= 60 && daPy_py_c::checkNowWolf()) {
                            i_this->mAction = ACT_FAIL_WAIT;
                            i_this->mHitInvincibilityTimer = 10;
                            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_DEATH, -1);
                        } else if (a_this->health <= 0) {
                            if (i_this->mAtInfo.mHitBit & 0x880) {
                                OS_REPORT("////////// S1 TURN CUT FAIL ..\n");
                                i_this->mAction = ACT_FAIL_WAIT;
                                i_this->mHitInvincibilityTimer = 10;
                                i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_DEATH, -1);
                            } else {
                                fail_id = 0;
                                same_id = 0;

                                fpcM_Search(s_last_sub, a_this);

                                OS_REPORT("////////// SAME ID %d\n", same_id);
                                OS_REPORT("////////// FAIL ID %d\n", fail_id);

                                if (i_this->mGroupID == 0xFF || same_id != fail_id) {
                                    i_this->mAction = ACT_FAIL_WAIT;
                                    i_this->mHitInvincibilityTimer = 10;
                                    i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_DEATH, -1);
                                }
                            }
                        } else {
                            i_this->mAction = ACT_DAMAGE;
                            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_DAMAGE, -1);
                        }
                    } else if (a_this->health <= 0) {
                        i_this->mAction = ACT_FAIL;
                        i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_DEATH, -1);
                    } else {
                        i_this->mAction = ACT_DAMAGE;
                        i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_DAMAGE, -1);
                    }
                }

                i_this->mMode = 0;
                a_this->speedF = 0.0f;
                i_this->field_0x6b0 = fopAcM_searchPlayerAngleY(a_this);
                i_this->mDrawShadow = true;
                break;
            }
        }

        for (int i = 0; i < 3; i++) {
            if (a_this->health <= 1) {
                i_this->mCcSph[i].SetTgHitMark(CcG_Tg_UNK_MARK_3);
            }
        }
    }
}

static void e_s1_wait(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz dist_from_home;
    cXyz random_home;

    f32 target_speed = 0.0f;
    s16 angle_step = 0;
    int anm_frame = i_this->mpMorf->getFrame();

    dist_from_home = a_this->home.pos - a_this->current.pos;

    f32 var_f30 = JMAFastSqrt(dist_from_home.x * dist_from_home.x + dist_from_home.z * dist_from_home.z);

    switch (i_this->mMode) {
    case 0:
        if (i_this->mTimers[0] == 0) {
            if (i_this->mpPath != NULL && path_check(i_this)) {
                i_this->mAction = ACT_PATH;
                i_this->mMode = 0;
            } else {
                random_home.x = a_this->home.pos.x + cM_rndFX(300.0f);
                random_home.y = a_this->home.pos.y;
                random_home.z = a_this->home.pos.z + cM_rndFX(300.0f);

                dist_from_home = random_home - a_this->current.pos;

                s16 var_r27 = cM_atan2s(dist_from_home.x, dist_from_home.z) - a_this->current.angle.y;
                if (var_r27 > 0x3000) {
                    var_r27 = 0x3000;
                } else if (var_r27 < -0x3000) {
                    var_r27 = -0x3000;
                }

                i_this->mTargetAngle = a_this->current.angle.y + var_r27;
                i_this->mTimers[0] = cM_rndF(100.0f) + 100.0f;
                i_this->mMode = 1;
                anm_init(i_this, ANM_WALK, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
        }
        break;
    case 1:
        angle_step = 0x100;
        if (i_this->mTimers[0] == 0 || var_f30 > i_this->field_0x3068) {
            i_this->mMode = 0;
            i_this->mTimers[0] = cM_rndF(100.0f) + 100.0f;
            anm_init(i_this, ANM_WAIT_02, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;
    case 2:
        i_this->mMode = 3;
        anm_init(i_this, ANM_WALK, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        /* fallthrough */
    case 3:
        angle_step = 0x100;

        dist_from_home.x = a_this->home.pos.x - a_this->current.pos.x;
        dist_from_home.z = a_this->home.pos.z - a_this->current.pos.z;
        i_this->mTargetAngle = cM_atan2s(dist_from_home.x, dist_from_home.z);

        if (JMAFastSqrt(dist_from_home.x * dist_from_home.x + dist_from_home.z * dist_from_home.z) < i_this->field_0x3068 - 50.0f) {
            i_this->mMode = 1;
        }
        break;
    }

    if (i_this->mAnm == ANM_WALK && ((anm_frame >= 0 && anm_frame <= 18) || (anm_frame >= 27 && anm_frame <= 45))) {
        target_speed = l_HIO.mMoveSpeed;
    }

    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 1.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetAngle, 8, angle_step);

    if (i_this->mTimers[2] == 0 && pl_check(i_this, i_this->mSearchRange)) {
        i_this->mAction = ACT_FIGHT_RUN;
        i_this->mMode = 0;
    }
}

static void e_s1_roof(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    s8 roof_wait = false;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_STICK, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        roof_wait = true;
        if (i_this->mSwBit != 0xFF) {
            if (dComIfGs_isSwitch(i_this->mSwBit, dComIfGp_roomControl_getStayNo())) {
                i_this->mMode = 2;
            }
        } else if (fopAcM_searchPlayerDistanceXZ(a_this) < i_this->mPrm1 * 100.0f) {
            i_this->mMode = 2;
        }
        break;
    case 2:
        i_this->mSound.startCreatureSound(Z2SE_EN_NS_FALLTREE, 0, -1);
        i_this->mMode = 3;
        i_this->mTimers[0] = l_HIO.mFallSEWaitTime;
        /* fallthrough */
    case 3:
        if (i_this->mTimers[0] == 0) {
            i_this->mDrawShadow = true;

            if (i_this->mAcch.ChkGroundHit()) {
                anm_init(i_this, ANM_DOWN, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mMode = 4;
                i_this->mSound.startCreatureSound(Z2SE_EN_NS_DOSA, 0, -1);
            }
        } else {
            roof_wait = true;
        }
        break;
    case 4:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_WAIT;
            i_this->mMode = 0;
        }
        break;
    }

    if (roof_wait) {
        a_this->speed.y = 0.0f;
        a_this->current.pos.y = a_this->home.pos.y;
    }
}

static void e_s1_fight_run(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp14;
    cXyz sp8;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_DASH_01, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mpMorf->setFrame(cM_rndF(10.0f));
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 4, 0x1000);
        cLib_addCalc2(&a_this->speedF, l_HIO.mDashSpeed, 1.0f, l_HIO.mDashSpeed * 0.333f);
        break;
    }

    if (!pl_check(i_this, i_this->mSearchRange + 50.0f)) {
        i_this->mAction = ACT_WAIT;
        i_this->mMode = 2;
    } else if (pl_at_check(i_this, l_HIO.field_0x18)) {
        i_this->mAction = ACT_FIGHT;
        i_this->mMode = 0;
    }
}

static void e_s1_fight(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp28;
    cXyz sp1C;

    int anm_frame = i_this->mpMorf->getFrame();
    f32 target_speed = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_WAIT_01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mTimers[0] = 30.0f + cM_rndF(30.0f);
        i_this->mMode = 1;
        break;
    case 1:
        if (i_this->mTimers[0] == 0) {
            if (cM_rndF(1.0f) < 0.3333f) {
                anm_init(i_this, ANM_ATTACK, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_ATTACK, -1);
                i_this->mIsSlowAttack = 0;
            } else {
                anm_init(i_this, ANM_ATTACK_02, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_ATTACK2, -1);
                i_this->mIsSlowAttack = 1;
            }

            i_this->mMode = 2;
        }
        break;
    case 2:
        if (i_this->mIsSlowAttack == 0 && anm_frame >= 21 && anm_frame <= 30) {
            i_this->field_0x6b9 = 1;

            if (anm_frame == 21) {
                i_this->mSound.startCreatureSound(Z2SE_EN_NS_UDEHURI, 0, -1);
            }

            if (anm_frame == 25) {
                i_this->mSound.startCreatureSound(Z2SE_EN_NS_HAND_L, 0, -1);
            }
        } else if (i_this->mIsSlowAttack == 1 && anm_frame >= 25 && anm_frame <= 33) {
            i_this->field_0x6b9 = 1;

            if (anm_frame == 25) {
                i_this->mSound.startCreatureSound(Z2SE_EN_NS_UDEHURI, 0, -1);
            }
        }

        if (i_this->mpMorf->isStop()) {
            if (!pl_at_check(i_this, l_HIO.field_0x18 + 10.0f)) {
                i_this->mAction = ACT_FIGHT_RUN;
            }

            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 4, 0x800);
    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 10.0f);
}

static void e_s1_bibiri(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_SHRINK, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f + cM_rndFX(0.1f));
        i_this->mpMorf->setFrame(cM_rndF(15.0f));
        i_this->mTimers[0] = l_HIO.mReactionTime + cM_rndF(10.0f);
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        if (pl_at_check(i_this, 10.0f + l_HIO.mReactionDist)) {
            if (daPy_getPlayerActorClass()->checkWolfThreat()) {
                i_this->mTimers[0] = l_HIO.mReactionTime + cM_rndF(10.0f);
            }
        }

        if (i_this->mTimers[0] == 0) {
            anm_init(i_this, ANM_SHRINK_DOWN, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 2;
        }
        break;
    case 2:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_WAIT;
            i_this->mMode = 2;
        }
        break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 4, 0x1000);
}

static void e_s1_damage(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp14;
    cXyz sp8;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_DAMAGED, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        break;
    case 1:
        if (i_this->mTimers[3] != 0) {
            i_this->onHeadLockFlg();
        }

        if (i_this->mpMorf->isStop()) {
            i_this->field_0x6ac = 0.0f;
            i_this->mAction = ACT_FIGHT_RUN;
            i_this->mMode = 0;
        }
        break;
    }
}

static void e_s1_path(e_s1_class* i_this) {
    cXyz sp14;
    cXyz sp8;
    f32 target_speed = 0.0f;
    int anm_frame = i_this->mpMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        anm_init(i_this, ANM_WALK, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        /* fallthrough */
    case 1:
        i_this->mCurrentPathPointNo += i_this->mPathDirection;

        if (i_this->mCurrentPathPointNo >= (i_this->mpPath->m_num & 0xFF)) {
            if (dPath_ChkClose(i_this->mpPath)) {
                i_this->mCurrentPathPointNo = 0;
            } else {
                i_this->mPathDirection = -1;
                i_this->mCurrentPathPointNo = (i_this->mpPath->m_num - 2);
            }
        } else if (i_this->mCurrentPathPointNo < 0) {
            i_this->mPathDirection = 1;
            i_this->mCurrentPathPointNo = 1;
        }

        {
            dPnt* point = &i_this->mpPath->m_points[i_this->mCurrentPathPointNo];
            i_this->mPathTargetPos.x = point->m_position.x;
            i_this->mPathTargetPos.y = point->m_position.y;
            i_this->mPathTargetPos.z = point->m_position.z;
        }
        i_this->mMode = 2;
        /* fallthrough */
    case 2:
        if ((anm_frame >= 12 && anm_frame <= 27) || (anm_frame >= 54 && anm_frame <= 86)) {
            target_speed = l_HIO.mMoveSpeed;
        }

        sp14.x = i_this->mPathTargetPos.x - i_this->current.pos.x;
        sp14.z = i_this->mPathTargetPos.z - i_this->current.pos.z;
        i_this->mTargetAngle = cM_atan2s(sp14.x, sp14.z);

        if (JMAFastSqrt((sp14.x * sp14.x) + (sp14.z * sp14.z)) < 4.0f * l_HIO.mMoveSpeed) {
            i_this->mMode = 3;
            i_this->mTimers[0] = 50.0f + cM_rndF(100.0f);
            anm_init(i_this, ANM_WAIT_02, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }

        path_check2(i_this);
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngle, 8, 0x200);
        break;
    case 3:
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, 1.0f);

    if (pl_check(i_this, i_this->mSearchRange)) {
        i_this->mAction = ACT_FIGHT_RUN;
        i_this->mMode = 0;
    }
}

static void* s_down_sub(void* i_actor, void* i_data) {
    fopAc_ac_c* a_actor = (fopAc_ac_c*)i_actor;
    fopAc_ac_c* a_data = (fopAc_ac_c*)i_data;

    if (fopAcM_IsActor(a_actor) && fopAcM_GetName(a_actor) == PROC_E_S1) {
        e_s1_class* e_data = (e_s1_class*)a_data;
        e_s1_class* e_actor = (e_s1_class*)a_actor;

        if (e_actor == e_data) {
            return NULL;
        }

        if (e_actor->mAction == ACT_FAIL && e_actor->mMode == 10 && e_actor->mTimers[1] == 0) {
            return NULL;
        }

        if (e_actor->mGroupID == e_data->mGroupID) {
            return i_actor;
        }
    }

    return NULL;
}

static void* s_fail_sub(void* i_actor, void* i_data) {
    fopAc_ac_c* a_actor = (fopAc_ac_c*)i_actor;
    fopAc_ac_c* a_data = (fopAc_ac_c*)i_data;

    if (fopAcM_IsActor(a_actor) && fopAcM_GetName(a_actor) == PROC_E_S1) {
        e_s1_class* e_data = (e_s1_class*)a_data;
        e_s1_class* e_actor = (e_s1_class*)a_actor;

        if (e_actor == e_data) {
            return NULL;
        }

        if (e_actor->mAction == ACT_FAIL && e_actor->mMode <= 10) {
            return NULL;
        }

        if (e_actor->mGroupID != e_data->mGroupID) {
            return NULL;
        }

        return i_actor;
    }

    return NULL;
}

static BOOL all_fail_check(e_s1_class* i_this) {
    return fpcM_Search(s_fail_sub, i_this) == NULL ? TRUE : FALSE;
}

static void* s_allfail_sub(void* i_actor, void* i_data) {
    fopAc_ac_c* a_actor = (fopAc_ac_c*)i_actor;
    fopAc_ac_c* a_data = (fopAc_ac_c*)i_data;

    if (fopAcM_IsActor(a_actor) && fopAcM_GetName(a_actor) == PROC_E_S1) {
        e_s1_class* e_data = (e_s1_class*)a_data;
        e_s1_class* e_actor = (e_s1_class*)a_actor;

        if (e_actor->mGroupID == e_data->mGroupID) {
            e_actor->mAction = ACT_FAIL;
            e_actor->mMode = 11;
            e_actor->mTimers[0] = cM_rndF(10.0f) + 30.0f;
        }
    }

    return NULL;
}

static void all_fail(e_s1_class* i_this) {
    fpcM_Search(s_allfail_sub, i_this);
}

static void* s_allwakeup_sub(void* i_actor, void* i_data) {
    fopAc_ac_c* a_actor = (fopAc_ac_c*)i_actor;
    fopAc_ac_c* a_data = (fopAc_ac_c*)i_data;

    if (fopAcM_IsActor(a_actor) && fopAcM_GetName(a_actor) == PROC_E_S1) {
        e_s1_class* e_data = (e_s1_class*)a_data;
        e_s1_class* e_actor = (e_s1_class*)a_actor;

        if (e_actor != e_data && e_actor->mGroupID == e_data->mGroupID) {
            e_actor->mMode = 12;

            if (dComIfGp_event_runCheck()) {
                e_actor->mTimers[0] = 50;
            } else {
                e_actor->mTimers[0] = 0;
            }
        }
    }

    return NULL;
}

static int s_check;

static cXyz s_p[2] = {
    cXyz(-16500.0f, 0.0f, -100.0f),
    cXyz(-16300.0f, 0.0f, -400.0f),
};

static s16 s_ya[2] = {0, 0};

static void* s_posrevise_sub(void* i_actor, void* i_data) {
    fopAc_ac_c* a_actor = (fopAc_ac_c*)i_actor;
    fopAc_ac_c* a_data = (fopAc_ac_c*)i_data;

    if (fopAcM_IsActor(a_actor) && fopAcM_GetName(a_actor) == PROC_E_S1) {
        e_s1_class* e_data = (e_s1_class*)a_data;
        e_s1_class* e_actor = (e_s1_class*)a_actor;

        if (e_actor != e_data && s_check <= 1) {
            e_actor->current.pos = s_p[s_check];
            e_actor->old.pos = s_p[s_check];
            e_actor->current.angle.y = s_ya[s_check];
            e_actor->shape_angle.y = s_ya[s_check];

            s_check++;
        }
    }

    return NULL;
}

static void e_s1_failwait(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    i_this->mHitInvincibilityTimer = 20;

    switch (i_this->mMode) {
    case 0:
        if (cM_rndF(1.0f) < 0.3333f) {
            anm_init(i_this, ANM_DEAD_02, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else if (cM_rndF(1.0f) < 0.5f) {
            anm_init(i_this, ANM_DEAD_03, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else {
            anm_init(i_this, ANM_DEAD_04, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        }

        i_this->mMode = 1;
        break;
    case 1:
        if (i_this->mpMorf->checkFrame(9.0f)) {
            i_this->mMode = 2;

            if (i_this->mAnm == ANM_DEAD_02) {
                anm_init(i_this, ANM_DEADWAIT_02, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            } else if (i_this->mAnm == ANM_DEAD_03) {
                anm_init(i_this, ANM_DEADWAIT_03, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            } else {
                anm_init(i_this, ANM_DEADWAIT_04, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
        }
        break;
    case 2:
        // TODO: try to fix checkNowWolf here
        if (i_this->mAtInfo.mAttackPower >= 60 && ((daPy_py_c*)dComIfGp_getLinkPlayer())->checkWolf())
        {
            if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_WOLF_LOCK) {
                i_this->mMode = 3;
                i_this->mTimers[0] = (fopAcM_GetID(i_this) & 7);
            }
        } else {
            i_this->mMode = 3;
            i_this->mTimers[0] = 0;
        }
        break;
    case 3:
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACT_FAIL;
            i_this->mMode = 1;

            if (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_FIELD &&
                all_fail_check(i_this))
            {
                if (!dComIfGp_event_runCheck()) {
                    i_this->mDemoMode = 1;
                    anm_init(i_this, ANM_DEAD_02, 2.0f, 0, 1.0f);
                }
            } else if (i_this->mAnm == ANM_DEADWAIT_02) {
                anm_init(i_this, ANM_DEAD_02, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            } else if (i_this->mAnm == ANM_DEADWAIT_03) {
                anm_init(i_this, ANM_DEAD_03, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            } else {
                anm_init(i_this, ANM_DEAD_04, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }

            i_this->mpMorf->setFrame(10.0f);
        }
        break;
    }
}

static void e_s1_fail(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    i_this->mHitInvincibilityTimer = 20;
    stage_stag_info_class* staginfo = dComIfGp_getStage()->getStagInfo();

    switch (i_this->mMode) {
    case 0:
        if (cM_rndF(1.0f) < 0.3333f) {
            anm_init(i_this, ANM_DEAD_02, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else if (cM_rndF(1.0f) < 0.5f) {
            anm_init(i_this, ANM_DEAD_03, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else {
            anm_init(i_this, ANM_DEAD_04, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        }

        if (dStage_stagInfo_GetSTType(staginfo) == ST_FIELD) {
            i_this->mMode = 1;

            if (all_fail_check(i_this) && !dComIfGp_event_runCheck()) {
                i_this->mDemoMode = 1;
                anm_init(i_this, ANM_DEAD_02, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }
        } else {
            i_this->mMode = 30;
        }
        break;
    case 1:
        if (i_this->mpMorf->checkFrame(25.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_DOSA, 0, -1);
        }

        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 10;
            i_this->mTimers[1] = 0;
        }
        break;
    case 10:
        if (all_fail_check(i_this)) {
            i_this->mTimers[0] = cM_rndF(10.0f) + 30.0f;
            i_this->mMode = 11;

            all_fail(i_this);
        }
        break;
    case 11:
        if (i_this->mTimers[0] == 0) {
            cXyz offset;
            cXyz pos;

            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(2), *calc_mtx);

            offset.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&offset, &pos);

            if (i_this->mDemoMode != 0) {
                i_this->mMode = 20;
            } else {
                fopAcM_delete(a_this);
                fopAcM_createDisappear(a_this, &pos, 12, 1, 49);
            }

            if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 && i_this->mSwBit != 0xFF)
            {
                dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
            }
        }
        break;
    case 12:
        if (i_this->mTimers[0] == 0) {
            if (i_this->mAnm == ANM_DEAD_02) {
                anm_init(i_this, ANM_DEADWAKE_02, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            } else if (i_this->mAnm == ANM_DEAD_03) {
                anm_init(i_this, ANM_DEADWAKE_03, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            } else {
                anm_init(i_this, ANM_DEADWAKE_04, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }

            a_this->health = 50;

            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_AWAKE, -1);

            if (dComIfGp_event_runCheck()) {
                i_this->mMode = 13;
            } else {
                i_this->mMode = 15;
            }
        }
        break;
    case 13:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_WAIT;
            i_this->mMode = 1;
            i_this->mTimers[0] = 0;
            i_this->mTimers[2] = 40;
        }
        break;
    case 15:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_WAIT;
            i_this->mMode = 2;
        }
        break;
    case 20:
        if (i_this->mDemoTimer == TREG_S(0) + 80) {
            cXyz sp50;
            cXyz sp5C;

            i_this->mMode = 21;
            i_this->field_0x306c = 1;
            fopAcM_createDisappear(a_this, &a_this->eyePos, 12, 2, 49);
        }
        break;
    case 30:
        if (i_this->mpMorf->checkFrame(25.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_DOSA, 0, -1);
        }

        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 11;
            i_this->mTimers[0] = cM_rndF(10.0f) + 30.0f;
        }
        break;
    }
}

static void e_s1_shout(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    i_this->mHitInvincibilityTimer = 10;

    switch (i_this->mMode) {
    case 0:
        a_this->health = 50;
        anm_init(i_this, ANM_SHOUT, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);

        i_this->mMode = 1;
        i_this->mTimers[0] = KREG_S(6) + 10;
        i_this->mTimers[1] = KREG_S(6) + 40;

        i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_SHOUT, -1);

        if (strcmp(dComIfGp_getStartStageName(), "F_SP108") == 0 &&
            !dComIfGs_isSwitch(14, fopAcM_GetRoomNo(a_this)))
        {
            i_this->mDemoMode = 10;
            a_this->current.pos.set(-16000.0f, 0.0f, -1000.0f);
            a_this->shape_angle.y = 0;
            a_this->current.angle.y = 0;

            s_check = 0;
            fpcM_Search(s_posrevise_sub, i_this);
        }
        break;
    case 1:
        if (i_this->mDemoMode != 0) {
            dComIfGp_getVibration().StartShock(8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->mTimers[0] > KREG_S(7) + 5) {
            daPy_getPlayerActorClass()->onNsScream();
        } else {
            daPy_getPlayerActorClass()->onNsScreamAnm();
        }

        if (i_this->mTimers[1] == 1) {
            fpcM_Search(s_allwakeup_sub, i_this);
        }

        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_WAIT;

            if (dComIfGp_event_runCheck()) {
                i_this->mTimers[2] = 150;
                i_this->mTimers[0] = 0;
                i_this->mMode = 1;
            } else {
                i_this->mMode = 2;
            }
        }
        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 3.0f);
}

static void e_s1_warpappear(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    i_this->mHitInvincibilityTimer = 20;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_DOWN, 1.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        a_this->current.pos.y = a_this->home.pos.y + 50000.0f;
        a_this->speedF = 0.0f;
        a_this->speed.y = 0.0f;
        break;
    case 2:
        i_this->mTimers[0] = 10;
        break;
    case 10:
        if (i_this->mTimers[0] != 0) {
            break;
        }

        if (!i_this->mAcch.ChkGroundHit()) {
            break;
        }

        a_this->current = a_this->home;
        a_this->old = a_this->current;
        a_this->speedF = 0.0f;

        i_this->mMode = 11;
        i_this->mSound.startCreatureSound(Z2SE_EN_NS_DOSA, 0, -1);
        /* fallthrough */
    case 11:
        i_this->mMode = 12;
        anm_init(i_this, ANM_DOWN, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        break;
    case 12:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_WAIT_02, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 13;
        }
        break;
    case 20:
        i_this->mAction = ACT_WAIT;
        i_this->mMode = 2;
        break;
    }
}

static void ke_control(e_s1_class* i_this, s1_ke_s* i_ke, int i_no, f32 i_posZ) {
    cXyz sp4C;
    cXyz sp40;
    int i;

    cXyz* var_r31 = &i_ke->field_0x0[1];
    cXyz* var_r30 = &i_ke->field_0xc0[1];

    sp4C.x = 0.0f;
    sp4C.y = 0.0f;
    sp4C.z = i_posZ;

    cXyz sp34;
    f32 sp28 = -12.0f + JREG_F(0);
    s16 sp14 = cM_rndF2(65536.0f);

    f32 temp_f26 = 3.0f + JREG_F(8);
    f32 sp2C;
    f32 var_f29;
    f32 temp_f25 = JREG_F(5) + (3.0f + i_this->mAcch.GetGroundH());
    f32 temp_f27 = 0.8f + JREG_F(17);

    for (i = 1; i < 16; i++, var_r31++, var_r30++) {
        f32 sp24 = temp_f26 * cM_ssin((s16)(sp14 + (i * 7000)));
        f32 sp20 = temp_f26 * cM_ssin((s16)(sp14 + 10000 + (i * 6000)));
        f32 temp_f28 = (16 - i) * 0.1f;

        sp34.x = var_r30->x + (sp24 + (i_ke->field_0x180.x * temp_f28));
        sp34.y = var_r30->y + (i_ke->field_0x180.y * temp_f28);
        sp34.z = var_r30->z + (sp20 + (i_ke->field_0x180.z * temp_f28));

        sp2C = sp34.x + (var_r31->x - var_r31[-1].x);
        
        f32 temp_f31;
        f32 temp_f30 = sp34.z + (var_r31->z - var_r31[-1].z);
        
        var_f29 = sp28 + (var_r31->y + sp34.y);
        if (var_f29 < temp_f25) {
            var_f29 = temp_f25;
        }

        temp_f31 = var_f29 - var_r31[-1].y;
        s16 sp18 = (s16)-cM_atan2s(temp_f31, temp_f30);
        s16 sp16 = (s16)cM_atan2s(sp2C, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));

        cMtx_XrotS(*calc_mtx, sp18);
        cMtx_YrotM(*calc_mtx, sp16);
        MtxPosition(&sp4C, &sp40);
        *var_r30 = *var_r31;

        var_r31->x = var_r31[-1].x + sp40.x;
        var_r31->y = var_r31[-1].y + sp40.y;
        var_r31->z = var_r31[-1].z + sp40.z;

        var_r30->x = temp_f27 * (var_r31->x - var_r30->x);
        var_r30->y = temp_f27 * (var_r31->y - var_r30->y);
        var_r30->z = temp_f27 * (var_r31->z - var_r30->z);
    }
}

static void ke_move(e_s1_class* i_this, mDoExt_3DlineMat0_c* i_line, s1_ke_s* i_ke, int i_no,
                    f32 i_posZ) {
    ke_control(i_this, i_ke, i_no, i_posZ);

    cXyz* ppos = i_line->getPos(i_no);
    f32* psize = i_line->getSize(i_no);

    for (int i = 0; i < 16; i++, ppos++, psize++) {
        *ppos = i_ke->field_0x0[i];

        if (i == JREG_S(7) + 14) {
            *psize = (JREG_S(8) + 6) * 0.1f;
        } else if (i < 5) {
            *psize = 3.5f;
        } else {
            *psize = cM_rndF2(1.0f) + 1.8f;
        }
    }
}

static void e_s1_wolfbite(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopEn_enemy_c* e_this = (fopEn_enemy_c*)a_this;
    i_this->mHitInvincibilityTimer = 10;

    cLib_addCalc0(&a_this->speedF, 1.0f, 2.0f);
    i_this->mAcchCir.SetWall(50.0f, 250.0f + nREG_F(11));

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_HANGED, 3.0f, 0, 1.0f);
        i_this->mMode = 1;
        i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_HANGED, -1);
        a_this->health -= 5;
        i_this->field_0x6bb = 0;
        break;
    case 1:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_HANG_WAIT, 3.0f, 2, 1.0f);
            i_this->mMode = 2;
        }
        break;
    case 2:
        if (i_this->mAnm == ANM_HANG_DAMAGE && i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_HANG_WAIT, 3.0f, 2, 1.0f);
        }

        if (a_this->health <= 0 || e_this->checkWolfBiteDamage()) {
            e_this->offWolfBiteDamage();
            anm_init(i_this, ANM_HANG_DAMAGE, 2.0f, 0, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_HANGEDAMAGE, -1);
            a_this->health -= 5;

            if (i_this->health <= 0) {
                player->offWolfEnemyHangBite();
                if (i_this->mGroupID == 0xFF) {
                    i_this->mAction = ACT_FAIL;
                } else {
                    i_this->mAction = ACT_FAIL_WAIT;
                }

                i_this->mMode = 0;
                i_this->mHitInvincibilityTimer = 10;
                i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_DEATH, -1);
                i_this->mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x20);
                return;
            }

            i_this->field_0x6bb++;
            if (i_this->field_0x6bb >= 5) {
                player->offWolfEnemyHangBite();
                anm_init(i_this, ANM_HANG_BRUSH2, 1.0f, 0, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_HANGEDBRUSH2, -1);
                i_this->mMode = 3;
            }

            i_this->mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x1E);
        }

        if (!player->checkWolfEnemyBiteAllOwn(a_this)) {
            anm_init(i_this, ANM_HANG_BRUSH, 3.0f, 0, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_HANGEDBRUSH, -1);
            i_this->mMode = 3;
        }
        break;
    case 3:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_FIGHT_RUN;
            i_this->mMode = 0;
        }
        break;
    }
}

static void action(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz mae;
    cXyz ato;

    damage_check(i_this);
    i_this->mAcchCir.SetWall(50.0f, 100.0f);

    s8 on_attention = true;
    s8 can_bibiri = false;
    s8 cc_move = true;
    s8 can_shout = true;
    s8 fail_pos_correct = false;
    s8 on_search_sound = false;

    i_this->offHeadLockFlg();

    switch (i_this->mAction) {
    case ACT_WAIT:
        e_s1_wait(i_this);
        can_bibiri = true;
        break;
    case ACT_ROOF:
        e_s1_roof(i_this);
        break;
    case ACT_FIGHT_RUN:
        e_s1_fight_run(i_this);
        can_bibiri = true;
        on_search_sound = true;
        break;
    case ACT_FIGHT:
        e_s1_fight(i_this);
        can_bibiri = true;
        on_search_sound = true;
        break;
    case ACT_BIBIRI:
        e_s1_bibiri(i_this);
        break;
    case ACT_DAMAGE:
        e_s1_damage(i_this);
        break;
    case ACT_PATH:
        e_s1_path(i_this);
        break;
    case ACT_WOLFBITE:
        e_s1_wolfbite(i_this);
        break;
    case ACT_FAIL_WAIT:
        e_s1_failwait(i_this);
        on_attention = false;
        can_shout = false;
        fail_pos_correct = true;
        break;
    case ACT_SHOUT:
        e_s1_shout(i_this);
        can_shout = false;
        on_attention = false;
        on_search_sound = true;
        break;
    case ACT_FAIL:
        e_s1_fail(i_this);
        on_attention = false;
        can_shout = false;
        fail_pos_correct = true;
        break;
    case ACT_WARP_APPEAR:
        e_s1_warpappear(i_this);
        on_attention = false;
        cc_move = false;
        break;
    }

    if (on_search_sound) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (fail_pos_correct) {
        dBgS_LinChk linchk;
        cXyz offset;
        cXyz start_pos;
        cXyz end_pos;

        start_pos = a_this->current.pos;
        start_pos.y += 30.0f;

        J3DModel* model = i_this->mpMorf->getModel();
        MTXCopy(model->getAnmMtx(4), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&end_pos);
        end_pos.y += 30.0f;

        linchk.Set(&start_pos, &end_pos, a_this);

        if (dComIfG_Bgsp().LineCross(&linchk)) {
            offset = start_pos - end_pos;
            mDoMtx_stack_c::YrotS(cM_atan2s(offset.x, offset.z));

            offset.x = 0.0f;
            offset.y = 0.0f;
            offset.z = 50.0f;
            mDoMtx_stack_c::multVec(&offset, &ato);
            a_this->current.pos += ato;
        }
    }

    if (!l_no_fail && can_shout && i_this->field_0x6aa == 0 && i_this->mGroupID != 0xFF && fpcM_Search(s_down_sub, i_this) == NULL) {
        i_this->mAction = ACT_SHOUT;
        i_this->mMode = 0;

        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
        player->offWolfEnemyHangBite();
    }

    if (on_attention) {
        fopAcM_OnStatus(a_this, 0);
        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        i_this->attention_info.flags = 0;
    }

    if (can_bibiri && (daPy_getPlayerActorClass()->checkWolfBark() || daPy_getPlayerActorClass()->checkWolfThreat()) && pl_at_check(i_this, l_HIO.mReactionDist)) {     
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        s16 angle_to_player = player->shape_angle.y - (fopAcM_searchPlayerAngleY(a_this) + 0x8000);
        s16 bibiri_angle = 182.04f * l_HIO.mReactionAngle;

        if (angle_to_player < bibiri_angle && angle_to_player > (s16)-bibiri_angle) {
            i_this->mAction = ACT_BIBIRI;
            i_this->mMode = 0;

            i_this->field_0x6ac = 40.0f + JREG_F(11);
            i_this->field_0x6b0 = fopAcM_searchPlayerAngleY(a_this);
            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_HIRUMU, -1);
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 4, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 4, 0x1000);

    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    mae.x = 0.0f;
    mae.y = 0.0f;
    mae.z = a_this->speedF * l_HIO.mBaseSize;
    MtxPosition(&mae, &ato);

    a_this->speed.x = ato.x;
    a_this->speed.z = ato.z;
    a_this->current.pos += a_this->speed;

    a_this->speed.y += a_this->gravity;
    if (a_this->speed.y < -120.0f) {
        a_this->speed.y = -120.0f;
    }

    if (i_this->field_0x6ac > 0.01f) {
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = -i_this->field_0x6ac;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6b0);
        MtxPosition(&mae, &ato);

        a_this->current.pos += ato;
        cLib_addCalc0(&i_this->field_0x6ac, 1.0f, 7.0f + TREG_F(12));
    }

    if (cc_move) {
        cXyz* ccmove = i_this->mCcStts.GetCCMoveP();
        if (ccmove != NULL) {
            a_this->current.pos.x += ccmove->x;
            a_this->current.pos.y += ccmove->y;
            a_this->current.pos.z += ccmove->z;
        }
    }
}

static void ke_set(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz mae;
    cXyz spC;

    cM_initRnd2(12, 123, (fopAcM_GetID(a_this) * 2) + 50);

    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(4), *calc_mtx);

    for (int i = 0; i < 22; i++) {
        MtxPush();
        cMtx_YrotM(*calc_mtx, (ZREG_S(0) + 0x4000));
        cMtx_XrotM(*calc_mtx, (ZREG_S(1) + (int)cM_rndF2(2000.0f) + 3000));

        static s16 ke_za[] = {
            -0x0320, -0x0190, 0x0000, 0x0190, 0x0320, -0x0320, -0x0190,
            0x0000, 0x0190, 0x0320, -0x0320, -0x0190, 0x0000, 0x0190,
            0x0320, 0x04B0, -0x0320, -0x0190, 0x0000, 0x0190, 0x0320,
            0x04B0,
        };

        f32 z_pos;
        f32 x_offset;
        f32 z_offset;
        if (i < 10) {
            cMtx_ZrotM(*calc_mtx, (ZREG_S(2) + 5) * ke_za[i]);

            z_pos = cM_rndF2(2.25f) + 7.5f;
            x_offset = cM_rndF2(10.0f) - 5.0f;
            z_offset = cM_rndF2(20.0f) - 10.0f;
        } else {
            cMtx_ZrotM(*calc_mtx, ((ZREG_S(2) + 5) * ke_za[i]) + 0x8000);

            z_pos = cM_rndF2(3.0f) + 8.0f;
            x_offset = cM_rndF2(20.0f) - 10.0f;
            z_offset = cM_rndF2(30.0f) - 15.0f;
        }

        mae.set(x_offset, 15.0f + ZREG_F(1), z_offset + ZREG_F(2));
        MtxPosition(&mae, &i_this->mKe[i].field_0x0[0]);

        mae.set(x_offset, 20.0f + (15.0f + ZREG_F(1)) + ZREG_F(4), z_offset + ZREG_F(2));
        MtxPosition(&mae, &i_this->mKe[i].field_0x180);

        i_this->mKe[i].field_0x180 -= i_this->mKe[i].field_0x0[0];
        ke_move(i_this, &i_this->mLineMat, &i_this->mKe[i], i, z_pos);

        MtxPull();
    }
}

static void demo_camera(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* spC = dComIfGp_getCamera(0);

    cXyz spC4;
    cXyz spD0;
    cXyz spDC;
    cXyz spE8;

    s8 sp8 = true;

    switch (i_this->mDemoMode) {
    case 1:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();

        i_this->mDemoMode = 2;
        i_this->mDemoTimer = 0;
        i_this->mDemoCamBank = 55.0f;
        i_this->field_0x3054 = 300.0;
        i_this->field_0x3058 = 1200.0f;
        i_this->field_0x3060 = 0.0f;

        camera->mCamera.SetTrimSize(3);
        /* fallthrough */
    case 2:
        mDoMtx_stack_c::YrotS(a_this->shape_angle.y);
        spC4.x = TREG_F(2);
        spC4.y = i_this->field_0x3054;
        spC4.z = i_this->field_0x3058;
        mDoMtx_stack_c::multVec(&spC4, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += a_this->current.pos;

        cLib_addCalc2(&i_this->field_0x3054, 30.0f, 0.1f, i_this->field_0x3060 * 270.0f);
        cLib_addCalc2(&i_this->field_0x3058, 750.0f, 0.1f, i_this->field_0x3060 * 850.0f);
        cLib_addCalc2(&i_this->field_0x3060, 0.05f, 1.0f, 0.005f);

        i_this->mDemoCamCenter = a_this->eyePos;
        i_this->mDemoCamCenter.y += 50.0f;        

        if (i_this->mDemoTimer == 137) {
            if (!dComIfGs_isSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this))) {
                dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
                OS_REPORT("S! BITSW %d\n", i_this->mSwBit);
                OS_REPORT("S! BITSW %d\n", dComIfGs_isSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this)));
            }

            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            fopAcM_delete(a_this);
        }
        break;
    case 10:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();

        i_this->mDemoMode = 11;
        i_this->mDemoTimer = 0;
        i_this->mDemoCamBank = 60.0f;
        
        i_this->mDemoCamCenter.set(-16046.0f, 177.0f, -955.0f);
        i_this->mDemoCamEye.set(-16174.0f, 331.0f, -731.0f);

        camera->mCamera.SetTrimSize(3);
        player->changeOriginalDemo();
        /* fallthrough */
    case 11:
        sp8 = false;
        spC4.set(-15400.0f, 0.0f, 400.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spC4, 0, 0);

        if (i_this->mDemoTimer != 60) {
            break;
        }

        i_this->mDemoMode = 12;
        i_this->mDemoTimer = 0;
        i_this->mDemoCamBank = 45.15f;
        /* fallthrough */
    case 12:
        sp8 = false;
        
        i_this->mDemoCamCenter.set(-40.0f, -39.0f, -68.0f);
        i_this->mDemoCamCenter += player->attention_info.position;

        i_this->mDemoCamEye.set(218.0f, -3.0f, 78.0f);
        i_this->mDemoCamEye += player->attention_info.position;

        if (i_this->mDemoTimer == 45) {
            i_this->mDemoMode = 13;
            i_this->mDemoTimer = 0;
            i_this->mDemoCamBank = 42.0f;

            i_this->mDemoCamCenter.set(-16232.0f, 45.6f, 234.0f);
            i_this->mDemoCamEye.set(-16140.0f, 17.6f, 518.0f);
        }
        break;
    case 13:
        sp8 = false;

        if (i_this->mDemoTimer == 110) {
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            dComIfGs_onSwitch(14, fopAcM_GetRoomNo(a_this));
            i_this->mDemoMode = 0;
        }
        break;
    }

    if (i_this->mDemoMode != 0) {
        cXyz center;
        cXyz eye;

        center = i_this->mDemoCamCenter;
        eye = i_this->mDemoCamEye;

        if (sp8) {
            dBgS_LinChk linchk;
            linchk.Set(&i_this->mDemoCamCenter, &i_this->mDemoCamEye, a_this);

            if (dComIfG_Bgsp().LineCross(&linchk)) {
                eye = linchk.GetCross() + ((center - eye) * 0.1f);

                f32 dist_x = eye.x - center.x;
                f32 dist_z = eye.z - center.z;
                eye.y += i_this->field_0x3058 - JMAFastSqrt(dist_x * dist_x + dist_z * dist_z);
            }
        }

        camera->mCamera.Set(center, eye, i_this->mDemoCamBank, 0);
        i_this->mDemoTimer++;
    }
}

static void anm_se_set(e_s1_class* i_this) {
    if (i_this->mAnm == ANM_WALK) {
        if (i_this->mpMorf->checkFrame(27.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_FN_L, 0, -1);
        } else if (i_this->mpMorf->checkFrame(57.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_FN_R, 0, -1);
        }
    } else if (i_this->mAnm == ANM_DASH_01) {
        if (i_this->mpMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_HAND_R, 0, -1);
        } else if (i_this->mpMorf->checkFrame(3.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_FN_R, 0, -1);
        } else if (i_this->mpMorf->checkFrame(4.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_HAND_L, 0, -1);
        } else if (i_this->mpMorf->checkFrame(7.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_FN_L, 0, -1);
        }
    } else if (i_this->mAnm == ANM_DASH_02) {
        if (i_this->mpMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_HAND_R, 0, -1);
        } else if (i_this->mpMorf->checkFrame(6.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_FN_R, 0, -1);
        } else if (i_this->mpMorf->checkFrame(8.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_HAND_L, 0, -1);
        } else if (i_this->mpMorf->checkFrame(14.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NS_FN_L, 0, -1);
        }
    } else if (i_this->mAnm == ANM_WAIT_01) {
        if (i_this->mpMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_WAIT, -1);
        } else if (i_this->mpMorf->checkFrame(45.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_WAIT, -1);
        }
    } else if (i_this->mAnm == ANM_WAIT_02) {
        if (i_this->mpMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_SEARCH, -1);
        } else if (i_this->mpMorf->checkFrame(45.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_NS_V_SEARCH, -1);
        }
    }
}

static void body_eff_set(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    J3DModel* model = i_this->mpMorf->getModel();

    i_this->mBodyEffEmtrID = dComIfGp_particle_set(i_this->mBodyEffEmtrID, 0x387, &a_this->current.pos, NULL, NULL);

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mBodyEffEmtrID);
    if (emitter != NULL) {
        emitter->setGlobalRTMatrix(model->getAnmMtx(2));
    }
}

static int daE_S1_Execute(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz spE0;
    cXyz spD4;

    i_this->mCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->mHitInvincibilityTimer != 0) {
        i_this->mHitInvincibilityTimer--;
    }

    if (i_this->field_0x6aa != 0) {
        i_this->field_0x6aa--;
    }

    action(i_this);

    if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && i_this->mPrm1 == 0xFE) {
        if (a_this->current.pos.z <= -27035.0f) {
            if (a_this->current.pos.z >= -35400.0f && a_this->current.pos.y < 0.0f) {
                if (a_this->current.pos.z > -28750.0f) {
                    a_this->current.pos.z = -28750.0f;
                }

                if (a_this->current.pos.x > 35330.0f) {
                    a_this->current.pos.x = 35330.0f;
                } else if (a_this->current.pos.x < 34270.0f) {
                    a_this->current.pos.x = 34270.0f;
                }
            }
        }
    }

    i_this->mAcch.CrrPos(dComIfG_Bgsp());

    s16 spC = 0;
    s16 spA = 0;

    if (i_this->mAction == ACT_FAIL) {
        Vec spC8;
        cXyz spBC;
        cXyz spB0;
        dBgS_GndChk gndchk;
        f32 temp_f30 = 75.0f;

        MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(1), *calc_mtx);
        spE0.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&spE0, &spBC);

        spBC.y += 100.0f;
        spBC.y += 100.0f;
        gndchk.SetPos(&spBC);

        spBC.y = dComIfG_Bgsp().GroundCross(&gndchk);
        if (-G_CM3D_F_INF != spBC.y) {
            spC8.x = spBC.x;
            spC8.y = spBC.y + 100.0f;
            spC8.z = spBC.z + temp_f30;
            gndchk.SetPos(&spC8);

            spC8.y = dComIfG_Bgsp().GroundCross(&gndchk);
            if (-G_CM3D_F_INF != spC8.y) {
                spA = -cM_atan2s(spC8.y - spBC.y, spC8.z - spBC.z);
                if (spA > 0x3000 || spA < -0x3000) {
                    spA = 0;
                }
            }
    
            spC8.x = spBC.x + temp_f30;
            spC8.y = spBC.y + 100.0f;
            spC8.z = spBC.z;
            gndchk.SetPos(&spC8);

            spC8.y = dComIfG_Bgsp().GroundCross(&gndchk);
            if (-G_CM3D_F_INF != spC8.y) {
                spC = (s16)cM_atan2s(spC8.y - spBC.y, spC8.x - spBC.x);
                if (spC > 0x3000 || spC < -0x3000) {
                    spC = 0;
                }
            }
        }

        dBgS_LinChk linchk;
        cXyz spA4;
        cXyz sp98;
        cXyz sp8C;

        sp98 = a_this->current.pos;
        sp98.y += 50.0f + JREG_F(5);

        J3DModel* model = i_this->mpMorf->getModel();
        spA4.set(0.0f, 0.0f, 0.0f);
        MTXCopy(model->getAnmMtx(4), *calc_mtx);

        MtxPosition(&spA4, &sp8C);
        sp8C.y += 50.0f + JREG_F(6);

        linchk.Set(&sp98, &sp8C, a_this);

        if (dComIfG_Bgsp().LineCross(&linchk)) {
            spA4 = sp98 - sp8C;
            cMtx_YrotS(*calc_mtx, cM_atan2s(spA4.x, spA4.z));

            spA4.x = 0.0f;
            spA4.y = 0.0f;
            spA4.z = 50.0f + TREG_F(11);
            MtxPosition(&spA4, &spD4);
            a_this->current.pos += spD4;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0x6b2, spA, 1, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0x6b6, spC, 1, 0x400);

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::XrotM(i_this->field_0x6b2);
    mDoMtx_stack_c::ZrotM(i_this->field_0x6b6);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.mBaseSize * a_this->scale.x, l_HIO.mBaseSize * a_this->scale.x, l_HIO.mBaseSize * a_this->scale.x);

    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    i_this->mpMorf->modelCalc();

    anm_se_set(i_this);
    body_eff_set(i_this);
    ke_set(i_this);

    MTXCopy(model->getAnmMtx(4), *calc_mtx);
    spE0.set(20.0f + KREG_F(0), KREG_F(1), KREG_F(2));
    MtxPosition(&spE0, &a_this->eyePos);

    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += 35.0f;

    cXyz sp80(0.0f, 0.0f, 0.0f);
    if (i_this->mHitInvincibilityTimer != 0) {
        sp80.x = 22340.0f;
    }

    i_this->mCcSph[0].SetC(a_this->eyePos + sp80);
    i_this->mCcSph[0].SetR(35.0f * l_HIO.mBaseSize);
    dComIfG_Ccsp()->Set(&i_this->mCcSph[0]);

    spE0.set(0.0f, 0.0f, 0.0f);
    MTXCopy(model->getAnmMtx(3), *calc_mtx);
    MtxPosition(&spE0, &spD4);

    i_this->mCcSph[1].SetC(spD4 + sp80);
    i_this->mCcSph[1].SetR(60.0f * l_HIO.mBaseSize);

    MTXCopy(model->getAnmMtx(2), *calc_mtx);
    MtxPosition(&spE0, &spD4);

    i_this->mCcSph[2].SetC(spD4 + sp80);
    i_this->mCcSph[2].SetR(50.0f * l_HIO.mBaseSize);

    dComIfG_Ccsp()->Set(&i_this->mCcSph[1]);
    dComIfG_Ccsp()->Set(&i_this->mCcSph[2]);

    if (i_this->field_0x6b9 != 0) {
        if (i_this->field_0x6b9 == 1) {
            MTXCopy(model->getAnmMtx(0xF), *calc_mtx);
        } else if (i_this->field_0x6b9 == 2) {
            MTXCopy(model->getAnmMtx(8), *calc_mtx);
        } else if (i_this->field_0x6b9 == 3) {
            MTXCopy(model->getAnmMtx(4), *calc_mtx);
        }

        spE0.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&spE0, &spD4);

        i_this->field_0x6b9 = 0;

        if (i_this->field_0x6ba == 0) {
            i_this->field_0x6ba = 1;
            i_this->mCcAtSph.StartCAt(spD4);
        } else {
            i_this->mCcAtSph.MoveCAt(spD4);
        }
    } else {
        spD4.set(20000.0f, 50000.0f, 20000.0f);
        i_this->mCcAtSph.SetC(spD4);
        i_this->field_0x6ba = 0;
    }

    i_this->mCcAtSph.SetR(40.0f * l_HIO.mBaseSize);
    dComIfG_Ccsp()->Set(&i_this->mCcAtSph);

    demo_camera(i_this);

    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(2), *calc_mtx);
    spE0.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&spE0, &spD4);

    cXyz eff_size(1.3f, 1.3f, 1.3f);
    setMidnaBindEffect(i_this, &i_this->mSound, &spD4, &eff_size);

    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(2), mDoMtx_stack_c::get());

    mDoMtx_stack_c::multVecZero(&spD4);
    spE0 = player->current.pos - spD4;

    s16 sp8 = cM_atan2s(spE0.x, spE0.z);
    cXyz downpos;
    cMtx_YrotS(*calc_mtx, sp8);

    spE0.x = 0.0f;
    spE0.y = 45.0f + NREG_F(18);
    spE0.z = (60.0f + BREG_F(12)) - 30.0f;
    MtxPosition(&spE0, &downpos);
    downpos += spD4;
    i_this->setDownPos(&downpos);

    cXyz lockpos(a_this->eyePos);
    lockpos.y += NREG_F(7) + 130.0f;
    i_this->setHeadLockPos(&lockpos);
    a_this->attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;
    return 1;
}

static int daE_S1_IsDelete(e_s1_class* i_this) {
    return 1;
}

static int daE_S1_Delete(e_s1_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dComIfG_resDelete(&i_this->mPhase, "E_S2");

    if (i_this->mInitHIO) {
        hio_set = false;
    }

    if (a_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }

    return 1;
}

static Vec jv_offset = {0.0f, 0.0f, 0.0f};

static dJntColData_c jc_data[] = {
    {0, 1, 1, 30.0f, &jv_offset},
    {0, 1, 2, 30.0f, &jv_offset},
    {0, 1, 3, 25.0f, &jv_offset},
    {0, 1, 7, 15.0f, &jv_offset},
    {0, 1, 8, 10.0f, &jv_offset},
    {0, 1, 14, 15.0f, &jv_offset},
    {0, 1, 15, 10.0f, &jv_offset},
    {0, 1, 21, 10.0f, &jv_offset},
    {0, 1, 24, 10.0f, &jv_offset},
};

static int useHeapInit(fopAc_ac_c* i_this) {
    e_s1_class* a_this = (e_s1_class*)i_this;

    a_this->mpMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_S2", 0x22), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_S2", 0x1D), 0, 1.0f, 0, -1, &a_this->mSound, 0x80000, 0x11000084);
    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    if (!a_this->mLineMat.init(0x16, 0x10, 1)) {
        return 0;
    }

    if (!a_this->mJntCol.init(i_this, jc_data, a_this->mpMorf->getModel(), 9)) {
        return 0;
    }

    return 1;
}

static int daE_S1_Create(fopAc_ac_c* i_this) {
    e_s1_class* a_this = (e_s1_class*)i_this;
    fopAcM_ct(a_this, e_s1_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "E_S2");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_S1 PARAM %x\n", fopAcM_GetParam(i_this));
        OS_REPORT("E_S1 AZ    %x\n", i_this->current.angle.z);
        l_no_fail = false;

        if ((strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 ||
             strcmp(dComIfGp_getStartStageName(), "D_MN08B") == 0 ||
             strcmp(dComIfGp_getStartStageName(), "D_MN08C") == 0))
        {
            if ((s8)fopAcM_GetRoomNo(i_this) == 51 || (s8)fopAcM_GetRoomNo(i_this) == 9 ||
                (s8)fopAcM_GetRoomNo(i_this) == 52)
            {
                l_no_fail = true;
            }
        }

        a_this->mRoofSpawn = fopAcM_GetParam(i_this) & 0xF;
        if (a_this->mRoofSpawn == 0xF) {
            a_this->mRoofSpawn = 0;
        }

        a_this->mGroupID = (fopAcM_GetParam(i_this) & 0xF0) >> 4;
        if (a_this->mGroupID == 0xF) {
            a_this->mGroupID = 0xFF;
        }

        a_this->mPrm1 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;
        if (a_this->mPrm1 == 0xFF) {
            if (a_this->mRoofSpawn) {
                a_this->mPrm1 = 3;
            } else {
                a_this->mPrm1 = 20;
            }
        }

        a_this->mSearchRange = a_this->mPrm1 * 100.0f;
        a_this->field_0x3068 = (i_this->current.angle.z & 0xFF) * 100.0f;

        if (a_this->field_0x3068 == 0.0f) {
            a_this->field_0x3068 = 300.0f;
        }

        i_this->current.angle.z = i_this->shape_angle.z = 0;

        int i;

        u8 path_no = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;
        a_this->mSwBit = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;

        if (a_this->mSwBit != 0xFF) {
            if (dComIfGs_isSwitch(a_this->mSwBit, (s8)fopAcM_GetRoomNo(i_this))) {
                // "After E_S1 defeated, so don't re-set\n"
                OS_REPORT("E_S1 やられ後なので再セットしません\n");
                return cPhs_ERROR_e;
            }
        }

        OS_REPORT("E_S1//////////////E_S1 SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x8EA0)) {
            OS_REPORT("//////////////E_S1 SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_S1 SET 2 !!\n");

        if (path_no != 0xFF) {
            a_this->mpPath = dPath_GetRoomPath(path_no, (s8)fopAcM_GetRoomNo(i_this));

            OS_REPORT("//////////////E_S1 PPD %x!!\n", a_this->mpPath);
            if (a_this->mpPath == NULL) {
                OS_REPORT("......NONONONONONO PATH !!!!\n");
                return cPhs_ERROR_e;
            }
        }

        if (!hio_set) {
            a_this->mInitHIO = true;
            hio_set = true;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_OnStatus(i_this, 0x100);
        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        if (!a_this->mRoofSpawn) {
            a_this->mAction = ACT_WAIT;
            a_this->mDrawShadow = true;
        } else {
            a_this->mAction = ACT_ROOF;
        }

        fopAcM_SetMtx(i_this, a_this->mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);

        OS_REPORT("//////////////E_S1 POS %d,%d,%d \n", (int)i_this->home.pos.x, (int)i_this->home.pos.y, (int)i_this->home.pos.z);
        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                          &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->mAcchCir.SetWall(50.0f, 100.0f);

        i_this->field_0x560 = i_this->health = 50;

        a_this->mCcStts.Init(0xFA, 0, i_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                 // mGObjTg
                {0x0},                                              // mGObjCo
            },                                                      // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
            }  // mSphAttr
        };

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0x0, 0x0}, 0x0}},  // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0},                      // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                           // mGObjTg
                {0x0},                                                        // mGObjCo
            },                                                                // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f}  // mSph
            }  // mSphAttr
        };

        for (i = 0; i <= 2; i++) {
            a_this->mCcSph[i].Set(cc_sph_src);
            a_this->mCcSph[i].SetStts(&a_this->mCcStts);
        }

        a_this->mCcAtSph.Set(at_sph_src);
        a_this->mCcAtSph.SetStts(&a_this->mCcStts);

        a_this->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->mSound.setEnemyName("E_s1");
        a_this->mAtInfo.mpSound = &a_this->mSound;

        a_this->field_0x6aa = 30;

             /* dSv_event_flag_c::M_050 - Main Event - Eldin Bridge disappears */
        if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[83])
            && strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && a_this->mPrm1 == 0xFE)
        {
            i_this->current.pos.y += 50000.0f;
            i_this->current.pos.x += 50000.0f;
        }

        daE_S1_Execute(a_this);
        i_this->gravity = -7.0f;
    }

    return phase_state;
}

static actor_method_class l_daE_S1_Method = {
    (process_method_func)daE_S1_Create,  (process_method_func)daE_S1_Delete,
    (process_method_func)daE_S1_Execute, (process_method_func)daE_S1_IsDelete,
    (process_method_func)daE_S1_Draw,
};

actor_process_profile_definition g_profile_E_S1 = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_S1,               // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(e_s1_class),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    121,                     // mPriority
    &l_daE_S1_Method,        // sub_method
    0x00040000,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
