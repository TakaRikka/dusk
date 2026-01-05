/**
 * @file d_a_obj_lv4bridge.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4bridge.h"

void daObjLv4Brg_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjLv4Brg_c::setBaseMtx() {
    cXyz sp14(0.0f, 0.0f, field_0x5bc);
    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVec(&sp14, &sp14);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(sp14);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    cXyz sp8(0.0f, 1700.0f, -5050.0f);
    mDoMtx_stack_c::multVec(&sp8, &field_0x5c4);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjLv4Brg_c::Create() {
    if (fopAcM_isSwitch(this, getSwbit())) {
        field_0x5bc = 2000.0f;
        mode_init_dead();
    } else {
        field_0x5bc = 0.0f;
        mode_init_wait();
    }

    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    MoveBGExecute();
    return 1;
}

static char* l_arcName = "Obj_l4brg";

int daObjLv4Brg_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(170, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daObjLv4Brg_c::create1st() {
    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0x2260, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daObjLv4Brg_c::Execute(Mtx** param_0) {
    action();
    *param_0 = &mBgMtx;
    setBaseMtx();
    return 1;
}

void daObjLv4Brg_c::action() {
    static void (daObjLv4Brg_c::*l_func[])() = {
        &daObjLv4Brg_c::mode_wait,
        &daObjLv4Brg_c::mode_move,
        &daObjLv4Brg_c::mode_dead,
    };

    (this->*l_func[mMode])();
    eventUpdate();
}

void daObjLv4Brg_c::mode_init_wait() {
    mMode = 0;
}

void daObjLv4Brg_c::mode_wait() {
    if (fopAcM_isSwitch(this, getSwbit())) {
        if (getEvID() != 0xFF) {
            orderEvent(getEvID(), 0xFF, 1);
            return;
        }

        dComIfGp_getVibration().StartQuake(4, 0xF, cXyz(0.0f, 1.0f, 0.0f));
        mode_init_move();
    }
}

void daObjLv4Brg_c::mode_init_move() {
    speed.y = 0.0f;
    mMode = 1;
}

void daObjLv4Brg_c::mode_move() {
    mDoAud_seStartLevel(Z2SE_OBJ_DS_END_BRG, &field_0x5c4, 0, 0);
    cLib_chaseF(&speed.y, 20.0f, 2.0f);

    if (cLib_chaseF(&field_0x5bc, 2000.0f, speed.y)) {
        mDoAud_seStart(Z2SE_OBJ_DS_END_BRG_ST, &field_0x5c4, 0, 0);
        dComIfGp_getVibration().StopQuake(0x1F);
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        mode_init_dead();
    }
}

void daObjLv4Brg_c::mode_init_dead() {
    mMode = 2;
}

void daObjLv4Brg_c::mode_dead() {}

bool daObjLv4Brg_c::eventStart() {
    dComIfGp_getVibration().StartQuake(4, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    mode_init_move();
    return true;
}

int daObjLv4Brg_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjLv4Brg_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjLv4Brg_create1st(daObjLv4Brg_c* i_this) {
    fopAcM_ct(i_this, daObjLv4Brg_c);
    return i_this->create1st();
}

static int daObjLv4Brg_MoveBGDelete(daObjLv4Brg_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjLv4Brg_MoveBGExecute(daObjLv4Brg_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjLv4Brg_MoveBGDraw(daObjLv4Brg_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjLv4Brg_METHODS = {
    (process_method_func)daObjLv4Brg_create1st,
    (process_method_func)daObjLv4Brg_MoveBGDelete,
    (process_method_func)daObjLv4Brg_MoveBGExecute,
    0,
    (process_method_func)daObjLv4Brg_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Lv4Bridge = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv4Bridge,     // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjLv4Brg_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  581,                    // mPriority
  &daObjLv4Brg_METHODS,   // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
