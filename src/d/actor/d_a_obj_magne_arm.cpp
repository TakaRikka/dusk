/**
* @file d_a_obj_magne_arm.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_magne_arm.h"
#include "d/actor/d_a_obj_mhole.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "Z2AudioLib/Z2Instances.h"


static u8 const l_magne_scale[12] = {
    0x40, 0x40, 0x00, 0x00, 0x40, 0x09, 0x99, 0x9A, 0x40, 0x40, 0x00, 0x00,
};

static f32 const l_trans[4] = {0.0f, 1050.0f, 450.0f, 900.0f};

static f32 const l_down_length[4] = {1100.0f, 800.0f, 1100.0f, 800.0f};

static s16 const l_roll_speed[4] = {200, 200, 200, 1000};

static u8 const l_stop_timer[4] = {60, 60, 60, 60};

static s16 const l_rot_speedY[4] = {200, 200, 200, 200};

void daObjMarm_c::getBpartsOffset(cXyz* i_BPartOffset) {
    static Vec const s_offsetB = {-150.0f, 1200.0f, 0.0f};
    *i_BPartOffset = s_offsetB;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(i_BPartOffset, i_BPartOffset);
}

void daObjMarm_c::getDpartsOffset(cXyz* i_DPartOffset) {
    static Vec const s_offsetD = {0.0f, 2500.0f, 0.0f};
    *i_DPartOffset = s_offsetD;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mCPartsYRot + mYRotOffset);
    mDoMtx_stack_c::multVec(i_DPartOffset, i_DPartOffset);
}

void daObjMarm_c::getEpartsOffset(cXyz* i_EPartOffset) {
    static Vec const s_offsetE = {0.0f, 2500.0f, 1460.0f};
    *i_EPartOffset = s_offsetE;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mCPartsYRot + mYRotOffset);
    mDoMtx_stack_c::multVec(i_EPartOffset, i_EPartOffset);
}

void daObjMarm_c::getFpartsOffset(cXyz* i_FPartOffset) {
    static Vec const s_offsetF = {0.0f, 2500.0f, 1780.0f};
    *i_FPartOffset = s_offsetF;
    i_FPartOffset->y -= l_down_length[mMoveType];
    i_FPartOffset->y += mLiftTotal;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mCPartsYRot + mYRotOffset);
    mDoMtx_stack_c::ZrotM(mZRotOffset * 2.0f);
    mDoMtx_stack_c::multVec(i_FPartOffset, i_FPartOffset);
}

void daObjMarm_c::getRopeStartPos(cXyz* i_ropeStartPos) {
    getEpartsOffset(i_ropeStartPos);
    cXyz rope_start_pos(0.0f, 0.0f, 320.0f);
    mDoMtx_stack_c::YrotS(mCPartsYRot + mYRotOffset);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&rope_start_pos, &rope_start_pos);
    *i_ropeStartPos += rope_start_pos;
}

void daObjMarm_c::initBaseMtx() {
    mpModel[0]->setBaseScale(scale);
    mpModel[1]->setBaseScale(scale);
    mpModel[2]->setBaseScale(scale);
    mpModel[3]->setBaseScale(scale);
    mpModel[4]->setBaseScale(scale);
    mpModel[5]->setBaseScale(scale);
    setBaseMtx();
}

void daObjMarm_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

    cXyz b_parts_offset;
    getBpartsOffset(&b_parts_offset);
    mDoMtx_stack_c::transS(b_parts_offset);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::XrotM(mBPartsXRot);
    mpModel[1]->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mCPartsYRot + mYRotOffset);
    mpModel[2]->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx1);

    cXyz d_parts_offset;
    getDpartsOffset(&d_parts_offset);
    mDoMtx_stack_c::transS(d_parts_offset);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mCPartsYRot + mYRotOffset);
    mDoMtx_stack_c::XrotM(mDPartsXRot);
    mpModel[3]->setBaseTRMtx(mDoMtx_stack_c::get());

    cXyz e_parts_offset;
    getEpartsOffset(&e_parts_offset);
    mDoMtx_stack_c::transS(e_parts_offset);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mCPartsYRot + mYRotOffset);
    mDoMtx_stack_c::XrotM(mEPartsXRot);
    mpModel[4]->setBaseTRMtx(mDoMtx_stack_c::get());

    cXyz f_parts_offset;
    getFpartsOffset(&f_parts_offset);
    mDoMtx_stack_c::transS(f_parts_offset);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mCPartsYRot + mYRotOffset);
    mDoMtx_stack_c::ZrotM(mFPartsZRot);
    mpModel[5]->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx3);
    
    mDoMtx_stack_c::multVecZero(&attention_info.position);
    eyePos = attention_info.position;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mCPartsYRot + mYRotOffset);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx2);
}

void rideCallBack(dBgW* param_0, fopAc_ac_c* i_this, fopAc_ac_c* i_rideActor) {
    if (fopAcM_GetName(i_rideActor) == PROC_ALINK) {
        static_cast<daObjMarm_c*>(i_this)->mPlayerRide = TRUE;
    }
}

static char* l_arcName = "D_Marm";

static void dummyStrings() {
    DEAD_STRING("D_MN54");
}

static cull_box l_cull_box = {{-500.0f, -3000.0f, -500.0f}, {500.0f, 3000.0f, 2500.0f}};

static cull_box l_cull_box2 = {{-500.0f, -4000.0f, -500.0f}, {500.0f, 3000.0f, 2500.0f}};

static Vec const l_offsetB = {-150.0f, 1200.0f, 0.0f};  // unused

static Vec const l_offsetD = {0.0f, 2500.0f, 0.0f};     // unused

static Vec const l_offsetE = {0.0f, 2500.0f, 1460.0f};  // unused

static Vec const l_offsetF = {0.0f, 1000.0f, 1760.0f};  // unused

int daObjMarm_c::Create() {
    calcHimo();
    f32* size = mpRope1->getSize(0);
    for (int i = 0; i < field_0xA28; size++) {
        *size = 25.5f;
        i++;
    }

    size = mpRope2->getSize(0);
    for (int i = 0; i < field_0xA29; size++) {
        *size = 25.5f;
        i++;
    }

    mLiftTotal = -l_trans[mMoveType];
    if (mMoveType == MOVETYPE_D_e || mMoveType == MOVETYPE_B_e) {
        mLiftTotal = 0;
    }

    if (mMoveType == MOVETYPE_B_e || mMoveType == MOVETYPE_D_e) {
        mYRotDirection = -1;
        mIsYRotForward = 1;
    } else {
        mYRotDirection = 1;
        mIsYRotForward = 0;
    }

    if (dComIfG_Bgsp().Regist((dBgW_Base*)mpBgW1, this) != NULL ||
        dComIfG_Bgsp().Regist((dBgW_Base*)mpBgW2, this) != NULL)
    {
        return FALSE;
    }

    mpBgW1->SetCrrFunc(dBgS_MoveBGProc_Typical);
    mpBgW2->SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
    fopAcM_SetMtx(this, mBgMtx2);
    mAcchCir.SetWall(30.0f, 30.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), 0, 0);

    if (mMoveType == MOVETYPE_D_e) {
        fopAcM_setCullSizeBox(this, l_cull_box2.min.x, l_cull_box2.min.y, l_cull_box2.min.z,
                              l_cull_box2.max.x, l_cull_box2.max.y, l_cull_box2.max.z);
    } else {
        fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z,
                              l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);
    }

    fopAcM_setCullSizeFar(this, 10.0f);
    mpBgW1->Move();
    mpBgW2->Move();

    switch (mMoveType) {
    case 0:
        init_typeA_modeWait();
        break;
    case 1:
        mBPartsXRot -= 0x4000;
        init_typeB_modeWait();
        break;
    case 2:
        init_typeC_modeWait();
        break;
    case 3:
        init_typeD_modeWait();
        break;
    }

    mpBgW2->SetRideCallback(rideCallBack);
    MoveBGExecute();
    initBaseMtx();

    return TRUE;
}

static f32 l_att_disXZ = 3000.0f;

static f32 l_att_high = 2000.0f;

static f32 l_att_low = -500.0f;

static s16 l_att_ang = 0x6000;

int daObjMarm_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 7);
    mpModel[0] = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel[0] == NULL) {
        return FALSE;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 8);
    mpModel[1] = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel[1] == NULL) {
        return FALSE;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 9);
    mpModel[2] = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel[2] == NULL) {
        return FALSE;
    }

    mpBgW1 = new dBgW();
    if (mpBgW1 == NULL || mpBgW1->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 22),
                                                cBgW::MOVE_BG_e, &mBgMtx1))
    {
        mpBgW1 = NULL;
        return FALSE;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 10);
    mpModel[3] = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel[3] == NULL) {
        return FALSE;
    }
    model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 11);
    mpModel[4] = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel[4] == NULL) {
        return FALSE;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 12);
    mpModel[5] = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel[5] == NULL) {
        return FALSE;
    }

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 15);
    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL ||
        !mpBrkAnm->init(model_data, pbrk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1))
    {
        return FALSE;
    }
    mpBrkAnm->setPlaySpeed(1.0f);

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 18);
    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL ||
        !mpBtkAnm->init(model_data, pbtk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1))
    {
        return FALSE;
    }
    mpBtkAnm->setPlaySpeed(1.0f);

    mpBgW2 = new dBgW();
    if (mpBgW2 == NULL || mpBgW2->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 23),
                                                cBgW::MOVE_BG_e, &mBgMtx3))
    {
        mpBgW2 = NULL;
        return FALSE;
    }

    field_0xA28 = 2;
    field_0xA29 = 5;

    mpRope1 = new mDoExt_3DlineMat1_c();
    mpRope2 = new mDoExt_3DlineMat1_c();
    if (mpRope1 == NULL || mpRope2 == NULL) {
        return FALSE;
    }

    if (!mpRope1->init(1, field_0xA28, (ResTIMG*)dComIfG_getObjectRes(l_arcName, 26), 1)) {
        return FALSE;
    }

    if (!mpRope2->init(1, field_0xA29, (ResTIMG*)dComIfG_getObjectRes(l_arcName, 26), 1)) {
        return FALSE;
    }

    return TRUE;
}

cPhs__Step daObjMarm_c::phase_0() {
    mMoveType = getMoveType();
    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName, 21, dBgS_MoveBGProc_Typical, 0x6000, NULL);
        if (phase == cPhs_ERROR_e) {
            return (cPhs__Step)phase;
        }
        mPhaseIndex++;
    }
    return cPhs_INIT_e;
}

cPhs__Step daObjMarm_c::phase_1() {
    cXyz scale(1.0f, 1.0f, 1.0f);
    mID = fopAcM_create(PROC_Obj_MHole, 0x12FF, &current.pos, fopAcM_GetRoomNo(this),
                        &current.angle, &scale, -1);

    if (mID == fpcM_ERROR_PROCESS_ID_e) {
        return cPhs_ERROR_e;
    }
    mPhaseIndex++;
    return cPhs_INIT_e;
}

cPhs__Step daObjMarm_c::phase_2() {
    daObjMHole_c* m_hole_actor = (daObjMHole_c*)fopAcM_SearchByID(mID);
    if (m_hole_actor != NULL) {
        csXyz angle(0, 0, 0);
        m_hole_actor->home.angle = angle;
        m_hole_actor->current.angle = angle;
        m_hole_actor->shape_angle = angle;
        if (fopAcM_isSwitch(this, getSwNo())) {
            m_hole_actor->setOn();
        }
        fopAcM_setCullSizeFar(m_hole_actor, 10.0f);
        mPhaseIndex++;
        return cPhs_COMPLEATE_e;
    }
    return cPhs_INIT_e;
}

int daObjMarm_c::create1st() {
    typedef cPhs__Step (daObjMarm_c::*daObjMarm_func)();
    static daObjMarm_func l_ct_func[] = {
        &daObjMarm_c::phase_0,
        &daObjMarm_c::phase_1,
        &daObjMarm_c::phase_2,
    };
    return (this->*l_ct_func[mPhaseIndex])();
}

int daObjMarm_c::Execute(Mtx** i_bgMtx) {
    mRotOffsetSwing++;
    action();

    mCPartsYRot = -mBPartsXRot;
    mEPartsXRot = -mDPartsXRot;
    shape_angle.y = mCPartsYRot;
    calcHimo();

    *i_bgMtx = &mBgMtx;
    setBaseMtx();

    mpBgW1->Move();
    mpBgW2->Move();
    mpBrkAnm->play();
    mpBtkAnm->play();

    daObjMHole_c* m_hole_actor = (daObjMHole_c*)fopAcM_SearchByID(mID);
    if (m_hole_actor != NULL) {
        cXyz f_parts_offset;
        getFpartsOffset(&f_parts_offset);

        cXyz offset(0.0f, -700.0f, 0.0f);
        mDoMtx_stack_c::YrotS(current.angle.y + mCPartsYRot + mYRotOffset);
        mDoMtx_stack_c::ZrotM(mFPartsZRot);
        mDoMtx_stack_c::multVec(&offset, &offset);
        f_parts_offset += offset;
        m_hole_actor->home.pos = f_parts_offset;
        m_hole_actor->current.pos = f_parts_offset;
        m_hole_actor->shape_angle.y = current.angle.y + mCPartsYRot + mYRotOffset;
        m_hole_actor->shape_angle.z = mFPartsZRot;
    }
    mYRotOffset = mRotOffsetForce * cM_ssin(mRotOffsetSwing * 8000);
    mZRotOffset = mRotOffsetForce * cM_ssin(mRotOffsetSwing * 4000);
    cLib_addCalc0(&mRotOffsetForce, 0.1f, 20.0f);

    mZRotSwing++;
    mFPartsZRot = mZRotForce * cM_ssin(mZRotSwing * 3000);
    cLib_addCalc0(&mZRotForce, 0.05f, 10.0f);

    mDoMtx_stack_c::copy((mpModel[1]->getBaseTRMtx()));
    mDoMtx_stack_c::multVecZero(&mSeMarmPos);
    mDoMtx_stack_c::copy((mpModel[3]->getBaseTRMtx()));
    mDoMtx_stack_c::multVecZero(&mSeMarmLiftPos);
    mDoMtx_stack_c::copy((mpModel[5]->getBaseTRMtx()));
    mDoMtx_stack_c::multVecZero(&mSeMarmSwingPos);

    mSeMarmSwingPos.y += -700.0f;
    mPlayerRide = FALSE;
    dComIfGp_att_LookRequest(this, l_att_disXZ, l_att_high, l_att_low, l_att_ang, 1);

    return 1;
}

void daObjMarm_c::action() {
    typedef void (daObjMarm_c::*daObjMarm_func)();

    static daObjMarm_func l_func_typeA[] = {
        &daObjMarm_c::typeA_modeWait,
        &daObjMarm_c::typeA_modeMholeOn,
        &daObjMarm_c::typeA_modeLiftUp, 
        &daObjMarm_c::typeA_modeLiftDown,
        &daObjMarm_c::typeA_modeRotate, 
        &daObjMarm_c::typeA_modeEnd,
    };

    static daObjMarm_func l_func_typeB[] = {
        &daObjMarm_c::typeB_modeWait,   
        &daObjMarm_c::typeB_modeMholeOn,
        &daObjMarm_c::typeB_modeLiftUp, 
        &daObjMarm_c::typeB_modeLiftDown,
        &daObjMarm_c::typeB_modeRotate, 
        &daObjMarm_c::typeB_modeEnd,
    };
    static daObjMarm_func l_func_typeC[] = {
        &daObjMarm_c::typeC_modeWait,   
        &daObjMarm_c::typeC_modeMholeOn,
        &daObjMarm_c::typeC_modeLiftUp, 
        &daObjMarm_c::typeC_modeLiftDown,
        &daObjMarm_c::typeC_modeRotate, 
        &daObjMarm_c::typeC_modeEnd,
    };
    static daObjMarm_func l_func_typeD[] = {
        &daObjMarm_c::typeD_modeWait,   
        &daObjMarm_c::typeD_modeMholeOn,
        &daObjMarm_c::typeD_modeLiftUp, 
        &daObjMarm_c::typeD_modeLiftDown,
        &daObjMarm_c::typeD_modeRotate, 
        &daObjMarm_c::typeD_modeEnd,
    };

    switch (mMoveType) {
    case MOVETYPE_A_e:
        (this->*l_func_typeA[mMode])();
        break;

    case MOVETYPE_B_e:
        (this->*l_func_typeB[mMode])();
        break;

    case MOVETYPE_C_e:
        (this->*l_func_typeC[mMode])();
        break;

    case MOVETYPE_D_e:
        (this->*l_func_typeD[mMode])();
        break;
    }
}

void daObjMarm_c::init_typeA_modeWait() {
    mStopTimer = 0;
    mMode = MODE_WAIT_e;
}

void daObjMarm_c::typeA_modeWait() {
    if (fopAcM_isSwitch(this, getSwNo())) {
        init_typeA_modeMholeOn();
    }
}

void daObjMarm_c::init_typeA_modeMholeOn() {
    daObjMHole_c* m_hole_actor = (daObjMHole_c*)fopAcM_SearchByID(mID);
    if (m_hole_actor != NULL) {
        m_hole_actor->setOn();
    }

    setMagneHoleEffect();
    mStopTimer = 0;
    mMode = MODE_MHOLE_ON_e;
}

void daObjMarm_c::typeA_modeMholeOn() {
    init_typeA_modeRotate();
}

void daObjMarm_c::typeA_modeLiftUp() {
    /* empty function */
}

