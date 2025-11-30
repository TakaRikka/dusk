/**
 * @file d_a_obj_kuwagata.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "f_op/f_op_camera_mng.h"
#include "d/actor/d_a_obj_kuwagata.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_menu_insect.h"
#include "f_pc/f_pc_name.h"

class daObj_KuwHIO_c : public JORReflexible{
public:
    daObj_KuwHIO_c();
    virtual ~daObj_KuwHIO_c() {}

    void genMessage(JORMContext* ctx);

    /* 0x04 */ s8 mId;
    /* 0x08 */ f32 mFemaleModelScale;
    /* 0x0C */ f32 mMaleModelScale;
};

daObj_KuwHIO_c::daObj_KuwHIO_c() {
    mId = -1;
    mMaleModelScale = 1.5f;
    mFemaleModelScale = 1.2f;
}

#if DEBUG
void daObj_KuwHIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("黄金蟲(カブト)", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("モデルスケール(オス)", &mMaleModelScale, 0.1f, 4.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("モデルスケール(メス)", &mFemaleModelScale, 0.1f, 4.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}
#endif

void daObjKUW_c::InitCcSph() {
    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x14002, 0x11}, 0x0}}, // mObj
            {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    mStts.Init(1, 0, this);
    mSph.Set(ccSphSrc);
    mSph.SetStts(&mStts);
    mSph.SetC(current.pos);
    mSph.OnTgNoHitMark();
}

void daObjKUW_c::SetCcSph() {
    mSph.SetC(current.pos);
    mSph.SetR(20.0f);
    dComIfG_Ccsp()->Set(&mSph);
}

static int useHeapInit(fopAc_ac_c* a_this) {
    daObjKUW_c* i_this = (daObjKUW_c*)a_this;
    return i_this->CreateHeap();
}

