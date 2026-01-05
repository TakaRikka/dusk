/**
 * @file d_a_b_yo_ice.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_yo_ice.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "SSystem/SComponent/c_math.h"
#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_b_yo.h"
#include "d/actor/d_a_mirror.h"
#include "d/actor/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"

class daB_YOI_HIO_c {
public:
    daB_YOI_HIO_c();
    virtual ~daB_YOI_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mScaleY;
    /* 0xC */ f32 mScaleXZ;
};

STATIC_ASSERT(sizeof(daB_YOI_HIO_c) == 0x10);

static bool hio_set;

static daB_YOI_HIO_c l_HIO;

namespace {

static dCcD_SrcCyl cc_yo_src = {
    {
        {0x0, {{0x100000, 0x2, 0xC}, {0xD8BBFDFF, 0x43}, 0x55}}, // mObj
        {dCcD_SE_METAL, 0x0, 0xD, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x203}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            40.0f, // mRadius
            40.0f // mHeight
        } // mCyl
    } // mCylAttr
};

static dCcD_SrcCyl cc_yo_iron_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x400000, 0x3}, 0x25}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x2, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x106}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            40.0f, // mRadius
            40.0f // mHeight
        } // mCyl
    } // mCylAttr
};

static dCcD_SrcCyl cc_yo_cam_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x5}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x2, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x102}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            40.0f, // mRadius
            40.0f // mHeight
        } // mCyl
    } // mCylAttr
};

}  // namespace

daB_YOI_HIO_c::daB_YOI_HIO_c() {
    field_0x4 = -1;
    mScaleY = 1.0f;
    mScaleXZ = 1.2f;
}

int daB_YOI_c::draw() {
    if (mDeleteTimer != 0) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    fopAcM_setEffectMtx(this, mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    daMirror_c::entry(mpModel);

    J3DModelData* model_data = mpCrackModel->getModelData();
    for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
        model_data->getMaterialNodePointer(i)->getTevKColor(3)->a = mCrackAlpha;
    }

    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpCrackModel, &tevStr);
    fopAcM_setEffectMtx(this, mpCrackModel->getModelData());
    mDoExt_modelUpdateDL(mpCrackModel);

    return 1;
}

static int daB_YOI_Draw(daB_YOI_c* i_this) {
    return i_this->draw();
}

void daB_YOI_c::setActionMode(int i_action, int i_mode) {
    mAction = i_action;
    mMode = i_mode;
}

void daB_YOI_c::damage_check() {
    mCcStts.Move();

    if (mIFrameTimer == 0) {
        mAtInfo.mpCollider = NULL;
        if (mCcCyl0.ChkTgHit()) {
            mAtInfo.mpCollider = mCcCyl0.GetTgHitObj();
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                mIFrameTimer = 20;
            } else {
                mIFrameTimer = 10;
            }
            if (mAtInfo.mAttackPower <= 1) {
                mIFrameTimer = 10;
            }
            def_se_set(&mCreatureSound, mAtInfo.mpCollider, 0x2A, this);
        }
    }

    if (mCcCyl1.ChkTgHit()) {
        dCam_getBody()->ForceLockOn(this);
        setBreakIceEffect();
        mpBlizzeta->onIceBreak(mIcicleNo);
        mSoundObj.startSound(Z2SE_EN_YO_ICE_BREAK, 0, -1);
        
        if (current.pos.y <= 100.0f) {
            fopAcM_delete(this);
        } else {
            mDeleteTimer = 15;
        }

        if (cM_rnd() < 0.33f) {
            cXyz pos = current.pos;
            if (pos.y < 50.0f) {
                pos.y = 50.0f;
            }
            fopAcM_createItem(&pos, fpcNm_ITEM_HEART, -1, -1, NULL, NULL, 0);
        }
    }
}

