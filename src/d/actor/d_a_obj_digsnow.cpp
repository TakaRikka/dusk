//
// d_a_obj_digsnow.cpp
// Object - Dig Snow
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_digsnow.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include <cmath.h>

void daObjDigSnow_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjDigSnow_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjDigSnow_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());

    attention_info.distances[0x07] = 31;

    mode_init_wait();

    return TRUE;
}

static char* l_arcName = "Y_horiyuk";

int daObjDigSnow_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(0x9F, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    if (mpModel == NULL) {
        return FALSE;
    }

    return TRUE;
}

int daObjDigSnow_c::create1st() {
    if (dComIfGs_isSwitch(getSwNo(), fopAcM_GetHomeRoomNo(this))) {
        return cPhs_ERROR_e;
    }

    cPhs__Step phase_state = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = (cPhs__Step)MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0x1000, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daObjDigSnow_c::Execute(Mtx** i_mtx) {
    if (daPy_py_c::checkNowWolf()) {
        attention_info.flags = fopAc_AttnFlag_ETC_e;
    } else {
        attention_info.flags = 0;
    }

    action();

    *i_mtx = &mBgMtx;
    setBaseMtx();

    return TRUE;
}

void daObjDigSnow_c::action() {
    typedef void (daObjDigSnow_c::*action_func)();
    static action_func l_func[] = {
        &daObjDigSnow_c::mode_wait,
        &daObjDigSnow_c::mode_dig,
        &daObjDigSnow_c::mode_end,
    };

    (this->*l_func[mAction])();
}

void daObjDigSnow_c::mode_init_wait() {
    mAction = ACTION_WAIT_e;
}

void daObjDigSnow_c::mode_wait() {
    if (mMode == MODE_DIG_e) {
        mode_init_dig();
    }
}

void daObjDigSnow_c::mode_init_dig() {
    dBgS_ObjGndChk obj_gnd_chk;

    obj_gnd_chk.SetActorPid(base.base.id);
    obj_gnd_chk.SetPos(&current.pos);

    f32 gnd_height = dComIfG_Bgsp().GroundCross(&obj_gnd_chk);
    if (gnd_height != -G_CM3D_F_INF) {
        mGroundHeight = gnd_height;
    } else {
#if DEBUG
        // "No BG under digsnow!"
        OS_REPORT_ERROR("掘る雪の下にＢＧがありません！");
        fopAcM_delete(this);
#endif
    }

    mAction = 1;
}

void daObjDigSnow_c::mode_dig() {
    f32 step = std::fabs(((current.pos.y - mGroundHeight) + 10.0f) / 35.0f);
    if (step < 1.0f) {
        step = 1.0f;
    }

    if (cLib_chaseF(&current.pos.y, mGroundHeight - 30.0f, step) && mMode == MODE_END_e) {
        mode_init_end();
    }
}

void daObjDigSnow_c::mode_init_end() {
    dComIfGs_onSwitch(getSwNo(), fopAcM_GetHomeRoomNo(this));
    fopAcM_delete(this);
    
    mAction = 2;
}

void daObjDigSnow_c::mode_end() {}

int daObjDigSnow_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return TRUE;
}

int daObjDigSnow_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return TRUE;
}

static int daObjDigSnow_create1st(daObjDigSnow_c* i_this) {
    fopAcM_ct(i_this, daObjDigSnow_c);
    return i_this->create1st();
}

static int daObjDigSnow_MoveBGDelete(daObjDigSnow_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjDigSnow_MoveBGExecute(daObjDigSnow_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjDigSnow_MoveBGDraw(daObjDigSnow_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjDigSnow_METHODS = {
    (process_method_func)daObjDigSnow_create1st,     (process_method_func)daObjDigSnow_MoveBGDelete,
    (process_method_func)daObjDigSnow_MoveBGExecute, NULL,
    (process_method_func)daObjDigSnow_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_DigSnow = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_DigSnow,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjDigSnow_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    634,                     // mPriority
    &daObjDigSnow_METHODS,   // sub_method
    0x44100,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
