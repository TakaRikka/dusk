#ifndef M_DO_M_DO_AUDIO_H
#define M_DO_M_DO_AUDIO_H

#include "Z2AudioLib/Z2AudioMgr.h"
#include "Z2AudioLib/Z2EnvSeMgr.h"
#include "Z2AudioLib/Z2LinkMgr.h"

class mDoAud_zelAudio_c : public Z2AudioMgr {
public:
    void reset();
    mDoAud_zelAudio_c() {
#if DEBUG
        setMode(2);
#endif
    }
    ~mDoAud_zelAudio_c() {}

#if DEBUG
    u8 getMode() { return field_0x13bd; }
    void setMode(u8 mode) { field_0x13bd = mode; }

    /* 0x13BD */ u8 field_0x13bd;
#endif

    static void onInitFlag() { mInitFlag = true; }
    static bool isInitFlag() { return mInitFlag; }
    static bool isResetFlag() { return mResetFlag; }
    static void onResetFlag() { mResetFlag = true; }
    static void offResetFlag() { mResetFlag = false; }
    static bool isBgmSet() { return mBgmSet; }
    static void onBgmSet() { mBgmSet = true; }
    static void offBgmSet() { mBgmSet = false; }

    static bool mInitFlag;
    static bool mResetFlag;
    static bool mBgmSet;
};

extern JKRSolidHeap* g_mDoAud_audioHeap;

void mDoAud_Execute();
void mDoAud_resetProcess();
bool mDoAud_resetRecover();
void mDoAud_setSceneName(char const* spot, s32 room, s32 layer);
s32 mDoAud_load1stDynamicWave();
void mDoAud_setFadeInStart(u8 param_0);
void mDoAud_setFadeOutStart(u8 param_0);

inline void mDoAud_setLinkGroupInfo(u8 param_0) {
    if (Z2GetLink() != NULL) {
        Z2GetLink()->setLinkGroupInfo(param_0);
    }
}

inline void mDoAud_setLinkHp(s32 param_0, s32 param_1) {
    if (Z2GetLink() != NULL) {
        Z2GetLink()->setLinkHp(param_0, param_1);
    }
}

inline void mDoAud_bgmSetSwordUsing(s32 id) {
    Z2AudioMgr::getInterface()->bgmSetSwordUsing(id);
}

inline void mDoAud_bgmStart(u32 i_bgmID) {
    Z2AudioMgr::getInterface()->bgmStart(i_bgmID, 0, 0);
}

inline void mDoAud_bgmAllMute(u32 i_count) {
    Z2AudioMgr::getInterface()->bgmAllMute(i_count, 0.0f);
}

inline void mDoAud_subBgmStart(u32 i_bgmID) {
    Z2AudioMgr::getInterface()->subBgmStart(i_bgmID);
}

inline void mDoAud_subBgmStop() {
    Z2AudioMgr::getInterface()->subBgmStop();
}

inline u32 mDoAud_checkPlayingSubBgmFlag() {
    return Z2AudioMgr::getInterface()->checkPlayingSubBgmFlag();
}

inline void mDoAud_bgmNowBattle(f32 param_0) {
    Z2AudioMgr::getInterface()->bgmNowBattle(param_0);
}

inline void mDoAud_bgmStreamPrepare(u32 param_0) {
    Z2AudioMgr::getInterface()->bgmStreamPrepare(param_0);
}

inline void mDoAud_bgmStreamPlay() {
    Z2AudioMgr::getInterface()->bgmStreamPlay();
}

inline void mDoAud_setHour(s32 hour) {
    Z2AudioMgr::getInterface()->setHour(hour);
}

inline void mDoAud_setMinute(s32 min) {
    Z2AudioMgr::getInterface()->setMinute(min);
}

inline void mDoAud_setWeekday(s32 day) {
    Z2AudioMgr::getInterface()->setWeekday(day);
}

inline void mDoAud_setInDarkness(bool state) {
    Z2AudioMgr::getInterface()->setInDarkness(state);
}

inline void mDoAud_seStart(u32 i_sfxID, const Vec* i_sePos, u32 param_2, s8 i_reverb) {
    Z2AudioMgr::getInterface()->seStart(i_sfxID, i_sePos, param_2, i_reverb, 1.0f, 1.0f,
                                               -1.0f, -1.0f, 0);
}

