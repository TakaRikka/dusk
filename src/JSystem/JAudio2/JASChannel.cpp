#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JSupport/JSupport.h"
#include "JSystem/JMath/JMATrigonometric.h"
#include "JSystem/JGeometry.h"

OSMessageQueue JASChannel::sBankDisposeMsgQ;

OSMessage JASChannel::sBankDisposeMsg[16];

OSMessage JASChannel::sBankDisposeList[16];

int JASChannel::sBankDisposeListSize;

JASChannel::JASChannel(Callback i_callback, void* i_callbackData) :
    mStatus(STATUS_INACTIVE),
    mDspCh(NULL),
    mCallback(i_callback),
    mCallbackData(i_callbackData),
    mUpdateTimer(0),
    mBankDisposeID(NULL),
    mKey(0),
    mVelocity(0x7f),
    mKeySweep(0.0f),
    mKeySweepTarget(0.0f),
    mKeySweepCount(0),
    mSkipSamples(0)
{
    field_0xdc.field_0x0 = 0;
    field_0x104 = 0;
    mMixConfig[0].whole = 0x150;
    mMixConfig[1].whole = 0x210;
    mMixConfig[2].whole = 0x352;
    mMixConfig[3].whole = 0x412;
    mMixConfig[4].whole = 0;
    mMixConfig[5].whole = 0;
    mPriority = 0x13f;
    mPauseFlag = false;
}

JASChannel::~JASChannel() {
    if (mDspCh != NULL) {
        mDspCh->drop();
    }
    if (mCallback != NULL) {
        mCallback(CB_STOP, this, NULL, mCallbackData);
    }
}


int JASChannel::play() {
    JASDSPChannel* channel = JASDSPChannel::alloc(JSULoByte(mPriority), dspUpdateCallback, this);
    if (channel == NULL) {
        delete this;
        return 0;
    }
    mDspCh = channel;
    channel->start();
    mStatus = STATUS_ACTIVE;
    return 1;
}

int JASChannel::playForce() {
    JASDSPChannel* channel = JASDSPChannel::allocForce(JSULoByte(mPriority),
                                                       dspUpdateCallback, this);
    if (channel == NULL) {
        delete this;
        return 0;
    }
    mDspCh = channel;
    channel->start();
    mStatus = STATUS_ACTIVE;
    return 1;
}

void JASChannel::release(u16 i_directRelease) {
    if (mStatus == STATUS_ACTIVE) {
        if (i_directRelease != 0) {
            setDirectRelease(i_directRelease);
        }
        for (u32 i = 0; i < 2; i++) {
            if (mOscillators[i].isValid()) {
                mOscillators[i].release();
            }
        }
        mDspCh->setPriority(JSUHiByte(mPriority));
        mStatus = STATUS_RELEASE;
    }
}

void JASChannel::setOscInit(u32 i_index, JASOscillator::Data const* i_data) {
    mOscillators[i_index].initStart(i_data);
}

void JASChannel::setMixConfig(u32 i_index, u16 i_config) {
    mMixConfig[i_index].whole = i_config;
}

f32 JASChannel::calcEffect(JASChannel::PanVector const* i_vector) {
    return i_vector->mSound + i_vector->mEffect + i_vector->mChannel;
}

f32 JASChannel::calcPan(JASChannel::PanVector const* i_vector) {
    return 0.5f + (i_vector->mSound - 0.5f) + (i_vector->mEffect - 0.5f)
        + (i_vector->mChannel - 0.5f);
}

void JASChannel::effectOsc(u32 i_index, JASOscillator::EffectParams* i_params) {
    f32 value = mOscillators[i_index].getValue();
    switch (mOscillators[i_index].getTarget()) {
    case JASOscillator::TARGET_PITCH:
        i_params->mPitch *= value;
        break;
    case JASOscillator::TARGET_VOLUME:
        i_params->mVolume *= value;
        break;
    case JASOscillator::TARGET_PAN:
        value -= 0.5;
        i_params->mPan += value;
        break;
    case JASOscillator::TARGET_FXMIX:
        i_params->mFxMix += value;
        break;
    case JASOscillator::TARGET_DOLBY:
        i_params->mDolby += value;
        break;
    case JASOscillator::TARGET_5:
        i_params->_14 *= value;
        break;
    case JASOscillator::TARGET_6:
        i_params->_18 *= value;
        break;
    }
}

