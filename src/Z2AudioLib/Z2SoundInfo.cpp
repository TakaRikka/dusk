#include "Z2AudioLib/Z2SoundInfo.h"

#include "JSystem/JAudio2/JAISe.h"
#include "JSystem/JAudio2/JAISeq.h"
#include "JSystem/JAudio2/JAISoundChild.h"
#include "JSystem/JAudio2/JAIStream.h"
#include "JSystem/JAudio2/JAUSoundTable.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Z2AudioLib/Z2Calc.h"
#include "dusk/mods/svc/audio_res/bst.hpp"

struct JAUStdSoundTableType {
    static DUSK_GAME_DATA const u32 STRM_CH_SHIFT;
    struct StringOffset {
        static inline const char* getString(const void* addr, u32 offset) {
            return (const char*)addr + offset;
        }
    };
};

u16 Z2SoundInfo::getBgmSeqResourceID(JAISoundID soundID) const {
    JUT_ASSERT(20, isValid());
    JAUSoundTableItem DUSK_CONST* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID IF_DUSK_ARG(nullptr));
    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID IF_DUSK_ARG(nullptr));

    if (data != NULL) {
        switch ((typeID & 0xf0)) {
        case SOUND_TYPEID_SEQUENCE:
            return (u16)data->mResourceId;
        }
    }

    return 0xffff;
}

int Z2SoundInfo::getSoundType(JAISoundID soundID) const {
    switch (soundID.id_.info.type.parts.sectionID) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    }

    return -1;
}

int Z2SoundInfo::getCategory(JAISoundID soundID) const {
    return soundID.id_.info.type.parts.groupID;
}

u32 Z2SoundInfo::getPriority(JAISoundID soundID IF_DUSK_ARG(SoundTableReplacementSlot const* replacement)) const {
    JUT_ASSERT(63, isValid());

#if TARGET_PC
    // This function gets called at least once in a context where we can't easily pass in the replacement.
    // So check it here too.
    // It's only a tiny priority difference, highly unlikely to cause any issues.
    std::shared_ptr<SoundTableReplacementSlot> replacement_shared;
    if (!replacement) {
        replacement_shared = dusk::mods::svc::audio_res::bst::get_override_for(soundID);
        replacement = replacement_shared.get();
    }
#endif

    JAUSoundTableItem DUSK_CONST* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID IF_DUSK_ARG(replacement));
    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID IF_DUSK_ARG(replacement));

    if (data != NULL && (typeID & SOUND_TYPEID_VALID) != 0) {
        return data->mPriority;
    }

    return 0;
}

#define SW_BIT DUSK_IF_ELSE(sw_bit, getSwBit(soundID))

JAUAudibleParam Z2SoundInfo::getAudibleSwFull(JAISoundID soundID IF_DUSK_ARG(SoundTableReplacementSlot const* replacement)) {
    JAUAudibleParam audibleParam;
    JUT_ASSERT(82, isValid());
    int iVar1, uVar7;

    IF_DUSK(int sw_bit;)

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID IF_DUSK_ARG(replacement));
    switch (typeID) {
    case SOUND_TYPEID_SOUND_EFFECT:
        IF_DUSK(sw_bit = getSwBit(soundID, replacement);)

        audibleParam.field_0x0.bytes.mDopplerPower = (u32)SW_BIT >> SOUND_SW_DOPPLER_POWER_OFFSET;
        if ((SW_BIT & SOUND_SW_ALWAYS_MAX_PRIORITY) != 0) {
            audibleParam.field_0x0.bytes.mCalculatePriority = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalculatePriority = 1;
        }

        if ((SW_BIT & SOUND_SW_IGNORE_DISTANCE_VOL) != 0) {
            audibleParam.field_0x0.bytes.mCalcDistanceVolume = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalcDistanceVolume = 1;
        }

        if ((SW_BIT & SOUND_SW_IGNORE_FX_MIX) != 0) {
            audibleParam.field_0x0.bytes.mCalcFxMix = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalcFxMix = 1;
        }

        if ((SW_BIT & SOUND_SW_CULL_AT_MAX_DISTANCE) != 0) {
            audibleParam.field_0x0.bytes.mCullAtMaxDistance = 1;
        } else {
            audibleParam.field_0x0.bytes.mCullAtMaxDistance = 0;
        }

        if ((SW_BIT & SOUND_SW_IGNORE_PAN) != 0) {
            audibleParam.field_0x0.bytes.mCalcPan = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalcPan = 1;
        }

        if ((SW_BIT & SOUND_SW_IGNORE_DOLBY) != 0) {
            audibleParam.field_0x0.bytes.mCalcDolby = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalcDolby = 1;
        }

        uVar7 = 0;
        if ((SW_BIT & SOUND_SW_CLAMP_MIN_VOLUME) != 0) {
            uVar7 = 8;
        }

        iVar1 = (SW_BIT >> 16) & SOUND_VOL_DIST_BIT_MASK_SHIFTED;
        iVar1 += (SW_BIT >> 16) & SOUND_VOL_DIST_BIT_2_MASK_SHIFTED;
        iVar1 += (SW_BIT >> 16) & SOUND_VOL_SOMETHING_MASK_SHIFTED;
        audibleParam.field_0x0.bytes.mClampMinVolume = uVar7;
        audibleParam.field_0x0.half.f1 = iVar1;
        break;
    default:
        audibleParam.field_0x0.bytes.mDopplerPower = 0;
        audibleParam.field_0x0.bytes.mCalculatePriority = 1;
        audibleParam.field_0x0.bytes.mCalcDistanceVolume = 1;
        audibleParam.field_0x0.bytes.mCalcFxMix = 1;
        audibleParam.field_0x0.bytes.mCullAtMaxDistance = 0;
        audibleParam.field_0x0.bytes.mCalcPan = 1;
        audibleParam.field_0x0.bytes.mCalcDolby = 1;
        audibleParam.field_0x0.bytes.mClampMinVolume = 0;
        audibleParam.field_0x0.half.f1 = 0;
        break;
    }

    return audibleParam;
}

