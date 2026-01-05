#include "Z2AudioLib/Z2WolfHowlMgr.h"
#include "Z2AudioLib/Z2Calc.h"
#include "Z2AudioLib/Z2SeqMgr.h"
#include "Z2AudioLib/Z2SeMgr.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "Z2AudioLib/Z2SoundMgr.h"
#include "Z2AudioLib/Z2Audience.h"
#include "d/d_demo.h"

static f32 cPitchDown = 0.8909f;

static f32 cPitchCenter = 1.0f;

static f32 cPitchUp = 1.1892f;

static u8 struct_8045088C = 7;
static u8 struct_8045088D = 7;

static u8 cBeginHowlTime = 30;
static u8 data_8045088F = 0;

static f32 cR_FlatWidth = 0.94387f;  // 1.0f / 12th root of 2

static u8 struct_80450894 = 5;
static u8 struct_80450895 = 30;

static f32 sStickHigh = 0.5f;

static f32 sStickCenter = 1.0f / 5.0f;

static Z2WolfHowlLine sHowlTobikusa[4] = {
    {HOWL_LINE_HIGH, 30},
    {HOWL_LINE_LOW, 30},
    {HOWL_LINE_HIGH, 30},
    {HOWL_LINE_LOW, 30},
};

static Z2WolfHowlLine sHowlUmakusa[6] = {
    {HOWL_LINE_HIGH, 15},
    {HOWL_LINE_MID, 15},
    {HOWL_LINE_LOW, 60},
    {HOWL_LINE_HIGH, 15},
    {HOWL_LINE_MID, 15},
    {HOWL_LINE_LOW, 60},
};

static Z2WolfHowlLine sHowlZeldaSong[6] = {
    {HOWL_LINE_MID, 30},
    {HOWL_LINE_HIGH, 15},
    {HOWL_LINE_LOW, 45},
    {HOWL_LINE_MID, 30},
    {HOWL_LINE_HIGH, 15},
    {HOWL_LINE_LOW, 45},
};

static Z2WolfHowlLine sHowlLightPrelude[6] = {
    {HOWL_LINE_HIGH, 20},
    {HOWL_LINE_LOW, 40},
    {HOWL_LINE_HIGH, 20},
    {HOWL_LINE_LOW, 20},
    {HOWL_LINE_MID, 20},
    {HOWL_LINE_HIGH, 30},
};

static Z2WolfHowlLine sHowlSoulRequiem[6] = {
    {HOWL_LINE_LOW, 30},
    {HOWL_LINE_MID, 15},
    {HOWL_LINE_LOW, 15}, 
    {HOWL_LINE_HIGH, 30},
    {HOWL_LINE_MID, 30},
    {HOWL_LINE_LOW, 30},
};

static Z2WolfHowlLine sHealingSong[6] = {
    {HOWL_LINE_HIGH, 30},
    {HOWL_LINE_MID, 30},
    {HOWL_LINE_LOW, 30},
    {HOWL_LINE_HIGH, 30},
    {HOWL_LINE_MID, 30},
    {HOWL_LINE_LOW, 30},
};

static Z2WolfHowlLine sNewSong1[7] = {
    {HOWL_LINE_LOW, 30},
    {HOWL_LINE_MID, 30},
    {HOWL_LINE_HIGH, 30},
    {HOWL_LINE_LOW, 40},
    {HOWL_LINE_MID, 20},
    {HOWL_LINE_LOW, 30},
    {HOWL_LINE_HIGH, 30},
};

static Z2WolfHowlLine sNewSong2[8] = {
    {HOWL_LINE_HIGH, 20},
    {HOWL_LINE_MID, 20},
    {HOWL_LINE_LOW, 20},
    {HOWL_LINE_MID, 60},
    {HOWL_LINE_LOW, 20},
    {HOWL_LINE_HIGH, 20},
    {HOWL_LINE_MID, 20},
    {HOWL_LINE_LOW, 30},
};

static Z2WolfHowlLine sNewSong3[9] = {
    {HOWL_LINE_MID, 30},
    {HOWL_LINE_LOW, 60},
    {HOWL_LINE_MID, 40},
    {HOWL_LINE_HIGH, 20},
    {HOWL_LINE_LOW, 60},
    {HOWL_LINE_HIGH, 20},
    {HOWL_LINE_MID, 20},
    {HOWL_LINE_HIGH, 20},
    {HOWL_LINE_MID, 45},
};

