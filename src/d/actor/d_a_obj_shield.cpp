/**
 * @file d_a_obj_shield.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_shield.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_cc_d.h"
#include "d/d_item_data.h"
#include "cmath.h"

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

static f32 Reflect(cXyz* pSpeed, cBgS_PolyInfo const& param_2, f32 param_3) {
    cM3dGPla acStack_3c;
    if (dComIfG_Bgsp().GetTriPla(param_2, &acStack_3c)) {
        cXyz cStack_48;
        f32 dVar5 = pSpeed->absXZ();
        C_VECReflect(pSpeed, &acStack_3c.mNormal, &cStack_48);
        *pSpeed = (cStack_48 * dVar5) * param_3;
        return pSpeed->absXZ();
    }
    return 0;
}

void daItemShield_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daItemShield_c::setBaseMtx() {
    cLib_addCalcAngleS(&field_0x952, field_0x954, 3, 500, 100);
    if (field_0x950 > -9000 && field_0x954 > 0) {
        field_0x954 = -std::fabs(field_0x954 * 0.88f);
        if (std::fabs(field_0x954) < 200.0f) {
            field_0x954 = 0;
        }
    } else if (field_0x950 < -9000 && field_0x954 < 0) {
        field_0x954 = std::fabs(field_0x954 * 0.88f);
        if (std::fabs(field_0x954) < 200.0f) {
            field_0x954 = 0;
        }
    }
    field_0x950 += field_0x952;
    csXyz rotation;
    rotation.x = field_0x944.x * cM_scos(mRotAngleCoeff * 2000) - field_0x944.x;
    rotation.z = field_0x944.z * cM_scos(2000 * (mRotAngleCoeff + 8)) - field_0x944.z;
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    if (fopAcM_GetGravity(this) == 0.0f) {
        mDoMtx_stack_c::transM(-20.0f, 0.0f, -15.0f);
        mDoMtx_stack_c::YrotM(field_0x950);
        mDoMtx_stack_c::transM(20.0f, 0.0f, 15.0f);
    } else {
        mDoMtx_stack_c::ZXYrotM(rotation.x, 0, rotation.z);
    }
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daItemShield_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mAcchCir.SetWall(30.0f, 30.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                     &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
    mCcStts.Init(0, 0xff, this);
    mCcCyl.Set(l_cyl_src);
    mCcCyl.SetStts(&mCcStts);
    mCcCyl.SetR(dItem_data::getR(m_itemNo));
    mCcCyl.SetH(dItem_data::getH(m_itemNo));
    fopAcM_SetCullSize(this, fopAc_CULLSPHERE_0_e);
    actionWaitInit();
    if (fopAcM_isSwitch(this, getSwBit())) {
        field_0x94a = 3;
        fopAcM_SetGravity(this, -4.0f);
        shape_angle.z = 0;
        shape_angle.x = 0;
        if (fopAcM_gc_c::gndCheck(&current.pos)) {
            current.pos.y = fopAcM_gc_c::getGroundY();
        }
    }
    mEvId = getEvId();
    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, NULL, mEvId);
    return 1;
}

int daItemShield_c::__CreateHeap() {
    return 1;
}

int daItemShield_c::create() {
    fopAcM_ct(this, daItemShield_c);
    m_itemNo = fpcNm_ITEM_WOOD_SHIELD;
    if (fopAcM_isSwitch(this, getSwBit2())) {
        OS_REPORT("木の盾：もう取ったので出ません\n");
        return cPhs_ERROR_e;
    }
    if (getSwBit2() == 0xff) {
        OS_REPORT("[43;30m木の盾：スイッチビット２指定がありません！\n\x1b[m");
    }
    if (getSwBit() == 0xff) {
        OS_REPORT("[43;30m木の盾：スイッチビット指定がありません！\n\x1b[m");
    }
    int rv = dComIfG_resLoad(&mPhase, dItem_data::getFieldArc(m_itemNo));
    if (rv == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, CheckFieldItemCreateHeap, 0x820) == 0) {
            return cPhs_ERROR_e;
        } 
        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }
    return rv;
}

void daItemShield_c::bg_check() {
    f32 dVar6 = 0.7f;
    if (mAcch.ChkWallHit()) {
        if (speedF > 5.0f) {
            Reflect(&speed, mAcchCir, 1.0f);
        }
        current.angle.y = cM_atan2s(speed.x, speed.z);
    }
    if ( mAcch.ChkGroundLanding()) {
        cXyz acStack_38(speed);
        if (speedF > 5.0f) {
            Reflect(&acStack_38, mAcchCir, dVar6);
        }
        speed.y = -field_0x938.y * dVar6;
        if (speed.y < 3.0f) {
            fopAcM_SetSpeedF(this, 0.0f);
        }
        field_0x936++;
        if (field_0x936 == 1) {
            fopAcM_seStart(this, Z2SE_OBJ_WOODSHIELD_BOUND, 0);
        }
    }
}

int daItemShield_c::action_proc_call() {
    static daItemShield_c::actionFunc l_actionFunc[4] = {
        &daItemShield_c::actionInit,
        &daItemShield_c::actionWait,
        &daItemShield_c::actionOrderGetDemo,
        &daItemShield_c::actionGetDemo,
    };
    return (this->*(l_actionFunc[mStatus]))();
}

int daItemShield_c::actionInit() {
    daItemBase_c::show();
    actionWaitInit();
    return 1;
}

int daItemShield_c::actionWaitInit() {
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OnCoSPrmBit(1);
    attention_info.position = current.pos;
    daItemBase_c::show();
    setStatus(1);
    return 1;
}

int daItemShield_c::actionWait() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if ((player->checkWolfAttackReverse() || daPy_getPlayerActorClass()->checkFrontRollCrash()) &&
        player->current.pos.absXZ(current.pos) < 500.0f)
    {
        field_0x94a++;
        if (field_0x94a == 1) {
            field_0x952 = 0;
            field_0x954 = 2500;
            OS_REPORT("FrontCrash! %d\n", field_0x94a);
            fopAcM_seStart(this, Z2SE_OBJ_WOODSHIELD_SHAKE, 0);
        } else if (field_0x94a == 2) {
            field_0x937 = true;
            field_0x94c = 8;
            fopAcM_onSwitch(this, getSwBit());
            fopAcM_seStart(this, Z2SE_OBJ_WOODSHIELD_COME_OFF, 0);
        }
    } else if (field_0x94a == 0) {
        fopAcM_SetGravity(this, 0.0f);
    }
    if (field_0x94a == 2 && cLib_calcTimer(&field_0x94c) == 0) {
        fopAcM_SetGravity(this, -4.0f);
        fopAcM_SetMaxFallSpeed(this, -25.0f);
    }
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    bg_check();
    if (mAcch.ChkGroundLanding()) {
        field_0x944.x = 4000;
        field_0x944.z = 4000;
        shape_angle.x = 0;
    } else if (mAcch.ChkGroundHit()) {
        speedF *= 0.9f;
        if (speedF < 1.0f) {
            speedF = 0.0f;
        }
        shape_angle.x = 0;
    } else if (fopAcM_GetGravity(this) != 0.0f && field_0x936 == 0) {
        shape_angle.x += 3000;
    }
    if (mCcCyl.ChkCoHit()) {
        fopAc_ac_c* hitActor = mCcCyl.GetCoHitAc();
        if (hitActor != NULL) {
            if (fopAcM_GetName(hitActor) == fopAcM_GetName(dComIfGp_getLinkPlayer())) {
                initActionOrderGetDemo();
            }
        }
    }
    mCcCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcCyl);
    return 1;
}

int daItemShield_c::initActionOrderGetDemo() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    daItemBase_c::hide();
    fopAcM_orderItemEvent(this, 0, 0);
    eventInfo.onCondition(dEvtCnd_CANGETITEM_e);
    mItemId =
        fopAcM_createItemForTrBoxDemo(&current.pos, m_itemNo, -1, fopAcM_GetRoomNo(this), 0, 0);
    JUT_ASSERT(682, mItemId != fpcM_ERROR_PROCESS_ID_e)
    setStatus(STATUS_ORDERGETDEMO);
    return 1;
}

int daItemShield_c::actionOrderGetDemo() {
    if (eventInfo.checkCommandItem()) {
        setStatus(STATUS_GETDEMO);
        if (mItemId != fpcM_ERROR_PROCESS_ID_e) {
            dComIfGp_event_setItemPartnerId(mItemId);
        }
    } else {
        fopAcM_orderItemEvent(this, 0, 0);
        eventInfo.onCondition(dEvtCnd_CANGETITEM_e);
    }
    return 1;
}

int daItemShield_c::actionGetDemo() {
    if (dComIfGp_evmng_endCheck("DEFAULT_GETITEM")) {
        dComIfGp_event_reset();
        fopAcM_delete(this);
        fopAcM_onSwitch(this, getSwBit2());
    }
    return 1;
}

void daItemShield_c::event_proc_call() {
    static daItemShield_c::camActionFunc l_func[4] = {
        &daItemShield_c::actionWaitCamDemo,
        &daItemShield_c::actionOrderCamDemo,
        &daItemShield_c::actionCamDemo,
        &daItemShield_c::actionCamDemoEnd,
    };

    (this->*(l_func[mAction]))();
}

void daItemShield_c::actionWaitCamDemo() {
    if (field_0x937) {
        setAction(ACTION_ORDER_CAM_DEMO);
        fopAcM_orderOtherEventId(this, mEventIdx, mEvId,
                                                 0xffff, 0, 1);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daItemShield_c::actionOrderCamDemo() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACTION_CAM_DEMO);
    } else {
        fopAcM_orderOtherEventId(this, mEventIdx, mEvId,
                                                 0xffff, 0, 1);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daItemShield_c::actionCamDemo() {
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        setAction(ACTION_CAM_DEMO_END);
        dComIfGp_event_reset();
    }
}

void daItemShield_c::actionCamDemoEnd() {
}

int daItemShield_c::execute() {
    field_0x938 = speed;
    mRotAngleCoeff++;
    action_proc_call();
    event_proc_call();
    eyePos = current.pos;
    attention_info.position = current.pos;
    cLib_addCalcAngleS(&field_0x944.x, 0, 0x50, 0x28, 10);
    cLib_addCalcAngleS(&field_0x944.z, 0, 0x50, 0x28, 10);
    setBaseMtx();
    if (fopAcM_searchPlayerDistance(this) < 500.0f) {
        daPy_getPlayerActorClass()->setLookPos(&attention_info.position);
        attention_info.flags = fopAc_AttnFlag_CHECK_e;
    } else {
        attention_info.flags = 0;
    }
    return 1;
}

int daItemShield_c::draw() {
    if (!daItemBase_c::chkDraw()) {
        return 1;
    }
    if (DrawBase() != 0) {
    }
    return 1;
}

int daItemShield_c::_delete() {
    daItemBase_c::DeleteBase(dItem_data::getFieldArc(m_itemNo));
    return 1;
}

static int daItemShield_Draw(daItemShield_c* i_this) {
    return i_this->draw();
}

static int daItemShield_Execute(daItemShield_c* i_this) {
    return i_this->execute();
}

static int daItemShield_Delete(daItemShield_c* i_this) {
    return i_this->_delete();
}

static int daItemShield_Create(fopAc_ac_c* i_this) {
    return static_cast<daItemShield_c*>(i_this)->create();
}

static actor_method_class l_daItemShield_Method = {
    (process_method_func)daItemShield_Create,
    (process_method_func)daItemShield_Delete,
    (process_method_func)daItemShield_Execute,
    NULL,
    (process_method_func)daItemShield_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Shield = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Shield,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daItemShield_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  240,                    // mPriority
  &l_daItemShield_Method, // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
