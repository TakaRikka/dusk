#include "Z2AudioLib/Z2SoundInfo.h"
#include "JSystem/JAudio2/JAISeq.h"
#include "JSystem/JAudio2/JAISe.h"
#include "JSystem/JAudio2/JAISoundChild.h"
#include "JSystem/JAudio2/JAIStream.h"
#include "JSystem/JAudio2/JAUSoundTable.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Z2AudioLib/Z2Calc.h"

struct JAUStdSoundTableType {
    static const u32 STRM_CH_SHIFT;
    struct StringOffset {
        static inline const char* getString(const void* addr, u32 offset) {
            return (const char*)addr + offset;
        }
    };
};

u16 Z2SoundInfo::getBgmSeqResourceID(JAISoundID soundID) const {
    JUT_ASSERT(20, isValid());
    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    if (data != NULL) {
        switch ((typeID & 0xf0)) {
        case 0x60:
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

u32 Z2SoundInfo::getPriority(JAISoundID soundID) const {
    JUT_ASSERT(63, isValid());
    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    if (data != NULL && (typeID & 0x40) != 0) {
        return data->mPriority;
    }

    return 0;
}

JAUAudibleParam Z2SoundInfo::getAudibleSwFull(JAISoundID soundID) {
    JAUAudibleParam audibleParam;
    JUT_ASSERT(82, isValid());
    int iVar1, uVar7;

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);
    switch (typeID) {
    case 81:
        audibleParam.field_0x0.bytes.b0_0 = (u32)getSwBit(soundID) >> 8;
        if ((getSwBit(soundID) & 1) != 0) {
            audibleParam.field_0x0.bytes.b0_4 = 0;
        } else {
            audibleParam.field_0x0.bytes.b0_4 = 1;
        }

        if ((getSwBit(soundID) & 2) != 0) {
            audibleParam.field_0x0.bytes.b0_5 = 0;
        } else {
            audibleParam.field_0x0.bytes.b0_5 = 1;
        }

        if ((getSwBit(soundID) & 4) != 0) {
            audibleParam.field_0x0.bytes.b0_6 = 0;
        } else {
            audibleParam.field_0x0.bytes.b0_6 = 1;
        }

        if ((getSwBit(soundID) & 0x800000) != 0) {
            audibleParam.field_0x0.bytes.b0_7 = 1;
        } else {
            audibleParam.field_0x0.bytes.b0_7 = 0;
        }

        if ((getSwBit(soundID) & 0x1000) != 0) {
            audibleParam.field_0x0.bytes.b1_0 = 0;
        } else {
            audibleParam.field_0x0.bytes.b1_0 = 1;
        }

        if ((getSwBit(soundID) & 0x2000) != 0) {
            audibleParam.field_0x0.bytes.b1_1 = 0;
        } else {
            audibleParam.field_0x0.bytes.b1_1 = 1;
        }

        uVar7 = 0;
        if ((getSwBit(soundID) & 0x80000) != 0) {
            uVar7 = 8;
        }

        iVar1 = (getSwBit(soundID) >> 16) & 0x7;
        iVar1 += (getSwBit(soundID) >> 16) & 0x70;
        iVar1 += (getSwBit(soundID) >> 16) & 0xf00;
        audibleParam.field_0x0.bytes.b1_2_7 = uVar7;
        audibleParam.field_0x0.half.f1 = iVar1;
        break;
    default:
        audibleParam.field_0x0.bytes.b0_0 = 0;
        audibleParam.field_0x0.bytes.b0_4 = 1;
        audibleParam.field_0x0.bytes.b0_5 = 1;
        audibleParam.field_0x0.bytes.b0_6 = 1;
        audibleParam.field_0x0.bytes.b0_7 = 0;
        audibleParam.field_0x0.bytes.b1_0 = 1;
        audibleParam.field_0x0.bytes.b1_1 = 1;
        audibleParam.field_0x0.bytes.b1_2_7 = 0;
        audibleParam.field_0x0.half.f1 = 0;
        break;
    }

    return audibleParam;
}

u16 Z2SoundInfo::getAudibleSw(JAISoundID soundID) const {
    JAUAudibleParam audibleParam;
    JUT_ASSERT(184, isValid());
    int iVar1, uVar7;

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);
    switch (typeID) {
    case 81:
        audibleParam.field_0x0.bytes.b0_0 = (u32)getSwBit(soundID) >> 8;
        if ((getSwBit(soundID) & 1) != 0) {
            audibleParam.field_0x0.bytes.b0_4 = 0;
        } else {
            audibleParam.field_0x0.bytes.b0_4 = 1;
        }

        if ((getSwBit(soundID) & 2) != 0) {
            audibleParam.field_0x0.bytes.b0_5 = 0;
        } else {
            audibleParam.field_0x0.bytes.b0_5 = 1;
        }

        if ((getSwBit(soundID) & 4) != 0) {
            audibleParam.field_0x0.bytes.b0_6 = 0;
        } else {
            audibleParam.field_0x0.bytes.b0_6 = 1;
        }

        if ((getSwBit(soundID) & 0x800000) != 0) {
            audibleParam.field_0x0.bytes.b0_7 = 1;
        } else {
            audibleParam.field_0x0.bytes.b0_7 = 0;
        }

        if ((getSwBit(soundID) & 0x1000) != 0) {
            audibleParam.field_0x0.bytes.b1_0 = 0;
        } else {
            audibleParam.field_0x0.bytes.b1_0 = 1;
        }

        if ((getSwBit(soundID) & 0x2000) != 0) {
            audibleParam.field_0x0.bytes.b1_1 = 0;
        } else {
            audibleParam.field_0x0.bytes.b1_1 = 1;
        }

        uVar7 = 0;
        if ((getSwBit(soundID) & 0x80000) != 0) {
            uVar7 = 8;
        }

        iVar1 = (getSwBit(soundID) >> 16) & 0x7;
        iVar1 += (getSwBit(soundID) >> 16) & 0x70;
        iVar1 += (getSwBit(soundID) >> 16) & 0xf00;
        audibleParam.field_0x0.bytes.b1_2_7 = uVar7;
        audibleParam.field_0x0.half.f1 = iVar1;
        break;
    default:
        audibleParam.field_0x0.half.f0 = 0xffff;
        audibleParam.field_0x0.half.f1 = 0xffff;
        break;
    }

    return audibleParam.field_0x0.half.f0;
}

void Z2SoundInfo::getSeInfo(JAISoundID soundID, JAISe* sePtr) const {
    getSoundInfo_(soundID, sePtr);
    JUT_ASSERT(292, isValid());

    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);
    if (data == NULL) {
        return;
    }

    switch(typeID) {
    case 81:
        sePtr->getProperty().field_0x8 *= data->field_0x8;
        u32 uStack_6c = (getSwBit(soundID) & 0xf0) >> 4;
        if (uStack_6c > 8) {
            sePtr->getProperty().field_0x8 += Z2Calc::linearTransform(uStack_6c, 8.0f, 15.0f, 16.0f, 24.0f, true) / 48.0f * Z2Calc::getRandom_0_1();
        } else {
            sePtr->getProperty().field_0x8 += (uStack_6c / 48.0f) * Z2Calc::getRandom_0_1();
        }

        u32 uVar1 = (u32)getSwBit(soundID) >> 0x1c;
        if (uVar1 != 0) {
            f32 dVar18 = (uVar1 / 15.0f) * Z2Calc::getRandom_0_1();
            sePtr->getProperty().field_0x0 -= dVar18 < 0.0f ? 0.0f : (dVar18 > 1.0f ? 1.0f : dVar18);
        }
        break;
    }
}