static Z2WolfHowlData sGuideData[9] = {
    {ARRAY_SIZE(sHowlTobikusa), sHowlTobikusa},
    {ARRAY_SIZE(sHowlUmakusa), sHowlUmakusa},
    {ARRAY_SIZE(sHowlZeldaSong), sHowlZeldaSong},
    {ARRAY_SIZE(sHealingSong), sHealingSong},
    {ARRAY_SIZE(sHowlSoulRequiem), sHowlSoulRequiem},
    {ARRAY_SIZE(sHowlLightPrelude), sHowlLightPrelude},
    {ARRAY_SIZE(sNewSong1), sNewSong1},
    {ARRAY_SIZE(sNewSong2), sNewSong2},
    {ARRAY_SIZE(sNewSong3), sNewSong3},
};

Z2WolfHowlMgr::Z2WolfHowlMgr() : JASGlobalInstance(true) {
    mNowHowlData = NULL;
    mNowInputValue = 0.0f;
    mWolfHowlVolume = 1.0f;
    field_0x1c = 1.0f;
    field_0x20 = cPitchCenter;
    mNowPitch = cPitchCenter;
    field_0x28 = 1.0f;
    mTimer = 0;
    mReleaseTimer = 0;
    mCorrectCurveID = Z2WOLFHOWL_NONE;
    mCorrectScore = -1;
    mNowLineIndex = 0;
    field_0xbb = 0;
    mNowLineScore = 0;

    for (u8 i = 0; i < 20; i++) {
        field_0x92[i] = 0;
    }

    for (u8 i = 0; i < 10; i++) {
        field_0x38[i] = 0.0f;
        field_0x60[i] = 0.0f;
    }

    field_0x34 = 0.0f;
    field_0x30 = 0.0f;
    mGuideData = (Z2WolfHowlData*)&sGuideData;
}

void Z2WolfHowlMgr::resetState() {
    mWolfHowlVolume = 1.0f;
    mNowPitch = cPitchCenter;
    field_0x28 = 1.0f;
    mTimer = 0;
    mReleaseTimer = 0;
    mNowLineIndex = 0;

    for (u8 i = 0; i < 10; i++) {
        field_0x38[i] = 0.0f;
        field_0x60[i] = 0.0f;
    }

    field_0x34 = 0.0f;
    field_0x30 = 0.0f;
}

void Z2WolfHowlMgr::calcVolumeMod(f32) {
    f32 volume;

    if (mWolfHowlHandle) {
        if (mTimer < cBeginHowlTime) {
            mWolfHowlHandle->getAuxiliary().moveVolume(1.0f, 0);
        } else {
            volume = 1.0f;
            if (mReleaseTimer != 0) {
                volume = getParamByExp(mReleaseTimer, 30.0f, 0.0f, 2.0f, 0.0f, 1.0f, Z2Calc::CURVE_POSITIVE);
            }
            mWolfHowlVolume = volume;

            if (mWolfHowlHandle) {
                mWolfHowlHandle->getAuxiliary().moveVolume(mWolfHowlVolume, 0);
            }
            field_0x1c = mWolfHowlVolume;
        }
    }
}

f32 Z2WolfHowlMgr::getNowPitch() {
    if (mWolfHowlHandle) {
        return mWolfHowlHandle->getAuxiliary().params_.mPitch;
    }

    return 1.0f;
}

f32 Z2WolfHowlMgr::getNowInputValue() {
    return mNowInputValue;
}

