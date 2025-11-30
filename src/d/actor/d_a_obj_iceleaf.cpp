/**
* @file d_a_obj_iceleaf.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_iceleaf.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daObjIceLeaf_c*)i_this)->CreateHeap();
}

void daObjIceLeaf_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

static char* l_arcName = "V_IceLeaf";

void daObjIceLeaf_c::setBaseMtx() {
    if (mRide) {
        daPy_py_c* player = dComIfGp_getLinkPlayer();
        s16 foot_angle = 0;

        if (!player->checkBoardNoFootAngle()) {
            Vec left_vec;
            Vec right_vec;
            mDoMtx_multVec(player->getInvMtx(), player->getRightFootPosP(), &right_vec);
            mDoMtx_multVec(player->getInvMtx(), player->getLeftFootPosP(), &left_vec);

            foot_angle = cM_atan2s(left_vec.y - right_vec.y, right_vec.z - left_vec.z);
        }

        cXyz sp44(KREG_F(15), -9.0f, 0.0f);

        mDoMtx_stack_c::copy(player->getModelMtx());
        mDoMtx_stack_c::ZXYrotM(foot_angle, player->getBoardCutTurnOffsetAngleY(), 0);
        mDoMtx_stack_c::YrotM(0x4000);
        mDoMtx_stack_c::transM(sp44);
        mDoMtx_stack_c::multVecZero(&eyePos);
    } else {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
    }

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (mpBck == NULL) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
    } else {
        J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 4);
        JUT_ASSERT(0x106, pbck != NULL);
        pbck->getTransform(0, &mTransformInfo);

        cXyz pos;
        csXyz angle;
        mDoMtx_stack_c::transS(mTransformInfo.mTranslate.x, mTransformInfo.mTranslate.y,
                               mTransformInfo.mTranslate.z);
        mDoMtx_stack_c::transM(current.pos);
        mDoMtx_stack_c::multVecZero(&pos);

        angle.x = shape_angle.x + mTransformInfo.mRotation.x;
        angle.y = shape_angle.y + mTransformInfo.mRotation.y;
        angle.z = shape_angle.z + mTransformInfo.mRotation.z;
        mDoMtx_stack_c::transS(pos);
        mDoMtx_stack_c::ZXYrotM(angle);
    }

    MTXCopy(mDoMtx_stack_c::get(), mMtx);
}

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1f}, {0x400000, 0x11}, 0x78}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                 // mGObjTg
        {0x0},                                              // mGObjCo
    },                                                      // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        50.0f,               // mRadius
        100.0f               // mHeight
    }  // mCyl
};

int daObjIceLeaf_c::Create() {
    J3DJoint* joint = mpModel->getModelData()->getJointNodePointer(0);
    mTransformInfo = joint->getTransformInfo();

    J3DTransformInfo trans_info(mTransformInfo);
    trans_info.mRotation.x = 0;
    trans_info.mRotation.y = 0;
    trans_info.mRotation.z = 0;
    trans_info.mTranslate.x = 0.0f;
    trans_info.mTranslate.y = 0.0f;
    trans_info.mTranslate.z = 0.0f;
    joint->setTransformInfo(trans_info);

    initBaseMtx();
    fopAcM_SetMtx(this, mMtx);

    mAcchCir.SetWall(30.0f, 30.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), NULL, NULL);

    mStts.Init(0, 0xFF, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    mCyl.SetR(100.0f);
    mCyl.SetH(500.0f);

    fopAcM_setCullSizeBox2(this, mpModel->getModelData());

    mEvId = getEvId();
    mEventID = dComIfGp_getEventManager().getEventIdx(this, mEvId);

    if (checkRideStatus() == 1) {
        mRide = true;
        mpBck = NULL;
        setAnmPos();
    }

    return 1;
}

void daObjIceLeaf_c::setAnmPos() {
    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(0x168, pbck != NULL);
    pbck->getTransform(0, &mTransformInfo);

    mDoMtx_stack_c::transS(mTransformInfo.mTranslate.x, mTransformInfo.mTranslate.y,
                           mTransformInfo.mTranslate.z);
    mDoMtx_stack_c::transM(current.pos);
    mDoMtx_stack_c::multVecZero(&current.pos);

    shape_angle.x += mTransformInfo.mRotation.x;
    shape_angle.y += mTransformInfo.mRotation.y;
    shape_angle.z += mTransformInfo.mRotation.z;
}

int daObjIceLeaf_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 7);
    JUT_ASSERT(0x189, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(0x196, pbck != NULL);

    mpBck = new mDoExt_bckAnm();
    if (mpBck == NULL || !mpBck->init(pbck, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false)) {
        return 0;
    }

    mpBck->setPlaySpeed(0.0f);
    mpBck->setFrame(0.0f);
    return 1;
}

int daObjIceLeaf_c::create() {
    fopAcM_ct(this, daObjIceLeaf_c);

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0xDA0)) {
            return cPhs_ERROR_e;
        }

        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }

    return phase_state;
}

int daObjIceLeaf_c::execute() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    event_proc_call();
    action();

    if (mDeleteActor) {
        if (field_0x92b == 0) {
            dComIfGp_particle_set(0x8C12, &eyePos, NULL, NULL);
            dComIfGp_particle_set(0x8C13, &eyePos, NULL, NULL);

            field_0x92b = 1;
            player->seStartOnlyReverb(Z2SE_AL_SNOBO_ICEBREAK);
        }

        fopAcM_delete(this);
        // "Ice Leaf: Deleted\n"
        OS_REPORT("氷の葉：削除されました\n");
    }

    if (mBreakEffect) {
        dComIfGp_particle_set(0x8C12, &eyePos, NULL, NULL);
        dComIfGp_particle_set(0x8C13, &eyePos, NULL, NULL);

        mBreakEffect = false;
        player->seStartOnlyReverb(Z2SE_AL_SNOBO_ICEBREAK);
    }

    setBaseMtx();
    return 1;
}

void daObjIceLeaf_c::action() {
    typedef void (daObjIceLeaf_c::*mode_func)();
    static mode_func l_func[] = {
        &daObjIceLeaf_c::modeDropWait,
        &daObjIceLeaf_c::modeDrop,
        &daObjIceLeaf_c::modePlayerWait,
        &daObjIceLeaf_c::modeRide,
    };

    (this->*l_func[mMode])();
}

void daObjIceLeaf_c::modeDropWait() {
    daPy_py_c* player = daPy_getPlayerActorClass();

        /* dSv_event_flag_c::F_0308 - Snowpeak mountain - Watched first meeting event with Yeto at top of mountain (human) */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x134])) {
        bool start_drop = false;
        if (player->current.pos.abs(current.pos) < 250.0f &&
            (player->checkFrontRollCrash() || player->checkWolfAttackReverse()))
        {
            start_drop = true;
        }

        if (mCyl.ChkTgHit()) {
            cCcD_Obj* hit_obj = mCyl.GetTgHitObj();
            if (hit_obj != NULL && hit_obj->ChkAtType(AT_TYPE_IRON_BALL)) {
                start_drop = true;
            }
        }

        if (start_drop) {
            setAction(ACT_ORDER_EVENT_e);
        }
    }

    if (mRide) {
        mpBck = NULL;
        setAnmPos();
    }

    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
}