int daObjKUW_c::CreateHeap() {
    J3DModelData* modelData;
    
    if (mSex == SEX_FEMALE) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("I_Kuw", 10);
        JUT_ASSERT(255, modelData != NULL);
    } else {
        modelData = (J3DModelData*)dComIfG_getObjectRes("I_Kuw", 11);
        JUT_ASSERT(259, modelData != NULL);
    }
    
    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("I_Kuw", 7), 2, 1.0f, 0, -1, &mSound, 0, 0x11000284);
    
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }
    
    J3DModel* model = mpMorf->getModel();
    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }
    
    J3DAnmTevRegKey* brkRes;
    if (mSex == SEX_FEMALE) {
        brkRes = (J3DAnmTevRegKey*)dComIfG_getObjectRes("I_Kuw", 14);
        if (!mpBrkAnm->init(model->getModelData(), brkRes, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        brkRes = (J3DAnmTevRegKey*)dComIfG_getObjectRes("I_Kuw", 0xf);
        if (!mpBrkAnm->init(model->getModelData(), brkRes, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    }
    
    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL) {
        return 0;
    }
    
    J3DAnmTextureSRTKey* btkRes;
    if (mSex == SEX_FEMALE) {
        btkRes = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("I_Kuw", 18);
        if (!mpBtkAnm->init(model->getModelData(), btkRes, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        btkRes = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("I_Kuw", 19);
        if (!mpBtkAnm->init(model->getModelData(), btkRes, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    }
    
    return 1;
}

static int daObjKUW_Create(fopAc_ac_c* i_this) {
    fopAcM_RegisterCreateID(daObjKUW_c, i_this, "Obj_Kuw");
    return a_this->create();
}

static int daObjKUW_Delete(daObjKUW_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Obj_Kuw");
    i_this->Delete();
    return 1;
}

void daObjKUW_c::ShopWaitAction() {
    switch (mSubAction) {
    case 0:
        speedF = 0.0f;
        mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kuw", 7), 2, 5.0f, 0.0f, 0.0f, -1.0f);
        mSubAction++;
        mTimers[0] = cM_rndF(50.0f) + 50.0f;
        break;
    case 1:
        mTimers[0]--;
        if (mTimers[0] <= 0) {
            mTimers[0] = 0;
        }

        if (mTimers[0] == 0) {
            int randVal = cM_rndF(100.0f);
            if (randVal < 40) {
                mAction = ACTION_WALK;
                mSubAction = 0;
            } else if (randVal < 70) {
                mAction = ACTION_MOVE;
                mSubAction = 0;
            } else {
                mTimers[0] = cM_rndF(50.0f) + 50.0f;
            }
        }
        break;
    }
}
void daObjKUW_c::WaitAction() {
    switch (mSubAction) {
    case 0:
        speedF = 0.0f;
        mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kuw", 7), 2, 5.0f, 0.0f, 0.0f, -1.0f);
        mSubAction++;
        mTimers[0] = cM_rndF(50.0f) + 50.0f;
        break;
    case 1:
        if (fopAcM_searchPlayerDistance(this) < 1500.0f) {
            mTimers[0]--;
            if (mTimers[0] <= 0) {
                mTimers[0] = 0;
            }
            
            if (mTimers[0] == 0) {
                int randVal = cM_rndF(100.0f);
                if (randVal < 40) {
                    mAction = ACTION_WALK;
                    mSubAction = 0;
                } else if (randVal < 70) {
                    mAction = ACTION_MOVE;
                    mSubAction = 0;
                } else {
                    mTimers[0] = cM_rndF(50.0f) + 50.0f;
                }
            }
        }
        break;
    }
}

BOOL daObjKUW_c::WallCheck() {
    dBgS_LinChk linChk;
    linChk.SetObj();
    
    linChk.Set(&old.pos, &current.pos, NULL);
    
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        cXyz* normal = plane.GetNP();
        current.pos = linChk.GetCross();
        
        cXyz temp1(0.0f, 0.0f, 0.0f);
        cXyz temp2(0.0f, normal->y, normal->z);
        field_0x99a.z = -cM_atan2s(normal->x, temp1.abs(temp2));
        field_0x99a.x = cM_atan2s(normal->z, normal->y);
        field_0x9a2 = cM_atan2s(normal->x, normal->z);
        
        return TRUE;
    }

    return FALSE;
}

void daObjKUW_c::SpeedSet() {
    speed.y += gravity;
    cXyz cStack_1c(0.0f, speed.y, speedF);
    cXyz local_28(0.0f, 0.0f, 0.0f);
    
    mDoMtx_stack_c::ZXYrotS(field_0x99a);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&cStack_1c, &local_28);
    
    current.pos.x += local_28.x;
    current.pos.y += local_28.y;
    current.pos.z += local_28.z;
    
    cLib_addCalc2(&speedF, mSpeedFTarget, 0.4f, 10.0f);
    cLib_addCalc2(&speed.y, mSpeedYTarget, 0.1f, 0.5f);
    cLib_addCalcAngleS2(&current.angle.y, mAngleTarget, 0x10, 0x100);
    shape_angle.y = current.angle.y;
}

void daObjKUW_c::WallWalk() {
    cXyz cStack_13c(0.0f, -100.0f, 3.0f * speedF);
    cXyz cStack_148(0.0f, 40.0f, 0.0f);
    cXyz cStack_154(0.0f, 20.0f, 100.0f);
    cXyz cStack_160;
    
    dBgS_LinChk linChk;
    dBgS_LinChk linChk2;
    
    linChk.SetObj();
    linChk2.SetObj();
    
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x99a);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&cStack_13c, &cStack_13c);
    mDoMtx_stack_c::multVec(&cStack_148, &cStack_148);
    
    linChk.Set(&cStack_148, &cStack_13c, NULL);
    
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        speedF = 0.3f;
        
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        cXyz* normal = plane.GetNP();
        
        s16 angleZ = cM_atan2s(normal->x, normal->z);
        
        cXyz temp1(0.0f, 0.0f, 0.0f);
        cXyz temp2(0.0f, normal->y, normal->z);
        f32 normalLen = temp1.abs(temp2);
        
        cStack_160 = linChk.GetCross();
        
        int angleDiff = angleZ - field_0x9a2;

        if (angleDiff < 0x1000 && angleDiff > -0x1000 && cStack_160.abs(old.pos) < speedF * 3.0f) {
            current.pos = cStack_160;
            field_0x9a2 = angleZ;
            field_0x99a.z = -cM_atan2s(normal->x, normalLen);
            field_0x99a.x = cM_atan2s(normal->z, normal->y);
        } else {
            mAngleTarget += 0x100;
        }
    } else {
        mAngleTarget += 0x100;
    }

    cLib_addCalcAngleS2(&current.angle.y, mAngleTarget, 0x10, 0x50);
    shape_angle.y = current.angle.y;
}

void daObjKUW_c::WalkAction() {
    for (int i = 0; i < 3; i++) {
        mTimers[i]--;
        if (mTimers[i] <= 0) {
            mTimers[i] = 0;
        }
    }
    
    switch (mSubAction) {
    case 0:
        mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kuw", 7), 2, 5.0f, 1.0f, 0.0f, -1.0f);
        mSpeedFTarget = 0.5f;
        speed.y = 0.0f;
        mSpeedYTarget = 0.0f;
        mSubAction++;
        mTimers[1] = cM_rndF(50.0f) + 50.0f;
        break;
    case 1:
        if (mTimers[0] == 0) {
            cXyz targetDir(cM_rndFX(100.0f), 0.0f, cM_rndFX(100.0f));
            mAngleTarget = cM_atan2s(targetDir.x, targetDir.z);
            mTimers[0] = cM_rndF(50.0f) + 50.0f;
        }
        
        if (mTimers[1] == 0) {
            int randVal = cM_rndF(100.0f);
            
            if (randVal < 30) {
                mAction = ACTION_MOVE;
                mSubAction = 0;
            } else if (randVal < 60) {
                mAction = ACTION_WAIT;
                mSubAction = 0;
            } else {
                mTimers[1] = cM_rndF(50.0f) + 50.0f;
            }
        }
    }
    
    WallWalk();
}

void daObjKUW_c::MoveAction() {
    for (int i = 0; i < 3; i++) {
        mTimers[i]--;
        if (mTimers[i] <= 0) {
            mTimers[i] = 0;
        }
    }
    
    cXyz cStack_ac;
    cXyz cStack_b8(0.0f, 1.0f, 0.0f);
    dBgS_LinChk linChk;
    daPy_py_c* playerActor = daPy_getPlayerActorClass();
    linChk.SetObj();
    cXyz local_c4;
    cXyz cStack_d0;
    
    switch (mSubAction) {
    case 0:
        mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kuw", 6), 2, 5.0f, 0.0f, 0.0f, -1.0f);
        mSubAction++;
        mTimers[0] = 20;
        break;
    case 1:
        if (mTimers[0] == 10) {
            mpMorf->setPlaySpeed(1.0f);
        }
        if (mTimers[0] == 0) {
            mSubAction++;
            mDoMtx_stack_c::ZXYrotS(field_0x99a);
            mDoMtx_stack_c::multVec(&cStack_b8, &cStack_b8);
            mTimers[0] = cM_rndF(30.0f) + 10.0f;
            
            if (field_0x9c0 == 0) {
                mTimers[1] = cM_rndF(80.0f) + 80.0f;
            } else {
                mTimers[1] = cM_rndF(50.0f) + 50.0f;
            }
            
            mSpeedFTarget = cM_rndF(5.0f) + 8.0f;
            mAngleTarget = field_0x9a2;
            mSpeedYTarget = 3.0f;
            speed.y = 5.0f;
        }
        break;        
    case 2:
        cXyz* flamePos = playerActor->getKandelaarFlamePos();
        if (flamePos != NULL) {
            mAngleTarget = cLib_targetAngleY(&current.pos, flamePos);
            
            if (flamePos->abs(current.pos) < 200.0f && speed.y < 0.0f) {
                cLib_addCalcAngleS2(&current.angle.y, mAngleTarget, 2, 0x500);
            } else {
                cLib_addCalcAngleS2(&current.angle.y, mAngleTarget, 0x10, 0x200);
            }
            
            if (flamePos->abs(current.pos) < 11.0f && fopAcM_GetSpeedF(playerActor) < 1.0f) {
                local_c4.set(0.0f, 0.0f, 10.0f);
                if (field_0x9c2 == 0) {
                    field_0x9a2 = cLib_targetAngleY(&current.pos, flamePos);
                }

                cLib_offsetPos(&cStack_d0, flamePos, field_0x9a2, &local_c4);
                current.pos = cStack_d0;
                shape_angle.x = 0x4000;
                shape_angle.y = field_0x9a2;
                speedF = 0.0f;
                mSpeedFTarget = 0.0f;
                speed.y = 0.0f;
                mSpeedYTarget = 0.0f;
                mpMorf->setPlaySpeed(0.0f);
                mAngleTarget = 0;
                current.angle.y = field_0x9a2;
                field_0x9c2 = 1;
                mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kuw", 7), 2, 5.0f, 1.0f, 0.0f, -1.0f);
            } else {
                if (field_0x9c2 != 0) {
                    field_0x9c2 = 0;
                    mSpeedYTarget = 3.0f;
                    speed.y = 3.0f;
                    speedF = 8.0f;
                    mSpeedFTarget = 8.0f;
                    mpMorf->setPlaySpeed(1.0f);
                    field_0x9a2 = 0;
                    mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kuw", 6), 2, 5.0f, 1.0f, 0.0f, -1.0f);
                }
            }
        }
        
        if (mTimers[0] == 0) {
            if (flamePos == NULL) {
                local_c4 = home.pos - current.pos;
                local_c4.x += cM_rndFX(1050.0f);
                local_c4.z += cM_rndFX(1050.0f);
                mAngleTarget = cM_atan2s(local_c4.x, local_c4.z);
            }
            mTimers[0] = cM_rndF(20.0f) + 20.0f;
            mSpeedFTarget = cM_rndF(5.0f) + 8.0f;
        }
        
        if (mTimers[1] == 0) {
            mSpeedYTarget = -cM_rndF(3.0f);
            mTimers[1] = cM_rndF(20.0f) + 20.0f;
        }
        
        mpMorf->getFrame();
        SpeedSet();
        
        if (field_0x9c0 == 0) {
            cLib_addCalcAngleS2(&field_0x99a.x, 0, 0x10, 0x1000);
            cLib_addCalcAngleS2(&field_0x99a.z, 0, 0x10, 0x1000);
        } else {
            cLib_addCalcAngleS2(&field_0x99a.x, 0, 0x10, 0x100);
            cLib_addCalcAngleS2(&field_0x99a.z, 0, 0x10, 0x100);
        }
        
        if (WallCheck()) {
            mAction = ACTION_WAIT;
            mSubAction = 0;
            speedF = 0.0f;
            speed.y = 0.0f;
            shape_angle.x = 0;
            shape_angle.z = 0;
        }
        
        if (field_0x9c0 == 1) {
            cLib_addCalcAngleS2(&current.angle.y, mAngleTarget, 0x10, 0x100);
        }
        
        if (field_0x9c2 == 0) {
            cLib_chaseAngleS(&shape_angle.x, 0, 0x400);
        }
        break;
    }
    
    if (mpMorf->getPlaySpeed() == 1.0f) {
        mSound.startCreatureSoundLevel(Z2SE_INSCT_BTBT, 0, -1);
    }
}

void daObjKUW_c::Action() {
    switch (mAction) {
    case ACTION_WAIT:
        WaitAction();
        break;
    case ACTION_WALK:
        WalkAction();
        mSound.startCreatureSoundLevel(Z2SE_INSCT_KSKS, 0, -1);
        break;
    case ACTION_MOVE:
        MoveAction();
        break;
    }
    Insect_GetDemoMain();
    mSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
}

void daObjKUW_c::ShopAction() {
    switch (mAction) {
    case ACTION_WAIT:
        ShopWaitAction();
        break;
    case ACTION_WALK:
        WalkAction();
        mSound.startCreatureSoundLevel(Z2SE_INSCT_KSKS, 0, -1);
        break;
    case ACTION_MOVE:
        MoveAction();
        break;
    }
    mSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
}

void daObjKUW_c::Insect_Release() {
    field_0x56c = 1;
    mSubAction = 0;
    mAction = ACTION_MOVE;
}

static u8 const l_kuw_itemno[2] = {
    fpcNm_ITEM_M_STAG_BEETLE, fpcNm_ITEM_F_STAG_BEETLE,
};

static f32 dummyFloat() { return -9.0f; }

void daObjKUW_c::Z_BufferChk() {
    cXyz local_5c;
    cXyz cStack_68;
    
    cStack_68 = current.pos;
    cStack_68.y += 20.0f;
    mDoLib_project(&cStack_68, &local_5c);
    
    camera_class* cc = dComIfGp_getCamera(0);
    f32 trimHeight;
    if (cc != NULL) {
        trimHeight = cc->mCamera.TrimHeight();
    } else {
        trimHeight = 0.0f;
    }
    
    if (local_5c.x > 0.0f && local_5c.x < FB_WIDTH && 
        local_5c.y > trimHeight && local_5c.y < FB_HEIGHT - trimHeight) {
        dComIfGd_peekZ(local_5c.x, local_5c.y, &field_0x9bc);
    }
    
    view_class* view = dComIfGd_getView();
    f32 nearPlane = view->near;
    f32 farPlane = view->far;
    
    mDoLib_pos2camera(&cStack_68, &local_5c);
    local_5c.z += 50.0f;
    if (local_5c.z > 0.0f) {
        local_5c.z = 0.0f;
    }
    
    field_0x9b8 = ((nearPlane + ((farPlane * nearPlane) / local_5c.z)) / (farPlane - nearPlane) + 1.0f) * 16777215.0f;
}

void daObjKUW_c::ParticleSet() {
    if (field_0x9b8 > (f32)field_0x9bc) {
        cLib_addCalc2(&mParticleScale, 0.0f, 1.0f, 1.0f);
    } else {
        cLib_addCalc2(&mParticleScale, mParticleScaleTarget, 1.0f, 1.0f);
    }

    mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0xa1b, &current.pos, &tevStr, &shape_angle,
                                          0, 0xff, 0, -1, 0, 0, 0);

    f32 emitterScale = mParticleScale;
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey1);
    if (emitter != NULL) {
        emitter->setGlobalParticleScale(emitterScale, emitterScale);
    }
}

