//
// Translation Unit: JAUSoundAnimator
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUSoundAnimator.h"

int JAUSoundAnimation::getStartSoundIndex(f32 param_0) const {
    int i;
    for (i = 0; i < getNumSounds(); i++) {
        if (getSound(i)->field_0x04 >= param_0) {
            break;
        }
    }
    return i;
}

int JAUSoundAnimation::getEndSoundIndex(f32 param_0) const {
    int i;
    for (i = 0; i < getNumSounds(); i++) {
        if (getSound(i)->field_0x04 > param_0) {
            break;
        }
    }
    return i;
}
