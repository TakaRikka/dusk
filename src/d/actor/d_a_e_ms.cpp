/**
* @file d_a_e_ms.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ms.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_obj_carry.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor_enemy.h"

class daE_MS_HIO_c : public JORReflexible {
public:
    daE_MS_HIO_c();
    virtual ~daE_MS_HIO_c() {};

    void genMessage(JORMContext*);

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 mTargetSpeed1;
    /* 0x10 */ f32 mSpeedF;
    /* 0x14 */ f32 mTargetSpeed2;
    /* 0x18 */ f32 mPlayerDistanceLimit;
};

daE_MS_HIO_c::daE_MS_HIO_c() {
    field_0x4 = -1;
    mScale = 1.0f;
    mTargetSpeed1 = 15.0f;
    mSpeedF = 40.0f;
    mTargetSpeed2 = 4.0f;
    mPlayerDistanceLimit = 450.0f;
}

static void anm_init(e_ms_class* i_this, int param_2, f32 param_3, u8 param_4, f32 param_5) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_MS", param_2), param_4,
                                param_3, param_5, 0.0f, -1.0f);
    i_this->mNextSound = param_2;
}

static int pl_check(e_ms_class* i_this, f32 i_dist) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (i_this->mPlayerDistance < i_dist && !fopAcM_otherBgCheck(i_this, player)) {
        return 1;
    }

    return 0;
}

static int daE_MS_Draw(e_ms_class* i_this) {
    if (i_this->field_0xba5) {
        return 1;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);

    dComIfGd_setListDark();
    i_this->mpModelMorf->entryDL();
    dComIfGd_setList();

    cXyz shadow_pos(i_this->current.pos.x, i_this->current.pos.y + 100.0f, i_this->current.pos.z);
    i_this->mShadowKey =
        dComIfGd_setShadow(i_this->mShadowKey, 1, model, &shadow_pos, 400.0f, 40.0f,
                           i_this->current.pos.y, i_this->mAcch.m_ground_h, i_this->mAcch.m_gnd,
                           &i_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static void sibuki_set(e_ms_class* i_this) {
    if (!i_this->mCooldown2) {
        i_this->mCooldown2 = 20;
        cXyz pos(i_this->current.pos);
        pos.y = i_this->field_0x690;
        fopKyM_createWpillar(&pos, 1.0f, 0);
        if (i_this->field_0x690 - i_this->mAcch.m_ground_h > 50.0f) {
            i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_S, 0, -1);
        } else {
            i_this->mSound.startCreatureSound(Z2SE_EN_MS_FALLWATER, 0, -1);
        }
    }
}

static void ms_disappear(e_ms_class* i_this) {
    fopAcM_createDisappear(i_this, &i_this->eyePos, 10, 0, 0x26);
    int no = (fopAcM_GetParam(i_this) >> 0x10) & 0xff;
    if (no != 0xff) {
        dComIfGs_onSwitch(no, fopAcM_GetRoomNo(i_this));
    }

    if (i_this->field_0x5b6 == 1) {
        i_this->current = i_this->home;
        i_this->old = i_this->current;
        i_this->health = 1;
        i_this->mAction = 0;
        i_this->mMode = 0;
        if (i_this->field_0x5b8 != 0xff) {
            i_this->field_0xba6 = 1;
        }
    } else {
        fopAcM_delete(i_this);
    }
}

/* 807297FD 0003+00 data_807297FD None */
static u8 hioInit;

static daE_MS_HIO_c l_HIO;

static fopAc_ac_c* target_info[20];

static int target_info_count;

static void* s_d_sub(void* param_1, void* param_2) {
    daObjCarry_c* i_this = (daObjCarry_c*)param_1;
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == PROC_Obj_Carry && i_this->getType() == daObjCarry_c::TYPE_DOKURO &&
        target_info_count < 20)
    {
        target_info[target_info_count] = (fopAc_ac_c*)param_1;
        target_info_count++;
    }

    return NULL;
}

