/**
 * @file d_a_e_vt.cpp
 * Variant Enemy (Death Sword)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_vt.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "SSystem/SComponent/c_math.h"
#include "c/c_damagereaction.h"
#include "cmath.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_camera.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"

#define WL_CUT_TYPE_SMALL 1
#define WL_CUT_TYPE_JUMP 2
#define WL_CUT_TYPE_LARGE 3

class daE_VA_HIO_c {
public:
    daE_VA_HIO_c();
    virtual ~daE_VA_HIO_c() {}

    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 mModelSize;
    /* 0x0C */ f32 mAttackRange;
    /* 0x10 */ f32 mBiteCount;
    /* 0x14 */ f32 mCircleAttackRadius;
    /* 0x18 */ f32 mFadeAwayTime;
    /* 0x1C */ f32 mDownTime;
    /* 0x20 */ f32 mDownHP;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 mKRegLightR;
    /* 0x34 */ f32 mKRegLightG;
    /* 0x38 */ f32 mKRegLightB;
    /* 0x3C */ f32 mKRegLightA;
};

enum daE_VA_JOINT {
    JNT_ROOT,
    JNT_BACKBONE_01,
    JNT_BACKBONE_02,
    JNT_ARM_L01,
    JNT_ARM_L02,
    JNT_ARM_L03,
    JNT_ARM_L04,
    JNT_ARM_L05,
    JNT_FINGER_LB,
    JNT_FINGER_LC,
    JNT_FINGER_LA,
    JNT_LEFT_COTH_01,
    JNT_LEFT_COTH_02,
    JNT_LEFT_COTH_03,
    JNT_LEFT_COTH_04,
    JNT_ARM_R01,
    JNT_ARM_R02,
    JNT_ARM_R03,
    JNT_ARM_R04,
    JNT_ARM_R05,
    JNT_FINGER_RB,
    JNT_FINGER_RC,
    JNT_KEN_1,
    JNT_KEN_2,
    JNT_FINGER_RA,
    JNT_BUST,
    JNT_NECK_CLOTH,
    JNT_NECK_01,
    JNT_HEAD_CLOTH_01,
    JNT_HEAD_CLOTH_02,
    JNT_HEAD_CLOTH_03,
    JNT_HEAD_CLOTH_04,
    JNT_HEAD_CLOTH_05,
    JNT_NECK_02,
    JNT_HEAD,
    JNT_JOE,
    JNT_WAIST,
    JNT_BACK_CLOTH_01,
    JNT_BACK_CLOTH_02,
    JNT_BACK_CLOTH_03,
    JNT_FRONT_CLOTH_01,
    JNT_FRONT_CLOTH_02,
    JNT_FRONT_CLOTH_03,
    JNT_LEG_L01,
    JNT_LEG_L02,
    JNT_LEG_L03,
    JNT_LEG_R01,
    JNT_LEG_R02,
    JNT_LEG_R03,
    JNT_TAIL_CLOTH_01,
    JNT_TAIL_CLOTH_02,
    JNT_TAIL_CLOTH_03,
    JNT_TAIL_CLOTH_04,
};

namespace
{

static dCcD_SrcSph cc_vt_neck_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x80000000, 0x3}, 0x0}},  // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0},               // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},                // mGObjTg
        {0x0},                                             // mGObjCo
    },                                                     // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
    }  // mSphAttr
};

static dCcD_SrcCyl cc_vt_body_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x43}, 0x75}},  // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                  // mGObjTg
        {0x0},                                               // mGObjCo
    },                                                       // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        150.0f,              // mRadius
        158.0f               // mHeight
    }  // mCyl
};

static dCcD_SrcSph cc_vt_attack_src = {
    {
        {0x0, {{0x100000, 0x2, 0xD}, {0xD8FBFDFF, 0x43}, 0x75}},  // mObj
        {dCcD_SE_METAL, 0x0, 0x1, 0x0, 0x0},                      // mGObjAt
        {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x2},                       // mGObjTg
        {0x0},                                                    // mGObjCo
    },                                                            // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
    }  // mSphAttr
};

static dCcD_SrcSph cc_vt_line_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xD8FBFDFF, 0x43}, 0x75}},  // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                  // mGObjTg
        {0x0},                                               // mGObjCo
    },                                                       // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
    }  // mSphAttr
};

static dCcD_SrcSph cc_vt_magic_src = {
    {
        {0x0, {{0x400, 0x2, 0xD}, {0xD8FBFDFF, 0x0}, 0x0}},  // mObj
        {0xD, 0x0, 0x1, 0x0, 0x0},                           // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                  // mGObjTg
        {0x0},                                               // mGObjCo
    },                                                       // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
    }  // mSphAttr
};

static u8 va_tag_set_size[39] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02,
};

static u8 va_tag_set_num[39] = {
    0x01, 0x0C, 0x16, 0x1F, 0x21, 0x2A, 0x2B, 0x02, 0x04, 0x0D, 0x0F, 0x15, 0x18,
    0x20, 0x2C, 0x03, 0x05, 0x06, 0x0B, 0x10, 0x17, 0x19, 0x1A, 0x22, 0x24, 0x29,
    0x2D, 0x34, 0x49, 0x52, 0x36, 0x3E, 0x54, 0x5C, 0x39, 0x41, 0x4C, 0x56, 0x5E,
};

static f32 va_tag_offset[39] = {
    20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 0.0f,  40.0f, 0.0f, 30.0f, 10.0f, 0.0f,  0.0f,  20.0f,
    0.0f,  10.0f, 20.0f, 40.0f, 0.0f,  20.0f, 0.0f,  0.0f, 20.0f, 0.0f,  10.0f, 0.0f,  0.0f,
    20.0f, 10.0f, 20.0f, 30.0f, 0.0f,  10.0f, 20.0f, 0.0f, 0.0f,  10.0f, 20.0f, 30.0f, 0.0f,
};

}  // namespace

daE_VA_HIO_c::daE_VA_HIO_c() {
    field_0x04 = -1;
    mModelSize = 1.2f;
    mAttackRange = 500.0f;
    mBiteCount = 7.0f;
    mFadeAwayTime = 2500.0f;
    mCircleAttackRadius = 1000.0f;
    mDownTime = 100.0f;
    mDownHP = 800.0f;
    field_0x24 = 0.5f;
    field_0x28 = 3.0f;
    field_0x2c = -50.0f;

    mKRegLightR = 40.0f;
    mKRegLightG = 35.0f;
    mKRegLightB = 82.0f;
    mKRegLightA = 210.0f;
}

/* 807CF4B5 0003+00 data_807CF4B5 None */
#pragma push
#pragma force_active on
static u8 init_hio;
#pragma pop

static daE_VA_HIO_c l_HIO;

int daE_VA_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int joint_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_no));

    switch (joint_no) {
    case JNT_LEFT_COTH_01:
    case JNT_LEFT_COTH_02:
    case JNT_LEFT_COTH_03:
    case JNT_LEFT_COTH_04:
        if (field_0x138b) {
            mDoMtx_stack_c::transS(field_0x140c[joint_no - JNT_LEFT_COTH_01]);
            mDoMtx_stack_c::YrotM(field_0x130c);
            mDoMtx_stack_c::ZrotM(field_0x130e);
            mDoMtx_stack_c::ZrotM(field_0x1448[joint_no - JNT_LEFT_COTH_01].z);
            mDoMtx_stack_c::YrotM(field_0x1448[joint_no - JNT_LEFT_COTH_01].y);
            mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);
        }
        break;
    case JNT_NECK_01:
        mDoMtx_stack_c::YrotM(field_0x1398);
        if (field_0x1391 == 1) {
            mDoMtx_stack_c::XrotM(field_0x1394);
            mDoMtx_stack_c::ZrotM(field_0x1396);
        }
        break;
    case JNT_NECK_02:
        mDoMtx_stack_c::YrotM(field_0x1398);
        if (field_0x1391 == 2) {
            mDoMtx_stack_c::XrotM(field_0x1394);
        }
        break;
    case JNT_HEAD:
        if (field_0x1391 == 2) {
            mDoMtx_stack_c::ZrotM(field_0x1396);
        }
        break;
    }

    i_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daE_VA_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daE_VA_c* a_this = (daE_VA_c*)model->getUserArea();

        if (a_this != NULL) {
            a_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daE_VA_c::calcJointNeck() {
    csXyz sp18;

    if (field_0x1391 != 0) {
        s16 angleY_to_player = fopAcM_searchPlayerAngleY(this) - shape_angle.y;
        if (angleY_to_player > 0x3000) {
            angleY_to_player = 0x3000;
        }
        if (angleY_to_player < -0x3000) {
            angleY_to_player = -0x3000;
        }
        cLib_addCalcAngleS(&field_0x1394, angleY_to_player, 8, 0x800, 0x40);

        s16 angleX_to_player = 0x1400 - fopAcM_searchPlayerAngleX(this);
        if (angleX_to_player > 0x3000) {
            angleX_to_player = 0x3000;
        }
        if (angleX_to_player < -0x3000) {
            angleX_to_player = -0x3000;
        }
        cLib_addCalcAngleS(&field_0x1396, angleX_to_player, 8, 0x800, 0x40);

        if (checkBck(ANM_SUBS_INVITE_A_e) || checkBck(ANM_SUBS_INVITE_B_e)) {
            cLib_addCalcAngleS(&field_0x1398, -0x800, 8, 0x80, 0x40);
        } else if (checkBck(ANM_SUBS_INVITE_C_e)) {
            cLib_addCalcAngleS(&field_0x1398, -0x1800, 8, 0x80, 0x40);
        } else {
            cLib_addCalcAngleS(&field_0x1398, -0x1800, 8, 0x100, 0x40);
        }

        field_0x1392 = field_0x1391;
    } else {
        field_0x1391 = field_0x1392;
        cLib_addCalcAngleS(&field_0x1394, 0, 8, 0x100, 0x40);
        cLib_addCalcAngleS(&field_0x1396, 0, 8, 0x100, 0x40);
        cLib_addCalcAngleS(&field_0x1398, 0, 8, 0x100, 0x40);
    }
}

void daE_VA_c::calcJointSleeve() {
    csXyz sp128;
    cXyz sp90(field_0x13b8);

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_LEFT_COTH_01));
    mDoMtx_stack_c::multVecZero(&field_0x13b8);
    mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &sp128);

    f32 anm_frame = mpMorf->getFrame();
    if ((checkBck(ANM_SUBS_ATTACK_A1_e) && anm_frame >= 74.0f) ||
        (checkBck(ANM_SUBS_ATTACK_A3_e) && anm_frame <= 11.0f) ||
        (checkBck(ANM_TRANS_ATTACK_A1_e) && anm_frame >= 60.0f) ||
        checkBck(ANM_TRANS_ATTACK_A2_e) || (checkBck(ANM_TRANS_ATTACK_A3_e) && anm_frame <= 60.0f))
    {
        sp128.y = field_0x1466[0].y;
    }

    field_0x13c4 = field_0x13b8 - sp90;

    cXyz sp9C;
    cXyz spA8;
    cXyz spB4;

    field_0x13d0[0] = field_0x13c4;
    field_0x140c[0] = field_0x13b8;
    cXyz* line_pos = field_0x139c.getPos(0);
    *line_pos = field_0x140c[0];
    field_0x1466[0].set(-0x8000, sp128.y, -0x8000);
    line_pos++;
    for (int i = 1; i < 5; i++, line_pos++) {
        f32 var_f29;
        if (i == 1) {
            var_f29 = ZREG_F(0) + 80.0f;
        }
        if (i == 2) {
            var_f29 = ZREG_F(1) + 80.0f;
        }
        if (i == 3) {
            var_f29 = ZREG_F(2) + 50.0f;
        }
        if (i == 4) {
            var_f29 = ZREG_F(3) + 50.0f;
        }

        if (field_0x140c[i].abs(field_0x140c[i - 1]) > (var_f29 + 30.0f) + nREG_F(3)) {
            spA8 = field_0x140c[i - 1] - field_0x140c[i];
            f32 temp_f28 = spA8.abs();

            spA8.normalizeZP();
            spA8 = spA8 * (temp_f28 - var_f29);

            field_0x13d0[i].x = spA8.x;
            field_0x13d0[i].z = spA8.z;

            if (std::abs(field_0x140c[i].y - field_0x140c[i - 1].y) > var_f29) {
                field_0x13d0[i].y = spA8.y;
            } else if (spA8.y >= 0.0f) {
                if (field_0x13d0[i].y < spA8.y) {
                    field_0x13d0[i].y = spA8.y;
                }
            } else {
                field_0x13d0[i].y -= l_HIO.field_0x28;

                if (field_0x140c[i].y < field_0x140c[i - 1].y - (var_f29 * 0.8f)) {
                    if (field_0x13d0[i].y <= -10.0f) {
                        field_0x13d0[i].y = -10.0f;
                    }
                } else if (field_0x13d0[i].y <= l_HIO.field_0x2c) {
                    field_0x13d0[i].y = l_HIO.field_0x2c;
                }
            }
        } else {
            field_0x13d0[i].x *= l_HIO.field_0x24;
            field_0x13d0[i].z *= l_HIO.field_0x24;
            field_0x13d0[i].y -= l_HIO.field_0x28;

            if (field_0x140c[i].y < field_0x140c[i - 1].y - (var_f29 * 0.8f)) {
                if (field_0x13d0[i].y <= -10.0f) {
                    field_0x13d0[i].y = -10.0f;
                }
            } else if (field_0x13d0[i].y <= l_HIO.field_0x2c) {
                field_0x13d0[i].y = l_HIO.field_0x2c;
            }
        }

        if (field_0x1354 != 0) {
            field_0x13d0[i].y -= l_HIO.field_0x28;

            if (field_0x13d0[i].y <= l_HIO.field_0x2c) {
                field_0x13d0[i].y = l_HIO.field_0x2c;
            }
        }

        field_0x140c[i] += field_0x13d0[i];
        if (field_0x140c[i].y < 0.0f) {
            field_0x13d0[i] *= 0.1f;
            field_0x13d0[i].y = 0.0f;
            field_0x140c[i].y = 0.0f;
        }

        spA8 = field_0x140c[i] - field_0x140c[i - 1];
        spA8.normalizeZP();

        field_0x140c[i] = field_0x140c[i - 1] + (spA8 * var_f29);

        line_pos[0] = field_0x140c[i];
        spA8 = line_pos[0] - line_pos[-1];

        mDoMtx_stack_c::YrotS(-sp128.y);
        mDoMtx_stack_c::transM(spA8);
        mDoMtx_stack_c::multVecZero(&spA8);

        field_0x1466[i].x = cM_atan2s(spA8.z, spA8.y);
        field_0x1466[i].y = cM_atan2s(spA8.x, spA8.z);
        field_0x1466[i].z = cM_atan2s(spA8.x, spA8.y);
    }

    field_0x130c = sp128.y;
    field_0x130e = -0x4000;

    cXyz sp80;
    cXyz sp74;
    cXyz sp68;
    cXyz sp5C;

    for (int i = 0; i < 4; i++) {
        field_0x1448[i].y = field_0x1466[i + 1].x - field_0x1466[0].x;
        field_0x1448[i].z = field_0x1466[0].z - field_0x1466[i + 1].z;

        if (abs(field_0x1448[i].z) >= 0x4000) {
            field_0x1448[i].y = 0x8000 - field_0x1448[i].y;
        }
    }
}

