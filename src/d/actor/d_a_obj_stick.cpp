/**
 * @file d_a_obj_stick.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_stick.h"
#include "d/d_com_inf_game.h"
#include "d/d_resorce.h"
#include "m_Do/m_Do_ext.h"
#include "d/actor/d_a_npc.h"

const daObj_Stick_Param_c::daObj_Stick_HIOParam daObj_Stick_Param_c::m = {
    0.0f, -3.0f, 1.0f, 100.0f
};

dCcD_SrcSph daObj_Stick_c::mCcDSph = {
    daNpcT_c::mCcDObjData,                                               // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f}  // mSph
    }  // mSphAttr
};

static char* l_resName = "Taro6";

daObj_Stick_c::~daObj_Stick_c() {
    dComIfG_resDelete(&mPhase, getResName());
}

int daObj_Stick_c::create() {
    fopAcM_ct(this, daObj_Stick_c);

    mType = getType();
    
    int phase_state = dComIfG_resLoad(&mPhase, getResName());

    if (phase_state == cPhs_COMPLEATE_e) {
        if (isDelete()) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x810)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData;
        modelData = mpModel->getModelData();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -50.0, -50.0, -75.0, 50.0, 50.0, 75.0);
        mAcch.Set(fopAcM_GetPosition_p(this), 
            fopAcM_GetOldPosition_p(this), 
            this, 
            1,
            &mAcchCir, 
            fopAcM_GetSpeed_p(this), 
            fopAcM_GetAngle_p(this), 
            fopAcM_GetShapeAngle_p(this));
        mStts.Init(0xFF, 0, this);
        mSph.Set(daObj_Stick_c::mCcDSph);
        mSph.SetStts(&mStts);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundHeight = mAcch.GetGroundH();

        if(mGroundHeight != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }
        Execute();
    }

    return phase_state;
}

int daObj_Stick_c::CreateHeap() {
    J3DModelData* objectRes = (J3DModelData*)dComIfG_getObjectRes(getResName(), 3);
    if (objectRes == NULL) {
        return 0;
    }

    mpModel = mDoExt_J3DModel__create(objectRes, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    
    return 1;
}

int daObj_Stick_c::Delete() {
    this->~daObj_Stick_c();

    return 1;
}

int daObj_Stick_c::Execute() {
    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    
    mGroundHeight = mAcch.GetGroundH();
    if (mGroundHeight != -G_CM3D_F_INF) {
        setEnvTevColor();
        setRoomNo();
    }

    setMtx();
    mSph.ClrCoHit();
    attention_info.position = current.pos;
    attention_info.position.y += daObj_Stick_Param_c::m.attention_offset;
    eyePos = attention_info.position;
    attention_info.flags = 0;
    
    return 1;
}

int daObj_Stick_c::Draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);

    if(mGroundHeight != -G_CM3D_F_INF) {
        mShadowKey = dComIfGd_setShadow(mShadowKey, 
            1, 
            mpModel, 
            &current.pos, 
            daObj_Stick_Param_c::m.real_shadow_size, 
            20.0f, 
            current.pos.y,
            mGroundHeight, 
            mGndChk, 
            &tevStr, 
            0, 
            1.0, 
            dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

int daObj_Stick_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daObj_Stick_c*>(i_this)->CreateHeap();
}

const char* daObj_Stick_c::getResName() {
    return l_resName;
}

BOOL daObj_Stick_c::isDelete() {
    switch(mType) {
        case 0:
            return 0;
        default:
            return 0;
    }
}

void daObj_Stick_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_Stick_c::setRoomNo() {
    u32 roomId = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, roomId);
}

void daObj_Stick_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static u32 daObj_Stick_Create(void* i_this) {
    return static_cast<daObj_Stick_c*>(i_this)->create();
}

static void daObj_Stick_Delete(void* param_0) {
    static_cast<daObj_Stick_c*>(param_0)->Delete();
}

static void daObj_Stick_Execute(void* param_0) {
    static_cast<daObj_Stick_c*>(param_0)->Execute();
}

static void daObj_Stick_Draw(void* param_0) {
    static_cast<daObj_Stick_c*>(param_0)->Draw();
}

static bool daObj_Stick_IsDelete(void* param_0) {
    return true;
}

static actor_method_class daObj_Stick_MethodTable = {
    (process_method_func)daObj_Stick_Create,
    (process_method_func)daObj_Stick_Delete,
    (process_method_func)daObj_Stick_Execute,
    (process_method_func)daObj_Stick_IsDelete,
    (process_method_func)daObj_Stick_Draw,
};

actor_process_profile_definition g_profile_OBJ_STICK = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_OBJ_STICK,           // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daObj_Stick_c),    // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  85,                       // mPriority
  &daObj_Stick_MethodTable, // sub_method
  0x00044100,               // mStatus
  fopAc_ACTOR_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};

static daObj_Stick_Param_c l_HIO;
