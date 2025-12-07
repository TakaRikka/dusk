#include "Z2AudioLib/Z2SeqMgr.h"
#include "Z2AudioLib/Z2LinkMgr.h"
#include "Z2AudioLib/Z2Param.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "Z2AudioLib/Z2SoundMgr.h"
#include "Z2AudioLib/Z2SoundObjMgr.h"
#include "Z2AudioLib/Z2StatusMgr.h"
#include "Z2AudioLib/Z2Calc.h"
#include "JSystem/JAudio2/JAISoundChild.h"
#include "JSystem/JAudio2/JAISeq.h"

static const char* sSpotName[] = {
    "F_SP00",
    "F_SP103",
    "R_SP01",
    "F_SP104",
    "R_SP107",
    "F_SP108",
    "R_SP108",
    "F_SP117",
    "F_SP109",
    "R_SP109",
    "R_SP209",
    "F_SP110",
    "R_SP110",
    "F_SP111",
    "F_SP128",
    "R_SP128",
    "F_SP115",
    "F_SP112",
    "F_SP126",
    "F_SP127",
    "R_SP127",
    "F_SP113",
    "F_SP116",
    "R_SP116",
    "R_SP160",
    "R_SP161",
    "F_SP114",
    "F_SP118",
    "F_SP124",
    "F_SP125",
    "F_SP121",
    "F_SP122",
    "F_SP123",
    "F_SP200",
    "F_SP102",
    "",
    "R_SP300",
    "R_SP301",
    "T_ENEMY",
    "D_MN54",
    "D_MN05",
    "D_MN05B",
    "D_MN05A",
    "D_MN04",
    "D_MN04B",
    "D_MN04A",
    "D_MN01",
    "D_MN01B",
    "D_MN01A",
    "D_MN10",
    "D_MN10B",
    "D_MN10A",
    "D_MN11",
    "D_MN11B",
    "D_MN11A",
    "D_MN06",
    "D_MN06B",
    "D_MN06A",
    "D_MN07",
    "D_MN07B",
    "D_MN07A",
    "D_MN08",
    "D_MN08B",
    "D_MN08C",
    "D_MN08A",
    "D_MN08D",
    "D_MN09",
    "D_MN09A",
    "D_MN09B",
    "D_MN09C",
    "D_SB00",
    "D_SB01",
    "D_SB02",
    "D_SB03",
    "D_SB04",
    "D_SB05",
    "D_SB06",
    "D_SB07",
    "D_SB08",
    "D_SB09",
    "D_SB10",
};

Z2SeqMgr::Z2SeqMgr() : JASGlobalInstance<Z2SeqMgr>(true) {
    mMainBgmMaster.forceIn();
    mSubBgmMaster.forceIn();
    mBgmPause.forceIn();
    mSceneBgm.forceIn();
    mFanfareMute.forceIn();
    mStreamBgmMaster.forceIn();
    field_0x84.forceIn();
    mWindStone.forceIn();
    mTwilightGateVol = 1.0f;
    mAllBgmMaster.forceIn();
    field_0xa4.forceIn();

    #if DEBUG
    field_0x00_debug = 1.0f;
    field_0x04_debug = 0;
    #endif

    field_0xb8 = -1;
    resetBattleBgmParams();
    mBgmStatus = 0xff;
    mSubBgmStatus = 0xff;
    mFanfareCount = 0;
    mDekuToadCount = 0;
    mFanfareID.setAnonymous();
    field_0xc4 = 0;
    mFlags.mRiding = false;
    field_0xc8 = 1.0f;
    field_0xcc = 1.0f;
    mRideCount = 0;
    mFlags.mFieldBgmPlay = false;
    mFlags.mBattleBgmOff = true;
    mFlags.mHeightVolMod = false;
    mFlags.mTimeProcVolMod = false;
}

void Z2SeqMgr::bgmStart(u32 bgmID, u32 fadeTime, s32 param_2) {
    switch (bgmID) {
    case -1:
        return;
    case 0x200005D:
        bgmAllUnMute(33);
        bgmStreamPrepare(0x200005d);
        bgmStreamPlay();
        return;
    case Z2BGM_BOSSBABA_1:
        param_2 = 1;
        mMainBgmMaster.forceIn();
        break;
    case Z2BGM_GAME_OVER:
        if (getMainBgmID() != bgmID) {
            Z2GetSceneMgr()->setInGame(false);
            Z2GetSceneMgr()->setSceneExist(false);
            setBattleBgmOff(true);
            Z2GetSoundMgr()->getSeqMgr()->stop(0);
            Z2GetSoundMgr()->getStreamMgr()->stop(0);
            Z2GetSoundMgr()->startSound(bgmID, &mMainBgmHandle, NULL);
            mBgmStatus = 0xff;
        }
        return;
    case Z2BGM_LAKE:
        if (Z2GetLink() != NULL && Z2GetLink()->getCurrentPos() != NULL &&
            Z2GetLink()->getCurrentPos()->y > -10000.0f)
        {
            bgmID = Z2BGM_RAKKA_HOUSE;
        }
        break;
    case Z2BGM_VS_GANON_01:
        bgmAllUnMute(0);
        // fallthrough
    case Z2BGM_VS_GANON_02:
    case Z2BGM_VS_GANON_04:
    case Z2BGM_GOMA_BTL02:
        if (mStreamBgmHandle) {
            mStreamBgmHandle->stop(0);
        }
        mStreamBgmMaster.forceIn();
    }

    #if !PLATFORM_SHIELD
    if (Z2GetStatusMgr()->getDemoStatus() == 9) {
        if (mMainBgmHandle) {
            mMainBgmHandle->stop(0);
        }

        if (param_2 == 0) {
            if (mSubBgmHandle) {
                mSubBgmHandle->stop(0);
            }
            mMainBgmMaster.forceIn();
        }
    } else
    #endif
    {
        bgmStop(0, param_2);
    }

    if (bgmID == Z2BGM_FIELD_LINK_DAY || bgmID == Z2BGM_FIELD_LINK_NIGHT) {
        fieldBgmStart();
    } else {
        Z2GetSoundMgr()->startSound(bgmID, &mMainBgmHandle, NULL);
        mBgmStatus = 0xff;
    }

    if (param_2 == 0 && fadeTime != 0) {
        mMainBgmMaster.fadeInFromOut(fadeTime);
    }

    if (bgmID == Z2BGM_BOSS_SNOWWOMAN_1 || bgmID == Z2BGM_HORSE_BATTLE
        || bgmID == Z2BGM_VS_GANON_02 || bgmID == Z2BGM_TOAL_VILLEGE)
    {
        changeBgmStatus(0);
    }

    if (mFlags.mTimeProcVolMod && !Z2GetStatusMgr()->checkDayTime()) {
        field_0xa4.forceOut();
    } else {
        field_0xa4.forceIn();
    }

    #if DEBUG
    if (bgmID == Z2BGM_HIDDEN_VILLAGE) {
        OS_REPORT("[Z2SeqMgr::bgmStart] Z2BGM_HIDDEN_VILLAGE start(%d)\n", mMainBgmHandle.isSoundAttached());
        OS_REPORT("                        mainBgmMasterVol::%.2f \n", mMainBgmMaster.get());
        OS_REPORT("                        subBgmMasterVol::%.2f \n", mSubBgmMaster.get());
        OS_REPORT("                        allBgmMasterVol::%.2f \n", mAllBgmMaster.get());
        OS_REPORT("                        bgmPauseVol::%.2f \n", mBgmPause.get());
        OS_REPORT("                        fanfareMuteVol::%.2f \n", mFanfareMute.get());
        OS_REPORT("                        windStoneVol::%.2f \n", mWindStone.get());
        OS_REPORT("                        twilightGateVol::%.2f \n", mTwilightGateVol);
    }
    #endif
}

void Z2SeqMgr::bgmStop(u32 fadeTime, s32 param_1) {
    if (mMainBgmHandle) {
        mMainBgmHandle->stop(fadeTime);
    }

    if (mStreamBgmHandle) {
        mStreamBgmHandle->stop(fadeTime);
    }

    if (param_1 == 0) {
        if (mSubBgmHandle) {
            mSubBgmHandle->stop(fadeTime);
        }
        mMainBgmMaster.forceIn();
    }
}

