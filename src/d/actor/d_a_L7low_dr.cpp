/**
 * @file d_a_L7low_dr.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_L7low_dr.h"
#include "d/d_com_inf_game.h"

void daL7lowDr_c::setAction(daL7lowDr_c::actionFn i_action) {
    if (mAction != NULL) {
        field_0x63c = -1;
        (this->*mAction)();
    }

    field_0x630 = mAction;
    mAction = i_action;
    field_0x63c = 0;
    (this->*mAction)();
}

void daL7lowDr_c::action() {
    if (field_0x640 != 0) {
        field_0x640--;
    }

    (this->*mAction)();

    shape_angle = current.angle;
    fopAcM_posMoveF(this, NULL);
    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    mtx_set();
    eyePos = current.pos;
}

int daL7lowDr_c::_delete() {
    dComIfG_resDelete(&mPhase, "L7lowDr");

    if (heap != NULL) {
        mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int daL7lowDr_Delete(daL7lowDr_c* i_this) {
    return i_this->_delete();
}

void daL7lowDr_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    J3DModel* model = mpModelMorf->getModel();
    model->setBaseScale(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    mpModelMorf->modelCalc();
}

void daL7lowDr_c::fly() {
    if (field_0x63c == 0) {
        s16 var_r28 = cM_rndFX(32768.0f);
        current.pos.set(cM_ssin(var_r28) * 2800.0f, 8000.0f, cM_scos(var_r28) * 2800.0f);
        current.pos += home.pos;
        current.angle.y = cLib_targetAngleY(&current.pos, &home.pos) + 0x4000;

        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("L7lowDr", 4), 2, 20.0f, 1.0f, 0.0f, -1.0f);
        speedF = 30.0f;
        field_0x640 = cM_rndFX(60.0f) + 210.0f;
        field_0x63c++;
    } else if (field_0x63c != -1) {
        current.angle.y = cLib_targetAngleY(&current.pos, &home.pos) + 0x4000;
        fopAcM_posMoveF(this, NULL);

        if (field_0x63c == 1) {
            cLib_chaseAngleS(&current.angle.z, 0, 0x20);

            if (mpModelMorf->isStop()) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("L7lowDr", 4), 2, 3.0f, 1.0f, 0.0f, -1.0f);
            }

            if (field_0x640 == 0 && mpModelMorf->getFrame() >= 33.0f) {
                if ((int)cM_rndF(2.0f) != 0) {
                    field_0x640 = cM_rndFX(60.0f) + 210.0f;
                } else {
                    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("L7lowDr", 5), 0, 3.0f, 1.0f, 0.0f, -1.0f);
                    field_0x640 = cM_rndFX(60.0f) + 180.0f;
                    field_0x63c++;
                }
            }
        } else {
            cLib_chaseAngleS(&current.angle.z, 0x1000, 0x20);

            if (mpModelMorf->isStop()) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("L7lowDr", 6), 2, 3.0f, 1.0f, 0.0f, -1.0f);
            }

            if (field_0x640 == 0 && mpModelMorf->getFrame() >= 67.0f) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("L7lowDr", 7), 0, 3.0f, 1.0f, 0.0f, -1.0f);
                field_0x640 = cM_rndFX(60.0f) + 210.0f;
                field_0x63c = 1;
            }
        }
    }
}

int daL7lowDr_c::execute() {
    action();
    return 1;
}

static int daL7lowDr_Execute(daL7lowDr_c* i_this) {
    return i_this->execute();
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daL7lowDr_c* a_this = (daL7lowDr_c*)i_this;
    return a_this->CreateHeap();
}

int daL7lowDr_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L7lowDr", 10);
    JUT_ASSERT(244, modelData != NULL);
    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("L7lowDr", 4), 0, 1.0f, 0, -1, &mSound, 0x80000, 0x11000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpModelMorf->getModel();
    model->setUserArea((uintptr_t)this);
    return 1;
}

int daL7lowDr_c::create() {
    fopAcM_ct(this, daL7lowDr_c);

    int phase_state = dComIfG_resLoad(&mPhase, "L7lowDr");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("////////////// L7lowDr ///////////////////\n");
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1D20)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////// L7lowDr SET OK //////////////\n");

        if (dComIfGs_isStageBossEnemy()) {
            OS_REPORT("L7俯瞰翼竜：ボス死んでるのでセットしない\n");
            return cPhs_ERROR_e;
        }

        scale.setall(1.0f);
        attention_info.flags = 0;

        mSound.init(&current.pos, &eyePos, 3, 1);

        setAction(&daL7lowDr_c::fly);
        daL7lowDr_Execute(this);
    }

    return phase_state;
}

static int daL7lowDr_Create(daL7lowDr_c* i_this) {
    return i_this->create();
}

int daL7lowDr_c::draw() {
         /* dSv_event_flag_c::F_0510 - City in the Sky - Watched City in the Sky start cutscene */
    if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[510])) {
        return 1;
    }

    J3DModel* model = mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    mpModelMorf->entryDL();
    return 1;
}

static int daL7lowDr_Draw(daL7lowDr_c* i_this) {
    return i_this->draw();
}

static int daL7lowDr_IsDelete(daL7lowDr_c* i_this) {
    return 1;
}

static actor_method_class l_daL7lowDr_Method = {
    (process_method_func)daL7lowDr_Create,
    (process_method_func)daL7lowDr_Delete,
    (process_method_func)daL7lowDr_Execute,
    (process_method_func)daL7lowDr_IsDelete,
    (process_method_func)daL7lowDr_Draw,
};

extern actor_process_profile_definition g_profile_L7lowDr = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_L7lowDr,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daL7lowDr_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  207,                    // mPriority
  &l_daL7lowDr_Method,    // sub_method
  0x00040000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