static fopAc_ac_c* search_dokuro(e_ms_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    target_info_count = 0;

    for (int i = 0; i < 20; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_d_sub, i_this);

    f32 fVar1 = 100.0f;

    if (target_info_count != 0) {
        fopAc_ac_c* target_actor;
        int i = 0;
        while (i < target_info_count) {
            target_actor = target_info[i];
            cXyz diff = target_actor->current.pos - actor->current.pos;

            if (JMAFastSqrt(diff.x * diff.x + diff.z * diff.z) < fVar1 &&
                !fopAcM_otherBgCheck(actor, target_info[i]))
            {
                return target_actor;
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                fVar1 += 100.0f;
                if (fVar1 > 1000.0f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }
    return NULL;
}

static int way_set(e_ms_class* i_this) {
    fopAc_ac_c* actor = i_this;

    s16 s1 = cM_rndF(65535.0f);
    f32 fVar2 = 1000.0f;
    cXyz start_pos = actor->current.pos;
    start_pos.y += 50.0f;
    cXyz end_pos;

    for (int k = 0; k < 5; k++) {
        for (int i = 0; i < 16; i++) {
            cMtx_YrotS(*calc_mtx, s1);
            cXyz spdc(0.0f, 0.0f, fVar2);
            MtxPosition(&spdc, &end_pos);
            end_pos += start_pos;

            dBgS_LinChk line_check;
            line_check.Set(&start_pos, &end_pos, actor);

            if (dComIfG_Bgsp().LineCross(&line_check)) {
                s1 += 0x1000;
            } else {
                i_this->field_0x5c8 = s1;
                return 1;
            }
        }
        fVar2 -= 150.0f;
    }

    return 0;
}

static s8 e_ms_normal(e_ms_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    f32 fVar1 = 0.0f;
    s8 rv = 1;

    if ((i_this->mLifetime & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_NZ_V_NAKU, -1);
    }

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 14, 3.0f, 2, 1.0f);
        i_this->mActionTimer[0] = cM_rndF(60.0f) + 30;
        i_this->mMode = 1;
        i_this->mLifetime = cM_rndF(65535.0f);
        i_this->mActionTimer[2] = 20;
        break;
    case 1:
        fVar1 = l_HIO.mTargetSpeed1;
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->field_0x5c8 += (int)cM_rndFX(2000.0f);
        }

        if (i_this->mActionTimer[0] == 0 ||
            i_this->mActionTimer[2] == 0 && fopAcM_wayBgCheck(actor, 200.0f, 50.0f))
        {
            i_this->mActionTimer[0] = cM_rndF(60.0f) + 30;
            i_this->mMode = 2;
            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(i_this, 16, 3.0f, 2, 1.0f);
            } else {
                anm_init(i_this, 13, 3.0f, 2, 1.0f);
            }
        }
        break;
    case 2:
        if (i_this->mActionTimer[0] == 0) {
            if (way_set(i_this)) {
                i_this->mMode = 0;
            } else {
                i_this->mActionTimer[0] = 10;
            }
        }
        break;
    case 5:
        rv = 0;
        if (i_this->mActionTimer[0] == 0) {
            anm_init(i_this, 14, 3.0f, 2, 1.5f);
            i_this->mActionTimer[0] = cM_rndF(60.0f) + 30;
            i_this->mMode = 6;
            i_this->field_0x5c8 = i_this->mPlayerAngleY + 0x8000;
        }
        break;
    case 6:
        rv = 0;
        fVar1 = l_HIO.mTargetSpeed1 * 1.5f;
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->field_0x5c8 += (int)cM_rndFX(4000.0f);
        }

        if (i_this->mActionTimer[2] == 0 && fopAcM_wayBgCheck(i_this, 200.0f, 50.0f)) {
            i_this->mActionTimer[2] = 20;
            i_this->field_0x5c8 = actor->current.angle.y + 0x8000;
        }

        if (i_this->mActionTimer[0] == 0) {
            i_this->mMode = 0;
        }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x5c8, 2, 0x1000);
    cLib_addCalc2(&actor->speedF, fVar1, 1.0f, l_HIO.mTargetSpeed1 * 0.25f);

    if (i_this->field_0xba6 || (rv != 0 && pl_check(i_this, l_HIO.mPlayerDistanceLimit))) {
        i_this->mAction = 1;
        i_this->mMode = 0;
    }

    return rv;
}