void Z2SeqMgr::subBgmStart(u32 bgmID) {
    #if DEBUG
    const char** spotName = sSpotName;
    #endif

    if (bgmID == -1) {
        return;
    }

    int fadeinTime = 0;
    int fadeoutTime = 0;

    switch (bgmID) {
    case Z2BGM_ITEM_GET:
    case Z2BGM_ITEM_GET_MINI:
    case Z2BGM_OPEN_BOX:
    case Z2BGM_ITEM_GET_ME:
    case Z2BGM_HEART_GET:
    case Z2BGM_FISHING_BARE:
    case Z2BGM_FISHING_GET1:
    case Z2BGM_FISHING_GET2:
    case Z2BGM_FISHING_GET3:
    case Z2BGM_HOWL_TOBIKUSA:
    case Z2BGM_HOWL_UMAKUSA:
    case Z2BGM_HOWL_ZELDASONG:
    case Z2BGM_HOWL_LIGHT_PRLD:
    case Z2BGM_LIGHT_PRLD_DUO:
    case Z2BGM_SOUL_REQ_HOWL:
    case Z2BGM_SOUL_REQ_DUO:
    case Z2BGM_HEALING_HOWL:
    case Z2BGM_HEALING_DUO:
    case Z2BGM_NEW_01_HOWL:
    case Z2BGM_NEW_01_DUO:
    case Z2BGM_NEW_02_HOWL:
    case Z2BGM_NEW_02_DUO:
    case Z2BGM_NEW_03_HOWL:
    case Z2BGM_NEW_03_DUO:
    case Z2BGM_ITEM_GET_INSECT:
    case Z2BGM_ITEM_GET_SMELL:
    case Z2BGM_ITEM_GET_POU:
    case Z2BGM_ITEM_GET_ME_S:
    case Z2BGM_KOMONJO_GET_INTRO:
        if (getSubBgmID() == Z2BGM_OBACHAN) {
            subBgmStop();
        }
        mFanfareID = bgmID;
        return;
    case Z2BGM_FACE_OFF_BATTLE:
    case Z2BGM_BOSS_OCTAEEL_D02:
    case Z2BGM_DRAGON_D02:
    case Z2BGM_FACE_OFF_BATTLE2:
    case Z2BGM_FACE_OFF_BATTLE3:
        fadeinTime = 0;
        fadeoutTime = -1;
        break;
    case Z2BGM_LUTERA1:
    case Z2BGM_BOSS_SNOWWOMAN_D1:
        Z2GetStatusMgr()->setDemoName("force_start");
        fadeinTime = 0;
        fadeoutTime = Z2Param::BGM_CROSS_FADEOUT_TIME;
        break;
    case Z2BGM_TARO_RESCUE:
        fadeinTime = 0;
        fadeoutTime = 120;
        bgmStop(0, 0);
        break;
    case Z2BGM_OBACHAN:
    case Z2BGM_POSTMAN:
        if (mSubBgmHandle) {
            mSubBgmHandle->stop(0);
        }
        // fallthrough
    case Z2BGM_FISHING_HIT:
    case Z2BGM_SNOW_BOARD:
    case Z2BGM_RIVER_GAME:
    case Z2BGM_RIVER_GAME_00:
        setBattleBgmOff(true);
        // fallthrough
    case Z2BGM_BOSSBABA_0:
    case Z2BGM_WILD_GOAT:
    case Z2BGM_EVENT01:
    case Z2BGM_MAGNE_GORON:
    case Z2BGM_MAGNE_GORON_D02:
    case Z2BGM_SUMO:
    case Z2BGM_DEKUTOAD:
    case Z2BGM_RODEO:
    case Z2BGM_BOSSFIREMAN_1:
    case Z2BGM_SUMO_D1:
    case Z2BGM_STATUE_GAME:
    case Z2BGM_BOSS_OCTAEEL_D01:
    case Z2BGM_HIDDEN_VIL_D1:
    case Z2BGM_IB_MBOSS:
    case Z2BGM_GG_MBOSS:
    case Z2BGM_WCS_GAME:
    case Z2BGM_HARAGIGANT_D01:
    case Z2BGM_HARAGIGANT_D02:
    case Z2BGM_DRAGON_D01:
    case Z2BGM_KOROKORO_GAME:
    case Z2BGM_YAMIMUSHI_B_D01:
    case Z2BGM_GOMA_D01:
    case Z2BGM_MINIGAME_WIN01:
    case Z2BGM_MINIGAME_WIN02:
        fadeinTime = 0;
        fadeoutTime = 1;
        break;
    case Z2BGM_COWBOY_GAME:
        mAllBgmMaster.forceIn();
        fadeinTime = 0;
        fadeoutTime = 1;
        break;
    case Z2BGM_SNOWBOARD_WIN:
    case Z2BGM_SNOWBOARD_LOSE:
        if (mSubBgmHandle) {
            mSubBgmHandle->stop(0);
        }
        // fallthrough
    case Z2BGM_BOOMERAMG_MONKEY:
    case Z2BGM_VARIANT:
    case Z2BGM_TN_MBOSS:
    case Z2BGM_P_ZANT:
        if (mStreamBgmHandle) {
            mStreamBgmHandle->stop(0);
        }
        // fallthrough
    case Z2BGM_HORSE_BATTLE:
    case Z2BGM_TN_MBOSS_LV9:
        mStreamBgmMaster.forceIn();
        fadeinTime = 0;
        fadeoutTime = 1;
        break;
    case Z2BGM_MAGNE_GORON_D01:
    case Z2BGM_DEKUTOAD_D01:
        fadeinTime = 0;
        fadeoutTime = 20;
        break;
    case Z2BGM_IB_MBOSS_D01:
    case Z2BGM_GG_MBOSS_D01:
        fadeinTime = 0;
        fadeoutTime = Z2Param::BGM_CROSS_FADEOUT_TIME;
        break;
    default:
        fadeinTime = Z2Param::BGM_CROSS_FADEIN_TIME;
        fadeoutTime = Z2Param::BGM_CROSS_FADEOUT_TIME;
        break;
    }

    if (bgmID == getSubBgmID()) {
        if (bgmID == Z2BGM_SUMO || bgmID == Z2BGM_COWBOY_GAME) {
            mSubBgmHandle->stop(0);
            mSubBgmHandle.releaseSound();
        } else {
            field_0xb8 = -1;
            return;
        }
    } else if (mSubBgmHandle) {
        subBgmStop();
        subBgmStopInner();
    }

    Z2GetSoundMgr()->startSound(bgmID, &mSubBgmHandle, NULL);
    mSubBgmStatus = 0xff;

    switch (bgmID) {
    case Z2BGM_HORSE_BATTLE:
    case Z2BGM_FACE_OFF_BATTLE2:
        changeSubBgmStatus(0);
        break;
    case Z2BGM_VARIANT:
    case Z2BGM_TN_MBOSS:
        bgmStreamStop(0);
        // fallthrough
    case Z2BGM_TN_MBOSS_LV9:
        mStreamBgmMaster.forceIn();
        // fallthrough
    case Z2BGM_RODEO:
        changeSubBgmStatus(1);
        break;
    }

    if (fadeinTime != 0) {
        mSubBgmMaster.fadeInFromOut(fadeinTime);
    } else {
        mSubBgmMaster.forceIn();
    }

    if (fadeoutTime >= 0) {
        mMainBgmMaster.fadeOut(fadeoutTime);
    }

    field_0xb8 = -1;

    #if DEBUG
    if (bgmID == Z2BGM_HIDDEN_VIL_D1) {
        OS_REPORT("[Z2SeqMgr::subBgmStart] HIDDEN_VIL_D01 start(%d)\n", mMainBgmHandle.isSoundAttached());
        OS_REPORT("                        mainBgmMasterVol::%.2f \n", mMainBgmMaster.get());
        OS_REPORT("                        subBgmMasterVol::%.2f \n", mSubBgmMaster.get());
        OS_REPORT("                        allBgmMasterVol::%.2f \n", mAllBgmMaster.get());
        OS_REPORT("                        bgmPauseVol::%.2f \n", mBgmPause.get());
        OS_REPORT("                        fanfareMuteVol::%.2f \n", mFanfareMute.get());
        OS_REPORT("                        windStoneVol::%.2f \n", mWindStone.get());
        OS_REPORT("                        twilightGateVol::%.2f \n", mTwilightGateVol);
    }
    #endif
}

void Z2SeqMgr::subBgmStop() {
    switch (getSubBgmID()) {
    case Z2BGM_ITEM_GET:
    case Z2BGM_ITEM_GET_MINI:
    case Z2BGM_OPEN_BOX:
    case Z2BGM_ITEM_GET_ME:
    case Z2BGM_HEART_GET:
    case Z2BGM_FISHING_BARE:
    case Z2BGM_FISHING_GET1:
    case Z2BGM_FISHING_GET2:
    case Z2BGM_FISHING_GET3:
    case Z2BGM_ITEM_GET_INSECT:
    case Z2BGM_ITEM_GET_SMELL:
    case Z2BGM_ITEM_GET_POU:
    case Z2BGM_SNOWBOARD_WIN:
    case Z2BGM_SNOWBOARD_LOSE:
    case Z2BGM_MINIGAME_WIN01:
    case Z2BGM_MINIGAME_WIN02:
    case Z2BGM_ITEM_GET_ME_S:
    case Z2BGM_KOMONJO_GET_INTRO:
        return;
    case Z2BGM_LUTERA1:
    case Z2BGM_BOSS_SNOWWOMAN_D1:
        Z2GetStatusMgr()->setDemoName("force_end");
        field_0xb8 = 0;
        break;
    case Z2BGM_HIDDEN_VIL_D1:
        bgmStart(Z2BGM_HIDDEN_VILLAGE, 0, 0);
        // fallthrough
    case Z2BGM_BOSSBABA_0:
    case Z2BGM_MAGNE_GORON:
    case Z2BGM_MAGNE_GORON_D01:
    case Z2BGM_MAGNE_GORON_D02:
    case Z2BGM_DEKUTOAD:
    case Z2BGM_DEKUTOAD_D01:
    case Z2BGM_BOSSFIREMAN_1:
    case Z2BGM_SUMO_D1:
    case Z2BGM_BOSS_OCTAEEL_D01:
    case Z2BGM_BOSS_OCTAEEL_D02:
    case Z2BGM_VARIANT:
    case Z2BGM_DEATH_MOUNTAIN02:
    case Z2BGM_SNOW_BOARD:
    case Z2BGM_IB_MBOSS_D01:
    case Z2BGM_SUMOMO:
    case Z2BGM_RIVER_GAME:
    case Z2BGM_GG_MBOSS_D01:
    case Z2BGM_HARAGIGANT_D01:
    case Z2BGM_HARAGIGANT_D02:
    case Z2BGM_DRAGON_D01:
    case Z2BGM_DRAGON_D02:
    case Z2BGM_YAMIMUSHI_B_D01:
    case Z2BGM_GOMA_D01:
    case Z2BGM_FACE_OFF_BATTLE3:
    case Z2BGM_RIVER_GAME_00:
        field_0xb8 = 0;
        break;
    case Z2BGM_FISHING_HIT:
        setBattleBgmOff(false);
        mSubBgmHandle->stop(23);
        field_0xb8 = 55;
        break;
    case Z2BGM_IB_MBOSS:
        field_0xb8 = 420;
        mSubBgmHandle->stop(45);
        break;
    case Z2BGM_TN_MBOSS:
        field_0xb8 = 510;
        mSubBgmHandle->stop(45);
        break;
    case Z2BGM_TN_MBOSS_LV9:
        field_0xb8 = 360;
        mSubBgmHandle->stop(45);
        break;
    case Z2BGM_GG_MBOSS:
    case Z2BGM_P_ZANT:
        field_0xb8 = 420;
        mSubBgmHandle->stop(45);
        break;
    case Z2BGM_OBACHAN:
    case Z2BGM_POSTMAN:
        setBattleBgmOff(false);
        // fallthrough
    default:
        field_0xb8 = 15;
        break;
    }

    #if DEBUG
    if (getSubBgmID() == Z2BGM_HIDDEN_VIL_D1) {
        OS_REPORT("[Z2SeqMgr::subBgmStop] HIDDEN_VIL_D01 stop\n");
    }
    #endif
}

