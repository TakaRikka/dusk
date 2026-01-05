/**
 * d_a_e_ba.cpp
 * Enemy - Keese
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ba.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"
#include "f_op/f_op_actor_enemy.h"

class daE_BA_HIO_c {
public:
    daE_BA_HIO_c();
    virtual ~daE_BA_HIO_c() {}

    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 mFlySpeed;
    /* 0x10 */ f32 mFightDistance;
    /* 0x14 */ f32 mFightSpeed;
    /* 0x18 */ f32 mAttackSpeed;
};

STATIC_ASSERT(sizeof(daE_BA_HIO_c) == 0x1C);

static bool hio_set;

static daE_BA_HIO_c l_HIO;

daE_BA_HIO_c::daE_BA_HIO_c() {
    field_0x04 = -1;
    mScale = 1.0f;
    mFlySpeed = 15.0f;
    mFightDistance = 250.0f;
    mFightSpeed = 15.0f;
    mAttackSpeed = 40.0f;
}

static void ba_disappear(fopAc_ac_c* i_this) {
    fopAcM_createDisappear(i_this, &i_this->current.pos, 6, 0, 3);
    int sw = fopAcM_GetParam(i_this) >> 24;
    if (sw != 0xff) {
        dComIfGs_onSwitch(sw, fopAcM_GetRoomNo(i_this));
    }
}

static void anm_init(e_ba_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    J3DAnmTransform* anm =
        static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(i_this->mArcName, i_index));
    i_this->mpMorf->setAnm(anm, i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_index;
}

static int daE_BA_Draw(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
    i_this->mpMorf->entryDL();
    return 1;
}

static void* shot_b_sub(void* i_proc, void* i_this) {
    if (fopAcM_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_BOOMERANG
        && !dComIfGp_checkPlayerStatus0(0, 0x80000) && daPy_py_c::checkBoomerangCharge()
        && fopAcM_GetParam(i_proc) == 1)
    {
        return i_proc;
    }
    return NULL;
}

static BOOL other_bg_check(e_ba_class* i_this, fopAc_ac_c* i_other) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(&i_this->mEnemy);
    dBgS_LinChk lin_chk;
    cXyz vec1, vec2;
    vec2 = i_other->current.pos;
    vec2.y += 100.0f;
    vec1 = _this->current.pos;
    vec1.y = _this->eyePos.y;
    lin_chk.Set(&vec1, &vec2, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return true;
    } else {
        return false;
    }
}

static BOOL pl_check(e_ba_class* i_this, f32 i_maxDistance, s16 i_maxAngle) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (!daPy_getPlayerActorClass()->checkSwimUp() || dComIfGp_event_runCheck()) {
        return false;
    }

    if (player->current.pos.y < a_this->current.pos.y && i_this->mPlayerDistanceXZ < i_maxDistance)
    {
        s16 angle = a_this->shape_angle.y - i_this->mPlayerAngleY;
        if (i_maxAngle == 1 || (angle < i_maxAngle && angle > (s16)-i_maxAngle)) {
            if (!other_bg_check(i_this, player)) {
                return true;
            }
        }
    }

    return false;
}

static void damage_check(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
    if (i_this->mIFrames == 0) {
        i_this->mStts.Move();
        if (i_this->mSph.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();
            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                i_this->mAction = e_ba_class::ACT_WIND;
                i_this->mMode = 0;
                i_this->mType = e_ba_class::TYPE_NORMAL;
                i_this->mSph.SetAtType(AT_TYPE_CSTATUE_SWING);
            } else {
                cc_at_check(a_this, &i_this->mAtInfo);
                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)
                    || i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT))
                {
                    a_this->health--;
                }
                
                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                    i_this->mAction = e_ba_class::ACT_CHANCE;
                    i_this->mMode = 0;
                    i_this->mKnockbackSpeed = 70.0f;
                    i_this->mKnockbackAngle = a_this->shape_angle.y;
                    i_this->mIsDying = false;
                    dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                } else if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK)
                                            && player->onWolfEnemyCatch(a_this)) {
                    i_this->mAction = e_ba_class::ACT_WOLFBITE;
                    i_this->mMode = 0;
                    i_this->mIFrames = 200;
                    dScnPly_c::setPauseTimer(0);
                    i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_BITE, -1);
                } else {
                    if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                        i_this->mIFrames = 20;
                    } else {
                        i_this->mIFrames = 10;
                    }
                    i_this->mKnockbackSpeed = 80.0f;
                    i_this->mKnockbackAngle = i_this->mAtInfo.mHitDirection.y;
                    if (a_this->health <= 0) {
                        i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_DEATH, -1);
                        i_this->mpMorf->setPlaySpeed(0.2f);
                        i_this->mIsDying = true;
                    }
                }
            }
        }
    }
}

