/**
 * @file d_a_e_ymb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ymb.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_obj_drop.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_camera_mng.h"
#include <cmath.h>
#include "Z2AudioLib/Z2Instances.h"

struct daE_YMB_HIO_c {
public:
    daE_YMB_HIO_c();
    virtual ~daE_YMB_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 model_size;                  // モデルサイズ (Model Size)
    /* 0x0C */ f32 fly_movement_speed;          // 飛行移動速度 (Fly Movement Speed)
    /* 0x10 */ f32 swim_attack_speed;           // 泳ぎ攻撃速度 (Swim Attack Speed)
    /* 0x14 */ f32 fly_attack_speed;            // 飛行攻撃速度 (Fly Attack Speed)
    /* 0x18 */ f32 rollover_time;               // ひっくり返り時間 (Rollover Time)
    /* 0x1C */ f32 fly_height_adjust;           // 飛行高度-加減値 (Fly Height Adjust)
};

enum E_YB_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_YB_ATTACK_A = 0x6,
    /* 0x07 */ BCK_YB_ATTACK_B,
    /* 0x08 */ BCK_YB_ATTACK_C,
    /* 0x09 */ BCK_YB_CRY,
    /* 0x0A */ BCK_YB_DAMAGE,
    /* 0x0B */ BCK_YB_DAMAGE_DAWN,
    /* 0x0C */ BCK_YB_DAWN_DAMAGE,
    /* 0x0D */ BCK_YB_DAWN_IN,
    /* 0x0E */ BCK_YB_DAWN_WAIT,
    /* 0x0F */ BCK_YB_DEATH,
    /* 0x10 */ BCK_YB_DROP,
    /* 0x11 */ BCK_YB_FLY_WAIT,
    /* 0x12 */ BCK_YB_GUARD,
    /* 0x13 */ BCK_YB_RISE,
    /* 0x14 */ BCK_YB_SIM_IN,
    /* 0x15 */ BCK_YB_SIM_OUT,
    /* 0x16 */ BCK_YB_SIM_TO_FLY,

    /* BMDR */
    /* 0x19 */ BMDR_YB_TW = 0x19,

    /* BRK */
    /* 0x1C */ BRK_YB_WAIT = 0x1C,

    /* DZB */
    /* 0x1F */ DZB_YB = 0x1F,
};

enum Action {
    /* 0x0 */ ACTION_WAIT,
    /* 0x1 */ ACTION_FLY,
    /* 0x2 */ ACTION_FLY_ATTACK,
    /* 0x3 */ ACTION_RUN_AWAY,
    /* 0x4 */ ACTION_GUARD,
    /* 0x5 */ ACTION_SWIM,
    /* 0x6 */ ACTION_WATER_JUMP,
    /* 0x7 */ ACTION_DAMAGE,
    /* 0x8 */ ACTION_DOWN,
    /* 0x9 */ ACTION_DEATH,
    /* 0xA */ ACTION_LAKE_DEMO,
    /* 0xB */ ACTION_START_DEMO,
    /* 0xC */ ACTION_BATTLE_DEMO,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BODY1,
    /* 0x02 */ JNT_ARM_LB1,
    /* 0x03 */ JNT_ARM_LB2,
    /* 0x04 */ JNT_ARM_LB3,
    /* 0x05 */ JNT_ARM_LC1,
    /* 0x06 */ JNT_ARM_LC2,
    /* 0x07 */ JNT_ARM_LC3,
    /* 0x08 */ JNT_ARM_RB1,
    /* 0x09 */ JNT_ARM_RB2,
    /* 0x0A */ JNT_ARM_RB3,
    /* 0x0B */ JNT_ARM_RC1,
    /* 0x0C */ JNT_ARM_RC2,
    /* 0x0D */ JNT_ARM_RC3,
    /* 0x0E */ JNT_BODY2,
    /* 0x0F */ JNT_BODY3,
    /* 0x10 */ JNT_BODY4,
    /* 0x11 */ JNT_TAIL1,
    /* 0x12 */ JNT_TAIL2,
    /* 0x13 */ JNT_TUBE_LC1,
    /* 0x14 */ JNT_TUBE_LC2,
    /* 0x15 */ JNT_TUBE_RC1,
    /* 0x16 */ JNT_TUBE_RC2,
    /* 0x17 */ JNT_TUBE_LA1,
    /* 0x18 */ JNT_TUBE_LA2,
    /* 0x19 */ JNT_TUBE_LB1,
    /* 0x1A */ JNT_TUBE_LB2,
    /* 0x1B */ JNT_TUBE_RA1,
    /* 0x1C */ JNT_TUBE_RA2,
    /* 0x1D */ JNT_TUBE_RB1,
    /* 0x1E */ JNT_TUBE_RB2,
    /* 0x1F */ JNT_WING_LA,
    /* 0x20 */ JNT_WING_LB1,
    /* 0x21 */ JNT_WING_LB2,
    /* 0x22 */ JNT_WING_LC1,
    /* 0x23 */ JNT_WING_LC2,
    /* 0x24 */ JNT_WING_RA,
    /* 0x25 */ JNT_WING_RB1,
    /* 0x26 */ JNT_WING_RB2,
    /* 0x27 */ JNT_WING_RC1,
    /* 0x28 */ JNT_WING_RC2,
    /* 0x29 */ JNT_BUST,
    /* 0x2A */ JNT_ARM_LA1,
    /* 0x2B */ JNT_ARM_LA2,
    /* 0x2C */ JNT_ARM_LA3,
    /* 0x2D */ JNT_ARM_LA4,
    /* 0x2E */ JNT_ARM_RA1,
    /* 0x2F */ JNT_ARM_RA2,
    /* 0x30 */ JNT_ARM_RA3,
    /* 0x31 */ JNT_ARM_RA4,
    /* 0x32 */ JNT_HEAD,
    /* 0x33 */ JNT_JAW_L,
    /* 0x34 */ JNT_JAW_R,
};

enum Particle {
    /* 0x029D */ ZL2_MIDONAHAIR03_1 = 0x29D,
    /* 0x029E */ ZL2_MIDONAHAIR03_2,
    /* 0x029F */ ZL2_MIDONAHAIR01,
    /* 0x8650 */ ZL2_SAND00_IA_1 = 0x8650,
    /* 0x8651 */ ZL2_MIZUTAMA02,
    /* 0x8652 */ ZL2_HAMON_1,
    /* 0x8653 */ ZL2_FIRE_1,
    /* 0x8654 */ ZL2_SAND00_IA_2,
    /* 0x8655 */ ZL2_HAMON_2,
    /* 0x8656 */ ZL2_BOIL,
    /* 0x865D */ ZL2_POLYGON_IA8 = 0x865D,
    /* 0x865E */ ZL2_YAMIMOYOU01,
    /* 0x8630 */ ZL2_ELEC_1 = 0x8630,
    /* 0x8631 */ ZL2_ELECY_1,
    /* 0x8632 */ ZL2_ELECY_2,
    /* 0x8633 */ ZL2_GLOW02,
    /* 0x8634 */ ZL2_HAMON_3,
    /* 0x8635 */ ZL2_HAMON_4,
    /* 0x865F */ ZL2_HANEBLUR64I4_1 = 0x865F,
    /* 0x8660 */ ZL2_HANEBLUR64I4_2,
    /* 0x8636 */ ZL2_FIRE_2 = 0x8636,
    /* 0x8637 */ ZL2_HAMON_5,
    /* 0x8638 */ ZL2_WATER_EFFECT_1,
    /* 0x8639 */ ZL2_WATER_EFFECT_2,
    /* 0x863A */ ZL2_SMASH_KARI_1,
    /* 0x863B */ ZL2_MIZUTAMA_IA,
    /* 0x863C */ ZL2_FIRE_3,
    /* 0x863D */ ZL2_SMASH_KARI_2,
    /* 0x863E */ ZL2_SMASH_KARI_3,
    /* 0x863F */ ZL2_SMASH_KARI_4,
    /* 0x8640 */ ZL2_WATER_EFFECT_3,
    /* 0x8641 */ ZL2_WATER_EFFECT_4,
    /* 0x8642 */ ZL2_WATER_EFFECT_5,
    /* 0x8661 */ ZL2_LIQUIDANIM_1 = 0x8661,
    /* 0x8643 */ ZL2_LIQUIDANIM_2 = 0x8643,
    /* 0x8646 */ ZL2_LIQUIDANIM_3 = 0x8646,
};

namespace {
    /* 80821B30-80821B70 000020 0040+00 0/1 0/0 0/0 .data cc_ymb_src__23@unnamed@d_a_e_ymb_cpp@ */
    static dCcD_SrcSph cc_ymb_src = {
        {{0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xD}, {0xD8FBFDFF, 0x3}, 0x75}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x1, 0x7, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6}, // mGObjTg
        {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    /* 80821B70-80821BB0 000060 0040+00 0/1 0/0 0/0 .data cc_ymb_at_src__23@unnamed@d_a_e_ymb_cpp@ */
    static dCcD_SrcSph cc_ymb_at_src = {
        {{0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xD}, {0xD8FBFDFF, 0x0}, 0x0}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x1, 0x7, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f}, // mSph
        } // mSphAttr
    };

    /* 80821BB0-80821BF0 0000A0 0040+00 0/1 0/0 0/0 .data cc_ymb_eye_src__23@unnamed@d_a_e_ymb_cpp@ */
    static dCcD_SrcSph cc_ymb_eye_src = {
        {{0x0, {{AT_TYPE_0, 0x0, 0x0}, {0x80000000, 0x3}, 0x0}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f}, // mSph
        } // mSphAttr
    };

    /* 80821BF0-80821C30 0000E0 0040+00 0/1 0/0 0/0 .data cc_ymb_tube_src__23@unnamed@d_a_e_ymb_cpp@ */
    static dCcD_SrcSph cc_ymb_tube_src = {
        {{0x0, {{AT_TYPE_0, 0x0, 0x0}, {0xD8FBFDFF, 0x2}, 0x75}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x7}, // mGObjTg
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f}, // mSph
        } // mSphAttr
    };

    /* 80821C30-80821C48 000120 0018+00 3/4 0/0 0/0 .data YMB_DOWN_ATT_JNT__23@unnamed@d_a_e_ymb_cpp@ */
    static int YMB_DOWN_ATT_JNT[6] = {
        JNT_TUBE_LC2, JNT_TUBE_RA2, JNT_TUBE_LB2, JNT_TUBE_RC2, JNT_TUBE_LA2, JNT_TUBE_RB2,
    };

    /* 80821C48-80821C54 000138 000C+00 0/0 0/0 0/0 .data YMB_DROP_JNT__23@unnamed@d_a_e_ymb_cpp@ */
    static int YMB_DROP_JNT[3] = {
        JNT_TUBE_LC2, JNT_TUBE_RA2, JNT_BODY1,
    };
}

daE_YMB_HIO_c::daE_YMB_HIO_c() {
    field_0x4 = -1;
    model_size = 1.0f;
    fly_movement_speed = 10.0f;
    swim_attack_speed = 70.0f;
    fly_attack_speed = 40.0f;
    rollover_time = 750.0f;
    fly_height_adjust = 400.0f;
}

