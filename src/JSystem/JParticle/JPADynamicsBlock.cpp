#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPADynamicsBlock.h"
#include "JSystem/JMath/JMATrigonometric.h"
#include "JSystem/JParticle/JPAEmitter.h"

void JPAVolumePoint(JPAEmitterWorkData* work) {
    work->mVolumeCalcData.mVolumePos.zero();
    work->mVolumeCalcData.mVelOmni.set(work->mpEmtr->get_r_zh(), work->mpEmtr->get_r_zh(),
                                       work->mpEmtr->get_r_zh());
    work->mVolumeCalcData.mVelAxis.set(work->mVolumeCalcData.mVelOmni.x, 0.0f,
                                       work->mVolumeCalcData.mVelOmni.z);
}

void JPAVolumeLine(JPAEmitterWorkData* work) {
    if (work->mpEmtr->checkFlag(JPADynFlag_FixedInterval)) {
        work->mVolumeCalcData.mVolumePos.set(
            0.0f, 0.0f,
            work->mVolumeSize * ((work->mVolumeEmitIdx / (work->mEmitCount - 1.0f) - 0.5f)));
        work->mVolumeEmitIdx++;
    } else {
        work->mVolumeCalcData.mVolumePos.set(0.0f, 0.0f,
                                             work->mVolumeSize * work->mpEmtr->get_r_zh());
    }

    work->mVolumeCalcData.mVelOmni.set(0.0f, 0.0f,
                                       work->mVolumeCalcData.mVolumePos.z * work->mGlobalScl.z);
    work->mVolumeCalcData.mVelAxis.set(0.0f, 0.0f, work->mVolumeCalcData.mVolumePos.z);
}

// NONMATCHING regalloc. Could be issue with mul asm implementations
void JPAVolumeCircle(JPAEmitterWorkData* work) {
    s16 theta;
    f32 distance;
    
    if (work->mpEmtr->checkFlag(JPADynFlag_FixedInterval)) {
        theta = (s16)((work->mVolumeEmitIdx << 16) / work->mEmitCount);
        theta = theta * work->mVolumeSweep;
        work->mVolumeEmitIdx++;
    } else {
        theta = work->mVolumeSweep * work->mpEmtr->get_r_ss();
    }

    distance = work->mpEmtr->get_r_f();
    if (work->mpEmtr->checkFlag(JPADynFlag_FixedDensity)) {
        distance = 1.0f - (distance * distance);
    }

    distance = work->mVolumeSize * (work->mVolumeMinRad + distance * (1.0f - work->mVolumeMinRad));
    work->mVolumeCalcData.mVolumePos.set(distance * JMASSin(theta), 0.0f,
                                         distance * JMASCos(theta));
    work->mVolumeCalcData.mVelOmni.mul(work->mVolumeCalcData.mVolumePos, work->mGlobalScl);
    work->mVolumeCalcData.mVelAxis.set(work->mVolumeCalcData.mVolumePos.x, 0.0f,
                                       work->mVolumeCalcData.mVolumePos.z);
}

void JPAVolumeCube(JPAEmitterWorkData* work) {
    work->mVolumeCalcData.mVolumePos.set(work->mpEmtr->get_r_zh() * work->mVolumeSize,
                         work->mpEmtr->get_r_zh() * work->mVolumeSize,
                         work->mpEmtr->get_r_zh() * work->mVolumeSize);
    work->mVolumeCalcData.mVelOmni.mul(work->mVolumeCalcData.mVolumePos, work->mGlobalScl);
    work->mVolumeCalcData.mVelAxis.set(work->mVolumeCalcData.mVolumePos.x, 0.0f, work->mVolumeCalcData.mVolumePos.z);
}

static void JPAVolumeSphere(JPAEmitterWorkData* work) {
    s16 phi, theta;
    if (work->mpEmtr->checkFlag(JPADynFlag_FixedInterval)) {
        phi = (u16)(work->mVolumeX * 0x8000 / (work->mDivNumber - 1) + 0x4000);
        f32 tmp = (u16)(work->mVolumeAngleNum * 0x10000 / (work->mVolumeAngleMax - 1));
        theta = tmp * work->mVolumeSweep + 0x8000;
        work->mVolumeAngleNum++;
        if (work->mVolumeAngleNum == work->mVolumeAngleMax) {
            work->mVolumeAngleNum = 0;
            work->mVolumeX++;
            if (work->mVolumeX * 2 < work->mDivNumber) {
                work->mVolumeAngleMax = work->mVolumeAngleMax != 1 ?
                    work->mVolumeAngleMax + 4 : work->mVolumeAngleMax + 3;
            } else {
                work->mVolumeAngleMax = work->mVolumeAngleMax != 4 ? work->mVolumeAngleMax - 4 : 1;
            }
        }
    } else {
        phi = work->mpEmtr->get_r_ss() >> 1;
        theta = work->mVolumeSweep * work->mpEmtr->get_r_ss();
    }

    f32 rnd = work->mpEmtr->get_r_f();
    if (work->mpEmtr->checkFlag(JPADynFlag_FixedDensity)) {
        rnd = 1.0f - rnd * rnd * rnd;
    }
    f32 rad = work->mVolumeSize * (work->mVolumeMinRad + rnd * (1.0f - work->mVolumeMinRad));
    work->mVolumeCalcData.mVolumePos.set(rad * JMASCos(phi) * JMASSin(theta), -rad * JMASSin(phi),
                                         rad * JMASCos(phi) * JMASCos(theta));
    work->mVolumeCalcData.mVelOmni.mul(work->mVolumeCalcData.mVolumePos, work->mGlobalScl);
    work->mVolumeCalcData.mVelAxis.set(work->mVolumeCalcData.mVolumePos.x, 0.0f,
                                       work->mVolumeCalcData.mVolumePos.z);
}

