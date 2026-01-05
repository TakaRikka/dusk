/**
 * @file d_a_obj_lv5yiblltray.cpp
 * Snowpeak Ruins Cannon Ball Tray
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv5yiblltray.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_obj_carry.h"

static char* l_arcName = "YIblltray";

int daObjYIblltray_c::create1st() {
    int phase = dComIfG_resLoad(this, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        setMtx();

        phase = MoveBGCreate(l_arcName, 12, dBgS_MoveBGProc_TypicalRotY, 0x1210, &field_0x5c0);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }

    return phase;
}

static const cull_box l_cull_box = {
    {-238.95999f, -200.0f, -134.22f},
    {238.95999f, 200.0f, 218.67f},
};

static Vec l_in_ball = {0.0f, 0.0f, 87.0f};

static Vec l_base = {0.0f, 0.0f, -60.0f};

void daObjYIblltray_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);

    if (getType() == 0) {
        mDoMtx_stack_c::transM(125.0f, 100.0f, 0.0f);
    } else {
        mDoMtx_stack_c::transM(-125.0f, 100.0f, 0.0f);
    }

    mDoMtx_stack_c::XrotM(current.angle.x);
    MTXCopy(mDoMtx_stack_c::get(), mTrayMtx);
    MTXCopy(mDoMtx_stack_c::get(), field_0x5c0);

    MTXMultVec(field_0x5c0, &l_in_ball, &mInBallPos);
    MTXMultVec(field_0x5c0, &l_base, &mBasePos);

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    MTXCopy(mDoMtx_stack_c::get(), mHandleMtx);
}

int daObjYIblltray_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 9);
    JUT_ASSERT(0, model_data != NULL);

    mpTrayModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpTrayModel == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 8);
    JUT_ASSERT(0, model_data != NULL);

    mpHandleModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpHandleModel == NULL) {
        return 0;
    }

    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 5);
    if (!mBck.init(bck, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false)) {
        return 0;
    }

    mBck.setPlaySpeed(0.0f);
    return 1;
}

static void rideCallback(dBgW* i_bgw, fopAc_ac_c* i_this, fopAc_ac_c* i_rideActor) {
    ((daObjYIblltray_c*)i_this)->rideActor(i_rideActor);
}

static Vec l_back_pos = {0.0f, -52.0f, -369.0f};

static dCcD_SrcCyl l_cc_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x39}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x2},          // mGObjTg
        {0x0},                                       // mGObjCo
    },                                               // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            90.0f,               // mRadius
            200.0f               // mHeight
        }  // mCyl
    }
};

int daObjYIblltray_c::Create() {
    mpTrayModel->setBaseTRMtx(mTrayMtx);
    fopAcM_SetMtx(this, mTrayMtx);

    if (getType() == 0) {
        if (!fopAcM_isSwitch(this, getSwNo())) {
            current.angle.x = 0x1A4F;
            mMode = MODE_FRONT_WAIT;
            mBck.setFrame(mBck.getEndFrame());
            mBck.setPlayMode(J3DFrameCtrl::EMode_NONE);
        } else {
            current.angle.x = -0x1A4F;
            mMode = MODE_BACK_RIDE;
            mBck.setFrame(mBck.getStartFrame());
            mBck.setPlayMode(J3DFrameCtrl::EMode_RESET);
        }
    } else {
        if (!fopAcM_isSwitch(this, getSwNo())) {
            current.angle.x = -0x1A4F;
            mMode = MODE_BACK_RIDE;
            mBck.setFrame(mBck.getStartFrame());
            mBck.setPlayMode(J3DFrameCtrl::EMode_RESET);
        } else {
            current.angle.x = 0x1A4F;
            mMode = MODE_FRONT_WAIT;
            mBck.setFrame(mBck.getEndFrame());
            mBck.setPlayMode(J3DFrameCtrl::EMode_NONE);
        }
    }

    mBck.play();
    setMtx();

    mRotAmount = 0;

    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z,
                          l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);

    mBallID = fpcM_ERROR_PROCESS_ID_e;
    mTrayID = fpcM_ERROR_PROCESS_ID_e;

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);

    if (getType() == 0) {
        mDoMtx_stack_c::transM(125.0f, 100.0f, 0.0f);
    } else {
        mDoMtx_stack_c::transM(-125.0f, 100.0f, 0.0f);
    }

    MTXMultVec(mDoMtx_stack_c::get(), &l_back_pos, &mBackPos);

    mCcStts.Init(0xFF, 0, this);
    mCcCyl[0].Set(l_cc_cyl_src);
    mCcCyl[0].SetStts(&mCcStts);

    mCcCyl[1].Set(l_cc_cyl_src);
    mCcCyl[1].SetStts(&mCcStts);
    mCcCyl[1].SetR(40.0f);
    mCcCyl[1].SetH(300.0f);

    mCcCyl[2].Set(l_cc_cyl_src);
    mCcCyl[2].SetStts(&mCcStts);
    mCcCyl[2].SetR(100.0f);
    mCcCyl[2].SetH(10.0f);
    mCcCyl[2].SetCoVsGrp(0x60);

    attention_info.distances[fopAc_attn_ETC_e] = 84;
    return 1;
}

void daObjYIblltray_c::rideActor(fopAc_ac_c* i_actor) {
    if (mBallID == fpcM_ERROR_PROCESS_ID_e && fopAcM_GetProfName(i_actor) == PROC_Obj_Carry &&
        ((daObjCarry_c*)i_actor)->getType() == daObjCarry_c::TYPE_IRON_BALL)
    {
        mBallID = fopAcM_GetID(i_actor);
        mMode = MODE_FRONT_RIDE;
        fopAcM_SetRoomNo(i_actor, fopAcM_GetHomeRoomNo(this));
    }
}

void daObjYIblltray_c::sendBall(fpc_ProcID i_ID) {
    mBallID = i_ID;
    mMode = MODE_SEND_BALL;
    field_0x63c = 0.0f;

    daObjCarry_c* ball = (daObjCarry_c*)fopAcM_SearchByID(mBallID);
    ball->current.pos = mBasePos;
    ball->startCtrl();
    fopAcM_SetRoomNo(ball, fopAcM_GetHomeRoomNo(this));
}

static fopAc_ac_c* searchObjYIblltray(void* i_trayB, void* i_trayA) {
    if (i_trayB != NULL && fopAcM_IsActor(i_trayB) &&
        fopAcM_GetProfName(i_trayB) == PROC_Obj_YIblltray && i_trayB != i_trayA)
    {
        if (((daObjYIblltray_c*)i_trayA)->getSwNo() == ((daObjYIblltray_c*)i_trayB)->getSwNo()) {
            return (fopAc_ac_c*)i_trayB;
        }
    }

    return NULL;
}

static fopAc_ac_c* searchNearIronball(void* i_ball, void* i_tray) {
    if (i_ball != NULL && fopAcM_IsActor(i_ball) && fopAcM_GetProfName(i_ball) == PROC_Obj_Carry &&
        ((daObjCarry_c*)i_ball)->getType() == daObjCarry_c::TYPE_IRON_BALL &&
        VECSquareDistance(&((daObjCarry_c*)i_ball)->current.pos,
                          &((daObjYIblltray_c*)i_tray)->mInBallPos) < 900.0f)
    {
        return (fopAc_ac_c*)i_ball;
    }

    return NULL;
}

static fopAc_ac_c* searchNearBackTrayIronball(void* i_ball, void* i_tray) {
    if (i_ball != NULL && fopAcM_IsActor(i_ball) && fopAcM_GetProfName(i_ball) == PROC_Obj_Carry &&
        ((daObjCarry_c*)i_ball)->getType() == daObjCarry_c::TYPE_IRON_BALL &&
        VECSquareDistance(&((daObjCarry_c*)i_ball)->current.pos,
                          &((daObjYIblltray_c*)i_tray)->mBackPos) < 900.0f)
    {
        return (fopAc_ac_c*)i_ball;
    }

    return NULL;
}

void daObjYIblltray_c::onAttention() {
    attention_info.flags |= fopAc_AttnFlag_ETC_e;
}

void daObjYIblltray_c::offAttention() {
    attention_info.flags &= ~fopAc_AttnFlag_ETC_e;
}

int daObjYIblltray_c::Execute(Mtx** param_0) {
    if (!getNoSameRoom() && mTrayID == fpcM_ERROR_PROCESS_ID_e) {
        daObjYIblltray_c* ptray =
            (daObjYIblltray_c*)fopAcM_Search((fopAcIt_JudgeFunc)searchObjYIblltray, this);
        JUT_ASSERT(0, ptray != NULL);

        mTrayID = fopAcM_GetID(ptray);
    }

    if (mMode == MODE_FRONT_WAIT) {
        mpBgW->SetRideCallback(rideCallback);

        fopAc_ac_c* ball = fopAcM_Search((fopAcIt_JudgeFunc)searchNearIronball, this);
        if (ball != NULL) {
            rideActor(ball);
        }
    } else {
        mpBgW->SetRideCallback(NULL);
    }

    setMtx();
    mpTrayModel->setBaseTRMtx(mTrayMtx);
    mpHandleModel->setBaseTRMtx(mHandleMtx);
    *param_0 = &field_0x5c0;

    if (mMode == MODE_BACK_RIDE && mBck.getFrame() == mBck.getStartFrame()) {
        onAttention();
    } else {
        offAttention();
    }

    if (mIsHang) {
        mIsHang = false;

        if (!fopAcM_isSwitch(this, getSwNo())) {
            fopAcM_onSwitch(this, getSwNo());

            if (getType() != 0) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_IRONTRAY_SWITCH, &mHandlePos, 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);
            }
        } else {
            fopAcM_offSwitch(this, getSwNo());

            if (getType() == 0) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_IRONTRAY_SWITCH, &mHandlePos, 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);
            }
        }
    }

    if (mMode == MODE_BACK_RIDE) {
        mCcCyl[0].SetCoVsGrp(0x70);

        if (mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }
    } else {
        mCcCyl[0].SetCoVsGrp(0x30);

        if (!mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Regist(mpBgW, this);
        }
    }

    if (getType() == 0) {
        if (!fopAcM_isSwitch(this, getSwNo())) {
            mBck.setPlaySpeed(1.0f);
            mBck.setPlayMode(J3DFrameCtrl::EMode_NONE);

            if (current.angle.x < 0x19EB) {
                mRotAmount += 100;
            } else {
                mRotAmount = (f32)mRotAmount * -0.1f;
                current.angle.x = 0x1A4F;

                if (mMode != MODE_FRONT_WAIT && mMode != MODE_FRONT_RIDE && mMode == MODE_BACK_RIDE)
                {
                    fopAc_ac_c* ball =
                        fopAcM_Search((fopAcIt_JudgeFunc)searchNearBackTrayIronball, this);
                    if (ball == NULL) {
                        mMode = MODE_FRONT_WAIT;
                    } else {
                        sendBall(fopAcM_GetID(ball));
                    }
                }
            }
        } else {
            mBck.setPlaySpeed(-1.0f);
            mBck.setPlayMode(J3DFrameCtrl::EMode_RESET);

            if (current.angle.x > -0x19EB) {
                mRotAmount -= 100;
            } else {
                mRotAmount = (f32)mRotAmount * -0.1f;
                current.angle.x = -0x1A4F;

                if (mMode == MODE_FRONT_WAIT) {
                    mMode = MODE_BACK_RIDE;
                } else if (mMode == MODE_FRONT_RIDE) {
                    initRotTo();
                }
            }
        }

        current.angle.x += mRotAmount;
    } else {
        if (fopAcM_isSwitch(this, getSwNo())) {
            mBck.setPlaySpeed(1.0f);
            mBck.setPlayMode(J3DFrameCtrl::EMode_NONE);

            if (current.angle.x < 0x19EB) {
                mRotAmount += 100;
            } else {
                mRotAmount = (f32)mRotAmount * -0.1f;
                current.angle.x = 0x1A4F;

                if (mMode == MODE_BACK_RIDE) {
                    fopAc_ac_c* ball =
                        fopAcM_Search((fopAcIt_JudgeFunc)searchNearBackTrayIronball, this);
                    if (ball == NULL) {
                        mMode = MODE_FRONT_WAIT;
                    } else {
                        sendBall(fopAcM_GetID(ball));
                    }
                }
            }
        } else {
            mBck.setPlaySpeed(-1.0f);
            mBck.setPlayMode(J3DFrameCtrl::EMode_RESET);

            if (current.angle.x > -0x19EB) {
                mRotAmount -= 100;
            } else {
                mRotAmount = (f32)mRotAmount * -0.1f;
                current.angle.x = -0x1A4F;

                if (mMode == MODE_FRONT_WAIT) {
                    mMode = MODE_BACK_RIDE;
                } else if (mMode == MODE_FRONT_RIDE) {
                    initRotTo();
                }
            }
        }

        current.angle.x += mRotAmount;
    }

    if (mMode == MODE_FRONT_RIDE) {
        daObjCarry_c* ball = (daObjCarry_c*)fopAcM_SearchByID(mBallID);

        if (fopAcM_checkCarryNow(ball)) {
            mBallID = fpcM_ERROR_PROCESS_ID_e;
            mMode = MODE_FRONT_WAIT;
        } else {
            cXyz spAC;
            spAC = mInBallPos - ball->current.pos;

            f32 var_f31 = spAC.abs();
            if (var_f31 <= 30.0f) {
                ball->current.pos = mInBallPos;
            } else if (!cM3d_IsZero(var_f31)) {
                spAC /= var_f31;
                spAC *= 10.0f;
                ball->current.pos += spAC;
                ball->old.pos = ball->current.pos;
            }
        }

        ball->endCtrl();
    }

    if (mMode == MODE_INIT_ROT) {
        cM3d_IsZero(field_0x63c);
        field_0x63c += 1.0f;
        if (field_0x63c >= 10.0f) {
            field_0x63c = 10.0f;
        }

        daObjCarry_c* ball = (daObjCarry_c*)fopAcM_SearchByID(mBallID);

        cXyz spB8;
        spB8 = mBasePos - ball->current.pos;

        f32 var_f30 = spB8.abs();
        if (var_f30 <= field_0x63c) {
            ball->current.pos = mBasePos;

            if (mTrayID != fpcM_ERROR_PROCESS_ID_e) {
                daObjYIblltray_c* ptray = (daObjYIblltray_c*)fopAcM_SearchByID(mTrayID);
                ptray->sendBall(mBallID);
                mBallID = fpcM_ERROR_PROCESS_ID_e;
                mMode = MODE_BACK_RIDE;
            } else {
                fopAc_ac_c* ac = fopAcM_SearchByID(mBallID);
                ac->current.pos = mBackPos;
                mBallID = fpcM_ERROR_PROCESS_ID_e;
                mMode = MODE_BACK_RIDE;
            }
        } else if (!cM3d_IsZero(var_f30)) {
            spB8 /= var_f30;
            spB8 *= field_0x63c;
            ball->current.pos += spB8;
            ball->old.pos = ball->current.pos;
        }
    }

    if (mMode == MODE_SEND_BALL) {
        field_0x63c += 1.0f;
        if (field_0x63c >= 10.0f) {
            field_0x63c = 10.0f;
        }

        daObjCarry_c* ball = (daObjCarry_c*)fopAcM_SearchByID(mBallID);

        cXyz spC4;
        spC4 = mInBallPos - ball->current.pos;

        f32 var_f29 = spC4.abs();
        if (var_f29 <= field_0x63c) {
            ball->current.pos = mInBallPos;

            Z2GetAudioMgr()->seStart(Z2SE_OBJ_IRONTRAY_BALL, &mBasePos, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            mMode = MODE_FRONT_RIDE;
            fopAcM_SearchByID(mBallID)->current.pos = mInBallPos;
        } else if (!cM3d_IsZero(var_f29)) {
            spC4 /= var_f29;
            spC4 *= field_0x63c;
            ball->current.pos += spC4;
            ball->old.pos = ball->current.pos;
        }
    }

    cXyz spD0;
    spD0.x = mTrayMtx[0][3];
    spD0.y = mTrayMtx[1][3];
    spD0.z = mTrayMtx[2][3];
    spD0.y -= 100.0f;

    mCcCyl[0].SetC(spD0);
    dComIfG_Ccsp()->Set(&mCcCyl[0]);

    mCcCyl[1].SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcCyl[1]);

    spD0.y = home.pos.y;
    mCcCyl[2].SetC(spD0);
    dComIfG_Ccsp()->Set(&mCcCyl[2]);

    mCcStts.Move();

    mBck.play();

    cXyz spDC;
    spDC.x = 0.0f;
    spDC.y = 0.0f;
    spDC.z = 100.0f;

    MTXMultVec(mpHandleModel->getAnmMtx(0), &spDC, &mHandlePos);
    eyePos = mHandlePos;

    Mtx spA0;
    if (mMode == MODE_FRONT_WAIT && MTXInverse(mHandleMtx, spA0)) {
        Vec spE8;
        daPy_py_c* player = daPy_getPlayerActorClass();
        MTXMultVec(spA0, &player->current.pos, &spE8);

        if (cM_scos(player->current.angle.y - current.angle.y) < -0.7f && spE8.y > -50.0f &&
            spE8.y < 50.0f && spE8.z > 50.0f && spE8.z < 160.0f &&
            ((getType() == 0 && spE8.x < 175.0f && spE8.x > 75.0f) ||
             (getType() != 0 && spE8.x < -55.0f && spE8.x > -195.0f)))
        {
            player->onDoPutEmphasys();
        }
    }

    return 1;
}

void daObjYIblltray_c::initRotTo() {
    mMode = MODE_INIT_ROT;
    field_0x63c = 0.0f;

    daObjCarry_c* ball = (daObjCarry_c*)fopAcM_SearchByID(mBallID);
    ball->startCtrl();
}

int daObjYIblltray_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpTrayModel, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpHandleModel, &tevStr);

    mBck.entry(mpHandleModel->getModelData());

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpTrayModel);
    mDoExt_modelUpdateDL(mpHandleModel);
    dComIfGd_setList();
    return 1;
}

int daObjYIblltray_c::Delete() {
    if (mBallID != fpcM_ERROR_PROCESS_ID_e) {
        daObjCarry_c* ball = (daObjCarry_c*)fopAcM_SearchByID(mBallID);
        ball->startCtrl();
    }

    dComIfG_resDelete(this, l_arcName);
    return 1;
}

static int daObjYIblltray_create1st(daObjYIblltray_c* i_this) {
    fopAcM_ct(i_this, daObjYIblltray_c);
    return i_this->create1st();
}

static int daObjYIblltray_MoveBGDelete(daObjYIblltray_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjYIblltray_MoveBGExecute(daObjYIblltray_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjYIblltray_MoveBGDraw(daObjYIblltray_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjYIblltray_METHODS = {
    (process_method_func)daObjYIblltray_create1st,
    (process_method_func)daObjYIblltray_MoveBGDelete,
    (process_method_func)daObjYIblltray_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjYIblltray_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_YIblltray = {
    fpcLy_CURRENT_e,           // mLayerID
    3,                         // mListID
    fpcPi_CURRENT_e,           // mListPrio
    PROC_Obj_YIblltray,        // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(daObjYIblltray_c),  // mSize
    0,                         // mSizeOther
    0,                         // mParameters
    &g_fopAc_Method.base,      // sub_method
    667,                       // mPriority
    &daObjYIblltray_METHODS,   // sub_method
    0x00044100,                // mStatus
    fopAc_ACTOR_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,    // cullType
};
