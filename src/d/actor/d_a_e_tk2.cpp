/**
 * @file d_a_e_tk2.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_e_tk2.h"
#include "d/actor/d_a_e_tk_ball.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

enum Action {
    /* 0x0 */ ACT_TK2_WAIT,
    /* 0x1 */ ACT_TK2_FIND,
    /* 0x2 */ ACT_TK2_ATTACK,
    /* 0x9 */ ACT_TK2_S_DAMAGE = 0x9,
    /* 0xA */ ACT_TK2_DAMAGE,
};

enum Animation {
    /* 0x4 */ ANM_TK2_APPEAR = 0x4,
    /* 0x5 */ ANM_TK2_ATTACK,
    /* 0x6 */ ANM_TK2_DAMAGE,
    /* 0x7 */ ANM_TK2_HIDE,
    /* 0x8 */ ANM_TK2_KYORO2,
    /* 0x9 */ ANM_TK2_SWIM,
    /* 0xA */ ANM_TK2_WAIT01,
};

enum Mode {  // Not sure if these are correct...
    /* 0x0 */ MODE_TK2_NONE,
    /* 0x1 */ MODE_TK2_APPEAR,
    /* 0x2 */ MODE_TK2_ATTACK,
    /* 0x3 */ MODE_TK2_DAMAGE,
    /* 0x4 */ MODE_TK2_HIDE,
    /* 0xA */ MODE_TK2_KYORO2 = 0xA,
    /* 0xB */ MODE_TK2_SWIM,
    /* 0xC */ MODE_TK2_WAIT01,
};

static bool hio_set;

static daE_TK2_HIO_c l_HIO;

daE_TK2_HIO_c::daE_TK2_HIO_c() {
    mNo = -1;
    mBaseSize = 2.3f;
    mPlayerDetectionDistanceFar = 1400.0f;
    mPlayerDetectionDistanceNear = 500.0f;
    mSwimSpeedModifier = 10.0f;
    mFleeSpeedModifier = 25.0f;
}

#if DEBUG
void daE_TK2_HIO_c::genMessage(JORMContext* ctext) {
    // Tadpole
    ctext->genLabel("  タドポール", 0x80000001, 0U, NULL);
    // Base Size
    ctext->genSlider("基本サイズ", &mBaseSize, 0.0f, 5.0f);
    // Player Detection Distance Far
    ctext->genSlider("PL認識距離　遠", &mPlayerDetectionDistanceFar, 0.0f, 3000.0f);
    // Player Detection Distance Near
    ctext->genSlider("PL認識距離　近", &mPlayerDetectionDistanceNear, 0.0f, 3000.0f);
    ctext->genLabel("  ", 0x80000001);
    // ↓ Behavior not guaranteed if changed
    ctext->genLabel("  ↓変更すると動作保証しません", 0x80000001);
    // Swim Speed
    ctext->genSlider("泳ぎ速度", &mSwimSpeedModifier, 0.0f, 40.0f);
    // Flee Speed
    ctext->genSlider("逃げ速度", &mFleeSpeedModifier, 0.0f, 40.0f);
}
#endif

static void anm_init(e_tk2_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_tk2", i_index), i_attr, i_morf,
                           i_rate, 0.0f, -1.0f);
    i_this->mAnim = i_index;
}

static int daE_TK2_Draw(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;
    J3DModel* model = i_this->mpMorf->getModel();

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
    i_this->mpMorf->entryDL();
    return 1;
}

static int other_bg_check(e_tk2_class* i_this, fopAc_ac_c* i_ac) {
    fopAc_ac_c* actor = i_this;
    dBgS_LinChk line_check;
    cXyz _unk1;
    cXyz start_pos;
    cXyz end_pos;

    end_pos = i_ac->current.pos;
    end_pos.y += 100.0f;
    start_pos = actor->current.pos;
    start_pos.y = actor->eyePos.y;

    line_check.Set(&start_pos, &end_pos, actor);

    if (dComIfG_Bgsp().LineCross(&line_check)) {
        return true;
    } else {
        return false;
    }
}

static int pl_check(e_tk2_class* i_this, f32 i_limit, s16 i_max_diff) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->mPlayerDistanceLimit < i_limit) {
        s16 diff = actor->shape_angle.y - i_this->mPlayerAngleY;
        if (diff < i_max_diff && diff > (s16)-i_max_diff && !other_bg_check(i_this, player)) {
            return 1;
        }
    }

    return 0;
}

