#include "Z2AudioCS/SpkSpeakerCtrl.h"

#include "Z2AudioCS/SpkMixingBuffer.h"
#include "Z2AudioCS/SpkSound.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include <revolution/os.h>
#include <revolution/wpad.h>
#include <cstring>

OSAlarm sSpeakerAlarm;
SpeakerInfo sSpeakerInfo[WPAD_MAX_CONTROLLERS];
SpkSoundHandle sAdjustSoundHandle[WPAD_MAX_CONTROLLERS];
SpkMixingBuffer* sMixingBuffer;

#if VERSION == VERSION_WII_USA_R0
#define SPKSPEAKERCTRL_SET_STATE(s) (void)0
#else
#define SPKSPEAKERCTRL_SET_STATE(s) mState = s
#endif

void SpkSpeakerCtrl::setMixingBuffer(SpkMixingBuffer* pMixingBuffer) {
    JASCriticalSection cs;

    sMixingBuffer = pMixingBuffer;
}

void SpkSpeakerCtrl::setup(void) {
    for (s32 i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        sSpeakerInfo[i].mIsConnected = false;
        sSpeakerInfo[i].mIsPlaying = false;
        sSpeakerInfo[i].field_0x22 = true;
        sSpeakerInfo[i].mIsMuted = false;
#if VERSION != VERSION_WII_USA_R0
        sSpeakerInfo[i].mState = 0;
#endif
        sSpeakerInfo[i].mVolume = 64;
        sSpeakerInfo[i].mRadioSensitivityTimer = 0;
        sSpeakerInfo[i].mExtensionTimer = 0;
        memset(sSpeakerInfo[i].mEncInfo.data, 0, sizeof(sSpeakerInfo[i].mEncInfo));
    }

    OSCreateAlarm(&sSpeakerAlarm);
    OSSetPeriodicAlarm(&sSpeakerAlarm, OSGetTime(), OSNanosecondsToTicks(6666667), updateSpeaker);
}

void SpkSpeakerCtrl::connect(s32 chan) {
    JASCriticalSection cs;

    sSpeakerInfo[chan].mIsConnected = true;
    sSpeakerInfo[chan].mIsPlaying = false;
#if VERSION != VERSION_WII_USA_R0
    sSpeakerInfo[chan].mState = 0;
#endif
    setSpeakerOn(chan);
}

void SpkSpeakerCtrl::disconnect(s32 chan) {
    JASCriticalSection cs;

    sSpeakerInfo[chan].mIsConnected = false;
    sSpeakerInfo[chan].mIsPlaying = false;
#if VERSION != VERSION_WII_USA_R0
    sSpeakerInfo[chan].mState = 0;
#endif
    setSpeakerOff(chan);
}

void SpkSpeakerCtrl::setSpeakerOn(s32 chan) {
#if VERSION != VERSION_WII_USA_R0
    JASCriticalSection cs;
#endif

    s32 val = WPADControlSpeaker(chan, 1, setSpeakerOnCallback);

#if VERSION != VERSION_WII_USA_R0
    if (val == -2) {
        sSpeakerInfo[chan].mState = 1;
    } else {
        sSpeakerInfo[chan].mState = 0;
    }
#endif
}

void SpkSpeakerCtrl::setSpeakerOnCallback(s32 chan, s32 param_1) {
#if VERSION == VERSION_WII_USA_R0
    if (param_1 == 0) {
        setSpeakerPlay(chan);
    }
#else
    JASCriticalSection cs;

    if (param_1 == 0) {
        sSpeakerInfo[chan].mState = 0;
        setSpeakerPlay(chan);
    } else {
        if (param_1 == -3) {
            sSpeakerInfo[chan].mState = 1;
        }
    }
#endif
}

void SpkSpeakerCtrl::setSpeakerPlay(s32 chan) {
#if VERSION != VERSION_WII_USA_R0
    JASCriticalSection cs;
#endif

    s32 val = WPADControlSpeaker(chan, 4, startPlayCallback);

#if VERSION != VERSION_WII_USA_R0
    if (val == -2) {
        sSpeakerInfo[chan].mState = 2;
    } else {
        sSpeakerInfo[chan].mState = 0;
    }
#endif
}

void SpkSpeakerCtrl::startPlayCallback(s32 chan, s32 param_1) {
#if VERSION != VERSION_WII_USA_R0
    JASCriticalSection cs;
#endif

    if (param_1 == 0) {
        sSpeakerInfo[chan].mIsPlaying = true;
        sSpeakerInfo[chan].field_0x22 = true;
#if VERSION != VERSION_WII_USA_R0
        sSpeakerInfo[chan].mState = 0;
#endif
        memset(&sSpeakerInfo[chan].mEncInfo, 0, 0x20);
    } else if (param_1 == -3) {
#if VERSION != VERSION_WII_USA_R0
        sSpeakerInfo[chan].mState = 2;
#endif
    }
}

void SpkSpeakerCtrl::setSpeakerOff(s32 chan) {
    sSpeakerInfo[chan].mIsPlaying = false;
#if VERSION != VERSION_WII_USA_R0
    sSpeakerInfo[chan].mState = 0;
#endif
    WPADControlSpeaker(chan, 0, 0);
}

