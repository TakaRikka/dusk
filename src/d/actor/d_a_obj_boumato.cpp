/**
 * @file d_a_obj_boumato.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_boumato.h"
#include "SSystem/SComponent/c_counter.h"
#include "d/actor/d_a_arrow.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"

static const char* dummyString() { return ""; }

f32 const daObj_BouMato_Param_c::m[7] = {
    0.0f, -3.0f, 1.0f, 400.0f, 300.0f, 4.0f, 20.0f,
};

static dCcD_SrcGObjInf const l_ccDObjData = 
{
        {0x0, {{0x0, 0x0, 0}, {0xD8FBFDFF, 0x1F}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
};

static dCcD_SrcCyl l_ccDCyl = {
    l_ccDObjData, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        0.0f, // mRadius
        0.0f // mHeight
    } // mCyl
};

static char* l_resName = "H_BouMato";

static Vec jntCoOffset[2] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 300.0f, 0.0f},
};

// /* 80BBC58C-80BBC598 -00001 000C+00 1/1 0/0 0/0 .data            jntCoData$4045 */
static dJntColData_c jntCoData = {
    1, 1, 0, 4.0f, jntCoOffset,
};

daObj_BouMato_c::~daObj_BouMato_c() {
    OS_REPORT("|%06d:%x|daObj_BouMato_c -> デストラクト\n", g_Counter.mCounter0, this);
    dComIfG_resDelete(&mPhase, getResName());
}

int daObj_BouMato_c::create() {
    fopAcM_ct(this, daObj_BouMato_c);
    field_0xa32 = getType();
    int rv = dComIfG_resLoad(&mPhase, getResName());
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) onSWBit:%02x, offSWBit:%02x<%08x>\n", fopAcM_getProcNameString(this), getType(), getOnSwBit(),
                              getOffSwBit(), fopAcM_GetParam(this));
        OS_REPORT("\n");
        if (fopAcM_entrySolidHeap(this, createHeapCallBack, 0x810) == 0) {
            return cPhs_ERROR_e;
        } 
        mModel->getModelData();
        fopAcM_SetMtx(this, mModel->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                            &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();
        mStts.Init(0xff, 0, this);
        mCyl.Set(l_ccDCyl);
        mCyl.SetStts(&mStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        daNpcT_offTmpBit(0x3c);
        daNpcT_offTmpBit(0x3f);
        daNpcT_offTmpBit(0x40);
        daNpcT_offTmpBit(0x41);
        daNpcT_offTmpBit(0x42);
        mTargetId = -1;
        Execute();
    }
    return rv;
}

int daObj_BouMato_c::CreateHeap() {
    J3DModelData*  mdlData_p = (J3DModelData*)dComIfG_getObjectRes(getResName(), 4);
    JUT_ASSERT(374, NULL != mdlData_p);
    mModel = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
    if (mModel == NULL) {
        return 0;
    }
    return mJntCol.init(this, &jntCoData, mModel, 1) ? 1 : 0;
}

int daObj_BouMato_c::Delete() {
    OS_REPORT("|%06d:%x|daObj_BouMato_c -> Delete\n", g_Counter.mCounter0, this);
    this->~daObj_BouMato_c();
    return 1;
}

