/**
 * @file d_a_obj_lv4EdShutter.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4EdShutter.h"

class daLv4EdShutter_HIO_c : public mDoHIO_entry_c {
public:
    daLv4EdShutter_HIO_c();
    virtual ~daLv4EdShutter_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 close_max_speed;
    /* 0x08 */ f32 open_speed_max;
    /* 0x0C */ s16 field_0xc;
    /* 0x0E */ u8 field_0xe;
};

daLv4EdShutter_HIO_c::daLv4EdShutter_HIO_c() {
    close_max_speed = 80.0f;
    open_speed_max = 40.0f;
    field_0xc = 60;
    field_0xe = 4;
}

void daLv4EdShutter_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mMovePos, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, 0);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daLv4EdShutter_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("P_AShtr", 4);
    JUT_ASSERT(165, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daLv4EdShutter_c::create() {
    fopAcM_ct(this, daLv4EdShutter_c);

    int phase_state = dComIfG_resLoad(&mPhase, "P_AShtr");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("P_AShtr", 7, dBgS_MoveBGProc_TypicalRotY, 0x1900, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mOpenSw = getOpenSw();
        mCloseSw = getCloseSw();
        mMovePos = -300.0f;
        mStatus = 0;
        shape_angle.y += 0x4000;
        setBaseMtx();
        mZenmetuSw = getZenmetuSw();

        if (mZenmetuSw != 0xFF && fopAcM_isSwitch(this, mZenmetuSw)) {
            init_modeEnd();
        } else {
            init_modeWait();
        }
    }

    return phase_state;
}

int daLv4EdShutter_c::Execute(Mtx** param_0) {
    move();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

static daLv4EdShutter_HIO_c l_HIO;

void daLv4EdShutter_c::move() {
    static void (daLv4EdShutter_c::*mode_proc[])() = {
        &daLv4EdShutter_c::modeWait,
        &daLv4EdShutter_c::modeOpen,
        &daLv4EdShutter_c::modeClose,
        &daLv4EdShutter_c::modeEnd,
    };

    (this->*mode_proc[mMode])();
}

void daLv4EdShutter_c::init_modeWait() {
    mWaitTimer = getWaitTime();
    mMode = 0;
}

void daLv4EdShutter_c::modeWait() {
    if (mStatus == 0) {
        if (fopAcM_isSwitch(this, mCloseSw)) {
            if (mWaitTimer == 0) {
                if (getEventID() != 0xFF) {
                    orderEvent(getEventID(), 0xFF, 1);
                } else {
                    eventStart();
                }
            } else {
                mWaitTimer--;
            }
        }
    } else if (fopAcM_isSwitch(this, mOpenSw)) {
        if (mWaitTimer == 0) {
            if (getEventID() != 0xFF) {
                orderEvent(getEventID(), 0xFF, 1);
            } else {
                eventStart();
            }
        } else {
            mWaitTimer--;
        }
    }
}

void daLv4EdShutter_c::init_modeOpen() {
    mMoveSpeed = l_HIO.open_speed_max;

    cXyz se_pos(current.pos);
    se_pos.y = home.pos.y;
    mDoAud_seStart(Z2SE_OBJ_STL_FENCE_DW, &se_pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = 1;
}

void daLv4EdShutter_c::modeOpen() {
    f32 remaining = cLib_addCalc(&mMovePos, -300.0f, 0.3f, mMoveSpeed, 5.0f);
    if (remaining == 0.0f) {
        mStatus = 0;
        fopAcM_offSwitch(this, mOpenSw);
        fopAcM_offSwitch(this, mCloseSw);

        if (mZenmetuSw != 0xFF && fopAcM_isSwitch(this, mZenmetuSw)) {
            init_modeEnd();
        } else {
            init_modeWait();
        }
    }
}

void daLv4EdShutter_c::init_modeClose() {
    mMoveSpeed = l_HIO.close_max_speed;

    cXyz se_pos(current.pos);
    se_pos.y = home.pos.y;
    mDoAud_seStart(Z2SE_OBJ_STL_FENCE_UP, &se_pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = 2;
}

void daLv4EdShutter_c::modeClose() {
    f32 remaining = cLib_addCalc(&mMovePos, 0.0f, 0.3f, mMoveSpeed, 5.0f);
    if (remaining == 0.0f) {
        mStatus = 1;
        init_modeWait();
    }
}

void daLv4EdShutter_c::init_modeEnd() {
    mMode = 3;
}

void daLv4EdShutter_c::modeEnd() {}

bool daLv4EdShutter_c::eventStart() {
    if (mStatus == 0) {
        init_modeClose();
    } else {
        init_modeOpen();
    }

    return true;
}

int daLv4EdShutter_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daLv4EdShutter_c::Delete() {
    dComIfG_resDelete(&mPhase, "P_AShtr");
    return 1;
}

static int daLv4EdShutter_Draw(daLv4EdShutter_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv4EdShutter_Execute(daLv4EdShutter_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv4EdShutter_Delete(daLv4EdShutter_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daLv4EdShutter_Create(fopAc_ac_c* i_this) {
    return ((daLv4EdShutter_c*)i_this)->create();
}

static actor_method_class l_daLv4EdShutter_Method = {
    (process_method_func)daLv4EdShutter_Create,
    (process_method_func)daLv4EdShutter_Delete,
    (process_method_func)daLv4EdShutter_Execute,
    NULL,
    (process_method_func)daLv4EdShutter_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv4EdShutter = {
  fpcLy_CURRENT_e,          // mLayerID
  3,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_Obj_Lv4EdShutter,    // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daLv4EdShutter_c), // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  636,                      // mPriority
  &l_daLv4EdShutter_Method, // sub_method
  0x00040100,               // mStatus
  fopAc_ACTOR_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
