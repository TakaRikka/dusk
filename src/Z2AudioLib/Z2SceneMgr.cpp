#include "JSystem/JAudio2/JASBasicWaveBank.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
#include "JSystem/JAudio2/JAUSoundTable.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "Z2AudioLib/Z2Param.h"
#include "Z2AudioLib/Z2SeMgr.h"
#include "Z2AudioLib/Z2SeqMgr.h"
#include "Z2AudioLib/Z2SoundMgr.h"
#include "Z2AudioLib/Z2SoundObjMgr.h"
#include "Z2AudioLib/Z2StatusMgr.h"
#include "d/d_com_inf_game.h"

Z2SceneMgr::Z2SceneMgr() : JASGlobalInstance<Z2SceneMgr>(true) {
    sceneNum = -1;
    BGM_ID = -1;
    roomNum = -1;
    requestSeWave_1 = 0;
    loadedSeWave_1 = 0;
    requestSeWave_2 = 0;
    loadedSeWave_2 = 0;
    requestBgmWave_1 = 0;
    loadedBgmWave_1 = 0;
    requestBgmWave_2 = 0;
    loadedBgmWave_2 = 0;
    requestDemoWave = 0;
    loadedDemoWave = 0;
    field_0x18 = 0;
    field_0x19 = 0;
    field_0x1a = false;
    field_0x1b = false;
    sceneExist = 0;
    inGame = 0;
    inDarkness_ = false;
    load1stWait = 0;
}

void Z2SceneMgr::setInDarkness(bool inDarkness) {
    OS_REPORT("[Z2SceneMgr::setInDarkness] inDarkness = %d\n", inDarkness);

    inDarkness_ = inDarkness;
    if (!inDarkness) {
        Z2GetSoundMgr()->resetFilterAll();
    }
}

