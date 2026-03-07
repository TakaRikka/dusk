#include "Z2AudioCS/SpkSound.h"

#include "Z2AudioCS/SpkSpeakerCtrl.h"
#include "Z2AudioCS/SpkSystem.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"

#define SPK_SOUND_MIX_STEP 40

enum SpkSoundState {
    SpkSoundState_STARTING_e,
    SpkSoundState_LOCKED_e,
    SpkSoundState_UNLOCKED_e,
    SpkSoundState_STOPPING_e,
    SpkSoundState_DEAD_e,
};

void SpkSoundHandle::releaseSound() {
    if (!mSound) {
        return;
    }

    mSound->mHandle = NULL;
    mSound = NULL;
}

SpkSoundVolume::SpkSoundVolume() {
    field_0x00 = 1.0f;
    field_0x04 = 0.0f;
    field_0x08 = 1.0f;
    field_0x0c = 0.0f;
    field_0x10 = 1.0f;
    field_0x14 = 1.0f;
    field_0x18 = 0.0f;
    mTableVolume = 1.0f;
}

void SpkSoundVolume::setRelease(s32 release) {
    if (release > 0) {
        field_0x04 = 1.0f / release;
    } else {
        field_0x04 = 1.0f;
    }

    field_0x00 = 1.0f;
}

void SpkSoundVolume::setFadeOut(s32 fadeOut) {
    JASCriticalSection cs;

    if (fadeOut > 0) {
        field_0x0c = 1.0f / fadeOut;
    } else {
        field_0x0c = 1.0f;
    }

    field_0x08 = 1.0f;
}

f32 SpkSoundVolume::calc(bool& param_0) {
    param_0 = false;

    if (field_0x04 != 0.0f) {
        field_0x00 -= field_0x04;
        if (field_0x00 <= 0.0f) {
            field_0x00 = 0.0f;
            field_0x04 = 0.0f;
            param_0 = true;
        }
    }

    if (field_0x0c != 0.0f) {
        field_0x08 -= field_0x0c;
        if (field_0x08 <= 0.0f) {
            field_0x08 = 0.0f;
            field_0x0c = 0.0f;
            param_0 = true;
        }
    }

    if (field_0x18 != 0.0f) {
        field_0x10 += field_0x18;
        if (field_0x18 >= 0.0f) {
            if (field_0x10 >= field_0x14) {
                field_0x10 = field_0x14;
                field_0x18 = 0.0f;
            }
        } else if (field_0x10 <= field_0x14) {
            field_0x10 = field_0x14;
            field_0x18 = 0.0f;
        }
    }

    f32 result = mTableVolume * (field_0x10 * (field_0x08 * (field_0x00 * field_0x00)));
    return result;
}

void SpkSoundVolume::setTableVolume(f32 vol) {
    mTableVolume = vol;
}

SpkSound::SpkSound() : JSULink<SpkSound>(this),
                       mHandle(NULL), mSoundNum(-1), mWaveData(0), mCurPos(0),
                       field_0x20(0), mPriority(100), field_0x28(0), mWaveSize(0),
                       mWaveLoopStart(-1), mWaveLoopEnd(-1), mState(SpkSoundState_STARTING_e),
                       mLifeTime(-1) {
}

SpkSound::~SpkSound() {
    releaseHandle();
}

void SpkSound::update(f32 param_0) {
    if (!((SpkSoundState)mState != SpkSoundState_UNLOCKED_e &&
          (SpkSoundState)mState != SpkSoundState_STOPPING_e)) {
        SpkSystem* system = JASGlobalInstance<SpkSystem>::getInstance();
        JUT_ASSERT(227, system);

        SpkMixingBuffer* mixing_buffer = system->getMixingBuffer();
        JUT_ASSERT(230, mixing_buffer);

        bool calc_flag = false;
        f32 weight = mVolume.calc(calc_flag);
        if (calc_flag) {
            mState = SpkSoundState_DEAD_e;
        }
        weight *= param_0;

        if (!isLoopWave()) {
            s32 len = SPK_SOUND_MIX_STEP;
            if (mCurPos + len >= mWaveSize) {
                len = mWaveSize - mCurPos;
            }

            mixing_buffer->mix(field_0x20, mWaveData + mCurPos, len, weight, 0);

            mCurPos += SPK_SOUND_MIX_STEP;

            if (mCurPos >= mWaveSize) {
                mState = SpkSoundState_DEAD_e;
            }
            return;
        }
        s32 len = 0;
        s32 processedBytes = 0;
        s32 remainingBytes = SPK_SOUND_MIX_STEP;
        if (mWaveLoopEnd - mCurPos >= SPK_SOUND_MIX_STEP) {
            len = SPK_SOUND_MIX_STEP;
            mixing_buffer->mix(field_0x20, mWaveData + mCurPos, len, weight, 0);

            mCurPos += SPK_SOUND_MIX_STEP;

            if (mCurPos >= mWaveLoopEnd) {
                mCurPos = mWaveLoopStart;
            }
        } else {
            while (remainingBytes > 0) {
                len = mWaveLoopEnd - mCurPos;
                if (len >= remainingBytes) {
                    len = remainingBytes;
                }
                remainingBytes -= len;

                mixing_buffer->mix(field_0x20, mWaveData + mCurPos, len, weight,
                                   processedBytes);

                processedBytes += len;
                mCurPos += len;

                if (mCurPos >= mWaveLoopEnd) {
                    mCurPos = mWaveLoopStart;
                }
            }
        }
    }
}

