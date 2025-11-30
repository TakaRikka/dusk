#ifndef JASDRIVERIF_H
#define JASDRIVERIF_H

#include "JSystem/JAudio2/JASCallback.h"

typedef s32 (*DriverCallback)(void*);

namespace JASDriver {
    void setDSPLevel(f32);
    u16 getChannelLevel_dsp();
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

    extern JASCallbackMgr sDspSyncCallback;
    extern JASCallbackMgr sSubFrameCallback;
    extern JASCallbackMgr sUpdateDacCallback;
    extern u16 MAX_MIXERLEVEL;
    extern u32 JAS_SYSTEM_OUTPUT_MODE;
};

inline void JAISetOutputMode(u32 mode) {
    JASDriver::setOutputMode(mode);
}

#endif /* JASDRIVERIF_H */