static char* sSpotName[81] = {
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

void Z2SceneMgr::setSceneExist(bool isSceneExist) {
    sceneExist = isSceneExist;
    timer = 0;

    JAISeMgr* seMgr = Z2GetSoundMgr()->getSeMgr();

    if (isSceneExist) {
        inGame = true;

        if (requestDemoWave == 0x85) {
            seMgr->getCategory(9)->getParams()->moveVolume(0.0f, 0);
        } else if (requestDemoWave == 0x7F) {
            Z2GetSeMgr()->seMoveVolumeAll(0.0f, 0);
        } else {
            seMgr->getCategory(9)->getParams()->moveVolume(1.0f, 33);
        }
    } else {
        seMgr->getCategory(9)->getParams()->moveVolume(0.0f, 180);
    }
}

void Z2SceneMgr::setFadeOutStart(u8 fadeType) {
    OS_REPORT("[Z2SceneMgr::setFadeOutStart] fadeType = %d\n", fadeType);

    setSceneExist(false);
    Z2GetSeqMgr()->bgmAllMute(33,  3.0f / 10.0f);
    Z2GetSeMgr()->seMoveVolumeAll(0.0f, 33);
    Z2GetSeqMgr()->setBattleBgmOff(true);
    load1stWait = 40;
    timer = -1;
}

void Z2SceneMgr::setFadeInStart(u8 fadeType) {
    OS_REPORT("[Z2SceneMgr::setFadeInStart] fadeType = %d\n", fadeType);

    if (requestDemoWave == 0x7f) {
        Z2GetSeMgr()->seMoveVolumeAll(0.0f, 0);
    } else {
        Z2GetSeMgr()->seMoveVolumeAll(1.0f, 33);

        if (requestDemoWave == 0x85) {
            Z2GetSoundMgr()->getSeMgr()->getCategory(9)->getParams()->moveVolume(0.0f, 0);
        }
    }

    Z2GetSeqMgr()->setTwilightGateVol(1.0f);
    Z2GetStatusMgr()->menuOut();

    if (!field_0x1a) {
        Z2GetSeqMgr()->bgmAllUnMute(33);
    }

    inGame = true;
}

void Z2SceneMgr::setSceneName(char* spot, s32 room, s32 layer) {
    OS_REPORT("[Z2SceneMgr::setSceneName] spot = %s, room = %d, layer = %d\n", spot, room, layer);
    JAISoundID bgm_id = -1;
    JAISound* sound;

    int spotNo = 0;

    u8 se_wave1 = 0;
    u8 se_wave2 = 0;
    u8 bgm_wave1 = 0;
    u8 bgm_wave2 = 0;
    u8 demo_wave = 0;

    bool bVar6 = false;
    bool height_vol_mod = false;
    bool field_bgm_play = false;
    bool time_proc_vol_mod = false;
    bool bVar2 = false;
    field_0x1b = false;

    f32 fVar1 = -1.0f;
    
    Z2GetSeqMgr()->resetBattleBgmParams();
    Z2GetSeqMgr()->setWindStoneVol(1.0f, 0);
    Z2GetSeqMgr()->setTwilightGateVol(1.0f);
    Z2GetSeMgr()->resetCrowdSize();
    Z2GetSoundObjMgr()->setGhostEnemyState(0);
    Z2GetSeMgr()->resetModY();

    if (Z2GetStatusMgr()->getDemoStatus() == 8) {
        Z2GetStatusMgr()->setDemoName("force_end");
    }

    if (spot != NULL) {
        for (spotNo = 0; spotNo < ARRAY_SIZE(sSpotName); spotNo++) {
            if (strcmp(spot, sSpotName[spotNo]) == 0) {
                break;
            }
        }

        if (spotNo == ARRAY_SIZE(sSpotName)) {
            spotNo = Z2SCENE_NONE;
        }
    }

    switch (spotNo) {
    case Z2SCENE_ENEMY_TEST:
        bgm_wave2 = 2;
        if (room < 30) {
            se_wave1 = 0x8a;
        } else {
            se_wave1 = 0x8b;
        }
        break;
    case Z2SCENE_ORDON_RANCH:
        if (layer == 1) {
            if (dComIfGs_isSaveSwitch(0x67)) {
                bgm_id = Z2BGM_TOAL_NIGHT;
                bgm_wave1 = 6;
                Z2GetSeqMgr()->changeBgmStatus(0);
            }
        } else {
            if (layer == 8) {
                demo_wave = 0x5c;
            }
            bgm_id = Z2BGM_RANCH;
            bgm_wave1 = 1;
            bgm_wave2 = 2;
            time_proc_vol_mod = true;
        }

        se_wave1 = 0x26;
        se_wave2 = 0x25;
        field_0x1b = true;
        break;
    case Z2SCENE_ORDON_VILLAGE:
        field_0x1b = true;
        se_wave1 = 0x26;

        if (room == 0) {
            se_wave2 = 0x27;
            switch (layer) {
            case 1:
                bgm_id = Z2BGM_TOAL_NIGHT;
                bgm_wave1 = 6;
                Z2GetSeqMgr()->changeBgmStatus(0);
                break;
            case 8:
                bgm_id = 0x2000011;
                demo_wave = 0x64;
                break;
            case 10:
            case 11:
                se_wave1 = 0;
                se_wave2 = 0;
                demo_wave = 0x7f;
                break;
            default:
                bgm_id = Z2BGM_TOAL_VILLEGE;
                bgm_wave1 = 3;
                bgm_wave2 = 4;
                time_proc_vol_mod = true;
                break;
            }
        } else if (room == 1) {
            se_wave2 = 0x28;
            switch (layer) {
            case 1:
                field_0x1b = false;
                if (dComIfGs_isSaveSwitch(0x67)) {
                    bgm_id = Z2BGM_TOAL_NIGHT;
                    bgm_wave1 = 6;
                    Z2GetSeqMgr()->changeBgmStatus(0);
                }
                break;
            case 12:
                se_wave1 = 0;
                se_wave2 = 0;
                demo_wave = 0x7f;
                break;
            case 5:
            case 8:
                bgm_id = Z2BGM_EVENT01;
                bgm_wave1 = 5;
                break;
            case 9:
                demo_wave = 0x5c;
                break;
            case 3:
                bVar2 = true;
                if (!dComIfGs_isSaveSwitch(0xa)) {
                    break;
                }
                // fallthrough
            case 4:
                /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[18])
                        /* dSv_event_flag_c::F_0625 - Faron Woods - Saved Talo and a monkey */
                    && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[625]))
                {
                    bgm_wave1 = 0x55;
                    /* dSv_event_flag_c::F_0094 - Ordon Village - Talo went after the monkey */
                    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[194])) {
                        bgm_id = Z2BGM_EVENT05;
                    }
                    break;
                }
                // fallthrough
            default:
                bgm_id = Z2BGM_TOAL_VILLEGE;
                bgm_wave1 = 3;
                time_proc_vol_mod = true;
                break;
            }
        }
        break;
    case Z2SCENE_ORDON_INTERIOR:
        se_wave1 = 0x26;

        switch (room) {
        case 0:
            bgm_id = Z2BGM_INDOOR;
            bgm_wave1 = 8;
            bgm_wave2 = 9;
            se_wave2 = 0x2a;
            break;
        case 1:
            if (layer == 0) {
                /* dSv_event_flag_c::F_0038 - Ordon Village - Opening (2nd day) cat returns home */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[136])) {
                    bgm_id = Z2BGM_SHOP01;
                } else {
                    bgm_id = Z2BGM_SHOP02;
                }
                bgm_wave1 = 7;
            } else if (layer == 2) {
                /* dSv_event_flag_c::M_020 - Cutscene - [cutscene: ] Colin kidnapped : ON once watched */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[53])) {
                    bgm_id = Z2BGM_SHOP01;
                    bgm_wave1 = 7;
                } else {
                    bgm_id = Z2BGM_ROOM_2;
                    bgm_wave1 = 0x35;
                }
            }
            se_wave2 = 0x29;
            break;
        case 2:
            if (layer == 0 || layer == 3) {
                bgm_id = Z2BGM_INDOOR;
                bgm_wave1 = 8;
            } else if (layer == 1) {
                bgm_id = Z2BGM_TOAL_NIGHT;
                bgm_wave1 = 6;
                Z2GetSeqMgr()->changeBgmStatus(1);
            } else if (layer == 2) {
                /* dSv_event_flag_c::M_020 - Cutscene - [cutscene: ] Colin kidnapped : ON once watched */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[53])) {
                    bgm_id = Z2BGM_INDOOR;
                    bgm_wave1 = 8;
                } else {
                    bgm_id = Z2BGM_ROOM_2;
                    bgm_wave1 = 0x35;
                }
            }
            se_wave2 = 0x2c;
            break;
        case 3:
        case 4:
        case 7:
            bgm_id = Z2BGM_INDOOR;
            bgm_wave1 = 8;
            break;
        case 5:
            if (layer == 0) {
                if (dComIfGs_isSaveSwitch(0x67)) {
                    bgm_id = Z2BGM_TOAL_NIGHT;
                    bgm_wave1 = 6;
                    Z2GetSeqMgr()->changeBgmStatus(1);
                } else {
                    bgm_id = Z2BGM_INDOOR;
                    bgm_wave1 = 8;
                }
            } else {
                /* dSv_event_flag_c::M_020 - Cutscene - [cutscene: ] Colin kidnapped : ON once watched */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[53])) {
                    bgm_id = Z2BGM_INDOOR;
                    bgm_wave1 = 8;
                } else {
                    bgm_id = Z2BGM_ROOM_2;
                    bgm_wave1 = 0x35;
                }
            }
            se_wave2 = 0x2b;
        }
        field_0x1b = true;
        break;
    case Z2SCENE_ORDON_SPRING:
        se_wave1 = 0x26;
        se_wave2 = 0x2d;

        switch (layer) {
        case 4:
            field_0x1b = false;
            bVar2 = true;
            if (dComIfGs_isSaveSwitch(0x67)) {
                bgm_id = Z2BGM_TOAL_NIGHT;
                bgm_wave1 = 6;
                Z2GetSeqMgr()->changeBgmStatus(0);
            }
            break;
        case 7:
            se_wave1 = 0;
            se_wave2 = 0;
            demo_wave = 0x7f;
            break;
        case 8:
            demo_wave = 0x65;
            break;
        case 10:
            demo_wave = 0x69;
            break;
        case 9:
        case 11:
            demo_wave = 0x6a;
            break;
        case 12:
            demo_wave = 0x85;
            break;
        case 13:
        case 14:
            demo_wave = 0x5c;
            break;
        case 3:
            /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[18])
                    /* dSv_event_flag_c::F_0625 - Faron Woods - Saved Talo and a monkey */
                && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[625]))
            {
                bgm_id = Z2BGM_EVENT05;
                bgm_wave1 = 0x55;
                break;
            }
            // fallthrough
        default:
            bVar2 = true;
            break;
        }
        break;
    case Z2SCENE_TWILIGHT_HYRULE_CASTLE:
        se_wave1 = 0x2e;
        if (inDarkness_) {
            se_wave2 = 0x30;
            if (room == 0) {
                if (layer == 11) {
                    demo_wave = 0x66;
                } else if (layer == 7 || layer == 10) {
                    demo_wave = 0x67;
                } else {
                    /* dSv_event_flag_c::M_009 - Cutscene - [cutscene: 6B] Prison escape - Midna rides on back */
                    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[42])) {
                        bgm_id = Z2BGM_TWILIGHT;
                        bgm_wave1 = 0xe;
                    }
                }
            } else if (room == 3 && (layer == 8 || layer == 12)) {
                demo_wave = 0x68;
            } else if ((room == 3 && layer == 10) || (room == 2 && layer == 8)) {
                bgm_id = Z2BGM_DEMO08;
                bgm_wave1 = 0x21;
                demo_wave = 0x69;
            } else {
                if (room == 3 && layer == 14) {
                    bVar2 = true;
                }
                bgm_id = Z2BGM_TWILIGHT;
                bgm_wave1 = 0xe;
            }
        } else {
            if (room == 3 && layer == 9) {
                demo_wave = 0x75;
            }
            se_wave2 = 0x2f;
        }
        break;
    case Z2SCENE_CASTLE_THRONE_ROOM:
        if (layer == 8) {
            demo_wave = 0x68;
        } else if (layer == 9) {
            demo_wave = 0x7f;
        }
        break;
    case Z2SCENE_CORO_SHOP:
        se_wave1 = 0x31;
        if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
            se_wave2 = 0x33;
        } else {
            if (layer == 1) {
                bgm_id = Z2BGM_EVENT05;
                bgm_wave1 = 0x55;
                se_wave2 = 0x32;
            } else {
                bgm_id = Z2BGM_FILONE_FOREST;
                bgm_wave1 = 0xf;
                time_proc_vol_mod = true;
                Z2GetSeqMgr()->changeBgmStatus(1);
                se_wave2 = 0x32;
            }
        }
        break;
    case Z2SCENE_FARON_WOODS:
        se_wave1 = 0x31;
        if (layer == 11) {
            demo_wave = 0x6c;
            inDarkness_ = false;
        } else if (layer == 9) {
            demo_wave = 0x6a;
        } else if (inDarkness_) {
            if (layer == 7) {
                demo_wave = 0x78;
            } else if (layer == 8) {
                demo_wave = 0x66;
            } else if (layer == 10) {
                bgm_id = Z2BGM_DEMO10;
                bgm_wave1 = 0x21;
                demo_wave = 0x6b;
            } else {
                bgm_id = Z2BGM_TWILIGHT;
                bgm_wave1 = 0xe;
            }
            se_wave2 = 0x33;
        } else {
            se_wave2 = 0x32;
            switch (layer) {
            case 4:
            case 6:
            case 12:
                demo_wave = 0x7f;
                se_wave1 = 0;
                se_wave2 = 0;
                break;
            case 13:
                demo_wave = 0x5c;
                break;
            case 1:
                /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[18])
                        /* dSv_event_flag_c::F_0625 - Faron Woods - Saved Talo and a monkey */
                    && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[625]))
                {
                    bgm_id = Z2BGM_EVENT05;
                    bgm_wave1 = 0x55;
                }
                break;
            default:
                bgm_id = Z2BGM_FILONE_FOREST;
                bgm_wave1 = 0xf;
                time_proc_vol_mod = true;

                if (room == 14) {
                    Z2GetSeqMgr()->changeBgmStatus(1);
                } else {
                    Z2GetSeqMgr()->changeBgmStatus(0);
                }
                break;
            }
        }
        break;
    case Z2SCENE_KAKARIKO_VILLAGE:
        se_wave1 = 0x34;
        if (layer == 8) {
            demo_wave = 0x6e;
            inDarkness_ = false;
        } else if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
            se_wave2 = 0x36;
        } else {
            se_wave2 = 0x35;
            switch (layer) {
            case 6:
            case 7:
                demo_wave = 0x7f;
                se_wave1 = 0;
                se_wave2 = 0;
                break;
            case 9:
                demo_wave = 0x6e;
                break;
            case 10:
                demo_wave = 0x6f;
                break;
            case 11:
                bVar2 = true;
                demo_wave = 0x70;
                break;
            case 12:
                demo_wave = 0x71;
                break;
            case 13:
                demo_wave = 0x83;
                bgm_wave1 = 0x2c;
                break;
            case 0:
                bgm_id = Z2BGM_KAKARIKO;
                bgm_wave1 = 0x10;
                bgm_wave2 = 0x18;
                demo_wave = 0x84;
                time_proc_vol_mod = true;
                break;
            case 1:
                bgm_id = Z2BGM_EVENT02;
                bgm_wave1 = 0x1c;
                bgm_wave2 = 0x18;
                break;
            case 4:
                bVar2 = true;
                bgm_id = Z2BGM_LUTERA2;
                bgm_wave1 = 0x2c;
                break;
            default:
                bgm_id = Z2BGM_KAKARIKO;
                bgm_wave1 = 0x10;
                bgm_wave2 = 0x23;
                time_proc_vol_mod = true;
                break;
            }
        }
        break;
    case Z2SCENE_KAKARIKO_INTERIOR:
        se_wave1 = 0x34;
        if (inDarkness_) {
            se_wave2 = 0x36;
            if (room == 0 && layer == 8) {
                demo_wave = 0x6d;
                bgm_wave1 = 0xe;
            } else {
                bgm_id = Z2BGM_TWILIGHT;
                bgm_wave1 = 0xe;
                if (room == 5) {
                    bgm_wave2 = 0x1c;
                }
            }
        } else {
            switch (room) {
            case 1:
                /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[64])) {
                    bgm_id = Z2BGM_SHOP01;
                } else {
                    bgm_id = Z2BGM_SHOP02;
                }
                bgm_wave1 = 7;
                break;
            case 2:
                if (layer == 13) {
                    demo_wave = 0x83;
                } else {
                    /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
                    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[64])) {
                        bgm_id = Z2BGM_INDOOR;
                        bgm_wave1 = 8;
                    }
                }
                break;
            case 3:
                /* dSv_event_flag_c::M_076 - Misc. - First conversation with Castle Town Malo Mart shop clerk */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[109])) {
                    bgm_id = Z2BGM_SHOP_MARO;
                    bgm_wave1 = 0x33;
                           /* dSv_event_flag_c::M_052 - Main Event - Horseback battle clear */
                } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[85])) {
                    bgm_id = Z2BGM_SHOP01;
                    bgm_wave1 = 7;
                } else {
                    bgm_id = Z2BGM_INDOOR;
                    bgm_wave1 = 8;
                }
                break;
            case 0:
                if (layer == 9) {
                    demo_wave = 0x85;
                } else {
                    bgm_id = Z2BGM_CHURCH1;
                    bgm_wave1 = 0x2f;
                }
                break;
            case 4:
            case 5:
            case 6:
                break;
            }
            se_wave2 = 0x86;
        }
        break;
    case Z2SCENE_SANCTUARY_BASEMENT:
        se_wave1 = 0x34;
        if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
            se_wave2 = 0x36;
        } else {
            bgm_id = Z2BGM_CHURCH2;
            bgm_wave1 = 0x30;
            se_wave2 = 0x86;
            Z2GetSeqMgr()->muteSceneBgm(30, 0.5f);
        }
        break;
    case Z2SCENE_KAKARIKO_GRAVEYARD:
        se_wave1 = 0x34;
        if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
            se_wave2 = 0x36;
        } else {
            if (layer == 4) {
                bgm_id = Z2BGM_LUTERA2;
                bgm_wave1 = 0x2c;
            } else {
                bgm_id = Z2BGM_CHURCH2;
                bgm_wave1 = 0x30;
                Z2GetSeqMgr()->unMuteSceneBgm(30);
                time_proc_vol_mod = true;
                bgm_wave2 = 0x2c;
            }
            se_wave2 = 0x37;
        }
        break;
    case Z2SCENE_DEATH_MOUNTAIN:
        se_wave1 = 0x34;
        if (layer == 8) {
            demo_wave = 0x7f;
            se_wave1 = 0;
            se_wave2 = 0;
        } else if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
            se_wave2 = 0x39;
        } else {
            bgm_id = Z2BGM_DEATH_MOUNTAIN01;
            bgm_wave1 = 0x11;
            se_wave2 = 0x38;
            time_proc_vol_mod = true;
            if (room == 3) {
                height_vol_mod = true;
            }
        }
        break;
    case Z2SCENE_HIDDEN_VILLAGE:
        if (layer == 0) {
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
            if (dComIfGs_isSwitch(0x2e, 0) && !dComIfGs_isSwitch(0x11, 0)) {
                bgm_id = Z2BGM_HIDDEN_VILLAGE;
            }
            field_0x1b = true;
        } else if (layer == 1) {
            Z2GetSeqMgr()->bgmStop(33, 0);
        }
        bgm_wave1 = 0x31;
        se_wave1 = 0x34;
        se_wave2 = 0x4f;
        break;
    case Z2SCENE_IMPAZ_HOUSE:
        bgm_id = Z2BGM_INDOOR;
        bgm_wave1 = 8;
        se_wave1 = 0x34;
        se_wave2 = 0x4f;
        field_0x1b = true;
        break;
    case Z2SCENE_SUMO_HALL:
        if (layer == 8) {
            demo_wave = 0x7f;
            se_wave1 = 0;
            se_wave2 = 0;
        } else {
            bgm_id = Z2BGM_DEATH_MOUNTAIN02;
            bgm_wave1 = 0x11;
            se_wave1 = 0x34;
            bgm_wave2 = 0x12;
            se_wave2 = 0x3a;
        }
        break;
    case Z2SCENE_LAKE_HYLIA:
        se_wave1 = 0x3b;
        if (layer == 8) {
            demo_wave = 0x72;
            inDarkness_ = false;
        } else if (room == 1 && layer == 9) {
            demo_wave = 0x74;
        } else if (layer == 10 || layer == 11) {
            demo_wave = 0x7f;
            se_wave1 = 0;
            se_wave2 = 0;
        } else if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
            se_wave2 = 0x3d;
            if (layer == 14) {
                field_0x1b = true;
                Z2GetSoundObjMgr()->setForceBattleArea(true, 10000, 40000, 50000);
            } else {
                if (dComIfGs_isSaveSwitch(0xf) && !dComIfGs_isSaveSwitch(0x10)) {
                    field_0x1b = true;
                }
                bgm_wave2 = 0x4d;
            }
        } else {
            if (room == 0) {
                bgm_id = Z2BGM_LAKE;
                bgm_wave1 = 0x34;
                height_vol_mod = true;
                time_proc_vol_mod = true;
            }
            se_wave2 = 0x3c;
        }
        break;
    case Z2SCENE_UPPER_ZORAS_RIVER:
        se_wave1 = 0x55;
        if (layer == 8) {
            demo_wave = 0x7f;
            se_wave1 = 0;
            se_wave2 = 0;
        } else if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
            se_wave2 = 0x3f;
        } else {
            bgm_wave1 = 0x34;
            se_wave2 = 0x3e;
            height_vol_mod = true;
        }
        break;
    case Z2SCENE_FISHING_POND:
        if (layer == 8) {
            demo_wave = 0x7f;
            se_wave1 = 0;
            se_wave2 = 0;
        } else {
            se_wave1 = 0x3b;
            bgm_id = Z2BGM_FISHING;
            bgm_wave1 = 0x2b;
            se_wave2 = 0x3e;
            height_vol_mod = true;
            time_proc_vol_mod = true;
        }
        break;
    case Z2SCENE_HENA_CABIN:
        se_wave1 = 0x3b;
        bgm_id = Z2BGM_INDOOR;
        bgm_wave1 = 8;
        bgm_wave2 = 0x47;
        se_wave2 = 0x3e;
        break;
    case Z2SCENE_ZORAS_RIVER:
        se_wave1 = 0x55;
        if (inDarkness_) {
            bgm_id = Z2BGM_CARGO_GAME_TW;
            bgm_wave1 = 0x24;
            se_wave2 = 0x41;
            field_0x1b = true;
        } else {
            if (layer == 4) {
                bgm_id = 0x200000b;
            } else if (layer == 1) {
                bgm_id = Z2BGM_MINIGAME_ROOM;
                bgm_wave1 = 0x47;
                bgm_wave2 = 0x42;
            } else if (layer == 0 || layer == 2) {
                bgm_wave2 = 0x57;
            }
            se_wave2 = 0x40;
        }
        break;
    case Z2SCENE_ZORAS_DOMAIN:
        se_wave1 = 0x3b;
        if (layer == 8 || layer == 9) {
            demo_wave = 0x82;
        } else if (layer == 10 || layer == 11) {
            se_wave1 = 0;
            se_wave2 = 0;
            demo_wave = 0x7f;
        } else if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0x0e;
            bgm_wave2 = 0x2c;
            se_wave2 = 0x3f;
        } else {
            bgm_id = Z2BGM_ZORA_VILLAGE;
            bgm_wave1 = 0x20;
            se_wave2 = 0x3e;
            height_vol_mod = true;
            time_proc_vol_mod = true;
        }
        break;
    case Z2SCENE_CASTLE_TOWN:
        se_wave1 = 0x42;
        if (layer == 8) {
            demo_wave = 0x68;
        } else if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
            se_wave2 = 0x44;
        } else {
            se_wave2 = 0x43;
            time_proc_vol_mod = true;
            if (room == 1 && layer == 9) {
                demo_wave = 0x7a;
            } else if (layer == 10) {
                se_wave1 = 0;
                se_wave2 = 0;
                demo_wave = 0x7f;
            } else if (!(layer == 1 && room == 1
                   /* dSv_event_flag_c::F_0542 - Cutscene - [Cutscene] Hyrule Castle barrier disappears (Midna goes crazy) */
                && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[542])))
            {
                bgm_id = Z2BGM_CASTLE_TOWN;
                bgm_wave1 = 0x25;
                Z2GetSeqMgr()->changeBgmStatus(room);
            }
        }
        break;
    case Z2SCENE_CASTLE_TOWN_INTERIOR:
        if (room == 6) {
            se_wave1 = 0x2e;
            se_wave2 = 0x2f;
        } else {
            se_wave1 = 0x42;
            if (inDarkness_) {
                bgm_id = Z2BGM_TWILIGHT;
                bgm_wave1 = 0xe;
                se_wave2 = 0x44;
            } else {
                se_wave2 = 0x5b;
                if (room == 5) {
                    if (layer == 8) {
                        demo_wave = 0x73;
                    } else if (layer == 9) {
                        se_wave1 = 0;
                        se_wave2 = 0;
                        demo_wave = 0x7f;
                    } else {
                        bgm_id = Z2BGM_INDOOR_03;
                        bgm_wave1 = 8;
                    }
                }
            }
        }
        break;
    case Z2SCENE_CASTLE_TOWN_SHOPS:
        se_wave1 = 0x42;
        se_wave2 = 0x5b;

        switch (room) {
        case 0:
            if (layer == 1) {
                bgm_id = Z2BGM_SHOP_MARO;
                bgm_wave1 = 0x33;
            } else {
                bgm_id = Z2BGM_SHOP_CELEB;
                bgm_wave1 = 0x32;
            }
            break;
        case 1:
            bgm_id = Z2BGM_FORTUNE;
            bgm_wave1 = 0x37;
            Z2GetSeqMgr()->changeBgmStatus(0);
            break;
        case 2:
            bgm_id = Z2BGM_INDOOR;
            bgm_wave1 = 8;
            break;
        case 3:
            bgm_id = Z2BGM_INDOOR_04;
            bgm_wave1 = 0x56;
            break;
        case 4:
            bgm_id = Z2BGM_CASTLE_TOWN;
            bgm_wave1 = 0x25;
            Z2GetSeqMgr()->changeBgmStatus(5);
            break;
        case 5:
            bgm_id = Z2BGM_SHOP02;
            bgm_wave1 = 7;
            break;
        }
        break;
    case Z2SCENE_STAR_TENT:
        se_wave1 = 0x42;
        se_wave2 = 0x87;
            /* dSv_event_flag_c::F_289 - Castle Town - Heard conversation about entering Star Game 1 */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[289])) {
            bgm_id = Z2BGM_MINIGAME_ROOM;
        }
        bgm_wave1 = 0x46;
        break;
    case Z2SCENE_SACRED_GROVE:
        se_wave1 = 0x31;
        bgm_wave2 = 0x1b;
        se_wave2 = 0x45;

        if (room == 3) {
            bgm_id = Z2BGM_HOLY_FOREST;
            bgm_wave1 = 0x2a;
        } else if (room == 2) {
            if (layer == 10) {
                se_wave1 = 0;
                se_wave2 = 0;
                demo_wave = 0x7f;
            } else {
                bgm_id = Z2BGM_TEMPLE_OF_TIME;
                bgm_wave1 = 0x3e;
            }
        } else if (room == 1) {
            if (layer == 0 || layer == 9) {
                demo_wave = 0x76;
            } else if (layer == 8 || layer == 11) {
                se_wave1 = 0;
                se_wave2 = 0;
                demo_wave = 0x7f;
            } else {
                bgm_wave2 = 0x40;
            }
        }
        break;
    case Z2SCENE_SNOWPEAK:
        if (layer == 8) {
            se_wave1 = 0;
            se_wave2 = 0;
            demo_wave = 0x7f;
        } else {
            bgm_id = Z2BGM_SNOW_MOUNTAIN;
            bgm_wave1 = 0x2d;
            bgm_wave2 = 0x3a;
            se_wave1 = 0x46;
            se_wave2 = 0x47;
        }
        height_vol_mod = true;
        break;
    case Z2SCENE_GERUDO_DESERT:
        se_wave1 = 0x48;
        se_wave2 = 0x49;
        if (layer == 8 || layer == 9 || layer == 10) {
            demo_wave = 0x77;
        } else if (layer == 11) {
            se_wave1 = 0;
            se_wave2 = 0;
            demo_wave = 0x7f;
        } else {
            bgm_id = Z2BGM_DESERT;
            bgm_wave1 = 0x1f;
        }
        break;
    case Z2SCENE_ARBITERS_GROUNDS_EXTERIOR:
        if (room == 1) {
            Z2GetSoundObjMgr()->setForceBattleArea(true, 1000, 2000, 9000);
            bgm_wave1 = 0x3d;
            bgm_wave2 = 0x1c;
        }
        se_wave1 = 0x48;
        se_wave2 = 0x49;
        break;
    case Z2SCENE_MIRROR_CHAMBER:
        switch (layer) {
        case 7:
        case 8:
        case 9:
        case 10:
            demo_wave = 0x80;
            break;
        case 11:
        case 12:
            demo_wave = 0x78;
            break;
        case 13:
            demo_wave = 0x7e;
            break;
        default:
            bVar2 = true;
            bgm_id = Z2BGM_DESERT;
            bgm_wave1 = 0x1f;
            break;
        }
        se_wave1 = 0x48;
        se_wave2 = 0x49;
        break;
    case Z2SCENE_HYRULE_FIELD:
        se_wave1 = 0x4a;
        if (room == 10 && layer == 11) {
            demo_wave = 0x74;
            fVar1 = 0.7f;
        } else if (layer == 9 || layer == 10) {
            se_wave1 = 0;
            se_wave2 = 0;
            demo_wave = 0x7f;
        } else if (layer == 2 || layer == 3) {
            bgm_id = Z2BGM_HORSE_BATTLE;
            bgm_wave1 = 0xd;
            se_wave1 = 0x54;
            se_wave2 = 0x4e;
            field_0x1b = true;
            fVar1 = 1.0f;
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        } else if (inDarkness_) {
            if (layer == 12) {
                demo_wave = 0x6a;
            } else {
                bgm_id = Z2BGM_TWILIGHT;
                bgm_wave1 = 0xe;
            }

            se_wave2 = 0x4c;
            if (room == 13 && layer == 14) {
                bgm_wave2 = 0x1c;
            }
        } else {
            field_bgm_play = true;
            if (Z2GetStatusMgr()->checkDayTime()) {
                bgm_id = Z2BGM_FIELD_LINK_DAY;
            } else {
                bgm_id = Z2BGM_FIELD_LINK_NIGHT;
            }

            bgm_wave1 = 0x19;
            switch (room) {
            case 2:
            case 3:
            case 4:
                se_wave2 = 0x5e;
                break;
            case 0:
            case 5:
            case 7:
                se_wave2 = 0x5f;
                break;
            case 9:
            case 10:
            case 11:
                se_wave2 = 0x60;
                break;
            case 12:
            case 13:
            case 14:
                se_wave2 = 0x61;
                break;
            case 1:
            case 6:
            case 15:
                se_wave2 = 0x4b;
                break;
            }
            switch (room) {
            case 0:
            case 3:
            case 6:
            case 10:
            case 13:
                fVar1 = 1.0f;
                break;
            case 1:
            case 2:
            case 4:
            case 5:
            case 14:
            case 15:
                fVar1 = 0.7f;
                break;
            case 7:
            case 9:
            case 11:
            case 12:
                fVar1 = 0.8f;
                break;
            }
        }
        break;
    case Z2SCENE_CASTLE_TOWN_GATES:
        se_wave1 = 0x4a;
        if (layer == 8) {
            demo_wave = 0x73;
            fVar1 = 1.0f;
        } else if (layer == 9) {
            demo_wave = 0x75;
            fVar1 = 0.5f;
        } else if (layer == 10) {
            se_wave1 = 0;
            se_wave2 = 0;
            demo_wave = 0x7f;
        } else if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
            se_wave2 = 0x4c;
        } else {
            if (room == 8) {
                se_wave2 = 0x62;
                fVar1 = 0.8f;
            } else if (room == 16) {
                se_wave2 = 0x88;
                fVar1 = 0.8f;
            } else if (room == 17) {
                se_wave2 = 0x89;
                fVar1 = 1.0f;
            }

            field_bgm_play = true;
            if (Z2GetStatusMgr()->checkDayTime()) {
                bgm_id = Z2BGM_FIELD_LINK_DAY;
            } else {
                bgm_id = Z2BGM_FIELD_LINK_NIGHT;
            }
            bgm_wave1 = 0x19;
        }
        break;
    case Z2SCENE_HYLIA_BRIDGE_BATTLE:
        se_wave1 = 0x54;
        bgm_wave1 = 0x3d;
        se_wave2 = 0x4e;
        fVar1 = 1.0f;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_ELDIN_BRIDGE_BATTLE:
        se_wave1 = 0x4a;
        if (layer == 11) {
            se_wave1 = 0;
            se_wave2 = 0;
            demo_wave = 0x7f;
        } else if (layer == 10) {
            bgm_wave1 = 0x59;
        } else if (layer != 8 && layer != 9) {
            if (layer == 0) {
                bgm_wave1 = 13;
            } else if (layer == 4) {
                bgm_wave1 = 0x3d;
            }
            fVar1 = 1.0f;
            se_wave2 = 0x4d;
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        }
        break;
    case Z2SCENE_SHADES_REALM:
        se_wave1 = 0x4a;
        if (dComIfGs_getStartPoint() == 2) {
            bgm_wave2 = 0x50;
        } else if (dComIfGs_getStartPoint() == 3) {
            bgm_wave2 = 0x4f;
        } else if (dComIfGs_getStartPoint() == 4) {
            bgm_wave2 = 0x3b;
        } else if (dComIfGs_getStartPoint() == 5) {
            bgm_wave2 = 0x51;
        } else if (dComIfGs_getStartPoint() == 6) {
            bgm_wave2 = 0x52;
        } else if (dComIfGs_getStartPoint() == 7) {
            bgm_wave2 = 0x53;
        } else {
            bgm_wave1 = 0x44;
        }

        se_wave2 = 0x5d;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;

    case Z2SCENE_FOREST_TEMPLE:
        bgm_id = Z2BGM_DUNGEON_FOREST;
        bgm_wave1 = 0xa;
        bgm_wave2 = 0x40;
        se_wave1 = 1;
        se_wave2 = 2;
        Z2GetSeqMgr()->changeBgmStatus(room);
        break;
    case Z2SCENE_FOREST_TEMPLE_MINIBOSS:
        bgm_id = Z2BGM_DUNGEON_FOREST;
        bgm_wave1 = 0xa;
        bgm_wave2 = 0xb;
        se_wave1 = 1;
        se_wave2 = 3;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        Z2GetSeqMgr()->unMuteSceneBgm(Z2Param::SCENE_CHANGE_BGM_FADEOUT_TIME);
        break;
    case Z2SCENE_FOREST_TEMPLE_BOSS:
        se_wave1 = 1;
        if (dComIfGs_isStageSwitch(2, 1)) {
            bgm_id = Z2BGM_DUNGEON_FOREST;
            bgm_wave1 = 0xa;
                   /* dSv_event_flag_c::M_022 - Forest Temple - Forest Temple clear (Midna creates warp hole) */
        } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[55])) {
            bgm_id = 0x200005b;
        } else {
            bgm_wave1 = 0xc;
            se_wave2 = 4;
            field_0x1b = true;
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        }
        Z2GetSeqMgr()->unMuteSceneBgm(Z2Param::SCENE_CHANGE_BGM_FADEOUT_TIME);
        break;
    case Z2SCENE_GORON_MINES:
        bgm_id = Z2BGM_DUNGEON_LV2;
        bgm_wave1 = 0x13;
        bgm_wave2 = 0x3c;
        Z2GetSeqMgr()->changeBgmStatus(room);
        se_wave1 = 5;
        se_wave2 = 6;
        break;
    case Z2SCENE_GORON_MINES_MINIBOSS:
        bgm_id = Z2BGM_DUNGEON_LV2;
        bgm_wave1 = 0x13;
        se_wave1 = 5;
        se_wave2 = 7;
        bgm_wave2 = 0x14;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_GORON_MINES_BOSS:
        se_wave1 = 5;
        if (dComIfGs_isStageSwitch(3, 0x7c)) {
            bgm_id = Z2BGM_DUNGEON_LV2;
            bgm_wave1 = 0x13;
                   /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
        } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[64])) {
            bgm_id = 0x200005b;
        } else {
            bgm_wave1 = 0x16;
            se_wave2 = 8;
            field_0x1b = true;
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        }
        break;
    case Z2SCENE_LAKEBED_TEMPLE:
        bgm_id = Z2BGM_DUNGEON_LV3;
        bgm_wave1 = 0x15;
        bgm_wave2 = 0x40;
        se_wave1 = 9;
        se_wave2 = 0xa;
        break;
    case Z2SCENE_LAKEBED_TEMPLE_MINIBOSS:
        bgm_id = Z2BGM_DUNGEON_LV3;
        bgm_wave1 = 0x15;
        bgm_wave2 = 0x17;
        se_wave1 = 9;
        se_wave2 = 0xb;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_LAKEBED_TEMPLE_BOSS:
        se_wave1 = 9;
        if (dComIfGs_isStageSwitch(4, 0xe)) {
            bgm_id = Z2BGM_DUNGEON_LV3;
            bgm_wave1 = 0x15;
                   /* dSv_event_flag_c::M_045 - Lakebed Temple - Lakebed Temple clear */
        } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[78])) {
            bgm_id = 0x200005b;
        } else {
            bgm_wave1 = 0x1e;
            se_wave2 = 0xc;
            field_0x1b = true;
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        }
        break;
    case Z2SCENE_ARBITERS_GROUNDS:
        bgm_id = Z2BGM_DUNGEON_LV4;
        bgm_wave1 = 0x1a;
        bgm_wave2 = 0x41;
        se_wave1 = 0xd;
        se_wave2 = 0xe;
        if (room == 1) {
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        }
        break;
    case Z2SCENE_ARBITERS_GROUNDS_MINIBOSS:
        bgm_id = Z2BGM_DUNGEON_LV4;
        bgm_wave1 = 0x1a;
        bgm_wave2 = 0x22;
        se_wave1 = 0xd;
        se_wave2 = 0xf;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_ARBITERS_GROUNDS_BOSS:
        se_wave1 = 0xd;
        if (dComIfGs_isStageSwitch(0xa, 0xa)) {
            bgm_id = Z2BGM_DUNGEON_LV4;
            bgm_wave1 = 0x1a;
                   /* dSv_event_flag_c::F_0265 - Arbiter's Grounds - Arbiter's Grounds clear */
        } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[265])) {
            bgm_id = 0x200005b;
        } else {
            bgm_wave1 = 0x4c;
            se_wave2 = 0x10;
            field_0x1b = true;
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        }
        break;
    case Z2SCENE_SNOWPEAK_RUINS:
        bgm_id = Z2BGM_DUNGEON_LV5;
        bgm_wave1 = 0x1d;
        bgm_wave2 = 0x40;
        se_wave1 = 0x11;
        se_wave2 = 0x12;
        Z2GetSeqMgr()->changeBgmStatus(room);
        break;
    case Z2SCENE_SNOWPEAK_RUINS_MINIBOSS:
        bgm_id = Z2BGM_DUNGEON_LV5;
        bgm_wave1 = 0x1d;
        bgm_wave2 = 0x38;
        se_wave1 = 0x11;
        se_wave2 = 0x13;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        Z2GetSeqMgr()->changeBgmStatus(room);
        break;
    case Z2SCENE_SNOWPEAK_RUINS_BOSS:
        Z2GetSeqMgr()->unMuteSceneBgm(60);
        se_wave1 = 0x11;
        if (dComIfGs_isStageSwitch(8, 0x19)) {
            bgm_id = Z2BGM_DUNGEON_LV5;
            bgm_wave1 = 0x1d;
                   /* dSv_event_flag_c::F_0266 - Snowpeak Ruins - Snowpeak Ruins clear */
        } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[266])) {
            bgm_id = 0x200005b;
        } else {
            bgm_wave1 = 0x2e;
            se_wave2 = 0x14;
            field_0x1b = true;
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        }
        break;
    case Z2SCENE_TEMPLE_OF_TIME:
        bgm_id = Z2BGM_DUNGEON_LV6;
        bgm_wave1 = 0x26;
        bgm_wave2 = 0x40;
        se_wave1 = 0x15;
        se_wave2 = 0x16;
        break;
    case Z2SCENE_TEMPLE_OF_TIME_MINIBOSS:
        bgm_id = Z2BGM_DUNGEON_LV6;
        bgm_wave1 = 0x26;
        bgm_wave2 = 0x3f;
        se_wave1 = 0x15;
        se_wave2 = 0x17;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_TEMPLE_OF_TIME_BOSS:
        se_wave1 = 0x15;
        if (dComIfGs_isStageSwitch(7, 0x18)) {
            bgm_id = Z2BGM_DUNGEON_LV6;
            bgm_wave1 = 0x26;
                   /* dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear */
        } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[267])) {
            bgm_id = 0x200005b;
        } else {
            bgm_wave1 = 0x4e;
            se_wave2 = 0x18;
            field_0x1b = true;
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        }
        break;
    case Z2SCENE_CITY_IN_THE_SKY:
        if (room == 16) {
            bgm_id = Z2BGM_OBACHAN;
            bgm_wave1 = 0x40;
        } else {
            bgm_id = Z2BGM_DUNGEON_LV7;
            bgm_wave1 = 0x27;
        }
        se_wave1 = 0x19;
        se_wave2 = 0x1a;
        break;
    case Z2SCENE_CITY_IN_THE_SKY_MINIBOSS:
        bgm_id = Z2BGM_DUNGEON_LV7;
        bgm_wave1 = 0x27;
        bgm_wave2 = 0x43;
        se_wave1 = 0x19;
        se_wave2 = 0x1b;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_CITY_IN_THE_SKY_BOSS:
        se_wave1 = 0x19;
        if (dComIfGs_isStageSwitch(0x16, 0x25)) {
            bgm_id = Z2BGM_DUNGEON_LV7;
            bgm_wave1 = 0x27;
                   /* dSv_event_flag_c::F_0268 - City in the Sky - City in the Sky clear */
        } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[268])) {
            bgm_id = 0x200005b;
        } else {
            bgm_wave1 = 0x1e;
            se_wave2 = 0x1c;
            field_0x1b = true;
            Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        }
        break;
    case Z2SCENE_PALACE_OF_TWILIGHT:
        if (layer == 8) {
            if (room == 0 || room == 9) {
                demo_wave = 0x77;
            } else if (room == 11) {
                demo_wave = 0x81;
            }
        } else if (layer == 9) {
            demo_wave = 0x78;
        } else {
            bVar2 = true;
            bgm_id = Z2BGM_DUNGEON_LV8;
            bgm_wave1 = 0x28;
            Z2GetSeqMgr()->changeBgmStatus(room);
        }
        se_wave1 = 0x1d;
        se_wave2 = 0x1e;
        break;
    case Z2SCENE_PALACE_OF_TWILIGHT_MINIBOSS_A:
    case Z2SCENE_PALACE_OF_TWILIGHT_MINIBOSS_B:
        bgm_id = Z2BGM_DUNGEON_LV8;
        bgm_wave1 = 0x28;
        bgm_wave2 = 0x48;
        se_wave1 = 0x1d;
        se_wave2 = 0x1f;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_PALACE_OF_TWILIGHT_THRONE_ROOM:
        if (layer == 8) {
            demo_wave = 0x81;
        } else if (layer == 9) {
            demo_wave = 0x79;
        } else if (dComIfGs_isStageSwitch(0x17, 0x16)) {
            bgm_id = Z2BGM_DUNGEON_LV8;
            bgm_wave1 = 0x28;
                   /* dSv_event_flag_c::F_0570 - Palace of Twilight - Cleared Palace of Twilight */
        } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[570])) {
            bgm_id = 0x200005b;
            JSUList<JAIStream>* stream_list = Z2GetSoundMgr()->getStreamMgr()->getStreamList();
            JSULink<JAIStream>* stream;
            for (stream = stream_list->getFirst(); stream != NULL; stream = stream->getNext()) {
                if (stream->getObject()->getID() == 0x2000046) {
                    bgm_id = -1;
                }
            }
        }
        break;
    case Z2SCENE_PALACE_OF_TWILIGHT_BOSS:
        se_wave1 = 0x1d;
        bgm_wave1 = 0x39;
        se_wave2 = 0x20;

        if (room == 50) {
            inDarkness_ = true;
        }

        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_HYRULE_CASTLE:
        if (room >= 1 && room <= 6) {
            bgm_id = 0x2000040;
            bgm_wave2 = 0x58;
        } else if (room == 8 || room == 12) {
            bgm_id = Z2BGM_DUNGEON_LV9_02;
            bgm_wave1 = 0x29;
            height_vol_mod = true;
        } else {
            Z2GetSeqMgr()->bgmStop(45, 0);
            bgm_wave2 = 0x3d;
        }

        se_wave1 = 0x21;
        se_wave2 = 0x22;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_FINAL_BATTLE_THRONE_ROOM:
        if (room == 50) {
            if (layer == 8) {
                demo_wave = 0x7b;
            } else if (layer == 0) {
                bgm_wave1 = 0x49;
                se_wave2 = 0x23;
            } else if (layer == 10) {
                demo_wave = 0x7b;
            } else if (layer == 1) {
                bgm_wave1 = 0x4a;
                se_wave2 = 0x24;
            } else if (layer == 9) {
                demo_wave = 0x7c;
            }
        }

        se_wave1 = 0x56;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_FINAL_BATTLE_FIELD:
        bgm_wave1 = 0x4b;
        se_wave2 = 0x63;
        se_wave1 = 0x8c;
        field_0x1b = true;
        Z2GetSoundObjMgr()->setForceBattleArea(true, 700, 1100, 1500);
        break;
    case Z2SCENE_FINAL_BATTLE_CUTSCENE:
        if (layer == 8) {
            demo_wave = 0x7c;
            fVar1 = 0.1f;
        } else if (layer == 10) {
            demo_wave = 0x7d;
        } else if (layer == 9) {
            demo_wave = 0x7e;
            fVar1 = 0.08f;
        } else if (layer == 11) {
            demo_wave = 0x7f;
        }
        break;
    case Z2SCENE_LIGHT_SPIRIT_CHAMBER:
        demo_wave = 0x7c;
        break;
    case Z2SCENE_CAVE_OF_ORDEALS:
        se_wave2 = 0x51;
        if (room != 49 || layer < 5 || layer > 8) {
            bgm_id = Z2BGM_SUB_DUNGEON;
            bgm_wave1 = 0x45;
        }
        break;
    case Z2SCENE_ICE_BLOCK_PUZZLE:
        se_wave1 = 0x11;
        se_wave2 = 0x12;
        bgm_id = Z2BGM_SUB_DUNGEON;
        bgm_wave1 = 0x45;
        break;
    case Z2SCENE_GORGE_LANTERN_CAVE:
    case Z2SCENE_LAKE_LANTERN_CAVE:
        se_wave2 = 0x52;
        bgm_id = Z2BGM_SUB_DUNGEON;
        bgm_wave1 = 0x45;
        break;
    case Z2SCENE_ELDIN_MAGNET_CAVE:
        se_wave1 = 5;
        se_wave2 = 6;
        bgm_id = Z2BGM_SUB_DUNGEON;
        bgm_wave1 = 0x45;
        break;
    case Z2SCENE_GROTTO_GRASS_1:
    case Z2SCENE_GROTTO_GRASS_2:
    case Z2SCENE_GROTTO_ROCK_1:
    case Z2SCENE_GROTTO_ROCK_2:
    case Z2SCENE_GROTTO_POND:
        se_wave2 = 0x50;
        bgm_id = Z2BGM_SUB_DUNGEON;
        bgm_wave1 = 0x45;
        break;
    case Z2SCENE_FARON_WOODS_CAVE:
        se_wave2 = 0x53;
        if (inDarkness_) {
            bgm_id = Z2BGM_TWILIGHT;
            bgm_wave1 = 0xe;
        } else {
            bgm_id = Z2BGM_SUB_DUNGEON;
            bgm_wave1 = 0x45;
        }
        break;
    }

        /*dSv_event_flag_c::M_071 - Cutscene - [cutscene: 20] Zant appears (during Midna's desperate hour) */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[104])
            /* dSv_event_flag_c::F_0250 - Cutscene - [cutscene: 21] reunion with Zelda / Midna revived (Hyrule Castle barrier appears) */
        && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[250])
        && demo_wave == 0 && spotNo != Z2SCENE_ELDIN_BRIDGE_BATTLE)
    {
        bgm_wave1 = 0x36;
        if (spotNo == Z2SCENE_CASTLE_TOWN_SHOPS && room == 5) {
            bgm_id = -1;
        } else {
            bgm_id = Z2BGM_MIDNA_SOS;
            if ((spotNo == Z2SCENE_CASTLE_TOWN_INTERIOR && room == 5)
                || spotNo == Z2SCENE_CASTLE_TOWN_SHOPS || spotNo == Z2SCENE_STAR_TENT)
            {
                Z2GetSeqMgr()->muteSceneBgm(33, 0.5f);
            } else {
                Z2GetSeqMgr()->unMuteSceneBgm(33);
            }
        }

        time_proc_vol_mod = false;
    } else if (sceneNum == Z2SCENE_CASTLE_TOWN_SHOPS && roomNum == 1 && !(spotNo == Z2SCENE_CASTLE_TOWN && room == 3)) {
        bgm_id = Z2BGM_FORTUNE;
        bgm_wave1 = 0x37;
        Z2GetSeqMgr()->changeBgmStatus(1);
        time_proc_vol_mod = false;
    }

    if (Z2GetSoundMgr()->getStreamMgr()->isActive()) {
        JAUSoundTable* sound_table = JAUSoundTable::getInstance();
        JSUList<JAIStream>* stream_list = Z2GetSoundMgr()->getStreamMgr()->getStreamList();
        JSULink<JAIStream>* stream;
        for (stream = stream_list->getFirst(); stream != NULL; stream = stream->getNext()) {
            if (bVar2 || sound_table->getTypeID(stream->getObject()->getID()) != 0x71) {
                stream->getObject()->stop(Z2Param::SCENE_CHANGE_BGM_FADEOUT_TIME);
            }
        }
    }

    Z2GetSeqMgr()->setHeightVolMod(height_vol_mod, 30);
    Z2GetSeqMgr()->setTimeProcVolMod(time_proc_vol_mod, 0);
    Z2GetSeqMgr()->setFieldBgmPlay(field_bgm_play);
    Z2GetEnvSeMgr()->initSceneEnvSe(spotNo, room, fVar1);

    if (sceneNum != spotNo || bgm_id != BGM_ID || se_wave1 != loadedSeWave_1
        || se_wave2 != loadedSeWave_2 || bgm_wave1 != loadedBgmWave_1
        || bgm_wave2 != loadedBgmWave_2 || demo_wave != loadedDemoWave)
    {
        sceneNum = spotNo;
        sceneChange(bgm_id, se_wave1, se_wave2, bgm_wave1, bgm_wave2, demo_wave, false);
    }

    roomNum = room;
}

