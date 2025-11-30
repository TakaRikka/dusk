/**
 * @file d_a_b_yo.cpp
 * @brief Actor - Blizzeta (Boss)
 * 
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_yo.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "m_Do/m_Do_graphic.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_msg.h"
#include "f_op/f_op_msg_mng.h"
#include "c/c_damagereaction.h"
#include "d/actor/d_a_b_yo_ice.h"
#include "d/actor/d_a_e_fz.h"
#include "d/actor/d_a_mirror.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_obj_ystone.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "Z2AudioLib/Z2Instances.h"

class daB_YO_HIO_c {
public:
    daB_YO_HIO_c();
    virtual ~daB_YO_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mScale[8];
    /* 0x28 */ f32 mFreezardNum;
    /* 0x2C */ f32 mMaxFallSpeed;
    /* 0x30 */ f32 mReboundSpeedRatio;
    /* 0x34 */ f32 mChaseSpeed;
    /* 0x38 */ f32 mReboundSpeedMax;
    /* 0x3C */ bool field_0x3c;
    /* 0x40 */ f32 mGatherTimer;
    /* 0x44 */ f32 mAttackTimer;
    /* 0x48 */ f32 mFreezardRadius;
    /* 0x4C */ f32 field_0x4c;
    /* 0x50 */ f32 field_0x50;
    /* 0x54 */ bool field_0x54;
    /* 0x58 */ f32 field_0x58;
};

STATIC_ASSERT(sizeof(daB_YO_HIO_c) == 0x5C);

enum B_YO_RES_FILE_ID {
    /* BCK */
    /* 0x07 */ BCK_YKW_B_ANGRY = 7,
    /* 0x08 */ BCK_YKW_B_ANGRY_WAIT,
    /* 0x09 */ BCK_YKW_B_DOWN,
    /* 0x0A */ BCK_YKW_B_DOWNA,
    /* 0x0B */ BCK_YKW_B_DOWNB,
    /* 0x0C */ BCK_YKW_B_DOWNC,
    /* 0x0D */ BCK_YKW_B_FLOAT,
    /* 0x0E */ BCK_YKW_B_FLOATB,
    /* 0x0F */ BCK_YKW_B_SCREAM,
    /* 0x10 */ BCK_YKW_B_SCREAM_WAIT,
    /* 0x11 */ BCK_YKW_B_TALKA,
    /* 0x12 */ BCK_YKW_B_TALKB,
    /* 0x13 */ BCK_YKW_B_TO_TALKB,
    /* 0x14 */ BCK_YKW_B_WAITA,
    /* 0x15 */ BCK_YKW_B_WAITB,
    /* 0x16 */ BCK_YKW_B_WAITC,
    /* 0x17 */ BCK_YKW_B_WAITC2,
    /* 0x18 */ BCK_YKW_B_WAITD,
    /* 0x19 */ BCK_YKW_B_WALK,

    /* BMDE */
    /* 0x1C */ BMDE_YKW_B = 0x1C,
    /* 0x1D */ BMDE_YO01,
    /* 0x1E */ BMDE_YO02,
    /* 0x1F */ BMDE_YO_CORE,
    /* 0x20 */ BMDE_YO_HIBI,
    /* 0x21 */ BMDE_YO_ICE,

    /* BRK */
    /* 0x24 */ BRK_YKW_B = 0x24,
    /* 0x25 */ BRK_YKW_B_ANGRY,
    /* 0x26 */ BRK_YKW_B_ANGRY_WAIT,
    /* 0x27 */ BRK_YKW_B_WHITE,

    /* BTK */
    /* 0x2A */ BTK_YKW_B_ANGRY = 0x2A,
    /* 0x2B */ BTK_YKW_B_DOWN,
    /* 0x2C */ BTK_YKW_B_FLOAT,
    /* 0x2D */ BTK_YKW_B_SCREAM,
    /* 0x2E */ BTK_YKW_B_SCREAM_WAIT,
    /* 0x2F */ BTK_YKW_B_TALKA,
    /* 0x30 */ BTK_YKW_B_TALKB,
    /* 0x31 */ BTK_YKW_B_TO_TALKB,
    /* 0x32 */ BTK_YKW_B_WAITA,
    /* 0x33 */ BTK_YKW_B_WAITB,
    /* 0x34 */ BTK_YKW_B_WALK,
    /* 0x35 */ BTK_YO_KANNKYOU,

    /* BTP */
    /* 0x38 */ BTP_YKW_B_ANGRY = 0x38,
    /* 0x39 */ BTP_YKW_B_FACEA,
    /* 0x3A */ BTP_YKW_B_FACEB,
    /* 0x3B */ BTP_YKW_B_FLOAT,
    /* 0x3C */ BTP_YKW_B_TALKA,
    /* 0x3D */ BTP_YKW_B_TALKB,
    /* 0x3E */ BTP_YKW_B_TO_TALKB,
    /* 0x3F */ BTP_YKW_B_WAITA,
    /* 0x40 */ BTP_YKW_B_WAITB,
    /* 0x41 */ BTP_YKW_B_WALK,
};

enum L5_R50_RES_FIL_ID {
    /* BMDR */
    /* 0x04 */ BMDR_R50_P1 = 4,
    /* 0x05 */ BMDR_R50_P2,
    /* 0x06 */ BMDR_T_R50FURNITURE,

    /* DZB */
    /* 0x09 */ DZB_R50_P1 = 9,
    /* 0x0A */ DZB_R50_P2,
};

/* 80639F74 0001+00 data_80639F74 @1009 */
static bool hioInit;

static daB_YO_HIO_c l_HIO;

namespace {

static dCcD_SrcCyl cc_yo_src = {
    {
        {0x0, {{0x100000, 0x4, 0xD}, {0x0, 0x0}, 0x75}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        40.0f, // mRadius
        40.0f // mHeight
    } // mCyl
};

static dCcD_SrcSph cc_yo_tg_src = {
    {
        {0x0, {{0x100, 0x4, 0x11}, {0xD8BBFDFF, 0x43}, 0x45}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x203}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcSph cc_yo_iron_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x400000, 0x3}, 0x0}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x102}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

}  // namespace

static bool yo_material_draw[27] = {
    true, true, true, true, false, false, true, true, true,
    false, true, true, true, true, false, false, true, true,
    false, false, true, true, false, true, false, false, true,
};

static int yo_demo_bck_idx[19] = {
    BCK_YKW_B_ANGRY,
    BCK_YKW_B_ANGRY_WAIT,
    BCK_YKW_B_FLOAT,
    BCK_YKW_B_TALKA,
    BCK_YKW_B_TALKB,
    BCK_YKW_B_TO_TALKB,
    BCK_YKW_B_WAITA,
    BCK_YKW_B_WAITB,
    BCK_YKW_B_WALK,
    BCK_YKW_B_SCREAM,
    BCK_YKW_B_SCREAM_WAIT,
    BCK_YKW_B_WAITC2,
    BCK_YKW_B_WAITC,
    BCK_YKW_B_FLOATB,
    BCK_YKW_B_WAITD,
    BCK_YKW_B_DOWNA,
    BCK_YKW_B_DOWNB,
    BCK_YKW_B_DOWNC,
    BCK_YKW_B_DOWN,
};

static int yo_demo_btk_idx[19] = {
    BTK_YKW_B_ANGRY,
    0xFF,  // <none>
    BTK_YKW_B_FLOAT,
    BTK_YKW_B_TALKA,
    BTK_YKW_B_TALKB,
    BTK_YKW_B_TO_TALKB,
    BTK_YKW_B_WAITA,
    BTK_YKW_B_WAITB,
    BTK_YKW_B_WALK,
    BTK_YKW_B_SCREAM,
    BTK_YKW_B_SCREAM_WAIT,
    BTK_YKW_B_WALK,
    BTK_YKW_B_WALK,
    BTK_YKW_B_WALK,
    BTK_YKW_B_WALK,
    BTK_YKW_B_DOWN,
    BTK_YKW_B_DOWN,
    BTK_YKW_B_DOWN,
    BTK_YKW_B_WAITA,
};

static int yo_demo_btp_idx[19] = {
    BTP_YKW_B_ANGRY,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_TALKA,
    BTP_YKW_B_TALKB,
    BTP_YKW_B_TO_TALKB,
    BTP_YKW_B_FACEB,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEB,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEA,
    BTP_YKW_B_FACEB,
};

daB_YO_HIO_c::daB_YO_HIO_c() {
    field_0x4 = -1;
    mScale[0] = 7.0f;
    mScale[1] = 6.0f;
    mScale[2] = 5.0f;
    mScale[3] = 4.0f;
    mScale[4] = 3.0f;
    mScale[5] = 2.0f;
    mScale[6] = 1.5f;
    mScale[7] = 1.0f;
    mFreezardNum = 20.0f;
    mReboundSpeedRatio = 2.5f;
    mChaseSpeed = 6.0f;
    mReboundSpeedMax = 7.0f;
    field_0x3c = false;
    mMaxFallSpeed = -100.0f;
    mAttackTimer = 60.0f;
    mFreezardRadius = 0.0f;
    mGatherTimer = 0.0f;
    field_0x4c = 3000.0f;
    field_0x50 = 5.0f;
    field_0x54 = false;
    field_0x58 = 5.0f;
}

int daB_YO_c::draw() {
    cXyz pos(0.0f, 0.0f, 0.0f);
    g_env_light.settingTevStruct(0x10, &pos, &tevStr);
    dComIfGd_setListBG();
    J3DModel* model;

    for (int i = 0; i < 2; i++) {
        if (mRoomAlpha[i] == 0.0f) {
            continue;
        }

        model = mpRoomModel[i];
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);
        J3DModelData* model_data = model->getModelData();

        for (u16 j = 0; j < model_data->getMaterialNum(); j++) {
            J3DMaterial* material = model_data->getMaterialNodePointer(j);
            material->getTevColor(0)->a = (u8)mRoomAlpha[i];

            if (i == 0) {
                if (mRoomAlpha[i] == 255.0f) {
                    material->setMaterialMode(1);
                    material->setZCompLoc(1);
                    material->getZMode()->setUpdateEnable(1);
                    material->getZMode()->setFunc(3);
                    material->getZMode()->setCompareEnable(1);
                    material->getBlend()->setType(0);
                } else {
                    material->setMaterialMode(4);
                    material->setZCompLoc(1);
                    material->getZMode()->setUpdateEnable(0);
                    material->getZMode()->setFunc(3);
                    material->getZMode()->setCompareEnable(1);
                    material->getBlend()->setType(1);
                }
            }
        }

        mDoExt_modelUpdateDL(model);
    }

    if (mpFurnitureModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpFurnitureModel, &tevStr);
        mDoExt_modelUpdateDL(mpFurnitureModel);
    }

    dComIfGd_setList();

    if (mIsInactive == 1) {
        return 1;
    }
    
    if (field_0xfb5 == 2) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);

    int phase;
    if (mModelNo == 7) {
        phase = 2;
    } else if (mModelNo < 4) {
        phase = 0;
    } else {
        phase = 1;
    }

    if (field_0xfb5 != 3) {
        J3DModel* model = mpModel[phase];
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);
        if (mModelNo != 7) {
            mpBtkAnm->entry(model->getModelData());
        }

        if (phase == 2) {
            J3DModelData* model_data = model->getModelData();
            int iVar3 = mLastPhaseDamage;
            if (iVar3 > 2) {
                iVar3 = 2;
            }

            for (u16 j = 0; j < model_data->getMaterialNum(); j++) {
                J3DMaterial* material = model_data->getMaterialNodePointer(j);
                if (yo_material_draw[iVar3 * 9 + j]) {
                    material->getShape()->show();
                } else {
                    material->getShape()->hide();
                }
            }
        }

        fopAcM_setEffectMtx(this, model->getModelData());
        mDoExt_modelUpdateDL(model);
        daMirror_c::entry(model);
    }

    if (phase == 2 || field_0xfb5 == 3) {
        J3DModel* model;
        if (!mReverted) {
            model = mpYetaMorf->getModel();
            g_env_light.setLightTevColorType_MAJI(model, &tevStr);
            mpYetaBtpAnm->entry(model->getModelData());
            mpYetaBtkAnm->entry(model->getModelData());
            mpYetaBrkAnm->entry(model->getModelData());
            if (field_0xfd1) {
                mpYetaWhiteBrkAnm->entry(model->getModelData());
            }
            fopAcM_setEffectMtx(this, model->getModelData());
            mpYetaMorf->entryDL();
            daMirror_c::entry(model);
        } else {
            model = mpYetaRevertedMorf->getModel();
            g_env_light.setLightTevColorType_MAJI(model, &tevStr);
            fopAcM_setEffectMtx(this, model->getModelData());
            mpYetaRevertedMorf->entryDL();
        }

        cXyz pos;
        pos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &pos, 3500.0f, 0.0f, current.pos.y,
                                        mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f,
                                        dDlst_shadowControl_c::getSimpleTex());
    }

    if (mBlureRate != 0.0f) {
        mDoGph_gInf_c::setBlureRate(mBlureRate);
        mDoGph_gInf_c::onBlure();
    } else {
        mDoGph_gInf_c::offBlure();
    }

    return 1;
}

static int daB_YO_Draw(daB_YO_c* i_this) {
    return i_this->draw();
}

void daB_YO_c::onIceBreak(u16 param_0) {
    mIceBreak |= (1 << param_0);
}

void daB_YO_c::setBck(int i_anm, u8 i_attr, f32 i_morf, f32 i_rate) {
    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes("B_YO", yo_demo_bck_idx[i_anm]);
    mpYetaMorf->setAnm(bck, i_attr, i_morf, i_rate, 0.0f, -1.0f);

    J3DModelData* model_data = mpYetaMorf->getModel()->getModelData();
    mpYetaBtkAnm->remove(model_data);
    int btk_idx = yo_demo_btk_idx[i_anm];
    if (btk_idx != 0xff) {
        field_0xfcd = true;
        J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_YO", btk_idx);
        mpYetaBtkAnm->init(model_data, btk, TRUE, i_attr, i_rate, 0, -1);
    } else {
        field_0xfcd = false;
    }

    mpYetaBtpAnm->remove(model_data);
    J3DAnmTexPattern* btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("B_YO", yo_demo_btp_idx[i_anm]);
    mpYetaBtpAnm->init(model_data, btp, TRUE, i_attr, i_rate, 0, -1);

    int brk_idx;
    if (i_anm == ANM_ANGRY) {
        field_0xfcc = true;
        brk_idx = BRK_YKW_B_ANGRY;
    } else if (i_anm == ANM_ANGRY_WAIT) {
        brk_idx = BRK_YKW_B_ANGRY_WAIT;
    } else if (field_0xfcc) {
        brk_idx = BRK_YKW_B;
    } else {
        brk_idx = BRK_YKW_B_ANGRY;
    }
    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_YO", brk_idx);
    mpYetaBrkAnm->init(model_data, brk, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);
}

void daB_YO_c::setActionMode(int i_action, int i_mode) {
    mAction = i_action;
    mMode = i_mode;
}

