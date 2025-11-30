/**
 * d_a_e_yc.cpp
 * Twilit Carrier Kargarok
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_yc.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_procname.h"
#include "d/actor/d_a_e_rdy.h"

static f32 S_area_dis;

static bool hioInit;

static daE_YC_HIO_c l_HIO;

daE_YC_HIO_c::daE_YC_HIO_c() {
    field_0x4 = -1;
    mScale = 1.0f;
}

static void anm_init(e_yc_class* i_this, int i_anmID, f32 i_morf, u8 i_attr, f32 i_rate) {
    J3DAnmTransform* anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("E_yc", i_anmID));
    i_this->mpMorf->setAnm(anm, i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_anmID;
}

static int daE_YC_Draw(e_yc_class* i_this) {
    if (i_this->mNoDrawFlag) {
        return 1;
    }

    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(2, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);
    dComIfGd_setListDark();
    i_this->mpMorf->entryDL();
    dComIfGd_setList();
    
    cXyz pos;
    pos.set(i_this->current.pos.x, i_this->current.pos.y + 100.0f, i_this->current.pos.z);
    i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &pos, 1700.0f, 0.0f,
                                            i_this->current.pos.y, i_this->mAcch.GetGroundH(),
                                            i_this->mAcch.m_gnd, &i_this->tevStr, 0, 1.0f,
                                            dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static void damage_check(e_yc_class* i_this) {
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
    i_this->mCcStts.Move();
    if (i_this->mCcDisableTimer == 0 && i_this->mCcSph.ChkTgHit()) {
        i_this->mAtInfo.mpCollider = i_this->mCcSph.GetTgHitObj();
        if (player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_LEFT
            && player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_RIGHT
            && i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK)
            && player->onWolfEnemyBiteAll(i_this, daPy_py_c::FLG2_WOLF_ENEMY_HANG_BITE))
        {
            i_this->mAction = e_yc_class::ACT_WOLFBITE;
            i_this->mMode = 0;
            i_this->mCcDisableTimer = 1000;
            i_this->mCreatureSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x1f);
        }
    }
}

static void e_yc_fly(e_yc_class* i_this) {
    fopAc_ac_c* base_rdy = fopAcM_SearchByID(i_this->mRiderID);
    e_rdy_class* rider = (e_rdy_class*) base_rdy;
    int frame = i_this->mpMorf->getFrame();
    f32 target_speed = 0.0f;
    f32 accel = 1.0f;
    f32 delta_x, delta_y, delta_z;

    switch (i_this->mFlyAnmMode) {
    case 0:
        if (i_this->mTimer[0] == 0 && frame == 14) {
            i_this->mFlyAnmMode = 1;
            anm_init(i_this, e_yc_class::ANM_FLY_GLIDE, 12.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(70.0f) + 70.0f;
        }
        break;

    case 1:
        if (i_this->mTimer[0] == 0) {
            i_this->mFlyAnmMode = 0;
            anm_init(i_this, e_yc_class::ANM_FLY, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(70.0f) + 70.0f;
        }
        break;

    case 2:
        break;
    }

    s8 cvar6 = 0;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        // fallthrough

    case 1:
        target_speed = 25.0f;
        if (i_this->mTimer[1] == 0) {
            delta_x = i_this->home.pos.x + cM_rndFX(2000.0f) - i_this->current.pos.x;
            delta_z = i_this->home.pos.z + cM_rndFX(2000.0f) - i_this->current.pos.z;
            if (JMAFastSqrt(delta_x * delta_x + delta_z * delta_z) > 200.0f) {
                i_this->mTimer[1] = cM_rndF(150.0f) + 100.0f;
                i_this->mTargetPos.x = delta_x + i_this->current.pos.x;
                i_this->mTargetPos.y = i_this->home.pos.y;
                i_this->mTargetPos.z = delta_z + i_this->current.pos.z;
                i_this->mAngleSpeedRatio = 0.0f;
                i_this->mMaxAngleSpeed = cM_rndF(300.0f) + 200.0f;
            }
        }
        break;

    case 2:
        target_speed = 25.0f;
        i_this->mTargetPos = i_this->home.pos;
        i_this->mMaxAngleSpeed = 200.0f;
        i_this->mAngleXSpeedRatio = 0.0f;
        anm_init(i_this, e_yc_class::ANM_FLY_GLIDE, 0.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mFlyAnmMode = 2;
        break;

    case 3:
        target_speed = 25.0f;
        if (rider->mDemoTimer > 200) {
            target_speed = 35.0f;
            accel = 0.25f;
        }
        i_this->mTargetPos = base_rdy->current.pos;
        i_this->mTargetPos.y += 150.0f;
        break;

    case 4:
        accel = 0.3f;
        if (rider->mDemoTimer == 30) {
            anm_init(i_this, e_yc_class::ANM_FLY_BRAKE, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        if (rider->mDemoTimer == 60) {
            anm_init(i_this, e_yc_class::ANM_HOVERING, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        i_this->mMaxAngleSpeed = 300.0f;
        break;

    case 5:
        i_this->field_0x694 += i_this->field_0x698;
        cLib_addCalc0(&i_this->field_0x698, 1.0f, 4.0f);
        break;

    case 6:
        cLib_addCalc0(&i_this->field_0x694, 1.0f, 1.0f);
        cLib_addCalc2(&i_this->current.pos.y, base_rdy->home.pos.y + 400.0f, 0.05f, 3.0f);
        break;
    }

    delta_x = i_this->mTargetPos.x - i_this->current.pos.x;
    delta_y = i_this->mTargetPos.y - i_this->current.pos.y;
    delta_z = i_this->mTargetPos.z - i_this->current.pos.z;
    s16 target_angle_y = cM_atan2s(delta_x, delta_z);
    s16 target_angle_x = -cM_atan2s(delta_y, JMAFastSqrt(delta_x * delta_x + delta_z * delta_z));
    s16 prev_angle_y = i_this->current.angle.y;
    cLib_addCalcAngleS2(&i_this->current.angle.y, target_angle_y, 0x20,
                        i_this->mMaxAngleSpeed * i_this->mAngleSpeedRatio);
    s16 target_angle_z = (prev_angle_y - i_this->current.angle.y) * 0x20;
    if (target_angle_z > 5500) {
        target_angle_z = 5500;
    } else if (target_angle_z < -5500) {
        target_angle_z = -5500;
    }
    cLib_addCalcAngleS2(&i_this->current.angle.z, target_angle_z, 10,
                        i_this->mMaxAngleSpeed * i_this->mAngleSpeedRatio * 0.5f);
    cLib_addCalcAngleS2(&i_this->current.angle.x, target_angle_x, 0x20,
                        i_this->mMaxAngleSpeed * i_this->mAngleXSpeedRatio);
    cLib_addCalc2(&i_this->mAngleSpeedRatio, 1.0f, 1.0f, 0.04f);
    cLib_addCalc2(&i_this->mAngleXSpeedRatio, 0.5f, 1.0f, 0.01f);
    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, accel);
}

static void e_yc_f_fly(e_yc_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    int frame = i_this->mpMorf->getFrame();
    f32 delta_x, delta_y, delta_z;

    switch (i_this->mFlyAnmMode) {
    case 0:
        if (i_this->mTimer[0] == 0 && frame == 14) {
            i_this->mFlyAnmMode = 1;
            anm_init(i_this, e_yc_class::ANM_FLY_GLIDE, 12.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(70.0f) + 70.0f;
        }
        break;

    case 1:
        if (i_this->mTimer[0] == 0) {
            i_this->mFlyAnmMode = 0;
            anm_init(i_this, e_yc_class::ANM_FLY, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(70.0f) + 70.0f;
        }
        break;
    }

    s8 cvar6 = 0;

    switch (i_this->mMode) {
    case 0:
        i_this->mTimer[2] = cM_rndF(150.0f) + 150.0f;
        i_this->mMode = 1;
        // fallthrough

    case 1:
        if (i_this->mTimer[1] == 0) {
            delta_x = i_this->home.pos.x + cM_rndFX(2000.0f) - i_this->current.pos.x;
            delta_z = i_this->home.pos.z + cM_rndFX(2000.0f) - i_this->current.pos.z;
            if (JMAFastSqrt(delta_x * delta_x + delta_z * delta_z) > 200.0f) {
                i_this->mTimer[1] = cM_rndF(150.0f) + 100.0f;
                i_this->mTargetPos.x = delta_x + i_this->current.pos.x;
                i_this->mTargetPos.y = player->current.pos.y + 1000.0f + cM_rndF(1000.0f);
                i_this->mTargetPos.z = delta_z + i_this->current.pos.z;
                i_this->mAngleSpeedRatio = 0.0f;
                i_this->mMaxAngleSpeed = cM_rndF(300.0f) + 200.0f;
            }
        }

        if (i_this->mTimer[2] == 0 && S_area_dis < 4000.0f) {
            cvar6 = 1;
        }

        if (cvar6 == 1) {
            i_this->mAction = e_yc_class::ACT_ATTACK;
            i_this->mMode = 0;
        } else if (cvar6 == 2) {
            i_this->mAction = e_yc_class::ACT_ATTACK;
            i_this->mMode = 0;
        }
        break;
    }

    delta_x = i_this->mTargetPos.x - i_this->current.pos.x;
    delta_y = i_this->mTargetPos.y - i_this->current.pos.y;
    delta_z = i_this->mTargetPos.z - i_this->current.pos.z;
    s16 target_angle_y = cM_atan2s(delta_x, delta_z);
    s16 target_angle_x = -cM_atan2s(delta_y, JMAFastSqrt(delta_x * delta_x + delta_z * delta_z));
    s16 prev_angle_y = i_this->current.angle.y;
    cLib_addCalcAngleS2(&i_this->current.angle.y, target_angle_y, 10,
                        i_this->mMaxAngleSpeed * i_this->mAngleSpeedRatio);
    s16 target_angle_z = (prev_angle_y - i_this->current.angle.y) * 0x20;
    if (target_angle_z > 5500) {
        target_angle_z = 5500;
    } else if (target_angle_z < -5500) {
        target_angle_z = -5500;
    }
    cLib_addCalcAngleS2(&i_this->current.angle.z, target_angle_z, 10,
                        i_this->mMaxAngleSpeed * i_this->mAngleSpeedRatio * 0.5f);
    cLib_addCalcAngleS2(&i_this->current.angle.x, target_angle_x, 10,
                        i_this->mMaxAngleSpeed * i_this->mAngleSpeedRatio);
    cLib_addCalc2(&i_this->mAngleSpeedRatio, 1.0f, 1.0f, 0.04f);
    cLib_addCalc2(&i_this->speedF, 25.0f, 1.0f, 1.0f);
}

static void e_yc_hovering(e_yc_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 target_speed = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_yc_class::ANM_HOVERING, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        i_this->mTimer[0] = cM_rndF(100.0f) + 200.0f;
        break;

    case 1:
        if (i_this->current.pos.y < player->current.pos.y + 400.0f) {
            cLib_addCalc2(&i_this->current.pos.y, player->current.pos.y + 400.0f, 0.05f, 5.0f);
        }
        if (i_this->current.pos.y > player->current.pos.y + 1000.0f) {
            cLib_addCalc2(&i_this->current.pos.y, player->current.pos.y + 1000.0f, 0.05f, 5.0f);
        }

        if (i_this->mDistToPlayer < 1000.0f) {
            target_speed = -5.0f;
        } else if (i_this->mDistToPlayer > 3000.0f) {
            target_speed = 5.0f;
        }

        if (i_this->mTimer[0] == 0 || S_area_dis > 5000.0f) {
            i_this->mAction = e_yc_class::ACT_F_FLY;
            i_this->mMode = 0;
            i_this->mFlyAnmMode = 1;
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, 0.5f);
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mAngleToPlayer, 8, 0x200);
    cLib_addCalcAngleS2(&i_this->current.angle.z, 0, 4, 0x800);
}

static void e_yc_attack(e_yc_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz delta;
    fopAc_ac_c* base_rdy = fopAcM_SearchByID(i_this->mRiderID);
    e_rdy_class* rider = (e_rdy_class*) base_rdy;
    f32 target_speed = 0.0f;
    int frame = i_this->mpMorf->getFrame();

    if (i_this->mMode < 4) {
        i_this->mTargetPos = player->current.pos;
        i_this->mTargetPos.y += 50.0f;
    }
    delta = i_this->mTargetPos - i_this->current.pos;

    switch (i_this->mMode) {
    case 0:
        i_this->mAngleSpeedRatio = 0;
        i_this->mMaxAngleSpeed = 500.0f;
        i_this->mMode = 1;
        // fallthrough

    case 1:
        target_speed = 30.0f;
        if (delta.abs() < 1000.0f) {
            i_this->mMode = 2;
            anm_init(i_this, e_yc_class::ANM_FLY_BRAKE, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMaxAngleSpeed = 1000.0f;
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_YC_V_ATTACK, -1);
        }
        break;

    case 2:
        if (i_this->speedF < 10.0f) {
            i_this->mMode = 3;
            i_this->mTimer[0] = 15;
            i_this->mMaxAngleSpeed = 0.0f;
        }
        break;

    case 3:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, e_yc_class::ANM_CATCH_START, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 4;
        }
        break;

    case 4:
        if (frame == 11 && rider->mDemoMode == 0 & i_this->mDistToPlayer < 200.0f) {
            rider->mDemoMode = 1;
            i_this->mCreatureSound.startCreatureSound(Z2SE_EN_YC_HIT_SIDE, 0, -1);
        }

        cLib_addCalc2(&i_this->current.pos.x, i_this->mTargetPos.x, 1.0f, 30.0f);
        cLib_addCalc2(&i_this->current.pos.y, i_this->mTargetPos.y + 350.0f, 1.0f, 30.0f);
        cLib_addCalc2(&i_this->current.pos.z, i_this->mTargetPos.z, 1.0f, 30.0f);

        if (i_this->mpMorf->isStop()) {
            if (rider->mDemoMode == 0) {
                i_this->mAction = e_yc_class::ACT_HOVERING;
                i_this->mMode = 0;
            } else {
                anm_init(i_this, e_yc_class::ANM_CATCH_MIDDLE, 5.0f,
                         J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mMode = 5;
                i_this->mTimer[0] = 80;
            }
        }
        break;

    case 5:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, e_yc_class::ANM_CATCH_END, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 6;
        } else {
            i_this->current.pos.y += 5.0f;
        }
        break;

    case 6:
        if (frame == 30) {
            rider->mDemoMode = 3;
        }

        if (i_this->mpMorf->isStop()) {
            i_this->mAction = e_yc_class::ACT_HOVERING;
            i_this->mMode = 0;
        }
        break;
    }

    s16 target_angle_y = cM_atan2s(delta.x, delta.z);
    s16 target_angle_x = -cM_atan2s(delta.y, JMAFastSqrt(delta.x * delta.x + delta.z * delta.z));
    s16 prev_ang_y = i_this->current.angle.y;
    cLib_addCalcAngleS2(&i_this->current.angle.y, target_angle_y, 10,
                        i_this->mMaxAngleSpeed * i_this->mAngleSpeedRatio);
    s16 target_angle_z;
    if (i_this->mMode < 2) {
        target_angle_z = (prev_ang_y - i_this->current.angle.y) * 0x20;
        if (target_angle_z > 5500) {
            target_angle_z = 5500;
        } else if (target_angle_z < -5500) {
            target_angle_z = -5500;
        }
    } else {
        target_angle_z = 0;
    }
    cLib_addCalcAngleS2(&i_this->current.angle.z, target_angle_z, 10,
                        i_this->mMaxAngleSpeed * i_this->mAngleSpeedRatio * 0.5f);
    cLib_addCalcAngleS2(&i_this->current.angle.x, target_angle_x, 10,
                        i_this->mMaxAngleSpeed * i_this->mAngleSpeedRatio);
    cLib_addCalc2(&i_this->mAngleSpeedRatio, 1.0f, 1.0f, 0.04f);
    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, 0.75f);

    if (S_area_dis > 5000.0f) {
        i_this->mAction = e_yc_class::ACT_F_FLY;
        i_this->mMode = 0;
    }
}

static void e_yc_wolfbite(e_yc_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
    e_rdy_class* rider = (e_rdy_class*) fopAcM_SearchByID(i_this->mRiderID);

    int frame = i_this->mpMorf->getFrame();
    i_this->mCcDisableTimer = 10;
    cLib_addCalc0(&_this->speedF, 1.0f, 2.0f);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_yc_class::ANM_HANGED, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_DN_V_DRAWBACK, -1);
        _this->health -= 5;
        i_this->mWolfBiteDamageCount = 0;
        break;

    case 1:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, e_yc_class::ANM_HANGED_WAIT, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 2;
        }
        break;

    case 2:
        if (i_this->mAnm == e_yc_class::ANM_HANGED_DAMAGE && i_this->mpMorf->isStop()) {
            anm_init(i_this, e_yc_class::ANM_HANGED_WAIT, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }

        if (i_this->health <= 0 || i_this->checkWolfBiteDamage()) {
            i_this->offWolfBiteDamage();
            anm_init(i_this, e_yc_class::ANM_HANGED_DAMAGE, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->health -= 5;

            if (_this->health <= 0) {
                player->offWolfEnemyHangBite();
                anm_init(i_this, e_yc_class::ANM_HANGED_BRUSH2, 3.0f,
                         J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mMode = 3;
                rider->mDemoMode = 5;
                dScnPly_c::setPauseTimer(7);
                i_this->mCreatureSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x20);
                dComIfGp_setHitMark(3, i_this, &player->eyePos, &player->shape_angle, NULL, 0);
                dComIfGp_getVibration().StartShock(5, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
                return;
            }

            i_this->mWolfBiteDamageCount++;
            if (i_this->mWolfBiteDamageCount >= 5) {
                player->offWolfEnemyHangBite();
                anm_init(i_this, e_yc_class::ANM_HANGED_BRUSH, 3.0f,
                         J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mMode = 3;
            }

            i_this->mCreatureSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x1e);
        }

        if (!player->checkWolfEnemyBiteAllOwn(_this)) {
            anm_init(i_this, e_yc_class::ANM_HANGED_BRUSH, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 3;
        }

        break;

    case 3:
        _this->current.pos.y += 10.0f;
        if (i_this->mAnm == e_yc_class::ANM_HANGED_BRUSH2 && frame == 24) {
            rider->field_0xa8f = 1;
        }

        if (i_this->mpMorf->isStop()) {
            if (i_this->mAnm == e_yc_class::ANM_HANGED_BRUSH2) {
                i_this->mAction = e_yc_class::ACT_FLY;
                i_this->mMode = 0;
                i_this->mFlyAnmMode = 1;
                _this->home.pos.y += 5000.0f;
            } else {
                i_this->mAction = e_yc_class::ACT_F_FLY;
                i_this->mMode = 0;
                i_this->mFlyAnmMode = 1;
            }
        }

        break;
    }

    cLib_addCalcAngleS2(&_this->current.angle.y, player->shape_angle.y + 0x8000, 2, 0x1000);
}

static void anm_se_set(e_yc_class* i_this) {
    if ((i_this->mAnm == e_yc_class::ANM_FLY && i_this->mpMorf->checkFrame(28.0f))
        || (i_this->mAnm == e_yc_class::ANM_CATCH_START && i_this->mpMorf->checkFrame(27.0f))
        || (i_this->mAnm == e_yc_class::ANM_CATCH_MIDDLE && i_this->mpMorf->checkFrame(11.0f))
        || (i_this->mAnm == e_yc_class::ANM_CATCH_END
            && (i_this->mpMorf->checkFrame(11.0f) || i_this->mpMorf->checkFrame(36.0f))))
    {
        i_this->mCreatureSound.startCreatureSound(Z2SE_EN_YC_WING, 0, -1);
    } else if (i_this->mAnm == e_yc_class::ANM_FLY_GLIDE) {
        i_this->mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YC_GLIDE, 0, -1);
    } else if (i_this->mAnm == e_yc_class::ANM_HANGED_BRUSH && i_this->mpMorf->checkFrame(1.0f)) {
        i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_YC_V_BRUSH, -1);
    } else if (i_this->mAnm == e_yc_class::ANM_HANGED_BRUSH2 && i_this->mpMorf->checkFrame(1.0f)) {
        i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_YC_V_BRUSH2, -1);
    } else if (i_this->mAnm == e_yc_class::ANM_HANGED && i_this->mpMorf->checkFrame(1.0f)) {
        i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_YC_V_HANGED, -1);
        i_this->mCreatureSound.startCreatureSound(Z2SE_EN_YC_WING, 0, -1);
    } else if (i_this->mAnm == e_yc_class::ANM_HANGED_BRUSH && i_this->mpMorf->checkFrame(7.0f)) {
        i_this->mCreatureSound.startCreatureSound(Z2SE_EN_YC_WING, 0, -1);
    } else if (i_this->mAnm == e_yc_class::ANM_HANGED_BRUSH2
        && (i_this->mpMorf->checkFrame(1.0f) || i_this->mpMorf->checkFrame(6.0f)
            || i_this->mpMorf->checkFrame(14.0f) || i_this->mpMorf->checkFrame(36.0f)))
    {
        i_this->mCreatureSound.startCreatureSound(Z2SE_EN_YC_WING, 0, -1);
    } else if (i_this->mAnm == e_yc_class::ANM_HANGED_DAMAGE) {
        if (i_this->mpMorf->checkFrame(1.0f)) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_YC_V_DAMAGE, -1);
        } else if (i_this->mpMorf->checkFrame(7.0f) || i_this->mpMorf->checkFrame(15.0f)) {
            i_this->mCreatureSound.startCreatureSound(Z2SE_EN_YC_WING, 0, -1);
        }
    } else if (i_this->mAnm == e_yc_class::ANM_HOVERING) {
        i_this->mCreatureSound.startCreatureVoiceLevel(Z2SE_EN_YC_V_HOVERING, -1);
        if (i_this->mpMorf->checkFrame(11.0f)) {
            i_this->mCreatureSound.startCreatureSound(Z2SE_EN_YC_WING, 0, -1);
        }
    }
}

static void action(e_yc_class* i_this) {
    cXyz vec1, vec2;
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);

    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(_this);
    i_this->mDistToPlayer = fopAcM_searchPlayerDistanceXZ(_this);
    damage_check(i_this);

    switch (i_this->mAction) {
    case e_yc_class::ACT_FLY:
        e_yc_fly(i_this);
        break;
    case e_yc_class::ACT_F_FLY:
        e_yc_f_fly(i_this);
        break;
    case e_yc_class::ACT_HOVERING:
        e_yc_hovering(i_this);
        break;
    case e_yc_class::ACT_ATTACK:
        e_yc_attack(i_this);
        break;
    case e_yc_class::ACT_WOLFBITE:
        e_yc_wolfbite(i_this);
        break;
    }

    i_this->mCreatureSound.setLinkSearch(false);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = _this->speedF;
    cMtx_YrotS(*calc_mtx, _this->current.angle.y);
    cMtx_XrotM(*calc_mtx, _this->current.angle.x);
    MtxPosition(&vec1, &_this->speed);
    _this->current.pos += _this->speed;

    if (i_this->field_0x69c > 0.1f) {
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = -i_this->field_0x69c;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6a0);
        MtxPosition(&vec1, &vec2);
        _this->current.pos += vec2;
        cLib_addCalc0(&i_this->field_0x69c, 1.0f, 5.0f);
    }

    cLib_addCalcAngleS2(&_this->shape_angle.y, _this->current.angle.y, 4, 0x2000);
    cLib_addCalcAngleS2(&_this->shape_angle.x, 0, 4, 0x2000);
    cLib_addCalcAngleS2(&_this->shape_angle.z, _this->current.angle.z, 4, 0x2000);
    
    _this->current.pos.y -= 200.0f;
    _this->old.pos.y -= 200.0f;
    i_this->mAcch.CrrPos(dComIfG_Bgsp());
    _this->current.pos.y += 200.0f;
    _this->old.pos.y += 200.0f;
}

static int daE_YC_Execute(e_yc_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz vec1, vec2;

    f32 dist_x = -103171.0f;
    f32 dist_z = 38369.0f;
    dist_x -= player->current.pos.x;
    dist_z -= player->current.pos.z;
    S_area_dis = JMAFastSqrt(dist_x * dist_x + dist_z * dist_z);

    i_this->onWolfNoLock();
    
    i_this->mCounter++;
    for (int i = 0; i < 4; i++) {
        if (i_this->mTimer[i] != 0) {
            i_this->mTimer[i]--;
        }
    }
    if (i_this->mCcDisableTimer != 0) {
        i_this->mCcDisableTimer--;
    }

    action(i_this);

    J3DModel* model = i_this->mpMorf->getModel();
    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
    anm_se_set(i_this);

    static int wing_j[2] = {12, 17};

    if (i_this->mAnm == e_yc_class::ANM_FLY_GLIDE || i_this->mAnm == e_yc_class::ANM_HOVERING
        || i_this->mAnm == e_yc_class::ANM_FLY_BRAKE || i_this->mAnm == e_yc_class::ANM_CATCH_START
        || i_this->mAnm == e_yc_class::ANM_CATCH_MIDDLE || i_this->mAnm == e_yc_class::ANM_CATCH_END
        || i_this->mAnm == e_yc_class::ANM_FLY)
    {
        for (int i = 0; i < 2; i++) {
            i_this->mParticleKey[i] = dComIfGp_particle_set(i_this->mParticleKey[i], 0x8607,
                                                            &i_this->current.pos, NULL, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mParticleKey[i]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(model->getAnmMtx(wing_j[i] + 1));
            }
        }
    }

    if (i_this->mpMorf->checkFrame(1.0f)) {
        if (i_this->mAnm == e_yc_class::ANM_HANGED_DAMAGE
            || i_this->mAnm == e_yc_class::ANM_HANGED_BRUSH
            || i_this->mAnm == e_yc_class::ANM_HANGED_BRUSH2)
        {
            for (int i = 0; i < 2; i++) {
                JPABaseEmitter* emitter = dComIfGp_particle_set(0x8603, &i_this->current.pos,
                                                                NULL, NULL);
                if (emitter != NULL) {
                    emitter->setGlobalRTMatrix(model->getAnmMtx(wing_j[i] + 1));
                }
            }
        }

        if (i_this->mAnm == e_yc_class::ANM_HANGED) {
            JPABaseEmitter* emitter = dComIfGp_particle_set(0x8604, &i_this->current.pos,
                                                            NULL, NULL);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(model->getAnmMtx(1));
            }
        }
    }

    mDoMtx_stack_c::transS(i_this->current.pos.x,
                           i_this->current.pos.y + i_this->field_0x694,
                           i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.mScale, l_HIO.mScale, l_HIO.mScale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->modelCalc();

    MTXCopy(model->getAnmMtx(6), *calc_mtx);
    vec1.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&vec1, &i_this->eyePos);

    if (i_this->mAction == e_yc_class::ACT_WOLFBITE) {
        i_this->attention_info.position = i_this->current.pos;
        i_this->attention_info.position.y += 200.0f;
    } else {
        i_this->attention_info.position = i_this->eyePos;
        i_this->attention_info.position.y += 50.0f;
    }

    MTXCopy(model->getAnmMtx(1), *calc_mtx);
    vec1.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&vec1, &vec2);
    if (i_this->mCcDisableTimer != 0) {
        vec2.z -= 21000.0f;
    }
    i_this->mCcSph.SetC(vec2);
    i_this->mCcSph.SetR(80.0f * l_HIO.mScale);
    dComIfG_Ccsp()->Set(&i_this->mCcSph);

    mDoMtx_stack_c::copy(model->getAnmMtx(2));
    vec1.set(0.0f, -30.0f, 0.0f);
    mDoMtx_stack_c::multVec(&vec1, &vec2);
    i_this->setDownPos(&vec2);

    return 1;
}

static int daE_YC_IsDelete(e_yc_class* i_this) {
    return 1;
}

static int daE_YC_Delete(e_yc_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "E_yc");

    if (i_this->mHIOInit) {
        hioInit = false;
    }

    if (i_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_yc_class* _this = static_cast<e_yc_class*>(i_this);

    _this->mpMorf =
        new mDoExt_McaMorfSO(static_cast<J3DModelData*>(dComIfG_getObjectRes("E_yc", 24)), NULL,
                             NULL, static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("E_yc", 21)),
                             2, 1.0f, 0, -1, &_this->mCreatureSound, 0x80000, 0x11000084);
    if (_this->mpMorf == NULL || _this->mpMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static cPhs__Step daE_YC_Create(fopAc_ac_c* i_this) {
    e_yc_class* _this = static_cast<e_yc_class*>(i_this);
    fopAcM_ct(i_this, e_yc_class);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&_this->mPhase, "E_yc");
    if (step == cPhs_COMPLEATE_e) {
        _this->mParam = (u8)fopAcM_GetParam(_this);

        if (!fopAcM_entrySolidHeap(_this, useHeapInit, 0x23a0)) {
            return cPhs_ERROR_e;
        }

        if (!hioInit) {
            _this->mHIOInit = true;
            hioInit = true;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(_this, _this->mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(_this, -500.0f, -300.0f, -500.0f);
        fopAcM_SetMax(_this, 500.0f, 300.0f, 500.0f);
        
        _this->health = 60;
        _this->field_0x560 = 60;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        _this->mCcStts.Init(100, 0, _this);
        _this->mCcSph.Set(cc_sph_src);
        _this->mCcSph.SetStts(&_this->mCcStts);
        _this->mAtSph.Set(at_sph_src);
        _this->mAtSph.SetStts(&_this->mCcStts);
        _this->mAcch.Set(fopAcM_GetPosition_p(_this), fopAcM_GetOldPosition_p(_this), _this, 1,
                         &_this->mAcchCir, fopAcM_GetSpeed_p(_this), NULL, NULL);
        _this->mAcchCir.SetWall(50.0f, 50.0f);

        _this->mCreatureSound.init(&_this->current.pos, &_this->eyePos, 3, 1);
        _this->mCreatureSound.setEnemyName("E_yc");
        _this->mAtInfo.mpSound = &_this->mCreatureSound;

        _this->mCounter = cM_rndF(0xffff);
        _this->mAction = e_yc_class::ACT_FLY;
        
        _this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        _this->attention_info.distances[fopAc_attn_BATTLE_e] = 26;

        daE_YC_Execute(_this);
    }

    return step;
}

static actor_method_class l_daE_YC_Method = {
    (process_method_func)daE_YC_Create,
    (process_method_func)daE_YC_Delete,
    (process_method_func)daE_YC_Execute,
    (process_method_func)daE_YC_IsDelete,
    (process_method_func)daE_YC_Draw,
};

extern actor_process_profile_definition g_profile_E_YC = {
    fpcLy_CURRENT_e,
    4,
    fpcPi_CURRENT_e,
    PROC_E_YC,
    &g_fpcLf_Method.base,
    sizeof(e_yc_class),
    0,
    0,
    &g_fopAc_Method.base,
    0xBB,
    &l_daE_YC_Method,
    0x40100,
    fopAc_ENEMY_e,
    fopAc_CULLBOX_CUSTOM_e,
};
