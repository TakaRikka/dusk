#ifndef Z2SEQMGR_H
#define Z2SEQMGR_H

#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JAudio2/JASGadget.h"

struct Z2SoundFader {
    void move(f32 value, u32 moveTime) {
        if (moveTime != 0) {
            transition_.set(value, intensity_, moveTime);
        } else {
            intensity_ = value;
            transition_.zero();
        }
    }

    void forceIn() {
        intensity_ = 1.0f;
        transition_.zero();
    }

    void forceOut() {
        intensity_ = 0.0f;
        transition_.zero();
    }

    void fadeIn(u32 fadeTime) {
        if (fadeTime != 0) {
            transition_.set(1.0f, intensity_, fadeTime);
        } else {
            forceIn();
        }
    }

    void fadeOut(u32 fadeTime) {
        if (fadeTime != 0) {
            transition_.set(0.0f, intensity_, fadeTime);
        } else {
            forceOut();
        }
    }

    void fadeInFromOut(u32 fadeTime) {
        intensity_ = 0.0f;
        fadeIn(fadeTime);
    }

    u32 getCount() {
        return transition_.remainingSteps_;
    }

    f32 getIntensity() const {
        return intensity_;
    }

    f32 getDest() {
        if (getCount() != 0) {
            return transition_.targetValue_;
        } else {
            return intensity_;
        }
    }

    void calc() {
        intensity_ = transition_.apply(intensity_);
    }

    f32 get() {
        return getIntensity();
    }

    /* 0x0 */ f32 intensity_;
    /* 0x4 */ JAISoundParamsTransition::TTransition transition_;
};  // Size = 0x10

class Z2SeqMgr : public JASGlobalInstance<Z2SeqMgr> {
public:
    Z2SeqMgr();

    void bgmStart(u32 bgmID, u32 fadeTime, s32);
    void bgmStop(u32 fadeTime, s32);
    void subBgmStart(u32 bgmID);
    void subBgmStop();
    void subBgmStopInner();
    void bgmStreamPrepare(u32 bgmID);
    bool bgmStreamCheckReady();
    void bgmStreamPlay();
    void bgmStreamStop(u32 fadeTime);
    void changeBgmStatus(s32 status);
    void changeSubBgmStatus(s32 status);
    void onVariantBgmJumpEnd(bool);
    void changeFishingBgm(s32);
    void talkInBgm();
    void talkOutBgm();
    void menuInBgm();
    void menuOutBgm();
    void fanfareFramework();
    void stopWolfHowlSong();
    void setHeightVolMod(bool isVolMod, u32 fadeTime);
    void setTimeProcVolMod(bool isVolMod, u32 fadeTime);
    void processBgmFramework();
    bool checkBgmIDPlaying(u32 bgmID);
    f32 getChildTrackVolume(JAISoundHandle* handle, int trackId);
    void setChildTrackVolume(JAISoundHandle* handle, int trackId, f32 volume,
                             u32 moveTime, f32 pan, f32 dolby);
    void resetBattleBgmParams();
    void setBattleBgmOff(bool isBgmOff);
    void setBattleSearched(bool isBattleSearched);
    void setBattleDistIgnore(bool isBattleDistIgnore);
    void setBattleGhostMute(bool isBattleGhostMute);
    void setBattleDistState(u8 state);
    void setBattleSeqState(u8 state);
    void setBattleLastHit(u8 lastHit);
    void battleBgmFramework();
    void startBattleBgm(bool isFadeIn);
    void stopBattleBgm(u8 subFadeoutTime, u8 mainFadeinTime);
    void fieldBgmStart();
    void fieldRidingMute();
    void onFieldBgmJumpStart();
    void onFieldBgmJumpEnd();
    void fieldBgmFramework();
    void mbossBgmMuteProcess();
    void bgmSetSwordUsing(s32);
    void bgmNowBattle(f32);
    void taktModeMute();
    void taktModeMuteOff();

    void setFieldBgmPlay(bool value) { mFlags.mFieldBgmPlay = value; }

