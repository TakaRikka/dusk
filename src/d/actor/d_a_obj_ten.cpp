/**
 * @file d_a_obj_ten.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ten.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_camera_mng.h"
#include "d/d_cc_d.h"
#include "SSystem/SComponent/c_lib.h"
#include "m_Do/m_Do_lib.h"
#include "d/d_menu_insect.h"

daObj_TenHIO_c::daObj_TenHIO_c() {
    field_0x4 = -1;
    field_0xc = 1.0f;
    field_0x8 = 1.0f;
}

static u8 const l_ten_itemno[2] = {
    0xCE,
    0xCF,
};

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

void daObjTEN_c::InitCcSph() {
    mStts.Init(1, 0, this);
    mSph.Set(ccSphSrc);
    mSph.SetStts(&mStts);
    mSph.SetC(current.pos);
    mSph.OnTgNoHitMark();
}

void daObjTEN_c::SetCcSph() {
    mSph.SetC(current.pos);
    mSph.SetR(20.0f);
    dComIfG_Ccsp()->Set(&mSph);
}

static int useHeapInit(fopAc_ac_c* actor) {
    daObjTEN_c* i_this = static_cast<daObjTEN_c*>(actor);
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("I_Ten", 10);
    JUT_ASSERT(246, modelData != NULL);
    i_this->mMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, 2, 1.0f, 0, -1,
                                         &i_this->mCreature, 0, 0x11000284);
    if (i_this->mMorf == NULL || i_this->mMorf->getModel() == NULL) {
        return 0;
    }
    J3DModel* model = i_this->mMorf->getModel();
    i_this->mBrk = new mDoExt_brkAnm();
    if (i_this->mBrk == NULL) {
        return 0;
    }
    if (i_this->mSex == 1) {
        J3DAnmTevRegKey* anmKey = (J3DAnmTevRegKey*)dComIfG_getObjectRes("I_Ten", 13);
        if (i_this->mBrk->init(model->getModelData(), anmKey, 1, 2, 1.0f, 0, -1) == 0) {
            return 0;
        }
    }
    else {
        J3DAnmTevRegKey* anmKey = (J3DAnmTevRegKey*)dComIfG_getObjectRes("I_Ten", 14);
        if (i_this->mBrk->init(model->getModelData(), anmKey, 1, 2, 1.0f, 0, -1) == 0) {
            return 0;
        }
    }
    i_this->mBtk = new mDoExt_btkAnm();
    if (i_this->mBtk == NULL) {
        return 0;
    }
    if (i_this->mSex == 1) {
        J3DAnmTextureSRTKey* anmKey = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("I_Ten", 17);
        if (i_this->mBtk->init(model->getModelData(), anmKey, 1, 2, 1.0f, 0, -1) == 0) {
            return 0;
        }
    } else {
        J3DAnmTextureSRTKey* anmKey = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("I_Ten", 18);
        if (i_this->mBtk->init(model->getModelData(), anmKey, 1, 2, 1.0f, 0, -1) == 0) {
            return 0;
        }
    }
    return 1;
}

static int daObjTEN_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjTEN_c*>(i_this)->create();
}

static int daObjTEN_Delete(daObjTEN_c* i_this) {
    static_cast<daObjTEN_c*>(i_this)->Delete();
    return 1;
}

void daObjTEN_c::WaitAction() {
    switch (field_0x5e9) {
    case 0: {
        speedF = 0.0f;
        J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("I_Ten", 7);
        mMorf->setAnm(anmTransform, 2, 5.0f, 0.0f, 0.0f, -1.0f);
        field_0x5e9++;
        field_0x5ea[0] = cM_rndF(50.0f) + 50.0f;
        break;
    }
    case 1:
        if (fopAcM_searchPlayerDistance(this) < 1500.0f) {
            field_0x5ea[0]--;
            if (field_0x5ea[0] <= 0) {
                field_0x5ea[0]= 0;
            }
            if (field_0x5ea[0] == 0) {
                int rnd = cM_rndF(100.0f);
                if (rnd < 40) {
                    mAction = ACTION_WALK;
                    field_0x5e9 = 0;
                } else if (rnd < 70) {
                    mAction = ACTION_MOVE;
                    field_0x5e9 = 0;
                } else {
                    field_0x5ea[0] = cM_rndF(50.0f) + 50.0f;
                }
            }
        }
        break;
    }
}

BOOL daObjTEN_c::WallCheck() {
    dBgS_LinChk linChk;
    linChk.SetObj();
    linChk.Set(&old.pos, &current.pos, NULL);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        cM3dGPla acStack_b0;
        dComIfG_Bgsp().GetTriPla(linChk, &acStack_b0);
        cXyz* pfVar4 = acStack_b0.GetNP();
        current.pos = linChk.GetCross();
        cXyz cStack_bc(0.0f, 0.0f, 0.0f);
        cXyz cStack_c8(0.0f, pfVar4->y, pfVar4->z);
        field_0x602.z = -cM_atan2s(pfVar4->x, cStack_bc.abs(cStack_c8));
        field_0x602.x = cM_atan2s(pfVar4->z, pfVar4->y);
        field_0x60a = cM_atan2s(pfVar4->x, pfVar4->z);
        return 1;
    }
    return 0;
}

void daObjTEN_c::SpeedSet() {
    speed.y += gravity;
    cXyz speedAmp(0.0f, speed.y, speedF);
    cXyz speedRes(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotS(field_0x602);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&speedAmp, &speedRes);
    current.pos.x += speedRes.x;
    current.pos.y += speedRes.y;
    current.pos.z += speedRes.z;
    cLib_addCalc2(&speedF, field_0x5f0, 0.4f, 10.0f);
    cLib_addCalc2(&speed.y, field_0x5f4, 0.1f, 0.5f);
    cLib_addCalcAngleS2(&current.angle.y, field_0x5f8, 0x10, 0x100);
    shape_angle.y = current.angle.y;
}

void daObjTEN_c::WallWalk() {
    cXyz cStack_138(0.0f, -100.0f, 3.0f * speedF);
    cXyz cStack_144(0.0f, 40.0f, 0.0f);
    cXyz cStack_150;
    dBgS_LinChk adStack_a8;
    dBgS_LinChk adStack_118;
    adStack_a8.SetObj();
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x602);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&cStack_138, &cStack_138);
    mDoMtx_stack_c::multVec(&cStack_144, &cStack_144);
    adStack_a8.Set(&cStack_144, &cStack_138, NULL);
    if (dComIfG_Bgsp().LineCross(&adStack_a8) != 0) {
        speedF = 0.3f;
        cM3dGPla acStack_12c;
        dComIfG_Bgsp().GetTriPla(adStack_a8, &acStack_12c);
        cXyz* pfVar5 = acStack_12c.GetNP();
        s16 sVar7 = cM_atan2s(pfVar5->x, pfVar5->z);
        cXyz cStack_15c(0.0f, 0.0f, 0.0f);
        cXyz cStack_168(0.0f, pfVar5->y, pfVar5->z);
        f32 uVar11 = cStack_15c.abs(cStack_168);
        cStack_150 = adStack_a8.GetCross();
        if (0x1000 > sVar7 - field_0x60a && sVar7 - field_0x60a > -0x1000 &&
            cStack_150.abs(current.pos) < speedF * 3.0f)
        {
            current.pos = cStack_150;
            field_0x602.z = -cM_atan2s(pfVar5->x, uVar11);
            field_0x602.x = cM_atan2s(pfVar5->z, pfVar5->y);
            field_0x60a = sVar7;
        } else {
            field_0x5f8 += 0x100;
        }
    } else {
        field_0x5f8 += 0x100;
    }
    cLib_addCalcAngleS2(&current.angle.y, field_0x5f8, 0x10, 0x50);
    shape_angle.y = current.angle.y;
}

void daObjTEN_c::WalkAction() {
    for (int i = 0; i < 3; i++) {
        field_0x5ea[i]--;
        if (field_0x5ea[i] <= 0) {
            field_0x5ea[i] = 0;
        }
    }
    J3DAnmTransform* anmTransform;
    switch (field_0x5e9) {
    case 0:
        anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("I_Ten", 7);
        mMorf->setAnm(anmTransform, 2, 5.0f, 1.0f, 0.0f, -1.0f);
        field_0x5f0 = 0.5f;
        speed.y = 0.0f;
        field_0x5f4 = 0;
        field_0x5e9++;
        field_0x5ea[1] = cM_rndF(50.0f) + 50.0f;
        break;
    case 1:
        if (field_0x5ea[0] == 0) {
            cXyz local_48(cM_rndFX(100.0f), 0.0f, cM_rndFX(100.0f));
            field_0x5f8 = cM_atan2s(local_48.x, local_48.z);
            field_0x5ea[0] = cM_rndF(50.0f) + 50.0f;
        }
        if (field_0x5ea[1] == 0) {
            int rnd = cM_rndF(100.0f);
            if (rnd < 30) {
                mAction = ACTION_MOVE;
                field_0x5e9 = 0;
            } else if (rnd < 60) {
                mAction = ACTION_WAIT;
                field_0x5e9 = 0;
            } else {
                field_0x5ea[1] = cM_rndF(50.0f) + 50.0f;
            }
        }
        break;
    }
    WallWalk();
}

void daObjTEN_c::MoveAction() {
    for (int i = 0; i < 3; i++) {
        field_0x5ea[i]--;
        if (field_0x5ea[i] <= 0) {
            field_0x5ea[i] = 0;
        }
    }
    cXyz cStack_b0;
    cXyz cStack_bc(0.0f, 1.0f, 0.0f);
    dBgS_LinChk linkchk;
    daPy_py_c* player = daPy_getPlayerActorClass();
    linkchk.SetObj();
    cXyz cStack_c8;
    cXyz cStack_d4;
    
    switch (field_0x5e9) {
    case 0: 
        {
            J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("I_Ten", 6);
            mMorf->setAnm(anmTransform, 2, 5.0f, 0.0f, 0.0f, -1.0f);
            field_0x5e9++;
            field_0x5ea[0] = 20;
        }
        break;
    case 1:
        if (field_0x5ea[0] == 10) {
            mMorf->setPlaySpeed(1.0f);
        }
        if (field_0x5ea[0] == 0) {
            field_0x5e9++;
            mDoMtx_stack_c::ZXYrotS(field_0x602);
            mDoMtx_stack_c::multVec(&cStack_bc, &cStack_bc);
            field_0x5ea[0] = cM_rndF(10.0f) + 30.0f;
            if (field_0x624 == 0) {
                field_0x5ea[1] = cM_rndF(80.0f) + 80.0f;
            } else {
                field_0x5ea[1] = cM_rndF(40.0f) + 40.0f;
            }
            field_0x5f0 = cM_rndF(5.0f) + 8.0f;
            field_0x5f8 = field_0x60a;
            field_0x5f4 = 3.0f;
            speed.y = 5.0f;
        }
        break;
    case 2: 
        {
            cXyz local_e0;
            cXyz* flamePos = player->getKandelaarFlamePos();
            if (flamePos != NULL) {
                field_0x5f8 = cLib_targetAngleY(&current.pos, flamePos);
                if ((flamePos->abs(current.pos) < 200.0f) && speed.y < 0.0f) {
                    cLib_addCalcAngleS2(&current.angle.y, field_0x5f8, 2,
                                                    0x500);
                } else {
                    cLib_addCalcAngleS2(&current.angle.y, field_0x5f8, 0x10,
                                                    0x200);
                }
                if (flamePos->abs(current.pos) < 11.0f &&
                    fopAcM_GetSpeedF(player) < 1.0f)
                {
                    local_e0.set(0.0f, 0.0f, 10.0f);
                    if (field_0x60d == 0) {
                        field_0x60a = cLib_targetAngleY(&current.pos, flamePos);
                    }
                    cLib_offsetPos(&cStack_d4, flamePos, field_0x60a, &local_e0);
                    current.pos = cStack_d4;
                    shape_angle.x = 0x4000;
                    shape_angle.y = field_0x60a;
                    speedF = 0.0f;
                    field_0x5f0 = 0;
                    speed.y = 0;
                    field_0x5f4 = 0;
                    mMorf->setPlaySpeed(0.0f);
                    field_0x5f8 = 0;
                    current.angle.y = field_0x60a;
                    field_0x60d = 1;
                    J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("I_Ten", 7);
                    mMorf->setAnm(anmTransform, 2, 5.0f, 0.0f, 0.0f, -1.0f);
                } else {
                    if (field_0x60d != 0) {
                        field_0x60d = 0;
                        field_0x5f4 = 3.0f;
                        speed.y = 3.0f;
                        speedF = 8.0f;
                        field_0x5f0 = 8.0f;
                        mMorf->setPlaySpeed(1.0f);
                        field_0x60a = 0;
                        J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("I_Ten", 6);
                        mMorf->setAnm(anmTransform, 2, 5.0f, 0.0f, 0.0f, -1.0f);
                    }
                }
            }
            if (field_0x5ea[0] == 0) {
                if (flamePos == NULL) {
                    if (field_0x624 == 0) {
                        local_e0 = home.pos - current.pos;
                        local_e0.x += cM_rndFX(1050.0f);
                        local_e0.z += cM_rndFX(1050.0f);
                        field_0x5f8 = cM_atan2s(local_e0.x, local_e0.z);
                    } else {
                        field_0x5f8 = cM_rndF(64000.0f);
                    }
                }
                field_0x5ea[0] = cM_rndF(20.0f) + 20.0f;
                field_0x5f0 = cM_rndF(5.0f) + 8.0f;
            }
            if (field_0x5ea[1] == 0) {
                field_0x5f4 = -cM_rndF(3.0f);
                field_0x5ea[1] = cM_rndF(20.0f) + 20.0f;
            }
            mMorf->getFrame();
            SpeedSet();
            if (field_0x624 == 0) {
                cLib_addCalcAngleS2(&field_0x602.x, 0, 0x10, 0x1000);
                cLib_addCalcAngleS2(&field_0x602.z, 0, 0x10, 0x1000);
            } else {
                cLib_addCalcAngleS2(&field_0x602.x, 0, 0x10, 0x100);
                cLib_addCalcAngleS2(&field_0x602.z, 0, 0x10, 0x100);
            }
            if (WallCheck()) {
                mAction = ACTION_WAIT;
                field_0x5e9 = 0;
                speedF = 0;
                speed.y = 0;
                shape_angle.x = 0;
                shape_angle.z = 0;
            }
            if (field_0x624 == 1) {
                cLib_addCalcAngleS2(&current.angle.y, field_0x5f8, 0x10,
                                                0x100);
            }
            if (field_0x60d == 0) {
                cLib_chaseAngleS(&shape_angle.x, 0, 0x400);
            }
        }
        break;
    }
    if (mMorf->getPlaySpeed() == 1.0f) {
        mCreature.startCreatureSoundLevel(Z2SE_INSCT_BTBT, 0, -1);
    }
}

void daObjTEN_c::Action() {
    switch (mAction) {
    case ACTION_WAIT:
        WaitAction();
        break;
    case ACTION_WALK:
        WalkAction();
        mCreature.startCreatureSoundLevel(Z2SE_INSCT_KSKS, 0, -1);
        break;
    case ACTION_MOVE:
        MoveAction();
        break;
    }
    Insect_GetDemoMain();
    mCreature.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
}

void daObjTEN_c::ShopAction() {
    switch (mAction) {
    case ACTION_WAIT:
        WaitAction();
        break;
    case ACTION_WALK:
        WalkAction();
        mCreature.startCreatureSoundLevel(Z2SE_INSCT_KSKS, 0, -1);
        break;
    case ACTION_MOVE:
        MoveAction();
        break;
    }
    mCreature.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
}

void daObjTEN_c::checkGroundPos() {
    cXyz acStack_28(current.pos);
    mGndChk.SetPos(&acStack_28);
    field_0x5e4 = dComIfG_Bgsp().GroundCross(&mGndChk);
}

void daObjTEN_c::Insect_Release() {
    field_0x56c = 1;
    field_0x624 = 2;
    mAction = ACTION_MOVE;
}

static f32 dummyLiteral() { return -9.0f; }

void daObjTEN_c::ParticleSet() {
    if (field_0x618 > field_0x61c) {
        cLib_addCalc2(&mParticleScale, 0.0f, 1.0f, 1.0f);
    } else {
        cLib_addCalc2(&mParticleScale, 1.0f, 1.0f, 1.0f);
    }
    mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0xa1b, &current.pos, &tevStr,
                                          &shape_angle, 0, 0xff, 0, -1, 0, 0, 0);
    f32 partScale = mParticleScale;
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey1);
    if (emitter != NULL) {
        emitter->setGlobalParticleScale(partScale, partScale);
    }
}

void daObjTEN_c::BoomChk() {
    if (field_0x7a8 != 0) {
        cXyz* playerPos = &fopAcM_GetPosition(daPy_getPlayerActorClass());
        home.pos = current.pos;
        cXyz cStack_cc(*playerPos);
        cStack_cc.y += 100.0f;
        if (mBoomerang.posMove(&current.pos, &shape_angle.y, NULL,
                                              0x1c00) != 0) {
            mBoomerang.bgCheckAfterOffset(&current.pos);
            current.pos.y += 0.5f;
        } else {
            dBgS_LinChk linChk;
            linChk.SetObj();
            linChk.Set(&cStack_cc, &current.pos, NULL);
            if (dComIfG_Bgsp().LineCross(&linChk)) {
                cM3dGPla acStack_c0;
                dComIfG_Bgsp().GetTriPla(linChk, &acStack_c0);
                current.pos = old.pos = linChk.GetCross();
                mAction = ACTION_WAIT;
                field_0x5e9 = 0;
                cXyz* pfVar7 = acStack_c0.GetNP();
                current.pos = linChk.GetCross();
                cXyz cStack_d8(0.0f, 0.0f, 0.0f);
                cXyz cStack_e4(0.0f, pfVar7->y, pfVar7->z);
                field_0x602.z = -cM_atan2s(pfVar7->x, cStack_d8.abs(cStack_e4));
                field_0x602.x = cM_atan2s(pfVar7->z, pfVar7->y);
                field_0x60a = cM_atan2s(pfVar7->x, pfVar7->z);
            } else {
                mAction = 1;
                field_0x5e9 = 2;
                J3DAnmTransform* anmTransform =
                    (J3DAnmTransform*)dComIfG_getObjectRes("I_Ten", 6);
                mMorf->setAnm(anmTransform, 2, 5.0f, 1.0f,
                                         0.0f, -1.0f);
                speedF = 8.0f;
                field_0x5f0 = cM_rndF(5.0f) + 8.0f;
                home.pos = current.pos;
                field_0x5f4 = 5.0f;
                speed.y = 5.0f;
                field_0x5ea[0] = 50;
                current.pos.y += 50.0f;
                field_0x5ea[1] = cM_rndF(5.0f) + 5.0f;
                current.pos.y = old.pos.y = playerPos->y + 100.0f;
            }
            field_0x7a8 = 0;
        }
    }
}

int daObjTEN_c::Execute() {
    if (ChkGetDemo()) {
        field_0x618 = field_0x61c + 10000.0f;
        mCreature.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
        Insect_GetDemoMain();
        ParticleSet();
        return 1;
    } else {
        daPy_getPlayerActorClass();
        checkGroundPos();
        attention_info.position = current.pos;
        eyePos = current.pos;
        eyePos.y += 10.0f;
        switch(field_0x624) {
        case 0:
            if (!fopAcM_checkHookCarryNow(this)) {
                Action();
            } else {
                home.pos = current.pos;
            }
            mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xa1c,
                                                      &current.pos, &tevStr, &shape_angle, 0,
                                                      0xff, 0, -1, 0, 0, 0);
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

        if (mAction == ACTION_MOVE && field_0x5e9 == 2 &&
            speed.y < 0.0f)
        {
            mAcch.CrrPos(dComIfG_Bgsp());
        }
        s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
        mMorf->play(0, reverb);
        mBtk->play();
        mBrk->play();
        setBaseMtx();
    }
    return 1;
}

void daObjTEN_c::ObjHit() {
    if (mSph.ChkTgHit()) {
        cCcD_Obj* hitObj = mSph.GetTgHitObj();
        if (hitObj->ChkAtType(AT_TYPE_BOOMERANG)) {
            if (field_0x7a8 == 0) {
                field_0x7a8 = 1;
                mSph.GetTgHitGObj();
                mBoomerang.initOffset(&current.pos);
            }
        } else {
            if (!hitObj->ChkAtType(AT_TYPE_NORMAL_SWORD) && hitObj->ChkAtType(AT_TYPE_HOOKSHOT)) {
                cXyz cStack_38(0.0f, 0.0f, 0.0f);
                fpc_ProcID id = fopAcM_GetID(this);
                daPy_getPlayerActorClass()->setHookshotCarryOffset(id, &cStack_38);
                mAction = ACTION_MOVE;
                field_0x5e9 = 2;
                field_0x7a8 = 0;
                J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("I_Ten", 6);
                mMorf->setAnm(anmTransform, 2, 5.0f, 1.0f, 0.0f, -1.0f);
                speedF = 8.0f;
                field_0x5f0 = cM_rndF(5.0f) + 8.0f;
                field_0x5ea[1] = cM_rndF(5.0f) + 5.0f;
            }
        }
        mSph.ClrTgHit();
    }
}

void daObjTEN_c::Z_BufferChk() {
    cXyz local_5c;
    cXyz cStack_68;
    cStack_68 = current.pos;
    cStack_68.y += 20.0f;
    mDoLib_project(&cStack_68, &local_5c);
    camera_class* camera = dComIfGp_getCamera(0);
    f32 trimHeight;
    if (camera != NULL) {
        trimHeight = camera->mCamera.TrimHeight();
    } else {
        trimHeight = 0.0f;
    }
    if (local_5c.x > 0.0f && local_5c.x < FB_WIDTH && local_5c.y > trimHeight &&
        local_5c.y < FB_HEIGHT - trimHeight)
    {
        dComIfGd_peekZ(local_5c.x, local_5c.y, &field_0x61c);
    }
    f32 near = dComIfGd_getView()->near;
    f32 far = dComIfGd_getView()->far;
    mDoLib_pos2camera(&cStack_68, &local_5c);
    local_5c.z += 50.0f;
    if (local_5c.z > 0.0f) {
        local_5c.z = 0.0f;
    }
    field_0x618 = ((near + ((far * near) / local_5c.z)) / (far - near) + 1.0f) * 16777215.0f;
}

static u8 hio_set;

int daObjTEN_c::Delete() {
    dComIfG_resDelete(&mPhase, "I_Ten");
    if (field_0xa64 != 0) {
        hio_set = 0;
    }
    mCreature.deleteObject();
    return 1;
}

void daObjTEN_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x602);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mMorf->modelCalc();
}

static int daObjTEN_Draw(daObjTEN_c* i_this) {
    if (i_this->mDraw) {
        i_this->Z_BufferChk();
        J3DModel* model = i_this->mMorf->getModel();
        g_env_light.settingTevStruct(0x10, &i_this->current.pos, &i_this->tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);
        i_this->mBtk->entry(model->getModelData());
        i_this->mBrk->entry(model->getModelData());
        i_this->mMorf->entryDL();
        if (i_this->field_0x624 == 0) {
            dComIfGd_setSimpleShadow(&i_this->current.pos, i_this->mAcch.GetGroundH(), 15.0f,
                                     i_this->mAcch.m_gnd, 0, -0.6f,
                                     dDlst_shadowControl_c::getSimpleTex());
        }
    }
    return 1;
}

static int daObjTEN_Execute(daObjTEN_c* i_this) {
    return i_this->Execute();
}

static u16 const l_musiya_num[2] = {
    0x19F, /* dSv_event_flag_c::F_0415 - Misc. - Ladybug (M) */
    0x1A0, /* dSv_event_flag_c::F_0416 - Misc. - Ladybug (F) */
};