static void damage_check(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->mInvincibilityTimer == 0) {
        i_this->mStts.Move();

        if (i_this->mSph.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();
            at_power_check(&i_this->mAtInfo);

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                i_this->mInvincibilityTimer = 20;
            } else {
                i_this->mInvincibilityTimer = 10;
            }

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                i_this->mAction = ACT_TK2_FIND;
                i_this->mMode = MODE_TK2_KYORO2;
                i_this->mActionTimer[0] = 50;
            } else {
                cc_at_check(actor, &i_this->mAtInfo);
                if (i_this->mAtInfo.mHitType == HIT_TYPE_STUN) {
                    i_this->mAction = ACT_TK2_S_DAMAGE;
                } else {
                    i_this->mAction = ACT_TK2_DAMAGE;
                }
                i_this->mMode = MODE_TK2_NONE;
            }
        }

        if (actor->health <= 1) {
            actor->health = 0;
            i_this->mSph.SetTgHitMark(CcG_Tg_UNK_MARK_3);
        }
        if (actor->health <= 1) {  // Yes, this is a duplicate (??)
            actor->health = 0;
            i_this->mSph.SetTgHitMark(CcG_Tg_UNK_MARK_3);
        }
    }
}

static void e_tk2_wait_0(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;

    cXyz src_pos;
    f32 speed_mul = 1.0f;
    f32 speed_step = 1.0f + YREG_F(5);

    i_this->mAttentionOFF = true;

    switch (i_this->mMode) {
    case MODE_TK2_NONE:
        i_this->mMode = MODE_TK2_APPEAR;
        break;

    case MODE_TK2_APPEAR:
        if (i_this->mActionTimer[1] == 0 &&
            pl_check(i_this, l_HIO.mPlayerDetectionDistanceFar, 0x4000))
        {
            i_this->mAction = ACT_TK2_FIND;
            i_this->mMode = MODE_TK2_NONE;
        }
        break;
    }

    i_this->mSomeAngle = i_this->mPlayerAngleY;
    cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->mSomeAngle, 0x10, 0x400);
}

static void e_tk2_find(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;

    cXyz local_58;
    f32 speed_target = 0.0f;
    f32 speed_step = 3.0f;

    switch (i_this->mMode) {
    case MODE_TK2_NONE:
        anm_init(i_this, ANM_TK2_APPEAR, 3.0f, 0x0, 1.0f);
        i_this->mMode = MODE_TK2_APPEAR;
        break;

    case MODE_TK2_APPEAR:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_TK2_WAIT01, 3.0f, 0x2, 1.0f);
            i_this->mMode = MODE_TK2_ATTACK;
            i_this->mActionTimer[0] = cM_rndF(10.0f) + 10.0f;
        }
        break;

    case MODE_TK2_ATTACK:
        if (i_this->mActionTimer[0] == 0) {
            i_this->mAction = ACT_TK2_ATTACK;
            i_this->mMode = MODE_TK2_NONE;
        }
        break;

    case MODE_TK2_DAMAGE:
        anm_init(i_this, ANM_TK2_HIDE, 3.0f, 0x0, 1.0f);
        i_this->mMode = MODE_TK2_HIDE;
        break;

    case MODE_TK2_HIDE:
        i_this->mAttentionOFF = true;
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_TK2_WAIT;
            i_this->mMode = MODE_TK2_NONE;
            i_this->mActionTimer[1] = cM_rndF(30.0f) + 30.0f;
        }
        break;

    case MODE_TK2_KYORO2:
        anm_init(i_this, ANM_TK2_HIDE, 3.0f, 0x0, 3.0f);
        i_this->mMode = MODE_TK2_SWIM;
        break;

    case MODE_TK2_SWIM:
        ADD_ANGLE(i_this->mPlayerAngleY, 0x8000);
        i_this->mAttentionOFF = true;
        if (i_this->mpMorf->isStop()) {
            i_this->mAnimSpeed = 4.0f;
            anm_init(i_this, ANM_TK2_SWIM, 0.0f, 0x2, i_this->mAnimSpeed);
            i_this->mMode = MODE_TK2_WAIT01;
        }
        break;

    case MODE_TK2_WAIT01:
        i_this->mAttentionOFF = true;
        cLib_addCalc2(&i_this->mAnimSpeed, 2.0f, 1.0f, 0.15f);
        i_this->mpMorf->setPlaySpeed(i_this->mAnimSpeed);
        speed_target = 1.0f + YREG_F(17);
        speed_step = 10.0f + YREG_F(18);
        if (i_this->mActionTimer[0] == 0 &&
            i_this->mPlayerDistanceLimit > l_HIO.mPlayerDetectionDistanceNear * 1.2f)
        {
            i_this->mMode = MODE_TK2_NONE;
        } else {
            if (i_this->mActionTimer[1] == 0 && i_this->mAcch.ChkWallHit()) {
                i_this->mActionTimer[1] = cM_rndF(10.0f) + 20.0f;
                i_this->mSomeAngle = i_this->mPlayerAngleY + (s16)cM_rndFX(8000.0f);
            }
        }
        if (i_this->mActionTimer[1] == 0) {
            ADD_ANGLE(i_this->mPlayerAngleY, 0x8000);
        } else {
            i_this->mPlayerAngleY = i_this->mSomeAngle;
        }
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->mPlayerAngleY, 4, 0x800);
}

