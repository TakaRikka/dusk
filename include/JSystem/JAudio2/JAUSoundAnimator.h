#ifndef JAUSOUNDANIMATOR_H
#define JAUSOUNDANIMATOR_H

#include "JSystem/JAudio2/JAISound.h"

class JAUSoundAnimation;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAUSoundAnimationSound {
public:
    bool playsOnlyForward() const { return (mFlags & 1) != 0; }
    bool playsOnlyReverse() const { return (mFlags & 2) != 0; }
    bool stopsWhenAnimationChanges() const { return (mFlags & 4) != 0; }
    bool playsOnlyOnce() const { return (mFlags & 8) != 0; }
    bool stopsWhenNoteOff() const { return (mFlags & 0x10) != 0; }
    bool stopsWhenSpeedIsZero() const { return (mFlags & 0x20) != 0; }
    bool setsLifeTime() const { return (mFlags & 0x40) != 0; }
    bool playsAtIntervals() const { return (mFlags & 0x80) != 0; }

    bool isNoting(f32 param_0) const {
        if (field_0x04 == field_0x08) {
            return true;
        }
        if (field_0x04 > field_0x08) {
            if (field_0x08 <= param_0 && param_0 < field_0x04) {
                return true;
            }
        } else {
            if (field_0x04 <= param_0 && param_0 < field_0x08) {
                return true;
            }
        }
        return false;
    }

    bool isNotingOn(f32 param_0, bool param_1) const {
        if (setsLifeTime()) {
            return isNoting(param_0);
        }
        if (param_1) {
            return param_0 <= field_0x04;
        } else {
            return param_0 >= field_0x04;
        }
    }

    bool isNotingOff(f32 param_0, bool param_1) const {
        if (setsLifeTime()) {
            return !isNoting(param_0);
        }
        if (field_0x04 == field_0x08) {
            return false;
        }
        if (param_1) {
            return param_0 <= field_0x08;
        } else {
            return param_0 >= field_0x08;
        }
    }

    /* 0x00 */ JAISoundID mSoundId;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ u32 mFlags;
    /* 0x14 */ u8 field_0x14;
    /* 0x15 */ u8 field_0x15;
    /* 0x16 */ u8 field_0x16;
    /* 0x17 */ u8 field_0x17;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ s8 field_0x1a;
    /* 0x1B */ u8 unk_0x1b[5];
}; /* size 0x20 */

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAUSoundAnimationControl {
public:
    virtual ~JAUSoundAnimationControl() = 0;
    virtual JAUSoundAnimationSound* getSound(const JAUSoundAnimation*, int) = 0;
    virtual u16 getNumSounds(const JAUSoundAnimation*) = 0;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAUSoundAnimation {
public:
    int getStartSoundIndex(f32) const;
    int getEndSoundIndex(f32) const;

    u16 getNumSounds() const {
        if (mControl != NULL) {
            return mControl->getNumSounds(this);
        } else {
            return mNumSounds;
        }
    }

    const JAUSoundAnimationSound* getSound(int i_index) const {
        if (mControl != NULL) {
            return mControl->getSound(this, i_index);
        } else {
            return &mSounds + i_index;
        }
    }

    /* 0x0 */ u16 mNumSounds;
    /* 0x4 */ JAUSoundAnimationControl* mControl;
    /* 0x8 */ JAUSoundAnimationSound mSounds;  // actually an array
};

#endif /* JAUSOUNDANIMATOR_H */
