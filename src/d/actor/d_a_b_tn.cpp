/**
* @file d_a_b_tn.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_tn.h"
#include <cmath.h>
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_boomerang.h"
#include "d/actor/d_a_nbomb.h"

#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

class daB_TN_HIO_c {
public:
    daB_TN_HIO_c();
    virtual ~daB_TN_HIO_c() {};

    /* 0x04 */ s8 mUnk1;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 mKColorA;
    /* 0x10 */ f32 mTimer3Wolf;
    /* 0x14 */ f32 mTimer3HumanType0;
    /* 0x18 */ f32 mTimer3HumanType1;
    /* 0x1C */ f32 mTimer3NormalType0;
    /* 0x20 */ f32 mTimer3NormalType1;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 mTimer1Action1;
    /* 0x2C */ f32 mTimer1Action2;
};

STATIC_ASSERT(sizeof(daB_TN_HIO_c) == 0x30);

enum B_TN_RES_FILE_ID {
    /* BCK */
    /* 0x04 */ BCK_TN2B_DIE = 4,
    /* 0x05 */ BCK_TNA_ATACK_A,
    /* 0x06 */ BCK_TNA_ATACK_B,
    /* 0x07 */ BCK_TNA_ATACK_OP,
    /* 0x08 */ BCK_TNA_ATACK_SHIELD,
    /* 0x09 */ BCK_TNA_DAMAGE_L,
    /* 0x0A */ BCK_TNA_DAMAGE_LAST,
    /* 0x0B */ BCK_TNA_DAMAGE_R,
    /* 0x0C */ BCK_TNA_GUARD,
    /* 0x0D */ BCK_TNA_GUARD_DAMAGE,
    /* 0x0E */ BCK_TNA_TURN_OP,
    /* 0x0F */ BCK_TNA_WAIT,
    /* 0x10 */ BCK_TNA_WAIT_B_1,
    /* 0x11 */ BCK_TNA_WAIT_B_2,
    /* 0x12 */ BCK_TNA_WAIT_OP,
    /* 0x13 */ BCK_TNA_WALK,
    /* 0x14 */ BCK_TNB_ATACK_A,
    /* 0x15 */ BCK_TNB_ATACK_B,
    /* 0x16 */ BCK_TNB_ATACK_SHIELD,
    /* 0x17 */ BCK_TNB_DAMAGE_L,
    /* 0x18 */ BCK_TNB_DAMAGE_R,
    /* 0x19 */ BCK_TNB_DIE,
    /* 0x1A */ BCK_TNB_DOWN,
    /* 0x1B */ BCK_TNB_DOWN_SIPPU,
    /* 0x1C */ BCK_TNB_GUARD_A,
    /* 0x1D */ BCK_TNB_GUARD_B,
    /* 0x1E */ BCK_TNB_JUMP_B_1,
    /* 0x1F */ BCK_TNB_JUMP_B_2,
    /* 0x20 */ BCK_TNB_JUMP_F_1,
    /* 0x21 */ BCK_TNB_JUMP_F_2,
    /* 0x22 */ BCK_TNB_JUMP_L_1,
    /* 0x23 */ BCK_TNB_JUMP_L_2,
    /* 0x24 */ BCK_TNB_JUMP_R_1,
    /* 0x25 */ BCK_TNB_JUMP_R_2,
    /* 0x26 */ BCK_TNB_PULL,
    /* 0x27 */ BCK_TNB_SWORD_B_PULL_A,
    /* 0x28 */ BCK_TNB_SWORD_B_PULL_B,
    /* 0x29 */ BCK_TNB_THROW,
    /* 0x2A */ BCK_TNB_WAIT,
    /* 0x2B */ BCK_TNB_WAIT_B_1,
    /* 0x2C */ BCK_TNB_WAIT_B_2,
    /* 0X2D */ BCK_TNB_WALK,

    /* BMDR */
    /* 0x30 */ BMDR_TN_EFFECT = 0x30,
};

enum B_TNP_RES_FILE_ID {
    /* BMDR */
    /* 0x03 */ BMDR_TN = 3,
    /* 0x04 */ BMDR_TN_ARMOR_ARM_L,
    /* 0x05 */ BMDR_TN_ARMOR_ARM_R,
    /* 0x06 */ BMDR_TN_ARMOR_CHEST_B,
    /* 0x07 */ BMDR_TN_ARMOR_CHEST_F,
    /* 0x08 */ BMDR_TN_ARMOR_HEAD_B,
    /* 0x09 */ BMDR_TN_ARMOR_HEAD_F,
    /* 0x0A */ BMDR_TN_ARMOR_SHOULDER_L,
    /* 0x0B */ BMDR_TN_ARMOR_SHOULDER_R,
    /* 0x0C */ BMDR_TN_ARMOR_WAIST_B,
    /* 0x0D */ BMDR_TN_ARMOR_WAIST_F,
    /* 0x0E */ BMDR_TN_ARMOR_WAIST_L,
    /* 0x0F */ BMDR_TN_ARMOR_WAIST_R,
    /* 0x10 */ BMDR_TN_SHIELD,
    /* 0x11 */ BMDR_TN_SWORD_A,
    /* 0x12 */ BMDR_TN_SWORD_B,
    /* 0x13 */ BMDR_TN_SWORD_B_SAYA,
};

enum B_TNP2_RES_FILE_ID {
    /* BMDR */
    /* 0x04 */ BMDR_TN2 = 4,
    /* 0x05 */ BMDR_TN2_ARMOR_ARM_L,
    /* 0x06 */ BMDR_TN2_ARMOR_ARM_R,
    /* 0x07 */ BMDR_TN2_ARMOR_CHEST_B,
    /* 0x08 */ BMDR_TN2_ARMOR_CHEST_F,
    /* 0x09 */ BMDR_TN2_ARMOR_HEAD_A,
    /* 0x0A */ BMDR_TN2_ARMOR_HEAD_B,
    /* 0x0B */ BMDR_TN2_ARMOR_SHOULDER_L,
    /* 0x0C */ BMDR_TN2_ARMOR_SHOULDER_R,
    /* 0x0D */ BMDR_TN2_ARMOR_WAIST_B,
    /* 0x0E */ BMDR_TN2_ARMOR_WAIST_F,
    /* 0x0F */ BMDR_TN2_ARMOR_WAIST_L,
    /* 0x10 */ BMDR_TN2_ARMOR_WAIST_R,
    /* 0x11 */ BMDR_TN2_MACE,
    /* 0x12 */ BMDR_TN2_SHIELD,
    /* 0x13 */ BMDR_TN2_SWORD_A,
    /* 0x14 */ BMDR_TN2_SWORD_B,
    /* 0x15 */ BMDR_TN2_SWORD_B_SAYA,

    /* BRK */
    /* 0x18 */ BRK_TN2_BODY = 0x18,
    /* 0x19 */ BRK_TN2_COLOR_1,
    /* 0x1A */ BRK_TN2_COLOR_2,
    /* 0x1B */ BRK_TN2_SHIELD,
};

enum B_TN_JOINT {

};

namespace {
static dCcD_SrcSph cc_tt_src = {
    {
        {0, {{AT_TYPE_CSTATUE_SWING, 0, 0}, {0xD8000012, 0x43}, 0x75}},
        {dCcD_SE_METAL, 0, 0, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {6}},
        {0},
    },
    {{
        {0.0f, 0.0f, 0.0f},
        40.0f,
    }},
};

static dCcD_SrcSph cc_tt_src2 = {
    {
        {0, {{AT_TYPE_CSTATUE_SWING, 0, 0}, {0x00FBFDED, 0x43}, 0x0}},
        {dCcD_SE_METAL, 0, 0, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {7}},
        {0},
    },
    {{
        {0.0f, 0.0f, 0.0f},
        40.0f,
    }},
};

static dCcD_SrcSph cc_bomb_src = {
    {
        {0, {{AT_TYPE_CSTATUE_SWING, 0, 0}, {0x00000000, 0x00}, 0x145}},
        {dCcD_SE_NONE, 0, 0, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {0}},
        {0},
    },
    {{
        {0.0f, 0.0f, 0.0f},
        40.0f,
    }},
};

static dCcD_SrcSph cc_tt_at_src = {
    {
        {0, {{AT_TYPE_CSTATUE_SWING, 0, 0x5}, {0xD8FBFDFF, 0x00}, 0x15}},
        {dCcD_SE_METAL, 0, 9, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {2}},
        {0},
    },
    {{
        {0.0f, 0.0f, 0.0f},
        40.0f,
    }},
};

static dCcD_SrcCps cc_tt_at_cps_src = {
    {
        {0, {{AT_TYPE_CSTATUE_SWING, 0, 5}, {0xD8FBFDFF, 0x00}, 0x75}},
        {dCcD_SE_METAL, 0, 9, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {2}},
        {0},
    },
    {{
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        20.0f,
    }},
};

static dCcD_SrcSph cc_tt_tate_src = {
    {
        {0, {{AT_TYPE_CSTATUE_SWING, 0, 4}, {0xD8FBFDFF, 0x43}, 0x75}},
        {dCcD_SE_METAL, 0, 9, 0, {0}},
        {dCcD_SE_NONE, 2, 0, 0, {0x0303}},
        {0},
    },
    {{
        {0.0f, 0.0f, 0.0f},
        40.0f,
    }},
};

static dCcD_SrcSph cc_part_src = {
    {
        {0, {{AT_TYPE_CSTATUE_SWING, 0, 0}, {0x00000002, 0x00}, 0x75}},
        {dCcD_SE_METAL, 0, 0, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {6}},
        {0},
    },
    {{
        {0.0f, 0.0f, 0.0f},
        40.0f,
    }},
};

static s8 tn_model_brk_idx[16] = {
    0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 2, -1, -1, -1,
};

static s8 tn_model_brk_idx2[16] = {
    0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 2, -1, -1, -1,
};

static int tn_part_model_id[32] = {
    /* TN */
    BMDR_TN_ARMOR_ARM_L, BMDR_TN_ARMOR_ARM_R, BMDR_TN_ARMOR_CHEST_B, BMDR_TN_ARMOR_CHEST_F, 
    BMDR_TN_ARMOR_HEAD_B, BMDR_TN_ARMOR_HEAD_F,  BMDR_TN_ARMOR_SHOULDER_L, BMDR_TN_ARMOR_SHOULDER_R, 
    BMDR_TN_ARMOR_WAIST_B, BMDR_TN_ARMOR_WAIST_F, BMDR_TN_ARMOR_WAIST_L, BMDR_TN_ARMOR_WAIST_R, 
    BMDR_TN_SHIELD, BMDR_TN_SWORD_A, BMDR_TN_SWORD_B_SAYA, BMDR_TN_SWORD_B,

    /* TN2 */
    BMDR_TN2_ARMOR_ARM_L, BMDR_TN2_ARMOR_ARM_R, BMDR_TN2_ARMOR_CHEST_B, BMDR_TN2_ARMOR_CHEST_F, 
    BMDR_TN2_ARMOR_HEAD_A, BMDR_TN2_ARMOR_HEAD_B, BMDR_TN2_ARMOR_SHOULDER_L, BMDR_TN2_ARMOR_SHOULDER_R, 
    BMDR_TN2_ARMOR_WAIST_B, BMDR_TN2_ARMOR_WAIST_F, BMDR_TN2_ARMOR_WAIST_L, BMDR_TN2_ARMOR_WAIST_R, 
    BMDR_TN2_SHIELD, BMDR_TN2_MACE, BMDR_TN2_SWORD_B_SAYA, BMDR_TN2_SWORD_B,
};

static int tn_part_joint[16] = {8, 14, 3, 3, 5, 5, 11, 17, 26, 25, 27, 28, 9, 27, 27, 27};

static f32 ground_y_part[16] = {
    15.0f, 15.0f, 0.0f, 0.0f, 23.0f, 12.0f, 25.0f, 25.0f,
    -5.0f, 5.0f,  8.0f, 5.0f, 12.0f, 7.0f,  0.0f,  0.0f,
};
}  // namespace

daB_TN_HIO_c::daB_TN_HIO_c() {
    mUnk1 = -1;
    mScale = 1.3f;
    mKColorA = 40.0f;
    mTimer3NormalType0 = 150.0f;
    mTimer3NormalType1 = 250.0f;
    mTimer3HumanType0 = 210.0f;
    mTimer3HumanType1 = 350.0f;
    mTimer3Wolf = 120.0f;
    field_0x24 = 360.0f;
    mTimer1Action1 = 0.0f;
    mTimer1Action2 = 0.0f;
}

int daB_TN_c::ctrlJoint(J3DJoint* i_joint, J3DModel* model) {
    int mJntNo = i_joint->getJntNo();
    mDoMtx_stack_c::copy(model->getAnmMtx(mJntNo));

    if (mJntNo == 5) {
        mDoMtx_stack_c::XrotM(mNeckAngle);
    } else if (mJntNo == 1) {
        mDoMtx_stack_c::XrotM(mWaistAngle);
    }

    model->setAnmMtx(mJntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daB_TN_c::JointCallBack(J3DJoint* i_joint, int i_data) {
    if (i_data == 0) {
        J3DModel* model = j3dSys.getModel();
        daB_TN_c* mUserArea = (daB_TN_c*)model->getUserArea();
        if (mUserArea != NULL) {
            mUserArea->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daB_TN_c::calcNeckAngle() {
    csXyz acStack_18;
    s16 sVar1 = 0;

    if (mUpdateNeckAngle) {
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(5));
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &acStack_18);

        sVar1 = fopAcM_searchPlayerAngleY(this) - (acStack_18.y + 0x4000);
        if (sVar1 > 0x3000) {
            sVar1 = 0x3000;
        }

        if (sVar1 < -0x3000) {
            sVar1 = -0x3000;
        }
    }

    cLib_addCalcAngleS2(&mNeckAngle, sVar1, 4, 0x1000);
}

void daB_TN_c::calcWaistAngle() {
    csXyz acStack_18;
    s16 sVar1 = 0;

    if (mUpdateWaistAngle) {
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(1));
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &acStack_18);

        sVar1 = fopAcM_searchPlayerAngleY(this) - shape_angle.y;
        if (sVar1 > 0x3000) {
            sVar1 = 0x3000;
        }

        if (sVar1 < -0x3000) {
            sVar1 = -0x3000;
        }
    }

    cLib_addCalcAngleS2(&mWaistAngle, sVar1, 4, 0x1000);
}

/* 8062F01D 0003+00 data_8062F01D None */
static u8 s_hioinit;

static daB_TN_HIO_c l_HIO;

int daB_TN_c::draw() {
    J3DModel* model = mpModelMorf2->getModel();
    if (mDisappear || mInactive) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    J3DModelData* modelData = model->getModelData();
    if (mType == 1) {
        mpBrkAnm->entry(modelData);
    }

    if (mMaterialColor) {
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* mMatNodePtr = modelData->getMaterialNodePointer(i);
            J3DGXColorS10* mColor;
            mColor = mMatNodePtr->getTevColor(0);
            mColor->r = mMaterialColor;
            mColor = mMatNodePtr->getTevColor(0);
            mColor->g = mMaterialColor;
            mColor = mMatNodePtr->getTevColor(0);
            mColor->b = mMaterialColor;
        }
    }

    mpModelMorf2->entryDL();
    cXyz sp48(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &sp48, 1000.0f, 0.0f, current.pos.y,
                                    mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f,
                                    dDlst_shadowControl_c::getSimpleTex());

    for (int i = 0; i < 16; i++) {
        if (mStates[i] != 7) {
            if (i == 15) {
                model = mpModelMorf1->getModel();
                g_env_light.setLightTevColorType_MAJI(model, &tevStr);
                mpModelMorf1->entryDL();
            } else {
                model = mpPartModels[i];
                g_env_light.setLightTevColorType_MAJI(model, &tevStr);

                if (mType == 1 && mpBrkAnms[i]) {
                    modelData = model->getModelData();
                    mpBrkAnms[i]->entry(modelData);
                }
                mDoExt_modelUpdateDL(model);
            }

            if (mStates[i] == 0) {
                dComIfGd_addRealShadow(mShadowKey, model);
            }
        }
    }

    if (mUpdateModelTimer != 0) {
        g_env_light.setLightTevColorType_MAJI(swordEffectModel, &tevStr);
        modelData = swordEffectModel->getModelData();
        J3DGXColor* mKColor = modelData->getMaterialNodePointer(0)->getTevKColor(3);
        mKColor->a = l_HIO.mKColorA;
        mDoExt_modelUpdateDL(swordEffectModel);
    }

    return 1;
}

static int daB_TN_Draw(daB_TN_c* i_this) {
    return i_this->draw();
}

void daB_TN_c::setBreakPart(int i_idx) {
    static int break_part_no[13] = {
        6, 11, 8, 1, 10, 0, 9, 7, 4, 5, 12, 2, 3,
    };

    if (i_idx >= 12) {
        i_idx = 13;
    }

    int iVar1 = 0;
    for (int i = 0; i < 13; i++) {
        if (mStates[break_part_no[i]] != 0) {
            iVar1++;
        }
    }

    iVar1 = i_idx - iVar1;
    if (iVar1 != 0) {
        for (int i = 0; i <= 12; i++) {
            if (mStates[break_part_no[i]] == 0) {
                mStates[break_part_no[i]] = 2;
                mSound.startCreatureSound(Z2SE_EN_TN_BRK_ARMOR, 0, -1);

                if (mType == 1 && break_part_no[i] == 4) {
                    mNextBreakPart++;
                    iVar1++;
                }

                iVar1--;

                if (iVar1 == 0) {
                    return;
                }
            }
        }
    }
}

void daB_TN_c::setBreakHeadPart(int i_idx) {
    static int break_part_no2[13] = {
        4, 5, 7, 6, 11, 8, 1, 10, 0, 9, 12, 2, 3,
    };

    if (i_idx >= 12) {
        i_idx = 13;
    }

    int iVar1 = 0;
    for (int i = 0; i < 13; i++) {
        if (mStates[break_part_no2[i]] != 0) {
            iVar1++;
        }
    }

    iVar1 = i_idx - iVar1;
    if (iVar1 != 0) {
        for (int i = 0; i <= 12; i++) {
            if (mStates[break_part_no2[i]] == 0) {
                mStates[break_part_no2[i]] = 2;

                if (break_part_no2[i] == 4) {
                    mNextBreakPart++;
                    iVar1++;
                }

                iVar1--;

                if (iVar1 == 0) {
                    return;
                }
            }
        }
    }
}