int daE_VA_c::draw() {
    J3DModel* model = mpMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);

    if (!mPlayEndEf) {
        if (!mRopesEnabled) {
            g_env_light.setLightTevColorType_MAJI(model, &tevStr);

            J3DModelData* modelData = model->getModelData();
            for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
                J3DMaterial* material = modelData->getMaterialNodePointer(i);

                material->getZMode()->setUpdateEnable(1);
                material->setZCompLoc(1);
                material->setMaterialMode(4);

                if (i == 0) {
                    material->getTevKColor(3)->a = mEyeAlpha;
                } else {
                    material->getTevKColor(0)->r = mGlowLightR;
                    material->getTevKColor(0)->g = mGlowLightG;
                    material->getTevKColor(0)->b = mGlowLightB;

                    if (!field_0x1318) {
                        material->getTevColor(0)->r = field_0x1380;
                        material->getTevColor(0)->g = field_0x1380;
                        material->getTevColor(0)->b = field_0x1380;
                    } else {
                        material->getTevColor(0)->r = field_0x1318;
                        material->getTevColor(0)->g = field_0x1318;
                        material->getTevColor(0)->b = field_0x1318;
                    }

                    material->getTevKColor(3)->a = mGlowLightA;
                }

                J3DBlend* blend = material->getBlend();
                if (mAlphaType >= 2) {
                    blend->setDstFactor(5);
                } else {
                    blend->setDstFactor(1);
                }
            }

            mInvisModel.entryDL(NULL);
        }
    } else {
        model = mpEndEfMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);
        mpEndEfBrk->entry(model->getModelData());
        mpEndEfMorf->entryDL();
    }

    f32 var_f31 = tevStr.field_0x344;
    if (mAlphaType == 2) {
        f32 temp_f2 = (1000.0f - current.pos.y) / 1000.0f;
        if (temp_f2 < 0.0f) {
            temp_f2 = 0.0f;
        }

        if (temp_f2 > 1.0f) {
            temp_f2 = 1.0f;
        }

        tevStr.field_0x344 = temp_f2 * ((var_f31 * (f32)mGlowLightA) / 255.0f);
        if (tevStr.field_0x344 > 0.4f) {
            tevStr.field_0x344 = 0.4f;
        }

        mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model, &current.pos, nREG_F(19) + 2000.0f,
                                        0.0f, current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd,
                                        &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    if (mpWeaponModel != NULL && field_0x1240 != 2) {
        g_env_light.setLightTevColorType_MAJI(mpWeaponModel, &tevStr);
        mpWeaponBrk->entry(mpWeaponModel->getModelData());
        fopAcM_setEffectMtx(this, mpWeaponModel->getModelData());
        mDoExt_modelUpdateDL(mpWeaponModel);

        if (mAlphaType == 2) {
            dComIfGd_addRealShadow(mShadowKey, mpWeaponModel);
        } else {
            tevStr.field_0x344 = var_f31;
            mShadowKey =
                dComIfGd_setShadow(mShadowKey, 1, mpWeaponModel, &current.pos, nREG_F(19) + 2000.0f,
                                   0.0f, current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0,
                                   1.0f, dDlst_shadowControl_c::getSimpleTex());
        }
    }

    if (mRopesEnabled) {
        if (mDrawRopes) {
            static GXColor l_color = {0x64, 0x46, 0x00, 0xFF};

            mRope.update(110, l_color, &tevStr);
            dComIfGd_set3DlineMatDark(&mRope);
        }

        for (int i = 0; i < 40; i++) {
            if (!(mCardFlags[i] & 0x80)) {
                g_env_light.setLightTevColorType_MAJI(mpCardModels[i], &tevStr);
                mDoExt_modelUpdateDL(mpCardModels[i]);
            }
        }
    }

    return 1;
}

static int daE_VA_Draw(daE_VA_c* i_this) {
    return i_this->draw();
}

void daE_VA_c::setBck(int i_anmID, u8 i_attr, f32 i_morf, f32 i_rate) {
    mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_VA", i_anmID), i_attr, i_morf, i_rate,
                   0.0f, -1.0f);
}

bool daE_VA_c::checkBck(int i_anmID) {
    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes("E_VA", i_anmID);
    return mpMorf->getAnm() == pbck ? TRUE : FALSE;
}

void daE_VA_c::setActionMode(int i_action, int i_mode) {
    if (dComIfGs_isOneZoneSwitch(6, fopAcM_GetRoomNo(this)) && i_action != ACTION_OPACI_DOWN_e &&
        i_action != ACTION_OPACI_DOWN_DAMAGE_e)
    {
        dComIfGs_offOneZoneSwitch(6, fopAcM_GetRoomNo(this));

        if (!dComIfGs_isOneZoneSwitch(7, fopAcM_GetRoomNo(this))) {
            dComIfGs_onOneZoneSwitch(7, fopAcM_GetRoomNo(this));
        }
    }

    field_0x1381 = 0;
    mAction = i_action;
    mMode = i_mode;
    mGlowBody = 0;
}

int daE_VA_c::getWolfCutType() {
    switch (daPy_getPlayerActorClass()->getCutType()) {
    case daPy_py_c::CUT_TYPE_WOLF_B_LEFT:
    case daPy_py_c::CUT_TYPE_WOLF_B_RIGHT:
        return WL_CUT_TYPE_SMALL;
    case daPy_py_c::CUT_TYPE_WOLF_JUMP:
        return WL_CUT_TYPE_JUMP;
    case daPy_py_c::CUT_TYPE_WOLF_B_FRONT:
    case daPy_py_c::CUT_TYPE_WOLF_B_BACK:
    case daPy_py_c::CUT_TYPE_WOLF_UNK_30:
    case daPy_py_c::CUT_TYPE_WOLF_JUMP_S:
    case daPy_py_c::CUT_TYPE_WOLF_JUMP_S_FINISH:
    case daPy_py_c::CUT_TYPE_WOLF_TURN_LEFT:
    case daPy_py_c::CUT_TYPE_WOLF_TURN_RIGHT:
    case daPy_py_c::CUT_TYPE_WOLF_UNK_35:
    case daPy_py_c::CUT_TYPE_WOLF_LOCK:
        return WL_CUT_TYPE_LARGE;
    }

    return 0;
}

void daE_VA_c::onSwordAtBit() {
    mAttackSphs[0].OnAtSetBit();
    mAttackSphs[1].OnAtSetBit();
    mAttackSphs[2].OnAtSetBit();
    mAttackSphs[3].OnAtSetBit();
}

void daE_VA_c::offSwordShield() {
    if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_WOLF_JUMP) {
        mOffTgTimer = 3;

        for (int i = 0; i < 4; i++) {
            mAttackSphs[i].OffCoSetBit();
            mAttackSphs[i].OffTgShield();
            mAttackSphs[i].OnTgNoHitMark();
        }
    }
}

void daE_VA_c::setTransDamage(cXyz* param_0) {
    u16 hitmark;
    if (field_0x138f >= 4)
        hitmark = 3;
    else
        hitmark = 1;

    dComIfGp_setHitMark(hitmark, this, param_0, NULL, NULL, 0);
    dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
    setActionMode(ACTION_TRANS_DAMAGE_e, 0);
}

void daE_VA_c::damage_check() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    int cut_type = getWolfCutType();
    mBodyCcStts.Move();

    if (mAttackSphIFrameTimer == 0) {
        for (int i = 0; i < 4; i++) {
            if (mAttackSphs[i].ChkTgHit()) {
                if (mAttackSphs[i].GetTgHitObj()->ChkAtType(0xD8000000)) {
                    mAttackSphIFrameTimer = 20;
                } else {
                    mAttackSphIFrameTimer = 10;
                }

                def_se_set(&mSound, mAttackSphs[i].GetTgHitObj(), 0x2A, this);
                mAttackSphs[i].ClrTgHit();
            }
        }
    }

    if (mRopesEnabled) {
        if (mAction == ACTION_DEMO_OP_WAIT_e) {
            for (int i = 0; i < 190; i++) {
                if (mLineSphs[i].ChkTgHit()) {
                    if (mLineSphs[i].GetTgHitObj()->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
                        setActionMode(ACTION_DEMO_OP_e, 0);
                    } else {
                        int var_r24 = i / 19;
                        field_0x1104[var_r24] = nREG_F(17) + 1.0f;
                        field_0x112c[var_r24] = 0;
                        setVibTag(0, var_r24);
                    }
                }
            }
        }
    } else {
        if (mNeckSphIFrameTimer == 0 && mNeckSph.ChkTgHit()) {
            mAtInfo.mpCollider = mNeckSph.GetTgHitObj();

            if (mAtInfo.mpCollider->ChkAtType(0xD8000000)) {
                if (cut_type == WL_CUT_TYPE_SMALL) {
                    mNeckSphIFrameTimer = 15;
                } else {
                    mNeckSphIFrameTimer = 20;
                }
            } else {
                mNeckSphIFrameTimer = 10;
            }

            if (mAtInfo.mAttackPower <= 1) {
                mNeckSphIFrameTimer = KREG_S(8) + 10;
            }

            mOffTgTimer = mNeckSphIFrameTimer;

            if (cut_type != WL_CUT_TYPE_SMALL &&
                mAtInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK) &&
                player->onWolfEnemyHangBite(this))
            {
                setActionMode(ACTION_TRANS_BITE_DAMAGE_e, 0);
                mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 31);
                mBodyCylIFrameTimer = 20;
                return;
            }

            if (mAlphaType == 1 && mGlowBody != 0) {
                mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 32);
                setTransDamage(mNeckSph.GetTgHitPosP());
                return;
            }

            if (cut_type == WL_CUT_TYPE_JUMP) {
                setActionMode(ACTION_TRANS_THROUGH_e, 0);
                mNeckSph.ClrTgHit();
                return;
            }

            mNeckSphIFrameTimer = 0;
            mOffTgTimer = 0;
        }

        if (mBodyCylIFrameTimer == 0) {
            BOOL body_hit = false;
            dCcD_Cyl hit_cyl;

            if (mBodyCyls[0].ChkTgHit()) {
                mAtInfo.mpCollider = mBodyCyls[0].GetTgHitObj();
                body_hit = true;
                hit_cyl = mBodyCyls[0];
            }

            if (mBodyCyls[1].ChkTgHit()) {
                mAtInfo.mpCollider = mBodyCyls[1].GetTgHitObj();
                body_hit = true;
                hit_cyl = mBodyCyls[1];
            }

            if (body_hit) {
                if (mAlphaType != 2 && mAtInfo.mpCollider->ChkAtType(0xD8000000) && mGlowBody == 0)
                {
                    mBodyCyls[0].ClrTgHit();
                    mBodyCyls[1].ClrTgHit();
                    return;
                }

                mAtInfo.field_0x18 = 0;
                health = 100;
                cc_at_check(this, &mAtInfo);
                field_0x1368 = 100 - health;

                if (mAtInfo.mpCollider->ChkAtType(0xD8000000)) {
                    mBodyCylIFrameTimer = 20;
                } else {
                    mBodyCylIFrameTimer = 10;
                }

                if (mAtInfo.mAttackPower <= 1) {
                    mBodyCylIFrameTimer = KREG_S(8) + 10;
                }

                mOffTgTimer = mBodyCylIFrameTimer;

                if (mAlphaType == 1) {
                    setTransDamage(hit_cyl.GetTgHitPosP());
                } else if (mAlphaType == 2) {
                    if (field_0x1381 == 0) {
                        field_0x1385 = 0;

                        if (mAction == ACTION_OPACI_FLY_e) {
                            if (mAtInfo.mpCollider->ChkAtType(0x16060)) {
                                setActionMode(ACTION_OPACI_DAMAGE_e, 0);
                            }
                        } else if (mAtInfo.mpCollider->ChkAtType(0x14050)) {
                            if (field_0x1386 != 0) {
                                setActionMode(ACTION_OPACI_DOWN_DAMAGE_e, 2);
                                mDownTimer = l_HIO.mDownTime;
                                field_0x1381 = 1;
                            } else {
                                dComIfGp_getVibration().StopQuake(31);
                                setActionMode(ACTION_OPACI_DAMAGE_e, 10);
                            }
                        } else if (mAtInfo.mAttackPower != 0) {
                            setActionMode(ACTION_OPACI_DOWN_e, 0);
                            field_0x1381 = 1;
                        }
                    } else {
                        BOOL var_r26 = false;

                        if (player->getCutCount() >= 4) {
                            if (field_0x1383 != 0) {
                                var_r26 = true;
                            }
                        } else if (((dCcD_GObjInf*)mAtInfo.mpCollider)->GetAtSpl() == 1) {
                            field_0x1385++;

                            if (field_0x1385 >= 2) {
                                var_r26 = true;
                            }
                        }

                        int var_r29 = 0;

                        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
                            if (mAtInfo.mpCollider->GetAtAtp() >= 4) {
                                if (player->checkMasterSwordEquip()) {
                                    if (player->getSwordAtUpTime() != 0) {
                                        var_r29 = 6;
                                    } else {
                                        var_r29 = 2;
                                    }
                                } else if (player->getSwordAtUpTime() != 0) {
                                    var_r29 = 2;
                                } else {
                                    var_r29 = 1;
                                }
                            } else if (player->checkMasterSwordEquip()) {
                                if (player->getSwordAtUpTime() != 0 || var_r26) {
                                    var_r29 = 2;
                                }
                            } else if (var_r26) {
                                if (player->getSwordAtUpTime() != 0) {
                                    var_r29 = 2;
                                } else {
                                    var_r29 = 1;
                                }
                            }
                        } else if (mAtInfo.mpCollider->ChkAtType(0xD8000000) &&
                                   (((dCcD_GObjInf*)mAtInfo.mpCollider)->GetAtSpl() == 1 ||
                                    mAtInfo.mpCollider->GetAtAtp() >= 4))
                        {
                            var_r29 = 1;
                        }

                        field_0x1383++;

                        if (var_r29 != 0) {
                            field_0x1385 = 0;
                            field_0x1384 += var_r29;
                            setActionMode(ACTION_OPACI_DOWN_e, 5);
                        } else if (mAtInfo.mpCollider->ChkAtType(0x4050)) {
                            setActionMode(ACTION_OPACI_DOWN_DAMAGE_e, 2);
                        } else {
                            if (player->getCutCount() > 1) {
                                field_0x1382 ^= 1;
                            } else if (cM_rnd() < 0.5f) {
                                field_0x1382 = 0;
                            } else {
                                field_0x1382 = 1;
                            }

                            setActionMode(ACTION_OPACI_DOWN_DAMAGE_e, field_0x1382);
                        }

                        field_0x1381 = 1;
                    }
                }

                mBodyCyls[0].ClrTgHit();
                mBodyCyls[1].ClrTgHit();
            }
        }
    }
}

void daE_VA_c::setFireEffect(int param_0) {
    cXyz scale(1.0f, 1.0f, 1.0f);
    int idx = param_0 * 3;

    field_0x10f80[idx] =
        dComIfGp_particle_set(field_0x10f80[idx], 0x3AD, &field_0x1140[param_0], NULL, &scale);
    field_0x10f80[idx + 1] =
        dComIfGp_particle_set(field_0x10f80[idx + 1], 0x3AF, &field_0x1140[param_0], NULL, &scale);
    field_0x10f80[idx + 2] =
        dComIfGp_particle_set(field_0x10f80[idx + 2], 0x3AE, &field_0x1140[param_0], NULL, &scale);
}

f32 dummy() {
    return 3.2f;
}

