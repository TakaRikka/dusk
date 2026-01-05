/**
 * @file d_a_obj_chest.cpp
 * @brief Object - Chest
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_chest.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_s.h"
#include "d/d_bg_w.h"
#include "d/d_procname.h"
#include "d/actor/d_a_obj_eff.h"

void daObjChest_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

const static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xffffffff, 0x0}, 0x19}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjTg
        {0x0},                                              // mGObjCo
    },                                                      // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            40.0f,               // mRadius
            260.0f               // mHeight
        }                        // mCyl
    }
};

void daObjChest_c::setBaseMtx() {
    mRandomRotX -= 0x8C;
    mModifyRotX += mRandomRotX;
    if (mModifyRotX < 0) {
        mModifyRotX = 0;
        mRandomRotX *= -field_0xa9c;
    }
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::ZXYrotM(mFallingChestRot.x + mModifyRotX, mFallingChestRot.y,
                            mFallingChestRot.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    if ((s16)mFallingChestRot.x != 0) {
        csXyz rotation;
        rotation = mFallingChestRot;
        rotation.x = 0x4000;
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::ZXYrotM(rotation);
        mDoMtx_stack_c::scaleM(1.0f, mScaleY, 1.0f);
    }
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjChest_c::Create() {
    u8 switch_no = getSwNo();
    if (fopAcM_isSwitch(this, switch_no)) {
        mFallingChestRot.x = 0x4000;
        cXyz transform(current.pos);
        transform.y += 5.0f;
        mFallingChestPos.set(0.0f, 150.0f, 0.0f);
        mDoMtx_stack_c::transS(transform);
        mDoMtx_stack_c::YrotM(current.angle.y);
        mDoMtx_stack_c::XrotM(mFallingChestRot.x);
        mDoMtx_stack_c::multVec(&mFallingChestPos, &mFallingChestPos);
        if (fopAcM_gc_c::gndCheck(&mFallingChestPos)) {
            current.pos.y = fopAcM_gc_c::getGroundY();
        }
        mScaleY = 1.0f;
        setAction(ACTION_END);
    } else {
        setAction(ACTION_WAIT);
    }
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mAcchCir.SetWall(30.0f, 30.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                 fopAcM_GetSpeed_p(this), NULL, NULL);
    mStts.Init(0xff, 0xff, this);
    for (int i = 0; i < 2; i++) {
        mCyl[i].Set(l_cyl_src);
        mCyl[i].SetStts(&mStts);
    }
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    return 1;
}

static char* l_arcName = "H_Tansu";

int daObjChest_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel != NULL) {
        return 1;
    }
    return 0;
}

int daObjChest_c::create() {
    fopAcM_ct(this, daObjChest_c);

    int phase = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0xc00, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }
    return phase;
}

int daObjChest_c::Execute(Mtx** i_mtx) {
    mLifeCycleLength++;
    action();
    *i_mtx = &mBgMtx;
    setBaseMtx();
    return 1;
}

void daObjChest_c::action() {
    typedef void (daObjChest_c::*actionFunc)();
    static actionFunc l_func[3] = {
        &daObjChest_c::actionWait,
        &daObjChest_c::actionMove,
        &daObjChest_c::actionEnd,
    };

    (this->*l_func[mAction])();
}

void daObjChest_c::actionWait() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (getArg0() == 0 && cLib_calcTimer<u16>(&mRandomMoveStartTimer) == 0) {
        mRandomMoveStartTimer = cM_rndF(90.0f) + 70.0f;
        mRandomRotX = cM_rndFX(100.0f) + 450.0f;
        if (cM_rndF(1.0f) > 0.5f) {
            field_0xa9c = 0.0f;
        } else {
            field_0xa9c = 0.7f;
        }
        fopAcM_seStart(this, Z2SE_OBJ_TANSU_YURE, 0);
    }

    if (!fopAcM_isSwitch(this, getSwNo()) && player->checkNowWolf()) {
        if (player->checkWolfAttackReverse() || player->checkFrontRollCrash()) {
            for (int i = 0; i < 2; i++) {
                cXyz multiply[2] = {
                    cXyz(50.0f, 0.0f, -50.0f),
                    cXyz(-50.0f, 0.0f, -50.0f),
                };
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(current.angle.y);
                mDoMtx_stack_c::multVec(&multiply[i], &multiply[i]);

                cXyz playerMultiply(0.0f, 0.0f, 90.0f);
                mDoMtx_stack_c::transS(player->current.pos);
                mDoMtx_stack_c::YrotM(player->shape_angle.y);
                mDoMtx_stack_c::multVec(&playerMultiply, &playerMultiply);
                f32 multAbsXZ = multiply[i].absXZ(playerMultiply);
                if (multAbsXZ < 100.0f && playerMultiply.y < multiply[i].y + 260.0f) {
                    mMoveStartTimer = 5;
                    setAction(ACTION_MOVE);
                    mScaleY = 0.0f;
                    fopAcM_onSwitch(this, getSwNo());
                }
            }
        }
    }
}

void daObjChest_c::actionMove() {
    if (cLib_calcTimer<u8>(&mMoveStartTimer) == 0) {
        mExtraRot.x += 0x80;
        mFallingChestRot += mExtraRot;
        mScaleY = mFallingChestRot.x / 16384.0f;
        if (mScaleY > 1.0f) {
            mScaleY = 1.0f;
        }
        if (mFallingChestRot.x > 0x4000) {
            mFallingChestRot.x = 0x4000;
            mExtraRot.x = 0;
            fopAcM_SetGravity(this, -4.0f);
            current.pos.y -= gravity;
            mFallingChestPos.set(0.0f, 150.0f, 0.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(current.angle.y);
            mDoMtx_stack_c::XrotM(mFallingChestRot.x);
            mDoMtx_stack_c::multVec(&mFallingChestPos, &mFallingChestPos);
            if (fopAcM_gc_c::gndCheck(&mFallingChestPos)) {
                current.pos.y = fopAcM_gc_c::getGroundY();
                fopAcM_SetGravity(this, 0.0f);
                daObjEff::Act_c::make_land_smoke(&mFallingChestPos, 2.5f);
                dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                mDoAud_seStart(Z2SE_OBJ_CHEST_DOWN, &mFallingChestPos, 0,
                               dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
                setAction(ACTION_END);
            }
        }
    }
}

void daObjChest_c::actionEnd() {
    /* empty function */
}

int daObjChest_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjChest_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static int daObjChest_Draw(daObjChest_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjChest_Execute(daObjChest_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjChest_Delete(daObjChest_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjChest_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjChest_c*>(i_this)->create();
}

static actor_method_class l_daObjChest_Method = {
    (process_method_func)daObjChest_Create,  (process_method_func)daObjChest_Delete,
    (process_method_func)daObjChest_Execute, (process_method_func)NULL,
    (process_method_func)daObjChest_Draw,
};

actor_process_profile_definition g_profile_Obj_Chest = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Chest,          // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjChest_c),    // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    603,                     // mPriority
    &l_daObjChest_Method,    // sub_method
    0x40100,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
