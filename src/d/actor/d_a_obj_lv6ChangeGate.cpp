/**
 * @file d_a_obj_lv6ChangeGate.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv6ChangeGate.h"
#include "d/d_s_play.h"

class daLv6ChangeGate_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daLv6ChangeGate_HIO_c();
    virtual ~daLv6ChangeGate_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ f32 speed;
    /* 0x8 */ f32 move_amount;
    /* 0xC */ u8 unk_0xc;
};

daLv6ChangeGate_HIO_c::daLv6ChangeGate_HIO_c() {
    speed = 10.0f;
    move_amount = 562.5f;
    unk_0xc = 3;
}

static daLv6ChangeGate_HIO_c l_HIO;

const dCcD_SrcGObjInf daLv6ChangeGate_c::mCcDObjInfo = {
    {0, {{0, 0, 0}, {0x400, 0x11}, {0x79}}},
    {dCcD_SE_SWORD, 0, 0, 0, 0},
    {dCcD_SE_STONE, 0, 0, 0, 0},
    {0},
};

dCcD_SrcCyl daLv6ChangeGate_c::mCcDCyl = {
    mCcDObjInfo,
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f, 0.0f}
    }
};

void daLv6ChangeGate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(0, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(mMoveTransX, 0.0f, 0.0f);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daLv6ChangeGate_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L6ChBlk", 4);
    JUT_ASSERT(240, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daLv6ChangeGate_c::create() {
    fopAcM_ct(this, daLv6ChangeGate_c);

    if (getSw2() != 0xFF && fopAcM_isSwitch(this, getSw2())) {
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, "L6ChBlk");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L6ChBlk", 7, dBgS_MoveBGProc_TypicalRotY, 0x1200, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mCcStts.Init(0xFF, 0xFF, this);
        for (int i = 0; i < 12; i++) {
            mCcCyls[i].Set(mCcDCyl);
            mCcCyls[i].SetStts(&mCcStts);
        }

        mSwState = getSwState();
        if (mSwState == 0xF) {
            mSwState = 0;
        }

        mSwbit = getSw();
        mIsSwitch = isSwitch();
        mMoveTransX = 0.0f;

        if (mIsSwitch) {
            mMoveTransX = l_HIO.move_amount;
        } else {
            init_modeWait();
        }

        setBaseMtx();
    }

    return phase_state;
}

u8 daLv6ChangeGate_c::isSwitch() {
    return mSwState ^ fopAcM_isSwitch(this, mSwbit);
}

int daLv6ChangeGate_c::Execute(Mtx** param_0) {
    eventUpdate();
    moveGate();

    for (int i = 0; i < 12; i++) {
        if (mCcCyls[i].ChkTgHit()) {
            dCcD_GObjInf* gobj = mCcCyls[i].GetTgHitGObj();
            if (gobj->GetAtType() & AT_TYPE_CSTATUE_SWING) {
                init_modeBreak();
                break;
            }
        }
    }

    f32 cyl_x_offset = -250.0f;

    for (int i = 0; i < 12; i++) {
        cXyz spC(cyl_x_offset + mMoveTransX, 0.0f, 0.0f);
        mDoMtx_stack_c::ZXYrotS(NULL, shape_angle.y, shape_angle.z);
        mDoMtx_stack_c::multVec(&spC, &spC);
        spC += current.pos;

        mCcCyls[i].SetR(65.0f + oREG_F(0));
        mCcCyls[i].SetH(330.0f + oREG_F(1));
        mCcCyls[i].SetC(spC);
        dComIfG_Ccsp()->Set(&mCcCyls[i]);

        cyl_x_offset += 60.0f;
    }

    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

void daLv6ChangeGate_c::moveGate() {
    static void (daLv6ChangeGate_c::*mode_proc[])() = {
        &daLv6ChangeGate_c::modeWait,
        &daLv6ChangeGate_c::modeSlideRight,
        &daLv6ChangeGate_c::modeSlideLeft,
        &daLv6ChangeGate_c::modeBreak,
    };

    u8 prev_sw = mIsSwitch;
    mIsSwitch = isSwitch();

    if (mIsSwitch != prev_sw) {
        if (getEventID() != 0xFF) {
            orderEvent(getEventID(), 0xFF, 1);
        } else {
            eventStart();
        }
    }

    (this->*mode_proc[mMode])();
}

bool daLv6ChangeGate_c::eventStart() {
    if (mIsSwitch) {
        init_modeSlideRight();
    } else {
        init_modeSlideLeft();
    }

    return true;
}

void daLv6ChangeGate_c::init_modeWait() {
    mMode = 0;
}

void daLv6ChangeGate_c::modeWait() {}

void daLv6ChangeGate_c::init_modeSlideRight() {
   mTimer = getWaitTime();
    if (mTimer == 0xFF) {
        mTimer = 0;
    }

    if (mTimer == 0) {
        mDoAud_seStart(Z2SE_OBJ_L6_SW_GATE_ON, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    mMode = 1;
}

void daLv6ChangeGate_c::modeSlideRight() {
    if (mTimer != 0) {
        mTimer--;
        if (mTimer == 0) {
            mDoAud_seStart(Z2SE_OBJ_L6_SW_GATE_ON, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
        return;
    }

    f32 dist_to_target = cLib_addCalc(&mMoveTransX, l_HIO.move_amount, 0.1f, l_HIO.speed, 1.0f);
    mDoAud_seStartLevel(Z2SE_OBJ_L6_SW_GATE_MV, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (0.0f == dist_to_target) {
        mDoAud_seStart(Z2SE_OBJ_L6_SW_GATE_OFF, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        init_modeWait();
    }
}

void daLv6ChangeGate_c::init_modeSlideLeft() {
    mTimer = getWaitTime();
    if (mTimer == 0xFF) {
        mTimer = 0;
    }

    if (mTimer == 0) {
        mDoAud_seStart(Z2SE_OBJ_L6_SW_GATE_ON, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    mMode = 2;
}

void daLv6ChangeGate_c::modeSlideLeft() {
    if (mTimer != 0) {
        mTimer--;
        if (mTimer == 0) {
            mDoAud_seStart(Z2SE_OBJ_L6_SW_GATE_ON, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
        return;
    }

    f32 dist_to_target = cLib_addCalc(&mMoveTransX, 0.0f, 0.1f, l_HIO.speed, 1.0f);
    mDoAud_seStartLevel(Z2SE_OBJ_L6_SW_GATE_MV, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (0.0f == dist_to_target) {
        mDoAud_seStart(Z2SE_OBJ_L6_SW_GATE_OFF, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        init_modeWait();
    }
}

void daLv6ChangeGate_c::init_modeBreak() {
    mDoAud_seStart(Z2SE_OBJ_TRAP_BREAK_STN, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = 3;
}

void daLv6ChangeGate_c::modeBreak() {
    cXyz effpos(mMoveTransX, 0.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotS(NULL, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::multVec(&effpos, &effpos);
    effpos += current.pos;

    csXyz effrot(shape_angle);
    effrot.x = 0;

    dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV6CHBLK_BREAK_A), &effpos, &effrot, NULL);
    dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV6CHBLK_BREAK_B), &effpos, &effrot, NULL);
    dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV6CHBLK_BREAK_C), &effpos, &effrot, NULL);
    dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV6CHBLK_BREAK_D), &effpos, &effrot, NULL);
    dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV6CHBLK_BREAK_E), &effpos, &effrot, NULL);

    if (getSw2() != 0xFF) {
        fopAcM_onSwitch(this, getSw2());
    }

    fopAcM_delete(this);
}

int daLv6ChangeGate_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daLv6ChangeGate_c::Delete() {
    dComIfG_resDelete(&mPhase, "L6ChBlk");
    return 1;
}

static int daLv6ChangeGate_Draw(daLv6ChangeGate_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv6ChangeGate_Execute(daLv6ChangeGate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv6ChangeGate_Delete(daLv6ChangeGate_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daLv6ChangeGate_Create(fopAc_ac_c* i_this) {
    return ((daLv6ChangeGate_c*)i_this)->create();
}

static actor_method_class l_daLv6ChangeGate_Method = {
    (process_method_func)daLv6ChangeGate_Create,
    (process_method_func)daLv6ChangeGate_Delete,
    (process_method_func)daLv6ChangeGate_Execute,
    (process_method_func)NULL,
    (process_method_func)daLv6ChangeGate_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv6ChgGate = {
  fpcLy_CURRENT_e,           // mLayerID
  3,                         // mListID
  fpcPi_CURRENT_e,           // mListPrio
  PROC_Obj_Lv6ChgGate,       // mProcName
  &g_fpcLf_Method.base,     // sub_method
  sizeof(daLv6ChangeGate_c), // mSize
  0,                         // mSizeOther
  0,                         // mParameters
  &g_fopAc_Method.base,      // sub_method
  626,                       // mPriority
  &l_daLv6ChangeGate_Method, // sub_method
  0x00040000,                // mStatus
  fopAc_ACTOR_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,    // cullType
};
