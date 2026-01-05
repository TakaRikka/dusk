/**
 * @file d_a_obj_poTbox.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_poTbox.h"

class daPoTbox_HIO_c : public mDoHIO_entry_c {
public:
    daPoTbox_HIO_c();
    virtual ~daPoTbox_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ f32 open_speed;
    /* 0x8 */ f32 close_speed;
};

static char* l_type[2] = {
    "pouBox0",
    "pouBox1",
};

static int l_bmdIdx[] = {
    10, 8,
};

static int l_dzbIdx[] = {
    21, 11,
};

daPoTbox_HIO_c::daPoTbox_HIO_c() {
    open_speed = 2.0f;
    close_speed = 12.0f;
}

void daPoTbox_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpBoxModel->setBaseScale(scale);
    mpBoxModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (mpAnimModel != NULL) {
        mpAnimModel->setBaseScale(scale);
        mpAnimModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    if (mpLightModel != NULL) {
        mpLightModel->setBaseScale(scale);
        mpLightModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daPoTbox_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_type[mStat], l_bmdIdx[mStat]);
    JUT_ASSERT(185, modelData != NULL);
    int res;

    mpBoxModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpBoxModel == NULL) {
        return 0;
    }

    if (mStat == 0) {
        J3DModelData* modelData1 = (J3DModelData*)dComIfG_getObjectRes(l_type[mStat], 0xB);
        JUT_ASSERT(200, modelData1 != NULL);
        mpAnimModel = mDoExt_J3DModel__create(modelData1, 0x80000, 0x11000084);
        if (mpAnimModel == NULL) {
            return 0;
        }

        res = mBck.init((J3DAnmTransform*)dComIfG_getObjectRes(l_type[mStat], 7), 1, 0, 1.0f, 0, -1, 0);
        JUT_ASSERT(212, res == TRUE);
    
        J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes(l_type[mStat], 0xC);
        JUT_ASSERT(218, modelData2 != NULL);

        mpLightModel = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11000284);
        if (mpLightModel == NULL) {
            return 0;
        }

        res = mBtk.init(modelData2, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_type[mStat], 0x12), 1, 2, 1.0f, 0, -1);
        JUT_ASSERT(235, res == TRUE);

        res = mBrk.init(modelData2, (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_type[mStat], 0xF), 1, 0, 1.0f, 0, -1);
        JUT_ASSERT(244, res == TRUE);

        mpBgw2 = new dBgW();
        if (mpBgw2 == NULL || mpBgw2->Set((cBgD_t*)dComIfG_getObjectRes(l_type[mStat], 0x16), 1, &mBgMtx)) {
            mpBgw2 = NULL;
            return 0;
        }
    } else {
        mpBgw2 = NULL;
        mpAnimModel = NULL;
        mpLightModel = NULL;
        res = mBck.init((J3DAnmTransform*)dComIfG_getObjectRes(l_type[mStat], 5), 1, 0, 1.0f, 0, -1, 0);
        JUT_ASSERT(268, res == TRUE);
    }

    return 1;
}

int daPoTbox_c::create() {
    fopAcM_ct(this, daPoTbox_c);

    mStat = getStat();
    
    int phase_state = dComIfG_resLoad(&mPhase, l_type[mStat]);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate(l_type[mStat], l_dzbIdx[mStat], dBgS_MoveBGProc_Typical, 0x6EA0, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpBoxModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpBoxModel->getModelData());
        field_0x605 = 0;

        if (mStat == 0) {
            mIsSwitch = fopAcM_isSwitch(this, getSw());
            if (mIsSwitch) {
                fopAcM_SetMtx(this, mpAnimModel->getBaseTRMtx());
                fopAcM_setCullSizeBox2(this, mpAnimModel->getModelData());

                field_0x605 = 1;
                mBck.setFrame(mBck.getEndFrame() - 1.0f);
                mBrk.setFrame(mBrk.getEndFrame() - 1.0f);

                if (mpBgW != NULL) {
                    dComIfG_Bgsp().Release(mpBgW);
                }

                if (mpBgw2 != NULL) {
                    dComIfG_Bgsp().Regist(mpBgw2, this);
                    mpBgw2->Move();
                }

                init_modeOpenEnd();
            } else {
                init_modeWait();
            }
        } else {
            init_modeEnd();
        }

        setBaseMtx();
    }

    return phase_state;
}

