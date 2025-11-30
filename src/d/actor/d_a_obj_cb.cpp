/**
 * @file d_a_obj_cb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_cb.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_w.h"

daObj_Cb_HIO_c::daObj_Cb_HIO_c() {
    field_0x4 = -1;
}

static int daObj_Cb_Draw(obj_cb_class* i_this) {
    g_env_light.settingTevStruct(0x10, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mModel, &i_this->tevStr);
    mDoExt_modelUpdateDL(i_this->mModel);
    return 1;
}

static void ride_call_back(dBgW* param_0, fopAc_ac_c* i_this, fopAc_ac_c* param_2) {
    static_cast<obj_cb_class*>(i_this)->field_0x59c = 3;
}

static void action(obj_cb_class* i_this) {
    fopAc_ac_c* a_this = i_this;
    cXyz local_60;
    int local_78 = 0;
    switch(i_this->field_0x596) {
    case 0:
        if (i_this->field_0x59c != 0) {
            i_this->field_0x59c--;
            i_this->field_0x59e++;
            f32 dist = (i_this->home.pos - i_this->current.pos).abs();
            if ((i_this->field_0x59e & 4) != 0) {
                local_78 = 0x100;
            } else {
                local_78 = -0x100;
            }
            cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
            local_60.x = 0.0f;
            local_60.y = 0.0f;
            local_60.z = i_this->speedF;
            MtxPosition(&local_60, &i_this->speed);
            i_this->current.pos += i_this->speed;
            if (dist > 30.0f) {
                cLib_addCalcAngleS2(&i_this->shape_angle.x, 0x2000, 2, 0x200);
            }
            cLib_addCalc2(&i_this->speedF, 5.0f, 1.0f, 0.5f);
            if (dist > 60.0f) {
                i_this->field_0x596 = 10;
                local_60.x = 0.0f;
                local_60.y = 0.0f;
                local_60.z = 5.0f;
                MtxPosition(&local_60, &i_this->speed);
                i_this->field_0x5a0.x = 0x900;
                dComIfG_Bgsp().Release(i_this->mBgW);
                i_this->mBgW = 0;
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CTL_ROCK_FALL, &i_this->current.pos, 0, 0, 1.0f,
                                         1.0f, -1.0f, -1.0f, 0);
            }
        } else {
            i_this->field_0x59e = 0;
            cLib_addCalcAngleS2(&i_this->shape_angle.x, 0, 1, 0x1000);
        }
        break;
    case 5:
        if (fopAcM_searchPlayerDistanceXZ(i_this) < 90.0f) {
            cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
            i_this->field_0x596 = 10;
            local_60.x = 0.0;
            local_60.y = 0.0;
            local_60.z = 20.0f;
            MtxPosition(&local_60, &i_this->speed);
            i_this->field_0x5a0.x = 0x900;
            i_this->field_0x598[0] = 20;
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CTL_ROCK_FALL, &i_this->current.pos, 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
        }
        break;
    case 10:
        i_this->current.pos += i_this->speed;
        i_this->speed.y += -5.0f;
        i_this->shape_angle.x += i_this->field_0x5a0.x;
        i_this->shape_angle.y += i_this->field_0x5a0.y;
        f32 speedY = i_this->speed.y;
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        if (i_this->field_0x598[0] == 0 && i_this->mAcch.ChkGroundHit()) {
            if (speedY <= -30.0f) {
                i_this->field_0x5a0.y = cM_rndFX(5000.0f);
                i_this->field_0x5a0.x = cM_rndFX(5000.0f);
                cMtx_YrotS(*calc_mtx, i_this->current.angle.y + (s16)cM_rndFX(7000.0f));
                local_60.x = 0.0f;
                local_60.y = 0.0f;
                local_60.z = 15.0f + cM_rndF(10.0f);
                MtxPosition(&local_60, &i_this->speed);
                i_this->speed.y = -0.3f * speedY;
                fopAcM_effSmokeSet1(&i_this->field_0x970, &i_this->field_0x974,
                                    &i_this->current.pos, 0, 2.0f, &i_this->tevStr, 1);
            } else {
                i_this->field_0x5a0.set(0, 0, 0);
                cLib_addCalcAngleS2(&i_this->shape_angle.y, 0, 1, 0x1000);
                cLib_addCalcAngleS2(&i_this->shape_angle.x, 0, 1, 0x1000);
                i_this->speed.set(0.0f, -5.0f, 0.0f);
                // !@bug field_0x594 is never initialized
                i_this->field_0x594++;
                if (i_this->field_0x594 > 150) {
                    fopAcM_delete(a_this);
                }
            }
            cLib_addCalc0(&i_this->field_0x5a8, 1.0f, 5.0f);
        } else {
            cLib_addCalc2(&i_this->field_0x5a8, -25.0f, 1.0f, 1.0f);
        }
        break;
    }

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::transM(0.0f, i_this->field_0x5a8, 0.0f);
    mDoMtx_stack_c::XrotM(local_78);
    i_this->mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::XrotM(-local_78);
    if (i_this->mBgW != NULL) {
        MTXCopy(mDoMtx_stack_c::get(), i_this->mBgMtx);
        i_this->mBgW->Move();
    }
}

static int daObj_Cb_Execute(obj_cb_class* i_this) {
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x598[i] != 0) {
            i_this->field_0x598[i]--;
        }
    }
    action(i_this);
    s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(i_this));
    i_this->mSoundObj.framework(0, reverb);
    return 1;
}

static int daObj_Cb_IsDelete(obj_cb_class* param_0) {
    return 1;
}

static u8 data_80BC58F8;

static int daObj_Cb_Delete(obj_cb_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "Obj_cb");
    if (i_this->field_0x978 != 0) {
        data_80BC58F8 = 0;
    }
    if (i_this->mBgW != NULL) {
        dComIfG_Bgsp().Release(i_this->mBgW);
    }
    if (i_this->heap != NULL) {
        i_this->mSoundObj.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    obj_cb_class* i_this = (obj_cb_class*)actor;
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes("Obj_cb", 4);
    JUT_ASSERT(502, modelData != NULL);
    i_this->mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mModel == NULL) {
        return 0;
    }
    if (i_this->home.angle.x == 0) {
        i_this->mBgW = new dBgW();
        if (i_this->mBgW == NULL) {
            return 0;
        }
        cBgD_t* bGd = (cBgD_t*)dComIfG_getObjectRes("Obj_cb", 7);
        if (i_this->mBgW->Set(bGd, 1, &i_this->mBgMtx) == 1) {
            return 0;
        }
        i_this->mBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
        i_this->mBgW->SetRideCallback(ride_call_back);
    }
    i_this->mSoundObj.init(&i_this->current.pos, 1);
    return 1;
}

static daObj_Cb_HIO_c l_HIO;

static int daObj_Cb_Create(fopAc_ac_c* actor) {
    fopAcM_ct(actor, obj_cb_class);
    obj_cb_class* i_this = (obj_cb_class*)actor;
    int rv = dComIfG_resLoad(&i_this->mPhase, "Obj_cb");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_CB PARAM %x\n", fopAcM_GetParam(i_this));
        if (i_this->home.angle.x != 0) {
            i_this->field_0x596 = 5;
        }
        OS_REPORT("OBJ_CB//////////////OBJ_CB SET 1 !!\n");
        if (fopAcM_entrySolidHeap(i_this, useHeapInit, 0x800) == 0) {
            OS_REPORT("//////////////OBJ_CB SET NON !!\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////////OBJ_CB SET 2 !!\n");
        if (i_this->home.angle.x == 0 && i_this->mBgW != NULL) {
            if (dComIfG_Bgsp().Regist(i_this->mBgW, i_this) != 0) {
                return cPhs_ERROR_e;
            }
        }
        if (data_80BC58F8 == 0) {
            i_this->field_0x978 = 1;
            data_80BC58F8 = 1;
            l_HIO.field_0x4 = -1;
        }
        fopAcM_SetMtx(i_this, i_this->mModel->getBaseTRMtx());
        i_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                          &i_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, 50.0f);
        daObj_Cb_Execute(i_this);
    }
    return rv;
}

static actor_method_class l_daObj_Cb_Method = {
    (process_method_func)daObj_Cb_Create,
    (process_method_func)daObj_Cb_Delete,
    (process_method_func)daObj_Cb_Execute,
    (process_method_func)daObj_Cb_IsDelete,
    (process_method_func)daObj_Cb_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_CB = {
  fpcLy_CURRENT_e,       // mLayerID
  3,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_OBJ_CB,           // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(obj_cb_class),  // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  49,                    // mPriority
  &l_daObj_Cb_Method,    // sub_method
  0x00040100,            // mStatus
  fopAc_ACTOR_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