void Z2SceneMgr::sceneChange(JAISoundID bgm, u8 seWave1, u8 seWave2, u8 bgmWave1, u8 bgmWave2,
                             u8 demoWave, bool param_6) {
    OS_REPORT("[Z2SceneMgr::sceneChange] bgm:0x%08x, SeWave1:%d, SeWave2:%d BgmWave1:%d BgmWave2:%d demoWave:%d (%d)\n",
        *(u32*)&bgm, seWave1, seWave2, bgmWave1, bgmWave2, demoWave, param_6);

    if (bgm != BGM_ID) {
        if (Z2GetSeqMgr()->getMainBgmID() == bgm) {
            field_0x1a = false;
        } else {
            field_0x1a = true;
        }
    } else {
        if (Z2GetSeqMgr()->getMainBgmID() == bgm) {
            if (param_6) {
                field_0x1a = true;
            } else {
                field_0x1a = false;
            }
        } else {
            field_0x1a = true;
        }
    }

    requestSeWave_1 = seWave1;
    requestSeWave_2 = seWave2;
    requestBgmWave_1 = bgmWave1;
    requestBgmWave_2 = bgmWave2;
    requestDemoWave = demoWave;
    BGM_ID = bgm;
    Z2GetFxLineMgr()->setSceneFx(sceneNum);
}