u16 Z2SoundInfo::getAudibleSw(JAISoundID soundID IF_DUSK_ARG(SoundTableReplacementSlot const* replacement)) const {
    JAUAudibleParam audibleParam;
    JUT_ASSERT(184, isValid());
    int iVar1, uVar7;

    IF_DUSK(int sw_bit);

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID IF_DUSK_ARG(replacement));
    switch (typeID) {
    case SOUND_TYPEID_SOUND_EFFECT:
        IF_DUSK(sw_bit = getSwBit(soundID, replacement);)

        audibleParam.field_0x0.bytes.mDopplerPower = (u32)SW_BIT >> SOUND_SW_DOPPLER_POWER_OFFSET;
        if ((SW_BIT & SOUND_SW_ALWAYS_MAX_PRIORITY) != 0) {
            audibleParam.field_0x0.bytes.mCalculatePriority = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalculatePriority = 1;
        }

        if ((SW_BIT & SOUND_SW_IGNORE_DISTANCE_VOL) != 0) {
            audibleParam.field_0x0.bytes.mCalcDistanceVolume = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalcDistanceVolume = 1;
        }

        if ((SW_BIT & SOUND_SW_IGNORE_FX_MIX) != 0) {
            audibleParam.field_0x0.bytes.mCalcFxMix = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalcFxMix = 1;
        }

        if ((SW_BIT & SOUND_SW_CULL_AT_MAX_DISTANCE) != 0) {
            audibleParam.field_0x0.bytes.mCullAtMaxDistance = 1;
        } else {
            audibleParam.field_0x0.bytes.mCullAtMaxDistance = 0;
        }

        if ((SW_BIT & SOUND_SW_IGNORE_PAN) != 0) {
            audibleParam.field_0x0.bytes.mCalcPan = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalcPan = 1;
        }

        if ((SW_BIT & SOUND_SW_IGNORE_DOLBY) != 0) {
            audibleParam.field_0x0.bytes.mCalcDolby = 0;
        } else {
            audibleParam.field_0x0.bytes.mCalcDolby = 1;
        }

        uVar7 = 0;
        if ((SW_BIT & SOUND_SW_CLAMP_MIN_VOLUME) != 0) {
            uVar7 = 8;
        }

        iVar1 = (SW_BIT >> 16) & SOUND_VOL_DIST_BIT_MASK_SHIFTED;
        iVar1 += (SW_BIT >> 16) & SOUND_VOL_DIST_BIT_2_MASK_SHIFTED;
        iVar1 += (SW_BIT >> 16) & SOUND_VOL_SOMETHING_MASK_SHIFTED;
        audibleParam.field_0x0.bytes.mClampMinVolume = uVar7;
        audibleParam.field_0x0.half.f1 = iVar1;
        break;
    default:
        audibleParam.field_0x0.half.f0 = 0xffff;
        audibleParam.field_0x0.half.f1 = 0xffff;
        break;
    }

    return audibleParam.field_0x0.half.f0;
}

#undef SW_BIT

