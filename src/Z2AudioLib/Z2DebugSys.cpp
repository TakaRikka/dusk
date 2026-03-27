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
#include "Z2AudioLib/Z2Param.h"
#include "Z2AudioLib/Z2SeView.h"
#include "Z2AudioLib/Z2SoundPlayer.h"
#include "Z2AudioLib/Z2TrackView.h"
#include "Z2AudioLib/Z2WaveArcLoader.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_hostIO.h"

Z2DebugSys::Z2DebugSys() : JASGlobalInstance<Z2DebugSys>(true) {}

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
    soundPlayer = JKR_NEW_ARGS (heap, 0) Z2SoundPlayer("GC-Zelda2");
    sysMemView = JKR_NEW_ARGS (heap, 0) JAWSysMemView;
    bankView = JKR_NEW_ARGS (heap, 0) JAWBankView;
    chView = JKR_NEW_ARGS (heap, 0) JAWChView;
    trackView = JKR_NEW_ARGS (heap, 0) Z2TrackView(0, Z2TrackView::DISPMODE0);
    reportView = JKR_NEW_ARGS (heap, 0) JAWReportView;
    JASReportInit(heap, 500);
    hioBankEdit = JKR_NEW_ARGS (heap, 0) JAWHioBankEdit;
    waveArcLoader = JKR_NEW_ARGS (heap, 0) Z2WaveArcLoader;
    volume = JKR_NEW_ARGS (heap, 0) JAWVolume(90, 480);
    playSeView = JKR_NEW_ARGS (heap, 0) Z2PlaySeView;
    audSettingView = JKR_NEW_ARGS (heap, 0) Z2AudSettingView;
    dspCheck = JKR_NEW_ARGS (heap, 0) Z2DSPCheck;
    JAWExtSystem::registWindow(1, soundPlayer, 10, 22);
    entrySeView = JKR_NEW_ARGS (heap, 0) Z2EntrySeView;
    JAWExtSystem::registWindow(2, entrySeView, 10, 22);
    JAWExtSystem::registWindow(3, playSeView, 10, 22);
    JAWExtSystem::registWindow(4, sysMemView, 10, 22);
    JAWExtSystem::registWindow(5, bankView, 10, 22);
    JAWExtSystem::registWindow(6, chView, 10, 22);
    JAWExtSystem::registWindow(7, trackView, 10, 22);
    playerChView = JKR_NEW_ARGS (heap, 0) JAWPlayerChView;
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
    JAHioNode* soundRoot = JKR_NEW_ARGS (heap, 0) JAHioNode("サウンド");
    JUT_ASSERT(239, soundRoot);
    JAHSingletonBase<JAHioMgr>::getIns()->init_OnGame();
    JAHSingletonBase<JAHioMgr>::getIns()->appendRootNode(&mDoHIO_root, soundRoot);
    mDoHIO_createChild("サウンド", soundRoot);
    JAHSoundPlayerNode* soundPlayerNode = JKR_NEW_ARGS (heap, 0) JAHSoundPlayerNode;
    JUT_ASSERT(258, soundPlayerNode);
    soundRoot->appendNode(soundPlayerNode, NULL);
    Z2ParamNode* paramNode = JKR_NEW_ARGS (heap, 0) Z2ParamNode;
    JUT_ASSERT(262, paramNode);
    soundRoot->appendNode(paramNode, NULL);
    Z2FxLineEditNode* fxNode = JKR_NEW_ARGS (heap, 0) Z2FxLineEditNode(heap);
    JUT_ASSERT(267, fxNode);
    soundRoot->appendNode(fxNode, NULL);
    Z2F1SwingTestNode* f1TestWin = JKR_NEW_ARGS (heap, 0) Z2F1SwingTestNode;
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
    JAWHioReceiver* receiver = JKR_NEW_ARGS (heap, 0) JAWHioReceiver;
    JAWExtSystem::registWindow(0, receiver, 10, 22);
    seqDataMgr = JKR_NEW_ARGS (heap, 0) Z2HioSeSeqDataMgr(param_1, receiver);
    JKRSetCurrentHeap(oldHeap);
    return seqDataMgr;
}

