/**
 * @file d_a_obj_fw.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_fw.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_w.h"
#include "d/d_s_play.h"

enum RYUW00_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_K_RYUWOOD00 = 0x4,
    /* 0x5 */ BMDR_K_RYUWOOD01,

    /* DZB */
    /* 0x8 */ DZB_K_RYUWOOD00 = 0x8,
    /* 0x9 */ DZB_K_RYUWOOD01,
};

enum Action {
    /* 0x0 */ ACTION_FLOAT,
};

daObj_Fw_HIO_c::daObj_Fw_HIO_c() {
    field_0x4 = -1;
    size = 1.0f;
}

static u8 l_initHIO;

static daObj_Fw_HIO_c l_HIO;

static void ride_call_back(dBgW* i_bgw, fopAc_ac_c* a_this, fopAc_ac_c* param_3) {
    if (fopAcM_GetName(param_3) == PROC_ALINK) {
        obj_fw_class* i_this = (obj_fw_class*)a_this;
        
        if (i_this->field_0x592 == 0) {
            i_this->field_0x598 = AREG_F(0) + -50.0f;
        }

        i_this->field_0x592 = 10;
        f32 fVar1 = param_3->current.pos.x - a_this->current.pos.x;
        f32 fVar2 = param_3->current.pos.z - a_this->current.pos.z;
        
        cLib_addCalcAngleS2(&i_this->field_0x5a0, cM_atan2s(fVar1, fVar2), 4, 0x80);
        s16 sVar1 = JMAFastSqrt(fVar1 * fVar1 + fVar2 * fVar2) * (AREG_F(3) + 10.0f) * l_HIO.size;
        if (sVar1 > 0x800) {
            sVar1 = 0x800;
        } else if (sVar1 < -0x800) {
            sVar1 = -0x800;
        }

        cLib_addCalcAngleS2(&i_this->field_0x5a2, sVar1, 4, 0x20);
    }
}

static int daObj_Fw_Draw(obj_fw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    g_env_light.settingTevStruct(16, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &a_this->tevStr);
    mDoExt_modelUpdateDL(i_this->mpModel);
    return 1;
}

static BOOL water_check(obj_fw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    Vec sp78;
    dBgS_ObjGndChk_Spl obj_gnd_chk_spl;

    sp78.x = a_this->current.pos.x;
    sp78.y = a_this->current.pos.y + 500.0f;
    sp78.z = a_this->current.pos.z;
    obj_gnd_chk_spl.SetPos(&sp78);
    i_this->field_0x584 = dComIfG_Bgsp().GroundCross(&obj_gnd_chk_spl);

    if (i_this->field_0x584 > a_this->current.pos.y + TREG_F(11)) {
        return TRUE;
    }

    return FALSE;
}

static void fw_float(obj_fw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    cLib_addCalc0(&a_this->speed.y, 1.0f, TREG_F(6) + 0.25f);
    cXyz sp78(a_this->current.pos);
    sp78.y = i_this->field_0x584;
    fopAcM_effHamonSet(&i_this->field_0x61c, &sp78, 3.5f, 0.05f);
    
    f32 fVar1 = (i_this->field_0x58c * (JREG_F(3) + 0.03f)) * cM_ssin(i_this->field_0x578 * (JREG_S(2) + 500));
    f32 fVar2 = i_this->field_0x584 + JREG_F(8) + -(TREG_F(11) + 300.0f) * cM_ssin(i_this->field_0x5a4 / 2);
    cLib_addCalc2(&a_this->current.pos.y, fVar2 + fVar1, 0.1f, JREG_F(4) + 10.0f);
    i_this->field_0x590++;

    if (a_this->field_0x567 == 1) {
        i_this->field_0x590 += 5;
        cLib_addCalc2(&i_this->field_0x58c, 300.0f, 0.1f, JREG_F(18) + 2.0f);
        a_this->current.pos += a_this->speed;
    } else if (a_this->field_0x567 == 2) {
        if (i_this->field_0x57e[0] == 0) {
            cLib_addCalcAngleS2(&i_this->field_0x5a4, 0x7F80, 0x20, 0x200);
            a_this->current.pos += a_this->speed;
        }
    } else {
        cLib_addCalc2(&i_this->field_0x58c, 300.0f, 0.1f, 20.0f);
    }

    a_this->shape_angle.x = i_this->field_0x58c * cM_ssin(i_this->field_0x590 * (TREG_S(3) + 500));
    a_this->shape_angle.z = i_this->field_0x58c * cM_ssin(i_this->field_0x590 * (TREG_S(4) + 600));
    cLib_addCalc2(&i_this->field_0x594, i_this->field_0x598, 0.2f, 20.0f);

    f32 fVar3;
    if (i_this->field_0x592 != 0) {
        fVar3 = -20.0f;
    } else {
        fVar3 = 0.0f;
    }

    cLib_addCalc2(&i_this->field_0x598, fVar3, 1.0f, 3.0f);
}