void daObjKUW_c::BoomChk() {
    if (field_0x9c1 != 0) {
        daPy_py_c* playerActor = daPy_getPlayerActorClass();
        cXyz* playerPos = fopAcM_GetPosition_p(playerActor);
        home.pos = current.pos;
        cXyz targetPos = *playerPos;
        targetPos.y += 100.0f;
        
        if (mBoomerangMove.posMove(&current.pos, &shape_angle.y, NULL, 0x1c00)) {
            mBoomerangMove.bgCheckAfterOffset(&current.pos);
            current.pos.y += 0.5f;
        } else {
            dBgS_LinChk linChk;
            linChk.SetObj();
            linChk.Set(&targetPos, &current.pos, NULL);
            
            if (dComIfG_Bgsp().LineCross(&linChk)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(linChk, &plane);
                current.pos = old.pos = linChk.GetCross();
                mAction = ACTION_WAIT;
                mSubAction = 0;
                cXyz* normal = plane.GetNP();
                current.pos = linChk.GetCross();
                cXyz temp1(0.0f, 0.0f, 0.0f);
                cXyz temp2(0.0f, normal->y, normal->z);
                field_0x99a.z = -cM_atan2s(normal->x, temp1.abs(temp2));
                field_0x99a.x = cM_atan2s(normal->z, normal->y);
                field_0x9a2 = cM_atan2s(normal->x, normal->z);
            } else {
                mAction = ACTION_MOVE;
                mSubAction = 2;
                J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes("I_Kuw", 6);
                mpMorf->setAnm(anm, 2, 5.0f, 1.0f, 0.0f, -1.0f);
                speedF = 8.0f;
                mSpeedFTarget = cM_rndF(5.0f) + 8.0f;
                home.pos = current.pos;
                mSpeedYTarget = 5.0f;
                speed.y = 5.0f;
                mTimers[0] = 50;
                current.pos.y += 50.0f;
                mTimers[1] = cM_rndF(5.0f) + 5.0f;
                current.pos.y = old.pos.y = playerPos->y + 100.0f;
            }

            field_0x9c1 = 0;
        }
    }
}