void Z2SceneMgr::framework() {
    if (load1stWait > 0) {
        load1stWait--;
        if (load1stWait == 0 && timer == 0) {
            _load1stWaveInner_1();
        }
    } else if (load1stWait < 0) {
        load1stWait++;
        if (load1stWait == 0 && field_0x1a) {
            _load1stWaveInner_2();
        }
    }

    if (isSceneExist()) {
        if (timer < 10000) {
            timer++;
        }
        if (!field_0x1b && Z2GetStatusMgr()->getDemoStatus() != 3 && timer == Z2Param::BATTLE_BGM_WAIT_TIME) {
            Z2GetSeqMgr()->setBattleBgmOff(false);
        }
    } else {
        Z2GetSeqMgr()->setBattleBgmOff(true);
    }
}

void Z2SceneMgr::load1stDynamicWave() {
    OS_REPORT("[Z2SceneMgr::load1stDynamicWave] load1stWait:%d\n", load1stWait);

    timer = 0;
    Z2GetSoundObjMgr()->deleteEnemyAll();
    setSceneExist(false);
    if (load1stWait == 0) {
        _load1stWaveInner_1();
    }
}

void Z2SceneMgr::_load1stWaveInner_1() {
    OS_REPORT("[Z2SceneMgr::_load1stWaveInner_1] requestSe:%d loadedSe:%d\n", requestSeWave_1, loadedSeWave_1);

    Z2GetSeMgr()->seStopAll(0);
    Z2GetEnvSeMgr()->resetSceneInner();
                 /* dSv_event_flag_c::M_071 - Cutscene - [cutscene: 20] Zant appears (during Midna's desperate hour) */
    field_0x18 = dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[104]) ? 0x59 : 0x58;

    if (loadedSeWave_2 != 0 && requestSeWave_2 != loadedSeWave_2) {
        eraseSeWave(loadedSeWave_2);
        loadedSeWave_2 = 0;
    }

    if (loadedDemoWave != 0 && requestDemoWave != loadedDemoWave) {
        if (loadedSeWave_2 != 0 && eraseSeWave(loadedSeWave_2)) {
            loadedSeWave_2 = 0;
        }
        eraseSeWave(loadedDemoWave);
        loadedDemoWave = 0;
    }

    if (field_0x18 != field_0x19 || requestSeWave_1 != loadedSeWave_1) {
        if (loadedSeWave_2 != 0 && eraseSeWave(loadedSeWave_2)) {
            loadedSeWave_2 = 0;
        }
        if (loadedDemoWave != 0 && eraseSeWave(loadedDemoWave)) {
            loadedDemoWave = 0;
        }
        if (loadedSeWave_1 != 0 && eraseSeWave(loadedSeWave_1)) {
            loadedSeWave_1 = 0;
        }
        if (field_0x18 != field_0x19 && eraseSeWave(field_0x19) && loadSeWave(field_0x18)) {
            field_0x19 = field_0x18;
        }
        if (requestSeWave_1 != 0 && loadSeWave(requestSeWave_1)) {
            loadedSeWave_1 = requestSeWave_1;
        } else {
            loadedSeWave_1 = 0;
        }
    }

    if (field_0x1a && Z2GetSeqMgr()->checkBgmPlaying()) {
        OS_REPORT("[Z2SceneMgr::load1stDynamicWave]bgm StopCount = %d\n", 15);
        Z2GetSeqMgr()->bgmStop(15, 0);
        load1stWait = -15;
    } else {
        _load1stWaveInner_2();
    }
}