int daE_YMB_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    cXyz sp54, sp60;
    csXyz local_8c, local_94;

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E: {
            int iVar1 = (jntNo - 0x13) / 2;
            if (field_0x668[iVar1] != 0.0f) {
                if (((u16)jntNo & 1) != 0) {
                    mDoMtx_stack_c::multVecZero(&sp54);
                    mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &local_8c);
                    s16 sVar1 = (s16)(0x4000 - local_8c.z) * field_0x668[iVar1];
                    mDoMtx_stack_c::transS(sp54);
                    mDoMtx_stack_c::ZXYrotM(local_8c.x, local_8c.y, local_8c.z + sVar1);
                    mDoMtx_stack_c::scaleM(1.0f, 1.0f, 1.0f);
                } else {
                    mDoMtx_stack_c::multVecZero(&sp54);
                    mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &local_8c);
                    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo - 1));
                    mDoMtx_stack_c::transM(-100.0f, 0.0f, 0.0f);
                    mDoMtx_stack_c::multVecZero(&sp60);

                    mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &local_94);
                    sp54 = ((sp54 * (1.0f - field_0x668[iVar1])) + (sp60 * field_0x668[iVar1]));
                    local_8c.set(local_8c.x * (1.0f - field_0x668[iVar1]) + local_94.x * field_0x668[iVar1],
                                local_8c.y * (1.0f - field_0x668[iVar1]) + local_94.y * field_0x668[iVar1],
                                local_8c.z * (1.0f - field_0x668[iVar1]) + local_94.z * field_0x668[iVar1]);
                    mDoMtx_stack_c::transS(sp54);
                    mDoMtx_stack_c::ZXYrotM(local_8c);
                    mDoMtx_stack_c::scaleM(1.0f, 1.0f, 1.0f);
                }
            }

            break;
        }
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daE_YMB_c::JointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daE_YMB_c* i_this = (daE_YMB_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

static void rideCallBack(dBgW* param_1, fopAc_ac_c* aActor_p, fopAc_ac_c* param_3) {
    if (fopAcM_GetName(param_3) == PROC_ALINK) {
        ((daE_YMB_c*)aActor_p)->field_0x724 = 20;
    }
}

int daE_YMB_c::draw() {
    g_env_light.settingTevStruct(6, &current.pos, &tevStr);

    if (!field_0x6d8 || field_0x710 != 0) {
        return 1;
    }

    J3DModel* model = mpModelMorf->getModel();
    J3DShape* shape = model->getModelData()->getMaterialNodePointer(4)->getShape();
    if (shape != NULL) {
        if (field_0x717 != 0) {
            shape->show();
        } else {
            shape->hide();
        }
    }

    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    J3DModelData* modelData = model->getModelData();
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* matNodeP = modelData->getMaterialNodePointer(i);
        if (i == 4) {
            matNodeP->getTevKColor(3)->a = field_0x6dc;
        } else if (i == 3) {
            matNodeP->getTevKColor(3)->a = field_0x6d8;
        } else if (field_0x6d8 >= 255.0f) {
            matNodeP->setMaterialMode(1);
            matNodeP->getZMode()->setUpdateEnable(1);
            matNodeP->getBlend()->setType(0);
        } else {
            matNodeP->setMaterialMode(4);
            matNodeP->getZMode()->setUpdateEnable(0);
            matNodeP->getBlend()->setType(1);
            matNodeP->getTevKColor(3)->a = field_0x6d8;
        }

        if (i == 0 || i == 1) {
            matNodeP->getTevColor(0)->r = field_0x6e0;
            matNodeP->getTevColor(0)->b = field_0x6e0;
            matNodeP->getTevColor(0)->g = field_0x6e0;
        }
    }

    mpBrkAnm->entry(model->getModelData());

    if (field_0x6d8 >= 255.0f) {
        dComIfGd_setListDark();
        mpModelMorf->entryDL();
        dComIfGd_setList();
    } else {
        mInvisModel.entryDL(NULL);
    }

    cXyz sp58;
    sp58.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    tevStr.field_0x344 = (tevStr.field_0x344 * field_0x6d8) / 255.0f;
    mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model, &sp58, 2500.0f, 0.0f, 
                                    current.pos.y, field_0x6d4, mGndChk, &tevStr, 0, 
                                    1.0f, dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static int daE_YMB_Draw(daE_YMB_c* i_this) {
    return i_this->draw();
}

void daE_YMB_c::setBck(int i_index, u8 i_attr, f32 i_morf, f32 i_rate) {
    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_YB", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

bool daE_YMB_c::checkBck(int i_index) {
    if (mpModelMorf->getAnm() == (J3DAnmTransform*)dComIfG_getObjectRes("E_YB", i_index)) {
        return true;
    } else {
        return false;
    }
}

void daE_YMB_c::setActionMode(int i_action, int i_mode) {
    mAction = i_action;
    mMode = i_mode;
}

void daE_YMB_c::setLastDamage() {
    setRockDamageEffect();

    field_0x6f4++;
    if (field_0x6f4 > 6) {
        health = 0;
    }

    mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x20);
    dComIfGp_setHitMark(3, this, &eyePos, NULL, NULL, 0);
    field_0x6f8 = 3;
    setBck(BCK_YB_DAWN_DAMAGE, 0, 3.0f, 1.0f);
    dComIfGp_getVibration().StartShock(3, 31, cXyz(0.0f, 1.0f, 0.0f));
}

void daE_YMB_c::damage_check() {
    cCcD_Obj* tgHitObj;
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (field_0x6f8 == 0) {
        if (field_0xa9c.ChkTgHit() && field_0x704 == 0) {
            mAtInfo.mpCollider = field_0xa9c.GetTgHitObj();
            health = 100;
            cc_at_check(this, &mAtInfo);
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                field_0x6f8 = 20;
            } else {
                field_0x6f8 = 10;
            }

            if (mAtInfo.mAttackPower <= 1) {
                field_0x6f8 = 10;
            }

            if (mAction != 8 && player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_LEFT && player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_RIGHT &&
                mAtInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK) && player->onWolfEnemyHangBite(this)) {
                setActionMode(7, 0);
                return;
            }
        }

        tgHitObj = NULL;
        if (field_0x82c[0].ChkTgShield() && field_0x82c[0].ChkTgHit()) {
            tgHitObj = field_0x82c[0].GetTgHitObj();
        }

        if (field_0x82c[1].ChkTgShield() && field_0x82c[1].ChkTgHit()) {
            tgHitObj = field_0x82c[1].GetTgHitObj();
        }

        if (field_0xa9c.ChkTgShield() && field_0xa9c.ChkTgHit()) {
            tgHitObj = field_0xa9c.GetTgHitObj();
        }

        if (tgHitObj != NULL && (mAction == 1 || mAction == 2 || mAction == 4)) {
            setActionMode(4, 0);
        }
    }

    int iVar1 = 0;
    for (int i = 0; i < 6; i++) {
        if (field_0xbd4[i].ChkTgHit()) {
            tgHitObj = field_0xbd4[i].GetTgHitObj();
            if (!tgHitObj->ChkAtType(AT_TYPE_MIDNA_LOCK)) {
                mAtInfo.mpCollider = field_0xbd4[i].GetTgHitObj();
                health = 100;
                mAtInfo.field_0x18 = 30;
                cc_at_check(this, &mAtInfo);

                cXyz sp28;
                mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(i * 2 + 20));
                mDoMtx_stack_c::multVecZero(&sp28);
                dComIfGp_setHitMark(1, this, &sp28, NULL, NULL, 0);

                field_0x680[i] = 30;

                if (tgHitObj->ChkAtType(AT_TYPE_WOLF_CUT_TURN)) {
                    iVar1++;
                    field_0x698 = 30;
                }

                field_0xbd4[i].ResetTgHit();
                if (iVar1 >= 4) {
                    return;
                }
            }
        }
    }
}

void daE_YMB_c::setCameraSwitch() {
    field_0x713 = 0;
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (field_0x69c.absXZ(player->current.pos) < 3000.0f) {
        field_0x713 = 1;
    }

    if (field_0x713 != 0) {
        dComIfGs_onSwitch(5, fopAcM_GetRoomNo(this));
    } else {
        dComIfGs_offSwitch(5, fopAcM_GetRoomNo(this));
    }

    if (field_0x714 != 0) {
        dComIfGs_onOneZoneSwitch(15, fopAcM_GetRoomNo(this));
    } else {
        dComIfGs_offOneZoneSwitch(15, fopAcM_GetRoomNo(this));
    }
}

static u8 l_initHIO;

static daE_YMB_HIO_c l_HIO;

void daE_YMB_c::checkWaterPos() {
    Vec pos = current.pos;
    pos.y += 1000.0f;

    dBgS_ObjGndChk_Spl wtr_chk;
    wtr_chk.SetPos(&pos);
    f32 wtr_pos = dComIfG_Bgsp().GroundCross(&wtr_chk);
    if (wtr_pos != -G_CM3D_F_INF) {
        field_0x6cc = wtr_pos;
        field_0x69c.y = wtr_pos + 1000.0f + l_HIO.fly_height_adjust;

        std::fabsf(field_0x6cc - current.pos.y);
        if (current.pos.y < (200.0f + field_0x6cc)) {
            if (field_0x715 == 0) {
                setWaterEffect1();
            }

            field_0x715 = 1;
        } else {
            field_0x715 = 0;
        }
    }
}

void daE_YMB_c::checkGroundPos() {
    cXyz pos(current.pos);
    pos.y += 500.0f;
    field_0x6d0 = field_0x6cc;
    mGndChk.SetPos(&pos);
    f32 fVar1 = dComIfG_Bgsp().GroundCross(&mGndChk);
    field_0x6d4 = fVar1;
    if (fVar1 != -G_CM3D_F_INF && field_0x6d0 < fVar1) {
        field_0x6d0 = fVar1;
    }
}

void daE_YMB_c::setFlyWaitVoice() {
    if ((u8)checkBck(BCK_YB_FLY_WAIT) && mpModelMorf->checkFrame(0.0f)) {
        mSound.startCreatureVoice(Z2SE_EN_YB_V_FLY_WAIT, -1);
    }
}

void daE_YMB_c::setDownWaitVoice() {
    if ((u8)checkBck(BCK_YB_DAWN_WAIT) && mpModelMorf->checkFrame(0.0f)) {
        mSound.startCreatureVoice(Z2SE_EN_YB_V_DOWN_WAIT, -1);
    }
}

void daE_YMB_c::setHitBoardSe() {
    if (field_0x722 == 0 && mAction == 5 && mMode >= 3 && mMode <= 7) {
        if (current.pos.y > field_0x6cc - 150.0f - 200.0f) {
            mSound.startCreatureSound(Z2SE_EN_YB_HIT_FLOATBOARD, 0, -1);
            field_0x722 = 1;
            field_0x723 = 10;
        }
    }
}

void daE_YMB_c::setElecEffect1() {
    MtxP mtx = mpModelMorf->getModel()->getAnmMtx(15);
    cXyz sp28(mtx[0][3], mtx[1][3], mtx[2][3]);
    cXyz sp34(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);

    mElecParticles[0] = dComIfGp_particle_set(mElecParticles[0], ZL2_ELEC_1, &sp28, &tevStr, &shape_angle, &sp34, 0xFF, NULL, -1, NULL, NULL, NULL);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mElecParticles[0]);
    if (emitter != NULL) {
        emitter->setRate(0.2f);
        emitter->setGlobalAlpha(field_0x6c4);
    }

    mElecParticles[1] = dComIfGp_particle_set(mElecParticles[1], ZL2_ELECY_1, &sp28, &tevStr, &shape_angle, &sp34, 0xFF, NULL, -1, NULL, NULL, NULL);
    emitter = dComIfGp_particle_getEmitter(mElecParticles[1]);
    if (emitter != NULL) {
        emitter->setRate(20.0f);
        emitter->setGlobalAlpha(field_0x6c4);
    }

    mSound.startCreatureSoundLevel(Z2SE_EN_YB_ELECTRIC, 0, -1);
}

void daE_YMB_c::setElecEffect2() {
    static u16 w_eff_id[4] = {
        ZL2_ELEC_1, ZL2_ELECY_1, ZL2_ELECY_2, ZL2_GLOW02,
    };

    field_0x1324.OnAtSetBit();
    field_0x82c[0].OnAtSetBit();
    field_0x82c[1].OnAtSetBit();

    MtxP mtx = mpModelMorf->getModel()->getAnmMtx(15);
    cXyz sp34(mtx[0][3], mtx[1][3], mtx[2][3]);
    cXyz sp40(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);

    for (int i = 0; i < 4; i++) {
        mElecParticles[i] = dComIfGp_particle_set(mElecParticles[i], w_eff_id[i], &sp34, &tevStr, &shape_angle, &sp40, 0xFF, NULL, -1, NULL, NULL, NULL);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mElecParticles[i]);
        if (emitter != NULL) {
            emitter->setGlobalAlpha(field_0x6c4);
            if (i == 0) {
                emitter->setRate(2.0f);
            } else if (i == 1) {
                emitter->setRate(60.0f);
            }
        }
    }

    if (field_0x6c1 == 1) {
        mSound.startCreatureSoundLevel(Z2SE_EN_YB_ELECTRIC_LOOP_WTR, 0, -1);
    } else {
        mSound.startCreatureSoundLevel(Z2SE_EN_YB_ELECTRIC_LOOP, 0, -1);
    }
}

namespace {
    /* 80821F38-80821FEC 000128 00B4+00 4/5 0/0 0/0 .bss YMB_DOWN_POS__23@unnamed@d_a_e_ymb_cpp@ */
    static cXyz YMB_DOWN_POS[15] = {
        cXyz(-88457.0f, -18786.0f, 42166.0f),
        cXyz(-89307.0f, -18786.0f, 41366.0f),
        cXyz(-89389.0f, -18785.0f, 37267.0f),
        cXyz(-88442.0f, -18753.0f, 38460.0f),
        cXyz(-87864.0f, -18753.0f, 37112.0f),
        cXyz(-87639.0f, -18764.0f, 41016.0f),
        cXyz(-88977.0f, -18764.0f, 40658.0f),
        cXyz(-86467.0f, -18758.0f, 39379.0f),
        cXyz(-87372.0f, -18758.0f, 40362.0f),
        cXyz(-86980.0f, -18758.0f, 37583.0f),
        cXyz(-90195.0f, -18754.0f, 39893.0f),
        cXyz(-88342.0f, -18754.0f, 39785.0f),
        cXyz(-90200.0f, -18754.0f, 38100.0f),
        cXyz(-88442.0f, -18753.0f, 38460.0f),
        cXyz(-88442.0f, -18753.0f, 38460.0f),
    };
}

