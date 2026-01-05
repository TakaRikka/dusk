#include "Z2AudioLib/Z2Creature.h"
#include "Z2AudioLib/Z2Param.h"
#include "Z2AudioLib/Z2Calc.h"
#include "Z2AudioLib/Z2AudioMgr.h"

static void Z2_E_sw_modPitch(Z2SoundHandlePool*, u32);
static void Z2_E_ms_modVol(Z2SoundHandlePool*, u32);
static void Z2_E_mm_modPitch(Z2SoundHandlePool*, u32);
static void Z2_B_zan_modPitch(Z2SoundHandlePool*, u32);

s8 data_80451358;
s8 data_80451359;

static Z2EnemyArea const sAreaDefault = {true, 700, 1100, 1500};

static Z2EnemyArea const sAreaFloating = {false, 700, 1100, 1500};

static Z2EnemyArea const sAreaWide = {true, 700, 1500, 3300};

static Z2EnemyArea const sAreaWideFloating = {false, 700, 1500, 3300};

static Z2EnemyArea const sAreaSmall = {true, 300, 700, 1000};

static Z2EnemyArea const sAreaSmallFloating = {false, 500, 800, 1200};

struct EnemyInfo {
    char mName[8];
    Z2EnemyArea mEnemyArea;
};

static EnemyInfo mEnemyInfo[64] = {
    {"Dummy", sAreaDefault},
    {"E_s1", {true, 700, 1200, 2000}},
    {"E_yk", sAreaSmallFloating},
    {"E_yr", sAreaWideFloating},
    {"E_yg", {true, 300, 500, 700}},
    {"E_yc", sAreaWideFloating},
    {"E_yd", {true, 500, 900, 1500}},
    {"E_rdy", sAreaDefault},
    {"E_ymb", {false, 10, 20, 30}},
    {"", sAreaDefault},
    {"E_wb", sAreaDefault},
    {"E_rd", sAreaDefault},
    {"E_dn", sAreaDefault},
    {"E_kr", sAreaWideFloating},
    {"E_st", sAreaDefault},
    {"E_tk", {true, 700, 1500, 2500}},
    {"E_tk2", {true, 700, 1500, 2500}},
    {"E_cr", sAreaSmall},
    {"E_df", sAreaDefault},
    {"E_fs", sAreaDefault},
    {"E_SB", sAreaDefault},
    {"E_PM", sAreaDefault},
    {"E_db", {true, 500, 900, 1800}},
    {"E_kg", sAreaSmall},
    {"E_sh", sAreaDefault},
    {"E_sf", sAreaDefault},
    {"E_bs", {true, 200, 400, 600}},
    {"E_SM", sAreaDefault},
    {"E_ba", sAreaSmallFloating},
    {"E_bu", sAreaFloating},
    {"E_HM", sAreaDefault},
    {"E_ge", sAreaFloating},
    {"E_hz", {false, 500, 800, 1300}},
    {"E_ws", sAreaSmallFloating},
    {"E_oc", sAreaDefault},
    {"E_fz", sAreaDefault},
    {"E_fb", sAreaDefault},
    {"E_bg", sAreaSmall},
    {"E_mm", {true, 500, 800, 1500}},
    {"E_nz", {true, 500, 700, 1100}},
    {"E_po", sAreaWide},
    {"E_rb", sAreaDefault},
    {"E_sg", {true, 500, 700, 900}},
    {"E_hb", {true, 500, 900, 1500}},
    {"E_gb", {true, 1600, 3000, 4000}},
    {"E_ms", {true, 500, 700, 1100}},
    {"E_dd", sAreaDefault},
    {"E_bi", {true, 500, 700, 900}},
    {"E_tt", sAreaSmallFloating},
    {"E_ai", sAreaDefault},
    {"E_dk", sAreaSmall},
    {"E_ww", sAreaWide},
    {"E_gi", sAreaDefault},
    {"E_sm2", sAreaDefault},
    {"E_sw", {true, 400, 600, 1500}},
    {"E_rdb", {true, 500, 1000, 1500}},
    {"E_kk", sAreaDefault},
    {"E_hp", sAreaDefault},
    {"E_zh", sAreaDefault},
    {"E_gm", sAreaSmall},
    {"B_GG", {false, 1000, 2000, 4000}},
    {"E_mf", sAreaDefault},
    {"E_zm", sAreaDefault},
    {"B_tn", sAreaDefault},
};

Z2Creature::Z2Creature() {
    mpPos = NULL;
}

Z2Creature::~Z2Creature() {
    deleteObject();
}

void Z2Creature::deleteObject() {
    mSoundObjAnime.stopAnime();
    mSoundObjAnime.deleteObject();
    mSoundObjSimple1.deleteObject();
    mSoundObjSimple2.deleteObject();
}

void Z2Creature::init(Vec* animePosPtr, Vec* simplePosPtr, u8 animeHandleNum, u8 simpleHandleNum) {
    if (animePosPtr) {
        mSoundObjAnime.init(animePosPtr, animeHandleNum);
        mpPos = animePosPtr;
    }

    if (simplePosPtr) {
        mSoundObjSimple1.init(simplePosPtr, simpleHandleNum);
    }
}

void Z2Creature::init(Vec* animePosPtr, Vec* simplePosPtr, Vec* simple2PosPtr, u8 animeHandleNum, u8 simpleHandleNum,
                      u8 simple2HandleNum) {
    init(animePosPtr, simplePosPtr, animeHandleNum, simpleHandleNum);

    if (simple2PosPtr) {
        mSoundObjSimple2.init(simple2PosPtr, simple2HandleNum);
    }
}

