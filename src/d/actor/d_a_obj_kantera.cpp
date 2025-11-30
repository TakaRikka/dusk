/**
 * @file d_a_obj_kantera.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kantera.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_a_itembase_static.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_item_data.h"
#include "d/d_procname.h"
#include "f_op/f_op_actor_mng.h"

const static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xffffffff, 0x11}, 0x59}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                  // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},                  // mGObjTg
        {0x0},                                               // mGObjCo
    },                                                       // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        20.0f,               // mRadius
        40.0f                // mHeight
    }                        // mCyl
};

static f32 Reflect(cXyz* i_xyz, cBgS_PolyInfo const& param_2, f32 param_3) {
    cM3dGPla acStack_3c;
    if (dComIfG_Bgsp().GetTriPla(param_2, &acStack_3c) != 0) {
        cXyz cStack_48;
        f32 dVar5 = i_xyz->absXZ();
        C_VECReflect(i_xyz, &acStack_3c.mNormal, &cStack_48);
        *i_xyz = (cStack_48 * dVar5) * param_3;
       return i_xyz->absXZ();
    }

    return 0.0f;
}

void daItemKantera_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daItemKantera_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    MTXCopy(mDoMtx_stack_c::get(), mpModel->mBaseTransformMtx);
}

int daItemKantera_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mAcchCir.SetWall(30.0f, 30.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), NULL, NULL);
    mCcStts.Init(0, 0xff, this);
    mCcCyl.Set(l_cyl_src);
    mCcCyl.SetStts(&mCcStts);
    mCcCyl.SetR(dItem_data::getR(m_itemNo));
    mCcCyl.SetH(dItem_data::getH(m_itemNo));
    fopAcM_SetCullSize(this, fopAc_CULLSPHERE_0_e);
    actionWaitInit();
    fopAcM_SetGravity(this, -6.0f);
    fopAcM_OnCarryType(this, fopAcM_CARRY_ITEM);
    return 1;
}

int daItemKantera_c::__CreateHeap() {
    return 1;
}

int daItemKantera_c::create() {
    fopAcM_ct(this, daItemKantera_c);

    if (field_0x937 == 0) {
        field_0x938 = home.angle.x;
        field_0x93a = home.angle.z;
        home.angle.z = 0;
        home.angle.x = 0;
        current.angle.z = 0;
        current.angle.x = 0;
        shape_angle.z = 0;
        shape_angle.x = 0;
        field_0x937 = 1;
    }
    m_itemNo = 0x48;
    if (checkItemGet(m_itemNo, 1)) {
        return cPhs_ERROR_e;
    }
    int phase = dComIfG_resLoad(&mPhase, dItem_data::getFieldArc(m_itemNo));
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckFieldItemCreateHeap, 0x1940)) {
            return cPhs_ERROR_e;
        }
        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }
    return phase;
}

void daItemKantera_c::bg_check() {
    f32 dVar6 = 0.7f;
    if (mAcch.ChkWallHit()) {
        if (speedF > 5.0f) {
            Reflect(&speed, mAcchCir, 1.0f);
        }
        current.angle.y = cM_atan2s(speed.x, speed.z);
    }
    if (mAcch.ChkGroundLanding()) {
        cXyz l_speed = speed;
        if (speedF > 5.0f) {
            Reflect(&l_speed, mAcchCir, 0.7f);
        }
        f32 tmp = field_0x93c.y;
        tmp = -tmp;
        speed.y = tmp * dVar6;
        if (speed.y < 3.0f) {
            fopAcM_SetSpeedF(this, 0.0f);
        }
        field_0x936++;
    }
}

int daItemKantera_c::actionInit() {
    show();
    actionWaitInit();
    return 1;
}

int daItemKantera_c::actionWaitInit() {
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    attention_info.distances[fopAc_attn_CARRY_e] = 16;
    attention_info.position = current.pos;
    show();
    setStatus(1);
    return 1;
}

int daItemKantera_c::actionWait() {
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    bg_check();
    if (mAcch.ChkGroundHit()) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        speedF *= 0.9f;
        if (speedF < 1.0f) {
            speedF = 0.0f;
        }
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (fopAcM_checkCarryNow(this)) {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        if (player->getGrabUpStart()) {
            fopAcM_cancelCarryNow(this);
            initActionOrderGetDemo();
        }
    } else {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }
    attention_info.position = current.pos;
    return 1;
}

int daItemKantera_c::initActionOrderGetDemo() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    hide();
    fopAcM_orderItemEvent(this, 0, 0);
    eventInfo.onCondition(8);
    field_0x92c = fopAcM_createItemForTrBoxDemo(&current.pos, m_itemNo, -1, fopAcM_GetRoomNo(this),
                                                NULL, NULL);
    setStatus(2);
    return 1;
}

int daItemKantera_c::actionOrderGetDemo() {
    if (eventInfo.checkCommandItem()) {
        setStatus(3);
        if (field_0x92c != -1) {
            dComIfGp_event_setItemPartnerId(field_0x92c);
        }
    } else {
        fopAcM_orderItemEvent(this, 0, 0);
        eventInfo.onCondition(8);
    }
    return 1;
}

int daItemKantera_c::actionGetDemo() {
    if (dComIfGp_evmng_endCheck("DEFAULT_GETITEM")) {
        dComIfGp_event_reset();
        fopAcM_delete(this);
    }
    return 1;
}

int daItemKantera_c::execute() {
    static daItemKantera_c::actionFunc l_demoFunc[4] = {
        &daItemKantera_c::actionInit,
        &daItemKantera_c::actionWait,
        &daItemKantera_c::actionOrderGetDemo,
        &daItemKantera_c::actionGetDemo,
    };
    field_0x93c = speed;
    field_0x930++;
    (this->*(l_demoFunc[mStatus]))();
    eyePos = current.pos;
    attention_info.position = current.pos;
    setBaseMtx();
    return 1;
}

int daItemKantera_c::draw() {
    if (!chkDraw()) {
        return 1;
    }
    DrawBase();
    return 1;
}

int daItemKantera_c::_delete() {
    DeleteBase(dItem_data::getFieldArc(m_itemNo));
    return 1;
}

static int daItemKantera_Draw(daItemKantera_c* i_this) {
    return i_this->draw();
}

static int daItemKantera_Execute(daItemKantera_c* i_this) {
    return i_this->execute();
}

static int daItemKantera_Delete(daItemKantera_c* i_this) {
    return i_this->_delete();
}

static int daItemKantera_Create(fopAc_ac_c* i_this) {
    return static_cast<daItemKantera_c*>(i_this)->create();
}


static actor_method_class l_daItemKantera_Method = {
    (process_method_func)daItemKantera_Create,
    (process_method_func)daItemKantera_Delete,
    (process_method_func)daItemKantera_Execute,
    NULL,
    (process_method_func)daItemKantera_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Kantera = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_Kantera,        // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daItemKantera_c), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  238,                     // mPriority
  &l_daItemKantera_Method, // sub_method
  0x00040100,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