void daE_YMB_c::setWaterEffect1() {
    cXyz sp28(current.pos.x, field_0x6cc, current.pos.z);
    static cXyz sc(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    static u16 w_eff_id[6] = {
        ZL2_FIRE_2, ZL2_HAMON_5, ZL2_WATER_EFFECT_1, ZL2_WATER_EFFECT_2, ZL2_SMASH_KARI_1, ZL2_MIZUTAMA_IA,
    };

    for (int i = 0; i < 6; i++) {
        mWaterParticles1[i] = dComIfGp_particle_set(mWaterParticles1[i], w_eff_id[i], &sp28, &tevStr, NULL, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    }
}

void daE_YMB_c::setWaterEffect2() {
    cXyz sp28(current.pos.x, field_0x6cc, current.pos.z);
    static cXyz sc(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    static u16 w_eff_id[7] = {
        ZL2_FIRE_3, ZL2_SMASH_KARI_2, ZL2_SMASH_KARI_3, ZL2_SMASH_KARI_4, ZL2_WATER_EFFECT_3, ZL2_WATER_EFFECT_4, ZL2_WATER_EFFECT_5,
    };

    for (int i = 0; i < 7; i++) {
        mWaterParticles2[i] = dComIfGp_particle_set(mWaterParticles2[i], w_eff_id[i], &sp28, &tevStr, &shape_angle, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    }
}

void daE_YMB_c::setBoilEffect(int param_1) {
    cXyz sp30(current.pos.x, field_0x6cc, current.pos.z);
    static cXyz sc(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    static u16 w_eff_id[7] = {
        ZL2_HAMON_1, ZL2_FIRE_1, ZL2_SAND00_IA_2, ZL2_HAMON_2, ZL2_BOIL, ZL2_SAND00_IA_1, ZL2_MIZUTAMA02,
    };
    int iVar1 = param_1 == 0 ? 7 : 5;

    for (int i = 0; i < iVar1; i++) {
        mWaterParticles2[i] = dComIfGp_particle_set(mWaterParticles2[i], w_eff_id[i], &sp30, &tevStr, NULL, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    }

    Z2GetAudioMgr()->seStartLevel(Z2SE_EN_YB_ABUKU, &sp30, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void daE_YMB_c::setDamageEffect(int param_1) {
    static cXyz sc(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    static u16 w_eff_id[2] = {
        ZL2_LIQUIDANIM_2, ZL2_LIQUIDANIM_3,
    };
    cXyz pos;
    getBellyBitePos(&pos);
    mDamageParticle = dComIfGp_particle_set(mDamageParticle, w_eff_id[param_1], &pos, &tevStr, &shape_angle, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
}

void daE_YMB_c::setDownHamonEffect() {
    cXyz pos(current.pos.x, field_0x6cc, current.pos.z);
    static cXyz sc(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    static u16 w_eff_id[2] = {
        ZL2_HAMON_3, ZL2_HAMON_4,
    };
    for (int i = 0; i < 2; i++) {
        mDownHamonParticles[i] = dComIfGp_particle_set(mDownHamonParticles[i], w_eff_id[i], &pos, &tevStr, NULL, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    }
}

void daE_YMB_c::setFlyBlurEffect() {
    static u16 w_eff_id[2] = {
        ZL2_HANEBLUR64I4_1, ZL2_HANEBLUR64I4_2,
    };

    if (field_0x717 == 0 || !field_0x6dc) {
        return;
    }

    if (!field_0x6d8 || field_0x710) {
        return;
    }

    J3DModel* model = mpModelMorf->getModel();
    cXyz pos;
    mDoMtx_stack_c::copy(model->getAnmMtx(1));
    mDoMtx_stack_c::multVecZero(&pos);
    if (pos.y < field_0x6cc) {
        return;
    }

    for (int i = 0; i < 2; i++) {
        mBlurParticles[i] = dComIfGp_particle_set(mBlurParticles[i], w_eff_id[i], &pos, &tevStr);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mBlurParticles[i]);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(1));
        }
    }
}

void daE_YMB_c::setRockDamageEffect() {
    cXyz pos;
    int iVar1 = field_0x6f4 - 1;
    if (iVar1 < 0) {
        iVar1 = 0;
    }

    if (iVar1 > 5) {
        iVar1 = 5;
    }

    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(YMB_DOWN_ATT_JNT[iVar1]));
    mDoMtx_stack_c::multVecZero(&pos);
    static cXyz sc(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);

    dComIfGp_particle_set(ZL2_LIQUIDANIM_1, &pos, &tevStr, NULL, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    mSound.startCreatureVoice(Z2SE_EN_YB_V_DOWN_DAMAGE, -1);
}

void daE_YMB_c::setDeathEffect() {
    cXyz pos;
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(15));
    mDoMtx_stack_c::multVecZero(&pos);
    static cXyz sc(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    static u16 w_eff_id[2] = {
        ZL2_POLYGON_IA8, ZL2_YAMIMOYOU01,
    };
    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(w_eff_id[i], &pos, &tevStr, NULL, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    }
    fopAcM_seStart(this, Z2SE_DARK_VANISH, 0);
}

static void* search_drop(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_Drop) {
        if (!fpcM_IsCreating(fopAcM_GetID(i_actor))) {
            u8 switchBit = ((daE_YMB_c*)i_data)->getSwitchBit();
            if (switchBit == 0xFF) {
                return NULL;
            }

            if (((daObjDrop_c*)i_actor)->getYmSwbit() == switchBit) {
                return i_actor;
            }
        }
    }

    return NULL;
}

void daE_YMB_c::executeWait() {
    switch (mMode) {
        case 0:
            setActionMode(1, 0);
            field_0x70c = cM_rndF(150.0f) + 300.0f;
            field_0x708 = cM_rndF(100.0f) + 150.0f;
            break;

        case 1:
            if (field_0x6fc == 0) {
                field_0x6fc = 30;
                mMode = 2;
            }
            break;

        case 2:
            setElecEffect1();
            if (field_0x6fc == 0) {
                field_0x6fc = 60;
                mMode = 3;
            }
            break;

        case 3:
            setElecEffect2();
            if (field_0x6fc == 0) {
                mMode = 1;
                field_0x6fc = (s16)(cM_rndF(300.0f) + 300.0f);
            }
            break;
    }
}

void daE_YMB_c::setBitePos(int param_1) {
    cXyz pos;
    getBellyBitePos(&pos);
    mDownPos = pos;
}

void daE_YMB_c::setInclination() {
    s16 sVar1 = cM_atan2s(speedF, speed.y) - 0x4000;
    f32 fVar1 = speed.abs();
    if (fVar1 > 50.0f) {
        fVar1 = 50.0f;
    }

    s16 sVar2 = sVar1 * (fVar1 / 50.0f);
    if (sVar2 < -6000) {
        sVar2 = -6000;
    }

    if (sVar2 > 6000) {
        sVar2 = 6000;
    }

    cLib_chaseAngleS(&field_0x6ea, sVar2, 0x180);
}

void daE_YMB_c::setBodyAngle() {
    s16 sVar1 = current.angle.y - shape_angle.y;
    cLib_chaseAngleS(&shape_angle.z, std::abs(cM_scos(current.angle.x)) * (speed.abs() * -32.0f * cM_ssin(sVar1)), 0x100);

    f32 fVar1 = std::abs(cM_ssin(current.angle.x));
    f32 fVar2 = std::abs(cM_scos(current.angle.x));
    if (fVar2 >= fVar1) {
        cLib_chaseAngleS(&field_0x6ea, std::abs(cM_scos(current.angle.x)) * (speed.abs() * 48.0f * cM_scos(sVar1)), 0x200);
    } else {
        cLib_chaseAngleS(&field_0x6ea, speed.abs() * 48.0f * std::abs(cM_ssin(current.angle.x)), 0x200);
    }
}

bool daE_YMB_c::checkWolfLockIn() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (field_0x764 != 0 && dComIfGp_checkPlayerStatus1(0, 0x800000)) {
        f32 fVar1 = player->getSearchBallScale() + 100.0f;
        if (player->current.pos.absXZ(current.pos) < fVar1) {
            return true;
        }
    }

    return false;
}

void daE_YMB_c::executeFly() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp44(player->current.pos);
    s16 adj_angle;
    s16 playerAngleY = fopAcM_searchPlayerAngleY(this);
    cXyz sp50;

    mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
    setFlyWaitVoice();

    f32 fVar1;
    if (!checkStartBattleDemo()) {
        field_0x712 = 1;
        switch (mMode) {
            case 1:
                field_0x82c[0].OnTgSetBit();
                field_0x82c[1].OnTgSetBit();
                field_0xa9c.OnTgSetBit();

                if (field_0x70c < 60) {
                    field_0x70c = 60;
                }
                // fallthrough
            case 0:
                if (!checkBck(BCK_YB_FLY_WAIT)) {
                    setBck(BCK_YB_FLY_WAIT, 2, 5.0f, 1.0f);
                }

                mMode = 2;
                // fallthrough
            case 2:
                fVar1 = current.pos.absXZ(field_0x69c);
                if (sp44.absXZ(field_0x69c) > 3000.0f || fVar1 > 3000.0f) {
                    field_0x6a8 = field_0x69c;
                } else {
                    if (fVar1 > 2200.0f) {
                        adj_angle = cLib_targetAngleY(&current.pos, &field_0x69c);
                    } else {
                        if (fVar1 > 1800.0f) {
                            adj_angle = playerAngleY + 0x8000 + cM_rndFX(2048.0f);
                            if ((s16)(cLib_targetAngleY(&current.pos, &field_0x69c) - playerAngleY) > 0) {
                                adj_angle -= (s16) 0x1800;
                            } else {
                                adj_angle += (s16) 0x1800;
                            }
                        } else {
                            adj_angle = playerAngleY + 0x8000 + cM_rndFX(2048.0f);
                            if (cM_rnd() < 0.5f) {
                                adj_angle += (s16) 0x1800;
                            } else {
                                adj_angle -= (s16) 0x1800;
                            }
                        }
                    }

                    sp50.set(0.0f, 0.0f, 1000.0f);
                    cLib_offsetPos(&field_0x6a8, &sp44, adj_angle, &sp50);
                    field_0x6a8.y = field_0x69c.y;
                }

                current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a8);
                current.angle.x = cLib_targetAngleX(&current.pos, &field_0x6a8);
                speedF = speed.y = 0.0f;
                field_0x6c8 = 0.0f;
                field_0x6fc = 30;
                mMode = 3;

                if (std::abs(field_0x69c.y - current.pos.y) < 200.0f) {
                    field_0x6f0 = 0;
                } else {
                    field_0x6f0 = 1;
                }
                break;

            case 3:
                current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a8);
                current.angle.x = cLib_targetAngleX(&current.pos, &field_0x6a8);

                if (field_0x6f0 == 0) {
                    cLib_chaseF(&field_0x6c8, l_HIO.fly_movement_speed, 1.0f);
                } else {
                    cLib_chaseF(&field_0x6c8, l_HIO.fly_movement_speed + 10.0f, 1.0f);
                }

                cLib_chaseF(&speedF, field_0x6c8 * std::abs(cM_scos(current.angle.x)), 1.0f);
                cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &sp44), 0x400);
                cLib_chaseF(&speed.y, field_0x6c8 * cM_ssin(current.angle.x), 1.0f);
                setBodyAngle();

                if (std::abs(current.pos.y - field_0x6a8.y) < 50.0f) {
                    if (field_0x6a8.absXZ(current.pos) < 100.0f || field_0x6fc == 0) {
                        field_0x6fc = 30;
                        mMode = 4;
                    }
                }
                break;

            case 4:
                if (current.pos.y < field_0x6d0 + 650.0f) {
                    current.pos.y = field_0x6d0 + 650.0f;
                }

                cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &sp44), 0x800);
                cLib_chaseAngleS(&field_0x6ea, 0, 0x400);
                cLib_chaseAngleS(&shape_angle.z, 0, 0x400);
                cLib_chaseF(&speedF, 0.0f, 1.0f);
                cLib_chaseF(&speed.y, 0.0f, 1.0f);

                if (current.pos.y < field_0x6cc) {
                    current.pos.y = field_0x6cc;
                }

                if (checkWolfLockIn()) {
                    setActionMode(3, 0);
                    break;
                }

                if (field_0x6fc == 0) {
                    speedF = speed.y = 0.0f;
                    mMode = 2;
                }

                if (field_0x764 == 0) {
                    break;
                }

                if (field_0x70c != 0) {
                    break;
                }

                if (sp44.absXZ(field_0x69c) < 2000.0f) {
                    if (sp44.absXZ(current.pos) > 700.0f) {
                        if (std::abs(field_0x69c.y - current.pos.y) < 200.0f) {
                            speedF = speed.y = 0.0f;
                            setActionMode(2, 0);
                            return;
                        }
                    }
                }
                break;
        }
    }
}