void Z2Creature::setSoundStarter(Z2SoundStarter* soundStarter) {
    mSoundObjAnime.setSoundStarter(soundStarter);
    mSoundObjSimple1.setSoundStarter(soundStarter);
    mSoundObjSimple2.setSoundStarter(soundStarter);
}

void Z2Creature::initAnime(void* animation, bool param_1, f32 startFrame, f32 param_3) {
    mSoundObjAnime.initAnime(animation, param_1, startFrame, param_3);
}

void Z2Creature::framework(u32 mapinfo, s8 reverb) {
    mSoundObjAnime.framework(mapinfo, reverb);
    mSoundObjSimple1.framework(mapinfo, reverb);
    mSoundObjSimple2.framework(mapinfo, reverb);
}

void Z2Creature::updateAnime(f32 param_0, f32 param_1) {
    mSoundObjAnime.updateAnime(param_0, param_1);
}

void Z2Creature::stopAnime() {
    deleteObject();
}

Z2SoundHandlePool* Z2Creature::startCreatureSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    switch (soundID) {
    case Z2SE_KOSARU_V_KAGO_WAIT:
        return mSoundObjSimple1.startSound(soundID, mapinfo, 0);
    default:
        Z2SoundHandlePool* handle = mSoundObjAnime.startSound(soundID, mapinfo, reverb);
        if (handle != NULL && *handle) {
            switch (soundID) {
            case Z2SE_FAIRY_S_FLY:
                f32 pitch = Z2Calc::getParamByExp((f32)mapinfo, 30.0f, 100.0f, 0.3f,
                                                    0.8f, 1.2f, Z2Calc::CURVE_NEGATIVE);
                f32 volume = Z2Calc::getParamByExp((f32)mapinfo, 30.0f, 100.0f, 0.3f,
                                                    0.5f, 1.0f, Z2Calc::CURVE_NEGATIVE);
                (*handle)->getAuxiliary().moveVolume(volume, 0);
                (*handle)->getAuxiliary().movePitch(pitch, 0);
            }
        }
        return handle;
    }
}

Z2SoundHandlePool* Z2Creature::startCreatureSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    switch (soundID) {
    case Z2SE_FAIRY_S_LV:
        Z2GetAudioMgr()->seStartLevel(soundID, mpPos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        return NULL;
    default:
        Z2SoundHandlePool* handle = mSoundObjAnime.startLevelSound(soundID, mapinfo, reverb);
        if (handle != NULL && *handle) {
            if (soundID == Z2SE_YM_SNOBO_RIDE || soundID == Z2SE_YW_SNOBO_RIDE) {
                f32 volume = 0.0f;
                if (mapinfo != 0) {
                    volume = Z2Calc::getParamByExp((f32)mapinfo, 0.0f, 92.0f, 0.25f,
                                                    0.4f, 1.0f, Z2Calc::CURVE_NEGATIVE);
                }
                f32 pitch = Z2Calc::getParamByExp((f32)mapinfo, 0.0f, 92.0f, 0.3f,
                                                    0.5f, 1.2f, Z2Calc::CURVE_NEGATIVE);
                
                (*handle)->getAuxiliary().moveVolume(volume, 0);
                (*handle)->getAuxiliary().movePitch(pitch, 0);
            } else if (soundID == Z2SE_ZRZ_MV) {
                f32 volume = (f32)mapinfo / 100.0f;
                (*handle)->getAuxiliary().moveVolume(volume, 0);
            }
        }
        return handle;
    }
}

Z2SoundHandlePool* Z2Creature::startCreatureVoice(JAISoundID soundID, s8 reverb) {
    switch (soundID) {
    case Z2SE_MDN_V_FLY_OUT:
    case Z2SE_MDN_V_MGN_TAME:
    case Z2SE_MDN_V_CLINGST:
    case Z2SE_MDN_V_LV_PANIC:
    case Z2SE_MDN_V_HIT:
    case Z2SE_MDN_V_LOOKABOUT_1:
    case Z2SE_MDN_V_WLSWIMDIE:
    case Z2SE_MDN_V_WAITD:
    case Z2SE_MDN_V_SWAITA:
    case Z2SE_MDN_V_SWAITB_1:
    case Z2SE_MDN_V_SWAITB_3:
    case Z2SE_MDN_V_SWAITC:
    case Z2SE_MDN_V_SWAITD:
    case Z2SE_MDN_V_LEADSWAIT:
    case Z2SE_MDN_V_HIP:
    case Z2SE_MDN_V_WLDIEA:
    case Z2SE_MDN_V_ENTRANCE:
    case Z2SE_MDN_V_DOOROP:
    case Z2SE_MDN_V_ROPEWAIT_1:
    case Z2SE_MDN_V_ROPEWAIT_2:
    case Z2SE_MDN_V_ROPEFALTER:
    case Z2SE_MDN_V_WAITTP:
        if (Z2GetSpeechMgr2()->isMidnaSpeak()) {
            return NULL;
        }
    }

    return mSoundObjSimple1.startSound(soundID, 0, reverb);
}

Z2SoundHandlePool* Z2Creature::startCreatureVoiceLevel(JAISoundID soundID, s8 reverb) {
    if (soundID == Z2SE_MDN_V_WAITD) {
        if (Z2GetStatusMgr()->getDemoStatus() == 2 || !Z2GetSceneMgr()->isInGame()) {
            return NULL;
        }
    }

    return mSoundObjSimple1.startLevelSound((u32)soundID, 0, reverb);
}

