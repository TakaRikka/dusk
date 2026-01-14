#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JAudio2/JASCmdStack.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JASProbe.h"
#include "JSystem/JAudio2/JASReport.h"
#include "JSystem/JAudio2/JASLfo.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/ai.h"
#include <dolphin/os.h>
#include <stdint.h>

s16* JASDriver::sDmaDacBuffer[3];

static u8 data_804507A8 = 3;

s16** JASDriver::sDspDacBuffer;

s32 JASDriver::sDspDacWriteBuffer;

s32 JASDriver::sDspDacReadBuffer;

s32 JASDriver::sDspStatus;

JASDriver::DSPBufCallback JASDriver::sDspDacCallback;

s16* JASDriver::lastRspMadep;

void (*JASDriver::dacCallbackFunc)(s16*, u32);

JASDriver::MixCallback JASDriver::extMixCallback;

u32 JASDriver::sOutputRate;

JASMixMode JASDriver::sMixMode = MIX_MODE_EXTRA;

f32 JASDriver::sDacRate = 32028.5f;

u32 JASDriver::sSubFrames = 0x00000007;

void JASDriver::initAI(void (*param_0)(void)) {
    setOutputRate(OUTPUT_RATE_0);
    u32 dacSize = getDacSize();
    const u32 size = dacSize * 2;
    for (int i = 0; i < 3; i++) {
        sDmaDacBuffer[i] = new(JASDram, 0x20) s16[dacSize];
        JUT_ASSERT(102, sDmaDacBuffer[i])
        JASCalc::bzero(sDmaDacBuffer[i], size);
        DCStoreRange(sDmaDacBuffer[i], size);
    }
    sDspDacBuffer = new(JASDram, 0) s16*[data_804507A8];
    JUT_ASSERT(113, sDspDacBuffer);
    for (int i = 0; i < data_804507A8; i++) {
        sDspDacBuffer[i] = new(JASDram, 0x20) s16[getDacSize()];
        JUT_ASSERT(119, sDspDacBuffer[i]);
        JASCalc::bzero(sDspDacBuffer[i], size);
        DCStoreRange(sDspDacBuffer[i], size);
    }
    sDspDacWriteBuffer = data_804507A8 - 1;
    sDspDacReadBuffer = 0;
    sDspStatus = 0;
    JASChannel::initBankDisposeMsgQueue();
    AIInit(NULL);
    AIInitDMA((uintptr_t)sDmaDacBuffer[2], size);
    BOOL isOutputRate;
    if (sOutputRate == 0) {
        isOutputRate = FALSE;
    } else {
        isOutputRate = TRUE;
    }
    AISetDSPSampleRate((u8)isOutputRate);
    AIRegisterDMACallback(param_0);
}

void JASDriver::startDMA() {
    AIStartDMA();
}


void JASDriver::stopDMA() {
    AIStopDMA();
}

void JASDriver::setOutputRate(JASOutputRate param_0) {
    sOutputRate = param_0;
    if (param_0 == 0) {
        sSubFrames = 7;
        sDacRate = 32000.0f;
    } else {
        sSubFrames = 10;
        sDacRate = 48000.0f;
    }
    sDacRate *= 1.0008897f;
}

const JASDriver::MixFunc JASDriver::sMixFuncs[4] = {
    mixMonoTrack,
    mixMonoTrackWide,
    mixExtraTrack,
    mixInterleaveTrack,
};

u32 JASDriver::sSubFrameCounter;

void JASDriver::updateDac() {
    static u32 dacp = 0;
    s16* r30 = lastRspMadep;
    lastRspMadep = NULL;
    if (r30) {
        AIInitDMA((uintptr_t)r30, getDacSize() * 2);
    }
    s32 frameSamples = getFrameSamples();
    readDspBuffer(sDmaDacBuffer[dacp], frameSamples);
    if (sDspStatus == 0) {
        finishDSPFrame();
    }
    if (extMixCallback) {
        sMixFuncs[sMixMode](sDmaDacBuffer[dacp], frameSamples, extMixCallback);
    }
    {
        JASCriticalSection cs;
        DCStoreRange(sDmaDacBuffer[dacp], getDacSize() * 2);
    }
    lastRspMadep = sDmaDacBuffer[dacp];
    dacp++;
    if (dacp >= 3) {
        dacp = 0;
    }
    if (dacCallbackFunc) {
        dacCallbackFunc(lastRspMadep, getFrameSamples());
    }
}