void daObjMarm_c::typeA_modeLiftDown() {
    /* empty function */
}

void daObjMarm_c::init_typeA_modeRotate() {
    mStopTimer = l_stop_timer[0];
    if (mPlayerRide != FALSE) {
        mStopTimer *= 1.5f;
    }

    mRotationAngle = 0;
    mRotationTotal = 0;
    mMode = MODE_ROTATE_e;
}

void daObjMarm_c::typeA_modeRotate() {
    u32 stop_timer = (u8)mStopTimer;
    if (cLib_calcTimer(&mStopTimer) == 0) {
        endMagneHoleEffect();
        if (stop_timer != 0) {
            seStart_MOVESTART();
        }

        s16 rot_speedY = l_rot_speedY[0];
        if (mYRotDirection == -1) {
            rot_speedY = -l_rot_speedY[0];
        }

        cLib_addCalcAngleS(&mRotationAngle, rot_speedY, 20, 50, 10);
        mBPartsXRot -= mRotationAngle;
        mRotationTotal += abs(mRotationAngle);
        if (mRotationTotal > 0x4000) {
            setMagneHoleEffect();
            mIsYRotForward += mYRotDirection;
            if (mIsYRotForward == 0) {
                mYRotDirection = 1;
            } else if (mIsYRotForward == 2) {
                mYRotDirection = -1;
            }
            mRotOffsetForce = 150.0f;
            mZRotForce = 800.0f;
            mZRotSwing = 0;

            init_typeA_modeRotate();
            seStart_SWING();
            seStart_STOP();
        }

        seStartLevel_MOVE();
    }
}

