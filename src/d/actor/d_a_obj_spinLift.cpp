/**
 * @file d_a_obj_spinLift.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_spinLift.h"

class daSpinLift_HIO_c : public mDoHIO_entry_c {
public:
    daSpinLift_HIO_c();
    ~daSpinLift_HIO_c() {};

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x06 */ u8 field_0x06;
    /* 0x08 */ f32 mInitialSpeed;
    /* 0x0C */ u8 mVibration;
};

static daSpinLift_HIO_c l_HIO;

daSpinLift_HIO_c::daSpinLift_HIO_c() {
    field_0x06 = 0x3c;
    mInitialSpeed = 0.0f;
    mVibration = 4;
}

#if DEBUG
void daSpinLift_HIO_c::genMessage(JORMContext* ctx) {
    // === Ascent ===
    ctx->genLabel("\n=== 上昇 ===", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Initial Speed
    ctx->genSlider("初速", &mInitialSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Vibration
    ctx->startComboBox("振動", &mVibration, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);
    // Strength 1 - 8
    ctx->genComboBoxItem("強さ１", 1);
    ctx->genComboBoxItem("強さ２", 2);
    ctx->genComboBoxItem("強さ３", 3);
    ctx->genComboBoxItem("強さ４", 4);
    ctx->genComboBoxItem("強さ５", 5);
    ctx->genComboBoxItem("強さ６", 6);
    ctx->genComboBoxItem("強さ７", 7);
    ctx->genComboBoxItem("強さ８", 8);
    ctx->endComboBox();
}
#endif

static char* l_resNameIdx[] = {
    "L4_N_Entr", "L4_N_SlpH", "L4_N_SlpL", "L4_Center", "L4_Switch",
    "L4_S_Slp",  "L4_Wal_ZE", "L4_Wal_ZW", "L4_Wal_X",
};

void daSpinLift_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daSpinLift_c::CreateHeap() {
    static u32 const l_bmdIdx[] = {4, 4, 4, 4, 4, 4, 4, 4, 4};
    J3DModelData* modelData =
        (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mModelType], l_bmdIdx[mModelType]);
    JUT_ASSERT(222, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    return 1;
}

int daSpinLift_c::create() {
    static const int l_dzbIdx[] = {7, 7, 7, 7, 7, 7, 7, 7, 7};
    fopAcM_ct(this, daSpinLift_c);
    mModelType = getModelType();
    int phase = dComIfG_resLoad(&mPhase, l_resNameIdx[mModelType]);

    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate(l_resNameIdx[mModelType], l_dzbIdx[mModelType],
                         dBgS_MoveBGProc_TypicalRotY, 0x9300, NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        s8 move_height = (s8)getMoveHeight();
        mMoveHeight = move_height * 50.0f;

        u8 move_speed = (u8)getMoveSpeed();
        mMoveSpeed = mMoveHeight / move_speed;

        mTimer = getTimer();
        field_0x5bc = fopAcM_isSwitch(this, getSw());

        if (fopAcM_isSwitch(this, getEndSw())) {
            current.pos.y += mMoveHeight;
            init_modeMoveEnd();
        } else {
            init_modeWait();
        }

        setBaseMtx();
#if DEBUG
        // Spin Lift
        l_HIO.entryHIO("スピンリフト");
#endif
    }

    return phase;
}

int daSpinLift_c::Execute(Mtx** param_0) {
    moveLift();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

void daSpinLift_c::moveLift() {
    static daSpinLift_c::modeFunc mode_proc[] = {
        &daSpinLift_c::modeWait,
        &daSpinLift_c::modeUpMove,
        &daSpinLift_c::modeDownMove,
        &daSpinLift_c::modeMoveEnd,
    };

    if (!fopAcM_isSwitch(this, getEndSw())) {
        u8 f5bc = field_0x5bc;
        field_0x5bc = fopAcM_isSwitch(this, getSw());
        if (f5bc != field_0x5bc) {
            if (field_0x5bc != 0) {
                init_modeUpMove();
            } else {
                init_modeDownMove();
            }
        }
    }

    (this->*mode_proc[field_0x5ac])();
}

void daSpinLift_c::init_modeWait() {
    field_0x5ac = 0;
}

void daSpinLift_c::modeWait() {}

void daSpinLift_c::init_modeUpMove() {
    fopAcM_SetSpeedF(this, l_HIO.mInitialSpeed);
    field_0x5ba = mTimer;
    field_0x5ac = 1;
}

void daSpinLift_c::modeUpMove() {
    if (field_0x5ba != 0) {
        field_0x5ba--;
        return;
    }

    int unused = cLib_chaseF(&speedF, mMoveSpeed, mMoveSpeed / 30.0f);
    f32 dVar3 =
        cLib_addCalc(&current.pos.y, home.pos.y + mMoveHeight, 1.0f, fopAcM_GetSpeedF(this), 0.1f);
    cXyz cStack_34(current.pos);
    cStack_34.y += cull.box.max.y;
    mDoAud_seStartLevel(Z2SE_OBJ_STN_SPRL_RAIL, &cStack_34, 0,
                        dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (dVar3 == 0.0f) {
        if (mModelType == 1 || mModelType == 4) {
            init_modeMoveEnd();
            return;
        }

        mDoAud_seStart(Z2SE_OBJ_STN_SPRL_RAIL_ST, 0, 0, 0);
        dComIfGp_getVibration().StartShock(l_HIO.mVibration, 15, cXyz(0.0f, 1.0f, 0.0f));

        csXyz local_48(shape_angle);
        local_48.x = 0;

        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_A), &current.pos, &local_48, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_B), &current.pos, &local_48, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_C), &current.pos, &local_48, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_D), &current.pos, &local_48, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_E), &current.pos, &local_48, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_F), &current.pos, &local_48, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_G), &current.pos, &local_48, NULL);
        init_modeWait();
    }
}