void daObjKUW_c::ObjHit() {
    if (mSph.ChkTgHit()) {
        cCcD_ObjHitInf* hitObj = mSph.GetTgHitObj();
        if (hitObj->ChkAtType(AT_TYPE_BOOMERANG)) {
            if (field_0x9c1 == 0) {
                field_0x9c1 = 1;
                mBoomerangMove.initOffset(&current.pos, mSph.GetTgHitGObj());
            }
        } else if (!hitObj->ChkAtType(AT_TYPE_NORMAL_SWORD) && hitObj->ChkAtType(AT_TYPE_HOOKSHOT)) {
            cXyz catchPos(0.0f, 0.0f, 0.0f);
            daPy_py_c* playerActor = daPy_getPlayerActorClass();
            u32 actorId = fopAcM_GetID(this);
            playerActor->setHookshotCarryOffset(actorId, &catchPos);
            mAction = ACTION_MOVE;
            mSubAction = 2;
            field_0x9c1 = 0;
            mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kuw", 6), 2, 5.0f, 1.0f, 0.0f, -1.0f);
            speedF = 8.0f;
            mSpeedFTarget = cM_rndF(5.0f) + 8.0f;
            mTimers[1] = cM_rndF(5.0f) + 5.0f;
        }

        mSph.ClrTgHit();
    }
}

