/**
 * @file d_a_obj_swpush2.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_swpush2.h"
#include "d/actor/d_a_player.h"

#if DEBUG
daObjSwpush2::Hio_c daObjSwpush2::Act_c::M_hio;
#endif

const char* daObjSwpush2::Act_c::getArcName() {
    return "K_swmn00";
}

const char* daObjSwpush2::Act_c::getBmdName() {
    return "K_hum_isw00.bmd";
}

const char* daObjSwpush2::Act_c::getDzbName() {
    return "K_hum_isw00.dzb";
}

const char* daObjSwpush2::Act_c::getNmlBtkName() {
    return "K_isw_nml.btk";
}

const char* daObjSwpush2::Act_c::getNmlBrkName() {
    return "K_isw_nml.brk";
}

const char* daObjSwpush2::Act_c::getOnBtkName() {
    return "K_isw_on.btk";
}

const char* daObjSwpush2::Act_c::getRunBpkName() {
    return "K_isw_run.bpk";
}

const char* daObjSwpush2::Act_c::getRunBrkName() {
    return "K_isw_run.brk";
}

const char* daObjSwpush2::Act_c::getRunBtkName() {
    return "K_isw_run.btk";
}

void daObjSwpush2::Act_c::prmZ_init() {
    if (!mPrmInit) {
        mPrmZ = home.angle.z;
        mPrmInit = true;
        home.angle.z = 0;
        current.angle.z = 0;
        shape_angle.z = 0;
    }
}

bool daObjSwpush2::Act_c::is_switch2() const {
    int swbit = prmZ_get_swSave2();
    if (swbit == 0xFF) {
        return 0;
    }

    return fopAcM_isSwitch(this, swbit);
}

int daObjSwpush2::Act_c::solidHeapCB(fopAc_ac_c* i_this) {
    return ((daObjSwpush2::Act_c*)i_this)->create_heap();
}

const char daObjSwpush2::Act_c::M_arcname[9] = "K_swmn00";

const daObjSwpush2::Attr_c daObjSwpush2::Act_c::M_attr[4] = {
    {
        0x2000,
        daObjSwpush2::AttrFlag_e(AttrFlag_UNK_0x1_e | AttrFlag_UNK_0x2_e),
        1.0f,
        M_arcname,
        0.90f,
        0.6f,
        0.0f,
        -0.3f,
        1,
        6,
        6,
        13,
        1.0f,
        2.5f,
        6,
        300.0f,
    },
    {
        0x2000,
        daObjSwpush2::AttrFlag_e(AttrFlag_UNK_0x20_e),
        1.0f,
        M_arcname,
        0.90f,
        0.6f,
        0.0f,
        -0.3f,
        1,
        6,
        6,
        13,
        1.0f,
        2.5f,
        6,
        300.0f,
    },
    {
        0x2000,
        daObjSwpush2::AttrFlag_e(AttrFlag_UNK_0x10_e | AttrFlag_UNK_0x1_e | AttrFlag_UNK_0x2_e),
        1.0f,
        M_arcname,
        0.90f,
        0.6f,
        0.0f,
        -0.3f,
        1,
        6,
        6,
        13,
        1.0f,
        2.5f,
        6,
        300.0f,
    },
    {
        0x8000,
        daObjSwpush2::AttrFlag_e(AttrFlag_HEAVY_e | AttrFlag_UNK_0x1_e | AttrFlag_UNK_0x2_e),
        1.5f,
        M_arcname,
        0.93f,
        0.42f,
        0.0f,
        0.0f,
        4,
        4,
        15,
        4,
        1.0f,
        2.5f,
        6,
        300.0f,
    },
};

bool daObjSwpush2::Act_c::create_heap() {
    void* model_data = dComIfG_getObjectRes(getArcName(), getBmdName());
    JUT_ASSERT(436, model_data != NULL);

    mpModel = mDoExt_J3DModel__create((J3DModelData*)model_data, 0x80000, 0x11000285);
    if (mpModel == NULL) {
        return 0;
    }

    {
        J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(getArcName(), getNmlBrkName());
        JUT_ASSERT(452, pbrk != NULL);
        mpNmlBrk = new mDoExt_brkAnm();
        if (mpNmlBrk == NULL || !mpNmlBrk->init((J3DModelData*)model_data, pbrk, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }

        mpNmlBrk->setPlaySpeed(1.0f);
    }

    {
        J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(getArcName(), getNmlBtkName());
        JUT_ASSERT(465, pbtk != NULL);

        mpNmlBtk = new mDoExt_btkAnm();
        if (mpNmlBtk == NULL || !mpNmlBtk->init((J3DModelData*)model_data, pbtk, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }

        mpNmlBtk->setPlaySpeed(1.0f);
    }

    {
        J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(getArcName(), getOnBtkName());
        JUT_ASSERT(478, pbtk != NULL);
        mpOnBtk = new mDoExt_btkAnm();
        if (mpOnBtk == NULL || !mpOnBtk->init((J3DModelData*)model_data, pbtk, 1, 0, 1.0f, 0, -1)) {
            return 0;
        }

        mpOnBtk->setPlaySpeed(0.0f);
    }

    {
        J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(getArcName(), getRunBtkName());
        JUT_ASSERT(491, pbtk != NULL);
        mpRunBtk = new mDoExt_btkAnm();
        if (mpRunBtk == NULL || !mpRunBtk->init((J3DModelData*)model_data, pbtk, 1, 0, 1.0f, 0, -1)) {
            return 0;
        }

        mpRunBtk->setPlaySpeed(0.0f);
    }

    {
        J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(getArcName(), getRunBrkName());
        JUT_ASSERT(504, pbrk != NULL);
        mpRunBrk = new mDoExt_brkAnm();
        if (mpRunBrk == NULL || !mpRunBrk->init((J3DModelData*)model_data, pbrk, 1, 0, 1.0f, 0, -1)) {
            return 0;
        }

        mpRunBrk->setPlaySpeed(0.0f);
    }

    {
        J3DAnmColor* pbpk = (J3DAnmColor*)dComIfG_getObjectRes(getArcName(), getRunBpkName());
        JUT_ASSERT(517, pbpk != NULL);
        mpRunBpk = new mDoExt_bpkAnm();
        if (mpRunBpk == NULL || !mpRunBpk->init((J3DModelData*)model_data, pbpk, 1, 0, 1.0f, 0, -1)) {
            return 0;
        }

        mpRunBpk->setPlaySpeed(0.0f);
    }

    mpBgw = dBgW_NewSet((cBgD_t*)dComIfG_getObjectRes(getArcName(), getDzbName()), 1, &mBgMtx);
    if (mpBgw == NULL) {
        return 0;
    }

    return 1;
}

bool daObjSwpush2::Act_c::create_init() {
    bool rt = true;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
    mpBgw->SetRideCallback(rideCB);
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    init_mtx();

    fopAcM_setCullSizeBox(this, -150.0f * attr().size, 0.0f, -150.0f * attr().size, 150.0f * attr().size, 60.0f, 150.0f * attr().size);

    mShockTimer = 0;
    mRideType = 0;
    mPrevRideType = 0;
    mRideTimer = 0;
    mPushFlag = false;
    mIsPlayerRideHeavy = false;
    mPrevPlayerRideHeavy = false;
    mHeavyRideTimer = 0;
    mHeavyPushFlag = false;

    u8 temp_r26 = cLib_checkBit<AttrFlag_e>(attr().flags, AttrFlag_REV_SW_e) != 0;
    u8 temp_r28 = cLib_checkBit<AttrFlag_e>(attr().flags, AttrFlag_UNK_0x10_e) != 0;
    u8 temp_r25 = cLib_checkBit<AttrFlag_e>(attr().flags, AttrFlag_UNK_0x20_e) != 0;

    bool is_sw = is_switch();
    bool is_sw2 = is_switch2();
    if ((temp_r26 || (!is_sw && !temp_r28) || (is_sw && temp_r28)) && (!temp_r25 || !is_sw2)) {
        field_0x5e8 = 1.0f;
        mode_upper_init();
    } else {
        field_0x5e8 = -1.0f;
        mode_lower_init();
    }

    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, prm_get_evId());
    if (dComIfG_Bgsp().Regist(mpBgw, this)) {
        rt = false;
    }

    return rt;
}

int daObjSwpush2::Act_c::create_res_load() {
    int phase_state;
    if (attr().arcname != NULL) {
        phase_state = dComIfG_resLoad(&mPhase, attr().arcname);
    } else {
        phase_state = cPhs_COMPLEATE_e;
    }

    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    return cPhs_COMPLEATE_e;
}

int daObjSwpush2::Act_c::Mthd_Create() {
    fopAcM_ct(this, daObjSwpush2::Act_c);
    
    prmZ_init();
    mType = prm_get_type();
    
    int phase_state = create_res_load();
    if (phase_state == cPhs_COMPLEATE_e) {
        scale.x *= attr().size;
        scale.z *= attr().size;

        if (!fopAcM_entrySolidHeap(this, solidHeapCB, attr().heap_size)) {
            return cPhs_ERROR_e;
        }

        if (!create_init()) {
            return cPhs_ERROR_e;
        }

        demo_non_init();

        #if DEBUG
        M_hio.ct();
        #endif
    }

    return phase_state;
}

int daObjSwpush2::Act_c::Mthd_Delete() {
    if (mpBgw != NULL && mpBgw->ChkUsed()) {
        if (dComIfG_Bgsp().Release(mpBgw)) {
            OS_REPORT_ERROR("Release Error(%s %d)\n", __FILE__, 683);
        }
    }

    dComIfG_resDelete(&mPhase, attr().arcname);

    #if DEBUG
    M_hio.dt();
    #endif
    return 1;
}

void daObjSwpush2::Act_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos.x, mTopPos + (-10.0f + current.pos.y), current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::scaleM(scale);
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

void daObjSwpush2::Act_c::init_mtx() {
    mpModel->setBaseScale(scale);
    set_mtx();
}

void daObjSwpush2::Act_c::rideCB(dBgW* i_bgw, fopAc_ac_c* i_this, fopAc_ac_c* i_rideActor) {
    UNUSED(i_bgw);
    daObjSwpush2::Act_c* a_this = (daObjSwpush2::Act_c*)i_this;

    if (fopAcM_CheckStatus(i_rideActor, 0x400)) {
        if (fopAcM_CheckCarryType(i_rideActor, fopAcM_CARRY_TYPE_1)) {
            a_this->mRideType = 2;
        } else {
            a_this->mRideType = 1;
        }

        bool is_player_ride = fopAcM_GetProfName(i_rideActor) == PROC_ALINK;
        if (is_player_ride) {
            a_this->mShockTimer = 4;
        }

        if (is_player_ride && ((daPy_py_c*)i_rideActor)->checkEquipHeavyBoots()) {
            a_this->mIsPlayerRideHeavy = true;
        }
    }
}

void daObjSwpush2::Act_c::calc_top_pos() {
    cLib_addCalc(&field_0x5e8, field_0x5ec, 0.05f, 0.02f, 0.01f);
    mTopPos = -50.0f * (1.0f - field_0x5e8);
}

void daObjSwpush2::Act_c::set_push_flag() {
    if (mShockTimer != 0) {
        mShockTimer--;
    }

    if (mPushFlag) {
        if (mRideType != 0) {
            if (mRideType == 2) {
                mRideTimer = attr().field_0x26;
            } else {
                mRideTimer = attr().ride_time;
            }
        } else {
            if (--mRideTimer <= 0) {
                mPushFlag = false;
            }
        }
    } else if (mRideType != 0) {
        if (++mRideTimer >= attr().ride_time) {
            mPushFlag = true;
        }
    } else {
        mRideTimer = 0;
    }

    if (mHeavyPushFlag) {
        if (mIsPlayerRideHeavy) {
            mHeavyRideTimer = attr().heavy_ride_time;
        } else if (--mHeavyRideTimer <= 0) {
            mHeavyPushFlag = false;
        }
    } else if (mIsPlayerRideHeavy) {
        if (++mHeavyRideTimer >= attr().heavy_ride_time) {
            mHeavyPushFlag = true;
        }
    } else {
        mHeavyRideTimer = 0;
    }
}

void daObjSwpush2::Act_c::mode_upper_init() {
    mModeProc = 0;
    field_0x5df = 0;
    mPauseDemoTimer = 0;
    field_0x5ec = 1.0f;
}

void daObjSwpush2::Act_c::mode_upper() {
    bool do_pause_demo = false;
    bool do_sw_demo = false;
    bool is_push_flag = false;
    f32 var_f31 = 1.0f;

    if (mPushFlag) {
        if (cLib_checkBit<AttrFlag_e>(attr().flags, AttrFlag_HEAVY_e)) {
            if (mHeavyPushFlag) {
                is_push_flag = true;
            }
        } else {
            is_push_flag = true;
        }
    }

    if (mRideType != 0 && mPrevRideType == 0 && mPauseDemoTimer <= 0) {
        mPauseDemoTimer = 30;
        do_pause_demo = true;
    } else if (mPauseDemoTimer > 0) {
        mPauseDemoTimer--;
    }

    bool is_sw = is_switch();
    bool temp_r26 = cLib_checkBit<AttrFlag_e>(attr().flags, AttrFlag_UNK_0x10_e);
    if (is_push_flag || (cLib_checkBit<AttrFlag_e>(attr().flags, AttrFlag_UNK_0x1_e) && ((is_sw && !temp_r26) || (!is_sw && temp_r26)))) {
        if (is_push_flag) {
            field_0x5df = 1;
        }
        do_sw_demo = true;
        mode_u_l_init();
    }

    if (do_sw_demo) {
        demo_reqSw_init();
    } else if (do_pause_demo) {
        demo_reqPause_init();
    }
}

void daObjSwpush2::Act_c::mode_u_l_init() {
    mModeProc = 1;
    field_0x5f0 = 12;
}

void daObjSwpush2::Act_c::mode_u_l() {
    if (field_0x5f0 != 0) {
        field_0x5f0--;
        if (field_0x5f0 == 0) {
            mpRunBpk->setPlaySpeed(1.0f);
            mpRunBtk->setPlaySpeed(1.0f);
            mpRunBrk->setPlaySpeed(1.0f);
            field_0x5ec = -1.0f;
        }
    }

    if (field_0x5e8 <= -1.0f) {
        if (field_0x5df != 0) {
            if (cLib_checkBit<AttrFlag_e>(attr().flags, AttrFlag_REV_SW_e)) {
                rev_switch();
            } else if (cLib_checkBit<AttrFlag_e>(attr().flags, AttrFlag_UNK_0x10_e)) {
                off_switch();
            } else {
                on_switch();
            }

            if (mShockTimer != 0) {
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            }
        }

        fopAcM_seStart(this, JA_SE_OBJ_FOOT_SW_ON, 0);
        mode_lower_init();
    }
}

void daObjSwpush2::Act_c::mode_lower_init() {
    mModeProc = 2;
    field_0x5df = 0;
    field_0x5ec = -1.0f;
}

void daObjSwpush2::Act_c::mode_lower() {}

void daObjSwpush2::Act_c::mode_l_u() {
    if (field_0x5e8 >= 1.0f) {
        if (field_0x5df != 0) {
            if (cLib_checkBit<AttrFlag_e>(attr().flags, AttrFlag_UNK_0x10_e)) {
                on_switch();
            } else {
                off_switch();
            }
        }
        mode_upper_init();
    }
}

void daObjSwpush2::Act_c::demo_non_init() {
    mDemoProc = 0;
}

void daObjSwpush2::Act_c::demo_non() {}

void daObjSwpush2::Act_c::demo_reqPause_init() {
    if (mDemoProc == 0) {
        mDemoProc = 1;
        fopAcM_orderPotentialEvent(this, 2, 0, 0);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daObjSwpush2::Act_c::demo_reqPause() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        demo_runPause_init(TRUE);
    } else {
        demo_non_init();
    }
}

void daObjSwpush2::Act_c::demo_runPause_init(BOOL i_setRunTime) {
    mDemoProc = 2;
    if (i_setRunTime) {
        mDemoRunTime = attr().demo_run_time;
    }
}

void daObjSwpush2::Act_c::demo_runPause() {
    mDemoRunTime--;
    if (mDemoRunTime <= 0) {
        dComIfGp_event_reset();
        demo_non_init();
    }
}

void daObjSwpush2::Act_c::demo_stop_puase() {
    if (mDemoProc == 1) {
        demo_reqPause();
    }

    if (mDemoProc == 2) {
        dComIfGp_event_reset();
    }
}

void daObjSwpush2::Act_c::demo_reqSw_init() {
    if (dComIfGp_evmng_existence(mEventIdx)) {
        if (mDemoProc == 0 || mDemoProc == 1 || mDemoProc == 2) {
            demo_stop_puase();
            mDemoProc = 3;
            fopAcM_orderOtherEventId(this, mEventIdx, prm_get_evId(), 0xFFFF, 0, 1);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        }
    } else {
        mDemoRunTime = 100;
        demo_runPause_init(FALSE);
    }
}

void daObjSwpush2::Act_c::demo_reqSw() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        demo_runSw_init();
    } else {
        fopAcM_orderOtherEventId(this, mEventIdx, prm_get_evId(), 0xFFFF, 0, 1);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daObjSwpush2::Act_c::demo_runSw_init() {
    mDemoProc = 4;
}

void daObjSwpush2::Act_c::demo_runSw() {
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        dComIfGp_event_reset();
        demo_non_init();
    }
}

int daObjSwpush2::Act_c::Mthd_Execute() {
    static void (daObjSwpush2::Act_c::*demo_proc[])() = {
        &daObjSwpush2::Act_c::demo_non,
        &daObjSwpush2::Act_c::demo_reqPause,
        &daObjSwpush2::Act_c::demo_runPause,
        &daObjSwpush2::Act_c::demo_reqSw,
        &daObjSwpush2::Act_c::demo_runSw,
    };

    (this->*demo_proc[mDemoProc])();
    set_push_flag();

    static void (daObjSwpush2::Act_c::*mode_proc[])() = {
        &daObjSwpush2::Act_c::mode_upper,
        &daObjSwpush2::Act_c::mode_u_l,
        &daObjSwpush2::Act_c::mode_lower,
        &daObjSwpush2::Act_c::mode_l_u,
    };

    (this->*mode_proc[mModeProc])();
    calc_top_pos();

    mpNmlBrk->play();
    mpNmlBtk->play();
    mpOnBtk->play();
    mpRunBpk->play();
    mpRunBrk->play();
    mpRunBtk->play();

    if (!is_switch()) {
        if (daPy_getPlayerActorClass()->current.pos.abs(current.pos) < attr().effect_distance) {
            mpOnBtk->setPlaySpeed(1.0f);
        } else {
            mpOnBtk->setPlaySpeed(-1.0f);
        }
    }

    mPrevRideType = mRideType != 0;
    mPrevPlayerRideHeavy = mIsPlayerRideHeavy;
    mRideType = 0;
    mIsPlayerRideHeavy = false;

    set_mtx();
    mpBgw->Move();

    eyePos.x = current.pos.x;
    eyePos.y = (current.pos.y + mTopPos) - -10.0f;
    eyePos.z = current.pos.z;
    return 1;
}

int daObjSwpush2::Act_c::Mthd_Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    mpNmlBrk->entry(mpModel->getModelData());
    mpNmlBtk->entry(mpModel->getModelData());

    if (mpOnBtk->isStop() && mpOnBtk->getFrame() != 0.0f) {
        mpRunBtk->entry(mpModel->getModelData());
        mpRunBpk->entry(mpModel->getModelData());
        mpRunBrk->entry(mpModel->getModelData());
    } else {
        mpOnBtk->entry(mpModel->getModelData());
    }

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    mpNmlBrk->remove(mpModel->getModelData());
    mpNmlBtk->remove(mpModel->getModelData());

    if (mpOnBtk->isStop() && mpOnBtk->getFrame() != 0.0f) {
        mpRunBtk->remove(mpModel->getModelData());
        mpRunBpk->remove(mpModel->getModelData());
        mpRunBrk->remove(mpModel->getModelData());
    } else {
        mpOnBtk->remove(mpModel->getModelData());
    }

    return 1;
}

namespace daObjSwpush2 {
namespace {
int Mthd_Create(void* i_this) {
    return ((Act_c*)i_this)->Mthd_Create();
}

int Mthd_Delete(void* i_this) {
    return ((Act_c*)i_this)->Mthd_Delete();
}

int Mthd_Execute(void* i_this) {
    return ((Act_c*)i_this)->Mthd_Execute();
}

int Mthd_Draw(void* i_this) {
    return ((Act_c*)i_this)->Mthd_Draw();
}

actor_method_class Mthd_Table = {
    (process_method_func)Mthd_Create, 
    (process_method_func)Mthd_Delete, 
    (process_method_func)Mthd_Execute, 
    NULL,
    (process_method_func)Mthd_Draw,
};
}
}

actor_process_profile_definition g_profile_Obj_Swpush2 = {
  fpcLy_CURRENT_e,        // mLayerID
  2,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Swpush2,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjSwpush2::Act_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  12,                     // mPriority
  &daObjSwpush2::Mthd_Table,         // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
