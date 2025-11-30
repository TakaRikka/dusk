#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JASProbe.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "dolphin/dsp.h"

JASAudioThread::JASAudioThread(int stackSize, int msgCount, u32 threadPriority)
    : 
    JKRThread(JASDram, threadPriority, msgCount, stackSize),
    JASGlobalInstance<JASAudioThread>(true)
{
    sbPauseFlag = false;
    OSInitThreadQueue(&sThreadQueue);
}

void JASAudioThread::create(long threadPriority) {
	JASAudioThread* sAudioThread = new (JASDram, 0) JASAudioThread(threadPriority, 0x10, 0x1400);
    sAudioThread->setCurrentHeap(JKRGetSystemHeap());
	sAudioThread->resume();
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

    while (true) {
        OSMessage msg = waitMessageBlock();
        switch ((int)msg) {
        case AUDIOMSG_DMA:
            if (sbPauseFlag) {
                JASDriver::stopDMA();
                OSSleepThread(&sThreadQueue);
            }
            JASDriver::updateDac();
            JASDriver::updateDacCallback();
            break;

        case AUDIOMSG_DSP:
            snIntCount--;
            if (snIntCount == 0) {
                JASProbe::stop(7);
                JASDriver::finishDSPFrame();
            } else {
                JASProbe::start(2, "SFR_DSP");
                JASDriver::updateDSP();
                JASProbe::stop(2);
            }
            break;

        case AUDIOMSG_STOP:
            JASDriver::stopDMA();
            OSExitThread(NULL);
            break;
        }
    }
}

void JASAudioThread::DMACallback() {
    JASAudioThread* thread = getInstance();
	JASProbe::stop(4);
	JASProbe::start(4, "UPDATE-DAC");
	thread->sendMessage((void*)AUDIOMSG_DMA);
}

void JASAudioThread::DSPCallback(void*) {
    JASAudioThread* thread = getInstance();
	while (DSPCheckMailFromDSP() == 0) { }

	u32 mail = DSPReadMailFromDSP();
	if (mail >> 0x10 == 0xF355) {
		if ((mail & 0xFF00) == 0xFF00) {
            thread->sendMessage((void*)AUDIOMSG_DSP);
		} else {
			JASDsp::finishWork(mail);
		}
	}
}