void daObjMarm_c::typeA_modeEnd() {
    /* empty function */
}

void daObjMarm_c::init_typeB_modeWait() {
    mStopTimer = 0;
    mMode = MODE_WAIT_e;
}

void daObjMarm_c::typeB_modeWait() {
    if (fopAcM_isSwitch(this, getSwNo())) {
        init_typeB_modeMholeOn();
    }
}

void daObjMarm_c::init_typeB_modeMholeOn() {
    daObjMHole_c* m_hole_actor = (daObjMHole_c*)fopAcM_SearchByID(mID);
    if (m_hole_actor != NULL) {
        m_hole_actor->setOn();
    }

    setMagneHoleEffect();
    mStopTimer = 0;
    mMode = MODE_MHOLE_ON_e;
}

void daObjMarm_c::typeB_modeMholeOn() {
    init_typeB_modeRotate();
}

void daObjMarm_c::init_typeB_modeLiftUp() {
    mStopTimer = 30;
    mLiftRotation = 0;
    mMode = MODE_LIFT_UP_e;
}

void daObjMarm_c::typeB_modeLiftUp() {
    if (cLib_calcTimer(&mStopTimer) == 0) {
        cLib_addCalcAngleS(&mLiftRotation, l_roll_speed[1], 20, 50, 10);
        mDPartsXRot += mLiftRotation;
        mLiftTotal += mLiftRotation * 1884.9557f / 65536.0f;
        if (mLiftTotal > 0.0f) {
            mLiftTotal = 0.0f;
            init_typeB_modeRotate();
        }
        seStartLevel_UP();
    }
}

