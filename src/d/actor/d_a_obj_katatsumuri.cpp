/**
 * @file d_a_obj_katatsumuri.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_katatsumuri.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_menu_insect.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_lib.h"

static u8 hio_set;

static daObj_KatHIO_c l_HIO;

daObj_KatHIO_c::daObj_KatHIO_c() {
    field_0x4 = -1;
    mScaleMale = 1.0f;
    mScaleFemale = 1.0f;
}

static u8 const l_kat_itemno[2] = {
    fpcNm_ITEM_M_SNAIL,
    fpcNm_ITEM_F_SNAIL,
};

void daObjKAT_c::InitCcSph() {
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

void daObjKAT_c::SetCcSph() {
    mSph.SetC(current.pos);
    mSph.SetR(20.0f);
    dComIfG_Ccsp()->Set(&mSph);
}

int daObjKAT_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* joint = i_joint;
    s32 jointNo = joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jointNo));
    if (jointNo == 1) {
        mDoMtx_stack_c::YrotM(field_0x7f2 * (0.2f + nREG_F(1)) + yREG_F(1));
    }
    i_model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

static int JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daObjKAT_c* kat = (daObjKAT_c*)model->getUserArea();
        if (kat != NULL) {
            kat->ctrlJoint(i_joint, model);
        }
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daObjKAT_c* kat = (daObjKAT_c*)i_this;
    return kat->CreateHeap();
}

int daObjKAT_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Kat", 9);
    JUT_ASSERT(0x136, modelData != NULL);
    mpMorfSO =
        new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("Kat", 6),
                             2, 1.0f, 0, -1, &mCreatureSound, 0, 0x11000284);
    if (mpMorfSO == NULL || mpMorfSO->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorfSO->getModel();
    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }

    if (mSex == SEX_FEMALE) {
        if (mpBrkAnm->init(model->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("Kat", 12),
                           1, 2, 1.0f, 0, -1) == 0)
        {
            return 0;
        }
    } else {
        if (mpBrkAnm->init(model->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("Kat", 13),
                           1, 2, 1.0f, 0, -1) == 0)
        {
            return 0;
        }
    }

    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL) {
        return 0;
    }

    if (mSex == SEX_FEMALE) {
        if (mpBtkAnm->init(model->getModelData(),
                           (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Kat", 16), 1, 2, 1.0f, 0,
                           -1) == 0)
        {
            return 0;
        }
    } else {
        if (mpBtkAnm->init(model->getModelData(),
                           (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Kat", 17), 1, 2, 1.0f, 0,
                           -1) == 0)
        {
            return 0;
        }
    }

    return 1;
}

static int daObjKAT_Create(fopAc_ac_c* i_this) {
    daObjKAT_c* kat = (daObjKAT_c*)i_this;
    fpc_ProcID unused = fopAcM_GetID(i_this);
    return kat->create();
}

static int daObjKAT_Delete(daObjKAT_c* i_this) {
    fpc_ProcID unused = fopAcM_GetID(i_this);
    i_this->Delete();
    return 1;
}

f32 dummy0() {
    return 0.0f;
}

f32 dummy1() {
    return 0.4f;
}

f32 dummy2() {
    return 10.0f;
}

f32 dummy3() {
    return 0.02f;
}

void daObjKAT_c::WallWalk() {
    s16 unkShort1;
    float unkFloat1;

    cXyz unkXyz1(0.0f, -50.0f, (2.0f + TREG_F(2)) * speedF);
    cXyz unkXyz2(0.0f, 0.0f, 0.0f);
    cXyz unkXyz3(0.0f, 50.0f + yREG_F(5), 0.0f);
    cXyz unkXyz4(0.0f, 0.0f, 0.0f);
    cXyz unkXyz5(0.0f, 50.0f, speedF);
    cXyz unkXyz6(0.0f, 0.0f, 0.0f);
    cXyz unkXyz7(0.0f, -50.0f, 0.0f);
    cXyz unkXyz8(0.0f, 0.0f, 0.0f);
    cXyz unkXyz9;

    dBgS_LinChk unkLinChk1;
    dBgS_LinChk unkLinChk2;
    unkLinChk1.SetObj();
    unkLinChk2.SetObj();

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x7ea);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&unkXyz1, &unkXyz2);
    mDoMtx_stack_c::multVec(&unkXyz3, &unkXyz4);
    mDoMtx_stack_c::multVec(&unkXyz5, &unkXyz6);
    mDoMtx_stack_c::multVec(&unkXyz7, &unkXyz8);

    unkLinChk1.Set(&unkXyz4, &unkXyz2, NULL);
    unkLinChk2.Set(&unkXyz6, &unkXyz8, NULL);

    if (dComIfG_Bgsp().LineCross(&unkLinChk1) != 0) {
        speedF = 0.3f + TREG_F(1);
        cM3dGPla cStack_154;
        dComIfG_Bgsp().GetTriPla(unkLinChk1, &cStack_154);

        cXyz* unkXyzP1 = cStack_154.GetNP();
        unkShort1 = cM_atan2s(unkXyzP1->x, unkXyzP1->z);
        f32 unused = unkXyzP1->y;
        cXyz unkXyz10(0.0f, 0.0f, 0.0f);
        cXyz unkXyz11(0.0f, unkXyzP1->y, unkXyzP1->z);
        f32 unkFloat1 = unkXyz10.abs(unkXyz11);

        field_0x7c8 = unkLinChk1.GetCross();
        field_0x7f6 = unkShort1;

        field_0x7c2.z = -cM_atan2s(unkXyzP1->x, unkFloat1);
        field_0x7c2.x = cM_atan2s(unkXyzP1->z, unkXyzP1->y);
        field_0x7f6 = cM_atan2s(unkXyzP1->x, unkXyzP1->z);
    } else {
        if (dComIfG_Bgsp().LineCross(&unkLinChk2) != 0) {
            speedF = 0.3f + TREG_F(1);
            cM3dGPla cStack_168;
            dComIfG_Bgsp().GetTriPla(unkLinChk2, &cStack_168);

            cXyz* unkXyzP2 = cStack_168.GetNP();
            unkShort1 = cM_atan2s(unkXyzP2->x, unkXyzP2->z);
            f32 unused1 = unkXyzP2->y;
            cXyz unkXyz12(0.0f, 0.0f, 0.0f);
            cXyz unkXyz13(0.0f, unkXyzP2->y, unkXyzP2->z);
            f32 unkFloat2 = unkXyz12.abs(unkXyz13);

            field_0x7c8 = unkLinChk2.GetCross();
            field_0x7f6 = unkShort1;

            field_0x7c2.z = -cM_atan2s(unkXyzP2->x, unkFloat2);
            field_0x7c2.x = cM_atan2s(unkXyzP2->z, unkXyzP2->y);
            field_0x7f6 = cM_atan2s(unkXyzP2->x, unkXyzP2->z);
        } else {
            field_0x7e0 += (s16)0x100;
        }
    }

    cLib_chasePos(&current.pos, field_0x7c8, speedF);
    old.pos = current.pos;
    cLib_addCalcAngleS2(&field_0x7ea.x, field_0x7c2.x, 5, 0x200);
    cLib_addCalcAngleS2(&field_0x7ea.z, field_0x7c2.z, 5, 0x200);
    cLib_addCalcAngleS2(&current.angle.y, field_0x7e0, 0x10, 0x100);
    shape_angle.y = current.angle.y;
}

void daObjKAT_c::MoveAction() {
    if (fopAcM_searchPlayerDistance(this) < 1500.0f) {
        cXyz unkXyz1;
        cXyz unkXyz2;

        if (field_0x7d4 == 0) {
            unkXyz1 = home.pos - current.pos;
            unkXyz1.x += cM_rndFX(1050.0f);
            unkXyz1.z += cM_rndFX(1050.0f);
            field_0x7e0 = (s16)cM_atan2s(unkXyz1.x, unkXyz1.z);
            field_0x7d4 = cM_rndF(400.0f) + 400.0f;
        }

        if (mpMorfSO->getFrame() > 50.0f + XREG_F(0)) {
            field_0x7d8 = 0.1f + yREG_F(0);
            speedF = 0.1f + yREG_F(0);
            s16 unused1 = current.angle.y;
            cLib_addCalcAngleS2(&current.angle.y, field_0x7e0, 0x15, 0x50);
            cLib_addCalcAngleS2(&field_0x7f2, field_0x7e0 - current.angle.y, 8, 0x100);
            shape_angle.y = current.angle.y;
        } else {
            speedF = 0.0f;
            field_0x7d8 = 0;
        }

        WallWalk();
    }
}

void daObjKAT_c::Action() {
    field_0x7d4--;
    if (field_0x7d4 <= 0) {
        field_0x7d4 = 0;
    }
    MoveAction();
    Insect_GetDemoMain();
}

void daObjKAT_c::Insect_Release() {
    field_0x56c = 1;
    field_0x808 = 2;
    field_0x7c0 = 1;
}

void daObjKAT_c::checkGround() {
    dBgS_LinChk unkLinChk1;
    unkLinChk1.SetObj();
    unkLinChk1.Set(&old.pos, &current.pos, NULL);

    if (dComIfG_Bgsp().LineCross(&unkLinChk1) != 0) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(unkLinChk1, &plane);
        cXyz* pfVar3 = plane.GetNP();

        current.pos = unkLinChk1.GetCross();

        cXyz cStack_bc(0.0f, 0.0f, 0.0f);
        cXyz cStack_c8(0.0f, pfVar3->y, pfVar3->z);

        f32 temp = cStack_bc.abs(cStack_c8);
        field_0x7c2.z = field_0x7ea.z = -cM_atan2s(pfVar3->x, temp);
        field_0x7c2.x = field_0x7ea.x = cM_atan2s(pfVar3->z, pfVar3->y);
        field_0x7f6 = cM_atan2s(pfVar3->x, pfVar3->z);

        shape_angle.x = 0;

        field_0x7c1++;
    }
}

void daObjKAT_c::F_MoveAction() {
    switch (field_0x7c1) {
    case 0:
        if (!fopAcM_checkHookCarryNow(this)) {
            speed.y += -2.0f;
            current.pos.y = current.pos.y + speed.y;
            shape_angle.x = 0x4000;
            checkGround();
        }
        break;
    case 1:
        field_0x7c0 = 0;
        field_0x7c1 = 0;
        field_0x808 &= 0xf0;
        shape_angle.x = 0;
        break;
    }
}

void daObjKAT_c::FallAction() {
    switch (field_0x7c0) {
    case 0:
        break;
    case 1:
        F_MoveAction();
        break;
    }
}

void daObjKAT_c::ParticleSet() {
    if (field_0x800 > field_0x804) {
        cLib_addCalc2(&field_0x80c, 0.0f, yREG_F(11) + 1.0f, 1.0f);
    } else {
        cLib_addCalc2(&field_0x80c, 1.0f, yREG_F(11) + 1.0f, 1.0f);
    }

    mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0xa1b, &current.pos, &tevStr, &shape_angle,
                                          0, -1, 0, -1, 0, 0, 0);

    f32 fVar1 = field_0x80c;
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey1);
    if (emitter != NULL) {
        emitter->setGlobalParticleScale(fVar1, fVar1);
    }
}

void daObjKAT_c::BoomChk() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 unused1 = fopAcM_searchPlayerDistanceXZ(this);

    if (field_0x990 != 0) {
        cXyz* unkXyzP1 = &fopAcM_GetPosition(player);
        home.pos = current.pos;
        cXyz unkXyz1 = *unkXyzP1;
        unkXyz1.y += 100.0f;

        if (mBoomerangMove.posMove(&current.pos, &shape_angle.y, NULL, 0x1c00) != 0) {
            mBoomerangMove.bgCheckAfterOffset(&current.pos);
            current.pos.y += 0.5f + yREG_F(19);
        } else {
            dBgS_LinChk unkLinChk1;
            unkLinChk1.SetObj();
            unkLinChk1.Set(&unkXyz1, &current.pos, NULL);

            if (dComIfG_Bgsp().LineCross(&unkLinChk1) != 0) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(unkLinChk1, &plane);
                current.pos = old.pos = unkLinChk1.GetCross();
                field_0x7c0 = 0;
                field_0x7c1 = 0;

                cXyz* unkXyzP2 = plane.GetNP();
                cXyz unkXyz2(0.0f, 0.0f, 0.0f);
                cXyz unkXyz3(0.0f, unkXyzP2->y, unkXyzP2->z);

                f32 temp = unkXyz2.abs(unkXyz3);
                field_0x7c2.z = field_0x7ea.z = -cM_atan2s(unkXyzP2->x, temp);
                field_0x7c2.x = field_0x7ea.x = cM_atan2s(unkXyzP2->z, unkXyzP2->y);
                field_0x7f6 = cM_atan2s(unkXyzP2->x, unkXyzP2->z);
            } else {
                current.pos.y = old.pos.y = unkXyzP1->y + 150.0f;
                field_0x808 = 3;
                field_0x7c0 = 1;
                field_0x7c1 = 0;
            }
            field_0x990 = 0;
        }
    }
}

void daObjKAT_c::ObjHit() {
    if (mSph.ChkTgHit() != 0) {
        cCcD_ObjHitInf* tgHitObj = mSph.GetTgHitObj();

        if (tgHitObj->ChkAtType(AT_TYPE_BOOMERANG)) {
            if (field_0x990 == 0) {
                field_0x990 = 1;

                mBoomerangMove.initOffset(&current.pos, mSph.GetTgHitGObj());
            }
        } else {
            if (!tgHitObj->ChkAtType(AT_TYPE_NORMAL_SWORD) &&
                tgHitObj->ChkAtType(AT_TYPE_HOOKSHOT))
            {
                cXyz acStack_28(0.0f, 0.0f, 0.0f);
                fpc_ProcID uVar3 = fopAcM_GetID(this);

                daPy_getPlayerActorClass()->setHookshotCarryOffset(uVar3, &acStack_28);

                field_0x990 = 0;
                field_0x808 = 3;
                field_0x7c0 = 1;
                field_0x7c1 = 0;
            }
        }
        mSph.ClrTgHit();
    }
}

int daObjKAT_c::Execute() {
    if (ChkGetDemo() != 0) {
        field_0x800 = field_0x804 + 10000.0f;

        mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);

        Insect_GetDemoMain();
        ParticleSet();

        return 1;
    }

    daPy_py_c* unused1 = daPy_getPlayerActorClass();

    attention_info.position = current.pos;

    eyePos = current.pos;
    eyePos.y += 10.0f;

    cXyz* unkXyzP1;
    f32 unkFloat1;
    switch (field_0x808) {
    case 0:
        if (fopAcM_checkHookCarryNow(this) == 0) {
            Action();
        } else {
            home.pos = current.pos;
        }

        mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xa1c, &current.pos, &tevStr,
                                              &shape_angle, 0, 0xff, 0, -1, 0, 0, 0);

        SetCcSph();
        ObjHit();
        BoomChk();

        eyePos = current.pos;
        eyePos.y += 10.0f;

        attention_info.position = eyePos;
        attention_info.position.y += 10.0f;

        ParticleSet();

        break;
    case 3:
        FallAction();

        mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xa1c, &current.pos, &tevStr,
                                              &shape_angle, 0, 0xff, 0, -1, 0, 0, 0);

        SetCcSph();
        ObjHit();
        BoomChk();

        eyePos = current.pos;
        eyePos.y += 10.0f;

        attention_info.position = eyePos;
        attention_info.position.y += 10.0f;

        ParticleSet();

        break;
    case 1: {
        field_0x7d4--;
        if (field_0x7d4 <= 0) {
            field_0x7d4 = 0;
        }

        MoveAction();

        if (field_0x7e8 == 0) {
            cXyz unkXyz1(0.0f, 0.0f, 200.0f);
            cXyz unkXyz2(0.0f, 0.0f, 0.0f);

            dBgS_LinChk unkLinChk1;
            unkLinChk1.SetObj();

            cXyz unkXyz3;
            unkXyz3 = current.pos;
            unkXyz3.y += 100.0f;

            unkXyz2.set(current.pos.x, current.pos.y - 4000.0f, current.pos.z);

            unkLinChk1.Set(&unkXyz3, &unkXyz2, NULL);

            if (dComIfG_Bgsp().LineCross(&unkLinChk1) != 0) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(unkLinChk1, &plane);
                cXyz* planeNormal = plane.GetNP();

                field_0x7c8 = current.pos = unkLinChk1.GetCross();
                old.pos = current.pos;

                cXyz unkXyz4(0.0f, 0.0f, 0.0f);
                cXyz unkXyz5(0.0f, planeNormal->y, planeNormal->z);
                unkFloat1 = unkXyz4.abs(unkXyz5);

                field_0x7c2.z = field_0x7ea.z = -cM_atan2s(planeNormal->x, unkFloat1);
                field_0x7c2.x = field_0x7ea.x = cM_atan2s(planeNormal->z, planeNormal->y);
                field_0x7f6 = cM_atan2s(planeNormal->x, planeNormal->z);

                field_0x7e8 = 1;
            }
            unkXyz2.set(0.0f, 0.0f, 0.0f);
            for (s32 i = 0; i < 4; i++) {
                cLib_offsetPos(&unkXyz2, &current.pos, ((float)i * 160000.0f), &unkXyz1);
                unkLinChk1.Set(&current.pos, &unkXyz2, NULL);
                if (dComIfG_Bgsp().LineCross(&unkLinChk1) != 0) {
                    cM3dGPla plane;
                    dComIfG_Bgsp().GetTriPla(unkLinChk1, &plane);
                    cXyz* planeNormal = plane.GetNP();

                    field_0x7c8 = current.pos = unkLinChk1.GetCross();
                    old.pos = current.pos;

                    cXyz unkXyz6(0.0f, 0.0f, 0.0f);
                    cXyz unkXyz7(0.0f, planeNormal->y, planeNormal->z);
                    f32 unkFloat2 = unkXyz6.abs(unkXyz7);

                    field_0x7c2.z = field_0x7ea.z = -cM_atan2s(planeNormal->x, unkFloat2);
                    field_0x7c2.x = field_0x7ea.x = cM_atan2s(planeNormal->z, planeNormal->y);
                    field_0x7f6 = cM_atan2s(planeNormal->x, planeNormal->z);

                    field_0x7e8 = 1;
                }
            }
        }
        break;
    }
    }

    mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);

    if (field_0x990 != 0) {
        mAcch.CrrPos(dComIfG_Bgsp());
    }

    mpMorfSO->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpBrkAnm->play();
    mpBtkAnm->play();

    setBaseMtx();

    return 1;
}

void daObjKAT_c::Z_BufferChk() {
    cXyz projected;
    cXyz curWithOff;
    curWithOff = current.pos;
    curWithOff.y += 20.0f;
    mDoLib_project(&curWithOff, &projected);

    camera_class* camera = dComIfGp_getCamera(0);
    f32 unkFloat1;
    if (camera != NULL) {
        unkFloat1 = camera->mCamera.TrimHeight();
    } else {
        unkFloat1 = 0.0f;
    }

#if DEBUG
#define KAT_Z_BUFFERCHK_MAX_X 640.0f
#define KAT_Z_BUFFERCHK_MAX_Y FB_HEIGHT
#else
#define KAT_Z_BUFFERCHK_MAX_X FB_WIDTH
#define KAT_Z_BUFFERCHK_MAX_Y FB_HEIGHT
#endif
    if (projected.x > 0.0f && projected.x < KAT_Z_BUFFERCHK_MAX_X && projected.y > unkFloat1 &&
        projected.y < KAT_Z_BUFFERCHK_MAX_Y - unkFloat1)
    {
        dComIfGd_peekZ(projected.x,projected.y, &field_0x804);
    }

    f32 near = dComIfGd_getView()->near;
    f32 far = dComIfGd_getView()->far;
    mDoLib_pos2camera(&curWithOff, &projected);

    projected.z += 50.0f;
    if (projected.z > 0.0f) {
        projected.z = 0.0f;
    }

    field_0x800 = ((near + far * near / projected.z) / (far - near) + 1.0f) * 16777215.0f;
}

int daObjKAT_c::Delete() {
    dComIfG_resDelete(&mPhase, "Kat");
    if (field_0xa70 != 0) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.field_0x4);
    }
    if (heap != NULL) {
        mpMorfSO->stopZelAnime();
    }
    return 1;
}

void daObjKAT_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x7ea);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);

    mpMorfSO->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorfSO->modelCalc();
}

static int daObjKAT_Draw(daObjKAT_c* i_this) {
    return i_this->Draw();
}

static int daObjKAT_Execute(daObjKAT_c* i_this) {
    return i_this->Execute();
}

static u16 const l_musiya_num[2] = {
    0x01A1, /* dSv_event_flag_c::F_0417 - Misc. - Snail (M) */
    0x01A2, /* dSv_event_flag_c::F_0418 - Misc. - Snail (F) */
};

