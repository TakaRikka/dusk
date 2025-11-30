/**
 * @file d_a_obj_lv4sand.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4sand.h"
#include "d/d_s_play.h"

static char* l_arcName = "P_L4Sand";

void daObjLv4Sand_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjLv4Sand_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mHeight, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjLv4Sand_c::Create() {
    if (fopAcM_isSwitch(this, getSwbit()) || dComIfGs_isStageBossEnemy() || BREG_S(9) != 0) {
        mHeight = -3500.0f;
        mode_init_dead();
        fopAcM_onSwitch(this, getSwbit());
    } else {
        mHeight = 0.0f;
        mode_init_wait();
    }

    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    return 1;
}

int daObjLv4Sand_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(167, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, J3D_DIFF_FLAG(FALSE, FALSE, TRUE, 8, 2, FALSE, 0, 0, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE));
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 8);
    JUT_ASSERT(181, pbtk != NULL);

    mpBtk = new mDoExt_btkAnm();
    if (mpBtk == NULL || !mpBtk->init(modelData, pbtk, TRUE, 2, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

int daObjLv4Sand_c::create1st() {
    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(l_arcName, 11, dBgS_MoveBGProc_TypicalRotY, 0x4000, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daObjLv4Sand_c::Execute(Mtx** param_0) {
    action();
    *param_0 = &mBgMtx;

    if (fopAcM_isSwitch(this, 234)) {
        mpBtk->play();
        if (std::fabs(mHeight) < 3300.0f) {
            eyePos = current.pos;
            eyePos.y += mHeight;
            fopAcM_seStartLevel(this, Z2SE_ENV_DS_SAND, 0);
        }
    }

    setBaseMtx();
    return 1;
}

void daObjLv4Sand_c::action() {
    static void (daObjLv4Sand_c::*l_func[])() = {
        &daObjLv4Sand_c::mode_wait,
        &daObjLv4Sand_c::mode_move,
        &daObjLv4Sand_c::mode_dead,
    };

    (this->*l_func[mMode])();
}

void daObjLv4Sand_c::mode_init_wait() {
    mMode = MODE_WAIT;
}

void daObjLv4Sand_c::mode_wait() {
    if (fopAcM_isSwitch(this, getSwbit())) {
        mode_init_move();
    }
}

void daObjLv4Sand_c::mode_init_move() {
    speed.y = 0.0f;
    mMode = MODE_MOVE;
}

void daObjLv4Sand_c::mode_move() {
    cLib_chaseF(&speed.y, 8.5f + KREG_F(1), 0.3f + KREG_F(2));
    if (cLib_chaseF(&mHeight, -3500.0f, speed.y)) {
        mode_init_dead();
    }
}

void daObjLv4Sand_c::mode_init_dead() {
    mMode = MODE_DEAD;
}

void daObjLv4Sand_c::mode_dead() {}

bool daObjLv4Sand_c::eventStart() {
    mode_init_move();
    return true;
}

int daObjLv4Sand_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mpBtk->entry(mpModel->getModelData());

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjLv4Sand_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjLv4Sand_create1st(daObjLv4Sand_c* i_this) {
    fopAcM_ct(i_this, daObjLv4Sand_c);
    return i_this->create1st();
}

static int daObjLv4Sand_MoveBGDelete(daObjLv4Sand_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjLv4Sand_MoveBGExecute(daObjLv4Sand_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjLv4Sand_MoveBGDraw(daObjLv4Sand_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjLv4Sand_METHODS = {
    (process_method_func)daObjLv4Sand_create1st,
    (process_method_func)daObjLv4Sand_MoveBGDelete,
    (process_method_func)daObjLv4Sand_MoveBGExecute,
    0,
    (process_method_func)daObjLv4Sand_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Lv4Sand = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv4Sand,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjLv4Sand_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  573,                    // mPriority
  &daObjLv4Sand_METHODS,  // sub_method
  0x00044100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
