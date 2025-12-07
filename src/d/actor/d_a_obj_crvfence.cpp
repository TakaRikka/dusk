/**
 * @file d_a_obj_crvfence.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_crvfence.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_e_wb.h"

enum CrvFence_RES_File_ID {
    /* BMDG */
    /* 0x5 */ BMDG_CARAVANPIECE = 0x5,

    /* BMDR */
    /* 0x8 */ BMDR_CARAVANFENCE = 0x8,
    /* 0x9 */ BMDR_U_CARAVANGATE_PIECEB,
    /* 0xA */ BMDR_U_CRVFENCE_BBREAKA,
    /* 0xB */ BMDR_U_CRVFENCE_BBREAKB,

    /* DZB */
    /* 0xE */ DZB_CARAVANFENCE = 0xE,
};

enum Action {
    /* 0x0 */ ACTION_NORMAL,
    /* 0x1 */ ACTION_BROKEN,
};

static char* l_arcName = "CrvFence";

BOOL daObjCRVFENCE_c::CheckVec() {
    cXyz vec(fopAcM_GetPosition((fopAc_ac_c*)daPy_getPlayerActorClass()));
    Mtx mtx;
    mDoMtx_inverse(field_0x65c->getBaseTRMtx(), mtx);
    mDoMtx_stack_c::copy(mtx);
    mDoMtx_stack_c::multVec(&vec, &vec);

    return vec.z > 0.0f;
}

BOOL daObjCRVFENCE_c::Wall_Check(cXyz i_start, cXyz param_2) {
    dBgS_LinChk lin_chk;
    cXyz i_end(i_start.x + param_2.x, i_start.y, i_start.z + param_2.z);
    lin_chk.Set(&i_start, &i_end, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk) == TRUE) {
        return TRUE;
    }

    return FALSE;
}

static int daObjCRVFENCE_Create(fopAc_ac_c* a_this) {
    return static_cast<daObjCRVFENCE_c*>(a_this)->create();
}

static int daObjCRVFENCE_Delete(daObjCRVFENCE_c* a_this) {
    static_cast<daObjCRVFENCE_c*>(a_this)->MoveBGDelete();
    return 1;
}

void daObjCRVFENCE_c::BgRelease() {
    if (mpBgW != NULL) {
        if (mpBgW->ChkUsed() && dComIfG_Bgsp().Release(mpBgW)) {
            OSReport("Release Error\n");
        }
    }
}

void daObjCRVFENCE_c::PosSet(cXyz* pos) {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(pos, pos);
}

void daObjCRVFENCE_c::FenceMove(u8& param_1, cXyz* param_2, csXyz* param_3, cXyz* param_4, s16& param_5) {
    if (param_1 <= 3) {
        f32 fVar1 = 20.0f;
        param_2->y += -9.0f;

        if (Wall_Check(*param_4, *param_2)) {
            param_2->x = -0.5f * param_2->x;
        }

        if (param_1 == 0) {
            cLib_chaseAngleS(&param_3->y, param_5, 0x700);
            fVar1 = 100.0f;
        } else {
            cLib_chaseAngleS(&param_3->x, param_5, 0x700);
        }

        *param_4 += *param_2;

        if (param_2->y < 0.0f && field_0x640 + fVar1 >= param_4->y) {
            param_1++;
            param_4->y = field_0x640 + fVar1;
            param_2->y = -(HREG_F(0) + 0.7f) * param_2->y;
            param_2->x = (HREG_F(0) + 0.7f) * param_2->x;
            param_2->z = (HREG_F(0) + 0.7f) * param_2->z;
        }
    }
}

