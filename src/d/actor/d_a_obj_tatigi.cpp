/**
 * @file d_a_obj_tatigi.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_tatigi.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_s_play.h"
#include "d/d_com_inf_game.h"

static int daObj_Tatigi_Draw(obj_tatigi_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mBase;
    int roomNo = fopAcM_GetRoomNo(a_this);
    tg_ss* pTg = i_this->mTgs;
    for (int i = 0; i < i_this->mTgNum; i++, pTg++) {
        if (pTg->mActive != 0) {
            dComIfGp_entrySimpleModel(pTg->mModel, roomNo);
        }
    }
    return 1;
}

static int daObj_Tatigi_Execute(obj_tatigi_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mBase;
    i_this->field_0x574++;
    if (i_this->field_0x576 == 0) {
        tg_ss* pTg = i_this->mTgs;
        for (s32 i = 0; i < i_this->mTgNum; i++, pTg++) {
            if (pTg->mActive != 0) {
                mDoMtx_stack_c::transS(pTg->mPos.x, pTg->mPos.y, pTg->mPos.z);
                mDoMtx_stack_c::YrotM(pTg->mAngle);
                mDoMtx_stack_c::scaleM(pTg->mScale.x, pTg->mScale.y, pTg->mScale.z);
                pTg->mModel->setBaseTRMtx(mDoMtx_stack_c::get());
                pTg->mCyl.SetC(pTg->mPos);
                pTg->mCyl.SetR(pTg->mScale.x * (TREG_F(16) + 30.0f));
                pTg->mCyl.SetH(TREG_F(17) + 500.0f);
            }
        }
        i_this->field_0x576 = 1;
    } else {
        tg_ss* pTg = i_this->mTgs;
        s16 xRot;
        s16 zRot;
        int i;
        for (i = 0; i < i_this->mTgNum; i++, pTg++) {
            dComIfG_Ccsp()->Set(&pTg->mCyl);
            if (pTg->field_0x1e == 0 && pTg->mCyl.ChkCoHit()) {
                fopAc_ac_c* hitActor = dCc_GetAc(pTg->mCyl.GetCoHitObj()->GetAc());
                if (hitActor != NULL && hitActor->speedF >= 5.0f) {
                    pTg->field_0x1e = 20;
                }
            }
            if (pTg->field_0x1e != 0) {
                pTg->field_0x1e--;
                xRot = ((TREG_F(18) + 7.0f) * (pTg->field_0x1e * cM_ssin(i_this->field_0x574 * (WREG_S(3) + 5000))));
                zRot = ((TREG_F(18) + 7.0f) * (pTg->field_0x1e * cM_ssin(i_this->field_0x574 * (WREG_S(4) + 6000))));
                mDoMtx_stack_c::transS(pTg->mPos.x, pTg->mPos.y, pTg->mPos.z);
                mDoMtx_stack_c::YrotM(pTg->mAngle);
                mDoMtx_stack_c::XrotM(xRot);
                mDoMtx_stack_c::ZrotM(zRot);
                mDoMtx_stack_c::scaleM(pTg->mScale.x, pTg->mScale.y, pTg->mScale.z);
                pTg->mModel->setBaseTRMtx(mDoMtx_stack_c::get());
            }
        }
    }
    return 1;
}

static int daObj_Tatigi_IsDelete(obj_tatigi_class* param_0) {
    return 1;
}

static int daObj_Tatigi_Delete(obj_tatigi_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mBase;
    fopAcM_RegisterDeleteID(i_this, "Obj_Tatigi");
    if (i_this->mModelLoaded) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_tg", 3);
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(a_this));
    }
    dComIfG_resDelete(&i_this->mPhase, "Obj_tg");
    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_tatigi_class* i_this = (obj_tatigi_class*)a_this;
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_tg", 3);
    JUT_ASSERT(279, modelData != NULL);
    for (int i = 0; i < i_this->mTgNum; i++) {
        i_this->mTgs[i].mModel = mDoExt_J3DModel__create(modelData, 0x20000, 0x11000084);
        if (i_this->mTgs[i].mModel == NULL) {
            return 0;
        }
    }
    return 1;
}

static BOOL set_pos_check(obj_tatigi_class* i_this, int param_2) {
    cXyz cStack_2c;
    f32 dVar7 = KREG_F(7) + 150.0f;
    for (int i = 0; i < param_2; i++) {
        cStack_2c = i_this->mTgs[i].mPos - i_this->mTgs[param_2].mPos;
        if (cStack_2c.abs() < dVar7) {
            return 0;
        }
    }
    return TRUE;
}

static int daObj_Tatigi_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcCyl cc_cyl_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x11}, 0x79}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            35.0f, // mRadius
            120.0f // mHeight
        } // mCyl
    };

    obj_tatigi_class* i_this = (obj_tatigi_class*)a_this;
    fopAcM_ct(&i_this->mBase, obj_tatigi_class);

    int rv = dComIfG_resLoad(&i_this->mPhase, "Obj_tg");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_TATIGI PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->field_0x570 = fopAcM_GetParam(a_this) & 0xff;
        i_this->field_0x571 = (fopAcM_GetParam(a_this) & 0xff00) >> 8;
        i_this->field_0x572 = (fopAcM_GetParam(a_this) & 0xff0000) >> 16;
        if (i_this->field_0x572 == 0xff) {
            i_this->field_0x572 = 0;
        }

        i_this->mTgNum = i_this->field_0x570 + 1;
        if (i_this->mTgNum > 16) {
            i_this->mTgNum = 16;
        }

        OS_REPORT("OBJ_TATIGI//////////////OBJ_TATIGI SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x4b000)) {
            OS_REPORT("//////////////OBJ_TATIGI SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_TATIGI SET 2 !!\n");

        MtxTrans(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z, 0);
        cXyz cStack_80(0.0f, 0.0f, 0.0f);
        Vec local_8c;
        dBgS_GndChk gndCheck;
        int local_94 = 0;
        cM_initRnd2(13, 123, 33);
        for (int i = 0; i < i_this->mTgNum; i++) {
            MtxPush();
            cMtx_YrotM(*calc_mtx, cM_rndF2(65536.0f));
            MtxPosition(&cStack_80, &i_this->mTgs[i].mPos);
            MtxPull();
            local_8c.x = i_this->mTgs[i].mPos.x;
            local_8c.y = i_this->mTgs[i].mPos.y + 100.0f;
            local_8c.z = i_this->mTgs[i].mPos.z;
            gndCheck.SetPos(&local_8c);
            i_this->mTgs[i].mPos.y = dComIfG_Bgsp().GroundCross(&gndCheck);
            i_this->mTgs[i].mAngle = cM_rndF2(65536.0f);
            if (set_pos_check(i_this, i)) {
                i_this->mTgs[i].mActive = 1;
                i_this->mTgs[i].mScale.x = i_this->mTgs[i].mScale.z = cM_rndF2(0.4f) + 0.6f;
                i_this->mTgs[i].mScale.y = cM_rndF2(0.4f) + 0.6f;
            } else {
                i--;
                local_94++;
                if (local_94 > 10000) {
                    // The designated area for the standing tree is too narrow to place it.
                    OS_REPORT("      立ち木の指定範囲が狭すぎて置けません！！！！\n");
                    return cPhs_ERROR_e;
                }
            }
            cStack_80.z = cM_rndF2(1.0f);
            cStack_80.z = 1.0f - cStack_80.z * cStack_80.z;
            cStack_80.z = cStack_80.z * (i_this->field_0x571 * 100.0f);
        }

        i_this->field_0x574 = cM_rndF(65535.0f);
        i_this->mStts.Init(0xff, 0, a_this);
        for (int i = 0; i < i_this->mTgNum; i++) {
            i_this->mTgs[i].mCyl.Set(cc_cyl_src);
            i_this->mTgs[i].mCyl.SetStts(&i_this->mStts);
        }
        daObj_Tatigi_Execute(i_this);
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_tg", 3);
        JUT_ASSERT(476, modelData != NULL);
        if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(a_this), 0) == -1) {
            // Standing tree: simple model registration failed.
            OS_REPORT("\x1B" "[43;30m立ち木：シンプルモデル登録失敗しました。\n" "\x1B" "[m");
        }
        i_this->mModelLoaded = true;
    }
    return rv;
}

static actor_method_class l_daObj_Tatigi_Method = {
    (process_method_func)daObj_Tatigi_Create,
    (process_method_func)daObj_Tatigi_Delete,
    (process_method_func)daObj_Tatigi_Execute,
    (process_method_func)daObj_Tatigi_IsDelete,
    (process_method_func)daObj_Tatigi_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_TATIGI = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_OBJ_TATIGI,          // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(obj_tatigi_class), // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  709,                      // mPriority
  &l_daObj_Tatigi_Method,   // sub_method
  0x00040000,               // mStatus
  fopAc_ACTOR_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