int daObj_BouMato_c::Execute() {
    cXyz cStack_34;
    cXyz local_40;
    mStts.Move();

    u8 dVar1 = field_0xa38;
    field_0xa38 = 1;
    if (getOnSwBit() == 0xff || (getOnSwBit() != 0xff && dComIfGs_isSwitch(getOnSwBit(), fopAcM_GetRoomNo(this)))) {
        if (daNpcT_chkTmpBit(0x3c) || (daNpcT_chkEvtBit(0x4b) && daNpcT_chkEvtBit(0x4c))) {
            field_0xa38 = 0;
        }
    }
    attention_info.flags = 0;
    if (field_0xa38 == 1 && dVar1 == 0) {
        deleteStuckArrow();
    }
    dComIfG_play_c& play = g_dComIfG_gameInfo.play;
    if (play.getEvent().runCheck() && !eventInfo.checkCommandTalk()) {
        if (eventInfo.checkCommandDemoAccrpt() && dComIfGp_getEventManager().endCheck(field_0xa30)) {
            play.getEvent().reset();
            field_0xa30 = -1;
        } else {
            int staffId = dComIfGp_getEventManager().getMyStaffId("BouMato", this, -1);
            if (staffId != -1) {
                dComIfGp_getEventManager().cutEnd(staffId);
            }
        }
    }
    if (field_0xa10 != 0) {
        field_0xa10--;
    }
    if (field_0xa10 == 0 && mCyl.ChkTgHit()) {
        if (mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_ARROW)) {
            mTargetId = fopAcM_GetID(mCyl.GetTgHitAc());
            if (getOffSwBit() != 0xff) {
                if (!dComIfGs_isSwitch(getOffSwBit(), fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(getOffSwBit(), fopAcM_GetRoomNo(this));
                }
            }
            fopAcM_seStartCurrent(this, Z2SE_OBJ_ARW_TRGT_BRK_LAST, 0);
        }
        setSwayParam(mCyl.GetTgHitAc());
    }
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    mGroundH = mAcch.GetGroundH();
    if (mGroundH != -G_CM3D_F_INF) {
        setEnvTevColor();
        setRoomNo();
    }
    for (int i = 0; i < 3; i++) {
        if (mIsCurTurnRight) {
            mDoMtx_stack_c::YrotS(field_0xa24[i]);
            cStack_34.set(0.0f, 0.0f, field_0xa18[i]);
            mDoMtx_stack_c::multVec(&cStack_34, &local_40);
            field_0x9f8[i].x = local_40.z;
            field_0x9f8[i].z = -local_40.x;
            field_0xa24[i] -= cM_deg2s(45.0f);
        } else {
            mDoMtx_stack_c::YrotS(field_0xa2a);
            cStack_34.set(0.0f, 0.0f,
                        field_0xa18[i] * cM_ssin(field_0xa24[i]));
            mDoMtx_stack_c::multVec((Vec*)&cStack_34, (Vec*)&local_40);
            field_0x9f8[i].x = local_40.z;
            field_0x9f8[i].z = -local_40.x;
            field_0xa24[i] += cM_deg2s(45.0f);
        }
    }
    cLib_addCalc2(field_0xa18, 0, 0.075f, 50.0f);
    cLib_addCalc2(field_0xa18 + 1, 0, 0.1f, 100.0f);
    cLib_addCalc2(field_0xa18 + 2, 0, 0.125f, 125.0f);
    setMtx();
    attention_info.position = current.pos;
    attention_info.position.y += daObj_BouMato_Param_c::m[0];
    eyePos = attention_info.position;
    if (field_0xa38 == 0) {
        mCyl.SetC(current.pos);
        mCyl.SetH(daObj_BouMato_Param_c::m[4]);
        mCyl.SetR(daObj_BouMato_Param_c::m[5]);
        dComIfG_Ccsp()->Set(&mCyl);
    }
    mCyl.ClrTgHit();
    return 1;
}

int daObj_BouMato_c::Draw() {
    if (field_0xa38 == 0) {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
        mDoExt_modelUpdateDL(mModel);
        if (mGroundH != -G_CM3D_F_INF) {
            mShadowId =
                dComIfGd_setShadow(mShadowId, 1, mModel, &current.pos, daObj_BouMato_Param_c::m[3],
                                   20.0f, current.pos.y, mGroundH, mGndChk, &tevStr, 0, 1.0f,
                                   dDlst_shadowControl_c::getSimpleTex());
        }
    }
    return 1;
}

int daObj_BouMato_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daObj_BouMato_c*>(i_this)->CreateHeap();
}

void daObj_BouMato_c::tgHitCallBack(fopAc_ac_c* param_1, dCcD_GObjInf* param_2,
                                        fopAc_ac_c* param_3, dCcD_GObjInf* param_4) {
    u8 cutType = 0;
    if (param_3 != NULL) {
        if (fopAcM_GetProfName(param_3) == PROC_ALINK) {
            cutType = static_cast<daPy_py_c*>(param_3)->getCutType();
        } else {
            cutType = daPy_py_c::CUT_TYPE_NM_VERTICAL;
        }
    }
    static_cast<daObj_BouMato_c*>(param_1)->setCutType(cutType);
}

static daArrow_c* l_findActorPtrs[100];

static u32 l_findCount;