bool daObjTEN_c::CreateChk() {
    u8 uVar1 = (fopAcM_GetParam(this) & 0xf00) >> 8;
    if (uVar1 == 15) {
        uVar1 = 0;
    }
    if (field_0x624 != 2) {
        if (uVar1 != 0) {
            if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[0]]) || !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[1]])) {
                return 0;
            }
        } else {
            if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 &&
                dComIfGp_getStartStageRoomNo() == 3)
            {
                if (dMenu_Insect_c::isCatchNotGiveInsect(l_ten_itemno[mSex])) {
                    return 0;
                }
                if ((mSex == 0 &&
                     !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]])) ||
                    (mSex == 1 &&
                     !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]])))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

static daObj_TenHIO_c l_HIO;

int daObjTEN_c::create() {
    fopAcM_ct(this, daObjTEN_c);
    int rv = dComIfG_resLoad(&mPhase, "I_Ten");
    if (rv == cPhs_COMPLEATE_e) {
        field_0x624 = fopAcM_GetParam(this) & 0xf;
        if (field_0x624 == 2) {
            field_0x56c = 0;
            shape_angle.x -= 0x2000;
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        } else {
            mDraw = true;
        }
        attention_info.distances[4] = 0x5d;
        mSex = (fopAcM_GetParam(this) & 0x10) >> 4;
        setItemNo(l_ten_itemno[mSex]);
        setSaveBitNo(l_musiya_num[mSex]);
        if (mSex == 1) {
            scale.set(l_HIO.field_0x8, l_HIO.field_0x8, l_HIO.field_0x8);
        } else if (mSex == 0) {
            scale.set(l_HIO.field_0xc, l_HIO.field_0xc, l_HIO.field_0xc);
        }
        if (CreateChk() == 0) {
            return cPhs_ERROR_e;
        }
        OS_REPORT("TEN PARAM %x\n", fopAcM_GetParam(this));
        if (fopAcM_entrySolidHeap(this, useHeapInit, 0xbc0) == 0) {
            return cPhs_ERROR_e;
        }
        if (hio_set == 0) {
            hio_set = 1;
            field_0xa64 = 1;
            l_HIO.field_0x4 = -1;
        }
        cXyz cStack_f0(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        if (fopAcM_gc_c::gndCheck(&cStack_f0)) {
            current.pos.y = fopAcM_gc_c::getGroundY();
        }
        gravity = 0.0f;
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                            &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        bool bVar3 = false;
        cXyz cStack_fc(0.0f, 0.0f, 500.0f);
        cXyz cStack_108(0.0f, 0.0f, -200.0f);
        cXyz cStack_114;
        cXyz cStack_120;
        dBgS_LinChk linChk;
        linChk.SetObj();
        for (int i = 0; i < 4; i++) {
            cLib_offsetPos(&cStack_114, &current.pos, shape_angle.y + (i << 14), &cStack_fc);
            cLib_offsetPos(&cStack_120, &current.pos, shape_angle.y + (i << 14), &cStack_108);
            linChk.Set(&cStack_120, &cStack_114, NULL);
            linChk.SetObj();
            if (dComIfG_Bgsp().LineCross(&linChk)) {
                cM3dGPla acStack_d0;
                dComIfG_Bgsp().GetTriPla(linChk, &acStack_d0);
                cXyz* pfVar13 = acStack_d0.GetNP();
                current.pos = linChk.GetCross();
                cXyz cStack_12c(0.0f, 0.0f, 0.0f);
                cXyz cStack_138(0.0f, pfVar13->y, pfVar13->z);
                field_0x602.z = -cM_atan2s(pfVar13->x, cStack_12c.abs(cStack_138));
                field_0x602.x = cM_atan2s(pfVar13->z, pfVar13->y);
                field_0x60a = cM_atan2s(pfVar13->x, pfVar13->z);
                bVar3 = true;
                break;
            }
        }
        if (!bVar3) {
            cStack_fc.set(0.0f, 500.0f, 0.0f);
            cStack_114.set(0.0f, -1000.0f, 0.0f);
            linChk.Set(&cStack_fc, &cStack_114, NULL);
            linChk.SetObj();
            if (dComIfG_Bgsp().LineCross(&linChk)) {
                cM3dGPla acStack_e4;
                dComIfG_Bgsp().GetTriPla(linChk, &acStack_e4);
                cXyz* pfVar13 = acStack_e4.GetNP();
                current.pos = linChk.GetCross();
                cXyz cStack_144(0.0f, 0.0f, 0.0f);
                cXyz cStack_150(0.0f, pfVar13->y, pfVar13->z);
                field_0x602.z = -cM_atan2s(pfVar13->x, cStack_144.abs(cStack_150));
                field_0x602.x = cM_atan2s(pfVar13->z, pfVar13->y);
                field_0x60a = cM_atan2s(pfVar13->x, pfVar13->z);
            }
        }
        if (field_0x624 == 0) {
            InitCcSph();
        }
        if (field_0x624 == 2) {
            if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 &&
                dComIfGp_getStartStageRoomNo() == 3)
            {
                field_0x624 = 3;
            }
        }
        fopAcM_SetMtx(this, mMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);
        mCreature.init(&current.pos, &eyePos, 3, 1);
        daObjTEN_Execute(this);
    }
    return rv;
}

static int daObjTEN_IsDelete(daObjTEN_c* param_0) {
    return 1;
}

static actor_method_class l_daObjTEN_Method = {
    (process_method_func)daObjTEN_Create,
    (process_method_func)daObjTEN_Delete,
    (process_method_func)daObjTEN_Execute,
    (process_method_func)daObjTEN_IsDelete,
    (process_method_func)daObjTEN_Draw,
};

actor_process_profile_definition g_profile_Obj_Ten = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Ten,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjTEN_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  483,                    // mPriority
  &l_daObjTEN_Method,     // sub_method
  0x000C0120,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