static void action(obj_fw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp30, sp3c;

    switch (i_this->field_0x57a) {
        case ACTION_FLOAT:
            fw_float(i_this);
            break;
    }

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y + i_this->field_0x594 + i_this->field_0x588, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->field_0x5a0);
    mDoMtx_stack_c::XrotM(i_this->field_0x5a2);
    mDoMtx_stack_c::YrotM(-i_this->field_0x5a0);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x + i_this->field_0x5a4);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.size, 1.0f, l_HIO.size);

    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    sp30.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&sp30, &i_this->field_0x5a8);

    cXyz sp48(i_this->field_0x5a8);
    sp48.y += TREG_F(0) + 15.0f;
    a_this->eyePos = sp48;
    a_this->attention_info.position = sp48;

    MTXCopy(mDoMtx_stack_c::get(), i_this->mMtx);
    i_this->mpBgW->Move();

    if (i_this->field_0x592 == 0) {
        cLib_addCalcAngleS2(&i_this->field_0x5a2, 0, 4, 0x10);
    }
}

static int daObj_Fw_Execute(obj_fw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    i_this->field_0x578++;

    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x57e[i] != 0) {
            i_this->field_0x57e[i]--;
        }
    }

    if (i_this->field_0x592 != 0) {
        i_this->field_0x592--;
    }

    action(i_this);

    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    return 1;
}

static int daObj_Fw_IsDelete(obj_fw_class* i_this) {
    return 1;
}

static int daObj_Fw_Delete(obj_fw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    dComIfG_resDelete(&i_this->mPhase, "RYUW00");

    if (i_this->field_0x624 != 0) {
        l_initHIO = 0;
    }

    dComIfG_Bgsp().Release(i_this->mpBgW);

    if (a_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_fw_class* i_this = (obj_fw_class*)a_this;

    static int bmdd[2] = {
        BMDR_K_RYUWOOD00, BMDR_K_RYUWOOD01,
    };

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("RYUW00", bmdd[i_this->field_0x570]);

    JUT_ASSERT(543, modelData != NULL);
    
    i_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mpModel == NULL) {
        return 0;
    }

    i_this->mpBgW = new dBgW();
    if (i_this->mpBgW == NULL) {
        return 0;
    }

    static int dzbd[2] = {
        DZB_K_RYUWOOD00, DZB_K_RYUWOOD01,
    };

    if (i_this->mpBgW->Set((cBgD_t*)dComIfG_getObjectRes("RYUW00", dzbd[i_this->field_0x570]), 1, &i_this->mMtx) == true) {
        return 0;
    }

    i_this->mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
    i_this->mpBgW->SetRideCallback(ride_call_back);
    return 1;
}

static cPhs__Step daObj_Fw_Create(fopAc_ac_c* a_this) {
    fopAcM_ct(a_this, obj_fw_class);
    obj_fw_class* i_this = (obj_fw_class*)a_this;

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "RYUW00");
    if (phase == cPhs_COMPLEATE_e) {
        if (dComIfGs_isStageBossEnemy()) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("OBJ_FW PARAM %x\n", fopAcM_GetParam(a_this));

        i_this->field_0x570 = fopAcM_GetParam(a_this);

        if (i_this->field_0x570 == 0xFF) {
            i_this->field_0x570 = 0;
        }

        u32 i_size;
        if (i_this->field_0x570 == 1) {
            i_size = 0x1400;
        } else {
            i_size = 0x1940;
        }

        OS_REPORT("OBJ_FW//////////////OBJ_FW SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, i_size)) {
            OS_REPORT("//////////////OBJ_FW SET NON !!\n");
            
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_FW SET 2 !!\n");

        if (dComIfG_Bgsp().Regist(i_this->mpBgW, a_this)) {
            return cPhs_ERROR_e;
        }

        if (l_initHIO == 0) {
            i_this->field_0x624 = 1;
            l_initHIO = 1;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(a_this, i_this->mpModel->getBaseTRMtx());
        fopAcM_SetMin(a_this, -1000.0f, -600.0f, -1000.0f);
        fopAcM_SetMax(a_this, 1000.0f, 600.0f, 1000.0f);
        i_this->mSound.init(&a_this->current.pos, 2);
        i_this->field_0x578 = cM_rndF(65536.0f);
        i_this->field_0x590 = cM_rndF(65536.0f);

        water_check(i_this);
        daObj_Fw_Execute(i_this);
    }

    return phase;
}

static actor_method_class l_daObj_Fw_Method = {
    (process_method_func)daObj_Fw_Create,
    (process_method_func)daObj_Fw_Delete,
    (process_method_func)daObj_Fw_Execute,
    (process_method_func)daObj_Fw_IsDelete,
    (process_method_func)daObj_Fw_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_FW = {
  fpcLy_CURRENT_e,        // mLayerID
  8,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_FW,            // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_fw_class),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  42,                     // mPriority
  &l_daObj_Fw_Method,     // sub_method
  0x00044100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