static BOOL path_check(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    if (i_this->mpPath != NULL) {
        dBgS_LinChk lin_chk;
        cXyz vec1, vec2;
        vec1 = a_this->current.pos;
        vec1.y += 100.0f;
        static bool check_index[255];
        dPnt* point = i_this->mpPath->m_points;
        for (int i = 0; i < i_this->mpPath->m_num; i++, point++) {
            vec2.x = point->m_position.x;
            vec2.y = point->m_position.y + 100.0f;
            vec2.z = point->m_position.z;
            lin_chk.Set(&vec1, &vec2, a_this);
            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                check_index[i] = true;
            } else {
                check_index[i] = false;
            }
        }

        f32 delta_x, delta_y, delta_z;
        f32 threshold = 0.0f;
        bool bvar5 = false;
        for (int i = 0; i < 100; i++, threshold += 50.0f) {
            point = i_this->mpPath->m_points;
            for (int j = 0; j < i_this->mpPath->m_num; j++, point++) {
                if (check_index[j]) {
                    delta_x = a_this->current.pos.x - point->m_position.x;
                    delta_y = a_this->current.pos.y - point->m_position.y;
                    delta_z = a_this->current.pos.z - point->m_position.z;
                    f32 dist =
                        JMAFastSqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
                    if (dist < threshold) {
                        i_this->mPathPoint = j - i_this->mPathStep;
                        if (i_this->mPathPoint >= (s8)i_this->mpPath->m_num) {
                            i_this->mPathPoint = i_this->mpPath->m_num;
                        } else if (i_this->mPathPoint < 0) {
                            i_this->mPathPoint = 0;
                        }
                        bvar5 = true;
                        break;
                    }
                }
            }
            if (bvar5) {
                break;
            }
        }

        if (!bvar5) {
            i_this->field_0x5bc = 0;
        } else {
            i_this->field_0x5bc = i_this->mPathIndex + 1;
            return true;
        }
    }

    return false;
}

static void fly_move(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    f32 delta_x = i_this->mTargetPos.x - a_this->current.pos.x;
    f32 delta_y = i_this->mTargetPos.y - a_this->current.pos.y;
    f32 delta_z = i_this->mTargetPos.z - a_this->current.pos.z;
    s16 angle_y = cM_atan2s(delta_x, delta_z);
    f32 dist_xz = JMAFastSqrt(delta_x * delta_x + delta_z * delta_z);
    s16 angle_x = -cM_atan2s(delta_y, dist_xz);
    cLib_addCalcAngleS2(&a_this->current.angle.y, angle_y, 10,
                        i_this->mBaseAngleSpeed * i_this->mSpeedRatio);
    i_this->mBaseAngleSpeed = 2000.0f;
    cLib_addCalcAngleS2(&a_this->current.angle.x, angle_x, 10,
                        i_this->mBaseAngleSpeed * i_this->mSpeedRatio);
    cLib_addCalc2(&i_this->mSpeedRatio, 1.0f, 1.0f, 0.04f);

    cXyz vec;
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = a_this->speedF;
    mDoMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    mDoMtx_XrotM(*calc_mtx, a_this->current.angle.x);
    MtxPosition(&vec, &a_this->speed);
    a_this->current.pos += a_this->speed;
}

static void e_ba_roof(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_WAIT, 15.0f,
                 J3DFrameCtrl::EMode_LOOP, cM_rndF(0.1f) + 0.9f);
        i_this->mMode = 1;
        break;

    case 1:
        if ((i_this->mCounter & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_NAKU, -1);
        }
        break;
    }

    cLib_addCalc2(&a_this->current.pos.x, a_this->home.pos.x, 0.5f, fabsf(a_this->speed.x));
    cLib_addCalc2(&a_this->current.pos.y, a_this->home.pos.y, 0.5f, fabsf(a_this->speed.y));
    cLib_addCalc2(&a_this->current.pos.z, a_this->home.pos.z, 0.5f, fabsf(a_this->speed.z));
    
    if (pl_check(i_this, i_this->mFightFlyDistance, 1)) {
        i_this->mAction = e_ba_class::ACT_FIGHT_FLY;
        i_this->mMode = 0;
    }
}