#if VERSION != VERSION_WII_USA_R0
void SpkSpeakerCtrl::retryConnection(s32 chan) {
    switch (sSpeakerInfo[chan].mState) {
    case 0:
    case 3:
        break;
    case 1:
        setSpeakerOn(chan);
        break;
    case 2:
        setSpeakerPlay(chan);
        break;
    }
}
#endif

void SpkSpeakerCtrl::framework() {
    for (s32 i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        retryConnection(i);
    }
}

void SpkSpeakerCtrl::updateSpeaker(OSAlarm*, OSContext*) {
    for (s32 i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (sMixingBuffer != NULL) {
            BOOL isBusy = FALSE;

            if (!checkRadioSensitivity(i)) {
                isBusy = TRUE;
            }

            if (!updateExtensionProcess(i)) {
                isBusy = TRUE;
            }

            if (sSpeakerInfo[i].mIsMuted) {
                continue;
            }

            bool updateResult = sMixingBuffer->update(i);
            if (isBusy) {
                continue;
            }

            if (!sSpeakerInfo[i].mIsPlaying) {
                continue;
            }

            if (updateResult) {
                bool var_r28 = true;

                if (sSpeakerInfo[i].mIsPlaying && sMixingBuffer != NULL) {
                    if (sSpeakerInfo[i].field_0x22) {
                        var_r28 = false;
                        sSpeakerInfo[i].field_0x22 = false;
                    }
                    u8 data[24];
                    WENCGetEncodeData(&sSpeakerInfo[i].mEncInfo, var_r28,
                                      sMixingBuffer->getSamples(i), 0x28, data);
                    WPADSendStreamData(i, data, 0x14);
                }
            } else {
                sSpeakerInfo[i].field_0x22 = true;
            }
        }
    }
}

bool SpkSpeakerCtrl::isEnable(s32 chan) {
    return WPADIsSpeakerEnabled(chan) &&
           sSpeakerInfo[chan].mIsPlaying &&
           !isSubmitPlayByRadioSensitivity(chan) &&
           !isSubmitPlayByExtensionConnect(chan) &&
           sMixingBuffer != NULL;
}

void SpkSpeakerCtrl::extensionProcess(s32 chan, s32 param_1) {
    if (!sSpeakerInfo[chan].mIsConnected) {
        return;
    }

    sSpeakerInfo[chan].mExtensionTimer = 49;
    sSpeakerInfo[chan].field_0x22 = true;
    if (param_1 == 0xFF) {
        sSpeakerInfo[chan].mExtensionTimer = 50;
    }

    if (sSpeakerInfo[chan].mIsPlaying == 0) {
        return;
    }

    setSpeakerOff(chan);
}

bool SpkSpeakerCtrl::updateExtensionProcess(s32 chan) {
    if (sSpeakerInfo[chan].mIsConnected == 0) {
        return false;
    }

    if (sSpeakerInfo[chan].mExtensionTimer < 50) {
        if (sSpeakerInfo[chan].mExtensionTimer > 0) {
            if (sSpeakerInfo[chan].mExtensionTimer == 1 && !sSpeakerInfo[chan].mIsPlaying) {
                setSpeakerOn(chan);
            }

            sSpeakerInfo[chan].mExtensionTimer--;
        } else {
            sSpeakerInfo[chan].mExtensionTimer = 0;
        }
    }

    bool retval;
    if (sSpeakerInfo[chan].mExtensionTimer == 0) {
        retval = true;
    } else {
        retval = false;
    }
    return retval;
}

bool SpkSpeakerCtrl::isSubmitPlayByExtensionConnect(s32 chan) {
    if (sSpeakerInfo[chan].mExtensionTimer > 0) {
        return true;
    } else {
        return false;
    }
}

bool SpkSpeakerCtrl::checkRadioSensitivity(s32 chan) {
    if (sSpeakerInfo[chan].mIsConnected == 0) {
        return false;
    }

    if (WPADGetRadioSensitivity(chan) < 60) {
        if (sSpeakerInfo[chan].mIsPlaying == 1) {
            setSpeakerOff(chan);
        }

        sSpeakerInfo[chan].mRadioSensitivityTimer = 200;
        sSpeakerInfo[chan].field_0x22 = true;
    } else {
        if (sSpeakerInfo[chan].mRadioSensitivityTimer > 0) {
            if (sSpeakerInfo[chan].mRadioSensitivityTimer == 1) {
                if (!sSpeakerInfo[chan].mIsPlaying) {
                    setSpeakerOn(chan);
                }
            }

            sSpeakerInfo[chan].mRadioSensitivityTimer--;
        } else {
            sSpeakerInfo[chan].mRadioSensitivityTimer = 0;
        }
    }

    bool retval;
    if (sSpeakerInfo[chan].mRadioSensitivityTimer == 0) {
        retval = true;
    } else {
        retval = false;
    }
    return retval;
}

bool SpkSpeakerCtrl::isSubmitPlayByRadioSensitivity(s32 chan) {
    if (sSpeakerInfo[chan].mRadioSensitivityTimer > 0) {
        return true;
    } else {
        return false;
    }
}