void Z2SeqMgr::subBgmStopInner() {
    if (field_0xb8 == -1) {
        return;
    }

    if (!mSubBgmHandle) {
        return;
    }

    int fadeTime = Z2Param::BGM_CROSS_FADEOUT_TIME;
    switch (getSubBgmID()) {
    case Z2BGM_MAGNE_GORON:
    case Z2BGM_MAGNE_GORON_D01:
    case Z2BGM_DEKUTOAD:
    case Z2BGM_DEKUTOAD_D01:
    case Z2BGM_SUMO_D1:
    case Z2BGM_VARIANT:
    case Z2BGM_IB_MBOSS_D01:
        fadeTime = 10;
        break;
    case Z2BGM_LUTERA1:
        fadeTime = 50;
        break;
    case Z2BGM_RIVER_GAME:
    case Z2BGM_RIVER_GAME_00:
        fadeTime = 33;
        setBattleBgmOff(false);
        break;
    case Z2BGM_SNOW_BOARD:
        fadeTime = 90;
        setBattleBgmOff(false);
        break;
    }

    mSubBgmHandle->stop(fadeTime);
    mSubBgmHandle.releaseSound();
    mMainBgmMaster.fadeIn(Z2Param::BGM_CROSS_FADEOUT_TIME);
    field_0xb8 = -1;
}

void Z2SeqMgr::bgmStreamPrepare(u32 bgmID) {
    if (mStreamBgmHandle) {
        bgmStreamStop(0);
    }

    Z2GetSoundMgr()->startSound(bgmID, &mStreamBgmHandle, NULL);
    mStreamBgmHandle->lockWhenPrepared();

    switch (bgmID) {
    case 0x2000010:
        mAllBgmMaster.forceIn();
        // fallthrough
    case 0x200000f:
        return;
    case 0x2000014:
        Z2GetSceneMgr()->setSceneExist(true);
        return;
    case 0x2000023:
        if (mMainBgmHandle) {
            mMainBgmHandle->stop(Z2Param::BGM_CROSS_FADEOUT_TIME);
        }
        return;
    case 0x2000038:
        if (mMainBgmHandle) {
            mMainBgmHandle->stop(0);
        }
        // fallthrough
    case 0x2000003:
    case 0x2000047:
    case 0x200005f:
        mStreamBgmMaster.forceOut();
        return;
    case 0x200007c:
        mStreamBgmMaster.fadeOut(45);
        return;
    case 0x200003f:
    case 0x200005c:
        Z2GetStatusMgr()->setDemoName("force_start");
        break;
    }

    mStreamBgmMaster.fadeOut(Z2Param::BGM_CROSS_FADEOUT_TIME);
}


bool Z2SeqMgr::bgmStreamCheckReady() {
    if (mStreamBgmHandle) {
        return mStreamBgmHandle->isPrepared();
    } else {
        return false;
    }
}

void Z2SeqMgr::bgmStreamPlay() {
    if (mStreamBgmHandle) {
        mStreamBgmHandle->unlockIfLocked();
    }

    if (getStreamBgmID() == -1) {
        if (mMainBgmHandle) {
            mMainBgmHandle->stop(30);
        }
    }

    #if !PLATFORM_SHIELD
    else if (getStreamBgmID() == 0x2000000) {
        if (mStreamBgmHandle) {
            mStreamBgmHandle->stop();
        }
        bgmStart(Z2BGM_MENU_SELECT, 0, 0);
    }
    #endif
}

void Z2SeqMgr::bgmStreamStop(u32 fadeTime) {
    if (getStreamBgmID() == 0x200003f || getStreamBgmID() == 0x200005c) {
        Z2GetStatusMgr()->setDemoName("force_end");
    }

    if (mStreamBgmHandle) {
        mStreamBgmHandle->stop(fadeTime);
    }

    mStreamBgmHandle.releaseSound();
}

void Z2SeqMgr::changeBgmStatus(s32 status) {
    if (!mMainBgmHandle) {
        return;
    }

    u32 moveTime = 0;
    bool mute;

    #if PLATFORM_SHIELD
    f32 volume1 = 1.0f;
    f32 volume2 = 1.0f;
    f32 volume3 = 1.0f;
    f32 volume4 = 1.0f;
    #else
    f32 volume1, volume2, volume3, volume4;
    #endif

    switch (getMainBgmID()) {
    case Z2BGM_TOAL_VILLEGE:
        if (status == mBgmStatus) {
            return;
        }

        if (mBgmStatus != 0xff) {
            moveTime = 60;
        }

        if (status == 1) {
            volume1 = 0.65f;
            volume2 = 0.0f;
            volume3 = 1.0f;
            moveTime = 30;
        } else {
            volume1 = 1.0f;
            volume2 = 1.0f;
            volume3 = 0.0f;
        }

        setChildTrackVolume(&mMainBgmHandle, 0, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 1, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 2, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 3, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 4, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 5, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 6, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 7, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 8, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 9, volume3, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 10, volume3, moveTime, -1.0f, -1.0f);
        break;
    case Z2BGM_HORSE_BATTLE:
        setChildTrackVolume(&mMainBgmHandle, 11, 0.0f, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 12, 0.0f, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 13, 0.0f, moveTime, -1.0f, -1.0f);
        break;
    case Z2BGM_DUNGEON_FOREST:
        mute = false;
        switch (status) {
        case 4:
        case 0xc:
            muteSceneBgm(Z2Param::SCENE_CHANGE_BGM_FADEOUT_TIME, 0.0f);
            // fallthrough
        case 7:
            mute = true;
            break;
        case 2:
            Z2GetSoundObjMgr()->setForceBattleArea(true, 1500, 5000, 10000);
            break;
        }

        if (!mute) {
            unMuteSceneBgm(Z2Param::SCENE_CHANGE_BGM_FADEOUT_TIME);
        }
        break;
    case Z2BGM_DUNGEON_LV2:
        switch (status) {
        case 0xe:
        case 0x10:
        case 0x11:
            subBgmStart(Z2BGM_DEATH_MOUNTAIN02);
            return;
        default:
            if (getSubBgmID() == Z2BGM_DEATH_MOUNTAIN02) {
                subBgmStop();
            }
            return;
        }
        break;
    case Z2BGM_DUNGEON_LV5:
        if (mBgmStatus != 0xff) {
            moveTime = 45;
        }

        if (status == 2 || status == 1) {
            volume1 = 0.0f;
            volume2 = 1.0f;
        } else {
            volume1 = 1.0f;
            volume2 = 0.0f;
        }

        setChildTrackVolume(&mMainBgmHandle, 0, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 1, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 2, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 3, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 4, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 8, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 9, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 10, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 11, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 12, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 13, volume2, moveTime, -1.0f, -1.0f);

        if (status == 4) {
            muteSceneBgm(60, 0.29f);
        } else {
            unMuteSceneBgm(60);
        }
        break;
    case Z2BGM_CASTLE_TOWN:
        if (mBgmStatus != 0xff) {
            moveTime = 45;
        }

        switch (status) {
        case 0:
        case 1:
        case 5:
            setChildTrackVolume(&mMainBgmHandle, 0, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 1, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 2, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 3, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 4, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 5, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 6, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 7, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 8, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 9, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 10, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 11, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 12, 0.0f, moveTime, -1.0f, -1.0f);
            break;
        case 2:
            setChildTrackVolume(&mMainBgmHandle, 0, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 1, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 2, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 3, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 4, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 5, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 6, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 7, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 8, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 9, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 10, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 11, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 12, 0.0f, moveTime, -1.0f, -1.0f);
            break;
        case 3:
            setChildTrackVolume(&mMainBgmHandle, 0, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 1, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 2, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 3, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 4, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 5, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 6, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 7, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 8, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 9, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 10, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 11, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 12, 1.0f, moveTime, -1.0f, -1.0f);
            break;
        case 4:
            setChildTrackVolume(&mMainBgmHandle, 0, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 1, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 2, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 3, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 4, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 5, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 6, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 7, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 8, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 9, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 10, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 11, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 12, 0.0f, moveTime, -1.0f, -1.0f);
            break;
        }

        if (status == 5) {
            muteSceneBgm(moveTime, 0.35f);
            mFlags.mHeightVolMod = false;
        } else {
            unMuteSceneBgm(moveTime);
            if (status < 2) {
                mFlags.mHeightVolMod = false;
            } else {
                mFlags.mHeightVolMod = true;
            }
        }
        break;
    case Z2BGM_HOLY_FOREST:
        if (mBgmStatus != 0xff) {
            moveTime = 45;
            setChildTrackVolume(&mMainBgmHandle, 7, 0.0f, 5, -1.0f, -1.0f);
        } else {
            setChildTrackVolume(&mMainBgmHandle, 7, 0.0f, 0, -1.0f, -1.0f);
        }

        if (status == 1) {
            volume1 = 0.6f;
            volume2 = 1.0f;
        } else {
            volume1 = 1.0f;
            volume2 = 0.0f;
        }

        setChildTrackVolume(&mMainBgmHandle, 0, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 1, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 2, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 3, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 4, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 5, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 6, volume2, moveTime, -1.0f, -1.0f);
        break;
    case Z2BGM_LUTERA2:
        if (status == 0) {
            muteSceneBgm(0, 0.5f);
        } else if (status == 1) {
            unMuteSceneBgm(80);
        }
        break;
    case Z2BGM_DEMO08:
        if (status == 1) {
            setChildTrackVolume(&mMainBgmHandle, 4, 1.0f, 90, -1.0f, -1.0f);
        } else if (status == 2) {
            bgmStop(90, 0);
        } else {
            setChildTrackVolume(&mMainBgmHandle, 4, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 5, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 6, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 7, 0.0f, 0, -1.0f, -1.0f);
        }
        break;
    case Z2BGM_DEMO10:
        if (status == 1) {
            setChildTrackVolume(&mMainBgmHandle, 0, 0.3f, 143, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 1, 0.3f, 143, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 2, 0.6f, 143, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 3, 0.6f, 143, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 4, 0.0f, 143, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 5, 1.0f, 143, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 6, 1.0f, 143, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 7, 1.0f, 143, -1.0f, -1.0f);
        } else {
            setChildTrackVolume(&mMainBgmHandle, 5, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 6, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 7, 0.0f, 0, -1.0f, -1.0f);
        }
        break;
    case Z2BGM_BOSS_SNOWWOMAN_1:
        if (status == 1) {
            setChildTrackVolume(&mMainBgmHandle, 9, 0.0f, 60, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 10, 1.0f, 60, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 11, 1.0f, 60, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 12, 1.0f, 60, -1.0f, -1.0f);
        } else {
            setChildTrackVolume(&mMainBgmHandle, 10, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 11, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 12, 0.0f, 0, -1.0f, -1.0f);
        }
        break;
    case Z2BGM_DUNGEON_LV8:
        if (status == mBgmStatus) {
            return;
        }

        if (mBgmStatus != 0xff) {
            moveTime = 45;
        }

        if (status == 0 || status == 0xb) {
            volume1 = 1.0f;
            volume2 = 0.0f;
            volume3 = 0.0f;
            volume4 = 0.0f;
        } else if (status >= 100) {
            volume1 = 0.0f;
            volume2 = 1.0f;

            volume3 = status < 200  ? 1.0f :
                      status > 3100 ? 0.0f :
                      Z2Calc::getParamByExp(status, 3100.0f, 200.0f, 0.3f, 0.0f, 1.0f,
                                              Z2Calc::CURVE_POSITIVE);

            volume4 = status < 200  ? 1.0f :
                      status > 1600 ? 0.0f :
                      Z2Calc::getParamByExp(status, 1600.0f, 200.0f, 0.3f, 0.0f, 1.0f,
                                            Z2Calc::CURVE_POSITIVE);
        } else {
            volume1 = 0.0f;
            volume2 = 1.0f;
            volume3 = 0.0f;
            volume4 = 0.0f;
        }

        setChildTrackVolume(&mMainBgmHandle, 2, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 3, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 4, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 5, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 6, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 7, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 8, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 9, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 10, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 11, volume3, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 12, volume3, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 13, volume4, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 14, volume4, moveTime, -1.0f, -1.0f);
        break;
    case Z2BGM_BOSS_ZANT:
        if (status == 0xd) {
            mSceneBgm.move(0.3f, 134);
        } else {
            mSceneBgm.forceIn();
            if (status > 6) {
                status = (status - 7) * 2;
            } else {
                status = status * 2 - 1;
            }
        }
        break;
    case Z2BGM_FORTUNE:
        if (status == 1) {
            muteSceneBgm(45, 0.5f);
        } else {
            unMuteSceneBgm(45);
        }
        break;
    case Z2BGM_DUNGEON_LV9_02:
        if (mBgmStatus != 0xff) {
            moveTime = 30;
        }

        if (status == 0xc) {
            volume4 = 1.0f;
            volume1 = 0.0f;
            volume2 = 0.5f;
            volume3 = 0.0f;
        } else if (status == 8) {
            volume4 = 0.0f;
            volume1 = 0.5f;
            volume2 = 1.0f;
            volume3 = 0.8f;
        } else if (status > 3000) {
            volume4 = Z2Calc::linearTransform(status, 3150.0f, 6150.0f, 1.0f, 0.0f, false);
            volume1 = Z2Calc::linearTransform(status, 3150.0f, 6150.0f, 0.0f, 1.0f, false);
            volume2 = Z2Calc::linearTransform(status, 3150.0f, 6150.0f, 0.5f, 1.0f, false);
            volume3 = Z2Calc::linearTransform(status, 3150.0f, 6150.0f, 0.0f, 1.0f, false);
        }

        setChildTrackVolume(&mMainBgmHandle, 0, volume4, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 1, volume4, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 2, volume4, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 3, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 4, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 5, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 6, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 7, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 8, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 9, volume3, moveTime, -1.0f, -1.0f);
        break;
    case Z2BGM_VS_GANON_02:
        if (status == mBgmStatus) {
            return;
        }

        if (mBgmStatus != 0xff) {
            moveTime = 45;
        }

        if (status == 1) {
            volume1 = 0.0f;
            volume2 = 1.0f;
        } else {
            volume1 = 1.0f;
            volume2 = 0.0f;
        }

        setChildTrackVolume(&mMainBgmHandle, 0, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 1, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 2, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 3, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 4, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 5, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 6, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 7, volume1, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 10, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 11, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 12, volume2, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 13, volume2, moveTime, -1.0f, -1.0f);
        break;
    case Z2BGM_TOAL_NIGHT:
    case Z2BGM_FILONE_FOREST:
        if (status == 1) {
            muteSceneBgm(45, 0.5f);
        } else if (status == 2) {
            muteSceneBgm(0, 0.5f);
        } else {
            unMuteSceneBgm(45);
        }
        break;
    case Z2BGM_VS_GANON_04:
        if (status == mBgmStatus) {
            return;
        }

        if (status >= 4) {
            moveTime = 50;
            if (status == 4) {
                volume4 = 1.0f;
                volume1 = 0.0f;
            } else if (status == 5) {
                volume4 = 0.0f;
                volume1 = 1.0f;
            } else if (status == 6) {
                volume4 = 0.0f;
                volume1 = 0.0f;
            } else if (status == 7) {
                volume4 = 0.0f;
                volume1 = 0.0f;
                moveTime = 0;
            } else if (status == 8) {
                volume4 = 1.0f;
                volume1 = 1.0f;
                moveTime = 0;
            }

            setChildTrackVolume(&mMainBgmHandle, 12, volume4, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 13, volume4, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 14, volume1, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mMainBgmHandle, 15, volume1, moveTime, -1.0f, -1.0f);
        }
        break;
    }

    Z2GetSoundStarter()->setPortData(&mMainBgmHandle, 9, status, -1);
    mBgmStatus = status & 0xff;
}

