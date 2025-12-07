/**
 * @file d_a_obj_waterGate.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_waterGate.h"
#include "d/d_com_inf_game.h"

typedef void (daWtGate_c::*actionFunc)();

static int daWtGate_Draw(daWtGate_c*);
static int daWtGate_Execute(daWtGate_c*);
static int daWtGate_Delete(daWtGate_c*);
static int daWtGate_Create(fopAc_ac_c*);

static daWtGate_HIO_c l_HIO;

daWtGate_HIO_c::daWtGate_HIO_c() {
    mMaxSpeed = 4.0f;
    field_0x8 = 30;
    field_0x9 = 4;
}

#if DEBUG
void daWtGate_HIO_c::genMessage(JORMContext* ctx) {
    // "Maximum speed"
    ctx->genSlider("最大速度", &mMaxSpeed, 0.1, 500.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

void daWtGate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z); 
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daWtGate_c::CreateHeap() {
    J3DModelData* const modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("S_Zsuimon", 4));
    JUT_ASSERT(159, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    if(!mpModel)
        return 0;
    else
        return 1;
}

cPhs__Step daWtGate_c::create() {
    fopAcM_ct(this, daWtGate_c);

    const cPhs__Step resPhase = static_cast<cPhs__Step>(dComIfG_resLoad(&mPhase, "S_Zsuimon"));
    if(resPhase == cPhs_COMPLEATE_e) {
        if(MoveBGCreate("S_Zsuimon", 7, dBgS_MoveBGProc_TypicalRotY, 0xE00, NULL) == cPhs_ERROR_e)
            return cPhs_ERROR_e;
        
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mOpenYOffset = ((fopAcM_GetParam(this) & 0xFF00) >> 8) * 10.0f;

        if(static_cast<u8>(fopAcM_GetParam(this)) == 0xFF) {
            // Gate is never opened
            current.pos.y -= mOpenYOffset;
            init_modeEnd();
        }
        else {
            mPrevSwitchState = fopAcM_isSwitch(this, static_cast<u8>(fopAcM_GetParam(this)));
            mClosedYPos = current.pos.y;

            // Place gate in open position if it was previously opened
            if(mPrevSwitchState != CLOSED)
                current.pos.y += mOpenYOffset;

            init_modeWait();
        }

        setBaseMtx();


        // "Sluice gate(Lv3)"
        #if DEBUG
        l_HIO.entryHIO("水門(Lv3)");
        #endif
    }

    return resPhase;
}

int daWtGate_c::Execute(Mtx** i_mtx) {
    move();

    *i_mtx = &mpModel->getBaseTRMtx();
    setBaseMtx();

    return 1;
}

void daWtGate_c::move() {
    static const actionFunc mode_proc[] = {
        &daWtGate_c::modeWait, &daWtGate_c::modeOpen,
        &daWtGate_c::modeClose, &daWtGate_c::modeEnd
    };

    (this->*mode_proc[mMode])();
}

void daWtGate_c::init_modeWait() {
    mMode = WAIT;
}

void daWtGate_c::modeWait() {
    //  The state of the switch never actually changes in-game, so after a water gate opens,
    //  it never closes. The aesthetic functionality of closing still works, though
    u8 currentSwitchState = fopAcM_isSwitch(this, static_cast<u8>(fopAcM_GetParam(this))); 
    if(currentSwitchState != mPrevSwitchState) {
        if(currentSwitchState != CLOSED)
            init_modeOpen();
        else
            init_modeClose();

        mPrevSwitchState = currentSwitchState;
    }
}

void daWtGate_c::init_modeOpen() {
    mMaxMoveSpeed = l_HIO.mMaxSpeed;
    mMode = OPEN;
}

void daWtGate_c::modeOpen() {
    const f32 currentAndOpenedYPosDiff = cLib_addCalc(&current.pos.y, mClosedYPos + mOpenYOffset, 1.0f / 5.0f, mMaxMoveSpeed, 1.0f);
    if(currentAndOpenedYPosDiff == 0.0f) {
        fopAcM_seStartCurrent(this, Z2SE_OBJ_OPEN_CHAIN_SW_DOOR, 0);
        init_modeWait();
    }
    else {
        fopAcM_seStartCurrentLevel(this, Z2SE_OBJ_WATERGATE_MOVE, 0);
    }
}

void daWtGate_c::init_modeClose() {
    mMaxMoveSpeed = l_HIO.mMaxSpeed;
    mMode = CLOSE;
}

void daWtGate_c::modeClose() {
    const f32 currentAndClosedYPosDiff = cLib_addCalc(&current.pos.y, mClosedYPos, 1.0f / 5.0f, mMaxMoveSpeed, 1.0f);
    if(currentAndClosedYPosDiff == 0.0f) {
        fopAcM_seStartCurrent(this, Z2SE_OBJ_OPEN_CHAIN_SW_DOOR, 0);
        init_modeWait();
    }
    else {
        fopAcM_seStartCurrentLevel(this, Z2SE_OBJ_WATERGATE_MOVE, 0);
    }
}

void daWtGate_c::init_modeEnd() {
    mMode = END;
}

void daWtGate_c::modeEnd() {
    /* empty function */
}

int daWtGate_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return 1;
}

int daWtGate_c::Delete() {
    dComIfG_resDelete(&mPhase, "S_Zsuimon");

    #if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

static int daWtGate_Draw(daWtGate_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daWtGate_Execute(daWtGate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daWtGate_Delete(daWtGate_c* i_this) {
    const fpc_ProcID procID = fopAcM_GetID(i_this);
    return i_this->MoveBGDelete();
}

static int daWtGate_Create(fopAc_ac_c* i_this) {
    daWtGate_c* const waterGate = static_cast<daWtGate_c*>(i_this);
    const fpc_ProcID procID = fopAcM_GetID(i_this);
    return waterGate->create();
}

static actor_method_class l_daWtGate_Method = {
    (process_method_func)daWtGate_Create,
    (process_method_func)daWtGate_Delete,
    (process_method_func)daWtGate_Execute,
    0,
    (process_method_func)daWtGate_Draw,
};

extern actor_process_profile_definition g_profile_Obj_WtGate = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_WtGate,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daWtGate_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  522,                    // mPriority
  &l_daWtGate_Method,     // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