void Z2SoundInfo::getSeqInfo(JAISoundID soundID, JAISeq* seqPtr) const {
    getSoundInfo_(soundID, seqPtr);
}

const u32 JAUStdSoundTableType::STRM_CH_SHIFT = 2;

void Z2SoundInfo::getStreamInfo(JAISoundID soundID, JAIStream* streamPtr) const {
    int numChild;
    JAUSoundTableItem* data;
    getSoundInfo_(soundID, streamPtr);
    JUT_ASSERT(349, isValid());

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);
    switch (typeID & 0xf0) {
    case 0x70:
        u16 uVar1;
        s32 iVar4;
        data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
        JUT_ASSERT(356, data);

        uVar1 = data->mResourceId;
        numChild = streamPtr->getNumChild();
        iVar4 = 0;
        for (; iVar4 < numChild && uVar1 != 0; uVar1 >>= JAUStdSoundTableType::STRM_CH_SHIFT, iVar4++) {
            u32 uVar2 = uVar1 & 3;
            if (uVar2 != 0) {
                JAISoundChild* child = streamPtr->getChild(iVar4);
                if (child != NULL) {
                    switch (uVar2) {
                    case 1:
                        child->mMove.params_.mPan = 0.5f;
                        break;
                    case 2:
                        child->mMove.params_.mPan = 0.0f;
                        break;
                    case 3:
                        child->mMove.params_.mPan = 1.0f;
                        break;
                    }
                }
            }
        }
    }
}

const char* Z2SoundInfo::getStreamFilePath(JAISoundID soundID) {
    JUT_ASSERT(387, isValid());
    JAUSoundTableItem* data;
    const void* resource;

    switch (JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID) & 0xf0) {
    case 0x70:
        data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
        JUT_ASSERT(394, data);
        resource = JASGlobalInstance<JAUSoundTable>::getInstance()->getResource();
        JUT_ASSERT(398, resource);
        return JAUStdSoundTableType::StringOffset::getString(resource, data->field_0x4);
    default:
        return NULL;
    }
}

s32 Z2SoundInfo::getStreamFileEntry(JAISoundID soundID) {
    const char* path = getStreamFilePath(soundID);
    return !path ? -1 : DVDConvertPathToEntrynum(path);
}

int Z2SoundInfo::getSwBit(JAISoundID soundID) const {
    JUT_ASSERT(418, isValid());
    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);
    if (data != NULL) {
        switch(typeID) {
        case 81:
            return data->field_0x4;
        }
    }

    return 0xFFFFFFFF;
}

void Z2SoundInfo::getSoundInfo_(JAISoundID soundID, JAISound* soundPtr) const {
    JUT_ASSERT(440, isValid());
    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);

    u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);
    if (data != NULL && (typeID & 0x40) != 0) {
        soundPtr->getProperty().field_0x0 = (1.0f / 127.0f) * data->field_0x1;
    }
}