void daB_YOI_c::setOperate(int param_0) {
    switch (param_0) {
    case 0:
        setActionMode(ACT_SINGLE_FALL, 0);
        break;
    case 1:
        setActionMode(ACT_RISE_UP, 0);
        break;
    case 2:
        setActionMode(ACT_RISE_UP, 1);
        break;
    case 3:
        fopAcM_OffStatus(this, 0x4000);
        setActionMode(ACT_RISE_UP, 8);
        break;
    case 4:
        setActionMode(ACT_RISE_UP, 9);
        break;
    case 5:
        setActionMode(ACT_YOSE, 0);
        break;
    case 6:
        setActionMode(ACT_YOSE_FALL, 0);
        break;
    case 7:
        setBreakIceEffect();
        mpBlizzeta->onIceBreak(mIcicleNo);
        mSoundObj.startSound(Z2SE_EN_YO_ICE_BREAK, 0, -1);
        fopAcM_delete(this);
        if (cM_rnd() < 0.33f) {
            cXyz pos = current.pos;
            if (pos.y < 50.0f) {
                pos.y = 50.0f;
            }
            fopAcM_createItem(&pos, fpcNm_ITEM_HEART, -1, -1, NULL, NULL, 0);
        }
        break;
    case 8:
        setActionMode(ACT_DEMO_REVIVAL, 0);
        break;
    case 9:
        setActionMode(ACT_DAMAGE, 0);
        break;
    case 10:
        fopAcM_OnStatus(this, 0x4000);
        setActionMode(ACT_DEATH, 0);
        break;
    }
}

void daB_YOI_c::setWeight(u8 i_weight) {
    mCcStts.SetWeight(i_weight);
    mCcCyl0.SetStts(&mCcStts);
}

void daB_YOI_c::setBreakIceEffect() {
    static u16 break_effect_id[3] = {0x87BB, 0x87BC, 0x87BD};
    current.angle.y = fopAcM_searchPlayerAngleY(this) + 0x8000;
    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(break_effect_id[i], &current.pos, &tevStr, &current.angle, NULL);
    }
}

void daB_YOI_c::setWallBreakIceEffect() {
    static u16 wall_break_effect_id[3] = {0x87B8, 0x87B9, 0x87BA};
    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(wall_break_effect_id[i], &current.pos, &tevStr, NULL, NULL);
    }
}

void daB_YOI_c::setApperEffect() {
    mAppearKey = dComIfGp_particle_set(mAppearKey, 0x87B5, &current.pos, &tevStr);
}

void daB_YOI_c::setApperEffect2() {
    static u16 apper_effect_id[2] = {0x87B6, 0x87B7};
    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(apper_effect_id[i], &current.pos, &tevStr, NULL, NULL);
    }
}

void daB_YOI_c::setLandingEffect() {
    static u16 l_landing_effect_id[6] = {0x86DC, 0x86DD, 0x86DE, 0x86DF, 0x86E0, 0x86E1};
    for (int i = 0; i < 6; i++) {
        dComIfGp_particle_set(l_landing_effect_id[i], &current.pos, &tevStr, &shape_angle, NULL);
    }
}

void daB_YOI_c::executeFirst() {
    cXyz pos = mpBlizzeta->getIceCenterPos();
    f32 range = mpBlizzeta->getIceRange();
    s16 angle;

    switch (mMode) {
    case 0:
        gravity = 0.0f;
        speed.y = 0.0f;
        mMode = 1;
        mTimer1 = 150.0f + cM_rndFX(10.0f);
        mTimer2 = 200;
        // fallthrough

    case 1:
        setApperEffect();
        angle = (mIcicleNo + 1) * 0x1555 + 0x0AAA;
        current.pos.x = pos.x + range * cM_ssin(angle);
        current.pos.y = pos.y;
        current.pos.z = pos.z + range * cM_scos(angle);

        if (mTimer1 == 0) {
            setApperEffect2();
            dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mScaleF = 1.0f;
            mMode = 2;
            speed.y = 10.0f;
        }
        break;

    case 2:
        mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_MOVE, 0, -1);
        if (current.pos.y < pos.y) {
            cLib_chaseF(&speed.y, 10.0f, 0.5f);
        } else {
            cLib_chaseF(&speed.y, -10.0f, 0.5f);
        }

        if (mTimer2 == 0) {
            angle = mIcicleNo * 0x1999;
            pos.set(pos.x + range * cM_ssin(angle), current.pos.y, pos.z + range * cM_scos(angle));
            cLib_chasePosXZ(&current.pos, pos, 5.0f);
        }
    }
}

static int const yoi_fall_timer[10] = {
    0x00, 0x0C, 0x18, 0x24, 0x30, 0x3C, 0x48, 0x54, 0x60, 0x6C,
};

cXyz daB_YOI_c::getSingleFallPos() {
    Vec eye = dCam_getBody()->Eye();
    Vec center = dCam_getBody()->Center();
    s16 angle = (s16)cLib_targetAngleY(&eye, &center);
    int iVar5 = mTimer1 - 5;
    if (iVar5 < 0) {
        iVar5 = 0;
    }
    cXyz pos(mpBlizzeta->getPlayerXBuf(iVar5) + current.pos.y * cM_ssin(angle) * 0.6f,
             current.pos.y,
             mpBlizzeta->getPlayerZBuf(iVar5) + current.pos.y * cM_scos(angle) * 0.6f);
    return pos;
}