static void e_tk2_attack(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;

    switch (i_this->mMode) {
    case MODE_TK2_NONE:
        i_this->mMode = MODE_TK2_APPEAR;
        anm_init(i_this, ANM_TK2_ATTACK, 3.0f, 0x00, 1.0f);
        break;

    case MODE_TK2_APPEAR:
        if ((int)i_this->mpMorf->getFrame() == 0x18) {
            i_this->mBallID =
                fopAcM_createChild(PROC_E_TK_BALL, fopAcM_GetID(actor), 1, &actor->eyePos,
                                   fopAcM_GetRoomNo(actor), &actor->shape_angle, 0, -1, 0);
        }
        if ((int)i_this->mpMorf->getFrame() == 0x1c) {
            i_this->mTKBallSpawned = true;
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_TK2_WAIT01, 1.0f, 0x02, 1.0f);
            i_this->mActionTimer[0] = cM_rndF(30.0f) + 70.0f;
            i_this->mMode = MODE_TK2_ATTACK;
        }
        break;

    case MODE_TK2_ATTACK:
        if (i_this->mActionTimer[0] == 0) {
            i_this->mMode = MODE_TK2_NONE;
        } else if (!pl_check(i_this, l_HIO.mPlayerDetectionDistanceFar + 50.0f, 0x6000)) {
            i_this->mAction = ACT_TK2_FIND;
            i_this->mMode = MODE_TK2_DAMAGE;
        }
        break;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->mPlayerAngleY, 4, 0x800);
}

static void e_tk2_s_damage(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;

    switch (i_this->mMode) {
    case MODE_TK2_NONE:
        anm_init(i_this, ANM_TK2_DAMAGE, 2.0f, 0x0, 1.0f);
        i_this->mMode = MODE_TK2_APPEAR;
        break;

    case MODE_TK2_APPEAR:
        if (i_this->mpMorf->getFrame() > 10.0f + YREG_F(3)) {
            i_this->mAction = ACT_TK2_FIND;
            i_this->mMode = MODE_TK2_DAMAGE;
        }
        break;
    }
}

static void e_tk2_damage(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;

    i_this->mInvincibilityTimer = 6;
    i_this->mAttentionOFF = true;

    switch (i_this->mMode) {
    case MODE_TK2_NONE:
        anm_init(i_this, ANM_TK2_DAMAGE, 2.0f, 0x0, 1.0f);
        i_this->mMode = MODE_TK2_APPEAR;
        break;

    case MODE_TK2_APPEAR:
        if (i_this->mpMorf->isStop()) {
            fopAcM_createDisappear(actor, &actor->eyePos, 10, 0, 0x12);
            fopAcM_delete(actor);
        }
        break;
    }
}