Z2SoundHandlePool* Z2Creature::startCreatureExtraSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    return mSoundObjSimple2.startSound(soundID, mapinfo, reverb);
}

Z2SoundHandlePool* Z2Creature::startCreatureExtraSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    return mSoundObjSimple2.startLevelSound(soundID, mapinfo, reverb);
}

Z2SoundHandlePool* Z2Creature::startCollisionSE(u32 hitID, u32 mapinfo) {
    return mSoundObjAnime.startCollisionSE(hitID, mapinfo, NULL);
}

Z2CreatureCitizen::Z2CreatureCitizen() :
    JSULink<Z2CreatureCitizen>(this),
    mCitizenID(-1),
    field_0xa1(-1),
    mLinkSearch(false)
{}

Z2CreatureCitizen::~Z2CreatureCitizen() {}

void Z2CreatureCitizen::init(Vec* animePosPtr, Vec* simplePosPtr, u8 animeHandleNum, u8 simpleHandleNum) {
    Z2Creature::init(animePosPtr, simplePosPtr, animeHandleNum, simpleHandleNum);
}

void Z2CreatureCitizen::deleteObject() {
    if (mLinkSearch == false) {
        Z2GetSeMgr()->decrCrowdSize();
    }

    Z2Creature::deleteObject();
}

void Z2CreatureCitizen::setMdlType(s8 param_0, bool param_1, bool param_2) {
    s8 a = param_0 & 0x3f;
    s8 b = param_1 ? 0x40 : 0;
    s8 c = param_2 ? 0x80 : 0;
    mCitizenID = a + c + b;

    if (!param_2) {
        Z2GetSeMgr()->incrCrowdSize();
    }

    if (param_0 == 0) {
        field_0xa1 = data_80451358;
        data_80451358 = !data_80451358 ? 1 : 0;
    } else if (param_0 == 1) {
        field_0xa1 = data_80451359;
        data_80451359 = !data_80451359 ? 1 : 0;
    }
}

void Z2CreatureCitizen::playVoice(int i_sound) {
    u32 sound_id = Z2SE_MAN_A1_V_AWAY + 6 * (s8)mCitizenID + i_sound;
    switch (mCitizenID) {
    case 0:
    case 1:
        if (field_0xa1 == 1) {
            sound_id += 3;
        }
        break;
    }

    mSoundObjSimple1.startSound(sound_id, 0, -1);
    if ((i_sound == 0 || i_sound == 2) && !mLinkSearch) {
        Z2GetSeMgr()->decrCrowdSize();
        mLinkSearch = true;
    }
}

Z2SoundHandlePool* Z2CreatureCitizen::startCreatureVoice(JAISoundID soundID, s8 reverb) {
    switch (soundID) {
    case Z2SE_KIU_V_FEAR:
    case Z2SE_HANA_V_FEAR:
    case Z2SE_MICH_V_FEAR:
        if (!mLinkSearch) {
            Z2GetSeMgr()->decrCrowdSize();
            mLinkSearch = true;
        }
        break;
    }

    return Z2Creature::startCreatureVoice(soundID, -1);
}

Z2CreatureEnemy::Z2CreatureEnemy() :
    JSULink<Z2CreatureEnemy>(this),
    mEnemyID(0),
    field_0xa1(1),
    mLinkSearch(false),
    field_0xa3(0)
{}

void Z2CreatureEnemy::deleteObject() {
    if (mEnemyID == Z2_ENEMY_GB) {
        Z2GetSeqMgr()->unMuteSceneBgm(33);
    }

    if (getSupervisor() != NULL) {
        Z2GetSoundObjMgr()->removeEnemy(this);
    }

    mSoundObjAnime.stopAnime();
    mSoundObjAnime.deleteObject();
    mSoundObjSimple1.deleteObject();
    mSoundObjSimple2.deleteObject();
}

void Z2CreatureEnemy::init(Vec* animePosPtr, Vec* simplePosPtr, u8 animeHandleNum, u8 simpleHandleNum) {
    Z2Creature::init(animePosPtr, simplePosPtr, animeHandleNum, simpleHandleNum);
}

void Z2CreatureEnemy::init(Vec* animePosPtr, Vec* simplePosPtr, Vec* simple2PosPtr, u8 animeHandleNum, u8 simpleHandleNum,
                           u8 simple2HandleNum) {
    Z2Creature::init(animePosPtr, simplePosPtr, simple2PosPtr, animeHandleNum, simpleHandleNum, simple2HandleNum);
}

void Z2CreatureEnemy::framework(u32 mapinfo, s8 reverb) {
    if (field_0xa1 < 1) {
        field_0xa1++;
    }

    Z2Creature::framework(mapinfo, reverb);
}

