#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_hata.h"
#include "f_op/f_op_actor.h"

static int daObjHata_c_createHeap(fopAc_ac_c* a_this) {
    return static_cast<daObjHata_c*>(a_this)->createHeap();
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 != 0) {
        return 1;
    }

    int jntNo = i_joint->getJntNo();
    if (jntNo < 0 || jntNo >= 3) {
        return 1;
    }

    J3DModel* model = j3dSys.getModel();
    daObjHata_c* i_this = (daObjHata_c*)model->getUserArea();
    if (model == NULL || i_this == NULL) {
        return 1;
    }

    csXyz angle;
    i_this->getJntAngle(&angle, jntNo);
    MTXCopy(model->getAnmMtx(jntNo), mDoMtx_stack_c::get());
    mDoMtx_stack_c::ZXYrotM(angle);
    model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

static char* l_arcName = "Hata";

daObjHata_c::daObjHata_c() {}

daObjHata_c::~daObjHata_c() {
    dComIfG_resDelete(this, l_arcName);
}

int daObjHata_c::createHeap() {
    J3DModelData* a_model_data_p = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 3);
    JUT_ASSERT(109, a_model_data_p != NULL);
    model = mDoExt_J3DModel__create(a_model_data_p, 0x80000, 0x11000084);
    if (model == NULL) {
        return 0;
    }

    return 1;
}

cPhs__Step daObjHata_c::create() {
    fopAcM_ct(this, daObjHata_c);

    cPhs__Step phase_step = (cPhs__Step)dComIfG_resLoad(this, l_arcName);
    if (phase_step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daObjHata_c_createHeap, 0x870)) {
            return cPhs_ERROR_e;
        }

        init();
        setModelMtx();
        fopAcM_SetMtx(this, model->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, model->getModelData());

        for (int i = 0; i < 3; i++) {
            J3DJoint* jnt_node_p = model->getModelData()->getJointNodePointer(i);
            if (jnt_node_p != NULL) {
                jnt_node_p->setCallBack(nodeCallBack);
                model->setUserArea((uintptr_t)this);
            }
        }
    }

    return phase_step;
}

int daObjHata_c::Delete() {
    this->~daObjHata_c();
    return 1;
}

int daObjHata_c::draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(model);
    dComIfGd_setList();
    return 1;
}

int daObjHata_c::execute() {
    moveSwing();
    setModelMtx();
    return 1;
}

void daObjHata_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObjHata_c::init() {
    for (int i = 0; i < 3; i++) {
        field_0x58c[i] = cM_rndFX(1280.0f);
        field_0x598[i] = cM_rndFX(1280.0f);
        field_0x5bc[i] = 1;
        field_0x5bf[i] = -1;
        field_0x5a4[i] = cM_rndF(1.0f);
        field_0x5b0[i] = cM_rndF(1.0f);
    }
}

void daObjHata_c::moveSwing() {
    #undef SING
    static int const SING[3] = {
        1, -1, 1,
    };

    static int const AIM_ANGLE_X[3] = {
        0, 0x2BC, 0x2BC,
    };

    static int const STEP_ANGLE_X[3] = {
        0, 0x3C, 0x5A,
    };
    
    for (int i = 0; i < 3; i++) {
        f32 fVar1 = jnt_angle[i].x;
        f32 fVar2 = jnt_angle[i].z;
        if (cLib_chaseF(&fVar2, field_0x598[i] * (field_0x5bc[i] * SING[i]), field_0x5a4[i] * 50.0f) != 0) {
            field_0x598[i] = cM_rndF(1500.0f);

            if (i == 2) {
                field_0x5bc[i] *= -1;
                field_0x5a4[i] = cM_rndF(1.0f) + 0.1f;
            }
        }

        if (cLib_chaseF(&fVar1, field_0x58c[i] * (field_0x5bf[i] * SING[i]), field_0x5b0[i] * STEP_ANGLE_X[i]) != 0) {
            field_0x58c[i] = cM_rndF(AIM_ANGLE_X[i]);

            if (i == 2) {
                field_0x5bf[i] *= -1;
                field_0x5b0[i] = cM_rndF(1.0f) + 0.5f;
            }
        }

        jnt_angle[i].x = fVar1;
        jnt_angle[i].z = fVar2;
    }
}

static int daObjHata_create(daObjHata_c* i_this) {
    fopAcM_ct(i_this, daObjHata_c);
    return i_this->create();
}

static int daObjHata_Delete(daObjHata_c* i_this) {
    return i_this->Delete();
}

static int daObjHata_execute(daObjHata_c* i_this) {
    return i_this->execute();
}

static int daObjHata_draw(daObjHata_c* i_this) {
    return i_this->draw();
}

static actor_method_class daObjHata_METHODS = {
    (process_method_func)daObjHata_create,
    (process_method_func)daObjHata_Delete,
    (process_method_func)daObjHata_execute,
    (process_method_func)NULL,
    (process_method_func)daObjHata_draw,
};

extern actor_process_profile_definition g_profile_Obj_Hata = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Hata,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjHata_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  747,                    // mPriority
  &daObjHata_METHODS,   // sub_method
  0x00040180,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