void daObjMarm_c::typeB_modeLiftDown() {
    if (cLib_calcTimer(&mStopTimer) == 0) {
        cLib_addCalcAngleS(&mLiftRotation, l_roll_speed[1], 20, 50, 10);
        mDPartsXRot -= mLiftRotation;
        mLiftTotal -= mLiftRotation * 1884.9557f / 65536.0f;
        if (mLiftTotal < -l_trans[mMoveType]) {
            mLiftTotal = -l_trans[mMoveType];
            init_typeB_modeLiftUp();
        }
        seStartLevel_DOWN();
    }
}

void daObjMarm_c::init_typeB_modeRotate() {
    mStopTimer = l_stop_timer[1];
    if (mPlayerRide != FALSE) {
        mStopTimer *= 1.5f;
    }

    mRotationAngle = 0;
    mRotationTotal = 0;
    mMode = MODE_ROTATE_e;
}

void daObjMarm_c::typeB_modeRotate() {
    u32 stop_timer = mStopTimer;
    if (cLib_calcTimer(&mStopTimer) == 0) {
        if (stop_timer != 0) {
            seStart_MOVESTART();
        }

        endMagneHoleEffect();
        s16 rot_speedY = l_rot_speedY[1];
        if (mYRotDirection == -1) {
            rot_speedY = -l_rot_speedY[1];
        }

        cLib_addCalcAngleS(&mRotationAngle, rot_speedY, 20, 50, 10);
        mBPartsXRot -= mRotationAngle;
        mRotationTotal += abs(mRotationAngle);
        if (mRotationTotal > 0x4000) {
            setMagneHoleEffect();
            mRotOffsetForce = 150.0f;
            mZRotForce = 800.0f;
            mZRotSwing = 0;
            mIsYRotForward += mYRotDirection;
            
            if (mIsYRotForward == 0) {
                mYRotDirection = 1;
                init_typeB_modeRotate();
            } else if (mIsYRotForward == 1) {
                mYRotDirection = -1;
                init_typeB_modeRotate();
            }

            seStart_SWING();
            seStart_STOP();
        }

        seStartLevel_MOVE();
    }
}