static void e_ba_fight_fly(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_FLY, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        i_this->mSpeedRatio = 0.0f;
        break;

    case 1:
        if ((i_this->mCounter & 0xf) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_NAKU, -1);
        }
        break;
    }

    cLib_addCalc2(&a_this->speedF, l_HIO.mFlySpeed, 1.0f, l_HIO.mFlySpeed * 0.3f);
    i_this->mTargetPos = player->current.pos;
    fly_move(i_this);

    if (!pl_check(i_this, i_this->mFightFlyDistance + 50.0f, 1)) {
        if (i_this->mHomeType != e_ba_class::HOME_APPEAR) {
            if (!path_check(i_this)) {
                if (i_this->mHomeType == e_ba_class::HOME_ROOF) {
                    i_this->mAction = e_ba_class::ACT_RETURN;
                    i_this->mMode = 0;
                } else {
                    i_this->mAction = e_ba_class::ACT_FLY;
                    i_this->mMode = 0;
                }
            } else {
                i_this->mAction = e_ba_class::ACT_PATH_FLY;
                i_this->mMode = 0;
            }
        }
    } else {
        if (pl_check(i_this, l_HIO.mFightDistance, 1)) {
            i_this->mAction = e_ba_class::ACT_FIGHT;
            i_this->mMode = 0;
        }
    }
}

static void e_ba_fight(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    s16 player_angle = player->shape_angle.y;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_HOVERING, 2.0f,
                 J3DFrameCtrl::EMode_LOOP, cM_rndF(0.1f) + 1.0f);
        i_this->mMode = 1;
        i_this->mTimer[0] = 0;
        i_this->mTimer[1] = cM_rndF(100.0f) + 30.0f;
        break;

    case 1:
        if (i_this->mTimer[0] == 0) {
            mDoMtx_YrotS(*calc_mtx, player_angle + (s16)cM_rndFX(12288.0f));
            cXyz vec;
            vec.x = 0.0f;
            vec.y = cM_rndF(100.0f) + 150.0f;
            vec.z = cM_rndF(150.0f) + 150.0f;
            MtxPosition(&vec, &i_this->mTargetPos);
            i_this->mTargetPos += player->current.pos;
            vec = i_this->mTargetPos - a_this->current.pos;
            mDoMtx_YrotS(*calc_mtx, cM_atan2s(vec.x, vec.z));
            f32 dist_xz = JMAFastSqrt(vec.x * vec.x + vec.z * vec.z);
            mDoMtx_XrotM(*calc_mtx, -cM_atan2s(vec.y, dist_xz));
            vec.x = 0.0f;
            vec.y = 0.0f;
            vec.z = l_HIO.mFightSpeed;
            MtxPosition(&vec, &a_this->speed);
            i_this->mTimer[0] = cM_rndF(30.0f) + 10.0f;
            i_this->mSpeedRatio = 0.0f;
        }

        if (i_this->mTimer[1] == 0) {
            i_this->mTimer[1] = cM_rndF(100.0f) + 30.0f;
            if (i_this->mHomeType != e_ba_class::HOME_APPEAR || cM_rndF(1.0f) < 0.2f) {
                i_this->mAction = e_ba_class::ACT_ATTACK;
                i_this->mMode = 0;
            }
        }
        break;
    }

    cLib_addCalc2(&a_this->current.pos.x, i_this->mTargetPos.x, 0.2f,
                  i_this->mSpeedRatio * fabsf(a_this->speed.x));
    cLib_addCalc2(&a_this->current.pos.y, i_this->mTargetPos.y, 0.2f,
                  i_this->mSpeedRatio * fabsf(a_this->speed.y));
    cLib_addCalc2(&a_this->current.pos.z, i_this->mTargetPos.z, 0.2f,
                  i_this->mSpeedRatio * fabsf(a_this->speed.z));
    cLib_addCalc2(&i_this->mSpeedRatio, 1.0f, 1.0f, 0.1f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 4, 0x800);

    if (i_this->mHomeType != e_ba_class::HOME_APPEAR
        && !pl_check(i_this, i_this->mFightFlyDistance + 50.0f, 1))
    {
        if (!path_check(i_this)) {
            if (i_this->mHomeType == e_ba_class::HOME_ROOF) {
                i_this->mAction = e_ba_class::ACT_RETURN;
                i_this->mMode = 0;
            } else {
                i_this->mAction = e_ba_class::ACT_FLY;
                i_this->mMode = 0;
            }
        } else {
            i_this->mAction = e_ba_class::ACT_PATH_FLY;
            i_this->mMode = 0;
        }
    }
}

