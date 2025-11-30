#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAFieldBlock.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAParticle.h"

void JPAFieldBase::calcAffect(JPAFieldBlock* block, JPABaseParticle* ptcl) {
    JGeometry::TVec3<f32> vec = mAccel;
    if (!ptcl->checkStatus(4) && block->checkStatus(0x78)) {
        vec.scale(calcFadeAffect(block, ptcl->mTime));
    }

    switch (block->getAddType()) {
    case 0:
        ptcl->mVelType0.add(vec);
        break;
    case 1:
        ptcl->mVelType1.add(vec);
        break;
    case 2:
        ptcl->mVelType2.add(vec);
        break;
    }
}

f32 JPAFieldBase::calcFadeAffect(JPAFieldBlock* block, f32 time) const {
    f32 fade = 1.0f;
    if ((block->checkStatus(8) && time < block->getEnTime())
        || (block->checkStatus(0x10) && time >= block->getDisTime()))
    {
        fade = 0.0f;
    } else if (block->checkStatus(0x40) && time >= block->getFadeOutTime()) {
        fade = (block->getDisTime() - time) * block->getFadeOutRate();
    } else if (block->checkStatus(0x20) && time < block->getFadeInTime()) {
        fade = (time - block->getEnTime()) * block->getFadeInRate();
    }
    return fade;
}

void JPAFieldGravity::prepare(JPAEmitterWorkData* work, JPAFieldBlock* block) {
    if (block->checkStatus(2)) {
        mAccel.scale(block->getMag(), block->getDir());
    } else {
        MTXMultVecSR(work->mRotationMtx, &block->getDir(), &mAccel);
        mAccel.scale(block->getMag());
    }
}

void JPAFieldGravity::calc(JPAEmitterWorkData* work, JPAFieldBlock* block, JPABaseParticle* ptcl) {
    calcAffect(block, ptcl);
}

void JPAFieldAir::prepare(JPAEmitterWorkData* work, JPAFieldBlock* block) {
    JGeometry::TVec3<f32> vec;
    vec.normalize(block->getDir());
    if (block->checkStatus(2)) {
        mAccel.scale(block->getMag(), vec);
    } else {
        MTXMultVecSR(work->mRotationMtx, vec, mAccel);
        mAccel.scale(block->getMag());
    }
}

void JPAFieldAir::calc(JPAEmitterWorkData* work, JPAFieldBlock* block, JPABaseParticle* ptcl) {
    calcAffect(block, ptcl);
    if (block->checkStatus(4)) {
        f32 len = ptcl->mVelType1.length();
        if (len > block->getMagRndm()) {
            ptcl->mVelType1.scale(block->getMagRndm() / len);
        }
    }
}

void JPAFieldMagnet::prepare(JPAEmitterWorkData* work, JPAFieldBlock* block) {
    mDir.sub(block->getPos(), work->mEmitterPos);
    MTXMultVecSR(work->mRotationMtx, &mDir, &mDir);
}

void JPAFieldMagnet::calc(JPAEmitterWorkData* work, JPAFieldBlock* block, JPABaseParticle* ptcl) {
    mAccel.sub(mDir, ptcl->mLocalPosition);
    mAccel.setLength(block->getMag());
    calcAffect(block, ptcl);
}

void JPAFieldNewton::prepare(JPAEmitterWorkData* work, JPAFieldBlock* block) {
    mDir.sub(block->getPos(), work->mEmitterPos);
    MTXMultVecSR(work->mRotationMtx, &mDir, &mDir);
    mCutoff = block->getVal1() * block->getVal1();
}

void JPAFieldNewton::calc(JPAEmitterWorkData* work, JPAFieldBlock* block, JPABaseParticle* ptcl) {
    mAccel.sub(mDir, ptcl->mLocalPosition);
    f32 len_sq = mAccel.squared();
    if (len_sq > mCutoff) {
        mAccel.setLength(mCutoff * (block->getMag() * 10.0f) / len_sq);
    } else {
        mAccel.setLength(block->getMag() * 10.0f);
    }
    calcAffect(block, ptcl);
}

void JPAFieldVortex::prepare(JPAEmitterWorkData* work, JPAFieldBlock* block) {
    MTXMultVecSR(work->mGlobalRot, &block->getDir(), &field_0x10);
    field_0x10.normalize();
    field_0x1c = block->getPos().z * block->getPos().z;
    field_0x20 = 1.0f / field_0x1c;
}

void JPAFieldVortex::calc(JPAEmitterWorkData* work, JPAFieldBlock* block, JPABaseParticle* ptcl) {
    JGeometry::TVec3<f32> vec;
    vec.scale(field_0x10.dot(ptcl->mLocalPosition), field_0x10);
    vec.sub(ptcl->mLocalPosition, vec);
    f32 mag = vec.squared();
    if (mag > field_0x1c) {
        mag = block->getMagRndm();
    } else {
        mag *= field_0x20;
        mag = (1.0f - mag) * block->getMag() + mag * block->getMagRndm();
    }
    vec.normalize();
    mAccel.cross(vec, field_0x10);
    mAccel.scale(mag);
    calcAffect(block, ptcl);
}