void Z2SeqMgr::changeSubBgmStatus(s32 status) {
    if (!mSubBgmHandle) {
        return;
    }

    u32 moveTime = 0;
    bool bvar2 = true;

    switch (getSubBgmID()) {
    case Z2BGM_DEKUTOAD:
        if (status == 1 && mSubBgmStatus == 2) {
            mSubBgmMaster.forceOut();
            mDekuToadCount = 100;
        } else if (status == 3) {
            mSubBgmMaster.forceOut();
            mDekuToadCount = 30;
        }
        break;
    case Z2BGM_HORSE_BATTLE:
        if (status == mSubBgmStatus) {
            return;
        }

        if (status != 2) {
            bvar2 = false;
            if (mSubBgmStatus != 0xff) {
                moveTime = 40;
            }
        }

        if (status == 1) {
            setChildTrackVolume(&mSubBgmHandle, 0, 0.0f, 60, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 1, 0.0f, 60, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 11, 1.0f, 60, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 12, 1.0f, 60, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 13, 1.0f, 60, -1.0f, -1.0f);
        } else {
            setChildTrackVolume(&mSubBgmHandle, 0, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 1, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 11, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 12, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 13, 0.0f, moveTime, -1.0f, -1.0f);
        }
        break;
    case Z2BGM_FACE_OFF_BATTLE2:
        if (status == mSubBgmStatus) {
            return;
        }

        bvar2 = false;
        if (mSubBgmStatus != 0xff) {
            moveTime = 30;
        }

        if (status == 1) {
            setChildTrackVolume(&mSubBgmHandle, 0, 0.0f, 10, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 10, 0.0f, 10, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 11, 1.0f, 10, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 12, 1.0f, 10, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 13, 1.0f, 10, -1.0f, -1.0f);
        } else {
            setChildTrackVolume(&mSubBgmHandle, 0, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 10, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 11, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 12, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 13, 0.0f, moveTime, -1.0f, -1.0f);
        }
        break;
    case Z2BGM_RODEO:
        if (status == mSubBgmStatus) {
            break;
        }

        if (status == 1) {
            if (mSubBgmStatus != 0xff) {
                moveTime = 60;
            }
            setChildTrackVolume(&mSubBgmHandle, 9, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 10, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 11, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 12, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 13, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 14, 0.0f, moveTime, -1.0f, -1.0f);
        } else if (status == 2) {
            if (mSubBgmStatus != 0xff) {
                moveTime = 20;
            }
            setChildTrackVolume(&mSubBgmHandle, 9, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 10, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 11, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 12, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 13, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 14, 1.0f, moveTime, -1.0f, -1.0f);
        }
        break;
    case Z2BGM_VARIANT:
        switch (status) {
        case 1:
            switch (mSubBgmStatus) {
            case 2:
                setChildTrackVolume(&mSubBgmHandle, 0, 1.0f, 15, -1.0f, -1.0f);
                setChildTrackVolume(&mSubBgmHandle, 8, 0.0f, 15, -1.0f, -1.0f);
                setChildTrackVolume(&mSubBgmHandle, 9, 0.0f, 15, -1.0f, -1.0f);
                setChildTrackVolume(&mSubBgmHandle, 10, 0.0f, 15, -1.0f, -1.0f);
                break;
            case 3:
                Z2GetSoundStarter()->setPortData(&mSubBgmHandle, 9, 1, -1);
                return;
            }
            break;
        case 2:
            setChildTrackVolume(&mSubBgmHandle, 0, 0.0f, 10, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 8, 1.0f, 10, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 9, 1.0f, 10, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 10, 1.0f, 10, -1.0f, -1.0f);
            break;
        }
        break;
    case Z2BGM_BATTLE_TWILIGHT:
        if (mSubBgmStatus == status) {
            return;
        }

        if (mSubBgmStatus != 0xff) {
            moveTime = 25;
        }

        bvar2 = false;
        if (status == 2) {
            setChildTrackVolume(&mSubBgmHandle, 2, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 3, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 4, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 5, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 6, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 7, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 8, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 9, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 10, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 11, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 12, 0.0f, moveTime, -1.0f, -1.0f);
        } else if (status == 3) {
            setChildTrackVolume(&mSubBgmHandle, 2, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 3, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 4, 0.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 5, 1.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 6, 1.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 7, 1.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 8, 1.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 9, 1.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 10, 1.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 11, 1.0f, 0, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 12, 1.0f, 0, -1.0f, -1.0f);
        } else {
            setChildTrackVolume(&mSubBgmHandle, 2, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 3, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 4, 1.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 5, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 6, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 7, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 8, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 9, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 10, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 11, 0.0f, moveTime, -1.0f, -1.0f);
            setChildTrackVolume(&mSubBgmHandle, 12, 0.0f, moveTime, -1.0f, -1.0f);
        }
        break;
    case Z2BGM_BATTLE_NORMAL:
        return;
    }

    if (bvar2) {
        Z2GetSoundStarter()->setPortData(&mSubBgmHandle, 9, status, -1);
    }

    mSubBgmStatus = status & 0xff;
}