static void e_ba_attack(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 target_speed = 0.0f;
    i_this->mSpeedRatio = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_FLY, 3.0f, J3DFrameCtrl::EMode_LOOP, 2.0f);
        i_this->mMode = 1;
        i_this->mTimer[1] = 20;
        break;

    case 1:
        i_this->mTargetPos = player->current.pos;
        i_this->mTargetPos.y += 120.0f;
        i_this->mSpeedRatio = 2.0f;
        if (i_this->mTimer[1] == 0) {
            i_this->mMode = 2;
            i_this->mTimer[0] = 15;
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_ATTACK, -1);
        }
        break;

    case 2:
        target_speed = l_HIO.mAttackSpeed;
        if (i_this->mSph.ChkAtShieldHit()) {
            i_this->mAction = e_ba_class::ACT_CHANCE;
            i_this->mMode = 0;
            i_this->mKnockbackSpeed = 70.0f;
            i_this->mKnockbackAngle = a_this->shape_angle.y;
            i_this->mIsDying = false;
            dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        } else {
            if (i_this->mTimer[0] == 0) {
                i_this->mMode = 3;
            }
        }
        break;

    case 3:
        if (a_this->speedF <= 1.0f) {
            i_this->mAction = e_ba_class::ACT_FIGHT;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, l_HIO.mAttackSpeed * 0.2f);
    fly_move(i_this);
}

static void e_ba_fly(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_FLY, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        break;

    case 1:
        if ((i_this->mCounter & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_NAKU, -1);
        }
        if (i_this->mTimer[0] == 0) {
            i_this->mTargetPos.x = a_this->home.pos.x + cM_rndFX(500.0f);
            i_this->mTargetPos.y = a_this->home.pos.y + cM_rndFX(200.0f);
            i_this->mTargetPos.z = a_this->home.pos.z + cM_rndFX(500.0f);
            cXyz vec = i_this->mTargetPos - a_this->current.pos;
            mDoMtx_YrotS(*calc_mtx, cM_atan2s(vec.x, vec.z));
            mDoMtx_XrotM(*calc_mtx, -cM_atan2s(vec.y, JMAFastSqrt(vec.x * vec.x + vec.z * vec.z)));
            vec.x = 0.0f;
            vec.y = 0.0f;
            vec.z = l_HIO.mFightSpeed;
            MtxPosition(&vec, &a_this->speed);
            i_this->mTimer[0] = cM_rndF(30.0f) + 10.0f;
            i_this->mSpeedRatio = 0.0f;
        }
        break;
    }

    cLib_addCalc2(&a_this->speedF, l_HIO.mFlySpeed, 1.0f, l_HIO.mFlySpeed * 0.3f);
    fly_move(i_this);
    if (pl_check(i_this, i_this->mFightFlyDistance, 1)) {
        i_this->mAction = e_ba_class::ACT_FIGHT_FLY;
        i_this->mMode = 0;
    }
}

static void e_ba_return(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_FLY, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        i_this->mSpeedRatio = 0.0f;

    case 1:
        break;
    }

    cLib_addCalc2(&a_this->speedF, l_HIO.mFlySpeed, 1.0f, l_HIO.mFlySpeed * 0.3f);
    i_this->mTargetPos = a_this->home.pos;
    fly_move(i_this);

    cXyz delta = a_this->current.pos - i_this->mTargetPos;
    if (delta.abs() < 100.0f) {
        i_this->mAction = e_ba_class::ACT_ROOF;
        i_this->mMode = 0;
    } else {
        if (pl_check(i_this, i_this->mFightFlyDistance, 1)) {
            i_this->mAction = e_ba_class::ACT_FIGHT_FLY;
            i_this->mMode = 0;
        }
    }
}

static void e_ba_path_fly(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    dPnt* point;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_FLY, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        // fallthrough

    case 1:
        if ((i_this->mCounter & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_NAKU, -1);
        }
        i_this->mPathPoint += i_this->mPathStep;
        if (i_this->mPathPoint >= (s8)i_this->mpPath->m_num) {
            if (dPath_ChkClose(i_this->mpPath)) {
                i_this->mPathPoint = 0;
            } else {
                i_this->mPathStep = -1;
                i_this->mPathPoint = i_this->mpPath->m_num - 2;
            }
            int next_id = i_this->mpPath->m_nextID;
            if (next_id != 0xffff) {
                i_this->mpPath = dPath_GetRoomPath(next_id, fopAcM_GetRoomNo(a_this));
            }
        } else {
            if (i_this->mPathPoint < 0) {
                i_this->mPathStep = 1;
                i_this->mPathPoint = 1;
            }
        }
        // fallthrough

    case 2:
        i_this->mMode = 3;
        point = i_this->mpPath->m_points;
        point = &point[i_this->mPathPoint];
        i_this->mSpeedRatio = 0.0f;
        i_this->mTargetPos.x = point->m_position.x + cM_rndFX(150.0f);
        i_this->mTargetPos.y = point->m_position.y + cM_rndFX(150.0f);
        i_this->mTargetPos.z = point->m_position.z + cM_rndFX(150.0f);
        break;

    case 3:
        cXyz delta = i_this->mTargetPos - a_this->current.pos;
        if (delta.abs() < 200.0f) {
            i_this->mMode = 1;
        }
        break;
    }

    cLib_addCalc2(&a_this->speedF, l_HIO.mFlySpeed, 1.0f, l_HIO.mFlySpeed * 0.3f);
    fly_move(i_this);
}