void daB_YO_c::damage_check() {
    mCcStts.Move();
    mCcIronStts.Move();

    if (mAction == ACT_CHASE && mAtCyl.ChkAtHit()
        && fopAcM_GetName(mAtCyl.GetAtHitObj()->GetAc()) == PROC_ALINK)
    {
        reflectFreeMove();
        mActionTimer2 = 90;
    }

    if (mIFrameTimer == 0) {
        mAtInfo.mpCollider = NULL;
        if (mTgSph[0].ChkTgHit()) {
            mAtInfo.mpCollider = mTgSph[0].GetTgHitObj();
        } else if (mTgSph[1].ChkTgHit()) {
            mAtInfo.mpCollider = mTgSph[1].GetTgHitObj();
        } else if (mTgSph[2].ChkTgHit()) {
            mAtInfo.mpCollider = mTgSph[2].GetTgHitObj();
        }

        if (mAtInfo.mpCollider != NULL) {
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                mIFrameTimer = 20;
            } else {
                mIFrameTimer = 10;
            }

            if (mAtInfo.mAttackPower <= 1) {
                mIFrameTimer = 10;
            }

            if (mAction == ACT_CHASE) {
                def_se_set(&mCreatureSound, mAtInfo.mpCollider, 0x2A, this);
            }
        }
    }

    if (mIFrameIronTimer == 0) {
        mAtInfo.mpCollider = NULL;
        if (mTgIronSph[0].ChkTgHit()) {
            mAtInfo.mpCollider = mTgIronSph[0].GetTgHitObj();
        } else if (mTgIronSph[1].ChkTgHit()) {
            mAtInfo.mpCollider = mTgIronSph[1].GetTgHitObj();
        } else if (mTgIronSph[2].ChkTgHit()) {
            mAtInfo.mpCollider = mTgIronSph[2].GetTgHitObj();
        }

        if (mAtInfo.mpCollider != NULL) {
            cc_at_check(this, &mAtInfo);
            health = 100;
            mIFrameIronTimer = 20;

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL)) {
                if (mAction == ACT_CHASE) {
                    mChillKey = 0;
                    if (mTgIronSph[0].ChkTgShield()) {
                        dScnPly_c::setPauseTimer(0);
                    } else if (mModelNo >= 6) {
                        setActionMode(ACT_SERIOUS_DEMO, 0);
                    } else {
                        dComIfGs_onOneZoneSwitch(6, fopAcM_GetRoomNo(this));
                        setActionMode(ACT_CHASE, 1);
                    }
                } else if (!mTgIronSph[0].ChkTgShield()) {
                    dComIfGs_onOneZoneSwitch(8, fopAcM_GetRoomNo(this));
                    mLastPhaseDamage++;
                    if (mLastPhaseDamage >= 3) {
                        mLastPhaseDamage = 3;
                        setIcicleOperate(10);
                        setActionMode(ACT_DEATH, 0);
                    } else {
                        setIcicleOperate(9);
                        setActionMode(ACT_DAMAGE, 0);
                    }
                }
            }
        }
    }
}

void daB_YO_c::setBreakFrizad() {
    cXyz pos;
    csXyz angle;
    if (mModelNo >= 3) {
        angle.set(0, shape_angle.y + cM_rndFX(4000.0f), 0);
        pos.set(current.pos.x + cM_rndF(50.0f), current.pos.y + (cM_rndF(100.0f) + 100.0f),
                current.pos.z + cM_rndF(50.0f));
        fopAcM_createChild(PROC_E_FZ, fopAcM_GetID(this), mFreezardNo << 8 | 3, &pos,
                           fopAcM_GetRoomNo(this), &angle, NULL, -1, NULL);
    }
}

void daB_YO_c::setIcicleOperate(int i_operate) {
    for (int i = 0; i < 10; i++) {
        if (mIcicleID[i] != -1) {
            daB_YOI_c* icicle;
            fopAcM_SearchByID(mIcicleID[i], (fopAc_ac_c**)&icicle);
            if (icicle != NULL) {
                icicle->setOperate(i_operate);
            } else {
                mIcicleID[i] = -1;
            }
        }
    }
}

void daB_YO_c::setIcicleSubNumber() {
    int sub_number = 0;
    for (int i = 0; i < 10; i++) {
        if (mIcicleID[i] != -1) {
            daB_YOI_c* icicle;
            fopAcM_SearchByID(mIcicleID[i], (fopAc_ac_c**)&icicle);
            if (icicle != NULL) {
                icicle->setSubNumber(sub_number);
                sub_number++;
            }
        }
    }
}

void daB_YO_c::setWindowBreakEffect(int param_0) {
    static s16 yo_window_angle[6] = {0xC000, 0xE000, 0x6000, 0x2000, 0x4000, 0x0000};
    static u16 madoware_effect_id[4] = {0x87C1, 0x87C2, 0x87C3, 0x87C4};
    csXyz angle(0, yo_window_angle[param_0], 0);
    cXyz pos(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; i++) {
        dComIfGp_particle_set(madoware_effect_id[i], &pos, &tevStr, &angle, NULL);
    }
}

void daB_YO_c::setHensinEffect() {
    static u16 hensin_effect_id[4] = {0x87B1, 0x87B2, 0x87B3, 0x87B4};
    cXyz scale(mHensinScale, mHensinScale, mHensinScale);
    cXyz pos;
    mDoMtx_stack_c::copy(mpYetaMorf->getModel()->getAnmMtx(2));
    mDoMtx_stack_c::multVecZero(&pos);
    pos.z += (mHensinScale - 1.0f) * 10.0f;
    for (int i = 0; i < 4; i++) {
        mHensinKey[i] = dComIfGp_particle_set(mHensinKey[i], hensin_effect_id[i], &pos, &tevStr,
                                              &shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mHensinKey[i]);
        if (emitter != NULL) {
            scale.set(mHensinScale, mHensinScale, mHensinScale);
            if (i == 0) {
                scale = scale * 2.0f;
            }
            emitter->setGlobalScale(JGeometry::TVec3<f32>(scale));
        }
    }
}

void daB_YO_c::setLandingEffect(f32 i_scale) {
    static u16 l_landing_effect_id[6] = {0x86DC, 0x86DD, 0x86DE, 0x86DF, 0x86E0, 0x86E1};
    cXyz pos = current.pos;
    if (mModelNo == 7) {
        pos.y -= 230.0f;
    }
    cXyz scale(i_scale, i_scale, i_scale);
    for (int i = 0; i < 6; i++) {
        dComIfGp_particle_set(l_landing_effect_id[i], &pos, &tevStr, &shape_angle, &scale);
    }
    pos.set(0.0f, 0.0f, 0.0f);
    dComIfGp_particle_set(0x86E2, &pos, &tevStr, &shape_angle, NULL);
}

void daB_YO_c::setBreakIceEffect() {
    static u16 break_ice_effect_id[12] = {
        0x86B4, 0x86B5, 0x879C,
        0x879F, 0x87A0, 0x87A1,
        0x87A3, 0x87A4, 0x87A5,
        0x87A7, 0x87A8, 0x87A9,
    };
    cXyz scale;
    cXyz pos = current.pos;
    int idx;

    if (mModelNo == 7) {
        pos.y -= 230.0f;
        scale.set(1.0f, 1.0f, 1.0f);
        idx = mLastPhaseDamage * 3;
        mCreatureSound.startCreatureSound(Z2SE_EN_YO_BREAK3, 0, -1);
    } else {
        scale.set(l_HIO.mScale[mModelNo], l_HIO.mScale[mModelNo], l_HIO.mScale[mModelNo]);
        idx = 0;
        if (mModelNo < 4) {
            mCreatureSound.startCreatureSound(Z2SE_EN_YO_BREAK1, 0, -1);
        } else {
            mCreatureSound.startCreatureSound(Z2SE_EN_YO_BREAK2, 0, -1);
        }
    }

    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(break_ice_effect_id[idx + i], &pos, &tevStr, &shape_angle, &scale);
    }
}

void daB_YO_c::setWallHitEffect() {
    static u16 wall_hit_effect_id[6] = {
        0x879D, 0x879E,
        0x879D, 0x879E,
        0x87AA, 0x87AB,
    };
    cXyz scale;
    cXyz pos = current.pos;

    if (mModelNo == 7) {
        pos.y -= 230.0f;
        scale.set(1.0f, 1.0f, 1.0f);
        int idx = (mLastPhaseDamage - 1) * 2;
        mCreatureSound.startCreatureSound(Z2SE_EN_YO_HIT_WALL, 0, -1);
        for (int i = 0; i < 2; i++) {
            dComIfGp_particle_set(wall_hit_effect_id[idx + i], &pos, &tevStr, &shape_angle, &scale);
        }
    } else {
        scale.set(l_HIO.mScale[mModelNo], l_HIO.mScale[mModelNo], l_HIO.mScale[mModelNo]);
        if (mModelNo < 4) {
            mCreatureSound.startCreatureSound(Z2SE_EN_YO_BOUND1, 0, -1);
        } else {
            mCreatureSound.startCreatureSound(Z2SE_EN_YO_BOUND2, 0, -1);
        }
        dComIfGp_particle_set(0x87BF, &pos, &tevStr, &shape_angle, &scale);
    }
}

void daB_YO_c::setChillEffect() {
    static u16 chill_effect_id[4] = {0x86DA, 0x87A2, 0x87A6, 0x87AC};
    cXyz scale;
    cXyz pos = current.pos;
    int idx;

    if (mModelNo == 7) {
        pos.y -= 230.0f;
        scale.set(1.0f, 1.0f, 1.0f);
        idx = mLastPhaseDamage + 1;
    } else {
        scale.set(l_HIO.mScale[mModelNo], l_HIO.mScale[mModelNo], l_HIO.mScale[mModelNo]);
        idx = 0;
    }

    mChillKey = dComIfGp_particle_set(mChillKey, chill_effect_id[idx], &pos, &tevStr, &shape_angle,
                                      &scale, 0xff, NULL, -1, NULL, NULL, NULL);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mChillKey);
    if (emitter != NULL) {
        emitter->setGlobalScale(JGeometry::TVec3<f32>(scale));
    }
}

void daB_YO_c::setApperEffect() {
    cXyz pos = current.pos;
    pos.y -= 230.0f;
    mAppearKey = dComIfGp_particle_set(mAppearKey, 0x87AD, &pos, &tevStr, &shape_angle,
                                      NULL, 0xff, NULL, -1, NULL, NULL, NULL);
}

void daB_YO_c::setApperEffect2() {
    static u16 apper_effect_id[2] = {0x87AE, 0x87AF};
    cXyz pos = current.pos;
    pos.y -= 230.0f;
    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(apper_effect_id[i], &pos, &tevStr, &shape_angle, NULL);
    }
}

static void* s_frizad_delete(void* param_0, void* param_1) {
    if (fopAcM_IsActor(param_0) && !fpcM_IsCreating(fopAcM_GetID(param_0))
                                && fopAcM_GetName(param_0) == PROC_E_FZ) {
        static_cast<daE_FZ_c*>(param_0)->demoDelete();
    }
    return NULL;
}