static void action(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;

    cXyz _unk1;
    cXyz _unk2;

    i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(actor);
    i_this->mPlayerDistanceLimit = fopAcM_searchPlayerDistance(actor);
    damage_check(i_this);

    s8 link_search_flag = false;
    switch (i_this->mAction) {
    case ACT_TK2_WAIT:
        e_tk2_wait_0(i_this);
        break;
    case ACT_TK2_FIND:
        e_tk2_find(i_this);
        link_search_flag = true;
        break;
    case ACT_TK2_ATTACK:
        e_tk2_attack(i_this);
        link_search_flag = true;
        break;
    case ACT_TK2_S_DAMAGE:
        e_tk2_s_damage(i_this);
        break;
    case ACT_TK2_DAMAGE:
        e_tk2_damage(i_this);
        break;
    }

    if (link_search_flag) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }
}

static int daE_TK2_Execute(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;

    cXyz cStack_94;
    cXyz cStack_a0;

    if (i_this->mExecuteState == 0x00) {
        dBgS_ObjGndChk_Spl ground_check;
        cStack_94 = actor->current.pos;
        cStack_94.y += 200.0f;
        ground_check.SetPos(&cStack_94);
        f32 ground_cross = dComIfG_Bgsp().GroundCross(&ground_check);
        actor->current.pos.y = ground_cross - 10.0f + JREG_F(8);
    }
    i_this->mLifetime++;
    i_this->mAttentionOFF = false;

    for (int i = 0; i < 4; i++) {
        if (i_this->mActionTimer[i] != 0) {
            i_this->mActionTimer[i]--;
        }
    }

    if (i_this->mInvincibilityTimer != 0) {
        i_this->mInvincibilityTimer--;
    }

    action(i_this);
    i_this->mAcch.CrrPos(dComIfG_Bgsp());
    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::scaleM(l_HIO.mBaseSize, l_HIO.mBaseSize, l_HIO.mBaseSize);

    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    if (i_this->mAnim == ANM_TK2_SWIM) {
        if (i_this->mpMorf->checkFrame(0.0f) || i_this->mpMorf->checkFrame(6.0f) ||
            i_this->mpMorf->checkFrame(12.0f) || i_this->mpMorf->checkFrame(18.0f) ||
            i_this->mpMorf->checkFrame(24.0f))
        {
            i_this->mSound.startCreatureSound(Z2SE_EN_TK2_SWIM, 0, -1);
        }
    } else if (i_this->mAnim == ANM_TK2_APPEAR) {
        if (i_this->mpMorf->checkFrame(5.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_TK2_APPEAR, 0, -1);
        } else if (i_this->mpMorf->checkFrame(20.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_TK2_APPEAR2, 0, -1);
        }
    } else if (i_this->mAnim == ANM_TK2_HIDE) {
        if (i_this->mpMorf->checkFrame(6.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_TK2_HIDE, 0, -1);
        }
    } else if (i_this->mAnim == ANM_TK2_ATTACK) {
        if (i_this->mpMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_TK2_V_ATTACK, 0, -1);
        } else if (i_this->mpMorf->checkFrame(27.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_TK2_ATTACK, 0, -1);
        }
    } else if (i_this->mAnim == ANM_TK2_DAMAGE) {
        if (i_this->mpMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_TK2_V_DAMAGE, -1);
        }
    } else if (i_this->mAnim == ANM_TK2_WAIT01) {
        if (i_this->mpMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_TK2_V_WAIT, -1);
        }
    } else if (i_this->mAnim == ANM_TK2_KYORO2 &&
               (i_this->mpMorf->checkFrame(1.0f) || i_this->mpMorf->checkFrame(30.0f)))
    {
        i_this->mSound.startCreatureVoice(Z2SE_EN_TK2_KYORO, -1);
    }
    i_this->mpMorf->modelCalc();

    MTXCopy(model->getAnmMtx(3), *calc_mtx);

    cStack_94.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&cStack_94, &actor->eyePos);

    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y = actor->attention_info.position.y + l_HIO.mBaseSize * 35.0f;

    if (i_this->mTKBallSpawned) {
        e_tk_ball_class* ball_actor =
            static_cast<e_tk_ball_class*>(fopAcM_SearchByID(i_this->mBallID));
        if (ball_actor != NULL) {
            ball_actor->current.pos = actor->eyePos;
            ball_actor->mSuspended = false;
        }
        cXyz scale(2.0f + TREG_F(8), 2.0f + TREG_F(8), 2.0f + TREG_F(8));
        dComIfGp_particle_set(0x8198, &actor->eyePos, &actor->shape_angle, &scale);
        dComIfGp_particle_set(0x8199, &actor->eyePos, &actor->shape_angle, &scale);
        i_this->mTKBallSpawned = false;
    }

    if (i_this->mAttentionOFF) {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    } else {
        fopAcM_OnStatus(actor, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    }

    MTXCopy(model->getAnmMtx(2), *calc_mtx);
    cStack_94.set(0.0f, 0.0f, 0.0f);

    MtxPosition(&cStack_94, &cStack_a0);
    i_this->mSph.SetC(cStack_a0);
    i_this->mSph.SetR((50.0f + JREG_F(3)) * l_HIO.mBaseSize);

    dComIfG_Ccsp()->Set(&i_this->mSph);

    return 1;
}