void daE_VA_c::setFootEffect() {
    if (!field_0x1388) {
        int var_r6 = 0;
        if (current.pos.y < 10.0f) {
            var_r6 = 2;
        }

        cXyz pos(current.pos);
        cXyz scale(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);

        static u16 va_foot_smoke_id[] = {0x8891, 0x8892, 0x85C4, 0x85C5};
        mFootSmokeEmtrIDs[0] =
            dComIfGp_particle_set(mFootSmokeEmtrIDs[0], va_foot_smoke_id[var_r6], &pos, &tevStr,
                                  &shape_angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
        mFootSmokeEmtrIDs[1] =
            dComIfGp_particle_set(mFootSmokeEmtrIDs[1], va_foot_smoke_id[var_r6 + 1], &pos, &tevStr,
                                  &shape_angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mFootSmokeEmtrIDs[0]);
        if (emitter != NULL) {
            emitter->setGlobalAlpha(mFootSmokeAlpha);
        }

        emitter = dComIfGp_particle_getEmitter(mFootSmokeEmtrIDs[1]);
        if (emitter != NULL) {
            emitter->setGlobalAlpha(mFootSmokeAlpha);
        }
    }
}

static u16 va_wepon_eff_id1[] = {
    0x85D8,
    0x85D9,
    0x85DA,
};

static u16 va_wepon_eff_id2[] = {
    0x85C6,
    0x85C7,
    0x85C8,
};

void daE_VA_c::setWeponEffect() {
    cXyz pos;
    cXyz scale(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);

    switch (mWeponEfMode) {
    case 1:
        pos.set(11.0f, 0.0f, -7.0f);
        for (int i = 0; i < 3; i++) {
            mWeponEffEmtrIDs[i] =
                dComIfGp_particle_set(mWeponEffEmtrIDs[i], va_wepon_eff_id1[i], &pos, &tevStr,
                                      &shape_angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
        }
        break;
    case 2:
        pos.set(11.0f, 0.0f, -7.0f);
        field_0x10f74 = dComIfGp_particle_set(field_0x10f74, 0x8601, &pos, &tevStr, &shape_angle,
                                              &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
        /* fallthrough */
    case 3:
        pos = current.pos;
        MtxP model_mtx = mpWeaponModel->getBaseTRMtx();
        mSound.startCreatureSoundLevel(Z2SE_EN_VA_SWD_FLY, 0, -1);

        for (int i = 0; i < 3; i++) {
            mWeponEffEmtrIDs[i] =
                dComIfGp_particle_set(mWeponEffEmtrIDs[i], va_wepon_eff_id2[i], &pos, &tevStr,
                                      &shape_angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mWeponEffEmtrIDs[i]);
            if (emitter != NULL) {
                emitter->setGlobalSRTMatrix(model_mtx);
            }
        }
        break;
    }
}

static u16 va_bugs_eff_jnt[] = {
    0x01, 0x02, 0x22, 0x04, 0x05, 0x07, 0x10, 0x11, 0x13, 0x2C, 0x2F, 0x32,
};

static u16 va_bugs_eff_id[] = {
    0x85E1, 0x85E2, 0x85E3, 0x85DB, 0x85DC, 0x85DD, 0x85DE, 0x85DF, 0x85E0, 0x85E4, 0x85E5, 0x85E6,
};

void daE_VA_c::setBugsEffect() {
    J3DModel* model = mpMorf->getModel();
    JGeometry::TVec3<f32> scale(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);

    for (int i = 0; i < 12; i++) {
        JPABaseEmitter* emitter =
            dComIfGp_particle_set(va_bugs_eff_id[i], &current.pos, NULL, NULL);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(va_bugs_eff_jnt[i]));
            emitter->setGlobalParticleScale(scale);
        }
    }
}

static u16 va_weapon_land_eff_id[] = {
    0x85C9,
    0x85CA,
    0x85CB,
};

void daE_VA_c::setWeponLandEffect() {
    cXyz scale(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);

    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(va_weapon_land_eff_id[i], &current.pos, &tevStr, &shape_angle,
                              &scale);
    }
}

void daE_VA_c::setWeponGlow() {
    mWeponEffEmtrIDs[0] = dComIfGp_particle_set(mWeponEffEmtrIDs[0], 0x8600, &current.pos, &tevStr);

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mWeponEffEmtrIDs[0]);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(mpWeaponModel->getBaseTRMtx());
    }
}

void daE_VA_c::setWeponFlashScreen() {
    cXyz pos(0.0f, 0.0f, 0.0f);
    dComIfGp_particle_set(0x85FE, &pos, &tevStr, NULL, NULL);
    dComIfGp_particle_set(0x85FF, &pos, &tevStr, NULL, NULL);
}

static u16 va_magic_eff_id[] = {
    0x8848,
    0x8849,
    0x884A,
};

void daE_VA_c::setMagicEffect(int param_0) {
    int idx = param_0 * 3;

    for (int i = 0; i < 3; i++) {
        field_0x10f80[idx + i] = dComIfGp_particle_set(field_0x10f80[idx + i], va_magic_eff_id[i],
                                                       &mMagicPos[param_0], NULL, NULL);
    }
}

void daE_VA_c::setMagicHitEffect(int param_0) {
    dComIfGp_particle_set(0x8846, &mMagicPos[param_0], NULL, NULL);
    dComIfGp_particle_set(0x8847, &mMagicPos[param_0], NULL, NULL);
}

void daE_VA_c::demo_skip(int param_0) {
    Z2GetAudioMgr()->bgmStreamStop(30);
    cDmr_SkipInfo = 1;
    dStage_changeScene(1, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
}

int daE_VA_c::DemoSkipCallBack(void* i_this, int param_1) {
    if (i_this != NULL) {
        ((daE_VA_c*)i_this)->demo_skip(param_1);
        return 1;
    }

    return 0;
}

f32 daE_VA_c::getRopeLength(int param_0) {
    if (param_0 < 5) {
        return nREG_F(18) + 115.0f;
    } else {
        return nREG_F(19) + 65.0f;
    }
}

static f32 line_pull_power[] = {
    0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f,
};

static cXyz line_end_pos[] = {
    cXyz(0.0f, 5.0f, 990.0f),     cXyz(971.0f, 5.0f, 326.0f),  cXyz(582.0f, 5.0f, -801.0f),
    cXyz(-582.0f, 5.0f, -801.0f), cXyz(-942.0f, 5.0f, 306.0f), cXyz(202.0f, 5.0f, 501.0f),
    cXyz(559.0f, 5.0f, -17.0f),   cXyz(458.0f, 5.0f, -306.0f), cXyz(-131.0f, 5.0f, -524.0f),
    cXyz(-414.0f, 5.0f, 347.0f),
};

static cXyz line_start_pos[] = {
    cXyz(-5.0f, -10.0f, -15.0f), cXyz(0.0f, 0.0f, -10.0f), cXyz(0.0f, -10.0f, -10.0f),
    cXyz(0.0f, 0.0f, -5.0f),     cXyz(0.0f, -5.0f, -5.0f), cXyz(0.0f, 25.0f, -15.0f),
    cXyz(0.0f, 5.0f, -10.0f),    cXyz(3.0f, 25.0f, -3.0f), cXyz(8.0f, 25.0f, 0.0f),
    cXyz(-8.0f, 23.0f, 8.0f),
};

void daE_VA_c::calcOpRopePos() {
    cXyz sp60;
    cXyz sp6C;
    cXyz sp78;
    cXyz sp84;
    cXyz sp90;

    int idx;
    s16 var_r30;

    for (int i = 0; i < 10; i++) {
        int temp_3C = i * 10;
        mDoMtx_stack_c::transS(field_0x122c);
        mDoMtx_stack_c::transM(0.0f, 150.0f, 0.0f);
        mDoMtx_stack_c::ZXYrotM(field_0x1238);
        mDoMtx_stack_c::transM(0.0f, -150.0f, 0.0f);
        mDoMtx_stack_c::transM(0.0f, 400.0f, 0.0f);
        mDoMtx_stack_c::XrotM(field_0x123e);
        mDoMtx_stack_c::transM(0.0f, -400.0f, 0.0f);
        mDoMtx_stack_c::transM(line_start_pos[i]);
        mDoMtx_stack_c::multVecZero(&sp60);

        sp6C = line_end_pos[i];
        if (i == nREG_S(0)) {
            sp6C.x += nREG_F(8);
            sp6C.z += nREG_F(9);
        }

        f32 var_f31 = 0.0f;

        if (field_0x1104[i]) {
            field_0x112c[i] += nREG_S(0) + 0x800;
            cLib_chaseF(&field_0x1104[i], 0.0f, nREG_F(19) + 0.01f);

            var_r30 = (s16)cLib_targetAngleY(&sp60, &sp6C);
            var_f31 = field_0x1104[i] * cM_ssin(field_0x112c[i]);
        }

        for (int j = 0; j < 10; j++) {
            idx = temp_3C + j;

            if (mRopeCutStatus[idx] & 2) {
                if (j == 0) {
                    field_0x994[idx] = sp60;
                } else {
                    field_0xe44[idx].y = 0x3130;

                    if (abs(field_0xe44[idx].z) <= 0x20 && abs(field_0xe44[idx].x + 0x4000) <= 0x20)
                    {
                        field_0xe44[idx].x = -0x4000;
                        field_0xe44[idx].z = 0;
                    } else if (field_0xe44[idx].x < -0x4000) {
                        field_0xe44[idx].z += 0x20;
                    } else {
                        field_0xe44[idx].z -= 0x20;
                    }

                    cLib_chaseAngleS(&field_0xe44[idx].z, 0, 8);
                    field_0xe44[idx].x += field_0xe44[idx].z;

                    f32 rope_len = getRopeLength(i);
                    sp84.y = rope_len * cM_ssin(field_0xe44[idx].x);

                    f32 temp_f31_2 = rope_len * cM_scos(field_0xe44[idx].x);
                    sp84.x = temp_f31_2 * cM_scos(field_0xe44[idx].y);
                    sp84.z = temp_f31_2 * cM_ssin(field_0xe44[idx].y);

                    sp78 = field_0x994[idx - 1] + sp84;
                    cLib_chasePos(&field_0x994[idx], sp78, 10.0f);
                }
            } else if (mRopeCutStatus[idx] & 4) {
                field_0x994[idx].y += -5.0f;
                if (field_0x994[idx].y < 5.0f) {
                    field_0x994[idx].y = 5.0f;
                }
            } else if (mRopeCutStatus[idx] & 8) {
                field_0x994[idx].y += -3.0f;
                if (field_0x994[idx].y < 5.0f) {
                    field_0x994[idx].y = 5.0f;
                }
            } else {
                if (j == 0) {
                    field_0x994[idx] = sp60;
                } else if (j == 9) {
                    field_0x994[idx] = sp6C;
                } else {
                    sp84 = field_0x994[idx] - sp60;
                    sp84.normalizeZP();

                    sp90 = sp6C - field_0x994[idx];
                    sp90.normalizeZP();

                    sp78 = (sp84 * line_pull_power[10 - j]) * (nREG_F(12) + 300.0f);
                    sp78 += (sp90 * line_pull_power[j]) * (nREG_F(13) + 300.0f);
                    sp78.y += -5.0f;
                    sp78.normalizeZP();

                    field_0x994[idx] = field_0x994[idx - 1] + (sp78 * getRopeLength(i));
                    if (field_0x994[idx].y < 5.0f) {
                        field_0x994[idx].y = 5.0f;
                    }

                    if (var_f31) {
                        f32 temp_f29 = (nREG_F(16) + 0.2f) * ((f32)(10 - j) * var_f31);
                        field_0x994[idx].x += temp_f29 * cM_scos(var_r30);
                        field_0x994[idx].z += temp_f29 * cM_ssin(var_r30);
                    }
                }
            }
        }
    }

    cXyz* pos;
    for (int i = 0; i < 11; i++) {
        pos = mRope.getPos(i);
        int temp_r5_2;
        if (i != 10) {
            int temp_r5 = i * 10;
            int sp68 = field_0x11b8[i] / getRopeLength(i);

            for (int j = 0; j < 10; j++, pos++) {
                temp_r5_2 = temp_r5 + j;
                *pos = field_0x994[temp_r5_2];

                if (sp68 >= 4) {
                    if (j <= sp68 && field_0x11b8[i]) {
                        *pos = field_0x1140[i];
                        mRopeCutStatus[temp_r5_2] |= 1;
                    } else if ((mRopeCutStatus[temp_r5_2] & 0x10) && j < 4) {
                        *pos = field_0x994[temp_r5 + 4];
                    }
                } else if ((mRopeCutStatus[temp_r5_2] & 0x10) && j < 4) {
                    *pos = field_0x994[temp_r5 + 4];
                } else if (j <= sp68 && field_0x11b8[i]) {
                    *pos = field_0x1140[i];
                    mRopeCutStatus[temp_r5_2] |= 1;
                }
            }

        } else {
            int temp_r5 = 50;
            int sp68 = field_0x11b8[5] / getRopeLength(5);

            for (int j = 0; j < 10; j++, pos++) {
                temp_r5_2 = temp_r5 + j;
                *pos = field_0x994[temp_r5_2];

                if (sp68 > 3) {
                    *pos = field_0x994[temp_r5 + 9];
                } else if (j <= sp68 && field_0x11b8[5]) {
                    *pos = field_0x1140[5];
                } else if ((mRopeCutStatus[temp_r5_2] & 0x10) && j >= 3) {
                    *pos = field_0x994[temp_r5 + 3];
                }
            }
        }
    }

    for (int i = 0; i < 40; i++) {
        int idx = va_tag_set_num[i];

        if (mRopeCutStatus[idx] & 1) {
            mCardFlags[i] |= 0x80;
        }

        if (mRopeCutStatus[idx] & 0xC) {
            mCardFlags[i] |= 0x8;
        }

        field_0x660[i].y = cLib_targetAngleY(&field_0x994[idx - 1], &field_0x994[idx + 1]) + 0x4000;
    }
}

void daE_VA_c::onRopeCutStatus(int param_0, int param_1, int param_2) {
    int var_r31 = param_0 * 10;

    for (int i = 0; i < 10; i++) {
        int var_r24 = var_r31 + i;

        if (param_2 == 0) {
            mRopeCutStatus[var_r24] |= 0x10;
        }

        if (i < param_1) {
            mRopeCutStatus[var_r24] |= 2;

            if (param_2 == 0) {
                field_0xe44[var_r24].x =
                    cLib_targetAngleX(&field_0x994[var_r24 - 1], &field_0x994[var_r24]);
                field_0xe44[var_r24].y =
                    cLib_targetAngleY(&field_0x994[var_r24 - 1], &field_0x994[var_r24]);
                field_0xe44[var_r24].z = 0;
            }
        } else if (param_2 == 0) {
            mRopeCutStatus[var_r24] |= 4;
        } else {
            mRopeCutStatus[var_r24] |= 8;
        }
    }
}

void daE_VA_c::setVibRope(f32 param_0, f32 param_1) {
    field_0x1336 += (s16)(param_1 * 7168.0f);
    field_0x122c.y += (s16)(param_0 * cM_ssin(field_0x1336));
    field_0x123e += (s16)(param_0 * 50.0f * cM_ssin(field_0x1336));
}

static s16 TAG_VIB_ANGLE[] = {
    -0x800,
    0x800,
    -0x1000,
    0x1000,
};

static s16 TAG_VIB_WIDTH[] = {
    0x1C0, 0x280, 0x180, 0x240, 0x380, 0x500, 0x300, 0x480,
};

void daE_VA_c::setVibTag(int param_0, int param_1) {
    for (int i = 0; i < 40; i++) {
        if (param_1 == -1 || param_1 == va_tag_set_num[i] / 10) {
            mCardFlags[i] |= 4;

            if (!(mCardFlags[i] & 8)) {
                switch (param_0) {
                case 0:
                    field_0x750[i].x += TAG_VIB_ANGLE[(int)cM_rndF(3.9f)];
                    field_0x750[i].z += TAG_VIB_ANGLE[(int)cM_rndF(3.9f)];
                    field_0x840[i].x = TAG_VIB_WIDTH[(int)(cM_rndF(3.9f) + 4.0f)];
                    field_0x840[i].z = TAG_VIB_WIDTH[(int)(cM_rndF(3.9f) + 4.0f)];
                    break;
                case 1:
                    field_0x750[i].x += TAG_VIB_ANGLE[i & 3];
                    field_0x750[i].z += TAG_VIB_ANGLE[(i >> 1) & 3];
                    field_0x840[i].x = TAG_VIB_WIDTH[i & 3];
                    field_0x840[i].z = TAG_VIB_WIDTH[(i >> 2) & 3];
                    break;
                case 2:
                    field_0x750[i].x += TAG_VIB_ANGLE[(i & 1) + 2];
                    field_0x750[i].z += TAG_VIB_ANGLE[((i >> 1) & 1) + 2];
                    field_0x840[i].x = TAG_VIB_WIDTH[(i & 3) + 4];
                    field_0x840[i].z = TAG_VIB_WIDTH[((i >> 2) & 3) + 4];
                    break;
                }

                field_0x660[i].x = field_0x840[i].x * cM_ssin(field_0x750[i].x);
                field_0x660[i].z = field_0x840[i].z * cM_ssin(field_0x750[i].z);
            }
        }
    }
}

void daE_VA_c::calcTagAngle() {
    for (int i = 0; i < 40; i++) {
        if (mCardFlags[i] & 8) {
            f32 var_f30;
            switch (mCardFlags[i] & 3) {
            case 0:
                var_f30 = 160.0f;
                break;
            case 1:
                var_f30 = 100.0f;
                break;
            case 2:
                var_f30 = 50.0f;
                break;
            }

            f32 var_f31 = field_0x994[va_tag_set_num[i]].y - 5.0f;
            if (var_f31 < var_f30) {
                if (var_f31) {
                    f32 var_f29 = JMAFastSqrt(var_f30 * var_f30 - var_f31 * var_f31);
                    if (field_0x660[i].x < 0) {
                        field_0x660[i].x = -cM_atan2s(var_f29, var_f31);
                    } else {
                        field_0x660[i].x = cM_atan2s(var_f29, var_f31);
                    }
                }

                field_0x660[i].z = 0;
                mCardFlags[i] |= 4;
            }
        }

        if (!(mCardFlags[i] & 4)) {
            field_0x750[i].x += TAG_VIB_ANGLE[i & 1];
            field_0x750[i].z += TAG_VIB_ANGLE[(i >> 1) & 1];

            cLib_chaseAngleS(&field_0x840[i].x, 0, 4);
            cLib_chaseAngleS(&field_0x840[i].z, 0, 4);

            field_0x660[i].x = field_0x840[i].x * cM_ssin(field_0x750[i].x);
            field_0x660[i].z = field_0x840[i].z * cM_ssin(field_0x750[i].z);
        }

        mCardFlags[i] &= ~4;
    }
}

void daE_VA_c::calcRopeFirePos() {
    int temp_r28;
    int var_r24;
    f32 var_f29;
    BOOL var_r31;

    cXyz sp48;
    for (int i = 0; i < 10; i++) {
        field_0x11b8[i] += nREG_F(15) + 60.0f;

        var_r24 = field_0x11b8[i] / getRopeLength(i);
        var_f29 = field_0x11b8[i] - ((f32)var_r24 * getRopeLength(i));
        var_r31 = 1;

        if (var_r24 < 9) {
            temp_r28 = var_r24 + (i * 10);
            sp48 = field_0x994[temp_r28 + 1] - field_0x994[temp_r28];
            sp48.normalizeZP();
            field_0x1140[i] = field_0x994[temp_r28] + (sp48 * var_f29);
        } else if (cLib_chasePos(&field_0x1140[i], field_0x994[(i * 10) + 9], nREG_F(15) + 60.0f)) {
            var_r31 = 0;
        }

        if (var_r31) {
            setFireEffect(i);
        }
    }
}

void daE_VA_c::executeDemoOpWait() {
    cXyz sp34;

    switch (mMode) {
    case 0:
        mRopesEnabled = true;
        mpWeaponBrk->setPlaySpeed(0.0f);
        mpWeaponBrk->setFrame(0.0f);

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (i < 5) {
                    field_0x994[j + i * 10].x = j * 110.0f * cM_ssin(i * 0x3333);
                    field_0x994[j + i * 10].z = j * 110.0f * cM_scos(i * 0x3333);
                    field_0x994[j + i * 10].y = 275.0f - j * 30.0f;
                } else {
                    field_0x994[j + i * 10].x = j * 60.0f * cM_ssin(i * 0x3333);
                    field_0x994[j + i * 10].z = j * 60.0f * cM_scos(i * 0x3333);
                    field_0x994[j + i * 10].y = 275.0f - j * 30.0f;
                }
            }
        }

        field_0x122c.set(51.0f, 0.0f, 0.0f);
        field_0x1238.set(0, -0x5000, -0x8000);
        field_0x123e = 0;
        mMode = 1;
        mDrawRopes = true;
    case 1:
        for (int i = 0; i < 190; i++) {
            if (!daPy_getPlayerActorClass()->checkNowWolf()) {
                mLineSphs[i].OffTgShield();
            } else {
                mLineSphs[i].OnTgShield();
            }
        }
        calcOpRopePos();
        break;
    }

    calcTagAngle();
}