void daObjMarm_c::typeB_modeEnd() {
    /* empty function */
}

void daObjMarm_c::init_typeC_modeWait() {
    mStopTimer = 0;
    mMode = MODE_WAIT_e;
}

void daObjMarm_c::typeC_modeWait() {
    if (fopAcM_isSwitch(this, getSwNo())) {
        init_typeC_modeMholeOn();
    }
}

void daObjMarm_c::init_typeC_modeMholeOn() {
    daObjMHole_c* m_hole_actor = (daObjMHole_c*)fopAcM_SearchByID(mID);
    if (m_hole_actor != NULL) {
        m_hole_actor->setOn();
    }

    setMagneHoleEffect();
    mStopTimer = 0;
    mMode = MODE_MHOLE_ON_e;
}

void daObjMarm_c::typeC_modeMholeOn() {
    init_typeC_modeLiftUp();
}

void daObjMarm_c::init_typeC_modeLiftUp() {
    mStopTimer = 30;
    mLiftRotation = 0;
    mMode = MODE_LIFT_UP_e;
}

void daObjMarm_c::typeC_modeLiftUp() {
    if (cLib_calcTimer(&mStopTimer) == 0) {
        cLib_addCalcAngleS(&mLiftRotation, l_roll_speed[2], 20, 50, 10);
        mDPartsXRot += mLiftRotation;
        mLiftTotal += mLiftRotation * 1884.9557f / 65536.0f;
        if (mLiftTotal > 0.0f) {
            mLiftTotal = 0.0f;
            init_typeC_modeRotate();
        }
        seStartLevel_UP();
    }
}