void daB_TN_c::setPartLandEffect(int i_idx, int i_data) {
    static int break_part_vib[16] = {
        2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3, 2, 0xFF, 2,
    };

    if (i_data == 0) {
        csXyz cStack_30(0, 0, 0);
        cXyz sp1c(l_HIO.mScale, l_HIO.mScale, l_HIO.mScale);
        dComIfGp_particle_setPolyColor(0x87ea, mAcch.m_gnd, &mPositions[i_idx], &tevStr, &cStack_30,
                                       &sp1c, 0, NULL, -1, NULL);
        dComIfGp_particle_set(0x880b, &mPositions[i_idx], &tevStr, &cStack_30, &sp1c);
    }

    int mVibMode = break_part_vib[i_idx];
    if (mVibMode != 0xff) {
        dComIfGp_getVibration().StartShock(mVibMode, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
    }
}

namespace {
static cXyz offset_part[16] = {
    cXyz(-20.0f, 0.0f, 0.0f), cXyz(-20.0f, 0.0f, 0.0f),   cXyz(0.0f, 0.0f, 0.0f),
    cXyz(0.0f, 0.0f, 0.0f),   cXyz(-20.0f, 20.0f, 0.0f),  cXyz(-12.0f, 20.0f, 0.0f),
    cXyz(-17.0f, 0.0f, 0.0f), cXyz(-17.0f, 0.0f, 0.0f),   cXyz(0.0f, 35.0f, 0.0f),
    cXyz(0.0f, 15.0f, 0.0f),  cXyz(0.0f, 30.0f, 0.0f),    cXyz(0.0f, -30.0f, 0.0f),
    cXyz(-20.0f, 0.0f, 0.0f), cXyz(-40.0f, 0.0f, -70.0f), cXyz(-15.0f, 50.0f, 30.0f),
    cXyz(-12.0f, 8.0f, 0.0f),
};
}  // namespace

namespace {
static csXyz ground_angle_part[16] = {
    csXyz(0x0000, 0x0000, 0x0000),   csXyz(0x0000, 0x0000, 0x0000),   csXyz(0x0000, 0x0000, 0x0000),
    csXyz(0x0000, 0x0000, -0x8000),  csXyz(0x4000, 0x0000, -0x093e),  csXyz(0x0000, 0x0000, 0x5c00),
    csXyz(0x0000, 0x0000, 0x0400),   csXyz(-0x8000, 0x0000, -0x0400), csXyz(0x0000, 0x0000, 0x3c00),
    csXyz(0x0000, 0x0000, 0x4000),   csXyz(0x0000, 0x0000, 0x36b0),   csXyz(0x0000, 0x0000, 0x4800),
    csXyz(-0x02cc, 0x0000, 0x1000),  csXyz(-0x800, 0x0000, -0x0980),  csXyz(0x0000, 0x0000, 0x3c8c),
    csXyz(-0x04e9, -0x1800, 0x3d2a),
};
}  // namespace

void daB_TN_c::calcShieldMove() {
    s16 sVar2;
    int iVar1;

    switch (mStates[12]) {
    case 2:
        mDoMtx_stack_c::copy(mpPartModels[12]->getBaseTRMtx());
        mDoMtx_stack_c::multVecZero(&mPositions[12]);
        mPositionsCopy[12] = mPositions[12];
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &field_0x99c[12]);

        sVar2 = shape_angle.y + 0x4000;
        field_0xa5c = 0x1800;
        field_0x8dc[12].set((cM_rndF(5.0f) + 10.0f) * cM_ssin(sVar2), cM_rndF(7.0f) + 35.0f,
                            (cM_rndF(5.0f) + 10.0f) * cM_scos(sVar2));
        mStates[12] = 3;
        break;

    case 3:
        mAcchArr[12].SetGroundUpY(ground_y_part[12] + 55.0f);
        cLib_chaseF(&field_0x8dc[12].y, -70.0f, 5.0f);
        cLib_chaseAngleS(&field_0x99c[12].x, ground_angle_part[12].x, 0x400);
        cLib_chaseAngleS(&field_0x99c[12].z, field_0xa5c + ground_angle_part[12].z, 0x800);

        if (mAcchArr[12].ChkGroundHit()) {
            Z2GetAudioMgr()->seStart(Z2SE_EN_TN_SHIELD_BND, &mPositions[12], 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
            s16 sVar1 = cM_rndFX(32768.0f);
            field_0x8dc[12].set((cM_rndF(2.0f) + 5.0f) * cM_ssin(sVar1), cM_rndF(5.0f) + 18.0f,
                                (cM_rndF(2.0f) + 5.0f) * cM_scos(sVar1));
            mStates[12] = 4;
            setPartLandEffect(12, 0);
        }
        break;

    case 4:
    case 5:
        mAcchArr[12].SetGroundUpY(ground_y_part[12] + cM_ssin(field_0xa5c) * 60.0f);
        cLib_chaseAngleS(&field_0x99c[12].x, ground_angle_part[12].x, 0x400);
        cLib_chaseAngleS(&field_0x99c[12].z, ground_angle_part[12].z, 0x400);
        cLib_chaseAngleS(&field_0xa5c, 0, 0x60);
        field_0xa3c[12] += 0x1800;
        field_0x99c[12].x += (int)(field_0xa5c * cM_ssin(field_0xa3c[12]));
        field_0x99c[12].z += (int)(field_0xa5c * cM_scos(field_0xa3c[12]));
        cLib_chaseF(&field_0x8dc[12].y, -70.0f, 5.0f);

        if (mStates[12] == 4 && mAcchArr[12].ChkGroundHit()) {
            if (mType == 0) {
                mStates[12] = 5;
                field_0xa3c[12] = 0;
                field_0x8dc[12].set(0.0f, 0.0f, 0.0f);
                setPartLandEffect(12, 1);
            } else {
                fopAcM_createDisappear(this, &mPositions[12], 5, 0, 0xff);
                mStates[12] = 7;
            }
        }
        break;

    case 6:
        sVar2 = shape_angle.y + 0x4000;
        field_0x8dc[12].set((cM_rndF(2.0f) + 5.0f) * cM_ssin(sVar2), cM_rndF(5.0f) + 18.0f,
                            (cM_rndF(2.0f) + 5.0f) * cM_scos(sVar2));
        mStates[12] = 4;
        field_0xa5c = 0x1800;
        break;
    }
}

void daB_TN_c::calcSwordMoveA() {
    int idx = 13;
    switch (mStates[idx]) {
    case 0:
    case 1: {
        break;
    }

    case 2: {
        mDoMtx_stack_c::copy(mpPartModels[idx]->getBaseTRMtx());
        mDoMtx_stack_c::multVecZero(&mPositions[idx]);
        mPositionsCopy[idx] = mPositions[idx];
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &field_0x99c[idx]);

        if (mType == 0) {
            field_0x8dc[idx].set(0.0f, 0.0f, 70.0f);
        } else {
            cXyz sp20 = daPy_getPlayerActorClass()->current.pos;
            sp20.y += 200.0f;
            s16 sVar5 = cLib_targetAngleY(&mPositions[idx], &sp20);
            s16 sVar6 = cLib_targetAngleX(&mPositions[idx], &sp20);
            f32 fVar1 = fabsf(cM_scos(sVar6) * 80.0f);
            field_0x8dc[idx].set(fVar1 * cM_ssin(sVar5), cM_ssin(sVar6) * 80.0f,
                                 fVar1 * cM_scos(sVar5));
        }

        mStates[idx] = 3;
        break;
    }

    case 3: {
        Z2GetAudioMgr()->seStartLevel(Z2SE_EN_TN_WEAPON_FLY, &mPositions[idx], 0, 0, 1.0f, 1.0f,
                                      -1.0f, -1.0f, 0);

        cLib_chaseF(&field_0x8dc[idx].y, -70.0f, 0.3f);
        cLib_chaseAngleS(&field_0x99c[idx].x, ground_angle_part[idx].x, 0x800);
        field_0x99c[idx].y -= 0x1800;
        cLib_chaseAngleS(&field_0x99c[idx].z, ground_angle_part[idx].z, 0x800);

        if (mAcchArr[idx].ChkGroundHit() || mAcchArr[idx].ChkWallHit()) {
            Z2GetAudioMgr()->seStart(Z2SE_EN_TN_WEAPON_STOP, &mPositions[idx], 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
            setPartLandEffect(idx, 0);

            if (mType == 1) {
                field_0x99c[idx].y = 0x1800;
                fopAcM_createDisappear(this, &mPositions[idx], 3, 0, 0xff);
                mStates[idx] = 7;
            } else {
                field_0x8dc[idx].set(0.0f, 18.0f, 25.0f);
                mStates[idx] = 4;
            }
        }

        break;
    }

    case 4: {
        cLib_chaseF(&field_0x8dc[idx].y, -70.0f, 5.0f);
        cLib_chaseAngleS(&field_0x99c[idx].x, ground_angle_part[idx].x, 0x800);
        field_0x99c[idx].y -= 0x1000;
        cLib_chaseAngleS(&field_0x99c[idx].z, ground_angle_part[idx].z, 0x800);

        if (mAcchArr[idx].ChkGroundHit() || mAcchArr[idx].ChkWallHit()) {
            field_0x99c[idx] = ground_angle_part[idx];
            field_0x8dc[idx].set(0.0f, 0.0f, 0.0f);
            field_0xa3c[idx] = -0x1800;
            setPartLandEffect(idx, 1);
            mStates[idx] = 7;
        }

        break;
    }

    case 5:
    case 6:
    case 7: {
        return;
    }
    }
}

void daB_TN_c::calcSwordMoveB() {
    switch (mStates[15]) {
    case 2:
        mDoMtx_stack_c::copy(mpModelMorf1->getModel()->getBaseTRMtx());
        mDoMtx_stack_c::multVecZero(&mPositions[15]);
        mPositionsCopy[15] = mPositions[15];
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &field_0x99c[15]);
        field_0x8dc[15].set(0.0f, 0.0f, 10.0f);
        mStates[15] = 3;
        // [[fallthrough]]

    case 3:
    case 4:
        cLib_chaseAngleS(&field_0x99c[15].x, ground_angle_part[15].x, 16);
        cLib_chaseAngleS(&field_0x99c[15].y, ground_angle_part[15].y, 16);
        cLib_chaseAngleS(&field_0x99c[15].z, ground_angle_part[15].z, 16);
        cLib_chaseF(&field_0x8dc[15].y, -70.0f, 5.0f);

        if (mAcchArr[15].ChkGroundHit()) {
            cXyz* pos;
            Z2GetAudioMgr()->seStart(Z2SE_EN_TN_SWD_DROP, (pos = &mPositions[15]), 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);

            if (mStates[15] == 3) {
                field_0x8dc[15].set(0.0f, 10.0f, 10.0f);
                mStates[15] = 4;
                setPartLandEffect(15, 0);
            } else if (mType == 0) {
                field_0x8dc[15].set(0.0f, 0.0f, 7.0f);
                field_0x99c[15] = ground_angle_part[15];
                mStates[15] = 5;
            } else {
                fopAcM_createDisappear(this, pos, 3, 0, 0xff);
                mStates[15] = 7;
            }
        }
        return;

    case 5:
        cLib_chaseF(&field_0x8dc[15].y, -70.0f, 5.0f);
        cLib_chaseF(&field_0x8dc[15].z, 0.0f, 0.3f);
        field_0x99c[15].set(ground_angle_part[15].x, ground_angle_part[15].y,
                            ground_angle_part[15].z);
        break;

    case 6:
    case 7:
        return;
    }
}

void daB_TN_c::calcOtherPartMove(int i_idx) {
    cXyz sp7c;
    s16 sVar1;

    switch (mStates[i_idx]) {
    case 0:
    case 1: {
        break;
    }

    case 2: {
        if (i_idx != 14) {
            setArmorBreakEffect(i_idx);
        }
        mDoMtx_stack_c::copy(mpPartModels[i_idx]->getBaseTRMtx());
        mDoMtx_stack_c::multVecZero(&mPositions[i_idx]);
        mPositionsCopy[i_idx] = mPositions[i_idx];
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &field_0x99c[i_idx]);

        sp7c = daPy_getPlayerActorClass()->current.pos;

        if (i_idx == 14) {
            sVar1 = shape_angle.y + 0x4000;
        } else if (i_idx == 2) {
            sVar1 = shape_angle.y + -0x4000;
        } else if (i_idx == 3) {
            sVar1 = shape_angle.y + 0x2000;
        } else {
            sVar1 = cLib_targetAngleY(&mPositions[i_idx], &sp7c);
            if (cM_rnd() < 0.5f) {
                sVar1 = sVar1 + 0x2000 + cM_rndFX(4096.0f);
            } else {
                sVar1 = sVar1 - 0x2000 + cM_rndFX(4096.0f);
            }
        }

        field_0x8dc[i_idx].set((cM_rndF(5.0f) + 10.0f) * cM_ssin(sVar1), cM_rndF(7.0f) + 35.0f,
                               (cM_rndF(5.0f) + 10.0f) * cM_scos(sVar1));
        mStates[i_idx] = 3;
        break;
    }

    case 3:
        cLib_chaseF(&field_0x8dc[i_idx].y, -70.0f, 5.0f);

        if (i_idx == 14) {
            if (field_0x8dc[i_idx].y > 0.0f) {
                field_0x99c[i_idx].y += 0x800;
                field_0x99c[i_idx].z += 0x800;
            } else {
                cLib_chaseAngleS(&field_0x99c[i_idx].x, ground_angle_part[i_idx].x, 0x800);
                cLib_chaseAngleS(&field_0x99c[i_idx].z, ground_angle_part[i_idx].z, 0x800);
            }
        } else if (field_0x8dc[i_idx].y > 0.0f) {
            field_0x99c[i_idx].y += 0x1000;
            field_0x99c[i_idx].z += 0x1000;
        } else {
            cLib_chaseAngleS(&field_0x99c[i_idx].x, ground_angle_part[i_idx].x, 0x400);
            cLib_chaseAngleS(&field_0x99c[i_idx].z, ground_angle_part[i_idx].z, 0x400);
        }

        if ((mType != 1 || i_idx != 5) && mAcchArr[i_idx].ChkGroundHit()) {
            if (i_idx == 14) {
                Z2GetAudioMgr()->seStart(Z2SE_EN_TN_SAYA_BND_1, &mPositions[i_idx], 0, 0, 1.0f,
                                         1.0f, -1.0f, -1.0f, 0);
            } else {
                Z2GetAudioMgr()->seStart(Z2SE_EN_TN_ARMOR_BND, &mPositions[i_idx], 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);
            }

            sVar1 = cM_rndFX(32768.0f);
            field_0x8dc[i_idx].set((cM_rndF(2.0f) + 5.0f) * cM_ssin(sVar1), cM_rndF(5.0f) + 18.0f,
                                   (cM_rndF(2.0f) + 5.0f) * cM_scos(sVar1));
            mStates[i_idx] = 4;
            setPartLandEffect(i_idx, 0);
        }
        break;

    case 4:
        cLib_chaseF(&field_0x8dc[i_idx].y, -70.0f, 5.0f);
        cLib_chaseAngleS(&field_0x99c[i_idx].x, ground_angle_part[i_idx].x, 0x800);
        field_0x99c[i_idx].y += 0x400;
        cLib_chaseAngleS(&field_0x99c[i_idx].z, ground_angle_part[i_idx].z, 0x800);

        if (mAcchArr[i_idx].ChkGroundHit()) {
            if (mType == 0) {
                if (i_idx == 14) {
                    Z2GetAudioMgr()->seStart(Z2SE_EN_TN_SAYA_BND_2, &mPositions[i_idx], 0, 0, 1.0f,
                                             1.0f, -1.0f, -1.0f, 0);
                }

                mStates[i_idx] = 5;
                field_0x99c[i_idx].x = ground_angle_part[i_idx].x;
                field_0x99c[i_idx].z = ground_angle_part[i_idx].z;
                field_0xa3c[i_idx] = 0x400;
                setPartLandEffect(i_idx, 1);
            } else {
                fopAcM_createDisappear(this, &mPositions[i_idx], 3, 0, 0xff);
                mStates[i_idx] = 7;
                if (mType == 1 && i_idx == 4) {
                    mStates[5] = 7;
                }
            }
        }
        break;

    case 5:
        cLib_chaseF(&field_0x8dc[i_idx].y, -70.0f, 5.0f);
        cLib_chaseF(&field_0x8dc[i_idx].x, 0.0f, 2.0f);
        cLib_chaseF(&field_0x8dc[i_idx].z, 0.0f, 2.0f);
        cLib_chaseAngleS(&field_0xa3c[i_idx], 0, 0x40);
        field_0x99c[i_idx].y += field_0xa3c[i_idx];
        break;

    case 6:
        sVar1 = shape_angle.y + cM_rndFX(16384.0f);
        field_0x8dc[i_idx].set((cM_rndF(5.0f) + 50.0f) * cM_ssin(sVar1), 0.0f,
                               (cM_rndF(5.0f) + 50.0f) * cM_scos(sVar1));
        field_0xa3c[i_idx] = 0x800;
        mStates[i_idx] = 5;
        break;

    case 7: {
        break;
    }
    }
}

void daB_TN_c::calcPartMove() {
    for (int i = 0; i < 16; i++) {
        if (field_0xa1c[i] != 0) {
            field_0xa1c[i]--;
        }

        mPositionsCopy[i] = mPositions[i];

        if (i == 12) {
            calcShieldMove();
        } else if (i == 13) {
            calcSwordMoveA();
        } else if (i == 15) {
            calcSwordMoveB();
        } else {
            calcOtherPartMove(i);
        }

        mPositions[i] += *mSttsArr[i].GetCCMoveP();

        if (mStates[i] == 3 || mStates[i] == 4 || mStates[i] == 5) {
            mPositions[i] += field_0x8dc[i];
            mAcchArr[i].CrrPos(dComIfG_Bgsp());

            if (mChkCoHitOK && mSphArr[i].ChkCoHit()) {
                cCcD_Obj* mCoHitObj = mSphArr[i].GetCoHitObj();
                daB_TN_c* i_this = (daB_TN_c*)dCc_GetAc(mCoHitObj->GetAc());
                if (i_this == this && mCoHitObj->GetAtAtp() && field_0xa1c[i] == 0) {
                    mStates[i] = 6;
                    field_0xa1c[i] = 30;
                }

                mSphArr[i].ClrCoHit();
            }
        }
    }
}

void daB_TN_c::setBck(int i_index, u8 i_attr, f32 i_morf, f32 i_rate) {
    mpModelMorf2->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_tn", i_index), i_attr, i_morf,
                         i_rate, 0.0f, -1.0f);
}

int daB_TN_c::checkBck(int i_index) {
    return mpModelMorf2->getAnm() == dComIfG_getObjectRes("B_tn", i_index);
}

void daB_TN_c::setActionMode(int i_mode1, int i_mode2) {
    offHeadLockFlg();
    mActionMode1 = i_mode1;
    mActionMode2 = i_mode2;
}

void daB_TN_c::setSwordAtBit(int param_1) {
    if (param_1 == 0) {
        for (int i = 0; i < 4; i++) {
            mSwordSphs[i].OffAtSetBit();
            mSwordSphs[i].ClrAtHit();
        }

        mCps.OffAtSetBit();
        mCps.ClrAtHit();
    } else {
        for (int i = 0; i < 4; i++) {
            mSwordSphs[i].OnAtSetBit();
            if (mActionMode1 <= 8) {
                mSwordSphs[i].SetAtAtp(4);
            } else {
                mSwordSphs[i].SetAtAtp(2);
            }
        }

        mCps.OnAtSetBit();
        if (mActionMode1 <= 8) {
            mCps.SetAtAtp(4);
        } else {
            mCps.SetAtAtp(2);
        }
    }
}

void daB_TN_c::setSwordAtBreak(int param_1) {
    if (param_1 == 0) {
        for (int i = 0; i < 4; i++) {
            mSwordSphs[i].SetAtSpl((dCcG_At_Spl)0x1);
        }
        mCps.SetAtSpl((dCcG_At_Spl)0x1);
    } else {
        for (int i = 0; i < 4; i++) {
            mSwordSphs[i].SetAtSpl((dCcG_At_Spl)0xA);
        }
        mCps.SetAtSpl((dCcG_At_Spl)0xA);
        field_0xa9a = false;
    }
}

void daB_TN_c::setBodyShield() {
    daPy_py_c* player;
    bool check = true;

    player = (daPy_py_c*)daPy_getPlayerActorClass();
    if (mActionMode1 <= 1) {
        for (int i = 0; i < 3; i++) {
            mSphA[i].OnTgShield();
        }

        mSphC.OnTgShield();
        mSphC.OnTgSetBit();

    } else if (mActionMode1 < 8) {
        if (!(daPy_py_c::checkNowWolf() || player->getCutType() == daPy_py_c::CUT_TYPE_TWIRL) &&
            player->getCutCount() >= 4)
        {
            mCutFlag = true;
        } else {
            mCutFlag = false;
        }

        if (!field_0xa91 || abs((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y)) > 0x3000) {
            check = false;
        }

        int iVar3 = false;
        if (player->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP ||
            player->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B ||
            player->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A || mTimer12 != 0)
        {
            check = false;
            iVar3 = true;
        }

        if (mNextBreakPart >= 11) {
            check = false;
            mTimer10 = 0;
        }

        if (!(check || daPy_py_c::checkNowWolf() || mTimer10 != 0)) {
            for (int i = 0; i < 3; i++) {
                mSphA[i].OffTgShield();
            }
        } else {
            for (int i = 0; i < 3; i++) {
                mSphA[i].OnTgShield();
            }
        }

        if ((mActionMode1 == ACT_DAMAGEH || mNextBreakPart >= 11) && mTimer10 == 0) {
            iVar3 = true;
        }

        if (iVar3) {
            mSphC.OffTgShield();
            mSphC.OffTgSetBit();
        } else {
            mSphC.OnTgShield();
            mSphC.OnTgSetBit();
        }

    } else {
        mCutFlag = false;

        if (!(daPy_py_c::checkNowWolf() || player->getCutType() == daPy_py_c::CUT_TYPE_TWIRL ||
              player->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B ||
              player->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A ||
              player->getCutCount() < 4))
        {
            mCutFlag = true;
        } else if ((player->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
                    player->getCutType() == daPy_py_c::CUT_TYPE_TURN_LEFT) &&
                   mTimer13 == 0)
        {
            mCutFlag = true;
        }

        int chck = false;
        if (player->getCutType() != daPy_py_c::CUT_TYPE_HEAD_JUMP &&
            ((field_0xa91 == true && mCutFlag) || mTimer10 != 0 || daPy_py_c::checkNowWolf()))
        {
            chck = true;
        }

        if (chck) {
            for (int i = 0; i < 3; i++) {
                mSphA[i].OnTgShield();
            }

            return;
        }

        for (int i = 0; i < 3; i++) {
            mSphA[i].OffTgShield();
        }
    }
}

static daB_TN_c* m_attack_tn;

static int m_attack_timer;

bool daB_TN_c::checkNormalAttackAble() {
    if (mType == 1) {
        if (daPy_getPlayerActorClass()->getCutCount() != 0) {
            mTimer3 = cM_rndF(60.0f) + 30.0f;
            return 0;
        }

        if (m_attack_tn && m_attack_tn != this) {
            mTimer3 = cM_rndF(60.0f) + 30.0f;
            return 0;
        }

        if (m_attack_timer != 0) {
            mTimer3 = cM_rndF(60.0f) + 30.0f;
            return 0;
        }

        m_attack_timer = 30;
    }

    return 1;
}