void Z2WolfHowlMgr::calcPitchMod(f32 param_0, f32 param_1) {
    UNUSED(param_0);

    f32 pitch = 1.0f;
    field_0x20 = getNowPitch();

    if (mReleaseTimer != 0) {
        f32 releaseScale = getParamByExp(mReleaseTimer, 30.0f, 0.0f, 1.0f, 0.97f, 1.0f, Z2Calc::CURVE_NEGATIVE);
        pitch = mNowPitch * releaseScale;

        if (mWolfHowlHandle) {
            mWolfHowlHandle->getAuxiliary().movePitch(pitch, 0);
        }
    } else {
        if (mTimer < cBeginHowlTime) {
            field_0x34 = 0.0f;
        } else {
            f32 inputPitch = 1.0f;
            if (mNowInputValue < 0.0f) {
                inputPitch = Z2Calc::linearTransform(mNowInputValue, -1.0f, 0.0f, cPitchDown, 1.0f, false);
            } else if (mNowInputValue > 0.0f) {
                inputPitch = Z2Calc::linearTransform(mNowInputValue, 1.0f, 0.0f,cPitchUp, 1.0f, false);
            }

            f32 newPitch = inputPitch * cPitchCenter;
            f32 pitchDiff = newPitch - mNowPitch;
            if (pitchDiff == 0.0f) {
                pitch = mNowPitch;
            } else if (pitchDiff > 0.0f) {
                pitch = mNowPitch + (pitchDiff / struct_8045088C);
            } else {
                pitch = mNowPitch + (pitchDiff / struct_8045088D);
            }
        }

        mNowPitch = pitch;

        f32 dVar11 = Z2Calc::linearTransform(param_1, 0.0f, 1.0f, 1.0f, cR_FlatWidth, true);
        f32 fVar5 = field_0x28 + (dVar11 - field_0x28) / struct_80450894;
        field_0x28 = fVar5;
        pitch *= fVar5;

        if (mWolfHowlHandle) {
            mWolfHowlHandle->getAuxiliary().movePitch(pitch, 0);
        }
    }
}

s8 Z2WolfHowlMgr::startWolfHowlSound(f32 param_0, f32 stickValue, bool isHowl, f32 param_3) {
    if (mCorrectCurveID > Z2WOLFHOWL_UMAKUSA) {
        Z2GetSeqMgr()->setWindStoneVol(0.0f, 0);
    }

    if (Z2GetSeqMgr()->isItemGetDemo() || mReleaseTimer != 0) {
        isHowl = false;
    } else {
        if (stickValue > sStickCenter) {
            mNowInputValue = Z2Calc::linearTransform(stickValue, sStickCenter, sStickHigh, 0.0f, 1.0f, false);
        } else if (stickValue < -1.0f * sStickCenter) {
            mNowInputValue = Z2Calc::linearTransform(stickValue, -1.0f * sStickCenter, -1.0f * sStickHigh, 0.0f, -1.0f, false);
        } else {
            mNowInputValue = 0.0f;
        }
    }

    if (isHowl) { 
        if (mTimer > cBeginHowlTime) {
            Z2GetSoundStarter()->startLevelSound(Z2SE_WL_V_HOWL, &mWolfHowlHandle, NULL);
        }

        if (mWolfHowlHandle) {
            calcPitchMod(0.0f, 0.0f);
            calcVolumeMod(param_3);
        }

        mTimer++;
    } else {
         if (mTimer > cBeginHowlTime) {
            if (mReleaseTimer < 30) {
                Z2GetSoundStarter()->startLevelSound(Z2SE_WL_V_HOWL, &mWolfHowlHandle, NULL);
                if (mWolfHowlHandle) {
                    calcPitchMod(0.0f, 0.0f);
                    calcVolumeMod(param_3);
                }
                mReleaseTimer++;
                mTimer++;
            } else {
                resetState();
            }
        } else {
            resetState();
        }
    }

    return checkLine();
}