void daObjMarm_c::init_typeC_modeLiftDown() {
    mStopTimer = 30;
    mLiftRotation = 0;
    mMode = MODE_LIFT_DOWN_e;
}

void daObjMarm_c::typeC_modeLiftDown() {
    if (cLib_calcTimer(&mStopTimer) == 0) {
        cLib_addCalcAngleS(&mLiftRotation, l_roll_speed[2], 20, 50, 10);
        mDPartsXRot -= mLiftRotation;
        mLiftTotal -= mLiftRotation * 1884.9557f / 65536.0f;
        if (mLiftTotal < -l_trans[mMoveType]) {
            mLiftTotal = -l_trans[mMoveType];
            init_typeC_modeLiftUp();
        }
        seStartLevel_DOWN();
    }
}

void daObjMarm_c::init_typeC_modeRotate() {
    mStopTimer = l_stop_timer[2];
    if (mPlayerRide != FALSE) {
        mStopTimer *= 1.5f;
    }

    mRotationAngle = 0;
    mRotationTotal = 0;
    mMode = MODE_ROTATE_e;
}

void daObjMarm_c::typeC_modeRotate() {
    u32 stop_timer = mStopTimer;
    if (cLib_calcTimer(&mStopTimer) == 0) {
        endMagneHoleEffect();
        if (stop_timer != 0) {
            seStart_MOVESTART();
        }

        s16 rot_speedY = l_rot_speedY[2];
        if (mYRotDirection == -1) {
            rot_speedY = -l_rot_speedY[2];
        }

        cLib_addCalcAngleS(&mRotationAngle, rot_speedY, 20, 50, 10);
        mBPartsXRot -= mRotationAngle;
        mRotationTotal += abs(mRotationAngle);
        if (mRotationTotal > 0x4000) {
            setMagneHoleEffect();
            mRotOffsetForce = 150.0f;
            mZRotForce = 800.0f;
            mZRotSwing = 0;
            mIsYRotForward += mYRotDirection;

            if (mIsYRotForward == 0) {
                mYRotDirection = 1;
                init_typeC_modeLiftDown();
            } else if (mIsYRotForward == 1) {
                mYRotDirection = -1;
                init_typeB_modeRotate();
            }

            seStart_SWING();
            seStart_STOP();
        }

        seStartLevel_MOVE();
    }
}

void daObjMarm_c::typeC_modeEnd() {
    /* empty function */
}

void daObjMarm_c::init_typeD_modeWait() {
    mStopTimer = 0;
    mMode = MODE_WAIT_e;
}

void daObjMarm_c::typeD_modeWait() {
    if (fopAcM_isSwitch(this, getSwNo())) {
        init_typeD_modeMholeOn();
    }
}

void daObjMarm_c::init_typeD_modeMholeOn() {
    daObjMHole_c* m_hole_actor = (daObjMHole_c*)fopAcM_SearchByID(mID);
    if (m_hole_actor != NULL) {
        m_hole_actor->setOn();
    }

    setMagneHoleEffect();
    mStopTimer = 0;
    mMode = MODE_MHOLE_ON_e;
}

void daObjMarm_c::typeD_modeMholeOn() {
    init_typeD_modeRotate();
}