void daE_YMB_c::executeFlyAttack() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp28;
    mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
    setFlyWaitVoice();

    s16 sVar1;
    fopAc_ac_c* actor;
    switch (mMode) {
        case 0:
            field_0x70c = cM_rndF(150.0f) + 300.0f;
            field_0x6e4 = 0;
            mSound.startCreatureVoice(Z2SE_EN_YB_V_ATTACK, -1);
            setBck(BCK_YB_ATTACK_A, 0, 5.0f, 1.0f);
            mMode = 1;
            field_0x6a8 = player->current.pos;
            field_0x6a8.y = field_0x6d0 + 650.0f - 100.0f;

            sVar1 = fopAcM_searchPlayerAngleY(this);
            shape_angle.y = sVar1;
            current.angle.y= sVar1;
            speed.y = 15.0f;
            speedF = -15.0f;
            // fallthrough
        case 1:
            setElecEffect1();
            cLib_chaseAngleS(&field_0x6ea, 0, 0x400);
            cLib_chaseAngleS(&shape_angle.z, 0, 0x400);

            sVar1 = fopAcM_searchPlayerAngleY(this);
            shape_angle.y = sVar1;
            current.angle.y = sVar1;

            cLib_chaseF(&speed.y, 0.0f, 1.0f);
            cLib_chaseF(&speedF, 0.0f, 1.0f);

            if (mpModelMorf->checkFrame(15.0f)) {
                mMode = 2;
                setElecEffect2();
                field_0x6fc = 10;
            }
            break;

        case 2:
            if (mpModelMorf->isStop()) {
                setBck(BCK_YB_ATTACK_B, 2, 0.0f, 1.0f);
            }

            setElecEffect2();
            field_0x704 = 3;
            sp28 = field_0x6a8 - current.pos;
            cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &field_0x6a8), 0x800);
            current.angle.y = shape_angle.y;
            sVar1 = (s16) cM_atan2s(sp28.absXZ(), sp28.y);
            cLib_chaseF(&speedF, l_HIO.fly_attack_speed * (cM_ssin(sVar1)), 3.0f);
            cLib_chaseF(&speed.y, l_HIO.fly_attack_speed * cM_scos(sVar1), 5.0f);

            if (field_0x6fc != 0) {
                speed.y += 5.0f;
            }

            if (current.pos.y < field_0x6d0 + 650.0f - 200.0f) {
                current.pos.y += 15.0f;
                if (speed.y < 0.0f) {
                    cLib_chaseF(&speed.y, 0.0f, 7.0f);
                }
            }

            if (field_0x1324.ChkAtHit()) {
                actor = dCc_GetAc(field_0x1324.GetAtHitObj()->GetAc());
                if (fopAcM_GetName(actor) == PROC_ALINK) {
                    field_0x6fc = 30;
                    speed.y = 0.0f;
                    speedF = 0.0f;
                    mMode = 5;
                    setBck(BCK_YB_ATTACK_C, 0, 5.0f, 1.0f);
                    current.angle.y = cLib_targetAngleY(&current.pos, &field_0x69c);
                    field_0x6ea = 0;
                    return;
                }
            }

            if (sp28.abs() < 100.0f) {
                mMode = 4;
                field_0x6fc = 10;
            }

            setInclination();
            break;

        case 4:
            setElecEffect2();
            field_0x704 = 3;
            cLib_chaseF(&speed.y, 0.0f, 3.0f);
            cLib_chaseF(&speedF, 10.0f, 3.0f);

            if (current.pos.y < field_0x6d0 + 650.0f - 150.0f) {
                current.pos.y += 15.0f;
                if (speed.y < 0.0f) {
                    cLib_chaseF(&speed.y, 0.0f, 7.0f);
                }
            }

            if (field_0x6fc == 0) {
                setBck(BCK_YB_ATTACK_C, 0, 5.0f, 1.0f);
                mMode = 5;
                field_0x6fc = 30;
            }

            setInclination();
            break;

        case 5:
            setElecEffect2();
            field_0x704 = 3;
            field_0x712 = 1;
            field_0x6e4 += 0x800;
            cLib_chaseF(&speed.y, 0.0f, 3.0f);
            cLib_chaseF(&speedF, 0.0f, 3.0f);

            if (current.pos.y < field_0x6d0 + 650.0f) {
                current.pos.y += 15.0f;
                if (speed.y < 0.0f) {
                    cLib_chaseF(&speed.y, 0.0f, 7.0f);
                }
            }

            if (checkBck(BCK_YB_ATTACK_C)) {
                if (mpModelMorf->isStop()) {
                    setBck(BCK_YB_FLY_WAIT, 2, 5.0f, 1.0f);
                }
            } else {
                cLib_chaseAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x400);
            }

            static s16 ymb_chance_time[3] = {
                110, 80, 50,
            };
            if (field_0x6fc == 0) {
                mMode = 9;
                speedF = 0.0f;
                speed.y = 0.0f;
                field_0x6fc = ymb_chance_time[field_0x6ec];
            }

            setInclination();
            break;

        case 9:
        case 10:
            field_0x712 = 1;
            if (checkWolfLockIn()) {
                setActionMode(3, 0);
            } else {
                sVar1 = fopAcM_searchPlayerAngleY(this);
                cLib_addCalcAngleS(&current.angle.y, sVar1 + 0x8000, 4, 0x1000, 0x100);
                cLib_addCalcAngleS(&shape_angle.y, sVar1, 8, 0x400, 0x80);

                if (mMode == 9) {
                    cLib_chaseF(&speed.y, 0.0f, 1.0f);
                    cLib_chaseF(&speedF, 0.0f, 1.0f);
                } else {
                    cLib_chaseF(&speed.y, 30.0f, 1.0f);
                    cLib_chaseF(&speedF, 10.0f, 1.0f);
                }

                if (current.pos.y < field_0x6d0 + 650.0f) {
                    current.pos.y += 10.0f;
                    if (speed.y < 0.0f) {
                        cLib_chaseF(&speed.y, 0.0f, 5.0f);
                    }
                }

                setInclination();

                if (checkBck(BCK_YB_ATTACK_C) && mpModelMorf->isStop()) {
                    setBck(BCK_YB_FLY_WAIT, 2, 5.0f, 1.0f);
                }

                if (mMode == 9) {
                    if (field_0x6fc == 0) {
                        mMode = 10;
                    }
                } else if (std::abs(field_0x69c.y - current.pos.y) < 100.0f) {
                    mMode = 11;
                }
            }
            break;

        case 11:
            if (cLib_chaseF(&speed.y, 0.0f, 1.0f) != 0) {
                setActionMode(1, 0);
            }
            break;
    }
}

void daE_YMB_c::executeRunAway() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp34(player->current.pos);
    s16 playerAngleY = fopAcM_searchPlayerAngleY(this);
    cXyz sp40;

    mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
    setFlyWaitVoice();
    field_0x712 = 1;

    switch (mMode) {
        case 0:
            setBck(BCK_YB_FLY_WAIT, 2, 5.0f, 1.0f);
            mMode = 1;
            // fallthrough
        case 1: {
            s16 adj_angle;
            if (current.pos.absXZ(field_0x69c) > 2200.0f) {
                adj_angle = cLib_targetAngleY(&current.pos, &field_0x69c);
            } else if (current.pos.absXZ(field_0x69c) > 1800.0f) {
                adj_angle = playerAngleY + 0x8000 + cM_rndFX(2048.0f);
                if ((s16)(cLib_targetAngleY(&current.pos, &field_0x69c) - playerAngleY) > 0) {
                    adj_angle -= 0x1800;
                } else {
                    adj_angle += 0x1800;
                }
            } else {
                adj_angle = playerAngleY + 0x8000 + cM_rndFX(2048.0f);
                if (cM_rnd() < 0.5f) {
                    adj_angle += 0x1800;
                } else {
                    adj_angle -= 0x1800;
                }
            }

            sp40.set(0.0f, 0.0f, 1300.0f);
            cLib_offsetPos(&field_0x6a8, &sp34, adj_angle, &sp40);
            field_0x6a8.y = field_0x69c.y;
            current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a8);
            current.angle.x = cLib_targetAngleX(&current.pos, &field_0x6a8);
            speed.y = 0.0f;
            speedF = 0.0f;
            field_0x6c8 = 0.0f;
            field_0x6fc = 30;
            mMode = 2;
            break;
        }

        case 2:
            current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a8);
            current.angle.x = cLib_targetAngleX(&current.pos, &field_0x6a8);
            cLib_chaseF(&field_0x6c8, l_HIO.fly_attack_speed, 5.0f);
            cLib_chaseF(&speedF, field_0x6c8 * std::abs(cM_scos(current.angle.x)), 5.0f);
            cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &sp34), 0x400);
            cLib_chaseF(&speed.y, field_0x6c8 * cM_ssin(current.angle.x), 5.0f);
            setBodyAngle();

            if (field_0x6a8.absXZ(current.pos) < 100.0f || field_0x6fc == 0) {
                mMode = 3;
                field_0x6fc = 30;
            }
            break;

        case 3:
            if (current.pos.y < field_0x6d0 + 650.0f) {
                current.pos.y = field_0x6d0 + 650.0f;
            }

            cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &sp34), 0x800);
            cLib_chaseAngleS(&field_0x6ea, 0, 0x400);
            cLib_chaseAngleS(&shape_angle.z, 0, 0x400);
            cLib_chaseF(&speedF, 0.0f, 3.0f);
            cLib_chaseF(&speed.y, 0.0f, 3.0f);

            if (current.pos.y < field_0x6cc) {
                current.pos.y = field_0x6cc;
            }

            if (checkWolfLockIn()) {
                mMode = 1;
            }

            if (field_0x6fc == 0) {
                setActionMode(1, 0);
            }
            break;
    }
}

void daE_YMB_c::executeGuard() {
    mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);

    switch (mMode) {
        case 0:
            mSound.startCreatureVoice(Z2SE_EN_YB_V_GUARD, -1);
            setBck(BCK_YB_GUARD, 0, 3.0f, 1.0f);
            current.angle.y = fopAcM_searchPlayerAngleY(this) - 0x8000;
            speedF = 10.0f;
            speed.y = 10.0f;
            mMode = 1;
            // fallthrough
        case 1:
            cLib_chaseF(&speedF, 0.0f, 0.3f);
            cLib_chaseF(&speed.y, 0.0f, 0.3f);
            cLib_chaseAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x400);

            if (mpModelMorf->isStop()) {
                setActionMode(1, 0);
            }
            break;
    }
}

void daE_YMB_c::executeSwim() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    s16 camAngleY = (s16) fopCamM_GetAngleY(camera);
    s16 cam_ply_ang_diff = camAngleY - fopAcM_searchPlayerAngleY(this);
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz player_pos(player->current.pos);
    cXyz sp40;

    switch (mMode) {
        case 0:
            setBck(BCK_YB_SIM_IN, 2, 5.0f, 1.0f);
            mMode = 1;
            field_0x6fc = 60;
            // fallthrough
        case 1:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_UNDERWATER, 0, -1);
            cLib_addCalcAngleS(&field_0x6ea, 0, 8, 0x800, 0x100);
            cLib_chaseF(&speedF, 0.0f, 1.0f);
            cLib_chaseF(&speed.y, 0.0f, 1.0f);

            if (field_0x6fc == 0 && abs(field_0x6ea) < 0x1000) {
                mMode = 2;
                speedF = 0.0f;
            }
            break;

        case 2:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_UNDERWATER, 0, -1);
            setElecEffect1();
            cLib_chaseF(&speedF, l_HIO.swim_attack_speed, 5.0f);
            cLib_chaseF(&speed.y, 0.0f, 1.0f);
            cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this) + 0x8000, 8, 0x1000, 0x100);
            current.angle.y = shape_angle.y;

            if (player_pos.absXZ(current.pos) < 500.0f) {
                field_0x6fc = 30;
            } else if (field_0x6fc == 0) {
                mMode = 3;
                field_0x700 = 150;
                field_0x6fc = 0;
                field_0x6f0 = 0;
                field_0x6e6 = cM_rndF(8192.0f) + 16384.0f;
                field_0x6e4 = field_0x6e6 + fopAcM_searchPlayerAngleY(this);
                field_0x6e8 = cM_rndF(1536.0f) + 512.0f;
            }
            break;

        case 3:
        case 4:
            field_0x71f = 1;
            setElecEffect2();

            if (field_0x6f0 == 0) {
                mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_UNDERWATER, 0, -1);
                cLib_chaseF(&speed.y, 10.0f, 1.0f);

                if (current.pos.y > field_0x6cc - 150.0f) {
                    field_0x6f0 = 1;
                    setBck(BCK_YB_SIM_OUT, 2, 5.0f, 1.0f);
                }
            } else {
                mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
                mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_ONWATER, 0, -1);
                setWaterEffect2();
                cLib_chaseF(&speed.y, -10.0f, 1.0f);

                if (current.pos.y < field_0x6cc - 150.0f) {
                    speed.y = 0.0f;
                    cLib_chaseF(&current.pos.y, field_0x6cc - 150.0f, 2.0f);
                }
            }

            cLib_chaseF(&speedF, l_HIO.swim_attack_speed, 5.0f);
            sp40.set(0.0f, 0.0f, 1000.0f);
            cLib_offsetPos(&field_0x6a8, &player_pos, field_0x6e4, &sp40);

            if (mMode == 3) {
                cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &field_0x6a8), 8, field_0x6e8, 0x40);
                current.angle.y = shape_angle.y;

                if (abs((s16)(shape_angle.y - cLib_targetAngleY(&current.pos, &field_0x6a8))) > 0x3000) {
                    field_0x6e4 = field_0x6e6 + fopAcM_searchPlayerAngleY(this);
                }

                if (current.pos.absXZ(field_0x6a8) < 500.0f) {
                    mMode = 4;
                    field_0x6fc = 30;
                }
            } else if (mMode == 4 && field_0x6fc == 0 && current.pos.absXZ(field_0x6a8) > 500.0f) {
                if (field_0x700 != 0) {
                    mMode = 3;
                    field_0x6e6 = cM_rndF(8192.0f) + 16384.0f;
                    field_0x6e4 = field_0x6e6 + fopAcM_searchPlayerAngleY(this);
                    field_0x6e8 = cM_rndF(1536.0f) + 512.0f;
                } else {
                    mMode = 5;
                    field_0x6fc = 30;
                    field_0x6e4 = fopAcM_searchPlayerAngleY(this) + 0x7000;
                }
            }
            break;

        case 5:
        case 6:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_ONWATER, 0, -1);
            setWaterEffect2();
            setElecEffect2();
            if (abs( s16(field_0x6e4 - s16(fopAcM_searchPlayerAngleY(this) - 0x8000)) ) < 0x1000) {
                field_0x6e4 -= 0x180;
            }

            if (mMode == 5) {
                sp40.set(0.0f, 0.0f, 3000.0f);
            } else {
                sp40.set(0.0f, 0.0f, 4000.0f);
            }

            cLib_offsetPos(&field_0x6a8, &player_pos, field_0x6e4, &sp40);
            cLib_chaseF(&speedF, l_HIO.swim_attack_speed, 5.0f);
            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &field_0x6a8), 8, 0x800, 0x100);
            current.angle.y = shape_angle.y;

            if (mMode == 5) {
                if (field_0x6fc == 0) {
                    bool bVar1 = false;
                    if (dComIfGp_getAttention()->LockonTruth()) {
                        if (dComIfGp_getAttention()->LockonTarget(0) == this) {
                            bVar1 = true;
                        }
                    }

                    if (bVar1 || abs(cam_ply_ang_diff) > 0x5800) {
                        mMode = 6;
                        field_0x6fc = 45;
                    }
                }
            } else if (field_0x6fc == 0) {
                mMode = 7;
                field_0x6a8 = player_pos;
                setBck(BCK_YB_ATTACK_A, 0, 10.0f, 1.0f);
            }
            break;

        case 7:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            if (mpModelMorf->isStop()) {
                setBck(BCK_YB_ATTACK_B, 2, 3.0f, 1.0f);
            }

            mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_ONWATER, 0, -1);
            setWaterEffect2();
            setElecEffect2();
            cLib_chaseF(&speedF, l_HIO.swim_attack_speed, 5.0f);
            cLib_chaseF(&current.pos.y, field_0x6d0 + 650.0f, 10.0f);
            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &field_0x6a8), 8, 0x800, 0x40);
            current.angle.y = shape_angle.y;

            if (current.pos.absXZ(field_0x6a8) < 500.0f) {
                mMode = 8;
                field_0x6fc = 10;
                field_0x6e4 = 0;
            }
            break;

        case 8:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            setElecEffect2();
            cLib_chaseF(&current.pos.y, field_0x6d0 + 650.0f, 10.0f);
            field_0x6c8 = l_HIO.swim_attack_speed;
            cLib_chaseF(&speedF, field_0x6c8 * std::abs(cM_scos(field_0x6e4)), 1.0f);
            cLib_chaseF(&speed.y, field_0x6c8 * cM_ssin(field_0x6e4), 1.0f);
            cLib_chaseAngleS(&field_0x6e4, 0x4000, 0x400);

            if (field_0x6fc == 0) {
                mMode = 9;
                setBck(BCK_YB_FLY_WAIT, 2, 5.0f, 1.0f);
                field_0x6c8 = l_HIO.swim_attack_speed;
                field_0x714 = 0;
            }
            break;

        case 9:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            setFlyWaitVoice();

            if (current.pos.y < field_0x69c.y - 500.0f) {
                cLib_chaseF(&field_0x6c8, l_HIO.fly_movement_speed, 3.0f);
            } else {
                cLib_chaseF(&field_0x6c8, 0.0f, 3.0f);
            }

            cLib_chaseAngleS(&field_0x6e4, 0x4000, 0x400);
            cLib_chaseF(&speedF, field_0x6c8 * std::abs(cM_scos(field_0x6e4)), 3.0f);
            cLib_chaseF(&speed.y, field_0x6c8 * cM_ssin(field_0x6e4), 3.0f);
            cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x1000, 0x100);

            if (!speedF && !speed.y) {
                setActionMode(1, 1);
            }
            break;
    }
}