static void e_ba_chance(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_HOVERING, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.5f);
        i_this->mMode = 1;
        i_this->mTimer[0] = cM_rndF(30.0f) + 100.0f;
        a_this->speed.x = 0.0f;
        a_this->speed.y = 0.0f;
        a_this->speed.z = 0.0f;
        i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_BITE, -1);
        break;

    case 1:
        if (i_this->mAcch.ChkGroundHit()) {
            a_this->speed.y = cM_rndF(10.0f) + 10.0f;
            a_this->speed.x = cM_rndFX(10.0f);
            a_this->speed.z = cM_rndFX(10.0f);
            if ( cM_rndF(1.0f) < 0.5f) {
                i_this->mChanceAngle.z = 0;
            } else {
                i_this->mChanceAngle.z = -0x8000;
            }
            i_this->mChanceAngle.y = cM_rndF(0x10000);
            fopAcM_effSmokeSet1(&i_this->mSmokeKey1, &i_this->mSmokeKey2,
                                &a_this->current.pos, &a_this->shape_angle, 0.8f,
                                &a_this->tevStr, 1);
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_FAINT, -1);
        }

        if (i_this->mTimer[0] == 0) {
            a_this->current.angle.z = 0;
            i_this->mAction = e_ba_class::ACT_FIGHT;
            i_this->mMode = 0;
            return;
        }
        break;
    }

    a_this->current.pos += a_this->speed;
    a_this->speed.y -= 2.0f;
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mChanceAngle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&a_this->current.angle.z, i_this->mChanceAngle.z, 2, 0x1000);
}

static void e_ba_wolfbite(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_HOLDWAIT, 0.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        break;

    case 1:
        if (!player->checkWolfEnemyCatchOwn(a_this)) {
            if (player->checkWolfEnemyLeftThrow()) {
                a_this->current.angle.y = player->shape_angle.y + 0x4000;
            } else {
                a_this->current.angle.y = player->shape_angle.y - 0x4000;
            }
            a_this->speedF = 40.0f;
            a_this->speed.y = -20.0f;
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_DEATH, -1);
            anm_init(i_this, e_ba_class::ANM_DEAD, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mTimer[0] = 60;
            i_this->mMode = 2;
            a_this->health = 0;
        }
        break;

    case 2:
        if (i_this->mAcch.ChkGroundHit()) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_DEATH2, -1);
            i_this->mCreatureSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
            i_this->mMode = 3;
        }
        // fallthrough

    case 3:
        if (i_this->mTimer[0] == 0) {
            ba_disappear(a_this);
            fopAcM_delete(a_this);
        }
        break;
    }

    cXyz vec1, vec2;
    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = a_this->speedF;
    mDoMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    MtxPosition(&vec1, &vec2);
    a_this->speed.x = vec2.x;
    a_this->speed.z = vec2.z;
    a_this->current.pos += a_this->speed;
    a_this->speed.y -= 4.0f;
    if (i_this->mAcch.ChkGroundHit()) {
        cLib_addCalc0(&a_this->speedF, 1.0f, 15.0f);
    }
}

static void e_ba_wind(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    fopAc_ac_c* boomerang = (fopAc_ac_c*)fpcM_Search(shot_b_sub, i_this);
    a_this->speedF = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_FURA2, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        i_this->mWindSpinSpeed = -(cM_rndFX(1000.0f) + 15000.0f);
        i_this->mWindOffset.x = cM_rndFX(50.0f);
        i_this->mWindOffset.y = cM_rndFX(50.0f);
        i_this->mWindOffset.z = cM_rndFX(50.0f);
        // fallthrough

    case 1:
        if (boomerang == NULL) {
            i_this->mMode = 2;
            i_this->mTimer[0] = 60;
        } else {
            a_this->current.pos = boomerang->current.pos + i_this->mWindOffset;
            i_this->mCreatureSound.startCreatureVoiceLevel(Z2SE_EN_BA_V_SPIN, -1);
        }
        break;

    case 2:
        cLib_addCalcAngleS2(&i_this->mWindSpinSpeed, 0, 4, 450);
        if (i_this->mTimer[0] == 0) {
            i_this->mAction = e_ba_class::ACT_FIGHT_FLY;
            i_this->mMode = 0;
        }
    }

    a_this->current.angle.y += i_this->mWindSpinSpeed;
    a_this->shape_angle.y = a_this->current.angle.y;
    a_this->current.angle.z = 0;
}