u8 gMicOn = true;
u8 gMicOffWhenOutOfSight = true;
extern u8 gMuffleOutOfRangeMic;

void Z2ParamNode::message(JAHControl& ctrl) {
    ctrl.makeComment("**** 各種パラメータ編集用ノード ****", 0, 5, 0);
    ctrl.returnY(1);
    ctrl.indent(1);
    ctrl.makeCheckBox("ＢＧＭ強制ボリュームＯＮ", &Z2GetSeqMgr()->field_0x04_debug, 1, false, 0);
    ctrl.makeSlider("ＢＧＭ強制ボリューム", &Z2GetSeqMgr()->field_0x00_debug, 0.0f, 1.0f, 0);
    ctrl.returnY(1);
    ctrl.makeComment("-- ミドナボイス --", 0, 5, 0);
    ctrl.indent(1);
    ctrl.makeSlider("発話スタイル", (s16*)Z2GetSpeechMgr2()->getStylePtr(), 0, 0x10, 0);
    ctrl.makeSlider("語尾(1=UP)", Z2GetSpeechMgr2()->getTalkerPtr(), 0, 1, 0);
    ctrl.indent(-1);
    ctrl.makeComment("-- ＢＧＭ汎用パラメータ --", 0, 5, 0);
    ctrl.indent(1);
    ctrl.makeSlider("シーンチェンジ時のＢＧＭフェードアウトタイム",
                    &Z2Param::SCENE_CHANGE_BGM_FADEOUT_TIME, 0, 0xff, 0);
    ctrl.makeSlider("ＢＧＭクロスフェードインタイム", &Z2Param::BGM_CROSS_FADEIN_TIME, 0, 0xff, 0);
    ctrl.makeSlider("ＢＧＭクロスフェードアウトタイム", &Z2Param::BGM_CROSS_FADEOUT_TIME, 0, 0xff,
                    0);
    ctrl.indent(-1);
    ctrl.makeComment("-- 戦闘ＢＧＭパラメータ --", 0, 5, 0);
    ctrl.indent(1);
    ctrl.makeSlider("戦闘ＢＧＭスタート禁止時間", &Z2Param::BATTLE_BGM_WAIT_TIME, 0, 0xff, 0);
    ctrl.makeSlider("戦闘ＢＧＭスタート距離", &Z2Param::BATTLE_FADEIN_DIST, 0.0f, 5000.0f, 0);
    ctrl.makeSlider("戦闘ＢＧＭフェードアウト距離", &Z2Param::BATTLE_FADEOUT_DIST, 0.0f, 5000.0f,
                    0);
    ctrl.makeSlider("接近戦状態距離", &Z2Param::ENEMY_NEARBY_DIST, 0.0f, 5000.0f, 0);
    ctrl.makeSlider("接近戦トラックＦＩタイム", &Z2Param::CLOSE_BATTLE_TRACK_FI_TIME, 0, 0xff, 0);
    ctrl.makeSlider("接近戦トラックＦＯタイム", &Z2Param::CLOSE_BATTLE_TRACK_FO_TIME, 0, 0xff, 0);
    ctrl.makeSlider("見つかりトラックＦＩタイム", &Z2Param::FOUND_TRACK_FI_TIME, 0, 0xff, 0);
    ctrl.makeSlider("見つかりトラックＦＯタイム", &Z2Param::FOUND_TRACK_FO_TIME, 0, 0xff, 0);
    ctrl.makeSlider("最後の一撃音量下げ範囲", &Z2Param::ENEMY_LASTHIT_MUTE_VOLUME, 0.0f, 1.0f, 0);
    ctrl.makeSlider("最後の一撃音量下げ時間", &Z2Param::ENDING_BLOW_VOL_DOWN_TIME, 0, 0xff, 0);
    ctrl.makeSlider("最後の一撃音量下げ持続時間", &Z2Param::ENDING_BLOW_VOL_LOWER_TIME, 0, 0xff, 0);
    ctrl.makeSlider("最後の一撃音量下げ復帰時間", &Z2Param::ENDING_BLOW_VOL_LOWER_RECOVER_TIME, 0,
                    0xff, 0);
    ctrl.makeSlider("最後の一撃〜フィニッシュ最低時間", &Z2Param::ENDING_BLOW_MIN_FINISH_TIME, 0,
                    0xff, 0);
    ctrl.indent(-1);
    ctrl.returnY(1);
    ctrl.makeComment("-- オーディブル演算パラメータ --", 0, 5, 0);
    ctrl.indent(1);
    ctrl.makeSlider("ボリューム変化許容範囲(+-)", &Z2Param::AUDIBLE_DELTA_RANGE_VOLUME, 0.0f, 1.0f,
                    0);
    ctrl.makeSlider("パン変化許容範囲(+-)", &Z2Param::AUDIBLE_DELTA_RANGE_PAN, 0.0f, 1.0f, 0);
    ctrl.makeSlider("ドルビー変化許容範囲(+-)", &Z2Param::AUDIBLE_DELTA_RANGE_DOLBY, 0.0f, 1.0f, 0);
    ctrl.makeSlider("ドップラーピッチ変化許容範囲(*/)", &Z2GetAudience()->getSetting()->field_0x60,
                    1.0f, 2.0f, 0);
    ctrl.indent(-1);
    ctrl.returnY(1);
    Z2SpotMic* pfVar2 = Z2GetAudience()->getLinkMic();
    if (pfVar2 != NULL) {
        ctrl.makeComment("-- リンクスポットマイク設定 --", 0, 5, 0);
        ctrl.indent(1);
        ctrl.makeCheckBox("マイクＯＮ？", &gMicOn, 1, false, 0);
        ctrl.makeCheckBox("視野外でマイクオフ？", &gMicOffWhenOutOfSight, 1, false, 0);
        ctrl.makeSlider("マイク音量最大距離", &pfVar2->field_0x0, 0.0f, 1000.0f, 0);
        ctrl.makeSlider("マイク音量最小距離", &pfVar2->field_0x4, 0.0f, 1000.0f, 0);
        ctrl.makeSlider("最小音量", &pfVar2->field_0x8, 0.0f, 1.0f, 0);
        ctrl.makeSlider("最大音量の最小値", &pfVar2->field_0xc, 0.0f, 1.0f, 0);
        ctrl.makeButton("マイク設定更新", 3, 3, 0);
        ctrl.returnY(1);
        ctrl.makeCheckBox("マイク圏外をボリュームダウン", &gMuffleOutOfRangeMic, 1, false, 0);
        ctrl.indent(1);
        ctrl.makeSlider("マイク圏外の音量比", &Z2GetAudience()->field_0x4, 0.0f, 1.0f, 0);
        ctrl.indent(-1);
        ctrl.indent(-1);
    }
    ctrl.makeComment("-- 闇のＳＥフィルタ実験 --", 0, 5, 0);
    ctrl.indent(1);
    ctrl.makeButton("フィルタリセット", 2, 3, 0);
    ctrl.makeCheckBox("フィルタＯＮ", &Z2Param::DARK_SE_FILTER_ON, 1, false, 0);
    ctrl.makeCheckBox("システムＳＥにも適用", &Z2Param::SYSTEM_SE_USE_DARK_SE_SETTING, 1, false, 0);
    ctrl.makeSlider("ローパスフィルタ設定（強→弱）", &Z2Param::DARK_SE_LOW_PASS_FILTER_SETTING, 0,
                    0x7f, 0);
}

void Z2ParamNode::propertyEvent(JAH_P_Event param_1, u32 param_2) {
    if (param_1 == JAH_P_EVENT0)
        return;

    switch (param_2) {
    case 2:
        Z2GetSoundMgr()->resetFilterAll();
        break;
    case 3:
        Z2GetAudience()->getLinkMic()->calcPriorityFactor();
        break;
    }
}

void Z2ParamNode::onFrame() {
    Z2SpotMic* mic = Z2GetAudience()->getLinkMic();
    if (!mic)
        return;

    mic->setMicOn(gMicOn != 0);
    mic->setIgnoreIfOut(gMicOffWhenOutOfSight != 0);
    Z2GetAudience()->setUsingOffMicVol(gMuffleOutOfRangeMic != 0);
}