void Z2WolfHowlMgr::setCorrectData(s8 curveID, Z2WolfHowlData* data) {
    if (curveID == mCorrectCurveID) {
        return;
    }

    if (curveID == Z2WOLFHOWL_NONE) {
        OS_REPORT("[Z2WolfHowMgr:setCorrectData] CorrectData NONE\n");
        mCorrectCurveID = Z2WOLFHOWL_NONE;
        mNowHowlData = NULL;
        return;
    }

    if (data == NULL) {
        if (curveID >= Z2WOLFHOWL_MAX) {
            mCorrectCurveID = Z2WOLFHOWL_NONE;
            mNowHowlData = NULL;
            return;
        }
        data = &mGuideData[curveID];
    }

    JUT_ASSERT(648, data);

    mCorrectCurveID = curveID;
    mNowHowlData = data;

    switch(curveID) {
    case Z2WOLFHOWL_HEALINGSONG:
        cPitchUp = 1.12246f;
        cPitchCenter = 1.05946f;
        cPitchDown = 0.793686f;
        break;
    case Z2WOLFHOWL_SOULREQUIEM:
        cPitchUp = 1.259906f;
        cPitchCenter = 0.94387f;
        cPitchDown = 0.840885f;
        break;
    case Z2WOLFHOWL_NEWSONG1:
        cPitchUp = 1.33484f;
        cPitchCenter = 0.8909f;
        cPitchDown = 0.8909f;
        break;
    case Z2WOLFHOWL_NEWSONG2:
        cPitchUp = 1.259906f;
        cPitchCenter = 0.94387f;
        cPitchDown = 0.8909f;
        break;
    case Z2WOLFHOWL_NEWSONG3:
        cPitchUp = 1.12246f;
        cPitchCenter = 0.94387f;
        cPitchDown = 0.74915f;
        break;
    default:
        cPitchUp = 1.1892f;
        cPitchCenter = 1.0f;
        cPitchDown = 0.8909f;
        break;
    }

    mFullScore = 0;

    u32 length = 0;
    for (u8 i = 0; i < getCorrectLineNum(); i++) {
        length += getCorrectLine(i).length;
        if (getCorrectLine(i).type != 0) {
            mFullScore += struct_80450895;
            mFullScore += getCorrectLine(i).length;
        }
    }

    OS_REPORT("[Z2WolfHowMgr:setCorrectData] id:%d length:%d blockNum:%d, fullScore:%d\n",
        mCorrectCurveID, length, getCorrectLineNum(), mFullScore);
    mFullScore -= FAST_DIV(length, 4);
}

Z2WolfHowlLine Z2WolfHowlMgr::getCorrectLine(u8 lineIndex) {
    u16 local_18;

    if (mNowHowlData == NULL || lineIndex >= mNowHowlData->lineNum) {
        Z2WolfHowlLine line;
        line.type = 0;
        line.length = 0;
        return line; 
    }

    return mNowHowlData->lineData[lineIndex];
}

u8 Z2WolfHowlMgr::getCorrectLineNum() {
    if (mNowHowlData != NULL) {
        return mNowHowlData->lineNum;
    }

    return 0;
}

static JAISoundID sCorrectPhrase[9] = {
    Z2BGM_HOWL_TOBIKUSA,
    Z2BGM_HOWL_UMAKUSA,
    Z2BGM_HOWL_ZELDASONG,
    Z2BGM_HEALING_HOWL,
    Z2BGM_SOUL_REQ_HOWL,
    Z2BGM_HOWL_LIGHT_PRLD,
    Z2BGM_NEW_01_HOWL,
    Z2BGM_NEW_02_HOWL,
    Z2BGM_NEW_03_HOWL,
};

static JAISoundID sWindStoneSound[9] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    Z2BGM_STONE_ZELDASONG,
    Z2BGM_HEALING_STONE,
    Z2BGM_SOUL_REQ_STONE,
    Z2BGM_STONE_LIGHT_PRLD,
    Z2BGM_NEW_01_STONE,
    Z2BGM_NEW_02_STONE,
    Z2BGM_NEW_03_STONE,
};

static JAISoundID sCorrectDuo[9] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    Z2BGM_HEALING_DUO,
    Z2BGM_SOUL_REQ_DUO,
    Z2BGM_LIGHT_PRLD_DUO,
    Z2BGM_NEW_01_DUO,
    Z2BGM_NEW_02_DUO,
    Z2BGM_NEW_03_DUO,
};