static s8 e_ms_attack(e_ms_class* i_this) {
    f32 fVar1 = 0.0f;
    s8 rv = 1;
    s8 sVar1 = 0;

    switch (i_this->mMode) {
    case 0:
        i_this->mSound.startCreatureVoice(Z2SE_EN_NZ_V_NAKU, -1);
        anm_init(i_this, 8, 3.0f, 0, 1.0f);
        i_this->mMode = 1;
        break;
    case 1:
        i_this->field_0x5c8 = i_this->mPlayerAngleY;
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 14, 3.0f, 2, 1.0f);
            i_this->mMode = 2;
        }
        break;
    case 2:
        fVar1 = l_HIO.mTargetSpeed1;
        i_this->field_0x5c8 = i_this->mPlayerAngleY;
        if (i_this->mPlayerDistance < 300.0f) {
            i_this->mMode = 3;
            anm_init(i_this, 12, 3.0f, 0, 1.0f);
        }
        break;
    case 3:
        i_this->field_0x5c8 = i_this->mPlayerAngleY;
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 11, 1.0f, 0, 1.0f);
            i_this->speed.y = 30.0f;
            i_this->speedF = l_HIO.mSpeedF;
            fVar1 = l_HIO.mSpeedF;
            i_this->mMode = 4;
        }
        break;
    case 4:
        rv = 0;
        i_this->field_0xb84 = 1;
        fVar1 = i_this->speedF = l_HIO.mSpeedF;
        if (i_this->mSph2.ChkAtShieldHit()) {
            dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            i_this->field_0xb84 = 0;
            fVar1 = i_this->speedF = 0.0f;
        }

        if (i_this->field_0xb84 == 0 || i_this->mAcch.ChkGroundHit()) {
            anm_init(i_this, 10, 1.0f, 0, 1.0f);
            i_this->mMode = 5;
        }
        break;
    case 5:
        rv = 0;
        if (i_this->mpModelMorf->isStop()) {
            sVar1 = 1;
            i_this->field_0xba6 = 0;
        }
    }

    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->field_0x5c8, 2, 0x1000);
    cLib_addCalc2(&i_this->speedF, fVar1, 1.0f, 10.0f);

    if (i_this->field_0xba6 == 0 && (sVar1 || !pl_check(i_this, l_HIO.mPlayerDistanceLimit + 100.0f))) {
        i_this->mAction = 0;
        i_this->mMode = 0;
    }

    return rv;
}

static void search_ground_1(e_ms_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    dBgS_LinChk dStack_174;
    cXyz sp104[16];
    cXyz sp180, sp18c, sp198, sp1a4, sp1b0;
    s16 mShapeAngleY = i_this->shape_angle.y;
    s8 sVar1 = 0;

    for (int i = 0; i < 16; i++) {
        sp180 = actor->current.pos;
        sp180.y = i_this->field_0x690;
        cMtx_YrotS(*calc_mtx, mShapeAngleY);

        sp198.set(0.0f, 0.0f, 2000.0f);
        MtxPosition(&sp198, &sp18c);
        sp18c += sp180;

        dStack_174.Set(&sp180, &sp18c, actor);
        if (dComIfG_Bgsp().LineCross(&dStack_174)) {
            sp18c = dStack_174.GetCross();
            sp198.set(0.0f, 30.0f, 100.0f);
            MtxPosition(&sp198, &sp180);
            sp180 += sp18c;
            sp18c = sp180;
            sp18c.y -= 70.0f;
            dStack_174.Set(&sp180, &sp18c, actor);
            if (dComIfG_Bgsp().LineCross(&dStack_174)) {
                sp104[i] = dStack_174.GetCross();
                sVar1 = 1;
            } else {
                sp104[i] = actor->current.pos;
                sp104[i].x += 10000.0f;
            }
        }

        mShapeAngleY += 0x1000;
    }

    if (sVar1) {
        int idx = 0;
        for (int i = 1; i < 16; i++) {
            if ((sp104[idx] - actor->current.pos).abs() > (sp104[i] - actor->current.pos).abs()) {
                idx = i;
            }
        }

        i_this->field_0x5bc = sp104[idx];
    } else {
        i_this->field_0x5bc.x = actor->current.pos.x + cM_rndFX(300.0f);
        i_this->field_0x5bc.y = actor->current.pos.y;
        i_this->field_0x5bc.z = actor->current.pos.z + cM_rndFX(300.0f);
    }
}