void Z2SceneMgr::_load1stWaveInner_2() {
    OS_REPORT("[Z2SceneMgr::_load1stWaveInner_2] requestBgm:%d loadedBgm:%d\n", requestBgmWave_1, loadedBgmWave_1);

    if (loadedBgmWave_2 != 0 && requestBgmWave_2 != loadedBgmWave_2) {
        eraseBgmWave(loadedBgmWave_2);
        loadedBgmWave_2 = 0;
    }

    if (requestBgmWave_1 != loadedBgmWave_1) {
        if (loadedBgmWave_2 != 0 && eraseBgmWave(loadedBgmWave_2)) {
            loadedBgmWave_2 = 0;
        }
        if (loadedBgmWave_1 != 0 && eraseBgmWave(loadedBgmWave_1)) {
            loadedBgmWave_1 = 0;
        }
        if (requestBgmWave_1 != 0 && loadBgmWave(requestBgmWave_1)) {
            loadedBgmWave_1 = requestBgmWave_1;
        } else {
            loadedBgmWave_1 = 0;
        }
    }
}

bool Z2SceneMgr::check1stDynamicWave() {
    return load1stWait != 0 || getSeLoadStatus(requestSeWave_1) == 1 || getBgmLoadStatus(requestBgmWave_1) == 1;
}

