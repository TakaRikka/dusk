/**
 * d_a_obj_mirror_6pole.cpp
 * Mirror Chamber Sage Poles
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_mirror_6pole.h"
#include "d/d_procname.h"
#include "f_op/f_op_actor_mng.h"

static char* l_arcName = "MR-6Pole";

const actionFunc daObjMirror6Pole_c::ActionTable[][2] = {
    {&daObjMirror6Pole_c::initWait, &daObjMirror6Pole_c::executeWait},
    {&daObjMirror6Pole_c::initDemo, &daObjMirror6Pole_c::executeDemo},
};

void daObjMirror6Pole_c::setAction(Mode_e i_action) {
    JUT_ASSERT(89, i_action < MODE_MAX_e);
    Mode_e oldMode = mMode;
    mMode = i_action;
    mAction = (actionFunc*)ActionTable[mMode];

    callInit();
}

void daObjMirror6Pole_c::callInit() {
    JUT_ASSERT(123, mAction != NULL);
    (this->*mAction[0])();
}

void daObjMirror6Pole_c::callExecute() {
    JUT_ASSERT(136, mAction != NULL);
    (this->*mAction[1])();
}

void daObjMirror6Pole_c::initWait() {
    mpBck->setPlaySpeed(0.0f);
}

void daObjMirror6Pole_c::executeWait() {
    if (dComIfGp_event_runCheck()) {
        setAction(MODE_DEMO_e);
    }
}

void daObjMirror6Pole_c::initDemo() {}

void daObjMirror6Pole_c::executeDemo() {
    if (!dComIfGp_event_runCheck()) {
        setAction(MODE_WAIT_e);
    }

    setBaseMtx();
}

void daObjMirror6Pole_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObjMirror6Pole_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    setBaseMtx();
}

void daObjMirror6Pole_c::create_init() {
    initBaseMtx();
    attention_info.flags = 0;
    setAction(MODE_WAIT_e);
}

int daObjMirror6Pole_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daObjMirror6Pole_c* a_this = (daObjMirror6Pole_c*)i_this;
    return a_this->CreateHeap();
}

int daObjMirror6Pole_c::CreateHeap() {
    BOOL r26 = FALSE;
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 7);
    JUT_ASSERT(318, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(327, bck != NULL);
    mpBck = new mDoExt_bckAnm();

    f32 speed = isSwitch() ? 1.0f : 0.0f;

    if (mpBck == NULL || !mpBck->init(bck, FALSE, J3DFrameCtrl::EMode_NONE, speed, 0, -1, false)) {
        return 0;
    }

    if (isSwitch()) {
        mpBck->setFrame(bck->getFrameMax());
    }

    return 1;
}

int daObjMirror6Pole_c::create() {
    fopAcM_ct(this, daObjMirror6Pole_c);

    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x1C00)) {
            return cPhs_ERROR_e;
        }

        create_init();
    }

    return phase;
}

static int daObjMirror6Pole_Create(fopAc_ac_c* i_this) {
    fopAcM_RegisterCreateID(daObjMirror6Pole_c, i_this, "Obj_Mirror6Pole");
    return static_cast<daObjMirror6Pole_c*>(i_this)->create();
}

daObjMirror6Pole_c::~daObjMirror6Pole_c() {
    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daObjMirror6Pole_Delete(daObjMirror6Pole_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Obj_Mirror6Pole");
    i_this->~daObjMirror6Pole_c();
    return 1;
}

int daObjMirror6Pole_c::execute() {
    callExecute();
    mpBck->play();
    return 1;
}

static int daObjMirror6Pole_Execute(daObjMirror6Pole_c* i_this) {
    return i_this->execute();
}

int daObjMirror6Pole_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    mpBck->entry(mpModel->getModelData());

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    mpBck->remove(mpModel->getModelData());
    return 1;
}

static int daObjMirror6Pole_Draw(daObjMirror6Pole_c* i_this) {
    return i_this->draw();
}

static int daObjMirror6Pole_IsDelete(daObjMirror6Pole_c* i_this) {
    return 1;
}

static actor_method_class l_daObjMirror6Pole_Method = {
    (process_method_func)daObjMirror6Pole_Create,  (process_method_func)daObjMirror6Pole_Delete,
    (process_method_func)daObjMirror6Pole_Execute, (process_method_func)daObjMirror6Pole_IsDelete,
    (process_method_func)daObjMirror6Pole_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Mirror6Pole = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Mirror6Pole,
    &g_fpcLf_Method.base,
    sizeof(daObjMirror6Pole_c),
    0,
    0,
    &g_fopAc_Method.base,
    682,
    &l_daObjMirror6Pole_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