void daE_YMB_c::executeWaterJump() {
    switch (mMode) {
        case 0:
            setBck(BCK_YB_SIM_IN, 2, 5.0f, 1.0f);
            mMode = 1;
            shape_angle.y += 0x100;
            field_0x6a8 = current.pos;
            // fallthrough
        case 1:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_UNDERWATER, 0, -1);
            cLib_addCalcAngleS(&field_0x6ea, 0x4000, 8, 0x400, 0x100);
            cLib_chaseF(&speed.y, -50.0f, 5.0f);

            if (current.pos.y < field_0x6cc - 1000.0f - 500.0f) {
                mMode = 2;
                field_0x6fc = 60;
                speedF = 0.0f;
            }
            break;

        case 2:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_UNDERWATER, 0, -1);
            cLib_addCalcAngleS(&field_0x6ea, -0x4000, 8, 0x400, 0x100);
            cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x1000, 0x100);
            current.angle.y = shape_angle.y;
            cLib_chaseF(&speed.y, 80.0f, 5.0f);

            if (field_0x6fc < 30) {
                setElecEffect1();
            }

            if (current.pos.y > field_0x6cc - 1000.0f) {
                shape_angle.y = fopAcM_searchPlayerAngleY(this);
                current.angle.y = shape_angle.y;
                field_0x6ea = 0;
                mMode = 3;
                speed.y = 80.0f;
                field_0x6f0 = 0;
                field_0x6fc = 25;
                setBck(BCK_YB_RISE, 2, 0.0f, 1.0f);
            }
            break;

        case 3:
            setElecEffect2();
            if (field_0x6f0 == 0 && current.pos.y > field_0x6cc - 500.0f) {
                field_0x6f0 = 1;
                setWaterEffect1();
                mSound.startCreatureSound(Z2SE_EN_YB_SPLASH, 0, -1);
                mSound.startCreatureVoice(Z2SE_EN_YB_V_RISE, -1);
                field_0x714 = 0;
            }

            if (field_0x715 != 0) {
                mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_UNDERWATER, 0, -1);
            } else {
                mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
                if (field_0x6fc == 0) {
                    cLib_chaseF(&speed.y, -30.0f, 3.0f);
                    if (speed.y <= 24.0f) {
                        setBck(BCK_YB_SIM_TO_FLY, 0, 3.0f, 1.0f);
                        mMode = 4;
                    }
                }
            }
            break;

        case 4:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            if (cLib_chaseF(&speed.y, -30.0f, 3.0f) != 0) {
                field_0x6fc = 10;
                mMode = 5;
            }
            break;

        case 5:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            
            if (field_0x6fc == 0 && cLib_chaseF(&speed.y, 0.0f, 3.0f) != 0 && mpModelMorf->isStop()) {
                setActionMode(1, 1);
            }
            break;
    }
}

void daE_YMB_c::getNearDownPos() {
    s16 sVar2;
    int iVar2;
    f32 fVar2;
    s16 sVar1[3];
    f32 fVar1[3];
    int iVar1[3];

    fVar1[2] = 1500.0f;
    fVar1[1] = 1500.0f;
    fVar1[0] = 1500.0f;
    iVar1[2] = -1;
    iVar1[1] = -1;
    iVar1[0] = -1;

    for (iVar2 = 0; iVar2 < 15; iVar2++) {
        sVar2 = abs((s16)(shape_angle.y + 0x8000 - cLib_targetAngleY(&current.pos, &YMB_DOWN_POS[iVar2])));
        fVar2 = current.pos.absXZ(YMB_DOWN_POS[iVar2]);

        if (sVar2 < 0x3000) {
            if (fVar2 < fVar1[0]) {
                fVar1[2] = fVar1[1];
                sVar1[2] = sVar1[1];
                iVar1[2] = iVar1[1];
                fVar1[1] = fVar1[0];
                sVar1[1] = sVar1[0];
                iVar1[1] = iVar1[0];
                fVar1[0] = fVar2;
                sVar1[1] = sVar2;
                iVar1[0] = iVar2;
            } else if (fVar2 < fVar1[1]) {
                fVar1[2] = fVar1[1];
                sVar1[2] = sVar1[1];
                iVar1[2] = iVar1[1];
                fVar1[1] = fVar2;
                sVar1[1] = sVar2;
                iVar1[1] = iVar2;
            } else if (fVar2 < fVar1[2]) {
                fVar1[2] = fVar2;
                sVar1[2] = sVar2;
                iVar1[2] = iVar2;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        if (iVar1[i] != -1 && fVar1[i] < 1000.0f && sVar1[i] < 0x3000) {
            field_0x711 = iVar1[i];
            return;
        }
    }

    if (iVar1[0] != -1) {
        field_0x711 = iVar1[0];
    } else {
        field_0x711 = 0;
    }
}

void daE_YMB_c::executeDamage() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    setFlyWaitVoice();

    switch (mMode) {
        case 0:
            mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 31);
            setBck(BCK_YB_DAMAGE, 0, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_YB_V_DAMAGE, -1);
            mSound.startCreatureSound(Z2SE_EN_YB_SHIRU, 0, -1);
            mMode = 1;
            field_0x6f0 = 0;
            field_0x6fc = 20;
            speed.y = 0.0f;
            speedF = 0.0f;
            getNearDownPos();
            dComIfGp_getVibration().StartShock(3, 31, cXyz(0.0f, 1.0f, 0.0f));
            // fallthrough
        case 1:
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            field_0x82c[0].OffCoSetBit();
            field_0x82c[1].OffCoSetBit();
            field_0x82c[0].OffTgShield();
            field_0x82c[1].OffTgShield();

            if (mpModelMorf->isStop()) {
                setBck(BCK_YB_FLY_WAIT, 2, 3.0f, 1.0f);
            }

            if (!(u8)checkBck(BCK_YB_FLY_WAIT)) {
                cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &YMB_DOWN_POS[field_0x711]) + 0x8000, 8, 0x200, 0x80);
            }
            
            if (field_0x6fc == 0) {
                cLib_chaseF(&speed.y, 0.0f, 5.0f);
            }

            cLib_addCalcAngleS(&field_0x6ea, 0, 8, 0x1000, 0x10);

            if (!field_0x6d8) {
                player->offWolfEnemyHangBite();
                setActionMode(1, 1);
                break;
            }

            player->setWolfEnemyHangBiteAngle(shape_angle.y - 0x8000);

            if (checkWolfBiteDamage()) {
                dComIfGp_getVibration().StartShock(3, 31, cXyz(0.0f, 1.0f, 0.0f));
                offWolfBiteDamage();
                field_0x6f0++;

                if (field_0x6f0 >= 5) {
                    field_0x6ec++;
                    player->offWolfEnemyHangBite();
                    mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x20);
                    mSound.startCreatureVoice(Z2SE_EN_YB_V_DAMAGE, -1);

                    if (field_0x6ec >= 3) {
                        setActionMode(8, 0);
                        break;
                    }

                    mMode = 5;
                    break;
                }

                setBck(BCK_YB_DAMAGE, 0, 3.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_YB_V_DAMAGE, -1);
                speed.y = 20.0f;
                mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 30);
                mSound.startCreatureSound(Z2SE_EN_YB_SHIRU, 0, -1);
            } else {
                if (field_0x720 == 22) {
                    setDamageEffect(1);
                } else {
                    setDamageEffect(0);
                }
            }

            if (player->checkWolfEnemyHangBiteOwn(this)) {
                break;
            }

            setActionMode(1, 1);
            break;

        case 5:
            mMode = 6;
            initDownToWater();
            // fallthrough
        case 6:
            calcDownToWater();

            if (current.pos.y < field_0x6cc && field_0x714 == 0) {
                mSound.startCreatureSound(Z2SE_EN_YB_SPLASH, 0, -1);
                field_0x714 = 1;
            }

            if (current.pos.y < field_0x6cc - 1000.0f) {
                if (speed.y < -20.0f) {
                    speed.y = -20.0f;
                }

                mMode = 7;
                field_0x6fc = 10;
            }
            break;

        case 7:
            if (mpModelMorf->isStop()) {
                setBck(BCK_YB_DROP, 2, 3.0f, 1.0f);
            }

            cLib_addCalcAngleS(&field_0x6ea, -0x8000, 8, 0x1000, 0x10);
            cLib_chaseF(&speedF, 0.0f, 1.0f);
            cLib_chaseF(&speed.y, -10.0f, 5.0f);

            if (field_0x6fc != 0) {
                break;
            }

            setActionMode(5, 0);
            break;
    }
}

void daE_YMB_c::setMidnaBindInit(cXyz* i_pos) {
    cXyz i_scale(1.0f, 1.0f, 1.0f);
    csXyz angle;
    cXyz sp30;
    MtxP mtx = mDoMtx_stack_c::get();
    daMidna_c* midna = daPy_py_c::getMidnaActor();
    MtxP mtxHairTop = midna->getMtxHairTop();

    MTXCopy(mtxHairTop, mtx);
    mDoMtx_stack_c::multVecZero(&sp30);
    cXyz sp3c = sp30 - *i_pos;
    angle.y = cM_atan2s(sp3c.x, sp3c.z);
    angle.x = -cM_atan2s(sp3c.y, JMAFastSqrt(sp3c.x * sp3c.x + sp3c.z * sp3c.z));
    angle.z = 0;

    static GXColor e_prim = {0xFF, 0x64, 0x78, 0x00};
    static GXColor e_env = {0x3C, 0x1E, 0x1E, 0x00};
    JPABaseEmitter* emitter = dComIfGp_particle_set(0x29B, i_pos, &tevStr, &angle, &i_scale, 0xFF, NULL, fopAcM_GetRoomNo(this), &e_prim, &e_env, NULL);
    if (emitter != NULL) {
        emitter->setGlobalParticleHeightScale(0.01f * sp3c.abs());
    }

    dComIfGp_particle_set(0x29C, i_pos, &tevStr, &shape_angle, &i_scale, 0xFF, NULL, fopAcM_GetRoomNo(this), &e_prim, &e_env, NULL);
    mSound.startCreatureSound(Z2SE_MIDNA_BIND_LOCK_ON, 0, -1);
}

void daE_YMB_c::setMidnaBindLevel(int i_index) {
    cXyz i_scale(1.0f, 1.0f, 1.0f);
    cXyz i_pos;
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(YMB_DOWN_ATT_JNT[i_index]));
    mDoMtx_stack_c::multVecZero(&i_pos);

    static GXColor e_prim = {0xFF, 0x64, 0x78, 0x00};
    static GXColor e_env = {0x3C, 0x1E, 0x1E, 0x00};
    static u16 eff_id[3] = {
        ZL2_MIDONAHAIR03_1, ZL2_MIDONAHAIR03_2, ZL2_MIDONAHAIR01,
    };
    int iVar1 = i_index * 3;
    for (int i = 0; i < 3; i++) {
        mMidnaBindParticles[iVar1 + i] = dComIfGp_particle_set(mMidnaBindParticles[iVar1 + i], eff_id[i], &i_pos, &tevStr, &shape_angle, &i_scale, 0xFF, 0, fopAcM_GetRoomNo(this), &e_prim, &e_env, NULL);
    }
}

void daE_YMB_c::setWolfLockEffect() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz i_pos;

    if (!player->checkWolfLock(this)) {
        field_0x6f0 = 0;
        field_0x700 = 0;
        field_0x718 = 0;
    } else {
        int iVar1 = 0;
        f32 fVar1 = 3000.0f;
        f32 fVarTemp;

        if (field_0x700 == 0 && field_0x6f0 < 6) {
            for (int i = 0; i < 6; i++) {
                if ((field_0x718 & 1 << i) == 0) {
                    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(YMB_DOWN_ATT_JNT[i]));
                    mDoMtx_stack_c::multVecZero(&i_pos);

                    fVarTemp = i_pos.abs(player->current.pos);
                    if (fVarTemp < fVar1) {
                        iVar1 = i;
                        fVar1 = fVarTemp;
                    }
                }
            }

            field_0x718 |= 1 << iVar1;
            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(YMB_DOWN_ATT_JNT[iVar1]));
            mDoMtx_stack_c::multVecZero(&i_pos);
            setMidnaBindInit(&i_pos);
            field_0x700 = 3;
            field_0x6f0++;
        }

        for (int i = 0; i < 6; i++) {
            if ((field_0x718 & 1 << i) != 0) {
                setMidnaBindLevel(i);
            }
        }

        if (field_0x718 != 0) {
            mSound.startCreatureSoundLevel(Z2SE_MIDNA_BIND_LOCK_SUS, 0, -1);
        }
    }
}

void daE_YMB_c::initDownToWater() {
    speed.y = 40.0f;
    speedF = 40.0f;
    shape_angle.y = cLib_targetAngleY(&current.pos, &YMB_DOWN_POS[field_0x711]) + 0x8000;
    current.angle.y = shape_angle.y + 0x8000;
    setBck(BCK_YB_DAMAGE_DAWN, 0, 3.0f, 1.0f);
    mSound.startCreatureVoice(Z2SE_EN_YB_V_DAMAGE_DOWN, -1);
    field_0x6fc = (s16)((current.pos.y - field_0x6cc) / 50.0f + 16.0f);
}

