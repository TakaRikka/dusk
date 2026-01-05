/**
 * @file d_a_obj_lv8UdFloor.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv8UdFloor.h"

class daUdFloor_HIO_c : public mDoHIO_entry_c {
public:
    daUdFloor_HIO_c();
    virtual ~daUdFloor_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ u8 field_0x4;
    /* 0x08 */ f32 init_speed;
    /* 0x0C */ u8 vibration;
    /* 0x0D */ u8 down_color_R;
    /* 0x0E */ u8 down_color_G;
    /* 0x0F */ u8 down_color_B;
    /* 0x10 */ u8 down_framecount;
    /* 0x11 */ u8 up_color_R;
    /* 0x12 */ u8 up_color_G;
    /* 0x13 */ u8 up_color_B;
    /* 0x14 */ u8 up_framecount;
};

static char* l_resNameIdx[2] = {
    "L8Step",
    "L8StepX",
};

daUdFloor_HIO_c::daUdFloor_HIO_c() {
    field_0x4 = 60;
    init_speed = 0.0f;
    vibration = 4;

    down_color_R = 7;
    down_color_G = 14;
    down_color_B = 16;
    down_framecount = 10;

    up_color_R = 0;
    up_color_G = 255;
    up_color_B = 255;
    up_framecount = 15;
}

void daUdFloor_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(0, current.angle.y, 0);
    mDoMtx_stack_c::transM(0.0f, field_0x5c0, 0.0f);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static const int l_bmdIdx[] = {4, 4};

