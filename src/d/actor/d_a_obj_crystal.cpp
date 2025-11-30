/**
 * d_a_obj_crystal.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_crystal.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjCrystal_c*>(i_this)->CreateHeap();
}

void daObjCrystal_c::initBaseMtx() {
    mpModel[0]->setBaseScale(scale);
    mpModel[1]->setBaseScale(scale);
    setBaseMtx();
}

void daObjCrystal_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModel[1]->setBaseTRMtx(mDoMtx_stack_c::get());
}

static char* l_arcName = "H_Suisho";

int daObjCrystal_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel[0]->getBaseTRMtx());

    mpEmitter = dComIfGp_particle_set(0x8BED, &current.pos, &home.angle, &scale, 255, NULL, -1,
                                      NULL, NULL, NULL);
    mJointID = 0xFFFF;

    JUTNameTab* joint_name = mpModel[0]->getModelData()->getJointTree().getJointName();
    for (u16 i = 0; i < mpModel[0]->getModelData()->getJointNum(); i++) {
        if (strcmp(joint_name->getName(i), "polySurface1") == 0) {
            mJointID = i;
        }
    }

    mpModel[0]->calc();

    if (mpEmitter != NULL) {
        mpEmitter->setGlobalRTMatrix(mpModel[0]->getAnmMtx(mJointID));
    }

    return 1;
}

int daObjCrystal_c::CreateHeap() {
    static int const l_bmd[] = {4, 7};
    static u32 const l_diff_flag[] = {0, 0x80000};
    static u32 const l_flag[] = {0x19000084, 0x11000084};

    for (int i = 0; i < 2; i++) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmd[i]);

        mpModel[i] = mDoExt_J3DModel__create(modelData, l_diff_flag[i], l_flag[i]);
        if (mpModel[i] == NULL) {
            return 0;
        }
    }

    return 1;
}

int daObjCrystal_c::create() {
    fopAcM_ct(this, daObjCrystal_c);

    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x4000)) {
            return cPhs_ERROR_e;
        }

        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }

    return phase;
}

int daObjCrystal_c::execute() {
    setBaseMtx();
    return 1;
}

int daObjCrystal_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);

    for (int i = 0; i < 2; i++) {
        g_env_light.setLightTevColorType_MAJI(mpModel[i], &tevStr);
        mDoExt_modelUpdateDL(mpModel[i]);
    }

    return 1;
}

int daObjCrystal_c::_delete() {
    if (mpEmitter != NULL) {
        mpEmitter->becomeInvalidEmitter();
        mpEmitter->quitImmortalEmitter();
        mpEmitter = NULL;
    }

    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjCrystal_Draw(daObjCrystal_c* i_this) {
    return i_this->draw();
}

static int daObjCrystal_Execute(daObjCrystal_c* i_this) {
    return i_this->execute();
}

static int daObjCrystal_Delete(daObjCrystal_c* i_this) {
    return i_this->_delete();
}

static int daObjCrystal_Create(daObjCrystal_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daObjCrystal_Method = {
    (process_method_func)daObjCrystal_Create,  (process_method_func)daObjCrystal_Delete,
    (process_method_func)daObjCrystal_Execute, (process_method_func)NULL,
    (process_method_func)daObjCrystal_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Crystal = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_Crystal,
    &g_fpcLf_Method.base,
    sizeof(daObjCrystal_c),
    0,
    0,
    &g_fopAc_Method.base,
    579,
    &l_daObjCrystal_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