void daB_YOI_c::executeRiseUp() {
    cXyz center_pos = mpBlizzeta->getIceCenterPos();
    f32 range = mpBlizzeta->getIceRange();
    s16 angle = mpBlizzeta->getIceAngle();
    cXyz target;
    f32 step;

    switch (mMode) {
    case 0:
        speed.y = 0.0f;
        speedF = 0.0f;
        mTimer1 = 150;
        mMode = 5;
        // fallthrough

    case 5:
    case 6:
        mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_VIBERATE, 0, -1);
        cLib_chaseAngleS(&shape_angle.x, 0, 0x40);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x40);
        mWobbleAngle.x = cM_rndFX(0x40);
        mWobbleAngle.z = cM_rndFX(0x40);
        
        if (mMode == 5) {
            current.pos.y += 0.5f;
        } else {
            current.pos.y += 0.2f;
        }

        if (mTimer1 == 0) {
            mSoundObj.startSound(Z2SE_EN_YO_ICE_PULLOUT, 0, -1);
            mMode = 7;
            speed.y = 60.0f;
            mWobbleAngle.x = mWobbleAngle.z = 0;
            if (cM_rnd() < 0.5f) {
                mAngleSpeedY = cM_rndF(0x100) + 0xE00;
            } else {
                mAngleSpeedY = cM_rndF(0x100) + -0xE00;
            }
        }
        break;

    case 1:
        mMode = 7;
        speed.y = 55.0f;
        mWobbleAngle.x = mWobbleAngle.z = 0;
        if (cM_rnd() < 0.5f) {
            mAngleSpeedY = cM_rndF(0x100) + 0xE00;
        } else {
            mAngleSpeedY = cM_rndF(0x100) + -0xE00;
        }
        // fallthrough

    case 7:
        cLib_chaseAngleS(&mAngleSpeedY, 0, 0x40);
        shape_angle.y += mAngleSpeedY;
        cLib_chaseAngleS(&shape_angle.x, 0, 0x40);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x40);
        if (cLib_chaseF(&speed.y, 5.0f, 5.0f)) {
            mMode = 8;
        }
        break;

    case 8:
        mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_MOVE, 0, -1);
        cLib_chaseAngleS(&mAngleSpeedY, 0, 0x40);
        shape_angle.y += mAngleSpeedY;
        cLib_chaseF(&mScaleF, 1.0f, 0.1f);
        cLib_chaseAngleS(&shape_angle.x, 0, 0x40);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x40);

        if (center_pos.y < 200.0f) {
            center_pos.y = 200.0f;
        }
        if (center_pos.y > 800.0f) {
            center_pos.y = 800.0f;
        }
        if (current.pos.y < center_pos.y - 100.0f) {
            cLib_chaseF(&speed.y, 15.0f, 0.5f);
        } else {
            cLib_chaseF(&speed.y, -15.0f, 0.5f);
        }

        target.set(center_pos.x + range * cM_ssin(angle + mIcicleNo * 0x1999), current.pos.y,
                   center_pos.z + range * cM_scos(angle + mIcicleNo * 0x1999));
        cLib_chasePos(&current.pos, target, 50.0f);
        break;

    case 9:
        mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_MOVE, 0, -1);
        cLib_chaseF(&mScaleF, 1.0f, 0.1f);
        cLib_chaseAngleS(&shape_angle.x, 0, 0x40);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x40);

        if (center_pos.y < 200.0f) {
            center_pos.y = 200.0f;
        }
        if (center_pos.y > 800.0f) {
            center_pos.y = 800.0f;
        }
        if (current.pos.y < center_pos.y - 100.0f) {
            cLib_chaseF(&speed.y, 15.0f, 0.5f);
        } else {
            cLib_chaseF(&speed.y, -15.0f, 0.5f);
        }

        mTimer1 = yoi_fall_timer[mSubNumber] * 1.3f;
        step = daPy_getPlayerActorClass()->speedF + 30.0f;
        if (step > 50.0f) {
            step = 50.0f;
        }
        cLib_chasePosXZ(&current.pos, getSingleFallPos(), step);
    }
}

