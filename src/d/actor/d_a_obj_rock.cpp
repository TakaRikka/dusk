/**
 * @file d_a_obj_rock.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_rock.h"

#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

static int daObj_Rock_Draw(obj_rock_class* i_this) {
    int room = fopAcM_GetRoomNo(i_this);
    rock_ss* rock = i_this->mRocks;

    for (int i = 0; i < i_this->mRockNum; i++, rock++) {
        if (rock->mActive) {
            dComIfGp_entrySimpleModel(rock->mpModel, room);
        }
    }

    return TRUE;
}

static int daObj_Rock_Execute(obj_rock_class* i_this) {
    if (i_this->mPatchInitialized == FALSE) {
        rock_ss* rock = i_this->mRocks;
        for (int i = 0; i < i_this->mRockNum; i++, rock++) {
            if (rock->mActive) {
                mDoMtx_trans(mDoMtx_stack_c::get(), rock->mPos.x, rock->mPos.y + rock->mOffsetY,
                             rock->mPos.z);
                mDoMtx_YrotM(mDoMtx_stack_c::get(), rock->mRot);
                mDoMtx_stack_c::scaleM(rock->mScale.x, rock->mScale.y, rock->mScale.z);

                rock->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

                rock->mCollider.SetC(rock->mPos);
                rock->mCollider.SetR(rock->mScale.x * 100.0f);
            }
        }
        i_this->mPatchInitialized = TRUE;
    } else {
        rock_ss* rock = i_this->mRocks;
        for (int i = 0; i < i_this->mRockNum; i++, rock++) {
            dComIfG_Ccsp2().Set(&rock->mCollider);
        }
    }
    return TRUE;
}

static bool daObj_Rock_IsDelete(obj_rock_class* i_this) {
    return true;
}

static int daObj_Rock_Delete(obj_rock_class* i_this) {
    if (i_this->mInitialized) {
        J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("Obj_rock", 3);
        dComIfGp_removeSimpleModel(model_data, fopAcM_GetRoomNo(i_this));
    }
    dComIfG_resDelete(&i_this->mPhaseReq, "Obj_rock");
    return TRUE;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    obj_rock_class* a_this = static_cast<obj_rock_class*>(i_this);

    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("Obj_rock", 3);

    for (int i = 0; i < a_this->mRockNum; i++) {
        a_this->mRocks[i].mpModel = mDoExt_J3DModel__create(model_data, 0x20000, 0x11000084);
        if (a_this->mRocks[i].mpModel == NULL) {
            return FALSE;
        }
    }

    return TRUE;
}

static int set_pos_check(obj_rock_class* i_this, int target_index) {
    for (int i = 0; i < target_index; i++) {
        cXyz dir = i_this->mRocks[i].mPos - i_this->mRocks[target_index].mPos;
        if (dir.abs() < 200.0f) {
            return FALSE;
        }
    }

    return TRUE;
}

static dCcD_SrcSph cc_sph_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x11}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                  // mGObjAt
        {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3},                 // mGObjTg
        {0x0},                                               // mGObjCo
    },                                                       // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
    }                                // mSphAttr
};

static int daObj_Rock_Create(fopAc_ac_c* i_this) {
    obj_rock_class* a_this = static_cast<obj_rock_class*>(i_this);
    fopAcM_ct(a_this, obj_rock_class);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&a_this->mPhaseReq, "Obj_rock");
    if (step == cPhs_COMPLEATE_e) {
        a_this->mPatchRockNum = fopAcM_GetParam(a_this);
        a_this->mRockSpacing = fopAcM_GetParamBit(a_this, 8, 8);
        a_this->field_0x572 = fopAcM_GetParamBit(a_this, 16, 8);

        if (a_this->field_0x572 == 0xFF) {
            a_this->field_0x572 = 0;
        }

        // Unclear why a separate counter is used here, or why it adds 1
        a_this->mRockNum = a_this->mPatchRockNum + 1;
        if (a_this->mRockNum > 16) {
            a_this->mRockNum = 16;
        }

        if (!fopAcM_entrySolidHeap(i_this, (heapCallbackFunc)useHeapInit, 0x4B000)) {
            return cPhs_ERROR_e;
        } else {
            MtxTrans(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z, 0);

            cXyz init_pos(0.0f, 0.0f, 0.0f);
            dBgS_GndChk gnd_chk;
            dBgS_ObjGndChk_Spl obj_gnd_chk;

            int count = 0;
            cM_initRnd2(0x10, 0x7b, 0x50);

            cXyz pos = i_this->current.pos;
            pos.y += 100.0f;
            obj_gnd_chk.SetPos((Vec*)&pos);

            f32 gnd_height = dComIfG_Bgsp().GroundCross(&obj_gnd_chk);
            for (int i = 0; i < a_this->mRockNum; i++) {
                MtxPush();
                mDoMtx_YrotM(*calc_mtx, cM_rndF2(65536.0f));
                MtxPosition(&init_pos, &a_this->mRocks[i].mPos);
                MtxPull();

                pos.x = a_this->mRocks[i].mPos.x;
                pos.y = a_this->mRocks[i].mPos.y + 100.0f;
                pos.z = a_this->mRocks[i].mPos.z;
                gnd_chk.SetPos((Vec*)&pos);

                a_this->mRocks[i].mPos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

                if (set_pos_check(a_this, i) && a_this->mRocks[i].mPos.y < gnd_height) {
                    a_this->mRocks[i].mActive = 1;
                    a_this->mRocks[i].mScale.x = cM_rndF2(0.4f) + 0.5f;
                    a_this->mRocks[i].mScale.z = cM_rndF2(0.4f) + 0.5f;
                    a_this->mRocks[i].mScale.y = cM_rndF2(0.4f) + 0.5f;
                    a_this->mRocks[i].mOffsetY = -cM_rndF2(70.0f);
                    a_this->mRocks[i].mRot = cM_rndF2(65536.0f);

                } else {
                    count++;
                    i--;
                    if (count > 10000) {
                        return cPhs_ERROR_e;
                    }
                }

                init_pos.z = cM_rndF2(1.0f);
                init_pos.z = 1.0f - (init_pos.z * init_pos.z);
                init_pos.z *= 100.0f * a_this->mRockSpacing;
            }

            a_this->mStts.Init(0xff, 0, i_this);

            for (int i = 0; i < a_this->mRockNum; i++) {
                a_this->mRocks[i].mCollider.Set(cc_sph_src);
                a_this->mRocks[i].mCollider.SetStts(&a_this->mStts);
            }

            daObj_Rock_Execute(a_this);

            J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("Obj_rock", 3);
            dComIfGp_addSimpleModel(model_data, fopAcM_GetRoomNo(i_this), 0);

            a_this->mInitialized = 1;
        }
    }

    return step;
}

rock_ss::~rock_ss() {}

rock_ss::rock_ss() {}

static actor_method_class l_daObj_Rock_Method = {
    (process_method_func)daObj_Rock_Create, (process_method_func)daObj_Rock_Delete,
    (process_method_func)daObj_Rock_Execute, (process_method_func)daObj_Rock_IsDelete,
    (process_method_func)daObj_Rock_Draw};

extern actor_process_profile_definition g_profile_OBJ_ROCK = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_OBJ_ROCK,           // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(obj_rock_class),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    710,                     // mPriority
    &l_daObj_Rock_Method,    // sub_method
    0x40000,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