inline void mDoAud_seStartLevel(u32 i_sfxID, const Vec* i_sePos, u32 param_2, s8 i_reverb) {
    Z2AudioMgr::getInterface()->seStartLevel(i_sfxID, i_sePos, param_2, i_reverb, 1.0f, 1.0f,
                                                    -1.0f, -1.0f, 0);
}

inline void mDoAud_seStop(u32 i_sfxID, u32 param_1) {
    Z2AudioMgr::getInterface()->seStop(i_sfxID, param_1);
}

inline void mDoAud_messageSePlay(u16 param_0, Vec* position, s8 param_2) {
    Z2AudioMgr::getInterface()->messageSePlay(param_0, position, param_2);
}

inline void mDoAud_sceneBgmStart() {
    Z2AudioMgr::getInterface()->sceneBgmStart();
}

inline void mDoAud_seDeleteObject(Vec*) {}

inline void mDoAud_load2ndDynamicWave() {
    Z2AudioMgr::getInterface()->load2ndDynamicWave();
}

inline bool mDoAud_check1stDynamicWave() {
    return Z2AudioMgr::getInterface()->check1stDynamicWave();
}

inline void mDoAud_bgmStop(u32 param_0) {
    Z2AudioMgr::getInterface()->bgmStop(param_0, 0);
}

inline void mDoAud_rainPlay(s32 enable) {
    g_mEnvSeMgr.startRainSe(enable, 0);
}

inline void mDoAud_heartGaugeOn() {
    Z2AudioMgr::getInterface()->heartGaugeOn();
}

inline void mDoAud_setSnowPower(s8 i_power) {
    g_mEnvSeMgr.setSnowPower(i_power);
}

inline void mDoAud_setFogWipeWidth(f32 i_width) {
    g_mEnvSeMgr.setFogWipeWidth(i_width);
}

inline void mDoAud_startFogWipeTrigger(const Vec* param_0) {
    g_mEnvSeMgr.startFogWipeTrigger((Vec*)param_0);
}

inline void mDoAud_changeSubBgmStatus(s32 param_0) {
    Z2AudioMgr::getInterface()->changeSubBgmStatus(param_0);
}

inline void mDoAud_taktModeMute() {
    Z2AudioMgr::getInterface()->taktModeMute();
}

inline void mDoAud_taktModeMuteOff() {
    Z2AudioMgr::getInterface()->taktModeMuteOff();
}

inline void mDoAud_getCameraMapInfo(u32 param_0) {
    Z2AudioMgr::getInterface()->getCameraMapInfo(param_0);
}

inline void mDoAud_setCameraGroupInfo(u8 param_0) {
    Z2AudioMgr::getInterface()->setCameraGroupInfo(param_0);
}

inline void mDoAud_setLinkShieldType(s32 param_0, s32 param_1) {
    if (Z2GetLink() != NULL) {
        Z2GetLink()->setLinkShieldType(param_0, param_1);
    }
}

inline void mDoAud_mEnvse_framework() {
    g_mEnvSeMgr.framework();
}

inline void mDoAud_mEnvse_resetScene() {
    g_mEnvSeMgr.resetScene();
}

inline void mDoAud_mEnvSe_startFarThunderSe(const Vec* param_0) {
    g_mEnvSeMgr.startFarThunderSe((Vec*)param_0, 0);
}

inline void mDoAud_mEnvSe_startNearThunderSe() {
    g_mEnvSeMgr.startNearThunderSe(0);
}

inline void mDoAud_mEnvse_initStrongWind() {
    g_mEnvSeMgr.initStrongWindSe();
}

inline void mDoAud_mEnvse_setWindDirection(Vec* i_direction) {
    g_mEnvSeMgr.setWindDirection(i_direction);
}

inline void mDoAud_mEnvse_startStrongWindSe(s8 i_reverb) {
    g_mEnvSeMgr.startStrongWindSe(i_reverb);
}

inline void mDoAud_mEnvse_setWindType(u8 i_type) {
    g_mEnvSeMgr.setWindType(i_type);
}

