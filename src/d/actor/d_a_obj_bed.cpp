/**
 * @file d_a_obj_bed.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bed.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_ext.h"
#include "d/actor/d_a_npc.h"
#include "d/d_procname.h"

static char* l_resName = "KH_Bed";

daObj_Bed_Param_c::params const daObj_Bed_Param_c::m = {NULL, -3.0f, 1.0f, 600.0f};

daObj_Bed_c::~daObj_Bed_c() {
    if (mpCollider != NULL) {
        dComIfG_Bgsp().Release(mpCollider);
    }
    dComIfG_resDelete(&mPhase, getResName());
}

cPhs__Step daObj_Bed_c::create() {
    fopAcM_ct(this, daObj_Bed_c);
    mType = getType();
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, getResName());
    if (step == cPhs_COMPLEATE_e) {
        if (isDelete()) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2520)) {
            return cPhs_ERROR_e;
        }
        if (mpCollider != NULL) {
            if (dComIfG_Bgsp().Regist(mpCollider, this) == true) {
                return cPhs_ERROR_e;
            }
        }
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 200.0f, 200.0f);
        mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir,
                  &speed, &current.angle, &shape_angle);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }
        Execute();
    }
    return step;
}

int daObj_Bed_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(getResName(), 4);
    if (model_data == NULL) {
        return 0;
    }
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    mpCollider = new dBgW();
    if (mpCollider == NULL) {
        return 0;
    }
    cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes(getResName(), 7);
    if (mpCollider->Set(dzb, 1, &mMtx) == true) {
        return 0;
    }
    mpCollider->SetCrrFunc(dBgS_MoveBGProc_Typical);
    return 1;
}

int daObj_Bed_c::Delete() {
    this->~daObj_Bed_c();
    return 1;
}

int daObj_Bed_c::Execute() {
    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    mGroundH = mAcch.GetGroundH();
    if (mGroundH != -G_CM3D_F_INF) {
        setEnvTevColor();
        setRoomNo();
    }
    setMtx();
    if (mpCollider != NULL) {
        PSMTXCopy(mpModel->getBaseTRMtx(), mMtx);
        mpCollider->Move();
    }
    eyePos = attention_info.position = current.pos;
    attention_info.flags = 0;
    return 1;
}

int daObj_Bed_c::Draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    if (mGroundH != -G_CM3D_F_INF) {
        mShadow = dComIfGd_setShadow(mShadow, 1, mpModel, &current.pos,
                                     daObj_Bed_Param_c::m.field_0xc, 20.0f,
                                     current.pos.y, mGroundH, mGndChk, &tevStr, 0,
                                     1.0f, &dDlst_shadowControl_c::mSimpleTexObj);
    }
    return 1;
}

int daObj_Bed_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daObj_Bed_c*>(i_this)->CreateHeap();
}

u8 daObj_Bed_c::getType() {
    return 0;
}

char* daObj_Bed_c::getResName() {
    return l_resName;
}

int daObj_Bed_c::isDelete() {
    switch (mType) {
        case 0:
            return daNpcT_chkEvtBit(0x108) == 0;
        default:
            return 0;
    }
}

void daObj_Bed_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_Bed_c::setRoomNo() {
    fopAcM_SetRoomNo(this, dComIfG_Bgsp().GetRoomId(mGndChk));
}

void daObj_Bed_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}


static cPhs__Step daObj_Bed_Create(void* i_this) {
    return static_cast<daObj_Bed_c*>(i_this)->create();
}

static int daObj_Bed_Delete(void* i_this) {
    return static_cast<daObj_Bed_c*>(i_this)->Delete();
}

static int daObj_Bed_Execute(void* i_this) {
    return static_cast<daObj_Bed_c*>(i_this)->Execute();
}

static int daObj_Bed_Draw(void* i_this) {
    return static_cast<daObj_Bed_c*>(i_this)->Draw();
}

static int daObj_Bed_IsDelete(void* param_0) {
    return 1;
}

static daObj_Bed_Param_c l_HIO;

static actor_method_class daObj_Bed_MethodTable = {
    (process_method_func)daObj_Bed_Create,
    (process_method_func)daObj_Bed_Delete,
    (process_method_func)daObj_Bed_Execute,
    (process_method_func)daObj_Bed_IsDelete,
    (process_method_func)daObj_Bed_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_BED = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_OBJ_BED,
    &g_fpcLf_Method.base,
    sizeof(daObj_Bed_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x4F,
    &daObj_Bed_MethodTable,
    0x44100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