s8 Z2WolfHowlMgr::checkLine() {
    if (mCorrectCurveID == Z2WOLFHOWL_NONE || mNowHowlData == NULL) {
        return -1;
    }

    if (Z2GetSeqMgr()->isItemGetDemo()) {
        return mCorrectCurveID;
    }

    if (mTimer == 0) {
        if (mCorrectScore > 0) {
            OS_REPORT("↑↑↑CheckCurveDone!!! correctCount:%d / %d\n", mCorrectScore, mFullScore);
            mCorrectScore = -1;
        }
        return -1;
    } else if (mCorrectScore >= mFullScore) {
        u32 length = 0;
        for (u8 i = 0; i < getCorrectLineNum(); i++) {
            length += getCorrectLine(i).length;
        }

        if (mTimer - cBeginHowlTime < length) {
            return -1;
        } 

        OS_REPORT("↑↑↑CheckCurveDone!!! correctCount:%d / %d\n", mCorrectScore, mFullScore);

        Z2GetSeMgr()->seStart(Z2SE_READ_RIDDLE_B, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

        if (mCorrectCurveID > Z2WOLFHOWL_ZELDASONG) {
            if (Z2GetSceneMgr()->getCurrentSceneNum() == Z2SCENE_SHADES_REALM) {
                Z2GetSeqMgr()->subBgmStart(sCorrectDuo[mCorrectCurveID]);
            }
        } else { 
            Z2GetSeqMgr()->subBgmStart(sCorrectPhrase[mCorrectCurveID]);
        }

        mCorrectScore = -1;
        return mCorrectCurveID;
    } else {
        if (mTimer == 1) {
            mCorrectScore = 0;
            mNowLineIndex = 0;
            mNowLineScore = 0;
            OS_REPORT("↓↓↓CheckCurveStart!!! correctCount:%d, block:%d, length%d\n", mCorrectScore, mNowLineIndex, field_0x92[mNowLineIndex]);
        } else if (mTimer >= cBeginHowlTime) {
            u32 howlTime = mTimer - cBeginHowlTime;

            u32 length = 0;
            for (u8 i = 0; i <= mNowLineIndex; i++) {
                length += getCorrectLine(i).length;
            }

            if (howlTime > length && mNowLineIndex < getCorrectLineNum()) {
                mCorrectScore += mNowLineScore;
                if (mNowLineScore > getCorrectLine(mNowLineIndex).length / 6) {
                    mCorrectScore += struct_80450895;
                }
                mNowLineIndex++;
                mNowLineScore = 0;
            } else {
                if (getOnLineNum() > -1) {
                    mNowLineScore++;
                }
            }
        }
    }

    return -1;
}

s8 Z2WolfHowlMgr::getOnLineNum() {
    Z2WolfHowlLine line = getCorrectLine(mNowLineIndex);
    u8 type = line.type;
    if ((type == HOWL_LINE_HIGH && mNowInputValue == 1.0f) ||
        (type == HOWL_LINE_MID  && mNowInputValue == 0.0f) ||
        (type == HOWL_LINE_LOW  && mNowInputValue == -1.0f))
    {
        s8 rv = mNowLineIndex;
        return rv;
    }

    return -1;
}

void Z2WolfHowlMgr::startWindStoneSound(s8 curveID, Vec* pos) {
    if (pos == NULL || Z2GetLink() == NULL || Z2GetLink()->getCurrentPos() == NULL) {
        return; 
    }

    Vec audioCamPos = Z2GetAudience()->getAudioCamPos(0);
    f32 camDist = sqrtf(VECSquareDistance(pos, &audioCamPos));
    if (camDist > 2100.0f && mWindStoneHandle) {
        mWindStoneHandle->stop();
    } else if (camDist < 2100.0f && !mWindStoneHandle) {
        Z2GetSoundMgr()->startSound(sWindStoneSound[curveID], &mWindStoneHandle, (const JGeometry::TVec3<f32>*)pos);
    }

    if (mWindStoneHandle) {
        mWindStoneHandle->fadeIn(0);
        mWindStoneHandle->stop(30);

        if (Z2GetLink()->getLinkState() == 1) {
            f32 linkDist = sqrtf(VECSquareDistance(pos, Z2GetLink()->getCurrentPos()));
            f32 volume = 1.0f;
            if (linkDist < 150.0f) {
                volume = 0.0f;
            } else if (linkDist < 2000.0f) {
                volume = Z2Calc::getParamByExp(linkDist, 150.0f, 2000.0f, 
                    0.45f, 0.05f, 1.0f, Z2Calc::CURVE_NEGATIVE);
            }

            Z2GetSeqMgr()->setWindStoneVol(volume, 0);
            mWindStoneHandle->getAuxiliary().moveVolume(0.4f, 0);
        } else {
            mWindStoneHandle->getAuxiliary().moveVolume(0.04f, 0);
        }
    }
}

int Z2WolfHowlMgr::startGuideMelody(bool startSound) {
    Z2GetSeqMgr()->setWindStoneVol(0.0f, 0);

    if (startSound) {
        Z2GetSoundMgr()->startSound(sWindStoneSound[mCorrectCurveID], &mGuideHandle, NULL);
    }

    if (mGuideHandle) {
        mGuideHandle->fadeIn(0);
        mGuideHandle->stop(30);
        return mGuideHandle->getCount();
    }

    return 0;
}

void Z2WolfHowlMgr::skipCorrectDemo() {
    Z2GetSeqMgr()->stopWolfHowlSong();
}