void daObjMarm_c::init_typeD_modeLiftUp() {
    mStopTimer = 30;
    mLiftRotation = 0;
    mMode = MODE_LIFT_UP_e;
}

void daObjMarm_c::typeD_modeLiftUp() {
    if (cLib_calcTimer(&mStopTimer) == 0) {
        cLib_addCalcAngleS(&mLiftRotation, l_roll_speed[3], 20, 50, 10);
        mDPartsXRot += mLiftRotation;
        mLiftTotal += mLiftRotation * 1884.9557f / 65536.0f;
        if (mLiftTotal > 0.0f) {
            mLiftTotal = 0.0f;
            init_typeD_modeRotate();
        }
        seStartLevel_UP();
    }
}

void daObjMarm_c::init_typeD_modeLiftDown() {
    mStopTimer = 30;
    mLiftRotation = 0;
    mMode = MODE_LIFT_DOWN_e;
}

void daObjMarm_c::typeD_modeLiftDown() {
    if (cLib_calcTimer(&mStopTimer) == 0) {
        cLib_addCalcAngleS(&mLiftRotation, l_roll_speed[3], 20, 50, 10);
        mDPartsXRot -= mLiftRotation;
        mLiftTotal -= mLiftRotation * 1884.9557f / 65536.0f;
        if (mLiftTotal < -l_trans[mMoveType]) {
            mLiftTotal = -l_trans[mMoveType];
            init_typeD_modeLiftUp();
        }

        seStartLevel_DOWN();
    }
}

void daObjMarm_c::init_typeD_modeRotate() {
    mStopTimer = l_stop_timer[3];
    if (mPlayerRide != FALSE) {
        mStopTimer *= 1.5f;
    }

    mRotationAngle = 0;
    mRotationTotal = 0;
    mMode = MODE_ROTATE_e;
}

void daObjMarm_c::typeD_modeRotate() {
    u32 stop_timer = mStopTimer;
    if (cLib_calcTimer(&mStopTimer) == 0) {
        endMagneHoleEffect();
        if (stop_timer != 0) {
            seStart_MOVESTART();
        }

        s16 rot_speedY = l_rot_speedY[3];
        if (mYRotDirection == -1) {
            rot_speedY = -l_rot_speedY[3];
        }

        cLib_addCalcAngleS(&mRotationAngle, rot_speedY, 20, 50, 10);
        mBPartsXRot -= mRotationAngle;
        mRotationTotal += abs(mRotationAngle);
        if (mRotationTotal > 0x4000) {
            setMagneHoleEffect();
            mRotOffsetForce = 150.0f;
            mZRotForce = 800.0f;
            mZRotSwing = 0;
            mIsYRotForward += mYRotDirection;

            if (mIsYRotForward == 0) {
                mYRotDirection = 1;
                init_typeD_modeLiftDown();
            } else if (mIsYRotForward == 1) {
                mYRotDirection = -1;
                init_typeD_modeRotate();
            }

            seStart_SWING();
            seStart_STOP();
        }

        seStartLevel_MOVE();
    }
}

void daObjMarm_c::typeD_modeEnd() {
    /* empty function */
}

void daObjMarm_c::setMagneHoleEffect() {
    /* empty function */
}

void daObjMarm_c::endMagneHoleEffect() {
    if (mpEmitter != NULL) {
        mpEmitter->becomeInvalidEmitter();
        mpEmitter->quitImmortalEmitter();
        mpEmitter = NULL;
    }
}

void daObjMarm_c::calcHimo() {
    cXyz offset1;
    cXyz offset2;

    getRopeStartPos(&offset1);
    getFpartsOffset(&offset2);
    fabs(offset1.y - offset2.y);

    cXyz* line_mat1_pos = mpRope1->getPos(0);
    *line_mat1_pos = offset1;
    line_mat1_pos++;
    for (int i = 1; i < field_0xA28; line_mat1_pos++) {
        *line_mat1_pos = offset2;
        i++;
    }
    getEpartsOffset(&offset1);

    cXyz line_offsets[5];
    line_offsets[4].set(0.0f, 0.0f, 320.0f);
    line_offsets[3].set(0.0f, 200.0f, 250.0f);
    line_offsets[2].set(0.0f, 330.0f, 0.0f);
    line_offsets[1].set(0.0f, 230.0f, -200.0f);
    line_offsets[0].set(0.0f, 50.0f, -800.0f);

    mDoMtx_stack_c::YrotS(mCPartsYRot + mYRotOffset);
    mDoMtx_stack_c::YrotM(current.angle.y);

    for (int i = 0; i < field_0xA29; i++) {
        mDoMtx_stack_c::multVec(&line_offsets[i], &line_offsets[i]);
        line_offsets[i] += offset1;
    }

    line_mat1_pos = mpRope2->getPos(0);
    for (int i = 0; i < field_0xA29; line_mat1_pos++) {
        *line_mat1_pos = line_offsets[i];
        i++;
    }
}

