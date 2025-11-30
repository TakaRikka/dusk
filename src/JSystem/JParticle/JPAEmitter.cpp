#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAEmitterManager.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JParticle/JPABaseShape.h"
#include "dolphin/types.h"

JPAEmitterCallBack::~JPAEmitterCallBack() {
}

void JPABaseEmitter::init(JPAEmitterManager* param_0, JPAResource* param_1) {
    mpEmtrMgr = param_0;
    mpRes = param_1;
    mpRes->getDyn()->getEmitterScl(&mLocalScl);
    mpRes->getDyn()->getEmitterTrs(&mLocalTrs);
    mpRes->getDyn()->getEmitterDir(&mLocalDir);
    mLocalDir.normalize();
    mpRes->getDyn()->getEmitterRot(&mLocalRot);
    mMaxFrame = mpRes->getDyn()->getMaxFrame();
    mLifeTime = mpRes->getDyn()->getLifetime();
    mVolumeSize = mpRes->getDyn()->getVolumeSize();
    mRate = mpRes->getDyn()->getRate();
    mRateStep = mpRes->getDyn()->getRateStep();
    mVolumeSweep = mpRes->getDyn()->getVolumeSweep();
    mVolumeMinRad = mpRes->getDyn()->getVolumeMinRad();
    mAwayFromCenterSpeed = mpRes->getDyn()->getInitVelOmni();
    mAwayFromAxisSpeed = mpRes->getDyn()->getInitVelAxis();
    mDirSpeed = mpRes->getDyn()->getInitVelDir();
    mSpread = mpRes->getDyn()->getInitVelDirSp();
    mRndmDirSpeed = mpRes->getDyn()->getInitVelRndm();
    mAirResist = mpRes->getDyn()->getAirRes();
    mRndm.set_seed(mpEmtrMgr->pWd->mRndm.get_rndm_u());
    MTXIdentity(mGlobalRot);
    mGlobalScl.set(1.0f, 1.0f, 1.0f);
    mGlobalTrs.zero();
    mGlobalPScl.set(1.0f, 1.0f);
    mGlobalEnvClr.a = 0xff;
    mGlobalEnvClr.b = 0xff;
    mGlobalEnvClr.g = 0xff;
    mGlobalEnvClr.r = 0xff;
    mGlobalPrmClr.a = 0xff;
    mGlobalPrmClr.b = 0xff;
    mGlobalPrmClr.g = 0xff;
    mGlobalPrmClr.r = 0xff;
    param_1->getBsp()->getPrmClr(&mPrmClr);
    param_1->getBsp()->getEnvClr(&mEnvClr);
    mpUserWork = NULL;
    mScaleOut = 1.0f;
    mEmitCount = 0.0f;
    initStatus(0x30);
    mDrawTimes = 1;
    mTick = 0;
    mWaitTime = 0;
    mRateStepTimer = 0;
    mTexAnmIdx = 0;
}

JPABaseParticle* JPABaseEmitter::createParticle() {
    if (mpPtclPool->getNum() != 0) {
        JPANode<JPABaseParticle>* node = mpPtclPool->pop_front();
        mAlivePtclBase.push_front(node);
        mpRes->getDyn()->calc(mpEmtrMgr->pWd);
        node->mData.init_p(mpEmtrMgr->pWd);
        return &node->mData;
    }

    return NULL;
}

JPABaseParticle* JPABaseEmitter::createChild(JPABaseParticle* parent) {
    if (mpPtclPool->getNum() != 0) {
        JPANode<JPABaseParticle>* node = mpPtclPool->pop_front();
        mAlivePtclChld.push_front(node);
        node->mData.init_c(mpEmtrMgr->pWd, parent);
        return &node->mData;
    }

    return NULL;
}

void JPABaseEmitter::deleteAllParticle() {
    while (mAlivePtclBase.getNum())
        mpPtclPool->push_front(mAlivePtclBase.pop_back());
    while (mAlivePtclChld.getNum())
        mpPtclPool->push_front(mAlivePtclChld.pop_back());
}

bool JPABaseEmitter::processTillStartFrame() {
    JPADynamicsBlock* dyn = mpRes->getDyn();
    s16 startFrame = dyn->getStartFrame();

    if (mWaitTime >= startFrame)
        return true;

    if (!(mStatus & 2))
        mWaitTime++;

    return false;
}

bool JPABaseEmitter::processTermination() {
    if (checkStatus(0x100)) {
        return true;
    }

    if (mMaxFrame == 0) {
        return false;
    }
    if (mMaxFrame < 0) {
        setStatus(8);
        return getParticleNumber() == 0;
    } 
    if (mTick >= mMaxFrame) {
        setStatus(8);
        if (checkStatus(0x40)) {
            return 0;
        }
        return getParticleNumber() == 0;
    }
    return false;
}

void JPABaseEmitter::calcEmitterGlobalPosition(JGeometry::TVec3<f32>* dst) const {
    Mtx mtx;
    MTXScale(mtx, mGlobalScl.x, mGlobalScl.y, mGlobalScl.z);
    MTXConcat(mGlobalRot, mtx, mtx);
    mtx[0][3] = mGlobalTrs.x;
    mtx[1][3] = mGlobalTrs.y;
    mtx[2][3] = mGlobalTrs.z;
    MTXMultVec(mtx, mLocalTrs, *dst);
}

u32 JPABaseEmitter::getCurrentCreateNumber() const {
    return mpEmtrMgr->pWd->mEmitCount;
}

u8 JPABaseEmitter::getDrawCount() const {
    return mpEmtrMgr->pWd->mDrawCount;
}

bool JPABaseEmitter::loadTexture(u8 idx, GXTexMapID texMapID) {
    mpEmtrMgr->pWd->mpResMgr->load(mpRes->getTexIdx(idx), texMapID);
    return true;
}