void daE_YMB_c::calcDownToWater() {
    if (mpModelMorf->isStop()) {
        setBck(BCK_YB_DROP, 2, 3.0f, 1.0f);
    }

    cLib_addCalcAngleS(&field_0x6ea, -0x7800, 8, 0x1000, 0x10);

    if (field_0x6fc != 0) {
        current.pos.absXZ(YMB_DOWN_POS[field_0x711]);
    }

    f32 fVarTemp = current.pos.y;
    cXyz sp48 = YMB_DOWN_POS[field_0x711];
    cLib_chasePosXZ(&current.pos, sp48, 40.0f);
    current.pos.y = fVarTemp;
    cLib_chaseF(&speed.y, -60.0f, 5.0f);
}

void daE_YMB_c::executeDown() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    setDownWaitVoice();

    switch (mMode) {
        case 0:
            mMode = 1;
            initDownToWater();
            // fallthrough
        case 1:
            calcDownToWater();

            if (current.pos.y < field_0x6cc - 50.0f) {
                mSound.startCreatureSound(Z2SE_EN_YB_SPLASH, 0, -1);
                field_0x714 = 1;
                speedF = 0.0f;
                mMode = 2;
                mSound.startCreatureVoice(Z2SE_EN_YB_V_DOWN_WAIT, -1);
                setBck(BCK_YB_DAWN_WAIT, 2, 3.0f, 1.0f);
                field_0x717 = 0;
            }
            break;

        case 2:
            if (mpBgW != NULL && !mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Regist(mpBgW, this);
            }

            cLib_chaseF(&speed.y, 20.0f, 3.0f);

            if (current.pos.y > field_0x6cc - 50.0f) {
                current.pos.y = field_0x6cc - 50.0f;
                speed.y = 0.0f;
                speedF = 0.0f;
                mMode = 3;
                field_0x6fc = l_HIO.rollover_time;
                field_0x6f4 = 1;
                field_0x6f0 = 0;
                field_0x700 = 0;
                field_0x718 = 0;
                dComIfGs_onOneZoneSwitch(1, fopAcM_GetRoomNo(this));
            }
            break;

        case 3:
            if (field_0x724 != 0) {
                field_0x716 = 1;
            } else {
                field_0x716 = 0;
            }

            setWolfLockEffect();
            setDownHamonEffect();
            field_0x82c[0].OffCoSetBit();
            field_0x82c[1].OffCoSetBit();
            field_0x82c[0].OffTgSetBit();
            field_0x82c[1].OffTgSetBit();

            for (int i = 0; i < 6; i++) {
                field_0xbd4[i].OnCoSetBit();

                if (field_0x680[i] != 0 || field_0x698 != 0) {
                    field_0xbd4[i].OffTgSetBit();
                } else {
                    field_0xbd4[i].OnTgSetBit();
                }

                if (player->checkWolfLock(this)) {
                    field_0xbd4[i].OffTgShield();
                }
            }

            if (mpBgW != NULL && !mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Regist(mpBgW, this);
            }

            cLib_addCalcAngleS(&field_0x6ea, -0x8000, 8, 0x1000, 0x10);

            if (field_0x6fc == 0 && !player->checkWolfLock(this)) {
                if (mpBgW != NULL) {
                    dComIfG_Bgsp().Release(mpBgW);
                }

                for (int i = 0; i < 6; i++) {
                    field_0xbd4[i].OffTgSetBit();
                    field_0xbd4[i].OffCoSetBit();
                }

                setActionMode(6, 0);
                field_0x6ec = 2;
                field_0x716 = 0;
                field_0x6f4 = 0;
                field_0x717 = 1;
                dComIfGs_offOneZoneSwitch(1, fopAcM_GetRoomNo(this));
            }

            if (player->getCutType() == daPy_py_c::CUT_TYPE_WOLF_LOCK) {
                mMode = 4;
                field_0x6fc = 0;
            }
            break;

        case 4:
            setDownHamonEffect();
            field_0x82c[0].OffCoSetBit();
            field_0x82c[1].OffCoSetBit();
            field_0x82c[0].OffTgSetBit();
            field_0x82c[1].OffTgSetBit();
            field_0x82c[0].OffTgShield();
            field_0x82c[1].OffTgShield();

            for (int i = 0; i < 6; i++) {
                field_0xbd4[i].OffTgSetBit();
                field_0xbd4[i].OffTgShield();
                field_0xbd4[i].OffCoSetBit();
            }

            if (mpBgW != NULL && mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW);
            }

            if (field_0x6fc == 0) {
                cXyz sp40;
                mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(getDownLockPoint()));
                mDoMtx_stack_c::multVecZero(&sp40);
                if (sp40.absXZ(player->current.pos) < 150.0f) {
                    setLastDamage();
                    field_0x6fc = 3;
                }
            }

            if (player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_LOCK) {
                setActionMode(9, 0);
                dComIfGs_offOneZoneSwitch(1, fopAcM_GetRoomNo(this));
            }
            break;
    }
}

void daE_YMB_c::setCreateDrop() {
    daObjDrop_c* drop = (daObjDrop_c*)fpcM_Search(search_drop, this);
    if (drop != NULL) {
        cXyz sp1c;
        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(15));
        mDoMtx_stack_c::multVecZero(&sp1c);
        drop->setPos(sp1c);
        drop->setAppear();
        fopAcM_OnStatus(drop, fopAcM_STATUS_UNK_0x4000);
    }

    field_0x721 = 1;
}

void daE_YMB_c::executeDeath() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp24, pos;

    switch (mMode) {
        case 0:
            setDownHamonEffect();
            if (!eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
                eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            mSound.startCreatureVoice(Z2SE_EN_YB_V_DEATH, -1);
            setBck(BCK_YB_DEATH, 0, 3.0f, 1.0f);
            attention_info.flags = 0;

            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);

            current.pos.set(-88400.0f, field_0x6cc, 39000.0f);
            shape_angle.y = 0x800;
            pos.set(-87900.0f, -18800.0f, 38050.0f);
            player->setPlayerPosAndAngle(&pos, 0x6500, 0);

            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(15));
            mDoMtx_stack_c::multVecZero(&mDemoCamCenter);
            field_0x760 = 0x6F00;
            field_0x75c = 1800.0f;
            sp24.set(0.0f, 100.0f, field_0x75c);
            cLib_offsetPos(&mDemoCamEye, &current.pos, field_0x760, &sp24);
            mDemoCamFovy = 60.0f;
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
            field_0x6e6 = 0x6500;
            field_0x762 = 40;
            field_0x6fc = 270;
            mMode = 2;
            Z2GetAudioMgr()->setBattleBgmOff(true);
            break;

        case 1:
        case 2:
            setDownHamonEffect();
            cLib_addCalcAngleS(&field_0x6e6, -0x1C00, 8, 0x200, 0x80);
            player->setPlayerPosAndAngle(&player->current.pos, field_0x6e6, 0);
            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(15));
            mDoMtx_stack_c::multVecZero(&mDemoCamCenter);

            if (field_0x6fc == 0) {
                cLib_chaseAngleS(&field_0x762, 0, 4);
            }

            field_0x760 += field_0x762;
            cLib_chaseF(&field_0x75c, 1300.0f, 5.0f);
            sp24.set(0.0f, 100.0f, field_0x75c);
            cLib_offsetPos(&mDemoCamEye, &current.pos, field_0x760, &sp24);
            mDemoCamFovy = 60.0f;

            if (mMode == 2) {
                if (mpModelMorf->checkFrame(263.0f)) {
                    setWaterEffect1();
                    mSound.startCreatureSound(Z2SE_EN_YB_SPLASH, 0, -1);
                }

                if (mpModelMorf->checkFrame(280.0f)) {
                    mMode = 3;
                    field_0x6fc = 60;
                    mDemoCamCenter = current.pos;
                    mDemoCamEye.set(-87893.0f, -18731.0f, 37578.0f);
                    field_0x75c = 30.0f;
                }
            }

            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
            break;

        case 3:
            setDownHamonEffect();
            current.pos.y -= 2.0f;
            cLib_chaseF(&field_0x6e0, -100.0f, 2.0f);

            if ( ((field_0x6fc == 0) & mpModelMorf->isStop()) != 0) {
                field_0x710 = 1;
                mMode = 4;
                field_0x6fc = 90;
                setCreateDrop();
                setDeathEffect();
            }
            break;

        case 4:
            cLib_chaseF(&field_0x75c, -5.0f, 1.0f);
            mDemoCamCenter.y += field_0x75c;
            
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

            if (field_0x6fc == 0) {
                if (mSwitchBit != 0xFF && !dComIfGs_isSwitch(mSwitchBit, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(mSwitchBit, fopAcM_GetRoomNo(this));
                }

                camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye);
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                dComIfGs_offSwitch(5, fopAcM_GetRoomNo(this));
                dComIfGs_offOneZoneSwitch(15, fopAcM_GetRoomNo(this));
                fopAcM_delete(this);
            }
            break;
    }
}

void daE_YMB_c::demo_skip(int param_1) {
    switch (param_1) {
        case 0:
            setActionMode(11, 0);
            Z2GetAudioMgr()->setBattleBgmOff(true);
            break;

        case 1:
            dComIfGp_getVibration().StopQuake(31);
            setActionMode(1, 1);
            Z2GetAudioMgr()->subBgmStop();
            Z2GetAudioMgr()->setBattleBgmOff(false);
            Z2GetAudioMgr()->setForceBattleArea(true, 3000, 4000, 6000);
            current.pos.y = field_0x69c.y;
            speedF = 0.0f;
            speed.y = 0.0f;
            shape_angle.y = cLib_targetAngleY(&current.pos, &daPy_getPlayerActorClass()->current.pos);
            break;

        case 3:
            dComIfGp_getVibration().StopQuake(31);
            // fallthrough
        case 2:
            attention_info.flags &= ~fopAc_AttnFlag_UNK_0x400000;
            setActionMode(1, 1);
            field_0x764 = 1;
            speedF = 0.0f;
            speed.y = 0.0f;
            shape_angle.y = cLib_targetAngleY(&current.pos, &daPy_getPlayerActorClass()->current.pos);
            break;
    }

    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye);
    camera->mCamera.Start();
    camera->mCamera.SetTrimSize(0);
    dComIfGp_event_reset();
}

int daE_YMB_c::DemoSkipCallBack(void* i_this, int param_2) {
    if (i_this) {
        static_cast<daE_YMB_c*>(i_this)->demo_skip(param_2);
        return 1;
    }

    return 0;
}

void daE_YMB_c::calcLakeDemoPlayerPos() {
    cXyz sp1c(field_0x69c.x, 0.0f, field_0x69c.z);
    cXyz sp28(0.0f, field_0x6cc + field_0x75c, 2000.0f);
    cLib_offsetPos(&current.pos, &sp1c, field_0x6e4, &sp28);
}

void daE_YMB_c::executeLakeDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp24;

    switch (mMode) {
        case 0:
            field_0x710 = 1;
            field_0x82c[0].OffTgSetBit();
            field_0x82c[1].OffTgSetBit();
            field_0xa9c.OffTgSetBit();

                 /* dSv_event_flag_c::F_0005 - Misc. - Gathered 14 Tears of Light in area 4 */
            if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[9])) {
                return;
            }

            if (dComIfGs_isSaveSwitch(17)) {
                field_0x710 = 0;
                setActionMode(1, 1);
                field_0x764 = 1;
                Z2GetAudioMgr()->setBattleBgmOff(false);
                Z2GetAudioMgr()->setForceBattleArea(true, 3000, 4000, 6000);
                field_0x765 = 1;
                return;
            }

            if (dComIfGs_isSaveSwitch(16)) {
                field_0x710 = 0;
                setActionMode(1, 1);
                Z2GetAudioMgr()->setBattleBgmOff(false);
                Z2GetAudioMgr()->setForceBattleArea(true, 3000, 4000, 6000);
                return;
            }

            if (dComIfGs_isSaveSwitch(15)) {
                field_0x710 = 0;
                setActionMode(11, 0);
                Z2GetAudioMgr()->setBattleBgmOff(true);
                return;
            }

            field_0x6f0 = 0;
            mMode = 1;
            field_0x715 = 1;
            field_0x75c = -300.0f;
            field_0x6c4 = 0.0f;
            break;

        case 1:
            if (!eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
                eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            mDemoCamCenter.set(-81000.0f, -17000.0f, 33000.0f);
            mDemoCamFovy = 50.0f;
            field_0x760 = 0x7000;
            field_0x762 = -0x30;
            sp24.set(0.0f, 0.0f, 1000.0f);
            cLib_offsetPos(&mDemoCamEye, &mDemoCamCenter, field_0x760, &sp24);
            mMode = 2;
            field_0x6e4 = 0x7FFF;
            field_0x710 = 0;
            dComIfGs_onSaveSwitch(15);
            /* dSv_event_flag_c::M_086 - Twilight Hyrule Field - Show Boss Bug's Tear of Light on the map */
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0x77]);
            break;

        case 2:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 0);
            field_0x760 += field_0x762;
            mDemoCamCenter.set(-81000.0f, -17000.0f, 33000.0f);
            sp24.set(0.0f, 0.0f, 1000.0f);
            cLib_offsetPos(&mDemoCamEye, &mDemoCamCenter, field_0x760, &sp24);
            mDemoCamFovy = 50.0f;

            if (field_0x760 < 0x6800) {
                cLib_chaseF(&field_0x75c, 300.0f, 10.0f);
            }

            if (field_0x760 < 0x6000) {
                mMode = 3;
                field_0x6fc = 90;
            }
            break;

        case 3:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 0);
            cLib_chaseF(&field_0x75c, 300.0f, 10.0f);
            cLib_chaseAngleS(&field_0x762, 0, 0x10);
            field_0x760 += field_0x762;
            sp24.set(0.0f, 0.0f, 1000.0f);
            cLib_offsetPos(&mDemoCamEye, &mDemoCamCenter, field_0x760, &sp24);
            cLib_chasePos(&mDemoCamCenter, current.pos, 40.0f);
            cLib_chasePos(&mDemoCamEye, current.pos, 40.0f);
            mDemoCamFovy = 50.0f;

            if (field_0x6fc == 0) {
                field_0x6e4 = 0;
                calcLakeDemoPlayerPos();
                mDemoCamCenter = current.pos;
                sp24.set(0.0f, -700.0f, 3000.0f);
                cLib_offsetPos(&mDemoCamEye, &field_0x69c, 0x6000, &sp24);
                mDemoCamFovy = 50.0f;
                mMode = 5;
                field_0x6fc = 70;
                field_0x700 = 170;
            }
            break;

        case 5:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 0);

            if (field_0x6fc != 0) {
                cXyz sp30(mDemoCamCenter);
                mDemoCamCenter = current.pos;
                sp24.set(0.0f, -700.0f, 3500.0f);
                cLib_offsetPos(&mDemoCamEye, &field_0x69c, 0x6000, &sp24);
                mDemoCamFovy = 50.0f;
                field_0x740 = mDemoCamCenter - sp30;
            } else {
                field_0x6f0 = 1;
                mDemoCamCenter += field_0x740;
                cLib_chasePos(&field_0x740, cXyz(0.0f, 0.0f, 0.0f), 5.0f);
                cLib_chaseF(&field_0x75c, 0.0f, 5.0f);
            }

            if (field_0x700 == 0) {
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                Z2GetAudioMgr()->setBattleBgmOff(true);
                setActionMode(11, 0);
                return;
            }
            break;
    }

    camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

    if (field_0x6f0 == 0) {
        if (field_0x75c >= 200.0f) {
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_MOVE_ONWATER, 0, -1);
            setWaterEffect2();
        }

        setElecEffect2();
        field_0x6a8 = current.pos;
        field_0x6e8 = -0x300;
        field_0x6e4 += field_0x6e8;
        calcLakeDemoPlayerPos();
        speed.y = 0.0f;
        speedF = 0.0f;
        shape_angle.y = cLib_targetAngleY(&field_0x6a8, &current.pos);
    }
}