void Z2SeqMgr::onVariantBgmJumpEnd(bool param_0) {
    if (param_0) {
        u32 moveTime = 0;
        if (mSubBgmStatus == 3) {
            moveTime = 45;
            mSubBgmStatus = 1;
        }
        setChildTrackVolume(&mSubBgmHandle, 8, 0.0f, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mSubBgmHandle, 9, 0.0f, moveTime, -1.0f, -1.0f);
        setChildTrackVolume(&mSubBgmHandle, 10, 0.0f, moveTime, -1.0f, -1.0f);
    } else {
        setChildTrackVolume(&mSubBgmHandle, 0, 1.0f, 0, -1.0f, -1.0f);
    }

    Z2GetSoundMgr()->getSeqMgr()->calc();
    Z2GetSoundMgr()->getSeqMgr()->mixOut();
}

void Z2SeqMgr::changeFishingBgm(s32 param_0) {
    switch (param_0) {
    case 0:
        if (getSubBgmID() == Z2BGM_FISHING_HIT) {
            subBgmStop();
        }
        unMuteSceneBgm(Z2Param::BGM_CROSS_FADEOUT_TIME);
        break;
    case 1:
        if (getSubBgmID() == Z2BGM_FISHING_HIT) {
            subBgmStop();
        }
        muteSceneBgm(2, 0.5f);
        break;
    case 2:
        subBgmStart(Z2BGM_FISHING_HIT);
        muteSceneBgm(1, 0.0f);
        break;
    case 4:
        if (getSubBgmID() == Z2BGM_FISHING_HIT) {
            subBgmStop();
        }
        muteSceneBgm(1, 0.0f);
        break;
    }
}

void Z2SeqMgr::talkInBgm() {
    if (getStreamBgmID() == 0x2000038 || getStreamBgmID() == 0x200007c || getMainBgmID() == Z2BGM_WCS_D01) {
        return;
    }

    mBgmPause.move(Z2Param::VOL_BGM_TALKING, 2);
}

void Z2SeqMgr::talkOutBgm() {
    mBgmPause.move(Z2Param::VOL_BGM_DEFAULT, 2);
}

void Z2SeqMgr::menuInBgm() {
    mBgmPause.move(Z2Param::VOL_BGM_PAUSING, 2);
}

void Z2SeqMgr::menuOutBgm() {
    mBgmPause.move(Z2Param::VOL_BGM_DEFAULT, 2);
}

void Z2SeqMgr::fanfareFramework() {
    switch (mFanfareID) {
    case Z2BGM_OPEN_BOX:
        mFanfareCount = 150;
        // fallthrough
    case Z2BGM_KOMONJO_GET_INTRO:
        Z2GetSoundMgr()->startSound(mFanfareID, &mFanfareHandle, 0);
        mFanfareMute.fadeOut(1);
        mFanfareID.setAnonymous();
        break;
    case Z2BGM_ITEM_GET_ME:
        if (mFanfareCount == 0) {
            Z2GetSoundMgr()->startSound(mFanfareID, &mFanfareHandle, 0);
            mFanfareCount = 25;
        }
        if (mFanfareCount == 1) {
            mFanfareMute.fadeOut(1);
            mFanfareID.setAnonymous();
            mFanfareCount = 0;
        }
        break;
    case Z2BGM_FISHING_BARE:
    case Z2BGM_FISHING_GET1:
    case Z2BGM_FISHING_GET2:
    case Z2BGM_FISHING_GET3:
        changeFishingBgm(0);
        // fallthrough
    case Z2BGM_ITEM_GET:
    case Z2BGM_ITEM_GET_MINI:
    case Z2BGM_HEART_GET:
    case Z2BGM_ITEM_GET_INSECT:
    case Z2BGM_ITEM_GET_SMELL:
    case Z2BGM_ITEM_GET_POU:
    case Z2BGM_ITEM_GET_ME_S:
        if (mFanfareCount == 0) {
            Z2GetSoundMgr()->startSound(mFanfareID, &mFanfareHandle, 0);
            mFanfareMute.fadeOut(1);
            mFanfareID.setAnonymous();
        }
        break;
    case Z2BGM_HOWL_TOBIKUSA:
    case Z2BGM_HOWL_UMAKUSA:
    case Z2BGM_HOWL_ZELDASONG:
    case Z2BGM_HOWL_LIGHT_PRLD:
    case Z2BGM_LIGHT_PRLD_DUO:
    case Z2BGM_SOUL_REQ_HOWL:
    case Z2BGM_SOUL_REQ_DUO:
    case Z2BGM_HEALING_HOWL:
    case Z2BGM_HEALING_DUO:
    case Z2BGM_NEW_01_HOWL:
    case Z2BGM_NEW_01_DUO:
    case Z2BGM_NEW_02_HOWL:
    case Z2BGM_NEW_02_DUO:
    case Z2BGM_NEW_03_HOWL:
    case Z2BGM_NEW_03_DUO:
        if (mFanfareCount == 0) {
            mFanfareCount = 50;
            mFanfareMute.fadeOut(30);
        } else if (mFanfareCount == 1) {
            Z2GetSoundMgr()->startSound((u32)mFanfareID, &mFanfareHandle, 0);
            mFanfareID.setAnonymous();
        }
        break;
    }

    if (!isItemGetDemo() && mFanfareMute.getDest() != 1.0f) {
        mFanfareMute.fadeIn(Z2Param::BGM_CROSS_FADEIN_TIME);
    }
    if (mFanfareCount != 0) {
        mFanfareCount--;
    }
}

void Z2SeqMgr::stopWolfHowlSong() {
    switch (mFanfareID) {
    case Z2BGM_HOWL_TOBIKUSA:
    case Z2BGM_HOWL_UMAKUSA:
    case Z2BGM_HOWL_ZELDASONG:
    case Z2BGM_HOWL_LIGHT_PRLD:
    case Z2BGM_LIGHT_PRLD_DUO:
    case Z2BGM_SOUL_REQ_HOWL:
    case Z2BGM_SOUL_REQ_DUO:
    case Z2BGM_HEALING_HOWL:
    case Z2BGM_HEALING_DUO:
    case Z2BGM_NEW_01_HOWL:
    case Z2BGM_NEW_01_DUO:
    case Z2BGM_NEW_02_HOWL:
    case Z2BGM_NEW_02_DUO:
    case Z2BGM_NEW_03_HOWL:
    case Z2BGM_NEW_03_DUO:
        mFanfareID.setAnonymous();
        mFanfareCount = 0;
        break;
    }

    if (mFanfareHandle) {
        switch (mFanfareHandle->getID()) {
        case Z2BGM_HOWL_TOBIKUSA:
        case Z2BGM_HOWL_UMAKUSA:
        case Z2BGM_HOWL_ZELDASONG:
        case Z2BGM_HOWL_LIGHT_PRLD:
        case Z2BGM_LIGHT_PRLD_DUO:
        case Z2BGM_SOUL_REQ_HOWL:
        case Z2BGM_SOUL_REQ_DUO:
        case Z2BGM_HEALING_HOWL:
        case Z2BGM_HEALING_DUO:
        case Z2BGM_NEW_01_HOWL:
        case Z2BGM_NEW_01_DUO:
        case Z2BGM_NEW_02_HOWL:
        case Z2BGM_NEW_02_DUO:
        case Z2BGM_NEW_03_HOWL:
        case Z2BGM_NEW_03_DUO:
            mFanfareHandle->stop(30);
        }
    }
}

void Z2SeqMgr::setHeightVolMod(bool isVolMod, u32 fadeTime) {
    mFlags.mHeightVolMod = isVolMod;
    if (!isVolMod) {
        field_0x84.fadeIn(fadeTime);
    }
}

void Z2SeqMgr::setTimeProcVolMod(bool isVolMod, u32 fadeTime) {
    mFlags.mTimeProcVolMod = isVolMod;
}

static f32 sDeathMtBottom = -1000.0f;

static f32 sDeathMtTop = 3650.0f;

static f32 sUnderWaterDepthMax = 3500.0f;