void daB_YOI_c::executeSingleFall() {
    switch (mMode) {
    case 0:
        mCcCyl1.OffTgSetBit();
        mCcCyl0.OffTgSetBit();
        mCcCyl0.OnAtSetBit();
        mCcOffset = 100.0f;
        setWeight(200);
        mAngleSpeedY = 0;
        mMode = 10;
        mTimer2 = 20;
        // fallthrough

    case 10:
        mTimer1 = yoi_fall_timer[mSubNumber] * 1.3f;
        cLib_chasePosXZ(&current.pos, getSingleFallPos(), 50.0f);

        if (mTimer2 + mTimer1 >= 20) {
            cLib_chaseF(&speed.y, 0.0f, 0.5f);
            cLib_chaseF(&current.pos.y, 600.0f, 5.0f);
            mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_MOVE, 0, -1);
        } else {
            cLib_chaseF(&speed.y, 15.0f, 5.0f);
            cLib_chaseAngleS(&mAngleSpeedY, 0xC00, 0x100);
            shape_angle.y += mAngleSpeedY;
            mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_ATTACK, 0, -1);
        }

        if (mTimer2 == 0) {
            if (mTimer1 != 0) {
                mTimer1 += cM_rndFX(4.0f);
            }
            mMode = 1;
        }
        break;

    case 1:
        cLib_chasePosXZ(&current.pos, getSingleFallPos(), 50.0f);

        if (mTimer1 >= 20) {
            cLib_chaseF(&speed.y, 0.0f, 0.5f);
            cLib_chaseF(&current.pos.y, 600.0f, 5.0f);
            mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_MOVE, 0, -1);

        } else {
            cLib_chaseF(&speed.y, 15.0f, 5.0f);
            cLib_chaseAngleS(&mAngleSpeedY, 0xC00, 0x100);
            shape_angle.y += mAngleSpeedY;
            mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_ATTACK, 0, -1);

            if (mTimer1 == 0) {
                mMode = 2;
                speed.y = -85.0f;

                if (daPy_getPlayerActorClass()->getDamageWaitTimer() != 0
                    && daPy_getPlayerActorClass()->speedF < 10.0f)
                {
                    s16 angle = cM_rndFX(0x8000);
                    f32 offset = cM_rndF(40.0f) + 180.0f;
                    mFallTarget = daPy_getPlayerActorClass()->current.pos;
                    mFallTarget.x += offset * cM_ssin(angle);
                    mFallTarget.z += offset * cM_scos(angle);

                } else {
                    mFallTarget = daPy_getPlayerActorClass()->current.pos;
                    f32 offset = daPy_getPlayerActorClass()->speedF;
                    if (offset >= 3.0f) {
                        offset *= 5.0f;
                        if (offset < 30.0f) {
                            offset = 30.0f;
                        }
                        s16 angle = daPy_getPlayerActorClass()->current.angle.y
                                        + 0x8000 + cM_rndFX(0x4000);
                        mFallTarget.x += offset * cM_ssin(angle);
                        mFallTarget.z += offset * cM_scos(angle);
                    }
                }

                speedF = current.pos.absXZ(mFallTarget) / 10.0f;
                if (speedF > 140.0f) {
                    speedF = 140.0f;
                }
                current.angle.y = cLib_targetAngleY(&current.pos, &mFallTarget);
            }
        }
        break;

    case 2:
        mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_ATTACK, 0, -1);
        cLib_chaseAngleS(&mAngleSpeedY, 0, 0x20);
        shape_angle.y += mAngleSpeedY;

        if (current.pos.y <= 100.0f) {
            mMode = 4;
            mTargetAngleX = cM_rndFX(0x800);
            shape_angle.y = cM_rndFX(0x8000);
            mTimer1 = 10;
        }
        break;

    case 4:
        field_0x6ad = true;
        cLib_chaseAngleS(&mAngleSpeedY, 0, 0x20);
        shape_angle.y += mAngleSpeedY;

        if (current.pos.y < 0.0f) {
            dComIfGp_getVibration().StartShock(6, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            setLandingEffect();
            mSoundObj.startSound(Z2SE_EN_YO_ICE_LAND, 0, -1);
            mMode = 5;
        }
        break;

    case 5:
        field_0x6ad = true;
        cLib_chaseAngleS(&mAngleSpeedY, 0, 0x20);
        shape_angle.y += mAngleSpeedY;
        cLib_addCalcAngleS(&shape_angle.x, mTargetAngleX, 4, 0x400, 0x100);

        if (current.pos.y < -200.0f) {
            mAngleSpeedY = 0;
            if (daPy_getPlayerActorClass()->current.pos.absXZ() > 1700.0f) {
                setWeight(200);
            } else {
                setWeight(0xFE);
            }
            mCcCyl1.OnTgSetBit();
            mCcCyl0.OnTgSetBit();
            mCcCyl0.OffAtSetBit();
            mCcOffset = 0.0f;
            mpBlizzeta->onIceStatus(mIcicleNo);
            speed.y = 0.0f;
            speedF = 0.0f;
        }
    }
}

