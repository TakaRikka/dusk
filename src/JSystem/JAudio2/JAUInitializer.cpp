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
    heapSize_ = 0x1000;
    dvdThreadPriority_ = 3;
    audioThreadPriority_ = 2;
    field_0x1c = 0x80;
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

        JASTrack::newMemPool(field_0x1c);
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
    JASDriver::setOutputMode(1);
    #else
    switch (OSGetSoundMode()) {
    case 0:
        JASDriver::setOutputMode(0);
        break;
    case 1:
        JASDriver::setOutputMode(1);
        break;
    }
    #endif
}

JAU_JAIInitializer::JAU_JAIInitializer() {
    field_0x0 = 100;
    field_0x4 = 4;
    field_0x8 = 2;
    field_0xc = 16;
}

// NONMATCHING JASPoolAllocObject<_> locations
void JAU_JAIInitializer::initJAInterface() {
    s32 r30 = JASDram->getFreeSize();
    JAIStream::newMemPool(field_0x8);
    JAISeq::newMemPool(field_0x4);
    JAISe::newMemPool(field_0x0);
    JAISoundChild::newMemPool(field_0xc);
    s32 r29 = JASDram->getFreeSize();
    OS_REPORT("JAU_JAIInitializer uses %d bytes\n", r30 - r29);
}