u32 daB_TN_c::getCutType() {
    u32 rv = 0;

    if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
        if (daPy_getPlayerActorClass()->getCutCount() >= 4) {
            rv = 4;
        }

        if (mAtInfo.mpCollider && ((dCcD_GObjInf*)mAtInfo.mpCollider)->GetAtSpl() == 1) {
            rv = 4;
        }
    }

    switch (daPy_getPlayerActorClass()->getCutType()) {
    case daPy_py_c::CUT_TYPE_TURN_LEFT:
    case daPy_py_c::CUT_TYPE_TURN_RIGHT:
        if (mTimer13 == 0) {
            return 0x20;
        }

        return rv | 0x20;

    case daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT:
    case daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT:
        return rv | 0x20;

    case daPy_py_c::CUT_TYPE_JUMP:
    case daPy_py_c::CUT_TYPE_LARGE_JUMP:
        return rv | 0x10;

    case daPy_py_c::CUT_TYPE_TWIRL:
        return rv | 0x22;

    case daPy_py_c::CUT_TYPE_NM_VERTICAL:
    case daPy_py_c::CUT_TYPE_NM_RIGHT:
    case daPy_py_c::CUT_TYPE_FINISH_LEFT:
    case daPy_py_c::CUT_TYPE_FINISH_VERTICAL:
    case daPy_py_c::CUT_TYPE_DASH_UNK_26:
        return rv | 0x12;

    case daPy_py_c::CUT_TYPE_NM_STAB:
    case daPy_py_c::CUT_TYPE_FINISH_STAB:
        return rv | 0x02;

    case daPy_py_c::CUT_TYPE_NM_LEFT:
    case daPy_py_c::CUT_TYPE_DASH_UNK_25:
        return rv | 0x21;

    case daPy_py_c::CUT_TYPE_COMBO_STAB:
        return rv | 0x01;

    default:
        return rv | 0x03;
    }
}

void daB_TN_c::setDamage(dCcD_Sph* i_sph, int param_2) {
    u8 uVar5;

    health = 100;
    cc_at_check(this, &mAtInfo);
    if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
        mInvincibilityTimer = 20;
    } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
        mInvincibilityTimer = 20;
    } else {
        mInvincibilityTimer = 10;
    }

    if (mAtInfo.mAttackPower <= 1) {
        mInvincibilityTimer = 10;
    }

    dComIfGp_setHitMark(1, this, i_sph->GetTgHitPosP(), NULL, NULL, 0);

    if (mTimer4 != 0) {
        mNextActionMode2 ^= 1;
    } else {
        u8 uVar4 = 0;
        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
            uVar4 = getCutType() & -1;
        }

        if ((uVar4 & 3) != 0) {
            if (abs((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y)) > 0x4000) {
                if ((uVar4 & 1) != 0) {
                    uVar5 = ACTION2_1_e;
                } else {
                    uVar5 = ACTION2_0_e;
                }
            } else if ((uVar4 & 1) != 0) {
                uVar5 = ACTION2_0_e;
            } else {
                uVar5 = ACTION2_1_e;
            }
        } else if (cM_rnd() < 0.5f) {
            uVar5 = ACTION2_0_e;
        } else {
            uVar5 = ACTION2_1_e;
        }

        mNextActionMode2 = uVar5;
    }

    mTimer4 = 30;
    int iVar1 = mActionMode1;
    if (iVar1 < 8) {
        if (iVar1 != ACT_DAMAGEH && mTimer5 == 0) {
            mTimer5 = 70;
        }

        mNextBreakPart += param_2;
        setActionMode(ACT_DAMAGEH, mNextActionMode2);
    } else {
        if (iVar1 != ACT_DAMAGEL && mTimer5 == 0) {
            mTimer5 = 90;

            if (field_0x6fc > field_0x700 - 50.0f) {
                field_0x6fc -= 20;
            }
        }

        if (field_0x6fc > field_0x700 - 50.0f && mCutFlag) {
            field_0x6fc += 50;
        } else {
            field_0x6fc += 100 - health;
        }

        if (field_0x6fc >= field_0x700) {
            setActionMode(ACT_ENDING, ACTION2_0_e);
            dComIfGp_setHitMark(3, this, i_sph->GetTgHitPosP(), NULL, NULL, 0);
        } else {
            setActionMode(ACT_DAMAGEL, mNextActionMode2);
        }
    }
}

void daB_TN_c::setShieldEffect(dCcD_Sph* i_sph) {
    def_se_set(&mSound, mAtInfo.mpCollider, 42, this);
    dComIfGp_setHitMark(2, this, i_sph->GetTgHitPosP(), NULL, NULL, 0);
}

void daB_TN_c::damage_check() {
    mStts1.Move();

    for (int i = 0; i < 4; i++) {
        if (mSwordSphs[i].ChkAtHit()) {
            field_0xa9a = true;
            if (mSwordSphs[i].ChkAtShieldHit() && mSwordSphs[i].GetAtSpl() == 10) {
                setSwordAtBit(0);
                m_attack_tn = this;
                break;
            }
        }
    }

    if (mCps.ChkAtHit()) {
        field_0xa9a = true;
        if (mCps.ChkAtShieldHit() && mCps.GetAtSpl() == 10) {
            setSwordAtBit(0);
            m_attack_tn = this;
        }
    }

    if (mSphC.ChkAtHit() && !mSphC.ChkAtShieldHit()) {
        field_0xa9a = true;
        mSound.startCreatureSound(Z2SE_EN_TN_ATK_NO_DMG, 0, -1);
        mSphC.ClrAtHit();
        m_attack_tn = this;
    }

    if (mInvincibilityTimer != 0) {
        return;
    }

    if (mActionMode1 == ACT_CHANGEDEMO) {
        return;
    }

    if (mActionMode1 == ACT_ENDING) {
        return;
    }

    dCcD_Sph dStack_160;

    if (mActionMode1 <= 1) {
        mAtInfo.mpCollider = NULL;
        if (mSphC.ChkTgHit()) {
            mAtInfo.mpCollider = mSphC.GetTgHitObj();
            dStack_160 = mSphC;
        }

        for (int i = 0; i < 3; i++) {
            if (mSphA[i].ChkTgHit()) {
                mAtInfo.mpCollider = mSphA[i].GetTgHitObj();
                dStack_160 = mSphA[i];
                break;
            }
        }

        for (int i = 0; i < 3; i++) {
            if (mSphB[i].ChkTgHit()) {
                mAtInfo.mpCollider = mSphB[i].GetTgHitObj();
                dStack_160 = mSphB[i];
                break;
            }
        }

        if (mAtInfo.mpCollider != NULL) {
            setShieldEffect(&dStack_160);
            setActionMode(ACT_OPENING, ACTION2_2_e);

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG) ||
                mAtInfo.mpCollider->ChkAtType(AT_TYPE_40) ||
                mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT))
            {
                mTimer9 = 15;
            }
        }

        return;
    }

    int cut_type = daPy_getPlayerActorClass()->getCutType();
    if (mActionMode1 < 8 && mSphC.ChkTgHit() && mSphC.GetTgHitObj()->ChkAtType(18) &&
        cut_type != daPy_py_c::CUT_TYPE_HEAD_JUMP &&
        cut_type != daPy_py_c::CUT_TYPE_MORTAL_DRAW_B &&
        cut_type != daPy_py_c::CUT_TYPE_MORTAL_DRAW_A)
    {
        def_se_set(&mSound, mSphC.GetTgHitObj(), 42, this);
        field_0xaa8 = false;
        if (mSphC.GetTgHitObj()->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
            if (mActionMode1 != ACT_GUARDH) {
                return;
            }

            if (mActionMode1 == ACT_GUARDH && field_0xaa8) {
                return;
            }

            field_0xaa8 = true;
        }

        setActionMode(ACT_GUARDH, ACTION2_0_e);
        return;
    }

    mAtInfo.mpCollider = NULL;
    for (int i = 0; i < 3; i++) {
        if (mSphA[i].ChkTgHit()) {
            mAtInfo.mpCollider = mSphA[i].GetTgHitObj();
            dStack_160 = mSphA[i];
            break;
        }
    }

    if (mAtInfo.mpCollider != NULL) {
        if (cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH) {
            return;
        } else if (mActionMode1 < 8) {
            if (cut_type == daPy_py_c::CUT_TYPE_DASH_UNK_25 ||
                cut_type == daPy_py_c::CUT_TYPE_DASH_UNK_26 ||
                mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK))
            {
                setShieldEffect(&dStack_160);
                mTimer9 = 15;
                return;
            }

            if (dStack_160.ChkTgShield()) {
                if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                    if (mActionMode1 == ACT_GUARDH && field_0xaa8) {
                        return;
                    }

                    field_0xaa8 = true;
                    def_se_set(&mSound, dStack_160.GetTgHitObj(), 42, this);
                } else {
                    field_0xaa8 = false;
                    setShieldEffect(&dStack_160);
                }

                setActionMode(ACT_GUARDH, ACTION2_0_e);
                return;

            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                setShieldEffect(&dStack_160);
                mTimer9 = 0x14;
                return;
            }

            if (cut_type == daPy_py_c::CUT_TYPE_HEAD_JUMP ||
                cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B ||
                cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A)
            {
                mTimer12 = 60;
            }
        } else {
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                field_0xaa8 = true;
                def_se_set(&mSound, dStack_160.GetTgHitObj(), 42, this);
                setActionMode(ACT_GUARDL, ACTION2_0_e);
                return;
            }

            if (field_0xa91) {
                if (mCutFlag || (getCutType() & 4) != 0) {
                    setShieldEffect(&dStack_160);
                    if (mCutFlag) {
                        setActionMode(ACT_GUARDL, ACTION2_11_e);
                    } else {
                        setActionMode(ACT_YOROKE, ACTION2_0_e);
                    }

                    return;
                }

                if (mTimer10 != 0 ||
                    abs((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y)) < 0x3000 ||
                    cut_type == daPy_py_c::CUT_TYPE_DASH_UNK_25 ||
                    cut_type == daPy_py_c::CUT_TYPE_DASH_UNK_26)
                {
                    field_0xaa8 = false;
                    setShieldEffect(&dStack_160);
                    setActionMode(ACT_GUARDL, ACTION2_0_e);
                    return;
                }
            }
        }
        setDamage(&dStack_160, 1);
        return;
    }

    mAtInfo.mpCollider = NULL;
    for (int i = 0; i < 3; i++) {
        if (mSphB[i].ChkTgHit()) {
            mAtInfo.mpCollider = mSphB[i].GetTgHitObj();
            dStack_160 = mSphB[i];
            break;
        }
    }

    int bVar1 = false;
    if (abs((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y)) > 0x3000) {
        bVar1 = true;
    };

    if (mActionMode1 < 8) {
        if (!dStack_160.ChkTgShield() || mNextBreakPart >= 11) {
            bVar1 = true;
        }
    } else if (!field_0xa91) {
        bVar1 = true;
    }

    if (mActionMode1 < 8 && mSphC.ChkTgHit()) {
        mAtInfo.mpCollider = mSphC.GetTgHitObj();
        dStack_160 = mSphC;
        bVar1 = false;
    }

    if (mAtInfo.mpCollider) {
        field_0xaa8 = false;
        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG) ||
            mAtInfo.mpCollider->ChkAtType(AT_TYPE_40))
        {
            if (mActionMode1 < 8) {
                mTimer9 = 15;
            } else {
                setActionMode(ACT_GUARDL, ACTION2_10_e);
            }
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
            if (mActionMode1 < 8) {
                if (mNextBreakPart >= 11 ||
                    abs((s16)(cLib_targetAngleY(
                                  &current.pos,
                                  &dCc_GetAc(mAtInfo.mpCollider->GetAc())->current.pos) -
                              shape_angle.y)) > 0x4000)
                {
                    setDamage(&dStack_160, 2);
                } else {
                    setActionMode(ACT_GUARDH, ACTION2_0_e);
                }
            } else if (abs((s16)(cLib_targetAngleY(
                                     &current.pos,
                                     &dCc_GetAc(mAtInfo.mpCollider->GetAc())->current.pos) -
                                 shape_angle.y)) > 0x4000)
            {
                setDamage(&dStack_160, 2);
            }
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SPINNER)) {
            setShieldEffect(&dStack_160);
            if (mActionMode1 < 8) {
                setActionMode(ACT_GUARDH, ACTION2_0_e);
            } else {
                setActionMode(ACT_GUARDL, ACTION2_10_e);
            }
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
            if (mActionMode1 < 8) {
                mTimer9 = 15;
                setShieldEffect(&dStack_160);
            } else if (!bVar1 || mTimer10 != 0) {
                setShieldEffect(&dStack_160);
                setActionMode(ACT_GUARDL, ACTION2_0_e);
            } else {
                health = 100;
                cc_at_check(this, &mAtInfo);
                mNextActionMode2 = ACTION2_0_e;
                if (cM_rnd() < 0.5f) {
                    mNextActionMode2 = ACTION2_1_e;
                }

                if (mActionMode1 != ACT_DAMAGEL && mTimer5 == 0) {
                    mTimer5 = 90;
                }

                setActionMode(ACT_DAMAGEL, mNextActionMode2);
            }
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL)) {
            if (!bVar1) {
                setShieldEffect(&dStack_160);
                if (mActionMode1 < 8) {
                    setActionMode(ACT_GUARDH, ACTION2_0_e);
                } else {
                    setActionMode(ACT_GUARDL, ACTION2_10_e);
                }
            } else {
                setDamage(&dStack_160, 2);
            }
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_ARROW)) {
            if (mActionMode1 < 8) {
                setShieldEffect(&dStack_160);
            } else if (bVar1 == 0 || mTimer10 != 0) {
                setShieldEffect(&dStack_160);
                setActionMode(ACT_GUARDL, ACTION2_0_e);
            } else {
                setDamage(&dStack_160, 1);
            }
        }
    }
}

int daB_TN_c::checkMoveArea() {
    if (mType != 0) {
        return 0;
    }

    if (current.pos.absXZ() > 1700.0f) {
        return 3;
    }

    if (current.pos.absXZ() > 1600.0f) {
        return 2;
    }

    if (current.pos.absXZ() > 1500.0f) {
        return 1;
    }

    return 0;
}

bool daB_TN_c::checkMoveAngle() {
    s16 sVar1 = cM_atan2s(-current.pos.x, -current.pos.z);
    return abs((s16)(fopAcM_searchPlayerAngleY(this) - sVar1)) < 0x4000;
}

void daB_TN_c::setAttackBlurEffect(int i_data) {
    static u16 blur_effect_id_5997[4] = {
        0x87DA,
        0x87DB,
        0x87DC,
        0x87DD,
    };

    if (i_data != 0) {
        for (int i = 0; i < 4; i++) {
            mParticleKeys[i] = dComIfGp_particle_set(mParticleKeys[i], blur_effect_id_5997[i],
                                                     &current.pos, &tevStr);

            JPABaseEmitter* mBaseEmitter = dComIfGp_particle_getEmitter(mParticleKeys[i]);
            if (mBaseEmitter) {
                mBaseEmitter->setGlobalSRTMatrix(mpPartModels[13]->getBaseTRMtx());
            }
        }
    }
}

void daB_TN_c::setArmorBreakEffect(int i_idx) {
    static u16 blur_effect_id_6035[3] = {0x87DE, 0x87DF, 0x87E0};

    cXyz sp24(l_HIO.mScale, l_HIO.mScale, l_HIO.mScale);
    cXyz sp30;
    mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(tn_part_joint[i_idx]));
    mDoMtx_stack_c::multVecZero(&sp30);

    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(blur_effect_id_6035[i], &sp30, &tevStr, &shape_angle, &sp24);
    }
}

static void* s_obj_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor)) {
        if (!fpcM_IsCreating(fopAcM_GetID(i_actor)) &&
            fopAcM_checkCarryNow((fopAc_ac_c*)i_actor) == 0)
        {
            f32 mActorDistance =
                fopAcM_searchActorDistanceXZ((fopAc_ac_c*)i_actor, (fopAc_ac_c*)i_data);
            if (mActorDistance < 300.0f && fopAcM_GetSpeed((fopAc_ac_c*)i_actor).y) {
                if (fopAcM_GetName(i_actor) == PROC_Obj_Carry) {
                    return i_actor;
                }

                if (fopAcM_GetName((fopAc_ac_c*)i_actor) == PROC_NBOMB) {
                    return i_actor;
                }
            }

            if (mActorDistance < 300.0f && fopAcM_GetName(i_actor) == PROC_NBOMB) {
                daNbomb_c* mNBomb = (daNbomb_c*)i_actor;
                if (mNBomb->checkInsectBombMove((fopAc_ac_c*)i_actor)) {
                    return i_actor;
                }

                if (mNBomb->getExTime() < 10) {
                    return i_actor;
                }
            }

            if ((mActorDistance < 700.0f && fopAcM_GetName(i_actor) == PROC_ARROW) &&
                fopAcM_GetSpeedF((fopAc_ac_c*)i_actor))
            {
                s16 mAngleY = ((fopAc_ac_c*)i_actor)->current.angle.y;
                if (abs((s16)(mAngleY - fopAcM_searchActorAngleY((fopAc_ac_c*)i_actor,
                                                                 (fopAc_ac_c*)i_data))) < 0x2000)
                {
                    return i_actor;
                }
            }
        }
    }

    return NULL;
}

void daB_TN_c::demo_skip(int param_1) {
    camera_class* cameraClass = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (param_1 == 0) {
        mCamCenter.set(0.0f, -180.0f, 0.0f);
        mCamEye.set(300.0f, -200.0f, 1000.0f);
        cameraClass->mCamera.Reset(mCamCenter, mCamEye);
        cameraClass->mCamera.Start();
        cameraClass->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();

        Z2GetAudioMgr()->bgmStreamStop(0x1e);
        Z2GetAudioMgr()->subBgmStart(Z2BGM_TN_MBOSS);

        setActionMode(ACT_WAITH, ACTION2_0_e);
        mUpdateNeckAngle = true;
        mBlendStatus = 2;
        mBlend = 1.0f;
    } else if (param_1 == 1) {
        mCamCenter.set(0.0f, -239.0f, 1874.0f);
        mCamEye.set(0.0f, -202.0f, 2251.0f);
        cXyz sp28(0.0f, -350.0f, 2200.0f);
        player->setPlayerPosAndAngle(&sp28, -0x8000, 0);

        cameraClass->mCamera.Reset(mCamCenter, mCamEye);
        cameraClass->mCamera.Start();
        cameraClass->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();

        setActionMode(ACT_OPENING, ACTION2_0_e);
    }
}

int daB_TN_c::DemoSkipCallBack(void* i_this, int param_1) {
    if (i_this) {
        ((daB_TN_c*)i_this)->demo_skip(param_1);
        return 1;
    }

    return 0;
}

