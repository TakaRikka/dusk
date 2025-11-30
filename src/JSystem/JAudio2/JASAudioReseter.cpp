#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASAudioReseter.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "dolphin/types.h"

struct JASDSPChannel {
    void drop();
    static JASDSPChannel* getHandle(u32);

    inline u32 getStatus() {return mStatus;}
    u32 mStatus;
};

JASAudioReseter::JASAudioReseter() {
    field_0x0 = 0;
    mIsDone = true;
    field_0xc = false;
    mDSPLevel = JASDriver::getDSPLevel();
}

JASAudioReseter::~JASAudioReseter() {}

bool JASAudioReseter::start(u32 param_0, bool param_1) {
    if (mIsDone == false) {
        return false;
    }
    field_0xc = param_1;
    JASCriticalSection critical;
    if (!JASDriver::registerDspSyncCallback(callback, this)) {
        return false;
    } else {
        mDSPLevel = JASDriver::getDSPLevel();
        field_0x0 = param_0;
        mIsDone = false;
        return true;
    }
}

void JASAudioReseter::resume() {
    JASDriver::setDSPLevel(mDSPLevel);
}

s32 JASAudioReseter::checkDone() const {
    return mIsDone;
}


s32 JASAudioReseter::calc() {
    if(field_0x0==0) {
        for(size_t i = 0; i<64; i++) {
            JASDSPChannel* handle = JASDSPChannel::getHandle(i);
            if ((handle->getStatus()&0xFF)==0) {
                handle->drop();
            }
        }
        if(field_0xc!=false) {
            JASGlobalInstance<JASAudioThread>::getInstance()->stop();//JASGlobalInstance<JASAudioThread>::sInstance->stop();
        }
        mIsDone = 1;
        return -1;
    }
    field_0x0--;
    u32 unk = field_0x0;
    float dspLevel = JASDriver::getDSPLevel();
    JASDriver::setDSPLevel(((float)unk*dspLevel)/(float)(unk+1));
    return 0;
}

s32 JASAudioReseter::callback(void* param_0) {
    return ((JASAudioReseter*)param_0)->calc();
}