static void e_ba_appear(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    cXyz vec;
    i_this->mIFrames = 60;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_ba_class::ANM_APPEAR, 0.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        i_this->mTimer[0] = cM_rndF(20.0f) + 40.0f;
        a_this->speedF = 30.0f;
        mDoMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        vec.x = 0.0f;
        vec.y = 0.0f;
        vec.z = 100000.0f;
        MtxPosition(&vec, &i_this->mTargetPos);
        i_this->mTargetPos += a_this->current.pos;
        // fallthrough

    case 1:
        cLib_addCalc0(&a_this->speedF, 1.0f, 0.7f);
        if (i_this->mTimer[0] == 0 || a_this->speedF < 0.1f || i_this->mAcch.ChkWallHit()) {
            i_this->mAction = e_ba_class::ACT_FIGHT_FLY;
            i_this->mMode = 0;
        }
        break;
    }

    fly_move(i_this);
}

static void action(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    cXyz vec1, vec2, vec3;
    i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(a_this);
    i_this->mPlayerDistanceXZ = fopAcM_searchPlayerDistanceXZ(a_this);
    a_this->field_0x566 = 0;
    damage_check(i_this);
    i_this->mSph.OffAtVsPlayerBit();

    s8 link_search = false;
    switch (i_this->mAction) {
    case e_ba_class::ACT_ROOF:
        e_ba_roof(i_this);
        break;
    case e_ba_class::ACT_FIGHT_FLY:
        e_ba_fight_fly(i_this);
        a_this->field_0x566 = 1;
        link_search = true;
        break;
    case e_ba_class::ACT_FIGHT:
        e_ba_fight(i_this);
        a_this->field_0x566 = 1;
        link_search = true;
        break;
    case e_ba_class::ACT_ATTACK:
        e_ba_attack(i_this);
        i_this->mSph.OnAtVsPlayerBit();
        a_this->field_0x566 = 1;
        link_search = true;
        break;
    case e_ba_class::ACT_RETURN:
        e_ba_return(i_this);
        break;
    case e_ba_class::ACT_FLY:
        e_ba_fly(i_this);
        a_this->field_0x566 = 1;
        break;
    case e_ba_class::ACT_PATH_FLY:
        e_ba_path_fly(i_this);
        break;
    case e_ba_class::ACT_CHANCE:
        e_ba_chance(i_this);
        break;
    case e_ba_class::ACT_WOLFBITE:
        e_ba_wolfbite(i_this);
        break;
    case e_ba_class::ACT_WIND:
        e_ba_wind(i_this);
        break;
    case e_ba_class::ACT_APPEAR:
        e_ba_appear(i_this);
        link_search = true;
        break;
    }

    if (link_search) {
        i_this->mCreatureSound.setLinkSearch(true);
    } else {
        i_this->mCreatureSound.setLinkSearch(false);
    }

    if (i_this->mKnockbackSpeed > 0.1f) {
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = -i_this->mKnockbackSpeed;
        mDoMtx_YrotS(*calc_mtx, i_this->mKnockbackAngle);
        MtxPosition(&vec1, &vec2);
        a_this->current.pos += vec2;
        cLib_addCalc0(&i_this->mKnockbackSpeed, 1.0f, 5.0f);
        if (i_this->mIsDying) {
            a_this->shape_angle.y += 0x1300;
            a_this->shape_angle.z += 0x1700;
            if (i_this->mKnockbackSpeed <= 1.0f || i_this->mAcch.ChkWallHit()) {
                fopAcM_delete(a_this);
                if (i_this->mHomeType == e_ba_class::HOME_APPEAR) {
                    // should be fpcNm_ITEM_HEART : fpcNm_ITEM_ARROW_10 but that gives incorrect code
                    int item_no = dComIfGs_getLife() <= 4 ? 0 : 0xE;
                    fopAcM_createItem(&a_this->current.pos, item_no, -1, -1, NULL, NULL, 0);
                    fopAcM_createDisappear(a_this, &a_this->current.pos, 6, 0, 0xff);
                } else {
                    ba_disappear(a_this);
                }
            }
        }
    } else {
        if (i_this->mAction != e_ba_class::ACT_WIND) {
            cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x2000);
            cLib_addCalcAngleS2(&a_this->shape_angle.x, 0, 4, 0x2000);
            cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 4, 0x2000);
        }
    }

    a_this->current.pos.y -= 30.0f;
    a_this->old.pos.y -= 30.0f;
    i_this->mAcch.CrrPos(dComIfG_Bgsp());
    a_this->current.pos.y += 30.0f;
    a_this->old.pos.y += 30.0f;

    vec3.x = 0.5f;
    vec3.y = 0.5f;
    vec3.z = 0.5f;
    setMidnaBindEffect(a_this, &i_this->mCreatureSound, &a_this->eyePos, &vec3);
}

