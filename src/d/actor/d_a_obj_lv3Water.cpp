/**
* @file d_a_obj_lv3Water.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv3Water.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_msg_mng.h"

#if DEBUG
#include "f_ap/f_ap_game.h"
#include "m_Do/m_Do_graphic.h"
#include "CaptureScreen.h"
#endif

class daLv3Water_HIO_c : public mDoHIO_entry_c {
public:
    daLv3Water_HIO_c();
    virtual ~daLv3Water_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x00 */ /* vtable */
    /* 0x04 */ u8 mLevelControlWaitFrames;
};

daLv3Water_HIO_c::daLv3Water_HIO_c() {
    mLevelControlWaitFrames = NULL;
}

#if DEBUG
void daLv3Water_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genSlider("wait time", &mLevelControlWaitFrames, 0, 0xFF);
}
#endif

static daLv3Water_HIO_c l_HIO;

void daLv3Water_c::setBaseMtx() {
    if (mpModel2 != NULL) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);

        mpModel2->setBaseScale(scale);
        mpModel2->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    mpModel1->setBaseScale(scale);
    mpModel1->setBaseTRMtx(mDoMtx_stack_c::get());
}

static const int l_bmdIdx[] = {
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
};

static const int l_dzbIdx[] = {
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 11, 11,
};

static const int l_btkIdx[] = {
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 8,
};

static const int l_bmdIdrIdx[] = {
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, -1, -1,
};

static const int l_btkIdrIdx[] = {
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, -1, -1,
};

static char* l_resNameIdx[] = {
    "Kr10water", "Kr10wat01", "Kr02wat00", "Kr03wat00", "Kr03wat01", "Kr03wat02", "Kr03wat03",
    "Kr03wat04", "Kr07wat00", "Kr08wat00", "Kr08wat01", "Kr02wat01", "Kr02wat02", "Kr02wat03",
    "Kr11wat00", "Kr12wat00", "Kr13wat00", "Kr13wat01", "Kr13wat02", "Kr03wat05", "Kr03wat06",
};

int daLv3Water_c::CreateHeap() {
    J3DModelData* modelData =
        (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mType], l_bmdIdx[mType]);

    JUT_ASSERT(384, modelData != NULL);

    mpModel1 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);

    if (mpModel1 == NULL) {
        return 0;
    }

    int res = mBtk1.init(modelData, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_resNameIdx[mType], l_btkIdx[mType]),
                         TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);

    JUT_ASSERT(400, res == 1);

    if (l_bmdIdrIdx[mType] != -1) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mType], l_bmdIdrIdx[mType]);

        JUT_ASSERT(404, modelData != NULL);

        mpModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x19000284);

        if (mpModel2 == NULL) {
            return 0;
        }

        res = mBtk2.init(modelData, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_resNameIdx[mType], l_btkIdrIdx[mType]),
                         TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
    } else {
        mpModel2 = NULL;
    }

    return 1;
}