void* daObj_BouMato_c::srchArrow(void* param_1, void* param_2) {
    if (l_findCount < 100 && param_1 != NULL && param_1 != param_2) {
        if (fopAcM_IsExecuting(fopAcM_GetID(param_1)) && fopAcM_GetName(param_1) == PROC_ARROW &&
            static_cast<daArrow_c*>(param_1)->getHitAcID() != -1)
        {
            l_findActorPtrs[l_findCount] = (daArrow_c*)param_1;
            l_findCount++;
        }
    }
    return NULL;
}

void daObj_BouMato_c::deleteStuckArrow() {
    l_findCount = 0;
    fopAcM_Search(srchArrow, this);
    for (int i = 0; i < (int)l_findCount; i++) {
        daArrow_c* pArrow = l_findActorPtrs[i];
        if (this == fopAcM_SearchByID(pArrow->getHitAcID())) {
            fopAcM_delete(pArrow);
        }
    }
}

char* daObj_BouMato_c::getResName() {
    return l_resName;
}

void daObj_BouMato_c::setSwayParam(fopAc_ac_c* param_1) {
    f32 dVar7 = 1.0f;
    f32 local_48[3] = {0.0f, 0.0f, daObj_BouMato_Param_c::m[6]};;
    field_0xa2a = (fopAcM_searchActorAngleY(this, param_1) - shape_angle.y) + 0x8000;
    field_0xa10 = 8;
    mIsCurTurnRight = false;
    if (fopAcM_GetProfName(param_1) == PROC_ALINK) {
        switch (mCutType) {
        case daPy_py_c::CUT_TYPE_NM_RIGHT:
        case daPy_py_c::CUT_TYPE_NM_LEFT:
        case daPy_py_c::CUT_TYPE_FINISH_LEFT:
        case daPy_py_c::CUT_TYPE_FINISH_VERTICAL:
            if (mCutType == daPy_py_c::CUT_TYPE_NM_RIGHT) {
                field_0xa2a -= 0x4000;
            }
            if (mCutType == daPy_py_c::CUT_TYPE_NM_LEFT) {
                field_0xa2a += 0x4000;
            }
            dVar7 = 0.6f;
            break;
        case daPy_py_c::CUT_TYPE_TURN_RIGHT:
            field_0xa10 = 20;
            mIsCurTurnRight = true;
            break;
        default:
            dVar7 = 0.6f;
            break;
        }
    } else {
        dVar7 = 0.6f;
    }
    for (int i = 0; i < 3; i++) {
        field_0xa18[i] = dVar7 * cM_deg2s(local_48[i]);
        field_0xa24[i] = 0x4000;
    }
}

void daObj_BouMato_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_BouMato_c::setRoomNo() {
    s32 roomId = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, roomId);
    mStts.SetRoomId(roomId);
}

void daObj_BouMato_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::XrotM(field_0x9f8[2].x);
    mDoMtx_stack_c::ZrotM(field_0x9f8[2].z);
    mDoMtx_stack_c::scaleM(scale);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daObj_BouMato_Create(void* i_this) {
    return static_cast<daObj_BouMato_c*>(i_this)->create();
}

static int daObj_BouMato_Delete(void* i_this) {
    return static_cast<daObj_BouMato_c*>(i_this)->Delete();
}

static int daObj_BouMato_Execute(void* i_this) {
    return static_cast<daObj_BouMato_c*>(i_this)->Execute();
}

static int daObj_BouMato_Draw(void* i_this) {
    return static_cast<daObj_BouMato_c*>(i_this)->Draw();
}

static int daObj_BouMato_IsDelete(void* i_this) {
    return 1;
}

static daObj_BouMato_Param_c l_HIO;

static actor_method_class daObj_BouMato_MethodTable = {
    (process_method_func)daObj_BouMato_Create,
    (process_method_func)daObj_BouMato_Delete,
    (process_method_func)daObj_BouMato_Execute,
    (process_method_func)daObj_BouMato_IsDelete,
    (process_method_func)daObj_BouMato_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_BOUMATO = {
  fpcLy_CURRENT_e,            // mLayerID
  7,                          // mListID
  fpcPi_CURRENT_e,            // mListPrio
  PROC_OBJ_BOUMATO,           // mProcName
  &g_fpcLf_Method.base,      // sub_method
  sizeof(daObj_BouMato_c),    // mSize
  0,                          // mSizeOther
  0,                          // mParameters
  &g_fopAc_Method.base,       // sub_method
  80,                         // mPriority
  &daObj_BouMato_MethodTable, // sub_method
  0x00044100,                 // mStatus
  fopAc_ACTOR_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,     // cullType
};