static void e_ms_swim(e_ms_class* i_this) {
    s16 sVar3 = 0;
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 15, 5.0f, 2, 1.0f);
        i_this->mMode++;
        i_this->speedF = 0.0f;
        search_ground_1(i_this);
        i_this->mActionTimer[1] = 50;
    case 1:
        sVar3 = 0x300;
        if ((i_this->mLifetime & 0x3f) == 0 && cM_rndF(1.0f) < 0.5f) {
            search_ground_1(i_this);
        }

        cXyz sp28 = i_this->field_0x5bc - i_this->current.pos;
        i_this->field_0x5c8 = cM_atan2s(sp28.x, sp28.z);
    }

    cLib_addCalc2(&i_this->speedF, l_HIO.mTargetSpeed2, 1.0f, 0.5f);
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->field_0x5c8, 0x20, sVar3);
    i_this->speed.y = 0.0f;
    i_this->gravity = 0.0f;
    f32* mCurrentPosY = &i_this->current.pos.y;
    cLib_addCalc2(mCurrentPosY, i_this->field_0x690 - 40.0f, 1.0f, 5.0f);

    if (i_this->mActionTimer[1] == 0 && i_this->mAcch.ChkWallHit()) {
        i_this->current.pos.y += 5.0f;
        i_this->speed.y = 20.0f;
        i_this->speedF = 20.0f;
        i_this->mAction = 0;
        anm_init(i_this, 14, 3.0f, 2, 1.0f);
        i_this->mMode = 2;
        i_this->mActionTimer[0] = 20;
    }
}

static void e_ms_dokuro(e_ms_class* i_this) {
    daObjCarry_c* mObjCarry = (daObjCarry_c*)fopAcM_SearchByID(i_this->mSkullID);

    if (!mObjCarry || i_this->mAcch.ChkWallHit() || fopAcM_checkCarryNow(mObjCarry)) {
        i_this->mAction = 0;
        i_this->mMode = 0;
    } else {
        cXyz sp4c = mObjCarry->current.pos - i_this->current.pos;
        f32 fVar7 = sp4c.abs();
        i_this->field_0x5c8 = cM_atan2s(sp4c.x, sp4c.z);
        f32 fVar6 = 0.0f;

        switch (i_this->mMode) {
        case 0:
            i_this->mSound.startCreatureVoice(Z2SE_EN_NZ_V_NAKU, -1);
            anm_init(i_this, 8, 3.0f, 0, 1.0f);
            i_this->mMode = 1;
            break;
        case 1:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 14, 3.0f, 2, 1.0f);
                i_this->mMode = 2;
            }
            break;
        case 2:
            if (fVar7 < 75.0f) {
                anm_init(i_this, 9, 3.0f, 2, 1.0f);
                i_this->mActionTimer[0] = cM_rndF(60.0f) + 90;
                i_this->mMode = 3;
            } else {
                fVar6 = l_HIO.mTargetSpeed1;
            }
            break;
        case 3:
            if (fVar7 < 65.0f) {
                fVar6 = -2.0f;
            } else {
                if (fVar7 > 70.0f) {
                    fVar6 = 2.0f;
                }
            }

            if (i_this->mActionTimer[0] == 0) {
                anm_init(i_this, 13, 3.0f, 2, 1.0f);
                i_this->mActionTimer[0] = cM_rndF(60.0f) + 60;
                i_this->mMode = 4;
            }
            break;
        case 4:
            if (i_this->mActionTimer[0] == 0) {
                i_this->mMode = 2;
            }
        }

        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->field_0x5c8, 2, 0x2000);
        cLib_addCalc2(&i_this->speedF, fVar6, 1.0f, 10.0f);
        if (pl_check(i_this, l_HIO.mPlayerDistanceLimit)) {
            i_this->mAction = 1;
            i_this->mMode = 0;
        }
    }
}

static void e_ms_damage(e_ms_class* i_this) {
    i_this->mCooldown1 = 6;
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 5, 2.0f, 2, 1.0f);
        i_this->mMode = 1;
        i_this->speed.y = cM_rndF(10.0f) + 40.0f;
        i_this->speedF = -30.0f;
        break;
    case 1:
        if (i_this->mAcch.ChkGroundHit()) {
            if (i_this->health <= 0) {
                anm_init(i_this, 6, 5.0f, 0, 1.0f);
                i_this->mActionTimer[0] = 0x28;
                i_this->mMode = 2;
                i_this->speed.y = 10.0f;
                i_this->speedF *= 0.5f;
            } else {
                i_this->mAction = 1;
                i_this->mMode = 0;
            }
            i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
        } else if (i_this->speed.y <= 0.0f && i_this->mInWater) {
            anm_init(i_this, 7, 3.0f, 0, 1.0f);
            sibuki_set(i_this);
            i_this->mActionTimer[0] = 0x50;
            i_this->mMode = 2;
        }
        break;
    case 2:
        if (i_this->speed.y <= 0.0f && (i_this->mAcch.ChkGroundHit() || i_this->mInWater)) {
            i_this->speedF *= 0.5f;
        }

        if (i_this->mActionTimer[0] == 0) {
            ms_disappear(i_this);
        }
    }

    if (i_this->speed.y <= 0.0f && i_this->current.pos.y <= i_this->field_0x690 - 20.0f) {
        i_this->speed.y = 0.0f;
        i_this->gravity = 0.0f;
        cLib_addCalc2(&i_this->current.pos.y, i_this->field_0x690 - 20.0f, 1.0f, 5.0f);
    }
}

