#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAExtraShape.h"
#include "JSystem/JMath/JMATrigonometric.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "dolphin/os.h"

void JPACalcScaleX(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    JPAExtraShape* esp = work->mpRes->getEsp();
    if (work->mScaleAnm < esp->getScaleInTiming()) {
        ptcl->mParticleScaleX =
            ptcl->mScaleOut * (esp->getScaleIncRateX() * work->mScaleAnm + esp->getScaleInValueX());
    } else if (work->mScaleAnm > esp->getScaleOutTiming()) {
        ptcl->mParticleScaleX =
            ptcl->mScaleOut *
            (esp->getScaleDecRateX() * (work->mScaleAnm - esp->getScaleOutTiming()) + 1.0f);
    } else {
        ptcl->mParticleScaleX = ptcl->mScaleOut;
    }
}

void JPACalcScaleY(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    JPAExtraShape* esp = work->mpRes->getEsp();
    if (work->mScaleAnm < esp->getScaleInTiming()) {
        ptcl->mParticleScaleY =
            ptcl->mScaleOut * (esp->getScaleIncRateY() * work->mScaleAnm + esp->getScaleInValueY());
    } else if (work->mScaleAnm > esp->getScaleOutTiming()) {
        ptcl->mParticleScaleY =
            ptcl->mScaleOut *
            (esp->getScaleDecRateY() * (work->mScaleAnm - esp->getScaleOutTiming()) + 1.0f);
    } else {
        ptcl->mParticleScaleY = ptcl->mScaleOut;
    }
}

void JPACalcScaleCopy(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    JPAExtraShape* esp = work->mpRes->getEsp();
    ptcl->mParticleScaleY = ptcl->mParticleScaleX;
}

void JPACalcScaleAnmNormal(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    work->mScaleAnm = ptcl->mTime;
}

void JPACalcScaleAnmRepeatX(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    JPAExtraShape* esp = work->mpRes->getEsp();
    work->mScaleAnm = (ptcl->mAge % esp->getScaleAnmCycleX()) / (f32)esp->getScaleAnmCycleX();
}

void JPACalcScaleAnmRepeatY(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    JPAExtraShape* esp = work->mpRes->getEsp();
    work->mScaleAnm = (ptcl->mAge % esp->getScaleAnmCycleY()) / (f32)esp->getScaleAnmCycleY();
}

void JPACalcScaleAnmReverseX(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    JPAExtraShape* esp = work->mpRes->getEsp();
    s32 cycle = ptcl->mAge / esp->getScaleAnmCycleX();
    f32 base = (ptcl->mAge % esp->getScaleAnmCycleX()) / (f32)esp->getScaleAnmCycleX();
    work->mScaleAnm = base + ((cycle & 1) * (1.0f - base * 2.0f));
}

void JPACalcScaleAnmReverseY(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    JPAExtraShape* esp = work->mpRes->getEsp();
    s32 cycle = ptcl->mAge / esp->getScaleAnmCycleY();
    f32 base = (ptcl->mAge % esp->getScaleAnmCycleY()) / (f32)esp->getScaleAnmCycleY();
    work->mScaleAnm = base + ((cycle & 1) * (1.0f - base * 2.0f));
}

void JPACalcAlphaAnm(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    JPAExtraShape* esp = work->mpRes->getEsp();
    f32 alpha;
    if (ptcl->mTime < esp->getAlphaInTiming()) {
        alpha = 255.0f * (esp->getAlphaInValue() + esp->getAlphaIncRate() * ptcl->mTime);
    } else if (ptcl->mTime > esp->getAlphaOutTiming()) {
        alpha = 255.0f * ((ptcl->mTime - esp->getAlphaOutTiming()) * esp->getAlphaDecRate() +
                          esp->getAlphaBaseValue());
    } else {
        alpha = 255.0f * esp->getAlphaBaseValue();
    }
    OSf32tou8(&alpha, &ptcl->mPrmColorAlphaAnm);
}

void JPACalcAlphaFlickAnm(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    JPAExtraShape* esp = work->mpRes->getEsp();
    f32 alpha;
    if (ptcl->mTime < esp->getAlphaInTiming()) {
        alpha = (esp->getAlphaInValue() + esp->getAlphaIncRate() * ptcl->mTime);
    } else if (ptcl->mTime > esp->getAlphaOutTiming()) {
        alpha = ((ptcl->mTime - esp->getAlphaOutTiming()) * esp->getAlphaDecRate() +
                 esp->getAlphaBaseValue());
    } else {
        alpha = esp->getAlphaBaseValue();
    }
    s32 theta = ptcl->mAlphaWaveRandom * ptcl->mAge * 16384.0f * (1.0f - esp->getAlphaFreq());
    f32 wave = JMASSin(theta);
    alpha *= (1.0f + esp->getAlphaAmp() * (wave - 1.0f) * 0.5f) * 255.0f;
    OSf32tou8(&alpha, &ptcl->mPrmColorAlphaAnm);
}

JPAExtraShape::JPAExtraShape(u8 const* data) {
    mpData = (const JPAExtraShapeData*)data;
    init();
}

void JPAExtraShape::init() {
    mAlphaIncRate = (getAlphaInTiming() != 0.0f) ?
                        (getAlphaBaseValue() - getAlphaInValue()) / getAlphaInTiming() :
                        1.0f;
    mAlphaDecRate = (getAlphaOutTiming() != 1.0f) ?
                        (getAlphaOutValue() - getAlphaBaseValue()) / (1.0f - getAlphaOutTiming()) :
                        1.0f;

    if (getScaleInTiming() != 0.0f) {
        mScaleIncRateX = (1.0f - getScaleInValueX()) / getScaleInTiming();
        mScaleIncRateY = (1.0f - getScaleInValueY()) / getScaleInTiming();
    } else {
        mScaleIncRateX = mScaleIncRateY = 1.0f;
    }

    if (getScaleOutTiming() != 1.0f) {
        mScaleDecRateX = (getScaleOutValueX() - 1.0f) / (1.0f - getScaleOutTiming());
        mScaleDecRateY = (getScaleOutValueY() - 1.0f) / (1.0f - getScaleOutTiming());
    } else {
        mScaleDecRateX = mScaleDecRateY = 1.0f;
    }
}