static int daE_BA_Execute(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));

    i_this->mCounter++;
    for (int i = 0; i < 4; i++) {
        if (i_this->mTimer[i] != 0) {
            i_this->mTimer[i]--;
        }
    }
    if (i_this->mIFrames != 0) {
        i_this->mIFrames--;
    }

    action(i_this);

    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    if ((i_this->mAnm == e_ba_class::ANM_HOVERING || i_this->mAnm == e_ba_class::ANM_FLY)) {
        if (i_this->mpMorf->checkFrame(4.0f)) {
            if (i_this->mAnm == e_ba_class::ANM_HOVERING) {
                i_this->mCreatureSound.startCreatureSound(Z2SE_EN_BA_WING, 0, -1);
            } else {
                i_this->mCreatureSound.startCreatureSound(Z2SE_EN_BA_WING, 0, -1);
            }
        }
    } else if (i_this->mAnm == e_ba_class::ANM_FURA2) {
        if (i_this->mpMorf->checkFrame(0.0f)) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_BA_V_FURA, -1);
        }
    }

    J3DModel* model = i_this->mpMorf->getModel();
    if (i_this->mAction == e_ba_class::ACT_WOLFBITE && i_this->mMode < 2) {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
        MTXCopy(daPy_getLinkPlayerActorClass()->getWolfMouthMatrix(), mDoMtx_stack_c::get());
        model->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&a_this->current.pos);
    } else {
        if (a_this->health > 0 && !i_this->mIsDying
                    && player->current.pos.y < a_this->current.pos.y) {
            fopAcM_OnStatus(a_this, 0);
            a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        } else {
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags = 0;
        }
        mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
        mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
        mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
        mDoMtx_stack_c::scaleM(l_HIO.mScale, l_HIO.mScale, l_HIO.mScale);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    i_this->mpMorf->modelCalc();
    MTXCopy(model->getAnmMtx(2), *calc_mtx);

    cXyz zero;
    zero.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&zero, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += 20.0f;

    cXyz center;
    zero.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&zero, &center);
    if (i_this->mIFrames != 0) {
        center.z -= 20000.0f;
    }
    i_this->mSph.SetC(center);
    i_this->mSph.SetR(l_HIO.mScale * 30.0f);
    dComIfG_Ccsp()->Set(&i_this->mSph);

    if (i_this->mType != e_ba_class::TYPE_NORMAL) {
        i_this->field_0x6b0 = a_this->current.pos - a_this->old.pos;
        i_this->field_0x6b0 *= 0.6f;
        if (i_this->mType == e_ba_class::TYPE_FIRE) {
            static u16 fire_name[4] = {0x8216, 0x8217, 0x8218, 0x8219};
            if (i_this->mAction != e_ba_class::ACT_APPEAR) {
                for (int i = 0; i < 4; i++) {
                    i_this->mParticleKey[i] =
                        dComIfGp_particle_set(i_this->mParticleKey[i], fire_name[i],
                                              &a_this->current.pos, NULL, NULL);
                    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mParticleKey[i]);
                    if (emitter != NULL) {
                        emitter->setGlobalSRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(18));
                        emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                        emitter->setUserWork((uintptr_t)&i_this->field_0x6b0);
                    }
                }
                i_this->mCreatureSound.startCreatureSoundLevel(Z2SE_EN_BA_FIRE, 0, -1);
            }
        } else {
            static u16 ice_name[3] = {0x821a, 0x821b, 0x821c};
            for (int i = 0; i < 3; i++) {
                i_this->mParticleKey[i] =
                    dComIfGp_particle_set(i_this->mParticleKey[i], ice_name[i],
                                          &a_this->current.pos, NULL, NULL);
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mParticleKey[i]);
                if (emitter != NULL) {
                    emitter->setGlobalSRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(18));
                    emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                    emitter->setUserWork((uintptr_t)&i_this->field_0x6b0);
                }
            }
        }
    }

    a_this->attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;
    return 1;
}

static int daE_BA_IsDelete(e_ba_class* i_this) {
    return 1;
}

static int daE_BA_Delete(e_ba_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->mEnemy;
    dComIfG_resDelete(&i_this->mPhase, i_this->mArcName);
    if (i_this->mHIOInit) {
        hio_set = false;
    }
    if (a_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_ba_class* _this = (e_ba_class*)i_this;
    _this->mpMorf = new mDoExt_McaMorfSO(
        static_cast<J3DModelData*>(dComIfG_getObjectRes(_this->mArcName, 13)), NULL, NULL,
        static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(_this->mArcName, 10)),
        2, 1.0f, 0, -1, &_this->mCreatureSound, 0x80000, 0x11000084);
    if (_this->mpMorf == NULL || _this->mpMorf->mpModel == NULL) {
        return 0;
    } else {
        return 1;
    }
}