void daB_TN_c::executeRoomDemo() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp24 = player->current.pos;
    cXyz sp30;

    fopAcM_searchPlayerDistance(this);

    switch (mActionMode2) {
    case ACTION2_0_e:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(2);
            return;
        }

        mBlendStatus = 0;
        fopAcM_OffStatus(this, 0x4000);

        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);

        mCamCenter = dCam_getBody()->Center();
        mCamEye = dCam_getBody()->Eye();

        mCamFovY = dCam_getBody()->Fovy();
        mActionMode2 = ACTION2_2_e;
        mTimer1 = 30;
        break;

    case ACTION2_2_e:
        if (mTimer1 == 0) {
            if (!daPy_py_c::checkNowWolf()) {
                mTimer1 = 70;
            } else {
                mTimer1 = 30;
            }

            player->changeOriginalDemo();
            player->changeDemoMoveAngle(-0x8000);
            sp24.set(0.0f, -350.0f, 1850.0f);
            player->changeDemoPos0(&sp24);
            player->changeDemoMode(2, 0, 0, 0);
            mActionMode2 = ACTION2_3_e;
        }
        break;

    case ACTION2_3_e:
        cLib_addCalcPos2(&mCamCenter, sp24, 0.1f, 1.0f);

        if (mTimer1 == 0) {
            player->changeDemoMoveAngle(-0x8000);

            if (!daPy_py_c::checkNowWolf()) {
                sp24.set(0.0f, -350.0f, 1923.0f);
            } else {
                sp24.set(0.0f, -350.0f, 2173.0f);
            }

            player->changeDemoPos0(&sp24);
            player->changeDemoMode(2, 0, 0, 0);
            sp24.z += 500.0f;
            player->setPlayerPosAndAngle(&sp24, -0x8000, 0);

            mTimer1 = 30;
            mActionMode2 = ACTION2_10_e;
            field_0x720 = -0x7000;
            mCamEye.x = cM_ssin(field_0x720) * 400.0f;
            mCamEye.y = -170.0f;
            mCamEye.z = cM_scos(field_0x720) * 400.0f + 1900.0f;
            mCamCenter.set(sp24.x, sp24.y + 150.0f, sp24.z);
            return;
        }
        break;

    case ACTION2_10_e:
    case ACTION2_11_e:
        sp30.set(sp24.x, sp24.y + 150.0f, sp24.z);
        cLib_addCalcPos2(&mCamCenter, sp30, 0.5f, 10.0f);
        cLib_addCalcAngleS2(&field_0x720, -0x4000, 16, 32);
        cLib_addCalc2(&mCamEye.y, -250.0f, 16.0f, 0.5f);
        mCamEye.x = cM_ssin(field_0x720) * 400.0f;
        mCamEye.z = cM_scos(field_0x720) * 400.0f + 1900.0f;

        if (mTimer1 == 0) {
            if (mActionMode2 == ACTION2_10_e) {
                player->changeDemoMode(20, 0, 0, 0);
                mTimer1 = 120;
                mActionMode2 = ACTION2_11_e;
            } else if (mActionMode2 == ACTION2_11_e) {
                if (!daPy_py_c::checkNowWolf()) {
                    player->changeDemoMode(47, 2, 0, 0);
                }

                mActionMode2 = ACTION2_15_e;
                mTimer1 = 150;

                if (!daPy_py_c::checkNowWolf()) {
                    mCamEye.set(0.0f, -200.0f, 2400.0f);
                } else {
                    mCamEye.set(0.0f, -265.0f, 2400.0f);
                }

                field_0x720 = 0x5c00;
                mCamCenter = mCamEye;
                mCamCenter.y += 400.0f * cM_ssin(field_0x720);
                mCamCenter.z += 400.0f * cM_scos(field_0x720);
            }
        }
        break;

    case ACTION2_15_e:
        mCamCenter = mCamEye;
        mCamCenter.y += 400.0f * cM_ssin(field_0x720);
        mCamCenter.z += 400.0f * cM_scos(field_0x720);
        cLib_addCalcAngleS2(&field_0x720, 0x7800, 16, 64);

        if (mTimer1 == 0) {
            mActionMode2 = ACTION2_16_e;
            mTimer1 = 70;
            field_0x724 = 1200.0f;
        }
        break;

    case ACTION2_16_e:
        cLib_chaseF(&field_0x724, 1200.0f, 50.0f);
        sp30.set(0.0f, 423.0f, -1841.0f);
        cLib_addCalcPos2(&mCamCenter, sp30, 0.3f, field_0x724);
        sp30.set(0.0f, 370.0f, -1442.0f);
        cLib_addCalcPos2(&mCamEye, sp30, 0.3f, field_0x724);

        if (mTimer1 == 0) {
            mCamCenter.set(0.0f, -239.0f, 1874.0f);
            mCamEye.set(0.0f, -202.0f, 2251.0f);
            camera->mCamera.Reset(mCamCenter, mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();

            setActionMode(ACT_OPENING, ACTION2_0_e);
            return;
        }
    }

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
    dComIfGp_getEvent().onSkipFade();
    dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 1);
}

void daB_TN_c::executeOpening() {
    cXyz sp7c, sp34;
    daPy_py_c* player = daPy_getPlayerActorClass();
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    fopAcM_searchPlayerDistance(this);

    switch (mActionMode2) {
    case ACTION2_0_e:
        speedF = 0.0f;
        setBck(BCK_TNA_WAIT_OP, 2, 3.0f, 1.0f);
        mActionMode2 = ACTION2_1_e;
        // [[fallthrough]]

    case ACTION2_1_e:
        sp7c = player->current.pos;
        if (sp7c.absXZ() < 2200.0f) {
            sp34.set(0.0f, -350.0f, 2250.0f);
            if (sp7c.absXZ(sp34) > 1300.0f) {
                mActionMode2 = ACTION2_2_e;
            }
        }

        return;

    case ACTION2_2_e:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(2);
            return;
        }

        dComIfGs_onOneZoneSwitch(14, fopAcM_GetRoomNo(this));

        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);
        mTimer1 = 30;

        Z2GetAudioMgr()->bgmStreamPrepare(0x2000037);
        Z2GetAudioMgr()->bgmStreamPlay();

        mActionMode2 = ACTION2_3_e;
        mCamFovY = 60.0f;
        field_0x724 = 380.0f;
        field_0x720 = 0x1000;
        mCamCenter.set(current.pos.x, current.pos.y + 220.0f, current.pos.z);
        mCamEye.set(mCamCenter.x + field_0x724 * cM_ssin(field_0x720), -210.0f,
                    mCamCenter.z + field_0x724 * cM_scos(field_0x720));

        mpModelMorf2->setFrame(15.0f);
        player->changeOriginalDemo();
        sp7c.set(150.0f, -400.0f, 600.0f);
        player->setPlayerPosAndAngle(&sp7c, -0x7000, 0);
        player->changeDemoMode(4, 1, 0, 0);
        break;

    case ACTION2_3_e:
        cLib_addCalcAngleS2(&field_0x720, 0x400, 8, 48);
        mCamCenter.set(current.pos.x, current.pos.y + 220.0f, current.pos.z);
        mCamEye.set(mCamCenter.x + field_0x724 * cM_ssin(field_0x720), -210.0f,
                    mCamCenter.z + field_0x724 * cM_scos(field_0x720));

        if (mTimer1 == 0) {
            setBck(BCK_TNA_TURN_OP, 0, 20.0f, 1.0f);
            mActionMode2 = ACTION2_4_e;
            mBlendStatus = 1;
            mBlend = 0.0f;
        }
        break;

    case ACTION2_4_e:
        if (mpModelMorf2->checkFrame(111.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(148.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->getFrame() < 80.0f) {
            cLib_addCalcAngleS2(&field_0x720, -0x400, 8, 48);
            mCamCenter.set(current.pos.x, current.pos.y + 220.0f, current.pos.z);
        } else {
            cLib_addCalc2(&field_0x724, 250.0f, 8.0f, 5.0f);
            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
            mDoMtx_stack_c::multVecZero(&sp34);
            cLib_addCalcPos2(&mCamCenter, sp34, 0.5f, 5.0f);
        }

        mCamEye.set(mCamCenter.x + field_0x724 * cM_ssin(field_0x720), -210.0f,
                    mCamCenter.z + field_0x724 * cM_scos(field_0x720));
        if (mpModelMorf2->isStop()) {
            setBck(BCK_TNA_WAIT, 2, 3.0f, 1.0f);
            mTimer1 = 60;
            mActionMode2 = ACTION2_10_e;
        }
        break;

    case ACTION2_10_e:
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&sp34);
        cLib_addCalcPos2(&mCamCenter, sp34, 0.5f, 5.0f);

        if (mTimer1 < 30) {
            sp34.set(0.0f, -350.0f, 400.0f);
            cLib_addCalcPos2(&mCamEye, sp34, 0.5f, 5.0f);
        }

        if (mTimer1 == 0) {
            setBck(BCK_TNA_ATACK_OP, 0, 10.0f, 1.0f);
            mActionMode2 = ACTION2_14_e;
        }
        break;

    case ACTION2_14_e:
        if (mpModelMorf2->checkFrame(33.0f) || mpModelMorf2->checkFrame(105.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        } else if (mpModelMorf2->checkFrame(57.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_SWD_L, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_TN_V_KIAI_OP, -1);
        } else if (mpModelMorf2->checkFrame(62.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        } else if (mpModelMorf2->checkFrame(98.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_SWD_L, 0, -1);
        }

        if (mpModelMorf2->getFrame() < 30.0f) {
            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
            mDoMtx_stack_c::multVecZero(&sp34);
            cLib_addCalcPos2(&mCamCenter, sp34, 0.5f, 5.0f);
            sp34.set(0.0f, -350.0f, 400.0f);
            cLib_addCalcPos2(&mCamEye, sp34, 0.5f, 5.0f);
        } else if (mpModelMorf2->getFrame() < 57.0f) {
            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(14));
            mDoMtx_stack_c::multVecZero(&sp34);
            sp34.y -= 50.0f;
            cLib_addCalcPos2(&mCamCenter, sp34, 0.5f, 3.0f);
            sp34.set(0.0f, -350.0f, 400.0f);
            cLib_addCalcPos2(&mCamEye, sp34, 0.5f, 5.0f);
        } else {
            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(14));
            mDoMtx_stack_c::multVecZero(&sp34);
            sp34.y -= 50.0f;
            cLib_addCalcPos2(&mCamCenter, sp34, 1.0f, 30.0f);
            sp34.set(0.0f, -350.0f, 500.0f);
            cLib_addCalcPos2(&mCamEye, sp34, 0.5f, 3.0f);
        }

        if (mpModelMorf2->checkFrame(58.0f) || mpModelMorf2->checkFrame(98.0f)) {
            dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
            mVibrationTimer = 7;
        }

        if (mpModelMorf2->checkFrame(126.0f)) {
            mActionMode2 = ACTION2_17_e;
            mTimer1 = 30;
        }
        break;

    case ACTION2_17_e:
        mCamCenter.set(2.0f, -225.0f, -109.0f);
        sp34.set(0.0f, -350.0f, 430.0f);
        cLib_addCalcPos2(&mCamEye, sp34, 0.5f, 100.0f);
        if (mpModelMorf2->checkFrame(160.0f)) {
            mActionMode2 = ACTION2_20_e;
            player->changeDemoMode(23, 0, 0, 0);
        }
        break;

    case ACTION2_20_e:
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&sp34);
        cLib_addCalcPos2(&mCamCenter, sp34, 0.5f, 5.0f);
        sp34.set(0.0f, -300.0f, 1000.0f);
        cLib_addCalcPos2(&mCamEye, sp34, 0.1f, 20.0f);

        if (mpModelMorf2->checkFrame(220.0f)) {
            camera->mCamera.Reset(mCamCenter, mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();

            Z2GetAudioMgr()->subBgmStart(Z2BGM_TN_MBOSS);

            setActionMode(ACT_WAITH, ACTION2_0_e);
            mUpdateNeckAngle = true;
            mBlendStatus = 2;
            mBlend = 0.0f;
            return;
        }
    }

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
    dComIfGp_getEvent().onSkipFade();
    dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 0);
}

void daB_TN_c::executeWaitH() {
    fopAcM_searchPlayerAngleY(this);
    f32 mPlayerDistance = fopAcM_searchPlayerDistance(this);

    switch (mActionMode2) {
    case ACTION2_0_e:
        speedF = 0.0f;
        setBck(BCK_TNA_WAIT, 2, 3.0f, 1.0f);
        mActionMode2 = ACTION2_1_e;
        // [[fallthrough]]

    case ACTION2_1_e:
        if (mPlayerDistance < 1500.0f) {
            if (mStayNo2Flag) {
                Z2GetAudioMgr()->subBgmStart(Z2BGM_TN_MBOSS_LV9);
            }

            setActionMode(ACT_CHASEH, ACTION2_0_e);
        }
    }
}

void daB_TN_c::setAwaitSound() {
    if (mpModelMorf2->checkFrame(17.0f)) {
        mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
    }

    if (mpModelMorf2->checkFrame(36.0f)) {
        mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
    }
}

