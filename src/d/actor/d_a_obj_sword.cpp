/**
* @file d_a_obj_sword.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_sword.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"

static cull_box l_cull_box = {{-200.0f, 0.0f, -200.0f}, {200.0f, 100.0f, 200.0f}};

void daObjSword_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjSword_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjSword_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);
    fopAcM_OnCarryType(this, fopAcM_CARRY_UNK_30);
    show();
    execute();
    return 1;
}

cPhs__Step daObjSword_c::create() {
    fopAcM_ct(this, daObjSword_c);
    m_itemNo = 0x28;
    if (fopAcM_isItem(this, getItemBit())) {
        return cPhs_ERROR_e;
    }

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, dItem_data::getFieldArc(m_itemNo));
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckFieldItemCreateHeap, 0x4000)) {
            return cPhs_ERROR_e;
        }
        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }
    return phase;
}

int daObjSword_c::actionWait() {
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
    return 1;
}

int daObjSword_c::initActionOrderGetDemo() {
    hide();
    fopAcM_orderItemEvent(this, 0, 0);
    eventInfo.onCondition(8);
    mProcID = fopAcM_createItemForTrBoxDemo(&current.pos, m_itemNo, -1, fopAcM_GetRoomNo(this),
                                            NULL, NULL);
    setStatus(1);
    return 1;
}

int daObjSword_c::actionOrderGetDemo() {
    if (eventInfo.checkCommandItem()) {
        setStatus(2);
        if (mProcID != fpcM_ERROR_PROCESS_ID_e) {
            dComIfGp_event_setItemPartnerId(mProcID);
        }
    } else {
        fopAcM_orderItemEvent(this, 0, 0);
        eventInfo.onCondition(8);
    }
    return 1;
}

int daObjSword_c::actionGetDemo() {
    if (dComIfGp_evmng_endCheck("DEFAULT_GETITEM")) {
        fopAcM_onItem(this, getItemBit());
        fopAcM_onSwitch(this, getSwbit());
        dComIfGp_event_reset();
        fopAcM_delete(this);
    }
    return 1;
}

int daObjSword_c::execute() {
    static const actionFunc l_demoFunc[] = {
        &daObjSword_c::actionWait, &daObjSword_c::actionOrderGetDemo, &daObjSword_c::actionGetDemo};
    field_0x92c++;
    (this->*l_demoFunc[mStatus])();
    attention_info.position = current.pos;
    eyePos = current.pos;
    setBaseMtx();
    return 1;
}

int daObjSword_c::draw() {
    if (!chkDraw()) {
        return 1;
    }
    DrawBase();
    return 1;
}

int daObjSword_c::_delete() {
    DeleteBase(dItem_data::getFieldArc(m_itemNo));
    return 1;
}

static int daObjSword_Draw(daObjSword_c* param_0) {
    return param_0->draw();
}

static int daObjSword_Execute(daObjSword_c* param_0) {
    return param_0->execute();
}

static int daObjSword_Delete(daObjSword_c* param_0) {
    return param_0->_delete();
}

static int daObjSword_Create(daObjSword_c* param_0) {
    return param_0->create();
}

static actor_method_class l_daObjSword_Method = {
    (process_method_func)daObjSword_Create,  (process_method_func)daObjSword_Delete,
    (process_method_func)daObjSword_Execute, 0,
    (process_method_func)daObjSword_Draw,
};

actor_process_profile_definition g_profile_Obj_Sword = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Sword,          // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjSword_c),    // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    586,                     // mPriority
    &l_daObjSword_Method,    // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