Z2SoundHandlePool* Z2CreatureEnemy::startCreatureSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    Z2SoundHandlePool* handle = mSoundObjAnime.startSound(soundID, mapinfo, reverb);

    switch (mEnemyID) {
    case Z2_ENEMY_SF:
        if (soundID == Z2SE_EN_SF_BREAK_DOWN) {
            field_0xa3 = 1;
            Z2GetSoundObjMgr()->getEnemyList()->remove(this);
        } else if (field_0xa3 != 0 && soundID == Z2SE_EN_SF_BUILD_UP) {
            field_0xa3 = 0;
            Z2GetSoundObjMgr()->getEnemyList()->append(this);
        }
        break;
    case Z2_ENEMY_FZ:
        if (handle != NULL && *handle && soundID == Z2SE_EN_FZ_BOUND) {
            f32 volume = Z2Calc::linearTransform(mapinfo, 10.0f, 50.0f, 0.1f, 1.0f, false);
            f32 pitch = Z2Calc::linearTransform(mapinfo, 10.0f, 50.0f, 0.8f, 1.2f, false);
            (*handle)->getAuxiliary().moveVolume(volume, 0);
            (*handle)->getAuxiliary().movePitch(pitch, 0);
        }
        break;
    case Z2_ENEMY_MS: {
        u32 var_r24 = mSoundObjAnime.field_0x1c;
        Z2_E_ms_modVol(handle, var_r24);
        break;
    }
    case Z2_ENEMY_SW:
        Z2_E_sw_modPitch(handle, mapinfo);
        break;
    case Z2_ENEMY_MM: {
        u32 var_r23 = mSoundObjAnime.field_0x1c;
        Z2_E_mm_modPitch(handle, var_r23);
        break;
    }
    case Z2_ENEMY_SM2:
        if (soundID == Z2SE_EN_SM_HIT) {
            field_0xa3 = 1;
            Z2GetSeqMgr()->setBattleLastHit(Z2Param::ENDING_BLOW_VOL_LOWER_TIME);
        }
        break;
    }
    
    switch (soundID) {
    case Z2SE_EN_ZAN_YO_LAND: {
        u32 var_r22 = mSoundObjAnime.field_0x1c;
        Z2_B_zan_modPitch(handle, var_r22);
        break;
    }
    case Z2SE_EN_ZAN_YO_FLOAT_WAIT:
        u32 var_r21 = mSoundObjAnime.field_0x1c; 
        if (var_r21 > 100) {
            return NULL;
        }
        break;
    }

    return handle;
}

Z2SoundHandlePool* Z2CreatureEnemy::startCreatureSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    if (soundID == 0x7FFFF) {
        Vec pos;
        Z2GetAudience()->convertAbsToRel(*mpPos, &pos, 0);
        f32 volume = Z2GetAudience()->calcRelPosVolume(pos, 2.0f, 0);
        f32 pan = Z2GetAudience()->calcRelPosPan(pos, 0);
        f32 dolby = Z2GetAudience()->calcRelPosDolby(pos, 0);
        Z2GetSeqMgr()->setChildTrackVolume(Z2GetSeqMgr()->getMainBgmHandle(),
                                           7, volume, 0, pan, dolby);
        return NULL;
    }

    Z2SoundHandlePool* handle = mSoundObjAnime.startLevelSound(soundID, mapinfo, reverb);

    switch (mEnemyID) {
    case Z2_ENEMY_FZ:
        if (handle != NULL && *handle && soundID == Z2SE_EN_FZ_MOVE) {
            f32 volume = Z2Calc::getParamByExp(mapinfo, 1.0f, 50.0f, 0.4f, 0.1f, 1.1f,
                                               Z2Calc::CURVE_POSITIVE);
            (*handle)->getAuxiliary().moveVolume(volume, 0);
        }
        break;
    case Z2_ENEMY_SW:
        Z2_E_sw_modPitch(handle, mapinfo);
        break;
    case Z2_ENEMY_RB:
        if (handle != NULL && *handle && soundID == Z2SE_EN_RB_MOVE) {
            f32 volume = Z2Calc::getParamByExp(mapinfo, 4.0f, 35.0f, 0.6f, 0.7f, 1.0f,
                                               Z2Calc::CURVE_POSITIVE);
            f32 pitch = Z2Calc::getParamByExp(mapinfo, 4.0f, 35.0f, 0.4f, 0.8f, 1.0f,
                                              Z2Calc::CURVE_POSITIVE);
            (*handle)->getAuxiliary().moveVolume(volume, 0);
            (*handle)->getAuxiliary().movePitch(pitch, 0);
            break;
        }
        // fallthrough
    default:
        if (handle != NULL && *handle && soundID == Z2SE_EN_PH_PROPELLER) {
            f32 volume = Z2Calc::getParamByExp(mapinfo, 0.0f, 30.0f, 0.4f, 0.0f, 1.0f,
                                               Z2Calc::CURVE_POSITIVE);
            f32 pitch = Z2Calc::getParamByExp(mapinfo, 4000.0f, 13000.0f, 0.4f, 0.8f, 1.2f,
                                              Z2Calc::CURVE_POSITIVE);
            (*handle)->getAuxiliary().moveVolume(volume, 0);
            (*handle)->getAuxiliary().movePitch(pitch, 0);
        }
        break;
    }

    return handle;
}