bool daObjKAT_c::CreateChk() {
    u8 uVar4 = (fopAcM_GetParam(this) & 0xf00) >> 8;
    if (uVar4 == 0xf) {
        uVar4 = 0;
    }
    if (field_0x808 != 2) {
        if (uVar4) {
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[SEX_MALE]]) ==
                    0 ||
                dComIfGs_isEventBit(
                    dSv_event_flag_c::saveBitLabels[l_musiya_num[SEX_FEMALE]]) == 0)
            {
                return false;
            }
        } else {
            if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 &&
                dComIfGp_getStartStageRoomNo() == 3)
            {
                if (dMenu_Insect_c::isCatchNotGiveInsect(l_kat_itemno[mSex]) != 0) {
                    return false;
                }
                if ((mSex == SEX_MALE &&
                     dComIfGs_isEventBit(
                         dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]]) == 0) ||
                    (mSex == SEX_FEMALE &&
                     dComIfGs_isEventBit(
                         dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]]) == 0))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

int daObjKAT_c::create() {
    fopAcM_ct(this, daObjKAT_c);

    s32 loadResult = dComIfG_resLoad(&mPhase, "Kat");
    if (loadResult == cPhs_COMPLEATE_e) {
        OS_REPORT("KAT PARAM: %x", fopAcM_GetParam(this));
        field_0x808 = fopAcM_GetParam(this) & 0xf;
        if (field_0x808 == 2) {
            field_0x56c = 0;
            shape_angle.x += (s16)-0x2000;
            fopAcM_OnStatus(this, 0x4000);
        } else {
            mDraw = true;
        }

        attention_info.distances[4] = 93;
        mSex = (u8)((fopAcM_GetParam(this) & 0x10) >> 4);
        setItemNo(l_kat_itemno[mSex]);
        setSaveBitNo(l_musiya_num[mSex]);

        if (mSex == SEX_FEMALE) {
            scale.set(l_HIO.mScaleFemale, l_HIO.mScaleFemale, l_HIO.mScaleFemale);
        } else if (mSex == 0) {
            scale.set(l_HIO.mScaleMale, l_HIO.mScaleMale, l_HIO.mScaleMale);
        }

        if ((u8)CreateChk() == 0) {
            return cPhs_ERROR_e;
        }

        if (fopAcM_entrySolidHeap(this, useHeapInit, 0xea0) == 0) {
            return cPhs_ERROR_e;
        }

        if (hio_set == 0) {
            hio_set = 1;
            field_0xa70 = 1;
            // Golden Snail
            l_HIO.field_0x4 = mDoHIO_CREATE_CHILD("黄金蟲(カタツムリ)", &l_HIO);
        }

        gravity = nREG_F(0);

        bool unkBool1 = false;

        cXyz unkXyz1(0.0f, 0.0f, 500.0f);
        cXyz unkXyz2(0.0f, 0.0f, -200.0f);
        cXyz unkXyz3;
        cXyz unkXyz4;

        dBgS_LinChk unkLinChk;
        unkLinChk.SetObj();

        for (s32 i = 0; i < 4; i++) {
            cLib_offsetPos(&unkXyz3, &current.pos, shape_angle.y + (i << 14),
                           &unkXyz1);
            cLib_offsetPos(&unkXyz4, &current.pos, shape_angle.y + (i << 14),
                           &unkXyz2);

            unkLinChk.Set(&unkXyz3, &unkXyz4, NULL);
            unkLinChk.SetObj();

            if (dComIfG_Bgsp().LineCross(&unkLinChk) != 0) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(unkLinChk, &plane);

                cXyz* planeNormal = plane.GetNP();

                current.pos = unkLinChk.GetCross();

                cXyz unkXyz5(0.0f, 0.0f, 0.0f);
                cXyz unkXyz6(0.0f, planeNormal->y, planeNormal->z);

                f32 unkFloat1 = unkXyz5.abs(unkXyz6);
                field_0x7c2.z = field_0x7ea.z = -cM_atan2s(planeNormal->x, unkFloat1);
                field_0x7c2.x = field_0x7ea.x = cM_atan2s(planeNormal->z, planeNormal->y);
                field_0x7f6 = cM_atan2s(planeNormal->x, planeNormal->z);

                unkBool1 = true;

                old.pos = current.pos;

                break;
            }
        }

        if (!unkBool1) {
            unkXyz1.set(0.0f, 500.0f, 0.0f);
            unkXyz3.set(0.0f, -1000.0f, 0.0f);

            unkLinChk.Set(&unkXyz1, &unkXyz3, NULL);
            unkLinChk.SetObj();

            if (dComIfG_Bgsp().LineCross(&unkLinChk) != 0) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(unkLinChk, &plane);

                cXyz* planeNormal = plane.GetNP();
                current.pos = unkLinChk.GetCross();

                cXyz unkXyz7(0.0f, 0.0f, 0.0f);
                cXyz unkXyz8(0.0f, planeNormal->y, planeNormal->z);

                f32 fVar16 = unkXyz7.abs(unkXyz8);
                field_0x7c2.z = field_0x7ea.z = -cM_atan2s(planeNormal->x, fVar16);
                field_0x7c2.x = field_0x7ea.x = cM_atan2s(planeNormal->z, planeNormal->y);
                field_0x7f6 = cM_atan2s(planeNormal->x, planeNormal->z);

                old.pos = current.pos;
            }
        }

        field_0xa24.SetWall(20.0f, 20.0f);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &field_0xa24,
                  fopAcM_GetSpeed_p(this), NULL, NULL);

        J3DModel* model = mpMorfSO->getModel();
        model->setUserArea((uintptr_t)this);

        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            if (i != 0) {
                model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
            }
        }

        if (field_0x808 == 0) {
            InitCcSph();
        }

        if (field_0x808 == 2) {
            if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 && dComIfGp_getStartStageRoomNo() == 3) {
                field_0x808 = 3;
            }
        }

        field_0x7c8 = current.pos;

        fopAcM_SetMtx(this, mpMorfSO->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);

        mCreatureSound.init(&current.pos, &eyePos, 3, 1);

        daObjKAT_Execute(this);
    }

    return loadResult;
}

static int daObjKAT_IsDelete(daObjKAT_c* param_0) {
    return 1;
}

static actor_method_class l_daObjKAT_Method = {
    (process_method_func)daObjKAT_Create,
    (process_method_func)daObjKAT_Delete,
    (process_method_func)daObjKAT_Execute,
    (process_method_func)daObjKAT_IsDelete,
    (process_method_func)daObjKAT_Draw,
};

actor_process_profile_definition g_profile_Obj_Kat = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_Kat,           // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daObjKAT_c),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    488,                    // mPriority
    &l_daObjKAT_Method,     // sub_method
    0x000C0100,             // mStatus
    fopAc_ENV_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
