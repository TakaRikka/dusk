#include "Z2AudioLib/Z2AudioMgr.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASResArcLoader.h"
#include "JSystem/JAudio2/JASSeqParser.h"
#include "JSystem/JAudio2/JAUInitializer.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
#include "JSystem/JAudio2/JAUStreamAramMgr.h"
#include "JSystem/JAudio2/JAUSeqCollection.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "Z2AudioLib/Z2AudioArcLoader.h"
#include "Z2AudioLib/Z2Param.h"
#include "Z2AudioLib/Z2SoundHandles.h"

#if PLATFORM_WII || PLATFORM_SHIELD
#include "Z2AudioLib/Z2AudioCS.h"
#endif

Z2AudioMgr* Z2AudioMgr::mAudioMgrPtr;

Z2AudioMgr::Z2AudioMgr() : mSoundStarter(true) {
    mAudioMgrPtr = this;
    mResettingFlag = false;
    field_0x519 = false;
}

// NONMATCHING JASPoolAllocObject<_> locations
void Z2AudioMgr::init(JKRSolidHeap* heap, u32 memSize, void* baaData, JKRArchive* seqArc) {
    JAU_JASInitializer JASInitializer;
    JASInitializer.audioMemSize_ = memSize;
    JASInitializer.field_0x1c = 140;
    JASInitializer.dspLevel_ = 1.3f;
    JASInitializer.waveArcDir_ = "Audiores/Waves/";
    JASInitializer.initJASystem(heap);

    JAU_JAIInitializer JAIInitializer;
    JAIInitializer.field_0x0 = 78;
    JAIInitializer.field_0x4 = 4;
    JAIInitializer.field_0xc = 48;
    JAIInitializer.initJAInterface();
    
    JAISeMgr* seMgr = mSoundMgr.getSeMgr();

    JAISeCategoryArrangement categoryArrangement;
    categoryArrangement.mItems[0].mMaxActiveSe = 4;
    categoryArrangement.mItems[0].mMaxInactiveSe = 2;
    categoryArrangement.mItems[1].mMaxActiveSe = 2;
    categoryArrangement.mItems[1].mMaxInactiveSe = 1;
    categoryArrangement.mItems[2].mMaxActiveSe = 6;
    categoryArrangement.mItems[2].mMaxInactiveSe = 3;
    categoryArrangement.mItems[3].mMaxActiveSe = 16;
    categoryArrangement.mItems[3].mMaxInactiveSe = 8;
    categoryArrangement.mItems[4].mMaxActiveSe = 8;
    categoryArrangement.mItems[4].mMaxInactiveSe = 4;
    categoryArrangement.mItems[5].mMaxActiveSe = 6;
    categoryArrangement.mItems[5].mMaxInactiveSe = 3;
    categoryArrangement.mItems[6].mMaxActiveSe = 6;
    categoryArrangement.mItems[6].mMaxInactiveSe = 3;
    categoryArrangement.mItems[7].mMaxActiveSe = 12;
    categoryArrangement.mItems[7].mMaxInactiveSe = 8;
    categoryArrangement.mItems[8].mMaxActiveSe = 10;
    categoryArrangement.mItems[8].mMaxInactiveSe = 5;
    categoryArrangement.mItems[9].mMaxActiveSe = 8;
    categoryArrangement.mItems[9].mMaxInactiveSe = 4;
    seMgr->setCategoryArrangement(categoryArrangement);

    seMgr->getCategory(0)->getParams()->moveVolume(Z2Param::VOL_SE_SYSTEM_DEFAULT, 0);
    seMgr->getCategory(1)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_VOICE_DEFAULT, 0);
    seMgr->getCategory(2)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_MOTION_DEFAULT, 0);
    seMgr->getCategory(3)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_FOOTNOTE_DEFAULT, 0);
    seMgr->getCategory(4)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_DEFAULT, 0);
    seMgr->getCategory(5)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_DEFAULT, 0);
    seMgr->getCategory(6)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_MOVE_DEFAULT, 0);
    seMgr->getCategory(7)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_DEFAULT, 0);
    seMgr->getCategory(8)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_DEFAULT, 0);
    seMgr->getCategory(9)->getParams()->moveVolume(Z2Param::VOL_SE_ATMOSPHERE_DEFAULT, 0);

    seMgr->getParams()->moveVolume(1.0f, 0);

    JAISeqMgr* seqMgr = mSoundMgr.getSeqMgr();
    seqMgr->getParams()->moveVolume(Z2Param::VOL_BGM_DEFAULT, 0);

    JAIStreamMgr* streamMgr = mSoundMgr.getStreamMgr();
    JAUStreamStaticAramMgr_<1>* streamStaticAramMgr = new(heap, 0) JAUStreamStaticAramMgr_<1>();
    streamStaticAramMgr->reserveAram(NULL, 0, 0x14);
    streamMgr->setStreamAramMgr(streamStaticAramMgr);
    streamMgr->getParams()->moveVolume(Z2Param::VOL_BGM_DEFAULT, 0);

    JASPoolAllocObject<Z2Audible>::newMemPool(0x4e);
    mSoundMgr.getSeMgr()->setAudience(&mAudience);
    mSoundMgr.getSeqMgr()->setAudience(&mAudience);

    #if PLATFORM_WII || PLATFORM_SHIELD
    Z2AudioCS::newSpkSoundMemPool();
    #endif

    JASPoolAllocObject<Z2SoundHandlePool>::newMemPool(0x4e);
    OS_REPORT("[Z2AudioMgr::init]before Create Section: %d\n", heap->getFreeSize());

    JAUSectionHeap* sectionHeap = JAUNewSectionHeap(true);
    sectionHeap->setSeqDataArchive(seqArc);
    size_t resMaxSize = JASResArcLoader::getResMaxSize(seqArc);
    sectionHeap->newDynamicSeqBlock(0xE00);
    sectionHeap->newDynamicSeqBlock(0x17E0);
    sectionHeap->newDynamicSeqBlock(0x5380);
    sectionHeap->newDynamicSeqBlock(resMaxSize);

    Z2AudioArcLoader audioArcLoader(sectionHeap);
    bool baaLoadResult = audioArcLoader.load(baaData);
    JUT_ASSERT(252, baaLoadResult);
    
    #if DEBUG
    seMgr->setSeqDataMgr(mDebugSys.initSeSeqDataMgr(((JAUSeqDataMgr_SeqCollection*)sectionHeap->getSeSeqDataMgr())->getResource()));
    #else
    seMgr->setSeqDataMgr(sectionHeap->getSeSeqDataMgr());
    #endif
    seqMgr->setSeqDataMgr(sectionHeap->getSeqSeqDataMgr());
    
    if (sectionHeap->getStreamDataMgr()) {
        // The following is to force stack reg to be r31 in dbg:
        int unused;
        streamMgr->setStreamDataMgr(sectionHeap->getStreamDataMgr());
    } else {
        streamMgr->setStreamDataMgr(&mSoundInfo);
    }

    sectionHeap->finishBuild();
    initSe();
    JASSeqParser::registerSeqCallback(seqCallback);

    #if DEBUG
    mDebugSys.initSoundHioNode();
    mDebugSys.initJAW();
    #endif
}