int daObjKUW_c::Execute() {
    if (ChkGetDemo()) {
        field_0x9b8 = field_0x9bc + 10000.0f;
        mSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
        Insect_GetDemoMain();
        ParticleSet();
        return 1;
    }

    daPy_getPlayerActorClass();
    attention_info.position = current.pos;
    eyePos = current.pos;
    eyePos.y += 10.0f;
    
    switch (field_0x9c0) {
    case 0:
        if (!fopAcM_checkHookCarryNow(this)) {
            Action();
        } else {
            home.pos = current.pos;
        }
        
        mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xa1c, &current.pos, &tevStr, &shape_angle, 0, 0xff, 0, -1, 0, 0, 0);
        SetCcSph();
        ObjHit();
        BoomChk();
        eyePos = current.pos;
        eyePos.y += 10.0f;
        attention_info.position = eyePos;
        attention_info.position.y += 10.0f;
        ParticleSet();
        break;
    case 1:
        ShopAction();
        break;
    }
    
    if (mAction == ACTION_MOVE && mSubAction == 2 && speed.y < 0.0f) {
        mAcch.CrrPos(dComIfG_Bgsp());
    }
    
    mpBrkAnm->play();
    mpBtkAnm->play();
    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    setBaseMtx();
    return 1;
}

static bool l_HIOInit;

