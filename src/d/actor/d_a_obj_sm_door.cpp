/**
 * @file d_a_obj_sm_door.cpp
 * Object - Sacred Meadow Door
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_sm_door.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_e_pm.h"

static char* l_arcName = "sm_door";

static cPhs__Step daObjSM_DOOR_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjSM_DOOR_c*>(i_this)->create();
}

static int daObjSM_DOOR_Delete(daObjSM_DOOR_c* i_this) {
    i_this->MoveBGDelete();
    return 1;
}

static void* s_obj_sub(void* i_actor, void* i_this) {
    fopAc_ac_c* actor = static_cast<fopAc_ac_c*>(i_actor);
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    cXyz& actor_pos = fopAcM_GetPosition(actor);
    cXyz& pos = fopAcM_GetPosition(_this);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_PM
        && actor_pos.absXZ(pos) < 800.0f)
    {
        return i_actor;
    }
    return NULL;
}

void daObjSM_DOOR_c::ChangeModel() {
    mUseAlphaModel = true;
}

f32 dummy_literal() {
    return 1000.0f;
}

void daObjSM_DOOR_c::Action() {
    switch (mMode) {
    case 0: {
        daE_PM_c* skullkid = (daE_PM_c*)fpcM_Search(s_obj_sub, this);
        if (skullkid != NULL) {
            if (skullkid->DoorChk() == 2) {
                ChangeModel();
                mMode++;
            }
            if (skullkid->DoorChk() == 5) {
                fopAcM_delete(this);
                fopAcM_delete(skullkid);
            }
        }
        break;
    }

    case 1: {
        daE_PM_c* skullkid = (daE_PM_c*)fpcM_Search(s_obj_sub, this);
        if (skullkid != NULL) {
            if (current.pos.absXZ(fopAcM_GetPosition(skullkid)) < 600.0f) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_PM_WALL_FADEAWAY, &current.pos,
                                         0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                mMode++;
                ReleaceDzb();
            }
            if (skullkid->DoorChk() == 5) {
                fopAcM_delete(this);
                fopAcM_delete(skullkid);
            }
        }
        break;
    }

    case 2:
        mAlpha -= 3.0f;
        if (mAlpha < 4) {
            fopAcM_delete(this);
        }
        break;
    }
}

int daObjSM_DOOR_c::SetDzb() {
    if (mpBgW != NULL && dComIfG_Bgsp().Regist(mpBgW, this)) {
        return 0;
    }
    return 1;
}

int daObjSM_DOOR_c::ReleaceDzb() {
    if (mpBgW != NULL && mpBgW->ChkUsed() && dComIfG_Bgsp().Release(mpBgW)) {
        return 0;
    }
    return 1;
}

void daObjSM_DOOR_c::DrawChk1() {
    if (mSwBit != 0xff && dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this)) && mActive == false) {
        if (SetDzb()) {
            mActive = true;
        }
    } else if (mSwBit != 0xff && !dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))
                              && mActive == true) {
        if (ReleaceDzb()) {
            mActive = false;
        }
    }
}

void daObjSM_DOOR_c::DrawChk2() {
    if (mSwBit != 0xff && !dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this)) && mActive == false) {
        if (SetDzb()) {
            mActive = true;
        }
    } else if (mSwBit != 0xff && dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))
                              && mActive == true) {
        if (ReleaceDzb()) {
            mActive = false;
        }
    }
}

void daObjSM_DOOR_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    if (!mUseAlphaModel) {
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        mpAlphaModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

static int daObjSM_DOOR_Draw(daObjSM_DOOR_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjSM_DOOR_Execute(daObjSM_DOOR_c* i_this) {
    return i_this->MoveBGExecute();
}

inline int daObjSM_DOOR_c::CreateHeap() {
    J3DModelData* model_data;
    J3DModelData* alpha_model_data;

    if (mType == 0) {
        model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "A_SM_DoorA.bmd");
        alpha_model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "A_SM_DoorA_Alpha.bmd");
    } else if (mType == 1) {
        model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "A_SM_DoorBl.bmd");
        alpha_model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "A_SM_DoorBl_Alpha.bmd");
    } else if (mType == 2) {
        model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "A_SM_DoorWo.bmd");
        alpha_model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "A_SM_DoorWo_Alpha.bmd");
    }

    mpAlphaModel = mDoExt_J3DModel__create(alpha_model_data, 0x80000, 0x11000084);
    if (mpAlphaModel == NULL) {
        return 0;
    }

    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

cPhs__Step daObjSM_DOOR_c::create() {
    fopAcM_ct(this, daObjSM_DOOR_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);

    if (step == cPhs_COMPLEATE_e) {
        gravity = -9.0f;
        mActive = true;
        mType = fopAcM_GetParam(this) & 0xF;
        mSwType = (fopAcM_GetParam(this) >> 4) & 0xF;
        mUseAlphaModel = false;

        int dzb_id;
        if (mType == 0) {
            mType = 0;
            dzb_id = dComIfG_getObjctResName2Index(l_arcName, "A_SM_DoorA.dzb");
        } else if (mType == 1) {
            mType = 1;
            dzb_id = dComIfG_getObjctResName2Index(l_arcName, "A_SM_DoorBl.dzb");
        } else if (mType == 2) {
            mType = 2;
            dzb_id = dComIfG_getObjctResName2Index(l_arcName, "A_SM_DoorWo.dzb");
        } else {
            mType = 0;
            dzb_id = dComIfG_getObjctResName2Index(l_arcName, "A_SM_DoorA.dzb");
        }

        mSwBit = fopAcM_GetParam(this) >> 8;

        step = (cPhs__Step)MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY,
                                        0x1F20, NULL);
        if (step == cPhs_ERROR_e) {
            return step;
        }

        fopAcM_SetMtx(this, mpAlphaModel->getBaseTRMtx());
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        mAlpha = 0xff;
        cXyz pos(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        if (fopAcM_gc_c::gndCheck(&pos)) {
            current.pos.y = fopAcM_gc_c::getGroundY();
        }
        fopAcM_setCullSizeBox(this, -1000.0f, -500.0f, -1000.0f, 1000.0f, 500.0f, 1000.0f);

        daObjSM_DOOR_Execute(this);
    }

    return step;
}

static int daObjSM_DOOR_IsDelete(daObjSM_DOOR_c* i_this) {
    return 1;
}

inline int daObjSM_DOOR_c::Create() {
    fopAcM_setCullSizeBox(this, -1000.0f, -500.0f, -1000.0f, 1000.0f, 500.0f, 1000.0f);
    return cPhs_COMPLEATE_e;
}

inline int daObjSM_DOOR_c::Execute(Mtx** i_mtxP) {
    if (mSwType == 0xF) {
        DrawChk1();
    } else if (mSwType == 0) {
        DrawChk2();
    }

    Action();
    *i_mtxP = &mBgMtx;
    setBaseMtx();
    return 1;
}

inline int daObjSM_DOOR_c::Draw() {
    if (mActive) {
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);

        if (!mUseAlphaModel) {
            g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
            dComIfGd_setListDarkBG();
            J3DModelData* model_data = mpModel->getModelData();
            for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
                model_data->getMaterialNodePointer(i)->getTevKColor(3)->a = mAlpha;
            }
            mDoExt_modelUpdateDL(mpModel);
            for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
                model_data->getMaterialNodePointer(i)->getTevKColor(0)->a = 0xFF;
            }
        } else {
            g_env_light.setLightTevColorType_MAJI(mpAlphaModel, &tevStr);
            dComIfGd_setListDarkBG();
            J3DModelData* model_data = mpAlphaModel->getModelData();
            for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
                model_data->getMaterialNodePointer(i)->getTevKColor(3)->a = mAlpha;
            }
            mDoExt_modelUpdateDL(mpAlphaModel);
            for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
                model_data->getMaterialNodePointer(i)->getTevKColor(0)->a = 0xFF;
            }
        }

        dComIfGd_setList();
    }

    return 1;
}

inline int daObjSM_DOOR_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static actor_method_class l_daObjSM_DOOR_Method = {
    (process_method_func)daObjSM_DOOR_Create,
    (process_method_func)daObjSM_DOOR_Delete,
    (process_method_func)daObjSM_DOOR_Execute,
    (process_method_func)daObjSM_DOOR_IsDelete,
    (process_method_func)daObjSM_DOOR_Draw,
};

extern actor_process_profile_definition g_profile_Obj_SM_DOOR = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_SM_DOOR,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjSM_DOOR_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  476,                    // mPriority
  &l_daObjSM_DOOR_Method, // sub_method
  0x00044100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
