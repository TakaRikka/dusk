/**
 * @file d_a_obj_lv7bridge.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv7bridge.h"
#include "d/actor/d_a_obj_swspinner.h"
#include "d/d_s_play.h"

#if DEBUG
class daObjLv7Brg_HIO_c : public JORReflexible {
public:
    daObjLv7Brg_HIO_c();
    virtual ~daObjLv7Brg_HIO_c() {}

    void genMessage(JORMContext*);

    u8 field_0x04[0x08 - 0x04];
    f32 field_0x08;
    f32 field_0x0c;
};

daObjLv7Brg_HIO_c::daObjLv7Brg_HIO_c() {
}

void daObjLv7Brg_HIO_c::genMessage(JORMContext* ctx) {
    // DEBUG NONMATCHING
}

static daObjLv7Brg_HIO_c l_HIO;
#endif

static void* searchSwSpinner(void* i_actor, void* i_data) {
    fopAc_ac_c* swspinner = (fopAc_ac_c*)i_actor;
    fopAc_ac_c* brg = (fopAc_ac_c*)i_data;

    if (swspinner != NULL && fopAcM_IsActor(swspinner) && fopAcM_GetProfName(swspinner) == PROC_Obj_SwSpinner) {
        if (((daObjLv7Brg_c*)brg)->getSwbit() == ((daObjSwSpinner_c*)swspinner)->getSwbit()) {
            return swspinner;
        }
    }

    return NULL;
}

daObjSwSpinner_c* daObjLv7Brg_c::search_swspinner() {
    daObjSwSpinner_c* swspinner = NULL;
    if (parentActorID == fpcM_ERROR_PROCESS_ID_e) {
        swspinner = (daObjSwSpinner_c*)fpcM_Search(searchSwSpinner, this);
        if (swspinner != NULL) {
            parentActorID = fopAcM_GetID(swspinner);
        }
    }

    return swspinner;
}

void daObjLv7Brg_c::initBaseMtx() {
    mpModel->setBaseScale(scale);

    if (mpBrokenModel != NULL) {
        mpBrokenModel->setBaseScale(scale);
    }

    setBaseMtx();
}

static const int l_bmd[] = {4, 10};

static const int l_dzb[] = {7, 13};

static const u16 l_eff_id[] = {
    dPa_RM(ID_ZI_S_LV7BRDG_BREAK_A),
    dPa_RM(ID_ZI_S_LV7BRDG_BREAK_B),
    dPa_RM(ID_ZI_S_LV7BRDG_BREAK_C),
    dPa_RM(ID_ZI_S_LV7BRDG_BREAK_D),
    dPa_RM(ID_ZI_S_LV7BRDG_BREAK_E),
    dPa_RM(ID_ZI_S_LV7BRDG_BREAK_F),
    dPa_RM(ID_ZI_S_LV7BRDG_BREAK_G),
    dPa_RM(ID_ZI_S_LV7BRDG_BREAK_H),
    dPa_RM(ID_ZI_S_LV7BRDG_BREAK_I),
};

static const u32 l_heap_size[] = {0x6000, 0x20000};

void daObjLv7Brg_c::setBaseMtx() {
    cXyz sp8(field_0xa78, 0.0f, 0.0f);
    if (mKind == 1) {
       sp8.set(-field_0xa78, 0.0f, 0.0f);
    }

    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVec(&sp8, &sp8);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(sp8);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

    if (mKind == 1) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::transM(sp8);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mpBrokenModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

static char* l_arcName[2] = {
    "Obj_l7brg",
    "K_bridge",
};

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x0, 0xd}, {0x0, 0x0}, 0x19}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0xd}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            100.0f, // mRadius
            250.0f // mHeight
        } // mCyl
    }
};

int daObjLv7Brg_c::Create() {
    field_0xa94 = -1;
    field_0xa95 = field_0xa94;

    if (mKind == 1) {
        if (fopAcM_isSwitch(this, getSwbit3())) {
            if (mpBgW != NULL && mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW);
            }

            if (mpBgw2 != NULL && !mpBgw2->ChkUsed()) {
                dComIfG_Bgsp().Regist(mpBgw2, this);
            }

            field_0xa94 = 1;
            mpBck[1]->setFrame(mpBck[1]->getEndFrame());
            field_0xa96 = 1;
            OS_REPORT("壊れる橋：破壊済みです！\n");
        }

        if (fopAcM_isSwitch(this, getSwbit2())) {
            field_0xa78 = 5100.0f;
            setMode(1);
            OS_REPORT("壊れる橋：移動済みです！\n");
        }
    } else if (mType == 0 && fopAcM_isSwitch(this, getSwbit2())) {
        field_0xa78 = 5100.0f;
        setMode(1);
    }

    fopAcM_setCullSizeFar(this, 100.0f);
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    mCcStts.Init(0xFF, 0xFF, this);

    for (int i = 0; i < 2; i++) {
        mCcCyl[i].Set(l_cyl_src);
        mCcCyl[i].SetStts(&mCcStts);
    }

    calcCoPos();
    MoveBGExecute();
    return 1;
}

int daObjLv7Brg_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mKind], l_bmd[mKind]);
    JUT_ASSERT(374, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    if (mKind == 1) {
        J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mKind], 9);
        JUT_ASSERT(374, modelData2 != NULL);
        mpBrokenModel = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11000084);
        if (mpBrokenModel == NULL) {
            return 0;
        }

        for (int i = 0; i < 2; i++) {
            static const int l_bck[] = {5, 6};

            J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName[mKind], l_bck[i]);
            JUT_ASSERT(411, pbck != NULL);

            mpBck[i] = new mDoExt_bckAnm();
            if (mpBck[i] == NULL || !mpBck[i]->init(pbck, 1, 0, 1.0f, 0, -1, 0)) {
                return 0;
            }
        }

        mpBgw2 = new dBgW();
        if (mpBgw2 != NULL && !mpBgw2->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName[mKind], 0xE), 1, &mBgMtx)) {
            mpBgw2->SetCrrFunc(dBgS_MoveBGProc_Typical);
        } else {
            mpBgw2 = NULL;
            return 0;
        }
    }

    return 1;
}

int daObjLv7Brg_c::create1st() {
    if (!mInitParams) {
        mInitParams = TRUE;
        field_0xa80 = home.angle.x;
        field_0xa82 = home.angle.z;
        shape_angle.x = 0;
        current.angle.x = 0;
        home.angle.x = 0;
        shape_angle.z = 0;
        current.angle.z = 0;
        home.angle.z = 0;
    }

    mType = getType();
    mKind = getKind();

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName[mKind]);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(l_arcName[mKind], l_dzb[mKind], dBgS_MoveBGProc_Typical, l_heap_size[mKind], NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daObjLv7Brg_c::Execute(Mtx** param_0) {
    search_swspinner();
    action();

    *param_0 = &mBgMtx;
    setBaseMtx();

    cXyz sp1C(2550.0f, 0.0f, 0.0f);
    if (mKind == 1) {
        sp1C.x = -sp1C.x;
    }

    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVec(&sp1C, &sp1C);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(sp1C);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVecZero(&eyePos);
    
    sp1C.set(7650.0f, 0.0f, 0.0f);
    if (mKind == 1) {
        sp1C.x = -sp1C.x;
    }

    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVec(&sp1C, &sp1C);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(sp1C);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVecZero(&field_0xa98);

    if (field_0xa94 == 0) {
        mpBck[0]->setPlaySpeed(1.0f);
        mpBck[0]->play();

        if (field_0xa95 == -1) {
            cXyz effpos(-field_0xa78, 0.0f, 0.0f);
            mDoMtx_stack_c::YrotS(shape_angle.y);
            mDoMtx_stack_c::multVec(&effpos, &effpos);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::transM(effpos);
            mDoMtx_stack_c::YrotM(shape_angle.y);
            mDoMtx_stack_c::multVecZero(&effpos);

            for (int i = 0; i < 9; i++) {
                dComIfGp_particle_set(l_eff_id[i], &effpos, &home.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
            }
        }
    } else if (field_0xa94 == 1) {
        mpBck[1]->setPlaySpeed(1.0f);
        if (mpBck[1]->play()) {
            if (mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW);
            }

            if (mpBgw2 != NULL && !mpBgw2->ChkUsed()) {
                dComIfG_Bgsp().Regist(mpBgw2, this);
            }

            field_0xa96 = 1;
        }
    }

    if (mpBgw2 != NULL && mpBgw2->ChkUsed()) {
        mpBgw2->Move();
    }

    field_0xa95 = field_0xa94;
    return 1;
}

void daObjLv7Brg_c::action() {
    static void (daObjLv7Brg_c::*mode_proc[])() = {
        &daObjLv7Brg_c::mode_action,
        &daObjLv7Brg_c::mode_end,
    };

    (this->*mode_proc[mMode])();

    if (speedF < 0.0f || KREG_S(7) != 0) {
        dComIfG_Ccsp()->Set(&mCcCyl[0]);
        dComIfG_Ccsp()->Set(&mCcCyl[1]);
    }
}

void daObjLv7Brg_c::mode_action() {
    f32 var_f31;
    f32 temp_f30 = field_0xa78;
    f32 temp_f29 = speedF;

    if (fopAcM_isSwitch(this, getSwbit())) {
        if (5100.0f == field_0xa78) {
            var_f31 = 0.0f;
        } else {
#if DEBUG
            var_f31 = l_HIO.field_0x08;
#else
            var_f31 = 30.0f;
#endif
        }
    } else if (0.0f == field_0xa78) {
        var_f31 = 0.0f;
    } else {
#if DEBUG
        var_f31 = -l_HIO.field_0x0c;
#else
        var_f31 = -45.0f;
#endif
    }

    cLib_addCalc(&speedF, var_f31, 0.02f, 1.0f, 0.1f);
    field_0xa78 += speedF;

    if (0.0f != speedF) {
#if DEBUG
        u32 var_r28 = 127.0f * speedF / l_HIO.field_0x08;
#else
        u32 var_r28 = 127.0f * speedF / 30.0f;
#endif
        fopAcM_seStartLevel(this, 0x8025F, var_r28);
    }

    if (0.0f != speedF && 0.0f == temp_f29) {
        dComIfGp_getVibration().StartQuake(2, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    }

    if (field_0xa78 < 0.0f) {
        field_0xa78 = 0.0f;
        speedF = 0.0f;
        dComIfGp_getVibration().StopQuake(0x1F);

        if (temp_f30 > 0.0f) {
            dComIfGp_getVibration().StartShock(8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        }
    } else if (field_0xa78 > 5100.0f) {
        field_0xa78 = 5100.0f;
        speedF = 0.0f;
        dComIfGp_getVibration().StopQuake(0x1F);

        if (temp_f30 < 5100.0f) {
            mDoAud_seStart(Z2SE_OBJ_BRDG_STOP, &field_0xa98, 0, 0);
            dComIfGp_getVibration().StartShock(8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            if (mType == 0) {
                fopAcM_onSwitch(this, getSwbit2());
                setMode(1);
            }
        }
    }
}

void daObjLv7Brg_c::mode_end() {}

void daObjLv7Brg_c::calcCoPos() {
    cXyz sp8[2];
    sp8[0].set(2700.0f, -80.0f, 450.0f);
    sp8[1].set(2700.0f, -80.0f, -450.0f);

    if (mKind == 1) {
        sp8[0].set(-2700.0f, -80.0f, 450.0f);
        sp8[1].set(-2700.0f, -80.0f, -450.0f);
    }

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    for (int i = 0; i < 2; i++) {
        mDoMtx_stack_c::multVec(&sp8[i], &sp8[i]);
    }

    mCcCyl[0].SetC(sp8[0]);
    mCcCyl[1].SetC(sp8[1]);
}

int daObjLv7Brg_c::Draw() {
    if (0.0f == field_0xa78) {
        return 1;
    }

    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);

    if (mKind == 1) {
        g_env_light.setLightTevColorType_MAJI(mpBrokenModel, &tevStr);
        if (mpBck[0] != NULL && mpBck[1] != NULL && field_0xa96 == 0) {
            if (field_0xa94 != -1) {
                mpBck[field_0xa94]->entry(mpBrokenModel->getModelData());
            }
            mDoExt_modelUpdateDL(mpBrokenModel);
            mDoExt_bckAnmRemove(mpBrokenModel->getModelData());
        }
    }

    dComIfGd_setList();
    return 1;
}

int daObjLv7Brg_c::Delete() {
    if (mpBgw2 != NULL && mpBgw2->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgw2);
    }

    dComIfG_resDelete(&mPhase, l_arcName[mKind]);
    return 1;
}

static int daObjLv7Brg_create1st(daObjLv7Brg_c* i_this) {
    fopAcM_ct(i_this, daObjLv7Brg_c);
    return i_this->create1st();
}

static int daObjLv7Brg_MoveBGDelete(daObjLv7Brg_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjLv7Brg_MoveBGExecute(daObjLv7Brg_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjLv7Brg_MoveBGDraw(daObjLv7Brg_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjLv7Brg_METHODS = {
    (process_method_func)daObjLv7Brg_create1st,
    (process_method_func)daObjLv7Brg_MoveBGDelete,
    (process_method_func)daObjLv7Brg_MoveBGExecute,
    0,
    (process_method_func)daObjLv7Brg_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Lv7Bridge = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv7Bridge,     // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjLv7Brg_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  566,                    // mPriority
  &daObjLv7Brg_METHODS,   // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