void daObjCRVFENCE_c::BrokenAction() {
    switch (field_0x5a0) {
        case 0:
            field_0x5ac.set(180.0f, 180.0f, 0.0f);
            field_0x5d0.set(-180.0f, 180.0f, 0.0f);
            PosSet(&field_0x5d0);
            PosSet(&field_0x5ac);
            field_0x5f0 = -0x4000;
            field_0x5cc = 0x4000;
            field_0x5f2 = -field_0x5f0;

            if (field_0x644 < 3) {
                field_0x5b8.set(15.0f, 30.0f, 20.0f);
                field_0x5dc.set(-25.0f, 40.0f, 30.0f);
                mDoMtx_stack_c::YrotS(shape_angle.y);
                mDoMtx_stack_c::multVec(&field_0x5b8, &field_0x5b8);
                mDoMtx_stack_c::multVec(&field_0x5dc, &field_0x5dc);
            } else {
                field_0x5b8.set(25.0f, 40.0f, 30.0f);
                field_0x5dc.set(-15.0f, 30.0f, 20.0f);
                mDoMtx_stack_c::YrotS(shape_angle.y);
                mDoMtx_stack_c::multVec(&field_0x5b8, &field_0x5b8);
                mDoMtx_stack_c::multVec(&field_0x5dc, &field_0x5dc);
            }

            if (CheckVec()) {
                field_0x5b8.set(field_0x5b8.x, field_0x5b8.y, -field_0x5b8.z);
                field_0x5dc.set(field_0x5dc.x, field_0x5dc.y, -field_0x5dc.z);
                field_0x5f0 = 0x4000;
                field_0x5cc = -0x4000;
                field_0x5f2 = -field_0x5f0;
                field_0x5aa = -field_0x5aa;
            }

            field_0x5a0 = 1;
            break;
        
        case 1:
            if (field_0x5ee == 0) {
                FenceMove(field_0x5ee, &field_0x5dc, &field_0x5e8, &field_0x5d0, field_0x5f0);
            } else {
                FenceMove(field_0x5ee, &field_0x5dc, &field_0x5e8, &field_0x5d0, field_0x5f2);
            }

            FenceMove(field_0x5ca, &field_0x5b8, &field_0x5c4, &field_0x5ac, field_0x5cc);

            if (field_0x5ee > 3 && field_0x5ca > 3 && !checkViewArea(&field_0x5d0) && !checkViewArea(&field_0x5ac) && fopAcM_searchPlayerDistance(this) > 1000.0f) {
                if (field_0x5a8 != 0xFF && !dComIfGs_isSwitch(field_0x5a8, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(field_0x5a8, fopAcM_GetRoomNo(this));
                }

                fopAcM_delete(this);
            }
            break;

    }
}

static void* s_obj_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_searchActorDistance((fopAc_ac_c*)i_data, (fopAc_ac_c*)i_actor) < 600.0f && fopAcM_GetName(i_actor) == PROC_E_WB) {
        return i_actor;
    }

    return NULL;
}

void daObjCRVFENCE_c::NormalAction() {
    if (!field_0x5f4) {
        e_wb_class* wb_p = (e_wb_class*)fpcM_Search(s_obj_sub, this);
        if (wb_p != NULL && wb_p->mActionID == ACT_PL_RIDE2) {
            fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
            fopAcM_seStartCurrent(this, Z2SE_OBJ_SAKU_BREAK, 0);
            cXyz sp5c(fopAcM_GetPosition(&wb_p->mEnemy));
            Mtx mtx;
            mDoMtx_inverse(field_0x65c->getBaseTRMtx(), mtx);
            mDoMtx_stack_c::copy(mtx);
            mDoMtx_stack_c::multVec(&sp5c, &sp5c);

            if (sp5c.x < 0.0f) {
                field_0x644 = 1;
            } else {
                field_0x644 = 4;
            }

            BgRelease();
            BOOL bVar1 = CheckVec();
            field_0x5f4 = true;
            dComIfGp_getVibration().StartShock(5, 31, cXyz(0.0f, 1.0f, 0.0f));
            mAction = ACTION_BROKEN;
            cXyz sp68(150.0f, 300.0f, 30.0f);
            cXyz sp74(yREG_F(1) + -200.0f, yREG_F(2) + 50.0f, 0.0f);
            cXyz sp80(yREG_F(4) + 25.0f, yREG_F(0) + 120.0f, yREG_F(3) + 50.0f);
            cXyz sp8c(0.0f, 20.0f, 30.0f);

            if (bVar1) {
                sp8c.set(-sp8c.x, -sp8c.y, -sp8c.z);
                sp80.set(sp80.x, sp80.y, -sp80.z);
                field_0x5aa = shape_angle.y + 0x8000;
            }

            sp68.set(150.0f, 300.0f, 30.0f);
            sp74.set(200.0f - yREG_F(1), yREG_F(2) + 50.0f, 0.0f);
            sp80.set(yREG_F(4) + 25.0f, yREG_F(0) + 120.0f, yREG_F(3) + 50.0f);
            sp8c.set(0.0f, 20.0f, 30.0f);

            if (bVar1) {
                sp8c.set(-sp8c.x, -sp8c.y, -sp8c.z);
                sp80.set(sp80.x, sp80.y, -sp80.z);
            }

            csXyz angle(0, field_0x5aa, 0);
            dComIfGp_particle_set(0x8617, &current.pos, &tevStr, &angle, NULL);
            dComIfGp_particle_set(0x8618, &current.pos, &tevStr, &angle, NULL);
            dComIfGp_particle_set(0x8619, &current.pos, &tevStr, &angle, NULL);
            dComIfGp_particle_set(0x861A, &current.pos, &tevStr, &angle, NULL);
        }
    }
}

bool daObjCRVFENCE_c::checkViewArea(cXyz* param_1) {
    Vec sp24;
    mDoLib_project(param_1, &sp24);
    bool rv = false;
    bool bVar1 = false;

    #if DEBUG
    if (sp24.x >= 0.0f) {
        if (sp24.y <= FB_WIDTH) {
            bVar1 = true;
        }
    }

    if (bVar1) {
        bVar1 = false;

        if (sp24.y >= 0.0f) {
            if (sp24.y <= FB_HEIGHT) {
                bVar1 = true;
            }
        }

        if (bVar1) {
            rv = true;
        }
    }
    #else
    if (sp24.x >= 0.0f && sp24.x <= FB_WIDTH && sp24.y >= 0.0f && sp24.y <= FB_HEIGHT) {
        rv = true;
    }
    #endif

    return rv;
}