void JASChannel::setKeySweepTarget(s32 i_target, u32 i_count) {
    if (i_count == 0) {
        mKeySweep = i_target;
    } else {
        mKeySweep = 0.0f;
        mKeySweepTarget = i_target;
    }
    mKeySweepCount = i_count;
}

void JASChannel::updateEffectorParam(JASDsp::TChannel* i_channel, u16* i_mixerVolume,
                                     JASOscillator::EffectParams const& i_params) {
    PanVector pan_vector, fxmix_vector, dolby_vector;
    pan_vector.mSound = mSoundParams.mPan;
    pan_vector.mChannel = mParams.mPan;
    pan_vector.mEffect = i_params.mPan;
    fxmix_vector.mSound = mSoundParams.mFxMix;
    fxmix_vector.mChannel = mParams.mFxMix;
    fxmix_vector.mEffect = i_params.mFxMix;
    dolby_vector.mSound = mSoundParams.mDolby;
    dolby_vector.mChannel = mParams.mDolby;
    dolby_vector.mEffect = i_params.mDolby;

    f32 pan = 0.5f;
    f32 dolby = 0.0f;
    switch (JASDriver::getOutputMode()) {
    case 0:
        break;
    case 1:
        pan = calcPan(&pan_vector);
        break;
    case 2:
        pan = calcPan(&pan_vector);
        dolby = calcEffect(&dolby_vector);
        break;
    }
    f32 fxmix = calcEffect(&fxmix_vector);
    f32 volume = mVelocity / 127.0f;
    volume = volume * volume;
    volume = mSoundParams.mVolume * i_params.mVolume * mParams.mVolume
        * (i_params._18 * mTremolo.getValue() + 1.0f) * volume;
    
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    pan = JASCalc::clamp01(pan);
    fxmix = JASCalc::clamp01(fxmix);
    dolby = JASCalc::clamp01(dolby);

    if (isDolbyMode()) {
        updateAutoMixer(i_channel, volume, pan, fxmix, dolby);
    } else {
        updateMixer(volume, pan, fxmix, dolby, i_mixerVolume);
    }
}

s32 JASChannel::dspUpdateCallback(u32 i_type, JASDsp::TChannel* i_channel, void* i_this) {
    JASChannel* _this = static_cast<JASChannel*>(i_this);
    switch (i_type) {
    case JASDSPChannel::CB_PLAY:
        return _this->updateDSPChannel(i_channel);
    case JASDSPChannel::CB_START:
        return _this->initialUpdateDSPChannel(i_channel);
    case JASDSPChannel::CB_STOP:
    case JASDSPChannel::CB_DROP:
        _this->mDspCh->free();
        _this->mDspCh = NULL;
        delete _this;
        return -1;
    }
    return 0;
}