static cPhs__Step daE_BA_Create(fopAc_ac_c* i_this) {
    static char* arc_name[3] = {"E_ba", "E_fb", "E_ib"};

    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
            {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    fopAcM_ct(i_this, e_ba_class);
    e_ba_class* _this = (e_ba_class*)i_this;

    _this->mType = (fopAcM_GetParam(_this) & 0xf000) >> 12;
    if (_this->mType > 2) {
        _this->mType = e_ba_class::TYPE_NORMAL;
    }
    _this->mArcName = arc_name[_this->mType];

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&_this->mPhase, _this->mArcName);

    if (step == cPhs_COMPLEATE_e) {
        int sw = fopAcM_GetParam(_this) >> 24;
        if (sw != 0xff && dComIfGs_isSwitch(sw, fopAcM_GetRoomNo(i_this))) {
            return cPhs_ERROR_e;
        }

        _this->mHomeType = fopAcM_GetParam(_this) & 0xff;
        _this->mDistanceParam = (fopAcM_GetParam(_this) >> 8) & 0xf;
        _this->mPathIndex = (fopAcM_GetParam(_this) >> 16) & 0xff;
        if (_this->mHomeType == 0xff) {
            _this->mHomeType = e_ba_class::HOME_ROOF;
        }
        if (_this->mDistanceParam == 0xf) {
            _this->mDistanceParam = 10;
        }

        _this->mFightFlyDistance = (f32)_this->mDistanceParam * 100.0f;
        if (fopAcM_SearchByName(PROC_E_PZ) != NULL) {
            _this->mFightFlyDistance = 100000.0f;
        }

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x2000)) {
            return cPhs_ERROR_e;
        }

        if (_this->mPathIndex != 0xff) {
            _this->mpPath = dPath_GetRoomPath(_this->mPathIndex, fopAcM_GetRoomNo(i_this));
            if (_this->mpPath == NULL) {
                return cPhs_ERROR_e;
            }
            _this->field_0x5bc = _this->mPathIndex + 1;
            _this->mPathStep = 1;
            _this->mAction = e_ba_class::ACT_PATH_FLY;
        } else {
            if (_this->mHomeType == e_ba_class::HOME_FLY) {
                _this->mAction = e_ba_class::ACT_FLY;
            } else if (_this->mHomeType == e_ba_class::HOME_APPEAR) {
                _this->mAction = e_ba_class::ACT_APPEAR;
                _this->mIFrames = 10;
            }
        }

        if (!hio_set) {
            _this->mHIOInit = true;
            hio_set = true;
            l_HIO.field_0x04 = -1;
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(i_this, _this->mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);
        i_this->health = 1;
        i_this->field_0x560 = 1;

        _this->mStts.Init(30, 0, i_this);
        _this->mSph.Set(cc_sph_src);
        _this->mSph.SetStts(&_this->mStts);
        if (_this->mType == e_ba_class::TYPE_FIRE) {
            _this->mSph.SetAtType(0x100);
            _this->mSph.SetAtMtrl(dCcD_MTRL_FIRE);
        } else if (_this->mType == e_ba_class::TYPE_ICE) {
            _this->mSph.SetAtType(0x100);
            _this->mSph.SetAtMtrl(dCcD_MTRL_ICE);
        }

        _this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this,
                          1, &_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        _this->mAcchCir.SetWall(50.0f, 50.0f);

        _this->mCreatureSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        _this->mCreatureSound.setEnemyName("E_ba");
        _this->mAtInfo.mpSound = &_this->mCreatureSound;
        _this->mAtInfo.mPowerType = 1;
        _this->mCounter = cM_rndF(0xFFFF);

        daE_BA_Execute(_this);
    }
    return step;
}

static actor_method_class l_daE_BA_Method = {
    (process_method_func)daE_BA_Create,
    (process_method_func)daE_BA_Delete,
    (process_method_func)daE_BA_Execute,
    (process_method_func)daE_BA_IsDelete,
    (process_method_func)daE_BA_Draw,
};

actor_process_profile_definition g_profile_E_BA = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_E_BA,
    &g_fpcLf_Method.base,
    sizeof(e_ba_class),
    0,
    0,
    &g_fopAc_Method.base,
    0xB4,
    &l_daE_BA_Method,
    0x10050100,
    fopAc_ENEMY_e,
    fopAc_CULLBOX_CUSTOM_e,
};
