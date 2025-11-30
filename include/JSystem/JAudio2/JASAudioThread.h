#ifndef JASAUDIOTHREAD_H
#define JASAUDIOTHREAD_H

#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JKernel/JKRThread.h"

enum JASAudioMessage {
	AUDIOMSG_DMA  = 0,
	AUDIOMSG_DSP  = 1,
	AUDIOMSG_STOP = 2,
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASAudioThread : public JKRThread, public JASGlobalInstance<JASAudioThread> {
    JASAudioThread(int, int, u32);
    static void create(s32);
    void stop();
    virtual void* run();
    static void DMACallback();
    static void DSPCallback(void*);
    virtual ~JASAudioThread() {}

    static JASAudioThread* getThreadPointer() { return getInstance(); }
    static int getDSPSyncCount() { return snIntCount; }
    static void setDSPSyncCount(u32 param_0) { snIntCount = param_0; }

	/* 0x7c */ OSThreadQueue sThreadQueue;
	/* 0x84 */ bool sbPauseFlag;        // type unsure
	
    static volatile int snIntCount; // type unsure
};

#endif /* JASAUDIOTHREAD_H */
