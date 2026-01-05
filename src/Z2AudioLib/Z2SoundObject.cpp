#include "Z2AudioLib/Z2SoundObject.h"
#include "Z2AudioLib/Z2SoundObjMgr.h"
#include "Z2AudioLib/Z2Calc.h"
#include "Z2AudioLib/Z2SeMgr.h"
#include "Z2AudioLib/Z2SoundInfo.h"
#include "Z2AudioLib/Z2SoundMgr.h"
#include "Z2AudioLib/Z2SoundStarter.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "Z2AudioLib/Z2Audience.h"
#include "JSystem/JAudio2/JAUSoundAnimator.h"

#if PLATFORM_WII || PLATFORM_SHIELD
#include "Z2AudioLib/Z2AudioCS.h"
#endif

Z2SoundObjBase::Z2SoundObjBase()
#if DEBUG
: JSULink<Z2SoundObjBase>(this)
#endif
{
    pos_ = NULL;
    alive_ = false;
    soundStarter_ = Z2GetSoundStarter();
    field_0x1c = 0;
    reverb_ = 0;
}

Z2SoundObjBase::~Z2SoundObjBase() {
    deleteObject();
}

void Z2SoundObjBase::init(Vec* posPtr, u8 handleNum) {
    #if DEBUG
    Z2GetSoundObjMgr()->getAllList()->append(this);
    #endif

    initHandlesPool(handleNum);
    pos_ = (JGeometry::TVec3<f32>*)posPtr;
    alive_ = true;
}

void Z2SoundObjBase::deleteObject() {
    #if DEBUG
    Z2GetSoundObjMgr()->getAllList()->remove(this);
    #endif

    dispose();
    deleteHandlesPool();
}

void Z2SoundObjBase::framework(u32 param_0, s8 reverb) {
    if (alive_) {
        field_0x1c = param_0;
        reverb_ = reverb;
        setPos(*pos_);
    }
}

void Z2SoundObjBase::dispose() {
    JAISoundHandle* handle;
    JSULink<Z2SoundHandlePool>* i;
    for (i = getFirst(); i != NULL; i = i->getNext()) {
        handle = i->getObject();
        if (handle != NULL && *handle) {
            if ((Z2GetSoundInfo()->getSwBit((*handle)->getID()) & 0x8000) != 0) {
                handle->releaseSound();
            } else {
                (*handle)->stop();
            }
        }
    }

    alive_ = false;
}

bool Z2SoundObjBase::stopOK(Z2SoundHandlePool& handlePool) {
    if ((Z2GetSoundInfo()->getSwBit(handlePool->getID()) & 0x8000) != 0) {
        return false;
    } else {
        return true;
    }
}

Z2SoundHandlePool* Z2SoundObjBase::startSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    if (!alive_) {
        return NULL;
    }

    if (Z2GetSeMgr()->isSoundCulling(soundID)) {
        return NULL;
    }

    if (mapinfo == 0) {
        mapinfo = (u32)field_0x1c;
    }

    if (reverb < 0) {
        reverb = reverb_;
    }

    f32 fxMix = (f32)reverb / 127.0f;

    Z2SoundHandlePool* handle = getHandleSoundID(soundID);
    if (handle != NULL) {
        if ((Z2GetSoundInfo()->getSwBit(soundID) & 0x4000) != 0) {
            handle = NULL;
        } else {
            return NULL;
        }
    }

    if (handle == NULL) {
        handle = getFreeHandle();
    }

    if (handle == NULL) {
        handle = getLowPrioSound(soundID);
    }

    if (handle != NULL) {
        soundStarter_->startSound(soundID, handle, pos_, mapinfo, fxMix,
                                  1.0f, 1.0f, -1.0f, -1.0f, 0);
    }

    return handle;
}

