#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUInitializer.h"
#include "JSystem/JAudio2/JAISe.h"
#include "JSystem/JAudio2/JAISeq.h"
#include "JSystem/JAudio2/JAISoundChild.h"
#include "JSystem/JAudio2/JAIStream.h"
#include "JSystem/JAudio2/JASAramStream.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JAudio2/JASTrack.h"
#include "JSystem/JAudio2/JASWaveArcLoader.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JKernel/JKRThread.h"

JAU_JASInitializer::JAU_JASInitializer() {
    audioMemory_ = 0;
    audioMemSize_ = 0;
    dvdThreadId_ = -1;
    audioThreadId_ = -1;
#if TARGET_PC
    heapSize_ = 0x4000;
#else
    heapSize_ = 0x1000;
#endif
    dvdThreadPriority_ = 3;
    audioThreadPriority_ = 2;
    mJasTrackPoolSize = 0x80;
    dspLevel_ = 1.0f;
    aramBlockSize_ = 0x2760;
    aramChannelNum_ = 2;
    field_0x20 = 0;
    waveArcDir_ = NULL;
}

// NONMATCHING JASPoolAllocObject_MultiThreaded<_> locations
void JAU_JASInitializer::initJASystem(JKRSolidHeap* heap) {
    if (JASAudioThread::getThreadPointer()) {
        JUT_WARN(48, "%s", "Though initialized JASAudioThread, called JAU_JASInitializer::initJASystem()\n");
    } else if (JASDvd::getThreadPointer()) {
        JUT_WARN(52, "%s", "Though initialized JASDvd, called JAU_JASInitializer::initJASystem()\n");
    } else {
        JASKernel::setupRootHeap(heap, heapSize_);

        if (audioMemory_ == 0) {
            audioMemory_ = JKRAram::getManager()->getAudioMemory();
        }

        if (audioMemSize_ == 0) {
            audioMemSize_ = JKRAram::getManager()->getAudioMemSize();
        }

        JASKernel::setupAramHeap(audioMemory_, audioMemSize_);

        JASTrack::newMemPool(mJasTrackPoolSize);
        if (field_0x20 > 0) {
            JASTrack::TChannelMgr::newMemPool(field_0x20);
        }

        JASDvd::createThread(dvdThreadPriority_, 0x80, 0x1000);
        JASAudioThread::create(audioThreadPriority_);
        JKRThreadSwitch* threadSwitch = JKRThreadSwitch::getManager();
        if (threadSwitch) {
            if (dvdThreadId_ >= 0) {
                threadSwitch->enter(JASDvd::getThreadPointer(), dvdThreadId_);
            }
            if (audioThreadId_ >= 0) {
                threadSwitch->enter(JASAudioThread::getThreadPointer(), audioThreadId_);
            }
        }

        JASDriver::setDSPLevel(dspLevel_);
        JASAramStream::initSystem(aramBlockSize_, aramChannelNum_);
    }

    if (waveArcDir_) {
        JASWaveArcLoader::setCurrentDir(waveArcDir_);
    }

    #if PLATFORM_SHIELD
    JASDriver::setOutputMode(JAS_OUTPUT_STEREO);
    #else
    switch (OSGetSoundMode()) {
    case OS_SOUND_MODE_MONO:
        JASDriver::setOutputMode(JAS_OUTPUT_MONO);
        break;
    case OS_SOUND_MODE_STEREO:
        JASDriver::setOutputMode(JAS_OUTPUT_STEREO);
        break;
    }
    #endif
}

JAU_JAIInitializer::JAU_JAIInitializer() {
    mJaiSePoolSize = 100;
    mJaiSeqPoolSize = 4;
    mJaiStreamPoolSize = 2;
    mJaiSoundChildPoolSize = 16;
}

// NONMATCHING JASPoolAllocObject<_> locations
void JAU_JAIInitializer::initJAInterface() {
    s32 r30 = JASDram->getFreeSize();
    JAIStream::newMemPool(mJaiStreamPoolSize);
    JAISeq::newMemPool(mJaiSeqPoolSize);
    JAISe::newMemPool(mJaiSePoolSize);
    JAISoundChild::newMemPool(mJaiSoundChildPoolSize);
    s32 r29 = JASDram->getFreeSize();
    OS_REPORT("JAU_JAIInitializer uses %d bytes\n", r30 - r29);
}
