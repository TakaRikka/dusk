/**
 * @file d_a_obj_lv3Water2.cpp
 *
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv3Water2.h"
#include "d/d_com_inf_game.h"
#include "f_ap/f_ap_game.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_graphic.h"

struct daLv3Water2_HIO_c : public mDoHIO_entry_c {
    daLv3Water2_HIO_c();
    ~daLv3Water2_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ u8 mLevelControlWaitFrames;
};

typedef void (daLv3Water2_c::*actionFunc)(void);

static int daLv3Water2_Draw(daLv3Water2_c* i_this);
static int daLv3Water2_Execute(daLv3Water2_c* i_this);
static int daLv3Water2_Delete(daLv3Water2_c* i_this);
static int daLv3Water2_Create(fopAc_ac_c* i_this);

static daLv3Water2_HIO_c l_HIO;

daLv3Water2_HIO_c::daLv3Water2_HIO_c() {
    mLevelControlWaitFrames = 0;
}

#if DEBUG
void daLv3Water2_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genSlider("wait time", &mLevelControlWaitFrames, 0, 255);
}
#endif

static char* l_resNameIdx[] = {"Kr03wat04"};

void daLv3Water2_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static const int l_bmdIdx[] = {5};

static const int l_dzbIdx[] = {13};

static const int l_btkIdx[] = {9};

int daLv3Water2_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameIdx[mResIdx], l_bmdIdx[mResIdx]));
    JUT_ASSERT(171, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, (1 << 19), 0x19000284);
    if(!mpModel)
        return 0;

    int res = mRefractionAnm.init(modelData, static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(l_resNameIdx[mResIdx], l_btkIdx[mResIdx])), 1, 2, 1.0f, 0, -1);
    JUT_ASSERT(188, res == 1);

    return 1;
}

cPhs_Step daLv3Water2_c::create() {
    fopAcM_ct(this, daLv3Water2_c);
    mResIdx = getParam(0, 4);

    cPhs_Step resPhase = static_cast<cPhs_Step>(dComIfG_resLoad(&mPhase, l_resNameIdx[mResIdx]));
    if(resPhase == cPhs_COMPLEATE_e) {
        if(MoveBGCreate(l_resNameIdx[mResIdx], l_dzbIdx[mResIdx], NULL, 0x2D00, NULL) == cPhs_ERROR_e)
            return cPhs_ERROR_e;

        mEastSwInitialState = fopAcM_isSwitch(this, getParamSw1());
        mWestSwInitialState = fopAcM_isSwitch(this, getParamSw2());

        setBaseMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

        if(mEastSwInitialState)
            current.pos.y = home.pos.y + getParamLevel1() * 5.0f;

        if(mWestSwInitialState)
            current.pos.y = home.pos.y + getParamLevel1() * 5.0f + getParamLevel1() * 5.0f;

        mMode = WAIT;

        // "LV3 Water surface (2SW)"
        #if DEBUG
        l_HIO.entryHIO("ＬＶ３水面(2SW)");
        #endif
    }

    return resPhase;
}

static actionFunc l_mode_func[] = {
    &daLv3Water2_c::mode_proc_wait, &daLv3Water2_c::mode_proc_levelCtrl
};

int daLv3Water2_c::Execute(Mtx** i_mtx) {
    mRefractionAnm.play();

    eventUpdate();

    mEastSwCurrentState = fopAcM_isSwitch(this, getParamSw1());
    mWestSwCurrentState = fopAcM_isSwitch(this, getParamSw2());

    (this->*l_mode_func[mMode])();

    if(fopAcM_isSwitch(this, 0xE)) {
        mEastParticles[0] = dComIfGp_particle_set(mEastParticles[0], 0x8AAC, &current.pos, NULL, NULL);
        mEastParticles[1] = dComIfGp_particle_set(mEastParticles[1], 0x8AAD, &current.pos, NULL, NULL);
        mEastParticles[2] = dComIfGp_particle_set(mEastParticles[2], 0x8AAE, &current.pos, NULL, NULL);
        mEastParticles[3] = dComIfGp_particle_set(mEastParticles[3], 0x8AAF, &current.pos, NULL, NULL);
    }

    if(fopAcM_isSwitch(this, 0xF)) {
        mWestParticles[0] = dComIfGp_particle_set(mWestParticles[0], 0x8AA8, &current.pos, NULL, NULL);
        mWestParticles[1] = dComIfGp_particle_set(mWestParticles[1], 0x8AA9, &current.pos, NULL, NULL);
        mWestParticles[2] = dComIfGp_particle_set(mWestParticles[2], 0x8AAA, &current.pos, NULL, NULL);
        mWestParticles[3] = dComIfGp_particle_set(mWestParticles[3], 0x8AAB, &current.pos, NULL, NULL);
    }

    *i_mtx = &mpModel->getBaseTRMtx();
    setBaseMtx();

    return 1;
}

void daLv3Water2_c::mode_proc_wait() {
    if(mEastSwInitialState != mEastSwCurrentState) {
        if(getParamEvent() != 0xFF)
            orderEvent(getParamEvent(), 0xFF, 1);
        else
            eventStart();
    }
    else if(mWestSwInitialState != mWestSwCurrentState) {
        if(getParamEvent2() != 0xFF)
            orderEvent(getParamEvent2(), 0xFF, 1);
        else
            eventStart();
    }
}

void daLv3Water2_c::mode_init_levelCtrl() {
    mCurrentWaterLvFrame = 0;
    mLvControlWaitFrames = l_HIO.mLevelControlWaitFrames;
    mBaseYPos = current.pos.y;
    mMode = LEVEL_CTRL;
}

void daLv3Water2_c::mode_proc_levelCtrl() {
    if(mLvControlWaitFrames) {
        mLvControlWaitFrames--;
        return;
    }

    f32 ratioOfWaterLevelFramesAdvancedToTarget = fopMsgM_valueIncrease(mWaterLvFrame, mCurrentWaterLvFrame, mEvActivatorSwState);

    if(!mEastSwInitialState) {
        // Water level should lower, invert the ratio so that it goes (1.0f -> 0.0f)
        ratioOfWaterLevelFramesAdvancedToTarget = 1.0f - ratioOfWaterLevelFramesAdvancedToTarget;
    }

    mCurrentWaterLvFrame++;

    if(mCurrentWaterLvFrame >= mWaterLvFrame) {
        ratioOfWaterLevelFramesAdvancedToTarget = mEvActivatorSwState;
        mMode = WAIT;
    }

    current.pos.y = mWaterLv * ratioOfWaterLevelFramesAdvancedToTarget + mBaseYPos;
}

int daLv3Water2_c::Draw() {
    J3DTexMtxInfo* texMtxInfo;
    J3DModelData* modelData;
    J3DMaterial* btkMaterial;

    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    modelData = mpModel->getModelData();
    mRefractionAnm.entry(modelData);
    btkMaterial = modelData->getMaterialNodePointer(0);

    dComIfGd_setListInvisisble();

    if(btkMaterial->getTexGenBlock()->getTexMtx(0)) {
        texMtxInfo = &btkMaterial->getTexGenBlock()->getTexMtx(0)->getTexMtxInfo();
        if(texMtxInfo) {
            Mtx lightProjMtx;
            C_MTXLightPerspective(lightProjMtx, dComIfGd_getView()->fovy, dComIfGd_getView()->aspect, 1.0f, 1.0f, -0.01f, 0);

            #if WIDESCREEN_SUPPORT
            mDoGph_gInf_c::setWideZoomLightProjection(lightProjMtx);
            #endif

            #if DEBUG
            if(fapGm_HIO_c::isCaptureScreen()) {
                Mtx44 screenCaptureMtx;
                MTXCopy(lightProjMtx, screenCaptureMtx);

                screenCaptureMtx[2][3] = -2.0f;
                CPerspDivider perspectiveDivider(screenCaptureMtx, fapGm_HIO_c::getCaptureScreenDivH(), fapGm_HIO_c::getCaptureScreenDivV());
                perspectiveDivider.divide(screenCaptureMtx, fapGm_HIO_c::getCaptureScreenNumH(), fapGm_HIO_c::getCaptureScreenNumV());
                screenCaptureMtx[2][3] = 0.0f;

                MTXCopy(screenCaptureMtx, lightProjMtx);
            }
            #endif

            texMtxInfo->setEffectMtx(lightProjMtx);
            modelData->simpleCalcMaterial(const_cast<MtxP>(j3dDefaultMtx));
        }
    }

    mDoExt_modelUpdateDL(mpModel);

    dComIfGd_setList();

    return 1;
}

int daLv3Water2_c::Delete() {
    dComIfG_resDelete(&mPhase, l_resNameIdx[mResIdx]);

    #if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

bool daLv3Water2_c::eventStart() {
    // Check which switch initiated the event
    if(mEastSwInitialState != mEastSwCurrentState) {
        mWaterLv = getParamLevel1() * 5.0f;
        mWaterLvFrame = static_cast<u8>(getParamFrame1());
        mEvActivatorSwState = mEastSwCurrentState;
        mEastSwInitialState = mEastSwCurrentState;

        mDoAud_seStart(Z2SE_ENV_FILL_UP_LV3WTR1, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
    else {
        mWaterLv = getParamLevel1() * 5.0f;
        mWaterLvFrame = static_cast<u8>(getParamFrame2());
        mEvActivatorSwState = mWestSwCurrentState;
        mWestSwInitialState = mWestSwCurrentState;

        mDoAud_seStart(Z2SE_ENV_FILL_UP_LV3WTR2, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    OS_REPORT("== mWaterLv %f mWaterLvFrame %d ==\n", mWaterLv, mWaterLvFrame);

    mode_init_levelCtrl();

    return true;
}

static int daLv3Water2_Draw(daLv3Water2_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv3Water2_Execute(daLv3Water2_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv3Water2_Delete(daLv3Water2_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "daLv3Water2");
    return i_this->MoveBGDelete();
}

static int daLv3Water2_Create(fopAc_ac_c* i_this) {
    daLv3Water2_c* const actor = static_cast<daLv3Water2_c*>(i_this);
    fopAcM_RegisterCreateID(i_this, "daLv3Water2");
    return actor->create();
}

static actor_method_class l_daLv3Water2_Method = {
    (process_method_func)daLv3Water2_Create,
    (process_method_func)daLv3Water2_Delete,
    (process_method_func)daLv3Water2_Execute,
    0,
    (process_method_func)daLv3Water2_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv3Water2 = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv3Water2,     // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daLv3Water2_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  617,                    // mPriority
  &l_daLv3Water2_Method,  // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
