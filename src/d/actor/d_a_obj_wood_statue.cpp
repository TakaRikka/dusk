/**
 * @file d_a_obj_wood_statue.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_wood_statue.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"
#include "d/d_s_play.h"

#if DEBUG

class daObjWStatue_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daObjWStatue_HIO_c() {
        mRespawnTimer = 30;
    }

    void genMessage(JORMContext* context) {
        // Wooden Statue
        context->genLabel("木彫りの像", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

        // Respawn timer
        context->genSlider("全滅後の出現タイマー", &mRespawnTimer, 0, 200, 0, NULL, 0xffff, 0xffff,
                           0x200, 0x18);
    }

    u8 mRespawnTimer;
};

daObjWStatue_HIO_c l_HIO;

#endif


const static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xffffffff, 0x11}, 0x59}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        20.0f, // mRadius
        40.0f // mHeight
    } // mCyl
};

static char* l_arcName = "O_wood";

static char* l_eventName = "GET_WOOD_STATUE";

static f32 Reflect(cXyz* param_1, cBgS_PolyInfo const& param_2, f32 param_3) {
    cM3dGPla acStack_3c;
    bool isTriPla = dComIfG_Bgsp().GetTriPla(param_2, &acStack_3c);
    if (isTriPla) {
        cXyz cStack_48;
        f32 dVar5 = param_1->absXZ();
        VECReflect(param_1, &acStack_3c.mNormal, &cStack_48);
        *param_1 = (cStack_48 * dVar5) * param_3;
        return param_1->absXZ();
    } else {
        return 0.0f;
    }
}

static void lifeGetTgCallBack(fopAc_ac_c* i_this, dCcD_GObjInf* param_2, fopAc_ac_c* param_3,
                              dCcD_GObjInf* param_4) {
    if (i_this != NULL &&
        (param_4->ChkAtType(AT_TYPE_40) || param_4->ChkAtType(AT_TYPE_BOOMERANG)) &&
        !dComIfGp_event_runCheck() &&
        !((daObjWStatue_c*)i_this)->chkStatus(daObjWStatue_c::STATUS_BOOMERANG_CARRY))
    {
        ((daObjWStatue_c*)i_this)->actionInitBoomerangCarry();
    }
    return;
}

static void lifeGetCoCallBack(fopAc_ac_c* i_this, dCcD_GObjInf* param_2, fopAc_ac_c* param_3,
                              dCcD_GObjInf* param_4) {
    if (i_this != NULL && param_3 != NULL && param_3 == dComIfGp_getLinkPlayer()) {
        ((daObjWStatue_c*)i_this)->initActionOrderGetDemo();
    }
}

static void* s_sh_sub(void* param_1, void* param_2) {
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == PROC_E_SH) {
        return param_1;
    }
    return NULL;
}

void daObjWStatue_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjWStatue_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjWStatue_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mAcchCir.SetWall(30.0f, 30.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                     &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
    mCcStts.Init(0, 0xff, this);
    mCcCyl.Set(l_cyl_src);
    mCcCyl.SetStts(&mCcStts);
    mCcCyl.SetCoHitCallback(lifeGetCoCallBack);
    mCcCyl.SetTgHitCallback(lifeGetTgCallBack);
    mCcCyl.SetR(dItem_data::getR(m_itemNo));
    mCcCyl.SetH(dItem_data::getH(m_itemNo));
    fopAcM_SetCullSize(this, fopAc_CULLSPHERE_0_e);
    actionInitSwOnWait();
    fopAcM_SetGravity(this, -6.0f);
    eventInfo.setArchiveName(l_arcName);
    mEventIdx = (s16)dComIfGp_getEventManager().getEventIdx(this, l_eventName, 0xff);

    #if DEBUG
    if (mEventIdx == -1) {
        // Wooden statue: Event acquisition failure
        OS_REPORT_ERROR("木彫りの像：イベント取得失敗\n");
        return 0;
    } else {
        return 1;
    }
    #else
    return 1;
    #endif
}

int daObjWStatue_c::__CreateHeap() {
    return 1;
}

int daObjWStatue_c::create() {
    fopAcM_ct(this, daObjWStatue_c);
    m_itemNo = fpcNm_ITEM_WOOD_STATUE;
    if (fopAcM_isSwitch(this, getSwbit2())) {
        return cPhs_ERROR_e;
    }

    int rv = dComIfG_resLoad(&mPhase, dItem_data::getFieldArc(m_itemNo));
    if (rv == 4) {
        if (fopAcM_entrySolidHeap(this, CheckFieldItemCreateHeap, 0x820) == 0) {
            return cPhs_ERROR_e;
        }

        if (Create() == 0) {
            return cPhs_ERROR_e;
        }

        #if DEBUG
        // Wooden statue
        l_HIO.entryHIO("木彫りの像");
        #endif
    }
    return rv;
}

void daObjWStatue_c::bg_check() {
    f32 dVar6 = 0.7f;
    if (mAcch.ChkWallHit()) {
        if (speedF > 5.0f) {
            Reflect(&speed, mAcchCir, 1.0f);
        }
        current.angle.y = cM_atan2s(speed.x, speed.z);
    }

    if (mAcch.ChkGroundLanding()) {
        cXyz acStack_38(speed);
        if (speedF > 5.0f) {
            Reflect(&acStack_38, mAcchCir, dVar6);
        }
        speed.y = -field_0x93c.y * dVar6;
        if (speed.y < 3.0f) {
            fopAcM_SetSpeedF(this, 0.0f);
        }
        if (mBoundSETimer > 5) {
            mShouldBoundSEPlay++;
        }

        if (mShouldBoundSEPlay == 1) {
            fopAcM_seStart(this, Z2SE_OBJ_WOOD_STATUE_BOUND, 0);
            // Wooden Statue: Bound SE
            OS_REPORT("木彫りの像：バウンドＳＥ\n");
        }
    }
}

int daObjWStatue_c::actionWaitInit() {
    mCcCyl.OnTgSPrmBit(1);
    mCcCyl.OnCoSPrmBit(1);
    mBoundSETimer = 0;
    attention_info.position = current.pos;
    setStatus(STATUS_WAIT);
    return 1;
}

int daObjWStatue_c::actionWait() {
    mBoundSETimer++;
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        dComIfGp_event_reset();
    } else {
        demoProc();
    }
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    bg_check();
    if (mAcch.ChkGroundHit()) {
        speedF *= 0.9f;
        if (speedF < 1.0f) {
            speedF = 0.0f;
        }
    }

    if (mCcCyl.ChkCoHit()) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        fopAc_ac_c* hitActor = mCcCyl.GetCoHitAc();
        if (player == hitActor) {
            initActionOrderGetDemo();
        }
    }
    attention_info.position = current.pos;
    return 1;
}

int daObjWStatue_c::initActionOrderGetDemo() {
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    hide();
    s16 eventIdx = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xff);
    dComIfGp_getEvent().reset(this);
    fopAcM_orderChangeEventId(this, eventIdx, 1, 0xffff);
    mItemId = fopAcM_createItemForTrBoxDemo(&current.pos, m_itemNo, 0xffffffff,
                                            fopAcM_GetRoomNo(this), 0, 0);
    JUT_ASSERT(544, mItemId != fpcM_ERROR_PROCESS_ID_e);
    setStatus(STATUS_ORDER_GET_DEMO);
    return 1;
}

int daObjWStatue_c::actionOrderGetDemo() {
    setStatus(STATUS_GET_DEMO);
    if (mItemId != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(mItemId);
    }
    return 1;
}

int daObjWStatue_c::actionGetDemo() {
    if (dComIfGp_evmng_endCheck("DEFAULT_GETITEM")) {
        fopAcM_onSwitch(this, getSwbit2());
        dComIfGp_event_reset();
        fopAcM_delete(this);
    }
    return 1;
}

int daObjWStatue_c::actionInitSwOnWait() {
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    hide();
    setStatus(STATUS_SW_ON_WAIT);
    field_0x93a = 0;
    return 1;
}

int daObjWStatue_c::actionSwOnWait() {
    base_process_class* iVar2 = fpcM_Search(s_sh_sub, this);
    switch (field_0x93a) {
    case 0:
        if (iVar2 == NULL) {
            #if DEBUG
            mRespawnTimer = l_HIO.mRespawnTimer;
            #else
            mRespawnTimer = 30;
            #endif
            field_0x93a = 1;
        }
        break;
    case 1:
        if (iVar2 != NULL) {
            field_0x93a = 0;
        } else {
            if (cLib_calcTimer(&mRespawnTimer) == 0) {
                field_0x93a = 2;
            }
        }
        break;
    case 2:
        if (eventInfo.checkCommandDemoAccrpt()) {
            actionWaitInit();
        } else {
            fopAcM_orderOtherEventId(this, mEventIdx, 0xff, 0xffff, 0, 1);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        }
        break;
    default:
        JUT_ASSERT(644, FALSE);
    }

    return 1;
}

int daObjWStatue_c::actionInitBoomerangCarry() {
    mCcCyl.OnTgSPrmBit(1);
    mCcCyl.OnCoSPrmBit(1);
    f32 cylH = dItem_data::getH(m_itemNo);
    f32 cylR = dItem_data::getR(m_itemNo);
    mCcCyl.SetR(cylR * 4.0f);
    mCcCyl.SetH(cylH * 4.0f);
    mCcCyl.OnCoSPrmBit(1);
    setStatus(STATUS_BOOMERANG_CARRY);
    return 1;
}

int daObjWStatue_c::actionBoomerangCarry() {
    fopAc_ac_c* boomerang = fopAcM_SearchByName(PROC_BOOMERANG);
    if (boomerang != NULL) {
        current.pos = boomerang->current.pos;
    } else {
        // Wooden Statue: can't find boomerang!!!
        OS_REPORT_ERROR("木彫りの像：ブーメランが見つかりません！！！\n");
    }
    // Wooden Statue: carrying boomerang
    OS_REPORT("木彫りの像：引き寄せ中\n");
    return 1;
}

void daObjWStatue_c::demoProc() {
    static char* action_table[4] = {
        "WAIT",
        "MOVE",
        "SETPOS",
        "NOTEND",
    };
    int* pTimer;
    daPy_py_c* player = daPy_getPlayerActorClass();
    int actIdx = dComIfGp_evmng_getMyActIdx(mStaffIdx, (char**)action_table, 4, 0, 0);
    if (dComIfGp_evmng_getIsAddvance(mStaffIdx)) {
        switch (actIdx)
        case 0: {
            pTimer = dComIfGp_evmng_getMyIntegerP(mStaffIdx, "Timer");
            if (pTimer == NULL) {
                mRespawnTimer = 1;
            } else {
                mRespawnTimer = *pTimer;
            }
            break;
        case 1:
            effectSet();
            show();
            fopAcM_SetSpeedF(this, KREG_F(0) + 10.0f);
            fopAcM_SetGravity(this, KREG_F(1) + -2.5f);
            speed.y = KREG_F(2) + 20.0f;
            current.pos.y += KREG_F(3) + 100.0f;
            mRespawnTimer = 80;
            break;
        case 2:
            dComIfGp_evmng_setGoal(&current.pos);
            break;
        case 3:
            break;
        }
    }

    switch(actIdx) {
    case 0:
        if (cLib_calcTimer(&mRespawnTimer) == 0) {
            dComIfGp_evmng_cutEnd(mStaffIdx);
        }
        break;
    case 1:
        if (cLib_calcTimer(&mRespawnTimer) == 0) {
            effectStop();
            dComIfGp_evmng_cutEnd(mStaffIdx);
        }
        break;
    case 3:
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffIdx);
        break;
    }
}

void daObjWStatue_c::effectSet() {
    if (mCallbacks[0].getEmitter() == NULL) {
        dComIfGp_particle_set(0x827a, &current.pos, &shape_angle, &scale, 0xff, &mCallbacks[0],
                              fopAcM_GetRoomNo(this), NULL, NULL, NULL);
        dComIfGp_particle_set(0x827b, &current.pos, &shape_angle, &scale, 0xff, &mCallbacks[1],
                              fopAcM_GetRoomNo(this), NULL, NULL, NULL);
    }
}

void daObjWStatue_c::effectStop() {
    mCallbacks[0].remove();
    mCallbacks[1].remove();
}

int daObjWStatue_c::execute() {
    static daObjWStatue_c::demoFunc l_demoFunc[5] = {
        &daObjWStatue_c::actionWait,           &daObjWStatue_c::actionOrderGetDemo,
        &daObjWStatue_c::actionGetDemo,        &daObjWStatue_c::actionSwOnWait,
        &daObjWStatue_c::actionBoomerangCarry,
    };
    field_0x93c = speed;
    field_0x930++;
    (this->*l_demoFunc[mStatus])();
    eyePos = current.pos;
    attention_info.position = current.pos;
    setBaseMtx();
    animPlay(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    mCcCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcCyl);
    return 1;
}

int daObjWStatue_c::draw() {
    if (!chkDraw()) {
        return 1;
    }
    DrawBase();
    return 1;
}

void daObjWStatue_c::setListStart() {}

int daObjWStatue_c::_delete() {
    DeleteBase(dItem_data::getFieldArc(m_itemNo));

    #if DEBUG
    l_HIO.removeHIO(this);
    #endif

    return 1;
}

static int daObjWStatue_Draw(daObjWStatue_c * i_this) {
    return i_this->draw();
}

static int daObjWStatue_Execute(daObjWStatue_c * i_this) {
    return i_this->execute();
}

static int daObjWStatue_Delete(daObjWStatue_c * i_this) {
    fopAcM_RegisterDeleteID(i_this, "ObjLife");
    return i_this->_delete();
}

static int daObjWStatue_Create(fopAc_ac_c * i_this) {
    fopAcM_RegisterCreateID(daObjWStatue_c, i_this, "ObjLife");
    return a_this->create();
}

static actor_method_class l_daObjWStatue_Method = {
    (process_method_func)daObjWStatue_Create,  (process_method_func)daObjWStatue_Delete,
    (process_method_func)daObjWStatue_Execute, NULL,
    (process_method_func)daObjWStatue_Draw,
};

extern actor_process_profile_definition g_profile_Obj_WoodStatue = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_WoodStatue,     // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjWStatue_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    563,                     // mPriority
    &l_daObjWStatue_Method,  // sub_method
    0x00044100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