int daLv3Water_c::create() {
    static u16 const estimateSizeTbl[] = {
        0x1CE0,
        0x4F90,
        0x1C80,
        0x1C30,
        0x1C30,
        0x1C30,
        0x1C30,
        0x8000,
        0x27D0,
        0x2490,
        0x1E60,
        0x1C80,
        0x1C80,
        0x1C80,
        0x4F90,
        0x2880,
        0x29D0,
        0x2B00,
        0x2040,
        0x1360,
        0x1360,
    };

    fopAcM_ct(this, daLv3Water_c);

    mType = getParamType();

    int phase = dComIfG_resLoad(&mPhase, l_resNameIdx[mType]);

    if (phase == cPhs_COMPLEATE_e) {
        u32 heapSize = estimateSizeTbl[mType];
        OS_REPORT("l3water: == type:%d [0x%04x:%s] ==\n", mType, heapSize, l_resNameIdx[mType]);

        if (MoveBGCreate(l_resNameIdx[mType], l_dzbIdx[mType], NULL, heapSize,
                         NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        }

        mWaterLv = getParam(12, 12);
        mSwInitialState = fopAcM_isSwitch(this, getParam(0, 8));
        mSwStatePostEvent = true;

        if (mType == 1 || mType == 10 || mType == 15 || mType == 17 || mType == 19 || mType == 20) {
            if (!mSwInitialState) {
                if (mpBgW != NULL) {
                    dComIfG_Bgsp().Release(mpBgW);
                }
                mSwStatePostEvent = false; // Water is not raised upon creation, ensure it is drawn accordingly
            } else if (mType == 1) {
                current.pos.y = home.pos.y + mWaterLv;
            }
        } else if (mSwInitialState) {
            current.pos.y = home.pos.y + mWaterLv;
        }

        fopAcM_SetMtx(this, mpModel1->getBaseTRMtx());
        setBaseMtx();
        mMode = WAIT;

        #if DEBUG
        l_HIO.entryHIO("ＬＶ３水面"); // "LV3 water surface"
        #endif
    }

    return phase;
}

static daLv3Water_c::modeFunc l_mode_func[] = {
    &daLv3Water_c::mode_proc_wait,
    &daLv3Water_c::mode_proc_levelCtrl,
};

int daLv3Water_c::Execute(Mtx** i_mtx) {
    mBtk1.play();

    if (mpModel2 != NULL) {
        mBtk2.play();
    }

    eventUpdate();
    mSwCurrentState = fopAcM_isSwitch(this, getParam(0, 8));
    (this->*l_mode_func[mMode])();
    effectSet();
    *i_mtx = &mpModel1->getBaseTRMtx();
    setBaseMtx();

    return 1;
}

void daLv3Water_c::effectSet() {
    switch (mType) {
    case 8:
        if (fopAcM_isSwitch(this, 8)) {
            mEmitterIDs[0] =
                dComIfGp_particle_set(mEmitterIDs[0], 0X8AB2, &current.pos, NULL, NULL);
            mEmitterIDs[1] =
                dComIfGp_particle_set(mEmitterIDs[1], 0X8AB3, &current.pos, NULL, NULL);
        }
        break;
    case 9:
        if (mSwInitialState) {
            mEmitterIDs[0] =
                dComIfGp_particle_set(mEmitterIDs[0], 0X8AB4, &current.pos, NULL, NULL);
            mEmitterIDs[1] =
                dComIfGp_particle_set(mEmitterIDs[1], 0X8AB5, &current.pos, NULL, NULL);
            mEmitterIDs[2] =
                dComIfGp_particle_set(mEmitterIDs[2], 0X8AB6, &current.pos, NULL, NULL);
            mEmitterIDs[3] =
                dComIfGp_particle_set(mEmitterIDs[3], 0X8AB7, &current.pos, NULL, NULL);
        }
        break;
    case 1:
        if (fopAcM_isSwitch(this, 8)) {
            mEmitterIDs[0] =
                dComIfGp_particle_set(mEmitterIDs[0], 0x8AB9, &current.pos, NULL, NULL);
            mEmitterIDs[1] =
                dComIfGp_particle_set(mEmitterIDs[1], 0x8ABA, &current.pos, NULL, NULL);
            mEmitterIDs[2] =
                dComIfGp_particle_set(mEmitterIDs[2], 0x8ABB, &current.pos, NULL, NULL);
            mEmitterIDs[3] =
                dComIfGp_particle_set(mEmitterIDs[3], 0x8ABC, &current.pos, NULL, NULL);
            mEmitterIDs[4] =
                dComIfGp_particle_set(mEmitterIDs[4], 0x8AB8, &current.pos, NULL, NULL);
        }
        break;
    case 14:
        if (fopAcM_isSwitch(this, 9)) {
            mEmitterIDs[0] =
                dComIfGp_particle_set(mEmitterIDs[0], 0X8ABF, &current.pos, NULL, NULL);
            mEmitterIDs[1] =
                dComIfGp_particle_set(mEmitterIDs[1], 0X8AC0, &current.pos, NULL, NULL);
            mEmitterIDs[2] =
                dComIfGp_particle_set(mEmitterIDs[2], 0X8AC1, &current.pos, NULL, NULL);
            mEmitterIDs[3] =
                dComIfGp_particle_set(mEmitterIDs[3], 0X8AC2, &current.pos, NULL, NULL);
        }
        break;
    case 17:
        if (mSwInitialState) {
            mEmitterIDs[0] =
                dComIfGp_particle_set(mEmitterIDs[0], 0X8AC4, &current.pos, NULL, NULL);
            mEmitterIDs[1] =
                dComIfGp_particle_set(mEmitterIDs[1], 0X8AC5, &current.pos, NULL, NULL);
            mEmitterIDs[2] =
                dComIfGp_particle_set(mEmitterIDs[2], 0X8AC6, &current.pos, NULL, NULL);
            mEmitterIDs[3] =
                dComIfGp_particle_set(mEmitterIDs[3], 0X8AC7, &current.pos, NULL, NULL);
        }
        break;
    case 16:
        if (mSwInitialState) {
            mEmitterIDs[0] =
                dComIfGp_particle_set(mEmitterIDs[0], 0X8AC8, &current.pos, NULL, NULL);
            mEmitterIDs[1] =
                dComIfGp_particle_set(mEmitterIDs[1], 0X8AC9, &current.pos, NULL, NULL);
        }
        if (fopAcM_isSwitch(this, 9)) {
            mEmitterIDs[2] =
                dComIfGp_particle_set(mEmitterIDs[2], 0x8AC3, &current.pos, NULL, NULL);
        }
    }
}

void daLv3Water_c::mode_proc_wait() {
    // Only change the water level when the pull switch is activated (i.e. its initial state differs from its current state)
    if (mSwInitialState != mSwCurrentState) {
        if (getParamEvent() != 0xFF) {
            orderEvent(getParamEvent(), 0xFF, 1);
        } else {
            eventStart();
        }
    }
}

void daLv3Water_c::mode_init_levelCtrl() {
    mCurrentWaterLvFrame = 0;
    mLvControlWaitFrames = l_HIO.mLevelControlWaitFrames;

    if (mType == 9) {
        mDoAud_seStart(Z2SE_ENV_FILL_UP_LV3WTR3, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    mMode = LEVEL_CTRL;
}

void daLv3Water_c::mode_proc_levelCtrl() {
    f32 ratioOfWaterLevelFramesAdvancedToTarget;

    if (mLvControlWaitFrames != 0) {
        mLvControlWaitFrames--;
    } else {
        ratioOfWaterLevelFramesAdvancedToTarget = fopMsgM_valueIncrease(mWaterLvFrame, mCurrentWaterLvFrame, mSwInitialState);

        if (!mSwInitialState) {
            // Water level should lower, invert the ratio so that it goes (1.0f -> 0.0f)
            ratioOfWaterLevelFramesAdvancedToTarget = 1.0f - ratioOfWaterLevelFramesAdvancedToTarget;
        }

        mCurrentWaterLvFrame++;
        if (mCurrentWaterLvFrame >= mWaterLvFrame) {
            ratioOfWaterLevelFramesAdvancedToTarget = mSwInitialState;
            mMode = WAIT;
        }

        current.pos.y = mWaterLv * ratioOfWaterLevelFramesAdvancedToTarget + home.pos.y;
    }
}

int daLv3Water_c::Draw() {
    J3DTexMtxInfo* texMtxInfo;
    J3DMaterial* material;
    J3DModelData* modelData;

    // Only draw if the water is raised
    if (!mSwStatePostEvent)
        return 0;

    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel1, &tevStr);

    if (mpModel2 != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpModel2, &tevStr);
    }

    modelData = mpModel1->getModelData();
    mBtk1.entry(modelData);

    if (mpModel2 != NULL) {
        modelData = mpModel2->getModelData();
        mBtk2.entry(modelData);
    }

    dComIfGd_setXluListDarkBG();
    mDoExt_modelUpdateDL(mpModel1);
    dComIfGd_setList();

    if (mpModel2 != NULL) {
        modelData = mpModel2->getModelData();
        material = modelData->getMaterialNodePointer(0);

        dComIfGd_setListInvisisble();

        if (material->getTexGenBlock()->getTexMtx(0) != NULL) {
            texMtxInfo = &material->getTexGenBlock()->getTexMtx(0)->getTexMtxInfo();
            if (texMtxInfo != NULL) {
                Mtx lightProjMtx;
                C_MTXLightPerspective(lightProjMtx, dComIfGd_getView()->fovy,
                                      dComIfGd_getView()->aspect, 1.0f, 1.0f, -0.01f, 0.0f);

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
                modelData->simpleCalcMaterial((MtxP)j3dDefaultMtx);
            }
        }

        mDoExt_modelUpdateDL(mpModel2);
        dComIfGd_setList();
    }

    return 1;
}

int daLv3Water_c::Delete() {
    dComIfG_resDelete(&mPhase, l_resNameIdx[mType]);

    #if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

bool daLv3Water_c::eventStart() {
    mWaterLvFrame = getParam(24, 8);
    mSwInitialState = mSwCurrentState; // Ensure the water raise event isn't activated again

    if (mType == 1 || mType == 10 || mType == 15 || mType == 17 || mType == 19 || mType == 20) {
        if (mpBgW != NULL) {
            dComIfG_Bgsp().Regist(mpBgW, this);
        }
        if (mType == 1) {
            current.pos.y = home.pos.y + mWaterLv;
        }
        mSwStatePostEvent = mSwCurrentState;
        mMode = WAIT;
    } else {
        mode_init_levelCtrl();
    }

    return true;
}

static int daLv3Water_Draw(daLv3Water_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv3Water_Execute(daLv3Water_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv3Water_Delete(daLv3Water_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "daTreeSh"); //! @note Suggests copy-and-pasting from d_a_obj_treesh
    return i_this->MoveBGDelete();
}

static int daLv3Water_Create(fopAc_ac_c* i_this) {
    daLv3Water_c* const actor = static_cast<daLv3Water_c*>(i_this);
    fopAcM_RegisterCreateID(i_this, "daLv3Water");
    return actor->create();
}

static actor_method_class l_daLv3Water_Method = {
    (process_method_func)daLv3Water_Create,  (process_method_func)daLv3Water_Delete,
    (process_method_func)daLv3Water_Execute, 0,
    (process_method_func)daLv3Water_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv3Water = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Lv3Water,       // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daLv3Water_c),    // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    616,                     // mPriority
    &l_daLv3Water_Method,    // sub_method
    0x00044000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