void JASDriver::updateDSP() {
    JASProbe::start(3, "SFR-UPDATE");
    JASDsp::invalChannelAll();

    #if DEBUG
    JASDsp::dspMutex = 1;
    #endif

    JASPortCmd::execAllCommand();
    DSPSyncCallback();
    static u32 old_time = 0;
    static u32 history[10] = {0x000F4240};
    u32 r28 = OSGetTick();
    u32 r27 = r28 - old_time;
    old_time = r28;
    u32 subFrame = getSubFrames();
    int r26 = JASAudioThread::getDSPSyncCount();
    JUT_ASSERT(254, subFrame <= 10);
    history[subFrame - r26] = r27;
    if (subFrame != r26 && f32(history[0]) / r27 < 1.1f) {
        #if DEBUG
        static int killCounter;
        JASReport("kill DSP channel", killCounter);
        JASDSPChannel::killActiveChannel();
        killCounter++;
        #else
        JASReport("kill DSP channel");
        JASDSPChannel::killActiveChannel();
        #endif
    }
    JASChannel::receiveBankDisposeMsg();
    JASDSPChannel::updateAll();

    #if DEBUG
    JASDsp::dspMutex = 0;
    #endif

    subframeCallback();
    f32 freeRun = 32028.5f / getDacRate();
    JASLfo::updateFreeRun(freeRun);
    JASProbe::stop(3);
    sSubFrameCounter++;
}

void JASDriver::readDspBuffer(s16* param_0, u32 param_1) {
    s32 nbuf = sDspDacReadBuffer + 1;
    if (nbuf == data_804507A8) {
        nbuf = 0;
    }
    if (nbuf == sDspDacWriteBuffer && data_804507A8 >= 3) {
        s16 r25 = (s16)sDspDacBuffer[sDspDacReadBuffer][param_1 / 2 - 1];
        s16 r24 = (s16)sDspDacBuffer[sDspDacReadBuffer][param_1 - 1];
        for (int i = 0; i < param_1; i++) {
            sDspDacBuffer[sDspDacReadBuffer][i] = (s16)r25;
        }
        for (int i = param_1; i < param_1 * 2; i++) {
            sDspDacBuffer[sDspDacReadBuffer][i] = (s16)r24;
        }
#if DEBUG
        JASReport("readDspBuffer nbuf:%d sWBuf:%d BCount:%d stat:%d", nbuf, sDspDacWriteBuffer,
                  data_804507A8, sDspStatus);
#endif
    } else {
        sDspDacReadBuffer = nbuf;
        DCInvalidateRange(sDspDacBuffer[sDspDacReadBuffer], param_1 * 2 * 2);
    }
    s16* dacBuffer = sDspDacBuffer[sDspDacReadBuffer];
    s16* endDacBuffer = dacBuffer + param_1;
    JASCalc::imixcopy(endDacBuffer, dacBuffer, param_0, param_1);
}

u32 sDspUpCount;

void JASDriver::finishDSPFrame() {
    static u32 waitcount;
    int r30 = sDspDacWriteBuffer + 1;
    if (r30 == data_804507A8) {
        r30 = 0;
    }
    if (r30 == sDspDacReadBuffer) {
        sDspStatus = 0;
#if DEBUG
        if (sDspStatus != 0) {
            waitcount = 0;
        } else {
            waitcount++;
        }
        if (waitcount != 7) {
            return;
        }
        if (sDspUpCount != 0) {
            JUT_WARN(0, "forth DSP return\n");
        }
        if (sDspUpCount == 0) {
            sDspUpCount += 1;
        }
        JASReport("finishDSPFrame nbuf:%d sWBuf:%d BCount:%d stat:%d", r30, sDspDacWriteBuffer, data_804507A8, sDspStatus);
#else
        return;
#endif
    }

    sDspDacWriteBuffer = r30;
    JASAudioThread::setDSPSyncCount(getSubFrames());
    JASProbe::start(7, "DSP-MAIN");
    u32 r27 = getFrameSamples();
    JASDsp::syncFrame(getSubFrames(), u32(sDspDacBuffer[sDspDacWriteBuffer]), u32(sDspDacBuffer[sDspDacWriteBuffer] + r27));
    sDspStatus = 1;
    updateDSP();
    if (sDspDacCallback) {
        sDspDacCallback(sDspDacBuffer[sDspDacWriteBuffer], r27);
    }
}