static void e_ms_wolfbite(e_ms_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 4, 0.0f, 0, 1.0f);
        i_this->mMode = 1;
        i_this->mSound.startCreatureVoice(Z2SE_EN_MS_V_DAMAGE, -1);
        break;
    case 1:
        if (!player->checkWolfEnemyCatchOwn(i_this)) {
            i_this->mAction = 4;
            i_this->mMode = 0;
            i_this->mSound.startCreatureVoice(Z2SE_EN_MS_V_DEATH, -1);
            i_this->health = 0;
            if (player->checkWolfEnemyLeftThrow()) {
                i_this->current.angle.y = player->shape_angle.y - 0x4000;
            } else {
                i_this->current.angle.y = player->shape_angle.y + 0x4000;
            }
        }
    }
}

static void e_ms_standby(e_ms_class* i_this) {
    i_this->mCooldown1 = 6;
    if (dComIfGs_isSwitch(i_this->field_0x5b8, fopAcM_GetRoomNo(i_this))) {
        i_this->field_0xba5 = 0;
        i_this->mAction = 0;
        i_this->mMode = 0;
    }
}

static void damage_check(e_ms_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (i_this->mCooldown1 == 0) {
        i_this->mDStts.Move();
        if (i_this->mSph1.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSph1.GetTgHitObj();
            cc_at_check(i_this, &i_this->mAtInfo);
            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT) ||
                i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT))
            {
                i_this->health = 0;
            }

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                i_this->mCooldown1 = 20;
            } else {
                i_this->mCooldown1 = 10;
            }

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK) &&
                player->onWolfEnemyCatch(i_this))
            {
                i_this->mAction = 5;
                i_this->mMode = 0;
                i_this->mCooldown1 = 200;
                dScnPly_c::setPauseTimer(0);
                return;
            }

            i_this->mAction = 4;
            i_this->mMode = 0;
            i_this->current.angle.y = i_this->mAtInfo.mHitDirection.y;
            if (i_this->health != 0 && i_this->mAtInfo.mHitType == 0x10) {
                i_this->health = 1;
            } else {
                i_this->mSound.startCreatureVoice(Z2SE_EN_MS_V_DEATH, -1);
            }
        }

        if (i_this->health <= 10) {
            i_this->mSph1.SetTgHitMark((CcG_Tg_HitMark)3);
        }
    }
}