    void unMuteSceneBgm(u32 fadeTime) {
        mBgmPause.move(1.0f, 0);
        mSceneBgm.move(1.0f, fadeTime);
    }

    void muteSceneBgm(u32 fadeTime, f32 vol) { mSceneBgm.move(vol, fadeTime); }

    void setTwilightGateVol(f32 vol) {
        mTwilightGateVol = vol < 0.0f ? 0.0f : vol > 1.0f ? 1.0f : vol;
    }

    void setWindStoneVol(f32 vol, u32 fadeTime) { mWindStone.move(vol, fadeTime); };

    void onEnemyDamage() { setBattleSeqState(2); }

    void bgmAllMute(u32 fadeTime, f32 val) {
        mAllBgmMaster.move(val, fadeTime);
    }

    void bgmAllUnMute(u32 fadeTime) {
        mAllBgmMaster.move(1.0f, fadeTime);
    }

    bool isItemGetDemo() {
        return mFanfareHandle.isSoundAttached() || mFanfareCount != 0;
    }

    u32 getMainBgmID() {
        u32 id;
        if (mMainBgmHandle) {
            id = mMainBgmHandle->getID();
        } else {
            id = -1;
        }

        return id;
    }

    u32 getSubBgmID() {
        u32 id;
        if (mSubBgmHandle) {
            id = mSubBgmHandle->getID();
        } else {
            id = -1;
        }

        return id;
    }

    u32 getStreamBgmID() {
        u32 id;
        if (mStreamBgmHandle) {
            id = mStreamBgmHandle->getID();
        } else {
            id = -1;
        }

        return id;
    }

    bool checkBgmPlaying() {
        return mMainBgmHandle || mStreamBgmHandle;
    }

    u32 checkPlayingSubBgmFlag() {
        return getSubBgmID();
    }

    JAISoundHandle* getMainBgmHandle() { return &mMainBgmHandle; }
    JAISoundHandle* getSubBgmHandle() { return &mSubBgmHandle; }

    #if DEBUG
    f32 field_0x00_debug;
    u8 field_0x04_debug;
    #endif

    /* 0x00 */ JAISoundHandle mMainBgmHandle;
    /* 0x04 */ JAISoundHandle mSubBgmHandle;
    /* 0x08 */ JAISoundHandle mFanfareHandle;
    /* 0x0C */ JAISoundHandle mStreamBgmHandle;
    /* 0x10 */ JAISoundID mFanfareID;
    /* 0x14 */ Z2SoundFader mMainBgmMaster;
    /* 0x24 */ Z2SoundFader mSubBgmMaster;
    /* 0x34 */ Z2SoundFader mBgmPause;
    /* 0x44 */ Z2SoundFader mSceneBgm;
    /* 0x54 */ Z2SoundFader mFanfareMute;
    /* 0x64 */ Z2SoundFader mStreamBgmMaster;
    /* 0x74 */ Z2SoundFader mAllBgmMaster;
    /* 0x84 */ Z2SoundFader field_0x84;
    /* 0x94 */ Z2SoundFader mWindStone;
    /* 0xA4 */ Z2SoundFader field_0xa4;
    /* 0xB4 */ f32 mTwilightGateVol;
    /* 0xB8 */ s16 field_0xb8;
    /* 0xBA */ u8 mFanfareCount;
    /* 0xBB */ u8 mDekuToadCount;
    /* 0xBC */ u8 mBgmStatus;
    /* 0xBD */ u8 mSubBgmStatus;
    /* 0xBE */ u8 mBattleDistState;
    /* 0xBF */ u8 mBattleSeqState;
    /* 0xC0 */ u8 mBattleSeqCount;
    /* 0xC1 */ u8 field_0xc1;
    /* 0xC2 */ u8 mBattleLastHit;
    /* 0xC3 */ u8 mRideCount;
    /* 0xC4 */ u8 field_0xc4;
    /* 0xC8 */ f32 field_0xc8;
    /* 0xCC */ f32 field_0xcc;
    /* 0xD0 */ struct {
        bool mBattleDistIgnore : 1;
        bool flag1 : 1;
        bool mBattleSearched : 1;
        bool mBattleBgmOff : 1;
        bool mRiding : 1;
        bool mFieldBgmPlay : 1;
        bool mHeightVolMod : 1;
        bool mTimeProcVolMod : 1;
    } mFlags;
};  // Size = 0xD4

