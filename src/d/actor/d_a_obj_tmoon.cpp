/**
 * d_a_obj_tmoon.cpp
 * Moon in Hidden Skill Scene
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_tmoon.h"
#include "f_op/f_op_draw_tag.h"

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjTMoon_c*>(i_this)->CreateHeap();
}

void daObjTMoon_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjTMoon_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    MTXCopy(mDoMtx_stack_c::now, mpModel->mBaseTransformMtx);
}

int daObjTMoon_c::Create() {
    field_0x574 = getEventBit1();
    field_0x576 = getEventBit2();
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    return 1;
}

static const char* l_arcName = "A_TMoon";

int daObjTMoon_c::CreateHeap() {
    mpModel = mDoExt_J3DModel__create((J3DModelData*)dComIfG_getObjectRes(l_arcName, 3), 0x80000,
                                      0x11000084);
    return mpModel != NULL ? 1 : 0;
}

int daObjTMoon_c::create() {
    fopAcM_ct(this, daObjTMoon_c);
    int phase = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)CheckCreateHeap, 0x880)) {
            return cPhs_ERROR_e;
        } else if (!Create()) {
            return cPhs_ERROR_e;
        }
    }
    return phase;
}

int daObjTMoon_c::execute() {
    if ((field_0x576 != 0x3ff) &&
            (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x576])) ||
        (field_0x574 != 0x3ff) &&
            (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x574])))
    {
        fopDwTg_DrawQTo(&draw_tag);
        return 1;
    } else if ((field_0x574 != 0x3ff) &&
               (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x574])))
    {
        fopDwTg_ToDrawQ(&draw_tag, fpcM_DrawPriority(this));
        return 1;
    }
    return 1;
}

int daObjTMoon_c::draw() {
    g_env_light.moon_pos = current.pos;
    return 1;
}

int daObjTMoon_c::_delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static int daObjTMoon_Draw(daObjTMoon_c* i_this) {
    return i_this->draw();
}

static int daObjTMoon_Execute(daObjTMoon_c* i_this) {
    return i_this->execute();
}

static int daObjTMoon_Delete(daObjTMoon_c* i_this) {
    return i_this->_delete();
}

static int daObjTMoon_Create(daObjTMoon_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daObjTMoon_Method = {
    (process_method_func)daObjTMoon_Create,  (process_method_func)daObjTMoon_Delete,
    (process_method_func)daObjTMoon_Execute, (process_method_func)NULL,
    (process_method_func)daObjTMoon_Draw,
};

actor_process_profile_definition g_profile_Obj_TMoon = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_TMoon_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjTMoon_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_TMoon_e,
    /* Actor SubMtd */ &l_daObjTMoon_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