static void action(e_ms_class* i_this) {
    fopAc_ac_c* actor = i_this;
    i_this->mPlayerDistance = fopAcM_searchPlayerDistance(actor);
    i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(actor);
    damage_check(i_this);
    s8 sVar10 = 0;
    s8 sVar11 = 1;
    s8 sVar3 = 0;
    s8 sVar1 = 1;
    s8 iVar4 = 1;
    s8 link_search = 0;

    fopAc_ac_c* mActor;

    dBgS_ObjGndChk_Spl cStack_9c;
    cXyz spa8 = i_this->current.pos;
    spa8.y += 200.0f;
    cStack_9c.SetPos(&spa8);
    i_this->field_0x690 = dComIfG_Bgsp().GroundCross(&cStack_9c);
    cXyz spb4;

    switch (i_this->mAction) {
    case 0:
        sVar10 = e_ms_normal(i_this);
        sVar3 = 1;
        break;
    case 1:
        sVar10 = e_ms_attack(i_this);
        link_search = 1;
        break;
    case 2:
        e_ms_dokuro(i_this);
        break;
    case 3:
        e_ms_swim(i_this);
        sVar11 = 0;
        break;
    case 4:
        e_ms_damage(i_this);
        sVar11 = -1;
        iVar4 = 0;
        break;
    case 5:
        e_ms_wolfbite(i_this);
        iVar4 = 0;
        sVar11 = -1;
        sVar1 = 0;
        break;
    case 10:
        e_ms_standby(i_this);
        iVar4 = 0;
        sVar11 = -1;
        sVar1 = 0;
    }

    if (sVar1 && i_this->health > 0) {
        fopAcM_OnStatus(i_this, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(i_this, 0);
        actor->attention_info.flags = 0;
    }

    if (link_search) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (sVar3 && (i_this->mSkullID = -1, (i_this->mLifetime & 15) == 0) &&
        (mActor = search_dokuro(i_this), mActor))
    {
        spa8 = mActor->current.pos - i_this->current.pos;
        if (spa8.abs() < 500.0f) {
            i_this->mSkullID = fopAcM_GetID(mActor);
            i_this->mAction = 2;
            i_this->mMode = 0;
        }
    }

    if ((sVar10 && i_this->mPlayerDistance < 500.0f) && daPy_getPlayerActorClass()->checkWolfBark()) {
        i_this->mAction = 0;
        i_this->mMode = 5;
        anm_init(i_this, 8, 1.0f, 0, 1.0f);
        i_this->mActionTimer[0] = cM_rndF(10.0f) + 5;
    }

    cLib_addCalcAngleS2(&i_this->shape_angle.y, i_this->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&i_this->shape_angle.x, i_this->current.angle.x, 2, 0x2000);
    cLib_addCalcAngleS2(&i_this->shape_angle.z, i_this->current.angle.z, 2, 0x2000);
    cMtx_YrotS(*calc_mtx, i_this->current.angle.y);

    spa8.x = 0.0f;
    spa8.y = 0.0f;
    spa8.z = i_this->speedF;
    MtxPosition(&spa8, &spb4);
    i_this->speed.x = spb4.x;
    i_this->speed.z = spb4.z;

    i_this->current.pos += i_this->speed * l_HIO.mScale;
    i_this->speed.y += i_this->gravity;
    i_this->gravity = -7.0f;
    if (i_this->speed.y < -80.0f) {
        i_this->speed.y = -80.0f;
    }

    if (iVar4) {
        i_this->mSph1.OnCoSetBit();
        cXyz* pcVar9 = i_this->mDStts.GetCCMoveP();
        if (pcVar9) {
            i_this->current.pos.x += pcVar9->x * 0.3f;
            i_this->current.pos.z += pcVar9->z * 0.3f;
        }
    } else {
        i_this->mSph1.OffCoSetBit();
    }

    i_this->mAcch.CrrPos(dComIfG_Bgsp());
    if (i_this->current.pos.y <= i_this->field_0x690 - 10.0f) {
        i_this->mInWater = 1;
    } else {
        i_this->mInWater = 0;
    }

    f32 fVar12 = 0.0f;
    if (sVar11 == 1) {
        if (i_this->current.pos.y <= i_this->field_0x690 - 45.0f) {
            i_this->mAction = 3;
            i_this->mMode = 0;
            sibuki_set(i_this);
        }
    } else if (sVar11 == 0) {
        fVar12 = 47.0f;
        cXyz spc0 = i_this->current.pos;
        spc0.y = i_this->field_0x690;
        fopAcM_effHamonSet(&i_this->mHamon, &spc0, 1.0f, 0.1f);

        if (i_this->current.pos.y > i_this->field_0x690 - 35.0f) {
            i_this->mAction = 0;
            i_this->mMode = 0;
        }
    }

    cLib_addCalc2(&i_this->field_0x694, fVar12, 0.4f, 5.0f);
    cXyz eff_size(0.5f, 0.5f, 0.5f);
    setMidnaBindEffect((fopEn_enemy_c*)actor, &i_this->mSound, &i_this->eyePos, &eff_size);
}

static void anm_se_set(e_ms_class* i_this) {
    s8 step = 0;
    int sound = i_this->mNextSound;
    if (sound == 16) {
        if (i_this->mpModelMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MS_V_NAKU, -1);
        }
    } else if (sound == 13) {
        if (i_this->mpModelMorf->checkFrame(0.0f) || i_this->mpModelMorf->checkFrame(44.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MS_V_NAKU, -1);
        }
    } else if (sound == 17) {
        if (i_this->mpModelMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MS_V_NAKU, -1);
        }

        if (i_this->mpModelMorf->checkFrame(6.0f) || i_this->mpModelMorf->checkFrame(15.0f)) {
            step = 1;
        }
    } else if (sound == 14) {
        if (i_this->mpModelMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MS_V_NAKU, -1);
        }

        if (i_this->mpModelMorf->checkFrame(2.0f) || i_this->mpModelMorf->checkFrame(6.0f)) {
            step = 1;
        }
    } else if (sound == 12) {
        if (i_this->mpModelMorf->checkFrame(6.5f) || i_this->mpModelMorf->checkFrame(8.0f)) {
            step = 1;
        }
    } else if (sound == 10) {
        if (i_this->mpModelMorf->checkFrame(2.5f) || i_this->mpModelMorf->checkFrame(6.0f)) {
            step = 1;
        }
    } else if (sound == 8) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MS_V_FIND, -1);
        }
    } else if (sound == 9) {
        if (i_this->mpModelMorf->checkFrame(0.0f) || i_this->mpModelMorf->checkFrame(4.5f) ||
            i_this->mpModelMorf->checkFrame(9.5f) || i_this->mpModelMorf->checkFrame(15.0f))
        {
            i_this->mSound.startCreatureSound(Z2SE_EN_MS_BITE, 0, -1);
        }
    } else if (sound == 15) {
        if (i_this->mpModelMorf->checkFrame(0.0f) || i_this->mpModelMorf->checkFrame(9.5f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MS_SWIM, 0, -1);
        }
    }

    if (step) {
        if (i_this->mInWater != 0) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MS_FN_WATER, 0, -1);
        } else {
            i_this->mSound.startCreatureSound(Z2SE_EN_MS_FOOTNOTE, 0, -1);
        }
    }
}

