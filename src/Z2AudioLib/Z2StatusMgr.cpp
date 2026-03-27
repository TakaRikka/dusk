#include "Z2AudioLib/Z2StatusMgr.h"
#include "Z2AudioLib/Z2Param.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include <cstring>
#include "Z2AudioLib/SpotName.h"

Z2StatusMgr::Z2StatusMgr() : JASGlobalInstance(true) {
    mHour = 0;
    mMinute = 0;
    mWeekday = 0;
    field_0x03 = 0;
    mTime = 3072;
    mEventBit = NULL;
    mPauseFlag = false;
    mCameraMapInfo = 0;

    mPolygonPosition.x = 10000000.0f;
    mPolygonPosition.y = 10000000.0f;
    mPolygonPosition.z = 10000000.0f;

    mUnderwaterDepth = 0.0f;
    mCameraInWaterDepthRatio = 0.0f;

    mDemoStatus = 0;
    mHeartGaugeOn = 0;
}

void Z2StatusMgr::heartGaugeOn() {
    mHeartGaugeOn = 2;
}

void Z2StatusMgr::processHeartGaugeSound() {
    if (!dComIfGp_event_runCheck() && mHeartGaugeOn != 0) {
        if (!dScnPly_c::isPause()) {
            mHeartGaugeOn--;
        }

        if (Z2GetLink() == NULL) {
            return;
        }
    
        u8 linkHp = Z2GetLink()->getLinkHp();
        if (linkHp == 0) {
            return;
        }
    
        if (Z2GetSeqMgr()->isItemGetDemo()) {
            return;
        }
    
        if (linkHp <= 2) {
            Z2GetSeMgr()->seStartLevel(Z2SE_ALMOST_DIE_ALERM_3, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else if (linkHp <= 4) {
            Z2GetSeMgr()->seStartLevel(Z2SE_ALMOST_DIE_ALERM_2, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else if (linkHp <= 6) {
            Z2GetSeMgr()->seStartLevel(Z2SE_ALMOST_DIE_ALERM_1, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    }
}

void Z2StatusMgr::talkIn() {
    if (!isMovieDemo()) {
        Z2GetSeqMgr()->talkInBgm();
        Z2GetSeMgr()->talkInSe();
    }
}

void Z2StatusMgr::talkOut() {
    if (!isMovieDemo()) {
        Z2GetSeqMgr()->talkOutBgm();
        Z2GetSeMgr()->talkOutSe();
    }
}

void Z2StatusMgr::menuIn() {
    Z2GetSeqMgr()->menuInBgm();
    Z2GetSeMgr()->menuInSe();

    mPauseFlag = true;
}

void Z2StatusMgr::menuOut() {
    Z2GetSeqMgr()->menuOutBgm();
    Z2GetSeMgr()->talkOutSe();

    mPauseFlag = false;
}

bool Z2StatusMgr::isMovieDemo() {
    return mDemoStatus == 2 || mDemoStatus == 8 || mDemoStatus == 9;
}

void Z2StatusMgr::setDemoName(char* demoName) {
    if (mDemoStatus == 11) {
        mDemoStatus = 0;
        if (Z2GetSceneMgr()->isInGame()) {
            Z2GetSceneMgr()->sceneBgmStart();
        }
    }

    if (demoName == NULL) {
        OS_REPORT("[Z2StatusMgr::setDemoName] → No demo\n");
        if (mDemoStatus == 3) {
            Z2GetSeqMgr()->setBattleBgmOff(false);
        } else if (mDemoStatus == 4) {
            Z2GetSeqMgr()->unMuteSceneBgm(Z2Param::BGM_CROSS_FADEIN_TIME);
        } else if (mDemoStatus == 5) {
            OS_REPORT("[Z2StatusMgr::setDemoName] HIDDEN_VIL DEMO_END\n");
            Z2GetSeqMgr()->subBgmStop();
        } else if (mDemoStatus == 9) {
            Z2GetSeqMgr()->bgmStreamStop(60);
        }

        if (mDemoStatus != 8) {
            mDemoStatus = 0;
        }
    } else {
        OS_REPORT("[Z2StatusMgr::setDemoName] demoName : %s\n", demoName);

        bool bVar1 = false;
        if (mDemoStatus == 8) {
            if (strcmp(demoName, "force_end") == 0) {
                mDemoStatus = 0;
            }
        } else if (strcmp(demoName, "SAVEREQ") == 0 || strcmp(demoName, "SAVEGID") == 0) {
            mDemoStatus = 11;
        } else if (strcmp(demoName, "demo36_01") == 0 || strcmp(demoName, "demo36_02") == 0) {
            mDemoStatus = 4;
            Z2GetSeqMgr()->muteSceneBgm(0x1e, 0.0f);
        } else if (strcmp(demoName, "demo37_01") == 0) {
            mDemoStatus = 12;
            Z2GetSoundMgr()->getSeMgr()->getCategory(9)->getParams()->moveVolume(0.0f, 0);
        } else if (strncmp(demoName, "demo31", 6) == 0) {
            mDemoStatus = 13;
            Z2GetSeMgr()->seMoveVolumeAll(0.0f, 0);
        } else if (strncmp(demoName, "demo", 4) == 0) {
            mDemoStatus = 2;
        } else if (strcmp(demoName, "force_start") == 0) {
            mDemoStatus = 8;
        } else if (strcmp(demoName, "force_end") == 0) {
            mDemoStatus = 0;
        } else if (strcmp(demoName, "KAGO_HIKARI") == 0) {
            mDemoStatus = 6;
        } else if (strcmp(demoName, "KAGO_YAMI") == 0) {
            mDemoStatus = 7;
        } else if (strcmp(demoName, "HYRULEOP") == 0) {
            mDemoStatus = 10;
        } else {
            switch (Z2GetSceneMgr()->getCurrentSceneNum()) {
            case Z2SCENE_HIDDEN_VILLAGE:
                if (strcmp(demoName, "R00_start") == 0) {
                    mDemoStatus = 5;
                    Z2GetSeqMgr()->subBgmStart(Z2BGM_HIDDEN_VIL_D1);
                    OS_REPORT("[Z2StatusMgr::setDemoName] HIDDEN_VIL DEMO_START\n");
                }
                break;
            case Z2SCENE_FARON_WOODS:
                if (strcmp(demoName, "B_W_START") == 0) {
                    Z2GetSeqMgr()->bgmStreamPrepare(0x200005f);
                    Z2GetSeqMgr()->bgmStreamPlay();
                    mDemoStatus = 9;
                }
                break;
            case Z2SCENE_TWILIGHT_HYRULE_CASTLE:
                if (strcmp(demoName, "L0_rampart01") == 0) {
                    bVar1 = true;
                }
                break;
            case Z2SCENE_FOREST_TEMPLE:
                if (strcmp(demoName, "R22-opening") == 0) {
                    bVar1 = true;
                }
                break;
            case Z2SCENE_GORON_MINES:
                if (strcmp(demoName, "R01-start") == 0) {
                    bVar1 = true;
                }
                break;
            case Z2SCENE_LAKEBED_TEMPLE:
                if (strcmp(demoName, "LV3R00OP") == 0) {
                    bVar1 = true;
                }
                break;
            case Z2SCENE_ARBITERS_GROUNDS:
                if (strcmp(demoName, "R00_start") == 0) {
                    bVar1 = true;
                }
                break;
            }

            if (bVar1) {
                Z2GetSeqMgr()->setBattleBgmOff(true);
                mDemoStatus = 3;
            } else if (mDemoStatus == 0) {
                mDemoStatus = 1;
            }
        }
    }
}

void Z2StatusMgr::processTime() {
    u16 processed_hour = mHour * 256;
    mTime = processed_hour + mMinute;
}

bool Z2StatusMgr::checkDayTime() {
    if (mHour >= 6 && mHour < 19) {
        return true;
    }

    return false;
}

void Z2StatusMgr::setEventBit(void* eventBit) {
    mEventBit = eventBit;
}

void Z2StatusMgr::setCameraPolygonPos(Vec* polygonPos) {
    if (polygonPos != NULL) {
        mPolygonPosition = *polygonPos;
    }
}

void Z2StatusMgr::setCameraGroupInfo(u8) {}

void Z2StatusMgr::setCameraInWaterDepth(f32 depth) {
    if (0.0f == depth) {
        mCameraInWaterDepthRatio = 0.0f;
        Z2GetFxLineMgr()->setUnderWaterFx(false);
    } else {
        if (Z2GetLink() != NULL) {
            if (Z2GetLink()->isInWater()) {
                if (depth >= 2000.0f) {
                    mCameraInWaterDepthRatio = 1.0f;
                } else {
                    mCameraInWaterDepthRatio =
                        Z2Calc::getParamByExp(depth, 0.0f, 2000.0f, 0.2f, 0.0f, 1.0f, Z2Calc::CURVE_NEGATIVE);
                }
                Z2GetFxLineMgr()->setUnderWaterFx(true);
            }
        }
    }

    mUnderwaterDepth = depth;
}
