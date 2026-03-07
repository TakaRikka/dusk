#include "Z2AudioLib/Z2DebugSys.h"
#include "JSystem/JAHNodeLib/JAHSoundPlayerNode.h"
#include "JSystem/JAHostIO/JAHioMgr.h"
#include "JSystem/JAWExtSystem/JAWExtSystem.h"
#include "JSystem/JAWWinLib/JAWBankView.h"
#include "JSystem/JAWWinLib/JAWChView.h"
#include "JSystem/JAWWinLib/JAWHioBankEdit.h"
#include "JSystem/JAWWinLib/JAWHioReceiver.h"
#include "JSystem/JAWWinLib/JAWPlayerChView.h"
#include "JSystem/JAWWinLib/JAWReportView.h"
#include "JSystem/JAWWinLib/JAWSysMemView.h"
#include "JSystem/JAWWinLib/JAWVolume.h"
#include "JSystem/JAudio2/JASReport.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "Z2AudioLib/Z2F1TestWindow.h"
#include "Z2AudioLib/Z2FxLineMgr.h"
#include "Z2AudioLib/Z2SeView.h"
#include "Z2AudioLib/Z2SoundPlayer.h"
#include "Z2AudioLib/Z2TrackView.h"
#include "Z2AudioLib/Z2WaveArcLoader.h"
#include "m_Do/m_Do_hostIO.h"
#include "m_Do/m_Do_ext.h"

Z2DebugSys::Z2DebugSys() : JASGlobalInstance<Z2DebugSys>(true) { }

void Z2DebugSys::initJAW() {
    JKRHeap* oldHeap = JKRHeap::getCurrentHeap();
    JKRExpHeap* heap = mDoExt_getGameHeap();
    Z2SoundPlayer* soundPlayer;
    JAWSysMemView* sysMemView;
    JAWBankView* bankView;
    JAWChView* chView;
    Z2TrackView* trackView;
    JAWReportView* reportView;
    JAWHioBankEdit* hioBankEdit;
    Z2WaveArcLoader* waveArcLoader;
    JAWVolume* volume;
    Z2PlaySeView* playSeView;
    Z2AudSettingView* audSettingView;
    Z2DSPCheck* dspCheck;
    Z2EntrySeView* entrySeView;
    JAWPlayerChView* playerChView;
    JKRSetCurrentHeap(heap);
    u32 local_50 = heap->getFreeSize();
    soundPlayer = new (heap, 0) Z2SoundPlayer("GC-Zelda2");
    sysMemView = new (heap, 0) JAWSysMemView();
    bankView = new (heap, 0) JAWBankView();
    chView = new (heap, 0) JAWChView();
    trackView = new (heap, 0) Z2TrackView(0, Z2TrackView::DISPMODE0);
    reportView = new (heap, 0) JAWReportView();
    JASReportInit(heap, 500);
    hioBankEdit = new (heap, 0) JAWHioBankEdit();
    waveArcLoader = new (heap, 0) Z2WaveArcLoader();
    volume = new (heap, 0) JAWVolume(90, 480);
    playSeView = new (heap, 0) Z2PlaySeView();
    audSettingView = new (heap, 0) Z2AudSettingView();
    dspCheck = new (heap, 0) Z2DSPCheck();
    JAWExtSystem::registWindow(1, soundPlayer, 10, 22);
    entrySeView = new (heap, 0) Z2EntrySeView();
    JAWExtSystem::registWindow(2, entrySeView, 10, 22);
    JAWExtSystem::registWindow(3, playSeView, 10, 22);
    JAWExtSystem::registWindow(4, sysMemView, 10, 22);
    JAWExtSystem::registWindow(5, bankView, 10, 22);
    JAWExtSystem::registWindow(6, chView, 10, 22);
    JAWExtSystem::registWindow(7, trackView, 10, 22);
    playerChView = new (heap, 0) JAWPlayerChView();
    JAWExtSystem::registWindow(8, playerChView, 10, 22);
    JAWExtSystem::registWindow(11, reportView, 330, 0);
    JAWExtSystem::registWindow(12, hioBankEdit, 10, 0);
    JAWExtSystem::registWindow(13, volume, 0, 0);
    JAWExtSystem::registWindow(15, waveArcLoader, 10, 100);
    JAWExtSystem::registWindow(16, dspCheck, 10, 100);
    JAWExtSystem::registWindow(17, audSettingView, 10, 100);
    JKRSetCurrentHeap(oldHeap);
}

void Z2DebugSys::initSoundHioNode() {
    JKRHeap* oldHeap = JKRHeap::getCurrentHeap();
    JKRExpHeap* heap = mDoExt_getGameHeap();
    JKRSetCurrentHeap(heap);
    s32 freeSize = heap->getFreeSize();
    JAHSingletonBase<JAHioMgr>::newInstance();
    JAHioNode* soundRoot = new (heap, 0) JAHioNode("サウンド");
    JUT_ASSERT(239, soundRoot);
    JAHSingletonBase<JAHioMgr>::getIns()->init_OnGame();
    JAHSingletonBase<JAHioMgr>::getIns()->appendRootNode(&mDoHIO_root, soundRoot);
    mDoHIO_createChild("サウンド", soundRoot);
    JAHSoundPlayerNode* soundPlayerNode = new (heap, 0) JAHSoundPlayerNode();
    JUT_ASSERT(258, soundPlayerNode);
    soundRoot->appendNode(soundPlayerNode, NULL);
    Z2ParamNode* paramNode = new (heap, 0) Z2ParamNode();
    JUT_ASSERT(262, paramNode);
    soundRoot->appendNode(paramNode, NULL);
    Z2FxLineEditNode* fxNode = new (heap, 0) Z2FxLineEditNode(heap);
    JUT_ASSERT(267, fxNode);
    soundRoot->appendNode(fxNode, NULL);
    Z2F1SwingTestNode* f1TestWin = new (heap, 0) Z2F1SwingTestNode();
    JUT_ASSERT(284, f1TestWin);
    soundRoot->appendNode(f1TestWin, NULL);
    JAWExtSystem::registWindow(10, f1TestWin, 10, 22);
    JKRSetCurrentHeap(oldHeap);
}

void Z2DebugSys::debugframework() {
    JAHSingletonBase<JAHioMgr>::getIns()->framework();
}

JAISeqDataMgr* Z2DebugSys::initSeSeqDataMgr(const void* param_1) {
    Z2HioSeSeqDataMgr* seqDataMgr = NULL;
    JKRHeap* oldHeap = JKRHeap::getCurrentHeap();
    JKRExpHeap* heap = mDoExt_getGameHeap();
    JKRSetCurrentHeap(heap);
    s32 freeSize = heap->getFreeSize();
    JAWHioReceiver* receiver = new (heap, 0) JAWHioReceiver();
    JAWExtSystem::registWindow(0, receiver, 10, 22);
    seqDataMgr = new (heap, 0) Z2HioSeSeqDataMgr(param_1, receiver);
    JKRSetCurrentHeap(oldHeap);
    return seqDataMgr;
}
