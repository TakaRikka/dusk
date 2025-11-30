/**
 * d_a_obj_zcloth.cpp
 * Object - Zora Armor
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_zcloth.h"
#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"
#include "d/d_a_itembase_static.h"
#include "d/d_procname.h"

void daObjZCloth_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjZCloth_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjZCloth_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    return 1;
}

int daObjZCloth_c::create() {
    fopAcM_ct(this, daObjZCloth_c);
    m_itemNo = 0x31;
    int phase = dComIfG_resLoad(&mPhase, dItem_data::getFieldArc(m_itemNo));
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)CheckFieldItemCreateHeap, 0x2fb0)) {
            return cPhs_ERROR_e;
        }
        else if (!Create()) {
            return cPhs_ERROR_e;
        } 
    }
    return phase;
}

int daObjZCloth_c::execute() {
    setBaseMtx();
    return 1;
}

int daObjZCloth_c::draw() {
    DrawBase();
    return 1;
}

int daObjZCloth_c::_delete() {
    dComIfG_resDelete(&mPhase, dItem_data::getFieldArc(m_itemNo)); 
    return 1;
}  

static int daObjZCloth_Draw(daObjZCloth_c* i_this) {
    return i_this->draw();
}

static int daObjZCloth_Execute(daObjZCloth_c* i_this) {
    return i_this->execute();
}

static int daObjZCloth_Delete(daObjZCloth_c* i_this) {
    return i_this->_delete();
}

static int daObjZCloth_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjZCloth_c*>(i_this)->create();
}

static actor_method_class l_daObjZCloth_Method = {
    (process_method_func)daObjZCloth_Create,
    (process_method_func)daObjZCloth_Delete,
    (process_method_func)daObjZCloth_Execute,
    (process_method_func)NULL,
    (process_method_func)daObjZCloth_Draw,
};

extern actor_process_profile_definition g_profile_Obj_ZoraCloth = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_ZoraCloth,
    &g_fpcLf_Method.base,
    sizeof(daObjZCloth_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x220,
    &l_daObjZCloth_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