void daObjCRVFENCE_c::Action() {
    switch (mAction) {
        case ACTION_NORMAL:
            NormalAction();
            break;

        case ACTION_BROKEN:
            BrokenAction();
            break;
    }
}

void daObjCRVFENCE_c::setBaseMtx() {
    mDoMtx_stack_c::transS(field_0x5ac);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::ZXYrotM(field_0x5c4);
    mDoMtx_stack_c::scaleM(1.0f, 1.2f, 1.0f);
    field_0x660->setBaseTRMtx(mDoMtx_stack_c::get());
    
    mDoMtx_stack_c::transS(field_0x5d0);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::ZXYrotM(field_0x5e8);
    mDoMtx_stack_c::scaleM(1.0f, 1.2f, 1.0f);
    field_0x664->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    field_0x65c->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

static int daObjCRVFENCE_Draw(daObjCRVFENCE_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjCRVFENCE_Execute(daObjCRVFENCE_c* i_this) {
    return i_this->MoveBGExecute();
}

int daObjCRVFENCE_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "CaravanFence.bmd");

    JUT_ASSERT(88, modelData != NULL);

    field_0x65c = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (field_0x65c == NULL) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "U_CaravanGate_PieceB.bmd");

    JUT_ASSERT(99, modelData != NULL);

    field_0x660 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (field_0x660 == NULL) {
        return 0;
    }

    field_0x664 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (field_0x664 == NULL) {
        return 0;
    }

    return 1;
}

cPhs__Step daObjCRVFENCE_c::create() {
    fopAcM_ct(this, daObjCRVFENCE_c);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("Obj_Fence PARAM %x\n", fopAcM_GetParam(this));

        field_0x5a8 = fopAcM_GetParam(this);
        if (field_0x5a8 != 0xFF) {
            if (dComIfGs_isSwitch(field_0x5a8, fopAcM_GetRoomNo(this))) {
                OS_REPORT("ObjFence再セットしません\n");
                return cPhs_ERROR_e;
            }
        }

        gravity = nREG_F(0) + -9.0f;

        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "CaravanFence.dzb");

        if (dzb_id == -1) {
            // "dzb data not found!<%s>"
            OS_REPORT("dzbデータが見つかりませんでした!<%s>\n\n", l_arcName);

            JUT_ASSERT(783, dzb_id != -1)
        }

        phase = (cPhs__Step)MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x1CA0, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }

        mBgc.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        mAction = ACTION_NORMAL;
        fopAcM_SetMtx(this, field_0x65c->getBaseTRMtx());

        cXyz sp30(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        if (fopAcM_gc_c::gndCheck(&sp30)) {
            field_0x640 = fopAcM_gc_c::getGroundY();
        }

        field_0x5aa = shape_angle.y;
        fopAcM_setCullSizeFar(this, 3.0f);
        fopAcM_setCullSizeBox(this, -500.0f, 0.0f, -500.0f, 500.0f, 450.0f, 500.0f);
        daObjCRVFENCE_Execute(this);
    }

    return phase;
}

static int daObjCRVFENCE_IsDelete(daObjCRVFENCE_c* i_this) {
    return 1;
}

int daObjCRVFENCE_c::Create() {
    fopAcM_setCullSizeBox(this, -1000.0f, -500.0f, -1000.0f, 1000.0f, 500.0f, 1000.0f);
    return cPhs_COMPLEATE_e;
}

int daObjCRVFENCE_c::Execute(Mtx** mtx) {
    Action();
    *mtx = &mBgMtx;
    setBaseMtx();
    return 1;
}

int daObjCRVFENCE_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);

    if (field_0x5a0 == 1) {
        g_env_light.setLightTevColorType_MAJI(field_0x660, &tevStr);
        g_env_light.setLightTevColorType_MAJI(field_0x664, &tevStr);
    } else {
        g_env_light.setLightTevColorType_MAJI(field_0x65c, &tevStr);
    }

    dComIfGd_setListBG();

    if (field_0x5a0 == 1) {
        mDoExt_modelUpdateDL(field_0x660);
        mDoExt_modelUpdateDL(field_0x664);
    } else {
        mDoExt_modelUpdateDL(field_0x65c);
    }

    return 1;
}

int daObjCRVFENCE_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static actor_method_class l_daObjCRVFENCE_Method = {
    (process_method_func)daObjCRVFENCE_Create,
    (process_method_func)daObjCRVFENCE_Delete,
    (process_method_func)daObjCRVFENCE_Execute,
    (process_method_func)daObjCRVFENCE_IsDelete,
    (process_method_func)daObjCRVFENCE_Draw,
};

extern actor_process_profile_definition g_profile_Obj_CRVFENCE = {
  fpcLy_CURRENT_e,         // mLayerID
  3,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_CRVFENCE,       // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjCRVFENCE_c), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  467,                     // mPriority
  &l_daObjCRVFENCE_Method, // sub_method
  0x00040180,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