s32 JASChannel::initialUpdateDSPChannel(JASDsp::TChannel* i_channel) {
    if (isDolbyMode()) {
        i_channel->initAutoMixer();
    }
    
    if (mCallback != NULL) {
        mCallback(CB_START, this, i_channel, mCallbackData);
    }

    if (field_0xdc.field_0x4.field_0x20[0] == 0) {
        mDspCh->free();
        mDspCh = NULL;
        delete this;
        return -1;
    }
    
    if (checkBankDispose()) {
        mDspCh->free();
        mDspCh = NULL;
        delete this;
        return -1;
    }

    switch (field_0xdc.field_0x0) {
    case 0:
        i_channel->setWaveInfo(field_0xdc.field_0x4, field_0x104, mSkipSamples);
        break;
    case 2:
        i_channel->setOscInfo(field_0x104);
        break;
    }

    for (u8 i = 0; i < 6; i++) {
        MixConfig mix_config = mMixConfig[i];
        u32 output_mode = JASDriver::getOutputMode();
        if (output_mode == 0) {
            switch (mix_config.parts.upper) {
            case 8:
                mix_config.parts.upper = 11;
                break;
            case 9:
                mix_config.parts.upper = 2;
                break;
            }
        } else if (output_mode == 1 && mix_config.parts.upper == 8) {
            mix_config.parts.upper = 11;
        }
        i_channel->setBusConnect(i, mix_config.parts.upper);
    }

    JASOscillator::EffectParams effect_params;
    for (u32 i = 0; i < 2; i++) {
        if (mOscillators[i].isValid()) {
            mOscillators[i].update();
            effectOsc(i, &effect_params);
        }
    }
    mVibrate.resetCounter();
    mTremolo.resetCounter();
    u16 mixer_volume[6];
    updateEffectorParam(i_channel, mixer_volume, effect_params);
    for (u8 i = 0; i < 6; i++) {
        i_channel->setMixerInitVolume(i, mixer_volume[i]);
    }

    f32 pitch = JASCalc::pow2(mParams.field_0x8 + (mKey + mKeySweep) / 12.0f
                                                + effect_params._14 * mVibrate.getValue());
    pitch = mSoundParams.mPitch * effect_params.mPitch * pitch * mParams.mPitch * 4096.0f;
    if (pitch < 0.0f) {
        pitch = 0.0f;
    }
    i_channel->setPitch(pitch);

    i_channel->setPauseFlag(mPauseFlag);
    i_channel->field_0x066 = 0;
    return 0;
}

s32 JASChannel::updateDSPChannel(JASDsp::TChannel* i_channel) {
    if (mCallback != NULL) {
        mCallback(CB_PLAY, this, i_channel, mCallbackData);
    }

    if (field_0xdc.field_0x4.field_0x20[0] == 0) {
        mDspCh->free();
        mDspCh = NULL;
        delete this;
        return -1;
    }
    
    if (checkBankDispose()) {
        mDspCh->free();
        mDspCh = NULL;
        delete this;
        return -1;
    }

    i_channel->setPauseFlag(mPauseFlag);
    JASOscillator::EffectParams effect_params;
    if (mPauseFlag) {
        if (mOscillators[0].isRelease()) {
            mDspCh->free();
            mDspCh = NULL;
            delete this;
            return -1;
        }
    } else {
        f32 inc = 32028.5f / JASDriver::getDacRate();
        mVibrate.incCounter(inc);
        mTremolo.incCounter(inc);
        if (mUpdateTimer != 0) {
            mUpdateTimer--;
            if (mUpdateTimer == 0 && mCallback != NULL) {
                mCallback(CB_TIMER, this, i_channel, mCallbackData);
            }
        }
        inc = 48000.0f / JASDriver::getDacRate();
        for (u32 i = 0; i < 2; i++) {
            if (mOscillators[i].isValid()) {
                mOscillators[i].incCounter(inc);
                effectOsc(i, &effect_params);
                if (i == 0 && mOscillators[i].isStop()) {
                    mDspCh->free();
                    mDspCh = NULL;
                    delete this;
                    return -1;
                }
            }
        }
    }

    u16 mixer_volume[6];
    updateEffectorParam(i_channel, mixer_volume, effect_params);
    for (u8 i = 0; i < 6; i++) {
        i_channel->setMixerVolume(i, mixer_volume[i]);
    }

    f32 pitch = JASCalc::pow2(mParams.field_0x8 + (mKey + mKeySweep) / 12.0f
                                                + effect_params._14 * mVibrate.getValue());
    pitch = mSoundParams.mPitch * effect_params.mPitch * pitch * mParams.mPitch * 4096.0f;
    if (pitch < 0.0f) {
        pitch = 0.0f;
    }
    i_channel->setPitch(pitch);

    if (!mPauseFlag && mKeySweepCount != 0) {
        mKeySweep += (mKeySweepTarget - mKeySweep) / mKeySweepCount;
        mKeySweepCount--;
    }
    return 0;
}

