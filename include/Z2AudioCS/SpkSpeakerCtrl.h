#ifndef Z2AUDIOCS_SPKSPEAKERCTRL_H
#define Z2AUDIOCS_SPKSPEAKERCTRL_H

#include "Z2AudioCS/SpkMixingBuffer.h"
#include <revolution/os.h>
#include <revolution/types.h>
#include <revolution/wenc.h>

struct SpeakerInfo {
    /* 0x00 */ bool mIsConnected;
    /* 0x01 */ bool mIsPlaying;
    /* 0x02 */ WENCInfo mEncInfo;
    /* 0x22 */ bool field_0x22;
    /* 0x23 */ bool mIsMuted;
#if VERSION != VERSION_WII_USA_R0
    /* 0x24 */ s32 mState;
#endif
    /* 0x28 */ s32 mRadioSensitivityTimer;
    /* 0x2C */ s32 mExtensionTimer;
    /* 0x30 */ u8 mVolume;
};

class SpkSpeakerCtrl {
public:
    static void setMixingBuffer(SpkMixingBuffer*);
    static void setup(void);
    static void connect(s32 chan);
    static void disconnect(s32 chan);
    static void setSpeakerOn(s32 chan);
    static void setSpeakerOnCallback(s32 chan, s32 param_1);
    static void setSpeakerPlay(s32 chan);
    static void startPlayCallback(s32 chan, s32 param_1);
    static void setSpeakerOff(s32 chan);
    static void retryConnection(s32 chan);
    static void framework(void);
    static void updateSpeaker(OSAlarm* alarm, OSContext* ctx);
    static bool isEnable(s32 chan);
    static void extensionProcess(s32 chan, s32 param_1);
    static bool updateExtensionProcess(s32 chan);
    static bool isSubmitPlayByExtensionConnect(s32 chan);
    static bool checkRadioSensitivity(s32 chan);
    static bool isSubmitPlayByRadioSensitivity(s32 chan);

private:
};

#endif /* Z2AUDIOCS_SPKSPEAKERCTRL_H */