void daB_YOI_c::executeYose() {
    cXyz center_pos = mpBlizzeta->getIceCenterPos();
    f32 range = mpBlizzeta->getIceRange();
    s16 angle = mpBlizzeta->getIceAngle();
    cXyz target;

    switch (mMode) {
    case 0:
        speed.y = 0.0f;
        speedF = 0.0f;
        mTimer1 = 90;
        mMode = 5;
        // fallthrough

    case 5:
        mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_VIBERATE, 0, -1);
        field_0x6ad = true;
        if (daPy_getPlayerActorClass()->current.pos.absXZ() > 1700.0f) {
            setWeight(200);
        } else {
            setWeight(0xFD);
        }
        cLib_chaseAngleS(&shape_angle.x, 0, 0x40);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x40);
        mWobbleAngle.x = cM_rndFX(0x40);
        mWobbleAngle.z = cM_rndFX(0x40);
        current.pos.y += 0.5f;

        if (mTimer1 == 0) {
            mSoundObj.startSound(Z2SE_EN_YO_ICE_PULLOUT, 0, -1);
            setWeight(0xFD);
            mMode = 7;
            speed.y = 70.0f;
            mWobbleAngle.x = mWobbleAngle.z = 0;
            if (cM_rnd() < 0.5f) {
                mAngleSpeedY = cM_rndF(0x100) + 0xE00;
            } else {
                mAngleSpeedY = cM_rndF(0x100) + -0xE00;
            }
        }
        break;

    case 7:
        mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_MOVE, 0, -1);
        cLib_chaseAngleS(&mAngleSpeedY, 0, 0x40);
        shape_angle.y += mAngleSpeedY;
        cLib_chaseAngleS(&shape_angle.x, 0, 0x40);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x40);
        if (cLib_chaseF(&speed.y, 5.0f, 5.0f)) {
            mMode = 8;
            mYoseSpeed = 0.0f;
        }
        break;

    case 8:
    case 9:
        mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_MOVE, 0, -1);
        cLib_chaseAngleS(&mAngleSpeedY, 0, 0x40);
        shape_angle.y += mAngleSpeedY;
        cLib_chaseAngleS(&shape_angle.x, 0, 0x40);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x40);
        if (current.pos.y < 200.0f) {
            cLib_chaseF(&speed.y, 0.0f, 1.0f);
        } else {
            cLib_chaseF(&speed.y, -10.0f, 0.5f);
        }
        
        target.set(center_pos.x + range * cM_ssin(angle + mIcicleNo * 0x1999), current.pos.y,
                   center_pos.z + range * cM_scos(angle + mIcicleNo * 0x1999));
        if (target.absXZ() > 1600.0f) {
            s16 target_angle = cM_atan2s(target.x, target.z);
            target.x = 1600.0f * cM_ssin(target_angle);
            target.z = 1600.0f * cM_scos(target_angle);
        }
        cLib_chaseF(&mYoseSpeed, 75.0f, 1.0f);
        cLib_addCalcPosXZ2(&current.pos, target, 0.2f, mYoseSpeed);

        if (current.pos.absXZ(target) < 100.0f) {
            mpBlizzeta->onIceStatus(mIcicleNo);
            if (current.pos.absXZ(target) < 50.0f) {
                mMode = 9;
            }
        }
        break;
    }
}

