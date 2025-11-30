/**
 * d_a_obj_ss_item.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ss_item.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "d/actor/d_a_myna.h"

static char* l_resFileName[] = {
    "Always",
    "Always",
    "Pumpkin",
};

static int l_bmdIdxName[] = {0x23, 0x23, 0x00};

static char* l_bmdFileName[] = {
    "",
    "",
    "pumpkin.bmd",
};

dCcD_SrcGObjInf const daObj_SSItem_c::mCcDObjInfo = {
    {0, {{0, 0, 0}, {0, 0}, {0x79}}},
    {0, 0, 0, 0, {0}},
    {0, 0, 0, 0, {0}},
    {{0}},
};

dCcD_SrcCyl daObj_SSItem_c::mCcDCyl = {mCcDObjInfo, {}};

daObj_SSItem_c::~daObj_SSItem_c() {
    dComIfG_resDelete(&mPhase, getResName());
}

int daObj_SSItem_c::create() {
    fopAcM_ct(this, daObj_SSItem_c);
    field_0xB0E = getTypeFromParam();
    setFlowNodeNumber(getFlowNodeNum());
    setValueNumber(getValue());
    field_0xAFC = 0;
    field_0xAF0.set(current.pos);
    field_0xB04 = -1;

    if (field_0xB0E == 2) {
        field_0xB04 = fopAcM_create(PROC_OBJ_PUMPKIN, 0x10000319, &current.pos,
                                    fopAcM_GetRoomNo(this), &current.angle, NULL, -1);
        return cPhs_COMPLEATE_e;
    } else {
        int retVal = dComIfG_resLoad(&mPhase, getResName());
        if (retVal == cPhs_COMPLEATE_e) {
            if (!fopAcM_entrySolidHeap(this, &createHeapCallBack, 0xEB0)) {
                return cPhs_ERROR_e;
            } else {
                initialize();
            }
        }
        return retVal;
    }
}

BOOL daObj_SSItem_c::CreateHeap() {
    J3DModelData* modelData;
    if (l_bmdIdxName[field_0xB0E] != 0) {
        modelData = static_cast<J3DModelData*>(
            dComIfG_getObjectRes(getResName(), l_bmdIdxName[field_0xB0E]));
    } else {
        modelData = static_cast<J3DModelData*>(
            dComIfG_getObjectRes(getResName(), l_bmdFileName[field_0xB0E]));
    }
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mpModel != NULL;
}

int daObj_SSItem_c::Delete() {
    this->~daObj_SSItem_c();
    return 1;
}

int daObj_SSItem_c::Execute() {
    if (field_0xB0E == 2) {
        return 1;
    } else {
        field_0xAFC = fopAcM_checkCarryNow(this);
        setParam();
        if (field_0xAFC != 0) {
            mAcch.ClrWallHit();
            mAcch.ClrGroundHit();
            speedF = 0.0f;
            speed.set(0.0f, 0.0f, 0.0f);
        } else {
            mStts.Move();
            if (mAtCyl.ChkCoHit() != 0) {
                field_0xA8C = *mStts.GetCCMoveP();
            } else {
                field_0xA8C.set(0.0f, 0.0f, 0.0f);
            }
            fopAcM_posMoveF(this, &field_0xA8C);
            mAcch.CrrPos(dComIfG_Bgsp());
            mGndChk = mAcch.m_gnd;
            field_0xB00 = mAcch.GetGroundH();
            if (field_0xB00 != -G_CM3D_F_INF) {
                setEnvTevColor();
                setRoomNo();
            }
        }
        if (mProcessFunc != NULL) {
            (this->*mProcessFunc)(NULL);
        }
        setMtx();
        if (field_0xAFC == 0) {
            mTgCyl.SetCoSPrm(0);
            mTgCyl.SetTgType(0xD8FBFDFF);
            mTgCyl.SetTgSPrm(0x1F);
            mTgCyl.SetR(8.0f);
            mTgCyl.SetH(62.0f);
            cXyz currentPos = current.pos;
            mTgCyl.SetC(currentPos);
            dComIfG_Ccsp()->Set(&mTgCyl);
            mAtCyl.SetAtType(0);
            mAtCyl.SetAtSPrm(0);
            mAtCyl.SetR(8.0f);
            mAtCyl.SetH(62.0f);
            mAtCyl.SetC(current.pos);
            dComIfG_Ccsp()->Set(&mAtCyl);
        }
        setAttnPos();
        return 1;
    }
}

int daObj_SSItem_c::Draw() {
    if (field_0xB0E == 2) {
        return 1;
    } else {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
        mDoExt_modelUpdateDL(mpModel);
        if (field_0xB00 != -G_CM3D_F_INF) {
            cM3dGPla plane;
            bool tri_pla = dComIfG_Bgsp().GetTriPla(mGndChk, &plane);
            if (tri_pla) {
                f32 param2 = 20.0f;
                dComIfGd_setSimpleShadow(&current.pos, field_0xB00, param2, &plane.mNormal, 0, 1.0f,
                                         dDlst_shadowControl_c::getSimpleTex());
            }
        }
        return 1;
    }
}

int daObj_SSItem_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daObj_SSItem_c*>(i_this)->CreateHeap();
}

void daObj_SSItem_c::setSoldOut() {
    if (getParentPtr() != NULL) {
        static_cast<daMyna_c*>(getParentPtr())->onEventFlag(0);
        static_cast<daMyna_c*>(getParentPtr())->soldoutItem(fopAcM_GetID(this));
    }
}

u32 daObj_SSItem_c::getProcessID() {
    if (field_0xB0E == 2) {
        return field_0xB04;
    } else {
        return fopAcM_GetID(this);
    }
}

daObj_SSItem_c* daObj_SSItem_c::getExchangeItemPtr() {
    if (field_0xB0E == 2) {
        return static_cast<daObj_SSItem_c*>(fopAcM_SearchByID(field_0xB04));
    }
    return this;
}

char* daObj_SSItem_c::getResName() {
    return l_resFileName[field_0xB0E];
}

u8 daObj_SSItem_c::getTypeFromParam() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    default:
        return 0;
    }
}

u16 daObj_SSItem_c::getFlowNodeNum() {
    u16 num = home.angle.x;
    bool bVar1 = false;
    if (num == 0xFFFF || num == 0) {
        bVar1 = true;
    }

    int num2;
    if (!bVar1) {
        num2 = num;
    } else {
        num2 = -1;
    }

    return num2;
}

u16 daObj_SSItem_c::getValue() {
    return (fopAcM_GetParam(this) >> 0xC) & 0xFFFF;
}

void daObj_SSItem_c::restart() {
    current.angle.set(0, home.angle.y, 0);
    shape_angle = current.angle;
    setProcess(&daObj_SSItem_c::wait);
}

void daObj_SSItem_c::initialize() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -30.0f, -15.0f, -30.0f, 30.0f, 45.0f, 30.0f);
    eventInfo.setArchiveName(getResName());
    attention_info.flags = 0;
    attention_info.distances[fopAc_attn_CARRY_e] = 6;
    fopAcM_OnCarryType(this, fopAcM_CARRY_SIDE);
    mAcchCir.SetWall(24.0f, 12.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mStts.Init(0xFF, 0, this);
    mAtCyl.Set(mCcDCyl);
    mAtCyl.SetStts(&mStts);
    mTgCyl.Set(mCcDCyl);
    mTgCyl.SetStts(&mStts);
    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    field_0xB00 = mAcch.GetGroundH();
    setEnvTevColor();
    setRoomNo();
    restart();
    Execute();
}

int daObj_SSItem_c::setProcess(ProcessFunc i_process) {
    mEventType = 2;
    if (mProcessFunc != NULL) {
        (this->*mProcessFunc)(NULL);
    }
    mEventType = 0;
    mProcessFunc = i_process;
    if (mProcessFunc != NULL) {
        (this->*mProcessFunc)(NULL);
    }
    mEventType = 1;
    return 1;
}

void daObj_SSItem_c::setParam() {
    scale.set(1.4f, 1.4f, 1.4f);
    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mAcchCir.SetWallR(24.0f);
    mAcchCir.SetWallH(12.0f);
    gravity = -5.0f;
}

void daObj_SSItem_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_SSItem_c::setRoomNo() {
    s32 roomId = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, roomId);
    mStts.SetRoomId(roomId);
}

void daObj_SSItem_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObj_SSItem_c::setAttnPos() {
    attention_info.position = current.pos;
    eyePos = current.pos;
}

int daObj_SSItem_c::wait(void* param_0) {
    switch (mEventType) {
    case 0:
        field_0xA8C.set(0.0f, 0.0f, 0.0f);
    case 1:
    case 2:
    default:
        return 1;
    }
}

int daObj_SSItem_c::buy(void* param_0) {
    switch (mEventType) {
    case 0:
        break;
    case 1:
        dComIfGp_event_reset();

        if (getParentPtr() != NULL) {
            ((daMyna_c *)getParentPtr())->deleteItem(fopAcM_GetID(this));
        }

        fopAcM_delete(this);
        break;
    case 2:
        break;
    }

    return 1;
}

int daObj_SSItem_c::cancel(void* param_0) {
    switch (mEventType) {
    case 0:
        break;
    case 1:
        if (field_0xAFC == 0) {
            dComIfGp_event_reset();

            current.pos.set(field_0xAF0);

            setProcess(&daObj_SSItem_c::wait);
        }

        break;
    case 2:
        break;
    }

    return 1;
}

static int daObj_SSItem_Create(void* i_this) {
    return static_cast<daObj_SSItem_c*>(i_this)->create();
}

static int daObj_SSItem_Delete(void* i_this) {
    return static_cast<daObj_SSItem_c*>(i_this)->Delete();
}

static int daObj_SSItem_Execute(void* i_this) {
    return static_cast<daObj_SSItem_c*>(i_this)->Execute();
}

static int daObj_SSItem_Draw(void* i_this) {
    return static_cast<daObj_SSItem_c*>(i_this)->Draw();
}

static int daObj_SSItem_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daObj_SSItem_MethodTable = {
    (process_method_func)daObj_SSItem_Create,  (process_method_func)daObj_SSItem_Delete,
    (process_method_func)daObj_SSItem_Execute, (process_method_func)daObj_SSItem_IsDelete,
    (process_method_func)daObj_SSItem_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_SSITEM = {
    fpcLy_CURRENT_e,
    7,
    fpcLy_CURRENT_e,
    PROC_OBJ_SSITEM,
    &g_fpcLf_Method.base,
    sizeof(daObj_SSItem_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x45,
    &daObj_SSItem_MethodTable,
    0x44100,
    0,
    fopAc_CULLBOX_CUSTOM_e,
};
