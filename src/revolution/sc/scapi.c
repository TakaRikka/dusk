#include <revolution/sc.h>

u8 SCGetAspectRatio(void) {
    u8 aspect;

    if (!SCFindU8Item(&aspect, SC_ITEM_ID_IPL_ASPECT_RATIO)) {
        aspect = 0;
    } else {
        if (aspect != 1) {
            aspect = 0;
        }
    }

    return aspect;
}

s8 SCGetDisplayOffsetH(void) {
    s8 offset;

    if (!SCFindS8Item(&offset, SC_ITEM_ID_IPL_DISPLAY_OFFSET_H)) {
        offset = 0;
    } else {
        if (offset < -32) {
            offset = -32;
        } else if (offset > 32) {
            offset = 32;
        }
    }

    offset &= ~1;
    return offset;
}

u8 SCGetEuRgb60Mode(void) {
    u8 mode;

    if (!SCFindU8Item(&mode, SC_ITEM_ID_IPL_EURGB60_MODE)) {
        mode = 0;
    } else {
        if (mode != 1) {
            mode = 0;
        }
    }

    return mode;
}

BOOL SCGetIdleMode(SCIdleModeInfo* info) {
    return SCFindByteArrayItem(info, sizeof(*info), SC_ITEM_ID_IPL_IDLE_MODE);
}

u8 SCGetLanguage(void) {
    u8 lang;
    s8 area;

    if (!SCFindU8Item(&lang, SC_ITEM_ID_IPL_LANGUAGE)) {
        area = SCGetProductArea();

        if (area == 0) {
            lang = SC_LANG_JAPANESE;
        } else {
            lang = SC_LANG_ENGLISH;
        }
    } else {
        if (lang > SC_LANG_KOREAN) {
            lang = SC_LANG_ENGLISH;
        }
    }

    return lang;
}

u8 SCGetProgressiveMode(void) {
    u8 mode;

    if (!SCFindU8Item(&mode, SC_ITEM_ID_IPL_PROGRESSIVE_MODE)) {
        mode = 0;
    } else {
        if (mode != 1) {
            mode = 0;
        }
    }

    return mode;
}

u8 SCGetScreenSaverMode(void) {
    u8 mode;

    if (!SCFindU8Item(&mode, SC_ITEM_ID_IPL_SCREEN_SAVER_MODE)) {
        mode = 1;
    } else {
        if (mode != 1) {
            mode = 0;
        }
    }

    return mode;
}

u8 SCGetSoundMode(void) {
    u8 mode;

    if (!SCFindU8Item(&mode, SC_ITEM_ID_IPL_SOUND_MODE)) {
        mode = 1;
    } else {
        if (mode != 0 && mode != 1 && mode != 2) {
            mode = 1;
        }
    }

    return mode;
}

u32 SCGetCounterBias(void) {
    u32 bias;

    if (!SCFindU32Item(&bias, SC_ITEM_ID_IPL_COUNTER_BIAS)) {
        bias = 189388800;
    }

    return bias;
}

u8 SCGetWpadSensorBarPosition(void) {
    // NONMATCHING
}

u8 SCGetWpadMotorMode(void) {
    // NONMATCHING
}

u8 SCGetWpadSpeakerVolume(void) {
    // NONMATCHING
}

u8 SCGetBtDpdSensibility(void) {
    // NONMATCHING
}

BOOL SCSetWpadSpeakerVolume(u8 volume) {
    // NONMATCHING
}

BOOL SCSetWpadMotorMode(u8 mode) {
    // NONMATCHING
}