static int daE_MS_Execute(e_ms_class* i_this) {
    cXyz sp34;
    cXyz sp40;

    i_this->mLifetime++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mActionTimer[i]) {
            i_this->mActionTimer[i]--;
        }
    }

    if (i_this->mCooldown1 != 0) {
        i_this->mCooldown1--;
    }

    if (i_this->mCooldown2 != 0) {
        i_this->mCooldown2--;
    }

    if (strcmp(dComIfGp_getStartStageName(), "D_MN10") == 0 && fopAcM_GetRoomNo(i_this) == 15) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        if (player->current.pos.y < -400.0f && i_this->current.pos.y > -400.0f &&
            !dComIfGs_isSwitch(10, fopAcM_GetRoomNo(i_this)))
        {
            i_this->field_0xba8 = 1;
        } else {
            i_this->field_0xba8 = 0;
        }
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    action(i_this);

    if (i_this->mAction == 5) {
        PSMTXCopy(daPy_getLinkPlayerActorClass()->getWolfMouthMatrix(), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&i_this->current.pos);
        mDoMtx_stack_c::YrotM(0);
        mDoMtx_stack_c::XrotM(-0x8000);
        mDoMtx_stack_c::ZrotM(0x9c4);
        mDoMtx_stack_c::transM(10.0f, -60.0f, -20.0f);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y + i_this->field_0x694,
                               i_this->current.pos.z);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
        mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
        mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    }

    mDoMtx_stack_c::scaleM(l_HIO.mScale, l_HIO.mScale, l_HIO.mScale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpModelMorf->play(i_this->field_0xba8, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
    i_this->mpModelMorf->modelCalc();
    anm_se_set(i_this);

    PSMTXCopy(model->getAnmMtx(9), *calc_mtx);

    sp34.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp34, &i_this->eyePos);
    i_this->attention_info.position = i_this->eyePos;
    i_this->attention_info.position.y += 40.0f;
    sp34.set(-30.0f, 10.0f, 0.0f);
    MtxPosition(&sp34, &sp40);
    i_this->mSph1.SetC(sp40);
    i_this->mSph1.SetR(l_HIO.mScale * 35.0f);
    dComIfG_Ccsp()->Set(&i_this->mSph1);

    if (i_this->field_0xb84 == 0) {
        sp40.x += 10000.0f;
    } else {
        sp34.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp34, &sp40);
        i_this->field_0xb84 = 0;
    }

    i_this->mSph2.SetC(sp40);
    i_this->mSph2.SetR(l_HIO.mScale * 20.0f);
    dComIfG_Ccsp()->Set(&i_this->mSph2);

    if ((i_this->mInWater && i_this->mNextSound == 14) &&
        (i_this->mpModelMorf->checkFrame(1.0f) || i_this->mpModelMorf->checkFrame(7.0f)))
    {
        if (cM_rndF(1.0f) < 0.5f) {
            static u16 eff_id[4] = {
                0x01B8,
                0x01B9,
                0x01BA,
                0x01BB,
            };
            static cXyz sc(0.3f, 0.3f, 0.3f);

            for (int i = 0; i < 4; i++) {
                i_this->mParticleKey[i] = dComIfGp_particle_set(
                    i_this->mParticleKey[i], eff_id[i], &i_this->current.pos, &i_this->tevStr,
                    &i_this->shape_angle, &sc, 0xff, 0, -1, 0, 0, 0);
            }
        }
    }

    return 1;
}

