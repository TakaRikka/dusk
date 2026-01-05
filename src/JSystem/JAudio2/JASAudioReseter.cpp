#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASAudioReseter.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "dolphin/types.h"

struct JASDSPChannel {
    void drop();
    static JASDSPChannel* getHandle(u32);

    inline u8 getStatus() const { return mStatus; }
    u32 mStatus;
};

JASAudioReseter::JASAudioReseter() {
    field_0x0 = 0;
    mDoneFlag = true;
    mThreadStopFlag = false;
    mDSPLevel = JASDriver::getDSPLevel();
}

JASAudioReseter::~JASAudioReseter() {
    JUT_ASSERT(28, mDoneFlag);
}

bool JASAudioReseter::start(u32 param_0, bool param_1) {
    if (mDoneFlag == false) {
        return false;
    }
    mThreadStopFlag = param_1;
    JASCriticalSection critical;
    if (!JASDriver::registerDspSyncCallback(callback, this)) {
        JUT_WARN(40, "%s", "Failed JASAudioReseter start\n");
        return false;
    } else {
        mDSPLevel = JASDriver::getDSPLevel();
        field_0x0 = param_0;
        mDoneFlag = false;
        return true;
    }
}

void JASAudioReseter::resume() {
    JUT_ASSERT(55, mDoneFlag);
    JUT_ASSERT(56, ! mThreadStopFlag);
    JASDriver::setDSPLevel(mDSPLevel);
}

s32 JASAudioReseter::checkDone() const {
    return mDoneFlag;
}


s32 JASAudioReseter::calc() {
    if(field_0x0==0) {
        for(size_t i = 0; i<64; i++) {
            JASDSPChannel* handle = JASDSPChannel::getHandle(i);
            if ((handle->getStatus())==0) {
                handle->drop();
            }
        }
        if(mThreadStopFlag!=false) {
            JASAudioThread* pAudioThread = JASGlobalInstance<JASAudioThread>::getInstance();
            JUT_ASSERT(78, pAudioThread);
            pAudioThread->stop();
        }
        mDoneFlag = 1;
        return -1;
    }
    field_0x0--;
    JASDriver::setDSPLevel(field_0x0 * JASDriver::getDSPLevel() / (field_0x0 + 1));
    return 0;
}

s32 JASAudioReseter::callback(void* param_0) {
    JASAudioReseter* i_this = (JASAudioReseter*) param_0;
    return i_this->calc();
}
