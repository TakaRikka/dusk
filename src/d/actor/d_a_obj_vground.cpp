/**
 * d_a_obj_vground.cpp
 * Object - Volcano Ground
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_vground.h"
#include "d/d_procname.h"

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjVGnd_c*>(i_this)->CreateHeap();
}

void daObjVGnd_c::initBaseMtx() {
    current.pos.y += 5.0f;
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjVGnd_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    MTXCopy(mDoMtx_stack_c::now, mpModel->mBaseTransformMtx);
}

int daObjVGnd_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    execute();
    return 1;
}

static char* l_arcName = "M_VolcGnd";

int daObjVGnd_c::CreateHeap() {
    J3DModelData* mpObjectRes = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    mpModel = mDoExt_J3DModel__create(mpObjectRes, 0x80000, 0x11000284);
    if (!mpModel) {
        return 0;
    }

    J3DAnmTextureSRTKey* mpTextureSRTKey =
        (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0xb);
    mpBtk = new mDoExt_btkAnm();
    if ((!mpBtk) || (!mpBtk->init(mpObjectRes, mpTextureSRTKey, 1, NULL, 1.0f, NULL, -1))) {
        return 0;
    }

    J3DAnmTevRegKey* mpTevReqKey = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 8);
    mpBrk = new mDoExt_brkAnm();
    if ((!mpBrk) || (!mpBrk->init(mpObjectRes, mpTevReqKey, 1, NULL, 1.0f, NULL, -1))) {
        return 0;
    }

    return 1;
}

int daObjVGnd_c::create() {
    fopAcM_ct(this, daObjVGnd_c);
    int phase = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)CheckCreateHeap, 0x960)) {
            return cPhs_ERROR_e;
        } else if (!Create()) {
            return cPhs_ERROR_e;
        }
    }
    return phase;
}

int daObjVGnd_c::execute() {
        /* dSv_event_flag_c::M_032 - Main Event - Melted Zora river ice with magma rock */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[65])) {
        mpBrk->setFrame(2.0f);
        mpBtk->setFrame(2.0f);
    } else {
        if (fopAcM_isSwitch(this, getSwbit())) {
            mpBrk->setFrame(1.0f);
            mpBtk->setFrame(1.0f);
        } else {
            mpBrk->setFrame(0.0f);
            mpBtk->setFrame(0.0f);
        }
    }
    setBaseMtx();
    return 1;
}

int daObjVGnd_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mpBrk->entry(mpModel->getModelData());
    mpBtk->entry(mpModel->getModelData());
    dComIfGd_setListDarkBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjVGnd_c::_delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static int daObjVGnd_Draw(daObjVGnd_c* i_this) {
    return i_this->draw();
}

static int daObjVGnd_Execute(daObjVGnd_c* i_this) {
    return i_this->execute();
}

static int daObjVGnd_Delete(daObjVGnd_c* i_this) {
    return i_this->_delete();
}

static int daObjVGnd_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjVGnd_c*>(i_this)->create();
}

static actor_method_class l_daObjVGnd_Method = {
    (process_method_func)daObjVGnd_Create,  (process_method_func)daObjVGnd_Delete,
    (process_method_func)daObjVGnd_Execute, (process_method_func)NULL,
    (process_method_func)daObjVGnd_Draw,
};

extern actor_process_profile_definition g_profile_Obj_VolcGnd = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_VolcGnd,
    &g_fpcLf_Method.base,
    sizeof(daObjVGnd_c),
    0,
    0,
    &g_fopAc_Method.base,
    595,
    &l_daObjVGnd_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