void daB_TN_c::executeChaseH() {
    f32 mPlayerDistance = fopAcM_searchPlayerDistance(this);
    s16 sVar4 = fopAcM_searchPlayerAngleY(this);
    s16 sVar5 = fopAcM_searchPlayerAngleY(this) - cM_atan2s(-current.pos.x, -current.pos.z);
    int mMoveArea = checkMoveArea();

    switch (mActionMode2) {
    case ACTION2_0_e:
    case ACTION2_76_e:
        if (mMoveArea == 3) {
            if (checkMoveAngle()) {
                mActionMode2 = ACTION2_100_e;
            } else {
                mActionMode2 = ACTION2_103_e;
            }
        } else {
            if (mMoveArea != 0 && mPlayerDistance > 800.0f) {
                mPlayerDistance = 400.0f;
            }

            if (mPlayerDistance < 200.0f) {
                mActionMode2 = ACTION2_3_e;
            } else if (mPlayerDistance >= 800.0f) {
                if (mActionMode2 == ACTION2_0_e) {
                    mActionMode2 = ACTION2_10_e;
                } else {
                    mActionMode2 = ACTION2_8_e;
                }
            } else if (mPlayerDistance >= 400.0f) {
                mActionMode2 = ACTION2_1_e;
            } else {
                mActionMode2 = ACTION2_5_e;
            }
        }

        setSwordAtBit(0);
        field_0xa91 = true;
        break;

    case ACTION2_8_e:
    case ACTION2_10_e:
        if (mActionMode2 == ACTION2_8_e) {
            setBck(BCK_TNA_WALK, 2, 3.0f, 1.0f);
        } else {
            setBck(BCK_TNA_WALK, 2, 10.0f, 1.0f);
        }

        mActionMode2 = ACTION2_9_e;
        speedF = 4.3f;
        // [[fallthrough]]

    case ACTION2_9_e:
        if (mpModelMorf2->checkFrame(23.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(0.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        cLib_addCalcAngleS(&shape_angle.y, sVar4, 4, 0x400, 16);
        current.angle.y = shape_angle.y;

        if (mPlayerDistance < 700.0f) {
            mActionMode2 = ACTION2_11_e;
        }
        break;

    case ACTION2_1_e:
    case ACTION2_11_e:
        if (mActionMode2 == ACTION2_1_e) {
            setBck(BCK_TNA_WAIT_B_1, 2, 3.0f, 1.0f);
        } else {
            setBck(BCK_TNA_WAIT_B_1, 2, 10.0f, 1.0f);
        }

        mActionMode2 = ACTION2_2_e;
        speedF = 2.3f;
        current.angle.y = shape_angle.y;
        // [[fallthrough]]

    case ACTION2_2_e:
        setAwaitSound();
        cLib_addCalcAngleS(&shape_angle.y, sVar4, 4, 0x400, 16);
        current.angle.y = shape_angle.y;

        if (mPlayerDistance > 800.0f) {
            if (mMoveArea == 0) {
                mActionMode2 = ACTION2_10_e;
            }
        } else if (mPlayerDistance < 300.0f) {
            speedF = 0.0f;
            mActionMode2 = ACTION2_5_e;
        }
        break;

    case ACTION2_3_e:
        setBck(BCK_TNA_WAIT_B_1, 2, 3.0f, -1.0f);
        mActionMode2 = ACTION2_4_e;
        current.angle.y = shape_angle.y + 0x8000;
        mWalkDir = 0;
        // [[fallthrough]]

    case ACTION2_4_e:
        setAwaitSound();
        speedF = 2.3f;
        cLib_addCalcAngleS(&shape_angle.y, sVar4, 4, 0x400, 16);
        current.angle.y = shape_angle.y + 0x8000;

        if (mPlayerDistance < 200.0f && mTimer3 == 0) {
            mWalkDir++;
            if (mWalkDir > 60) {
                if (mType == 1) {
                    mTimer3 = l_HIO.mTimer3NormalType0;
                } else {
                    mTimer3 = l_HIO.mTimer3NormalType1;
                }

                setActionMode(ACT_ATTACKH, ACTION2_0_e);
            }
        } else {
            mWalkDir = 0;
        }

        if (mPlayerDistance > 300.0f) {
            speedF = 0.0f;
            mActionMode2 = ACTION2_5_e;
        }
        break;

    case ACTION2_5_e:
        if (cM_rnd() < 0.5f) {
            mActionMode2 = ACTION2_6_e;
            setBck(BCK_TNA_WAIT_B_2, 2, 3.0f, 1.0f);
            current.angle.y = shape_angle.y + 0x4000;
        } else {
            mActionMode2 = ACTION2_7_e;
            setBck(BCK_TNA_WAIT_B_2, 2, 3.0f, -1.0f);
            current.angle.y = shape_angle.y + -0x4000;
        }

        mTimer1 = cM_rndFX(20.0f) + 60.0f;
        speedF = 2.3f;
        break;

    case ACTION2_6_e:
        setAwaitSound();
        cLib_addCalcAngleS(&shape_angle.y, sVar4, 4, 0x400, 16);
        current.angle.y = shape_angle.y + 0x4000;

        if (mTimer1 == 0) {
            if (cM_rnd() < 0.5f) {
                setBck(BCK_TNA_WAIT_B_2, 2, 3.0f, -1.0f);
                current.angle.y = shape_angle.y + -0x4000;
                mActionMode2 = ACTION2_7_e;
            }

            mTimer1 = cM_rndFX(20.0f) + 60.0f;
        }

        if (mPlayerDistance < 200.0f) {
            mActionMode2 = ACTION2_3_e;
        }

        if (mPlayerDistance > 400.0f) {
            mActionMode2 = ACTION2_1_e;
        }
        break;

    case ACTION2_7_e:
        setAwaitSound();
        cLib_addCalcAngleS(&shape_angle.y, sVar4, 4, 0x400, 16);
        current.angle.y = shape_angle.y + -0x4000;

        if (mTimer1 == 0) {
            if (cM_rnd() < 0.5f) {
                setBck(BCK_TNA_WAIT_B_2, 2, 3.0f, 1.0f);
                current.angle.y = shape_angle.y + 0x4000;
                mActionMode2 = ACTION2_6_e;
            }
            mTimer1 = cM_rndFX(20.0f) + 90.0f;
        }

        if (mPlayerDistance < 200.0f) {
            mActionMode2 = ACTION2_3_e;
        }

        if (mPlayerDistance > 400.0f) {
            mActionMode2 = ACTION2_1_e;
        }
        break;

    case ACTION2_100_e:
        setBck(BCK_TNA_WAIT_B_1, 2, 10.0f, 1.0f);
        mActionMode2 = ACTION2_102_e;
        speedF = 2.3f;
        current.angle.y = shape_angle.y;
        // [[fallthrough]]

    case ACTION2_102_e:
        setAwaitSound();
        cLib_addCalcAngleS(&shape_angle.y, sVar4, 4, 0x400, 16);
        cLib_addCalcAngleS(&current.angle.y, cM_atan2s(-current.pos.x, -current.pos.z), 4, 0x400,
                           16);

        if (!checkMoveAngle()) {
            mActionMode2 = ACTION2_103_e;
        }

        if (mMoveArea <= 1) {
            mActionMode2 = ACTION2_76_e;
        }
        break;

    case ACTION2_103_e:
        mPlayerDistance = 3.0f;

        if ((u8)checkBck(BCK_TNA_WALK)) {
            mPlayerDistance = 10.0f;
        }

        if (abs(sVar5) < 0x2000) {
            if (cM_rnd() < 0.5f) {
                mActionMode2 = ACTION2_106_e;
                setBck(BCK_TNA_WAIT_B_2, 2, mPlayerDistance, 1.0f);
            } else {
                mActionMode2 = ACTION2_107_e;
                setBck(BCK_TNA_WAIT_B_2, 2, mPlayerDistance, -1.0f);
            }
        } else if (sVar5 < 0) {
            mActionMode2 = ACTION2_106_e;
            setBck(BCK_TNA_WAIT_B_2, 2, mPlayerDistance, 1.0f);
        } else {
            mActionMode2 = ACTION2_107_e;
            setBck(BCK_TNA_WAIT_B_2, 2, mPlayerDistance, -1.0f);
        }

        mTimer1 = cM_rndFX(20.0f) + 60.0f;
        // [[fallthrough]]

    case ACTION2_106_e:
    case ACTION2_107_e:
        setAwaitSound();
        speedF = 2.3f;
        cLib_addCalcAngleS(&shape_angle.y, sVar4, 4, 0x400, 16);

        if (mActionMode2 == ACTION2_106_e) {
            if (mMoveArea == 3) {
                current.angle.y = shape_angle.y + 0x6000;
            } else if (mMoveArea == 2) {
                current.angle.y = shape_angle.y + 0x4000;
            } else {
                current.angle.y = shape_angle.y + 0x3000;
            }
        } else if (mMoveArea == 3) {
            current.angle.y = shape_angle.y + -0x6000;
        } else if (mMoveArea == 2) {
            current.angle.y = shape_angle.y + -0x4000;
        } else {
            current.angle.y = shape_angle.y + -0x3000;
        }

        if (mTimer1 == 0 && mpModelMorf2->checkFrame(0.0f)) {
            if (abs(sVar5) > 0x7800) {
                mTimer1 = cM_rndFX(20.0f) + 90.0f;
                if (cM_rnd() < 0.5f) {
                    if (mActionMode2 == ACTION2_106_e) {
                        setBck(BCK_TNA_WAIT_B_2, 2, 3.0f, -1.0f);
                        mActionMode2 = ACTION2_107_e;
                    } else {
                        setBck(BCK_TNA_WAIT_B_2, 2, 3.0f, 1.0f);
                        mActionMode2 = ACTION2_106_e;
                    }
                }
            } else if (abs(sVar5) < 0x7000) {
                if (mActionMode2 == ACTION2_106_e) {
                    if (sVar5 > 0) {
                        setBck(BCK_TNA_WAIT_B_2, 2, 3.0f, -1.0f);
                        mActionMode2 = ACTION2_107_e;
                    }
                } else if (sVar5 < 0) {
                    setBck(BCK_TNA_WAIT_B_2, 2, 3.0f, 1.0f);
                    mActionMode2 = ACTION2_106_e;
                }
            }
        }

        if (checkMoveAngle()) {
            mActionMode2 = ACTION2_100_e;
        }

        if (mMoveArea < 1) {
            mActionMode2 = ACTION2_76_e;
        }
        break;
    }

    if (mActionMode2 < 100) {
        checkStartAttackH();
        if (mMoveArea == 3) {
            if (checkMoveAngle()) {
                mActionMode2 = ACTION2_100_e;
            } else {
                mActionMode2 = ACTION2_103_e;
            }
        }
    }
}

void daB_TN_c::checkStartAttackH() {
    f32 mPlayerDistance = fopAcM_searchPlayerDistance(this);
    fopAcM_searchPlayerAngleY(this);
    s16 sVar1 = fopAcM_searchPlayerAngleY(this) - shape_angle.y;

    if (mPlayerDistance < 400.0f && abs(sVar1) < 0x3000) {
        if (mNextBreakPart >= 11) {
            if (mType == 1) {
                if (mTimer3 > 40) {
                    mTimer3 = cM_rndF(20.0f) + 15.0f;
                }
            } else if (mTimer3 > 10) {
                mTimer3 = 10;
            }

            if (mTimer3 == 0 && checkNormalAttackAble()) {
                if (mType == 1) {
                    mTimer3 = l_HIO.mTimer3NormalType0;
                } else {
                    mTimer3 = l_HIO.mTimer3NormalType1;
                }

                setActionMode(ACT_ATTACKH, ACTION2_0_e);
            }
        } else if (mPlayerDistance < 200.0f) {
            setActionMode(ACT_ATTACKSHIELDH, ACTION2_0_e);
        } else {
            if (mPlayerDistance < 250.0f) {
                if (mType == 1) {
                    if (mTimer3 > 60) {
                        mTimer3 = cM_rndF(20.0f) + 40.0f;
                    }
                } else if (mTimer3 > 30) {
                    mTimer3 = 30;
                }
            }

            if (mTimer3 == 0 && checkNormalAttackAble()) {
                if (mType == 1) {
                    mTimer3 = l_HIO.mTimer3NormalType0;
                } else {
                    mTimer3 = l_HIO.mTimer3NormalType1;
                }

                setActionMode(ACT_ATTACKH, ACTION2_0_e);
            }
        }
    }
}

void daB_TN_c::executeAttackH() {
    f32 frame;
    s16 playerAngleY = fopAcM_searchPlayerAngleY(this);
    f32 playerDistance = fopAcM_searchPlayerDistance(this);

    switch (mActionMode2) {
    case ACTION2_0_e:
        speedF = 0.0f;

        if (playerDistance < 250.0f) {
            mActionMode2 = ACTION2_1_e;
            setBck(BCK_TNA_ATACK_A, 0, 3.0f, 1.0f);
        } else if (playerDistance < 350.0f) {
            mActionMode2 = ACTION2_2_e;
            setBck(BCK_TNA_ATACK_B, 0, 3.0f, 1.0f);
        } else if (cM_rnd() < 0.5f) {
            mActionMode2 = ACTION2_1_e;
            setBck(BCK_TNA_ATACK_A, 0, 3.0f, 1.0f);
        } else {
            mActionMode2 = ACTION2_2_e;
            setBck(BCK_TNA_ATACK_B, 0, 3.0f, 1.0f);
            setAttackBlurEffect(1);
        }

        field_0xa91 = false;
        break;

    case ACTION2_1_e:
        if (mpModelMorf2->getFrame() >= 20.0f && mpModelMorf2->getFrame() < 25.0f) {
            mUpdateModelTimer = 3;
            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(15));
            mDoMtx_stack_c::transM(40.0f, 0.0f, 70.0f);
            mDoMtx_stack_c::YrotM(-0x8000);
            MTXCopy(mDoMtx_stack_c::get(), mNewTRMtx);
        }

        if (mpModelMorf2->checkFrame(18.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_SWD_L, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_TN_V_KIAI, -1);
        }

        if (mpModelMorf2->checkFrame(23.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_L, 0, -1);
        }

        if (mpModelMorf2->checkFrame(72.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->getFrame() <= 30.0f) {
            cLib_addCalcAngleS(&shape_angle.y, playerAngleY, 8, 0x400, 16);
        }

        frame = mpModelMorf2->getFrame();
        if (frame >= 18.0f && frame < 29.0f) {
            mSphCSmallFlag = true;
        }

        if (mpModelMorf2->checkFrame(18.0f)) {
            setSwordAtBit(1);
        }

        if (mpModelMorf2->checkFrame(29.0f)) {
            setSwordAtBit(0);
        }

        if (mpModelMorf2->isStop()) {
            setActionMode(ACT_CHASEH, ACTION2_0_e);
            field_0xa91 = true;
        }
        break;

    case ACTION2_2_e:
        setAttackBlurEffect(1);
        if (mpModelMorf2->checkFrame(11.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        } else if (mpModelMorf2->checkFrame(19.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_KIAI_AGE, -1);
        } else if (mpModelMorf2->checkFrame(22.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_SWD_L, 0, -1);
        } else if (mpModelMorf2->checkFrame(23.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_L, 0, -1);
        } else if (mpModelMorf2->checkFrame(66.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->getFrame() <= 22.0f) {
            cLib_addCalcAngleS(&shape_angle.y, playerAngleY, 8, 0x400, 16);
        }

        if (mpModelMorf2->getFrame() >= 22.0f && mpModelMorf2->getFrame() < 30.0f) {
            mChkCoHitOK = true;
            mSphCSmallFlag = true;
        }

        if (mpModelMorf2->checkFrame(22.0f)) {
            setSwordAtBit(1);
        }

        if (mpModelMorf2->checkFrame(30.0f)) {
            setSwordAtBit(0);
        }

        if (mpModelMorf2->checkFrame(26.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_SWD_IMPCT, 0, -1);
            dComIfGp_getVibration().StartShock(6, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (mpModelMorf2->isStop()) {
            setActionMode(ACT_CHASEH, ACTION2_0_e);
            field_0xa91 = true;
        }
    }
}

void daB_TN_c::executeAttackShieldH() {
    switch (mActionMode2) {
    case ACTION2_0_e:
        speedF = 0.0f;
        setBck(BCK_TNA_ATACK_SHIELD, 0, 3.0f, 1.0f);
        mActionMode2 = ACTION2_1_e;
        break;

    case ACTION2_1_e:
        if (mpModelMorf2->checkFrame(4.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_ATK_SLD, -1);
        }

        if (mpModelMorf2->checkFrame(8.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_L, 0, -1);
        }

        if (mpModelMorf2->checkFrame(21.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(5.0f)) {
            mSphC.OnAtSetBit();
        }

        if (mpModelMorf2->checkFrame(9.0f)) {
            mSphC.OffAtSetBit();
        }

        if (mpModelMorf2->isStop()) {
            setActionMode(ACT_ATTACKH, ACTION2_0_e);
        }
    }
}

void daB_TN_c::executeGuardH() {
    cXyz sp18;
    switch (mActionMode2) {
    case ACTION2_0_e:
        setSwordAtBit(0);
        mSphC.OffAtSetBit();
        field_0xa91 = true;

        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(18));
        mDoMtx_stack_c::multVecZero(&sp18);
        current.pos.set(sp18.x, current.pos.y, sp18.z);

        mActionMode2 = ACTION2_1_e;
        speedF = 0.0f;

        if (field_0xaa8) {
            shape_angle.y = fopAcM_searchPlayerAngleY(this);
            setBck(BCK_TNA_GUARD_DAMAGE, 0, 0.0f, 1.0f);
        } else {
            cLib_chaseAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x2000);
            setBck(BCK_TNA_GUARD, 0, 0.0f, 1.0f);
        }
        break;

    case ACTION2_1_e:
        if (field_0xaa8) {
            onHeadLockFlg();
        } else {
            // Obtained 4 secret techinques
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[341])) {
                attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;
            }
        }

        if (mpModelMorf2->isStop() &&
            daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_HEAD_JUMP)
        {
            setActionMode(ACT_CHASEH, ACTION2_0_e);
        }
    }
}

void daB_TN_c::executeDamageH() {
    s16 mPlayerAngleY = fopAcM_searchPlayerAngleY(this);
    daPy_py_c* player = daPy_getPlayerActorClass();
    field_0xa91 = false;

    if (mTimer5 == 0) {
        field_0xa91 = true;
        mTimer10 = 30;
    }

    if (mType == 0) {
        if (player->getCutCount() >= 3) {
            field_0xa91 = true;
        }
    } else if (mTimer10 != 0) {
        field_0xa91 = true;
    }

    int part_idx;
    switch (mActionMode2) {
    case ACTION2_0_e:
    case ACTION2_1_e:
        setSwordAtBit(0);
        part_idx = mNextBreakPart;
        if (part_idx >= 12) {
            setActionMode(ACT_CHANGEDEMO, ACTION2_0_e);
        } else {
            if (player->getCutType() != daPy_py_c::CUT_TYPE_HEAD_JUMP) {
                setBreakPart(part_idx);
            } else {
                setBreakHeadPart(part_idx);
            }

            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(18));
            cXyz sp1c;
            mDoMtx_stack_c::multVecZero(&sp1c);
            current.pos.set(sp1c.x, current.pos.y, sp1c.z);

            if (mActionMode2 == ACTION2_0_e) {
                setBck(BCK_TNA_DAMAGE_L, 0, 0.0f, 1.0f);
            } else {
                setBck(BCK_TNA_DAMAGE_R, 0, 0.0f, 1.0f);
            }

            mSound.startCreatureVoice(Z2SE_EN_TN_V_DMG, -1);
            speedF = 0.0f;
            mActionMode2 = ACTION2_2_e;

            cLib_chaseAngleS(&shape_angle.y, mPlayerAngleY, 0x1800);

            if (mType == 1 && mNextBreakPart < 11 && mCutFlag) {
                field_0xa91 = true;
                mTimer10 = 40;
            }
        }
        break;

    case ACTION2_2_e:
        if (mpModelMorf2->checkFrame(3.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(35.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->isStop()) {
            setActionMode(ACT_CHASEH, ACTION2_0_e);
            field_0xa91 = true;
        }
    }
}

void daB_TN_c::executeChangeDemo() {
    cXyz sp28, sp34, sp40;
    daPy_py_c* player = daPy_getPlayerActorClass();
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    switch (mActionMode2) {
    case ACTION2_0_e:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(2);
            return;
        }

        Z2GetAudioMgr()->changeSubBgmStatus(1);

        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);

        mTimer1 = 30;
        mCamFovY = 55.0f;
        field_0x724 = 380.0f;
        field_0x720 = 0x1000;
        mCamCenter.set(0.0f, -179.0f, -342.0f);
        mCamEye.set(0.0f, -300.0f, 350.0f);

        player->changeOriginalDemo();
        sp28.set(100.0f, -400.0f, 1000.0f);
        player->setPlayerPosAndAngle(&sp28, -0x8000, 0);
        player->changeDemoMode(4, 1, 0, 0);

        speedF = 0.0f;
        setBck(BCK_TNA_DAMAGE_LAST, 0, 3.0f, 1.0f);

        mSound.startCreatureVoice(Z2SE_EN_TN_V_DMG_2, -1);

        mActionMode2 = ACTION2_100_e;

        for (int i = 0; i < 16; i++) {
            if (mStates[i] >= 3 && mStates[i] <= 5 && std::abs(mPositions[i].x) < 200.0f) {
                if (mPositions[i].x < 0.0f) {
                    mPositions[i].x -= 200.0f;
                } else {
                    mPositions[i].x += 200.0f;
                }
            }
        }

        return;

    case ACTION2_1_e:
    case ACTION2_100_e:
    case ACTION2_101_e:
        if (mActionMode2 == ACTION2_100_e) {
            current.pos.set(0.0f, -400.0f, -350.0f);
            shape_angle.y = 0;
            mActionMode2 = ACTION2_101_e;

        } else {
            if (mActionMode2 == ACTION2_101_e) {
                setBreakPart(mNextBreakPart);
                mActionMode2 = ACTION2_1_e;
            }

            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
            mDoMtx_stack_c::multVecZero(&mCamCenter);
        }

        mCamEye.set(0.0f, -300.0f, 0.0f);

        if (mpModelMorf2->checkFrame(3.0f) || mpModelMorf2->checkFrame(26.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(15.0f) || mpModelMorf2->checkFrame(37.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->isStop()) {
            setBck(BCK_TNB_JUMP_B_1, 0, 3.0f, 1.0f);
            current.angle.y = shape_angle.y + 0x10000 + -0x8000;
            mActionMode2 = ACTION2_2_e;
        }
        break;

    case ACTION2_2_e:
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mCamCenter);
        mCamEye.set(0.0f, -300.0f, 0.0f);

        if (mpModelMorf2->checkFrame(4.0f)) {
            speedF = 35.0f;
            speed.y = 4.0f;
            gravity = -1.0f;
        }

        if (mpModelMorf2->checkFrame(6.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_JUMP, -1);
        }

        if (mpModelMorf2->isStop() && mAcch.ChkGroundHit()) {
            setBck(BCK_TNB_JUMP_B_2, 0, 5.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
            mActionMode2 = ACTION2_3_e;
        }
        break;

    case ACTION2_3_e:
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mCamCenter);
        mCamEye.set(0.0f, -300.0f, 0.0f);
        cLib_chaseF(&speedF, 0.0f, 7.0f);
        if (mpModelMorf2->checkFrame(4.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->isStop()) {
            speedF = 0.0f;
            setBck(BCK_TNB_THROW, 0, 3.0f, 1.0f);
            mActionMode2 = ACTION2_4_e;
        }
        break;

    case ACTION2_4_e:
        if (mpModelMorf2->checkFrame(11.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(19.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_THROW, -1);
        }

        if (mpModelMorf2->checkFrame(22.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mCamCenter);
        mCamEye.set(0.0f, -300.0f, 0.0f);

        if (mpModelMorf2->checkFrame(23.0f)) {
            mStates[13] = 2;
        }

        if (mpModelMorf2->checkFrame(33.0f)) {
            mActionMode2 = ACTION2_10_e;
            mPositions[13].z = -200.0f;
            mCamEye.set(250.0f, -330.0f, 1250.0f);
            mCamCenter = mPositions[13];
        }
        break;

    case ACTION2_10_e:
        mCamCenter = mPositions[13];

        if (mpModelMorf2->isStop()) {
            player->changeDemoMode(16, 2, 0, 0);
            setBck(BCK_TNB_WAIT, 2, 3.0f, 1.0f);
            mTimer1 = 60;
            mActionMode2 = ACTION2_11_e;
        }
        break;

    case ACTION2_11_e:
        sp40 = mCamEye;
        sp34.set(100.0f, -330.0f, 1250.0f);
        cLib_addCalcPos2(&mCamEye, sp34, 0.5f, 10.0f);
        mCamCenter += mCamEye - sp40;
        sp28 = player->current.pos;
        sp28.y += 150.0f;
        cLib_addCalcPos2(&mCamCenter, sp28, 0.5f, 30.0f);

        if (mTimer1 == 45) {
            player->changeDemoMode(25, 0, 0, 0);
        }

        if (mTimer1 == 15) {
            mTimer1 = 30;
            mActionMode2 = ACTION2_12_e;
        }
        break;

    case ACTION2_12_e:
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&sp34);
        cLib_addCalcPos2(&mCamCenter, sp34, 0.1f, 60.0f);

        if (mTimer1 == 0) {
            mTimer1 = 40;
            mActionMode2 = ACTION2_13_e;
        }
        break;

    case ACTION2_13_e:
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&sp34);
        sp34.y -= 100.0f;
        cLib_addCalcPos2(&mCamCenter, sp34, 0.1f, 30.0f);
        sp34.set(0.0f, -380.0f, -270.0f);
        cLib_addCalcPos2(&mCamEye, sp34, 0.1f, 70.0f);

        if (mTimer1 == 0) {
            setBck(BCK_TNB_PULL, 0, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_TN_V_PULL_OUT, -1);
            mActionMode2 = ACTION2_15_e;
            sp28.set(-100.0f, -400.0f, 300.0f);
            player->setPlayerPosAndAngle(&sp28, 0x7000, 0);
        }
        break;

    case ACTION2_15_e:
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&sp34);
        sp34.y -= 100.0f;
        cLib_addCalcPos2(&mCamCenter, sp34, 0.1f, 30.0f);
        sp34.set(0.0f, -380.0f, -270.0f);
        cLib_addCalcPos2(&mCamEye, sp34, 0.1f, 70.0f);

        if (mpModelMorf2->checkFrame(10.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_PULL_OUT, 0, -1);
            mStates[15] = 1;
            mStates[14] = 2;
            mpModelMorf1->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_tn", BCK_TNB_SWORD_B_PULL_B), 0, 0.0f, 1.0f,
                                 0.0f, -1.0f);
        }

        if (mpModelMorf2->isStop()) {
            for (int i = 0; i < 4; i++) {
                mSwordSphs[i].OffCoSetBit();
            }
            mCps.OffCoSetBit();
            setBck(BCK_TNB_WAIT, 2, 3.0f, 1.0f);
            mTimer1 = 30;
            mActionMode2 = ACTION2_16_e;
            return;
        }
        break;

    case ACTION2_16_e:
        if (mTimer1 == 0) {
            camera->mCamera.Reset(mCamCenter, mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            setActionMode(ACT_CHASEL, ACTION2_0_e);
            initChaseL(16);
            mSphC.OffAtSetBit();
        }
        break;
    }

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
}

void daB_TN_c::executeZakoChangeDemo() {
    s16 mPlayerAngleY = fopAcM_searchPlayerAngleY(this);

    switch (mActionMode2) {
    case ACTION2_0_e:
        if (mStayNo2Flag) {
            Z2GetAudioMgr()->changeSubBgmStatus(1);
        }

        mTimer1 = 30;
        speedF = 0.0f;
        setBck(BCK_TNA_DAMAGE_LAST, 0, 3.0f, 1.0f);

        mSound.startCreatureVoice(Z2SE_EN_TN_V_DMG_2, -1);

        setBreakPart(mNextBreakPart);
        mActionMode2 = ACTION2_1_e;
        // [[fallthrough]]

    case ACTION2_1_e:
        if (mpModelMorf2->checkFrame(3.0f) || mpModelMorf2->checkFrame(26.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(15.0f) || mpModelMorf2->checkFrame(37.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 8, 0x100, 16);

        if (mpModelMorf2->isStop()) {
            setBck(BCK_TNB_JUMP_B_1, 0, 3.0f, 1.0f);
            current.angle.y = shape_angle.y + 0x8000;
            mActionMode2 = ACTION2_2_e;
        }
        break;

    case ACTION2_2_e:
        if (mpModelMorf2->checkFrame(4.0f)) {
            speedF = 45.0f;
            speed.y = 5.0f;
            gravity = -1.0f;
        }

        if (mpModelMorf2->checkFrame(6.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_JUMP, -1);
        }

        if (!mpModelMorf2->isStop()) {
            return;
        }

        if (!mAcch.ChkGroundHit()) {
            return;
        }

        setBck(BCK_TNB_JUMP_B_2, 0, 5.0f, 1.0f);

        mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        mActionMode2 = ACTION2_3_e;
        return;

    case ACTION2_3_e:
        cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 8, 0x800, 16);
        cLib_chaseF(&speedF, 0.0f, 9.0f);

        if (mpModelMorf2->checkFrame(4.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->isStop()) {
            speedF = 0.0f;
            setBck(BCK_TNB_THROW, 0, 3.0f, 1.0f);
            mActionMode2 = ACTION2_4_e;
        }
        break;

    case ACTION2_4_e:
        if (mpModelMorf2->checkFrame(11.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(19.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_THROW, -1);
        }

        if (mpModelMorf2->checkFrame(22.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->getFrame() < 23.0f) {
            cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 8, 0x800, 16);
        }

        if (mpModelMorf2->checkFrame(23.0f)) {
            mStates[13] = 2;
            setSwordAtBit(1);
        }

        if (mpModelMorf2->isStop()) {
            setBck(BCK_TNB_PULL, 0, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_TN_V_PULL_OUT, -1);
            mActionMode2 = ACTION2_5_e;
        }
        break;

    case ACTION2_5_e:
        if (mpModelMorf2->checkFrame(10.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_PULL_OUT, 0, -1);
            mStates[15] = 1;
            mStates[14] = 2;
            mpModelMorf1->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_tn", BCK_TNB_SWORD_B_PULL_B), 0, 0.0f, 1.0f,
                                 0.0f, -1.0f);
        }

        if (mpModelMorf2->isStop()) {
            setActionMode(ACT_CHASEL, ACTION2_0_e);
            initChaseL(3);
            mSphC.OffAtSetBit();
        }
    }
}

void daB_TN_c::setWalkDir() {
    f32 mPlayerDistance = fopAcM_searchPlayerDistance(this);

    if (checkMoveArea()) {
        s16 sVar1 = cM_atan2s(-current.pos.x, -current.pos.z);
        s16 mPlayerAngleY = fopAcM_searchPlayerAngleY(this) - sVar1;
        if (abs(mPlayerAngleY) < 0x2000) {
            mWalkDir = 0;
        } else if (abs(mPlayerAngleY) > 0x6000) {
            mWalkDir = 1;
        } else {
            if (abs(mPlayerAngleY) < 0x4000) {
                mWalkDir = 0;
            } else {
                mWalkDir = 1;
            }

            if (mPlayerAngleY < 0) {
                if (mActionMode2 == ACTION2_5_e && mTimer1 > 30) {
                    mTimer1 = 30;
                }
            } else if (mActionMode2 == ACTION2_4_e && mTimer1 > 30) {
                mTimer1 = 30;
            }
        }
    } else {
        if (mPlayerDistance > 800.0f) {
            mTimer1 = 0;
        }

        if (mWalkDir == 0) {
            if (mTimer3 != 0) {
                if (mPlayerDistance < 500.0f) {
                    mWalkDir = 1;
                }
            } else if (mPlayerDistance < 300.0f) {
                mWalkDir = 1;
            }
        } else if (mWalkDir == 1) {
            if (mTimer3 != 0) {
                if (mPlayerDistance > 550.0f) {
                    mWalkDir = 0;
                }
            } else if (mPlayerDistance > 350.0f) {
                mWalkDir = 0;
            }

            if (mPlayerDistance < 200.0f) {
                mWalkDir = 2;
            }
        } else if (mPlayerDistance > 250.0f) {
            mWalkDir = 1;
        }
    }
}

void daB_TN_c::initChaseL(int param_1) {
    mActionMode2 = param_1;
    speedF = 0.0f;

    switch (mActionMode2) {
    case ACTION2_2_e:
        setBck(BCK_TNB_WAIT, 2, 10.0f, 1.0f);
        mTimer1 = 30;
        break;

    case ACTION2_3_e:
        setBck(BCK_TNB_WALK, 2, 10.0f, 1.0f);
        speedF = 4.7f;
        break;

    case ACTION2_4_e:
        if ((u8)checkBck(BCK_TNB_WAIT_B_1) == 0) {
            setBck(BCK_TNB_WAIT_B_1, 2, 10.0f, 1.0f);
        }

        current.angle.y = shape_angle.y + 0x4000;
        mTimer1 = cM_rndFX(20.0f) + 90.0f;
        speedF = 4.8f;
        mWalkDir = 1;
        break;

    case ACTION2_5_e:
        if ((u8)checkBck(BCK_TNB_WAIT_B_2) == 0) {
            setBck(BCK_TNB_WAIT_B_2, 2, 10.0f, 1.0f);
        }

        current.angle.y = shape_angle.y + -0x4000;
        mTimer1 = cM_rndFX(20.0f) + 90.0f;
        speedF = 4.8f;
        mWalkDir = 1;
        break;

    case ACTION2_6_e:
        setBck(BCK_TNB_JUMP_B_1, 0, 3.0f, 1.0f);
        break;

    case ACTION2_7_e:
        mpModelMorf2->setPlaySpeed(1.0f);
        mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        break;

    case ACTION2_8_e:
    case ACTION2_17_e:
        setBck(BCK_TNB_JUMP_F_1, 0, 3.0f, 1.0f);
        break;

    case ACTION2_9_e:
        setBck(BCK_TNB_JUMP_F_2, 0, 5.0f, 1.0f);
        mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        break;

    case ACTION2_10_e:
    case ACTION2_11_e:
        setBck(BCK_TNB_JUMP_R_1, 0, 3.0f, 1.0f);
        break;

    case ACTION2_12_e:
        setBck(BCK_TNB_JUMP_R_2, 0, 3.0f, 1.0f);
        mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        speedF = 15.0f;
        break;

    case ACTION2_13_e:
    case ACTION2_14_e:
        setBck(BCK_TNB_JUMP_L_1, 0, 3.0f, 1.0f);
        break;

    case ACTION2_15_e:
        setBck(BCK_TNB_JUMP_L_2, 0, 3.0f, 1.0f);
        mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        speedF = 15.0f;
        break;

    case ACTION2_16_e:
        setBck(BCK_TNB_WALK, 2, 10.0f, 1.0f);
        speedF = 4.7;
        mTimer1 = 30;
    }
}

bool daB_TN_c::checkAttackAble() {
    if (fopAcM_searchPlayerDistance(this) < 500.0f &&
        abs((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y)) < 0x3000)
    {
        return true;
    }

    return false;
}

bool daB_TN_c::checkNextMove() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 mPlayerDistance = fopAcM_searchPlayerDistance(this);
    s16 mPlayerAngleY = fopAcM_searchPlayerAngleY(this) - shape_angle.y;
    s16 isAttackAble = checkAttackAble();
    int iVar1 = 0;
    s16 sVar7 = current.angle.y;

    fopAc_ac_c* mActor = (fopAc_ac_c*)fpcM_Search(s_obj_sub, this);
    if (mActor != NULL) {
        sVar7 = fopAcM_searchActorAngleY(this, mActor);
        iVar1 = 1;
    }

    if (dComIfGp_checkPlayerStatus0(0, 0x4000) && player->getHookshotTopPos() &&
        player->getHookshotTopPos()->absXZ(current.pos) < 300.0f)
    {
        sVar7 = cLib_targetAngleY(&current.pos, player->getHookshotTopPos());
        iVar1 = 1;
    }

    if (player->getIronBallCenterPos() && !player->checkIronBallGroundStop() &&
        player->getIronBallCenterPos()->absXZ(current.pos) < 300.0f)
    {
        sVar7 = cLib_targetAngleY(&current.pos, player->getIronBallCenterPos());
        iVar1 = 1;
    }

    if (daPy_py_c::getThrowBoomerangActor() &&
        daPy_py_c::getThrowBoomerangActor()->current.pos.absXZ(current.pos) < 300.0f)
    {
        sVar7 = cLib_targetAngleY(&current.pos, &daPy_py_c::getThrowBoomerangActor()->current.pos);
        iVar1 = 1;
    }

    if (iVar1) {
        if ((s16)(sVar7 - shape_angle.y) < 0) {
            initChaseL(13);
        } else {
            initChaseL(10);
        }

        mTimer7 = 10;
        return 1;
    }

    if (isAttackAble) {
        if (player->checkFrontRoll() && mPlayerDistance < 250.0f) {
            setActionMode(ACT_ATTACKSHIELDL, ACTION2_10_e);
            return 1;
        }

        if (player->getCutType() != daPy_py_c::CUT_TYPE_JUMP && mPlayerDistance < 200.0f) {
            setActionMode(ACT_ATTACKSHIELDL, ACTION2_0_e);
            return 1;
        }
    }

    if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && !player->checkCutJumpCancelTurn() &&
        mPlayerDistance < 350.0f)
    {
        if ((field_0xa9d & 1) == 0) {
            field_0xa9d |= 1;

            if (player->speedF < 28.0f) {
                initChaseL(6);
            } else {
                if (mPlayerAngleY < 0) {
                    initChaseL(14);
                } else {
                    initChaseL(11);
                }

                mTimer7 = 10;
            }
            return 1;
        }

    } else {
        field_0xa9d &= ~0x1;
    }

    if (checkMoveArea() == 3) {
        return 0;
    }

    if (player->checkCutCharge() && isAttackAble) {
        if ((field_0xa9d & 2) == 0) {
            field_0xa9d |= 2;
            setActionMode(ACT_ATTACKL, ACTION2_10_e);
            return 1;
        }
    } else {
        field_0xa9d &= ~0x2;
    }

    if (mTimer3 == 0 && abs(mPlayerAngleY) < 0x3000 && mPlayerDistance > 450.0f &&
        mPlayerDistance < 700.0f)
    {
        if (cM_rnd() < 0.5f) {
            initChaseL(8);
            return 1;
        }

        if (mType == 1) {
            mTimer3 = cM_rndF(30.0f) + 10.0f;
        } else {
            mTimer3 = 10;
        }
    }

    if (player->getCutCount() != 0) {
        if (isAttackAble && (field_0xa9d & 4) == 0) {
            field_0xa9d |= 4;
            if (cM_rnd() < 0.4f && mPlayerDistance < 450.0f && mPlayerDistance > 200.0f) {
                if (cM_rnd() < 0.5f) {
                    initChaseL(0xe);
                } else {
                    initChaseL(0xb);
                }

                mTimer7 = 10;
                return 1;
            }
        }
    } else {
        field_0xa9d &= ~0x4;
    }

    if (isAttackAble && mPlayerDistance < 350.0f) {
        if (mPlayerDistance < 250.0f) {
            if (mType == 1) {
                if (mTimer3 > 90) {
                    mTimer3 = cM_rndF(30.0f) + 60.0f;
                }
            } else if (mTimer3 > 30) {
                mTimer3 = 30;
            }
        }

        if (mTimer3 == 0 && checkNormalAttackAble()) {
            setActionMode(ACT_ATTACKL, ACTION2_0_e);
            return 1;
        }
    }

    if (mPlayerDistance < 100.0f) {
        if (!isAttackAble) {
            initChaseL(6);
            return 1;
        }

        if (mTimer3 > 5) {
            mTimer3 = 5;
        }

        if (mTimer3 == 0 && checkNormalAttackAble()) {
            setActionMode(ACT_ATTACKL, ACTION2_0_e);
            return 1;
        }
    }

    return 0;
}

void daB_TN_c::executeChaseL() {
    f32 mPlayerDistance = fopAcM_searchPlayerDistance(this);
    s16 mPlayerAngleY = fopAcM_searchPlayerAngleY(this);
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (player->getCutCount() <= 1) {
        field_0xa9c++;
    } else {
        field_0xa9c = 0;
    }

    switch (mActionMode2) {
    case ACTION2_0_e:
        field_0xa9d = 0;
        // [[fallthrough]]

    case ACTION2_1_e:
        speedF = 0.0f;
        if (checkMoveArea()) {
            if (cM_rnd() < 0.5f) {
                initChaseL(4);
            } else {
                initChaseL(5);
            }
        } else if (mPlayerDistance > 800.0f) {
            initChaseL(3);
        } else if (cM_rnd() < 0.5f) {
            initChaseL(4);
        } else {
            initChaseL(5);
        }

        setSwordAtBit(0);
        field_0xa91 = true;
        break;

    case ACTION2_3_e:
        if (checkNextMove() == 0) {
            if (mpModelMorf2->checkFrame(16.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
            }

            if (mpModelMorf2->checkFrame(33.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
            }

            cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 4, 0x400, 16);
            current.angle.y = shape_angle.y;

            if (mPlayerDistance < 700.0f) {
                initChaseL(1);
            }

            if (checkMoveArea()) {
                if (cM_rnd() < 0.5f) {
                    initChaseL(4);
                } else {
                    initChaseL(5);
                }
            }
        }
        break;

    case ACTION2_16_e:
        if (mpModelMorf2->checkFrame(16.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(33.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 4, 0x400, 16);
        current.angle.y = shape_angle.y;

        if (mTimer1 == 0) {
            initChaseL(17);
        }
        break;

    case ACTION2_2_e:
        if (mTimer1 == 0) {
            initChaseL(1);
        }
        break;

    case ACTION2_4_e:
        if (checkNextMove() == 0) {
            if (mpModelMorf2->checkFrame(13.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
            }

            if (mpModelMorf2->checkFrame(27.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
            }

            mUpdateWaistAngle = true;
            setWalkDir();
            if (mWalkDir == 0) {
                cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY + -0x2000, 4, 0x400, 16);
            } else if (mWalkDir == 1) {
                cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 4, 0x400, 16);
            } else {
                cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY + 0x2000, 4, 0x400, 16);
            }

            current.angle.y = shape_angle.y + 0x4000;

            if (mTimer1 == 0) {
                initChaseL(1);
            }
        }
        break;

    case ACTION2_5_e:
        if (!checkNextMove()) {
            if (mpModelMorf2->checkFrame(13.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
            }

            if (mpModelMorf2->checkFrame(27.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
            }

            mUpdateWaistAngle = true;
            setWalkDir();
            if (mWalkDir == 0) {
                cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY + 0x2000, 4, 0x400, 16);
            } else if (mWalkDir == 1) {
                cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 4, 0x400, 16);
            } else {
                cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY + -0x2000, 4, 0x400, 16);
            }

            current.angle.y = shape_angle.y + -0x4000;

            if (mTimer1 == 0) {
                initChaseL(1);
            }
        }
        break;

    case ACTION2_6_e:
        cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 4, 0x400, 16);
        current.angle.y = shape_angle.y + 0x8000;

        if ((u8)checkBck(BCK_TNB_JUMP_B_1)) {
            if (mpModelMorf2->checkFrame(4.0f)) {
                speedF = 50.0f;
                speed.y = 19.0f;
                gravity = -4.0f;
            }

            if (mpModelMorf2->checkFrame(6.0f)) {
                mSound.startCreatureVoice(Z2SE_EN_TN_V_JUMP, -1);
            }

            if (mpModelMorf2->isStop()) {
                setBck(BCK_TNB_JUMP_B_2, 0, 5.0f, 0.0f);
            }
        } else if (mAcch.ChkGroundHit()) {
            initChaseL(7);
        }
        break;

    case ACTION2_7_e:
        cLib_chaseF(&speedF, 0.0f, 6.0f);

        if (mpModelMorf2->checkFrame(4.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->isStop()) {
            if (!checkAttackAble() || mTimer13 != 0 || !checkNormalAttackAble()) {
                initChaseL(1);
            } else if (mPlayerDistance < 200.0f) {
                setActionMode(ACT_ATTACKSHIELDL, ACTION2_0_e);
            } else {
                setActionMode(ACT_ATTACKL, ACTION2_10_e);
            }
        }
        break;

    case ACTION2_8_e:
    case ACTION2_17_e:
        cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 4, 0x400, 16);
        current.angle.y = shape_angle.y;

        if (mpModelMorf2->checkFrame(4.0f)) {
            if (mActionMode2 == ACTION2_8_e) {
                speedF = 35.0f;
                speed.y = 4.0f;
            } else {
                speedF = 45.0f;
                speed.y = 5.0f;
            }

            gravity = -1.0f;
        }

        if (mpModelMorf2->checkFrame(6.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_JUMP, -1);
        }

        if (mpModelMorf2->isStop() && mAcch.ChkGroundHit()) {
            if (!checkAttackAble() || !checkNormalAttackAble()) {
                initChaseL(9);
            } else {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);

                if (mPlayerDistance < 200.0f) {
                    setActionMode(ACT_ATTACKSHIELDL, ACTION2_0_e);
                } else if (cM_rnd() < 0.5f) {
                    setActionMode(ACT_ATTACKL, ACTION2_10_e);
                } else {
                    setActionMode(ACT_ATTACKL, ACTION2_0_e);
                }
            }
        }
        break;

    case ACTION2_9_e:
        cLib_chaseF(&speedF, 0.0f, 7.0f);

        if (mpModelMorf2->checkFrame(4.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->isStop()) {
            initChaseL(1);
        }
        break;

    case ACTION2_10_e:
    case ACTION2_11_e:
    case ACTION2_13_e:
    case ACTION2_14_e:
        cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 4, 0x400, 16);

        if (mActionMode2 == ACTION2_11_e || mActionMode2 == ACTION2_10_e) {
            current.angle.y = shape_angle.y + -0x4000;
        } else {
            current.angle.y = shape_angle.y + 0x4000;
        }

        if (mpModelMorf2->checkFrame(7.0f)) {
            speedF = 33.0f;
        } else if (mpModelMorf2->checkFrame(6.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_JUMP, -1);
            speedF = 35.0f;
        } else if (mpModelMorf2->checkFrame(5.0f)) {
            speedF = 40.0f;
        } else if (mpModelMorf2->checkFrame(4.0f)) {
            speedF = 45.0f;
            speed.y = 5.0f;
            gravity = -1.0f;
        }

        if (mpModelMorf2->isStop() && mAcch.ChkGroundHit()) {
            if (mActionMode2 == ACTION2_11_e || mActionMode2 == ACTION2_10_e) {
                initChaseL(12);
            } else {
                initChaseL(15);
            }
        }
        break;

    case ACTION2_12_e:
    case ACTION2_15_e:
        if (mpModelMorf2->checkFrame(5.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        cLib_chaseF(&speedF, 0.0f, 7.0f);

        if (mpModelMorf2->isStop()) {
            if (!checkAttackAble() || !checkNormalAttackAble()) {
                initChaseL(1);
            } else if (mPlayerDistance < 200.0f) {
                setActionMode(ACT_ATTACKSHIELDL, ACTION2_0_e);
            } else {
                setActionMode(ACT_ATTACKL, ACTION2_10_e);
            }
        }
    }
}

void daB_TN_c::executeAttackL() {
    s16 mPlayerAngleY = fopAcM_searchPlayerAngleY(this);
    f32 mPlayerDistance = fopAcM_searchPlayerDistance(this);

    speedF = 0.0f;
    current.angle.y = shape_angle.y;

    switch (mActionMode2) {
    case ACTION2_0_e:
    case ACTION2_20_e:
    case ACTION2_10_e:
        mTimer1 = 0;
        mActionMode2Copy = mActionMode2;

        if (daPy_getPlayerActorClass()->checkNowWolf()) {
            mTimer3 = l_HIO.mTimer3Wolf;
        } else if (mType == 0) {
            mTimer3 = l_HIO.mTimer3HumanType0;
        } else {
            mTimer3 = l_HIO.mTimer3HumanType1;
        }

        if (mPlayerDistance < 350.0f) {
            if (mActionMode2Copy == ACTION2_20_e) {
                mActionMode2 = ACTION2_4_e;
                setBck(BCK_TNB_WAIT, 2, 3.0f, 1.0f);
                mTimer1 = 3;
            } else {
                mActionMode2 = ACTION2_2_e;
                setBck(BCK_TNB_ATACK_B, 0, 3.0f, 1.0f);
            }
        } else {
            if (mActionMode2Copy == ACTION2_20_e || mActionMode2Copy == ACTION2_10_e) {
                mActionMode2 = ACTION2_3_e;
                setBck(BCK_TNB_WAIT, 2, 3.0f, 1.0f);
                if (mActionMode2Copy == ACTION2_10_e) {
                    mTimer1 = 5;
                } else {
                    mTimer1 = 10;
                }
            } else {
                mActionMode2 = ACTION2_1_e;
                setBck(BCK_TNB_ATACK_A, 0, 3.0f, 1.0f);
            }
        }

        mWalkDir = 0;
        break;

    case ACTION2_11_e:
        mActionMode2Copy = mActionMode2;
        if (daPy_getPlayerActorClass()->checkNowWolf()) {
            mTimer3 = l_HIO.mTimer3Wolf;
        } else if (mType == 0) {
            mTimer3 = l_HIO.mTimer3HumanType0;
        } else {
            mTimer3 = l_HIO.mTimer3HumanType1;
        }

        if (cM_rnd() < 0.5f) {
            mActionMode2 = ACTION2_2_e;
            setBck(BCK_TNB_ATACK_B, 0, 10.0f, 1.0f);
        } else {
            mActionMode2 = ACTION2_1_e;
            setBck(BCK_TNB_ATACK_A, 0, 7.0f, 1.0f);
        }

        mWalkDir = 0;
        return;

    case ACTION2_1_e:
        if (mpModelMorf2->checkFrame(7.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_SWD_S_TRST, 0, -1);
        }

        if (mpModelMorf2->checkFrame(8.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_THRUST, -1);
        }

        if (mpModelMorf2->checkFrame(10.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_L, 0, -1);
        }

        if (mpModelMorf2->checkFrame(15.0f)) {
            mActionMode2 = ACTION2_101_e;
            mTimer1 = l_HIO.mTimer1Action1;
            mpModelMorf2->setPlaySpeed(0.0f);
        }

    case ACTION2_101_e:
        if (mActionMode2Copy != 0 && mpModelMorf2->getFrame() < 8.0f) {
            cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 4, 0x800, 16);
        }

        if (mpModelMorf2->checkFrame(35.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->checkFrame(7.0f)) {
            setSwordAtBit(1);
        } else if (mpModelMorf2->checkFrame(14.0f)) {
            setSwordAtBit(0);
        }

        if (mpModelMorf2->checkFrame(8.0f)) {
            field_0xa91 = false;
        } else if (mpModelMorf2->checkFrame(30.0f)) {
            field_0xa91 = true;
        }

        if (mActionMode2 == ACTION2_101_e && mTimer1 == 0) {
            mpModelMorf2->setPlaySpeed(1.0f);
            if (mpModelMorf2->isStop()) {
                setActionMode(ACT_CHASEL, ACTION2_0_e);
            }
        }
        break;

    case ACTION2_2_e:
        if (mActionMode2Copy != 0) {
            if (mpModelMorf2->checkFrame(40.0f)) {
                mActionMode2 = ACTION2_102_e;
                mTimer1 = l_HIO.mTimer1Action2;
                mpModelMorf2->setPlaySpeed(0.0f);
            }
        } else {
            mActionMode2 = ACTION2_102_e;
        }
        // [[fallthrough]]

    case ACTION2_102_e:
        if (mpModelMorf2->checkFrame(14.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_V_2_SWD_1, 0, -1);
        } else if (mpModelMorf2->checkFrame(15.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_SWD_S, 0, -1);
        } else if (mpModelMorf2->checkFrame(25.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_2_SWD_2, -1);
        } else if (mpModelMorf2->checkFrame(26.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_SWD_S, 0, -1);
        } else if (mpModelMorf2->checkFrame(28.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_L, 0, -1);
        } else if (mpModelMorf2->checkFrame(52.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mWalkDir != 0 && mpModelMorf2->getFrame() < 10.0f) {
            speedF = 10.0f;
        }

        if (mpModelMorf2->getFrame() < 20.0f) {
            cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 8, 0x400, 16);
        }

        if (mpModelMorf2->checkFrame(13.0f)) {
            setSwordAtBit(1);
            setSwordAtBreak(0);
        } else if (mpModelMorf2->checkFrame(38.0f)) {
            setSwordAtBit(0);
            setSwordAtBreak(1);
        }

        if (mActionMode2Copy == ACTION2_0_e) {
            if (mpModelMorf2->checkFrame(20.0f)) {
                field_0xa91 = false;
            } else if (mpModelMorf2->checkFrame(53.0f)) {
                field_0xa91 = true;
            }
        }

        if (mActionMode2Copy == ACTION2_0_e && mPlayerDistance < 800.0f &&
            daPy_getPlayerActorClass()->getDamageWaitTimer() == 0)
        {
            if (mpModelMorf2->checkFrame(30.0f)) {
                mWalkDir = 10;
            }

            if (mpModelMorf2->checkFrame(41.0f) && mWalkDir == 10) {
                setBck(BCK_TNB_ATACK_A, 0, 5.0f, 1.0f);
                setSwordAtBit(0);
                setSwordAtBreak(1);
                mActionMode2 = ACTION2_1_e;
            }
        }

        if (mActionMode2 != 102) {
            return;
        }

        if (mTimer1 != 0) {
            return;
        }

        mpModelMorf2->setPlaySpeed(1.0f);
        if (!mpModelMorf2->isStop()) {
            return;
        }

        setActionMode(ACT_CHASEL, ACTION2_0_e);
        return;

    case ACTION2_3_e:
        if (mTimer1 != 0) {
            return;
        }

        mActionMode2 = ACTION2_1_e;
        setBck(BCK_TNB_ATACK_A, 0, 3.0f, 1.0f);
        return;

    case ACTION2_4_e:
        if (mTimer1 != 0) {
            return;
        }

        mActionMode2 = ACTION2_2_e;
        setBck(BCK_TNB_ATACK_B, 0, 3.0f, 1.0f);
        return;
    }
}

void daB_TN_c::executeAttackShieldL() {
    s16 mPlayerAngleY = fopAcM_searchPlayerAngleY(this);
    fopAcM_searchPlayerDistance(this);

    switch (mActionMode2) {
    case ACTION2_10_e:
    case ACTION2_0_e:
        mActionMode2Copy = mActionMode2;
        if (daPy_getPlayerActorClass()->checkNowWolf()) {
            mTimer3 = l_HIO.mTimer3Wolf;
        } else if (mType == 0) {
            mTimer3 = l_HIO.mTimer3HumanType0;
        } else {
            mTimer3 = l_HIO.mTimer3HumanType1;
        }

        speedF = 0.0f;
        setBck(BCK_TNB_ATACK_SHIELD, 0, 3.0f, 1.0f);
        mActionMode2 = ACTION2_1_e;
        break;

    case ACTION2_1_e:
        if (mpModelMorf2->checkFrame(8.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_KICK, -1);
        }

        if (mpModelMorf2->checkFrame(26.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->getFrame() < 10.0f) {
            cLib_addCalcAngleS(&shape_angle.y, mPlayerAngleY, 4, 0x800, 16);
        }

        if (mpModelMorf2->checkFrame(8.0f)) {
            mSphC.OnAtSetBit();
        }

        if (mpModelMorf2->checkFrame(12.0f)) {
            mSphC.OffAtSetBit();
        }

        if (mpModelMorf2->isStop()) {
            if (checkAttackAble() && checkNormalAttackAble()) {
                setActionMode(ACT_ATTACKL, ACTION2_20_e);
            } else {
                setActionMode(ACT_CHASEL, ACTION2_0_e);
            }
        }
        break;
    }
}

void daB_TN_c::executeGuardL() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 mPlayerAngleY = fopAcM_searchPlayerAngleY(this) - shape_angle.y;
    f32 mPlayerDistance = fopAcM_searchPlayerDistance(this);

    switch (mActionMode2) {
    case ACTION2_0_e:
    case ACTION2_11_e:
    case ACTION2_10_e:
        setSwordAtBit(0);
        setSwordAtBreak(1);
        mSphC.OffAtSetBit();
        speedF = 0.0f;

        if (mActionMode2 == ACTION2_0_e) {
            field_0xa91 = true;

            u32 mCutType = getCutType();
            if ((mCutType & 16) != 0) {
                setBck(BCK_TNB_GUARD_A, 0, 3.0f, 1.0f);
            } else if ((mCutType & 32) != 0) {
                setBck(BCK_TNB_GUARD_B, 0, 3.0f, 1.0f);
            } else if (cM_rnd() < 0.5f) {
                setBck(BCK_TNB_GUARD_A, 0, 3.0f, 1.0f);
            } else {
                setBck(BCK_TNB_GUARD_B, 0, 3.0f, 1.0f);
            }

            speedF = 0.0f;
            mActionMode2 = ACTION2_1_e;
        } else {
            if (mActionMode2 == ACTION2_11_e) {
                mActionMode2 = ACTION2_3_e;
            } else {
                mActionMode2 = ACTION2_2_e;
            }

            field_0xa91 = false;
            setBck(BCK_TNB_DOWN, 0, 3.0f, 1.0f);
            speedF = 17.0f;
        }

        if (mTimer10 != 0) {
            shape_angle.y = fopAcM_searchPlayerAngleY(this);
        } else {
            cLib_chaseAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x2000);
        }

        current.angle.y = shape_angle.y + 0x8000;
        break;

    case ACTION2_1_e:
        field_0xa91 = false;

        if (player->getCutType() != daPy_py_c::CUT_TYPE_HEAD_JUMP && abs(mPlayerAngleY) < 0x3000) {
            field_0xa91 = true;
        }

        cLib_chaseF(&speedF, 0.0f, 1.0f);

        if (!mpModelMorf2->isStop()) {
            break;
        }

        if (player->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP) {
            break;
        }

        setActionMode(ACT_CHASEL, ACTION2_0_e);
        break;

    case ACTION2_2_e:
    case ACTION2_3_e:
        cLib_chaseF(&speedF, 0.0f, 1.0f);
        if (abs(mPlayerAngleY) < 0x3000) {
            field_0xa91 = true;
        } else {
            field_0xa91 = false;
        }

        if (mpModelMorf2->checkFrame(4.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TN_V_PULL_OUT, -1);
        }

        if (mpModelMorf2->checkFrame(37.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mActionMode2 == ACTION2_3_e) {
            if (!mpModelMorf2->checkFrame(15.0f)) {
                break;
            }

            field_0xa91 = true;
            if (checkAttackAble() && mPlayerDistance < 200.0f) {
                setActionMode(ACT_ATTACKSHIELDL, ACTION2_0_e);
                break;
            }

            setActionMode(ACT_ATTACKL, ACTION2_11_e);
            break;
        }

        if (!mpModelMorf2->isStop()) {
            break;
        }

        setActionMode(ACT_CHASEL, ACTION2_0_e);
        break;
    }
}

void daB_TN_c::executeDamageL() {
    s16 mPlayerAngleY = fopAcM_searchPlayerAngleY(this);
    daPy_py_c* player = daPy_getPlayerActorClass();

    field_0xa91 = false;

    if (mType == 0) {
        if (daPy_py_c::checkNowWolf()) {
            field_0xa91 = true;
        }

        if (mCutFlag && player->getCutType() != daPy_py_c::CUT_TYPE_TURN_RIGHT &&
            player->getCutType() != daPy_py_c::CUT_TYPE_TURN_LEFT)
        {
            field_0xa91 = true;
        }

        if (mTimer5 == 0) {
            field_0xa91 = true;
            mTimer10 = 30;
        }
    } else {
        field_0xa91 = false;

        if (mTimer5 == 0) {
            field_0xa91 = true;
            mTimer10 = 30;
        }

        if (mTimer10 != 0 || player->checkNowWolf()) {
            field_0xa91 = true;
        }
    }

    switch (mActionMode2) {
    case ACTION2_0_e:
    case ACTION2_1_e:
        setSwordAtBit(0);
        setSwordAtBit(0);
        setSwordAtBreak(1);
        mSphC.OffAtSetBit();

        if (mActionMode2 == ACTION2_0_e) {
            setBck(BCK_TNB_DAMAGE_L, 0, 3.0f, 1.0f);
        } else {
            setBck(BCK_TNB_DAMAGE_R, 0, 3.0f, 1.0f);
        }

        mSound.startCreatureVoice(Z2SE_EN_TN_V_DMG, -1);
        speedF = 0.0f;
        mActionMode2 += 2;

        cLib_chaseAngleS(&shape_angle.y, mPlayerAngleY, 0x1800);

        if (mType == 1 && mCutFlag) {
            field_0xa91 = true;
            mTimer10 = 40;
        }
        break;

    case ACTION2_2_e:
    case ACTION2_3_e:
        if (mpModelMorf2->checkFrame(3.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mActionMode2 == ACTION2_2_e) {
            if (mpModelMorf2->checkFrame(32.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
            }
        } else if (mpModelMorf2->checkFrame(35.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
        }

        if (mpModelMorf2->isStop()) {
            setActionMode(ACT_CHASEL, ACTION2_0_e);
            field_0xa91 = true;
        }
    }
}

void daB_TN_c::executeYoroke() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mActionMode2) {
    case ACTION2_0_e:
    case ACTION2_1_e:
        if (mActionMode2 == ACTION2_0_e) {
            setBck(BCK_TNB_DOWN_SIPPU, 0, 3.0f, 1.0f);
        } else {
            setBck(BCK_TNB_DOWN_SIPPU, 0, 3.0f, 1.0f);
        }

        mActionMode2 = ACTION2_10_e;
        setSwordAtBit(0);
        setSwordAtBreak(1);
        mSphC.OffAtSetBit();
        field_0xa91 = false;
        speedF = 0.0f;
        mCutJumpStatus = 0;

        if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP) {
            mCutJumpStatus = 1;
        }
        // [[fallthrough]]

    case ACTION2_10_e:
        if (mCutJumpStatus == 1) {
            if (!player->checkCutJumpMode()) {
                mCutJumpStatus = 2;
            }
        } else if (mCutJumpStatus == 2 && player->checkCutJumpMode() &&
                   !player->checkCutJumpCancelTurn() && fopAcM_searchPlayerDistance(this) < 350.0f)
        {
            setActionMode(ACT_CHASEL, ACTION2_0_e);
            if (player->speedF < 28.0f) {
                initChaseL(6);
                break;
            }

            if ((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y) < 0) {
                initChaseL(14);
            } else {
                initChaseL(11);
            }

            mTimer7 = 10;
            break;
        }

        if (mpModelMorf2->isStop()) {
            setActionMode(ACT_CHASEL, ACTION2_0_e);
            field_0xa91 = true;
        }
    }
}

void daB_TN_c::executeEnding() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp1c, sp28;

    switch (mActionMode2) {
    case ACTION2_0_e:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(2);
            return;
        }

        Z2GetAudioMgr()->subBgmStop();

        if (mSwitchNo != 0xff) {
            dComIfGs_onSwitch(mSwitchNo, fopAcM_GetRoomNo(this));
        }

        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);

        setBck(BCK_TNB_DIE, 0, 3.0f, 1.0f);
        mSound.startCreatureVoice(Z2SE_EN_TN_V_DMG_2, -1);
        mActionMode2 = ACTION2_100_e;
        speedF = 0.0f;
        mUpdateNeckAngle = false;

        for (int i = 0; i < 16; i++) {
            if (i != 15) {
                mStates[i] = 7;
            }
        }

        mCamFovY = 55.0f;
        field_0x724 = 450.0f;
        field_0x720 = 0x1000;
        mCamCenter.set(-5.0f, -217.0f, -61.0f);
        mCamEye.set(field_0x724 * cM_ssin(field_0x720), -350.0f,
                    field_0x724 * cM_scos(field_0x720));
        player->changeOriginalDemo();
        sp1c.set(-100.0f, -400.0f, 550.0f);
        player->setPlayerPosAndAngle(&sp1c, 0x7000, 0);
        player->changeDemoMode(4, 1, 0, 0);
        dComIfGs_onStageMiddleBoss();
        return;

    case ACTION2_1_e:
    case ACTION2_2_e:
    case ACTION2_100_e:
        if (mActionMode2 == ACTION2_100_e) {
            current.angle.y = 0;
            shape_angle.y = 0;
            current.pos.set(0.0f, -400.0f, 0.0f);
            mActionMode2 = ACTION2_1_e;
        } else {
            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(2));
            mDoMtx_stack_c::multVecZero(&mCamCenter);
        }

        mCamEye.set(field_0x724 * cM_ssin(field_0x720), -350.0f,
                    field_0x724 * cM_scos(field_0x720));
        cLib_addCalcAngleS2(&field_0x720, -0x1000, 8, 24);

        if (mActionMode2 == ACTION2_1_e) {
            if (mpModelMorf2->checkFrame(212.0f)) {
                mStates[15] = 2;
            }

            if (mpModelMorf2->checkFrame(6.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
            }

            if (mpModelMorf2->checkFrame(29.0f)) {
                mSound.startCreatureVoice(Z2SE_EN_TN_V_ZEIZEI, -1);
            }

            if (mpModelMorf2->checkFrame(94.0f)) {
                mSound.startCreatureVoice(Z2SE_EN_TN_V_KURUSI, -1);
            }

            if (mpModelMorf2->checkFrame(103.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
            }

            if (mpModelMorf2->checkFrame(121.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
            }

            if (mpModelMorf2->checkFrame(147.0f)) {
                mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M2, 0, -1);
            }

            if (mpModelMorf2->checkFrame(168.0f)) {
                mSound.startCreatureVoice(Z2SE_EN_TN_V_DEAD, -1);
            }

            if (mpModelMorf2->checkFrame(289.0f)) {
                dComIfGp_getVibration().StartShock(2, 31, cXyz(0.0f, 1.0f, 0.0f));
                mSound.startCreatureSound(Z2SE_EN_TN_HIZA, 0, -1);
            }

            if (mpModelMorf2->checkFrame(312.0f)) {
                dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
                mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
            }

            if (mpModelMorf2->isStop()) {
                mActionMode2 = ACTION2_2_e;
                mTimer1 = 60;
            }
        } else if (mTimer1 == 0) {
            mActionMode2 = ACTION2_3_e;
            mTimer1 = 60;
        }
        break;

    case ACTION2_3_e:
        if (mTimer1 < 36) {
            cLib_chaseF(&mMaterialColor, -50.0f, 0.3f);
        }

        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(2));
        mDoMtx_stack_c::multVecZero(&sp28);
        sp28.x += 100.0f;
        cLib_addCalcPos2(&mCamCenter, sp28, 0.1f, 2.0f);
        sp28.set(-350.0f, -250.0f, 700.0f);
        cLib_addCalcPos2(&mCamEye, sp28, 0.1f, 5.0f);

        if (mTimer1 == 0) {
            if (!daPy_py_c::checkNowWolf()) {
                player->changeDemoMode(30, 0, 0, 0);
            } else {
                player->changeDemoMode(1, 0, 0, 0);
            }

            mTimer1 = 54;
            mActionMode2 = ACTION2_4_e;
        }
        break;

    case ACTION2_4_e:
        cLib_chaseF(&mMaterialColor, -50.0f, 0.3f);
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(2));
        mDoMtx_stack_c::multVecZero(&sp28);
        sp28.x += 100.0f;
        cLib_addCalcPos2(&mCamCenter, sp28, 0.1f, 2.0f);
        sp28.set(-350.0f, -250.0f, 700.0f);
        cLib_addCalcPos2(&mCamEye, sp28, 0.1f, 5.0f);

        if (mTimer1 == 0) {
            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(2));
            mDoMtx_stack_c::multVecZero(&sp28);
            fopAcM_createDisappear(this, &sp28, 10, 0, 0xff);
            mDisappear = true;
            mActionMode2 = ACTION2_5_e;
            mTimer1 = 65;
        }
        break;

    case ACTION2_5_e:
        mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(2));
        mDoMtx_stack_c::multVecZero(&sp28);
        sp28.x += 100.0f;
        cLib_addCalcPos2(&mCamCenter, sp28, 0.1f, 2.0f);
        sp28.set(-350.0f, -250.0f, 700.0f);
        cLib_addCalcPos2(&mCamEye, sp28, 0.1f, 5.0f);

        if (mTimer1 == 0) {
            camera->mCamera.Reset(mCamCenter, mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            fopAcM_delete(this);
        }
    }

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
}