void Z2SceneMgr::load2ndDynamicWave() {
    OS_REPORT("[Z2SceneMgr::load2ndDynamicWave] requestSe:%d, loadedSe:%d requestBgm:%d loadedBgm:%d requestDemo:%d loadedDemo:%d\n",
        requestSeWave_2, loadedSeWave_2, requestBgmWave_2, loadedBgmWave_2, requestDemoWave, loadedDemoWave);

    if (requestDemoWave != loadedDemoWave) {
        if (loadedSeWave_2 != 0 && eraseSeWave(loadedSeWave_2)) {
            loadedSeWave_2 = 0;
        }
        if (loadedDemoWave != 0 && eraseSeWave(loadedDemoWave)) {
            loadedDemoWave = 0;
        }
        if (requestDemoWave != 0 && requestDemoWave != 0x7f && loadSeWave(requestDemoWave)) {
            loadedDemoWave = requestDemoWave;
        } else {
            loadedDemoWave = 0;
        }
    }

    if (requestSeWave_2 != loadedSeWave_2) {
        if (loadedSeWave_2 != 0 && eraseSeWave(loadedSeWave_2)) {
            loadedSeWave_2 = 0;
        }
        if (requestSeWave_2 != 0 && loadSeWave(requestSeWave_2)) {
            loadedSeWave_2 = requestSeWave_2;
        } else {
            loadedSeWave_2 = 0;
        }
    }

    if (requestBgmWave_2 != loadedBgmWave_2) {
        if (loadedBgmWave_2 != 0 && eraseBgmWave(loadedBgmWave_2)) {
            loadedBgmWave_2 = 0;
        }
        if (requestBgmWave_2 != 0 && loadBgmWave(requestBgmWave_2)) {
            loadedBgmWave_2 = requestBgmWave_2;
        } else {
            loadedBgmWave_2 = 0;
        }
    }
}

