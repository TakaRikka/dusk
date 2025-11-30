/**
 * d_a_obj_lv4floor.cpp
 * Moving sand-floor in Stallord Arena
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4floor.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

void daObjLv4Floor_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjLv4Floor_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mMoveYPos, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjLv4Floor_c::Create() {
    initBaseMtx();
    return 1;
}

static char* l_arcName = "P_L4Floor";

int daObjLv4Floor_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daObjLv4Floor_c::create1st() {
    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase != cPhs_COMPLEATE_e) {
        return phase;
    }

    if (fopAcM_isSwitch(this, getSwbit())) {
        return cPhs_ERROR_e;
    }

    phase = MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0x4000, NULL);
    if (phase == cPhs_ERROR_e) {
        return phase;
    }

    return phase;
}

int daObjLv4Floor_c::Execute(Mtx** param_0) {
    action();
    *param_0 = &mBgMtx;
    setBaseMtx();

    return 1;
}

void daObjLv4Floor_c::action() {
    typedef void (daObjLv4Floor_c::*actionFunc)();
    static actionFunc l_func[] = {&daObjLv4Floor_c::mode_wait, &daObjLv4Floor_c::mode_move,
                                  &daObjLv4Floor_c::mode_dead};

    (this->*l_func[mAction])();
}

void daObjLv4Floor_c::mode_wait() {
    if (fopAcM_isSwitch(this, getSwbit())) {
        mode_init_move();
    }
}

void daObjLv4Floor_c::mode_init_move() {
    speed.y = 0.0f;
    mAction = MODE_MOVE_e;
}

void daObjLv4Floor_c::mode_move() {
    cLib_chaseF(&speed.y, 3.8f, 0.08f);

    if (cLib_chaseF(&mMoveYPos, -1500.0f, speed.y)) {
        mode_init_dead();
    }
}

void daObjLv4Floor_c::mode_init_dead() {
    mAction = MODE_DEAD_e;
}

void daObjLv4Floor_c::mode_dead() {
    fopAcM_delete(this);
}

int daObjLv4Floor_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjLv4Floor_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjLv4Floor_create1st(daObjLv4Floor_c* i_this) {
    fopAcM_ct(i_this, daObjLv4Floor_c);
    return i_this->create1st();
}

static int daObjLv4Floor_MoveBGDelete(daObjLv4Floor_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjLv4Floor_MoveBGExecute(daObjLv4Floor_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjLv4Floor_MoveBGDraw(daObjLv4Floor_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjLv4Floor_METHODS = {
    (process_method_func)daObjLv4Floor_create1st,
    (process_method_func)daObjLv4Floor_MoveBGDelete,
    (process_method_func)daObjLv4Floor_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjLv4Floor_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Lv4Floor = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Lv4Floor,
    &g_fpcLf_Method.base,
    sizeof(daObjLv4Floor_c),
    0,
    0,
    &g_fopAc_Method.base,
    584,
    &daObjLv4Floor_METHODS,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
