/**
 * @file d_a_obj_amiShutter.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_amiShutter.h"
#include "d/actor/d_a_player.h"
#include "c/c_damagereaction.h"
#include "d/d_bg_w.h"

struct daAmiShutter_HIO_c : public mDoHIO_entry_c {
    /* 0x04 */ f32 mRange;
    /* 0x08 */ f32 mMaxOpenSpeed;
    /* 0x0c */ f32 mMaxCloseSpeed;
    /* 0x10 */ u8 mWaitTime;
    /* 0x11 */ u8 field_0x11;

    daAmiShutter_HIO_c();
    ~daAmiShutter_HIO_c() {}

    void genMessage(JORMContext*);
};

daAmiShutter_HIO_c::daAmiShutter_HIO_c() {
    mMaxOpenSpeed = 70.0f;
    mMaxCloseSpeed = 70.0f;
    field_0x11 = 3;
    mRange = 400.f;
    mWaitTime = 0;
}

#if DEBUG
void daAmiShutter_HIO_c::genMessage(JORMContext* i_ctx) {
    i_ctx->genSlider("open速度", &mMaxOpenSpeed, 0.1f, 1000.0f, 
        0, NULL, 0xffff, 0xffff, 512, 24);
    i_ctx->genSlider("close速度", &mMaxCloseSpeed, 0.1f, 1000.0f, 
        0, NULL, 0xffff, 0xffff, 512, 24);
    i_ctx->genSlider("range", &mRange, 0.1f, 1000.0f, 
        0, NULL, 0xffff, 0xffff, 512, 24);
    i_ctx->genSlider("waitTime", &mWaitTime, 0, 255, 
        0, NULL, 0xffff, 0xffff, 512, 24);
}
#endif

static daAmiShutter_HIO_c l_HIO;

void daAmiShutter_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}


int daAmiShutter_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes("S_Zami", 4);
    JUT_ASSERT(167, modelData != NULL); 
    
    mpModel = mDoExt_J3DModel__create(modelData, 
        J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    return 1;
}