inline void mDoAud_mEnvse_initStaticEnvSe(u8 param_0, u8 param_1, u8 param_2, u8 param_3, const Vec* param_4) {
    g_mEnvSeMgr.initStaticEnvSe(param_0, param_1, param_2, param_3, (Vec*)param_4);
}

inline void mDoAud_mEnvse_startStaticEnvSe(s8 i_reverb) {
    g_mEnvSeMgr.startStaticEnvSe(i_reverb);
}

inline void mDoAud_mEnvse_initRiverSe(u8 param_0, u8 param_1, u8 param_2, u8 param_3) {
    g_mEnvSeMgr.initRiverSe(param_0, param_1, param_2, param_3);
}

inline void mDoAud_mEnvse_registRiverSePos(const Vec* i_pos) {
    g_mEnvSeMgr.registRiverSePos((Vec*)i_pos);
}

inline void mDoAud_mEnvse_startRiverSe(s8 i_reverb) {
    g_mEnvSeMgr.startRiverSe(i_reverb);
}

inline void mDoAud_mEnvse_initFallSe(u8 param_0, u8 param_1, u8 param_2, u8 param_3) {
    g_mEnvSeMgr.initFallSe(param_0, param_1, param_2, param_3);
}

inline void mDoAud_mEnvse_registFallSePos(const Vec* i_pos) {
    g_mEnvSeMgr.registFallSePos((Vec*)i_pos);
}

inline void mDoAud_mEnvse_startFallSe(s8 i_reverb) {
    g_mEnvSeMgr.startFallSe(i_reverb);
}

inline void mDoAud_mEnvse_initSmellSe(u8 param_0, u8 param_1, u8 param_2, u8 param_3) {
    g_mEnvSeMgr.initSmellSe(param_0, param_1, param_2, param_3);
}

inline void mDoAud_mEnvse_registSmellSePos(const Vec* i_pos) {
    g_mEnvSeMgr.registSmellSePos((Vec*)i_pos);
}

inline void mDoAud_mEnvse_startSmellSe(s8 i_reverb) {
    g_mEnvSeMgr.startSmellSe(i_reverb);
}

inline void mDoAud_mEnvse_registWindowPos(const Vec* i_pos) {
    g_mEnvSeMgr.registWindowPos((Vec*)i_pos);
}

inline void mDoAud_mEnvse_registWolfSmellSePos(const Vec* i_pos) {
    g_mEnvSeMgr.registWolfSmellSePos((Vec*)i_pos);
}

inline void mDoAud_initLv3WaterSe(u8 param_0, u8 param_1, u8 param_2, u8 param_3) {
    g_mEnvSeMgr.initLv3WaterSe(param_0, param_1, param_2, param_3);
}

inline void mDoAud_registLv3WaterSePos(u8 param_0, const Vec* i_pos) {
    g_mEnvSeMgr.registLv3WaterSePos(param_0, (Vec*)i_pos);
}

inline void mDoAud_startLv3WaterSe(s8 i_reverb) {
    g_mEnvSeMgr.startLv3WaterSe(i_reverb);
}

inline void mDoAud_setHyrulSewerOpen(bool i_close) {
    g_mEnvSeMgr.setHyrulSewerOpen(i_close);
}

inline void mDoAud_startFogSe() {
    g_mEnvSeMgr.startFogSe();
}

inline void mDoAud_talkOut() {
    Z2AudioMgr::getInterface()->talkOut();
}

inline void mDoAud_talkIn() {
    Z2AudioMgr::getInterface()->talkIn();
}

inline void mDoAud_setOutputMode(u32 mode) {
    Z2AudioMgr::getInterface()->setOutputMode(mode);
}

inline void mDoAud_loadStaticWaves() {
    Z2AudioMgr::getInterface()->loadStaticWaves();
}

inline int mDoAud_monsSeStart(u32 i_soundId, const Vec* i_pos, u32 i_actorId, u32 param_3,
                               s8 i_reverb) {
    UNUSED(i_actorId);
    return Z2GetAudioMgr()->seStart(i_soundId, i_pos, param_3, i_reverb, 1.0f, 1.0f, -1.0f, -1.0f,
                                    0);
}

#endif /* M_DO_M_DO_AUDIO_H */