inline Z2SeqMgr* Z2GetSeqMgr() {
    return JASGlobalInstance<Z2SeqMgr>::getInstance();
}

enum Z2BgmID {
    Z2BGM_FIELD_LINK_DAY = 0x1000000,
    Z2BGM_FIELD_LINK_NIGHT = 0x1000001,
    Z2BGM_FIELD = 0x1000002,
    Z2BGM_DUNGEON = 0x1000003,
    Z2BGM_HORSE_BATTLE = 0x1000004,
    Z2BGM_TOAL_VILLEGE = 0x1000005,
    Z2BGM_COWBOY_GAME = 0x1000006,
    Z2BGM_FACE_OFF_BATTLE = 0x1000007,
    Z2BGM_BOOMERAMG_MONKEY = 0x1000008,
    Z2BGM_DUNGEON_FOREST = 0x1000009,
    Z2BGM_ITEM_GET = 0x100000A,
    Z2BGM_ITEM_GET_MINI = 0x100000B,
    Z2BGM_BOSSBABA_0 = 0x100000C,
    Z2BGM_BOSSBABA_1 = 0x100000D,
    Z2BGM_BOSSBABA_2 = 0x100000E,
    Z2BGM_BATTLE_NORMAL = 0x100000F,
    Z2BGM_RANCH = 0x1000010,
    Z2BGM_WILD_GOAT = 0x1000011,
    Z2BGM_OPEN_BOX = 0x1000012,
    Z2BGM_GAME_OVER = 0x1000013,
    Z2BGM_ITEM_GET_ME = 0x1000014,
    Z2BGM_HORSE_BATTLE_D02 = 0x1000015,
    Z2BGM_TWILIGHT = 0x1000016,
    Z2BGM_EVENT01 = 0x1000017,
    Z2BGM_DUNGEON_LV2 = 0x1000018,
    Z2BGM_TOAL_NIGHT = 0x1000019,
    Z2BGM_DEATH_MOUNTAIN01 = 0x100001A,
    Z2BGM_BATTLE_TWILIGHT = 0x100001B,
    Z2BGM_HEART_GET = 0x100001C,
    Z2BGM_MAGNE_GORON = 0x100001D,
    Z2BGM_SHOP01 = 0x100001E,
    Z2BGM_SHOP02 = 0x100001F,
    Z2BGM_DUNGEON_LV3 = 0x1000020,
    Z2BGM_MAGNE_GORON_D01 = 0x1000021,
    Z2BGM_MAGNE_GORON_D02 = 0x1000022,
    Z2BGM_FILONE_FOREST = 0x1000023,
    Z2BGM_SUMO = 0x1000024,
    Z2BGM_BOSSFIREMAN_0 = 0x1000025,
    Z2BGM_KAKARIKO = 0x1000026,
    Z2BGM_DEKUTOAD = 0x1000027,
    Z2BGM_DEKUTOAD_D01 = 0x1000028,
    Z2BGM_RODEO = 0x1000029,
    Z2BGM_BOSSFIREMAN_1 = 0x100002A,
    Z2BGM_SUMO_D1 = 0x100002B,
    Z2BGM_INDOOR = 0x100002C,
    Z2BGM_DUNGEON_LV4 = 0x100002D,
    Z2BGM_STATUE_GAME = 0x100002E,
    Z2BGM_EVENT02 = 0x100002F,
    Z2BGM_BOSS_OCTAEEL_0 = 0x1000030,
    Z2BGM_BOSS_OCTAEEL_1 = 0x1000031,
    Z2BGM_BOSS_OCTAEEL_D01 = 0x1000032,
    Z2BGM_BOSS_OCTAEEL_D02 = 0x1000033,
    Z2BGM_DESERT = 0x1000034,
    Z2BGM_ZORA_VILLAGE = 0x1000035,
    Z2BGM_BRIDGE_ON_FIRE = 0x1000036,
    Z2BGM_DUNGEON_LV5 = 0x1000037,
    Z2BGM_DEMO10 = 0x1000038,
    Z2BGM_VARIANT = 0x1000039,
    Z2BGM_CARGO_GAME_TW = 0x100003A,
    Z2BGM_CASTLE_TOWN = 0x100003B,
    Z2BGM_DUNGEON_LV6 = 0x100003C,
    Z2BGM_DUNGEON_LV7 = 0x100003D,
    Z2BGM_DUNGEON_LV8 = 0x100003E,
    Z2BGM_DUNGEON_LV9_02 = 0x100003F,
    Z2BGM_DEATH_MOUNTAIN02 = 0x1000040,
    Z2BGM_HOLY_FOREST = 0x1000041,
    Z2BGM_FISHING = 0x1000042,
    Z2BGM_FISHING_BARE = 0x1000043,
    Z2BGM_FISHING_GET1 = 0x1000044,
    Z2BGM_FISHING_GET2 = 0x1000045,
    Z2BGM_FISHING_GET3 = 0x1000046,
    Z2BGM_LUTERA1 = 0x1000047,
    Z2BGM_LUTERA2 = 0x1000048,
    Z2BGM_FISHING_HIT = 0x1000049,
    Z2BGM_SNOW_MOUNTAIN = 0x100004A,
    Z2BGM_SNOW_BOARD = 0x100004B,
    Z2BGM_BOSS_SNOWWOMAN_0 = 0x100004C,
    Z2BGM_BOSS_SNOWWOMAN_1 = 0x100004D,
    Z2BGM_DEMO08 = 0x100004E,
    Z2BGM_DEMO13_SEQ = 0x100004F,
    Z2BGM_CHURCH1 = 0x1000050,
    Z2BGM_CHURCH2 = 0x1000051,
    Z2BGM_HOWL_TOBIKUSA = 0x1000052,
    Z2BGM_HOWL_UMAKUSA = 0x1000053,
    Z2BGM_HOWL_ZELDASONG = 0x1000054,
    Z2BGM_HOWL_LIGHT_PRLD = 0x1000055,
    Z2BGM_HIDDEN_VIL_D1 = 0x1000056,
    Z2BGM_HIDDEN_VILLAGE = 0x1000057,
    Z2BGM_SHOP_CELEB = 0x1000058,
    Z2BGM_SHOP_MARO = 0x1000059,
    Z2BGM_LAKE = 0x100005A,
    Z2BGM_STONE_ZELDASONG = 0x100005B,
    Z2BGM_STONE_LIGHT_PRLD = 0x100005C,
    Z2BGM_BOSS_SNOWWOMAN_D1 = 0x100005D,
    Z2BGM_ROOM_2 = 0x100005E,
    Z2BGM_MIDNA_SOS = 0x100005F,
    Z2BGM_FORTUNE = 0x1000060,
    Z2BGM_IB_MBOSS = 0x1000061,
    Z2BGM_BOSS_ZANT = 0x1000062,
    Z2BGM_IB_MBOSS_D01 = 0x1000063,
    Z2BGM_SUMOMO = 0x1000064,
    Z2BGM_RAKKA_HOUSE = 0x1000065,
    Z2BGM_LIGHT_PRLD_DUO = 0x1000066,
    Z2BGM_SOUL_REQ_STONE = 0x1000067,
    Z2BGM_SOUL_REQ_HOWL = 0x1000068,
    Z2BGM_SOUL_REQ_DUO = 0x1000069,
    Z2BGM_LUTERA_DEMO = 0x100006A,
    Z2BGM_TEMPLE_OF_TIME = 0x100006B,
    Z2BGM_TN_MBOSS = 0x100006C,
    Z2BGM_OBACHAN = 0x100006D,
    Z2BGM_RIVER_GAME = 0x100006E,
    Z2BGM_GG_MBOSS = 0x100006F,
    Z2BGM_OUGI_TRAINING = 0x1000070,
    Z2BGM_HEALING_STONE = 0x1000071,
    Z2BGM_HEALING_HOWL = 0x1000072,
    Z2BGM_HEALING_DUO = 0x1000073,
    Z2BGM_NEW_01_STONE = 0x1000074,
    Z2BGM_NEW_01_HOWL = 0x1000075,
    Z2BGM_NEW_01_DUO = 0x1000076,
    Z2BGM_SUB_DUNGEON = 0x1000077,
    Z2BGM_WCS_GAME = 0x1000078,
    Z2BGM_WCS_D01 = 0x1000079,
    Z2BGM_MINIGAME_ROOM = 0x100007A,
    Z2BGM_NEW_02_STONE = 0x100007B,
    Z2BGM_NEW_02_HOWL = 0x100007C,
    Z2BGM_NEW_02_DUO = 0x100007D,
    Z2BGM_NEW_03_STONE = 0x100007E,
    Z2BGM_NEW_03_HOWL = 0x100007F,
    Z2BGM_NEW_03_DUO = 0x1000080,
    Z2BGM_ITEM_GET_INSECT = 0x1000081,
    Z2BGM_ITEM_GET_SMELL = 0x1000082,
    Z2BGM_ITEM_GET_POU = 0x1000083,
    Z2BGM_GG_MBOSS_D01 = 0x1000084,
    Z2BGM_P_ZANT = 0x1000085,
    Z2BGM_VS_GANON_01 = 0x1000086,
    Z2BGM_VS_GANON_02 = 0x1000087,
    Z2BGM_VS_GANON_04 = 0x1000088,
    Z2BGM_HARAGIGANT_D01 = 0x1000089,
    Z2BGM_HARAGIGANT_D02 = 0x100008A,
    Z2BGM_HARAGIGANT_BTL01 = 0x100008B,
    Z2BGM_HARAGIGANT_BTL02 = 0x100008C,
    Z2BGM_DRAGON_D01 = 0x100008D,
    Z2BGM_DRAGON_D02 = 0x100008E,
    Z2BGM_DRAGON_BTL01 = 0x100008F,
    Z2BGM_DRAGON_BTL02 = 0x1000090,
    Z2BGM_KOROKORO_GAME = 0x1000091,
    Z2BGM_YAMIMUSHI_B_D01 = 0x1000092,
    Z2BGM_GOMA_D01 = 0x1000093,
    Z2BGM_GOMA_BTL01 = 0x1000094,
    Z2BGM_GOMA_BTL02 = 0x1000095,
    Z2BGM_FACE_OFF_BATTLE2 = 0x1000096,
    Z2BGM_FACE_OFF_BATTLE3 = 0x1000097,
    Z2BGM_SNOWBOARD_WIN = 0x1000098,
    Z2BGM_SNOWBOARD_LOSE = 0x1000099,
    Z2BGM_INDOOR_03 = 0x100009A,
    Z2BGM_INDOOR_04 = 0x100009B,
    Z2BGM_MINIGAME_WIN01 = 0x100009C,
    Z2BGM_MINIGAME_WIN02 = 0x100009D,
    Z2BGM_POSTMAN = 0x100009E,
    Z2BGM_EVENT05 = 0x100009F,
    Z2BGM_ITEM_GET_ME_S = 0x10000A0,
    Z2BGM_D30_01_NEXT_REQ = 0x10000A1,
    Z2BGM_D30_02_NEXT_REQ = 0x10000A2,
    Z2BGM_TARO_RESCUE = 0x10000A3,
    Z2BGM_KOMONJO_GET_INTRO = 0x10000A4,
    Z2BGM_RIVER_GAME_00 = 0x10000A5,
    Z2BGM_ZORA_D01 = 0x10000A6,
    Z2BGM_SUMO_D02 = 0x10000A7,
    Z2BGM_TN_MBOSS_LV9 = 0x10000A8,
    Z2BGM_MENU_SELECT = 0x10000A9,
    Z2BGM_TEMP = 0x10000AA,
    Z2BGM_TEMP_O = 0x10000AB
};

#endif /* Z2SEQMGR_H */