static daObj_KuwHIO_c l_HIO;

int daObjKUW_c::Delete() {
    dComIfG_resDelete(&mPhase, "I_Kuw");
    if (mIsHIOOwner) {
        l_HIOInit = false;
        mDoHIO_DELETE_CHILD(l_HIO.mId);
    }

    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }
    return 1;
}

void daObjKUW_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x99a);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpMorf->getModel()->setBaseTRMtx( mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

int daObjKUW_c::Draw() {
    if (mDraw) {
        Z_BufferChk();
        J3DModel* model = mpMorf->getModel();
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);
        mpBtkAnm->entry(model->getModelData());
        mpBrkAnm->entry(model->getModelData());
        mpMorf->entryDL();
        if (field_0x9c0 == 0) {
            dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), 15.0f, mAcch.m_gnd, 0, -0.6f, dDlst_shadowControl_c::getSimpleTex());
        }
    }
    return 1;
}

static int daObjKUW_Draw(daObjKUW_c* i_this) {
    return i_this->Draw();
}

static int daObjKUW_Execute(daObjKUW_c* i_this) {
    return i_this->Execute();
}

static u16 const l_musiya_num[2] = { 0x195, 0x196 };

bool daObjKUW_c::CreateChk() {
    u8 type = (fopAcM_GetParam(this) & 0xf00) >> 8;
    if (type == 0xf) {
        type = 0;
    }
    
    if (field_0x9c0 != 2) {
        if (type != 0) {
            if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[0]]) || !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[1]])) {
                return false;
            }
        } else {
            if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 && dComIfGp_getStartStageRoomNo() == 3) {
                if (dMenu_Insect_c::isCatchNotGiveInsect(l_kuw_itemno[mSex])) {
                    return false;
                }

                if ((mSex == SEX_MALE && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]])) ||
                    (mSex == SEX_FEMALE && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]]))) {
                    return false;
                }
            }
        }
    }
    return true;
}

