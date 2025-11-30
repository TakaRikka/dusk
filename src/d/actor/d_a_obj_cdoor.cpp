/**
 * d_a_obj_cdoor.cpp
 * Object - Sewer Gate / Sluice Gate
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_cdoor.h"
#include "d/actor/d_a_obj_wchain.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static char* l_arcName[2] = {
    "Cdoor",
    "J_Suimon",
};

static int const l_bmdIdx[2] = {4, 9};

static int const l_dzbIdx[2] = {7, 12};

static f32 l_moveOffsetY[2] = {250.0f, 230.0f};

static f32 const l_attentionOffsetY[2] = {75.0f, 75.0f};

int daObjCdoor_c::CreateHeap() {
    J3DModelData* model_data =
        (J3DModelData*)dComIfG_getObjectRes(l_arcName[mType], l_bmdIdx[mType]);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    if (mType == 1) {
        J3DAnmTransform* open_anm = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName[mType], 6);
        mOpenAnm.init(open_anm, 1, 0, 1.0f, 0, -1, false);
        J3DAnmTransform* close_anm = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName[mType], 5);
        mCloseAnm.init(close_anm, 1, 0, 1.0f, 0, -1, false);
    }
    return 1;
}

static void* daObjCdoor_searchChain(fopAc_ac_c* i_actor, void* i_this) {
    daObjCdoor_c* _this = static_cast<daObjCdoor_c*>(i_this);
    if (fopAcM_GetName(i_actor) == PROC_Obj_Wchain) {
        daObjWchain_c* chain = static_cast<daObjWchain_c*>(i_actor);
        if (chain->getSwitchNum() == _this->getSwitchNum()) {
            _this->setChainID(fopAcM_GetID(chain));
            return chain;
        }
    }
    return NULL;
}

cPhs__Step daObjCdoor_c::create() {
    fopAcM_ct(this, daObjCdoor_c);
    mType = (fopAcM_GetParam(this) >> 8) & 0xf;
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName[mType]);
    if (step == cPhs_COMPLEATE_e) {
        step = (cPhs__Step)MoveBGCreate(l_arcName[mType], l_dzbIdx[mType], NULL, 0xc00, NULL);
        if (step == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        tevStr.room_no = dStage_roomControl_c::mStayNo;
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mSw = fopAcM_GetParam(this) & 0xff;
        mChainID = -1;
        mEnd = false;
        if (mType == 1) {
            mIsOpen = fopAcM_isSwitch(this, mSw);
            mMapToolID = (fopAcM_GetParam(this) >> 0xc) & 0xff;
            mEventID = dComIfGp_getEventManager().getEventIdx(this, mMapToolID);
            setAction(ACT_WAIT);
            if (mMapToolID == 0xff) {
                setAction(ACT_DEAD);
            }
            if (mIsOpen) {
                mOpenAnm.setFrame(mOpenAnm.getEndFrame());
                mOpenAnm.play();
            } else {
                mCloseAnm.setFrame(mOpenAnm.getEndFrame());
                mCloseAnm.play();
            }
            init_modeWait();
        } else {
            if (fopAcM_isSwitch(this, mSw)) {
                mEnd = 1;
                current.pos.y += l_moveOffsetY[mType];
            } else {
                fopAcIt_Judge((fopAcIt_JudgeFunc)daObjCdoor_searchChain, this);
            }
        }
        attention_info.position.set(
            current.pos.x,
            current.pos.y + l_attentionOffsetY[mType],
            current.pos.z
        );
        eyePos = attention_info.position;
        setMatrix();
    }
    return step;
}

static cPhs__Step daObjCdoor_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjCdoor_c*>(i_this)->create();
}

daObjCdoor_c::~daObjCdoor_c() {
    dComIfG_resDelete(&mPhaseReq, l_arcName[mType]);
}

static int daObjCdoor_Delete(daObjCdoor_c* i_this) {
    i_this->MoveBGDelete();
    i_this->~daObjCdoor_c();
    return 1;
}

void daObjCdoor_c::setMatrix() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjCdoor_c::Execute(Mtx** i_mtxP) {
    *i_mtxP = &mpModel->getBaseTRMtx();
    switch (mType) {
        case 0:
            execCdoor();
            break;
        case 1:
            execWgate();
            break;
    }
    return 1;
}

void daObjCdoor_c::execCdoor() {
    if (!mEnd) {
        if (mChainID == -1) {
            fopAcIt_Judge((fopAcIt_JudgeFunc)daObjCdoor_searchChain, this);
        }
        if (mChainID != -1) {
            daObjWchain_c* chain = static_cast<daObjWchain_c*>(fopAcM_SearchByID(mChainID));
            if (chain != NULL) {
                if (chain->getEndFlg()) {
                    mChainID = -1;
                    mEnd = true;
                    current.pos.y = home.pos.y + l_moveOffsetY[mType];
                    if (mCamLock) {
                        dCam_getBody()->ForceLockOff(this);
                        mCamLock = false;
                    }
                    fopAcM_seStartCurrent(this, Z2SE_OBJ_OPEN_CHAIN_SW_DOOR, 0);
                } else {
                    current.pos.y = home.pos.y + l_moveOffsetY[mType] * chain->getMoveDisRate();
                    if (chain->checkRideFlg()) {
                        dCam_getBody()->ForceLockOn(this);
                        mCamLock = true;
                    } else if (mCamLock) {
                        dCam_getBody()->ForceLockOff(this);
                        mCamLock = false;
                    }
                }
                setMatrix();
            }
        }
    }
}

void daObjCdoor_c::execWgate() {
    typedef void (daObjCdoor_c::*daObjCdoor_modeFunc)();
    static daObjCdoor_modeFunc mode_proc[3] = {
        &daObjCdoor_c::modeWait,
        &daObjCdoor_c::modeOpen,
        &daObjCdoor_c::modeClose,
    };
    u8 was_open = mIsOpen;
    mIsOpen = fopAcM_isSwitch(this, mSw);
    if (mIsOpen != was_open) {
        if (mIsOpen) {
            init_modeOpen();
        } else {
            init_modeClose();
        }
        mDoAud_setHyrulSewerOpen(was_open);
    }
    (this->*mode_proc[mMode])();
    setMatrix();
    event_proc_call();
}

void daObjCdoor_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daObjCdoor_c::modeWait() {
    /* empty function */
}