void Z2SoundInfo::getSeInfo(JAISoundID soundID, JAISe* sePtr IF_DUSK_ARG(SoundEffectReplacementSlot const* replacement)) const {
    getSoundInfo_(soundID, sePtr);
    JUT_ASSERT(292, isValid());

    JAUSoundTableItem DUSK_CONST* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID IF_DUSK_ARG(replacement));
    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID IF_DUSK_ARG(replacement));
    if (data == NULL) {
        return;
    }

    switch(typeID) {
    case SOUND_TYPEID_SOUND_EFFECT:
        sePtr->getProperty().mPitch *= data->mPitch;
        u32 pitchParam = (getSwBit(soundID IF_DUSK_ARG(replacement)) & SOUND_SW_RANDOM_PITCH_MASK) >> SOUND_SW_RANDOM_PITCH_OFFSET;
        if (pitchParam > 8) {
            sePtr->getProperty().mPitch += Z2Calc::linearTransform(pitchParam, 8.0f, 15.0f, 16.0f, 24.0f, true) / 48.0f * Z2Calc::getRandom_0_1();
        } else {
            sePtr->getProperty().mPitch += (pitchParam / 48.0f) * Z2Calc::getRandom_0_1();
        }

        u32 uVar1 = (u32)getSwBit(soundID IF_DUSK_ARG(replacement)) >> SOUND_SW_RANDOM_VOLUME_OFFSET;
        if (uVar1 != 0) {
            f32 dVar18 = (uVar1 / 15.0f) * Z2Calc::getRandom_0_1();
            sePtr->getProperty().mVolume -= dVar18 < 0.0f ? 0.0f : (dVar18 > 1.0f ? 1.0f : dVar18);
        }
        break;
    }
}

void Z2SoundInfo::getSeqInfo(JAISoundID soundID, JAISeq* seqPtr) const {
    getSoundInfo_(soundID, seqPtr);
}

DUSK_GAME_DATA const u32 JAUStdSoundTableType::STRM_CH_SHIFT = STRM_CH_SHIFT_;

void Z2SoundInfo::getStreamInfo(JAISoundID soundID, JAIStream* streamPtr IF_DUSK_ARG(StreamReplacementSlot const* replacement)) const {
    int numChild;
    JAUSoundTableItem DUSK_CONST* data;
    getSoundInfo_(soundID, streamPtr);
    JUT_ASSERT(349, isValid());

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID IF_DUSK_ARG(replacement));
    switch (typeID & 0xf0) {
    case SOUND_TYPEID_STREAM:
        u16 uVar1;
        s32 iVar4;
        data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID IF_DUSK_ARG(replacement));
        JUT_ASSERT(356, data);

        uVar1 = data->mStreamPanParameters;
        numChild = streamPtr->getNumChild();
        iVar4 = 0;
        for (; iVar4 < numChild && uVar1 != 0; uVar1 >>= JAUStdSoundTableType::STRM_CH_SHIFT, iVar4++) {
            u32 uVar2 = uVar1 & 3;
            if (uVar2 != 0) {
                JAISoundChild* child = streamPtr->getChild(iVar4);
                if (child != NULL) {
                    switch (uVar2) {
                    case STRM_CH_CENTER:
                        child->mMove.params_.mPan = 0.5f;
                        break;
                    case STRM_CH_LEFT:
                        child->mMove.params_.mPan = 0.0f;
                        break;
                    case STRM_CH_RIGHT:
                        child->mMove.params_.mPan = 1.0f;
                        break;
                    }
                }
            }
        }
    }
}

const char* Z2SoundInfo::getStreamFilePath(JAISoundID soundID IF_DUSK_ARG(StreamReplacementSlot const* replacement)) {
    JUT_ASSERT(387, isValid());
    JAUSoundTableItem DUSK_CONST* data;
    const void* resource;

    switch (JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID IF_DUSK_ARG(replacement)) & 0xf0) {
    case SOUND_TYPEID_STREAM:
#if TARGET_PC
        if (replacement) {
            return replacement->file_path.c_str();
        }
#endif

        data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID IF_DUSK_ARG(replacement));
        JUT_ASSERT(394, data);
        resource = JASGlobalInstance<JAUSoundTable>::getInstance()->getResource();
        JUT_ASSERT(398, resource);
        return JAUStdSoundTableType::StringOffset::getString(resource, data->mStreamFilePath);
    default:
        return NULL;
    }
}

s32 Z2SoundInfo::getStreamFileEntry(JAISoundID soundID IF_DUSK_ARG(StreamReplacementSlot const* replacement)) {
    const char* path = getStreamFilePath(soundID IF_DUSK_ARG(replacement));
    return !path ? -1 : DVDConvertPathToEntrynum(path);
}

int Z2SoundInfo::getSwBit(JAISoundID soundID IF_DUSK_ARG(SoundTableReplacementSlot const* replacement)) const {
    JUT_ASSERT(418, isValid());
    JAUSoundTableItem DUSK_CONST* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID IF_DUSK_ARG(replacement));

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID IF_DUSK_ARG(replacement));
    if (data != NULL) {
        switch(typeID) {
        case SOUND_TYPEID_SOUND_EFFECT:
            return data->mSwBit;
        }
    }

    return 0xFFFFFFFF;
}

void Z2SoundInfo::getSoundInfo_(JAISoundID soundID, JAISound* soundPtr) const {
    JUT_ASSERT(440, isValid());
    JAUSoundTableItem DUSK_CONST* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID IF_DUSK_ARG(soundPtr->getReplacement()));

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID IF_DUSK_ARG(soundPtr->getReplacement()));
    if (data != NULL && (typeID & SOUND_TYPEID_VALID) != 0) {
        soundPtr->getProperty().mVolume = (1.0f / 127.0f) * data->mVolume;
    }
}