void Z2SeqMgr::processBgmFramework() {
    fanfareFramework();
    battleBgmFramework();
    mbossBgmMuteProcess();
    fieldBgmFramework();

    if (!mStreamBgmHandle && mStreamBgmMaster.getDest() != 1.0f) {
        mStreamBgmMaster.fadeIn(Z2Param::BGM_CROSS_FADEIN_TIME);
    }

    if (field_0xb8 != -1) {
        if (field_0xb8 > 0) {
            field_0xb8--;
        } else {
            subBgmStopInner();
            field_0xb8 = -1;
        }
    } else if (!mSubBgmHandle && mMainBgmMaster.getDest() != 1.0f) {
        mMainBgmMaster.fadeIn(Z2Param::BGM_CROSS_FADEIN_TIME);
    }

    if (mFlags.mTimeProcVolMod) {
        if (Z2GetStatusMgr()->checkDayTime() && field_0xa4.getDest() != 1.0f) {
            field_0xa4.fadeIn(600);
        } else if (!Z2GetStatusMgr()->checkDayTime() && field_0xa4.getDest() != 0.0f) {
            field_0xa4.fadeOut(600);
        }
    }

    if (mFlags.mHeightVolMod && Z2GetSceneMgr()->isSceneExist()
        && Z2GetLink() != NULL && Z2GetLink()->getCurrentPos() != NULL)
    {
        f32 link_y = Z2GetLink()->getCurrentPos()->y;
        f32 volume, depth;
        switch (getMainBgmID()) {
        case Z2BGM_DEATH_MOUNTAIN01:
            volume = Z2Calc::getParamByExp(link_y, sDeathMtTop, sDeathMtBottom, 0.3f, 0.0f, 1.0f,
                                           Z2Calc::CURVE_NEGATIVE);
            field_0x84.move(volume, 0);
            break;
        case Z2BGM_RAKKA_HOUSE:
            if (link_y < -10500.0f) {
                bgmStart(Z2BGM_LAKE, 150, 0);
                field_0x84.forceIn();
            } else if (link_y < -7500.0f) {
                volume = Z2Calc::getParamByExp(link_y, -10500.0f, -7500.0f, 0.3f, 0.0f, 1.0f,
                                               Z2Calc::CURVE_POSITIVE);
                field_0x84.move(volume, 0);
            }
            break;
        case Z2BGM_FIELD_LINK_DAY:
        case Z2BGM_FIELD_LINK_NIGHT:
        case Z2BGM_ZORA_VILLAGE:
        case Z2BGM_FISHING:
        case Z2BGM_LAKE:
            depth = Z2GetStatusMgr()->getCameraInWaterDepth();
            if (depth > sUnderWaterDepthMax) {
                field_0x84.forceOut();
            } else if (depth > 0.0f) {
                volume = Z2Calc::getParamByExp(depth, sUnderWaterDepthMax, 0.0f, 0.3f, 0.1f, 1.0f,
                                               Z2Calc::CURVE_POSITIVE);
                field_0x84.move(volume, 0);
            } else if (field_0x84.getDest() != 1.0f) {
                field_0x84.fadeIn(30);
            }
            break;
        case Z2BGM_SNOW_MOUNTAIN:
            if (Z2GetSceneMgr()->getCurrentRoomNum() == 0) {
                if (link_y > -1500.0f) {
                    field_0x84.move(0.3f, 0);
                } else if (link_y > -13000.0f) {
                    volume = Z2Calc::getParamByExp(link_y, -1500.0f, -13000.0f, 0.3f, 0.3f, 1.0f,
                                                   Z2Calc::CURVE_NEGATIVE);
                    field_0x84.move(volume, 0);
                } else {
                    field_0x84.forceIn();
                }
            } else if (Z2GetSceneMgr()->getCurrentRoomNum() == 1) {
                if (link_y > -4500.0f) {
                    field_0x84.fadeOut(30);
                } else if (link_y > -45000.0f) {
                    volume = Z2Calc::getParamByExp(link_y, -3000.0f, -45000.0f, 0.3f, 0.0f, 1.0f,
                                                   Z2Calc::CURVE_NEGATIVE);
                    field_0x84.move(volume, 0);
                } else {
                    field_0x84.forceIn();
                }
            } else if (Z2GetSceneMgr()->getCurrentRoomNum() == 2) {
                if (link_y > 900.0f) {
                    field_0x84.forceOut();
                } else if (link_y > -1500.0f) {
                    volume = Z2Calc::getParamByExp(link_y, 900.0f, -1500.0f, 0.3f, 0.0f, 0.3f,
                                                   Z2Calc::CURVE_NEGATIVE);
                    field_0x84.move(volume, 0);
                } else {
                    field_0x84.move(0.3f, 0);
                }
            }
            break;
        case Z2BGM_DUNGEON_LV9_02:
            changeBgmStatus(link_y);
            break;
        }
    }

    if (mDekuToadCount != 0) {
        mDekuToadCount--;
        if (getSubBgmID() == Z2BGM_DEKUTOAD) {
            if (mDekuToadCount == 60) {
                mSubBgmMaster.fadeIn(60);
                mDekuToadCount = 0;
            } else if (mDekuToadCount == 0) {
                mSubBgmMaster.fadeIn(30);
            }
        }
    }

    mAllBgmMaster.calc();
    mBgmPause.calc();
    mFanfareMute.calc();
    mSceneBgm.calc();
    mMainBgmMaster.calc();
    mSubBgmMaster.calc();
    mStreamBgmMaster.calc();
    field_0x84.calc();
    mWindStone.calc();
    field_0xa4.calc();
    
    f32 base_vol = mAllBgmMaster.get() * mBgmPause.get() * mFanfareMute.get() * mWindStone.get() * mTwilightGateVol;

    if (mMainBgmHandle) {
        f32 volume = 1.0f;
        if (getMainBgmID() != Z2BGM_GAME_OVER) {
            volume = base_vol * mMainBgmMaster.get() * mSceneBgm.get() * mStreamBgmMaster.get() * field_0x84.get() * field_0xa4.get();
        }
        mMainBgmHandle->getAuxiliary().moveVolume(volume, 0);
    }

    if (mSubBgmHandle) {
        f32 volume = base_vol * mSubBgmMaster.get() * mStreamBgmMaster.get();
        mSubBgmHandle->getAuxiliary().moveVolume(volume, 0);
    }

    if (mStreamBgmHandle) {
        f32 volume = base_vol * mMainBgmMaster.get() * mSceneBgm.get();
        mStreamBgmHandle->getAuxiliary().moveVolume(volume, 0);
    }

    setWindStoneVol(1.0f, 30);
}

bool Z2SeqMgr::checkBgmIDPlaying(u32 bgmID) {
    if (getMainBgmID() == bgmID) {
        return true;
    }

    if (getSubBgmID() == bgmID) {
        return true;
    }

    if (getStreamBgmID() == bgmID) {
        return true;
    }

    return false;
}

f32 Z2SeqMgr::getChildTrackVolume(JAISoundHandle* handle, int trackId) {
    f32 volume = 0.0f;
    if (*handle) {
        JAISeq* seq = handle->getSound()->asSeq();
        if (seq != NULL && seq->getChildTrack(trackId) != NULL) {
            JAISoundChild* child = seq->getChild(trackId);
            if (child != NULL) {
                volume = child->mMove.params_.mVolume;
            }
        }
    }

    return volume;
}

void Z2SeqMgr::setChildTrackVolume(JAISoundHandle* handle, int trackId, f32 volume,
                                   u32 moveTime, f32 pan, f32 dolby) {
    if (!*handle) {
        return;
    }

    JAISeq* seq = (*handle)->asSeq();
    if (seq == NULL) {
        return;
    }

    if (seq->getChildTrack(trackId) == NULL) {
        return;
    }

    JAISoundChild* child = seq->getChild(trackId);
    if (child == NULL) {
        return;
    }

    if (volume > 1.0f) {
        volume = 1.0f;
    } else if (volume < 0.0f) {
        volume = 0.0f;
    }

    child->mMove.moveVolume(volume, moveTime);
    if (pan != -1.0f) {
        child->mMove.movePan(pan, moveTime);
    }

    if (dolby != -1.0f) {
        child->mMove.moveDolby(dolby, moveTime);
    }
}

void Z2SeqMgr::resetBattleBgmParams() {
    if (Z2GetSoundObjMgr() != NULL) {
        Z2GetSoundObjMgr()->setForceBattleArea(false, 700, 1100, 1500);
    }
}

void Z2SeqMgr::setBattleBgmOff(bool isBgmOff) {
    if (mFlags.mBattleBgmOff != isBgmOff && isBgmOff &&
        (getSubBgmID() == Z2BGM_BATTLE_NORMAL || getSubBgmID() == Z2BGM_BATTLE_TWILIGHT))
    {
        stopBattleBgm(Z2Param::BGM_CROSS_FADEOUT_TIME, Z2Param::BGM_CROSS_FADEIN_TIME);
    }

    mFlags.mBattleBgmOff = isBgmOff;
}

void Z2SeqMgr::setBattleSearched(bool isBattleSearched) {
    if (!mFlags.mBattleBgmOff && Z2GetSceneMgr()->isSceneExist()
        && mFlags.mBattleSearched != isBattleSearched && mBattleSeqState != 3)
    {
        if (getSubBgmID() == Z2BGM_BATTLE_NORMAL && mBattleSeqState == 1) {
            if (isBattleSearched) {
                setChildTrackVolume(&mSubBgmHandle, 2, 1.0f, Z2Param::FOUND_TRACK_FI_TIME, -1.0f, -1.0f);
                setChildTrackVolume(&mSubBgmHandle, 3, 1.0f, Z2Param::FOUND_TRACK_FI_TIME, -1.0f, -1.0f);
                setChildTrackVolume(&mSubBgmHandle, 12, 1.0f, Z2Param::FOUND_TRACK_FI_TIME, -1.0f, -1.0f);
                setChildTrackVolume(&mSubBgmHandle, 14, 1.0f, Z2Param::FOUND_TRACK_FI_TIME, -1.0f, -1.0f);
            } else {
                setChildTrackVolume(&mSubBgmHandle, 2, 0.0f, Z2Param::FOUND_TRACK_FO_TIME, -1.0f, -1.0f);
                setChildTrackVolume(&mSubBgmHandle, 3, 0.0f, Z2Param::FOUND_TRACK_FO_TIME, -1.0f, -1.0f);
                setChildTrackVolume(&mSubBgmHandle, 12, 0.0f, Z2Param::FOUND_TRACK_FO_TIME, -1.0f, -1.0f);
                setChildTrackVolume(&mSubBgmHandle, 14, 0.0f, Z2Param::FOUND_TRACK_FO_TIME, -1.0f, -1.0f);
            }
        }
        mFlags.mBattleSearched = isBattleSearched;
    }
}

void Z2SeqMgr::setBattleDistIgnore(bool isBattleDistIgnore) {
    mFlags.mBattleDistIgnore = isBattleDistIgnore;
}

void Z2SeqMgr::setBattleGhostMute(bool isBattleGhostMute) {
    if (getSubBgmID() == Z2BGM_BATTLE_NORMAL) {
        if (isBattleGhostMute) {
            mSubBgmMaster.move(1.0f, 5);
        } else {
            mSubBgmMaster.move(0.4f, 5);
        }
    }
}