void daB_YO_c::demo_skip(int param_0) {
    cDmr_SkipInfo = 1;
    dStage_changeScene(0, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
    Z2GetAudioMgr()->subBgmStop();
}

int daB_YO_c::DemoSkipCallBack(void* i_this, int param_1) {
    if (i_this != NULL) {
        static_cast<daB_YO_c*>(i_this)->demo_skip(param_1);
        return 1;
    }
    return 0;
}

void daB_YO_c::setYoMessage(int i_msgIdx) {
    mMsgIdx = i_msgIdx;
    mMsgPcID = fopMsgM_messageSet(i_msgIdx, 1000);
}

int daB_YO_c::doYoMessage() {
    if (mpMsg != NULL) {
        if (mpMsg->mode == 0xE) {
            mpMsg->mode = 0x10;
        } else if (mpMsg->mode == 0x12) {
            mpMsg->mode = 0x13;
            mMsgPcID = 0xFFFFFFFF;
            return 1;
        }
    } else {
        mpMsg = fopMsgM_SearchByID(mMsgPcID);
    }

    return 0;
}

void daB_YO_c::executeOpening() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz cam_pos, player_pos, ystone_pos;
    daPy_py_c* player = daPy_getPlayerActorClass();
    obj_ystone_class* ystone;
    f32 dVar12;

    switch (mMode) {
    case 0:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        Z2GetAudioMgr()->seStart(Z2SE_EN_YO_DEMO_OP1, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        fopAcM_OffStatus(this, 0x4000);
        field_0xfb5 = 3;
        setBck(ANM_WAIT_A, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        mMode = 1;
        gravity = 0.0f;
        current.pos.set(100.0f, 0.0f, 1000.0f);
        speed.y = 0.0f;
        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);
        mCamCenter = current.pos;
        mCamCenter.y = 180.0f;
        mCamEye.set(0.0f, 250.0f, 1450.0f);
        mCamFovY = 68.0f;
        current.angle.y = -0x1000;
        shape_angle.y = -0x1000;
        setYoMessage(0x234E);
        ystone_pos.set(0.0f, 0.0f, 3000.0f);
        mYstoneID = fopAcM_create(PROC_OBJ_YSTONE, 5, &ystone_pos, fopAcM_GetRoomNo(this),
                                  NULL, NULL, 0xff);
        Z2GetAudioMgr()->subBgmStart(Z2BGM_BOSS_SNOWWOMAN_D1);
        mAcch.CrrPos(dComIfG_Bgsp());
        break;

    case 1:
        mCamCenter = current.pos;
        mCamCenter.y = 180.0f;
        mCamEye.set(0.0f, 250.0f, 1450.0f);
        if (doYoMessage() == 1) {
            setBck(ANM_WALK, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 2;
            mActionTimer = 90;
            mActionTimer2 = 300;
        }
        break;

    case 2:
        if (mActionTimer == 30) {
            player->changeOriginalDemo();
            player->changeDemoMoveAngle(0x8000);
            player_pos.set(-30.0f, 0.0f, -1550.0f);
            player->changeDemoPos0(&player_pos);
            player->changeDemoMode(2, 0, 0, 0);
        }
        if (mActionTimer == 1) {
            player_pos.set(-30.0f, 2.0f, -700.0f);
            player->setPlayerPosAndAngle(&player_pos, 0x8000, 0);
        }
        if (mActionTimer == 0) {
            current.pos.set(100.0f, 0.0f, -1250.0f);
            mCamEye.set(-30.0f, 260.0f, -1574.0f);
            mMode = 3;
            mActionTimer2 = 183;
            mActionTimer = 75;
            cam_pos.set(50.0f, 0.0f, -1650.0f);
            field_0xf80 = cLib_targetAngleY(&current.pos, &cam_pos);
        }
        // fallthrough

    case 3:
        if (mActionTimer != 0) {
            mCamCenter = current.pos;
            mCamCenter.y = 180.0f;
        } else {
            cam_pos.set(120.0f, 120.0f, -2032.0f);
            cLib_chasePos(&mCamCenter, cam_pos, 4.0f);
        }
        dVar12 = 3.0f;
        cLib_chaseF(&speedF, 3.0f, 0.5f);
        cam_pos.set(50.0f, 0.0f, -1650.0f);
        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &cam_pos),
                           0x20, 0x200, 0x40);
        shape_angle.y = current.angle.y;
        if (cam_pos.abs(current.pos) < dVar12) {
            current.pos = cam_pos;
            speedF = 0.0f;
            shape_angle.y = field_0xf80;
            player->changeDemoMode(20, 0, 0, 0);
            setBck(ANM_WAIT_A, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mMode = 4;
        }
        break;

    case 4:
        cam_pos.set(120.0f, 120.0f, -2032.0f);
        cLib_chasePos(&mCamCenter, cam_pos, 4.0f);
        if (mActionTimer2 == 0) {
            mMode = 5;
            mActionTimer = 30;
            mActionTimer2 = 150;
        }
        break;

    case 5:
        cam_pos.set(120.0f, 120.0f, -2032.0f);
        cLib_chasePos(&mCamCenter, cam_pos, 4.0f);
        if (mActionTimer == 0) {
            mMode = 6;
            setYoMessage(0x234F);
            mActionTimer = 30;
        }
        break;

    case 6:
        if (mActionTimer == 0) {
            cLib_chaseF(&mCamFovY, 45.0f, 0.1f);
        }
        cam_pos.set(120.0f, 120.0f, -2032.0f);
        cLib_chasePos(&mCamCenter, cam_pos, 4.0f);
        if (mActionTimer2 == 0) {
            player->changeDemoMode(1, 0, 0, 0);
        }
        if (doYoMessage() == 1) {
            mMode = 7;
            if (mActionTimer2 > 30) {
                mActionTimer = mActionTimer2;
            } else {
                mActionTimer = 30;
            }
        }
        break;

    case 7:
        cLib_chaseF(&mCamFovY, 45.0f, 0.1f);
        cam_pos.set(120.0f, 120.0f, -2032.0f);
        cLib_chasePos(&mCamCenter, cam_pos, 4.0f);
        if (mActionTimer2 == 0) {
            player->changeDemoMode(1, 0, 0, 0);
        }
        if (mActionTimer == 15) {
            setBck(ANM_TALK_A, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
        }
        if (mActionTimer == 0) {
            setYoMessage(0x2350);
            mMode = 8;
            mActionTimer = 60;
        }
        break;

    case 8:
        cLib_chaseF(&mCamFovY, 45.0f, 0.1f);
        if (mActionTimer == 0) {
            player->changeDemoMode(60, 0, 0, 0);
        }
        cLib_chaseF(&mCamFovY, 45.0f, 0.1f);
        cam_pos.set(120.0f, 120.0f, -2032.0f);
        cLib_chasePos(&mCamCenter, cam_pos, 4.0f);
        if (doYoMessage() == 1) {
            mMode = 9;
            mActionTimer = 30;
            player->changeDemoMode(1, 0, 0, 0);
        }
        break;

    case 9:
        cLib_chaseF(&mCamFovY, 45.0f, 0.1f);
        cam_pos.set(120.0f, 120.0f, -2032.0f);
        cLib_chasePos(&mCamCenter, cam_pos, 4.0f);
        if (mActionTimer == 0) {
            setYoMessage(0x2351);
            mMode = 10;
        }
        break;

    case 10:
        if (doYoMessage() == 1) {
            mActionTimer = 150;
            setBck(ANM_TO_TALK_B, J3DFrameCtrl::EMode_NONE, 15.0f, 1.0f);
            mMode = 11;
            mActionTimer2 = 10;
            mColorMode = 1;
            mColBlend = 0.0f;
        }
        break;

    case 11:
        if (mpYetaMorf->isStop()) {
            setBck(ANM_TALK_B, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 12;
            setYoMessage(0x235F);
        }
        if (mActionTimer2 == 1) {
            player->changeDemoMode(1, 0, 0, 0);
        }
        break;

    case 12:
        if (doYoMessage() == 1) {
            mActionTimer = 150;
            cam_pos.set(51.0f, 207.0f, -2082.0f);
            mCamCenterSpeed = cam_pos.abs(mCamCenter) / 120.0f;
            cam_pos.set(51.0f, 207.0f, -1562.0f);
            mCamEyeSpeed = cam_pos.abs(mCamEye) / 120.0f;
            mMode = 13;
        }
        break;

    case 13:
        cam_pos.set(51.0f, 207.0f, -2082.0f);
        cLib_chasePos(&mCamCenter, cam_pos, mCamCenterSpeed);
        cam_pos.set(51.0f, 207.0f, -1562.0f);
        cLib_chasePos(&mCamEye, cam_pos, mCamEyeSpeed);
        if (mActionTimer == 0) {
            setBck(ANM_ANGRY, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mMode = 15;
            player->changeDemoMode(1, 0, 0, 0);
        }
        break;

    case 15:
        cam_pos.set(51.0f, 207.0f, -2082.0f);
        cLib_chasePos(&mCamCenter, cam_pos, mCamCenterSpeed);
        cam_pos.set(51.0f, 207.0f, -1562.0f);
        cLib_chasePos(&mCamEye, cam_pos, mCamEyeSpeed);
        if (mpYetaMorf->checkFrame(20.0f)) {
            dComIfGp_getVibration().StartQuake(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mQuakeTimer = 30;
        }
        if (mpYetaMorf->checkFrame(29.0f)) {
            mColorMode = 2;
            mColBlend = 1.0f;
        }
        if (mpYetaMorf->isStop()) {
            mMode = 16;
            setYoMessage(0x2352);
            setBck(ANM_ANGRY_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        }
        break;

    case 16:
        if (doYoMessage() == 1) {
            mActionTimer = 30;
            mMode = 17;
            mActionTimer = 30;
        }
        break;

    case 17:
        if (mActionTimer == 0) {
            mColorMode = 3;
            mColBlend = 1.0f;
            setBck(ANM_WAIT_B, J3DFrameCtrl::EMode_LOOP, 16.0f, 1.0f);
            shape_angle.y = 0;
            current.angle.y = 0;
            mMode = 20;
            mCamCenter.set(1339.0f, 2992.0f, 196.0f);
            mCamEye.set(1172.0f, 3086.0f, 582.0f);
            mCamFovY = 68.0f;
            mActionTimer = 60;
            mHensinScale = 1.0f;
            Z2GetAudioMgr()->seStart(Z2SE_EN_YO_DEMO_OP2, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
        break;

    case 20:
        if (mActionTimer == 50) {
            setWindowBreakEffect(0);
        }
        if (mActionTimer == 30) {
            setWindowBreakEffect(1);
        }
        if (mActionTimer == 0) {
            daMirror_c::remove();
            mMode = 21;
            mCamCenter.set(-700.0f, 2992.0f, 1046.0f);
            mCamEye.set(-333.0f, 3086.0f, 1251.0f),
            mActionTimer = 60;
            setWindowBreakEffect(5);
        }
        break;

    case 21:
        if (mActionTimer == 55) {
            setWindowBreakEffect(4);
        }
        if (mActionTimer == 45) {
            setWindowBreakEffect(2);
        }
        if (mActionTimer == 30) {
            setWindowBreakEffect(3);
        }
        if (mActionTimer == 0) {
            dComIfGs_onSwitch(mSwNo, fopAcM_GetRoomNo(this));
            mMode = 22;
            setBck(ANM_FLOAT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            current.pos.set(0.0f, 0.0f, -500.0f);
            mCamEye.set(0.0f, 50.0f, 500.0f);
            mCamCenter.set(0.0f, 500.0f, 400.0f);
            mActionTimer = 230;
            player_pos.set(-200.0f, 2.0f, 200.0f);
            player->setPlayerPosAndAngle(&player_pos, 0x8000, 0);
            fopAcM_SearchByID(mYstoneID, (fopAc_ac_c**)&ystone);
            if (ystone != NULL) {
                mYstonePos.set(0.0f, 250.0f, -1700.0f);
                ystone->setCurrentPos(mYstonePos);
                ystone->setScale(1.0f);
                ystone->setShapeAngle(csXyz(0, 0x8000, 0));
            }
        }
        break;

    case 22:
        fopAcM_SearchByID(mYstoneID, (fopAc_ac_c**)&ystone);
        if (ystone != NULL) {
            if (mActionTimer < 210) {
                ystone_pos.set(current.pos.x, current.pos.y + 300.0f, current.pos.z - 100.0f);
                cLib_addCalcPos2(&mYstonePos, ystone_pos, 0.05f, 30.0f);
                ystone->setCurrentPos(mYstonePos);
                ystone->setScale(1.0f);
                ystone->setShapeAngle(csXyz(0, 0x8000, 0));
            } else if (mActionTimer & 2) {
                mYstonePos.y += 3.0f;
            } else {
                mYstonePos.y -= 3.0f;
            }
        }
        if (mActionTimer < 180) {
            setHensinEffect();
        }
        if (mActionTimer == 160) {
            player->changeDemoMode(23, 0, 0, 0);
        }
        cam_pos = current.pos;
        cam_pos.y += 300.0f;
        cLib_addCalcPos2(&mCamCenter, cam_pos, 0.1f, 10.0f);
        if (mActionTimer == 15) {
            setWindowBreakEffect(0);
            setWindowBreakEffect(1);
            setWindowBreakEffect(2);
            setWindowBreakEffect(3);
            setWindowBreakEffect(4);
            setWindowBreakEffect(5);
        }
        if (mActionTimer == 0) {
            mMode = 23;
            mActionTimer = 310;
            field_0xf80 = 0x4000;
            field_0xf64 = 0.0f;
            mYstoneScale = 1.0f;
        }
        break;

    case 23:
        if (mActionTimer > 90) {
            cLib_chaseF(&mHensinScale, 9.0f, 0.034f);
            cLib_chaseF(&mCamEye.z, 1500.0f, 4.0f);
            setHensinEffect();
        } else if (mActionTimer < 60) {
            field_0xf80 += 0x3000;
            cLib_chaseF(&field_0xf64, 10.0f, 0.5f);
            current.pos.y += 10.0f;
            current.pos.x = field_0xf64 * cM_ssin(field_0xf80);
        }
        fopAcM_SearchByID(mYstoneID, (fopAc_ac_c**)&ystone);
        if (ystone != NULL) {
            ystone_pos.set(current.pos.x, current.pos.y + 300.0f, current.pos.z - 100.0f);
            ystone->setCurrentPos(ystone_pos);
            ystone->setScale(mYstoneScale);
            ystone->setShapeAngle(csXyz(0, 0x8000, 0));
        }
        cam_pos.set(0.0f, current.pos.y + 1000.0f, current.pos.z);
        cLib_addCalcPos2(&mCamCenter, cam_pos, 0.1f, 10.0f);
        if (current.pos.y < 1000.0f) {
            cLib_chaseF(&speed.y, 10.0f, 0.2f);
        } else {
            cLib_chaseF(&speed.y, 0.0f, 0.2f);
        }
        if (mActionTimer <= 210) {
            if (mActionTimer == 210) {
                field_0xfd1 = true;
                mpYetaWhiteBrkAnm->setPlaySpeed(1.0f);
            }
            cLib_chaseF(&mYstoneScale, 0.0f, 0.01f);
        }
        if (mActionTimer <= 130) {
            if (mActionTimer == 130) {
                field_0xfb5 = 0;
                mScale = 0.8f;
                field_0xfd1 = false;
                mpYetaWhiteBrkAnm->remove(mpYetaMorf->getModel()->getModelData());
            }
            cLib_chaseF(&mScale, 1.0f, 0.005f);
        }
        if (mActionTimer == 0) {
            current.pos.x = 0.0f;
            maxFallSpeed = -200.0f;
            speed.y = -200.0f;
            mMode = 24;
            player_pos.set(-200.0f, 2.0f, 580.0f);
            player->setPlayerPosAndAngle(&player_pos, 0, 0);
            mActionTimer2 = 0;
            fopAcM_SearchByID(mYstoneID, (fopAc_ac_c**)&ystone);
            if (ystone != NULL) {
                ystone_pos.set(0.0f, 0.0f, 3000.0f);
                ystone->setCurrentPos(ystone_pos);
                ystone->setScale(1.0f);
                ystone->setShapeAngle(csXyz(0, 0x8000, 0));
            }
        }
        break;

    case 24:
        cLib_chaseF(&speed.y, -200.0f, 5.0f);
        cam_pos = current.pos;
        cLib_addCalcPos2(&mCamCenter, cam_pos, 1.0f, 170.0f);
        if (mActionTimer2 == 0 && current.pos.y < 300.0f) {
            mActionTimer2 = 31;
            player->changeDemoMode(40, 0, 0, 0);
        }
        if (mAcch.ChkGroundHit()) {
            mRoomType = 1;
            mColorMode = 4;
            mColBlend = 0.0f;
            if (mpRoomNormalBgW != NULL) {
                dComIfG_Bgsp().Release(mpRoomNormalBgW);
            }
            if (mpRoomArenaBgW != NULL) {
                dComIfG_Bgsp().Regist(mpRoomArenaBgW, this);
            }
            mMode = 25;
            dComIfGp_getVibration().StartQuake(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mQuakeTimer = 30;
            setLandingEffect(7.0f);
            Z2GetAudioMgr()->seStart(Z2SE_EN_YO_VIBE_ROOM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            mActionTimer = 60;
            mpFurnitureModel = NULL;
        }
        break;

    case 25:
        cLib_chaseF(&mCamEye.y, 20.0f, 1.0f);
        if (mActionTimer2 != 0) {
            if (mActionTimer2 == 11) {
                player->changeDemoMoveAngle(0x8000);
                player->changeDemoMode(5, 0, 0, 0);
            } else if (mActionTimer2 == 1) {
                player->changeDemoMode(34, 0, 0, 0);
            }
        }
        if (mActionTimer > 57) {
            cam_pos = current.pos;
            cLib_addCalcPos2(&mCamCenter, cam_pos, 1.0f, 170.0f);
        } else {
            cam_pos = current.pos;
            cam_pos.y += 600.0f;
            cLib_addCalcPos2(&mCamCenter, cam_pos, 0.1f, 20.0f);
        }
        if (mActionTimer == 0) {
            fopMsgM_messageSetDemo(0x483);
            mActionTimer = 150;
            mMode = 27;
        }
        break;

    case 27:
        cam_pos = current.pos;
        cam_pos.y += 1300.0f;
        cLib_addCalcPos2(&mCamCenter, cam_pos, 0.1f, 7.0f);
        if (mActionTimer == 0) {
            mCamCenter.set(-200.0f, 170.0f, 1050.0f);
            mCamEye.set(-200.0f, 120.0f, 1350.0f);
            camera->mCamera.Reset(mCamCenter, mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            field_0xfb5 = 0;
            setActionMode(ACT_CHASE, 0);
            Z2GetAudioMgr()->bgmStart(Z2BGM_BOSS_SNOWWOMAN_0, 0, 0);
            return;
        }
        break;
    }

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
    dComIfGp_getEvent().onSkipFade();
    dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 0);
}

static f32 yo_max_bound_speed[8] = {10.0f, 12.0f, 15.0f, 18.0f, 21.0f, 24.0f, 27.0f, 30.0f};

void daB_YO_c::reflectFreeMove() {
    if (cM_rnd() < 0.5f) {
        mAngleSpeed = 0x200;
    } else {
        mAngleSpeed = -0x200;
    }
    mFreeMoveMode = 1;
    current.angle.y = fopAcM_searchPlayerAngleY(this) + cM_rndFX(8000.0f) + 0x8000;
    speedF = 8.0f;
}

void daB_YO_c::calcFreeMove(f32 param_0) {
    cLib_chaseF(&speedF, param_0, 0.05f);

    switch (mFreeMoveMode) {
    case 0:
        cLib_chaseAngleS(&mAngleSpeed, 0x100, 8);
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x20, mAngleSpeed, 8);
        break;

    case 1:
        if (cLib_chaseAngleS(&mAngleSpeed, 0, 8)) {
            mAngleSpeed = 0x10;
            mFreeMoveMode = 0;
        } else {
            shape_angle.y += mAngleSpeed;
        }
        break;

    case 2:
        cLib_chaseAngleS(&mAngleSpeed, 0x800, 0x40);
        shape_angle.y += mAngleSpeed;
        break;

    case 3:
        if (cLib_chaseAngleS(&mAngleSpeed, 0x200, 0x40)) {
            mFreeMoveMode = 1;
        } else {
            shape_angle.y += mAngleSpeed;
        }
        break;
    }

    if (mAcch.ChkWallHit()) {
        dComIfGp_getVibration().StartShock(8 - mModelNo, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        speedF *= (l_HIO.mReboundSpeedRatio + cM_rndF(0.5f));
        if (speedF > yo_max_bound_speed[mModelNo] + 3.0f * l_HIO.mReboundSpeedMax) {
            speedF = yo_max_bound_speed[mModelNo] + 3.0f * l_HIO.mReboundSpeedMax;
        }

        if (mFreeMoveMode != 2) {
            if (mFreeMoveMode == 0) {
                if (cM_rnd() < 0.5f) {
                    mAngleSpeed = 0x200;
                } else {
                    mAngleSpeed = -0x200;
                }
            } else if (mDamageTimer == 0) {
                if (mAngleSpeed > 0) {
                    mAngleSpeed = 0x200;
                } else {
                    mAngleSpeed = -0x200;
                }
            } else {
                if (mAngleSpeed > 0) {
                    mAngleSpeed += 0x200;
                    if (mAngleSpeed > 0x600) {
                        mAngleSpeed = 0x600;
                    }
                } else {
                    mAngleSpeed -= 0x200;
                    if (mAngleSpeed < -0x600) {
                        mAngleSpeed = -0x600;
                    }
                }
            }
            mFreeMoveMode = 1;
        }

        mWallAngle = mAcchCir.GetWallAngleY();
        setReflectAngle();
    }
}

void daB_YO_c::setReflectAngle() {
    current.angle.y += (s16)cM_rndFX(4000.0f);
    s16 angle_diff = current.angle.y - mWallAngle;
    if (abs(angle_diff) > 0x4800) {
        current.angle.y = mWallAngle * 2 - (current.angle.y + 0x8000);
    } else if (angle_diff > 0x3800) {
        current.angle.y = mWallAngle + 0x3800;
    } else if (angle_diff < -0x3800) {
        current.angle.y = mWallAngle - 0x3800;
    }
}

static f32 yo_damage_speed[7] = {12.0f, 14.0f, 16.0f, 18.0f, 20.0f, 22.0f, 24.0f};

static f32 yo_chase_speed[8] = {3.0f, 4.0f, 5.f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};

static int yo_damage_timer[7] = {90, 100, 110, 120, 130, 140, 150};

void daB_YO_c::executeChase() {
    mAttentionTimer = 5;

    if (dComIfGp_checkPlayerStatus0(0, 0x400)) {
        mTgIronSph[0].OnTgShield();
        mTgIronSph[1].OnTgShield();
        mTgIronSph[2].OnTgShield();
    } else {
        mTgIronSph[0].OffTgShield();
        mTgIronSph[1].OffTgShield();
        mTgIronSph[2].OffTgShield();
    }

    switch (mMode) {
    case 1:
        setBreakIceEffect();
        mModelNo++;

        if (mModelNo == 4) {
            fopAcM_SetMtx(this, mpModel[1]->getBaseTRMtx());
            mpBtkAnm->remove(mpModel[0]->getModelData());
            J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_YO", 0x35);
            mpBtkAnm->init(mpModel[1]->getModelData(), btk, FALSE,
                              J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
        } else if (mModelNo == 6) {
            mpBtkAnm->setFrame(2.0f);
        }

        mAcchCir.SetWall(20.0f, 140.0f * l_HIO.mScale[mModelNo]);
        mFreezardRadius = 130.0f * l_HIO.mScale[mModelNo] + l_HIO.mFreezardRadius;

        if (cM_rnd() < 0.5f) {
            mAngleSpeed = 0x600;
        } else {
            mAngleSpeed = -0x600;
        }

        mFreeMoveMode = 1;
        current.angle.y = fopAcM_searchPlayerAngleY(this) + cM_rndFX(8000.0f) + 0x8000;
        speedF = yo_damage_speed[mModelNo] * 1.5f;
        mDamageTimer = yo_damage_timer[mModelNo];
        mActionTimer = yo_damage_timer[mModelNo] + 30;

        if (mModelNo > 4) {
            mFreezardAttack = 3;
            mFreezardTimer = 10;
            mFreeMoveMode = 3;
        }
        // fallthrough

    case 0:
        mAtCyl.SetAtMtrl(dCcD_MTRL_NONE);
        mAtCyl.SetAtSpl(dCcG_At_Spl_UNK_D);
        mMode = 2;
        // fallthrough

    case 2:
        if (mModelNo < 4) {
            mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_MOVE1, 0, -1);
        } else {
            mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_MOVE2, 0, -1);
        }
        if (mAcch.ChkWallHit()) {
            setWallHitEffect();
        }

        if (mFreezardAttack == 1 || mFreezardAttack == 2) {
            if (current.pos.absXZ() < 1200.0f) {
                cLib_addCalcAngleS(&current.angle.y, fopAcM_searchPlayerAngleY(this),
                                   0x10, 0x80, 0x10);
                if (speedF < 30.0f) {
                    cLib_chaseF(&speedF, 10.0f, 0.5f);
                } else {
                    cLib_chaseF(&speedF, 10.0f, 1.0f);
                }
                calcFreeMove(0.0f);
            } else {
                cLib_addCalcAngleS(&current.angle.y, cM_atan2s(-current.pos.x, -current.pos.z),
                                   0x10, 0x40, 8);
                calcFreeMove(30.0f);
                if (mAcch.ChkWallHit()) {
                    current.angle.y = cM_atan2s(-current.pos.x, -current.pos.z) + cM_rndF(0x1000);
                }
            }
        } else {
            if (mActionTimer == 0) {
                if (mActionTimer2 == 0) {
                    cLib_addCalcAngleS(&current.angle.y, fopAcM_searchPlayerAngleY(this),
                                       0x20, 0x40, 4);
                } else {
                    cLib_addCalcAngleS(&current.angle.y, fopAcM_searchPlayerAngleY(this) + 0x8000,
                                       0x20, 0x40, 4);
                }
            }
            if (mActionTimer < 30) {
                calcFreeMove(l_HIO.mChaseSpeed + yo_chase_speed[mModelNo]);
            } else {
                calcFreeMove(yo_damage_speed[mModelNo]);
            }
        }

        if (mModelNo >= 4) {
            switch (mFreezardAttack) {
            case 0:
                if (mFreezardTimer == 0) {
                    field_0xfae = 0;
                    mFreezardNo = 0;
                    mIceAngleSpeed = 0;
                    mIceAngle = 0;
                    mFreezardAttack = 1;
                }
                break;

            case 1:
                cLib_chaseAngleS(&mIceAngleSpeed, 0x100, 1);
                mIceAngle += mIceAngleSpeed;
                if (speedF <= 30.0f) {
                    if (field_0xfae != 0) {
                        field_0xfae -= 1;
                    } else {
                        field_0xfae = 1;
                        setBreakFrizad();
                        mFreezardNo++;
                        if (mFreezardNo >= l_HIO.mFreezardNum) {
                            mFreezardAttack = 2;
                            mFreezardTimer = 45;
                            mFreeMoveMode = 2;
                            mFreezardNo = 0;
                        }
                    }
                }
                break;

            case 2:
                cLib_chaseAngleS(&mIceAngleSpeed, 0x400, 0x20);
                mIceAngle += mIceAngleSpeed;
                if (mFreezardTimer == 0) {
                    mFreezardAttack = 3;
                    mFreezardTimer = 10;
                    mFreeMoveMode = 3;
                    current.angle.y = fopAcM_searchPlayerAngleY(this);
                }
                break;

            case 3:
                if (mFreezardTimer == 0) {
                    mFreezardAttack = 0;
                    mFreezardTimer = 150;
                    speedF = 20.0f;
                }
                break;
            }
        }
    }
}

void daB_YO_c::executeSeriousDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz pos;

    switch (mMode) {
    case 0:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        dComIfGs_onOneZoneSwitch(7, fopAcM_GetRoomNo(this));
        Z2GetAudioMgr()->bgmStop(0x1e, 0);
        mAtCyl.OffAtSetBit();
        mFreeMoveMode = 0;
        mMode = 100;
        gravity = 0.0f;
        speed.y = 0.0f;
        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);
        pos.set(300.0f, 2.0f, 700.0f);
        daPy_getPlayerActorClass()
            ->setPlayerPosAndAngle(&pos, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
        mMode = 100;
        mCamCenter.set(0.0f, 250.0f, 0.0f);
        mCamEye.set(0.0f, 150.0f, 500.0f);
        mCamFovY = 68.0f;
        setBreakIceEffect();
        Z2GetAudioMgr()->seStart(Z2SE_EN_YO_DEMO_MID, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        fpcM_Search(s_frizad_delete, this);
        field_0xfb5 = 1;
        setBck(ANM_WAIT_C2, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
        mColorMode = 5;
        mColBlend = 0.0f;
        return;

    case 100:
        speedF = 60.0f;
        current.pos.set(0.0f, 0.0f, 0.0f);
        shape_angle.y = 0;
        current.angle.y = 0x8000;
        mMode = 1;
        // fallthrough

    case 1:
        fpcM_Search(s_frizad_delete, this);
        mCamCenter = current.pos;
        mCamCenter.y += 250.0f;
        pos.set(0.0f, 150.0f, -1400.0f);
        cLib_addCalcPos2(&mCamEye, pos, 0.5f, 30.0f);
        mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_MOVE2, 0, -1);
        if (mAcch.ChkWallHit()) {
            setChillEffect();
            dComIfGp_getVibration().StartShock(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            setBreakIceEffect();
            mMode = 2;
            speedF = 0.0f;
            field_0xfb5 = 3;
            mActionTimer = 60;
            current.pos.y = 60.0f;
        }
        break;

    case 2:
        //! @bug presumably should be == instead of !=
        if (mActionTimer != 0) {
            setChillEffect();
            mActionTimer = 130;
            mMode = 3;
            pos.set(0.0f, 200.0f, current.pos.z);
            mCamCenterSpeed = pos.abs(mCamCenter) / 30.0f;
            pos.set(0.0f, 200.0f, -1270.0f);
            mCamEyeSpeed = pos.abs(mCamEye) / 30.0f;
        }
        break;

    case 3:
        if (mActionTimer < 70) {
            pos.set(0.0f, 200.0f, current.pos.z);
            cLib_addCalcPos2(&mCamCenter, pos, 0.5f, mCamCenterSpeed);
            pos.set(0.0f, 200.0f, -1270.0f);
            cLib_addCalcPos2(&mCamEye, pos, 0.5f, mCamEyeSpeed);
        }
        if (mActionTimer == 0) {
            shape_angle.y = 0;
            current.angle.y = 0;
            setBck(ANM_FLOAT_B, J3DFrameCtrl::EMode_LOOP, 15.0f, 1.0f);
            mActionTimer = 90;
            mMode = 5;
            mCamCenterSpeed = 3.0f;
        }
        break;

    case 5:
        if (mActionTimer < 80) {
            if (mActionTimer < 60) {
                cLib_chaseF(&mCamCenterSpeed, 0.0f, 0.1f);
            }
            pos.set(current.pos.x, current.pos.y + 300.0f, current.pos.z);
            cLib_addCalcPos2(&mCamCenter, pos, 0.5f, mCamCenterSpeed);
            cLib_chaseF(&speed.y, 5.0f, 0.1f);
        }
        if (mActionTimer == 0) {
            current.pos.set(0.0f, 200.0f, -1200.0f);
            mCamCenter = current.pos;
            mCamCenter.y += 120.0f;
            mCamEye.set(0.0f, 450.0f, -730.0f);
            mCamFovY = 44.0f;
            mActionTimer = 120;
            mMode = 10;
            shape_angle.y = 0;
            current.angle.y = 0;
        }
        break;

    case 10:
        pos.set(current.pos.x, current.pos.y + 120.0f, current.pos.z);
        cLib_addCalcPos2(&mCamCenter, pos, 0.5f, 5.0f);
        if (speed.y != 0.0f) {
            if (current.pos.y < 300.0f) {
                cLib_chaseF(&speed.y, 5.0f, 0.2f);
            } else {
                if (cLib_chaseF(&speed.y, 0.0f, 0.2f)) {
                    setBck(ANM_WAIT_C, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
                }
            }
        }
        if (mActionTimer == 0) {
            setBck(ANM_SCREAM, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mMode = 11;
        }
        break;

    case 11:
        if (mpYetaMorf->checkFrame(20.0f)) {
            dComIfGp_getVibration().StartQuake(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mQuakeTimer = 70;
            mActionTimer = 120;
            speed.y = 0.0f;
            mMode = 12;
            mIceRange = 500.0f;
            mIceCenterPos = current.pos;
            mIceCenterPos.y -= 100.0f;
            mIceAngle = 0;
            for (int i = 0; i < 10; i++) {
                mIcicleID[i] = fopAcM_createChild(PROC_B_YOI, fopAcM_GetID(this), i, &current.pos,
                                                  fopAcM_GetRoomNo(this), &shape_angle,
                                                  NULL, -1, NULL);
            }
            mModelNo++;
            if (mModelNo == 7) {
                fopAcM_SetMtx(this, mpModel[2]->getBaseTRMtx());
                mpBtkAnm->remove(mpModel[1]->getModelData());
            }
            mAcchCir.SetWall(20.0f, 140.0f * l_HIO.mScale[mModelNo]);
            mFreezardRadius = 130.0f * l_HIO.mScale[mModelNo] + l_HIO.mFreezardRadius;
        }
        break;

    case 12:
        mActionTimer = 120;
        if (mpYetaMorf->isStop()) {
            setBck(ANM_SCREAM_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 17;
        }
        // fallthrough

    case 17:
        pos.set(current.pos.x, current.pos.y + 220.0f, current.pos.z);
        cLib_addCalcPos2(&mCamCenter, pos, 0.5f, 2.0f);
        pos.set(0.0f, 420.0f, -270.0f);
        cLib_addCalcPos2(&mCamEye, pos, 0.5f, 5.0f);
        cLib_chaseF(&mCamFovY, 68.0f, 0.2f);
        mIceCenterPos = current.pos;
        mIceCenterPos.y -= 100.0f;
        setApperEffect();
        if (mActionTimer == 0) {
            setApperEffect2();
            dComIfGp_getVibration().StartShock(6, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mMode = 18;
            field_0xfb5 = 0;
            current.pos.y -= 350.0f;
            mIceCenterPos.y = current.pos.y - 30.0f;
            mActionTimer = 90;
            setBck(ANM_WAIT_D, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        }
        break;

    case 18:
        mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_MOVE_FLY, 0, -1);
        mIceCenterPos = current.pos;
        mIceCenterPos.y = current.pos.y - 30.0f;
        if (current.pos.y < 1500.0f) {
            cLib_chaseF(&speed.y, 10.0f, 0.3f);
        } else {
            cLib_chaseF(&speed.y, -10.0f, 0.3f);
        }
        if (mActionTimer == 0) {
            mColorMode = 6;
            mColBlend = 0.0f;
            setIcicleOperate(3);
            setActionMode(ACT_JUMP, 0);
            Z2GetAudioMgr()->bgmStart(Z2BGM_BOSS_SNOWWOMAN_1, 0, 0);
            camera->mCamera.Reset(mCamCenter, mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
        }
        break;
    }

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
}

u8 daB_YO_c::checkIcecleRevival() {
    int ret = 0;
    if (mIceBreak == 0x3ff && field_0xfb2 != 0) {
        field_0xfb2--;
        if (field_0xfb2 == 0) {
            if ((field_0xfb1 & field_0xfb3) == 0) {
                field_0xfb1 |= field_0xfb3;
                field_0xfb1 |= field_0xfb3;
                return 2;
            }
            mIceBreak = 0;
            ret = 1;
        }
    }
    return ret;
}

void daB_YO_c::createIcecleRevival(u8 param_0) {
    if (param_0 == 2) {
        setActionMode(ACT_DEMO_REVIVAL, 0);
    } else {
        for (int i = 0; i < 10; i++) {
            daB_YOI_c* icicle = NULL;
            if (mIcicleID[i] != -1) {
                fopAcM_SearchByID(mIcicleID[i], (fopAc_ac_c**)&icicle);
            }
            if (icicle == NULL) {
                if (param_0 != 0) {
                    mIcicleID[i] = fopAcM_createChild(PROC_B_YOI, fopAcM_GetID(this), i | 0x10000,
                                                      &current.pos, fopAcM_GetRoomNo(this),
                                                      &shape_angle, NULL, -1, NULL);
                }
            } else {
                icicle->setOperate(4);
            }
        }
    }
}

void daB_YO_c::executeJump() {
    mIceAngle += mIceAngleSpeed;
    mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_MOVE_FLY, 0, -1);

    switch (mMode) {
    case 0:
        mTgIronSph[0].OffTgShield();
        mTgIronSph[1].OffTgShield();
        mTgIronSph[2].OffTgShield();
        mMode = 1;
        gravity = 0.0f;
        field_0xfae = 0;
        dComIfGs_onSaveDunSwitch(0x14);
        // fallthrough

    case 1:
        cLib_chaseAngleS(&mIceAngleSpeed, 0x400, 0x10);
        cLib_chaseF(&speed.y, 10.0f, 0.5f);
        cLib_chaseF(&speedF, 0.0f, 0.1f);
        if (mAcch.ChkWallHit()) {
            mWallAngle = mAcchCir.GetWallAngleY();
            setReflectAngle();
            speedF *= 0.5f;
        }
        cLib_chaseAngleS(&mAngleSpeed, 0, 8);
        if (current.pos.y > 1000.0f) {
            mTgIronSph[0].OnTgShield();
            mTgIronSph[1].OnTgShield();
            mTgIronSph[2].OnTgShield();
            mMode = 2;
            mFreeMoveMode = 0;
            mActionTimer = 180;
            mAngleSpeed = 0;
            field_0xfb4 = checkIcecleRevival();
            if (field_0xfb4 == 1) {
                mCreatureSound.startCreatureVoice(Z2SE_EN_YO_V_ICICLE, -1);
            }
        }
        mIceCenterPos = current.pos;
        break;

    case 2:
        cLib_addCalcAngleS(&current.angle.y, fopAcM_searchPlayerAngleY(this), 0x20, 0x40, 4);
        calcFreeMove(20.0f);
        if (mActionTimer < 150.0f) {
            setIcicleSubNumber();
            cLib_chaseAngleS(&mIceAngleSpeed, 0x400, 0x10);
            daPy_py_c* player = daPy_getPlayerActorClass();
            cXyz pos;
            pos.set(player->current.pos.x, 1500.0f, player->current.pos.z);
            cLib_chaseF(&mIceCenterSpeed, 100.0f, 1.0f);
            cLib_chasePos(&mIceCenterPos, pos, mIceCenterSpeed);
            if (current.pos.y > 1500.0f) {
                cLib_chaseF(&speed.y, 0.0f, 1.0f);
            } else {
                cLib_chaseF(&speed.y, 10.0f, 0.5f);
            }
            cLib_chaseF(&mIceRange, 150.0f, 3.0f);
            if (mActionTimer == 0) {
                if (mIceCenterPos.absXZ(pos) < 300.0f) {
                    setActionMode(ACT_ATTACK_ICE, 0);
                    return;
                }
            }
        } else {
            cLib_chaseAngleS(&mIceAngleSpeed, 0x400, 8);
            mIceCenterPos = current.pos;
            mIceCenterSpeed = 0.0f;
            if (field_0xfae == 0) {
                if (current.pos.y < 1500.0f) {
                    cLib_chaseF(&speed.y, 10.0f, 0.3f);
                } else {
                    field_0xfae = 1;
                }
            } else {
                if (current.pos.y < 1500.0f) {
                    cLib_chaseF(&speed.y, 10.0f, 0.3f);
                } else {
                    cLib_chaseF(&speed.y, -10.0f, 0.3f);
                }
            }
        }
        if (mActionTimer == 150) {
            createIcecleRevival(field_0xfb4);
            field_0xfb4 = 0;
        }
        break;
    }
}

void daB_YO_c::executeAttackIce() {
    mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_MOVE_FLY, 0, -1);
    mIceAngle += mIceAngleSpeed;
    cXyz player_pos = daPy_getPlayerActorClass()->current.pos;
    if (current.pos.y < 1200.0f) {
        cLib_chaseF(&speed.y, 5.0f, 0.5f);
    } else {
        cLib_chaseF(&speed.y, -5.0f, 0.5f);
    }

    switch (mMode) {
    case 0:
        mTgIronSph[0].OnTgShield();
        mTgIronSph[1].OnTgShield();
        mTgIronSph[2].OnTgShield();
        mIceStatus = 0;
        setIcicleOperate(0);
        mMode = 1;
        // fallthrough

    case 1:
        setIcicleSubNumber();
        cLib_chaseAngleS(&mIceAngleSpeed, 0, 8);
        cLib_addCalcAngleS(&current.angle.y, fopAcM_searchPlayerAngleY(this), 0x20, 0x40, 4);
        calcFreeMove(20.0f);
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz pos;
        pos.set(player->current.pos.x, 1500.0f, player->current.pos.z);
        cLib_chaseF(&mIceCenterSpeed, 100.0f, 1.0f);
        cLib_chasePos(&mIceCenterPos, pos, mIceCenterSpeed);
        cLib_addCalcAngleS(&current.angle.y, fopAcM_searchPlayerAngleY(this), 0x20, 0x40, 4);
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this),
                           0x10, mAngleSpeed, 0x10);
        if ((mIceStatus | mIceBreak) == 0x3ff) {
            setActionMode(ACT_ATTACK_YOSE, 0);
            setIcicleOperate(5);
        }
        break;
    }
}

void daB_YO_c::executeAttackYose() {
    mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_MOVE_FLY, 0, -1);
    cXyz player_pos = daPy_getPlayerActorClass()->current.pos;
    cXyz ice_pos = player_pos;
    reviseIceCenterPos(&ice_pos);

    switch (mMode) {
    case 0:
        mIceStatus = 0;
        mMode = 1;
        mIceRange = 750.0f;
        speedF = 0.0f;
        mActionTimer = 90;
        mIceCenterSpeed = 0.0f;
        break;

    case 1:
        mIceCenterPos = ice_pos;
        if (mActionTimer != 0) {
            cLib_addCalcAngleS(&current.angle.y, fopAcM_searchPlayerAngleY(this), 0x20, 0x40, 4);
            calcFreeMove(20.0f);
        } else {
            cLib_chaseF(&speedF, 0.0f, 0.5f);
            mIceCenterPos.y = current.pos.y;
            cLib_chaseF(&mIceCenterSpeed, 30.0f, 1.0f);
            cLib_chasePosXZ(&current.pos, mIceCenterPos, mIceCenterSpeed);
        }
        if (current.pos.y < 1200.0f) {
            cLib_chaseF(&speed.y, 5.0f, 0.5f);
        } else {
            cLib_chaseF(&speed.y, -5.0f, 0.5f);
        }
        if ((mIceStatus | mIceBreak) == 0x3ff) {
            mActionTimer = 30;
            mMode = 2;
        }
        break;

    case 2:
        mIceCenterPos = ice_pos;
        cLib_chaseF(&speedF, 0.0f, 0.5f);
        mIceCenterPos.y = current.pos.y;
        cLib_chaseF(&unk_F58, 1.0f, 0.02f);
        cLib_chaseF(&mIceCenterSpeed, 30.0f, 1.0f);
        cLib_chasePosXZ(&ice_pos, mIceCenterPos, mIceCenterSpeed);
        if (current.pos.y < 1200.0f) {
            cLib_chaseF(&speed.y, 5.0f, 0.5f);
        } else {
            cLib_chaseF(&speed.y, -5.0f, 0.5f);
        }
        if (mActionTimer == 0) {
            setIcicleOperate(6);
            mIceStatus = 0;
            mActionTimer = (int)l_HIO.mGatherTimer + 30;
            mIceCenterSpeed = 0.0f;
            mMode = 3;
        }
        break;

    case 3:
        if ((mIceStatus | mIceBreak) != 0x3ff) {
            mIceCenterPos = ice_pos;
            s16 angle = daPy_getPlayerActorClass()->current.angle.y;
            f32 speed = daPy_getPlayerActorClass()->getSpeedF() * 7.0f;
            mIceCenterPos.x += speed * cM_ssin(angle);
            mIceCenterPos.z += speed * cM_scos(angle);
            if (mActionTimer <= l_HIO.mGatherTimer) {
                mIceRange = 500.0f;
            }
        } else {
            if (mActionTimer < 30) {
                dComIfGs_offSaveDunSwitch(0x14);
                dComIfGs_onSaveDunSwitch(0x16);
            }
        }
        cLib_chaseF(&unk_F58, 1.0f, 0.02f);
        cLib_chaseF(&speedF, 0.0f, 0.5f);
        cLib_chaseF(&speed.y, 0.0f, 0.5f);
        cLib_chaseF(&current.pos.y, 1200.0f, 5.0f);
        mIceCenterPos.y = current.pos.y;
        cLib_chaseF(&mIceCenterSpeed, 50.0f, 1.0f);
        cLib_chasePosXZ(&current.pos, mIceCenterPos, mIceCenterSpeed);
        if (player_pos.absXZ(mIceCenterPos) > mIceRange + 100.0f) {
            setActionMode(ACT_ATTACK_BODY, 0);
        }
        if (mActionTimer == 0) {
            setActionMode(ACT_ATTACK_BODY, 0);
        }
    }
}

void daB_YO_c::checkDamageWithIceBreak() {
    s16 player_angle = fopAcM_searchPlayerAngleY(this);
    if (mAtCyl.ChkAtHit() && fopAcM_GetName(mAtCyl.GetAtHitObj()->GetAc()) == PROC_ALINK) {
        for (int i = 0; i < 10; i++) {
            s16 angle_diff = mIceAngle + 0x1999 * i - player_angle;
            if (abs(angle_diff) < 0xccc && mIcicleID[i] != -1) {
                daB_YOI_c* icicle;
                fopAcM_SearchByID(mIcicleID[i], (fopAc_ac_c**)&icicle);
                if (icicle != NULL) {
                    icicle->setOperate(7);
                }
            }
        }
        daPy_getPlayerActorClass()->setThrowDamage(player_angle, 35.0f, 30.0f, 4, 0, 0);
        mCamLockOnTimer = 60;
    }
}

void daB_YO_c::executeAttackBody() {
    mIceCenterPos.y = current.pos.y;

    switch (mMode) {
    case 0:
        mIceRange = 500.0f;
        mTgIronSph[0].OnTgShield();
        mTgIronSph[1].OnTgShield();
        mTgIronSph[2].OnTgShield();
        mAtCyl.SetAtMtrl(dCcD_MTRL_NONE);
        mAtCyl.SetAtSpl(dCcG_At_Spl_UNK_D);
        mAtCyl.OnAtSetBit();
        mAtCyl.SetAtAtp(4);
        mAtCyl.OnAtNoTgHitInfSet();
        mActionTimer = 30;
        mMode = 10;
        // fallthrough

    case 10:
        mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_FALL, 0, -1);
        cLib_chaseF(&mIceCenterSpeed, 50.0f, 1.0f);
        cLib_chasePosXZ(&current.pos, mIceCenterPos, mIceCenterSpeed);
        cLib_chaseAngleS(&mAngleSpeed, 0xc00, 0x100);
        shape_angle.y += mAngleSpeed;
        cLib_chaseF(&speed.y, 30.0f, 0.5f);
        if (mActionTimer == 0) {
            speedF = 0.0f;
            gravity = -10.0f;
            mMode = 1;
        }
        break;

    case 1:
        mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_FALL, 0, -1);
        cLib_chaseAngleS(&mAngleSpeed, 0xc00, 0x100);
        shape_angle.y += mAngleSpeed;
        checkDamageWithIceBreak();
        if (mAcch.ChkGroundHit()) {
            unk_F58 = 0.0f;
            mAngleSpeed = 0;
            field_0xfac = true;
            speed.y = 0.0f;
            speed.y = 0.0f;
            mMode = 2;
            mActionTimer = 30;
            dComIfGp_getVibration().StartQuake(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mQuakeTimer = 30;
            setIcicleOperate(1);
            setLandingEffect(2.0f);
            Z2GetAudioMgr()->seStart(Z2SE_EN_YO_VIBE_ROOM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            mCreatureSound.startCreatureSound(Z2SE_EN_YO_LAND, 0, -1);
            dComIfGs_offSaveDunSwitch(0x16);
        }
        break;

    case 2:
        checkDamageWithIceBreak();
        mMode = 3;
        // fallthrough

    case 3:
        mAttentionTimer = 10;
        if (mActionTimer == 0) {
            mTgIronSph[0].OffTgShield();
            mTgIronSph[1].OffTgShield();
            mTgIronSph[2].OffTgShield();
            mAtCyl.SetAtSpl(dCcG_At_Spl_UNK_0);
            mAtCyl.SetAtMtrl(dCcD_MTRL_ICE);
            mAtCyl.OffAtSetBit();
            mAtCyl.OffAtNoTgHitInfSet();
            mMode = 4;
            mActionTimer = l_HIO.mAttackTimer;
            gravity = 0.0f;
            field_0xf64 = 0.0f;
        }
        break;

    case 4:
        mCreatureSound.startCreatureSoundLevel(Z2SE_EN_YO_VIBRATE, 0, -1);
        mAttentionTimer = 10;
        current.pos.y += 0.5f;
        field_0xf68 += 0x2800;
        field_0xf6a += 0x2000;
        cLib_chaseF(&field_0xf64, 1000.0f, 3.0f);
        shape_angle.x = field_0xf64 * cM_ssin(field_0xf68);
        shape_angle.z = field_0xf64 * cM_ssin(field_0xf6a);
        if (mActionTimer == 0) {
            mCreatureSound.startCreatureSound(Z2SE_EN_YO_PULLOUT, 0, -1);
            shape_angle.x = shape_angle.z = 0;
            setIcicleOperate(2);
            setActionMode(ACT_JUMP, 0);
            speed.y = 50.0f;
            mAttentionTimer = 10;
            if (mIceBreak == 0x3ff && field_0xfb2 == 0) {
                field_0xfb3 = 2;
                field_0xfb2 = 2;
            }
        }
        break;
    }
}

void daB_YO_c::executeDemoRevival() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    switch (mMode) {
    case 0:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        mMode = 1;
        mIceAngleSpeed = 0x200;
        mActionTimer = 50;
        speedF = 0.0f;
        speed.y = 5.0f;
        shape_angle.y = 0;
        for (int i = 0; i < 10; i++) {
            if (mIceBreak & (1 << i)) {
                field_0xf80 = i * 0x1999;
                break;
            }
        }
        mIceAngle = 0x8000 - field_0xf80;
        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);
        current.pos.set(0.0f, 700.0f, 0.0f);
        mCamCenter = current.pos;
        mCamCenter.y = 1500.0f;
        mCamEye.set(0.0f, 1000.0f, 1200.0f);
        mCamFovY = 60.0f;
        setIcicleOperate(8);
        // fallthrough

    case 1:
        if (mActionTimer == 0) {
            cLib_chaseAngleS(&mIceAngleSpeed, 0, 0x10);
        }
        if (current.pos.y > 1150.0f && cLib_chaseF(&speed.y, 0.0f, 0.5f)) {
            mMode = 2;
            mActionTimer = 30;
            mCreatureSound.startCreatureVoice(Z2SE_EN_YO_V_ICICLE, -1);
        }
        break;

    case 2:
        cLib_chaseF(&speed.y, 0.0f, 0.5f);
        cLib_chaseF(&mIceRange, 400.0f, 1.0f);
        if (mActionTimer == 0) {
            mIceRange = 500.0f;
            mIceBreak = 0;
            field_0xfb0 = 0;
            for (int i = 0; i < 10; i++) {
                daB_YOI_c* icicle = NULL;
                if (mIcicleID[i] != -1) {
                    fopAcM_SearchByID(mIcicleID[i], (fopAc_ac_c**)&icicle);
                }
                if (icicle == NULL) {
                    mIcicleID[i] = fopAcM_createChild(PROC_B_YOI, fopAcM_GetID(this), i | 0x20000,
                                                      &current.pos, fopAcM_GetRoomNo(this),
                                                      &shape_angle, NULL, -1, NULL);
                } else {
                    fopAcM_OnStatus(icicle, 0x4000);
                }
                field_0xfb0++;
            }
            mMode = 3;
            mActionTimer = 60;
        }
        break;

    case 3:
        cLib_chaseAngleS(&mIceAngleSpeed, 0x400, 0x10);
        cLib_chaseF(&speed.y, -5.0f, 0.5f);
        cLib_addCalcAngleS(&current.angle.y, fopAcM_searchPlayerAngleY(this), 0x20, 0x40, 4);
        if (mActionTimer == 0) {
            mActionTimer = cM_rndF(60.0f) + 160.0f;
            for (int i = 0; i < 10; i++) {
                daB_YOI_c* icicle;
                if (mIcicleID[i] != -1) {
                    fopAcM_SearchByID(mIcicleID[i], (fopAc_ac_c**)&icicle);
                    if (icicle != NULL) {
                        fopAcM_OffStatus(icicle, 0x4000);
                    }
                }
            }
            setActionMode(ACT_JUMP, 2);
            camera->mCamera.Reset(mCamCenter, mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            return;
        }
        break;
    }

    mIceAngle += mIceAngleSpeed;
    mIceCenterPos = current.pos;
    mIceCenterPos.y += 100.0f;
    mIceCenterSpeed = 0.0f;
    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
}

void daB_YO_c::executeDamage() {
    mIceAngle += mIceAngleSpeed;
    mIceCenterPos = current.pos;
    mCamLockOnTimer = 15;

    switch (mMode) {
    case 0:
        mTgIronSph[0].OffTgShield();
        mTgIronSph[1].OffTgShield();
        mTgIronSph[2].OffTgShield();
        mMode = 1;
        gravity = 0.0f;
        mAttentionTimer = 0;
        speed.y = 15.0f;
        speedF = 80.0f;
        mAngleSpeed = 0x600;
        current.angle.y = fopAcM_searchPlayerAngleY(this) + cM_rndFX(8000.0f) + 0x8000;
        setBreakIceEffect();
        if (mLastPhaseDamage == 2) {
            Z2GetAudioMgr()->changeBgmStatus(1);
        }
        // fallthrough

    case 1:
        cLib_chaseAngleS(&mIceAngleSpeed, 0x400, 0x10);
        cLib_chaseF(&speed.y, 10.0f, 0.5f);
        if (current.pos.y < 900.0f) {
            cLib_chaseF(&speed.y, 15.0f, 0.5f);
        } else {
            cLib_chaseF(&speed.y, 0.0f, 0.5f);
        }
        cLib_chaseAngleS(&mAngleSpeed, 0, 8);
        if (mAcch.ChkWallHit()) {
            mCreatureSound.startCreatureSound(Z2SE_EN_YO_VIBE_ROOM, 0, -1);
            dComIfGp_getVibration().StartShock(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mAngleSpeed = 0;
            mMode = 2;
            mActionTimer = 30;
            speed.y = 0.0f;
            speedF = 0.0f;
            setWallHitEffect();
        }
        break;

    case 2:
        if (mActionTimer == 0) {
            mMode = 3;
            mActionTimer = 60;
        }
        break;

    case 3:
        if (current.pos.y < 1500.0f) {
            cLib_chaseF(&speed.y, 10.0f, 0.3f);
        } else {
            cLib_chaseF(&speed.y, -10.0f, 0.3f);
        }
        cLib_chaseF(&speedF, 20.0f, 0.5f);
        if (mActionTimer == 0 && mIceBreak == 0x3ff) {
            mTgIronSph[0].OnTgShield();
            mTgIronSph[1].OnTgShield();
            mTgIronSph[2].OnTgShield();
            setActionMode(ACT_JUMP, 0);
            field_0xfb3 = 1;
            field_0xfb2 = 1;
        }
        break;
    }
}

void daB_YO_c::executeDeath() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz pos1;
    cXyz pos2 = current.pos;
    pos2.y += 300.0f;
    obj_ystone_class* ystone;
    fopAcM_SearchByID(mYstoneID, (fopAc_ac_c**)&ystone);

    switch (mMode) {
    case 0:
        setChillEffect();
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        fopAcM_OnStatus(this, 0x4000);
        gravity = 0.0f;
        speed.y = 0.0f;
        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);
        current.pos.set(0.0f, 0.0f, 500.0f);
        setBreakIceEffect();
        pos1.set(-300.0f, 2.0f, 700.0f);
        player->setPlayerPosAndAngle(&pos1, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
        shape_angle.y = 0;
        current.angle.y = 0x8000;
        speed.y = 30.0f;
        speedF = 80.0f;
        mCamCenter = current.pos;
        mCamCenter.y += 300.0f;
        mCamEye.set(-300.0f, 500.0f, -1000.0f);
        mCamFovY = 68.0f;
        mMode = 1;
        dComIfGs_onStageBossEnemy();
        dComIfGs_onSwitch(mSwNo2, fopAcM_GetRoomNo(this));
        Z2GetAudioMgr()->bgmStop(0x1e, 0);
        Z2GetAudioMgr()->bgmStreamPrepare(0x2000059);
        Z2GetAudioMgr()->bgmStreamPlay();
        Z2GetAudioMgr()->seStart(Z2SE_EN_YO_DEMO_END, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        player->changeOriginalDemo();
        player->changeDemoMode(12, 0, 0, 0);
        break;

    case 1:
        setChillEffect();
        mCamCenter = pos2;
        if (mCamCenter.z < mCamEye.z) {
            mMode = 2;
        }
        break;

    case 2:
        mCamCenter = pos2;
        if (mAcch.ChkWallHit()) {
            current.pos.x = 0.0f;
            mCreatureSound.startCreatureSound(Z2SE_EN_YO_VIBE_ROOM, 0, -1);
            dComIfGp_getVibration().StartShock(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            speed.y = 0.0f;
            speedF = 0.0f;
            mMode = 3;
            mActionTimer = 30;
            setBck(ANM_DOWN_A, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            setBreakIceEffect();
            setWallHitEffect();
        }
        break;

    case 3:
        mCamCenter = pos2;
        pos1.set(0.0f, 800.0f, -650.0f);
        cLib_addCalcPos2(&mCamEye, pos1, 0.3f, 20.0f);
        mDoMtx_stack_c::copy(mpYetaMorf->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&pos1);
        pos1.y -= 200.0f;
        cLib_addCalcPos2(&mCamCenter, pos1, 0.1f, 3.0f);
        if (mActionTimer == 0) {
            mpYetaMorf->setPlaySpeed(1.0f);
            mMode = 99;
            mActionTimer = 35;
        }
        break;

    case 99:
        mDoMtx_stack_c::copy(mpYetaMorf->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&pos1);
        pos1.y -= 200.0f;
        cLib_addCalcPos2(&mCamCenter, pos1, 0.1f, 3.0f);
        if (mpYetaMorf->isStop()) {
            setBck(ANM_DOWN_C, J3DFrameCtrl::EMode_NONE, 15.0f, 1.0f);
            mMode = 110;
            mBlureRate = 0.0f;
        }
        // fallthrough

    case 100:
        mDoMtx_stack_c::copy(mpYetaMorf->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&pos1);
        pos1.y -= 200.0f;
        cLib_addCalcPos2(&mCamCenter, pos1, 0.1f, 3.0f);
        if (mActionTimer == 0) {
            mMode = 101;
            mActionTimer = 300;
            field_0xf80 = 0;
            mAngleSpeed = 0x400;
            speed.y = 0.0f;
            current.angle.y = 0;
            field_0xfae = 0;
            mActionTimer = 30;
            mIceCenterSpeed = 1.0f;
        }
        break;

    case 101:
        cLib_chaseF(&mIceCenterSpeed, 0.5f, 0.05f);
        mpYetaMorf->setPlaySpeed(mIceCenterSpeed);
        mDoMtx_stack_c::copy(mpYetaMorf->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&pos1);
        cLib_addCalcPos2(&mCamCenter, pos1, 0.1f, 30.0f);
        pos1.set(0.0f, 1230.0f, -1400.0f);
        cLib_addCalcPos2(&mCamEye, pos1, 0.1f, 60.0f);
        if (mActionTimer == 0) {
            setBck(ANM_DOWN_C, J3DFrameCtrl::EMode_NONE, 15.0f, 1.0f);
            mMode = 110;
            mBlureRate = 0.0f;
        }
        break;

    case 105:
        mDoMtx_stack_c::copy(mpYetaMorf->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&pos1);
        if (mActionTimer >= 30) {
            pos1.y -= 200.0f;
            cLib_addCalcPos2(&mCamCenter, pos1, 0.1f, 3.0f);
        } else {
            cLib_addCalcPos2(&mCamCenter, pos1, 0.1f, 30.0f);
            pos1.set(0.0f, 830.0f, -1100.0f);
            cLib_addCalcPos2(&mCamEye, pos1, 0.1f, 60.0f);
        }
        if (mActionTimer == 100 || mActionTimer == 70) {
            if (mActionTimer == 100) {
                field_0xfae = 12;
            } else {
                field_0xfae = 18;
            }
        } else if (field_0xfae != 0) {
            field_0xfae--;
            cLib_chaseF(&speed.y, -5.0f, 3.0f);
            if (field_0xfae == 17 || field_0xfae == 13 || field_0xfae == 9 || field_0xfae == 5) {
                speed.y = 3.0f;
            }
            if (field_0xfae == 16 || field_0xfae == 12 || field_0xfae == 8 || field_0xfae == 4) {
                speed.y = 0.0f;
            }
            mIceCenterSpeed = 1.0f;
            mpYetaMorf->setPlaySpeed(1.0f);
        } else {
            cLib_chaseF(&speed.y, 0.0f, 1.0f);
            cLib_chaseF(&mIceCenterSpeed, 0.5f, 0.05f);
            mpYetaMorf->setPlaySpeed(mIceCenterSpeed);
        }
        if (mActionTimer == 0) {
            setBck(ANM_DOWN_C, J3DFrameCtrl::EMode_NONE, 15.0f, 1.0f);
            mMode = 110;
            mBlureRate = 0.0f;
        }
        break;

    case 110:
        mDoMtx_stack_c::copy(mpYetaMorf->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&pos1);
        cLib_addCalcPos2(&mCamCenter, pos1, 0.1f, 30.0f);
        pos1.set(0.0f, 1230.0f, -1400.0f);
        cLib_addCalcPos2(&mCamEye, pos1, 0.1f, 30.0f);
        cLib_chaseF(&speed.y, 0.0f, 0.1f);
        if (mpYetaMorf->getFrame() < 100.0f) {
            cLib_addCalc2(&mBlureRate, 180.0f, 0.1f, 8.0f);
        } else {
            cLib_addCalc2(&mBlureRate, 0.0f, 0.1f, 5.0f);
        }
        if (mpYetaMorf->checkFrame(18.0f)) {
            dComIfGp_getVibration().StartQuake(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mQuakeTimer = 50;
        }
        if (mpYetaMorf->checkFrame(158.0f)) {
            dComIfGp_getVibration().StartQuake(6, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mQuakeTimer = 40;
        }
        if (mpYetaMorf->isStop()) {
            cLib_chaseF(&speed.y, -100.0f, 1.0f);
            mMode = 5;
            mActionTimer = 30;
            mCamCenterSpeed = 30.0f;
            mBlureRate = 0.0f;
        }
        break;

    case 5:
        cLib_chaseF(&speed.y, -100.0f, 1.0f);
        cLib_chaseF(&mCamCenterSpeed, 0.0f, 1.7f);
        mDoMtx_stack_c::copy(mpYetaMorf->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&pos1);
        cLib_chaseF(&mCamCenter.y, pos1.y, mCamCenterSpeed);
        if (mActionTimer == 0) {
            mMode = 6;
            current.pos.set(0.0f, 1000.0f, -900.0f);
            pos2.y = 1540.0f;
            mCamCenter = pos2;
            mCamEye.set(0.0f, 100.0f, -100.0f);
            speed.y = -30.0f;
            pos1.set(150.0f, 2.0f, -400.0f);
            player->setPlayerPosAndAngle(&pos1, 0x8000, 0);
            player->changeDemoMode(23, 0, 0, 0);
        }
        break;

    case 6:
        mCamCenter.y = current.pos.y + 540.0f;
        cLib_chaseF(&speed.y, -100.0f, 10.0f);
        if (mAcch.ChkGroundHit()) {
            dComIfGp_getVibration().StartQuake(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mQuakeTimer = 30;
            speed.y = 0.0f;
            speedF = 0.0f;
            setBreakIceEffect();
            mMode = 7;
            field_0xfb5 = 3;
            mColorMode = 7;
            mColBlend = 0.0f;
            mReverted = true;
            shape_angle.y = -0x4000;
            current.pos.y = -500.0f;
            speed.y = 0.0f;
            gravity = 0.0f;
            mMode = 8;
            mActionTimer = 60;
        }
        break;

    case 7:
        cLib_chaseF(&mCamCenter.y, 200.0f, 100.0f);
        cLib_chaseF(&mIceCenterSpeed, -100.0f, 5.0f);
        current.pos.y += mIceCenterSpeed;
        if (current.pos.y < 0.0f) {
            dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mMode = 8;
            mIceCenterSpeed = 0.0f;
            current.pos.y = 0.0f;
            mActionTimer = 60;
        }
        break;

    case 8:
        if (mActionTimer == 30) {
            mRoomType = 0;
            if (mpRoomNormalBgW != NULL) {
                dComIfG_Bgsp().Regist(mpRoomNormalBgW, this);
            }
            if (mpRoomArenaBgW != NULL) {
                dComIfG_Bgsp().Release(mpRoomArenaBgW);
            }
        }
        if (mActionTimer == 0) {
            mCamCenter.set(0.0f, 200.0f, -300.0f);
            mCamEyeAngle = 0x2800;
            mCamEyeRange = 800.0f;
            mCamEye.set(mCamEyeRange * cM_ssin(mCamEyeAngle), 200.0f,
                        mCamEyeRange * cM_scos(mCamEyeAngle) - 300.0f);
            mCamFovY = 60.0f;
            mMode = 9;
            mActionTimer = 90;
            pos1.set(0.0f, 2.0f, 0.0f);
            player->setPlayerPosAndAngle(&pos1, 0x8000, 0);
            mIceCenterSpeed = 0.0f;
            return;
        }
        break;

    case 9:
        current.pos.y = 0.0f;
        mMode = 10;
        // fallthrough

    case 10:
        if (mRoomAlpha[0] >= 250.0f) {
            mRoomAlpha[0] = 250.0f;
        }
        if (mRoomAlpha[1] < 10.0f) {
            mRoomAlpha[1] = 0.0f;
        }
        if (mActionTimer == 0) {
            mMode = 11;
            mActionTimer = 240;
            if (ystone != NULL) {
                mYstonePos.set(0.0f, 500.0f, -300.0f);
                ystone->setCurrentPos(mYstonePos);
                ystone->setShapeAngle(csXyz(0, 0x2000, 0));
                ystone->setMirrorMode(0);
            }
            player->changeDemoMode(1, 0, 0, 0);
            mAngleSpeed = 0;
        }
        break;

    case 11:
        if (mRoomAlpha[0] >= 250.0f) {
            mRoomAlpha[0] = 250.0f;
        }
        if (mRoomAlpha[1] < 10.0f) {
            mRoomAlpha[1] = 0.0f;
        }
        if (mActionTimer == 210) {
            if (ystone != NULL) {
                dComIfGp_event_setTalkPartner(ystone);
            }
            player->changeDemoMode(47, 3, 0, 0);
        }
        if (mActionTimer < 40) {
            if (ystone != NULL) {
                cLib_addCalc2(&mYstonePos.y, 180.0f, 0.1f, 1.0f);
                ystone->setCurrentPos(mYstonePos);
            }
        } else if (mActionTimer < 210) {
            cLib_chaseAngleS(&mAngleSpeed, 0x60, 8);
            cLib_addCalcAngleS2(&mCamEyeAngle, 0x3c00, 8, mAngleSpeed);
            cLib_addCalc2(&mCamEyeRange, 600.0f, 0.1f, 3.0f);
            cLib_addCalc2(&mCamCenter.y, 350.0f, 0.1f, 3.0f);
            cLib_addCalc2(&mCamEye.y, 0.0f, 0.1f, 3.0f);
            mCamEye.x = mCamEyeRange * cM_ssin(mCamEyeAngle);
            mCamEye.z = mCamEyeRange * cM_scos(mCamEyeAngle) - 300.0f;
        }
        if (mActionTimer == 0) {
            mCamEye.set(0.0f, 150.0f, 200.0f);
            mCamCenter.set(0.0f, 300.0f, -100.0f);
            mYstonePos.set(0.0f, 450.0f, -100.0f);
            mCamFovY = 68.0f;
            mMode = 13;
            mActionTimer = 120;
            if (ystone != NULL) {
                ystone->setCurrentPos(mYstonePos);
                dComIfGp_event_setTalkPartner(ystone);
            }
            player->changeDemoMode(47, 3, 0, 0);
            pos1.set(-550.0f, 0.0f, -250.0f);
            csXyz angle;
            angle.set(0, 0, 0);
            mWarpHoleID = fopAcM_createWarpHole(&pos1, &angle, fopAcM_GetRoomNo(this), 1, 1, 0xff);
            mRoomAlpha[0] = 255.0f;
            mRoomAlpha[1] = 0.0f;
        }
        break;

    case 13:
        cLib_addCalc2(&mCamCenter.y, 180.0f, 0.1f, 0.5f);
        cLib_addCalc2(&mYstonePos.y, 180.0f, 0.1f, 1.5f);
        if (ystone != NULL) {
            ystone->setCurrentPos(mYstonePos);
        }
        if (mActionTimer < 10 && fopAcM_SearchByID(mWarpHoleID) == NULL) {
            mActionTimer = 10;
        }
        if (mActionTimer == 0) {
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            mMode = 14;
            return;
        }
        break;

    case 14:
        if (!strcmp(dComIfGp_getEventManager().getRunEventName(), "BOSSCLEAR05")) {
            mMode = 15;
        }
        return;

    case 15:
        if (strcmp(dComIfGp_getEventManager().getRunEventName(), "BOSSCLEAR05")) {
            mIsEnemyDemoEnd = true;
            mMode = 16;
        }
        return;

    case 16:
        return;
    }

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
}

void daB_YO_c::reviseIceCenterPos(cXyz* param_0) {
    f32 fVar1 = mIceRange;
    if (fVar1 > 500.0f) {
        fVar1 = 500.0f;
    }
    fVar1 = 1800.0f - (fVar1 + 100.0f);
    if (param_0->absXZ() > fVar1) {
        s16 angle = cM_atan2s(param_0->x, param_0->z);
        param_0->x = fVar1 * cM_ssin(angle);
        param_0->z = fVar1 * cM_scos(angle);
    }
}

void daB_YO_c::calcPlayerBufPos() {
    cXyz player_pos = daPy_getPlayerActorClass()->current.pos;
    int dir = 0;
    cXyz buf_pos;
    mPlayerXBuf[0] = player_pos.x;
    mPlayerZBuf[0] = player_pos.z;
    unk_D88[0] += 0x200;
    s16 target_angle = (s16)cLib_targetAngleY(&player_pos, &mOldPlayerPos);
    cLib_addCalcAngleS(&mPlayerAngle, target_angle, 8, 0x200, 0x10);
    if (mOldPlayerPos.absXZ(player_pos) >= 10.0f) {
        mOldPlayerPos = player_pos;
    }

    for (int i = 1; i < 200; i++) {
        if (dir == 0) {
            unk_D88[i] = unk_D88[i - 1] + 0x200;
            s16 angle = mPlayerAngle;
            mPlayerXBuf[i] = mPlayerXBuf[i - 1] + 25.0f * cM_ssin(angle);
            mPlayerZBuf[i] = mPlayerZBuf[i - 1] + 25.0f * cM_scos(angle);
            buf_pos.set(mPlayerXBuf[i], 0.0f, mPlayerZBuf[i]);
            if (buf_pos.absXZ() > 1500.0f) {
                s16 center_angle2 = cM_atan2s(mPlayerXBuf[i], mPlayerZBuf[i]);
                s16 center_angle1 = cM_atan2s(mPlayerXBuf[i - 1], mPlayerZBuf[i - 1]);
                if ((s16)(center_angle2 - center_angle1) < 0) {
                    dir = 1;
                } else {
                    dir = -1;
                }
            }
        }

        if (dir != 0) {
            s16 center_angle;
            if (dir < 0) {
                center_angle = cM_atan2s(mPlayerXBuf[i - 1], mPlayerZBuf[i - 1]) - 0x1000;
            } else {
                center_angle = cM_atan2s(mPlayerXBuf[i - 1], mPlayerZBuf[i - 1]) + 0x1000;
            }
            mPlayerXBuf[i] = 1500.0f * cM_ssin(center_angle);
            mPlayerZBuf[i] = 1500.0f * cM_scos(center_angle);
            s16 angle = cM_atan2s(mPlayerXBuf[i - 1] - mPlayerXBuf[i],
                                  mPlayerZBuf[i - 1] - mPlayerZBuf[i]);
            mPlayerXBuf[i] = mPlayerXBuf[i - 1] + 25.0f * cM_ssin(angle);
            mPlayerZBuf[i] = mPlayerZBuf[i - 1] + 25.0f * cM_scos(angle);
        }
    }
}

void daB_YO_c::action() {
    damage_check();

    if (mAction != ACT_OPENING) {
        if (mAction == ACT_DEATH) {
            maxFallSpeed = -55.0f;
        } else {
            maxFallSpeed = l_HIO.mMaxFallSpeed;
        }
    }

    if (field_0xfb5 == 0 && mAction != ACT_DEATH) {
        setChillEffect();
    }

    if (mRoomType == 1) {
        cXyz pos(0.0f, 0.0f, 0.0f);
        mParticleKey = dComIfGp_particle_set(mParticleKey, 0x87B0, &pos, &tevStr);
    }

    attention_info.flags = 0;
    if (mAttentionTimer != 0) {
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    }

    BOOL bVar1 = TRUE;

    switch (mAction) {
    case ACT_OPENING:
        executeOpening();
        bVar1 = FALSE;
        break;
    case ACT_CHASE:
        executeChase();
        break;
    case ACT_SERIOUS_DEMO:
        executeSeriousDemo();
        break;
    case ACT_JUMP:
        executeJump();
        break;
    case ACT_ATTACK_ICE:
        executeAttackIce();
        break;
    case ACT_ATTACK_YOSE:
        executeAttackYose();
        break;
    case ACT_ATTACK_BODY:
        executeAttackBody();
        break;
    case ACT_DEMO_REVIVAL:
        executeDemoRevival();
        break;
    case ACT_DAMAGE:
        executeDamage();
        break;
    case ACT_DEATH:
        executeDeath();
        break;
    }

    if (bVar1) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }

    reviseIceCenterPos(&mIceCenterPos);

    if (mModelNo >= 5) {
        if (dComIfGs_isSaveDunSwitch(0x14) || dComIfGs_isSaveDunSwitch(0x16)) {
            dComIfGs_offSaveDunSwitch(0x15);
        } else if (dComIfGs_isSaveDunSwitch(0x15)) {
            dComIfGs_onSaveDunSwitch(0x15);
        }
    }

    if (mRoomType == 0) {
        cLib_chaseF(&mRoomAlpha[0], 255.0f, 2.7f);
        if (mRoomAlpha[0] > 128.0f) {
            cLib_chaseF(&mRoomAlpha[1], 0.0f, 5.4f);
        }
    } else {
        cLib_chaseF(&mRoomAlpha[0], 0.0f, 4.0f);
        if (mRoomAlpha[0] < 128.0f) {
            cLib_chaseF(&mRoomAlpha[1], 255.0f, 8.0f);
        }
    }

    switch (mColorMode) {
    case 0:
        mColBlend = 1.0f;
        dKy_custom_colset(0, 0, mColBlend);
        break;
    case 1:
        cLib_chaseF(&mColBlend, 1.0f, 0.008f);
        dKy_custom_colset(0, 4, mColBlend);
        break;
    case 2:
        cLib_chaseF(&mColBlend, 1.0f, 1.0f);
        dKy_custom_colset(4, 5, mColBlend);
        break;
    case 3:
        cLib_chaseF(&mColBlend, 1.0f, 1.0f);
        dKy_custom_colset(5, 1, mColBlend);
        break;
    case 4:
        cLib_chaseF(&mColBlend, 1.0f, 0.015f);
        dKy_custom_colset(1, 2, mColBlend);
        break;
    case 5:
        cLib_chaseF(&mColBlend, 1.0f, 0.015f);
        dKy_custom_colset(2, 1, mColBlend);
        break;
    case 6:
        cLib_chaseF(&mColBlend, 1.0f, 0.015f);
        dKy_custom_colset(1, 3, mColBlend);
        break;
    case 7:
        cLib_chaseF(&mColBlend, 1.0f, 0.01f);
        dKy_custom_colset(3, 0, mColBlend);
        break;
    }

    calcPlayerBufPos();
    fopAcM_posMoveF(this, NULL);
    if (field_0xfb5 != 3) {
        mAcch.CrrPos(dComIfG_Bgsp());
    }

    mpYetaMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpYetaRevertedMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    if (field_0xfcd) {
        mpYetaBtkAnm->play();
    }
    mpYetaBtpAnm->play();
    if (field_0xfcc) {
        mpYetaBrkAnm->play();
    }
    if (field_0xfd1) {
        mpYetaWhiteBrkAnm->play();
    }

    if (mpRoomNormalBgW != NULL) {
        mpRoomNormalBgW->Move();
    }
    if (mpRoomArenaBgW != NULL) {
        mpRoomArenaBgW->Move();
    }
}

void daB_YO_c::mtx_set() {
    mpRoomModel[0]->setBaseTRMtx(mBgMtx);
    mpRoomModel[1]->setBaseTRMtx(mBgMtx);
    if (mpFurnitureModel != NULL) {
        mpFurnitureModel->setBaseTRMtx(mBgMtx);
    }

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    if (field_0xfb5 == 3) {
        mpYetaMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        mpYetaMorf->modelCalc();
        if (mReverted) {
            mpYetaRevertedMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            mpYetaRevertedMorf->modelCalc();
        }
    } else {
        f32 scale = l_HIO.mScale[mModelNo] * mScale;
        mDoMtx_stack_c::scaleM(scale, scale, scale);
        if (mModelNo == 7) {
            mDoMtx_stack_c::transM(0.0f, -230.0f, 0.0f);
            mpModel[2]->setBaseTRMtx(mDoMtx_stack_c::get());
            mDoMtx_stack_c::transM(0.0f, 580.0f, 0.0f);
            mpYetaMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            mpYetaMorf->modelCalc();
        } else if (mModelNo < 4) {
            mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::get());
        } else {
            mpModel[1]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

static f32 yo_att_pos_y[7] = {1300.0f, 950.0f, 700.0f, 450.0f, 850.0f, 470.0f, 300.0f};

void daB_YO_c::cc_set() {
    static f32 yo_at_sph_radius[8] = {658.0f, 600.0f, 550.0f, 440.0f,
                                      300.0f, 200.0f, 150.0f, 280.0f};
    static f32 yo_sph_pos_addy[3] = {320.0f, 190.0f, 100.0f};
    static f32 yo_sph_pos_radius[3] = {55.0f, 100.0f, 125.0f};
    static f32 yo_sph_pos_addy2[3] = {250.0f, 400.0f, 550.0f};

    if (field_0xfb5 == 3) {
        return;
    }

    f32 scale = l_HIO.mScale[mModelNo];
    mDoMtx_stack_c::transS(current.pos);

    if (mModelNo == 7) {
        mDoMtx_stack_c::transM(0.0f, 300.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&eyePos);
        attention_info.position = eyePos;
        attention_info.position.y += 150.0f;
    } else {
        mDoMtx_stack_c::transM(0.0f, 200.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&eyePos);
        attention_info.position = eyePos;
        attention_info.position.y += yo_att_pos_y[mModelNo];
    }

    if (field_0xfac) {
        mModelNo = 2;
        scale = 4.0f;
    }

    mAtCyl.SetC(current.pos);

    if (mModelNo == 7) {
        mAtCyl.SetH(750.0f);
    } else {
        mAtCyl.SetH(scale * 250.0f);
    }

    if (field_0xfac) {
        mAtCyl.SetR(350.0f);
    } else {
        mAtCyl.SetR(yo_at_sph_radius[mModelNo]);
    }

    dComIfG_Ccsp()->Set(&mAtCyl);

    for (int i = 0; i < 3; i++) {
        cXyz pos = current.pos;
        f32 radius;
        if (mModelNo == 7) {
            pos.y += yo_sph_pos_addy2[i];
            radius = 270.0f;
        } else {
            pos.y += scale * yo_sph_pos_addy[i];
            radius = scale * yo_sph_pos_radius[i];
        }

        mTgSph[i].SetC(pos);
        mTgSph[i].SetR(radius);
        dComIfG_Ccsp()->Set(&mTgSph[i]);

        mTgIronSph[i].SetC(pos);
        mTgIronSph[i].SetR(radius);
        dComIfG_Ccsp()->Set(&mTgIronSph[i]);
    }

    if (field_0xfac) {
        mModelNo = 7;
        field_0xfac = false;
    }
}

int daB_YO_c::execute() {
    if (mIsInactive == 1) {
        mpRoomModel[mRoomType]->setBaseTRMtx(mBgMtx);

        if (mpRoomNormalBgW != NULL) {
            mpRoomNormalBgW->Move();
        }

        return 1;
    }

    if (mActionTimer != 0) {
        mActionTimer--;
    }

    if (mActionTimer2 != 0) {
        mActionTimer2--;
    }

    if (mDamageTimer != 0) {
        mDamageTimer--;
    }

    if (mFreezardTimer != 0) {
        mFreezardTimer--;
    }

    if (mIFrameTimer != 0) {
        mIFrameTimer--;
    }

    if (mIFrameIronTimer != 0) {
        mIFrameIronTimer--;
    }

    if (mQuakeTimer != 0) {
        mQuakeTimer--;
        if (mQuakeTimer == 0) {
            dComIfGp_getVibration().StopQuake(0x1f);
        }
    }

    if (mAttentionTimer != 0) {
        mAttentionTimer--;
    }

    if (mCamLockOnTimer != 0) {
        dCam_getBody()->ForceLockOn(this);
        mCamLockOnTimer--;
        if (mCamLockOnTimer == 0 && dCam_getBody()->GetForceLockOnActor() == this) {
            dCam_getBody()->ForceLockOff(this);
        }
    }

    action();
    mtx_set();
    cc_set();

    return 1;
}

static int daB_YO_Execute(daB_YO_c* i_this) {
    return i_this->execute();
}

static int daB_YO_IsDelete(daB_YO_c* i_this) {
    return 1;
}

int daB_YO_c::_delete() {
    dComIfG_resDelete(&mPhase2, "L5_R50");

    if (mpRoomNormalBgW != NULL) {
        dComIfG_Bgsp().Release(mpRoomNormalBgW);
    }

    if (heap != NULL) {
        mCreatureSound.deleteObject();
    }

    if (mIsInactive2 == 1) {
        return 1;
    }

    dComIfG_resDelete(&mPhase1, "B_YO");
    dComIfG_resDelete(&mPhase3, "ykW");
    dComIfG_resDelete(&mPhase4, "ykW1");
    dComIfG_resDelete(&mPhase5, "E_FZ");

    if (mHIOInit) {
        hioInit = false;
    }

    if (mpRoomArenaBgW != NULL) {
        dComIfG_Bgsp().Release(mpRoomArenaBgW);
    }

    return 1;
}

static int daB_YO_Delete(daB_YO_c* i_this) {
    return i_this->_delete();
}

int daB_YO_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("B_YO", BMDE_YO01);
    mpModel[0] = mDoExt_J3DModel__create(model_data, 0, 0x11000284);
    if (mpModel[0] == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("B_YO", BMDE_YO02);
    mpModel[1] = mDoExt_J3DModel__create(model_data, 0, 0x11000284);
    if (mpModel[1] == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("B_YO", BMDE_YO_CORE);
    mpModel[2] = mDoExt_J3DModel__create(model_data, 0, 0x11000284);
    if (mpModel[2] == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("B_YO", BMDE_YKW_B);
    mpYetaMorf = new mDoExt_McaMorfSO(
        model_data, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_YO", BCK_YKW_B_WAITA),
        J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, &mCreatureSound, 0, 0x11020284
    );
    if (mpYetaMorf == NULL || mpYetaMorf->getModel() == NULL) {
        return 0;
    }

    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL) {
        return 0;
    }
    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_YO", BTK_YO_KANNKYOU);
    if (!mpBtkAnm->init(mpModel[0]->getModelData(), btk, FALSE,
                           J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
        return 0;
    }

    model_data = mpYetaMorf->getModel()->getModelData();
    mpYetaBtkAnm = new mDoExt_btkAnm();
    if (mpYetaBtkAnm == NULL) {
        return 0;
    }
    btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_YO", BTK_YKW_B_WAITA);
    if (!mpYetaBtkAnm->init(model_data, btk, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1)) {
        return 0;
    }

    mpYetaBtpAnm = new mDoExt_btpAnm();
    if (mpYetaBtpAnm == NULL) {
        return 5;
    }
    J3DAnmTexPattern* btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("B_YO", BTP_YKW_B_WAITA);
    if (!mpYetaBtpAnm->init(model_data, btp, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1)) {
        return 5;
    }

    mpYetaBrkAnm = new mDoExt_brkAnm();
    if (mpYetaBrkAnm == NULL) {
        return 0;
    }
    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_YO", BRK_YKW_B);
    if (!mpYetaBrkAnm->init(model_data, brk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
        return 0;
    }

    mpYetaWhiteBrkAnm = new mDoExt_brkAnm();
    if (mpYetaWhiteBrkAnm == NULL) {
        return 0;
    }
    brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_YO", BRK_YKW_B_WHITE);
    if (!mpYetaWhiteBrkAnm->init(model_data, brk, TRUE, J3DFrameCtrl::EMode_NONE, 0.0f, 0, -1)) {
        return 0;
    }

    mpRoomNormalBgW = new dBgW();
    if (mpRoomNormalBgW == NULL) {
        return 0;
    }
    cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes("L5_R50", DZB_R50_P1);
    if (mpRoomNormalBgW->Set(dzb, 0, &mBgMtx) == 1) {
        return 0;
    }
    mpRoomNormalBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);

    model_data = (J3DModelData*)dComIfG_getObjectRes("L5_R50", BMDR_R50_P1);
    mpRoomModel[0] = mDoExt_J3DModel__create(model_data, 0, 0x31000084);
    if (mpRoomModel[0] == NULL) {
        return 0;
    }

    mpRoomArenaBgW = new dBgW();
    if (mpRoomArenaBgW == NULL) {
        return 0;
    }
    dzb = (cBgD_t*)dComIfG_getObjectRes("L5_R50", DZB_R50_P2);
    if (mpRoomArenaBgW->Set(dzb, 0, &mBgMtx) == 1) {
        return 0;
    }
    mpRoomArenaBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);

    model_data = (J3DModelData*)dComIfG_getObjectRes("L5_R50", BMDR_R50_P2);
    mpRoomModel[1] = mDoExt_J3DModel__create(model_data, 0, 0x31000084);
    if (mpRoomModel[1] == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("L5_R50", BMDR_T_R50FURNITURE);
    mpFurnitureModel = mDoExt_J3DModel__create(model_data, 0, 0x11000084);
    if (mpFurnitureModel == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("ykW", 0xb);
    mpYetaRevertedMorf = new mDoExt_McaMorfSO(
        model_data, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("ykW1", 0x1a),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, &mCreatureSound, 0, 0x11000084
    );
    if (mpYetaRevertedMorf == NULL || mpYetaRevertedMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daB_YO_c*>(i_this)->CreateHeap();
}

int daB_YO_c::CreateHeap2() {
    mpRoomNormalBgW = new dBgW();
    if (mpRoomNormalBgW == NULL) {
        return 0;
    }
    cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes("L5_R50", DZB_R50_P1);
    if (mpRoomNormalBgW->Set(dzb, 0, &mBgMtx) == 1) {
        return 0;
    }
    mpRoomNormalBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);

    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("L5_R50", BMDR_R50_P1);
    mpRoomModel[0] = mDoExt_J3DModel__create(model_data, 0, 0x31000084);
    if (mpRoomModel[0] == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit2(fopAc_ac_c* i_this) {
    return static_cast<daB_YO_c*>(i_this)->CreateHeap2();
}

cPhs__Step daB_YO_c::create() {
    fopAcM_ct(this, daB_YO_c);
    cPhs__Step step;

    mSwNo = fopAcM_GetParam(this) & 0xff;
    mSwNo2 = (fopAcM_GetParam(this) >> 8) & 0xff;
    mIsInactive = (fopAcM_GetParam(this) >> 0x10) & 0xff;
    if (mIsInactive != 1) {
        mIsInactive = 0;
    }
    if (dComIfGs_isStageBossEnemy()) {
        mIsInactive = 1;
    }
    mIsInactive2 = mIsInactive;

    if (mIsInactive == 1) {
        step = (cPhs__Step)dComIfG_resLoad(&mPhase2, "L5_R50");

        if (step == cPhs_COMPLEATE_e) {
            if (!daMirror_c::remove()) {
                return cPhs_INIT_e;
            }

            if (!fopAcM_entrySolidHeap(this, useHeapInit2, 0x2200)) {
                return cPhs_ERROR_e;
            }

            if (mpRoomNormalBgW != NULL && dComIfG_Bgsp().Regist(mpRoomNormalBgW, this)) {
                return cPhs_ERROR_e;
            }

            cXyz pos(-550.0f, 0.0f, -250.0f);
            csXyz angle(0, 0, 0);
            fopAcM_createWarpHole(&pos, &angle, fopAcM_GetRoomNo(this), 1, 0, 0xff);

            mRoomType = 0;
            mRoomAlpha[0] = 255.0f;
            mRoomAlpha[1] = 0.0f;
            mColorMode = 0;
            mColBlend = 1.0f;

            mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);
            MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

            daB_YO_Execute(this);
        }

        return step;

    } else {
        if ((step = (cPhs__Step)dComIfG_resLoad(&mPhase1, "B_YO"), step == cPhs_COMPLEATE_e)
            && (step = (cPhs__Step)dComIfG_resLoad(&mPhase2, "L5_R50"), step == cPhs_COMPLEATE_e)
            && (step = (cPhs__Step)dComIfG_resLoad(&mPhase3, "ykW"), step == cPhs_COMPLEATE_e)
            && (step = (cPhs__Step)dComIfG_resLoad(&mPhase4, "ykW1"), step == cPhs_COMPLEATE_e)
            && (step = (cPhs__Step)dComIfG_resLoad(&mPhase5, "E_FZ"), step == cPhs_COMPLEATE_e))
        {
            if (cDmr_SkipInfo != 0 && !daMirror_c::remove()) {
                return cPhs_INIT_e;
            }

            if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1bee0)) {
                return cPhs_ERROR_e;
            }

            if (!hioInit) {
                hioInit = true;
                mHIOInit = true;
                l_HIO.field_0x4 = -1;
            }

            attention_info.flags = 0;
            attention_info.distances[fopAc_attn_BATTLE_e] = 60;
            fopAcM_SetMtx(this, mpModel[0]->getBaseTRMtx());
            fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
            fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
            mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                      &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
            mAcchCir.SetWall(20.0f, l_HIO.mScale[mModelNo] * 140.0f);

            if (mpRoomNormalBgW != NULL && dComIfG_Bgsp().Regist(mpRoomNormalBgW, this)) {
                return cPhs_ERROR_e;
            }
            if (mpRoomArenaBgW != NULL && dComIfG_Bgsp().Regist(mpRoomArenaBgW, this)) {
                return cPhs_ERROR_e;
            }

            mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);
            MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

            health = 80;
            field_0x560 = 80;

            mCcStts.Init(0xff, 0, this);
            mCcIronStts.Init(0xff, 0, this);
            mAtCyl.Set(cc_yo_src);
            mAtCyl.SetStts(&mCcStts);
            for (int i = 0; i < 3; i++) {
                mTgSph[i].Set(cc_yo_tg_src);
                mTgSph[i].SetStts(&mCcStts);
                mTgIronSph[i].Set(cc_yo_iron_src);
                mTgIronSph[i].SetStts(&mCcIronStts);
            }

            mCreatureSound.init(&current.pos, &eyePos, 3, 1);
            mCreatureSound.setEnemyName("B_yo");

            mAtInfo.mpSound = &mCreatureSound;
            mAtInfo.mPowerType = 1;
            gravity = -5.0f;
            mIceRange = 500.0f;
            dComIfGs_offSaveDunSwitch(0x14);
            dComIfGs_offSaveDunSwitch(0x16);
            mpBtkAnm->setFrame(1.0f);
            mAcch.OffSameActorChk();

            if (cDmr_SkipInfo != 0) {
                mScale = 1.0f;
                setBck(ANM_ANGRY, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                cDmr_SkipInfo = 0;
                field_0xfb5 = 0;
                setActionMode(ACT_CHASE, 0);
                Z2GetAudioMgr()->bgmStart(Z2BGM_BOSS_SNOWWOMAN_0, 0, 0);

                if (!dComIfGs_isSwitch(mSwNo, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(mSwNo, fopAcM_GetRoomNo(this));
                }

                cXyz pos(0.0f, 0.0f, 3000.0f);
                mYstoneID = fopAcM_create(PROC_OBJ_YSTONE, 5, &pos, fopAcM_GetRoomNo(this),
                                         NULL, NULL, 0xff);
                
                mRoomType = 1;
                mRoomAlpha[0] = 0.0f;
                mRoomAlpha[1] = 255.0f;

                if (mpRoomNormalBgW != NULL) {
                    dComIfG_Bgsp().Release(mpRoomNormalBgW);
                }

                mpFurnitureModel = NULL;
                mColorMode = 4;
                mColBlend = 1.0f;
                g_env_light.mColpatWeather = 2;
                g_env_light.wether_pat0 = 2;
                g_env_light.wether_pat1 = 2;
                fopAcM_OffStatus(this, 0x4000);
            
            } else {
                mRoomType = 0;
                mRoomAlpha[0] = 255.0f;
                mRoomAlpha[1] = 0.0f;

                if (mpRoomArenaBgW != NULL) {
                    dComIfG_Bgsp().Release(mpRoomArenaBgW);
                }

                mColorMode = 0;
                mColBlend = 1.0f;
                field_0xfb5 = 3;
                setActionMode(ACT_OPENING, 0);
            }

            onWolfNoLock();
            daB_YO_Execute(this);
        }

        return step;
    }
}

static cPhs__Step daB_YO_Create(daB_YO_c* i_this) {
    return i_this->create();
}

namespace
{

static cXyz m_common_pos;

static u32 m_common_count;

}  // namespace

static actor_method_class l_daB_YO_Method = {
    (process_method_func)daB_YO_Create,
    (process_method_func)daB_YO_Delete,
    (process_method_func)daB_YO_Execute,
    (process_method_func)daB_YO_IsDelete,
    (process_method_func)daB_YO_Draw,
};

extern actor_process_profile_definition g_profile_B_YO = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_B_YO,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daB_YO_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  223,                    // mPriority
  &l_daB_YO_Method,       // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
