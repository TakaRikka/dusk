#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include <dolphin/os.h>

void JASDriver::setDSPLevel(f32 param_0) {
    JASDsp::setDSPMixerLevel(param_0);
}

u16 JASDriver::MAX_MIXERLEVEL = 0x2EE0;

u16 JASDriver::getChannelLevel_dsp() {
    return JASDriver::MAX_MIXERLEVEL;
}

f32 JASDriver::getDSPLevel() {
    return JASDsp::getDSPMixerLevel();
}

u32 JASDriver::JAS_SYSTEM_OUTPUT_MODE = 0x00000001;

void JASDriver::setOutputMode(u32 param_0) {
    JAS_SYSTEM_OUTPUT_MODE = param_0;
}

u32 JASDriver::getOutputMode() {
    return JASDriver::JAS_SYSTEM_OUTPUT_MODE;
}

void JASDriver::waitSubFrame() {
    u32 r31 = getSubFrameCounter();
    do {
        OSYieldThread();
    } while (r31 == getSubFrameCounter());
}

JASCallbackMgr JASDriver::sDspSyncCallback;

JASCallbackMgr JASDriver::sSubFrameCallback;

JASCallbackMgr JASDriver::sUpdateDacCallback;

int JASDriver::rejectCallback(DriverCallback callback, void* param_1) {
    int r31 = sDspSyncCallback.reject(callback, param_1);
    r31 += sSubFrameCallback.reject(callback, param_1);
    r31 += sUpdateDacCallback.reject(callback, param_1);
    return r31;
}

bool JASDriver::registerDspSyncCallback(DriverCallback callback, void* param_1) {
    return sDspSyncCallback.regist(callback, param_1);
}

bool JASDriver::registerSubFrameCallback(DriverCallback callback, void* param_1) {
    return sSubFrameCallback.regist(callback, param_1);
}

void JASDriver::subframeCallback() {
    sSubFrameCallback.callback();
}

void JASDriver::DSPSyncCallback() {
    sDspSyncCallback.callback();
}

void JASDriver::updateDacCallback() {
    sUpdateDacCallback.callback();
}