void daObjMarm_c::seStart_MOVESTART() {
    mDoAud_seStart(Z2SE_OBJ_MAGNEARM_MOVESTART, &mSeMarmPos, 0, 0);
}

void daObjMarm_c::seStartLevel_MOVE() {
    mDoAud_seStartLevel(Z2SE_OBJ_MAGNEARM_MOVE, &mSeMarmPos, 0, 0);
}

void daObjMarm_c::seStart_STOP() {
    mDoAud_seStart(Z2SE_OBJ_MAGNEARM_STOP, &mSeMarmPos, 0, 0);
}

void daObjMarm_c::seStartLevel_UP() {
    mDoAud_seStartLevel(Z2SE_OBJ_MAGNEARM_UP, &mSeMarmLiftPos, 0, 0);
}

void daObjMarm_c::seStartLevel_DOWN() {
    mDoAud_seStartLevel(Z2SE_OBJ_MAGNEARM_DOWN, &mSeMarmLiftPos, 0, 0);
}

void daObjMarm_c::seStart_SWING() {
    mDoAud_seStart(Z2SE_OBJ_MAGNEARM_SWING, &mSeMarmSwingPos, 0, 0);
}

int daObjMarm_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[0], &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[1], &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[2], &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[3], &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[4], &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[5], &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel[0]);
    mDoExt_modelUpdateDL(mpModel[2]);

    mpBrkAnm->entry(mpModel[5]->getModelData());
    mpBtkAnm->entry(mpModel[5]->getModelData());
    mDoExt_modelUpdateDL(mpModel[5]);
    mDoExt_brkAnmRemove(mpModel[5]->getModelData());
    mDoExt_btkAnmRemove(mpModel[5]->getModelData());

    dComIfGd_setList();
    mDoExt_modelUpdateDL(mpModel[1]);
    mDoExt_modelUpdateDL(mpModel[3]);
    mDoExt_modelUpdateDL(mpModel[4]);

    GXColor color = {0, 0, 0, 0};
    mpRope1->update(field_0xA28, 25.5f, color, 0, &tevStr);
    dComIfGd_set3DlineMat(mpRope1);
    mpRope2->update(field_0xA29, 25.5f, color, 0, &tevStr);
    dComIfGd_set3DlineMat(mpRope2);

    cXyz offset1;
    cXyz offset2(0.0f, -800.0f, 0.0f);

    mDoMtx_stack_c::YrotS(current.angle.y + mCPartsYRot + mYRotOffset);
    mDoMtx_stack_c::ZrotM(mFPartsZRot);
    mDoMtx_stack_c::multVec(&offset2, &offset2);
    getFpartsOffset(&offset1);
    offset1 += offset2;
    if (fopAcM_gc_c::gndCheck(&offset1)) {
        f32 ground_y = fopAcM_gc_c::getGroundY();
        tevStr.mLightPosWorld = offset2;
        tevStr.field_0x344 = 0.3f;
        mShadowKey = dComIfGd_setShadow(mShadowKey, 0, mpModel[5], &offset1, 1500.0f, 450.0f,
                                        offset1.y, ground_y, fopAcM_gc_c::mGndCheck, &tevStr, 0,
                                        1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
    debugDraw();

    return 1;
}

void daObjMarm_c::debugDraw() {
    /* empty function */
}

int daObjMarm_c::Delete() {
    if (mpBgW1 != NULL && mpBgW1->ChkUsed() != NULL) {
        dComIfG_Bgsp().Release(mpBgW1);
    }

    if (mpBgW2 != NULL && mpBgW2->ChkUsed() != NULL) {
        dComIfG_Bgsp().Release(mpBgW2);
    }
    endMagneHoleEffect();
    dComIfG_resDelete(&mPhase, l_arcName);

    return 1;
}

static int daObjMarm_create1st(daObjMarm_c* i_this) {
    fopAcM_ct(i_this, daObjMarm_c);
    return i_this->create1st();
}

static int daObjMarm_MoveBGDelete(daObjMarm_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjMarm_MoveBGExecute(daObjMarm_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjMarm_MoveBGDraw(daObjMarm_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjMarm_METHODS = {
    (process_method_func)daObjMarm_create1st,
    (process_method_func)daObjMarm_MoveBGDelete,
    (process_method_func)daObjMarm_MoveBGExecute,
    0,
    (process_method_func)daObjMarm_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_MagneArm = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_MagneArm,       // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjMarm_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    614,                     // mPriority
    &daObjMarm_METHODS,      // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
