/**
 * @file d_a_obj_lv5FloorBoard.cpp
 * Snowpeak Ruins Destructable Floor
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv5FloorBoard.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

daFlorBoad_HIO_c::daFlorBoad_HIO_c() {
    field_0x10 = 3;
    field_0x11 = 2;

    field_0x4.x = 1.0f;
    field_0x4.y = 1.0f;
    field_0x4.z = 1.0f;
}

void daFlorBoad_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daFlorBoad_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L5haYuka", 4);
    JUT_ASSERT(0, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daFlorBoad_c::create() {
    fopAcM_ct(this, daFlorBoad_c);

    mSwBit1 = getSwBit1();
    if (mSwBit1 != 0xFF && fopAcM_isSwitch(this, mSwBit1)) {
        return cPhs_ERROR_e;
    }

    int phase = dComIfG_resLoad(&mPhase, "L5haYuka");
    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L5haYuka", 7, dBgS_MoveBGProc_TypicalRotY, 0x1900, NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mCcStts.Init(0xFF, 0xFF, this);
        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);

        init_modeWait();
        setBaseMtx();
    }

    return phase;
}

int daFlorBoad_c::Execute(Mtx** param_0) {
    procMain();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    mCcStts.Move();
    return 1;
}

static daFlorBoad_HIO_c l_HIO;

const dCcD_SrcGObjInf daFlorBoad_c::mCcDObjInfo = {
    {0, {{0, 1, 0}, {0x400020, 0x11}, {0}}},
    {dCcD_SE_SWORD, 0, 0, 2, 1},
    {dCcD_SE_STONE, 0, 0, 0, 6},
    {0},
};

dCcD_SrcCyl daFlorBoad_c::mCcDCyl = {
    daFlorBoad_c::mCcDObjInfo,
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            0.0f,                // mRadius
            0.0f                 // mHeight
        }  // mCyl
    }
};

void daFlorBoad_c::procMain() {
    typedef void (daFlorBoad_c::*procFunc)();
    static procFunc mode_proc[] = {
        &daFlorBoad_c::modeWait,
        &daFlorBoad_c::modeBreak,
    };

    (this->*mode_proc[mMode])();
}

void daFlorBoad_c::init_modeWait() {
    field_0x725 = l_HIO.field_0x10;
    mMode = MODE_WAIT;
}

void daFlorBoad_c::modeWait() {
    if (mCcCyl.ChkTgHit()) {
        dCcD_GObjInf* hit_obj = mCcCyl.GetTgHitGObj();

        if (hit_obj->GetAtType() & AT_TYPE_IRON_BALL &&
            (hit_obj->GetAtAtp() == 100 || hit_obj->GetAtAtp() == 3))
        {
            init_modeBreak();
        }

        if (hit_obj->GetAtType() & AT_TYPE_BOMB) {
            init_modeBreak();
        }
    }

    mCcCyl.SetR(oREG_F(0) + 280.0f);
    mCcCyl.SetH(oREG_F(1) + 170.0f);

    cXyz center(current.pos);
    center.y -= 150.0f;
    mCcCyl.SetC(center);

    dComIfG_Ccsp()->Set(&mCcCyl);
}

void daFlorBoad_c::init_modeBreak() {
    if (mSwBit1 != 0xFF) {
        fopAcM_onSwitch(this, mSwBit1);
    }

    mDoAud_seStart(Z2SE_OBJ_BREAK_FLOOR, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = MODE_BREAK;
}

void daFlorBoad_c::modeBreak() {
    dComIfGp_particle_set(0x8C01, &current.pos, NULL, NULL);
    dComIfGp_particle_set(0x8C02, &current.pos, NULL, NULL);
    dComIfGp_particle_set(0x8C03, &current.pos, NULL, NULL);
    dComIfGp_particle_set(0x8C04, &current.pos, NULL, NULL);
    fopAcM_delete(this);
}

int daFlorBoad_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daFlorBoad_c::Delete() {
    dComIfG_resDelete(&mPhase, "L5haYuka");
    return 1;
}

static int daFlorBoad_Draw(daFlorBoad_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daFlorBoad_Execute(daFlorBoad_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daFlorBoad_Delete(daFlorBoad_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daFlorBoad_Create(fopAc_ac_c* i_this) {
    return ((daFlorBoad_c*)i_this)->create();
}

static actor_method_class l_daFlorBoad_Method = {
    (process_method_func)daFlorBoad_Create,  (process_method_func)daFlorBoad_Delete,
    (process_method_func)daFlorBoad_Execute, (process_method_func)NULL,
    (process_method_func)daFlorBoad_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv5FBoard = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Lv5FBoard,      // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daFlorBoad_c),    // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    538,                     // mPriority
    &l_daFlorBoad_Method,    // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