Z2SoundHandlePool* Z2CreatureEnemy::startCreatureVoice(JAISoundID soundID, s8 reverb) {
    if (Z2GetSeqMgr()->getMainBgmID() == Z2BGM_BOSS_ZANT) {
        switch (Z2GetSeqMgr()->mBgmStatus) {
        case 0:
        case 1:
            break;
        case 2:
        case 3:
            switch (soundID) {
            case Z2SE_EN_ZAN_V_ATK_BALL:
                soundID = Z2SE_EN_ZAN_V_ATK_BALL_MG;
                break;
            case Z2SE_EN_ZAN_V_DMG:
                soundID = Z2SE_EN_ZAN_V_DMG_MG;
                break;
            }
            break;
        case 4:
        case 5:
            switch (soundID) {
            case Z2SE_EN_ZAN_V_ATK_BALL:
                soundID = Z2SE_EN_ZAN_V_ATK_BALL_OI;
                break;
            case Z2SE_EN_ZAN_V_DMG:
                soundID = Z2SE_EN_ZAN_V_DMG_OI;
                break;
            }
            break;
        case 6:
        case 7:
            switch (soundID) {
            case Z2SE_EN_ZAN_V_ATK_BALL:
                soundID = Z2SE_EN_ZAN_V_ATK_BALL_MK;
                break;
            case Z2SE_EN_ZAN_V_DMG:
                soundID = Z2SE_EN_ZAN_V_DMG_MK;
                break;
            }
            break;
        case 8:
        case 9:
            switch (soundID) {
            case Z2SE_EN_ZAN_V_DMG:
                soundID = Z2SE_EN_ZAN_V_DMG_YO;
                break;
            }
            break;
        case 10:
        case 11:
            switch (soundID) {
            case Z2SE_EN_ZAN_V_DMG:
                soundID = Z2SE_EN_ZAN_V_DMG_CTL;
                break;
            }
            break;
        }
    }

    u32 mapinfo = 0;
    if (mEnemyID == Z2_ENEMY_RDY) {
        mapinfo = 1;
    }

    Z2SoundHandlePool* handle = mSoundObjSimple1.startSound(soundID, mapinfo, reverb);

    switch (mEnemyID) {
    case Z2_ENEMY_SH:
        if (soundID == Z2SE_EN_SH_V_DEAD) {
            field_0xa3 = 1;
            Z2GetSeqMgr()->setBattleLastHit(Z2Param::ENDING_BLOW_VOL_LOWER_TIME);
        }
        break;
    case Z2_ENEMY_BS:
        if (soundID == Z2SE_EN_BS_V_DEAD) {
            field_0xa3 = 1;
            Z2GetSeqMgr()->setBattleLastHit(Z2Param::ENDING_BLOW_VOL_LOWER_TIME);
        }
        break;
    case Z2_ENEMY_MS: {
        u32 var_r26 = mSoundObjSimple1.field_0x1c;
        Z2_E_ms_modVol(handle, var_r26);
        break;
    }
    case Z2_ENEMY_SW: {
        u32 var_r25 = mSoundObjSimple1.field_0x1c;
        Z2_E_sw_modPitch(handle, var_r25);
        break;
    }
    case Z2_ENEMY_MM: {
        u32 var_r24 = mSoundObjSimple1.field_0x1c;
        Z2_E_mm_modPitch(handle, var_r24);
    }
    }

    if (soundID == Z2SE_EN_ZAN_YO_V_KENKEN) {
        u32 var_r23 = mSoundObjSimple1.field_0x1c;
        Z2_B_zan_modPitch(handle, var_r23);
    }

    return handle;
}

Z2SoundHandlePool* Z2CreatureEnemy::startCreatureVoiceLevel(JAISoundID soundID, s8 reverb) {
    u32 mapinfo = 0;
    if (mEnemyID == Z2_ENEMY_RDY) {
        mapinfo = 1;
    }
    return mSoundObjSimple1.startLevelSound(soundID, mapinfo, reverb);
}

Z2SoundHandlePool* Z2CreatureEnemy::startCreatureExtraSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    return mSoundObjSimple2.startSound(soundID, mapinfo, reverb);
}

Z2SoundHandlePool* Z2CreatureEnemy::startCreatureExtraSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    return mSoundObjSimple2.startLevelSound(soundID, mapinfo, reverb);
}

Z2SoundHandlePool* Z2CreatureEnemy::startCollisionSE(u32 hitID, u32 mapinfo) {
    u8 var1;
    switch (mEnemyID) {
    case Z2_ENEMY_WS:
        field_0xa3 = 1;
        Z2GetSeqMgr()->setBattleLastHit(Z2Param::ENDING_BLOW_VOL_LOWER_TIME);
        return mSoundObjAnime.startCollisionSE(hitID, mapinfo, NULL);
    case Z2_ENEMY_BI:
        if (getSupervisor() != NULL) {
            Z2GetSoundObjMgr()->removeEnemy(this);
        }
        return mSoundObjAnime.startCollisionSE(hitID, mapinfo, NULL);
    default:
        switch (mapinfo) {
        case 0x1e:
        case 0x1f:
        case 0x22:
        case 0x23:
        case 0x25:
        case 0x28:
        case 0x29:
        case 0x2a:
        case 0x2d:
        case 0x34:
            Z2GetSeqMgr()->onEnemyDamage();
            break;
        case 0x20:
        case 0x24:
            field_0xa3 = 1;
            var1 = Z2Param::ENDING_BLOW_VOL_LOWER_TIME;
            if (hitID == Z2SE_HIT_SWORD || hitID == Z2SE_HIT_SWORD_STAB || hitID == Z2SE_HIT_WOLFBITE) {
                var1 = 20;
            }
            Z2GetSeqMgr()->setBattleLastHit(var1);
            break;
        case 0x21:
            if (mEnemyID == Z2_ENEMY_HB) {
                field_0xa3 = 1;
                Z2GetSeqMgr()->setBattleLastHit(Z2Param::ENDING_BLOW_VOL_LOWER_TIME);
            } else if (mEnemyID == Z2_ENEMY_DB) {
                Z2GetSeqMgr()->onEnemyDamage();
            }
            break;
        }

        return mSoundObjAnime.startCollisionSE(hitID, mapinfo, NULL);
    }
}

void Z2CreatureEnemy::setLinkSearch(bool isLinkSearch) {
    if (field_0xa3 == 0) {
        mLinkSearch = isLinkSearch;
    }
}

