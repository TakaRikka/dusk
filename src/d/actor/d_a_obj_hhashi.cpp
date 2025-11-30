/**
 * @file d_a_obj_hhashi.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_hhashi.h"
#include "d/actor/d_a_e_fm.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_lib.h"
#include "SSystem/SComponent/c_math.h"

void daObjHHASHI_c::initCcCylinder() {
    const static dCcD_SrcCyl ccCylSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x100000, 0x1f}, 0x11}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            110.0f, // mRadius
            158.0f // mHeight
        } // mCyl
    };

    mStts.Init(0xff, 0xff, this);
    mCyl.Set(ccCylSrc);
    mCyl.SetStts(&mStts);
    mCyl.SetC(current.pos);
}

void daObjHHASHI_c::setCcCylinder() {
    dComIfG_Ccsp()->Set(&mCyl);
}

static int daObjHHASHI_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daObjHHASHI_c*>(i_this)->create();
}

static int daObjHHASHI_Delete(daObjHHASHI_c* i_this) {
    fopAcM_GetID(i_this);
    i_this->MoveBGDelete();
    return 1;
}

void daObjHHASHI_c::Set_Hahen(cXyz* param_1) {
    csXyz cStack_38(0, 0, 0);
    fopAcM_seStartCurrent(this, Z2SE_OBJ_FM_BREAK_PILLAR, 0);
    if (field_0x5a4 == 0) {
        cXyz cStack_30(current.pos.x, current.pos.y + 300.0f, current.pos.z);
        cStack_38.y = field_0xcb8;
        dComIfGp_particle_set(0x8710, param_1, &tevStr, &cStack_38, 0);
        dComIfGp_particle_set(0x8711, param_1, &tevStr, &cStack_38, 0);
        dComIfGp_particle_set(0x8712, param_1, &tevStr, &cStack_38, 0);
        dComIfGp_particle_set(0x8713, &current.pos, &tevStr, &shape_angle, 0);
        dComIfGp_particle_set(0x8714, &current.pos, &tevStr, &shape_angle, 0);
        Set_Speed(0, 50, 50.0f, 50.0f, 300.0f, 300.0f);
        field_0x5a4 = 2;
        if (mpBgW != NULL &&
            mpBgW->ChkUsed())
        {
            if (!dComIfG_Bgsp().Release(mpBgW)) {
                OS_REPORT("Release Error\n");
            }
        }
    }
}

void daObjHHASHI_c::Set_Speed(s16 param_1, s16 param_2, f32 param_3, f32 param_4, f32 param_5,
                                  f32 param_6) {
    csXyz cStack_f0;
    for (int i = param_1; i < param_2 - 20; i++) {
        field_0x5b0[i].set(current.pos.x + cM_rndFX(70.0f),
                           (current.pos.y + param_5) + cM_rndF(param_6),
                           current.pos.z + cM_rndFX(70.0f));
        field_0xa60[i].set(cM_rndF(2.5f) + 0.5f, cM_rndF(1.5f) + 1.1f, cM_rndF(2.5f) + 0.5f);
        cM_rndFX(param_4);
        f32 fVar1 = cM_rndFX(30.0f) + 30.0f;
        s16 rot = (s16)cM_rndFX(8192.0f) +
                      (field_0xcb8 - 32768.0f);
        field_0x808[i].set(0.0f, 0.0f, fVar1);
        mDoMtx_stack_c::YrotS(rot);
        mDoMtx_stack_c::multVec(&field_0x808[i], &field_0x808[i]);
        field_0x808[i].y = fVar1;
        field_0xde8[i].x = cM_rndFX(15000.0f);
        field_0xde8[i].y = cM_rndFX(15000.0f);
        field_0xcbc[i].x = cM_rndFX(65536.0f);
        field_0xcbc[i].y = cM_rndFX(65536.0f);
    }
    for (int i = param_2 - 20; i < param_2; i++) {
        field_0x5b0[i].set(current.pos.x + cM_rndFX(70.0f),
                           (current.pos.y + param_5) + cM_rndFX(param_6),
                           current.pos.z + cM_rndFX(70.0f));
        field_0xa60[i].set(cM_rndF(2.5f) + 0.5f, cM_rndF(1.5f) + 1.1f, cM_rndF(2.5f) + 0.5f);
        f32 dVar10;
        f32 dVar12 = cM_rndFX(20.0f) + 20.0f;
        dVar10 = cM_rndFX(20.0f) + 10.0f;
        s16 sVar6 = cM_rndFX(32000.0f);
        f32 dVar13 = dVar10 + gravity;
        f32 dVar14 = dVar12 * cM_scos(sVar6);
        field_0x808[i].set((dVar12 * cM_ssin(sVar6)), dVar13, dVar14);
        field_0xde8[i].x = cM_rndFX(15000.0f);
        field_0xde8[i].y = cM_rndFX(15000.0f);
        field_0xcbc[i].x = cM_rndFX(65536.0f);
        field_0xcbc[i].y = cM_rndFX(65536.0f);
    }
}

void daObjHHASHI_c::Obj_Damage() {
    if (field_0xf46 == 0) {
        mStts.Move();
        if (mCyl.ChkTgHit()) {
            cXyz& playerPos = fopAcM_GetPosition(daPy_getPlayerActorClass());
            field_0xf46 = 10;
            e_fm_class* tgActor = (e_fm_class*)dCc_GetAc(mCyl.GetTgHitObj()->GetAc());
            cXyz* tgHitPos = mCyl.GetTgHitPosP();
            cLib_targetAngleY(&current.pos, tgHitPos);
            if (fopAcM_GetName(tgActor) == PROC_E_FM) {
                field_0x5aa++;
                e_fm_class* fm = (e_fm_class*)fopAcM_SearchByID(fopAcM_GetID(tgActor));
                if (fm != NULL) {
                    field_0xcb8 = tgActor->shape_angle.y;
                    field_0x5a8 = fm->field_0x1828;;
                }
                if (field_0x5a8 == 1) {
                    field_0xcb8 += 16384.0f;
                } else if (field_0x5a8 == 0) {
                    field_0xcb8 -= 16384.0f;
                }
                Set_Hahen(tgHitPos);
                dComIfGp_getVibration().StartShock(7, 1, cXyz(0.0f, 1.0f, 0.0f));
            }
            mCyl.ResetTgHit();
            // !@bug: I'm not quite sure but it seems like the access to tgActor fields is outside
            // proc name check so it might not be the right type of actor
            tgActor->mSound.startChainSound(
                Z2SE_EN_FM_CHAIN_HIT, tgActor->field_0x1828, 100.0f, 0,
                dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
    }
}

void daObjHHASHI_c::Wall_Check(s16 idx) {
    dBgS_LinChk linChk;
    cXyz cStack_8c(
                 field_0x5b0[idx].x + field_0x808[idx].x,
                 field_0x5b0[idx].y,
                 field_0x5b0[idx].z + field_0x808[idx].z);
    cXyz cStack_98(field_0x5b0[idx].x,
                 field_0x5b0[idx].y, field_0x5b0[idx].z);
    linChk.Set(&cStack_98, &cStack_8c, NULL);
    if (dComIfG_Bgsp().LineCross(&linChk) == 1) {
        field_0x808[idx].x = -0.05f * field_0x808[idx].x;
        field_0x808[idx].z = -0.05f * field_0x808[idx].z;
    }
}

void daObjHHASHI_c::Hahen_Motion(s16 param_1) {
    if (field_0xf14[param_1] != 1) {
        f32 dVar6 = 5.0f;
        Wall_Check(param_1);
        if (field_0x5b0[param_1].y < dVar6 + 10.0f) {
            f32 dVar5 = field_0x808[param_1].y;
            field_0x5b0[param_1].y = dVar6 + 10.0f;
            if (field_0x5a4 == 2) {
                field_0x808[param_1].y = (dVar5 * -((cM_rndF(0.2f) + 0.2f) + 0.3f));
                field_0x808[param_1].x = field_0x808[param_1].x * (cM_rndF(0.3f) + 0.7f);
                field_0x808[param_1].z = field_0x808[param_1].z * (cM_rndF(0.3f) + 0.7f);
            }
            if (field_0x808[param_1].y < 4.0f) {
                field_0xf14[param_1] = 1;
            }
            field_0xde8[param_1].x = cM_rndFX(5000.0f);
            field_0xde8[param_1].y = cM_rndFX(5000.0f);

        } else {
            if (field_0x5b0[param_1].y >= dVar6) {
                field_0x808[param_1].y += gravity;
                field_0x5b0[param_1].x += field_0x808[param_1].x;
                field_0x5b0[param_1].y += field_0x808[param_1].y;
                field_0x5b0[param_1].z += field_0x808[param_1].z;
                field_0xcbc[param_1].x += field_0xde8[param_1].x;
                field_0xcbc[param_1].y += field_0xde8[param_1].y;
            }
        }
    }
}

void daObjHHASHI_c::CheckCull() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz& playerPos = fopAcM_GetPosition(player);
    int culledNum = 0;
    for (int i = 0; i < 50; i++) {
        f32 dVar6 = field_0x5b0[i].absXZ(playerPos);
        if (dVar6 > 1500.0f && !checkViewArea(i)) {
            culledNum++;
            if (culledNum == 50) {
                fopAcM_delete(this);
            }
        }
    }
}

bool daObjHHASHI_c::checkViewArea(int param_1) {
    Vec local_20;
    mDoLib_project(&field_0x5b0[param_1], &local_20);
    bool rv = false;
    if (local_20.x >= 0.0f && local_20.x <= FB_WIDTH && local_20.y >= 0.0f && local_20.y <= FB_HEIGHT) {
        rv = true;
    }
    return rv;
}

void daObjHHASHI_c::Rolling(cXyz* param_1, f32 param_2, f32 param_3, s8 param_4) {
    field_0x5a8 = param_4;
    if (field_0x5a4 == 2) {
        for (s16 i = 0; i < 0x32; i++) {
            if (param_1->absXZ(field_0x5b0[i]) < param_2 && field_0xf14[i] == 1) {
                Roll_Set(param_1, param_3, i);
                field_0xf14[i] = 0;
            }
        }
    }
}

void daObjHHASHI_c::Roll_Set(cXyz* param_1, f32 param_2, s16 idx) {
    f32 dVar10 = (param_2 * (0.7f + cM_rndFX(0.2f)));
    s16 angleY = cLib_targetAngleY(&field_0x5b0[idx], param_1);
    f32 dVar9 = ((-1.0f * param_2) * cM_ssin(angleY));
    f32 dVar11 = (3.0f / field_0xa60[idx].abs());
    f32 dVar13 = dVar10 + gravity;
    f32 dVar14 = ((-1.0f * param_2) * cM_scos(angleY));
    field_0x808[idx].set(dVar11 * dVar9, dVar13, dVar11 * dVar14);
    field_0xde8[idx].x = cM_rndFX(5000.0f);
    field_0xde8[idx].y = cM_rndFX(5000.0f);
    field_0xcbc[idx].x += 0x1000;
    field_0xcbc[idx].y += 0x1000;
}

void daObjHHASHI_c::setBaseMtx() {
    if (field_0x5a4 == 2) {
        for (s16 i = 0; i < 50; i++) {
            mDoMtx_stack_c::transS(field_0x5b0[i]);
            mDoMtx_stack_c::ZXYrotM(field_0xcbc[i]);
            mDoMtx_stack_c::scaleM(field_0xa60[i]);
            mShardModels[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    } else {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    cMtx_copy(mDoMtx_stack_c::get(), (MtxP)&mBgMtx);
}

static int daObjHHASHI_Draw(daObjHHASHI_c* i_this) {
    return i_this->Draw();
}

static int daObjHHASHI_Execute(daObjHHASHI_c* i_this) {
    return i_this->MoveBGExecute();
}

static char* l_arcName = "L_hhashi";

int daObjHHASHI_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "L_hhashi00.bmd");
    JUT_ASSERT(90, modelData != NULL);
    J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "L_hhashi02.bmd");
    JUT_ASSERT(93, modelData2 != NULL);
    for (int i = 0; i < 50; i++) {
        mShardModels[i] = mDoExt_J3DModel__create(modelData2, 0x20000, 0x11000084);
        if (mShardModels[i] == NULL) {
            return FALSE;
        }
    }
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mModel != 0 ? TRUE : FALSE;
}

int daObjHHASHI_c::create() {
    fopAcM_ct(this, daObjHHASHI_c);
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        gravity = -9.0f;
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "L_hhashi00.dzb");
        JUT_ASSERT(1112, dzb_id != -1);
        rv = MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x6dc0, NULL);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
        initCcCylinder();
        fopAcM_SetMtx(this, mModel->getBaseTRMtx());
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "L_hhashi02.bmd");
        JUT_ASSERT(1133, modelData != NULL);
        dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(this), 0);
        field_0x1199 = 1;
        daObjHHASHI_Execute(this);
    }
    return rv;
}

static int daObjHHASHI_IsDelete(daObjHHASHI_c* param_0) {
    return 1;
}

int daObjHHASHI_c::Create() {
    fopAcM_setCullSizeBox(this, -1000.0f, -500.0f, -1000.0f, 1000.0f, 500.0f, 1000.0f);
    return cPhs_COMPLEATE_e;
}

int daObjHHASHI_c::Execute(f32 (**param_1)[3][4]) {
    Obj_Damage();
    if (field_0x5a4 == 2) {
        for (s16 i = 0; i < 50; i++) {
            Hahen_Motion(i);
        }
        CheckCull();
    }
    *param_1 = &mBgMtx;
    setBaseMtx();
    if (field_0x5a4 != 2) {
        setCcCylinder();
    }
    if (field_0xf46 != 0) {
        field_0xf46--;
    }
    return 1;
}

int daObjHHASHI_c::Draw() {
    if (dComIfGp_event_runCheck()) {
        camera_class* pCamera = dComIfGp_getCamera(0);
        f32 xdiff = current.pos.x - pCamera->lookat.eye.x;
        f32 zdiff = current.pos.z - pCamera->lookat.eye.z;
        if (JMAFastSqrt(xdiff * xdiff + zdiff * zdiff) < 200.0f) {
            return 1;
        }
    }
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    if (field_0x5a4 == 0) {
        g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    }
    dComIfGd_setListBG();
    if (field_0x5a4 == 0) {
        mDoExt_modelUpdateDL(mModel);
    } else {
        int roomNo = fopAcM_GetRoomNo(this);
        for (int i = 0; i < 50; i++) {
            dComIfGp_entrySimpleModel(mShardModels[i], roomNo);
        }
    }
    return 1;
}

int daObjHHASHI_c::Delete() {
    if (field_0x1199 != 0) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "L_hhashi02.bmd");
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(this));
    }
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static actor_method_class l_daObjHHASHI_Method = {
    (process_method_func)daObjHHASHI_Create,
    (process_method_func)daObjHHASHI_Delete,
    (process_method_func)daObjHHASHI_Execute,
    (process_method_func)daObjHHASHI_IsDelete,
    (process_method_func)daObjHHASHI_Draw,
};

extern actor_process_profile_definition g_profile_Obj_HHASHI = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_HHASHI,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjHHASHI_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  462,                    // mPriority
  &l_daObjHHASHI_Method,  // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
