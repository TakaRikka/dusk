/**
 * @file d_a_obj_klift00.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_klift00.h"
#include "JSystem/JHostIO/JORMContext.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_w.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"

struct daObjKLift00_HIO_c : public mDoHIO_entry_c {
    daObjKLift00_HIO_c();
    ~daObjKLift00_HIO_c() {};

    void genMessage(JORMContext*);

    /* 0x04 */ f32 mChainGravity;
    /* 0x08 */ f32 mRideParameters;
    /* 0x0C */ f32 mWindSwayOccuranceFactor;
    /* 0x10 */ f32 mWindMagnitudeChain;
    /* 0x14 */ f32 mWindMagnitudeFoundation;
    /* 0x18 */ f32 mChainHitSpeed;
    /* 0x1C */ f32 field_0x1C;
};

#if DEBUG
static daObjKLift00_HIO_c l_HIO;

daObjKLift00_HIO_c::daObjKLift00_HIO_c() {
    mChainGravity = -20.0f;
    mRideParameters = 0.0025f;
    mWindSwayOccuranceFactor = 0.05f;
    mWindMagnitudeChain = 0.05f;
    mWindMagnitudeFoundation = 1.25f;
    mChainHitSpeed = 15.0f;
    field_0x1C = 1.0f;
}

void daObjKLift00_HIO_c::genMessage(JORMContext* ctx) {
    // "Foothold"
    ctx->genLabel("足場", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Chain gravity"
    ctx->genSlider("チェイン重力", &mChainGravity, -40.0, 0.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Ride parameters"
    ctx->genSlider("Ride パラメータ", &mRideParameters, 0.0, 0.1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Wind effect occurence rate"
    ctx->genSlider("風影響発生率", &mWindSwayOccuranceFactor, 0.0, 0.5, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Chain・Wind"
    ctx->genSlider("鎖・風", &mWindMagnitudeChain, 0.0, 1000.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Foundation・Wind"
    ctx->genSlider("土台・風", &mWindMagnitudeFoundation, 0.0, 1000.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Chain hit speed"
    ctx->genSlider("鎖ヒット速度", &mChainHitSpeed, 0.0, 50.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Hammer adjustment"
    ctx->genSlider("ハンマー調整", &field_0x1C, 0.0, 10.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

static void rideCallBack(dBgW* unused, fopAc_ac_c* k_lift, fopAc_ac_c* riding_actor) {
    daObjKLift00_c* const kLift = static_cast<daObjKLift00_c*>(k_lift);
    fopAc_ac_c* const rideActor = riding_actor;

    kLift->rideActor(rideActor);
}

static const char* l_arcName = "K_lift00";

static dCcD_SrcSph l_cc_sph_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {(s32)0xd8fbfdff, 0x11}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcCyl l_cc_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x408000, 0x11}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        300.0f, // mRadius
        100.0f // mHeight
    } // mCyl
};

#if DEBUG
static const int l_dzbidx[] = {9};
#endif

// force dCcD_Sph::~dCcD_Sph to be emitted earlier than it otherwise would
void dummy() {
    delete (dCcD_Sph*)NULL;
    delete (dCcD_Cyl*)NULL;
}

cPhs__Step daObjKLift00_c::create1st() {
    mNumChainModels = getArg0();
    mNumChains = mNumChainModels + 1;

    cPhs__Step phase = static_cast<cPhs__Step>(dComIfG_resLoad(this, l_arcName));
    if(phase == cPhs_COMPLEATE_e) {
        #if DEBUG
        phase = static_cast<cPhs__Step>(MoveBGCreate(l_arcName, l_dzbidx[0], dBgS_MoveBGProc_TypicalRotY, 0x2000, NULL));
        #else
        phase = static_cast<cPhs__Step>(MoveBGCreate(l_arcName, 9, dBgS_MoveBGProc_TypicalRotY, 0x2000, NULL));
        #endif
        if(phase == cPhs_ERROR_e)
            return phase;
    }

    // "Foothold(Lv3)"
    #if DEBUG
    l_HIO.entryHIO("足場(Lv3)");
    #endif

    return phase;
}

static const int l_bmdidx[3] = {5, 6, 4};

void daObjKLift00_c::setMtx() {
    Mtx nonFoundationChainRotationMatrix;
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    if(!getNoBaseDisp())
        mpChainBase->setBaseTRMtx(mDoMtx_stack_c::get());

    MTXCopy(mDoMtx_stack_c::get(), mCullMtx);

    s16 negativeHomeYAngle = -home.angle.y;

    // Compute rotation matrices to make the chains' rotation match their change in position
    for(int i = 0; i < mNumChainModels; i++) {
        cXyz vectorToLowerChain =  mChainPositions[i + 1].mCurrentPos - mChainPositions[i].mCurrentPos;
        cXyz crossProductXAxis;
        VECCrossProduct(&cXyz::BaseZ, &vectorToLowerChain, &crossProductXAxis);

        if(!cM3d_IsZero(vectorToLowerChain.getSquareMag())) {
            vectorToLowerChain.normalize();
            f32 vectorToLowerChainNormalizedZComponent = VECDotProduct(&vectorToLowerChain, &cXyz::BaseZ);

            if(!cM3d_IsZero(crossProductXAxis.getSquareMag()) && -1.0f <= vectorToLowerChainNormalizedZComponent && vectorToLowerChainNormalizedZComponent <= 1.0f) {
                // |vectorToLowerChainNormalizedZComponent||BaseZ|cos(t) = 1 * 1 * cos(t) = cos(t)
                f32 currentChainPairZAngle = acos(vectorToLowerChainNormalizedZComponent);
                crossProductXAxis.normalize();
                MTXRotAxisRad(nonFoundationChainRotationMatrix, &crossProductXAxis, currentChainPairZAngle);
            }
            else {
                MTXIdentity(nonFoundationChainRotationMatrix);
            }
        }
        else {
            MTXIdentity(nonFoundationChainRotationMatrix);
        }

        negativeHomeYAngle += static_cast<s16>(0x4000);
        mDoMtx_stack_c::transS(mChainPositions[i].mCurrentPos);
        mDoMtx_stack_c::concat(nonFoundationChainRotationMatrix);
        mDoMtx_stack_c::ZrotM(negativeHomeYAngle);

        MtxP const now = mDoMtx_stack_c::get();
        mChainMdlObjs[i].setMtx(now);
    }

    Mtx foundationChainRotationMatrix;
    cXyz vectorFromSecondLowestChainToFoundation = mChainPositions[mNumChainModels].mCurrentPos - mChainPositions[mNumChainModels - 1].mCurrentPos;
    cXyz crossProduct;
    Vec inverseBaseY = {0.0f, -1.0f, 0.0f};
    if(!cM3d_IsZero(vectorFromSecondLowestChainToFoundation.getSquareMag())) {
        vectorFromSecondLowestChainToFoundation.normalize();
        VECCrossProduct(&inverseBaseY, &vectorFromSecondLowestChainToFoundation, &crossProduct);
        f32 dotProduct = VECDotProduct(&vectorFromSecondLowestChainToFoundation, &inverseBaseY);

        if(!cM3d_IsZero(crossProduct.getSquareMag()) && -1.0f <= dotProduct && dotProduct <= 1.0f) {
            f32 arcCos = acos(dotProduct);
            crossProduct.normalize();
            MTXRotAxisRad(foundationChainRotationMatrix, &crossProduct, arcCos);
        }
        else {
            MTXIdentity(foundationChainRotationMatrix);
        }
    }
    else {
        MTXIdentity(foundationChainRotationMatrix);
    }

    mDoMtx_stack_c::transS(mChainPositions[mNumChainModels].mCurrentPos);
    mDoMtx_stack_c::concat(foundationChainRotationMatrix);
    mDoMtx_stack_c::YrotM(-negativeHomeYAngle);
    mpLiftPlatform->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
    MTXCopy(mDoMtx_stack_c::get(), mNewBgMtx);
}

void daObjKLift00_c::rideActor(fopAc_ac_c* riding_actor) {
    if(!fopAcM_CheckCondition(this, fopAcCnd_NOEXEC_e)) {
        cXyz vectorFromFoundationChainToRideActor = riding_actor->current.pos - mChainPositions[mNumChains - 1].mCurrentPos;

        f32 weightMagnitude = 0.01f;

        if(!cM3d_IsZero(riding_actor->speed.getSquareMag()))
            weightMagnitude = 0.03f;

        #if DEBUG
        mChainPositions[mNumChains - 1].mDeltaPosVector.x -= vectorFromFoundationChainToRideActor.x * 0.0025f * (cM_rndFX(weightMagnitude) + l_HIO.mRideParameters);
        mChainPositions[mNumChains - 1].mDeltaPosVector.z -= vectorFromFoundationChainToRideActor.z * 0.0025f * (cM_rndFX(weightMagnitude) + l_HIO.mRideParameters);
        #else
        mChainPositions[mNumChains - 1].mDeltaPosVector.x -= vectorFromFoundationChainToRideActor.x * 0.0025f * (cM_rndFX(weightMagnitude) + 1.0f);
        mChainPositions[mNumChains - 1].mDeltaPosVector.z -= vectorFromFoundationChainToRideActor.z * 0.0025f * (cM_rndFX(weightMagnitude) + 1.0f);
        #endif
    }
}

int daObjKLift00_c::CreateHeap() {
    J3DModelData* const model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, l_bmdidx[2]));
    JUT_ASSERT(304, model_data != NULL);
    mpLiftPlatform = mDoExt_J3DModel__create(model_data, (1 << 19), 0x11000084);
    if(!mpLiftPlatform)
        return 0;

    if(!getNoBaseDisp()) {
        J3DModelData* const model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, l_bmdidx[0]));
        JUT_ASSERT(315, model_data != NULL);
        mpChainBase = mDoExt_J3DModel__create(model_data, (1 << 19), 0x11000084);
        if(!mpChainBase)
            return 0;
    }
    else {
        mpChainBase = NULL;
    }

    mChainPositions = new ChainPos[mNumChains];
    if(!mChainPositions)
        return 0;

    mChainModelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, l_bmdidx[1]));
    JUT_ASSERT(334, mChainModelData != NULL);

    mChainMdlObjs = new dMdl_obj_c[mNumChainModels];

    return mChainMdlObjs ? TRUE : FALSE;
}