Z2SoundHandlePool* Z2SoundObjBase::startLevelSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    if (!alive_) {
        return NULL;
    }

    if (mapinfo == 0) {
        mapinfo = (u32)field_0x1c;
    }

    if (reverb < 0) {
        reverb = reverb_;
    }

    f32 fxMix = (f32)reverb / 127.0f;

    Z2SoundHandlePool* handle = getHandleSoundID(soundID);
    if (handle != NULL) {
        JUT_ASSERT_MSG(218, *handle, "handle exist, but (*handle) is not exist!!\n");
        (*handle)->updateLifeTime(1);
        soundStarter_->setPortData(handle, 6, (u16)mapinfo, -1);
        (*handle)->getAuxiliary().moveFxMix(fxMix, 0);
    } else {
        if (Z2GetSeMgr()->isSoundCulling(soundID)) {
            return NULL;
        }

        handle = getFreeHandle();
        if (handle == NULL) {
            handle = getLowPrioSound(soundID);
        }

        if (handle != NULL) {
            soundStarter_->startSound(soundID, handle, pos_, mapinfo, fxMix,
                                      1.0f, 1.0f, -1.0f, -1.0f, 0);
            if (handle != NULL && (*handle) != NULL) {
                (*handle)->setLifeTime(1, false);

                #if PLATFORM_WII || PLATFORM_SHIELD
                if (Z2GetSceneMgr()->isSceneExist()) {
                    switch (soundID) {
                    case Z2SE_BOOM_POWER_RESUME:
                        Z2AudioCS::start(0x12, 0);
                        break;
                    case Z2SE_BOOM_TORNADO:
                        Z2AudioCS::start(0x13, 0);
                        break;
                    case Z2SE_AL_SPINNER_RAIL:
                        Z2AudioCS::start(0x3C, 0);
                        break;
                    case Z2SE_WOLFATTACK_WIND_RUSH:
                        Z2AudioCS::start(0x58, 0);
                        break;
                    }
                }
                #endif
            }
        }
    }
    return handle;
}

Z2SoundHandlePool* Z2SoundObjBase::startCollisionSE(u32 hitID, u32 mapinfo, Z2SoundObjBase* other) {
    if (other != NULL) {
        return other->startCollisionSE(hitID, mapinfo, NULL);
    }

    if (53 < mapinfo) {
        JUT_WARN_DEVICE(297, 1, "%s", "mapinfo > 53\n");
        OS_REPORT("startCollisionSE Invalid MAPINFO:%d hitID:%08x\n", mapinfo, hitID);
        mapinfo = 0;
    }

    for (JSULink<JAISe>* i = Z2GetSoundMgr()->getSeMgr()->getCategory(4)->getSeList()->getFirst(); i != NULL; i = i->getNext()) {
        JAISe* se = i->getObject();
        if (hitID == se->getID() && mapinfo == se->getUserData()) {
            se->stop();
        }
    }

    Z2SoundHandlePool* handle = Z2SoundObjBase::startSound(JAISoundID(hitID), mapinfo, -1);
    if (handle != NULL && (*handle) != NULL) {
        (*handle)->setUserData(mapinfo);
        if (30 <= mapinfo && mapinfo <= 52) {
            Z2Audible* audible = (Z2Audible*)(*handle)->getAudible();
            if (audible != NULL) {
                audible->getAudibleParam()->field_0x0.bytes.b1_2_7 = 8;
            }
        }
    }

    return handle;
}

Z2DopplerSoundObjBase::Z2DopplerSoundObjBase() : Z2SoundObjBase() {
    field_0x20 = Z2Calc::cNullVec;
    field_0x2c = Z2Calc::cNullVec;
}

Z2DopplerSoundObjBase::~Z2DopplerSoundObjBase() {}

void Z2DopplerSoundObjBase::init(Vec* posPtr, u8 handleNum) {
    Z2SoundObjBase::init(posPtr, handleNum);
    if (pos_ != NULL) {
        field_0x20 = *pos_;
        field_0x2c = *pos_;
    }
}

void Z2DopplerSoundObjBase::framework(u32 param_0, s8 reverb) {
    Z2SoundObjBase::framework(param_0, reverb);
    if (pos_ != NULL) {
        field_0x20 = field_0x2c;
        field_0x2c = *pos_;
    }
}