static int daE_MS_IsDelete(e_ms_class* i_this) {
    return 1;
}

static int daE_MS_Delete(e_ms_class* i_this) {
    dComIfG_resDelete(&i_this->mPhaseReq, "E_MS");
    if (i_this->mHIOInit) {
        hioInit = 0;
    }

    if (i_this->heap) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    e_ms_class* i_this = (e_ms_class*)actor;
    mDoExt_McaMorfSO* mMorf =
        new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_MS", 20), NULL, NULL,
                             (J3DAnmTransform*)dComIfG_getObjectRes("E_MS", 16), 2, 1.0f, 0, -1,
                             &i_this->mSound, 0x80000, 0x11000084);
    i_this->mpModelMorf = mMorf;
    if (!i_this->mpModelMorf || !i_this->mpModelMorf->getModel()) {
        return 0;
    }

    return 1;
}

static int daE_MS_Create(fopAc_ac_c* actor) {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xdafbfdff, 0x3}, 0x75}},  // mObj
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
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}},  // mObj
            {dCcD_SE_SOFT_BODY, 0x0, 0x0, 0x0, 0x0},                      // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                           // mGObjTg
            {0x0},                                                        // mGObjCo
        },                                                                // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 20.0f}  // mSph
        }  // mSphAttr
    };

    e_ms_class* i_this = (e_ms_class*)actor;

    fopAcM_ct(i_this, e_ms_class);

    int phase = dComIfG_resLoad(&i_this->mPhaseReq, "E_MS");
    if (phase == cPhs_COMPLEATE_e) {
        int params = fopAcM_GetParam(i_this) >> 0x10 & 0xff;
        if (params != 0xff && dComIfGs_isSwitch(params, fopAcM_GetRoomNo(i_this))) {
            return cPhs_ERROR_e;
        }

        i_this->field_0x5b6 = fopAcM_GetParam(i_this);
        i_this->field_0x5b7 = fopAcM_GetParam(i_this) >> 8;
        i_this->field_0x5b8 = fopAcM_GetParam(i_this) >> 0x18;

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x1860)) {
            return cPhs_ERROR_e;
        }

        if (!hioInit) {
            i_this->mHIOInit = 1;
            hioInit = 1;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(i_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        i_this->health = 1;
        i_this->field_0x560 = 1;

        i_this->mDStts.Init(0x1e, 0, i_this);
        i_this->mSph1.Set(cc_sph_src);
        i_this->mSph1.SetStts(&i_this->mDStts);
        i_this->mSph2.Set(at_sph_src);
        i_this->mSph2.SetStts(&i_this->mDStts);
        i_this->mAcch.Set(&i_this->current.pos, &i_this->old.pos, i_this, 1, &i_this->mAcchCir,
                          &i_this->speed, NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, 50.0f);

        i_this->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        i_this->mSound.setEnemyName("E_ms");
        i_this->mAtInfo.mpSound = &i_this->mSound;

        i_this->mLifetime = cM_rndF(65535.0f);
        if (i_this->field_0x5b8 != 0xff) {
            if (!dComIfGs_isSwitch(i_this->field_0x5b8, fopAcM_GetRoomNo(i_this))) {
                i_this->field_0xba5 = 1;
                i_this->mAction = 10;
            }

            i_this->field_0xba6 = 1;
        }

        daE_MS_Execute(i_this);
    }

    return phase;
}

static actor_method_class l_daE_MS_Method = {
    (process_method_func)daE_MS_Create,  (process_method_func)daE_MS_Delete,
    (process_method_func)daE_MS_Execute, (process_method_func)daE_MS_IsDelete,
    (process_method_func)daE_MS_Draw,
};

extern actor_process_profile_definition g_profile_E_MS = {
    fpcLy_CURRENT_e,       // mLayerID
    7,                     // mListID
    fpcPi_CURRENT_e,       // mListPrio
    PROC_E_MS,             // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(e_ms_class),    // mSize
    0,                     // mSizeOther
    0,                     // mParameters
    &g_fopAc_Method.base,  // sub_method
    177,                   // mPriority
    &l_daE_MS_Method,      // sub_method
    0x00050100,            // mStatus
    fopAc_ENEMY_e,         // mActorType
    fopAc_CULLBOX_0_e,     // cullType
};

AUDIO_INSTANCES;