void daB_TN_c::executeZakoEnding() {
    cXyz sp20;

    switch (mActionMode2) {
    case ACTION2_0_e:
        attention_info.flags = 0;
        setBck(4, 0, 3.0f, 1.0f);
        mSound.startCreatureVoice(Z2SE_EN_TN_V_DEAD2, -1);
        mActionMode2 = ACTION2_1_e;
        speedF = 0.0f;
        mUpdateNeckAngle = false;

        if (mStayNo2Flag) {
            Z2GetAudioMgr()->subBgmStop();
        }
        // [[fallthrough]]

    case ACTION2_1_e:
        if (mpModelMorf2->checkFrame(3.0f)) {
            mStates[15] = 2;
            mSound.startCreatureSound(Z2SE_EN_TN_FOOT_M1, 0, -1);
        }

        if (mpModelMorf2->checkFrame(33.0f)) {
            dComIfGp_getVibration().StartShock(2, 31, cXyz(0.0f, 1.0f, 0.0f));
            mSound.startCreatureSound(Z2SE_EN_TN_HIZA, 0, -1);
        }

        if (mpModelMorf2->checkFrame(68.0f)) {
            dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
        }

        cLib_chaseF(&mMaterialColor, -50.0f, 0.3f);

        if (mpModelMorf2->isStop()) {
            mActionMode2 = ACTION2_2_e;
            mTimer1 = 10;
        }
        break;

    case ACTION2_2_e:
        cLib_chaseF(&mMaterialColor, -50.0f, 0.3f);

        if (mTimer1 == 0) {
            mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(2));
            mDoMtx_stack_c::multVecZero(&sp20);
            fopAcM_createDisappear(this, &sp20, 10, 0, 29);

            if (mStayNo2Flag) {
                mTimer1 = 60;
                mInactive = true;
                mActionMode2 = ACTION2_3_e;
            } else {
                if (mSwitchNo != 0xff) {
                    dComIfGs_onSwitch(mSwitchNo, fopAcM_GetRoomNo(this));
                }

                fopAcM_delete(this);
            }
        }
        break;

    case ACTION2_3_e:
        if (mTimer1 == 0) {
            if (mSwitchNo != 0xff) {
                dComIfGs_onSwitch(mSwitchNo, fopAcM_GetRoomNo(this));
            }

            fopAcM_delete(this);
        }
    }
}