void Z2AudioMgr::setOutputMode(u32 mode) {
    if (mode <= 2) {
        JAISetOutputMode(mode);
    }
}

void Z2AudioMgr::zeldaGFrameWork() {
    processTime();
    mSpeechMgr.framework();
    processSeFramework();
    processBgmFramework();
    processHeartGaugeSound();

    #if DEBUG
    mDebugSys.debugframework();
    #endif
}

void Z2AudioMgr::gframeProcess() {
    zeldaGFrameWork();

    if (mResettingFlag && mAudioReseter.checkDone()) {
        if (!field_0x519) {
            mSoundMgr.stopSync();
        }
    } else {
        mSoundMgr.framework();
        framework();
    }
}

void Z2AudioMgr::resetProcess(u32 param_0, bool param_1) {
    JUT_ASSERT(413, !mResettingFlag);

    mAudioReseter.start(param_0 * JASDriver::getSubFrames(), param_1);
    field_0x519 = param_1;
    mResettingFlag = true;
}

void Z2AudioMgr::resetRecover() {
    JUT_ASSERT(427, mResettingFlag);
    JUT_ASSERT(428, mAudioReseter.checkDone());

    mAudioReseter.resume();
    mSoundMgr.initParams();
    mResettingFlag = false;

    setTwilightGateVol(1.0f);
    setWindStoneVol(1.0f, 0);
    menuOut();
    bgmAllUnMute(0);
    unMuteSceneBgm(0);
}


bool Z2AudioMgr::hasReset() const {
    return mResettingFlag && mAudioReseter.checkDone() &&
        (field_0x519 || (mSoundMgr.getSeMgr()->getNumActiveSe() == 0 && mSoundMgr.getSeqMgr()->getNumActiveSeqs() == 0));
}

bool Z2AudioMgr::startSound(JAISoundID soundID, JAISoundHandle* handle,
                            const JGeometry::TVec3<f32>* posPtr) {
    if (mResettingFlag) {
        return 0;
    }

    return mSoundMgr.startSound(soundID, handle, posPtr);
}

bool Z2AudioMgr::startLevelSound(JAISoundID soundID, JAISoundHandle* handle,
                                 const JGeometry::TVec3<f32>* posPtr) {
    if (mResettingFlag) {
        return 0;
    }

    return mSoundMgr.startSound(soundID, handle, posPtr);
}
