/**
 * @file d_a_obj_hfuta.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_hfuta.h"
#include "d/d_com_inf_game.h"
#include "Z2AudioLib/Z2Instances.h"

void daObjFuta_c::initBaseMtx() {
    mModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjFuta_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::transM(mTransM.x, mTransM.y, mTransM.z);
    mDoMtx_stack_c::transM(0.0f, 87.5f, 0.0f);
    mDoMtx_stack_c::ZXYrotM(mZXYRotationM.x, mZXYRotationM.y, mZXYRotationM.z);
    mDoMtx_stack_c::transM(0.0f, -87.5f, 0.0f);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), (MtxP)&mBgMtx);
}

int daObjFuta_c::Create() {
        /* dSv_event_flag_c::F_0070 - Ordon Woods - Colin went deep into the woods */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[168])) {
        setMode(MODE_2);
        mode_end();
    } else {
        setMode(MODE_0);
        mode_wait();
    }
    initBaseMtx();
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mModel->getModelData());
    return 1;
}

static char* l_arcName = "Obj_hfuta";

int daObjFuta_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(204, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mModel != NULL ? 1 : 0;
}

int daObjFuta_c::create1st() {
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        rv = MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0xb00, NULL);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
    }
    return rv;
}

int daObjFuta_c::Execute(f32 (**mtx)[3][4]) {
    mode_proc_call();
    static const csXyz l_rot_data[43] = {
        csXyz(-11, 0, 899),
        csXyz(602, 0, 641),
        csXyz(858, 0, 42),
        csXyz(632, 0, -552),
        csXyz(70, 0, -816),
        csXyz(-503, 0, -621),
        csXyz(-774, 0, -95),
        csXyz(-607, 0, 456),
        csXyz(-117, 0, 730),
        csXyz(410, 0, 591),
        csXyz(686, 0, 136),
        csXyz(572, 0, -367),
        csXyz(152, 0, -642),
        csXyz(-325, 0, -550),
        csXyz(-750, 0, -150),
        csXyz(-450, 0, 150),
        csXyz(-150, 0, 450),
        csXyz(150, 0, 750),
        csXyz(450, 0, 450),
        csXyz(750, 0, 150),
        csXyz(450, 0, -150),
        csXyz(150, 0, -450),
        csXyz(-150, 0, -750),
        csXyz(-450, 0, -450),
        csXyz(-750, 0, -150),
        csXyz(-450, 0, 150),
        csXyz(-150, 0, 450),
        csXyz(150, 0, 750),
        csXyz(450, 0, 450),
        csXyz(750, 0, 150),
        csXyz(450, 0, -150),
        csXyz(150, 0, -450),
        csXyz(-150, 0, -750),
        csXyz(-450, 0, -450),
        csXyz(-700, 0, -150),
        csXyz(-200, 0, 350),
        csXyz(300, 0, 700),
        csXyz(700, 0, 200),
        csXyz(200, 0, -300),
        csXyz(-300, 0, -700),
        csXyz(-700, 0, -200),
        csXyz(-200, 0, 300),
        csXyz(300, 0, 700),
    };
    if (field_0x5c8 != 0) {
        if (mRotationIndex < 43) {
            mZXYRotationM.x = l_rot_data[mRotationIndex].x;
            mZXYRotationM.z = l_rot_data[mRotationIndex].z;
        } else {
            mZXYRotationM.z = 0;
            mZXYRotationM.x = 0;
        }
        mRotationIndex++;
    }
    *mtx = &mBgMtx;
    setBaseMtx();
    field_0x5c0++;
    return 1;
}

void daObjFuta_c::mode_proc_call() {
    static const modeFunc l_func[] = {
        &daObjFuta_c::mode_wait,
        &daObjFuta_c::mode_move,
        &daObjFuta_c::mode_end,
    };

    (this->*(l_func[mMode]))();
}

void daObjFuta_c::mode_wait() {
    mTransM = cXyz::Zero;
    mZXYRotationM = csXyz::Zero;
    if (field_0x5bf != 0) {
        field_0x5c4 = 900.0f;
        setMode(MODE_1);
        field_0x5c8 = 1;
        mRotationIndex = 0;
    }
}

void daObjFuta_c::mode_move() {
    if (mRotationIndex > 12) {
        if (cLib_addCalc(&mTransM.x, 120.0f, 0.1f, 5.0f, 2.0f) == 0.0f)
        {
            setMode(MODE_2);
        }
    }
}

void daObjFuta_c::mode_end() {
    mTransM.x = 120.0f;
}

int daObjFuta_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return 1;
}

int daObjFuta_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjFuta_create1st(daObjFuta_c* i_this) {
    fopAcM_ct(i_this, daObjFuta_c);
    return i_this->create1st();
}

static int daObjFuta_MoveBGDelete(daObjFuta_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjFuta_MoveBGExecute(daObjFuta_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjFuta_MoveBGDraw(daObjFuta_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjFuta_METHODS = {
    (process_method_func)daObjFuta_create1st,
    (process_method_func)daObjFuta_MoveBGDelete,
    (process_method_func)daObjFuta_MoveBGExecute,
    NULL,
    (process_method_func)daObjFuta_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Hfuta = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Hfuta,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjFuta_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  450,                    // mPriority
  &daObjFuta_METHODS,     // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
