#ifndef JASDRIVERIF_H
#define JASDRIVERIF_H

#include "JSystem/JAudio2/JASCallback.h"

#define JAS_OUTPUT_MONO     OS_SOUND_MODE_MONO
#define JAS_OUTPUT_STEREO   OS_SOUND_MODE_STEREO
#define JAS_OUTPUT_SURROUND 2

typedef s32 (*DriverCallback)(void*);

namespace JASDriver {
    void setDSPLevel(f32);
    u16 getChannelLevel_dsp();
    f32 getChannelLevel();
    f32 getDSPLevel();
    void setOutputMode(u32);
    u32 getOutputMode();
    void waitSubFrame();
    int rejectCallback(DriverCallback, void*);
    bool registerDspSyncCallback(DriverCallback, void*);
    bool registerSubFrameCallback(DriverCallback, void*);
    void subframeCallback();
    void DSPSyncCallback();
    void updateDacCallback();

    DUSK_GAME_EXTERN JASCallbackMgr sDspSyncCallback;
    DUSK_GAME_EXTERN JASCallbackMgr sSubFrameCallback;
    DUSK_GAME_EXTERN JASCallbackMgr sUpdateDacCallback;
    DUSK_GAME_EXTERN u16 MAX_MIXERLEVEL;
    DUSK_GAME_EXTERN u32 JAS_SYSTEM_OUTPUT_MODE;
};

inline void JAISetOutputMode(u32 mode) {
    JASDriver::setOutputMode(mode);
}

#endif /* JASDRIVERIF_H */