void Z2CreatureEnemy::setEnemyName(const char* enemyName) {
    if (enemyName == NULL) {
        field_0xa1 = -1;
    } else {
        mEnemyID = Z2GetSoundObjMgr()->getEnemyID(enemyName, this);
    }
}

Z2CreatureSumomo::Z2CreatureSumomo() {}

void Z2CreatureSumomo::playSumomoBgm(u32 dist) {
    if (Z2GetSeqMgr()->getSubBgmID() != Z2BGM_SUMOMO) {
        if (dist < 700) {
            Z2GetSeqMgr()->subBgmStart(Z2BGM_SUMOMO);
            OS_REPORT("SUMOMO_BGM_START dist:%d\n", dist);
        }
    } else if (dist >= 1000 && Z2GetStatusMgr()->getDemoStatus() != 6) {
        Z2GetSeqMgr()->subBgmStop();
        OS_REPORT("SUMOMO_BGM_STOP dist:%d\n", dist);
    }
}

void Z2CreatureSumomo::deleteObject() {
    if (Z2GetSeqMgr()->getSubBgmID() == Z2BGM_SUMOMO && Z2GetStatusMgr()->getDemoStatus() != 6) {
        Z2GetSeqMgr()->subBgmStop();
        OS_REPORT("SUMOMO_BGM_STOP (DELETE)\n");
    }

    Z2Creature::deleteObject();
}

Z2SoundObjBeeGroup::Z2SoundObjBeeGroup() {}

void Z2SoundObjBeeGroup::init(Vec* posPtr, u8 handleNum) {
    Z2SoundObjBase::init(posPtr, handleNum);
}

void Z2SoundObjBeeGroup::playBeeGroupSound(JAISoundID soundID, u8 param_1) {
    if (param_1 == 0) {
        return;
    }

    JAISoundID sound_id2;

    switch ((u32)soundID) {
    case Z2SE_EN_BE_STAY:
        sound_id2 = Z2SE_EN_BE_STAY_GROUP;
        break;
    case Z2SE_EN_BE_FLY:
        sound_id2 = Z2SE_EN_BE_FLY_GROUP;
        break;
    case Z2SE_EN_BE_FLY_D:
        sound_id2 = Z2SE_EN_BE_FLY_GROUP_D;
        break;
    case Z2SE_EN_BUG_WALK_GRD:
        sound_id2 = Z2SE_EN_BUG_WALK_GRD_GRP;
        break;
    case Z2SE_EN_BUG_WALK_BODY:
        sound_id2 = Z2SE_EN_BUG_WALK_BODY_GRP;
        break;
    }

    Z2SoundHandlePool* handle1 = startLevelSound((u32)soundID, 0, -1);
    Z2SoundHandlePool* handle2 = startLevelSound(sound_id2, 0, -1);

    if (handle1 != NULL && *handle1 != NULL) {
        f32 volume = Z2Calc::linearTransform(param_1, 1.0f, 5.0f, 0.5f, 1.0f, false);
        (*handle1)->getAuxiliary().moveVolume(volume, 0);
    }
    if (handle2 != NULL && *handle2 != NULL) {
        f32 volume = Z2Calc::linearTransform(param_1, 1.0f, 20.0f, 0.1f, 1.0f, false);
        (*handle2)->getAuxiliary().moveVolume(volume, 0);
    }
}

Z2CreatureFM::Z2CreatureFM() {}

void Z2CreatureFM::deleteObject() {
    Z2CreatureEnemy::deleteObject();
    field_0xa4.deleteObject();
    field_0xc4.deleteObject();
    field_0xe4.deleteObject();
    field_0x104.deleteObject();
}

void Z2CreatureFM::init(Vec* animePosPtr, Vec* simplePosPtr, Vec* param_2, Vec* param_3, Vec* param_4,
                        Vec* param_5, u8 animeHandleNum, u8 simpleHandleNum, u8 param_8, u8 param_9,
                        u8 param_10, u8 param_11) {
    Z2Creature::init(animePosPtr, simplePosPtr, animeHandleNum, simpleHandleNum);
    if (param_2 != NULL) {
        field_0xa4.init(param_2, param_8);
    }
    if (param_3 != NULL) {
        field_0xc4.init(param_3, param_9);
    }
    if (param_4 != NULL) {
        field_0xe4.init(param_4, param_10);
    }
    if (param_5 != NULL) {
        field_0x104.init(param_5, param_11);
    }
}

void Z2CreatureFM::framework(u32 mapinfo, s8 reverb) {
    Z2Creature::framework(mapinfo, reverb);
    field_0xa4.framework(mapinfo, reverb);
    field_0xc4.framework(mapinfo, reverb);
    field_0xe4.framework(mapinfo, reverb);
    field_0x104.framework(mapinfo, reverb);
}

Z2SoundHandlePool* Z2CreatureFM::startChainSound(JAISoundID soundID, u8 param_1, f32 param_2,
                                                 u32 mapinfo, s8 reverb) {
    switch (param_1) {
    case 0:
        return field_0xa4.startSound(soundID, mapinfo, reverb);
    case 1:
        return field_0xc4.startSound(soundID, mapinfo, reverb);
    case 2:
        return field_0xe4.startSound(soundID, mapinfo, reverb);
    case 3:
        return field_0x104.startSound(soundID, mapinfo, reverb);
    default:
        return NULL;
    }
}