void JASDriver::registerMixCallback(MixCallback param_0, JASMixMode param_1) {
    extMixCallback = param_0;
    sMixMode = param_1;
}

void JASDriver::registDSPBufCallback(DSPBufCallback cb) {
    sDspDacCallback = cb;
}

f32 JASDriver::getDacRate() {
    return sDacRate;
}

u32 JASDriver::getSubFrames() {
    return sSubFrames;
}

u32 JASDriver::getDacSize() {
    return sSubFrames * 0x50 * 2;
}

u32 JASDriver::getFrameSamples() {
    return sSubFrames * 0x50;
}

void JASDriver::mixMonoTrack(s16* buffer, u32 param_1, MixCallback param_2) {
    JASProbe::start(5, "MONO-MIX");
    s16* r26 = param_2(param_1);
    if (r26 == NULL) {
        return;
    }
    JASProbe::stop(5);
    s16* pTrack = buffer;
    s16* r28 = r26;
    for (u32 i = param_1; i != 0; i--) {
        s32 src = pTrack[0];
        src += r28[0];
        pTrack[0] = JASCalc::clamp<s16, s32>(src);
        pTrack++;
        src = pTrack[0];
        src += r28[0];
        pTrack[0] = JASCalc::clamp<s16, s32>(src);
        pTrack++;
        r28++;
    }
}

void JASDriver::mixMonoTrackWide(s16* buffer, u32 param_1, MixCallback param_2) {
    JASProbe::start(5, "MONO(W)-MIX");
    s16* r26 = param_2(param_1);
    if (!r26) {
        return;
    }
    JASProbe::stop(5);
    s16* pTrack = buffer;
    s16* r28 = r26;
    for (u32 i = param_1; i != 0; i--) {
        s32 src = pTrack[0];
        src += r28[0];
        pTrack[0] = JASCalc::clamp<s16, s32>(src);
        pTrack++;
        src = pTrack[0];
        src -= r28[0];
        pTrack[0] = JASCalc::clamp<s16, s32>(src);
        pTrack++;
        r28++;
    }
}

void JASDriver::mixExtraTrack(s16* buffer, u32 param_1, MixCallback param_2) {
    JASProbe::start(5, "DSPMIX");
    s16* r27 = param_2(param_1);
    if (!r27) {
        return;
    }
    JASProbe::stop(5);
    JASProbe::start(6, "MIXING");
    s16* pTrack = buffer;
    s16* r29 = r27;
    s16* r28 = r27 + getFrameSamples();
    for (u32 i = param_1; i != 0; i--) {
        s32 r25 = pTrack[0] + r28[0];
        pTrack[0] = JASCalc::clamp<s16, s32>(r25);
        pTrack++;
        r25 = pTrack[0] + r29[0];
        pTrack[0] = JASCalc::clamp<s16, s32>(r25);
        pTrack++;
        r28++;
        r29++;
    }
    JASProbe::stop(6);
}

void JASDriver::mixInterleaveTrack(s16* buffer, u32 param_1, MixCallback param_2) {
    s16* r31 = param_2(param_1);
    if (!r31) {
        return;
    }
    s16* pTrack = buffer;
    s16* r30 = r31;
    for (u32 i = param_1 * 2; i != 0; i--) {
        s32 r26 = pTrack[0] + r30[0];
        pTrack[0] = JASCalc::clamp<s16, s32>(r26);
        pTrack++;
        r30++;
    }
}

u32 JASDriver::getSubFrameCounter() {
    return sSubFrameCounter;
}