void Z2SceneMgr::sceneBgmStart() {
    OS_REPORT("[Z2SceneMgr::sceneBgmStart] %08x\n", *(u32*)&BGM_ID);
    inGame = true;

    int var_r28 = 0;
    setSceneExist(true);
    Z2GetStatusMgr()->setPauseFlag(0);

    if (!field_0x1a && Z2GetSeqMgr()->checkBgmIDPlaying(BGM_ID)) {
        return;
    }

    if (!BGM_ID.isAnonymous() && var_r28 == 0 && Z2GetStatusMgr()->getDemoStatus() != 11) {
        bool var;
        int section = BGM_ID.id_.info.type.parts.sectionID;
        switch (section) {
        case 1:
            var = false;
            switch (Z2GetSeqMgr()->getSubBgmID()) {
            case Z2BGM_BATTLE_NORMAL:
            case Z2BGM_BOOMERAMG_MONKEY:
            case Z2BGM_SUMOMO:
                var = true;
                break;
            }
            Z2GetSeqMgr()->bgmStart(BGM_ID, 0, var);
            Z2GetSeqMgr()->unMuteSceneBgm(0);

            switch (BGM_ID) {
            case Z2BGM_DUNGEON_FOREST:
            case Z2BGM_DUNGEON_LV2:
            case Z2BGM_DUNGEON_LV5:
            case Z2BGM_CASTLE_TOWN:
            case Z2BGM_DUNGEON_LV8:
            case Z2BGM_DUNGEON_LV9_02:
            case Z2BGM_SNOW_MOUNTAIN:
                if (sceneNum == Z2SCENE_CASTLE_TOWN_SHOPS) {
                    Z2GetSeqMgr()->changeBgmStatus(5);
                } else {
                    Z2GetSeqMgr()->changeBgmStatus(roomNum);
                }
                break;
            case Z2BGM_HOLY_FOREST:
            case Z2BGM_LUTERA2:
            case Z2BGM_DEMO08:
            case Z2BGM_DEMO10:
                Z2GetSeqMgr()->changeBgmStatus(0);
                break;
            case Z2BGM_CHURCH2:
                if (sceneNum == Z2SCENE_SANCTUARY_BASEMENT) {
                    Z2GetSeqMgr()->muteSceneBgm(0, 0.5f);
                }
                break;
            case Z2BGM_MIDNA_SOS:
                if ((sceneNum == Z2SCENE_CASTLE_TOWN_INTERIOR && roomNum == 5)
                    || sceneNum == Z2SCENE_CASTLE_TOWN_SHOPS || sceneNum == Z2SCENE_STAR_TENT)
                {
                    Z2GetSeqMgr()->muteSceneBgm(0, 0.5f);
                }
                break;
            case Z2BGM_TOAL_NIGHT:
                if (sceneNum == Z2SCENE_ORDON_INTERIOR && roomNum == 2) {
                    Z2GetSeqMgr()->changeBgmStatus(2);
                } else {
                    Z2GetSeqMgr()->changeBgmStatus(0);
                }
                break;
            case Z2BGM_FILONE_FOREST:
                if (sceneNum == Z2SCENE_CORO_SHOP
                    || (sceneNum == Z2SCENE_FARON_WOODS && roomNum == 14))
                {
                    Z2GetSeqMgr()->changeBgmStatus(2);
                } else {
                    Z2GetSeqMgr()->changeBgmStatus(0);
                }
                break;
            }
            break;

        case 2:
            if (!Z2GetSeqMgr()->bgmStreamCheckReady()) {
                Z2GetSeqMgr()->bgmStreamPrepare(BGM_ID);
            }
            Z2GetSeqMgr()->bgmStreamPlay();
            break;
        }
    }

    Z2GetSeqMgr()->bgmAllUnMute(0);
    field_0x1a = false;
}