int daObjKLift00_c::Create() {
    mpLiftPlatform->setBaseScale(scale);
    mChainPositions[0].mCurrentPos = current.pos; 
    mChainPositions[0].mDeltaPosVector = cXyz::Zero;

    for(int i = 1; i < mNumChains; i++) {
        mChainPositions[i].mDeltaPosVector = cXyz::Zero;

        mChainPositions[i].mCurrentPos.x = current.pos.x;
        mChainPositions[i].mCurrentPos.y = mChainPositions[i-1].mCurrentPos.y - 35.0f;
        mChainPositions[i].mCurrentPos.z = current.pos.z;
    }

    setMtx();
    fopAcM_SetMtx(this, mCullMtx);
    fopAcM_setCullSizeBox(this, -350.0f, (-mNumChainModels * 40.0f) - 100.0f, -350.0f, 350.0f, 10.0f, 350.0f);

    mpBgW->SetRideCallback(rideCallBack);
    mStts.Init(0xFF, 0, this);

    for(int i = 0; i < 8; i++) {
        mChainSphereColliders[i].Set(l_cc_sph_src);
        mChainSphereColliders[i].SetStts(&mStts);
    }

    mCylinderCollider.Set(l_cc_cyl_src);
    mCylinderCollider.SetStts(&mStts);
    mStopSwingingFrames = 0;

    if(getLock())
        mStopSwingingFrames = 5;

    return 1;
}