static int daE_TK2_IsDelete(e_tk2_class* i_this) {
    return 1;
}

static int daE_TK2_Delete(e_tk2_class* i_this) {
    fopAc_ac_c* actor = i_this;

    fopAcM_RegisterDeleteID(i_this, "E_TK2");

    dComIfG_resDelete(&i_this->mPhaseReq, "E_tk2");
    if (i_this->mInitHIO) {
        hio_set = false;
        mDoHIO_DELETE_CHILD(l_HIO.mNo);
    }
    if (actor->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    e_tk2_class* i_this = static_cast<e_tk2_class*>(actor);

    i_this->mpMorf =
        new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_tk2", 0xE), NULL, NULL,
                             (J3DAnmTransform*)dComIfG_getObjectRes("E_tk2", 0x9), 2, 1.0f, 0, -1,
                             &i_this->mSound, 0x80000, 0x11000084);

    if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int daE_TK2_Create(fopAc_ac_c* actor) {
    e_tk2_class* i_this = (e_tk2_class*)actor;
    fopAcM_ct(i_this, e_tk2_class);

    cPhs_Step phase = dComIfG_resLoad(&i_this->mPhaseReq, "E_tk2");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_tk2 PARAM %x\n", fopAcM_GetParam(actor));
        i_this->mArg0 = (fopAcM_GetParam(actor) & 0xFF) >> 0;
        i_this->mArg1 = (fopAcM_GetParam(actor) & 0xF00) >> 8;
        i_this->mArg2 = (fopAcM_GetParam(actor) & 0xF000) >> 12;

        OS_REPORT("E_tk2//////////////E_TK2 SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0x23a0)) {
            OS_REPORT("//////////////E_TK2 SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_TK2 SET 2 !!\n");

        if (hio_set == false) {
            i_this->mInitHIO = true;
            hio_set = true;
            l_HIO.mNo = mDoHIO_CREATE_CHILD("たどぽーる2", &l_HIO);  // "Tadpole 2"
        }

        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(actor, i_this->mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(actor, -100.0f, -100.0f, -100.0f);
        fopAcM_SetMax(actor, 100.0f, 100.0f, 100.0f);

        actor->attention_info.distances[fopAc_attn_BATTLE_e] = 0x04;
        actor->field_0x560 = actor->health = 0;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0xd}, {0xd8fbfdff, 0x3}, 0x75}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                 // mGObjTg
                {0x0},                                              // mGObjCo
            },                                                      // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
            }  // mSphAttr
        };

        i_this->mStts.Init(100, 0, actor);
        i_this->mSph.Set(cc_sph_src);
        i_this->mSph.SetStts(&i_this->mStts);

        i_this->mAcch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1,
                          &i_this->mAcchCir, fopAcM_GetSpeed_p(actor), NULL, NULL);

        i_this->mAcchCir.SetWall(-50.0f, 60.0f);
        i_this->mSound.init(&actor->current.pos, &actor->eyePos, 0x3, 0x1);
        i_this->mSound.setEnemyName("E_tk2");
        i_this->mAtInfo.mpSound = &i_this->mSound;
        i_this->mExecuteState = 0x14;
        daE_TK2_Execute(i_this);
    }
    return phase;
}

static actor_method_class l_daE_TK2_Method = {
    (process_method_func)daE_TK2_Create,  (process_method_func)daE_TK2_Delete,
    (process_method_func)daE_TK2_Execute, (process_method_func)daE_TK2_IsDelete,
    (process_method_func)daE_TK2_Draw,
};

actor_process_profile_definition g_profile_E_TK2 = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_TK2,              // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(e_tk2_class),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    150,                     // mPriority
    &l_daE_TK2_Method,       // sub_method
    0x00040100,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
