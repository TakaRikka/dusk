/**
 * @file d_a_e_bg.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_bg.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_e_df.h"
#include "d/actor/d_a_mg_rod.h"
#include "d/actor/d_a_player.h"
#include "d/d_bomb.h"
#include "d/d_com_inf_game.h"
#include "d/d_drawlist.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"

daE_BG_HIO_c::daE_BG_HIO_c() {
    field_0x4 = -1;
    mTrackingSpeed = 8.0f;
    mRushSpeed = 16.0f;
    mPlayerSearchDistance = 1000.0f;
    mAttackRange = 1000.0f;
    mSwimRange = 500.0f;
    mWaitDistanceBeforeCharging = 500.0f;
    mJumpTime = 100.0f;
}

#if DEBUG
void daE_BG_HIO_c::genMessage(JORMContext* ctx) {
    // "Bomb fish"
    ctx->genLabel("爆弾魚", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("追跡速度", &mTrackingSpeed, 0.0, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("突進速度", &mRushSpeed, 0.0, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("プレイヤーサーチ距離", &mPlayerSearchDistance, 0.0, 2000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    ctx->genSlider("攻撃行動範囲", &mAttackRange, 0.0, 2000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("遊泳行動範囲", &mSwimRange, 0.0, 2000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("突進前待機距離", &mWaitDistanceBeforeCharging, 0.0, 1000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    ctx->genSlider("湧き出し時間", &mJumpTime, 0.0, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

namespace {
dCcD_SrcSph cc_bg_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {(s32)0xD8FBFDFF, 0x03}, 0x75}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

dCcD_SrcSph cc_bg_at_src = {
    {
        {0x0, {{0x400, 0x1, 0xD}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x02}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};
}

int daE_BG_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* joint = i_joint;
    s32 jointNo = joint->getJntNo();

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jointNo));

    if (jointNo != 0) {
        if (jointNo == 1) {
            if (field_0x6b0 == 0) {
                mDoMtx_stack_c::YrotM(field_0x6aa + nREG_S(7));
            } else {
                mDoMtx_stack_c::YrotM(-field_0x6a4[0] + field_0x6aa + nREG_S(7));
            }
        } else {
            mDoMtx_stack_c::YrotM(field_0x6a4[jointNo - 2]);
        }
    }

    i_model->setAnmMtx(jointNo, mDoMtx_stack_c::get());

    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daE_BG_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == NULL) {
        J3DModel* model = j3dSys.getModel();
        daE_BG_c* bg = (daE_BG_c*)model->getUserArea();
        if (bg != NULL) {
            bg->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

int daE_BG_c::draw() {
    if (field_0x6af != 0) {
        return 1;
    }

    J3DModel* model = mpMorfSO->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    mpBrkAnm->entry(model->getModelData());

    if ((mActionMode == 3 || mActionMode == 4) && field_0x694 != 0) {
        J3DGXColorS10 color;
        color.r = 32.0f - fabsf(cM_scos(field_0x698) * 30.0f);
        color.g = 0;
        color.b = 0;
        color.a = 0;

        J3DMaterial* mat = model->getModelData()->getMaterialNodePointer(0);
        mat->setTevColor(0, &color);
        mat = model->getModelData()->getMaterialNodePointer(1);
        mat->setTevColor(0, &color);
    }

    mpMorfSO->entryDL();

    cXyz offsetPos;
    offsetPos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);

    GXTexObj* texObj = dDlst_shadowControl_c::getSimpleTex();

    mShadowKey = dComIfGd_setShadow(mShadowKey, 0x01, model, &offsetPos, 200.0f, 0.0f, current.pos.y,
                                   mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f, texObj);

    return 1;
}

static int daE_BG_Draw(daE_BG_c* i_this) {
    return i_this->draw();
}

void daE_BG_c::setBck(int param_0, u8 param_1, f32 param_2, f32 param_3) {
    mpMorfSO->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_BG", param_0), param_1, param_2,
        param_3, 0.0f, -1.0f);
}

void daE_BG_c::setActionMode(int i_action, int i_mode) {
    if (mActionMode != i_action) {
        mActionMode = i_action;
        mMoveMode = i_mode;
    }
}

void daE_BG_c::damage_check() {
    if (field_0x68e != 0) {
        return;
    }

    mStts.Move();
    if (!mSphere.ChkTgHit()) {
        return;
    }

    if (field_0x694 != 0) {
        if (field_0x68d == 0 && field_0x68c != 0xff) {
            dComIfGs_onSwitch(field_0x68c, fopAcM_GetRoomNo(this));
        }

        dBomb_c::createWaterBombExplode(&current.pos);

        fopAcM_delete(this);
    } else {
        mAtInfo.mpCollider = mSphere.GetTgHitObj();

        // bug?
        fopAc_ac_c* actor = dCc_GetAc(mAtInfo.mpCollider->GetAc());
        cc_at_check(this, &mAtInfo);

        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
            field_0x68e = 20;
        } else {
            field_0x68e = 10;
        }

        if (mAtInfo.mAttackPower <= 1) {
            field_0x68e = 10 + KREG_S(8);
        }
        dScnPly_c::setPauseTimer(0);
        mSphere.ClrTgHit();
        mCreatureSound.startCreatureVoice(Z2SE_EN_BG_V_DAMAGE, -1);
        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
            setActionMode(6, 0);
        } else {
            setActionMode(3, 0);
        }
    }
}

void daE_BG_c::setSparkEffect() {
    static u16 enemyBombID[5] = {
        0x0A0D,
        0x0A0E,
        0x0A0F,
        0x0A10,
        0x0A11,
    };

    mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(1));

    for (s32 i = 0; i < 5; i++) {
        mParticle[i] = dComIfGp_particle_set(mParticle[i], enemyBombID[i], &current.pos, &tevStr);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticle[i]);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
        }
    }

    mCreatureSound.startCreatureSoundLevel(Z2SE_OBJ_BOMB_IGNITION, 0,
                                      dComIfGp_getReverb(dComIfGp_roomControl_getStayNo()));
}

fopAc_ac_c* daE_BG_c::search_esa() {
    dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByName(PROC_MG_ROD);
    if (rod != NULL && rod->kind == 1 && rod->action != 5 && rod->field_0x100d != 0 &&
        rod->actor.current.pos.y < rod->field_0x590 - 20.0f) {
        return &rod->actor;
    } else {
        return NULL;
    }
}

static u8 l_initHIO;

static daE_BG_HIO_c l_HIO;

void daE_BG_c::executeBorn() {
    dBgS_GndChk gndChk;
    cXyz currentWithOffset;
    switch (mMoveMode) {
    case 0: {
        field_0x6ac = 0;
        field_0x6af = 1;
        mBgId = 0xffffffff;
        mMoveMode = 1;
        field_0x68f = 0;
        attention_info.flags = 0;

        currentWithOffset = current.pos;
        currentWithOffset.y += 100.0f;

        gndChk.SetPos(&currentWithOffset);
        current.pos.y = dComIfG_Bgsp().GroundCross(&gndChk);
    }
    case 1: {
        if (field_0x68c != 0xff) {
            if (dComIfGs_isSwitch(field_0x68c, fopAcM_GetRoomNo(this))) {
                return;
            }
        }

        if (mBgId == 0xffffffff) {
            if (fopAcM_searchPlayerDistance(this) < 500.0f) {
                field_0x68f = l_HIO.mJumpTime;
                mMoveMode = 2;
            }
        } else {
            if (fopAcM_SearchByID(mBgId) == NULL) {
                mBgId = 0xffffffff;
            }
        }

        break;
    }
    case 2: {
        if (field_0x68c != 0xff) {
            if (dComIfGs_isSwitch(field_0x68c, fopAcM_GetRoomNo(this))) {
                return;
            }
        }

        if (field_0x68f == 0) {
            mBgId = fopAcM_createChild(PROC_E_BG, fopAcM_GetID(this), 0xffffff02, &current.pos,
                                           fopAcM_GetRoomNo(this), &shape_angle, NULL, -1, NULL);
            mMoveMode = 3;
        }

        break;
    }
    case 3: {
        if (field_0x68c != 0xff) {
            if (dComIfGs_isSwitch(field_0x68c, fopAcM_GetRoomNo(this))) {
                return;
            }
        }

        if (fopAcM_SearchByID(mBgId) != NULL) {
            mMoveMode = 1;
        }

        break;
    }
    }
}

void daE_BG_c::executeSwim() {
    field_0x6a2 = nREG_S(0) + 0xc00;
    field_0x69c += field_0x6a0;

    if (current.pos.abs(home.pos) < l_HIO.mAttackRange) {
        if (daPy_getPlayerActorClass()->current.pos.abs(home.pos) < l_HIO.mAttackRange &&
            fopAcM_searchPlayerDistance(this) < l_HIO.mPlayerSearchDistance)
        {
            if (!fopAcM_otherBgCheck(this, daPy_getPlayerActorClass())) {
                if (daPy_getPlayerActorClass()->checkEquipHeavyBoots()) {
                    if (field_0x684 != -G_CM3D_F_INF) {
                        if (daPy_getPlayerActorClass()->current.pos.y < field_0x684 - 20.0f) {
                            setActionMode(2, 0);
                            return;
                        }
                    } else {
                        setActionMode(2, 0);
                        return;
                    }
                } else if (dComIfGp_checkPlayerStatus0(0, fopAcM_STATUS_HOOK_CARRY_NOW)) {
                    setActionMode(2, 0);
                    return;
                }
            }
        }

        if (search_esa() != NULL) {
            setActionMode(7, 0);
            return;
        }
    }

    switch (mMoveMode) {
    case 0: {
        field_0x660.y = home.pos.y + cM_rndFX(500.0f);
        if (field_0x684 != -G_CM3D_F_INF && field_0x660.y > field_0x684 - 50.0f) {
            field_0x660.y = field_0x684 - 50.0f;
        }

        if (current.pos.absXZ(home.pos) < l_HIO.mSwimRange - 100.0f) {
            field_0x69a = shape_angle.y - 0x8000 + cM_rndFX(16000.0f);
        } else {
            field_0x69a = cLib_targetAngleY(&current.pos, &home.pos) + cM_rndFX(8192.0f);
        }

        field_0x660.x = cM_ssin(field_0x69a) * 500.0f;
        field_0x660.z = cM_scos(field_0x69a) * 500.0f;

        field_0x68f = cM_rndFX(20.0f) + 90.0f;

        mMoveMode = 1;
    }
    case 1: {
        field_0x6ac = field_0x69a - shape_angle.y;

        cLib_addCalcAngleS(&shape_angle.y, field_0x69a, 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, 0, 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, cM_scos(shape_angle.x) * 4.0f, 0.2f);
        cLib_chaseF(&speed.y, cM_ssin(shape_angle.x) * 4.0f, 0.2f);

        field_0x6a0 = 0x2000;
        if (abs((s16)(shape_angle.y - field_0x69a)) < 0x800) {
            mMoveMode = 2;
        }
        break;
    }
    case 2: {
        field_0x6ac = field_0x69a - shape_angle.y;

        cLib_addCalcAngleS(&shape_angle.y, field_0x69a, 0x10, 0x400, 0x100);
        s32 targetAngleX = cLib_targetAngleX(&current.pos, &field_0x660);
        cLib_addCalcAngleS(&shape_angle.x, targetAngleX, 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, cM_scos(shape_angle.x) * 4.0f, 0.2f);
        cLib_chaseF(&speed.y, cM_ssin(shape_angle.x) * 4.0f, 0.2f);

        cLib_addCalcAngleS(&field_0x6a0, 0xc00, 8, 0x400, 0x100);

        if (speed.y > 0.0f && field_0x684 != -G_CM3D_F_INF && current.pos.y > field_0x684 - 50.0f) {
            mMoveMode = 3;
        }

        if (current.pos.abs(home.pos) > l_HIO.mSwimRange) {
            if (abs((s16)(cLib_targetAngleY(&current.pos, &home.pos) - field_0x69a)) > 0x2000) {
                mMoveMode = 3;
            }
        }

        if (field_0x68f == 0) {
            mMoveMode = 3;
        }

        break;
    }
    case 3: {
        field_0x6ac = field_0x69a - shape_angle.y;
        cLib_addCalcAngleS(&shape_angle.y, field_0x69a, 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, 0, 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&field_0x6a0, 0x800, 8, 0x400, 0x100);

        cLib_chaseF(&speedF, cM_scos(shape_angle.x), 0.1f);
        cLib_chaseF(&speed.y, cM_ssin(shape_angle.x), 0.1f);

        if (speed.abs() <= 2.0f) {
            mMoveMode = 0;
        }

        break;
    }
    }

    current.angle.y = shape_angle.y;
}

void daE_BG_c::executeAttack() {
    s16 unusedShort1 = 0;
    u8 unkFlag1;
    s16 unkShort1;
    cXyz unkXyz1;

    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    cXyz playerPos = daPy_getPlayerActorClass()->current.pos;

    dBgS_LinChk linChk;

    field_0x6a2 = nREG_S(0) + 0xc00;
    field_0x69c += field_0x6a0;

    if (mMoveMode <= 2) {
        if (daPy_getPlayerActorClass()->checkEquipHeavyBoots()) {
            if (field_0x684 != -G_CM3D_F_INF && playerPos.y >= field_0x684 - 20.0f) {
                setActionMode(1, 0);
                return;
            }
        } else {
            if (!dComIfGp_checkPlayerStatus0(0, fopAcM_STATUS_HOOK_CARRY_NOW)) {
                setActionMode(1, 0);
                return;
            }
        }

        if (current.pos.abs(home.pos) > l_HIO.mAttackRange ||
             fopAcM_searchPlayerDistance(this) > l_HIO.mPlayerSearchDistance)
        {
            setActionMode(1, 0);
            return;
        }
    }

    switch (mMoveMode) {
    case 0:
        field_0x6ac = fopAcM_searchPlayerAngleY(this) - shape_angle.y;

        cLib_addCalcAngleS(&field_0x6a0, 0x2400, 8, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, fopAcM_searchPlayerAngleX(this), 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, l_HIO.mTrackingSpeed * cM_scos(shape_angle.x), 1.0f);
        cLib_chaseF(&speed.y, l_HIO.mTrackingSpeed * cM_ssin(shape_angle.x), 1.0f);

        if (!dComIfGp_checkPlayerStatus0(0, fopAcM_STATUS_HOOK_CARRY_NOW)) {
            if (daPy_getPlayerActorClass()->checkEquipHeavyBoots()) {
                mMoveMode = 1;
                field_0x69a = cM_rndFX(8192.0f);
            }
        }

        if (mAtSphere.ChkAtHit()) {
            fopAc_ac_c* hitActor = dCc_GetAc(mAtSphere.GetAtHitObj()->GetAc());
            if (fopAcM_GetName(hitActor) == PROC_ALINK) {
                mMoveMode = 10;
                field_0x68f = 30;
                speedF = cM_rndFX(1.0f) + -5.0f;
                shape_angle.x = 0x1800;
                field_0x6a0 = 0x4000;
            }
        }
        break;
    case 10:
        field_0x6a2 = nREG_S(0) + 0x1000;

        cLib_addCalcAngleS(&field_0x6a0, 0x1000, 8, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, 0, 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, 0.0f, 0.1f);
        cLib_chaseF(&speed.y, 0.0f, 1.0f);

        if (field_0x68f == 0) {
            mMoveMode = 0;
        }

        break;
    case 1:
        field_0x6a2 = nREG_S(0) + 0xc00;
        unkShort1 = field_0x69a + fopCamM_GetAngleY(camera);
        unkXyz1 = playerPos;
        unkXyz1.x += l_HIO.mWaitDistanceBeforeCharging * cM_ssin(unkShort1);
        unkXyz1.y += 200.0f;
        unkXyz1.z += l_HIO.mWaitDistanceBeforeCharging * cM_scos(unkShort1);
        linChk.Set(&playerPos, &unkXyz1, NULL);

        if (dComIfG_Bgsp().LineCross(&linChk)) {
            unkXyz1 = linChk.GetCross();
        }

        cLib_addCalcAngleS(&field_0x6a0, 0x1800, 8, 0x400, 0x100);

        field_0x6ac = cLib_targetAngleY(&current.pos, &unkXyz1) - shape_angle.y;

        cLib_addCalcAngleS(&shape_angle.y, (s16)cLib_targetAngleY(&current.pos, &unkXyz1), 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, (s16)cLib_targetAngleX(&current.pos, &unkXyz1), 0x10, 0x400, 0x100);

        if (current.pos.abs(playerPos) > l_HIO.mWaitDistanceBeforeCharging + 300.0f) {
            cLib_chaseF(&speedF, l_HIO.mTrackingSpeed * cM_scos(shape_angle.x), 1.0f);
            cLib_chaseF(&speed.y, l_HIO.mTrackingSpeed * cM_ssin(shape_angle.x), 1.0f);
        } else {
            cLib_chaseF(&speedF, cM_scos(shape_angle.x) * 4.0f, 1.0f);
            cLib_chaseF(&speed.y, cM_ssin(shape_angle.x) * 4.0f, 1.0f);
        }

        if (current.pos.abs(unkXyz1) < 200.0f) {
            field_0x6ae++;
            if (field_0x6ae > 30) {
                field_0x6ae = 0;
                field_0x69a = cM_rndFX(8192.0f);
            }
        } else {
            field_0x6ae = 0;
        }

        if (dComIfGp_checkPlayerStatus0(0, fopAcM_STATUS_HOOK_CARRY_NOW)) {
            mMoveMode = 0;
            break;
        }

        if (field_0x690 != 0) {
            break;
        }

        unkFlag1 = false;
        if (dComIfGp_getAttention()->LockonTruth() &&
            dComIfGp_getAttention()->LockonTarget(0) == this) {
            unkFlag1 = true;
        } else {
            if ((s16)cLib_distanceAngleS(unkShort1, fopAcM_searchPlayerAngleY(this)) > 0x6800) {
                if (current.pos.abs(unkXyz1) < 200.0f) {
                    unkFlag1 = true;
                } else if (mObjAcch.ChkWallHit()) {
                    unkFlag1 = true;
                }
            }
        }

        if (unkFlag1) {
            mMoveMode = 2;
            field_0x68f = 30;
        }
        break;
    case 2:
        unkXyz1 = playerPos;
        unkXyz1.y += 200.0f;

        cLib_addCalcAngleS(&field_0x6a0, 0x800, 8, 0x400, 0x100);
        field_0x6ac = cLib_targetAngleY(&current.pos, &unkXyz1) - shape_angle.y;

        cLib_addCalcAngleS(&shape_angle.y, (s16)cLib_targetAngleY(&current.pos, &unkXyz1), 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, 0, 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, 0.0f, 1.0f);
        cLib_chaseF(&speed.y, 0.0f, 1.0f);

        if (dComIfGp_checkPlayerStatus0(0, fopAcM_STATUS_HOOK_CARRY_NOW)) {
            mMoveMode = 0;
        } else {
            if (field_0x68f == 0) {
                mMoveMode = 3;
                field_0x68f = 10;
                field_0x6a0 = 0x2000;
            }
        }
        break;
    case 3:
        field_0x6b0 = 0x01;
        field_0x6a2 = nREG_S(0) + 0x1000;

        unkXyz1 = playerPos;
        unkXyz1.y += 100.0f;

        cLib_addCalcAngleS(&field_0x6a0, 0x2000, 8, 0x400, 0x100);

        field_0x6ac = cLib_targetAngleY(&current.pos, &unkXyz1) - shape_angle.y;

        cLib_addCalcAngleS(&shape_angle.y, (s16)cLib_targetAngleY(&current.pos, &unkXyz1), 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, (s16)cLib_targetAngleX(&current.pos, &unkXyz1), 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, 0.0f, 1.0f);
        cLib_chaseF(&speed.y, 0.0f, 1.0f);

        if (field_0x68f == 0) {
            mMoveMode = 4;
            field_0x68f = 60;
        }

        break;
    case 4:
        field_0x6b0 = 1;
        field_0x6a2 = nREG_S(0) + 0x1000;

        unkXyz1 = playerPos;
        unkXyz1.y += 100.0f;

        cLib_addCalcAngleS(&field_0x6a0, 0x1000, 8, 0x400, 0x100);

        field_0x6ac = cLib_targetAngleY(&current.pos, &unkXyz1) - shape_angle.y;

        cLib_addCalcAngleS(&shape_angle.y, (s16)cLib_targetAngleY(&current.pos, &unkXyz1), 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, (s16)cLib_targetAngleX(&current.pos, &unkXyz1), 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, l_HIO.mRushSpeed * cM_scos(shape_angle.x), 1.0f);
        cLib_chaseF(&speed.y, l_HIO.mRushSpeed * cM_ssin(shape_angle.x), 1.0f);

        if (mAtSphere.ChkAtHit()) {
            fopAc_ac_c* hitActor = dCc_GetAc(mAtSphere.GetAtHitObj()->GetAc());
            if (fopAcM_GetName(hitActor) == PROC_ALINK) {
                mMoveMode = 5;
                field_0x68f = 30;

                speedF = -5.0f;
                shape_angle.x = 0x1800;

                field_0x6a0 = 0x2000;

                return;
            }
        }
        if (field_0x68f == 0) {
            mMoveMode = 0;
            field_0x690 = 200;
        }

        break;
    case 5:
        unkXyz1 = playerPos;
        unkXyz1.y += 100.0f;

        field_0x6a2 = nREG_S(0) + 0x1000;

        cLib_addCalcAngleS(&field_0x6a0, 0x1000, 8, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.y, (s16)cLib_targetAngleY(&current.pos, &unkXyz1), 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, 0, 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, 0.0f, 0.1f);
        cLib_chaseF(&speed.y, 0.0f, 1.0f);

        if (field_0x68f == 0) {
            mMoveMode = 0;
            field_0x690 = 200;
        }

        break;
    }

    current.angle.y = shape_angle.y;
}

void daE_BG_c::executeDamage() {
    field_0x694 = 160;

    switch (mMoveMode) {
    case 0:
        field_0x6ac = field_0x6a2 = 0;

        mSphere.OffTgSetBit();
        mAtSphere.OffAtSetBit();

        attention_info.flags = 0;

        setBck(6, 0, 3.0f, 1.0f);

        mMoveMode = 1;

        current.angle.y = mAtInfo.mHitDirection.y + 0x8000;

        speedF = 10.0f;
        field_0x69a = 0x1000;
        speed.y = 0.0f;
    case 1:
        cLib_chaseF(&field_0x688, 0.0f, 5.0f);

        shape_angle.y += field_0x69a;
        shape_angle.x += field_0x69a;

        if (mObjAcch.ChkGroundHit()) {
            cLib_chaseAngleS(&field_0x69a, 0, 0x80);
        } else {
            cLib_chaseAngleS(&field_0x69a, 0x100, 0x60);
        }

        cLib_chaseF(&speedF, 0.0f, 0.2f);
        if (speedF < 5.0f) {
            gravity = -0.1f;
            maxFallSpeed = -3.0f;
        }

        if (mpMorfSO->isStop()) {
            setActionMode(4, 0);
        }

        mpBrkAnm->play();

        break;
    }
}

bool daE_BG_c::setBombCarry(int param_0) {
    fopAc_ac_c* bomb = NULL;

    if (param_0 == 0) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

        if (fopAcM_checkCarryNow(this)) {
            shape_angle.x = 0;
            bomb =
                dBomb_c::createEnemyWaterBomb(&current.pos, &shape_angle, fopAcM_GetRoomNo(this));
        }
    } else {
        shape_angle.x = 0;
        bomb = dBomb_c::createEnemyWaterBombHookshot(&current.pos, &shape_angle,
                                                     fopAcM_GetRoomNo(this));
    }

    if (bomb != NULL) {
        fopAc_ac_c* unkActor1;
        fopAcM_SearchByID(fopAcM_GetLinkId(this), &unkActor1);
        if (unkActor1 != NULL) {
            ((daE_BG_c*)unkActor1)->setBgId(fopAcM_GetID(bomb));
        }

        fopAcM_delete(this);

        return true;
    }

    return false;
}

void daE_BG_c::executeBomb() {
    switch (mMoveMode) {
    case 0:
        mSphere.OnTgSetBit();

        field_0x6ac = field_0x6a2 = 0;

        setBck(5, 2, 3.0f, 1.0f);

        mMoveMode = 1;
    case 1:
        field_0x694 = 0xa0;

        shape_angle.y += field_0x69a;
        shape_angle.x += field_0x69a;

        if (mObjAcch.ChkGroundHit()) {
            mMoveMode = 2;
            speed.y = 1.0f;
        } else {
            cLib_chaseAngleS(&field_0x69a, 0x100, 0x80);
        }

        if (speedF < 5.0f) {
            gravity = -0.1f;
            maxFallSpeed = -3.0f;
        }

        cLib_chaseF(&speedF, 0.0f, 0.2f);

        break;
    case 2:
        shape_angle.y += field_0x69a;

        if (mObjAcch.ChkGroundHit()) {
            if (field_0x68f != 0) {
                cLib_chaseAngleS(&shape_angle.x, -0x8000, 0x80);
            }

            cLib_chaseAngleS(&field_0x69a, 0, 0x80);

            if (setBombCarry(0)) {
                return;
            }
        } else {
            field_0x68f = 10;
            cLib_chaseAngleS(&field_0x69a, 0x100, 0x80);
            cLib_chaseAngleS(&shape_angle.x, -0x8000, 0x80);
        }

        if (speedF < 5.0f) {
            gravity = -0.1f;
            maxFallSpeed = -3.0f;
        }

        cLib_chaseF(&speedF, 0.0f, 0.2f);

        break;
    }
}

void daE_BG_c::executeBirth() {
    field_0x6ac = 0;
    field_0x6a2 = 0xc00;
    field_0x69c += field_0x6a0;

    switch (mMoveMode) {
    case 0:
        mAtSphere.OffAtSetBit();

        mMoveMode = 1;

        field_0x688 = -100.0f;
        field_0x68f = 30;
        gravity = -0.1f;
        maxFallSpeed = -3.0f;
        field_0x6a0 = 0x3000;
        shape_angle.x = 0x2000;

        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

        mCreatureSound.startCreatureSound(Z2SE_EN_BG_SANDSMOKE, 0, -1);

        break;
    case 1:
        field_0x6b0 = 1;

        if (setBombCarry(0)) {
            return;
        }

        cLib_chaseF(&field_0x688, -40.0f, 5.0f);

        if (field_0x68f == 0) {
            mMoveMode = 2;
            field_0x68f = 0x1e;
        }

        break;
    case 2:
        field_0x6b0 = 1;

        if (setBombCarry(0)) {
            return;
        }

        cLib_chaseF(&field_0x688, 0.0f, 1.0f);
        if (field_0x68f == 0) {
            mMoveMode = 3;
            field_0x688 = 0.0f;
            speed.y = 10.0f;

            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

            mAtSphere.OnAtSetBit();
        }

        break;
    case 3:
        cLib_chaseF(&speed.y, 0.0f, 0.2f);
        cLib_addCalcAngleS(&field_0x6a0, 0x1000, 0x10, 0x200, 0x100);

        shape_angle.y += field_0x69a;
        shape_angle.x += field_0x69a;

        if (cLib_chaseAngleS(&field_0x69a, 0, 0x100) != 0) {
            cLib_chaseAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x100);
            cLib_chaseAngleS(&shape_angle.x, 0, 0x180);

            if (speed.y <= 0.0f) {
                field_0x68f = 0x0f;
                mMoveMode = 4;
                gravity = 0.0f;
                maxFallSpeed = -40.0f;
                speed.y = 0.0f;
                home.pos = current.pos;
            }
        }

        break;
    case 4:
        cLib_addCalcAngleS(&field_0x6a0, 0x800, 8, 0x400, 0x100);

        cLib_chaseAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x100);
        cLib_chaseAngleS(&shape_angle.x, 0, 0x100);

        if (field_0x68f == 0) {
            setActionMode(2, 0);
        }
    }

    if (mMoveMode >= 1 && mMoveMode <= 3) {
        mParticle2 = dComIfGp_particle_set(mParticle2, 0x84c4, &home.pos, &shape_angle, 0);
        mParticle3 = dComIfGp_particle_set(mParticle3, 0x84c5, &home.pos, &shape_angle, 0);
    }
}

void daE_BG_c::executeHook() {
    field_0x6ac = 0;
    field_0x6a2 = 0xc00;
    field_0x69c += field_0x6a0;

    setBombCarry(1);
}

void daE_BG_c::executeEat() {
    fopAc_ac_c* rod = search_esa();
    if (rod == 0) {
        setActionMode(1, 0);
        return;
    }

    cXyz rodPos = rod->current.pos;
    s32 targetAngleY = cLib_targetAngleY(&current.pos, &rodPos);
    field_0x6a2 = nREG_S(0) + 0xc00;
    field_0x69c += field_0x6a0;

    switch (this->mMoveMode) {
    case 0:
        field_0x660.y = rodPos.y + cM_rndFX(100.0f);
        if (field_0x684 != -G_CM3D_F_INF && field_0x660.y > field_0x684 - 50.0f) {
            field_0x660.y = field_0x684 - 50.0f;
        }

        if (cM_rnd() < 0.5f) {
            field_0x69a = targetAngleY + 0x2000;
        } else {
            field_0x69a = targetAngleY + -0x2000;
        }

        field_0x660.x = (cM_rndF(100.0f) + 200.0f) * cM_ssin(field_0x69a);
        field_0x660.z = (cM_rndF(100.0f) + 200.0f) * cM_scos(field_0x69a);

        field_0x68f = cM_rndFX(10.0f) + 30.0f;
        mMoveMode = 1;
    case 1:
        field_0x6ac = field_0x69a - shape_angle.y;
        cLib_addCalcAngleS(&shape_angle.y, field_0x69a, 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, 0, 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, cM_scos(shape_angle.x) * 4.0f, 0.2f);
        cLib_chaseF(&speed.y, cM_ssin(shape_angle.x) * 4.0f, 0.2f);

        field_0x6a0 = 0x2000;

        if (abs((s16)(shape_angle.y - field_0x69a)) < 0x800) {
            mMoveMode = 2;
        }
    case 2:
        field_0x6ac = field_0x69a - shape_angle.y;
        cLib_addCalcAngleS(&shape_angle.y, field_0x69a, 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, (s16)cLib_targetAngleX(&current.pos, &field_0x660), 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, cM_scos(shape_angle.x) * 4.0f, 0.2f);
        cLib_chaseF(&speed.y, cM_ssin(shape_angle.x) * 4.0f, 0.2f);

        cLib_addCalcAngleS(&field_0x6a0, 0xc00, 8, 0x400, 0x100);

        if (field_0x660.abs(current.pos) < 50.0f) {
            mMoveMode = 3;
        }

        if (field_0x68f == 0) {
            mMoveMode = 3;
        }

        break;
    case 3:
        field_0x6ac = field_0x69a - shape_angle.y;
        cLib_addCalcAngleS(&shape_angle.y, field_0x69a, 0x10, 0x400,
                           0x100);
        cLib_addCalcAngleS(&shape_angle.x, 0, 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&field_0x6a0, 0x800, 8, 0x400, 0x100);
        cLib_chaseF(&speedF, cM_scos(shape_angle.x), 0.1f);
        cLib_chaseF(&speed.y, cM_ssin(shape_angle.x), 0.1f);
        if (speed.abs() <= 2.0f) {
            if (rodPos.abs(current.pos) < 100.0f) {
                mMoveMode = 5;
            } else {
                mMoveMode = 0;
            }
        }
        break;
    case 5:
        field_0x6ac = targetAngleY - shape_angle.y;

        cLib_addCalcAngleS(&field_0x6a0, 0x800, 8, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.y, targetAngleY, 0x10, 0x400, 0x100);

        cLib_addCalcAngleS(&shape_angle.x, (s16)cLib_targetAngleX(&current.pos, &rodPos), 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, cM_scos(shape_angle.x), 0.1f);
        cLib_chaseF(&speed.y, cM_ssin(shape_angle.x), 0.1f);

        if (abs((s16)(shape_angle.y - targetAngleY)) < 0x800) {
            mMoveMode = 6;
        }
    case 6:
        field_0x6ac = targetAngleY - shape_angle.y;
        cLib_addCalcAngleS(&field_0x6a0, 0x2400, 8, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.y, targetAngleY, 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, (s16)cLib_targetAngleX(&current.pos, &rodPos), 0x10, 0x400, 0x100);

        cLib_chaseF(&speedF, cM_scos(shape_angle.x) * 2.0f, 0.5f);
        cLib_chaseF(&speed.y, cM_ssin(shape_angle.x) * 2.0f, 0.5f);

        if (rodPos.abs(current.pos) < 30.0f) {
            mMoveMode = 7;
            field_0x68f = cM_rndF(30.0f) + 60.0f;
        }

        break;
    case 7:
        field_0x6ac = targetAngleY - shape_angle.y;

        cLib_addCalcAngleS(&field_0x6a0, 0x2000, 8, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.y, targetAngleY, 0x10, 0x400, 0x100);
        cLib_addCalcAngleS(&shape_angle.x, 0, 0x10, 0x400, 0x100);

        if (rodPos.abs(current.pos) > 70.0f) {
            cM_scos(shape_angle.x);
            cLib_chaseF(&speedF, 0.0f, 0.1f);
            cM_ssin(shape_angle.x);
            cLib_chaseF(&speed.y, 0.0f, 0.1f);
        } else {
            cLib_chaseF(&speedF, cM_scos(shape_angle.x) * -1.0f, 0.3f);
            cLib_chaseF(&speed.y, cM_ssin(shape_angle.x) * -1.0f, 0.3f);
        }

        if (rodPos.abs(current.pos) > 70.0f && !speed.abs()) {
            mMoveMode = 5;

            field_0x68f = cM_rndF(30.0f) + 30.0f;

            if (cM_rnd() < 0.3f) {
                mMoveMode = 0;
            }
        }

        return;
    }

    current.angle.y = shape_angle.y;
}

void daE_BG_c::action() {
    damage_check();
    field_0x6b0 = 0;
    BOOL isAttacking = FALSE;

    switch (this->mActionMode) {
    case 0:
        executeBorn();
        break;
    case 1:
        executeSwim();
        break;
    case 2:
        executeAttack();
        isAttacking = TRUE;
        break;
    case 3:
        executeDamage();
        break;
    case 4:
        executeBomb();
        break;
    case 5:
        executeBirth();
        break;
    case 6:
        executeHook();
        break;
    case 7:
        executeEat();
        break;
    }

    mCreatureSound.setLinkSearch(isAttacking);
    if (mActionMode != 0 && mActionMode != 3) {
        dBgS_LinChk linChk;
        linChk.Set(&dComIfGp_getCamera(0)->lookat.eye, &attention_info.position, this);
        if (dComIfG_Bgsp().LineCross(&linChk)) {
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
        } else {
            attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
        }
    }

    if (field_0x6ac > 0x2000) {
        field_0x6ac = 0x2000;
    }
    if (field_0x6ac < -0x2000) {
        field_0x6ac = -0x2000;
    }

    cLib_addCalcAngleS(&field_0x6aa, field_0x6ac, 0x10, 0x100, 0x10);

    f32 unkFloat1 = field_0x6a0 / 0x1000;
    if (unkFloat1 > 1.0f) {
        unkFloat1 = 1.0f;
    }
    if (field_0x69c < 0 && field_0x69e >= 0) {
        mCreatureSound.startCreatureSound(Z2SE_EN_BG_SWIM, 0, -1);
    }

    if (field_0x6b0 != 0 && field_0x69c > 0 && field_0x69e <= 0) {
        mCreatureSound.startCreatureSound(Z2SE_EN_BG_SWIM, 0, -1);
    }

    field_0x69e = field_0x69c;
    field_0x6a4[0] = (f32)field_0x6a2 * unkFloat1 * cM_ssin(field_0x69c);
    field_0x6a4[1] = (f32)field_0x6a2 * unkFloat1 * cM_ssin(field_0x69c + 0x4000) * 1.5f;
    field_0x6a4[2] = (f32)field_0x6a2 * unkFloat1 * cM_ssin(field_0x69c + 0x8000) * 3.0f;

    fopAcM_posMoveF(this, mStts.GetCCMoveP());

    if (field_0x684 != -G_CM3D_F_INF && current.pos.y > field_0x684 - 50.0f) {
        current.pos.y = field_0x684 - 50.0f;
        if (speed.y > 0.0f) {
            speed.y = 0.0f;
        }
    }

    mObjAcch.CrrPos(dComIfG_Bgsp());

    mpMorfSO->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daE_BG_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0x688, current.pos.z);
    mDoMtx_stack_c::transM(0.0f, 25.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotM(-shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, -25.0f, 0.0f);

    J3DModel* model = mpMorfSO->getModel();

    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((uintptr_t)this);

    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
    }

    mpMorfSO->modelCalc();

    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(NULL);
    }
}

void daE_BG_c::cc_set() {
    cXyz unkXyz1;
    cXyz center;

    J3DModel* model = mpMorfSO->getModel();

    if (field_0x6af == 0) {
        mDoMtx_stack_c::copy(model->getAnmMtx(0));

        unkXyz1.set(0.0f, 25.0f, 0.0f);
        mDoMtx_stack_c::multVec(&unkXyz1,&eyePos);

        attention_info.position = eyePos;
        attention_info.position.y += 25.0f;

        mDoMtx_stack_c::copy(model->getAnmMtx(0));

        unkXyz1.set(0.0f, 25.0f, 0.0f);
        mDoMtx_stack_c::multVec(&unkXyz1, &center);

        mSphere.SetC(center);
        mSphere.SetR(35.0f);

        dComIfG_Ccsp()->Set(&mSphere);

        mDoMtx_stack_c::copy(model->getAnmMtx(0));

        unkXyz1.set(0.0f, 25.0f, 20.0f);
        mDoMtx_stack_c::multVec(&unkXyz1, &center);

        mAtSphere.SetC(center);
        mAtSphere.SetR(20.0f);

        dComIfG_Ccsp()->Set(&mAtSphere);
    }
}

int daE_BG_c::execute() {
    if (field_0x68f != 0) {
        field_0x68f--;
    }

    if (field_0x690 != 0) {
        field_0x690--;
    }

    if (field_0x68e != 0) {
        field_0x68e--;
    }

    if (field_0x694 != 0) {
        setSparkEffect();

        field_0x694--;

        mIsBomb = true;

        if (field_0x694 < 30) {
            field_0x698 += (s16)0x1000;
        } else if (field_0x694 < 45) {
            field_0x698 += (s16)0x800;
        } else {
            field_0x698 += (s16)0x300;
        }

        if (field_0x694 == 0) {
            if (field_0x68d == 0 && field_0x68c != 0xff) {
                dComIfGs_onSwitch(field_0x68c, fopAcM_GetRoomNo(this));
            }

            dBomb_c::createWaterBombExplode(&current.pos);

            fopAcM_delete(this);

            return 1;
        }
    } else {
        mIsBomb = false;
    }

    action();
    mtx_set();
    cc_set();

    return 1;
}

static int daE_BG_Execute(daE_BG_c* i_this) {
    return i_this->execute();
}

static int daE_BG_IsDelete(daE_BG_c* i_this) {
    return TRUE;
}

int daE_BG_c::_delete() {
    dComIfG_resDelete(&mPhaseReq, "E_BG");

    if (mHIOInit) {
        l_initHIO = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.field_0x4);
    }

    if (heap != NULL) {
        mCreatureSound.deleteObject();
    }

    return 1;
}

static int daE_BG_Delete(daE_BG_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

int daE_BG_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_BG", 10);
    JUT_ASSERT(0, modelData != NULL);

    mpMorfSO = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                    (J3DAnmTransform*)dComIfG_getObjectRes("E_BG", 7), 0, 1.0f, 0,
                                    -1, &mCreatureSound, 0x80000, 0x11000084);
    if (mpMorfSO == NULL || mpMorfSO->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorfSO->getModel();

    model->setUserArea((uintptr_t)this);

    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
    }

    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }

    if (!mpBrkAnm->init(model->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_bg", 13),
                        1, 0, 1.0f, 0, -1))
    {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daE_BG_c* bg = (daE_BG_c*)i_this;
    return bg->CreateHeap();
}

int daE_BG_c::create() {
    fopAcM_ct(this, daE_BG_c);

    field_0x68d = fopAcM_GetParam(this);
    if (field_0x68d == 0xff) {
        field_0x68d = 0;
    }

    field_0x68c = (fopAcM_GetParam(this) >> 16) & 0xff;
    if (field_0x68c != 0xff) {
        if (dComIfGs_isSwitch(field_0x68c, fopAcM_GetRoomNo(this))) {
            // "I won't reset it because I've already been defeated"
            OS_REPORT("E_BG やられ後なので再セットしません\n");
            return cPhs_ERROR_e;
        }
    }

    s32 loadResult = dComIfG_resLoad(&this->mPhaseReq, "E_BG");
    if (loadResult == cPhs_COMPLEATE_e) {
        OS_REPORT("E_BG PARAM %x\n", fopAcM_GetParam(this));
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1100)) {
            return cPhs_ERROR_e;
        }

        if (l_initHIO == FALSE) {
            l_initHIO = 1;
            mHIOInit = 1;
            l_HIO.field_0x4 = mDoHIO_CREATE_CHILD("爆弾魚", &l_HIO);
        }

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(this, mpMorfSO->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);

        mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcchCir.SetWall(10.0f, 20.0f);

        field_0x560 = health = 10;

        mStts.Init(100, 0, this);

        mSphere.Set(*(dCcD_SrcSph*)&cc_bg_src);
        mSphere.SetStts(&mStts);

        mAtSphere.Set(*(dCcD_SrcSph*)&cc_bg_at_src);
        mAtSphere.SetStts(&mStts);

        mCreatureSound.init(&current.pos, &eyePos, 3, 1);
        mCreatureSound.setEnemyName("E_bg");

        mAtInfo.mpSound = &mCreatureSound;
        mAtInfo.mPowerType = 1;

        gravity = 0.0f;

        attention_info.distances[fopAc_attn_BATTLE_e] = 3;

        Vec gndChkPos = current.pos;
        gndChkPos.y += 3000.0f;
        dBgS_ObjGndChk_Spl gndChkSpl;
        gndChkSpl.SetPos(&gndChkPos);

        field_0x684 = dComIfG_Bgsp().GroundCross(&gndChkSpl);
        if (field_0x684 != -G_CM3D_F_INF && current.pos.y > field_0x684 - 50.0f) {
            current.pos.y = field_0x684 - 50.0f;
        }

        if (field_0x68d == 1) {
            setActionMode(0, 0);
        } else if (field_0x68d == 0x02) {
            setActionMode(5, 0);
        } else {
            setActionMode(1, 0);
        }

        onWolfNoLock();

        daE_BG_Execute(this);
    }

    return loadResult;
}

static int daE_BG_Create(daE_BG_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_BG_Method = {
    (process_method_func)daE_BG_Create,
    (process_method_func)daE_BG_Delete,
    (process_method_func)daE_BG_Execute,
    (process_method_func)daE_BG_IsDelete,
    (process_method_func)daE_BG_Draw,
};

extern actor_process_profile_definition g_profile_E_BG = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_BG,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daE_BG_c),       // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    204,                    // mPriority
    &l_daE_BG_Method,       // sub_method
    0x000D0100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
  };