void daE_VA_c::executeDemoOp() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp2C;
    cXyz sp38;
    daPy_py_c* player = daPy_getPlayerActorClass();

    calcOpRopePos();

    switch (mMode) {
    case 0:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
            eventInfo.onCondition(2);
            return;
        }

        dComIfGs_onOneZoneSwitch(9, fopAcM_GetRoomNo(this));
        player->changeOriginalDemo();

        sp38.set(270.0f, 0.0f, 210.0f);
        player->setPlayerPosAndAngle(&sp38, -0x6000, 0);
        player->changeDemoMode(1, 0, 0, 0);

        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);

        mDemoCamEye.set(300.0f, 300.0f, 600.0f);
        mDemoCamCenter.set(100.0f, 180.0f, 200.0f);
        mDemoCamBank = 60.0f;
        mDemoModeTimer = nREG_S(4) + 60;
        field_0x14a8 = 2.5f;

        onRopeCutStatus(5, 4, 0);

        Z2GetAudioMgr()->seStart(Z2SE_EN_VA_OPENING1, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        mMode = 1;
        Z2GetAudioMgr()->bgmStreamPrepare(0x200000A);
        Z2GetAudioMgr()->bgmStreamPlay();
        return;
    case 1:
        sp38.set(270.0f, 0.0f, 210.0f);
        player->setPlayerPosAndAngle(&sp38, -0x6000, 0);
        sp38.set(-50.0f, 90.0f, -100.0f);

        cLib_chasePos(&mDemoCamEye, sp38, field_0x14a8);
        cLib_chasePos(&mDemoCamCenter, sp38, field_0x14a8);

        if (mDemoModeTimer == 0) {
            mDownTimer = 98;
            mpWeaponBrk->setPlaySpeed(nREG_F(1) + 1.0f);
            mpWeaponBrk->setFrame(0.0f);
            field_0x1336 = 0;
            mMode = 2;
            mDemoModeTimer = 60 - nREG_S(4);
        }
        break;
    case 2:
        if (mpWeaponBrk->checkFrame(48.0f)) {
            mpWeaponBrk->setPlaySpeed(0.0f);
        }

        if (mDemoModeTimer != 0) {
            sp38.set(-50.0f, 90.0f, -100.0f);
            cLib_chasePos(&mDemoCamEye, sp38, field_0x14a8);
            cLib_chasePos(&mDemoCamCenter, sp38, field_0x14a8);

            if (mDemoModeTimer < 25) {
                cLib_chaseF(&field_0x14a8, 0.0f, 0.1f);

                if (mDemoModeTimer > 15 && mDemoModeTimer < 25) {
                    setVibRope(1.0f, 1.0f);
                }

                if (mDemoModeTimer > 12 && mDemoModeTimer < 22) {
                    setVibTag(1, -1);
                }

                if (mDemoModeTimer == 0) {
                    mMode = 3;
                }
            }
        } else if (mDownTimer == 0) {
            mMode = 4;
            mDemoModeTimer = 75;
        }
        break;
    case 4:
        setWeponGlow();

        if (mDemoModeTimer < 50) {
            setVibRope(1.0f, 1.3f);

            if (mDemoModeTimer < 45) {
                setVibTag(2, -1);
            }

            if (mDemoModeTimer == 0) {
                setWeponFlashScreen();
                mMode = 5;
                mDemoModeTimer = 50;

                for (int i = 0; i < 10; i++) {
                    field_0x1140[i] = field_0x994[i * 10];
                    onRopeCutStatus(i, 0, 1);
                }

                field_0x14a8 = 50.0f;

                dComIfGp_getVibration().StartQuake(6, 31, cXyz(0.0f, 1.0f, 0.0f));
                sp38.set(400.0f, 0.0f, 300.0f);
                player->setPlayerPosAndAngle(&sp38, -0x6000, 0);
                player->changeDemoMode(0x17, 0, 0, 0);
                return;
            }
        }
        break;
    case 5:
        sp38.set(459.0f, 394.0f, 920.0f);
        cLib_chasePos(&mDemoCamEye, sp38, field_0x14a8);

        sp38.set(259.0f, 275.0f, 519.0f);
        cLib_chasePos(&mDemoCamCenter, sp38, field_0x14a8);
        cLib_chaseF(&field_0x14a8, 0.0f, 1.0f);

        if (mDemoModeTimer == 20) {
            dComIfGp_getVibration().StopQuake(31);
        }

        setVibRope(1.0f, 1.3f);
        setVibTag(2, -1);
        calcRopeFirePos();

        if (mDemoModeTimer == 0) {
            sp38.set(800.0f, 0.0f, 380.0f);
            player->setPlayerPosAndAngle(&sp38, -0x5000, 0);
            mMode = 6;

            mpWeaponBrk->setPlaySpeed(1.0f);
            mDemoCamCenter.set(field_0x122c.x, 150.0f, field_0x122c.z);
            field_0x14ac = 0x1800;
            cLib_chaseAngleS(&field_0x14ac, 0x2800, 0x20);

            sp38.set(0.0f, 150.0f, 0.0f);
            sp2C.set(0.0f, 50.0f, 400.0f);
            cLib_offsetPos(&mDemoCamEye, &sp38, field_0x14ac, &sp2C);
            mDemoCamBank = 60.0f;
            mWeponEfMode = 1;
            mDemoModeTimer = 130;

            if (!player->checkNowWolf()) {
                player->changeDemoMode(0x17, 0, 0, 0);
            }

            Z2GetAudioMgr()->seStart(Z2SE_EN_VA_OPENING2, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            return;
        }
        break;
    case 6:
        mMode = 7;
        field_0x122c.y = -100.0f;
    case 7:
        if (mDemoModeTimer > 115 || (mDemoModeTimer < 90 && mDemoModeTimer > 60)) {
            field_0x122c.y += 1.0f;
            setVibRope(2.0f, 1.0f);
        } else if (mDemoModeTimer < 10) {
            field_0x122c.y += 1.0f;
            setVibRope(2.0f, 1.3f);
        } else if (mDemoModeTimer < 30) {
            field_0x122c.y += nREG_F(1) + 5.0f;
            setVibRope(2.0f, 1.0f);
        } else {
            field_0x122c.y += 0.2f;
            setVibRope(1.0f, 1.0f);
        }

        mDemoCamCenter.set(field_0x122c.x, 150.0f, field_0x122c.z);
        cLib_chaseAngleS(&field_0x14ac, 0x2800, 0x20);

        sp38.set(0.0f, 150.0f, 0.0f);
        sp2C.set(0.0f, 50.0f, 400.0f);
        cLib_offsetPos(&mDemoCamEye, &sp38, field_0x14ac, &sp2C);

        if (mDemoModeTimer == 0) {
            mMode = 8;
            mDemoModeTimer = 300;
            field_0x14a4 = 600.0f;
            mWeponEfMode = 2;
            field_0x1336 = 0x1400;
            field_0x1334 = 0xA00;
            field_0x1310 = 28.0f;
            field_0x123e = 0;
            mDrawRopes = false;
            mKankyoColType = 4;
        }
        break;
    case 8:
        mDemoCamCenter.set(field_0x122c.x, field_0x122c.y + 150.0f, field_0x122c.z);
        sp38.set(950.0f, 50.0f, 550.0f);
        cLib_chasePos(&mDemoCamEye, sp38, 15.0f);

        field_0x1310--;
        if (field_0x1310 < -10.0f) {
            field_0x1310 = -10.0f;
        }

        field_0x122c.y += field_0x1310;

        if (mDemoModeTimer < 267) {
            cLib_chaseAngleS(&field_0x1336, 0, 0x100);
        }

        if (mDemoModeTimer < 254) {
            cLib_chaseAngleS(&field_0x1334, 0, 0x100);
        }

        field_0x1238.z += field_0x1336;
        field_0x1238.x += field_0x1334;

        if (mDemoModeTimer < 270) {
            cLib_addCalcAngleS(&field_0x1238.x, -0x0C03, 4, 0x200, 0x100);
            cLib_addCalcAngleS(&field_0x1238.y, -0x43B1, 4, 0x200, 0x100);
            cLib_addCalcAngleS(&field_0x1238.z, -0x195D, 4, 0x200, 0x100);
        }

        sp38.set(26.0f, 187.0f, 236.0f);
        f32 old = field_0x122c.y;

        if (cLib_chasePosXZ(&field_0x122c, sp38, 4.2f)) {
            field_0x122c.y = old;
        }

        if (field_0x1310 < 0.0f && field_0x122c.y < 192.0f) {
            field_0x122c = sp38;
            field_0x1238.set(-0x0C03, -0x43B1, -0x195D);

            setBck(ANM_INVI_WAIT_e, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 9;
            field_0x1240 = 1;
            mDemoModeTimer = 60;
            mWeponEfMode = 3;
        }
        break;
    case 9:
        cLib_chaseAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x98);
        current.angle.y = shape_angle.y;

        sp38.set(field_0x122c.x, field_0x122c.y + 150.0f, field_0x122c.z);
        cLib_chasePos(&mDemoCamCenter, sp38, 5.0f);

        sp38.set(950.0f, 50.0f, 550.0f);
        cLib_chasePos(&mDemoCamEye, sp38, 15.0f);

        if (mDemoModeTimer == 0) {
            setBck(ANM_OP_ATTACK_A_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mMode = 10;
        }
        break;
    case 10:
        mDemoCamCenter.set(field_0x122c.x, field_0x122c.y + 150.0f, field_0x122c.z);
        sp38.set(950.0f, 50.0f, 550.0f);
        cLib_chasePos(&mDemoCamEye, sp38, nREG_F(19) + 15.0f);

        if (mpMorf->checkFrame(50.0f)) {
            player->changeDemoMode(0x22, 0, 0, 0);
        }

        if (mpMorf->checkFrame(67.0f)) {
            setWeponLandEffect();
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_IMPACT, 0, -1);
            dComIfGp_getVibration().StartShock(5, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (mpMorf->isStop()) {
            camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye, mDemoCamBank, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();

            mRopesEnabled = false;
            setActionMode(ACTION_CLEAR_WAIT_e, 0);
            mDemoModeTimer = 60;
            Z2GetAudioMgr()->subBgmStart(Z2BGM_VARIANT);
            Z2GetAudioMgr()->changeSubBgmStatus(1);
            return;
        }
    }

    calcTagAngle();
    camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamBank, 0);
    dComIfGp_getEvent().onSkipFade();
    dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 0);
}

void daE_VA_c::executeClearWait() {
    switch (mMode) {
    case 1:
        mDemoModeTimer = 60;
        /* fallthrough */
    case 0:
        setBck(ANM_INVI_WAIT_e, J3DFrameCtrl::EMode_LOOP, 20.0f, 1.0f);
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        mMode = 2;
        /* fallthrough */
    case 2:
        cLib_addCalc(&current.pos.y, 0.0f, 0.1f, 10.0f, 1.0f);

        if (mDemoModeTimer == 0) {
            if (fopAcM_searchPlayerDistance(this) < 2000.0f) {
                setActionMode(ACTION_CLEAR_CHASE_e, 0);
            }
        }

        if (daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
            setActionMode(ACTION_TRANS_WAIT_e, 0);
        }
    }
}