int daObjKUW_c::create() {
    fopAcM_ct(this, daObjKUW_c);
    int rv = dComIfG_resLoad(&mPhase, "I_Kuw");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("KUW PARAM %x\n", fopAcM_GetParam(this));
        field_0x9c0 = fopAcM_GetParam(this) & 0xf;
        if (field_0x9c0 == 2) {
            field_0x56c = 0;
            shape_angle.x -= 0x2000;
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        } else {
            mDraw = 1;
        }

        attention_info.distances[fopAc_attn_CARRY_e] = 93;
        mSex = (fopAcM_GetParam(this) & 0x10) >> 4;
        setItemNo(l_kuw_itemno[mSex]);
        setSaveBitNo(l_musiya_num[mSex]);
        if (mSex == SEX_FEMALE) {
            scale.set(l_HIO.mFemaleModelScale, l_HIO.mFemaleModelScale, l_HIO.mFemaleModelScale);
            mParticleScaleTarget = 1.0f;
        } else if (mSex == SEX_MALE) {
            scale.set(l_HIO.mMaleModelScale, l_HIO.mMaleModelScale, l_HIO.mMaleModelScale);
            mParticleScaleTarget = 1.1f;
        }

        if (!CreateChk()) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xb50)) {
            return cPhs_ERROR_e;
        }

        if (!l_HIOInit) {
            l_HIOInit = true;
            mIsHIOOwner = true;
            l_HIO.mId = mDoHIO_CREATE_CHILD("黄金蟲(クワガタ)", &l_HIO);
        }

        mAction = ACTION_WAIT;
        mSubAction = 1;
        gravity = 0.0f;
        bool bVar4 = false;
        cXyz cStack_ec(0.0f, 0.0f, 500.0f);
        cXyz cStack_f8(0.0f, 0.0f, -200.0f);
        cXyz cStack_104;
        cXyz cStack_110;
        dBgS_LinChk linChk;
        linChk.SetObj();

        for (int i = 0; i < 4; i++) {
            cLib_offsetPos(&cStack_104, &current.pos, shape_angle.y + (i << 14), &cStack_ec);
            cLib_offsetPos(&cStack_110, &current.pos, shape_angle.y + (i << 14), &cStack_f8);
            linChk.Set(&cStack_110, &cStack_104, NULL);
            linChk.SetObj();
            if (dComIfG_Bgsp().LineCross(&linChk)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(linChk, &plane);
                cXyz* normal = plane.GetNP();
                current.pos = linChk.GetCross();
                cXyz cStack_11c(0.0f, 0.0f, 0.0f);
                cXyz cStack_128(0.0f, normal->y, normal->z);
                field_0x99a.z = -cM_atan2s(normal->x, cStack_11c.abs(cStack_128));
                field_0x99a.x = cM_atan2s(normal->z, normal->y);
                field_0x9a2 = cM_atan2s(normal->x, normal->z);
                bVar4 = true;
                break;
            }
        }

        if (!bVar4) {
            cStack_ec.set(0.0f, 500.0f, 0.0f);
            cStack_104.set(0.0f, -1000.0f, 0.0f);
            linChk.Set(&cStack_ec, &cStack_104, NULL);
            linChk.SetObj();
            if (dComIfG_Bgsp().LineCross(&linChk)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(linChk, &plane);
                cXyz* normal = plane.GetNP();
                current.pos = linChk.GetCross();
                cXyz cStack_134(0.0f, 0.0f, 0.0f);
                cXyz cStack_140(0.0f, normal->y, normal->z);
                field_0x99a.z = -cM_atan2s(normal->x, cStack_134.abs(cStack_140));
                field_0x99a.x = cM_atan2s(normal->z, normal->y);
                field_0x9a2 = cM_atan2s(normal->x, normal->z);
            }
        }

        if (field_0x9c0 == 0) {
            InitCcSph();
        }

        if (field_0x9c0 == 2) {
            if (strcmp("R_SP160", dComIfGp_getStartStageName()) != 0 ||
                dComIfGp_getStartStageRoomNo() != 3)
            {
                InitCcSph();
            }
        }

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        daObjKUW_Execute(this);
    }
    return rv;
}

static int daObjKUW_IsDelete(daObjKUW_c* param_0) {
    return 1;
}

static actor_method_class l_daObjKUW_Method = {
    (process_method_func)daObjKUW_Create,
    (process_method_func)daObjKUW_Delete,
    (process_method_func)daObjKUW_Execute,
    (process_method_func)daObjKUW_IsDelete,
    (process_method_func)daObjKUW_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Kuw = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Kuw,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjKUW_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  479,                    // mPriority
  &l_daObjKUW_Method,     // sub_method
  0x000C0100,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