void Z2SeqMgr::setBattleDistState(u8 state) {
    if (!mFlags.mBattleBgmOff && Z2GetSceneMgr()->isSceneExist()) {
        if (!mFlags.mBattleDistIgnore) {
            if (state < 2 && mBattleSeqCount != 0) {
                mBattleSeqCount = 0;
                if (getSubBgmID() == Z2BGM_BATTLE_TWILIGHT || getSubBgmID() == Z2BGM_BATTLE_NORMAL)
                {
                    mSubBgmHandle->getFader().fadeIn(10);
                    mMainBgmMaster.fadeOut(10);
                }
            }

            if ((Z2GetSoundObjMgr()->isTwilightBattle() && getSubBgmID() == Z2BGM_BATTLE_NORMAL) ||
                (!Z2GetSoundObjMgr()->isTwilightBattle() && getSubBgmID() == Z2BGM_BATTLE_TWILIGHT))
            {
                mSubBgmHandle->stop(30);
                mSubBgmHandle->releaseHandle();
            }

            if (Z2GetSoundObjMgr()->isTwilightBattle() || getSubBgmID() == Z2BGM_BATTLE_TWILIGHT) {
                switch (state) {
                case 0:
                    if (mBattleSeqState == 0) {
                        startBattleBgm(true);
                    }
                    break;
                case 1:
                    if (mBattleSeqState == 0) {
                        startBattleBgm(true);
                    }
                    break;
                case 3:
                    if (mBattleSeqState != 0) {
                        stopBattleBgm(25, 25);
                    }
                    break;
                }
            } else if (getMainBgmID() != Z2BGM_FIELD_LINK_DAY) {
                switch (state) {
                case 0:
                    if (mBattleSeqState == 0) {
                        startBattleBgm(true);
                    } else if (mBattleSeqState == 1) {
                        setChildTrackVolume(&mSubBgmHandle, data_8045086C, 1.0f, Z2Param::CLOSE_BATTLE_TRACK_FI_TIME, -1.0f, -1.0f);
                    }
                    break;
                case 1:
                    if (mBattleSeqState == 0) {
                        startBattleBgm(true);
                    } else if (mBattleSeqState == 1) {
                        setChildTrackVolume(&mSubBgmHandle, data_8045086C, 0.0f, Z2Param::CLOSE_BATTLE_TRACK_FO_TIME, -1.0f, -1.0f);
                    }
                    break;
                case 2:
                    if (mBattleSeqState == 1) {
                        setChildTrackVolume(&mSubBgmHandle, data_8045086C, 0.0f, Z2Param::CLOSE_BATTLE_TRACK_FO_TIME, -1.0f, -1.0f);
                    }
                    break;
                case 3:
                    if (mBattleSeqState == 1 || mBattleSeqState == 2) {
                        stopBattleBgm(Z2Param::BGM_CROSS_FADEOUT_TIME, Z2Param::BGM_CROSS_FADEIN_TIME);
                    }
                    break;
                }
            }
        }
        mBattleDistState = state;
    }
}

void Z2SeqMgr::setBattleSeqState(u8 state) {
    if (!mFlags.mBattleBgmOff && Z2GetSceneMgr()->isSceneExist() && mBattleSeqState != state) {
        if (state == 0) {
            stopBattleBgm(Z2Param::BGM_CROSS_FADEOUT_TIME, Z2Param::BGM_CROSS_FADEIN_TIME);
        } else if (getSubBgmID() == Z2BGM_BATTLE_NORMAL && !(getMainBgmID() == Z2BGM_FIELD_LINK_DAY && !Z2GetSoundObjMgr()->isTwilightBattle())) {
            switch (state) {
            case 2:
                if (mBattleSeqState == 1) {
                    Z2GetSoundStarter()->setPortData(&mSubBgmHandle, 9, 1, -1);
                    setChildTrackVolume(&mSubBgmHandle, data_8045086C, 1.0f, 0, -1.0f, -1.0f);
                    setChildTrackVolume(&mSubBgmHandle, 2, 1.0f, 0, -1.0f, -1.0f);
                    setChildTrackVolume(&mSubBgmHandle, 3, 1.0f, 0, -1.0f, -1.0f);
                    setChildTrackVolume(&mSubBgmHandle, 12, 1.0f, 0, -1.0f, -1.0f);
                    setChildTrackVolume(&mSubBgmHandle, 14, 1.0f, 0, -1.0f, -1.0f);
                }

                if (mBattleSeqCount != 0) {
                    mBattleSeqCount = 0;
                    mSubBgmHandle->getFader().fadeIn(10);
                    mMainBgmMaster.fadeOut(10);
                }
                break;
            case 3:
                mMainBgmMaster.forceOut();
                mSubBgmMaster.forceIn();

                if (mBattleSeqState == 1) {
                    Z2GetSoundStarter()->setPortData(&mSubBgmHandle, 9, 3, -1);
                } else if (mBattleSeqState == 2) {
                    Z2GetSoundStarter()->setPortData(&mSubBgmHandle, 9, 2, -1);
                }
                break;
            }

            mBattleSeqState = state;
        }
    }
}

void Z2SeqMgr::setBattleLastHit(u8 lastHit) {
    if (getSubBgmID() == Z2BGM_BATTLE_NORMAL) {
        setBattleDistIgnore(true);
        mSubBgmMaster.move(Z2Param::ENEMY_LASTHIT_MUTE_VOLUME, Z2Param::ENDING_BLOW_VOL_DOWN_TIME);
        field_0xc1 = Z2Param::ENDING_BLOW_MIN_FINISH_TIME;
        mBattleLastHit = lastHit;
    }
}

void Z2SeqMgr::battleBgmFramework() {
    if (getSubBgmID() == Z2BGM_BATTLE_NORMAL || getSubBgmID() == Z2BGM_BATTLE_TWILIGHT) {
        if (mBattleSeqCount != 0) {
            mBattleSeqCount--;
            if (mBattleSeqCount == 0) {
                mSubBgmHandle->stop();
                mBattleSeqState = 0;
            }
        }
    }

    if (!mFlags.mBattleBgmOff && Z2GetSceneMgr()->isSceneExist()) {
        Z2GetSoundObjMgr()->searchEnemy();
        switch (getSubBgmID()) {
        case Z2BGM_BATTLE_TWILIGHT:
            break;
        case Z2BGM_BATTLE_NORMAL:
            if (getMainBgmID() == Z2BGM_FIELD_LINK_DAY && !Z2GetSoundObjMgr()->isTwilightBattle()) {
                return;
            }

            if (field_0xc1 != 0) {
                field_0xc1--;
                if (field_0xc1 < mBattleLastHit && mSubBgmMaster.getDest() != 1.0f) {
                    mSubBgmMaster.fadeIn(Z2Param::ENDING_BLOW_VOL_LOWER_RECOVER_TIME);
                }

                if (field_0xc1 == 0) {
                    setBattleDistIgnore(false);
                    if (Z2GetSoundObjMgr()->checkBattleFinish()) {
                        setBattleSeqState(3);
                    } else if (mSubBgmMaster.getDest() != 1.0f) {
                        mSubBgmMaster.fadeIn(Z2Param::ENDING_BLOW_VOL_LOWER_RECOVER_TIME);
                    }
                }
            }
            break;
        default:
            mBattleSeqState = 0;
            setBattleDistIgnore(false);
            mFlags.mBattleSearched = 0;
            field_0xc1 = 0;
            mBattleLastHit = Z2Param::ENDING_BLOW_VOL_LOWER_TIME;
        }
    }
}

void Z2SeqMgr::startBattleBgm(bool isFadeIn) {
    if (!mFlags.mBattleBgmOff && Z2GetSceneMgr()->isSceneExist()
        && mBattleSeqState == 0 && (mFlags.mBattleDistIgnore || mBattleDistState <= 1))
    {
        JAISoundID bgm_id = Z2BGM_BATTLE_NORMAL;
        if (Z2GetSceneMgr()->isInDarkness() || Z2GetSoundObjMgr()->isTwilightBattle()) {
            bgm_id = Z2BGM_BATTLE_TWILIGHT;
        }

        u32 subBgmID = getSubBgmID();
        if (subBgmID != bgm_id) {
            mBattleSeqState = 1;
            if (Z2GetSoundMgr()->startSound(bgm_id, &mSubBgmHandle, NULL)) {
                Z2GetSoundObjMgr()->setBattleInit();
                mSubBgmStatus = 0xff;

                s16 ivar2 = Z2GetLink()->getMoveSpeed() - 23;
                if (ivar2 < 0) {
                    ivar2 = 0;
                }

                if (ivar2 > Z2Param::BGM_CROSS_FADEIN_TIME) {
                    ivar2 = Z2Param::BGM_CROSS_FADEIN_TIME;
                }

                u8 fadeinTime, fadeoutTime;
                switch (bgm_id) {
                case Z2BGM_BATTLE_NORMAL:
                    fadeinTime = Z2Param::BGM_CROSS_FADEIN_TIME - ivar2;
                    fadeoutTime = Z2Param::BGM_CROSS_FADEOUT_TIME - ivar2 / 2;
                    if (!mFlags.mBattleSearched) {
                        setChildTrackVolume(&mSubBgmHandle, 2, 0.0f, 0, -1.0f, -1.0f);
                        setChildTrackVolume(&mSubBgmHandle, 3, 0.0f, 0, -1.0f, -1.0f);
                        setChildTrackVolume(&mSubBgmHandle, 12, 0.0f, 0, -1.0f, -1.0f);
                        setChildTrackVolume(&mSubBgmHandle, 14, 0.0f, 0, -1.0f, -1.0f);
                    }
                    if (mBattleDistState != 0) {
                        setChildTrackVolume(&mSubBgmHandle, data_8045086C, 0.0f, 0, -1.0f, -1.0f);
                    }
                    break;
                case Z2BGM_BATTLE_TWILIGHT:
                    changeSubBgmStatus(Z2GetSoundObjMgr()->isTwilightBattle());
                    fadeinTime = 25 - ivar2 / 4;
                    fadeoutTime = 25 - ivar2 / 4;
                    break;
                }

                if (isFadeIn) {
                    mSubBgmMaster.fadeInFromOut(fadeinTime);
                } else {
                    mSubBgmMaster.forceIn();
                }

                mMainBgmMaster.fadeOut(fadeoutTime);
            }
        }
    }
}