void daObjIceLeaf_c::modeDrop() {
    if (mpBck->play()) {
        J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 4);
        JUT_ASSERT(0x2C8, pbck != NULL);
        pbck->getTransform(0, &mTransformInfo);

        mDoMtx_stack_c::transS(mTransformInfo.mTranslate.x, mTransformInfo.mTranslate.y,
                               mTransformInfo.mTranslate.z);
        mDoMtx_stack_c::transM(current.pos);
        mDoMtx_stack_c::multVecZero(&current.pos);

        shape_angle.x += mTransformInfo.mRotation.x;
        shape_angle.y += mTransformInfo.mRotation.y;
        shape_angle.z += mTransformInfo.mRotation.z;

        attention_info.position = current.pos;
        eyePos = current.pos;

        mpBck = NULL;
        setMode(MODE_PLAYER_WAIT_e);
    }

    if (mpBck != NULL && mpBck->getFrame() == 12) {
        cXyz sp24(KREG_F(10) + 1.0f, KREG_F(10) + 1.0f, KREG_F(10) + 1.0f);

        J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 4);
        JUT_ASSERT(0x2EB, pbck != NULL);
        pbck->getTransform(0, &mTransformInfo);

        cXyz sp30;
        mDoMtx_stack_c::transS(mTransformInfo.mTranslate.x, mTransformInfo.mTranslate.y,
                               mTransformInfo.mTranslate.z);
        mDoMtx_stack_c::transM(current.pos);
        mDoMtx_stack_c::multVecZero(&sp30);
        sp30.y += 5.0f;

        if (fopAcM_gc_c::gndCheck(&sp30)) {
            dComIfGp_particle_setPolyColor(0x881F, *fopAcM_gc_c::getGroundCheck(), &sp30, &tevStr,
                                           &current.angle, &sp24, 0, NULL, -1, NULL);
            dComIfGp_particle_setPolyColor(0x8820, *fopAcM_gc_c::getGroundCheck(), &sp30, &tevStr,
                                           &current.angle, &sp24, 0, NULL, -1, NULL);

            mFallSEPos = sp30;
            mDoAud_seStart(Z2SE_OBJ_LEAF_FALL_DOWN, &mFallSEPos, 0, 0);
            // "Ice Leaf: Falling SE\n"
            OS_REPORT("氷の葉：落下ＳＥ\n");
        }
    }
}