void daE_VA_c::executeClearChase() {
    cLib_addCalc(&current.pos.y, 0.0f, 0.1f, 10.0f, 1.0f);

    switch (mMode) {
    case 0:
        setBck(ANM_INVI_WAIT_e, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        mMode = 1;
        /* fallthrough */
    case 1:
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x400, 0x40);
        current.angle.y = shape_angle.y;

        if (fopAcM_searchPlayerDistance(this) > l_HIO.mAttackRange) {
            cLib_chaseF(&speedF, 10.0f, 1.0f);
        } else if (fopAcM_searchPlayerDistance(this) < l_HIO.mAttackRange - 200.0f) {
            cLib_chaseF(&speedF, -10.0f, 1.0f);
        } else {
            cLib_chaseF(&speedF, 0.0f, 1.0f);

            if (speedF == 0.0f && mDemoModeTimer == 0) {
                setActionMode(ACTION_CLEAR_ATTACK_e, 0);
                return;
            }
        }

        if (speedF) {
            if (!checkBck(ANM_INVI_WALK_e)) {
                setBck(ANM_INVI_WALK_e, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            }
        } else {
            if (!checkBck(ANM_INVI_WAIT_e)) {
                setBck(ANM_INVI_WAIT_e, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            }
        }
    }

    if (daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
        setActionMode(ACTION_TRANS_WAIT_e, 0);
    }
}

void daE_VA_c::executeClearAttack() {
    f32 anm_frame = mpMorf->getFrame();
    cLib_addCalc(&current.pos.y, 0.0f, 0.1f, 10.0f, 1.0f);

    switch (mMode) {
    case 0:
        setBck(ANM_INVI_ATTACK_A_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK_T, 0, -1);
        mMode = 1;
        return;
    case 1:
        if (mpMorf->checkFrame(32.0f)) {
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK1, 0, -1);
        }

        if (mpMorf->checkFrame(36.0f)) {
            setWeponLandEffect();
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_IMPACT, 0, -1);
            dComIfGp_getVibration().StartShock(5, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (30.0f <= anm_frame && anm_frame <= 43.0f) {
            onSwordAtBit();
        } else if (anm_frame > 43.0f && anm_frame < 85.0f) {
            offSwordShield();
        }

        if (mpMorf->isStop()) {
            setActionMode(ACTION_CLEAR_CHASE_e, 0);
            mDemoModeTimer = 150;
        }
        break;
    }

    if (daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
        setActionMode(ACTION_TRANS_WAIT_e, 0);
    }
}

void daE_VA_c::executeTransWait() {
    switch (mMode) {
    case 0:
        Z2GetAudioMgr()->changeSubBgmStatus(2);
        mAlphaType = 1;
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        speedF = 0.0f;
        mMode = 1;
        setBck(ANM_TRANS_WAIT_e, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
        mDemoModeTimer = 5;

        if (!dComIfGs_isOneZoneSwitch(5, fopAcM_GetRoomNo(this))) {
            dComIfGs_onOneZoneSwitch(5, fopAcM_GetRoomNo(this));
        }
        /* fallthrough */
    case 1:
        cLib_addCalc(&current.pos.y, 0.0f, 0.1f, 10.0f, 1.0f);
        if (mDemoModeTimer == 0) {
            setBck(ANM_INVI_NOTICE_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_FOUND, -1);
            mMode = 2;
        }
        break;
    case 2:
        cLib_addCalc(&current.pos.y, 0.0f, 0.1f, 10.0f, 1.0f);
        field_0x1391 = 1;

        if (mpMorf->isStop()) {
            setActionMode(ACTION_TRANS_CHASE_e, 0);
        }
        break;
    }

    if (!daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
        Z2GetAudioMgr()->changeSubBgmStatus(1);
        setActionMode(ACTION_OPACI_FADE_AWAY_e, 0);
    }
}

void daE_VA_c::executeTransChase() {
    f32 dist_to_player = fopAcM_searchPlayerDistanceXZ(this);
    s16 angleY_to_player = fopAcM_searchPlayerAngleY(this);

    switch (mMode) {
    case 0:
        setBck(ANM_TRANS_WAIT_e, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
        mMode = 1;
        /* fallthrough */
    case 1:
        if (abs((s16)(shape_angle.y - angleY_to_player)) > 0x6000) {
            shape_angle.y += 0x400;
        } else {
            cLib_addCalcAngleS(&shape_angle.y, angleY_to_player, 8, 0x400, 0x40);
        }

        current.angle.y = shape_angle.y;

        if (dist_to_player > l_HIO.mAttackRange) {
            cLib_chaseF(&speedF, 15.0f, 1.0f);
        } else if (dist_to_player < l_HIO.mAttackRange - 200.0f) {
            if (mDemoModeTimer == 0) {
                cLib_chaseF(&speedF, 0.0f, 1.0f);
            } else {
                cLib_chaseF(&speedF, -10.0f, 1.0f);
            }
        } else {
            cLib_chaseF(&speedF, 0.0f, 1.0f);
        }

        if (speedF == 0.0f && mDemoModeTimer == 0 && dist_to_player <= l_HIO.mAttackRange &&
            abs((s16)(shape_angle.y - angleY_to_player)) < 0x2000)
        {
            setActionMode(ACTION_TRANS_ATTACK_e, 0);
            return;
        }

        if (speedF) {
            if (!checkBck(ANM_TRANS_WALK_e)) {
                setBck(ANM_TRANS_WALK_e, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            }
        } else {
            setBck(ANM_TRANS_WAIT_e, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
        }
    }

    if (!daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
        Z2GetAudioMgr()->changeSubBgmStatus(1);
        setActionMode(ACTION_OPACI_FADE_AWAY_e, 0);
    }
}

void daE_VA_c::executeTransAttack() {
    f32 anm_frame = mpMorf->getFrame();

    switch (mMode) {
    case 0:
        setBck(ANM_TRANS_ATTACK_A1_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK_T, 0, -1);
        mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK1, -1);
        mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK1, 0, -1);
        mMode = 2;
        break;
    case 2:
        if (mpMorf->checkFrame(22.0f)) {
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK1, 0, -1);
        }

        if (mpMorf->checkFrame(26.0f)) {
            setWeponLandEffect();
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_IMPACT, 0, -1);
            dComIfGp_getVibration().StartShock(5, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (20.0f <= anm_frame && anm_frame <= 33.0f) {
            onSwordAtBit();
        } else if (anm_frame > 33.0f) {
            offSwordShield();
        }

        if (mpMorf->checkFrame(20.0f)) {
            mGlowBody = 1;
            mBodyCyls[0].OnTgSetBit();
            mBodyCyls[1].OnTgSetBit();
            mBodyCyls[0].SetTgType(0xD8FBFDFF);
            mBodyCyls[1].SetTgType(0xD8FBFDFF);
        }

        if (mpMorf->isStop()) {
            setBck(ANM_TRANS_ATTACK_A3_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK3, -1);
            mMode = 4;
        }
        break;
    case 4:
        if (mpMorf->checkFrame(20.0f)) {
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK2, 0, -1);
        }

        if (mpMorf->checkFrame(53.0f)) {
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK3, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK3, -1);
        }

        if (anm_frame <= 20.0f) {
            offSwordShield();
        }

        if (mpMorf->checkFrame(40.0f)) {
            mGlowBody = 0;
            mBodyCyls[0].OffTgSetBit();
            mBodyCyls[1].OffTgSetBit();
        }

        if (mpMorf->isStop()) {
            setActionMode(ACTION_TRANS_CHASE_e, 0);
            mDemoModeTimer = 90;
        }
        break;
    }

    if (!daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
        Z2GetAudioMgr()->changeSubBgmStatus(1);

        if (!checkBck(ANM_TRANS_ATTACK_A3_e)) {
            setBck(ANM_TRANS_ATTACK_A3_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK3, -1);
        }

        setActionMode(ACTION_OPACI_FADE_AWAY_e, 10);
        mBodyCyls[0].OffTgSetBit();
        mBodyCyls[1].OffTgSetBit();
    }
}

void daE_VA_c::executeTransBiteDamage() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp38;

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_NECK_02));
    mDoMtx_stack_c::multVecZero(&sp38);

    switch (mMode) {
    case 0:
        speedF = 0.0f;
        setBck(ANM_TRANS_NECK_DMG_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mSound.startCreatureVoice(Z2SE_EN_VA_V_BITED, -1);
        mMode = 1;
        field_0x1324 = 0;
        field_0x135c = 0;
        mGlowBody = 1;

        mBodyCyls[0].OffTgSetBit();
        mBodyCyls[1].OffTgSetBit();

        if ((s16)(cLib_targetAngleY(&sp38, &player->current.pos) - shape_angle.y) < 0) {
            field_0x1390 = 0;
        } else {
            field_0x1390 = 1;
        }
        /* fallthrough */
    case 1:
        s16 var_r4;
        if (field_0x1390 != 0) {
            var_r4 = shape_angle.y - 0x4000;
        } else {
            var_r4 = shape_angle.y + 0x4000;
        }

        player->setWolfEnemyHangBiteAngle(var_r4);

        if (checkWolfBiteDamage()) {
            offWolfBiteDamage();
            field_0x135c++;
            field_0x1324++;

            if (field_0x135c >= l_HIO.mBiteCount) {
                field_0x135c = 0;
                player->offWolfEnemyHangBite();
                setActionMode(ACTION_OPACI_WAIT_e, 0);
                mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x20);
                attention_info.flags = 0;
                return;
            }

            setBck(ANM_TRANS_NECK_DMG_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_BITED, -1);
            mSound.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x1E);
        }

        if (checkBck(ANM_TRANS_NECK_DMG_e)) {
            if (mpMorf->isStop()) {
                setBck(ANM_TRANS_ATTACK_A2_e, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            }
        } else if (checkBck(ANM_TRANS_ATTACK_A2_e) && mpMorf->checkFrame(22.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK2, -1);
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK2, 0, -1);
        }

        if (!daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
            player->offWolfEnemyHangBite();
            Z2GetAudioMgr()->changeSubBgmStatus(1);
            setActionMode(ACTION_OPACI_FADE_AWAY_e, 0);
            return;
        }

        if (!player->checkWolfEnemyHangBiteOwn(this)) {
            setBck(ANM_TRANS_NECK_PAT_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mMode = 3;
            mGlowBody = 0;
        }
        break;
    case 3:
        if (mpMorf->isStop()) {
            setActionMode(ACTION_TRANS_CHASE_e, 0);
            return;
        }
    }

    setDownPos(&sp38);
    if (!daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
        Z2GetAudioMgr()->changeSubBgmStatus(1);
        setActionMode(ACTION_OPACI_FADE_AWAY_e, 0);
    }
}

void daE_VA_c::executeTransDamage() {
    if (checkWolfBiteDamage()) {
        offWolfBiteDamage();
        daPy_getPlayerActorClass()->offWolfEnemyHangBite();
    }

    switch (mMode) {
    case 0:
        field_0x138f++;
        if (field_0x138f >= 5) {
            setActionMode(ACTION_OPACI_WAIT_e, 0);
            attention_info.flags = 0;
            mBodyCyls[0].OffTgSetBit();
            mBodyCyls[1].OffTgSetBit();
            return;
        }

        speedF = 0.0f;
        setBck(ANM_TRANS_NECK_DMG_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mSound.startCreatureVoice(Z2SE_EN_VA_V_BITED, -1);

        mMode = 1;
        mGlowBody = 1;
        mBodyCyls[0].OnTgSetBit();
        mBodyCyls[1].OnTgSetBit();
        mBodyCyls[0].SetTgType(0xD8FBFDFF);
        mBodyCyls[1].SetTgType(0xD8FBFDFF);
        /* fallthrough */
    case 1:
        if (mpMorf->isStop()) {
            setBck(ANM_TRANS_NECK_PAT_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mMode = 2;
        }
        break;
    case 2:
        if (mpMorf->checkFrame(30.0f)) {
            mGlowBody = 0;
            mBodyCyls[0].OffTgSetBit();
            mBodyCyls[1].OffTgSetBit();
        }

        if (mpMorf->isStop()) {
            setActionMode(ACTION_TRANS_CHASE_e, 0);
        }
    }
}

void daE_VA_c::executeTransThrough() {
    switch (mMode) {
    case 0:
        speedF = 0.0f;
        setBck(ANM_TRANS_THROUGH_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mMode = 1;
        break;
    case 1:
        if (mpMorf->isStop()) {
            setActionMode(ACTION_TRANS_CHASE_e, 0);
        }
        break;
    }

    if (!daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
        Z2GetAudioMgr()->changeSubBgmStatus(1);
        setActionMode(ACTION_OPACI_FADE_AWAY_e, 0);
    }
}

void daE_VA_c::executeOpaciWait() {
    dCamera_c* camera = dCam_getBody();
    cXyz sp24;
    cXyz sp30;

    switch (mMode) {
    case 0:
        if (field_0x1486 == 0) {
            field_0x1486 = 1;
            mMode = 10;

            if (!dComIfGs_isOneZoneSwitch(8, fopAcM_GetRoomNo(this))) {
                dComIfGs_onOneZoneSwitch(8, fopAcM_GetRoomNo(this));
            }
        } else {
            mAlphaType = 2;
            field_0x1380 = 30;
            mGlowLightB = 0;
            mGlowLightG = 0;
            mGlowLightR = 0;
            mKankyoColBlend = 1.0f;
            mKankyoColType = 1;
            field_0x1354 = 10;
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            mFadeAwayTimer = l_HIO.mFadeAwayTime;

            speedF = 0.0f;
            setActionMode(ACTION_OPACI_FLY_e, 22);
        }

        Z2GetAudioMgr()->changeSubBgmStatus(3);
        break;
    case 10:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
            eventInfo.onCondition(2);
        } else {
            mAlphaType = 2;
            field_0x1380 = 30;
            mGlowLightB = 0;
            mGlowLightG = 0;
            mGlowLightR = 0;
            mKankyoColBlend = 1.0f;
            mKankyoColType = 1;

            setBck(ANM_TRANS_STRUGGLE_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_VISUAL, -1);
            field_0x1354 = 10;
            speedF = 0.0f;
            mMode = 12;

            sp24.set(200.0f, 0.0f, 100.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp24, 0x8000, 0);
            daPy_getPlayerActorClass()->changeOriginalDemo();
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 0, 0);

            camera->Stop();
            camera->SetTrimSize(3);
            field_0x14ac = -0x1000;
            mDemoCamCenter.set(15.0f, 291.0f, 201.0f);
            sp30.set(0.0f, 50.0f, 700.0f);
            cLib_offsetPos(&mDemoCamEye, &current.pos, field_0x14ac, &sp30);
            mDemoCamBank = 70.0f;
            field_0x14a4 = 0.0f;
        }
        break;
    case 12:
        current.pos.set(0.0f, 0.0f, 0.0f);
        eyePos.set(15.0f, 291.0f, 201.0f);
        current.angle.y = 0;
        shape_angle.y = 0;
        mMode = 13;
        /* fallthrough */
    case 13:
        field_0x14a4++;
        mDemoCamCenter = eyePos;

        sp30.set(0.0f, field_0x14a4 + 50.0f, 700.0f);
        cLib_offsetPos(&mDemoCamEye, &current.pos, field_0x14ac, &sp30);
        field_0x14ac += 80;

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamBank, 0);
        cLib_addCalcAngleS(&shape_angle.y, -0x1000, 8, 0x100, 0x100);

        if (mpMorf->checkFrame(90.0f)) {
            mMode = 14;
            dComIfGp_getVibration().StartQuake(2, 31, cXyz(0.0f, 1.0f, 0.0f));
            field_0x14b0 = 180.0f;
        }
        break;
    case 14:
        cLib_addCalc2(&field_0x14b0, 0.0f, 1.0f, 6.0f);
        mDemoCamCenter = eyePos;
        cLib_chaseF(&mDemoCamBank, 30.0f, 5.0f);

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamBank, 0);

        if (mpMorf->checkFrame(120.0f)) {
            dComIfGp_getVibration().StopQuake(31);
        }

        if (mpMorf->isStop()) {
            camera->Reset(mDemoCamCenter, mDemoCamEye);
            camera->Start();
            camera->SetTrimSize(0);
            dComIfGp_event_reset();

            mFadeAwayTimer = l_HIO.mFadeAwayTime;
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            setBck(ANM_SUBS_WAIT_e, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 15;
            mDemoModeTimer = 30;
        }
        break;
    case 15:
        if (mDemoModeTimer == 0) {
            setActionMode(ACTION_OPACI_FLY_e, 0);
        }
        break;
    }
}