void daB_YOI_c::executeYoseFall() {
    cXyz center_pos = mpBlizzeta->getIceCenterPos();
    f32 range = mpBlizzeta->getIceRange();
    s16 angle = mpBlizzeta->getIceAngle();
    cXyz target;

    switch (mMode) {
    case 0:
        mTimer1 = 30;
        mMode = 10;
        // fallthrough

    case 10:
        cLib_chaseF(&speed.y, 15.0f, 1.0f);
        cLib_chaseAngleS(&mAngleSpeedY, 0xC00, 0x100);
        shape_angle.y += mAngleSpeedY;
        target.set(center_pos.x + range * cM_ssin(angle + mIcicleNo * 0x1999), current.pos.y,
                   center_pos.z + range * cM_scos(angle + mIcicleNo * 0x1999));
        cLib_chaseF(&mYoseSpeed, 100.0f, 1.0f);
        cLib_addCalcPosXZ2(&current.pos, target, 0.2f, mYoseSpeed);

        if (mTimer1 == 0) {
            speed.y = 0.0f;
            speedF = 0.0f;
            mMode = 1;
            speed.y = -85.0f;
            mCcCyl0.OnAtSetBit();
        }
        break;

    case 1:
    case 2:
    case 3:
        cLib_chaseAngleS(&mAngleSpeedY, 0, 0x20);
        shape_angle.y += mAngleSpeedY;
        target.set(center_pos.x + range * cM_ssin(angle + mIcicleNo * 0x1999), current.pos.y,
                   center_pos.z + range * cM_scos(angle + mIcicleNo * 0x1999));
        cLib_chaseF(&mYoseSpeed, 100.0f, 1.0f);
        cLib_chasePosXZ(&current.pos, target, mYoseSpeed);

        if (mMode == 1) {
            if (current.pos.y <= 100.0f) {
                mMode = 2;
                mTargetAngleX = 0;
                shape_angle.y = cM_rndFX(0x8000);
                mTimer1 = 10;
            }
        } else if (mMode == 2) {
            if (current.pos.y < 0.0f) {
                dComIfGp_getVibration().StartShock(6, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                setLandingEffect();
                mSoundObj.startSound(Z2SE_EN_YO_ICE_LAND, 0, -1);
                mMode = 3;
            }
        } else {
            cLib_addCalcAngleS(&shape_angle.x, mTargetAngleX, 4, 0x400, 0x100);
            if (current.pos.y < -200.0f) {
                mpBlizzeta->onIceStatus(mIcicleNo);
                speed.y = 0.0f;
                speedF = 0.0f;
                mMode = 4;
                mAngleSpeedY = 0;
                mCcCyl0.OffAtSetBit();
            }
        }
        break;

    case 4:
        cLib_addCalcAngleS(&shape_angle.x, mTargetAngleX, 4, 0x400, 0x100);
        break;
    }
}

void daB_YOI_c::executeDemoRevival() {
    cXyz center_pos = mpBlizzeta->getIceCenterPos();
    f32 range = mpBlizzeta->getIceRange();
    s16 angle = mpBlizzeta->getIceAngle();
    cXyz target;
    cLib_chaseF(&mScaleF, 1.0f, 0.1f);
    target.set(center_pos.x + range * cM_ssin(angle + mIcicleNo * 0x1999), current.pos.y,
               center_pos.z + range * cM_scos(angle + mIcicleNo * 0x1999));
    
    switch (mMode) {
    case 0:
        speedF = 0.0f;
        speed.y = 0.0f;
        current.pos = target;
        current.pos.y = center_pos.y;
        mMode = 1;
        // fallthrough

    case 1:
        mSoundObj.startLevelSound(Z2SE_EN_YO_ICE_MOVE, 0, -1);
        if (current.pos.y < 1000.0f) {
            cLib_chaseF(&speed.y, 10.0f, 0.5f);
        } else {
            cLib_chaseF(&speed.y, -10.0f, 0.5f);
        }
        cLib_chasePos(&current.pos, target, 50.0f);
        break;
    }
}

void daB_YOI_c::executeDamage() {
    cXyz center_pos = mpBlizzeta->getIceCenterPos();
    s16 angle_diff;

    switch (mMode) {
    case 0:
        mMode = 2;
        speed.y = 40.0f + cM_rndFX(15.0f);
        speedF = 80.0f + cM_rndFX(5.0f);
        mWobbleAngle.x = mWobbleAngle.z = 0;
        if (cM_rnd() < 0.5f) {
            mAngleSpeedY = cM_rndF(0x100) + 0xE00;
        } else {
            mAngleSpeedY = cM_rndF(0x100) + -0xE00;
        }
        angle_diff = cLib_targetAngleY(&center_pos, &current.pos) - mpBlizzeta->current.angle.y;
        angle_diff = 0.3f * angle_diff;
        current.angle.y = mpBlizzeta->current.angle.y + angle_diff + cM_rndFX(0x400);
        shape_angle.x = shape_angle.z = 0;
        // fallthrough

    case 2:
        cLib_chaseAngleS(&mAngleSpeedY, 0, 0x40);
        shape_angle.y += mAngleSpeedY;
        cLib_chaseAngleS(&shape_angle.x, 0, 0x40);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x40);
        if (current.pos.y < center_pos.y - 100.0f) {
            cLib_chaseF(&speed.y, 15.0f, 1.0f);
        } else {
            cLib_chaseF(&speed.y, -15.0f, 1.0f);
        }

        if (current.pos.absXZ() >= 1700.0f) {
            setWallBreakIceEffect();
            mpBlizzeta->onIceBreak(mIcicleNo);
            mSoundObj.startSound(Z2SE_EN_YO_ICE_BREAK, 0, -1);
            fopAcM_delete(this);
        }
        break;
    }
}