Z2SoundHandlePool* Z2CreatureFM::startChainSoundLevel(JAISoundID soundID, u8 param_1, f32 param_2,
                                                      u32 mapinfo, s8 reverb) {
    Z2SoundHandlePool* handle;
    f32 pitch;
    switch (param_1) {
    case 0:
        handle = field_0xa4.startLevelSound(soundID, mapinfo, reverb);
        if (handle != NULL && *handle) {
            if (param_2 > 30.0f) {
                pitch = 1.06f;
            } else {
                pitch = (param_2 * 5.0f) / 100.0f;
            }
            pitch += Z2Calc::getRandom(0.03f, 1.0f, 0.5f) + 0.7f;
            if (pitch > 1.06f) {
                pitch = 1.06f;
            }
            (*handle)->getAuxiliary().movePitch(pitch, 3);
            return handle;
        }
        return NULL;
    case 1:
        handle = field_0xa4.startLevelSound(soundID, mapinfo, reverb);
        if (handle != NULL && *handle) {
            if (param_2 > 30.0f) {
                pitch = 1.06f;
            } else {
                pitch = (param_2 * 5.0f) / 100.0f;
            }
            pitch += Z2Calc::getRandom(0.03f, 1.0f, 0.5f) + 0.5f;
            if (pitch > 1.06f) {
                pitch = 1.06f;
            }
            (*handle)->getAuxiliary().movePitch(pitch, 3);
            return handle;
        }
        return NULL;
    case 2:
        return field_0xe4.startLevelSound(soundID, mapinfo, reverb);
    case 3:
        return field_0x104.startLevelSound(soundID, mapinfo, reverb);
    default:
        return NULL;
    }
}

void Z2CreatureGob::init(Vec* animePosPtr, Vec* simplePosPtr, Vec* simple2PosPtr, u8 animeHandleNum, u8 simpleHandleNum,
                         u8 simple2HandleNum) {
    Z2Creature::init(animePosPtr, simplePosPtr, simple2PosPtr, animeHandleNum, simpleHandleNum, simple2HandleNum);
}

Z2SoundHandlePool* Z2CreatureGob::startCreatureVoiceLevel(JAISoundID soundID, s8 reverb) {
    if (mSoundObjSimple1.getHandleSoundID(Z2SE_EN_GOB_V_DAMAGE) != NULL ||
        mSoundObjSimple1.getHandleSoundID(Z2SE_EN_GOB_V_STOMACH) != NULL)
    {
        return NULL;
    }

    return mSoundObjSimple1.startLevelSound(soundID, 0, reverb);
}

Z2CreatureOI::Z2CreatureOI() {}

void Z2CreatureOI::deleteObject() {
    Z2CreatureEnemy::deleteObject();
    field_0xa4.deleteObject();
    field_0xc4.deleteObject();
    field_0xe4.deleteObject();
    mTentacleSounds[0].deleteObject();
    mTentacleSounds[1].deleteObject();
    mTentacleSounds[2].deleteObject();
    mTentacleSounds[3].deleteObject();
    mTentacleSounds[4].deleteObject();
    mTentacleSounds[5].deleteObject();
    mTentacleSounds[6].deleteObject();
    mTentacleSounds[7].deleteObject();
}

void Z2CreatureOI::init(Vec* param_0, Vec* param_1, Vec* param_2, Vec* param_3, Vec* param_4,
                        Vec* param_5, Vec* param_6, Vec* param_7, Vec* param_8, Vec* param_9,
                        Vec* param_10, Vec* param_11, Vec* param_12) {
    Z2Creature::init(param_0, param_1, 1, 1);
    if (param_2 != NULL) {
        field_0xa4.init(param_2, 1);
    }
    if (param_3 != NULL) {
        field_0xc4.init(param_3, 1);
    }
    if (param_4 != NULL) {
        field_0xe4.init(param_4, 1);
    }
    if (param_5 != NULL) {
        mTentacleSounds[0].init(param_5, 1);
    }
    if (param_6 != NULL) {
        mTentacleSounds[1].init(param_6, 1);
    }
    if (param_7 != NULL) {
        mTentacleSounds[2].init(param_7, 1);
    }
    if (param_8 != NULL) {
        mTentacleSounds[3].init(param_8, 1);
    }
    if (param_9 != NULL) {
        mTentacleSounds[4].init(param_9, 1);
    }
    if (param_10 != NULL) {
        mTentacleSounds[5].init(param_10, 1);
    }
    if (param_11 != NULL) {
        mTentacleSounds[6].init(param_11, 1);
    }
    if (param_12 != NULL) {
        mTentacleSounds[7].init(param_12, 1);
    }
}

void Z2CreatureOI::framework(u32 mapinfo, s8 reverb) {
    Z2Creature::framework(mapinfo, reverb);
    field_0xa4.framework(mapinfo, reverb);
    field_0xc4.framework(mapinfo, reverb);
    field_0xe4.framework(mapinfo, reverb);
    mTentacleSounds[0].framework(mapinfo, reverb);
    mTentacleSounds[1].framework(mapinfo, reverb);
    mTentacleSounds[2].framework(mapinfo, reverb);
    mTentacleSounds[3].framework(mapinfo, reverb);
    mTentacleSounds[4].framework(mapinfo, reverb);
    mTentacleSounds[5].framework(mapinfo, reverb);
    mTentacleSounds[6].framework(mapinfo, reverb);
    mTentacleSounds[7].framework(mapinfo, reverb);
}

