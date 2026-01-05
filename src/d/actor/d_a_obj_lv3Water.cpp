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
#include "f_op/f_op_msg_mng.h"

class daLv3Water_HIO_c : public mDoHIO_entry_c {
public:
    daLv3Water_HIO_c();
    virtual ~daLv3Water_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x00 */ /* vtable */
    /* 0x04 */ u8 field_0x04;
};

static daLv3Water_HIO_c l_HIO;

static char* l_resNameIdx[] = {
    "Kr10water", "Kr10wat01", "Kr02wat00", "Kr03wat00", "Kr03wat01", "Kr03wat02", "Kr03wat03",
    "Kr03wat04", "Kr07wat00", "Kr08wat00", "Kr08wat01", "Kr02wat01", "Kr02wat02", "Kr02wat03",
    "Kr11wat00", "Kr12wat00", "Kr13wat00", "Kr13wat01", "Kr13wat02", "Kr03wat05", "Kr03wat06",
};

static daLv3Water_c::modeFunc l_mode_func[] = {
    &daLv3Water_c::mode_proc_wait,
    &daLv3Water_c::mode_proc_levelCtrl,
};

daLv3Water_HIO_c::daLv3Water_HIO_c() {
    field_0x04 = NULL;
}

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

int daLv3Water_c::CreateHeap() {
    J3DModelData* modelData =
        (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mType], l_bmdIdx[mType]);

    mpModel1 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);

    if (mpModel1 == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* btk =
        (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_resNameIdx[mType], l_btkIdx[mType]);
    mBtk1.init(modelData, btk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);

    if (l_bmdIdrIdx[mType] != -1) {
        J3DModelData* modelData =
            (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mType], l_bmdIdrIdx[mType]);

        mpModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x19000284);

        if (mpModel2 == NULL) {
            return 0;
        }

        J3DAnmTextureSRTKey* btk =
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_resNameIdx[mType], l_btkIdrIdx[mType]);
        mBtk2.init(modelData, btk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
    } else {
        mpModel2 = NULL;
    }

    return 1;
}

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
    /* padding */
    0x0000,
    0x0000,
    0x0000,
};

int daLv3Water_c::create() {
    fopAcM_ct(this, daLv3Water_c);

    mType = getParamType();

    int phase = dComIfG_resLoad(&mPhase, l_resNameIdx[mType]);

    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate(l_resNameIdx[mType], l_dzbIdx[mType], NULL, estimateSizeTbl[mType],
                         NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        }

        field_0x5fc = getParam();
        mSwitch1 = dComIfGs_isSwitch(getParamSw(), fopAcM_GetHomeRoomNo(this));
        field_0x605 = 1;

        if (mType == 1 || mType == 10 || mType == 15 || mType == 17 || mType == 19 || mType == 20) {
            if (mSwitch1 == 0) {
                if (mpBgW != NULL) {
                    dComIfG_Bgsp().Release(mpBgW);
                }
                field_0x605 = 0;
            } else if (mType == 1) {
                current.pos.y = home.pos.y + field_0x5fc;
            }
        } else if (mSwitch1 != 0) {
            current.pos.y = home.pos.y + field_0x5fc;
        }

        fopAcM_SetMtx(this, mpModel1->getBaseTRMtx());
        setBaseMtx();
        mMode = WAIT;
    }

    return phase;
}

int daLv3Water_c::Execute(Mtx** i_mtx) {
    mBtk1.play();

    if (mpModel2 != NULL) {
        mBtk2.play();
    }

    eventUpdate();
    mSwitch2 = dComIfGs_isSwitch(getParamSw(), fopAcM_GetHomeRoomNo(this));
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
        if (mSwitch1) {
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
        if (mSwitch1) {
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
        if (mSwitch1) {
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
    if (mSwitch1 != mSwitch2) {
        int event = getParamEvent();
        if (event != 0xFF) {
            orderEvent(event, 0xFF, 1);
        } else {
            eventStart();
        }
    }
}

void daLv3Water_c::mode_init_levelCtrl() {
    field_0x600 = 0;
    field_0x604 = l_HIO.field_0x04;

    if (mType == 9) {
        fopAcM_seStartCurrent(this, Z2SE_ENV_FILL_UP_LV3WTR3, 0);
    }

    mMode = LEVEL_CTRL;
}

void daLv3Water_c::mode_proc_levelCtrl() {
    if (field_0x604 != NULL) {
        field_0x604 = field_0x604 - 1;
    } else {
        f32 fVar = fopMsgM_valueIncrease(field_0x601, field_0x600, mSwitch1);

        if (mSwitch1 == 0) {
            fVar = 1.0f - fVar;
        }

        field_0x600 = field_0x600 + 1;
        if (field_0x600 >= field_0x601) {
            fVar = mSwitch1;
            mMode = WAIT;
        }

        current.pos.y = home.pos.y + field_0x5fc * fVar;
    }
}

int daLv3Water_c::Draw() {
    if (field_0x605 == NULL) {
        return 0;
    }

    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel1, &tevStr);

    if (mpModel2 != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpModel2, &tevStr);
    }

    mBtk1.entry(mpModel1->getModelData());

    if (mpModel2 != NULL) {
        mBtk2.entry(mpModel2->getModelData());
    }

    dComIfGd_setXluListDarkBG();
    mDoExt_modelUpdateDL(mpModel1);
    dComIfGd_setList();

    if (mpModel2 != NULL) {
        J3DModelData* modelData = mpModel2->getModelData();
        J3DMaterial* material = modelData->getMaterialNodePointer(0);

        dComIfGd_setListInvisisble();

        J3DTexMtx* texMtx = material->getTexGenBlock()->getTexMtx(0);
        if (texMtx != NULL) {
            J3DTexMtxInfo* texMtxInfo = &material->getTexGenBlock()->getTexMtx(0)->getTexMtxInfo();
            if (texMtxInfo != NULL) {
                Mtx lightProjMtx;
                C_MTXLightPerspective(lightProjMtx, dComIfGd_getView()->fovy,
                                      dComIfGd_getView()->aspect, 1.0f, 1.0f, -0.01f, 0.0f);
                texMtxInfo->setEffectMtx(lightProjMtx);
                modelData->simpleCalcMaterial(0, (MtxP)j3dDefaultMtx);
            }
        }

        mDoExt_modelUpdateDL(mpModel2);
        dComIfGd_setList();
    }

    return 1;
}

int daLv3Water_c::Delete() {
    dComIfG_resDelete(&mPhase, l_resNameIdx[mType]);
    return 1;
}

bool daLv3Water_c::eventStart() {
    field_0x601 = fpcM_GetParam(this) >> 0x18;
    mSwitch1 = mSwitch2;

    if (mType == 1 || mType == 10 || mType == 15 || mType == 17 || mType == 19 || mType == 20) {
        if (mpBgW != NULL) {
            dComIfG_Bgsp().Regist(mpBgW, this);
        }
        if (mType == 1) {
            current.pos.y = home.pos.y + field_0x5fc;
        }
        field_0x605 = mSwitch2;
        mMode = WAIT;
    } else {
        mode_init_levelCtrl();
    }

    return true;
}

static int daLv3Water_Draw(daLv3Water_c* i_this) {
    return i_this->Draw();
}

static int daLv3Water_Execute(daLv3Water_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv3Water_Delete(daLv3Water_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daLv3Water_Create(fopAc_ac_c* i_this) {
    return static_cast<daLv3Water_c*>(i_this)->create();
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