Z2SoundHandlePool* Z2DopplerSoundObjBase::startSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    JGeometry::TVec3<f32>* pos = pos_;
    pos_ = NULL;

    Z2SoundHandlePool* handle = Z2SoundObjBase::startSound(soundID, mapinfo, reverb);
    if (pos != NULL && handle != NULL && (*handle) != NULL) {
        if ((*handle)->acceptsNewAudible()) {
            (*handle)->newAudible(*pos, &field_0x20, 0, NULL);
        }
    }

    pos_ = pos;
    return handle;
}

Z2SoundHandlePool* Z2DopplerSoundObjBase::startLevelSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    JGeometry::TVec3<f32>* pos = pos_;
    pos_ = NULL;

    Z2SoundHandlePool* handle = Z2SoundObjBase::startLevelSound(soundID, mapinfo, reverb);
    if (pos != NULL && handle != NULL && (*handle) != NULL) {
        if ((*handle)->acceptsNewAudible()) {
            (*handle)->newAudible(*pos, &field_0x20, 0, NULL);
        }
    }

    pos_ = pos;
    return handle;
}

Z2SoundObjSimple::Z2SoundObjSimple() : Z2SoundObjBase() {}

void Z2SoundObjSimple::init(Vec* posPtr, u8 handleNum) {
    Z2SoundObjBase::init(posPtr, handleNum);
}

Z2SoundHandlePool* Z2SoundObjSimple::startSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    Z2SoundHandlePool* handle = Z2SoundObjBase::startSound(soundID, mapinfo, reverb);

    if (soundID == Z2SE_AL_UKI_POKOPOKO && handle != NULL && (*handle) != NULL) {
        f32 volume = Z2Calc::getParamByExp((f32)mapinfo, 0.0f, 127.0f, 0.2f, 0.4f, 1.0f, Z2Calc::CURVE_POSITIVE);
        f32 pitch = Z2Calc::getParamByExp((f32)mapinfo, 0.0f, 127.0f, 0.2f, 0.6f, 1.2f, Z2Calc::CURVE_POSITIVE);
        (*handle)->getAuxiliary().movePitch(pitch, 0);
        (*handle)->getAuxiliary().moveVolume(volume, 0);
    }

    return handle;
}

Z2SoundHandlePool* Z2SoundObjSimple::startLevelSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    Z2SoundHandlePool* handle = Z2SoundObjBase::startLevelSound(soundID, mapinfo, reverb);

    if (handle != NULL && (*handle) != NULL) {
        f32 pitch = 1.0f;
        f32 volume = 1.0f;
        switch (soundID) {
            case Z2SE_AL_UKI_KURUKURU_LOOP:
                volume = Z2Calc::getParamByExp((f32)mapinfo, 0.0f, 127.0f, 0.2f, 0.4f, 1.0f, Z2Calc::CURVE_POSITIVE);
                pitch = Z2Calc::getParamByExp((f32)mapinfo, 0.0f, 127.0f, 0.2f, 0.6f, 1.2f, Z2Calc::CURVE_POSITIVE);
                break;
            case Z2SE_OBJ_FLR_FALLING:
                if (mapinfo > 100) {
                    pitch = 2.0f;
                } else if (mapinfo > 21) {
                    pitch = Z2Calc::getParamByExp((f32)mapinfo, 22.0f, 50.0f, 0.4f, 1.0f, 1.5f, Z2Calc::CURVE_NEGATIVE);
                } else {
                    pitch = 1.0f;
                }
                break;
        }

        (*handle)->getAuxiliary().movePitch(pitch, 0);
        (*handle)->getAuxiliary().moveVolume(volume, 0);
    }

    return handle;
}

Z2SoundObjArrow::Z2SoundObjArrow() : Z2DopplerSoundObjBase() {}

