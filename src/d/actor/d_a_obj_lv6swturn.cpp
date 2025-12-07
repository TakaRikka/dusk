/**
 * @file d_a_obj_lv6swturn.cpp
 *
 */

 #include "d/dolzel_rel.h"  // IWYU pragma: keep
#include "d/d_s_play.h"


#include "d/actor/d_a_obj_lv6swturn.h"
#include "d/actor/d_a_player.h"

#if DEBUG
daObjLv6SwTurn_HIO_c::daObjLv6SwTurn_HIO_c() {
    mWaitTime = 0x2f;
    mChargeTime = 0x23;
    mRotationTime = 0x5a;
    unkA = 0x0;
    mDrawFlag = 0x0;
}

void daObjLv6SwTurn_HIO_c::genMessage(JORMContext* context) {
    context->genLabel("Ｌｖ６回転スイッチ", 0, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("タメ時間（初回）", &mWaitTime, 0, 100, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("タメ時間（連続）", &mChargeTime, 0, 100, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("回転時間", &mRotationTime, 0, 500, 0, NULL, -1, -1, 0x200, 0x18);
    context->genCheckBox("チェック描画", &mDrawFlag, 1, 0, NULL, -1, -1, 0x200, 0x18);
}
#endif

static fopAc_ac_c* PPCallBack(fopAc_ac_c* actor1, fopAc_ac_c* actor2, s16 param_2,
                              dBgW_Base::PushPullLabel pushPull) {
    (void)param_2;  // needed to match debug
    dBgW_Base::PushPullLabel pushPull_3 = cLib_checkBit(pushPull, dBgW_Base::PPLABEL_3);
    daObjLv6SwTurn_c* swTurnActor1 = (daObjLv6SwTurn_c*)actor1;
    daObjLv6SwTurn_c* swTurnActor2 = (daObjLv6SwTurn_c*)actor2;
    if (pushPull_3 != dBgW_Base::PPLABEL_NONE) {
        cXyz posVec = swTurnActor2->current.pos - swTurnActor1->current.pos;
        mDoMtx_stack_c::YrotS(-swTurnActor1->shape_angle.y);
        mDoMtx_stack_c::multVec(&posVec, &posVec);
        int ind;
        if (posVec.z > 0.0f) {
            ind = cLib_checkBit(pushPull_3, dBgW_Base::PPLABEL_PULL) ? 0 : 1;
        } else {
            ind = cLib_checkBit(pushPull_3, dBgW_Base::PPLABEL_PULL) ? 1 : 0;
        }
        for (int i = 0; i < 2; i++) {
            if (i == ind) {
                swTurnActor1->unk5AE[i]++;
            } else {
                swTurnActor1->unk5AE[i] = 0;
            }
        }
        swTurnActor1->unk5AD = 1;
        swTurnActor1->unk5C4 = pushPull;
    }
    return actor1;
}

static char* l_arcName = "Obj_l6tsw";

void daObjLv6SwTurn_c::initBaseMtx() {
    unk5A8->setBaseScale(scale);
    setBaseMtx();
}

void daObjLv6SwTurn_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    unk5A8->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjLv6SwTurn_c::Create() {
    if (fopAcM_isSwitch(this, getSwbit())) {
        unk5BC = 1;
    } else if (fopAcM_isSwitch(this, getSwbit2())) {
        unk5BC = -1;
    } else {
        unk5BC = 0;
    }
    fopAcM_SetMtx(this, unk5A8->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, unk5A8->getModelData());
    mpBgW->SetPushPullCallback(PPCallBack);
    init_modeWait();
    MoveBGExecute();
    initBaseMtx();
    return TRUE;
}


int daObjLv6SwTurn_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(268, modelData != NULL);
    unk5A8 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (unk5A8 == NULL) {
        return FALSE;
    }
    return TRUE;
}

#if DEBUG
static daObjLv6SwTurn_HIO_c l_HIO;
#endif

int daObjLv6SwTurn_c::create1st() {
    int step = dComIfG_resLoad(&unk5A0, l_arcName);
    if (step == cPhs_COMPLEATE_e) {
        step = MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0x1800, 0);
        if (step == cPhs_ERROR_e) {
            return step;
        }
#if DEBUG
        l_HIO.entryHIO("Ｌｖ６回転スイッチ");
#endif
    }
    return step;
}

int daObjLv6SwTurn_c::Execute(Mtx** i_pMtx) {
    unk5C0++;
    mode_proc_call();
    *i_pMtx = &mBgMtx;
    setBaseMtx();
    eyePos = current.pos;
    attention_info.position = current.pos;
    unk5AD = 0;
    return TRUE;
}

enum daObjLv6SwTurn_c_mode{
    DAOBJLV6SWTURN_C_MODE_WAIT,
    DAOBJLV6SWTURN_C_MODE_ROTATE,
};

void daObjLv6SwTurn_c::mode_proc_call() {
    typedef void (daObjLv6SwTurn_c::*func_type)();
    // naming difference between debug and release
#if DEBUG
    static func_type mode_proc_call[] = {
        &daObjLv6SwTurn_c::modeWait,
        &daObjLv6SwTurn_c::modeRotate
    };
    (this->*mode_proc_call[mMode])();
#else
    static func_type l_func[] = {
        &daObjLv6SwTurn_c::modeWait,
        &daObjLv6SwTurn_c::modeRotate
    };
    (this->*l_func[mMode])();
#endif
}

void daObjLv6SwTurn_c::init_modeWait() {
    unk5B2 = 0;
    unk5B8 = -1;
    for (int i = 0; i < 2; i++) {
        unk5AE[i] = 0;
    }
    mMode = DAOBJLV6SWTURN_C_MODE_WAIT;
}

void daObjLv6SwTurn_c::modeWait() {
    bool a = true;
    int b = -1;
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (unk5AD) {
        for (int i = 0; i < 2; i++) {
            u32 timer;
            if (cLib_checkBit(unk5C4, cBgW::PPLABEL_4)) {
#if DEBUG
                timer = l_HIO.mWaitTime;
#else
                timer = 0x2f;
#endif
            } else {
#if DEBUG
                timer = l_HIO.mChargeTime;
#else
                timer = 0x23;
#endif
            }
            if (unk5AE[i] == (u8)timer) {
                unk5B8 = i;
            }
            if (unk5AE[i] != 0) {
                b = i;
            }
        }
        if ((unk5B8 == 0 && unk5BC == 1) || (unk5B8 == 1 && unk5BC == -1)) {
            unk5B8 = -1;
        } else {
            a = 0;
        }
        bool c = false;
        if ((unk5BC == 0 && b != -1) || (unk5BC == 1 && b == 1) || (unk5BC == -1 && b == 0)) {
            c = true;
        }
        if (c) {
            if (b == 0) {
                cLib_addCalcAngleS2(&unk5B4, 500 + KREG_S(1), 60, 5);
            } else {
                cLib_addCalcAngleS2(&unk5B4, -500 - KREG_S(1), 60, 5);
            }
            if (unk5BD == 18) {
                fopAcM_seStart(this, Z2SE_OBJ_L6_RL_SW_ON, 0);
                OS_REPORT("Ｌｖ６回転スイッチ：Z2SE_OBJ_L6_RL_SW_ON\n");
            }
            unk5BD++;
            unk5B2 = unk5B4 * std::fabs(cM_ssin((KREG_S(0) + 992) * unk5BD)) + 10.0f;
            if (unk5B2 == unk5C8) {
                unk5B2++;
            }
        }
        if (unk5B8 != -1 && unk5CA == current.angle.y) {
            ((daPy_py_c*)dComIfGp_getPlayer(0))->onPushPullKeep();
            init_modeRotate();
        }
    } else {
        unk5B4 = 0;
        unk5BD = 0;
        cLib_addCalcAngleS(&unk5B2, 0, 4, 50, 5);
    }
    shape_angle.y = current.angle.y + (unk5BC * 0x4000) + unk5B2;
    if (a) {
        for (int i = 0; i < 2; i++) {
            unk5AE[i] = 0;
        }
        unk5B8 = -1;
    }
    if (daPy_py_c::checkNowWolf()) {
        cXyz c = player->current.pos - current.pos;
        mDoMtx_stack_c::YrotS(-shape_angle.y);
        mDoMtx_stack_c::multVec(&c, &c);
        if (c.z > 0.0f) {
            if (unk5BC == -1) {
                mpBgW->OffPushPullOk();
            } else {
                mpBgW->OnPushPullOk();
            }
        } else {
            if (unk5BC == 1) {
                mpBgW->OffPushPullOk();
            } else {
                mpBgW->OnPushPullOk();
            }
        }
    } else {
        mpBgW->OnPushPullOk();
    }
    unk5C8 = unk5B2;
    unk5CA = current.angle.y;
}

void daObjLv6SwTurn_c::init_modeRotate() {
    unk5B4 = 0;
    unk5BD = 0;
    unk5B0 = 0;
    unk5B6 = shape_angle.y;
    mMode = DAOBJLV6SWTURN_C_MODE_ROTATE;
}

void daObjLv6SwTurn_c::modeRotate() {
    unk5B0++;
#if DEBUG
    unk5B2 = (f32)unk5B0 / (f32)l_HIO.mRotationTime * (f32)0x4000;
#else
    unk5B2 = (f32)unk5B0 / (f32)0x5a * (f32)0x4000;
#endif
    if (unk5B8 == 1) {
        unk5B2 = -unk5B2;
    }

    shape_angle.y = unk5B6 + unk5B2;

    fopAcM_seStartLevel(this, Z2SE_OBJ_L6_RL_SW_MV, 0);
#if DEBUG
    if (unk5B0 == l_HIO.mRotationTime)
#else
    if (unk5B0 == 0x5a)
#endif
    {
        if (unk5B8 == 0) {
            unk5BC++;
        } else {
            unk5BC--;
        }

        if (unk5BC == 1) {
            fopAcM_onSwitch(this, getSwbit());
            fopAcM_offSwitch(this, getSwbit2());
        } else if (unk5BC == 0) {
            fopAcM_offSwitch(this, getSwbit());
            fopAcM_offSwitch(this, getSwbit2());
        } else if (unk5BC == -1) {
            fopAcM_offSwitch(this, getSwbit());
            fopAcM_onSwitch(this, getSwbit2());
        }
        ((daPy_py_c*)dComIfGp_getPlayer(0))->offPushPullKeep();
        fopAcM_seStart(this, Z2SE_OBJ_L6_RL_SW_OFF, 0);

        dComIfGp_getVibration().StartShock(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        init_modeWait();
    }
}

int daObjLv6SwTurn_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(unk5A8, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(unk5A8);
    dComIfGd_setList();
#if DEBUG
    if (l_HIO.mDrawFlag) {
        mpBgW->GetAttackThrough(0);
    }
#endif
    return TRUE;
}

int daObjLv6SwTurn_c::Delete() {
    dComIfG_resDelete(&unk5A0, l_arcName);
#if DEBUG
    l_HIO.removeHIO();
#endif
    return TRUE;
}

static int daObjLv6SwTurn_create1st(daObjLv6SwTurn_c* i_this) {
    fopAcM_ct(i_this, daObjLv6SwTurn_c);
    return i_this->create1st();
}

static int daObjLv6SwTurn_MoveBGDelete(daObjLv6SwTurn_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjLv6SwTurn_MoveBGExecute(daObjLv6SwTurn_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjLv6SwTurn_MoveBGDraw(daObjLv6SwTurn_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjLv6SwTurn_METHODS = {
    (process_method_func)daObjLv6SwTurn_create1st,
    (process_method_func)daObjLv6SwTurn_MoveBGDelete,
    (process_method_func)daObjLv6SwTurn_MoveBGExecute,
    NULL,
    (process_method_func)daObjLv6SwTurn_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Lv6SwTurn = {
    fpcLy_CURRENT_e,           // mLayerID
    3,                         // mListID
    fpcPi_CURRENT_e,           // mListPrio
    PROC_Obj_Lv6SwTurn,        // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daObjLv6SwTurn_c),  // mSize
    0,                         // mSizeOther
    0,                         // mParameters
    &g_fopAc_Method.base,      // sub_method
    15,                        // mPriority
    &daObjLv6SwTurn_METHODS,   // sub_method
    0x00040100,                // mStatus
    fopAc_ACTOR_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,    // cullType
};