void Z2SceneMgr::loadStaticWaves() {
    OS_REPORT("[Z2SceneMgr::loadStaticWaves] 常駐シーン波形を読み込みます\n");
    #if PLATFORM_SHIELD
    loadSceneWave(0, 2);
    #endif
    loadSeWave(0);
    loadBgmWave(0);
    loadSeWave(0x58);
    field_0x18 = 0x58;
    field_0x19 = 0x58;
}

BOOL Z2SceneMgr::checkFirstWaves() {
    return false;
}

bool Z2SceneMgr::eraseSeWave(u32 wave) {
    JAUSectionHeap* sectionHeap = JASGlobalInstance<JAUSectionHeap>::getInstance();
    JUT_ASSERT(2976, sectionHeap);

    JASWaveBank* wave_bank = sectionHeap->getWaveBankTable().getWaveBank(0);
    if (wave_bank != NULL) {
        JASWaveArc* wave_arc = wave_bank->getWaveArc(wave);
        if (wave_arc != NULL) {
            return wave_arc->erase();
        }
    }
    return false;
}

bool Z2SceneMgr::eraseBgmWave(u32 wave) {
    JAUSectionHeap* sectionHeap = JASGlobalInstance<JAUSectionHeap>::getInstance();
    JUT_ASSERT(2988, sectionHeap);

    JASWaveBank* wave_bank = sectionHeap->getWaveBankTable().getWaveBank(1);
    if (wave_bank != NULL) {
        JASWaveArc* wave_arc = wave_bank->getWaveArc(wave);
        if (wave_arc != NULL) {
            return wave_arc->erase();
        }
    }
    return false;
}

s32 Z2SceneMgr::getWaveLoadStatus(u32 wave, u32 bank) {
    JAUSectionHeap* sectionHeap = JASGlobalInstance<JAUSectionHeap>::getInstance();
    JUT_ASSERT(3001, sectionHeap);

    JASWaveBank* wave_bank = sectionHeap->getWaveBankTable().getWaveBank(bank);
    if (wave_bank != NULL) {
        JASWaveArc* wave_arc = wave_bank->getWaveArc(wave);
        if (wave_arc != NULL) {
            return wave_arc->getStatus();
        }
    }
    return 0;
}

#if PLATFORM_SHIELD
bool Z2SceneMgr::loadSceneWave(u32 wave, u32 bank) {
    JAUSectionHeap* sectionHeap = JASGlobalInstance<JAUSectionHeap>::getInstance();
    JUT_ASSERT(3014, sectionHeap);

    JASWaveBank* wave_bank = sectionHeap->getWaveBankTable().getWaveBank(bank);
    if (wave_bank != NULL) {
        JASWaveArc* wave_arc = wave_bank->getWaveArc(wave);
        if (wave_arc != NULL) {
            return wave_arc->load(NULL);
        }
    }
    return false;
}
#endif

bool Z2SceneMgr::loadSeWave(u32 wave) {
    JAUSectionHeap* sectionHeap = JASGlobalInstance<JAUSectionHeap>::getInstance();
    JUT_ASSERT(3030, sectionHeap);

    JASWaveBank* wave_bank = sectionHeap->getWaveBankTable().getWaveBank(0);
    if (wave_bank != NULL) {
        JASWaveArc* wave_arc = wave_bank->getWaveArc(wave);
        if (wave_arc != NULL) {
            return wave_arc->load(NULL);
        }
    }

    JUT_WARN_DEVICE(3038, 1, "Z2SceneMgr::cannot load SE wave:%d\n", wave);
    return false;
}

bool Z2SceneMgr::loadBgmWave(u32 wave) {
    JAUSectionHeap* sectionHeap = JASGlobalInstance<JAUSectionHeap>::getInstance();
    JUT_ASSERT(3047, sectionHeap);

    JASWaveBank* wave_bank = sectionHeap->getWaveBankTable().getWaveBank(1);
    if (wave_bank != NULL) {
        JASWaveArc* wave_arc = wave_bank->getWaveArc(wave);
        if (wave_arc != NULL) {
            return wave_arc->loadTail(NULL);
        }
    }

    JUT_WARN_DEVICE(3055, 1, "Z2SceneMgr::cannot load BGM wave:%d\n", wave);
    return false;
}
