/**
* @file d_a_obj_maki.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_maki.h"
#include "JSystem/J3DGraphAnimator/J3DAnimation.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/J3DGraphAnimator/J3DModelData.h"
#include "SSystem/SComponent/c_cc_d.h"
#include "SSystem/SComponent/c_phase.h"
#include "SSystem/SComponent/c_xyz.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "types.h"

daObj_Maki_HIO_c::daObj_Maki_HIO_c() {
    field_0x4 = -1;
}

int daObj_Maki_Draw(obj_maki_class* i_this) {
    g_env_light.settingTevStruct(0x10, &i_this->current.pos, &i_this->tevStr);

    if (i_this->field_0x57e == 0) {
        g_env_light.setLightTevColorType_MAJI(i_this->mModel1, &i_this->tevStr);
        i_this->mBrk->entry(i_this->mModel1->getModelData());
        mDoExt_modelUpdateDL(i_this->mModel1);
    } else if (i_this->field_0x57c != 0x01) {
        g_env_light.setLightTevColorType_MAJI(i_this->mModel2, &i_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mModel2);
    }
    return 1;
}

int daObj_Maki_Execute(obj_maki_class* i_this) {
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x580[i] != 0) {
            i_this->field_0x580[i]--;
        }
    }
    cXyz cxyz = i_this->current.pos;
    J3DModel* model;
    if (i_this->field_0x57e == 0) {
        model = i_this->mModel1;
        i_this->mBrk->play();

        for (int i = 0; i < 5; i++) {
            static u16 eff_id[] = {0x8204, 0x8205, 0x8206, 0x8207, 0x8208};
            i_this->field_0x6f8[i] = dComIfGp_particle_set(
                i_this->field_0x6f8[i], eff_id[i], &i_this->current.pos, &i_this->tevStr,
                &i_this->shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
        }

        s8 isHit = 0;
        if (i_this->mSph.ChkTgHit() != 0) {
            isHit = 1;
        }

        if (i_this->mSph.ChkCoHit() != 0) {
            cCcD_Obj* hitObj = i_this->mSph.GetCoHitObj();
            if (fopAcM_GetName(hitObj->GetAc()) == 0xef) {
                isHit = 1;
            }
        }

        if (i_this->field_0x580[0] == 0 && i_this->mSph.ChkAtHit() != 0) {
            i_this->field_0x580[0] = 10;
            fopAcM_seStart(i_this, 0x800a4, 0);
        }

        if (isHit) {
            i_this->field_0x57e = 1;
            u32 i_no = fopAcM_GetParam(i_this) >> 0x18;
            if (i_no != 0xff) {
                dComIfGs_onSwitch(i_no, fopAcM_GetRoomNo(i_this));
            }

            for (int i = 0; i < 2; i++) {
                static u16 eff_id[] = {0x8209, 0x820A};
                dComIfGp_particle_set(eff_id[i], &i_this->current.pos, &i_this->tevStr,
                                      &i_this->shape_angle, 0);
            }
            fopAcM_seStart(i_this, 0x800a5, 0);
            model = i_this->mModel2;
        }

        fopAcM_seStartLevel(i_this, 0x800a3, 0);

        if (i_this->field_0x57c != 1) {
            cxyz.x += -150.0f;
            i_this->mSph.SetR(250.0f);
        } else {
            i_this->mSph.SetR(50.0f);
        }

        i_this->mSph.SetC(cxyz);
        dComIfG_Ccsp()->Set(&i_this->mSph);

        if (i_this->mLightObj.mPow > 0.1f) {
            dKy_plight_cut(&i_this->mLightObj);
            i_this->mLightObj.mPow = 0.0f;
        }
    } else {
        model = i_this->mModel2;
        if (i_this->field_0x57c != 1) {
            cxyz.y += 20000.0f;
            i_this->field_0x6f8[0] = dComIfGp_particle_set(
                i_this->field_0x6f8[0], 0x820b, &i_this->current.pos, &i_this->tevStr,
                &i_this->shape_angle, 0, 0xff, 0, 0xffffffff, 0, 0, 0);
            fopAcM_seStartLevel(i_this, 0x800a6, 0);
        }
    }

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    fopAcM_SetMtx(i_this, model->getBaseTRMtx());
    i_this->eyePos = i_this->current.pos;

    return 1;
}

int daObj_Maki_IsDelete(obj_maki_class* i_this) {
    return true;
}

u8 lbl_567_bss_8;

int daObj_Maki_Delete(obj_maki_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Obj_maki");
    if (i_this->field_0x72c != 0) {
        lbl_567_bss_8 = 0;
    }

    if (i_this->mLightObj.mPow > 0.1f) {
        dKy_plight_cut(&i_this->mLightObj);
    }

    return 1;
}

int useHeapInit(fopAc_ac_c* i_this) {
    obj_maki_class* a_this = (obj_maki_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_maki", 4);
    a_this->mModel1 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (a_this->mModel1 == NULL) {
        return 0;
    }

    a_this->mBrk = new mDoExt_brkAnm();
    if (a_this->mBrk == NULL) {
        return 0;
    }

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Obj_maki", 8);
    if (!a_this->mBrk->init(a_this->mModel1->getModelData(), pbrk, TRUE, J3DFrameCtrl::EMode_LOOP,
                            1.0f, 0, -1))
    {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_maki", 5);
    a_this->mModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (a_this->mModel2 == NULL) {
        return 0;
    }

    return 1;
}

daObj_Maki_HIO_c l_HIO;

cPhs__Step daObj_Maki_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, obj_maki_class);
    obj_maki_class* a_this = (obj_maki_class*)i_this;
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x40000000, 0x3}, 0x75}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                                   // mGObjAt
            {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3},                                  // mGObjTg
            {0x0},                                                                // mGObjCo
        },                                                                        // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
        }  // mSphAttr
    };

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&a_this->mPhase, "Obj_maki");
    if (phase == cPhs_COMPLEATE_e) {
        a_this->field_0x57c = (u8)fopAcM_GetParam(a_this);
        u32 cVar3 = fopAcM_GetParam(a_this) >> 0x18;
        if (cVar3 != 0xff && dComIfGs_isSwitch(cVar3, fopAcM_GetRoomNo(a_this))) {
            a_this->field_0x57e = 1;
        }

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x1020)) {
            return cPhs_ERROR_e;
        }

        if (lbl_567_bss_8 == 0) {
            a_this->field_0x72c = 1;
            lbl_567_bss_8 = '\x01';
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMin(a_this, -500.0f, -200.0f, -500.0f);
        fopAcM_SetMax(a_this, 500.0f, 500.0f, 500.0f);
        a_this->mStts.Init(0xff, 0, a_this);
        a_this->mSph.Set(cc_sph_src);
        a_this->mSph.SetStts(&a_this->mStts);

        if (a_this->field_0x57c == 1) {
            a_this->mSph.SetTgType(0xd8fbfdff);
        }

        if (strcmp(dComIfGp_getStartStageName(), "F_SP118") != 0 && a_this->field_0x57e != 1) {
            a_this->mLightObj.mPosition = a_this->current.pos;
            a_this->mLightObj.mColor.r = 0xaf;
            a_this->mLightObj.mColor.g = 0x5d;
            a_this->mLightObj.mColor.b = 0;
            a_this->mLightObj.mPow = 500.0f;
            a_this->mLightObj.mFluctuation = 1.0f;
            dKy_plight_set(&a_this->mLightObj);
        }

        daObj_Maki_Execute(a_this);
    }
    return phase;
}

actor_method_class l_daObj_Maki_Method = {
    (process_method_func)daObj_Maki_Create,  (process_method_func)daObj_Maki_Delete,
    (process_method_func)daObj_Maki_Execute, (process_method_func)daObj_Maki_IsDelete,
    (process_method_func)daObj_Maki_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_MAKI = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_OBJ_MAKI,           // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(obj_maki_class),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    50,                      // mPriority
    &l_daObj_Maki_Method,    // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