void daSpinLift_c::init_modeDownMove() {
    field_0x5ac = 2;
}

void daSpinLift_c::modeDownMove() {
    f32 dVar3 = cLib_addCalc(&current.pos.y, home.pos.y, 0.3f, mMoveSpeed, 0.1f);

    if (current.pos.y != home.pos.y) {
        cXyz acStack_38(current.pos);
        acStack_38.y += cull.box.max.y;
        mDoAud_seStartLevel(Z2SE_OBJ_STN_SPRL_RAIL_DW, &acStack_38, 0,
                            dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    if (dVar3 == 0.0f) {
        init_modeWait();
    }
}

void daSpinLift_c::init_modeMoveEnd() {
    if (fopAcM_isSwitch(this, getEndSw()) == 0) {
        fopAcM_onSwitch(this, getEndSw());
        mDoAud_seStart(Z2SE_OBJ_STN_SPRL_RAIL_ST, 0, 0, 0);
        dComIfGp_getVibration().StartShock(l_HIO.mVibration, 15, cXyz(0.0f, 1.0f, 0.0f));

        csXyz local_28(shape_angle);
        local_28.x = 0;

        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_A), &current.pos, &local_28, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_B), &current.pos, &local_28, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_C), &current.pos, &local_28, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_D), &current.pos, &local_28, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_E), &current.pos, &local_28, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_F), &current.pos, &local_28, NULL);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV4SLPH_KIDOU_G), &current.pos, &local_28, NULL);
    }

    field_0x5ac = 3;
}

void daSpinLift_c::modeMoveEnd() {}

int daSpinLift_c::Draw() {
    g_env_light.settingTevStruct(0x40, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daSpinLift_c::Delete() {
    dComIfG_resDelete(&mPhase, l_resNameIdx[mModelType]);
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daSpinLift_Draw(daSpinLift_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daSpinLift_Execute(daSpinLift_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daSpinLift_Delete(daSpinLift_c* i_this) {
    fpc_ProcID proc_id = fopAcM_GetID(i_this);
    return i_this->MoveBGDelete();
}

static int daSpinLift_Create(fopAc_ac_c* a_this) {
    daSpinLift_c* i_this = (daSpinLift_c*)a_this;
    fpc_ProcID proc_id = fopAcM_GetID(a_this);
    return i_this->create();
}

static actor_method_class l_daSpinLift_Method = {
    (process_method_func)daSpinLift_Create,  (process_method_func)daSpinLift_Delete,
    (process_method_func)daSpinLift_Execute, 0,
    (process_method_func)daSpinLift_Draw,
};

actor_process_profile_definition g_profile_Obj_SpinLift = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_SpinLift,       // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daSpinLift_c),    // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    512,                     // mPriority
    &l_daSpinLift_Method,    // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