void daE_VA_c::executeOpaciFly() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 var_r31 = cM_atan2s(current.pos.x, current.pos.z);

    mBodyCyls[0].OnTgSetBit();
    mBodyCyls[1].OnTgSetBit();
    mBodyCyls[0].SetTgType(0x16060);
    mBodyCyls[1].SetTgType(0x16060);

    switch (mMode) {
    case 0:
    case 20:
    case 21:
    case 22:
        attention_info.flags = 0;

        if (mMode == 0) {
            setBck(ANM_FLOAT_WAIT_e, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        } else if (mMode == 20) {
            setBck(ANM_SUBS_DOWN_D_STAND_e, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_DMG_L, -1);
        } else if (mMode == 21) {
            setBck(ANM_SUBS_DOWN_STAND_e, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
        } else if (mMode == 22) {
            setBck(ANM_TRANS_NECK_DMG_e, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_BITED, -1);
        }

        mMode = 1;
        gravity = 0.0f;

        s16 temp_r0 = shape_angle.y + 0x8000;
        f32 temp_f29 = cM_scos(temp_r0) * 1000.0f;
        f32 temp_f1 = cM_ssin(temp_r0) * 1000.0f;
        field_0x12f8.set(temp_f1, 500.0f, temp_f29);
        current.angle.y = temp_r0;

        mDemoModeTimer = nREG_S(0) + 15;
        /* fallthrough */
    case 1:
        if (mDemoModeTimer == 0 && !checkBck(ANM_FLOAT_WAIT_e)) {
            setBck(ANM_FLOAT_WAIT_e, J3DFrameCtrl::EMode_LOOP, nREG_F(19) + 30.0f, 1.0f);
        }

        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x800, 0x80);

        s16 temp_r3 = cLib_targetAngleX(&current.pos, &field_0x12f8);
        f32 temp_f31 = std::abs(cM_scos(temp_r3) * 20.0f);
        f32 temp_f30 = cM_ssin(temp_r3) * 20.0f;

        if (current.pos.y >= 500.0f) {
            cLib_chaseF(&speed.y, 0.0f, 3.0f);
            cLib_chaseF(&current.pos.y, 500.0f, 1.0f);
        } else {
            cLib_chaseF(&speed.y, temp_f30, 1.0f);
        }

        if (current.pos.absXZ() > 950.0f) {
            field_0x12f8.set(current.pos.x, 500.0f, current.pos.z);
            cLib_chaseF(&speedF, 0.0f, 3.0f);
        } else {
            cLib_chaseF(&speedF, temp_f31, 1.0f);
        }

        if (speed.y == 0.0f && speedF == 0.0f) {
            mMode = 2;
        }
        break;
    case 2:
        mMode = 3;
        mDemoModeTimer = 30;
        field_0x1324 = 0;
        mDownTimer = cM_rndF(60.0f) + 150.0f;
        field_0x1348 = cM_rndF(30.0f) + 90.0f;
        /* fallthrough */
    case 3:
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x800, 0x80);

        if (field_0x1324 != 0) {
            current.angle.y = var_r31 + 0x4000;
        } else {
            current.angle.y = var_r31 - 0x4000;
        }

        cLib_chaseF(&speedF, 30.0f, 1.0f);

        if (mDemoModeTimer == 0) {
            mMode = 4;
        }
        break;
    case 4:
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x800, 0x80);

        if (field_0x1324 != 0) {
            current.angle.y = var_r31 + 0x4000;
        } else {
            current.angle.y = var_r31 - 0x4000;
        }

        cLib_chaseF(&speedF, 0.0f, 1.0f);

        if (!speedF) {
            if (mFadeAwayTimer == 0) {
                Z2GetAudioMgr()->changeSubBgmStatus(1);
                setActionMode(ACTION_OPACI_FADE_AWAY_e, 1);
            } else if (mDownTimer == 0 && current.pos.absXZ(player->current.pos) > 500.0f) {
                mMode = 10;
            } else {
                mDemoModeTimer = 30;
                mMode = 3;

                if ((s16)(var_r31 - fopAcM_searchPlayerAngleY(this)) < 0) {
                    field_0x1324 = 0;
                } else {
                    field_0x1324 = 1;
                }
            }
        }
        break;
    case 10:
        mDemoModeTimer = 30;
        mMode = 11;
        /* fallthrough */
    case 11:
        if (mDemoModeTimer == 0) {
            setBck(ANM_FLOAT_ATTACK_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mMode = 12;
        }
        break;
    case 12:
        if (mpMorf->checkFrame(42.0f)) {
            mSound.startCreatureSound(Z2SE_EN_VA_ATK_FLY, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK_FLY, -1);
        }

        if (mpMorf->checkFrame(62.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK_FLY2, -1);
            mSound.startCreatureSound(Z2SE_EN_VA_ATK_FLY2, 0, -1);
        }

        if (mpMorf->checkFrame(47.0f)) {
            field_0x1228[0] = 1;
        }

        if (mpMorf->checkFrame(67.0f)) {
            field_0x1228[1] = 1;
        }

        if (mpMorf->isStop()) {
            setBck(ANM_FLOAT_WAIT_e, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 2;
            field_0x1348 = cM_rndF(30.0f) + 90.0f;
        }
    }
}

void daE_VA_c::executeOpaciDamage() {
    switch (mMode) {
    case 0:
        attention_info.flags = 0;
        mBodyCyls[0].SetTgType(0xD8FBFDFF);
        mBodyCyls[1].SetTgType(0xD8FBFDFF);

        setBck(ANM_FLOAT_DMG_A_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mSound.startCreatureVoice(Z2SE_EN_VA_V_DOWN, -1);
        mSound.startCreatureVoice(Z2SE_EN_VA_V_DMG, -1);
        mSound.startCreatureSound(Z2SE_EN_VA_DOWN, 0, -1);

        mMode = 1;
        speedF = 0.0f;
        /* fallthrough */
    case 1:
        if (mpMorf->isStop()) {
            gravity = -5.0f;
            maxFallSpeed = -50.0f;
            mMode = 2;
            setBck(ANM_FLOAT_DMG_B_e, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
        }
        break;
    case 2:
        if (mAcch.ChkGroundHit() || speed.y <= nREG_F(4) + -30.0f) {
            setActionMode(ACTION_OPACI_CHASE_e, 0);
        }
        break;
    case 10:
        attention_info.flags = 0;
        setBck(ANM_SUBS_STAGGER_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mSound.startCreatureVoice(Z2SE_EN_VA_V_NO_DMG, -1);
        mMode = 11;
        /* fallthrough */
    case 11:
        if (mpMorf->isStop()) {
            setActionMode(ACTION_OPACI_FLY_e, 0);
        }
        break;
    }
}

void daE_VA_c::executeOpaciChase() {
    cXyz sp44;
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 angleY_to_player = fopAcM_searchPlayerAngleY(this);
    s16 angle_to_home = cLib_targetAngleY(&home.pos, &current.pos);

    switch (mMode) {
    case 0:
        attention_info.flags = 0;
        field_0x1330 = angle_to_home;
        field_0x1332 = field_0x1330;
        field_0x1334 = (f32)field_0x1332 + cM_rndF(3.0f) * 16384.0f;

        if (cM_rnd() < 0.5f) {
            field_0x1336 = -0x400;
        } else {
            field_0x1336 = 0x400;
        }

        field_0x1330 += field_0x1336;
        mMode = 10;
        mDownTimer = 30;

        setBck(ANM_SUBS_FLY_e, J3DFrameCtrl::EMode_LOOP, 30.0f, 1.0f);
        gravity = 0.0f;
        field_0x1324 = 0;

        sp44.set(0.0f, 0.0f, l_HIO.mCircleAttackRadius);
        cLib_offsetPos(&field_0x12f8, &home.pos, field_0x1330, &sp44);
        current.angle.y = cLib_targetAngleY(&current.pos, &field_0x12f8);
        /* fallthrough */
    case 10:
    case 11:
        field_0x1388 = 1;
        mSound.startCreatureVoiceLevel(Z2SE_EN_VA_V_ROTATE, -1);

        if (mDownTimer == 0 && !player->checkNowWolf()) {
            mBodyCyls[0].OnTgSetBit();
            mBodyCyls[1].OnTgSetBit();
        }

        switch (field_0x1324) {
        case 0:
            if (cLib_chaseF(&speed.y, nREG_F(0) + 25.0f, nREG_F(1) + 3.0f)) {
                field_0x1324 = 1;
            }
            break;
        case 1:
            if (cLib_chaseF(&speed.y, nREG_F(3) + -40.0f, nREG_F(2) + 3.0f)) {
                field_0x1324 = 0;
            }

            if (mAcch.ChkGroundHit()) {
                field_0x1324 = 2;
                gravity = -5.0f;
            }
            break;
        case 2:
            break;
        }

        cLib_chaseF(&speedF, 100.0f, 3.0f);

        sp44.set(0.0f, 0.0f, l_HIO.mCircleAttackRadius);
        cLib_offsetPos(&field_0x12f8, &home.pos, field_0x1330, &sp44);

        if (current.pos.absXZ() > nREG_F(19) + 1200.0f) {
            s16 temp_r26 = cM_atan2s(current.pos.x, current.pos.z);

            current.pos.x = (nREG_F(19) + 1200.0f) * cM_ssin(temp_r26);
            current.pos.z = (nREG_F(19) + 1200.0f) * cM_scos(temp_r26);
        }

        cLib_addCalcAngleS(&shape_angle.y, angleY_to_player, 8, 0x800, 0x80);
        cLib_addCalcAngleS(&current.angle.y, (s16)cLib_targetAngleY(&current.pos, &field_0x12f8), 8,
                           0x800, 0x80);

        if (abs((s16)(field_0x1330 - angle_to_home)) < 0x2000) {
            s16 var_r29 = field_0x1330;
            field_0x1330 += field_0x1336;

            if (mMode == 10) {
                if (abs((s16)(field_0x1332 - field_0x1330)) <= abs(field_0x1336)) {
                    if (abs((s16)(field_0x1332 - var_r29)) <= abs(field_0x1336)) {
                        if (field_0x1332 == field_0x1334) {
                            mMode = 12;
                        } else {
                            mMode = 11;
                        }
                    }
                }
            } else {
                if (abs((s16)(field_0x1334 - field_0x1330)) <= abs(field_0x1336)) {
                    mMode = 12;
                    attention_info.flags = fopAc_AttnFlag_BATTLE_e;
                }
            }
        }
        break;
    case 12:
        mSound.startCreatureVoiceLevel(Z2SE_EN_VA_V_ROTATE, -1);

        if (!player->checkNowWolf()) {
            mBodyCyls[0].OnTgSetBit();
            mBodyCyls[1].OnTgSetBit();
        }

        field_0x1388 = 1;
        cLib_addCalcAngleS(&shape_angle.y, angleY_to_player, 8, 0x800, 0x80);
        cLib_addCalcAngleS(&current.angle.y, angleY_to_player, 8, 0x800, 0x80);

        if (fopAcM_searchPlayerDistance(this) > l_HIO.mAttackRange) {
            cLib_chaseF(&speedF, 100.0f, 3.0f);
        } else if (fopAcM_searchPlayerDistance(this) < l_HIO.mAttackRange - 300.0f) {
            cLib_chaseF(&speedF, -30.0f, 3.0f);
        } else {
            mMode = 13;
        }
        break;
    case 13:
        mSound.startCreatureVoiceLevel(Z2SE_EN_VA_V_ROTATE, -1);

        if (!player->checkNowWolf()) {
            mBodyCyls[0].OnTgSetBit();
            mBodyCyls[1].OnTgSetBit();
        }

        cLib_addCalcAngleS(&shape_angle.y, angleY_to_player, 8, 0x800, 0x80);
        cLib_addCalcAngleS(&current.angle.y, angleY_to_player, 8, 0x800, 0x80);

        if (cLib_chaseF(&speedF, 0.0f, 3.0f)) {
            setActionMode(ACTION_OPACI_ATTACK_e, 0);
        }
        break;
    }
}

void daE_VA_c::executeOpaciAttack() {
    f32 anm_frame = mpMorf->getFrame();

    mBodyCyls[0].OnTgSetBit();
    mBodyCyls[1].OnTgSetBit();

    switch (mMode) {
    case 0:
        speedF = 0.0f;
        setBck(ANM_SUBS_ATTACK_A1_e, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
        mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK_T, 0, -1);
        mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK1, -1);
        mMode = 1;
        /* fallthrough */
    case 1:
        if (mpMorf->checkFrame(22.0f)) {
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK1, 0, -1);
        }

        if (mpMorf->checkFrame(27.0f)) {
            setWeponLandEffect();
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_IMPACT, 0, -1);
            dComIfGp_getVibration().StartShock(5, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (anm_frame < 16.0f) {
            cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x800, 0x80);
            current.angle.y = shape_angle.y;
        }

        if (20.0f <= anm_frame && anm_frame <= 33.0f) {
            onSwordAtBit();
        } else if (anm_frame > 33.0f) {
            offSwordShield();
            field_0x1386 = 1;
        }

        if (mpMorf->isStop()) {
            setBck(ANM_SUBS_ATTACK_A3_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK3, -1);
            mMode = 2;
        }
        break;
    case 2:
        if (mpMorf->checkFrame(12.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK3, -1);
            mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK3, 0, -1);
        }

        if (mpMorf->getFrame() <= 15.0f) {
            offSwordShield();
            field_0x1386 = 1;
        }

        if (mpMorf->isStop()) {
            if (mFadeAwayTimer == 0) {
                Z2GetAudioMgr()->changeSubBgmStatus(1);
                setActionMode(ACTION_OPACI_FADE_AWAY_e, 1);
                return;
            }

            setActionMode(ACTION_OPACI_FLY_e, 0);
        }
        break;
    }
}

