/**
 * @file d_a_bullet.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_bullet.h"
#include "d/d_com_inf_game.h"

static char* l_resFileNameList[] = {"Hanjo1"};

static char* l_bmdFileNameList[] = {"hanjo_stone.bmd"};

static daBullet_Param_c l_HIO;

const dCcD_SrcGObjInf daBullet_c::mCcDObjInfo = {
    {0, {{0, 0, 0}, {0x0, 0x0}, {0x79}}},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {0},
};

dCcD_SrcSph daBullet_c::mCcDSph = {
    daBullet_c::mCcDObjInfo,
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f}  // mSph
    }  // mSphAttr
};

daBullet_c::~daBullet_c() {
    dComIfG_resDelete(&mPhase, getResName());
}

int daBullet_c::create() {
    fopAcM_ct(this, daBullet_c);
    mType = getTypeFromParam();

    int phase_state = dComIfG_resLoad(&mPhase, getResName());
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x810)) {
            return cPhs_ERROR_e;
        }

        initialize();
    }

    return phase_state;
}

BOOL daBullet_c::CreateHeap() {
    void* mdlData_p = dComIfG_getObjectRes(getResName(), getBmdName());
    JUT_ASSERT(273, NULL != mdlData_p);
    mpModel = mDoExt_J3DModel__create((J3DModelData*)mdlData_p, 0x80000, 0x11000084);
    return mpModel != NULL;
}

int daBullet_c::Delete() {
    fopAcM_GetID(this);
    this->~daBullet_c();
    return 1;
}

daBullet_HIOParam const daBullet_Param_c::m = {
    -3.0f,
    255.0f,
    3.0f,
    1.5f,
    3.0f,
    0x708,
};

int daBullet_c::Execute() {
    gravity = daBullet_Param_c::m.gravity;

    if (field_0x957 == 0) {
        if (mProcess != NULL) {
            (this->*mProcess)(NULL);
        }

        fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundY = mAcch.GetGroundH();
    } else {
        mGroundY = -G_CM3D_F_INF;
    }

    int room_no;
    if (mGroundY != -G_CM3D_F_INF) {
        room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
    }

    tevStr.room_no = room_no;
    fopAcM_SetRoomNo(this, tevStr.room_no);
    attention_info.flags = 0;
    attention_info.position = current.pos;
    eyePos = attention_info.position;

    if (field_0x957 == 0) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::scaleM(scale);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    field_0x957 = 0;
    return 1;
}

int daBullet_c::Draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    return 1;
}

int daBullet_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daBullet_c*)i_this)->CreateHeap();
}

const char* daBullet_c::getResName() {
    return l_resFileNameList[mType];
}

const char* daBullet_c::getBmdName() {
    return l_bmdFileNameList[mType];
}

int daBullet_c::getTypeFromParam() {
    u8 prm = fopAcM_GetParam(this);
    switch (prm) {
    case 0:
        return 0;
    }

    return 0;
}

static f32 dummyLiteral() {
    f32 rt = 0.1f;
    rt += 0.5f;
    return rt;
}

static f32 dummyLiteral2() {
    return 0.0f;
}

void daBullet_c::initialize() {
    mpModel->getModelData();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -10.0f, -10.0f, -10.0f, 10.0f, 10.0f, 10.0f);

    mAcchCir.SetWall(daBullet_Param_c::m.width, daBullet_Param_c::m.knee_height);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));

    mCcStts.Init(daBullet_Param_c::m.weight, 0, this);
    mCcSph.Set(mCcDSph);
    mCcSph.SetStts(&mCcStts);
    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    mGroundY = mAcch.GetGroundH();

    setProcess(&daBullet_c::wait);
    mLifetime = daBullet_Param_c::m.lifetime;
    Execute();
}

int daBullet_c::setProcess(daBullet_c::processFn i_process) {
    mMode = 2;
    if (mProcess != NULL) {
        (this->*mProcess)(NULL);
    }

    mMode = 0;
    mProcess = i_process;

    if (mProcess != NULL) {
        (this->*mProcess)(NULL);
    }

    return 1;
}

int daBullet_c::wait(void*) {
    switch (mMode) {
    case 0:
        mMode = 1;
        break;
    case 1:
        if (cM3d_IsZero(speedF) == false) {
            setProcess(&daBullet_c::move);
        }
        break;
    case 2:
        break;
    }

    return 1;
}

int daBullet_c::move(void*) {
    switch (mMode) {
    case 0:
        mMode = 1;
        break;
    case 1:
        if (cLib_calcTimer<int>(&mLifetime) == 0) {
            fopAcM_delete(this);
        } else if (mAcch.ChkGroundHit() || mAcch.ChkWallHit()) {
            fopAcM_delete(this);
        }
        break;
    case 2:
        break;
    }

    return 1;
}

static int daBullet_Create(void* i_this) {
    return ((daBullet_c*)i_this)->create();
}

static int daBullet_Delete(void* i_this) {
    return ((daBullet_c*)i_this)->Delete();
}

static int daBullet_Execute(void* i_this) {
    return ((daBullet_c*)i_this)->Execute();
}

static int daBullet_Draw(void* i_this) {
    return ((daBullet_c*)i_this)->Draw();
}

static int daBullet_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daBullet_MethodTable = {
    (process_method_func)daBullet_Create,
    (process_method_func)daBullet_Delete,
    (process_method_func)daBullet_Execute,
    (process_method_func)daBullet_IsDelete,
    (process_method_func)daBullet_Draw,
};

actor_process_profile_definition g_profile_BULLET = {
  fpcLy_CURRENT_e,        // mLayerID
  8,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_BULLET,            // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daBullet_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  751,                    // mPriority
  &daBullet_MethodTable,  // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