static void JPAVolumeCylinder(JPAEmitterWorkData* work) {
    s16 theta = work->mVolumeSweep * work->mpEmtr->get_r_ss();
    f32 rnd = work->mpEmtr->get_r_f();
    if (work->mpEmtr->checkFlag(JPADynFlag_FixedDensity)) {
        rnd = 1.0f - rnd * rnd;
    }
    f32 rad = work->mVolumeSize * (work->mVolumeMinRad + rnd * (1.0f - work->mVolumeMinRad));
    work->mVolumeCalcData.mVolumePos.set(
        rad * JMASSin(theta), work->mVolumeSize * work->mpEmtr->get_r_zp(), rad * JMASCos(theta));
    work->mVolumeCalcData.mVelOmni.mul(work->mVolumeCalcData.mVolumePos, work->mGlobalScl);
    work->mVolumeCalcData.mVelAxis.set(work->mVolumeCalcData.mVolumePos.x, 0.0f,
                                       work->mVolumeCalcData.mVolumePos.z);
}

static void JPAVolumeTorus(JPAEmitterWorkData* work) {
    s16 theta = work->mVolumeSweep * work->mpEmtr->get_r_ss();
    s16 phi = work->mpEmtr->get_r_ss();
    f32 rad = work->mVolumeSize * work->mVolumeMinRad;
    work->mVolumeCalcData.mVelAxis.set(rad * JMASSin(theta) * JMASCos(phi), rad * JMASSin(phi),
                                       rad * JMASCos(theta) * JMASCos(phi));
    work->mVolumeCalcData.mVolumePos.set(
        work->mVolumeCalcData.mVelAxis.x + work->mVolumeSize * JMASSin(theta),
        work->mVolumeCalcData.mVelAxis.y,
        work->mVolumeCalcData.mVelAxis.z + work->mVolumeSize * JMASCos(theta));
    work->mVolumeCalcData.mVelOmni.mul(work->mVolumeCalcData.mVolumePos, work->mGlobalScl);
}

JPADynamicsBlock::JPADynamicsBlock(u8 const* data) {
    mpData = (const JPADynamicsBlockData*)data;
    init();
}

enum {
    VOL_Cube = 0x00,
    VOL_Sphere = 0x01,
    VOL_Cylinder = 0x02,
    VOL_Torus = 0x03,
    VOL_Point = 0x04,
    VOL_Circle = 0x05,
    VOL_Line = 0x06,
};

void JPADynamicsBlock::init() {
    switch (getVolumeType()) {
    case VOL_Cube:
        mpCalcVolumeFunc = &JPAVolumeCube;
        break;
    case VOL_Sphere:
        mpCalcVolumeFunc = &JPAVolumeSphere;
        break;
    case VOL_Cylinder:
        mpCalcVolumeFunc = &JPAVolumeCylinder;
        break;
    case VOL_Torus:
        mpCalcVolumeFunc = &JPAVolumeTorus;
        break;
    case VOL_Point:
        mpCalcVolumeFunc = &JPAVolumePoint;
        break;
    case VOL_Circle:
        mpCalcVolumeFunc = &JPAVolumeCircle;
        break;
    case VOL_Line:
        mpCalcVolumeFunc = &JPAVolumeLine;
        break;
    }
}

void JPADynamicsBlock::create(JPAEmitterWorkData* work) {
    if (work->mpEmtr->checkStatus(JPAEmtrStts_RateStepEmit)) {
        s32 emitCount;
        s32 createCount;

        // Probably an inlined function.
        if (work->mpEmtr->checkFlag(JPADynFlag_FixedInterval)) {
            s32 count;
            if (getVolumeType() == VOL_Sphere) {
                count = 4 * getDivNumber() * getDivNumber() + 2;
            } else {
                count = getDivNumber();
            }
            emitCount = count;

            work->mVolumeEmitIdx = 0;
        } else {
            f32 newPtclCount =
                work->mpEmtr->mRate * (getRateRndm() * work->mpEmtr->get_r_zp() + 1.0f);
            f32 newEmitCount = work->mpEmtr->mEmitCount + newPtclCount;
            work->mpEmtr->mEmitCount = newEmitCount;
            emitCount = (s32)newEmitCount;
            work->mpEmtr->mEmitCount -= emitCount;

            if (work->mpEmtr->checkStatus(JPAEmtrStts_FirstEmit) && 0.0f < newPtclCount &&
                newPtclCount < 1.0f)
                emitCount = 1;
        }

        work->mEmitCount = emitCount;
        if (work->mpEmtr->checkStatus(JPAEmtrStts_StopEmit)) {
            emitCount = 0;
        }

        // Probably an inlined function.
        createCount = emitCount;
        while (createCount > 0) {
            JPABaseParticle* ptcl = work->mpEmtr->createParticle();
            if (ptcl == NULL)
                break;
            createCount--;
        }
    }

    if (++work->mpEmtr->mRateStepTimer >= (work->mpEmtr->mRateStep + 1)) {
        work->mpEmtr->mRateStepTimer -= (work->mpEmtr->mRateStep + 1);
        work->mpEmtr->setStatus(JPAEmtrStts_RateStepEmit);
    } else {
        work->mpEmtr->clearStatus(JPAEmtrStts_RateStepEmit);
    }

    work->mpEmtr->clearStatus(JPAEmtrStts_FirstEmit);
}