void SpkSound::stop(s32 msec) {
    mState = SpkSoundState_STOPPING_e;
    s32 fade = convertMsecToFrames(msec);

    if (fade) {
        mVolume.setFadeOut(fade);
    } else {
        startRelease();
    }
}

void SpkSound::startRelease(void) {
    mVolume.setRelease(field_0x28);
}

void SpkSound::attachHandle(SpkSoundHandle* handle) {
    JUT_ASSERT(338, handle);
    if (isHandleAttached()) {
        releaseHandle();
    }

    mHandle = handle;
    mHandle->mSound = this;
}

bool SpkSound::isHandleAttached(void) const {
    return mHandle != NULL;
}

void SpkSound::releaseHandle() {
    if (!mHandle) {
        return;
    }

    mHandle->mSound = NULL;
    mHandle = NULL;
}

bool SpkSound::start(s32 param_0, s32 soundNum) {
    field_0x20 = param_0;

    bool setResult = setWaveData(soundNum);
    if (!setResult) {
        return false;
    }

    mState = SpkSoundState_LOCKED_e;
    return true;
}

void SpkSound::kill(void) {
    mState = SpkSoundState_DEAD_e;
}

bool SpkSound::isStopping(void) const {
    return mState == SpkSoundState_STOPPING_e;
}

bool SpkSound::isDead(void) const {
    return mState == SpkSoundState_DEAD_e;
}

bool SpkSound::isLocked(void) const {
    return mState == SpkSoundState_LOCKED_e;
}

void SpkSound::unlock() {
    if (mState != SpkSoundState_LOCKED_e) {
        return;
    }

    mState = SpkSoundState_UNLOCKED_e;
}

s32 SpkSound::getLifeTime(void) {
    return mLifeTime;
}

void SpkSound::setLifeTime(s32 lifeTime) {
    mLifeTime = lifeTime;
}

void SpkSound::updateLifeTime() {
    mLifeTime--;
}

s32 SpkSound::convertMsecToFrames(s32 msec) {
    return 6 * msec / 40;
}

inline bool SpkSound::isLoopWave(void) {
    return mWaveLoopStart >= 0 && mWaveLoopEnd > mWaveLoopStart;
}

bool SpkSound::setWaveData(s32 soundNum) {
    SpkSystem* system = JASGlobalInstance<SpkSystem>::getInstance();
    JUT_ASSERT(432, system);

    SpkData* data = system->getData();
    JUT_ASSERT(435, data);

    if (data->isValid() == 0) {
        JUT_WARN(438, "%s", "Speaker data is not valid\n");
        return false;
    }

    SpkTable& table = data->getTableMgr();
    JUT_ASSERT(444, soundNum < table.getNumOfSound());

    mSoundNum = soundNum;
    s32 waveNum = table.getParams(soundNum)->mWaveNum;
    mPriority = table.getParams(soundNum)->field_0x02;
    mVolume.setTableVolume(table.getParams(soundNum)->mVolume / 127.0f);
    field_0x28 = convertMsecToFrames(table.getParams(soundNum)->mMsec);

    SpkWave& wave = data->getWaveMgr();
    JUT_ASSERT(457, mSoundNum < wave.getNumOfWaves());
    mWaveData = wave.getWave(waveNum);
    mCurPos = 0;
    mWaveSize = wave.getWaveSize(waveNum) / 2;
    mWaveLoopStart = wave.getLoopStartPos(waveNum);
    mWaveLoopEnd = wave.getLoopEndPos(waveNum);

    JUT_ASSERT(466, mWaveLoopEnd <= mWaveSize);

    return true;
}