void JPAFieldConvection::prepare(JPAEmitterWorkData* work, JPAFieldBlock* block) {
    JGeometry::TVec3<f32> vec1, vec2;
    vec2.cross(block->getPos(), block->getDir());
    vec1.cross(block->getDir(), vec2);
    MTXMultVecSR(work->mGlobalRot, &vec1, &field_0x10);
    MTXMultVecSR(work->mGlobalRot, &block->getDir(), field_0x1c);
    MTXMultVecSR(work->mGlobalRot, &vec2, &field_0x28);
    field_0x10.normalize();
    field_0x1c.normalize();
    field_0x28.normalize();
}

void JPAFieldConvection::calc(JPAEmitterWorkData* work, JPAFieldBlock* block,
                              JPABaseParticle* ptcl) {
    JGeometry::TVec3<f32> vec1, vec2, vec3;
    vec1.scale(field_0x10.dot(ptcl->mLocalPosition), field_0x10);
    vec3.scale(field_0x28.dot(ptcl->mLocalPosition), field_0x28);
    vec1.add(vec3);
    vec1.setLength(vec1, block->getVal1());
    vec2.sub(ptcl->mLocalPosition, vec1);
    vec3.cross(field_0x1c, vec1);
    mAccel.cross(vec3, vec2);
    mAccel.setLength(block->getMag());
    calcAffect(block, ptcl);
}

void JPAFieldRandom::calc(JPAEmitterWorkData* work, JPAFieldBlock* block, JPABaseParticle* ptcl) {
    if (ptcl->mAge == 0 || (block->getCycle() != 0 && ptcl->mAge % block->getCycle() == 0)) {
        JPABaseEmitter* emtr = work->mpEmtr;
        mAccel.set(emtr->get_r_zh(), emtr->get_r_zh(), emtr->get_r_zh());
        mAccel.scale(block->getMag());
        calcAffect(block, ptcl);
    }
}

void JPAFieldDrag::calc(JPAEmitterWorkData* work, JPAFieldBlock* block, JPABaseParticle* ptcl) {
    if (!ptcl->checkStatus(4)) {
        f32 fade = calcFadeAffect(block, ptcl->mTime);
        ptcl->mDrag *= 1.0f - fade * (1.0f - block->getMag());
    } else {
        ptcl->mDrag *= block->getMag();
    }
}

void JPAFieldSpin::prepare(JPAEmitterWorkData* work, JPAFieldBlock* block) {
    JGeometry::TVec3<f32> axis;
    MTXMultVecSR(work->mGlobalRot, &block->getDir(), &axis);
    axis.normalize();
    Mtx mtx;
    MTXRotAxisRad(mtx, &axis, block->getMag());
    field_0x10.set(mtx[0][0], mtx[1][0], mtx[2][0]);
    field_0x1c.set(mtx[0][1], mtx[1][1], mtx[2][1]);
    field_0x28.set(mtx[0][2], mtx[1][2], mtx[2][2]);
}

void JPAFieldSpin::calc(JPAEmitterWorkData* work, JPAFieldBlock* block, JPABaseParticle* ptcl) {
    Mtx mtx;
    mtx[0][0] = field_0x10.x;
    mtx[1][0] = field_0x10.y;
    mtx[2][0] = field_0x10.z;
    mtx[0][1] = field_0x1c.x;
    mtx[1][1] = field_0x1c.y;
    mtx[2][1] = field_0x1c.z;
    mtx[0][2] = field_0x28.x;
    mtx[1][2] = field_0x28.y;
    mtx[2][2] = field_0x28.z;
    mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0f;
    Vec vec;
    MTXMultVecSR(mtx, &ptcl->mLocalPosition, &vec);
    mAccel.set(vec.x - ptcl->mLocalPosition.x, vec.y - ptcl->mLocalPosition.y,
               vec.z - ptcl->mLocalPosition.z);
    calcAffect(block, ptcl);
}

JPAFieldBlock::JPAFieldBlock(u8 const* data, JKRHeap* heap)
    : mpData((const JPAFieldBlockData*)data) {
    init(heap);
}

void JPAFieldBlock::init(JKRHeap* heap) {
    mFadeInRate = getFadeInTime() - getEnTime();
    if (mFadeInRate == 0.0f) {
        mFadeInRate = 1.0f;
    } else {
        mFadeInRate = 1.0f / mFadeInRate;
    }

    mFadeOutRate = getDisTime() - getFadeOutTime();
    if (mFadeOutRate == 0.0f) {
        mFadeOutRate = 1.0f;
    } else {
        mFadeOutRate = 1.0f / mFadeOutRate;
    }

    getPosOrig(&mPos);
    getDirOrig(&mDir);
    mMag = getMagOrig();

    switch (getType()) {
    case FIELD_GRAVITY:
        mpField = new (heap, 0) JPAFieldGravity();
        break;
    case FIELD_AIR:
        mpField = new (heap, 0) JPAFieldAir();
        break;
    case FIELD_MAGNET:
        mpField = new (heap, 0) JPAFieldMagnet();
        break;
    case FIELD_NEWTON:
        mpField = new (heap, 0) JPAFieldNewton();
        break;
    case FIELD_VORTEX:
        mpField = new (heap, 0) JPAFieldVortex();
        break;
    case FIELD_RANDOM:
        mpField = new (heap, 0) JPAFieldRandom();
        break;
    case FIELD_DRAG:
        mpField = new (heap, 0) JPAFieldDrag();
        break;
    case FIELD_CONVECTION:
        mpField = new (heap, 0) JPAFieldConvection();
        break;
    case FIELD_SPIN:
        mpField = new (heap, 0) JPAFieldSpin();
        break;
    default:
        mpField = NULL;
        break;
    }
}
