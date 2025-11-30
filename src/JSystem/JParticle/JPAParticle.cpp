#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPABaseShape.h"
#include "JSystem/JParticle/JPAChildShape.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAEmitterManager.h"
#include "JSystem/JParticle/JPAExtraShape.h"

JPAParticleCallBack::~JPAParticleCallBack() {
    /* empty function */
}

void JPABaseParticle::init_p(JPAEmitterWorkData* work) {
    JPABaseEmitter* emtr = work->mpEmtr;
    JPAExtraShape* esp = work->mpRes->getEsp();
    JPABaseShape* bsp = work->mpRes->getBsp();
    JPADynamicsBlock* dyn = work->mpRes->getDyn();

    mAge = -1;
    mLifeTime = (1.0f - dyn->getLifetimeRndm() * emtr->get_r_f()) * emtr->mLifeTime;
    mTime = 0.0f;

    initStatus(0);
    MTXMultVecSR(work->mGlobalSR, &work->mVolumeCalcData.mVolumePos, &mLocalPosition);
    if (emtr->checkFlag(8)) {
        setStatus(0x20);
    }

    mOffsetPosition.set(work->mGlobalPos);
    mPosition.set(mOffsetPosition.x + mLocalPosition.x * work->mPublicScale.x,
                  mOffsetPosition.y + mLocalPosition.y * work->mPublicScale.y,
                  mOffsetPosition.z + mLocalPosition.z * work->mPublicScale.z);

    JGeometry::TVec3<f32> velOmni; 
    if (emtr->mAwayFromCenterSpeed) {
        velOmni.setLength(work->mVolumeCalcData.mVelOmni, emtr->mAwayFromCenterSpeed);
    } else {
        velOmni.zero();
    }

    JGeometry::TVec3<f32> velAxis;
    if (emtr->mAwayFromAxisSpeed) {
        velAxis.setLength(work->mVolumeCalcData.mVelAxis, emtr->mAwayFromAxisSpeed);
    } else {
        velAxis.zero();
    }

    JGeometry::TVec3<f32> velDir;
    if (emtr->mDirSpeed) {
        Mtx mtx;
        JPAGetYZRotateMtx(emtr->get_r_zp() * 0x8000 * emtr->mSpread, emtr->get_r_ss(), mtx);
        MTXConcat(work->mDirectionMtx, mtx, mtx);
        velDir.set(emtr->mDirSpeed * mtx[0][2],
                   emtr->mDirSpeed * mtx[1][2],
                   emtr->mDirSpeed * mtx[2][2]);
    } else {
        velDir.zero();
    }

    JGeometry::TVec3<f32> velRndm;
    if (emtr->mRndmDirSpeed) {
        velRndm.set(emtr->mRndmDirSpeed * emtr->get_r_zh(),
                    emtr->mRndmDirSpeed * emtr->get_r_zh(),
                    emtr->mRndmDirSpeed * emtr->get_r_zh());
    } else {
        velRndm.zero();
    }

    f32 ratio = emtr->get_r_zp() * dyn->getInitVelRatio() + 1.0f;
    mVelType1.set(ratio * (velOmni.x + velAxis.x + velDir.x + velRndm.x),
                  ratio * (velOmni.y + velAxis.y + velDir.y + velRndm.y),
                  ratio * (velOmni.z + velAxis.z + velDir.z + velRndm.z));
    
    if (emtr->checkFlag(4)) {
        mVelType1.mul(emtr->mLocalScl);
    }
    MTXMultVecSR(work->mGlobalRot, &mVelType1, &mVelType1);

    mVelType0.zero();

    mMoment = 1.0f - dyn->getMomentRndm() * emtr->get_r_f();
    mDrag = 1.0f;
    field_0x78 = 0;

    mBaseAxis.set(work->mGlobalRot[0][1], work->mGlobalRot[1][1], work->mGlobalRot[2][1]);

    mPrmClr = emtr->mPrmClr;
    mEnvClr = emtr->mEnvClr;

    mAnmRandom = emtr->get_r_f() * bsp->getLoopOfstValue();

    if (esp != NULL && esp->isEnableScaleAnm()) {
        mParticleScaleX = mParticleScaleY = mScaleOut = emtr->mScaleOut * (emtr->get_r_zp() * esp->getScaleRndm() + 1.0f);
    } else {
        mParticleScaleX = mParticleScaleY = mScaleOut = emtr->mScaleOut;
    }

    mPrmColorAlphaAnm = 0xff;

    if (esp != NULL && esp->isEnableAlphaFlick()) {
        mAlphaWaveRandom = emtr->get_r_zp() * esp->getAlphaFreqRndm() + 1.0f;
    } else {
        mAlphaWaveRandom = 1.0f;
    }

    if (esp != NULL) {
        if (esp->isEnableRotateAnm()) {
            mRotateAngle = esp->getRotateInitAngle() + esp->getRotateRndmAngle() * emtr->get_r_zh();
            mRotateSpeed = esp->getRotateInitSpeed() *
                           (esp->getRotateRndmSpeed() * emtr->get_r_zp() + 1.0f);
            mRotateSpeed = emtr->get_r_zp() < esp->getRotateDirection() ?
                           mRotateSpeed : (s16)-mRotateSpeed;
        } else {
            mRotateAngle = 0;
            mRotateSpeed = 0;
        }
    } else {
        mRotateAngle = 0;
        mRotateSpeed = 0;
    }
}