int daObjKLift00_c::Execute(Mtx** i_mtx) {
    // Apply wind sway
    #if DEBUG
    if(cM_rndF(1.0f) < l_HIO.mWindSwayOccuranceFactor) {
        for(int i = 1; i < mNumChains; i++) {
            cXyz windVector = dKyw_get_wind_vecpow();
            windVector *= cM_rndF(l_HIO.mWindMagnitudeChain);
            mChainPositions[i].mDeltaPosVector += windVector;

            if(i == mNumChains - 1) {
                windVector *= cM_rndF(l_HIO.mWindMagnitudeFoundation);
                mChainPositions[i].mDeltaPosVector += windVector;
            }
        }
    }
    #else
    if(cM_rndF(1.0f) < 0.05f) {
        for(int i = 1; i < mNumChains; i++) {
            cXyz windVector = dKyw_get_wind_vecpow();
            VECScale(&windVector, &windVector, cM_rndF(0.05f));
            VECAdd(&mChainPositions[i].mDeltaPosVector, &windVector, &mChainPositions[i].mDeltaPosVector);

            if(i == mNumChains - 1) {
                VECScale(&windVector, &windVector, cM_rndF(1.25f));
                VECAdd(&mChainPositions[i].mDeltaPosVector, &windVector, &mChainPositions[i].mDeltaPosVector);
            }
        }
    }
    #endif

    // Check if player hit chains, and play sfx & move the chains accordingly
    bool playedHitSfx = false;
    #if DEBUG
    for(int i = 0; i < 8; i++) {
        const s32 currentChainIdx = (mNumChains - 1) - (i * 2);
        if(currentChainIdx >= 0 && mChainSphereColliders[i].ChkTgHit() ) {
            dCcD_GObjInf* const hitObj = static_cast<dCcD_GObjInf*>(mChainSphereColliders[i].GetTgHitObj());
            fopAc_ac_c* const hitObjActor = dCc_GetAc(hitObj->GetAc());
            cXyz vectorFromHitActorToCurrentChain = mChainPositions[currentChainIdx].mCurrentPos - hitObjActor->current.pos;
            if(!cM3d_IsZero(vectorFromHitActorToCurrentChain.getSquareMag())) {
                vectorFromHitActorToCurrentChain.normalize();
                vectorFromHitActorToCurrentChain *= l_HIO.mChainHitSpeed;
                mChainPositions[currentChainIdx].mDeltaPosVector += vectorFromHitActorToCurrentChain;
            }

            if(!playedHitSfx) {
                playedHitSfx = true;
                Z2GetAudioMgr()->seStart(mChainSphereColliders[i].getHitSeID(hitObj->GetAtSe(), 0), mChainSphereColliders[i].GetCP(), 53, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        }
    }
    #else
    for(int i = 0; i < 8; i++) {
        const s32 currentChainIdx = (mNumChains - 1) - (i * 2);
        if(currentChainIdx >= 0 && mChainSphereColliders[i].ChkTgHit() ) {
            dCcD_GObjInf* const hitObj = static_cast<dCcD_GObjInf*>(mChainSphereColliders[i].GetTgHitObj());
            fopAc_ac_c* const hitObjActor = dCc_GetAc(hitObj->GetAc());
            cXyz vectorFromHitActorToCurrentChain = mChainPositions[currentChainIdx].mCurrentPos - hitObjActor->current.pos;
            if(!cM3d_IsZero(vectorFromHitActorToCurrentChain.getSquareMag())) {
                vectorFromHitActorToCurrentChain.normalize();
                VECScale(&vectorFromHitActorToCurrentChain, &vectorFromHitActorToCurrentChain, 15.0f);
                VECAdd(&mChainPositions[currentChainIdx].mDeltaPosVector, &vectorFromHitActorToCurrentChain, &mChainPositions[currentChainIdx].mDeltaPosVector);
            }

            if(!playedHitSfx) {
                playedHitSfx = true;
                Z2GetAudioMgr()->seStart(mChainSphereColliders[i].getHitSeID(hitObj->GetAtSe(), 0), mChainSphereColliders[i].GetCP(), 53, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        }
    }
    #endif

    // Apply chain gravity
    #if DEBUG
    mChainPositions[0].mDeltaPosVector = cXyz::Zero;
    mChainPositions[0].mCurrentPos = current.pos;
    for(int i = 1; i < mNumChains; i++) {
        mChainPositions[i].mPrevPos = mChainPositions[i].mCurrentPos;
        mChainPositions[i].mDeltaPosVector.y += l_HIO.mChainGravity;
        mChainPositions[i].mCurrentPos += mChainPositions[i].mDeltaPosVector;
    }
    #else
    mChainPositions[0].mDeltaPosVector = cXyz::Zero;
    mChainPositions[0].mCurrentPos = current.pos;
    for(int i = 1; i < mNumChains; i++) {
        mChainPositions[i].mPrevPos = mChainPositions[i].mCurrentPos;
        mChainPositions[i].mDeltaPosVector.y += -4.0f;
        VECAdd(&mChainPositions[i].mCurrentPos, &mChainPositions[i].mDeltaPosVector, &mChainPositions[i].mCurrentPos);
    }
    #endif

    for(int i = 1; i < mNumChains - 1; i++) {
        if(i != mNumChains - 1) {
            mChainPositions[i].mCurrentPos = (mChainPositions[i - 1].mCurrentPos + mChainPositions[i].mCurrentPos + mChainPositions[i + 1].mCurrentPos) * 0.33333334f;
        }
    }

    for(int i = 1; i < mNumChains; i++) {
        cXyz vectorFromChainAboveToCurrentChain = mChainPositions[i].mCurrentPos - mChainPositions[i - 1].mCurrentPos;
        if(mStopSwingingFrames > 0) {
            vectorFromChainAboveToCurrentChain.set(0.0f, -1.0f, 0.0f); 
        }
        else {
            if(!cM3d_IsZero(vectorFromChainAboveToCurrentChain.getSquareMag()))
                vectorFromChainAboveToCurrentChain.normalize(); 
            else
                vectorFromChainAboveToCurrentChain.set(0.0f, -1.0f, 0.0f); 
        }
        #if DEBUG
            vectorFromChainAboveToCurrentChain *= 35.0f;
        #else
        VECScale(&vectorFromChainAboveToCurrentChain, &vectorFromChainAboveToCurrentChain, 35.0f);
        #endif
        mChainPositions[i].mCurrentPos = vectorFromChainAboveToCurrentChain + mChainPositions[i - 1].mCurrentPos;
        mChainPositions[i].mDeltaPosVector = mChainPositions[i].mCurrentPos - mChainPositions[i].mPrevPos;
    }

    if(mStopSwingingFrames > 0) {
        mStopSwingingFrames--;
        if(mStopSwingingFrames < 0)
            mStopSwingingFrames = 0;
    }

    for(int i = 0; i < 8; i++) {
        const s32 currentChainIdx = (mNumChains - 1) - (i * 2);
        if(currentChainIdx >= 0) {
            mChainSphereColliders[i].SetC(mChainPositions[currentChainIdx].mCurrentPos);
            dComIfG_Ccsp()->Set(&mChainSphereColliders[i]);
        }
    }

    mStts.Move();
    setMtx();
    *i_mtx = &mNewBgMtx;

    return 1;
}

int daObjKLift00_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpLiftPlatform, &tevStr);

    if(!getNoBaseDisp())
        g_env_light.setLightTevColorType_MAJI(mpChainBase, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpLiftPlatform);
    
    if(!getNoBaseDisp())
        mDoExt_modelUpdateDL(mpChainBase);

    dMdl_c* const dMdl = dMdl_mng_c::entry(mChainModelData, 0, current.roomNo);
    if(dMdl) {
        for(int i = 0; i < mNumChainModels; i++)
            dMdl->entryObj(&mChainMdlObjs[i]);
    }

    dComIfGd_setList();

    return 1;
}

int daObjKLift00_c::Delete() {
    dComIfG_resDelete(this, l_arcName);

    #if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

static int daObjKLift00_create1st(daObjKLift00_c* i_this) {
    fopAcM_ct(i_this, daObjKLift00_c);
    return i_this->create1st();
}

static int daObjKLift00_MoveBGDelete(daObjKLift00_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjKLift00_MoveBGExecute(daObjKLift00_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjKLift00_MoveBGDraw(daObjKLift00_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjKLift00_METHODS = {
    (process_method_func)daObjKLift00_create1st,
    (process_method_func)daObjKLift00_MoveBGDelete,
    (process_method_func)daObjKLift00_MoveBGExecute,
    0,
    (process_method_func)daObjKLift00_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_KLift00 = {
    fpcLy_CURRENT_e,        // mLayerID
    3,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_KLift00,       // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daObjKLift00_c), // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    673,                    // mPriority
    &daObjKLift00_METHODS,  // sub_method
    0x00040100,             // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
  };
