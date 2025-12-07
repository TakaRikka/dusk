/**
 * @file d_a_obj_thdoor.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_thdoor.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"

#if DEBUG

daObjThDoor_HIO_c::daObjThDoor_HIO_c() {
}

void daObjThDoor_HIO_c::genMessage(JORMContext* ctx) {
    // Telma's door
    ctx->genLabel("テルマの家のドア", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

static daObjThDoor_HIO_c l_HIO;
#endif

static BOOL checkPlayerPos(daObjThDoor_c* i_this) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz acStack_28 = player->current.pos - i_this->current.pos;
    mDoMtx_stack_c::YrotS(-i_this->current.angle.y);
    mDoMtx_stack_c::multVec(&acStack_28, &acStack_28);

    BOOL rv = TRUE;
    if (acStack_28.z > 0.0f) {
        rv = FALSE;
    }

    return rv;
}

static void doorCoHitCallBack(fopAc_ac_c* param_1, dCcD_GObjInf* param_2, fopAc_ac_c* param_3,
                              dCcD_GObjInf* param_4) {
    daPy_py_c* player = dComIfGp_getLinkPlayer();
    if (param_1 != NULL && param_3 != NULL && param_3 == player && player->speedF > 3.0f) {
        if (checkPlayerPos((daObjThDoor_c*)param_1)) {
            ((daObjThDoor_c*)param_1)->field_0x60a = 250;
        } else {
            ((daObjThDoor_c*)param_1)->field_0x60a = -250;
        }
    }
}

void daObjThDoor_c::initBaseMtx() {
    mModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjThDoor_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

static char* l_arcName = "V_CTDoor";

static char* l_evName = "CHASE_AWAY_WOLF";

static char* l_staffName = "thdoor";

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x10}, 0x19}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        37.5f, // mRadius
        250.0f // mHeight
    } // mCyl
};

int daObjThDoor_c::Create() {
    if (fopAcM_isSwitch(this, getSwbit())) {
        field_0x608 = 0;
        setAction(ACTION_DEAD);
    } else {
        field_0x608 = 5500;
        current.angle.y = home.angle.y - field_0x608;
        old.angle.y = current.angle.y;
        setAction(ACTION_WAIT);
    }

    initBaseMtx();
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    mStts.Init(0xff, 0xff, this);

    for (int i = 0; i < 2; i = i + 1) {
        mCyls[i].Set(l_cyl_src);
        mCyls[i].SetStts(&mStts);
        mCyls[i].SetCoHitCallback(doorCoHitCallBack);
    }
    fopAcM_setCullSizeBox2(this, mModel->getModelData());
    mMapToolId = 0xff;
    eventInfo.setArchiveName(l_arcName);
    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, l_evName, 0xff);

    // Telma's house door
    OS_REPORT("テルマの家のドア：evid<%d>tool<%d>\n", mEventIdx, mMapToolId);
    return 1;
}

int daObjThDoor_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(319, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mModel != NULL ? 1 : 0;
}

int daObjThDoor_c::create1st() {
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        rv = MoveBGCreate(l_arcName, 8, NULL, 0xc20, NULL);
        if (rv == 5) {
            return rv;
        }

        // Telma's door
        OS_REPORT("テルマの家のドア：sw<%d>msg<%d>\n", getSwbit(), getMsg());

        #if DEBUG
        // Telma's door
        l_HIO.entryHIO("テルマの家のドア");
        #endif
    }

    return rv;
}

int daObjThDoor_c::Execute(Mtx** i_mtx) {
    event_proc_call();
    current.angle.y = home.angle.y - field_0x608;
    if (field_0x5b5 == 0 && old.angle.y != current.angle.y) {
        fopAcM_seStartLevel(this, Z2SE_OBJ_THE_DOOR_MV, 0);
    }
    *i_mtx = &mBgMtx;
    setBaseMtx();
    return 1;
}

void daObjThDoor_c::set_cyl() {
    cXyz acStack_28[2] = {cXyz(37.5f, 0.0f, 0.0f), cXyz(112.5f, 0.0f, 0.0f)};
    cXyz cStack_38;
    for (int i = 0; i < 2; i++) {
        cStack_38 = acStack_28[i];
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y - field_0x608);
        mDoMtx_stack_c::multVec(&cStack_38, &cStack_38);
        mCyls[i].SetC(cStack_38);
        dComIfG_Ccsp()->Set(&mCyls[i]);
    }
}

void daObjThDoor_c::action() {
    s16 sVar1 = field_0x608;
    set_cyl();
    cLib_addCalcAngleS(&field_0x60a, 0, 0x1e, 0x60, 0x10);
    field_0x608 = field_0x608 + field_0x60a;
    if (field_0x608 > 0x4000) {
        field_0x608 = 0x4000;
        if (sVar1 == 0x4000) {
            field_0x60a = 0;
        } else {
            field_0x60a = -field_0x60a;
        }
    } else {
        if (field_0x608 < 5500) {
            field_0x608 = 5500;
            if (sVar1 == 5500) {
                field_0x60a = 0;
            } else {
                field_0x60a = -field_0x60a;
            }
        }
    }
}

void daObjThDoor_c::event_proc_call() {
    static daObjThDoor_c::actionFunc l_func[4] = {
        &daObjThDoor_c::actionWait,
        &daObjThDoor_c::actionOrderEvent,
        &daObjThDoor_c::actionEvent,
        &daObjThDoor_c::actionDead,
    };

    (this->*l_func[mAction])();
}

void daObjThDoor_c::actionWait() {
    action();
    if (checkDemo() && checkArea()) {
        setAction(ACTION_ORDER_EVENT);
        fopAcM_orderOtherEventId(this, mEventIdx, mMapToolId, 0xffff, 0, 1);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daObjThDoor_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACTION_EVENT);
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staffName, 0, 0);
        demoProc();
        if (mpBgW != NULL && mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }
    } else {
        fopAcM_orderOtherEventId(this, mEventIdx, mMapToolId, 0xffff, 0, 1);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daObjThDoor_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        setAction(ACTION_DEAD);
        dComIfGp_event_reset();
        fopAcM_onSwitch(this, getSwbit());

        // Telma's house door: SW<%d> turned on
        OS_REPORT("テルマの家のドア：ＳＷ<%d>オンしました\n", getSwbit());
        if (mpBgW != NULL) {
            dComIfG_Bgsp().Regist(mpBgW, this);
        }
    } else {
        demoProc();
    }
}

void daObjThDoor_c::actionDead() {}


int daObjThDoor_c::demoProc() {
    static char* action_table[3] = {
        "WAIT",
        "MSG",
        "CLOSE",
    };

    daPy_py_c* player = daPy_getPlayerActorClass();
    int actIdx = dComIfGp_evmng_getMyActIdx(mStaffId, action_table, 3, 0, 0);
    if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
        switch (actIdx) {
        case 0: {
            int* pTimer = dComIfGp_evmng_getMyIntegerP(mStaffId, "Timer");
            if (pTimer == NULL) {
                field_0x5b4 = 1;
            } else {
                field_0x5b4 = *pTimer;
            }
            break;
        }
        case 1:
            mMsgFlow.init(this, getMsg(), 0, NULL);
            break;
        case 2:
            field_0x5b8 = 0;
            fopAcM_seStart(this, Z2SE_OBJ_THE_DOOR_CL, 0);
            field_0x5b5 = 1;
            break;
        }
    }

    switch (actIdx) {
    case 0:
        if (cLib_calcTimer(&field_0x5b4)) {
            break;
        }
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    case 1:
        if (mMsgFlow.doFlow(this, NULL, 0)) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case 2:
        cLib_chaseS(&field_0x5b8, 0x6a4, 400);
        if (cLib_chaseS(&field_0x608, 0, field_0x5b8) == 0) {
            break;
        }
        dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    }

    return 0;
}

BOOL daObjThDoor_c::checkArea() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz local_28(player->current.pos);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::inverse();
    mDoMtx_stack_c::multVec(&local_28, &local_28);
    if (local_28.z > -50.0f || local_28.z < -300.0f) {
        return 0;
    } 
    
    if (local_28.x < -100.0f || local_28.x > 100.0f) {
        return 0;
    } 

    return 1;
}

int daObjThDoor_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return 1;
}

int daObjThDoor_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);

    #if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

static int daObjThDoor_create1st(daObjThDoor_c* i_this) {
    fopAcM_ct(i_this, daObjThDoor_c);
    return i_this->create1st();
}

static int daObjThDoor_MoveBGDelete(daObjThDoor_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjThDoor_MoveBGExecute(daObjThDoor_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjThDoor_MoveBGDraw(daObjThDoor_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjThDoor_METHODS = {
    (process_method_func)daObjThDoor_create1st,
    (process_method_func)daObjThDoor_MoveBGDelete,
    (process_method_func)daObjThDoor_MoveBGExecute,
    NULL,
    (process_method_func)daObjThDoor_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_TDoor = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_TDoor,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjThDoor_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  560,                    // mPriority
  &daObjThDoor_METHODS,   // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
