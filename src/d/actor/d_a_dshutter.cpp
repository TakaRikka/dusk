/**
 * d_a_dshutter.cpp
 * Gates that shutter open / close
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_dshutter.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"

static char* l_arcName[] = {
    "K_tetd",
    "S_bsaku00",
    "S_lv7saku",
};

daDsh_c::~daDsh_c() {}

static int const l_bmd[] = {
    4,
    4,
    4,
};

int daDsh_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mType], l_bmd[mType]);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daDsh_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daDsh_c::Execute(Mtx** param_0) {
    callExecute();
    setMtx();
    *param_0 = &mBgMtx;
    cLib_calcTimer(&mTiltTime);
    return 1;
}

void daDsh_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

void daDsh_c::setAction(daDsh_c::action_c* i_action) {
    mpAction = i_action;
    callInit();
}

int daDsh_c::callInit() {
    if (mpAction->getInit() == NULL) {
        return 1;
    }

    return (this->*mpAction->getInit())();
}

int daDsh_c::callExecute() {
    if (mpAction->getExecute() == NULL) {
        return 1;
    }

    return (this->*mpAction->getExecute())();
}

int daDsh_c::initOpenWait() {
    current.pos.y = home.pos.y;
    return 1;
}

static daDsh_c::action_c l_openWaitAction(&daDsh_c::initOpenWait, &daDsh_c::executeOpenWait);

static daDsh_c::action_c l_openAction(&daDsh_c::initOpen, &daDsh_c::executeOpen);

int daDsh_c::executeOpenWait() {
    if (fopAcM_isSwitch(this, mSw)) {
        setAction(&l_openAction);
    }

    return 1;
}

static int const l_dzb[] = {7, 7, 7};

static u32 const l_heap_size[] = {0xC10, 0xC10, 0x3D60};

int daDsh_c::initOpen() {
    mTiltTime = getOpenTiltTime();
    speed.y = 0.0f;
    gravity = getOpenAccel();

    cXyz check_pos(home.pos.x, home.pos.y + 100.0f, home.pos.z);
    bool water_check = fopAcM_wt_c::waterCheck(&check_pos);
    bool gnd_check = fopAcM_gc_c::gndCheck(&check_pos);

    if (water_check && fopAcM_wt_c::getWaterY() > fopAcM_gc_c::getGroundY()) {
        fopAcM_seStart(this, Z2SE_OBJ_SW_FENCE_DWN_WTR, 0);
    } else {
        fopAcM_seStart(this, Z2SE_OBJ_SW_FENCE_DOWN, 0);
    }

    return 1;
}

static daDsh_c::action_c l_closeWaitAction(&daDsh_c::initCloseWait, &daDsh_c::executeCloseWait);

int daDsh_c::executeOpen() {
    if (mTiltTime != 0) {
        current.pos.y = home.pos.y + cM_rndFX(2.0f);
    } else {
        cLib_chaseF(&speed.y, getOpenSpeed(), gravity);
        current.pos.y += speed.y;

        f32 closed_y_pos = home.pos.y - OPEN_SIZE;
        if (current.pos.y < closed_y_pos) {
            current.pos.y = closed_y_pos;

            if (speed.y < getOpenBoundSpeed()) {
                speed.y *= getOpenBoundRatio();
                gravity = 8.0f;
            } else {
                setAction(&l_closeWaitAction);
            }
        }
    }

    return 1;
}

int daDsh_c::initCloseWait() {
    current.pos.y = home.pos.y - OPEN_SIZE;
    return 1;
}

static daDsh_c::action_c l_closeAction(&daDsh_c::initClose, &daDsh_c::executeClose);

int daDsh_c::executeCloseWait() {
    if (!fopAcM_isSwitch(this, mSw)) {
        setAction(&l_closeAction);
    }

    return 1;
}

int daDsh_c::initClose() {
    mTiltTime = getCloseTiltTime();
    speed.y = 0.0f;
    gravity = getCloseAccel();

    return 1;
}

int daDsh_c::executeClose() {
    if (mTiltTime != 0) {
        current.pos.y = (home.pos.y - OPEN_SIZE) + cM_rndFX(2.0f);
    } else {
        cLib_chaseF(&speed.y, getCloseSpeed(), gravity);
        current.pos.y += speed.y;

        if (current.pos.y > home.pos.y) {
            current.pos.y = home.pos.y;

            if (speed.y > getCloseBoundSpeed()) {
                speed.y *= getCloseBoundRatio();
                gravity = 8.0f;
            } else {
                setAction(&l_openWaitAction);
            }
        }
    }

    return 1;
}

static int daDsh_Draw(daDsh_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daDsh_Execute(daDsh_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daDsh_IsDelete(daDsh_c* i_this) {
    return 1;
}

static int daDsh_Delete(daDsh_c* i_this) {
    int ret = i_this->MoveBGDelete();
    i_this->~daDsh_c();
    return ret;
}

AUDIO_INSTANCES;

int daDsh_c::create() {
    fopAcM_ct(this, daDsh_c);

    mType = getType();

    static const char* l_resName[] = {l_arcName[mType], ""};

    int phase = mResLoader.load(l_resName, NULL);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName[mType], l_dzb[mType], dBgS_MoveBGProc_Typical,
                             l_heap_size[mType], NULL);
        if (phase == cPhs_ERROR_e) {
            phase = cPhs_ERROR_e;
        } else {
            mSw = getSw();

            setAction(dComIfGs_isSwitch(mSw, fopAcM_GetHomeRoomNo(this)) ? &l_closeWaitAction : &l_openWaitAction);
            setMtx();
            fopAcM_SetMtx(this, model->getBaseTRMtx());
        }
    }

    return phase;
}

static int daDsh_Create(fopAc_ac_c* i_this) {
    return static_cast<daDsh_c*>(i_this)->create();
}

static actor_method_class l_daDsh_Method = {
    (process_method_func)daDsh_Create,  (process_method_func)daDsh_Delete,
    (process_method_func)daDsh_Execute, (process_method_func)daDsh_IsDelete,
    (process_method_func)daDsh_Draw,
};

actor_process_profile_definition2 g_profile_DSHUTTER = {
    {
        fpcLy_CURRENT_e,        // mLayerID
        7,                      // mListID
        fpcPi_CURRENT_e,        // mListPrio
        PROC_DSHUTTER,          // mProcName
        &g_fpcLf_Method.base,  // sub_method
        sizeof(daDsh_c),        // mSize
        0,                      // mSizeOther
        0,                      // mParameters
        &g_fopAc_Method.base,   // sub_method
        296,                    // mPriority
        &l_daDsh_Method,        // sub_method
        0x44000,                // mStatus
        fopAc_ACTOR_e,          // mActorType
        fopAc_CULLBOX_0_e,      // cullType
    },
    0,
};