void daE_YMB_c::executeStartDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp28, sp34;

    switch (mMode) {
        case 0:
            field_0x710 = 1;
            if (player->current.pos.absXZ(field_0x69c) > 2500.0f) {
                return;
            }

            if (dComIfGp_checkPlayerStatus0(0, 0x100000) || dComIfGp_checkPlayerStatus0(0, 0x100)) {
                return;
            }

            mMode = 1;
            break;

        case 1:
            if (!eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
                eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            sp34.set(-89100.0f, -18800.0f, 39410.0f);
            player->setPlayerPosAndAngle(&sp34, 0x5400, 0);
            player->changeDemoMode(4, 1, 0, 0);
            speed.y = 0.0f;
            speedF = 0.0f;
            current.pos.set(-88463.0f, field_0x6cc - 1500.0f, 38868.0f);
            field_0x715 = 1;
            field_0x710 = 0;
            mDemoCamCenter.set(-88660.0f, -18918.0f, 38981.0f);
            mDemoCamEye.set(-89144.0f, -18551.0f, 39753.0f);
            mDemoCamFovy = 50.0f;
            mMode = 2;
            field_0x6fc = 60;
            field_0x75c = mDemoCamEye.absXZ(current.pos);
            field_0x760 = cLib_targetAngleY(&current.pos, &mDemoCamEye);
            dComIfGs_onSaveSwitch(16);
            Z2GetAudioMgr()->subBgmStart(Z2BGM_YAMIMUSHI_B_D01);
            break;

        case 2:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 1);
            setBoilEffect(0);
            sp34.set(-89100.0f, player->current.pos.y, 39410.0f);
            player->setPlayerPosAndAngle(&sp34, 0x5400, 0);

            if (field_0x6fc == 0) {
                player->changeOriginalDemo();
                player->changeDemoMode(0x17, 0, 0, 0);
                mMode = 3;
                field_0x6fc = 360;
                field_0x762 = 16;
                u8 uVar1[6] = {0, 32, 1, 0, 32, 4};
                dComIfGp_getVibration().StartQuake(uVar1, 1, 1, cXyz(0.0f, 1.0f, 0.0f));
            }
            break;

        case 3:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 1);
            setBoilEffect(0);
            sp34.set(current.pos.x, field_0x6cc - 120.0f, current.pos.z);
            cLib_chasePos(&mDemoCamCenter, sp34, 1.5f);
            sp34.set(-88800.0f, -18266.0f, 39412.0f);
            cLib_chasePos(&mDemoCamEye, sp34, 1.7f);
            cLib_chaseF(&current.pos.y, field_0x6cc - 300.0f, 2.2f);
            setElecEffect2();

            if (field_0x6fc == 0) {
                mMode = 4;
                field_0x6fc = 120;
            }
            break;

        case 4:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 1);

            if (field_0x6fc >= 95) {
                setBoilEffect(1);
                
                if (field_0x6fc == 95) {
                    dComIfGp_getVibration().StopQuake(31);
                }
            }

            sp34.set(current.pos.x, field_0x6cc - 120.0f, current.pos.z);
            cLib_chasePos(&mDemoCamCenter, sp34, 1.5f);
            sp34.set(-88800.0f, -18266.0f, 39412.0f);
            cLib_chasePos(&mDemoCamEye, sp34, 1.7f);
            cLib_chaseF(&current.pos.y, field_0x6cc - 300.0f, 2.2f);

            if (field_0x6fc == 0) {
                shape_angle.y = fopAcM_searchPlayerAngleY(this);
                mMode = 8;
                speed.y = 80.0f;
                field_0x6fc = 15;
                setBck(BCK_YB_RISE, 2, 0.0f, 1.0f);
                field_0x6f0 = 0;
            }
            break;

        case 8:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 1);

            if (field_0x6f0 == 0) {
                if (current.pos.y >= field_0x6cc) {
                    setWaterEffect1();
                    mSound.startCreatureSound(Z2SE_EN_YB_SPLASH, 0, -1);
                    field_0x6f0 = 1;
                    dComIfGp_getVibration().StartShock(6, 31, cXyz(0.0f, 1.0f, 0.0f));
                }
            } else {
                mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            }

            mDemoCamCenter.set(current.pos.x, current.pos.y - 200.0f, current.pos.z);

            if (mDemoCamCenter.y < field_0x6cc - 120.0f) {
                mDemoCamCenter.y = field_0x6cc - 120.0f;
            }

            sp34.set(-89000.0f, -18583.0f, 39350.0f);
            cLib_chasePos(&mDemoCamEye, sp34, 10.0f);
            setElecEffect2();

            if (field_0x6fc == 0) {
                cLib_chaseF(&speed.y, -30.0f, 3.0f);
                if (speed.y <= 24.0f) {
                    setBck(BCK_YB_SIM_TO_FLY, 0, 3.0f, 1.0f);
                    mMode = 9;
                }
            }
            break;

        case 9:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 1);
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            sp34.set(-89000.0f, -18583.0f, 39350.0f);
            cLib_chasePos(&mDemoCamEye, sp34, 10.0f);
            mDemoCamCenter.set(current.pos.x, current.pos.y - 200.0f, current.pos.z);
            setElecEffect2();

            if (cLib_chaseF(&speed.y, -30.0f, 3.0f) != 0) {
                field_0x6fc = 10;
                mMode = 10;
            }
            break;

        case 10:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 1);
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            sp34.set(-89260.0f, -18650.0f, 39500.0f);
            cLib_chasePos(&mDemoCamEye, sp34, 10.0f);
            mDemoCamCenter.set(current.pos.x, current.pos.y - 200.0f, current.pos.z);
            setElecEffect2();

            if (field_0x6fc == 0 && cLib_chaseF(&speed.y, 0.0f, 3.0f) != 0 && mpModelMorf->isStop()) {
                mMode = 11;
                field_0x6fc = 30;
            }
            break;

        case 11:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 1);
            mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
            setElecEffect2();

            if (field_0x6fc == 0) {
                camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye);
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                setActionMode(1, 1);
                Z2GetAudioMgr()->setBattleBgmOff(false);
                Z2GetAudioMgr()->setForceBattleArea(true, 3000, 4000, 6000);
                return;
            }
            break;
    }

    camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
}

bool daE_YMB_c::checkStartBattleDemo() {
    if (field_0x764 == 0 && daPy_getPlayerActorClass()->checkNowWolfEyeUp()) {
        if (daPy_getPlayerActorClass()->current.pos.absXZ(field_0x69c) < 2500.0f) {
            setActionMode(12, 0);
            return true;
        }
    }

    return false;
}

void daE_YMB_c::executeBattleDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp1c(player->current.pos);
    cXyz sp28, sp34;
    s16 sVar1;

    mSound.startCreatureSoundLevel(Z2SE_EN_YB_FLY, 0, -1);
    setFlyWaitVoice();

    switch (mMode) {
        case 0:
            mMode = 1;
            attention_info.flags |= fopAc_AttnFlag_UNK_0x400000;
            // fallthrough
        case 1:
            if (!eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
                eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            dComIfGp_event_setTalkPartner(this);
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            mDemoCamCenter = camera->mCamera.Center();
            mDemoCamEye = camera->mCamera.Eye();
            mDemoCamFovy = camera->mCamera.Fovy();
            player->setPlayerPosAndAngle(&sp1c, fopCamM_GetAngleY(camera), 0);
            sp28.set(0.0f, 1000.0f, 2000.0f);
            cLib_offsetPos(&current.pos, &sp1c, fopCamM_GetAngleY(camera), &sp28);
            shape_angle.y = fopAcM_searchPlayerAngleY(this);
            speedF = 0.0f;
            speed.y = 0.0f;
            setBck(BCK_YB_CRY, 0, 3.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_YB_V_CRY, 0, -1);
            mMode = 2;
            dComIfGp_getVibration().StartQuake(8, 31, cXyz(0.0f, 1.0f, 0.0f));
            dComIfGs_onSaveSwitch(17);
            field_0x765 = 1;
            break;

        case 2:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 3);

            if (mpModelMorf->checkFrame(30.0f)) {
                dComIfGp_getVibration().StopQuake(31);
            }

            sp34.set(current.pos.x, current.pos.y - 70.0f, current.pos.z);
            cLib_chasePos(&mDemoCamCenter, sp34, 200.0f);
            sp28.set(0.0f, -200.0f, 500.0f);
            cLib_offsetPos(&sp34, &current.pos, shape_angle.y, &sp28);
            cLib_chasePos(&mDemoCamEye, sp34, 150.0f);
            cLib_chaseF(&mDemoCamFovy, 50.0f, 2.0f);

            if (mpModelMorf->isStop()) {
                speed.y = 0.0f;
                speedF = 0.0f;
                field_0x6c8 = 0.0f;
                field_0x74c.set(current.pos.x, field_0x69c.y, current.pos.z);
                field_0x760 = shape_angle.y;
                field_0x6f0 = 0;
                setBck(BCK_YB_FLY_WAIT, 2, 5.0f, 1.0f);
                mMode = 3;
            }
            break;

        case 3:
            sp28.set(0.0f, 0.0f, 600.0f);

            if (field_0x6f0 != 1) {
                sVar1 = field_0x760 + 0x5800;
            } else {
                sVar1 = field_0x760 - 0x5800;
            }

            cLib_offsetPos(&field_0x6a8, &current.pos, sVar1, &sp28);
            field_0x6a8.y = field_0x69c.y;
            mMode = 4;
            field_0x6f0++;
            field_0x6fc = 110;
            // fallthrough
        case 4:
        case 5:
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 2);
            cLib_chaseF(&mDemoCamCenter.y, current.pos.y - 200.0f, 3.0f);
            sp34.set(current.pos.x, mDemoCamCenter.y, current.pos.z);
            cLib_chasePosXZ(&mDemoCamCenter, sp34, 5.0f);
            cLib_chaseF(&mDemoCamEye.y, current.pos.y - 500.0f, 1.0f);
            current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a8);
            current.angle.x = cLib_targetAngleX(&current.pos, &field_0x6a8);
            cLib_chaseF(&speedF, field_0x6c8 * std::abs(cM_scos(current.angle.x)), 1.0f);
            cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &sp1c), 0x400);
            cLib_chaseF(&speed.y, field_0x6c8 * cM_ssin(current.angle.x), 1.0f);

            if (mMode == 4) {
                cLib_chaseF(&field_0x6c8, l_HIO.fly_movement_speed, 1.0f);
            } else {
                cLib_chaseF(&field_0x6c8, 0.0f, 1.0f);
            }

            setBodyAngle();

            if (mMode == 4) {
                if (field_0x6a8.absXZ(current.pos) < 200.0f) {
                    mMode = 5;
                }
            } else if (!field_0x6c8) {
                if (field_0x6f0 < 2) {
                    mMode = 3;
                } else if (field_0x6fc == 0) {
                    camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye);
                    camera->mCamera.Start();
                    camera->mCamera.SetTrimSize(0);
                    dComIfGp_event_reset();
                    attention_info.flags &= ~fopAc_AttnFlag_UNK_0x400000;
                    setActionMode(1, 1);
                    field_0x764 = 1;
                }
            }
            break;
    }

    camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
}