void daE_VA_c::executeOpaciDown() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    mBodyCyls[1].OnTgSetBit();

    switch (mMode) {
    case 0:
        setBck(ANM_SUBS_DOWN_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mDownTimer = l_HIO.mDownTime;
        mMode = 2;
        speedF = 0.0f;
        field_0x1383 = 0;
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        mSound.startCreatureVoice(Z2SE_EN_VA_V_DMG_L, -1);
        dComIfGs_onOneZoneSwitch(6, fopAcM_GetRoomNo(this));
        /* fallthrough */
    case 2:
        if (mpMorf->getFrame() < 40.0f) {
            mBodyCyls[1].OffTgSetBit();
        }

        if (mpMorf->isStop()) {
            setBck(ANM_SUBS_DOWN_WAIT_e, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 3;
        }
        break;
    case 1:
        setBck(ANM_SUBS_DOWN_WAIT_e, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        mMode = 3;
        /* fallthrough */
    case 3:
        if (mpMorf->checkFrame(1.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_VA_V_DYING, -1);
        }

        if (mFadeAwayTimer == 0) {
            Z2GetAudioMgr()->changeSubBgmStatus(1);
            setActionMode(ACTION_OPACI_FADE_AWAY_e, 2);
        } else if (mDownTimer == 0) {
            setActionMode(ACTION_OPACI_FLY_e, 21);
        }
        break;
    case 5:
        field_0x1364 += field_0x1368;

        if ((f32)field_0x1364 > l_HIO.mDownHP || field_0x1384 >= 6) {
            setActionMode(ACTION_OPACI_DEATH_e, 0);
            field_0x1381 = 1;
            return;
        }

        setActionMode(ACTION_OPACI_FLY_e, 20);
        break;
    case 4:
        mBodyCyls[1].OffTgSetBit();

        if (mpMorf->isStop()) {
            setActionMode(ACTION_OPACI_FLY_e, 0);
            return;
        }
        break;
    }

    if (player->getCutCount() > 1 || player->getCutType() == 10) {
        if (mFadeAwayTimer < 30) {
            mFadeAwayTimer = 30;
        }

        if (mDownTimer < 30) {
            mDownTimer = 30;
        }
    }
}

void daE_VA_c::executeOpaciDownDamage() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (player->getCutCount() > 1 || player->getCutType() == 10) {
        if (mFadeAwayTimer < 30) {
            mFadeAwayTimer = 30;
        }

        if (mDownTimer < 30) {
            mDownTimer = 30;
        }
    }

    if (mFadeAwayTimer != 0 && mDownTimer != 0) {
        mBodyCyls[1].OnTgSetBit();
    }

    switch (mMode) {
    case 0:
    case 1:
    case 2:
        speedF = 0.0f;
        field_0x1364 += field_0x1368;

        if ((f32)field_0x1364 > l_HIO.mDownHP) {
            setActionMode(ACTION_OPACI_DEATH_e, 0);
            field_0x1381 = 1;
            return;
        }

        if (mMode == 0) {
            setBck(ANM_SUBS_DOWN_DMG_A1_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_DMG, -1);
        } else if (mMode == 1) {
            setBck(ANM_SUBS_DOWN_DMG_A2_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_DMG, -1);
        } else {
            setBck(ANM_SUBS_DOWN_STAGGER_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_NO_DMG, -1);
        }

        mMode = 5;
    case 5:
        if (mpMorf->isStop()) {
            setActionMode(ACTION_OPACI_DOWN_e, 1);
            field_0x1381 = 1;
        }
        break;
    }
}

void daE_VA_c::executeOpaciFlip() {
    switch (mMode) {
    case 0:
        speedF = 0.0f;
        mMode = 1;
        daPy_getPlayerActorClass()->setThrowDamage(shape_angle.y, 20.0f, 40.0f, 1, 0, 0);
        /* fallthrough */
    case 1:
        if (mpMorf->isStop()) {
            setActionMode(ACTION_OPACI_CHASE_e, 0);
        }
    }
}

void daE_VA_c::executeOpaciFadeAway() {
    attention_info.flags = 0;
    mBodyCylIFrameTimer = 3;
    mNeckSphIFrameTimer = 3;
    mAttackSphIFrameTimer = 3;

    switch (mMode) {
    case 10:
        if (checkBck(ANM_TRANS_ATTACK_A3_e)) {
            if (mpMorf->checkFrame(20.0f)) {
                mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK2, 0, -1);
            }

            if (mpMorf->checkFrame(53.0f)) {
                mSound.startCreatureSound(Z2SE_EN_VA_SWD_ATK3, 0, -1);
                mSound.startCreatureVoice(Z2SE_EN_VA_V_ATK3, -1);
            }
        }

        mAlphaType = 0;
        speedF = 0.0f;

        if (mpMorf->isStop()) {
            setActionMode(ACTION_CLEAR_WAIT_e, 0);
        }
        break;
    case 0:
        mAlphaType = 0;
        speedF = 0.0f;
        setActionMode(ACTION_CLEAR_WAIT_e, 0);
        break;
    case 1:
        if (checkBck(ANM_SUBS_INVITE_B_e)) {
            setBck(ANM_SUBS_INVITE_C_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mMode = 5;
        } else {
            setBck(ANM_SUBS_TO_TRANS_e, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_VANISH, -1);
            mSound.startCreatureSound(Z2SE_EN_VA_VANISH, 0, -1);
            mAlphaType = 3;
            mMode = 4;
            mKankyoColType = 2;
        }

        speed.y = 0.0f;
        speedF = 0.0f;
        break;
    case 2:
        setBck(ANM_SUBS_DOWN_STAND_e, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
        mMode = 3;
        speedF = 0.0f;
        break;
    case 3:
        if (mpMorf->isStop()) {
            setBck(ANM_SUBS_TO_TRANS_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_VA_V_VANISH, -1);
            mSound.startCreatureSound(Z2SE_EN_VA_VANISH, 0, -1);
            mMode = 4;
            mAlphaType = 3;
            mKankyoColType = 2;
        }
        break;
    case 4:
        cLib_addCalc(&current.pos.y, 0.0f, 0.1f, 10.0f, 1.0f);
        if (mpMorf->isStop()) {
            setActionMode(ACTION_CLEAR_WAIT_e, 1);
            current.pos.y = 0.0f;
        }
        break;
    case 5:
        if (mpMorf->isStop()) {
            mMode = 1;
        }
        break;
    }

    if (daPy_getPlayerActorClass()->checkNowWolfEyeUp() && field_0x1358 == 0) {
        setActionMode(ACTION_TRANS_WAIT_e, 0);
    }
}

void daE_VA_c::executeOpaciDeath() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp24;
    cXyz sp30;

    mBodyCyls[0].OffTgSetBit();
    mBodyCyls[1].OffTgSetBit();

    switch (mMode) {
    case 0:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
            eventInfo.onCondition(2);
            return;
        }

        Z2GetAudioMgr()->subBgmStop();
        Z2GetAudioMgr()->muteSceneBgm(0, 0.0f);

        dComIfGs_onStageMiddleBoss();
        field_0x1364 = 0;

        setBck(ANM_SUBS_DOWN_DIE_e, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mSound.startCreatureVoice(Z2SE_EN_VA_V_DEAD, -1);
        mMode = 10;

        player->changeOriginalDemo();

        sp30.set(0.0f, 0.0f, 350.0f);
        player->setPlayerPosAndAngle(&sp30, 0x8000, 0);
        player->changeDemoMode(4, 1, 0, 0);

        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);

        mDemoCamCenter.set(-54.0f, 168.0f, 207.0f);
        field_0x14ac = -0x1800;
        field_0x14a4 = 600.0f;

        sp24.set(0.0f, 0.0f, field_0x14a4);
        cLib_offsetPos(&mDemoCamEye, &current.pos, shape_angle.y + field_0x14ac, &sp24);
        mDemoCamEye.y = 50.0f;
        mDemoCamBank = 70.0f;
        return;
    case 10:
        current.pos.set(0.0f, 0.0f, 0.0f);
        eyePos.set(-54.0f, 168.0f, 207.0f);
        shape_angle.y = 0;
        current.angle.y = 0;
        mMode = 1;
    case 1:
        mDemoCamCenter = eyePos;
        mDemoCamCenter.y -= 50.0f;
        field_0x14ac += 0x40;

        sp24.set(0.0f, 0.0f, field_0x14a4);
        cLib_offsetPos(&mDemoCamEye, &current.pos, shape_angle.y + field_0x14ac, &sp24);
        mDemoCamEye.y = 50.0f;

        if (mpMorf->checkFrame(155.0f)) {
            mMode = 2;
            mDemoModeTimer = 60;

            sp30.set(0.0f, 0.0f, 350.0f);
            player->setPlayerPosAndAngle(&sp30, 0x8000, 0);

            field_0x14a4 = 400.0f;
            sp24.set(0.0f, 0.0f, field_0x14a4);
            cLib_offsetPos(&mDemoCamEye, &current.pos, shape_angle.y + field_0x14ac, &sp24);
            mDemoCamEye.y = 500.0f;
        }
        break;
    case 2:
        mDemoCamCenter = eyePos;
        mDemoCamCenter.y -= 50.0f;

        if (mpMorf->isStop()) {
            mDoMtx_stack_c::copy(mpWeaponModel->getBaseTRMtx());
            mDoMtx_stack_c::transM(-10.0f, 160.0f, 15.0f);
            mDoMtx_stack_c::multVecZero(&sp30);

            fopAcM_createDisappear(this, &sp30, 20, 0, 0xFF);
            mDemoModeTimer = 120;
            mMode = 3;
            mPlayEndEf = true;

            mpEndEfMorf->setFrame(0.0f);
            mpEndEfBrk->setFrame(0.0f);
            mpEndEfBrk->setPlaySpeed(1.0f);
            mDownTimer = 100;

            setBugsEffect();
            mSound.startCreatureSound(Z2SE_EN_VA_END, 0, -1);
            mKankyoColBlend = 0.0f;
            mKankyoColType = 3;
        }
        break;
    case 3:
        mDemoCamCenter = eyePos;
        mDemoCamCenter.y -= 50.0f;

        if (mDownTimer == 0) {
            field_0x1388 = 1;
        }

        if (mDownTimer == 80) {
            field_0x1240 = 2;
            mWeponEfMode = 0;
        }

        if (mDemoModeTimer == 0) {
            field_0x122c = mDemoCamCenter;
            mMode = 4;
            mDemoModeTimer = 150;
        }
        break;
    case 4:
        if (mDownTimer == 0) {
            field_0x1388 = 1;
        }

        mDemoCamCenter = field_0x122c;
        field_0x122c.y += 10.0f;

        if (mDemoModeTimer == 0) {
            camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye, mDemoCamBank, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();

            if (mSwNo != 0xFF && !dComIfGs_isSwitch(mSwNo, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(mSwNo, fopAcM_GetRoomNo(this));
            }

            Z2GetAudioMgr()->unMuteSceneBgm(45);
            fopAcM_delete(this);
            return;
        }
        break;
    }

    camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamBank, 0);
}

void daE_VA_c::calcMagicMove() {
    for (int i = 0; i < 2; i++) {
        switch (field_0x1228[i]) {
        case 0:
            mMagicSphs[i].OffAtSetBit();
            break;
        case 1:
            mMagicSphs[i].OnAtSetBit();
            field_0x1228[i] = 2;

            mDoMtx_stack_c::copy(mpWeaponModel->getBaseTRMtx());
            mDoMtx_stack_c::transM(-30.0f, 320.0f, 65.0f);
            mDoMtx_stack_c::multVecZero(&mMagicPos[i]);
            mMagicOldPos[i] = mMagicPos[i];

            mMagicAcch[i].ClrGroundHit();
            mMagicAcch[i].ClrWallHit();
            mMagicSphs[i].ClrAtHit();

            cXyz sp5C(daPy_getPlayerActorClass()->current.pos);

            s16 spA = cLib_targetAngleY(&mMagicPos[i], &sp5C);
            s16 sp8 = cLib_targetAngleX(&mMagicPos[i], &sp5C);

            f32 temp_f31 = std::abs(cM_scos(sp8) * 50.0f);
            mMagicSpeed[i].set(temp_f31 * cM_ssin(spA), cM_ssin(sp8) * 50.0f,
                               temp_f31 * cM_scos(spA));
            /* fallthrough */
        case 2:
            Z2GetAudioMgr()->seStartLevel(Z2SE_EN_VA_ATK_BALL, &mMagicPos[i], 0, 0, 1.0f, 1.0f,
                                          -1.0f, -1.0f, 0);
            setMagicEffect(i);
            mMagicSphs[i].SetC(mMagicPos[i]);
            mMagicSphs[i].SetR(50.0f);
            dComIfG_Ccsp()->Set(&mMagicSphs[i]);

            if (mMagicAcch[i].ChkGroundHit() || mMagicAcch[i].ChkWallHit() ||
                mMagicSphs[i].ChkAtHit())
            {
                Z2GetAudioMgr()->seStart(Z2SE_EN_VA_FIRE_BURST, &mMagicPos[i], 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);
                mMagicPos[i].y = 0.0f;
                setMagicHitEffect(i);
                field_0x1228[i] = 0;
            }

            mMagicOldPos[i] = mMagicPos[i];
            mMagicPos[i] += mMagicSpeed[i];
            mMagicAcch[i].CrrPos(dComIfG_Bgsp());
            break;
        }
    }
}

void daE_VA_c::setAlphaType() {
    switch (mAlphaType) {
    case 0:
    case 3:
        cLib_chaseUC(&mGlowLightA, 0, 4);
        cLib_chaseUC(&mEyeAlpha, 0, 4);
        field_0x1380 = 0;

        if (mAlphaType == 3 && mGlowLightA == 0) {
            mAlphaType = 0;
        }

        cLib_chaseUC(&mGlowLightR, 0, 8);
        cLib_chaseUC(&mGlowLightG, 0, 8);
        cLib_chaseUC(&mGlowLightB, 0, 8);

        if (mAlphaType == 0) {
            cLib_chaseUC(&mFootSmokeAlpha, 0, 27);
        } else if (mFootSmokeAlpha < 33) {
            cLib_chaseUC(&mFootSmokeAlpha, 0, 1);
        } else {
            cLib_chaseUC(&mFootSmokeAlpha, 0, 10);
        }

        field_0x138f = 0;

        mNeckSph.OffTgSetBit();
        for (int i = 0; i < 3; i++) {
            mBodyCyls[i].OffCoSetBit();
            mBodyCyls[i].OffTgSetBit();
            mBodyCyls[i].OnTgNoHitMark();
        }
        break;
    case 1:
        if (mGlowBody) {
            cLib_chaseUC(&mGlowLightR, l_HIO.mKRegLightR, (u8)(l_HIO.mKRegLightR / 10.0f));
            cLib_chaseUC(&mGlowLightG, l_HIO.mKRegLightG, (u8)(l_HIO.mKRegLightG / 10.0f));
            cLib_chaseUC(&mGlowLightB, l_HIO.mKRegLightB, (u8)(l_HIO.mKRegLightB / 10.0f));
            cLib_chaseUC(&mGlowLightA, l_HIO.mKRegLightA,
                         (u8)((l_HIO.mKRegLightA - 128.0f) / 10.0f));
        } else {
            cLib_chaseUC(&mGlowLightR, 0, (u8)(l_HIO.mKRegLightR / 20.0f));
            cLib_chaseUC(&mGlowLightG, 0, (u8)(l_HIO.mKRegLightG / 20.0f));
            cLib_chaseUC(&mGlowLightB, 0, (u8)(l_HIO.mKRegLightB / 20.0f));
            cLib_chaseUC(&mGlowLightA, 0x80, 8);
        }

        cLib_chaseUC(&mEyeAlpha, 0, 4);
        field_0x1380 = 0;
        cLib_chaseUC(&mFootSmokeAlpha, 180, 8);

        mNeckSph.OnTgSetBit();
        mBodyCyls[0].OnTgSetBit();
        mBodyCyls[1].OffTgSetBit();
        mBodyCyls[2].OffTgSetBit();

        for (int i = 0; i < 3; i++) {
            mBodyCyls[i].OffCoSetBit();
            mBodyCyls[i].OnTgNoHitMark();
        }
        break;
    case 2:
        cLib_chaseUC(&mGlowLightA, 0xFF, 4);
        cLib_chaseUC(&mEyeAlpha, 0xFF, 8);
        cLib_chaseUC(&field_0x1380, 0, 1);
        cLib_chaseUC(&mFootSmokeAlpha, 0xFF, 4);

        mNeckSph.OffTgSetBit();
        for (int i = 0; i < 3; i++) {
            mBodyCyls[i].OnCoSetBit();
            mBodyCyls[i].OffTgNoHitMark();
            mBodyCyls[i].OffTgSetBit();
        }
        break;
    }
}

void daE_VA_c::action() {
    field_0x1391 = 0;
    setAlphaType();
    damage_check();

    for (int i = 0; i < 4; i++) {
        mAttackSphs[i].OffAtSetBit();

        if (mGlowBody && i == 0) {
            mAttackSphs[i].OffCoSetBit();
            mAttackSphs[i].OffTgShield();
            mAttackSphs[i].OnTgNoHitMark();
        } else {
            mAttackSphs[i].OnCoSetBit();

            if (mOffTgTimer != 0) {
                mAttackSphs[i].OffTgShield();
                mAttackSphs[i].OnTgNoHitMark();
            } else {
                mAttackSphs[i].OnTgShield();
                mAttackSphs[i].OffTgNoHitMark();
            }
        }
    }

    BOOL var_r29 = true;
    field_0x1386 = 0;
    field_0x1388 = 0;

    switch (mAction) {
    case ACTION_DEMO_OP_WAIT_e:
        executeDemoOpWait();
        var_r29 = false;
        break;
    case ACTION_DEMO_OP_e:
        executeDemoOp();
        var_r29 = false;
        break;
    case ACTION_CLEAR_WAIT_e:
        executeClearWait();
        break;
    case ACTION_CLEAR_CHASE_e:
        executeClearChase();
        break;
    case ACTION_CLEAR_ATTACK_e:
        executeClearAttack();
        break;
    case ACTION_TRANS_WAIT_e:
        executeTransWait();
        break;
    case ACTION_TRANS_CHASE_e:
        executeTransChase();
        break;
    case ACTION_TRANS_ATTACK_e:
        executeTransAttack();
        break;
    case ACTION_TRANS_BITE_DAMAGE_e:
        executeTransBiteDamage();
        break;
    case ACTION_TRANS_DAMAGE_e:
        executeTransDamage();
        break;
    case ACTION_TRANS_THROUGH_e:
        executeTransThrough();
        break;
    case ACTION_OPACI_WAIT_e:
        executeOpaciWait();
        break;
    case ACTION_OPACI_FLY_e:
        executeOpaciFly();
        break;
    case ACTION_OPACI_DOWN_e:
        executeOpaciDown();
        break;
    case ACTION_OPACI_DOWN_DAMAGE_e:
        executeOpaciDownDamage();
        break;
    case ACTION_OPACI_CHASE_e:
        executeOpaciChase();
        break;
    case ACTION_OPACI_ATTACK_e:
        executeOpaciAttack();
        break;
    case ACTION_OPACI_DAMAGE_e:
        executeOpaciDamage();
        break;
    case ACTION_OPACI_FLIP_e:
        executeOpaciFlip();
        break;
    case ACTION_OPACI_FADE_AWAY_e:
        executeOpaciFadeAway();
        break;
    case ACTION_OPACI_DEATH_e:
        executeOpaciDeath();
        break;
    }

    if (var_r29) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }

    if (!field_0x138e) {
        if (daPy_getPlayerActorClass()->checkNowWolfEyeUp()) {
            field_0x1358 = 20;
            field_0x138e = 1;
        }
    } else {
        if (!daPy_getPlayerActorClass()->checkNowWolfEyeUp()) {
            field_0x1358 = 20;
            field_0x138e = 0;
        }
    }

    fopAcM_posMoveF(this, mBodyCcStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());

    switch (mKankyoColType) {
    case 0:
        if (daPy_getPlayerActorClass()->checkNowWolfEyeUp()) {
            cLib_chaseF(&mKankyoColBlend, 1.0f, 0.1f);
        } else {
            cLib_chaseF(&mKankyoColBlend, 0.0f, 0.1f);
        }

        dKy_custom_colset(2, 0, mKankyoColBlend);
        break;
    case 1:
        cLib_chaseF(&mKankyoColBlend, 1.0f, 0.1f);
        dKy_custom_colset(0, 1, mKankyoColBlend);
        break;
    case 2:
        cLib_chaseF(&mKankyoColBlend, 0.0f, 0.1f);
        dKy_custom_colset(2, 1, mKankyoColBlend);

        if (!mKankyoColBlend) {
            mKankyoColType = 0;
        }
        break;
    case 3:
        cLib_chaseF(&mKankyoColBlend, 1.0f, 0.01f);
        dKy_custom_colset(1, 4, mKankyoColBlend);
        break;
    case 4:
        cLib_chaseF(&mKankyoColBlend, 1.0f, 0.01f);
        /* fallthrough */
    case 5:
        dKy_custom_colset(3, 2, mKankyoColBlend);

        if (mKankyoColBlend == 1.0f) {
            mKankyoColType = 0;
            mKankyoColBlend = 0.0f;
        }
        break;
    }

    s16 spC = shape_angle.y - current.angle.y;
    s16 target_x = (speedF * 128.0f) * cM_scos(spC);
    s16 target_z = (speedF * 128.0f) * cM_ssin(spC);

    cLib_addCalcAngleS(&field_0x1304.x, target_x, 8, 0x400, 0x40);
    cLib_addCalcAngleS(&field_0x1304.z, target_z, 8, 0x400, 0x40);

    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    if (mPlayEndEf) {
        mpEndEfMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    mpWeaponBrk->play();
    mpEndEfBrk->play();
}

