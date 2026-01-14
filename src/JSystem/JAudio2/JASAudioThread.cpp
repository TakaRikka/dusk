#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JASProbe.h"
#include "JSystem/JAudio2/JASReport.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "dolphin/dsp.h"
#include <stdint.h>

JASAudioThread::JASAudioThread(int stackSize, int msgCount, u32 threadPriority)
    : 
    JKRThread(JASDram, threadPriority, msgCount, stackSize),
    JASGlobalInstance<JASAudioThread>(true)
{
    sbPauseFlag = false;
    OSInitThreadQueue(&sThreadQueue);
}

void JASAudioThread::create(s32 threadPriority) {
#if PLATFORM_GCN
    const int size = 0x1400;
#else
    const int size = 0x2800;
#endif
    JASAudioThread* pAudioThread = new (JASDram, 0) JASAudioThread(threadPriority, 0x10, size);
    JUT_ASSERT(46, pAudioThread);
    JKRHeap* pCurrentHeap = JKRGetSystemHeap();
    JUT_ASSERT(48, pCurrentHeap);
    pAudioThread->setCurrentHeap(pCurrentHeap);
    pAudioThread->resume();
}

void JASAudioThread::stop() {
    jamMessageBlock((void*)2);
}

volatile int JASAudioThread::snIntCount;

class Lock {
public:
    Lock() {
        mInterrupts = OSDisableInterrupts();
    }
    ~Lock() {
        OSRestoreInterrupts(mInterrupts);
    }
private:
    BOOL mInterrupts;
};

class JASChannel {
    u8 filler[0x108];
};

// NONMATCHING location of JASPoolAllocObject_MultiThreaded<JASChannel>
void* JASAudioThread::run() {
    OSInitFastCast();
    JASDriver::initAI(DMACallback);
    JASDsp::boot(DSPCallback);
    JASDsp::initBuffer();
    JASDSPChannel::initAll();

    JASPoolAllocObject_MultiThreaded<JASChannel>::newMemPool(0x48);
    JASDriver::startDMA();

    OSMessage msg;
    while (true) {
        msg = waitMessageBlock();
        switch ((intptr_t)msg) {
        case AUDIOMSG_DMA:
            if (sbPauseFlag) {
                JUT_PANIC(107, "AUDIO THREAD PAUSED\n");
                JASDriver::stopDMA();
                OSSleepThread(&sThreadQueue);
            }
#if DEBUG
            if (snIntCount != 0) {
                JASReport("DSP process is over.");
            }
#endif
            JASDriver::updateDac();
            JASDriver::updateDacCallback();
            continue;

        case AUDIOMSG_DSP:
            JUT_ASSERT(125, snIntCount != 0);
            snIntCount--;
            if (snIntCount == 0) {
                JASProbe::stop(7);
                JASDriver::finishDSPFrame();
            } else {
                JASProbe::start(2, "SFR_DSP");
                JASDriver::updateDSP();
                JASProbe::stop(2);
            }
            continue;

        case AUDIOMSG_STOP:
            JASDriver::stopDMA();
            OSExitThread(NULL);
            continue;
        default:
            JUT_PANIC(152, "AUDIO THREAD INVALID MESSAGE\n");
        }
    }
}

void JASAudioThread::DMACallback() {
    JASAudioThread* pAudioThread = getInstance();
    JUT_ASSERT(167, pAudioThread);
	JASProbe::stop(4);
	JASProbe::start(4, "UPDATE-DAC");
	if (!pAudioThread->sendMessage((void*)AUDIOMSG_DMA)) {
        JUT_WARN_DEVICE(173, 1, "%s","----- DMACallback : Can\'t send DAC_SYNC message\n");
    }
}

void JASAudioThread::DSPCallback(void*) {
    JASAudioThread* pAudioThread = getInstance();
    JUT_ASSERT(184, pAudioThread);
	while (DSPCheckMailFromDSP() == 0) { }

	u32 mail = DSPReadMailFromDSP();
	if (mail >> 0x10 != 0xF355) {
        JUT_WARN(196, "DSP Mail format error %x\n", mail);
    } else {
		if ((mail & 0xFF00) == 0xFF00) {
            if (!pAudioThread->sendMessage((void*)AUDIOMSG_DSP)) {
                JUT_WARN_DEVICE(204, 1, "%s", "----- syncDSP : Send Miss\n");
            }
		} else {
			JASDsp::finishWork(mail);
		}
	}
}