void JASChannel::updateAutoMixer(JASDsp::TChannel* i_channel, f32 param_1, f32 param_2,
                                 f32 param_3, f32 param_4) {
    if (JASDriver::getOutputMode() == 0) {
        param_1 *= 0.707f;
    }
    f32 fvar1 = JASCalc::clamp01(param_1);
    i_channel->setAutoMixer(fvar1 * JASDriver::getChannelLevel_dsp(), param_2 * 127.5f,
                            param_4 * 127.5f, param_3 * 127.5f, 0);
}

void JASChannel::updateMixer(f32 i_volume, f32 i_pan, f32 i_fxmix, f32 i_dolby, u16* i_volumeOut) {
    for (u32 i = 0; i < 6; i++) {
        f32 volume = i_volume;
        MixConfig config = mMixConfig[i];
        if (config.parts.upper == 0) {
            i_volumeOut[i] = 0;
        } else {
            f32 scale;

            if (config.parts.lower0 != 0) {
                switch (config.parts.lower0) {
                case 1:
                    scale = i_pan;
                    break;
                case 2:
                    scale = i_fxmix;
                    break;
                case 3:
                    scale = i_dolby;
                    break;
                case 5:
                    scale = 1.0f - i_pan;
                    break;
                case 6:
                    scale = 1.0f - i_fxmix;
                    break;
                case 7:
                    scale = 1.0f - i_dolby;
                    break;
                }

                switch (config.parts.lower0) {
                case 2:
                case 6:
                    volume *= scale;
                    break;
                default:
                    if (JASDriver::getOutputMode() == 0) {
                        volume *= scale;
                    } else {
                        volume *= JMASinRadian(scale * JGeometry::TUtil<f32>::PI() * 0.5f);
                    }
                    break;
                }
            }

            if (config.parts.lower1 != 0) {
                switch (config.parts.lower1) {
                case 1:
                    scale = i_pan;
                    break;
                case 2:
                    scale = i_fxmix;
                    break;
                case 3:
                    scale = i_dolby;
                    break;
                case 5:
                    scale = 1.0f - i_pan;
                    break;
                case 6:
                    scale = 1.0f - i_fxmix;
                    break;
                case 7:
                    scale = 1.0f - i_dolby;
                    break;
                }

                switch (config.parts.lower1) {
                case 3:
                case 7:
                    volume *= JMASinRadian((scale * 0.34776f + 0.32612f)
                                           * JGeometry::TUtil<f32>::PI() * 0.5f);
                    break;
                case 2:
                case 6:
                    volume *= scale;
                    break;
                default:
                    if (JASDriver::getOutputMode() == 0) {
                        volume *= scale;
                    } else {
                        volume *= JMASinRadian(scale * JGeometry::TUtil<f32>::PI() * 0.5f);
                    }
                    break;
                }
            }

            i_volumeOut[i] = JASCalc::clamp01(volume) * JASDriver::getChannelLevel_dsp();
        }
    }
}

void JASChannel::free() {
    mCallback = NULL;
    mCallbackData = NULL;
}

void JASChannel::initBankDisposeMsgQueue() {
    OSInitMessageQueue(&sBankDisposeMsgQ, sBankDisposeMsg, 0x10);
    sBankDisposeListSize = 0;
}

void JASChannel::receiveBankDisposeMsg() {
    OSMessage msg;
    for (sBankDisposeListSize = 0;
         OSReceiveMessage(&sBankDisposeMsgQ, &msg, OS_MESSAGE_NOBLOCK);
         sBankDisposeListSize++)
    {
        sBankDisposeList[sBankDisposeListSize] = msg;
    }
}

bool JASChannel::checkBankDispose() const {
    if (mBankDisposeID == NULL) {
        return false;
    }
    for (int i = 0; i < sBankDisposeListSize; i++) {
        if (mBankDisposeID == sBankDisposeList[i]) {
            return true;
        }
    }
    return false;
}