void daObjCdoor_c::init_modeOpen() {
    mOpenAnm.reset();
    mMode = MODE_OPEN;
}

void daObjCdoor_c::modeOpen() {
    fopAcM_seStartCurrentLevel(this, Z2SE_OBJ_WATERGATE_MOVE, 0);
    if (mOpenAnm.play()) {
        fopAcM_seStartCurrent(this, Z2SE_OBJ_OPEN_CHAIN_SW_DOOR, 0);
        init_modeWait();
    }
}

void daObjCdoor_c::init_modeClose() {
    mCloseAnm.reset();
    mMode = MODE_CLOSE;
}

void daObjCdoor_c::modeClose() {
    fopAcM_seStartCurrentLevel(this, Z2SE_OBJ_WATERGATE_MOVE, 0);
    if (mCloseAnm.play()) {
        fopAcM_seStartCurrent(this, Z2SE_OBJ_OPEN_CHAIN_SW_DOOR, 0);
        init_modeWait();
    }
}

static int daObjCdoor_Execute(daObjCdoor_c* i_this) {
    return i_this->MoveBGExecute();
}

void daObjCdoor_c::event_proc_call() {
    typedef void (daObjCdoor_c::*daObjCdoor_eventFunc)();
    static daObjCdoor_eventFunc l_func[4] = {
        &daObjCdoor_c::actionWait,
        &daObjCdoor_c::actionOrderEvent,
        &daObjCdoor_c::actionEvent,
        &daObjCdoor_c::actionDead,
    };
    (this->*l_func[mAction])();
}

void daObjCdoor_c::actionWait() {
    if (mMapToolID != 0xff) {
        setAction(ACT_ORDER_EVENT);
        fopAcM_orderOtherEventId(this, mEventID, mMapToolID, 0xffff, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daObjCdoor_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACT_EVENT);
    } else {
        fopAcM_orderOtherEventId(this, mEventID, mMapToolID, 0xffff, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daObjCdoor_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventID)) {
        setAction(ACT_DEAD);
        dComIfGp_event_reset();
    }
}

void daObjCdoor_c::actionDead() {
    setAction(ACT_WAIT);
}

int daObjCdoor_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    if (mType == 1) {
        J3DModelData* model_data = mpModel->getModelData();
        if (mIsOpen) {
            mOpenAnm.entry(model_data);
        } else {
            mCloseAnm.entry(model_data);
        }
    }
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

static int daObjCdoor_Draw(daObjCdoor_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class l_daObjCdoor_Method = {
    (process_method_func)daObjCdoor_Create,
    (process_method_func)daObjCdoor_Delete,
    (process_method_func)daObjCdoor_Execute,
    (process_method_func)NULL,
    (process_method_func)daObjCdoor_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Cdoor = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Cdoor,
    &g_fpcLf_Method.base,
    sizeof(daObjCdoor_c),
    0,
    0,
    &g_fopAc_Method.base,
    0xF5,
    &l_daObjCdoor_Method,
    0x40100,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