int daUdFloor_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mModelType], l_bmdIdx[mModelType]);
    JUT_ASSERT(231, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

static const int l_dzbIdx[] = {7, 7};

static const u32 l_heap_size[] = {0x1200, 0x1400};

static daUdFloor_HIO_c l_HIO;

int daUdFloor_c::create() {
    fopAcM_ct(this, daUdFloor_c);

    mModelType = getModelType();

    int phase_state = dComIfG_resLoad(&mPhase, l_resNameIdx[mModelType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate(l_resNameIdx[mModelType], l_dzbIdx[mModelType], dBgS_MoveBGProc_TypicalRotY, l_heap_size[mModelType], NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }
    
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        s8 move_height = getMoveHeight();
        field_0x5b0 = 50.0f * move_height;

        u8 move_speed = getMoveSpeed();
        field_0x5b4 = field_0x5b0 / move_speed;

        mTimer = getTimer();
        mIsSwitch = fopAcM_isSwitch(this, getSw());
    
        if (mIsSwitch) {
            field_0x5c0 = field_0x5b0;
            mColorR = l_HIO.up_color_R;
            mColorG = l_HIO.up_color_G;
            mColorB = l_HIO.up_color_B;
        } else {
            field_0x5c0 = 0.0f;
            mColorR = l_HIO.down_color_R;
            mColorG = l_HIO.down_color_G;
            mColorB = l_HIO.down_color_B;
        }
    
        init_modeWait();
        setBaseMtx();

        #if DEBUG
        l_HIO.entryHIO("上下床(Lv8)");
        #endif
    }

    return phase_state;
}

int daUdFloor_c::Execute(Mtx** param_0) {
    moveLift();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

void daUdFloor_c::moveLift() {
    static void (daUdFloor_c::*mode_proc[])() = {
        &daUdFloor_c::modeWait,
        &daUdFloor_c::modeUpMoveInit,
        &daUdFloor_c::modeUpMove,
        &daUdFloor_c::modeDownMoveInit,
        &daUdFloor_c::modeDownMove,
    };

    u8 prev_switch = mIsSwitch;
    mIsSwitch = fopAcM_isSwitch(this, getSw());
    if (prev_switch != mIsSwitch) {
        if (mIsSwitch) {
            init_modeUpMoveInit();
        } else {
            init_modeDownMoveInit();
        }
    }

    (this->*mode_proc[mMode])();
}

void daUdFloor_c::init_modeWait() {
    mMode = 0;
}

void daUdFloor_c::modeWait() {}

void daUdFloor_c::init_modeUpMoveInit() {
    field_0x5c4 = 0;
    field_0x5c5 = l_HIO.up_framecount;
    mDoAud_seStart(Z2SE_OBJ_L8_STAIR_ON, &home.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = 1;
}

void daUdFloor_c::modeUpMoveInit() {
    if (colorAnm(1) == TRUE) {
        init_modeUpMove();
    }
}

u8 daUdFloor_c::colorAnm(BOOL param_0) {
    u8 rt = FALSE;

    f32 temp_f31 = field_0x5c4 / (field_0x5c5 - 1.0f);
    if (param_0) {
        mColorR = l_HIO.down_color_R + (temp_f31 * ((f32)l_HIO.up_color_R - (f32)l_HIO.down_color_R));
        mColorG = l_HIO.down_color_G + (temp_f31 * ((f32)l_HIO.up_color_G - (f32)l_HIO.down_color_G));
        mColorB = l_HIO.down_color_B + (temp_f31 * ((f32)l_HIO.up_color_B - (f32)l_HIO.down_color_B));
    } else {
        mColorR = l_HIO.up_color_R - (temp_f31 * ((f32)l_HIO.up_color_R - (f32)l_HIO.down_color_R));
        mColorG = l_HIO.up_color_G - (temp_f31 * ((f32)l_HIO.up_color_G - (f32)l_HIO.down_color_G));
        mColorB = l_HIO.up_color_B - (temp_f31 * ((f32)l_HIO.up_color_B - (f32)l_HIO.down_color_B));
    }

    field_0x5c4++;
    if (field_0x5c4 > field_0x5c5 - 1) {
        rt = TRUE;
    }

    return rt;
}

void daUdFloor_c::init_modeUpMove() {
    fopAcM_SetSpeedF(this, l_HIO.init_speed);
    field_0x5ba = mTimer;
    if (field_0x5ba == 0) {
        mDoAud_seStart(Z2SE_OBJ_L8_STAIR_MV_UP, &home.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
    mMode = 2;
}

void daUdFloor_c::modeUpMove() {
    if (field_0x5ba != 0) {
        field_0x5ba--;
        if (field_0x5ba == 0) {
            mDoAud_seStart(Z2SE_OBJ_L8_STAIR_MV_UP, &home.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
    } else {
        cLib_chaseF(&speedF, field_0x5b4, field_0x5b4 / 30.0f);
        if (cLib_addCalc(&field_0x5c0, field_0x5b0, 1.0f, fopAcM_GetSpeedF(this), 0.1f) == 0.0f) {
            mDoAud_seStart(Z2SE_OBJ_L8_STAIR_ST_UP, &home.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            dComIfGp_getVibration().StartShock(l_HIO.vibration, 0xF, cXyz(0.0f, 1.0f, 0.0f));
            init_modeWait();
        }
    }
}

void daUdFloor_c::init_modeDownMoveInit() {
    field_0x5c4 = 0;
    field_0x5c5 = l_HIO.down_framecount;
    mMode = 3;
}

void daUdFloor_c::modeDownMoveInit() {
    if (colorAnm(0) == TRUE) {
        init_modeDownMove();
    }
}

void daUdFloor_c::init_modeDownMove() {
    mDoAud_seStart(Z2SE_OBJ_L8_STAIR_MV_DW, &home.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = 4;
}

void daUdFloor_c::modeDownMove() {
    if (cLib_addCalc(&field_0x5c0, 0.0f, 0.3f, field_0x5b4, 0.1f) == 0.0f) {
        mDoAud_seStart(Z2SE_OBJ_L8_STAIR_ST_DW, &home.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        init_modeWait();
    }
}

int daUdFloor_c::Draw() {
    g_env_light.settingTevStruct(64, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    J3DGXColor* matColor_p = mpModel->getModelData()->getMaterialNodePointer(1)->getTevKColor(1);
    matColor_p->r = mColorR;
    matColor_p->g = mColorG;
    matColor_p->b = mColorB;
    return 1;
}

int daUdFloor_c::Delete() {
    dComIfG_resDelete(&mPhase, l_resNameIdx[mModelType]);
    #if DEBUG
    l_HIO.removeHIO();
    #endif
    return 1;
}

static int daUdFloor_Draw(daUdFloor_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daUdFloor_Execute(daUdFloor_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daUdFloor_Delete(daUdFloor_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daUdFloor_Create(fopAc_ac_c* i_this) {
    return ((daUdFloor_c*)i_this)->create();
}

static actor_method_class l_daUdFloor_Method = {
    (process_method_func)daUdFloor_Create,
    (process_method_func)daUdFloor_Delete,
    (process_method_func)daUdFloor_Execute,
    0,
    (process_method_func)daUdFloor_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv8UdFloor = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv8UdFloor,    // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daUdFloor_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  646,                    // mPriority
  &l_daUdFloor_Method,    // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