void Z2SoundObjArrow::init(Vec* posPtr, u8 handleNum) {
    Z2SoundObjBase::init(posPtr, handleNum);
}

Z2SoundObjAnime::Z2SoundObjAnime() : Z2SoundObjBase() {
    animation_ = NULL;
    reverse_ = false;
}

void Z2SoundObjAnime::init(Vec* posPtr, u8 handleNum) {
    Z2SoundObjBase::init(posPtr, handleNum);
}

void Z2SoundObjAnime::initAnime(void* animation, bool param_1, f32 startFrame, f32 param_3) {
    ageSounds_();
    animation_ = (JAUSoundAnimation*)animation;
    if (animation_ != NULL) {
        reverse_ = !param_1;
        field_0x3c = 0;

        if (reverse_) {
            curSoundIndex_ = animation_->getNumSounds() - 1;
            curSoundFrame_ = FLT_MAX;
            setLoopFrame(0.0f, startFrame);
        } else {
            curSoundIndex_ = 0;
            curSoundFrame_ = 0.0f;
            setLoopFrame(startFrame, 0.0f);
        }

        curSoundFrame_ = param_3;
        curSoundIndex_ = animation_->getStartSoundIndex(param_3);
        if (reverse_ && curSoundIndex_ > 0) {
            curSoundIndex_--;
        }
    }
}

void Z2SoundObjAnime::ageSounds_() {
    for (int i = 0; i < getNumHandles(); i++) {
        if ((*getHandle(i))) {
            switch ((*getHandle(i))->getAnimationState()) {
            case 0:
                break;
            case 1:
                if ((*getHandle(i))->hasLifeTime()) {
                    (*getHandle(i))->stop();
                } else {
                    JAUSoundAnimationSound* animationSound = (JAUSoundAnimationSound*)(*getHandle(i))->getUserData();
                    if (animationSound->stopsWhenAnimationChanges()) {
                        (*getHandle(i))->stop();
                    } else {
                        (*getHandle(i))->setAnimationState(2);
                    }
                }
                break;
            case 2:
                break;
            }
        }
    }
}

void Z2SoundObjAnime::updateAnime(f32 param_0, f32 param_1) {
    for (int i = 0; i < getNumHandles(); i++) {
        if ((*getHandle(i)) && (*getHandle(i))->isAnimated()) {
            (*getHandle(i))->setPos(*pos_);
        }
    }

    if (animation_ == NULL || animation_->getNumSounds() == 0) {
        return;
    }

    if (!reverse_) {
        if (param_0 < curSoundFrame_) {
            while (curSoundIndex_ < endSoundIndex_) {
                startSoundInner(*pos_, param_1, soundStarter_, field_0x1c, reverb_);
            }
            curSoundIndex_ = startSoundIndex_;
            curSoundFrame_ = param_0;
            if (field_0x3c < 0xFFFF) {
                field_0x3c++;
            }
        }

        updateSoundLifeTime_(param_0, param_1);

        while (curSoundIndex_ < animation_->getNumSounds() && animation_->getSound(curSoundIndex_)->isNotingOn(param_0, false)) {
            startSoundInner(*pos_, param_1, soundStarter_, field_0x1c, reverb_);
        }
    } else {
        if (param_0 > curSoundFrame_) {
            while (curSoundIndex_ >= startSoundIndex_) {
                startSoundInner(*pos_, param_1, soundStarter_, field_0x1c, reverb_);
            }
            curSoundIndex_ = endSoundIndex_ - 1;
            curSoundFrame_ = param_0;
            if (field_0x3c < 0xFFFF) {
                field_0x3c++;
            }
        }

        updateSoundLifeTime_(param_0, param_1);

        while (curSoundIndex_ >= 0 && animation_->getSound(curSoundIndex_)->isNotingOn(param_0, true)) {
            startSoundInner(*pos_, param_1, soundStarter_, field_0x1c, reverb_);
        }
    }

    curSoundFrame_ = param_0;
}