void daB_TN_c::action() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (m_attack_timer != 0) {
        m_attack_timer--;
    }

    if (mType == 0 || !player->checkGuardBreakMode()) {
        m_attack_tn = 0;
    }

    if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
        mTimer13 = 30;
    }

    damage_check();
    field_0x700 = l_HIO.field_0x24;
    mUpdateWaistAngle = false;
    mChkCoHitOK = false;
    mSphCSmallFlag = false;
    attention_info.flags &= ~fopAc_AttnFlag_UNK_0x200000;
    int iVar1 = 1;

    switch (mActionMode1) {
    case ACT_ROOMDEMO:
        executeRoomDemo();
        iVar1 = 0;
        break;

    case ACT_OPENING:
        executeOpening();
        iVar1 = 0;
        break;

    case ACT_WAITH:
        executeWaitH();
        if (mType == 1) {
            iVar1 = 0;
        }
        break;

    case ACT_CHASEH:
        executeChaseH();
        break;

    case ACT_ATTACKH:
        executeAttackH();
        break;

    case ACT_ATTACKSHIELDH:
        executeAttackShieldH();
        break;

    case ACT_GUARDH:
        executeGuardH();
        break;

    case ACT_DAMAGEH:
        executeDamageH();
        break;

    case ACT_CHANGEDEMO:
        if (mType == 0) {
            executeChangeDemo();
        } else {
            executeZakoChangeDemo();
        }
        break;

    case ACT_CHASEL:
        executeChaseL();
        break;

    case ACT_ATTACKL:
        executeAttackL();
        break;

    case ACT_ATTACKSHIELDL:
        executeAttackShieldL();
        break;

    case ACT_GUARDL:
        executeGuardL();
        break;

    case ACT_DAMAGEL:
        executeDamageL();
        break;

    case ACT_YOROKE:
        executeYoroke();
        break;

    case ACT_ENDING:
        if (mType == 0) {
            executeEnding();
        } else {
            executeZakoEnding();
        }
        break;
    }

    if (mType == 0) {
        if (iVar1) {
            player->onBossRoomWait();
        }

        switch (mBlendStatus) {
        case 0:
            mBlend = 1.0f;
            dKy_custom_colset(0, 0, mBlend);
            break;

        case 1:
            cLib_chaseF(&mBlend, 1.0f, 0.003f);
            dKy_custom_colset(0, 1, mBlend);
            break;

        case 2:
            cLib_chaseF(&mBlend, 1.0f, 0.01f);
            dKy_custom_colset(1, 2, mBlend);
        }
    } else {
        mSound.setLinkSearch(iVar1);
    }

    calcNeckAngle();
    calcWaistAngle();
    setBodyShield();
    calcPartMove();
    fopAcM_posMoveF(this, mStts1.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());

    cXyz sp38(eyePos);
    sp38.y += 300.0f;
    setHeadLockPos(&sp38);

    int iVar2 = 13 - mNextBreakPart;
    if (iVar2 < 0) {
        iVar2 = 0;
    }

    if (iVar2 > 13) {
        iVar2 = 13;
    }

    mpModelMorf2->play(iVar2 + 10, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpModelMorf1->play(iVar2 + 10, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daB_TN_c::mtx_set() {
    J3DModel* model2;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.mScale, l_HIO.mScale, l_HIO.mScale);
    J3DModel* model1 = mpModelMorf2->getModel();
    model1->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf2->modelCalc();

    for (int i = 0; i < 16; i++) {
        if (i == 15) {
            model2 = mpModelMorf1->getModel();
        } else {
            model2 = mpPartModels[i];
        }

        switch (mStates[i]) {
        case 0:
            mDoMtx_stack_c::copy(model1->getAnmMtx(tn_part_joint[i]));

            if (mTimer9 != 0 && i <= 11 && i != 5 && i != 4) {
                int iVar2 = (s16)(mTimer9 * 96);
                field_0xa64 += 1280;
                mDoMtx_stack_c::ZXYrotM(iVar2 * cM_ssin(field_0xa64 + (i << 13)),
                                        iVar2 * cM_ssin(field_0xa64 + (i * 0x6000)),
                                        iVar2 * cM_ssin(field_0xa64 - (i << 13)));
            }

            model2->setBaseTRMtx(mDoMtx_stack_c::get());
            break;

        case 1:
            if (i == 15) {
                mDoMtx_stack_c::copy(model1->getAnmMtx(15));
                mDoMtx_stack_c::transM(12.65f, 13.47f, 20.07f);
                mDoMtx_stack_c::ZXYrotM(cM_deg2s(-168.0f), cM_deg2s(-42.715f), cM_deg2s(77.897));
                model2->setBaseTRMtx(mDoMtx_stack_c::get());
            } else {
                model2->setBaseTRMtx(model1->getAnmMtx(15));
            }
            break;

        case 2:
        case 3:
        case 4:
        case 5:
            if (mType == 1 && i == 5) {
                mDoMtx_stack_c::copy(mpPartModels[4]->getBaseTRMtx());
            } else {
                mDoMtx_stack_c::transS(mPositions[i]);

                if (i == 4) {
                    mDoMtx_stack_c::YrotM(field_0x99c[i].y);
                    mDoMtx_stack_c::ZrotM(field_0x99c[i].z);
                    mDoMtx_stack_c::XrotM(field_0x99c[i].x);
                } else {
                    mDoMtx_stack_c::ZXYrotM(field_0x99c[i]);
                }

                mDoMtx_stack_c::transM(offset_part[i]);
                mDoMtx_stack_c::scaleM(l_HIO.mScale, l_HIO.mScale, l_HIO.mScale);
            }

            model2->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }

    mpModelMorf1->modelCalc();

    if (mUpdateModelTimer != 0) {
        swordEffectModel->setBaseTRMtx(mNewTRMtx);
    }
}

void daB_TN_c::cc_set() {
    cXyz sp38;

    J3DModel* model = mpModelMorf2->getModel();
    mDoMtx_stack_c::copy(model->getAnmMtx(5));
    mDoMtx_stack_c::transM(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&eyePos);
    attention_info.position = eyePos;

    if (mActionMode1 < 8) {
        attention_info.position.y += 80.0f;
    } else {
        attention_info.position.y += 50.0f;
    }

    if (mTimer7 == 0) {
        for (int i = 0; i < 3; i++) {
            switch (i) {
            case 0:
                mDoMtx_stack_c::copy(model->getAnmMtx(5));

                if (mActionMode1 < 8) {
                    mDoMtx_stack_c::transM(30.0f, 0.0f, 0.0f);
                    mSphA[0].SetR(40.0f);
                    mSphB[0].SetR(40.0f);
                } else {
                    mDoMtx_stack_c::transM(15.0f, 0.0f, 0.0f);
                    mSphA[0].SetR(30.0f);
                    mSphB[0].SetR(30.0f);
                }
                break;

            case 1:
                mSphA[1].SetR(65.0f);
                mSphB[1].SetR(65.0f);
                mDoMtx_stack_c::copy(model->getAnmMtx(3));
                break;

            case 2:
                mSphA[2].SetR(55.0f);
                mSphB[2].SetR(55.0f);
                mDoMtx_stack_c::copy(model->getAnmMtx(18));
                mDoMtx_stack_c::transM(30.0f, 0.0f, 0.0f);
                break;
            }

            mDoMtx_stack_c::multVecZero(&sp38);
            mSphA[i].SetC(sp38);
            mSphB[i].SetC(sp38);
            dComIfG_Ccsp()->Set(&mSphA[i]);
            dComIfG_Ccsp()->Set(&mSphB[i]);
        }
    } else {
        for (int i = 0; i < 3; i++) {
            // Bug: This feels like a bug, but the original
            // does this for some reason. The fields should
            // be indexed by i, not 0.
            mSphA[0].ClrTgHit();
            mSphB[0].ClrTgHit();
        }
    }

    if (mActionMode1 < 8) {
        sp38 = current.pos;
        sp38.y += 30.0f;
        mSphD.SetC(sp38);
        mSphD.SetR(60.0f);
        dComIfG_Ccsp()->Set(&mSphD);
    }

    if (mStates[13] == 1) {
        mDoMtx_stack_c::copy(model->getAnmMtx(15));
        mDoMtx_stack_c::transM(10.0f, 15.0f, -20.0f);

        for (int i = 0; i < 4; i++) {
            if (mType == 0) {
                mDoMtx_stack_c::transM(12.0f, -6.0f, 36.0f);
            } else {
                mDoMtx_stack_c::transM(9.0f, -4.0f, 27.0f);
            }

            mDoMtx_stack_c::multVecZero(&sp38);
            mSwordSphs[i].SetC(sp38);
            if (mActionMode2 == ACTION2_1_e) {
                mSwordSphs[i].SetR(70.0f);
            } else {
                mSwordSphs[i].SetR(60.0f);
            }

            dComIfG_Ccsp()->Set(&mSwordSphs[i]);
        }

        if (mActionMode2 == ACTION2_1_e) {
            mCps.cM3dGCps::Set(mCpsPos, sp38, 70.0f);
        } else {
            mCps.cM3dGCps::Set(mCpsPos, sp38, 60.0f);
        }

        mCpsPos = sp38;
        dComIfG_Ccsp()->Set(&mCps);
    } else if (mStates[15] == 1) {
        mDoMtx_stack_c::copy(mpModelMorf1->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::transM(-50.0f, 0.0f, 0.0f);

        for (int i = 0; i < 4; i++) {
            mDoMtx_stack_c::transM(40.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::multVecZero(&sp38);
            mSwordSphs[i].SetC(sp38);
            mSwordSphs[i].SetR(45.0f);
            dComIfG_Ccsp()->Set(&mSwordSphs[i]);
        }

        mCps.cM3dGCps::Set(mCpsPos, sp38, 45.0f);
        mCpsPos = sp38;
        dComIfG_Ccsp()->Set(&mCps);
    } else if (mStates[13] == 3) {
        mDoMtx_stack_c::copy(mpPartModels[13]->getBaseTRMtx());

        for (int i = 0; i < 4; i++) {
            mDoMtx_stack_c::transM(20.0f, 0.0f, 40.0f);
            mDoMtx_stack_c::multVecZero(&sp38);
            mSwordSphs[i].SetC(sp38);
            mSwordSphs[i].SetR(45.0f);
            dComIfG_Ccsp()->Set(&mSwordSphs[i]);
        }

        mCps.cM3dGCps::Set(mCpsPos, sp38, 45.0f);
        mCpsPos = sp38;
        dComIfG_Ccsp()->Set(&mCps);
    }

    if (mStates[12] == 0) {
        mDoMtx_stack_c::copy(model->getAnmMtx(9));
        mDoMtx_stack_c::transM(10.0f, -10.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&sp38);
        mSphC.SetC(sp38);

        if (mSphC.ChkAtSet()) {
            mSphC.SetR(70.0f);
        } else if (!mSphCSmallFlag) {
            mSphC.SetR(55.0f);
        } else {
            mSphC.SetR(30.0f);
        }

        dComIfG_Ccsp()->Set(&mSphC);
    } else if (mSphC.ChkAtSet() && mActionMode1 > 8) {
        mDoMtx_stack_c::copy(model->getAnmMtx(24));
        mDoMtx_stack_c::multVecZero(&sp38);
        mSphC.SetC(sp38);
        mSphC.SetR(70.0f);
        dComIfG_Ccsp()->Set(&mSphC);
    } else {
        mSphC.ClrTgHit();
    }

    for (int i = 0; i < 13; i++) {
        if (mStates[i] == 5) {
            mDoMtx_stack_c::transS(mPositions[i]);
            mDoMtx_stack_c::multVecZero(&sp38);
            mSphArr[i].SetC(sp38);

            if (i == 12) {
                mSphArr[i].SetR(50.0f);
            } else {
                mSphArr[i].SetR(30.0f);
            }

            dComIfG_Ccsp()->Set(&mSphArr[i]);
        }
    }
}

int daB_TN_c::execute() {
    if (mTimer1 != 0) {
        mTimer1--;
    }

    if (mInvincibilityTimer != 0) {
        mInvincibilityTimer--;
    }

    if (mTimer3 != 0) {
        mTimer3--;
    }

    if (mTimer4 != 0) {
        mTimer4--;
    }

    if (mTimer5 != 0) {
        mTimer5--;
    }

    if (mTimer6 != 0) {
        mTimer6--;
    }

    if (mTimer7 != 0) {
        mTimer7--;
    }

    if (mVibrationTimer != 0) {
        mVibrationTimer--;
        if (mVibrationTimer == 0) {
            dComIfGp_getVibration().StopQuake(31);
        }
    }

    if (mTimer9 != 0) {
        mTimer9--;
    }

    if (mTimer10 != 0) {
        mTimer10--;
    }

    if (mUpdateModelTimer != 0) {
        mUpdateModelTimer--;
    }

    if (mTimer12 != 0) {
        mTimer12--;
    }

    if (mTimer13 != 0) {
        mTimer13--;
    }

    action();
    mtx_set();

    if (mInactive) {
        return 1;
    }

    cc_set();
    mDoMtx_stack_c::copy(mpModelMorf2->getModel()->getAnmMtx(18));
    cXyz effPos, effSize;
    mDoMtx_stack_c::multVecZero(&effPos);
    effSize.set(1.5f, 2.2f, 1.5f);
    setMidnaBindEffect(this, &mSound, &effPos, &effSize);

    return 1;
}

static int daB_TN_Execute(daB_TN_c* i_this) {
    i_this->execute();
}

static int daB_TN_IsDelete(daB_TN_c* i_this) {
    return 1;
}

int daB_TN_c::_delete() {
    dComIfG_resDelete(&mPhaseReq1, "B_tn");
    dComIfG_resDelete(&mPhaseReq2, mArcName);
    if (mHioInit) {
        s_hioinit = 0;
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daB_TN_Delete(daB_TN_c* i_this) {
    i_this->_delete();
}

int daB_TN_c::CreateHeap() {
    static int tn2_brk_index[3] = {BRK_TN2_COLOR_1, BRK_TN2_COLOR_2, BRK_TN2_SHIELD};

    J3DModelData* modelData;

    if (mType == 0) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("B_tnp", BMDR_TN);
    } else {
        modelData = (J3DModelData*)dComIfG_getObjectRes("B_tnp2", BMDR_TN2);
    }

    mpModelMorf2 = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                        (J3DAnmTransform*)dComIfG_getObjectRes("B_tn", BCK_TNA_WAIT), 0, 1.0f,
                                        0, -1, &mSound, 0x80000, 0x11000084);
    if (mpModelMorf2 == NULL || mpModelMorf2->getModel() == NULL) {
        return 0;
    }

    if (mType == 1) {
        mpBrkAnm = new mDoExt_brkAnm();
        if (mpBrkAnm == NULL) {
            return 0;
        }

        if (mpBrkAnm->init(modelData, (J3DAnmTevRegKey*)dComIfG_getObjectRes(mArcName, BRK_TN2_BODY), 1, 2,
                           0.0f, 0, -1) == 0)
        {
            return 0;
        }
    }

    J3DModel* model = mpModelMorf2->getModel();
    model->setUserArea((uintptr_t)this);
    model->getModelData()->getJointNodePointer(5)->setCallBack(JointCallBack);
    model->getModelData()->getJointNodePointer(1)->setCallBack(JointCallBack);

    for (int i = 0; i < 15; i++) {
        if (i == 13 && mType == 1 && mType2 == 1) {
            modelData = (J3DModelData*)dComIfG_getObjectRes(mArcName, 0x13);
        } else {
            modelData =
                (J3DModelData*)dComIfG_getObjectRes(mArcName, tn_part_model_id[i + mType * 0x10]);
        }

        model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        mpPartModels[i] = model;

        if (mpPartModels[i] == NULL) {
            return 0;
        }

        if (mType == 1) {
            s8 sVar1 = tn_model_brk_idx[i];

            if (mBrkAnmFrame == 3) {
                sVar1 = tn_model_brk_idx2[i];
            }

            if (sVar1 != -1) {
                mpBrkAnms[i] = new mDoExt_brkAnm();
                mDoExt_brkAnm* myanm = mpBrkAnms[i];

                if (myanm == NULL) {
                    return 0;
                }

                if (mpBrkAnms[i]->init(
                        modelData,
                        (J3DAnmTevRegKey*)dComIfG_getObjectRes(mArcName, tn2_brk_index[sVar1]), 1,
                        2, 0.0f, 0, -1) == 0)
                {
                    return 0;
                }
            }
        }
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(mArcName, tn_part_model_id[mType * 0x10 + 0xf]);

    mpModelMorf1 = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                        (J3DAnmTransform*)dComIfG_getObjectRes("B_tn", BCK_TNB_SWORD_B_PULL_A), 0, 1.0f,
                                        0, -1, &mSound, 0x80000, 0x11000084);
    if (mpModelMorf1 == NULL || mpModelMorf1->getModel() == NULL) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("B_tn", BMDR_TN_EFFECT);
    model = mDoExt_J3DModel__create(modelData, 0x80000, 0x31000084);

    swordEffectModel = model;
    if (swordEffectModel == NULL) {
        return 0;
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    ((daB_TN_c*)actor)->CreateHeap();
}

int daB_TN_c::create() {
    fopAcM_ct(this, daB_TN_c);

    mSwitchNo = fopAcM_GetParam(this);
    mType = fopAcM_GetParamBit(this, 8, 8);

    if (mType != 1) {
        mType = 0;
    }

    if (mType == 0) {
        mArcName = "B_tnp";
        if (dComIfGs_isStageMiddleBoss()) {
            return cPhs_ERROR_e;
        }
    } else {
        mArcName = "B_tnp2";
        if (dComIfGs_isSwitch(mSwitchNo, fopAcM_GetRoomNo(this))) {
            return cPhs_ERROR_e;
        }
    }

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq1, "B_tn");
    if (step == cPhs_COMPLEATE_e) {
        step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq2, mArcName);

        if (step == cPhs_COMPLEATE_e) {
            mBrkAnmFrame = fopAcM_GetParamBit(this, 16, 16);
            if (mBrkAnmFrame > 4) {
                mBrkAnmFrame = 4;
            }

            mType2 = fopAcM_GetParamBit(this, 24, 24);
            ;
            if (mType2 != 1) {
                mType2 = 0;
            }

            if (mType == 1) {
                if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xb940)) {
                    return cPhs_ERROR_e;
                }
            } else if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xaa20)) {
                return cPhs_ERROR_e;
            }

            if (s_hioinit == 0) {
                s_hioinit = 1;
                mHioInit = 1;
                l_HIO.mUnk1 = -1;
            }

            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            fopAcM_SetMtx(this, mpModelMorf2->getModel()->getBaseTRMtx());
            fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
            fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);

            mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
            mAcchCir.SetWall(60.0f, 300.0f);
            health = 0x50;
            field_0x560 = 0x50;

            mStts1.Init(0xfa, 0, this);
            mStts2.Init(0xfa, 0, this);

            for (int i = 0; i < 3; i++) {
                mSphA[i].Set(cc_tt_src);
                mSphA[i].SetStts(&mStts1);

                mSphB[i].Set(cc_tt_src2);
                mSphB[i].SetStts(&mStts1);
            }

            mSphA[2].OnCoSameActorHit();
            mSphD.Set(cc_bomb_src);
            mSphD.SetStts(&mStts1);

            for (int i = 0; i < 4; i++) {
                mSwordSphs[i].Set(cc_tt_at_src);
                mSwordSphs[i].SetStts(&mStts2);
            }

            mCps.Set(cc_tt_at_cps_src);
            mCps.SetStts(&mStts2);

            mSphC.Set(cc_tt_tate_src);
            mSphC.SetStts(&mStts1);

            for (int k = 0; k < 16; k++) {
                mAcchArr[k].Set(&mPositions[k], &mPositionsCopy[k], this, 1, &mAcchCirArr[k],
                                &field_0x8dc[k], NULL, NULL);
                mAcchCirArr[k].SetWall(10.0f, 50.0f);
                mAcchArr[k].SetGroundUpY(ground_y_part[k]);
                mSttsArr[k].Init(100, 0, this);
                mSphArr[k].Set(cc_part_src);
                mSphArr[k].SetStts(&mSttsArr[k]);

                mSphArr[k].OnCoSameActorHit();
            }

            mSound.init(&current.pos, &eyePos, 0x3, 0x1);
            mSound.setEnemyName("B_tn");
            mAtInfo.mpSound = &mSound;
            mAtInfo.mPowerType = 1;
            gravity = -5.0f;
            mStates[0xd] = 1;

            setSwordAtBreak(1);

            if (mType == 0) {
                setActionMode(ACT_ROOMDEMO, ACTION2_0_e);
            } else {
                mpBrkAnm->setFrame(mBrkAnmFrame);

                for (int k = 0; k < 15; k++) {
                    if (mpBrkAnms[k] != NULL) {
                        mpBrkAnms[k]->setFrame(mBrkAnmFrame);
                    }
                }

                fopAcM_OffStatus(this, 0x4000);
                setActionMode(ACT_WAITH, ACTION2_0_e);
            }

            if (!strcmp("D_MN09", dComIfGp_getStartStageName()) &&
                dComIfGp_roomControl_getStayNo() == 0x2)
            {
                mStayNo2Flag = true;
            }

            daB_TN_Execute(this);
        }
    }

    return step;
}

static int daB_TN_Create(daB_TN_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daB_TN_Method = {
    (process_method_func)daB_TN_Create,  (process_method_func)daB_TN_Delete,
    (process_method_func)daB_TN_Execute, (process_method_func)daB_TN_IsDelete,
    (process_method_func)daB_TN_Draw,
};

extern actor_process_profile_definition g_profile_B_TN = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_B_TN,               // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daB_TN_c),        // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    225,                     // mPriority
    &l_daB_TN_Method,        // sub_method
    0x00044000,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