void daE_YMB_c::action() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    field_0x1324.OffAtSetBit();
    field_0x82c[0].OffAtSetBit();
    field_0x82c[1].OffAtSetBit();
    field_0x82c[0].OnCoSetBit();
    field_0x82c[1].OnCoSetBit();

    if ((field_0x712 != 0 && player->getCutType() == daPy_py_c::CUT_TYPE_WOLF_JUMP) || !field_0x6d8) {
        field_0x82c[0].OffTgShield();
        field_0x82c[1].OffTgShield();
        field_0xa9c.OffTgShield();
    } else {
        field_0x82c[0].OnTgShield();
        field_0x82c[1].OnTgShield();
        field_0xa9c.OnTgShield();
    }

    checkWaterPos();
    checkGroundPos();
    damage_check();
    setCameraSwitch();

    if (field_0x716 == 0) {
        if (player->checkWolfLock(this)) {
            player->cancelWolfLock(this);
        }

        onWolfNoLock();
    } else {
        offWolfNoLock();
    }

    if (mAction != 5) {
        attention_info.distances[2] = 24;
    } else {
        attention_info.distances[2] = 25;
    }

    field_0x712 = 0;
    field_0x71f = 0;

    switch (mAction) {
        case ACTION_WAIT:
            executeWait();
            break;

        case ACTION_FLY:
            executeFly();
            break;

        case ACTION_FLY_ATTACK:
            executeFlyAttack();
            break;

        case ACTION_RUN_AWAY:
            executeRunAway();
            break;

        case ACTION_GUARD:
            executeGuard();
            break;

        case ACTION_SWIM:
            executeSwim();
            break;

        case ACTION_WATER_JUMP:
            executeWaterJump();
            break;

        case ACTION_DAMAGE:
            executeDamage();
            break;

        case ACTION_DOWN:
            executeDown();
            break;

        case ACTION_DEATH:
            executeDeath();
            break;

        case ACTION_LAKE_DEMO:
            executeLakeDemo();
            break;

        case ACTION_START_DEMO:
            executeStartDemo();
            break;

        case ACTION_BATTLE_DEMO:
            executeBattleDemo();
            break;
    }

    mSound.setLinkSearch(false);
    fopAcM_posMoveF(this, NULL);

    if (mAction != 10 || mMode != 0) {
        if (player->current.pos.absXZ(field_0x69c) < 3000.0f) {
            player->onBossRoomWait();
        }

        if (mAction != 9) {
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_NEAR, 0, -1);
        }

        if (field_0x765 != 0) {
            Z2GetAudioMgr()->changeSubBgmStatus(3);
        }

        f32 fVar1 = mpModelMorf->getModel()->getAnmMtx(15)[1][3] - field_0x6cc;
        if (fVar1 > 0.0f) {
            cLib_chaseF(&field_0x6c4, 255.0f, 10.0f);
            field_0x6c1 = 0;
        } else {
            fVar1 = fVar1 / 40.0f + 50.0f;

            if (fVar1 < 10.0f) {
                fVar1 = 10.0f;
            }

            cLib_chaseF(&field_0x6c4, fVar1, 10.0f);
            field_0x6c1 = 1;
        }

        setFlyBlurEffect();
    }

    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpBrkAnm->play();
}

void daE_YMB_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    mDoMtx_stack_c::transM(0.0f, 0.0f, -250.0f);
    mDoMtx_stack_c::XrotM(field_0x6ea);
    mDoMtx_stack_c::transM(0.0f, 0.0f, 250.0f);
    mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(1));
    mDoMtx_stack_c::ZXYrotM(0, -0x4000, 0);
    MTXCopy(mDoMtx_stack_c::get(), field_0x768);

    if (mpBgW != NULL) {
        mpBgW->Move();
    }
}

void daE_YMB_c::getBellyBitePos(cXyz* i_pos) {
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(16));
    mDoMtx_stack_c::transM(80.0f, -120.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(i_pos);
}

int daE_YMB_c::getDownLockPoint() {
    if (field_0x6f4 >= 7) {
        return 1;
    }

    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp54;
    int uVar1 = field_0x6f4 - 1;
    int var_r26 = 0;
    f32 fVar1 = 3000.0f;
    s16 sVar1 = 0x4000;
    f32 fVar2;

    if (player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_LOCK && field_0x718 == 0) {
        for (int i = 0; i < 6; i++) {
            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(YMB_DOWN_ATT_JNT[i]));
            mDoMtx_stack_c::multVecZero(&sp54);
            fVar2 = sp54.abs(player->current.pos);

            if (fVar2 < fVar1 || fVar2 < 300.0f) {
                fVar1 = fVar2;
                if (fVar1 < 500.0f) {
                    fVar1 = 500.0f;
                    s16 spd_0x8 = abs((s16)(cLib_targetAngleY(&player->current.pos, &sp54) - player->shape_angle.y));
                    if (spd_0x8 >= sVar1) {
                        continue;
                    }

                    sVar1 = spd_0x8;
                    var_r26 = i;
                } else {
                    var_r26 = i;
                }
            }
        }

        if (dComIfGp_getAttention()->LockonTruth()) {
            if (dComIfGp_getAttention()->LockonTarget(0) == this) {
                var_r26 = field_0x725;
            }
        }

        field_0x725 = var_r26;
        uVar1 = var_r26;
    }

    if (uVar1 >= 6) {
        uVar1 = 5;
    }

    return YMB_DOWN_ATT_JNT[uVar1];
}

void daE_YMB_c::setAttentionPos() {
    cXyz sp2c;
    cXyz sp38(daPy_getPlayerActorClass()->current.pos);
    J3DModel* model = mpModelMorf->getModel();

    if (getMaxLockAttack() <= field_0x6f4) {
        attention_info.flags = 0;
    }

    if (field_0x6f4 == 0) {
        mDoMtx_stack_c::copy(model->getAnmMtx(1));
        
        if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_WOLF_JUMP || mAction == 7) {
            mDoMtx_stack_c::transM(200.0f, 50.0f, 0.0f);
        } else {
            mDoMtx_stack_c::transM(100.0f, 50.0f, 0.0f);
        }

        mDoMtx_stack_c::multVecZero(&attention_info.position);
        attention_info.position.y += 50.0f;
        field_0x720 = 16;
        getBellyBitePos(&sp2c);
        field_0xa9c.SetC(sp2c);
        field_0xa9c.SetR(100.0f);
        dComIfG_Ccsp()->Set(&field_0xa9c);
        eyePos = sp2c;
    } else {
        if (field_0x6f4 >= 7) {
            mDoMtx_stack_c::copy(model->getAnmMtx(1));
        } else {
            mDoMtx_stack_c::copy(model->getAnmMtx(getDownLockPoint()));
        }

        mDoMtx_stack_c::multVecZero(&eyePos);
        attention_info.position = eyePos;
        attention_info.position.y += 50.0f;

        for (int i = 0; i < 6; i++) {
            mDoMtx_stack_c::copy(model->getAnmMtx(i * 2 + 20));
            mDoMtx_stack_c::multVecZero(&sp2c);
            field_0xbd4[i].SetC(sp2c);
            field_0xbd4[i].SetR(50.0f);
            dComIfG_Ccsp()->Set(&field_0xbd4[i]);
        }
    }

    setBitePos(16);
}

void daE_YMB_c::cc_set() {
    cXyz sp28;
    J3DModel* model = mpModelMorf->getModel();
    
    setAttentionPos();
    mDoMtx_stack_c::copy(model->getAnmMtx(1));
    mDoMtx_stack_c::multVecZero(&sp28);
    field_0x82c[0].SetC(sp28);
    field_0x82c[0].SetR(l_HIO.model_size * 180.0f);
    dComIfG_Ccsp()->Set(&field_0x82c[0]);

    mDoMtx_stack_c::copy(model->getAnmMtx(15));
    mDoMtx_stack_c::multVecZero(&sp28);
    field_0x82c[1].SetC(sp28);
    field_0x82c[1].SetR(l_HIO.model_size * 240.0f);
    dComIfG_Ccsp()->Set(&field_0x82c[1]);

    field_0x1324.SetC(sp28);
    field_0x1324.SetR(l_HIO.model_size * 360.0f);
    dComIfG_Ccsp()->Set(&field_0x1324);
}

int daE_YMB_c::execute() {
    BOOL bVar1 = FALSE;
    if (mAction == 9 || ((mAction == 8 && mMode >= 3) || daPy_py_c::checkNowWolfEyeUp())) {
        bVar1 = TRUE;
    }

    if (bVar1) {
        cLib_chaseF(&field_0x6d8, 255.0f, 8.0f);
        cLib_chaseF(&field_0x6dc, 150.0f, 6.0f);
    } else {
        cLib_chaseF(&field_0x6d8, 0.0f, 20.0f);
        cLib_chaseF(&field_0x6dc, 0.0f, 15.0f);
    }

    if (dComIfGp_event_runCheck() && !eventInfo.checkCommandDemoAccrpt()) {
        return 1;
    }

    if (field_0x6fc != 0) {
        field_0x6fc--;
    }

    if (field_0x700 != 0) {
        field_0x700--;
    }

    if (field_0x6f8 != 0) {
        field_0x6f8--;
    }

    if (field_0x704 != 0) {
        field_0x704--;
    }

    if (field_0x708 != 0) {
        field_0x708--;
    }

    if (field_0x70c != 0) {
        field_0x70c--;
    }

    if (field_0x723 != 0) {
        field_0x723--;
        if (field_0x723 == 0) {
            field_0x722 = 0;
        }
    }

    for (int i = 0; i < 6; i++) {
        if (field_0x680[i] != 0) {
            field_0x680[i]--;
            cLib_addCalc(&field_0x668[i], 1.0f, 0.5f, 0.3f, 0.01f);
        } else {
            cLib_addCalc(&field_0x668[i], 0.0f, 0.5f, 0.3f, 0.01f);
        }
    }

    if (field_0x698 != 0) {
        field_0x698--;
    }

    if (field_0x724 != 0) {
        field_0x724--;
    }

    if (field_0x764 == 0 || !field_0x6d8 || mAction == 9) {
        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
    } else {
        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
    }

    action();
    mtx_set();
    cc_set();

    return 1;
}

static int daE_YMB_Execute(daE_YMB_c* i_this) {
    return i_this->execute();
}

static int daE_YMB_IsDelete(daE_YMB_c* i_this) {
    return 1;
}

int daE_YMB_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_YB");

    if (field_0x1520 != 0) {
        l_initHIO = 0;
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    if (mpBgW != NULL) {
        dComIfG_Bgsp().Release(mpBgW);
    }

    return 1;
}

static int daE_YMB_Delete(daE_YMB_c* i_this) {
    return i_this->_delete();
}

int daE_YMB_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_YB", BMDR_YB_TW);
    JUT_ASSERT(4196, modelData != NULL);
    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_YB", BCK_YB_FLY_WAIT),
                                  2, 1.0f, 0, -1, &mSound, 0x80000, 0x31000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    if (mInvisModel.create(mpModelMorf->getModel(), 1) == 0) {
        return 0;
    }

    J3DModel* model = mpModelMorf->getModel();
    model->setUserArea((uintptr_t)this);
    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
    }

    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }

    if (mpBrkAnm->init(mpModelMorf->getModel()->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_YB", BRK_YB_WAIT), 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    mpBgW = new dBgW();
    if (mpBgW == NULL) {
        return 0;
    }

    if (mpBgW->Set((cBgD_t*)dComIfG_getObjectRes("E_YB", DZB_YB), 1, &field_0x768) == 1) {
        return 0;
    }

    mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
    
    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    daE_YMB_c* i_this = (daE_YMB_c*)a_this;
    return i_this->CreateHeap();
}

cPhs__Step daE_YMB_c::create() {
    fopAcM_ct(this, daE_YMB_c);

    mSwitchBit = fopAcM_GetParam(this);
    if (mSwitchBit != 0xFF) {
        if (dComIfGs_isSwitch(mSwitchBit, fopAcM_GetRoomNo(this))) {
            // E_YMB I'm not going to reset it because I've been defeated\n
            OS_REPORT("E_YMB やられ後なので再セットしません\n");
            return cPhs_ERROR_e;
        }
    }

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, "E_YB");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_YMB PARAM %x\n", fopAcM_GetParam(this));
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x5200)) {
            return cPhs_ERROR_e;
        }

        if (mpBgW != NULL) {
            if (dComIfG_Bgsp().Regist(mpBgW, this)) {
                return cPhs_ERROR_e;
            }
        }

        mpBgW->SetRideCallback(rideCallBack);

        if (l_initHIO == 0) {
            l_initHIO = 1;
            field_0x1520 = 1;
            l_HIO.field_0x4 = -1;
        }

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        attention_info.field_0xa = 0x50;
        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -400.0f, -400.0f, -400.0f);
        fopAcM_SetMax(this, 400.0f, 400.0f, 400.0f);
        health = 100;
        field_0x560 = 100;

        mStts.Init(0xFF, 0, this);
        field_0x82c[0].Set(cc_ymb_src);
        field_0x82c[0].SetStts(&mStts);
        field_0x82c[1].Set(cc_ymb_src);
        field_0x82c[1].SetStts(&mStts);
        field_0x1324.Set(cc_ymb_at_src);
        field_0x1324.SetStts(&mStts);
        field_0xa9c.Set(cc_ymb_eye_src);
        field_0xa9c.SetStts(&mStts);

        for (int i = 0; i < 6; i++) {
            field_0xbd4[i].Set(cc_ymb_tube_src);
            field_0xbd4[i].SetStts(&mStts);
        }

        mSound.init(&current.pos, &eyePos, 5, 1);
        mSound.setEnemyName("E_ymb");
        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;
        gravity = 0.0f;
        field_0x69c = home.pos;
        field_0x717 = 1;
        mAction = 10;

        if (mpBgW != NULL) {
            dComIfG_Bgsp().Release(mpBgW);
        }

        daE_YMB_Execute(this);
    }

    return phase;
}

static int daE_YMB_Create(daE_YMB_c* i_this) {
    return i_this->create();
}

AUDIO_INSTANCES;

static actor_method_class l_daE_YMB_Method = {
    (process_method_func)daE_YMB_Create,
    (process_method_func)daE_YMB_Delete,
    (process_method_func)daE_YMB_Execute,
    (process_method_func)daE_YMB_IsDelete,
    (process_method_func)daE_YMB_Draw,
};

extern actor_process_profile_definition g_profile_E_YMB = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_YMB,             // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_YMB_c),      // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  196,                    // mPriority
  &l_daE_YMB_Method,      // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