cPhs__Step daAmiShutter_c::create() {
    fopAcM_ct(this, daAmiShutter_c);
    cPhs__Step phaseStep = (cPhs__Step) dComIfG_resLoad(&mPhaseReq, "S_Zami");

    if (phaseStep == cPhs_COMPLEATE_e) {
        cPhs__Step res = (cPhs__Step) MoveBGCreate("S_Zami", 7, 
            dBgS_MoveBGProc_Typical, 0x1a00, NULL);

        if (res == cPhs_ERROR_e) {
            phaseStep = cPhs_ERROR_e;
            return phaseStep;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mType = getType();
        mPosZ = current.pos.z;
        
        if (mType == 0 || mType == 1) {
            if (mType == 0 && cDmr_SkipInfo != 0 
                && daPy_getPlayerActorClass()->current.pos.y > -800.0f){
                mMode = MODE_CLOSE_END;
            } else {
                mPos = current.pos;
                current.pos.z -= 600.0f;
                mOpen = true;
                init_modeWaitEvent();
            }
        } else {
            mSwBit = getSwBit();
            mSwitch = fopAcM_isSwitch(this, mSwBit);
            mOpen = false;
            init_modeWait();
        }
        setBaseMtx();
        #if DEBUG
        l_HIO.entryHIO("アミシャッター");
        #endif
    }
    return phaseStep;
}

int daAmiShutter_c::Execute(Mtx** i_mtx) {
    eventUpdate();
    moveShutter();
    *i_mtx = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}


void daAmiShutter_c::moveShutter() {
    static daAmiShutter_c::modeProc const mode_proc[6] = {
        &daAmiShutter_c::modeWait,
        &daAmiShutter_c::modeWaitEvent,
        &daAmiShutter_c::modeClose,
        &daAmiShutter_c::modeCloseEvent,
        &daAmiShutter_c::modeCloseEnd,
        &daAmiShutter_c::modeOpen,
    };

    (this->*mode_proc[mMode])();
}


BOOL daAmiShutter_c::playerAreaCheck() {
    BOOL inArea = FALSE;
    if (mType == 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz posDiff = mPos - player->current.pos;
        f32 distance = posDiff.absXZ();
        if (distance >= l_HIO.mRange 
            && player->current.pos.y > current.pos.y
            && player->current.pos.y < current.pos.y + 50.f){
            inArea = TRUE;
        }
    } else if (dComIfGp_getLinkPlayer()->getFootOnGround()) {
        inArea = TRUE;
    }
    return inArea;
}

void daAmiShutter_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daAmiShutter_c::modeWait() {
    u8 isSwitch = fopAcM_isSwitch(this, mSwBit);
    if (mSwitch != isSwitch) {
        mSwitch = isSwitch;
        mSwitch == FALSE ? init_modeClose() : init_modeOpen();
    }
}

void daAmiShutter_c::init_modeWaitEvent() {
    mMode = MODE_WAIT_EVENT;
}

void daAmiShutter_c::modeWaitEvent() {
    if (playerAreaCheck() == TRUE) {
        if (getEvent() != 0xff) {
            if (mType == 0 && fopAcM_isSwitch(this, 4) == TRUE) {
                eventStart();
            } else {
                orderEvent(getEvent(), 0xff, 1);
            }
        } else {
            eventStart();
        }
    }
}

bool daAmiShutter_c::eventStart() {
    init_modeCloseEvent();
    return true;
}


void daAmiShutter_c::init_modeClose() {
    mDoAud_seStart(Z2SE_OBJ_NET_SHTR_CL, &current.pos, 0, 
        dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpBgW->OffRoofRegist();
    mMode = MODE_CLOSE;
}

void daAmiShutter_c::modeClose() {
    f32 distance = cLib_addCalc(&current.pos.z, mPosZ, 0.5f, 
        l_HIO.mMaxCloseSpeed, 1.0f);
    if (distance == 0.0f) {
        mpBgW->OnRoofRegist();
        mOpen = false;
        init_modeWait();
    }
}

void daAmiShutter_c::init_modeCloseEvent() {
    mDoAud_seStart(Z2SE_OBJ_NET_L_SHTR_CL, &current.pos, 0, 
        dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mWaitTime = l_HIO.mWaitTime;
    mMode = MODE_CLOSE_EVENT;
}


void daAmiShutter_c::modeCloseEvent() {
    if (mWaitTime != 0) {
        mWaitTime -= 1;
        return;
    }

    f32 distance = cLib_addCalc(&current.pos.z, mPosZ, 0.2, 
        l_HIO.mMaxOpenSpeed, 1.0f);
    if (distance == 0.0f) {
        mOpen = false;
        init_modeCloseEnd();
    }
}

void daAmiShutter_c::init_modeCloseEnd() {
    if (mType == 0 && fopAcM_isSwitch(this, 4) == 0) {
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_TURN_BACK_e,
             0, 0,0);
    }
    mMode = MODE_CLOSE_END;
}

void daAmiShutter_c::modeCloseEnd() {
    // EMPTY METHOD
}

void daAmiShutter_c::init_modeOpen() {
    mDoAud_seStart(Z2SE_OBJ_NET_SHTR_OP, &current.pos, 0, 
        dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = MODE_OPEN;
}

void daAmiShutter_c::modeOpen() {
    f32 distance = cLib_addCalc(&current.pos.z, mPosZ - 600.0f, 0.2, 
        l_HIO.mMaxOpenSpeed, 1.0f);
    if (distance == 0.0f) {
        mOpen = true;
        init_modeWait();
    }
}

int daAmiShutter_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daAmiShutter_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, "S_Zami");
    #if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

int daAmiShutter_Draw(daAmiShutter_c* i_this) {
    return i_this->MoveBGDraw();
}

int daAmiShutter_Execute(daAmiShutter_c* i_this) {
    return i_this->MoveBGExecute();
}

int daAmiShutter_Delete(daAmiShutter_c* i_this) {
    fopAcM_GetID(i_this);
    return i_this->MoveBGDelete();
}

int daAmiShutter_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daAmiShutter_c*>(i_this)->create();
}

actor_method_class l_daAmiShutter_Method = {
    (process_method_func)daAmiShutter_Create,
    (process_method_func)daAmiShutter_Delete,
    (process_method_func)daAmiShutter_Execute,
    NULL,
    (process_method_func)daAmiShutter_Draw,
};


actor_process_profile_definition g_profile_Obj_AmiShutter = {
    fpcLy_CURRENT_e,        // mLayerID
    3,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_AmiShutter,    // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daAmiShutter_c), // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    521,                    // mPriority
    &l_daAmiShutter_Method, // sub_method
    0x00044000,             // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
  };