void daB_YOI_c::executeDeath() {
    mSoundObj.startSound(Z2SE_EN_YO_ICE_BREAK, 0, -1);
    mpBlizzeta->onIceBreak(mIcicleNo);
    fopAcM_delete(this);
    dComIfGp_particle_levelEmitterOnEventMove(mParticleKey);
}

void daB_YOI_c::crack_execute() {
    if (current.pos.y <= 0.0f) {
        if (speed.y == 0.0f) {
            mCrackAlpha = 255.0f;
            mpCrackAngleY = shape_angle.y;
            mpCrackPos.set(current.pos.x, 1.0f, current.pos.z);
        }
    } else {
        cLib_chaseF(&mCrackAlpha, 0.0f, 2.0f);
    }
}

void daB_YOI_c::action() {
    cXyz scale(mScaleF, mScaleF, mScaleF);
    mParticleKey = dComIfGp_particle_set(mParticleKey, 0x87BE, &current.pos, &tevStr, &shape_angle,
                                         &scale, 0xff, NULL, -1, NULL, NULL, NULL);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey);
    if (emitter != NULL) {
        emitter->setGlobalScale(JGeometry::TVec3<f32>(scale));
    }

    damage_check();
    field_0x6ad = false;

    switch (mAction) {
    case ACT_FIRST:
        executeFirst();
        break;
    case ACT_RISE_UP:
        executeRiseUp();
        break;
    case ACT_DEMO_REVIVAL:
        executeDemoRevival();
        break;
    case ACT_SINGLE_FALL:
        executeSingleFall();
        break;
    case ACT_YOSE:
        executeYose();
        break;
    case ACT_YOSE_FALL:
        executeYoseFall();
        break;
    case ACT_DAMAGE:
        executeDamage();
        break;
    case ACT_DEATH:
        executeDeath();
        break;
    }

    if (!field_0x6ad) {
        mCcStts.ClrCcMove();
    }
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());

    if (mAction != ACT_DAMAGE && current.pos.absXZ() > 1600.0f) {
        s16 angle = cM_atan2s(current.pos.x, current.pos.z);
        current.pos.x = 1600.0f * cM_ssin(angle);
        current.pos.z = 1600.0f * cM_scos(angle);
    }

    cXyz gnd_chk_pos = current.pos;
    gnd_chk_pos.y += 1000.0f;
    mGndChk.SetPos(&gnd_chk_pos);
    dComIfG_Bgsp().GroundCross(&mGndChk);
    
    crack_execute();
}

void daB_YOI_c::mtx_set() {
    f32 scale_y = l_HIO.mScaleY * mScaleF;
    f32 scale_xz = l_HIO.mScaleXZ * mScaleF;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle + mWobbleAngle);
    mDoMtx_stack_c::scaleM(scale_xz, scale_y, scale_xz);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::transS(mpCrackPos);
    mDoMtx_stack_c::YrotM(mpCrackAngleY);
    mDoMtx_stack_c::scaleM(scale_xz, 1.0f, scale_xz);
    mpCrackModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daB_YOI_c::cc_set() {
    cXyz center;

    mDoMtx_stack_c::copy(mpModel->getBaseTRMtx());
    mDoMtx_stack_c::transM(0.0f, 350.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&eyePos);

    mCcCyl2.SetC(current.pos);
    mCcCyl2.SetH(550.0f);
    mCcCyl2.SetR(200.0f);
    dComIfG_Ccsp()->Set(&mCcCyl2);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::transM(0.0f, mCcOffset + 100.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&center);

    f32 radius = 160.0f;
    if (field_0x6ad) {
        radius = 120.0f;
    }

    mCcCyl0.SetC(center);
    mCcCyl0.SetR(radius);
    mCcCyl0.SetH(400.0f - mCcOffset);
    dComIfG_Ccsp()->Set(&mCcCyl0);

    radius = 160.0f;
    if (field_0x6ad) {
        radius = 180.0f;
    }

    mCcCyl1.SetC(center);
    mCcCyl1.SetR(radius);
    mCcCyl1.SetH(400.0f - mCcOffset);
    dComIfG_Ccsp()->Set(&mCcCyl1);
}