void JPABaseParticle::init_c(JPAEmitterWorkData* work, JPABaseParticle* parent) {
    JPABaseEmitter* emtr = work->mpEmtr;
    JPAChildShape* csp = work->mpRes->getCsp();

    mAge = -1;
    mLifeTime = csp->getLife();
    mTime = 0.0f;
    initStatus(4);
    mLocalPosition.set(parent->mLocalPosition);

    f32 pos_rndm = csp->getPosRndm();
    if (pos_rndm != 0.0f) {
        JGeometry::TVec3<f32> rnd(emtr->get_r_zh(), emtr->get_r_zh(), emtr->get_r_zh());
        rnd.setLength(pos_rndm * emtr->get_r_f());
        mLocalPosition.add(rnd);
    }

    if (emtr->checkFlag(0x10)) {
        setStatus(0x20);
    }

    mOffsetPosition.set(parent->mOffsetPosition);

    f32 base_speed = csp->getBaseVel() * (csp->getBaseVelRndm() * emtr->get_r_zp() + 1.0f);
    JGeometry::TVec3<f32> base_vel(emtr->get_r_zp(), emtr->get_r_zp(), emtr->get_r_zp());
    base_vel.setLength(base_speed);
    mVelType1.scaleAdd(csp->getVelInhRate(), parent->mVelType1, base_vel);
    mVelType0.scale(csp->getVelInhRate(), parent->mVelType2);
    
    mMoment = parent->mMoment;
    if (csp->isFieldAffected()) {
        mDrag = parent->mDrag;
    } else {
        setStatus(0x40);
        mDrag = 1.0f;
    }
    field_0x78 = parent->field_0x78;

    mVelType2.set(mVelType0);
    f32 ratio = mMoment * mDrag;
    mVelocity.set(ratio * (mVelType1.x + mVelType2.x),
                  ratio * (mVelType1.y + mVelType2.y),
                  ratio * (mVelType1.z + mVelType2.z));
    
    mBaseAxis.set(parent->mBaseAxis);

    if (csp->isScaleInherited()) {
        mParticleScaleX = mScaleOut = parent->mParticleScaleX * csp->getScaleInhRate();
        mParticleScaleY = mAlphaWaveRandom = parent->mParticleScaleY * csp->getScaleInhRate();
    } else {
        mParticleScaleX = mScaleOut = mParticleScaleY = mAlphaWaveRandom = 1.0f;
    }

    if (csp->isColorInherited()) {
        mPrmClr.r = parent->mPrmClr.r * csp->getColorInhRate();
        mPrmClr.g = parent->mPrmClr.g * csp->getColorInhRate();
        mPrmClr.b = parent->mPrmClr.b * csp->getColorInhRate();
        mEnvClr.r = parent->mEnvClr.r * csp->getColorInhRate();
        mEnvClr.g = parent->mEnvClr.g * csp->getColorInhRate();
        mEnvClr.b = parent->mEnvClr.b * csp->getColorInhRate();
    } else {
        csp->getPrmClr(&mPrmClr);
        csp->getEnvClr(&mEnvClr);
    }

    mPrmColorAlphaAnm = 0xff;
    if (csp->isAlphaInherited()) {
        mPrmClr.a = COLOR_MULTI(parent->mPrmClr.a, parent->mPrmColorAlphaAnm)
                    * csp->getAlphaInhRate();
    } else {
        mPrmClr.a = csp->getPrmAlpha();
    }

    mRotateAngle = parent->mRotateAngle;
    if (csp->isRotateOn()) {
        mRotateSpeed = csp->getRotInitSpeed();
    } else {
        mRotateSpeed = 0;
    }

    mTexAnmIdx = 0;
}

