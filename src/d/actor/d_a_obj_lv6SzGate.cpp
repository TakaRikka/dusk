/**
* @file d_a_obj_lv6SzGate.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv6SzGate.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

static cull_box l_cull_box = {{-1200.0f, -300.0f, -100.0f}, {1200.0f, 300.0f, 100.0f}};

static daLv6SzGate_HIO_c l_HIO;

daLv6SzGate_HIO_c::daLv6SzGate_HIO_c() {
    mSpeed = 4.0f;
    field_0x08 = 30.0f;
    field_0x0c = 15.0f;
    field_0x10 = 400.0f;
    mShockStrength = 3;
}

void daLv6SzGate_HIO_c::genMessage(JORMContext* arg) {
    // DEBUG NONMATCHING
}

void daLv6SzGate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(-200.0f - field_0x5e4, 0.0f, 0.0f);

    field_0x5a8->setBaseScale(scale);
    field_0x5a8->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), mBgMtx);

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y + 0x10000 - 0x8000, shape_angle.z);
    mDoMtx_stack_c::transM(-200.0f - field_0x5e4, 0.0f, 0.0f);

    field_0x5ac->setBaseScale(scale);
    field_0x5ac->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), mMtx);
}

int daLv6SzGate_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L6SzGate", 6);
    field_0x5a8 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);

    if (field_0x5a8 == NULL) {
        return 0;
    }

    field_0x5ac = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (field_0x5ac == NULL) {
        return 0;
    }

    field_0x5e8 = new dBgW();

    if (field_0x5e8 != NULL &&
        !field_0x5e8->Set(static_cast<cBgD_t*>(dComIfG_getObjectRes("L6SzGate", 15)), 1, &mMtx))
    {
        field_0x5e8->SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
    } else {
        field_0x5e8 = NULL;
        return 0;
    }

    field_0x5b0.init(modelData, static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("L6SzGate", 9)),
                     1, 0, 1.0f, 0, -1);
    field_0x5c8.init(modelData,
                     static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("L6SzGate", 12)), 1, 0,
                     1.0f, 0, -1);

    return 1;
}

cPhs__Step daLv6SzGate_c::create() {
    fopAcM_ct(this, daLv6SzGate_c);

    cPhs__Step phase = static_cast<cPhs__Step>(dComIfG_resLoad(&mPhase, "L6SzGate"));
    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L6SzGate", 15, dBgS_MoveBGProc_TypicalRotY, 0x1d80, NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, field_0x5a8->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z,
                              l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.y);

        if (field_0x5e8 != NULL) {
            dComIfG_Bgsp().Regist(field_0x5e8, this);
            field_0x5e8->Move();
        }
        field_0x5e2 = getSw();
        field_0x5e3 = fopAcM_isSwitch(this, field_0x5e2);
        field_0x5e4 = 0.0f;
        if (field_0x5e3 != 0) {
            field_0x5e4 = l_HIO.field_0x10;
            init_modeEnd();
        } else {
            init_modeWait();
        }
        setBaseMtx();
    }
    return phase;
}

int daLv6SzGate_c::Execute(Mtx** param_0) {
    moveGate();
    *param_0 = &field_0x5a8->getBaseTRMtx();
    setBaseMtx();
    if (field_0x5e8 != NULL) {
        field_0x5e8->Move();
    }
    return 1;
}

void daLv6SzGate_c::moveGate() {
    typedef void (daLv6SzGate_c::*daLv6SzGate_modeFunc)();
    static daLv6SzGate_modeFunc mode_proc[] = {
        &daLv6SzGate_c::modeWait, &daLv6SzGate_c::modeOpenWait0, &daLv6SzGate_c::modeOpenWait,
        &daLv6SzGate_c::modeOpen, &daLv6SzGate_c::modeClose,     &daLv6SzGate_c::modeEnd,
    };

    (this->*mode_proc[mMode])();
}

void daLv6SzGate_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daLv6SzGate_c::modeWait() {
    if (fopAcM_isSwitch(this, field_0x5e2)) {
        init_modeOpenWait0();
    }
}

void daLv6SzGate_c::init_modeOpenWait0() {
    field_0x5e1 = getWaitTime();
    mMode = MODE_OPENWAIT0;
}

void daLv6SzGate_c::modeOpenWait0() {
    if (field_0x5e1 != 0) {
        --field_0x5e1;
    } else {
        init_modeOpenWait();
    }
}

void daLv6SzGate_c::init_modeOpenWait() {
    mDoAud_seStart(Z2SE_OBJ_L6_MARIO_GATE_ON, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = MODE_OPENWAIT;
}

void daLv6SzGate_c::modeOpenWait() {
    int iVar1 = field_0x5b0.play();
    int iVar2 = field_0x5c8.play();
    if (iVar1 == 1 || iVar2 == 1) {
        init_modeOpen();
    }
}

void daLv6SzGate_c::init_modeOpen() {
    mMode = MODE_OPEN;
}

void daLv6SzGate_c::modeOpen() {
    float tmp = cLib_addCalc(&field_0x5e4, l_HIO.field_0x10, 0.2f, l_HIO.mSpeed, 0.5f);
    mDoAud_seStartLevel(Z2SE_OBJ_L6_MARIO_GATE_MV, &current.pos, 0,
                        dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (tmp == 0.0f) {
        mDoAud_seStart(Z2SE_OBJ_L6_MARIO_GATE_OFF, &current.pos, 0,
                       dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        dComIfGp_getVibration().StartShock(l_HIO.mShockStrength, 0xf, cXyz(0.0f, 1.0f, 0.0f));
        init_modeEnd();
    }
}

void daLv6SzGate_c::modeClose() {
    f32 fVar1 = cLib_chaseF(&speedF, l_HIO.field_0x08, l_HIO.field_0x0c);
    fVar1 = cLib_addCalc(&field_0x5e4, 0.0f, 1.0f, fopAcM_GetSpeedF(this), 5.0f);
    mDoAud_seStartLevel(Z2SE_OBJ_L6_MARIO_GATE_MV, &current.pos, 0,
                        dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    if (fVar1 == 0.0f) {
        init_modeWait();
    }
}

void daLv6SzGate_c::init_modeEnd() {
    field_0x5b0.setPlaySpeed(-1.0f);
    field_0x5c8.setPlaySpeed(-1.0f);
    mMode = MODE_END;
}

void daLv6SzGate_c::modeEnd() {
    field_0x5b0.play();
    field_0x5c8.play();
}

int daLv6SzGate_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(field_0x5a8, &tevStr);
    g_env_light.setLightTevColorType_MAJI(field_0x5ac, &tevStr);

    J3DModelData* tmp = field_0x5a8->getModelData();
    field_0x5b0.entry(tmp);
    field_0x5c8.entry(tmp);
    tmp = field_0x5ac->getModelData();
    field_0x5b0.entry(tmp);
    field_0x5c8.entry(tmp);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(field_0x5a8);
    mDoExt_modelUpdateDL(field_0x5ac);
    dComIfGd_setList();

    return 1;
}

int daLv6SzGate_c::Delete() {
    dComIfG_resDelete(&mPhase, "L6SzGate");
    if (field_0x5e8 != NULL) {
        if (field_0x5e8->ChkUsed()) {
            dComIfG_Bgsp().Release(field_0x5e8);
        }
    }
    return 1;
}

static int daLv6SzGate_Draw(daLv6SzGate_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv6SzGate_Execute(daLv6SzGate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv6SzGate_Delete(daLv6SzGate_c* i_this) {
    fopAcM_GetID(i_this);
    return i_this->MoveBGDelete();
}

static int daLv6SzGate_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daLv6SzGate_c*>(i_this)->create();
}

static actor_method_class l_daLv6SzGate_Method = {
    (process_method_func)daLv6SzGate_Create,  (process_method_func)daLv6SzGate_Delete,
    (process_method_func)daLv6SzGate_Execute, (process_method_func)NULL,
    (process_method_func)daLv6SzGate_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv6SzGate = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Lv6SzGate,      // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daLv6SzGate_c),   // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    628,                     // mPriority
    &l_daLv6SzGate_Method,   // sub_method
    0x00040000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
