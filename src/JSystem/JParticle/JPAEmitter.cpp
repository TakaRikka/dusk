#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAEmitterManager.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JParticle/JPABaseShape.h"
#include <dolphin/types.h>

JPAEmitterCallBack::~JPAEmitterCallBack() {
}

void JPABaseEmitter::init(JPAEmitterManager* param_0, JPAResource* param_1) {
    mpEmtrMgr = param_0;
    pRes = param_1;
    pRes->getDyn()->getEmitterScl(&mLocalScl);
    pRes->getDyn()->getEmitterTrs(&mLocalTrs);
    pRes->getDyn()->getEmitterDir(&mLocalDir);
    mLocalDir.normalize();
    pRes->getDyn()->getEmitterRot(&mLocalRot);
    mMaxFrame = pRes->getDyn()->getMaxFrame();
    mLifeTime = pRes->getDyn()->getLifetime();
    mVolumeSize = pRes->getDyn()->getVolumeSize();
    mRate = pRes->getDyn()->getRate();
    mRateStep = pRes->getDyn()->getRateStep();
    mVolumeSweep = pRes->getDyn()->getVolumeSweep();
    mVolumeMinRad = pRes->getDyn()->getVolumeMinRad();
    mAwayFromCenterSpeed = pRes->getDyn()->getInitVelOmni();
    mAwayFromAxisSpeed = pRes->getDyn()->getInitVelAxis();
    mDirSpeed = pRes->getDyn()->getInitVelDir();
    mSpread = pRes->getDyn()->getInitVelDirSp();
    mRndmDirSpeed = pRes->getDyn()->getInitVelRndm();
    mAirResist = pRes->getDyn()->getAirRes();
    mRndm.set_seed(mpEmtrMgr->pWd->mRndm.get_rndm_u());
    MTXIdentity(mGlobalRot);
    mGlobalScl.set(1.0f, 1.0f, 1.0f);
    mGlobalTrs.zero();
    mGlobalPScl.set(1.0f, 1.0f);
    mGlobalPrmClr.r = mGlobalPrmClr.g = mGlobalPrmClr.b = mGlobalPrmClr.a = mGlobalEnvClr.r = mGlobalEnvClr.g = mGlobalEnvClr.b = mGlobalEnvClr.a = 0xff;
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
        pRes->getDyn()->calc(mpEmtrMgr->pWd);
        node->getObject()->init_p(mpEmtrMgr->pWd);
        return node->getObject();
    } else {
        JUT_WARN(128, "%s", "JPA : Can NOT create particle more\n");
    }

    return NULL;
}

JPABaseParticle* JPABaseEmitter::createChild(JPABaseParticle* parent) {
    if (mpPtclPool->getNum() != 0) {
        JPANode<JPABaseParticle>* node = mpPtclPool->pop_front();
        mAlivePtclChld.push_front(node);
        node->getObject()->init_c(mpEmtrMgr->pWd, parent);
        return node->getObject();
    } else {
        JUT_WARN(151, "%s", "JPA : Can NOT create child particle more\n")
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
    if (mWaitTime >= pRes->getDyn()->getStartFrame())
        return true;

    if (!checkStatus(2)) {
        mWaitTime++;
    }

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
    MTXMultVec(mtx, &mLocalTrs, dst);
}

u32 JPABaseEmitter::getCurrentCreateNumber() const {
    return mpEmtrMgr->pWd->mEmitCount;
}

u8 JPABaseEmitter::getDrawCount() const {
    return mpEmtrMgr->pWd->mDrawCount;
}

bool JPABaseEmitter::loadTexture(u8 no, GXTexMapID texMapID) {
    JUT_ASSERT(286, pRes->texNum > no);
    mpEmtrMgr->pWd->mpResMgr->load(pRes->getTexIdx(no), texMapID);
    return true;
}
