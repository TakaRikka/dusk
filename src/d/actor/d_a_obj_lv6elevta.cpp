/**
 * @file d_a_obj_lv6elevta.cpp
 *
 */
 
#include "d/dolzel_rel.h"  // IWYU pragma: keep
#include "JSystem/JUtility/JUTAssert.h"
#include "d/actor/d_a_obj_lv6swturn.h"

#include "d/actor/d_a_obj_lv6elevta.h"

//
// Declarations:
//
static char* l_arcName = "Lv6ElevtA";
static const int l_dzbidx = {7};
static const int l_bmdidx = {4};

#if DEBUG
daObjLv6ElevtA_HIO_c::daObjLv6ElevtA_HIO_c() {
    mRightAngleTurnFrameCount = 150;
}

void daObjLv6ElevtA_HIO_c::genMessage(JORMContext* context) {
    context->genLabel("Lv6回転エレベータ", 0, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("90度回転フレーム数", &mRightAngleTurnFrameCount, 0, 1000, 0, NULL, -1, -1,
                       0x200, 0x18);
}

static daObjLv6ElevtA_HIO_c l_HIO;
#endif

int daObjLv6ElevtA_c::create1st() {
    int phase = dComIfG_resLoad(this, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        setMtx();
        phase = MoveBGCreate(l_arcName, l_dzbidx, dBgS_MoveBGProc_TypicalRotY, 0x55b0, &mMtx1);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
#if DEBUG
        l_HIO.entryHIO("Lv6回転エレベータ");
#endif
    }
    return phase;
}

void daObjLv6ElevtA_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    MTXCopy(mDoMtx_stack_c::get(), mMtx2);
    MTXCopy(mDoMtx_stack_c::get(), mMtx1);
}

int daObjLv6ElevtA_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmdidx);
    JUT_ASSERT(145, model_data != NULL);
    mModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mModel == NULL) {
        return FALSE;
    }
    return TRUE;
}

static const cull_box l_cull_box = {
    -393.13f, -3000.0f, -750.0f,
    393.13f, 150.0f, 393.13f
};

int daObjLv6ElevtA_c::Create() {
    mModel->setBaseTRMtx(mMtx2);
    fopAcM_SetMtx(this, mMtx2);
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z,
                          l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);
    if (getSwNo() == 0xff || getSw2No() == 0xff) {
        mAngle = 0;
    } else if (fopAcM_isSwitch(this, getSwNo())) {
        mAngle = 0x3fff;
    } else if (fopAcM_isSwitch(this, getSw2No())) {
        mAngle = -0x3fff;
    } else {
        mAngle = 0;
    }
    mMode = 0;
    mIsMoving = false;
    return TRUE;
}

static void* searchObjLv6SwTurn(void* i_turn, void* i_elevta) {
    if (i_turn != NULL && fopAcM_IsActor(i_turn) &&
        fopAcM_GetProfName(i_turn) == PROC_Obj_Lv6SwTurn)
    {
        daObjLv6SwTurn_c* this_turn = (daObjLv6SwTurn_c*)i_turn;
        daObjLv6ElevtA_c* this_elevta = (daObjLv6ElevtA_c*)i_elevta;
        if (this_turn->getSwbit() == this_elevta->getSwNo() &&
            this_turn->getSwbit2() == this_elevta->getSw2No())
        {
            return i_turn;
        }
    }
    return NULL;
}

bool daObjLv6ElevtA_c::eventStart() {
    if (fopAcM_isSwitch(this, getSwNo())) {
        mMode = 2;
    } else if (fopAcM_isSwitch(this, getSw2No())) {
        mMode = 3;
    } else {
        mMode = 1;
    }
    return true;
}

void daObjLv6ElevtA_c::moveAngle(void* i_FoundActor) {
    fopAc_ac_c* found = (fopAc_ac_c*)i_FoundActor;
    if (mAngle > 0) {
        f32 v = (mAngle / (f32)0x3fff) * 1500.0f;
        current.pos.y = home.pos.y - v;
        current.angle.y = home.angle.y + mAngle;
        found->current.pos.y = found->home.pos.y - v;
        found->current.angle.y = found->home.angle.y + mAngle;
    } else {
        f32 v = (mAngle / (f32)0x3fff) * 800.0f;
        current.pos.y = home.pos.y - v;
        current.angle.y = home.angle.y + mAngle;
        found->current.pos.y = found->home.pos.y - v;
        found->current.angle.y = found->home.angle.y + mAngle;
    }
    shape_angle = current.angle;
    mIsMoving = true;
    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_L6_RL_FLR_MV, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f,
                                  -1.0f, 0);
}