void Z2SeqMgr::stopBattleBgm(u8 subFadeoutTime, u8 mainFadeinTime) {
    if (mBattleSeqCount == 0) {
        s16 ivar6 = 0;
        if (Z2GetLink() != NULL) {
            ivar6 = Z2GetLink()->getMoveSpeed() - 23;
        }

        if (ivar6 < 0) {
            ivar6 = 0;
        }

        if (ivar6 > Z2Param::BGM_CROSS_FADEIN_TIME) {
            ivar6 = Z2Param::BGM_CROSS_FADEIN_TIME;
        }

        if (getSubBgmID() == Z2BGM_BATTLE_NORMAL) {
            subFadeoutTime = Z2Param::BGM_CROSS_FADEIN_TIME + ivar6;
            mainFadeinTime = Z2Param::BGM_CROSS_FADEOUT_TIME + ivar6 / 2;
            mSubBgmHandle->fadeOut(subFadeoutTime);
            mBattleSeqCount = subFadeoutTime;
        } else if (getSubBgmID() == Z2BGM_BATTLE_TWILIGHT) {
            if (mSubBgmStatus > 1) {
                subFadeoutTime = Z2Param::BGM_CROSS_FADEIN_TIME + ivar6 / 4;
                mainFadeinTime = Z2Param::BGM_CROSS_FADEIN_TIME + ivar6 / 4;
            } else {
                subFadeoutTime = 25 + ivar6 / 4;
                mainFadeinTime = 25 + ivar6 / 4;
            }

            mSubBgmHandle->fadeOut(subFadeoutTime);
            mBattleSeqCount = subFadeoutTime;
        }

        mMainBgmMaster.fadeIn(mainFadeinTime);
        mBattleDistState = 3;
        setBattleDistIgnore(false);
        mFlags.mBattleSearched = false;
    }
}

void Z2SeqMgr::fieldBgmStart() {
    if (Z2GetSceneMgr()->isSceneExist() && mFlags.mFieldBgmPlay) {
        if (Z2GetStatusMgr()->checkDayTime()) {
            Z2GetSoundMgr()->startSound(Z2BGM_FIELD_LINK_DAY, &mMainBgmHandle, NULL);
            changeBgmStatus(0);
            field_0xc4 = 0;

            if (Z2GetLink()->isRiding()) {
                mFlags.mRiding = true;
                mRideCount = 0;
                fieldRidingMute();
            } else {
                mFlags.mRiding = false;
                mRideCount = 0;
                fieldRidingMute();
            }
        } else {
            Z2GetSoundMgr()->startSound(Z2BGM_FIELD_LINK_NIGHT, &mMainBgmHandle, NULL);
            mBgmStatus = 0;
        }
    }
}

void Z2SeqMgr::fieldRidingMute() {
    if (Z2GetSceneMgr()->isSceneExist() && mMainBgmHandle) {
        f32 volume1, volume2;
        if (mFlags.mRiding) {
            volume1 = 1.0f;
            volume2 = 0.0f;
        } else {
            volume1 = 0.0f;
            volume2 = 1.0f;
        }

        setChildTrackVolume(&mMainBgmHandle, 5, volume1, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 6, volume1, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 7, volume1, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 8, volume1, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 9, volume1, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 10, volume1, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 11, volume1, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 12, volume2, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 13, volume2, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 14, volume2, mRideCount, -1.0f, -1.0f);
        setChildTrackVolume(&mMainBgmHandle, 15, volume2, mRideCount, -1.0f, -1.0f);
    }
}

void Z2SeqMgr::onFieldBgmJumpStart() {
    field_0xc8 = getChildTrackVolume(&mMainBgmHandle, 5);
    field_0xcc = getChildTrackVolume(&mMainBgmHandle, 12);
}

void Z2SeqMgr::onFieldBgmJumpEnd() {
    setChildTrackVolume(&mMainBgmHandle, 5, field_0xc8, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 6, field_0xc8, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 7, field_0xc8, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 8, field_0xc8, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 9, field_0xc8, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 10, field_0xc8, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 11, field_0xc8, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 12, field_0xcc, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 13, field_0xcc, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 14, field_0xcc, 0, -1.0f, -1.0f);
    setChildTrackVolume(&mMainBgmHandle, 15, field_0xcc, 0, -1.0f, -1.0f);

    Z2GetSoundMgr()->getSeqMgr()->calc();
    Z2GetSoundMgr()->getSeqMgr()->mixOut();

    if (mRideCount != 0) {
        fieldRidingMute();
    }
}

void Z2SeqMgr::fieldBgmFramework() {
    if (Z2GetSceneMgr()->isSceneExist() && !Z2GetSceneMgr()->isInDarkness()
        && (Z2GetSceneMgr()->getCurrentSceneNum() == Z2SCENE_HYRULE_FIELD || Z2GetSceneMgr()->getCurrentSceneNum() == Z2SCENE_CASTLE_TOWN_GATES)
        && mFlags.mFieldBgmPlay)
    {
        if (mRideCount != 0) {
            mRideCount--;
        }

        if (getMainBgmID() == Z2BGM_FIELD_LINK_DAY) {
            field_0xc4 = Z2GetSoundStarter()->getPortData(&mMainBgmHandle, 10, -1);
            if (Z2GetStatusMgr()->isPaused()) {
                changeBgmStatus(13);
            } else {
                if (field_0xc4 != 16) {
                    if (mFlags.mRiding != Z2GetLink()->isRiding()) {
                        mFlags.mRiding = Z2GetLink()->isRiding();
                        mRideCount = 50;
                        fieldRidingMute();
                    }
                }

                if (field_0xc4 == 0 || field_0xc4 == 21 || field_0xc4 == 22
                                    || field_0xc4 == 23 || field_0xc4 == 24) {
                    return;
                }

                if (Z2GetStatusMgr()->getDemoStatus() == 10) {
                    switch (field_0xc4) {
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                        changeBgmStatus(2);
                        break;
                    default:
                        changeBgmStatus(13);
                        break;
                    }
                } else if (mBattleDistState <= 2) {
                    if ((field_0xc4 < 7 || field_0xc4 > 15) && mBattleDistState <= 1) {
                        switch (field_0xc4) {
                        case 2:
                            changeBgmStatus(7);
                            break;
                        case 3:
                            changeBgmStatus(6);
                            break;
                        case 4:
                            changeBgmStatus(5);
                            break;
                        case 0:
                        case 1:
                        case 5:
                        case 7:
                        case 19:
                        case 20:
                        default:
                            changeBgmStatus(4);
                            break;
                        }
                    }
                } else if (Z2GetStatusMgr()->checkDayTime()) {
                    if (Z2GetLink()->getMovingTime() < 150) {
                        switch (field_0xc4) {
                        case 3:
                            if (mFlags.mRiding && Z2GetSoundObjMgr()->getEnemyNumVeryFar() == 0) {
                                changeBgmStatus(3);
                            }
                            break;
                        case 7:
                        case 8:
                        case 9:
                        case 10:
                        case 11:
                        case 12:
                        case 13:
                        case 14:
                        case 15:
                        case 17:
                        case 18:
                        case 19:
                        case 20:
                            changeBgmStatus(2);
                            break;
                        default:
                            changeBgmStatus(13);
                            break;
                        }
                    } else {
                        switch (field_0xc4) {
                        case 2:
                        case 3:
                            changeBgmStatus(13);
                            break;
                        default:
                            changeBgmStatus(12);
                            break;
                        }
                    }
                } else {
                    if (field_0xc4 >= 7 && field_0xc4 <= 15 && Z2GetStatusMgr()->getHour() >= 20) {
                        mMainBgmHandle->stop(60);
                        mMainBgmHandle.releaseSound();
                    } else {
                        switch (field_0xc4) {
                        case 2:
                        case 3:
                            break;
                        case 17:
                        case 18:
                        case 19:
                        case 20:
                            changeBgmStatus(11);
                            break;
                        default:
                            changeBgmStatus(10);
                            break;
                        }
                    }
                }
            }
        } else if (getMainBgmID() == Z2BGM_FIELD_LINK_NIGHT) {
            if (!Z2GetStatusMgr()->isPaused()) {
                u8 hour = Z2GetStatusMgr()->getHour();
                if (hour >= 5 && hour < 20) {
                    mMainBgmHandle->stop(60);
                    mMainBgmHandle.releaseSound();
                }
            }
        } else if (getMainBgmID() == -1 && !Z2GetStatusMgr()->isPaused() && mBattleSeqState == 0) {
            u8 hour = Z2GetStatusMgr()->getHour();
            if (hour >= 6 && hour < 19) {
                if (hour >= 8) {
                    Z2GetSoundMgr()->startSound(Z2BGM_FIELD_LINK_DAY, &mMainBgmHandle, NULL);
                    changeBgmStatus(9);
                    field_0xc4 = 24;
                } else {
                    Z2GetSoundMgr()->startSound(Z2BGM_FIELD_LINK_DAY, &mMainBgmHandle, NULL);
                    changeBgmStatus(8);
                    field_0xc4 = 23;
                }

                if (Z2GetLink()->isRiding()) {
                    mFlags.mRiding = true;
                    mRideCount = 0;
                    fieldRidingMute();
                } else {
                    mFlags.mRiding = false;
                    mRideCount = 0;
                    fieldRidingMute();
                }
            } else if (hour >= 20 || hour < 5) {
                Z2GetSoundMgr()->startSound(Z2BGM_FIELD_LINK_NIGHT, &mMainBgmHandle, NULL);
            }
        }
    }
}

void Z2SeqMgr::mbossBgmMuteProcess() {}

void Z2SeqMgr::bgmSetSwordUsing(s32 param_0) {}

void Z2SeqMgr::bgmNowBattle(f32 param_0) {}

void Z2SeqMgr::taktModeMute() {
    mBgmPause.move(0.3f, 10);
}

void Z2SeqMgr::taktModeMuteOff() {
    mBgmPause.fadeIn(10);
}