bool JPABaseParticle::calc_p(JPAEmitterWorkData* work) {
    if (++mAge >= mLifeTime) {
        return true;
    }
    mTime = (f32)mAge / (f32)mLifeTime;

    if (checkStatus(0x20)) {
        mOffsetPosition.set(work->mGlobalPos);
    }

    mVelType2.zero();
    if (!checkStatus(0x40)) {
        work->mpRes->calcField(work, this);
    }
    mVelType2.add(mVelType0);
    mVelType1.scale(work->mpEmtr->mAirResist);
    f32 ratio = mMoment * mDrag;
    mVelocity.set(ratio * (mVelType1.x + mVelType2.x),
                  ratio * (mVelType1.y + mVelType2.y),
                  ratio * (mVelType1.z + mVelType2.z));
    
    if (work->mpEmtr->mpPtclCallBack != NULL) {
        work->mpEmtr->mpPtclCallBack->execute(work->mpEmtr, this);
    }

    if (checkStatus(2)) {
        return true;
    }

    work->mpRes->calc_p(work, this);
    mRotateAngle += mRotateSpeed;

    if (work->mpRes->getCsp() != NULL && canCreateChild(work)) {
        for (int i = work->mpRes->getCsp()->getRate(); i > 0; i--) {
            work->mpEmtr->createChild(this);
        }
    }

    mLocalPosition.add(mVelocity);
    mPosition.set(mOffsetPosition.x + mLocalPosition.x * work->mPublicScale.x,
                  mOffsetPosition.y + mLocalPosition.y * work->mPublicScale.y,
                  mOffsetPosition.z + mLocalPosition.z * work->mPublicScale.z);

    return false;
}

bool JPABaseParticle::calc_c(JPAEmitterWorkData* work) {
    if (++mAge >= mLifeTime) {
        return true;
    }
    mTime = (f32)mAge / (f32)mLifeTime;

    if (mAge != 0) {
        if (checkStatus(0x20)) {
            mOffsetPosition.set(work->mGlobalPos);
        }

        mVelType1.y -= work->mpRes->getCsp()->getGravity();
        mVelType2.zero();
        if (!checkStatus(0x40)) {
            work->mpRes->calcField(work, this);
        }
        mVelType2.add(mVelType0);
        mVelType1.scale(work->mpEmtr->mAirResist);
        f32 ratio = mMoment * mDrag;
        mVelocity.set(ratio * (mVelType1.x + mVelType2.x),
                    ratio * (mVelType1.y + mVelType2.y),
                    ratio * (mVelType1.z + mVelType2.z));
    }
    
    if (work->mpEmtr->mpPtclCallBack != NULL) {
        work->mpEmtr->mpPtclCallBack->execute(work->mpEmtr, this);
    }

    if (checkStatus(2)) {
        return true;
    }

    work->mpRes->calc_c(work, this);
    mRotateAngle += mRotateSpeed;
    mLocalPosition.add(mVelocity);
    mPosition.set(mOffsetPosition.x + mLocalPosition.x * work->mPublicScale.x,
                  mOffsetPosition.y + mLocalPosition.y * work->mPublicScale.y,
                  mOffsetPosition.z + mLocalPosition.z * work->mPublicScale.z);

    return false;
}

bool JPABaseParticle::canCreateChild(JPAEmitterWorkData* work) {
    JPAChildShape* csp = work->mpRes->getCsp();
    bool ret = false;
    int time = mAge - (int)((mLifeTime - 1) * csp->getTiming());
    if (time >= 0 && time % (csp->getStep() + 1) == 0) {
        ret = true;
    }
    return ret;
}

f32 JPABaseParticle::getWidth(JPABaseEmitter const* emtr) const {
    return mParticleScaleX * 2.0f * emtr->mpEmtrMgr->pWd->mGlobalPtclScl.x;
}

f32 JPABaseParticle::getHeight(JPABaseEmitter const* emtr) const {
    return mParticleScaleY * 2.0f * emtr->mpEmtrMgr->pWd->mGlobalPtclScl.y;
}