SpkSoundHolder::SpkSoundHolder() : JASGlobalInstance(true) {
    mMasterVolume = 1.0f;
    mConfigVolume = 10;

    for (int i = 0; i < ARRAY_SIZE(mSoundVolumes); i++) {
        mSoundVolumes[i] = 1.0f;
    }
}

bool SpkSoundHolder::startSound(s32 chan, s32 soundNum, SpkSoundHandle* handle) {
    if (!SpkSpeakerCtrl::isEnable(chan)) {
        return false;
    }

    if (soundNum < 0) {
        return false;
    }

    if (handle != NULL) {
        if (*handle) {
            if ((*handle)->isLocked()) {
                (*handle)->kill();
            } else {
                (*handle)->stop(0);
            }
        }
    }

    SpkSound* new_sound_p = new SpkSound();
    if (new_sound_p == NULL) {
        JUT_WARN(520, "%s", "cannot new SpkSound\n");
        return false;
    }

    if (handle != NULL) {
        new_sound_p->attachHandle(handle);
    }

    bool startResult = new_sound_p->start(chan, soundNum);
    if (!startResult) {
        JUT_WARN(532, "%s", "cannot start SpkSound\n");
        delete new_sound_p;
        return false;
    }

    {
        JASCriticalSection cs;
        appendSound(chan, new_sound_p);
    }

    return true;
}

void SpkSoundHolder::appendSound(s32 chan, SpkSound* sound) {
    mSoundList[chan].append(sound);
}

bool SpkSoundHolder::startLevelSound(s32 chan, s32 soundNum, SpkSoundHandle* handle) {
    if (handle == NULL) {
        JUT_WARN(558, "%s", "Spk : No Handle for Level Sound");
        return false;
    }

    if (*handle) {
        if (!(*handle)->isStopping() && !(*handle)->isDead()) {
            (*handle)->setLifeTime(2);
        }
        return true;
    } else {
        bool startResult = startSound(chan, soundNum, handle);
        if (!startResult) {
            return false;
        }

        if (*handle) {
            (*handle)->setLifeTime(2);
            return true;
        }

        return false;
    }
}

void SpkSoundHolder::stopAll(s32 chan, s32 msec) {
    JASCriticalSection cs;

    for (JSULink<SpkSound>* it = mSoundList[chan].getFirst(); it != NULL; it = it->getNext()) {
        if (it->getObject() != NULL) {
            it->getObject()->stop(msec);
        }
    }
}

bool SpkSoundHolder::update(s32 chan) {
    JASCriticalSection cs;
    bool ret = updateEachSound(chan);
    return ret;
}

bool SpkSoundHolder::updateEachSound(s32 chan) {
    bool retval = false;

    f32 volume = getMasterVolume() * getConfigVolumeF32();

    for (JSULink<SpkSound>* it = mSoundList[chan].getFirst(); it != NULL; it = it->getNext()) {
        it->getObject()->update(volume * mSoundVolumes[chan]);
        retval = true;
    }

    return retval;
}

void SpkSoundHolder::freeDeadSound(s32 chan) {
    JSULink<SpkSound>* it = mSoundList[chan].getFirst();
    while (it != NULL) {
        JSULink<SpkSound>* next = it->getNext();
        if (it->getObject()->isDead()) {
            mSoundList[chan].remove(it);
            delete it->getObject();
        }
        it = next;
    }
}

void SpkSoundHolder::framework(void) {
    JASCriticalSection cs;

    for (int i = 0; i < ARRAY_SIZE(mSoundList); i++) {
        for (JSULink<SpkSound>* it = mSoundList[i].getFirst(); it != NULL; it = it->getNext()) {
            if (it->getObject() != NULL) {
                if (it->getObject()->isLocked()) {
                    it->getObject()->unlock();
                } else if (it->getObject()->getLifeTime() >= 0) {
                    it->getObject()->updateLifeTime();
                    if (it->getObject()->getLifeTime() <= 0) {
                        it->getObject()->stop(0);
                        it->getObject()->setLifeTime(-1);
                    }
                }
            }
        }

        freeDeadSound(i);
    }
}