Z2SoundHandlePool* Z2CreatureOI::startCreatureSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    if (soundID == Z2SE_EN_OI_MOVE) {
        Z2SoundHandlePool* handle1 = mSoundObjAnime.startLevelSound(soundID, mapinfo, reverb);
        Z2SoundHandlePool* handle2 = field_0xa4.startLevelSound(soundID, mapinfo, reverb);
        Z2SoundHandlePool* handle3 = field_0xc4.startLevelSound(soundID, mapinfo, reverb);
        Z2SoundHandlePool* handle4 = field_0xe4.startLevelSound(soundID, mapinfo, reverb);

        f32 volume = Z2Calc::getParamByExp(mapinfo, 0.0f, 42.0f, 0.4f, 0.0f, 1.0f,
                                           Z2Calc::CURVE_POSITIVE);
        f32 pitch = Z2Calc::getParamByExp(mapinfo, 0.0f, 42.0f, 0.4f, 0.9f, 1.1f,
                                          Z2Calc::CURVE_POSITIVE);
        
        if (handle1 != NULL && *handle1 != NULL) {
            (*handle1)->getAuxiliary().moveVolume(volume, 0);
            (*handle1)->getAuxiliary().movePitch(pitch, 0);
        } else {
            return NULL;
        }

        if (handle2 != NULL && *handle2 != NULL) {
            (*handle2)->getAuxiliary().moveVolume(volume, 0);
            (*handle2)->getAuxiliary().movePitch(pitch, 0);
        } else {
            return NULL;
        }

        if (handle3 != NULL && *handle3 != NULL) {
            (*handle3)->getAuxiliary().moveVolume(volume, 0);
            (*handle3)->getAuxiliary().movePitch(pitch, 0);
        } else {
            return NULL;
        }

        if (handle4 != NULL && *handle4 != NULL) {
            (*handle4)->getAuxiliary().moveVolume(volume, 0);
            (*handle4)->getAuxiliary().movePitch(pitch, 0);
            return handle4;
        } else {
            return NULL;
        }
    } else {
        return mSoundObjAnime.startLevelSound(soundID, mapinfo, reverb);
    }
}

Z2SoundHandlePool* Z2CreatureOI::startTentacleSound(JAISoundID soundID, u8 tentacleIndex, u32 mapinfo, s8 reverb) {
    return mTentacleSounds[tentacleIndex].startSound(soundID, mapinfo, reverb);
}

Z2SoundHandlePool* Z2CreatureOI::startTentacleSoundLevel(JAISoundID soundID, u8 tentacleIndex,
                                                         f32 param_2, u32 mapinfo, s8 reverb) {
    Z2SoundHandlePool* handle = NULL;
    handle = mTentacleSounds[tentacleIndex].startLevelSound(soundID, mapinfo, reverb);
    if (handle != NULL && *handle) {
        f32 volume = Z2Calc::getParamByExp(param_2, 0.0f, 60.0f, 0.4f, 0.0f, 1.0f,
                                           Z2Calc::CURVE_POSITIVE);
        f32 pitch = Z2Calc::getParamByExp(param_2, 0.0f, 60.0f, 0.4f, 0.9f, 1.1f,
                                          Z2Calc::CURVE_POSITIVE);
        (*handle)->getAuxiliary().moveVolume(volume, 0);
        (*handle)->getAuxiliary().movePitch(pitch, 0);
        return handle;
    } else {
        return NULL;
    }
}

static void Z2_E_sw_modPitch(Z2SoundHandlePool* handle, u32 mapinfo) {
    if (handle != NULL && *handle != NULL) {
        f32 pitch = 1.0f;
        switch (mapinfo) {
        case 1:
            pitch = 1.1f;
            break;
        case 3:
            pitch = 0.8f;
            break;
        }
        (*handle)->getAuxiliary().movePitch(pitch, 0);
    }
}

static void Z2_E_ms_modVol(Z2SoundHandlePool* handle, u32 mapinfo) {
    if (handle != NULL && *handle != NULL) {
        f32 var_f31 = 0.2f;
        if (mapinfo == 1) {
            (*handle)->getAuxiliary().moveVolume(var_f31, 0);
        }
    }
}

static void Z2_E_mm_modPitch(Z2SoundHandlePool* handle, u32 mapinfo) {
    if (handle != NULL && *handle != NULL) {
        f32 var_f31 = 0.7f;
        if (mapinfo == 3) {
            (*handle)->getAuxiliary().movePitch(var_f31, 0);
        }
    }
}

static void Z2_B_zan_modPitch(Z2SoundHandlePool* handle, u32 mapinfo) {
    if (handle != NULL && *handle != NULL) {
        f32 pitch = 1.0f;
        f32 volume = 1.0f;
        if (mapinfo > 400) {
            pitch = 0.5f;
        } else if (mapinfo > 100) {
            pitch = Z2Calc::getParamByExp(400 - mapinfo, 0.0f, 300.0f, 0.4f, 0.5f, 1.0f,
                                          Z2Calc::CURVE_NEGATIVE);
        } else {
            pitch = Z2Calc::getParamByExp(100 - mapinfo, 0.0f, 50.0f, 0.4f, 1.0f, 1.5f,
                                          Z2Calc::CURVE_NEGATIVE);
            volume = Z2Calc::getParamByExp(mapinfo, 50.0f, 100.0f, 0.4f, 0.4f, 1.0f,
                                           Z2Calc::CURVE_NEGATIVE);
        }
        (*handle)->getAuxiliary().movePitch(pitch, 0);
        (*handle)->getAuxiliary().moveVolume(volume, 0);
    }
}