static daPoTbox_HIO_c l_HIO;

int daPoTbox_c::Execute(Mtx** param_0) {
    static void (daPoTbox_c::*mode_proc[])() = {
        &daPoTbox_c::modeWait,
        &daPoTbox_c::modeOpen,
        &daPoTbox_c::modeOpenEnd,
        &daPoTbox_c::modeEnd,
    };

    (this->*mode_proc[mMode])();

    if (field_0x605 == 0) {
        *param_0 = &mpBoxModel->getBaseTRMtx();
    } else {
        *param_0 = &mpAnimModel->getBaseTRMtx();
    }

    setBaseMtx();
    return 1;
}

void daPoTbox_c::init_modeWait() {
    mMode = 0;
}

void daPoTbox_c::modeWait() {
    mIsSwitch = fopAcM_isSwitch(this, getSw());
    if (mIsSwitch) {
        init_modeOpen();
    }
}

void daPoTbox_c::init_modeOpen() {
    if (mpBgW != NULL) {
        dComIfG_Bgsp().Release(mpBgW);
    }

    if (mpBgw2 != NULL) {
        dComIfG_Bgsp().Regist(mpBgw2, this);
        mpBgw2->Move();
    }

    fopAcM_SetMtx(this, mpAnimModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpAnimModel->getModelData());
    mDoAud_seStart(Z2SE_OBJ_PO_TBOX, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    field_0x605 = 1;
    mMode = 1;
}

void daPoTbox_c::modeOpen() {
    int bck_play = mBck.play();
    int brk_play = mBrk.play();

    if (bck_play == TRUE && brk_play == TRUE) {
        init_modeOpenEnd();
    }
}

void daPoTbox_c::init_modeOpenEnd() {
    mMode = 2;
}

void daPoTbox_c::modeOpenEnd() {
    mBtk.play();
}

void daPoTbox_c::init_modeEnd() {
    mMode = 3;
}

void daPoTbox_c::modeEnd() {}

int daPoTbox_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpBoxModel, &tevStr);

    if (mpAnimModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpAnimModel, &tevStr);
    }

    if (mpLightModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpLightModel, &tevStr);
    }

    if (mStat == 0) {
        if (field_0x605 == 1) {
            mBck.entry(mpAnimModel->getModelData());

            J3DModelData* modelData = mpLightModel->getModelData();
            mBtk.entry(modelData);
            mBrk.entry(modelData);
        }
    } else {
        mBck.entry(mpBoxModel->getModelData());
    }

    dComIfGd_setListBG();

    if (field_0x605 == 0) {
        mDoExt_modelUpdateDL(mpBoxModel);
    } else {
        if (mpAnimModel != NULL) {
            mDoExt_modelUpdateDL(mpAnimModel);
        }

        if (mpLightModel != NULL) {
            mDoExt_modelUpdateDL(mpLightModel);
        }
    }

    dComIfGd_setList();
    return 1;
}

int daPoTbox_c::Delete() {
    dComIfG_resDelete(&mPhase, l_type[mStat]);

    if (mpBgw2 != NULL && mpBgw2->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgw2);
    }

    return 1;
}

static int daPoTbox_Draw(daPoTbox_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daPoTbox_Execute(daPoTbox_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daPoTbox_Delete(daPoTbox_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daPoTbox_Create(fopAc_ac_c* i_this) {
    return ((daPoTbox_c*)i_this)->create();
}

static actor_method_class l_daPoTbox_Method = {
    (process_method_func)daPoTbox_Create,
    (process_method_func)daPoTbox_Delete,
    (process_method_func)daPoTbox_Execute,
    0,
    (process_method_func)daPoTbox_Draw,
};

actor_process_profile_definition g_profile_Obj_poTbox = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_poTbox,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daPoTbox_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  660,                    // mPriority
  &l_daPoTbox_Method,     // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