void daE_VA_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::transM(0.0f, 300.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotM(field_0x1304);
    mDoMtx_stack_c::transM(0.0f, -300.0f, 0.0f);
    mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);

    J3DModel* model = mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    if (mPlayEndEf) {
        mpEndEfMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        mpEndEfMorf->modelCalc();
    }

    field_0x138b = 0;
    mpMorf->modelCalc();
    calcJointSleeve();

    field_0x138b = 1;
    mpMorf->modelCalc();

    if (!mRopesEnabled) {
        if (mpWeaponModel != NULL) {
            mpWeaponModel->setBaseTRMtx(model->getAnmMtx(JNT_KEN_2));
        }
    } else {
        if (mpWeaponModel != NULL) {
            switch (field_0x1240) {
            case 0:
                mDoMtx_stack_c::transS(field_0x122c);
                mDoMtx_stack_c::transM(0.0f, 150.0f, 0.0f);
                mDoMtx_stack_c::ZXYrotM(field_0x1238);
                mDoMtx_stack_c::transM(0.0f, -150.0f, 0.0f);
                mDoMtx_stack_c::transM(0.0f, 400.0f, 0.0f);
                mDoMtx_stack_c::XrotM(field_0x123e);
                mDoMtx_stack_c::transM(0.0f, -400.0f, 0.0f);
                mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);
                mpWeaponModel->setBaseTRMtx(mDoMtx_stack_c::get());
                break;
            case 1:
                mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_KEN_2));
                mpWeaponModel->setBaseTRMtx(mDoMtx_stack_c::get());
                mDoMtx_stack_c::multVecZero(&field_0x122c);
                break;
            }
        }

        cXyz sp34;
        cXyz sp40;
        for (int i = 0; i < 40; i++) {
            f32 var_f31;
            switch (mCardFlags[i] & 3) {
            case 0:
                var_f31 = 80.0f;
                break;
            case 1:
                var_f31 = 50.0f;
                break;
            case 2:
                var_f31 = 25.0f;
                break;
            }

            int idx = va_tag_set_num[i];
            sp34 = field_0x994[idx + 1] - field_0x994[idx];
            sp34.normalizeZP();
            sp40 = field_0x994[idx] + (sp34 * va_tag_offset[i]);

            mDoMtx_stack_c::transS(sp40);
            mDoMtx_stack_c::ZXYrotM(field_0x660[i]);
            mDoMtx_stack_c::transM(0.0f, -var_f31, 0.0f);
            mDoMtx_stack_c::XrotM(0x4000);
            mpCardModels[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

void daE_VA_c::cc_set() {
    cXyz sp24;

    if (mGlowLightA != 0) {
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&eyePos);
        attention_info.position = eyePos;
        attention_info.position.y += 50.0f;
    } else {
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_KEN_2));
        mDoMtx_stack_c::transM(-20.0f, 350.0f, 50.0f);
        mDoMtx_stack_c::multVecZero(&eyePos);
        attention_info.position = eyePos;
        attention_info.position.y += 30.0f;
    }

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_NECK_02));
    mDoMtx_stack_c::transM(0.0f, 40.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&sp24);

    mNeckSph.SetC(sp24);
    if (mGlowBody) {
        mNeckSph.SetR(nREG_F(7) + 70.0f);
    } else {
        mNeckSph.SetR(50.0f);
    }

    dComIfG_Ccsp()->Set(&mNeckSph);

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_ROOT));
    mDoMtx_stack_c::transM(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&sp24);
    mBodyCyls[0].SetC(sp24);
    mBodyCyls[0].SetH(200.0f);

    if (mGlowBody) {
        mBodyCyls[0].SetR(75.0f);
    } else {
        mBodyCyls[0].SetR(100.0f);
    }

    dComIfG_Ccsp()->Set(&mBodyCyls[0]);

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_NECK_02));
    mDoMtx_stack_c::multVecZero(&sp24);
    sp24.y -= 50.0f;

    mBodyCyls[1].SetC(sp24);
    mBodyCyls[1].SetH(100.0f);
    mBodyCyls[1].SetR(80.0f);
    dComIfG_Ccsp()->Set(&mBodyCyls[1]);

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_NECK_02));
    mDoMtx_stack_c::multVecZero(&sp24);
    sp24.y -= 200.0f;

    mBodyCyls[2].SetC(sp24);
    mBodyCyls[2].SetH(200.0f);
    mBodyCyls[2].SetR(80.0f);
    dComIfG_Ccsp()->Set(&mBodyCyls[2]);

    mDoMtx_stack_c::copy(mpWeaponModel->getBaseTRMtx());
    mDoMtx_stack_c::transM(10.0f, 0.0f, -25.0f);
    for (int i = 0; i < 4; i++) {
        mDoMtx_stack_c::transM(-10.0f, 80.0f, 20.0f);
        mDoMtx_stack_c::multVecZero(&sp24);
        mAttackSphs[i].SetC(sp24);

        if (mAttackSphs[i].ChkAtSet()) {
            mAttackSphs[i].SetR(70.0f);
        } else {
            mAttackSphs[i].SetR(50.0f);
        }

        dComIfG_Ccsp()->Set(&mAttackSphs[i]);
    }

    if (mRopesEnabled) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 19; j++) {
                int sp14 = i * 19;
                sp14 += j;

                int sp10 = i * 10;
                sp10 += (j >> 1);

                if (!(j & 1)) {
                    mLineSphs[sp14].SetC(field_0x994[sp10]);
                } else {
                    mLineSphs[sp14].SetC((field_0x994[sp10] + field_0x994[sp10 + 1]) / 2.0f);
                }

                mLineSphs[sp14].SetR(20.0f);
                dComIfG_Ccsp()->Set(&mLineSphs[sp14]);
            }
        }
    }
}

int daE_VA_c::execute() {
    if (mDemoModeTimer != 0) {
        mDemoModeTimer--;
    }

    if (mDownTimer != 0) {
        mDownTimer--;
    }

    if (mFadeAwayTimer != 0) {
        mFadeAwayTimer--;
    }

    if (field_0x1350 != 0) {
        field_0x1350--;
    }

    if (mAttackSphIFrameTimer != 0) {
        mAttackSphIFrameTimer--;
    }

    if (mNeckSphIFrameTimer != 0) {
        mNeckSphIFrameTimer--;
    }

    if (mBodyCylIFrameTimer != 0) {
        mBodyCylIFrameTimer--;
    }

    if (mOffTgTimer != 0) {
        mOffTgTimer--;
    }

    if (field_0x1348 != 0) {
        field_0x1348--;
    }

    if (field_0x1354 != 0) {
        field_0x1354--;
    }

    if (field_0x1358 != 0) {
        field_0x1358--;
    }

    action();
    calcJointNeck();
    mtx_set();
    setFootEffect();
    setWeponEffect();
    calcMagicMove();
    cc_set();

    if (mAlphaType < 2) {
        if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
            daPy_getPlayerActorClass()->cancelWolfLock(this);
        }

        onWolfNoLock();
    } else {
        offWolfNoLock();
    }

    cXyz bind_pos;
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&bind_pos);

    cXyz bind_scale(1.0f, 1.0f, 1.0f);
    setMidnaBindEffect(this, &mSound, &bind_pos, &bind_scale);
    return 1;
}

static int daE_VA_Execute(daE_VA_c* i_this) {
    return i_this->execute();
}

static int daE_VA_IsDelete(daE_VA_c* i_this) {
    return 1;
}

int daE_VA_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_VA");

    if (mInitHIO) {
        init_hio = false;
    }

    for (int i = 0; i < 2; i++) {
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mFootSmokeEmtrIDs[i]);
        if (emitter != NULL) {
            emitter->deleteAllParticle();
        }
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daE_VA_Delete(daE_VA_c* i_this) {
    return i_this->_delete();
}

int daE_VA_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_VA", 0x33);
    JUT_ASSERT(0, modelData != NULL);

    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                  (J3DAnmTransform*)dComIfG_getObjectRes("E_VA", 15), 0, 1.0f, 0,
                                  -1, &mSound, 0x80000, 0x31000084);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }

    if (!mInvisModel.create(mpMorf->getModel(), 1)) {
        return 0;
    }

    J3DModel* model = mpMorf->getModel();
    model->setUserArea((uintptr_t)this);

    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        if (i == 11 || i == 12 || i == 13 || i == 14 || i == 27 || i == 33 || i == 34) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("E_VA", 0x30);
    JUT_ASSERT(0, modelData != NULL);

    mpWeaponModel = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
    if (mpWeaponModel == NULL) {
        return 0;
    }

    mpWeaponBrk = new mDoExt_brkAnm();
    if (mpWeaponBrk == NULL) {
        return 0;
    }

    if (!mpWeaponBrk->init(mpWeaponModel->getModelData(),
                           (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_VA", 0x3B), TRUE,
                           J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1))
    {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("E_VA", 0x37);
    JUT_ASSERT(0, modelData != NULL);

    mpEndEfMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                       (J3DAnmTransform*)dComIfG_getObjectRes("E_VA", 7), 0, 1.0f,
                                       0, -1, &mSound, 0x80000, 0x31000084);
    if (mpEndEfMorf == NULL || mpEndEfMorf->getModel() == NULL) {
        return 0;
    }

    model = mpEndEfMorf->getModel();
    model->setUserArea((uintptr_t)this);

    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        if (i == 11 || i == 12 || i == 13 || i == 14 || i == 27 || i == 33 || i == 34) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }

    mpEndEfBrk = new mDoExt_brkAnm();
    if (mpEndEfBrk == NULL) {
        return 0;
    }

    if (!mpEndEfBrk->init(model->getModelData(),
                          (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_VA", 0x3A), TRUE,
                          J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1))
    {
        return 0;
    }

    if (!field_0x139c.init(1, 5, 1)) {
        return 0;
    }

    f32* size_p = field_0x139c.getSize(0);
    for (int i = 0; i < 5; i++, size_p++) {
        *size_p = 5.0f;
    }

    if (!mRope.init(11, 10, (ResTIMG*)dComIfG_getObjectRes("E_VA", 0x3E), 1)) {
        return 0;
    }

    for (int i = 0; i < 11; i++) {
        f32* size_p = mRope.getSize(i);

        for (int j = 0; j < 10; ++j, size_p++) {
            *size_p = 2.0f;
        }
    }

    for (int i = 0; i < 40; i++) {
        switch (va_tag_set_size[i]) {
        case 0:
            modelData = (J3DModelData*)dComIfG_getObjectRes("E_VA", 0x34);
            mCardFlags[i] = 0;
            break;
        case 1:
            modelData = (J3DModelData*)dComIfG_getObjectRes("E_VA", 0x35);
            mCardFlags[i] = 1;
            break;
        case 2:
            modelData = (J3DModelData*)dComIfG_getObjectRes("E_VA", 0x36);
            mCardFlags[i] = 2;
            break;
        }

        JUT_ASSERT(0, modelData != NULL);
        mpCardModels[i] = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daE_VA_c*)i_this)->CreateHeap();
}

int daE_VA_c::create() {
    fopAcM_ct(this, daE_VA_c);

    int phase_state = dComIfG_resLoad(&mPhase, "E_VA");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_VA PARAM %x\n", fopAcM_GetParam(this));

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x216A0)) {
            return cPhs_ERROR_e;
        }

        if (!init_hio) {
            init_hio = true;
            mInitHIO = true;
            l_HIO.field_0x04 = -1;
        }

        attention_info.flags = 0;
        mSwNo = fopAcM_GetParam(this);

        if (mSwNo != 0xFF && dComIfGs_isSwitch(mSwNo, fopAcM_GetRoomNo(this))) {
            g_env_light.mColpatWeather = 4;
            g_env_light.wether_pat0 = 4;
            g_env_light.wether_pat1 = 4;
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -500.0f, -500.0f, -500.0f);
        fopAcM_SetMax(this, 500.0f, 500.0f, 500.0f);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcchCir.SetWall(40.0f, 500.0f);

        for (int i = 0; i < 2; i++) {
            mMagicAcch[i].Set(&mMagicPos[i], &mMagicOldPos[i], this, 1, &mMagicAcchCir[i],
                              &mMagicSpeed[i], NULL, NULL);
            mMagicAcchCir[i].SetWall(0.0f, 120.0f);
        }

        health = 100;
        field_0x560 = 100;

        mBodyCcStts.Init(0xFF, 0, this);
        mNeckSph.Set(cc_vt_neck_src);
        mNeckSph.SetStts(&mBodyCcStts);

        for (int i = 0; i < 3; i++) {
            mBodyCyls[i].Set(cc_vt_body_src);
            mBodyCyls[i].SetStts(&mBodyCcStts);
        }

        for (int i = 0; i < 4; i++) {
            mAttackSphs[i].Set(cc_vt_attack_src);
            mAttackSphs[i].SetStts(&mBodyCcStts);
        }

        field_0x1b3c.Init(0xFF, 0, this);

        for (int i = 0; i < 190; i++) {
            mLineSphs[i].Set(cc_vt_line_src);
            mLineSphs[i].SetStts(&field_0x1b3c);
        }

        mMagicSphs[0].Set(cc_vt_magic_src);
        mMagicSphs[0].SetStts(&field_0x1b3c);
        mMagicSphs[1].Set(cc_vt_magic_src);
        mMagicSphs[1].SetStts(&field_0x1b3c);

        mSound.init(&current.pos, &eyePos, 3, 1);
        mSound.setEnemyName("E_va");

        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;
        mFootSmokeAlpha = 0;
        mEyeAlpha = 0;
        mGlowLightA = 0;

        attention_info.distances[fopAc_attn_BATTLE_e] = 62;
        g_env_light.mColpatWeather = 2;
        g_env_light.wether_pat0 = 2;
        g_env_light.wether_pat1 = 2;

        if (daPy_getPlayerActorClass()->current.pos.z >= 1850.0f) {
            cDmr_SkipInfo = 0;
        }

        if (cDmr_SkipInfo != 0) {
            dComIfGs_onOneZoneSwitch(9, fopAcM_GetRoomNo(this));
            cDmr_SkipInfo = 0;
            mWeponEfMode = 3;
            mAction = ACTION_CLEAR_WAIT_e;

            Z2GetAudioMgr()->subBgmStart(Z2BGM_VARIANT);
            Z2GetAudioMgr()->changeSubBgmStatus(1);
            mKankyoColType = 0;

            g_env_light.mColpatWeather = 2;
            g_env_light.wether_pat0 = 2;
            g_env_light.wether_pat1 = 2;
        } else {
            mAction = ACTION_DEMO_OP_WAIT_e;
            mKankyoColType = 5;
            g_env_light.mColpatWeather = 3;
            g_env_light.wether_pat0 = 3;
            g_env_light.wether_pat1 = 3;
        }

        daE_VA_Execute(this);
    }

    return phase_state;
}


daE_VA_c::daE_VA_c() {
    /* empty function */
}

static int daE_VA_Create(daE_VA_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_VA_Method = {
    (process_method_func)daE_VA_Create,
    (process_method_func)daE_VA_Delete,
    (process_method_func)daE_VA_Execute,
    (process_method_func)daE_VA_IsDelete,
    (process_method_func)daE_VA_Draw,
};

extern actor_process_profile_definition g_profile_E_VT = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_VT,               // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daE_VA_c),        // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    756,                     // mPriority
    &l_daE_VA_Method,        // sub_method
    0x00040000,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