void Z2SoundObjAnime::updateSoundLifeTime_(f32 param_0, f32 param_1) {
    if (animation_ == NULL) {
        return;
    }

    for (int i = 0; i < getNumHandles(); i++) {
        if ((*getHandle(i))) {
            switch ((*getHandle(i))->getAnimationState()) {
            case 0:
                break;
            case 1: {
                JAUSoundAnimationSound* animationSound = (JAUSoundAnimationSound*)(*getHandle(i))->getUserData();
                JUT_ASSERT(696, animationSound);

                bool bvar1 = param_1 == 0.0f && animationSound->stopsWhenSpeedIsZero();
                if (animationSound->setsLifeTime()) {
                    if (bvar1) {
                        (*getHandle(i))->pause(true);
                    } else {
                        if (animationSound->isNoting(param_0)) {
                            (*getHandle(i))->updateLifeTime(1);
                            (*getHandle(i))->pause(false);
                        } else {
                            (*getHandle(i))->stop();
                        }
                    }
                } else {
                    if (bvar1) {
                        (*getHandle(i))->stop();
                    } else if (animationSound->stopsWhenNoteOff() && animationSound->isNotingOff(param_0, reverse_)) {
                        (*getHandle(i))->stop();
                    }
                }
                break;
            }
            case 2:
                break;
            }
        }
    }
}

void Z2SoundObjAnime::startSoundInner(const JGeometry::TVec3<f32>& pos, f32 param_1,
                                      Z2SoundStarter* soundStarter, u32 mapinfo, s8 reverb) {
    JUT_ASSERT(746, curSoundIndex_ >= 0);
    JUT_ASSERT(747, curSoundIndex_ < animation_->getNumSounds());

    const JAUSoundAnimationSound* animationSound = animation_->getSound(curSoundIndex_);
    u32 user_data = (u32)animationSound;
    if (reverse_) {
        curSoundIndex_--;
    } else {
        curSoundIndex_++;
    }

    if (animationSound->field_0x1a > 0) {
        mapinfo = animationSound->field_0x1a;
    }

    if (playsSound(animationSound, pos, param_1)) {
        u32 id = getSoundID(animationSound, pos, param_1);
        if (!Z2GetSeMgr()->isSoundCulling(id)) {
            JAISoundHandle* handle = getHandleUserData(user_data);
            if (handle != NULL && (*handle) != NULL && (*handle)->getAnimationState() != 1) {
                handle = NULL;
            }

            if (handle == NULL) {
                handle = getFreeHandle();
            }

            if (handle != NULL) {
                bool result = soundStarter->startSound(id, handle, &pos, mapinfo, (f32)reverb / 127.0f,
                                                       animationSound->field_0x0c, (f32)animationSound->field_0x14 / 127.0f,
                                                       -1.0f, -1.0f, 0);
                if ((*handle) != NULL) {
                    (*handle)->setAnimationState(1);
                    (*handle)->setUserData(user_data);
                    if (animationSound->setsLifeTime()) {
                        (*handle)->setLifeTime(1, false);
                    }
                }
            }
        }
    }
}

u32 Z2SoundObjAnime::getSoundID(const JAUSoundAnimationSound* animationSound,
                                const JGeometry::TVec3<f32>& param_1, f32 param_2) {
    return animationSound->mSoundId;
}

bool Z2SoundObjAnime::playsSound(const JAUSoundAnimationSound* animationSound,
                                 const JGeometry::TVec3<f32>& param_1, f32 param_2) {
    UNUSED(param_1);
    UNUSED(param_2);

    if (animationSound->playsOnlyOnce()) {
        if (field_0x3c != animationSound->field_0x16) {
            return false;
        }
    } else {
        if (animationSound->playsAtIntervals() && animationSound->field_0x16 != field_0x3c % animationSound->field_0x19) {
            return false;
        }
    }

    if (reverse_) {
        if (animationSound->playsOnlyForward()) {
            return false;
        }
    } else {
        if (animationSound->playsOnlyReverse()) {
            return false;
        }
    }

    return true;
}
