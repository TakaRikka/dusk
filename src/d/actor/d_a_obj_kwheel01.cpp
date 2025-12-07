/**
 * @file d_a_obj_kwheel01.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kwheel01.h"
#include "JSystem/JHostIO/JORMContext.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_obj_klift00.h"
#include "d/actor/d_a_obj_kwheel00.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

static int daObjKWheel01_create1st(daObjKWheel01_c*);
static int daObjKWheel01_MoveBGDelete(daObjKWheel01_c*);
static int daObjKWheel01_MoveBGExecute(daObjKWheel01_c*);
static int daObjKWheel01_MoveBGDraw(daObjKWheel01_c*);

static const u32 l_dzbidx[3] = {9, 8, 10};

static const int l_bmdidx[2] = {4, 5};


static const char* l_arcName = "K_Wheel01";

// Likely to have been a macro; matches for loop at beginning of create1st for both debug and retail, despite retail only calling getArg4567()
#define CHECK_KLIFT_EXISTS(kLiftNum) (!((1 << kLiftNum) & getArg4567()))

#if DEBUG
static daObjKWheel01_HIO_c l_HIO;

daObjKWheel01_HIO_c::daObjKWheel01_HIO_c() {
    mTargetYAngularSpeed = 64;
    mYAngularAcceleration = 2;
}

void daObjKWheel01_HIO_c::genMessage(JORMContext* ctx) {
    // "Pulley"
    ctx->genLabel("滑車", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Rotational speed"
    ctx->genSlider("回転速度", &mTargetYAngularSpeed, 0, 0x2000, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Rotational acceleration"
    ctx->genSlider("回転加速度", &mYAngularAcceleration, 0, 0x20, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

static Vec l_pos[4] = {
    {930.0f, -165.0f, 0.0f}, {0.0f, -165.0f, 930.0f},
    {-930.0f, -165.0f, 0.0f}, {0.0f, -165.0f, -930.0f}
};

cPhs__Step daObjKWheel01_c::create1st() {
    bool atLeastOneKLiftExists = false;
    s8 debugStackVar;

    for(int i = 0; i < 4; i++) {
        if(CHECK_KLIFT_EXISTS(i)) {
            atLeastOneKLiftExists = true;
        } 
    }

    if(!mCreatedKLifts) {
        setMtx();
        u32 createLiftParameters = (getArg2() & 0x3F) | daObjKLift00_c::LOCK_e | daObjKLift00_c::NO_BASE_DISP;
        for(int i = 0; i < 4; i++) {
            if(CHECK_KLIFT_EXISTS(i)) {
                cXyz kLiftPos;
                MTXMultVec(mNewBgMtx, &l_pos[i], &kLiftPos);
                m_klift_pid[i] = fopAcM_create(PROC_Obj_KLift00, createLiftParameters, &kLiftPos, fopAcM_GetHomeRoomNo(this), 0, 0, -1);
                JUT_ASSERT(146, m_klift_pid[i] != fpcM_ERROR_PROCESS_ID_e);

                mCreatedKLifts = true;
            }
        }
    }

    cPhs__Step phase = static_cast<cPhs__Step>(dComIfG_resLoad(this, l_arcName));
    if(phase == cPhs_COMPLEATE_e) {
        mYAngularVelocity = 0;
        setMtx();


        phase = static_cast<cPhs__Step>(MoveBGCreate(l_arcName, (getOut() ? l_dzbidx[2] : l_dzbidx[0]), dBgS_MoveBGProc_TypicalRotY, 0x5D98, &mNewBgMtx));
        if(phase == cPhs_ERROR_e)
            return phase;

        for(int i = 0; i < 4; i++) {
            if(CHECK_KLIFT_EXISTS(i) && dComIfG_Bgsp().Regist(mKLiftCollisions[i], this))
                return cPhs_ERROR_e;
        }
    }

    #if DEBUG
    // "Pulley(Lv3)"
    l_HIO.entryHIO("滑車(Lv3)");
    #endif

    return phase;
}

static const cull_box l_cull_box = {
    {-1260.0f, -3050.0f, -1260.0f},
    {1260.0f, 2800.0f, 1260.0f}
};

void daObjKWheel01_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    
    MTXCopy(mDoMtx_stack_c::get(), mTransformMtx);
    MTXCopy(mDoMtx_stack_c::get(), mNewBgMtx);

    if(mYAngularVelocity) {
        mDoMtx_stack_c::copy(mTransformMtx);
        mDoMtx_stack_c::transM(cM_rndFX(1.0f), cM_rndFX(1.0f), cM_rndFX(1.0f));
        MTXCopy(mDoMtx_stack_c::get(), mTransformMtx);
    }

    for(int i = 0; i < 4; i++) {
        if(CHECK_KLIFT_EXISTS(i)) {
            Vec kLiftPos;
            MTXMultVec(mNewBgMtx, &l_pos[i], &kLiftPos);
            mDoMtx_stack_c::transS(kLiftPos.x, kLiftPos.y, kLiftPos.z);
            MTXCopy(mDoMtx_stack_c::get(), mKLiftBaseMatrices[i]);
        }
    }
}

int daObjKWheel01_c::CreateHeap() {
    J3DModelData* model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, l_bmdidx[getOut()]));
    JUT_ASSERT(229, model_data != NULL);
    mpModel = mDoExt_J3DModel__create(model_data, (1 << 19), 0x11000084);

    if(!mpModel)
        return 0;

    for(int i = 0; i < 4; i++) {
        if(!CHECK_KLIFT_EXISTS(i)) {
            mKLiftCollisions[i] = NULL;
        }
        else {
            mKLiftCollisions[i] = new (dBgW);

            if(!mKLiftCollisions[i])
                return 0;

            if(mKLiftCollisions[i]->Set(static_cast<cBgD_t*>(dComIfG_getObjectRes(l_arcName, l_dzbidx[1])), 1, &mKLiftBaseMatrices[i])) {
                mKLiftCollisions[i] = NULL;
                return 0;
            }
        }
    }

    return 1;
}

int daObjKWheel01_c::Create() {
    mpModel->setBaseTRMtx(mTransformMtx);
    fopAcM_SetMtx(this, mTransformMtx);

    if(getSwNo() != 0xFF && fopAcM_isSwitch(this, getSwNo())) {
        #if DEBUG
        if(getArg0()) {
            mYAngularVelocity = l_HIO.mTargetYAngularSpeed;
        }
        else
            mYAngularVelocity = -l_HIO.mTargetYAngularSpeed;
        #else
        if(getArg0())
            mYAngularVelocity = 64;
        else
            mYAngularVelocity = -64;
        #endif
    }

    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);

    return 1;
}

static void* searchKWheel00(void* param_0, void* i_this) {
    i_this; // Needed to bring closer to matching debug REL
    if(param_0 && fopAcM_IsActor(param_0) && fopAcM_GetProfName(param_0) == PROC_Obj_KWheel00) {
        daObjKWheel00_c* const kWheel00 = static_cast<daObjKWheel00_c*>(param_0);
        if(kWheel00->getType() == daObjKWheel00_c::TYPE_LARGE_GOLD) {
            return kWheel00;
        }
    }

    return NULL;
}

int daObjKWheel01_c::Execute(Mtx** i_mtx) {
    eventUpdate();

    if(getSwNo() == 0xFF) {
        daObjKWheel00_c* const foundKWheel00 = static_cast<daObjKWheel00_c*>(fopAcM_Search(searchKWheel00, this));
        if(foundKWheel00) {
            if(getArg0())
                current.angle.y += foundKWheel00->current.angle.z - foundKWheel00->old.angle.z;
            else
                current.angle.y -= foundKWheel00->current.angle.z - foundKWheel00->old.angle.z;

            shape_angle.y = current.angle.y;

            if(current.angle.y != old.angle.y) {
                #if DEBUG
                mYAngularVelocity = l_HIO.mTargetYAngularSpeed;
                #else
                mYAngularVelocity = 64;
                #endif
            }
        }
    }
    else {
        if(mYAngularVelocity == 0) {
            if(fopAcM_isSwitch(this, getSwNo())) {
                if(getEvent() != 0xFF) {
                    const s32 eventIndex = dComIfGp_getEventManager().getEventIdx(this, getEvent());
                    setEvent(eventIndex, getEvent(), 1);
                }
                else {
                    eventStart();
                }
            }
        }

        if(mYAngularVelocity != 0) {
            #if DEBUG
            if(mYAngularVelocity > 0) {
                if(mYAngularVelocity < l_HIO.mTargetYAngularSpeed)
                    mYAngularVelocity += l_HIO.mYAngularAcceleration;
            }
            else if(mYAngularVelocity > -l_HIO.mTargetYAngularSpeed) {
                mYAngularVelocity -= l_HIO.mYAngularAcceleration;
            }
            #else
            if(mYAngularVelocity > 0) {
                if(mYAngularVelocity < 64)
                    mYAngularVelocity += 2;
            }
            else if(mYAngularVelocity > -64) {
                mYAngularVelocity -= 2;
            }
            #endif
        }

        current.angle.y += mYAngularVelocity;
        shape_angle.y = current.angle.y;
    }

    setMtx();
    mpModel->setBaseTRMtx(mTransformMtx);
    *i_mtx = &mNewBgMtx;

    for(int i = 0; i < 4; i++) {
        if(CHECK_KLIFT_EXISTS(i)) {
            daObjKLift00_c* const foundKLift = static_cast<daObjKLift00_c*>(fopAcM_SearchByID(m_klift_pid[i]));

            if(foundKLift) {
                cXyz kLiftOffset;
                MTXMultVec(mNewBgMtx, &l_pos[i], &kLiftOffset);

                foundKLift->current.pos = kLiftOffset;
                foundKLift->shape_angle.y += mpBgW->GetDiffShapeAngleY();
                foundKLift->home.angle.y += mpBgW->GetDiffShapeAngleY();
            }
        }
    }


    for(int i = 0; i < 4; i++) {
        if(CHECK_KLIFT_EXISTS(i)) 
            mKLiftCollisions[i]->Move();
    }

    // Stack ordering issues arise if mDoAud_seStartLevel is used
    if(mYAngularVelocity)
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_GEAR_LV, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

    return 1;
}

int daObjKWheel01_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return 1;
}

int daObjKWheel01_c::Delete() {
    dComIfG_resDelete(this, l_arcName);

    #if DEBUG
    l_HIO.removeHIO();
    #endif

    for(int i = 0; i < 4; i++) {
        if(CHECK_KLIFT_EXISTS(i) && mKLiftCollisions[i] && mKLiftCollisions[i]->ChkUsed())
            dComIfG_Bgsp().Release(mKLiftCollisions[i]);
    }

    return 1;
}

bool daObjKWheel01_c::eventStart() {
    #if DEBUG
    if(getArg0())
        mYAngularVelocity = l_HIO.mYAngularAcceleration;
    else
        mYAngularVelocity = -l_HIO.mYAngularAcceleration;
    #else
    if(getArg0())
        mYAngularVelocity = 2;
    else
        mYAngularVelocity = -2;
    #endif

    return true;
}

static actor_method_class daObjKWheel01_METHODS = {
    (process_method_func)daObjKWheel01_create1st,
    (process_method_func)daObjKWheel01_MoveBGDelete,
    (process_method_func)daObjKWheel01_MoveBGExecute,
    0,
    (process_method_func)daObjKWheel01_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_KWheel01 = {
  fpcLy_CURRENT_e,         // mLayerID
  3,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_KWheel01,       // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjKWheel01_c), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  663,                     // mPriority
  &daObjKWheel01_METHODS,  // sub_method
  0x00040100,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};

static int daObjKWheel01_create1st(daObjKWheel01_c* i_this) {
    fopAcM_ct(i_this, daObjKWheel01_c);
    return i_this->create1st();
}

static int daObjKWheel01_MoveBGDelete(daObjKWheel01_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjKWheel01_MoveBGExecute(daObjKWheel01_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjKWheel01_MoveBGDraw(daObjKWheel01_c* i_this) {
    return i_this->MoveBGDraw();
}