int daB_YOI_c::execute() {
    if (mDeleteTimer != 0) {
        dCam_getBody()->ForceLockOn(this);
        mDeleteTimer--;
        if (mDeleteTimer == 0) {
            if (dCam_getBody()->GetForceLockOnActor() == this) {
                dCam_getBody()->ForceLockOff(this);
            }
            fopAcM_delete(this);
            mDeleteTimer = 5;
        }

        return 1;

    } else {
        if (mTimer1 != 0) {
            mTimer1--;
        }
        if (mTimer2 != 0) {
            mTimer2--;
        }
        if (mIFrameTimer != 0) {
            mIFrameTimer--;
        }

        action();
        mtx_set();
        cc_set();

        return 1;
    }
}

static int daB_YOI_Execute(daB_YOI_c* i_this) {
    return i_this->execute();
}

static int daB_YOI_IsDelete(daB_YOI_c* i_this) {
    return 1;
}

int daB_YOI_c::_delete() {
    dComIfG_resDelete(&mPhase, "B_YO");
    
    if (mHIOInit) {
        hio_set = false;
    }

    if (heap != NULL) {
        mCreatureSound.deleteObject();
        mSoundObj.deleteObject();
    }

    return 1;
}

static int daB_YOI_Delete(daB_YOI_c* i_this) {
    return i_this->_delete();
}

int daB_YOI_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("B_YO", 0x21);
    mpModel = mDoExt_J3DModel__create(model_data, 0, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("B_YO", 0x20);
    mpCrackModel = mDoExt_J3DModel__create(model_data, 0, 0x11000084);
    if (mpCrackModel == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daB_YOI_c*>(i_this)->CreateHeap();
}

// data/rodata swap
cPhs__Step daB_YOI_c::create() {
    fopAcM_ct(this, daB_YOI_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, "B_YO");

    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1CA0)) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            hio_set = true;
            mHIOInit = true;
            l_HIO.field_0x4 = -1;
        }

        mIcicleNo = fopAcM_GetParam(this) & 0xff;
        mInitAction = (fopAcM_GetParam(this) >> 0x10) & 0xff;
        attention_info.flags = 0;
        attention_info.distances[fopAc_attn_BATTLE_e] = 62;
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
        health = 80;
        field_0x560 = 80;

        mCcStts.Init(0xfd, 0, this);
        mCcCyl0.Set(cc_yo_src);
        mCcCyl0.SetStts(&mCcStts);
        mCcCyl1.Set(cc_yo_iron_src);
        mCcCyl1.SetStts(&mCcStts);
        mCcCyl2.Set(cc_yo_cam_src);
        mCcCyl2.SetStts(&mCcStts);

        mSoundObj.init(&current.pos, 2);
        mCreatureSound.init(&current.pos, &eyePos, 3, 1);
        mAtInfo.mpSound = &mCreatureSound;
        mAtInfo.mPowerType = 1;
        mScaleF = 1.0f;
        gravity = 0.0f;

        fopAcM_SearchByID(fopAcM_GetLinkId(this), (fopAc_ac_c**)&mpBlizzeta);
        if (mpBlizzeta != NULL) {
            cXyz center = mpBlizzeta->getIceCenterPos();
            f32 range = mpBlizzeta->getIceRange();
            s16 angle = mpBlizzeta->getIceAngle();
            current.pos.x = center.x + range * cM_ssin(angle + mIcicleNo * 0x1999);
            current.pos.y = center.y;
            current.pos.z = center.z + range * cM_scos(angle + mIcicleNo * 0x1999);
        } else {
            current.pos.set(1000.0f * cM_ssin(mIcicleNo * 0x1999), 1000.0f,
                            1000.0f * cM_scos(mIcicleNo * 0x1999));
        }

        mScaleF = 0.0f;
        switch (mInitAction) {
        case 0:
            setActionMode(ACT_FIRST, 0);
            break;
        case 1:
            fopAcM_OffStatus(this, 0x4000);
            setActionMode(ACT_RISE_UP, 9);
            break;
        case 2:
            setActionMode(ACT_DEMO_REVIVAL, 0);
            break;
        }

        daB_YOI_Execute(this);
    }

    return step;
}

daB_YOI_c::daB_YOI_c() {
    /* empty function */
}

static cPhs__Step daB_YOI_Create(daB_YOI_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daB_YOI_Method = {
    (process_method_func)daB_YOI_Create,
    (process_method_func)daB_YOI_Delete,
    (process_method_func)daB_YOI_Execute,
    (process_method_func)daB_YOI_IsDelete,
    (process_method_func)daB_YOI_Draw,
};

actor_process_profile_definition g_profile_B_YOI = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_B_YOI,             // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daB_YOI_c),      // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  224,                    // mPriority
  &l_daB_YOI_Method,      // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