int daObjLv6ElevtA_c::Execute(Mtx** i_pMtx) {
    fopAc_ac_c* found = fopAcM_Search(searchObjLv6SwTurn, this);
    if (found == NULL || getSwNo() == 0xff || getSw2No() == 0xff) {
        mAngle = 0;
        setMtx();
        mModel->setBaseTRMtx(mMtx2);
        *i_pMtx = &mMtx1;
        return TRUE;
    }

    if (!eventUpdate()) {
        if (fopAcM_isSwitch(this, getSwNo())) {
            if (mAngle == 0) {
                if (getEvent() != 0xff) {
                    orderEvent(getEvent(), 0xff, 1);
                } else {
                    mMode = 2;
                }
            }
        } else if (fopAcM_isSwitch(this, getSw2No())) {
            if (mAngle == 0) {
                if (getEvent() != 0xff) {
                    orderEvent(getEvent(), 0xff, 1);
                } else {
                    mMode = 3;
                }
            }
        } else {
            if (mAngle != 0) {
                if (getEvent() != 0xff) {
                    orderEvent(getEvent(), 0xff, 1);
                } else {
                    mMode = 1;
                }
            }
        }
    }

    if (mMode == 2) {
#if DEBUG
        mAngle += (s16)((f32)0x3fff / l_HIO.mRightAngleTurnFrameCount);
#else
        mAngle += (s16)((f32)0x3fff / 150);
#endif
        if (mAngle > 0x3fff) {
            mAngle = 0x3fff;
            mMode = 0;
        }
        moveAngle(found);
    } else if (mMode == 3) {
#if DEBUG
        mAngle -= (s16)((f32)0x3fff / l_HIO.mRightAngleTurnFrameCount);
#else
        mAngle -= (s16)((f32)0x3fff / 150);
#endif
        if (mAngle < -0x3fff) {
            mAngle = -0x3fff;
            mMode = 0;
        }
        moveAngle(found);
    } else if (mMode == 1) {
        if (mAngle > 0) {
#if DEBUG
            mAngle -= (s16)((f32)0x3fff / l_HIO.mRightAngleTurnFrameCount);
#else
            mAngle -= (s16)((f32)0x3fff / 150);
#endif
            if (mAngle < 0) {
                mAngle = 0;
                mMode = 0;
            }
            moveAngle(found);

        } else if (mAngle < 0) {
#if DEBUG
            mAngle += (s16)((f32)0x3fff / l_HIO.mRightAngleTurnFrameCount);
#else
            mAngle += (s16)((f32)0x3fff / 150);
#endif
            if (mAngle > 0) {
                mAngle = 0;
                mMode = 0;
            }
            moveAngle(found);
        }
    } else if (mIsMoving) {
        Z2GetAudioMgr()->seStart(Z2SE_OBJ_L6_RL_FLR_ST, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f,
                                 -1.0f, 0);
        mIsMoving = false;
    }

    setMtx();
    mModel->setBaseTRMtx(mMtx2);
    *i_pMtx = &mMtx1;
    return TRUE;
}

int daObjLv6ElevtA_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return TRUE;
}

int daObjLv6ElevtA_c::Delete() {
    dComIfG_resDelete(this, l_arcName);
#if DEBUG
    l_HIO.removeHIO();
#endif
    return TRUE;
}

static int daObjLv6ElevtA_create1st(daObjLv6ElevtA_c* i_this) {
    fopAcM_ct(i_this, daObjLv6ElevtA_c);
    return i_this->create1st();
}

static int daObjLv6ElevtA_MoveBGDelete(daObjLv6ElevtA_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjLv6ElevtA_MoveBGExecute(daObjLv6ElevtA_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjLv6ElevtA_MoveBGDraw(daObjLv6ElevtA_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjLv6ElevtA_METHODS = {
    (process_method_func)daObjLv6ElevtA_create1st,
    (process_method_func)daObjLv6ElevtA_MoveBGDelete,
    (process_method_func)daObjLv6ElevtA_MoveBGExecute,
    NULL,
    (process_method_func)daObjLv6ElevtA_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Lv6ElevtA = {
    fpcLy_CURRENT_e,           // mLayerID
    2,                         // mListID
    fpcPi_CURRENT_e,           // mListPrio
    PROC_Obj_Lv6ElevtA,        // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daObjLv6ElevtA_c),  // mSize
    0,                         // mSizeOther
    0,                         // mParameters
    &g_fopAc_Method.base,      // sub_method
    669,                       // mPriority
    &daObjLv6ElevtA_METHODS,   // sub_method
    0x00040100,                // mStatus
    fopAc_ACTOR_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,    // cullType
};