void daObjIceLeaf_c::modePlayerWait() {
    attention_info.flags = fopAc_AttnFlag_ETC_e;
}

void daObjIceLeaf_c::modeRide() {}

void daObjIceLeaf_c::event_proc_call() {
    typedef void (daObjIceLeaf_c::*action_func)();
    static action_func l_func[] = {
        &daObjIceLeaf_c::actionWait,
        &daObjIceLeaf_c::actionOrderEvent,
        &daObjIceLeaf_c::actionEvent,
        &daObjIceLeaf_c::actionDead,
    };

    (this->*l_func[mAction])();
}

void daObjIceLeaf_c::actionWait() {}

void daObjIceLeaf_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACT_EVENT_e);
        mpBck->setPlaySpeed(1.0f);
        setMode(MODE_DROP_e);
        setFallSE();

        fopAcM_onSwitch(this, getSwbit());
    } else {
        fopAcM_orderOtherEventId(this, mEventID, mEvId, 0xFFFF, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daObjIceLeaf_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventID)) {
        setAction(ACT_DEAD_e);
        dComIfGp_event_reset();
    }
}

void daObjIceLeaf_c::actionDead() {}

void daObjIceLeaf_c::setFallSE() {
    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(0x36C, pbck != NULL);
    pbck->getTransform(0, &mTransformInfo);

    mDoMtx_stack_c::transS(mTransformInfo.mTranslate.x, mTransformInfo.mTranslate.y,
                           mTransformInfo.mTranslate.z);
    mDoMtx_stack_c::transM(current.pos);
    mDoMtx_stack_c::multVecZero(&mFallStartSEPos);

    mDoAud_seStart(Z2SE_OBJ_LEAF_BEGIN_FALL, &mFallStartSEPos, 0, 0);
    // "Ice Leaf: Fall start SE(%.2f,%.2f,%.2f)\n"
    OS_REPORT("氷の葉：落下開始ＳＥ(%.2f,%.2f,%.2f)\n", mFallStartSEPos.x, mFallStartSEPos.y,
              mFallStartSEPos.z);
}

int daObjIceLeaf_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    if (mpBck != NULL) {
        mpBck->entry(mpModel->getModelData());
    }

    mDoExt_modelUpdateDL(mpModel);
    mDoExt_bckAnmRemove(mpModel->getModelData());

    return 1;
}

int daObjIceLeaf_c::_delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjIceLeaf_Draw(daObjIceLeaf_c* i_this) {
    return i_this->draw();
}

static int daObjIceLeaf_Execute(daObjIceLeaf_c* i_this) {
    return i_this->execute();
}

static int daObjIceLeaf_Delete(daObjIceLeaf_c* i_this) {
    return i_this->_delete();
}

static int daObjIceLeaf_Create(daObjIceLeaf_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daObjIceLeaf_Method = {
    (process_method_func)daObjIceLeaf_Create,  (process_method_func)daObjIceLeaf_Delete,
    (process_method_func)daObjIceLeaf_Execute, (process_method_func)NULL,
    (process_method_func)daObjIceLeaf_Draw,
};

extern actor_process_profile_definition g_profile_Obj_IceLeaf = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_IceLeaf,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjIceLeaf_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    567,                     // mPriority
    &l_daObjIceLeaf_Method,  // sub_method
    0x00060000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
