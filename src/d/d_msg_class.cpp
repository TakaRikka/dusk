#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_class.h"
#include <stdio.h>
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_msg_unit.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "d/d_msg_out_font.h"
#include "m_Do/m_Do_graphic.h"
#include "d/d_lib.h"
#include "JSystem/JUtility/JUTFont.h"

#if REGION_JPN
#define CHAR_CODE_MALE_ICON 0x8189
#define CHAR_CODE_FEMALE_ICON 0x818A
#define CHAR_CODE_STAR_ICON 0x819A
#define CHAR_CODE_REFMARK 0x81A6
#define CHAR_CODE_THIN_LEFT_ARROW 0x81A9
#define CHAR_CODE_THIN_RIGHT_ARROW 0x81A8
#define CHAR_CODE_THIN_UP_ARROW 0x81AA
#define CHAR_CODE_THIN_DOWN_ARROW 0x81AB
#else
#define CHAR_CODE_MALE_ICON 0xB2
#define CHAR_CODE_FEMALE_ICON 0xB3
#define CHAR_CODE_STAR_ICON 0xB1
#define CHAR_CODE_REFMARK 0x89
#define CHAR_CODE_THIN_LEFT_ARROW 0xB9
#define CHAR_CODE_THIN_RIGHT_ARROW 0xBC
#define CHAR_CODE_THIN_UP_ARROW 0xBD
#define CHAR_CODE_THIN_DOWN_ARROW 0xBE
#endif

static bool checkCharInfoCharactor(int c) {
    if (c != 0x8140 && c != 0x8141 && c != 0x8142 && c != 0x0020 && c != 0x0022 && c != 0x0027 &&
        c != 0x002C && c != 0x002E)
    {
        return true;
    }

    return false;
}

static char* changeCodeToChar(u16 param_0) {
    static char mMoji[3];

    mMoji[0] = mMoji[1] = mMoji[2] = 0;
    mMoji[0] = (param_0 & 0xFF00) >> 8;

    if (mMoji[0] == 0) {
        mMoji[0] = param_0 & 0xFF;
    } else {
        mMoji[1] = param_0 & 0xFF;
    }

    return mMoji;
}

static u16 change1ByteTo2Bytes(int iCharacter) {
    static u16 char_table[] = {
        0x0000, 0x8142, 0x8175, 0x8176, 0x8141, 0x8145, 0x8392, 0x8340, 0x8342, 0x8344, 0x8346,
        0x8348, 0x8383, 0x8385, 0x8387, 0x8362, 0x815B, 0x8341, 0x8343, 0x8345, 0x8347, 0x8349,
        0x834A, 0x834C, 0x834E, 0x8350, 0x8352, 0x8354, 0x8356, 0x8358, 0x835A, 0x835C, 0x835E,
        0x8360, 0x8363, 0x8365, 0x8367, 0x8369, 0x836A, 0x836B, 0x836C, 0x836D, 0x836E, 0x8371,
        0x8374, 0x8377, 0x837A, 0x837D, 0x837E, 0x8380, 0x8381, 0x8382, 0x8384, 0x8386, 0x8388,
        0x8389, 0x838A, 0x838B, 0x838C, 0x838D, 0x838F, 0x8393, 0x814A, 0x814B,
    };

    u16 character2Bytes = iCharacter;
    if (iCharacter >= '0' && iCharacter <= '9') {
        character2Bytes = iCharacter + 0x821F;
    } else if (iCharacter >= 'A' && iCharacter <= 'Z') {
        character2Bytes = iCharacter + 0x8230;
    } else if (iCharacter >= 'a' && iCharacter <= 'z') {
        character2Bytes = iCharacter + 0x8251;
    } else if (iCharacter >= 0xA0 && iCharacter <= 0xDF) {
        int sp8 = iCharacter - 0xA0;
        character2Bytes = char_table[sp8];
    } else {
        switch (iCharacter) {
        case ' ':
            character2Bytes = 0x8140;
            break;
        case '!':
            character2Bytes = 0x8149;
            break;
        case '\"':
            character2Bytes = 0x8168;
            break;
        case '#':
            character2Bytes = 0x8194;
            break;
        case '$':
            character2Bytes = 0x8190;
            break;
        case '%':
            character2Bytes = 0x8193;
            break;
        case '&':
            character2Bytes = 0x8195;
            break;
        case '\'':
            character2Bytes = 0x8166;
            break;
        case '(':
            character2Bytes = 0x8169;
            break;
        case ')':
            character2Bytes = 0x816a;
            break;
        case '*':
            character2Bytes = 0x8196;
            break;
        case '+':
            character2Bytes = 0x817b;
            break;
        case ',':
            character2Bytes = 0x8143;
            break;
        case '-':
            character2Bytes = 0x817c;
            break;
        case '.':
            character2Bytes = 0x8144;
            break;
        case '/':
            character2Bytes = 0x815e;
            break;
        case ':':
            character2Bytes = 0x8146;
            break;
        case ';':
            character2Bytes = 0x8147;
            break;
        case '<':
            character2Bytes = 0x8171;
            break;
        case '=':
            character2Bytes = 0x8181;
            break;
        case '>':
            character2Bytes = 0x8172;
            break;
        case '?':
            character2Bytes = 0x8148;
            break;
        case 'K':
            character2Bytes = 0x816d;
            break;
        case 'L':
            character2Bytes = 0x818f;
            break;
        case 'M':
            character2Bytes = 0x816e;
            break;
        case 'N':
            character2Bytes = 0x813f;
            break;
        case 'O':
            character2Bytes = 0x8151;
            break;
        case '[':
            character2Bytes = 0x816f;
            break;
        case '\\':
            character2Bytes = 0x8162;
            break;
        case ']':
            character2Bytes = 0x8170;
            break;
        case '^':
            character2Bytes = 0x8140;
            break;
        case '_':
            character2Bytes = 0;
            break;
        }
    }

    return character2Bytes;
}

static u16 changeKataToHira(int iCharacter) {
    u16 hiraCharacter = iCharacter;

    if (iCharacter >= 0x8340 && iCharacter <= 0x837E) {
        hiraCharacter = iCharacter - 0xA1;
    } else if (iCharacter >= 0x8380 && iCharacter <= 0x8393) {
        hiraCharacter = iCharacter - 0xA2;
    } else if (iCharacter == 0x8394) {
        hiraCharacter = 0x82D4;
    }

    return hiraCharacter;
}

static bool isOutfontKanjiCode(int iCharacter) {
    switch (iCharacter) {
    case '\\':
    case '@':
    case '$':
    case 0x81F2:
    case 0x81F3:
        return true;
    default:
        return false;
    }
}

static u32 getFontCCColorTable(u8 i_colorNo, u8 i_fukiKind) {
    static const u32 colorTable[9] = {
        0xFFFFFFFF, 0xF07878FF, 0xAADC8CFF, 0xA0B4DCFF, 0xDCDC82FF,
        0xB4C8E6FF, 0xC8A0DCFF, 0xFFFFFFFF, 0xDCAA78FF,
    };

    if (i_colorNo > 8) {
        return 0xFFFFFFFF;
    }

    if (g_MsgObject_HIO_c.mTextColorDebug) {
        return (g_MsgObject_HIO_c.mTextColorUpperR[i_colorNo] << 0x18) |
               (g_MsgObject_HIO_c.mTextColorUpperG[i_colorNo] << 0x10) |
               (g_MsgObject_HIO_c.mTextColorUpperB[i_colorNo] << 0x8) | 0xFF;
    }

    u32 color = colorTable[i_colorNo];

    if (i_colorNo != 0) {
        return color;
    }

    switch (i_fukiKind) {
    case 13:
        return 0x82E6E6FF;
    case 8:
        return 0xFFFFFFFF;
    case 14:
        return 0x96DC64FF;
    default:
        return color;
    }
}

static u32 getFontGCColorTable(u8 i_colorNo, u8 i_fukiKind) {
    static const u32 colorTable[9] = {
        0xFFFFFFFF, 0xF07878FF, 0xAADC8CFF, 0xA0B4DCFF, 0xDCDC82FF,
        0xB4C8E6FF, 0xC8A0DCFF, 0xFFFFFFFF, 0xDCAA78FF,
    };

    if (i_colorNo > 8) {
        return 0xFFFFFFFF;
    }

    if (g_MsgObject_HIO_c.mTextColorDebug) {
        return (g_MsgObject_HIO_c.mTextColorLowerR[i_colorNo] << 0x18) |
               (g_MsgObject_HIO_c.mTextColorLowerG[i_colorNo] << 0x10) |
               (g_MsgObject_HIO_c.mTextColorLowerB[i_colorNo] << 0x8) | 0xFF;
    }

    u32 color = colorTable[i_colorNo];

    if (i_colorNo != 0) {
        return color;
    }

    switch (i_fukiKind) {
    case 13:
        return 0x82E6E6FF;
    case 8:
        return 0xFFFFFFFF;
    case 14:
        return 0x96DC64FF;
    default:
        return color;
    }
}

static u8 getOutFontNumberType(int param_0) {
    switch (param_0) {
    case 0:
        return 0x1F;
    case 1:
        return 0x20;
    case 2:
        return 0x21;
    case 3:
        return 0x22;
    case 4:
        return 0x23;
    case 5:
        return 0x24;
    case 6:
        return 0x25;
    case 7:
        return 0x26;
    case 8:
        return 0x27;
    case 9:
        return 0x28;
    default:
        return 0x1F;
    }
}

#if VERSION == VERSION_GCN_PAL
static void setPlayerName(char* i_player_name, u8 param_2) {
    if (param_2 != 0) {
        strcpy(i_player_name, dComIfGs_getPlayerName());
        u32 name_length = strlen(i_player_name);
        char last = i_player_name[name_length - 1];
        if (last == 0x73 || last == 0x53 || last == 0x7a || last == 0x5a || last == 0x78 || last == 0x58 || last == 0xdf) {
            strcat(i_player_name, "'");
        } else {
            strcat(i_player_name, "s");
        }
    } else {
        strcpy(i_player_name, dComIfGs_getPlayerName());
    }
}

static void setHorseName(char* i_horse_name, u8 param_2) {
    if (param_2 != 0) {
        strcpy(i_horse_name, dComIfGs_getHorseName());
        u32 name_length = strlen(i_horse_name);
        char last = i_horse_name[name_length - 1];
        if (last == 0x73 || last == 0x53 || last == 0x7a || last == 0x5a || last == 0x78 || last == 0x58 || last == 0xdf) {
            strcat(i_horse_name, "'");
        } else {
            strcat(i_horse_name, "s");
        }
    } else {
        strcpy(i_horse_name, dComIfGs_getHorseName());
    }
}
#endif

static u8 getPohNum() {
    u8 num = 0;

    if (dComIfGs_getPohSpiritNum() < 20) {
        num = 20 - dComIfGs_getPohSpiritNum();
    } else if (dComIfGs_getPohSpiritNum() < 60) {
        num = 60 - dComIfGs_getPohSpiritNum();
    }

    return num;
}

jmessage_tReference::jmessage_tReference() {
    mpStick = new STControl(5, 2, 3, 2, 0.9f, 0.5f, 0, 0x2000);
    JUT_ASSERT(518, mpStick != NULL);
    mpStick->setWaitParm(5, 2, 3, 2, 0.9f, 0.5f, 0, 0x800);

    mActorPos.set(0.0f, 0.0f, 0.0f);

    mpSelMsgPtr = NULL;
    mpFont = NULL;
    mFontSizeX = 0.0f;
    mFontSizeY = 0.0f;
    mRubySize = 0.0f;
    mLineSpace = 0.0f;
    mCharSpace = 0.0f;
    mRubyCharSpace = 0.0f;
    mSelFontSize = 0.0f;
    mSelRubySize = 0.0f;
    mSelTBoxWidth = 0.0f;
    mSelCharSpace = 0.0f;
    mSelCharSpace = 0.0f;
    mDistanceScale = 1.0f;
    mAddCharAllAlphaRate = 0.0f;
    mCharAllAlphaRate = 0.0f;
    mAddCharAlpha = 0.0f;
    mCharAlpha = 0.0f;
    mStartLineCount = 0;
    mEndLineCount = 0;
    mLineCount = 0;
    field_0x5d2 = 0;
    mPageNum = 0;
    mSendTimer = 0;
    mDemoFrame = 0;
    mCharCnt = 0;
    mNowLightCount = 0;
    mDrawLightCount = 0;
    resetCharCountBuffer();
    mNowColorType = 0;
    mTopColorType = 0;
    mButtonTagStopFlag = 0;
    mPageEndCount = 0;
    mSelectNum = 0;
    mSelectType = 0;
    mSelectPos = 0;
    mSendFlag = 0;
    mStopFlag = 0;
    mpStatus = NULL;
    mpObjectPtr = NULL;
    mSelectRubyFlag = 0;
    mInputFigure = 0;
    mOutFontRupeeColor = 0;
    mNowWordCount = 0;
    mTopWordCount = 0;
    resetCharactor();
    resetWord();
    offSelectSetCancelFlag();
    offBombNameUseFlag();
    setRevoMessageID(0);
    mBatchColorFlag = 0;
    mTopTagScale = 0;
    mNowTagScale = 0;
}

jmessage_tReference::~jmessage_tReference() {
    delete mpStick;
    mpStick = NULL;
}

void jmessage_tReference::calcDistance() {
    if (mActorPos.x == 0.0f && mActorPos.y == 0.0f && mActorPos.z == 0.0f) {
        mDistanceScale = 1.0f;
    } else {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        f32 var_f31 = (mActorPos - player->attention_info.position).absXZ();
        mDistanceScale = 1.0f;
    }
}

u8 jmessage_tReference::getLineMax() {
    int line_max;

#if REGION_JPN
    if (isKanban()) {
        line_max = 6;
    } else if (isBook()) {
        line_max = 7;
    } else if (isStaffRoll()) {
        line_max = 10;
    } else if (isSaveSeq()) {
        line_max = 5;
    } else {
        line_max = 3;
    }
#else
    if (isKanban()) {
        line_max = 7;
    } else if (isBook()) {
        line_max = 9;
    } else if (isStaffRoll()) {
        line_max = 10;
    } else if (isSaveSeq()) {
        line_max = 6;
    } else {
        line_max = 4;
    }
#endif

    return line_max;
}

bool jmessage_tReference::isKanban() {
    if (getObjectPtr() == NULL) {
        return false;
    }

    return getObjectPtr()->isKanbanMessage();
}

bool jmessage_tReference::isPlaceName() {
    if (getObjectPtr() == NULL) {
        return false;
    }

    return getObjectPtr()->isPlaceMessage();
}

bool jmessage_tReference::isBossName() {
    if (getObjectPtr() == NULL) {
        return false;
    }

    return getObjectPtr()->isBossMessage();
}

bool jmessage_tReference::isSaveSeq() {
    if (getObjectPtr() == NULL) {
        return false;
    }

    return getObjectPtr()->isSaveMessage();
}

bool jmessage_tReference::isBook() {
    if (getObjectPtr() == NULL) {
        return false;
    }

    return getObjectPtr()->isBookMessage();
}

bool jmessage_tReference::isStaffRoll() {
    if (getObjectPtr() == NULL) {
        return false;
    }

    return getObjectPtr()->isStaffMessage();
}

bool jmessage_tReference::isHowl() {
    if (getObjectPtr() == NULL) {
        return false;
    }

    return getObjectPtr()->isHowlMessage();
}

bool jmessage_tReference::isMidona() {
    if (getObjectPtr() == NULL) {
        return false;
    }

    return getObjectPtr()->isMidonaMessage();
}

void jmessage_tReference::resetReference() {
    mSendTimer = 0;
    mSendFlag = 0;
    mStartLineCount = 0;
    mEndLineCount = 0;
    mLineCount = 0;
    field_0x5d2 = 0;
    mSelectNum = 0;
    mSelectType = 0;
}

void jmessage_tReference::pageSend() {
    field_0x5d2++;
    mLineCount = 0;
    mStartLineCount = mEndLineCount;

    mText[0] = 0;
    mTextS[0] = 0;
    mRuby[0] = 0;

    for (int i = 0; i < 3; i++) {
        mSelText[i][0] = 0;
        mSelRuby[i][0] = 0;
    }

    mSendFlag = 0;
    mSendTimer = 0;
    mSelectNum = 0;
    mSelectType = 0;
    mCharCnt = 0;

    if (mNowColorType != mTopColorType) {
        mTopColorType = mNowColorType;
    }

    mTopWordCount = mNowWordCount;
    mCharAlpha = 0.0f;

    if (getForm() == 8) {
        setAddCharAllAlphaRate(1.0f / (g_MsgObject_HIO_c.mBatchDisplayWeight_8 + 1.0f));
        setCharAllAlphaRate(0.0f);
    } else if (getForm() == 9) {
        setAddCharAllAlphaRate(1.0f / (g_MsgObject_HIO_c.mBatchDisplayWeight_9 + 1.0f));
        setCharAllAlphaRate(0.0f);
    }

    mTopTagScale = mNowTagScale;
}

void jmessage_tReference::selectMessage() {
    if (mSelectNum != 0) {
        mpStick->checkTrigger();

        if (mSelectType == 0) {
            if (mpStick->checkUpTrigger()) {
                if (mSelectNum == 2) {
                    if (mSelectPos == 1) {
                        mSelectPos = 0;
                        Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f,
                                                        -1.0f, -1.0f, 0);
                    }
                } else if (mSelectNum == 3 && mSelectPos != 0) {
                    mSelectPos--;
                    Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f,
                                                    -1.0f, -1.0f, 0);
                }
            } else if (mpStick->checkDownTrigger()) {
                if (mSelectNum == 2) {
                    if (mSelectPos == 0) {
                        mSelectPos = 1;
                        Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f,
                                                        -1.0f, -1.0f, 0);
                    }
                } else if (mSelectNum == 3 && mSelectPos < 2) {
                    mSelectPos++;
                    Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f,
                                                    -1.0f, -1.0f, 0);
                }
            }
        } else if (mpStick->checkLeftTrigger()) {
            if (mSelectPos == 1) {
                mSelectPos = 0;
                getObjectPtr()->getSequenceProcessor()->calcStringLength();
                Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                                -1.0f, 0);
            } else if (mSelectPos == 2) {
                mSelectPos = 1;
                getObjectPtr()->getSequenceProcessor()->calcStringLength();
                Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                                -1.0f, 0);
            }
        } else if (mpStick->checkRightTrigger()) {
            if (mSelectPos == 0) {
                mSelectPos = 1;
                getObjectPtr()->getSequenceProcessor()->calcStringLength();
                Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                                -1.0f, 0);
            } else if (mSelectPos == 1 && mSelectNum == 3) {
                mSelectPos = 2;
                getObjectPtr()->getSequenceProcessor()->calcStringLength();
                Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                                -1.0f, 0);
            }
        }
    }
}

void jmessage_tReference::inputNumber() {
    mpStick->checkTrigger();

    s16 input_val, new_input_val;
    input_val = new_input_val = getObjectPtr()->getInputValue();

    u8 max_figure = 0;
    if (dComIfGs_getRupee() >= 1000) {
        max_figure = 3;
    } else if (dComIfGs_getRupee() >= 100) {
        max_figure = 2;
    } else if (dComIfGs_getRupee() >= 10) {
        max_figure = 1;
    }

    if (mpStick->checkUpTrigger()) {
        switch (mInputFigure) {
        case 0:
            new_input_val = input_val + 1;
            break;
        case 1:
            new_input_val = input_val + 10;
            break;
        case 2:
            new_input_val = input_val + 100;
            break;
        case 3:
            new_input_val = input_val + 1000;
            break;
        }
    } else if (mpStick->checkDownTrigger()) {
        switch (mInputFigure) {
        case 0:
            new_input_val = input_val - 1;
            break;
        case 1:
            new_input_val = input_val - 10;
            break;
        case 2:
            new_input_val = input_val - 100;
            break;
        case 3:
            new_input_val = input_val - 1000;
            break;
        }
    } else if (mpStick->checkLeftTrigger()) {
        if (mInputFigure < max_figure) {
            mInputFigure++;
            Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                            -1.0f, 0);
        }
    } else if (mpStick->checkRightTrigger()) {
        if (mInputFigure != 0) {
            mInputFigure--;
            Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                            -1.0f, 0);
        }
    }

    if (new_input_val < 0) {
        new_input_val = 0;
    }

    if (new_input_val > dComIfGs_getRupee()) {
        new_input_val = dComIfGs_getRupee();
    }

    if (new_input_val != input_val) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                        0);
    }

    decideOutFontRupeeColor(new_input_val);
    getObjectPtr()->setInputValue(new_input_val);
}

char* jmessage_tReference::getWord(int i_no) {
    if (i_no >= 10) {
        JUT_WARN(1093, "%s", "message stack over!!");
        JUT_ASSERT(1094, i_no < (10));
    }

    return mWord[i_no];
}

void jmessage_tReference::resetWord() {
    for (int i = 0; i < 10; i++) {
        mWord[i][0] = 0;
    }
}

void jmessage_tReference::setCharactor(u16 i_character) {
    mCharactor.data[mCharactor.field_0x40c] = i_character;
    mCharactor.field_0x40c++;
}

void jmessage_tReference::addCharactor(u16 i_character) {
    if (i_character == mCharactor.data[mCharactor.field_0x40e]) {
        mCharactor.field_0x40e++;
    }
}

void jmessage_tReference::resetCharactor() {
    for (int i = 0; i < ARRAY_SIZE(mCharactor.data); i++) {
        mCharactor.data[i] = 0;
    }

    mCharactor.field_0x40c = 0;
    mCharactor.field_0x40e = 0;
    mCharactor.mCountBackUp = 0;
}

void jmessage_tReference::shiftCharCountBuffer() {
    for (int i = 30; i > 0; i--) {
        mCharCountBuffer[i] = mCharCountBuffer[i - 1];
    }

    mCharCountBuffer[0] = getNowLightCount();
}

void jmessage_tReference::resetCharCountBuffer() {
    for (int i = 0; i < 31; i++) {
        mCharCountBuffer[i] = 0;
    }

    mLightBatchFlag = false;
    mBatchFlag = false;
    resetNowLightCount();
    resetDrawLightCount();
}

void jmessage_tReference::allsetCharCountBuffer() {
    for (int i = 0; i < 31; i++) {
        mCharCountBuffer[i] = getNowLightCount();
    }
}

bool jmessage_tReference::isCharSend() {
    s16 delay_frame;
    s16 var_r5;

    if (getFukiKind() == 8) {
        delay_frame = g_MsgObject_HIO_c.mHaloDelayFrameSpirit;
        var_r5 = delay_frame;
    } else {
        delay_frame = g_MsgObject_HIO_c.mHaloDelayFrame;
        var_r5 = delay_frame;
    }

    if (var_r5 < 0) {
        var_r5 *= -1;
    }

    if (delay_frame < 0 && getDrawLightCount() >= getCharCountBuffer(var_r5)) {
        return false;
    }

    return true;
}

bool jmessage_tReference::isLightSend() {
    s16 delay_frame;
    s16 var_r5;

    if (getFukiKind() == 8) {
        delay_frame = g_MsgObject_HIO_c.mHaloDelayFrameSpirit;
        var_r5 = delay_frame;
    } else {
        delay_frame = g_MsgObject_HIO_c.mHaloDelayFrame;
        var_r5 = delay_frame;
    }

    if (var_r5 < 0) {
        var_r5 *= -1;
    }

    if (delay_frame > 0 && getDrawLightCount() >= getCharCountBuffer(var_r5)) {
        return false;
    }

    return true;
}

bool jmessage_tReference::isLightEnd() {
    s16 delay_frame;

    if (getFukiKind() == 8) {
        delay_frame = g_MsgObject_HIO_c.mHaloDelayFrameSpirit;
    } else {
        delay_frame = g_MsgObject_HIO_c.mHaloDelayFrame;
    }

    if (delay_frame < 0) {
        delay_frame *= -1;
    }

    if (getCharCountBuffer(delay_frame) == getCharCountBuffer(0)) {
        return true;
    }

    return false;
}

void jmessage_tReference::decideOutFontRupeeColor(int i_inputVal) {
    int price_diff = getObjectPtr()->getNowTotalPrice() - getObjectPtr()->getNowTotalPayment();

    if (price_diff > i_inputVal) {
        mOutFontRupeeColor = 1;
    } else if (price_diff == i_inputVal) {
        mOutFontRupeeColor = 0;
    } else {
        mOutFontRupeeColor = 2;
    }
}

jmessage_tControl::jmessage_tControl() {}

jmessage_tMeasureProcessor::jmessage_tMeasureProcessor(jmessage_tReference const* pReference)
    : TRenderingProcessor(pReference) {
    jmessage_tReference* reference_p = (jmessage_tReference*)getReference();

    reference_p->resetCharactor();
    field_0x38 = 1.0f;
    mTotalLineCnt = 0;
    field_0x3e = 0;
    field_0x40 = 0;
    field_0x48 = 0;
    field_0x44 = 0;
    mSelectType = 0;
    field_0x46 = 0;
    field_0x47 = 0;
    field_0x49 = 0;
    mPageLineMax = reference_p->getLineMax();
    field_0x4b = 0;
    mSeSpeaker = 0;
    field_0x4d = 0;
}

void jmessage_tMeasureProcessor::do_begin(void const* pEntry, char const* pszText) {
    (void)pszText;
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    pReference->resetCharactor();
    pReference->setRevoMessageID(0);
    field_0x38 = 1.0f;

    mSeSpeaker = ((JMSMesgEntry_c*)pEntry)->se_speaker;
    mSeMood = ((JMSMesgEntry_c*)pEntry)->se_mood;

    for (int i = 0; i < D_MSG_CLASS_PAGE_CNT_MAX; i++) {
        pReference->setLineLength(i, 0.0f, 0.0f);
        pReference->setPageLine(i, 0);
        pReference->setPageLineMax(i, 0);
        pReference->setPageType(i, 0);
        pReference->setLineArrange(i, ((JMSMesgEntry_c*)pEntry)->unk_0xd);
        pReference->setLineScale(i, 100);

        if (pReference->isPlaceName() || pReference->isStaffRoll() || pReference->isBossName() ||
            pReference->getFukiKind() == 9)
        {
            pReference->setLineArrange(i, 1);
        } else {
            if (((JMSMesgEntry_c*)pEntry)->message_id == 0x2A5) {
                pReference->setLineArrange(i, 1);
            }

#if !REGION_JPN
            if (((JMSMesgEntry_c*)pEntry)->unk_0xd == 0) {
                pReference->setLineArrange(i, 1);
            }
#endif
        }
    }

    for (int i = 0; i < 3; i++) {
        pReference->setSelLength(i, 0.0f);
        pReference->offSelectRubyFlag(i);
    }

    pReference->setPageEndCount(0);
    mTotalLineCnt = 0;
    field_0x3e = 0;
    field_0x40 = 0;
    field_0x48 = 0;
    field_0x44 = 0;
    mSelectType = 0;
    field_0x46 = 0;
    field_0x47 = 0;
    field_0x49 = 0;
    mPageLineMax = pReference->getLineMax();
    pReference->resetWord();
    field_0x4b = 0;

    dMsgObject_c* object_p = pReference->getObjectPtr();
    object_p->offAutoMessageFlag();

    if (!pReference->isSelectSetCancelFlag()) {
        object_p->resetSelectBomBag();
    }

    if (pReference->getDistanceScale() != 1.0f) {
        do_scale(field_0x38);
    }
}

void jmessage_tMeasureProcessor::do_end() {
    if (field_0x49 == 0) {
        field_0x3e++;
    }

    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    
    if (field_0x46 == 0) {
        if (field_0x49 > 1) {
            pReference->setPageLine(field_0x40, field_0x3e - (field_0x49 - 1));
            pReference->setPageLineMax(field_0x40, field_0x3e - (field_0x49 - 1));
        } else {
            pReference->setPageLine(field_0x40, field_0x3e);
            pReference->setPageLineMax(field_0x40, field_0x3e);
        }
    }

    pReference->setPageEndCount(field_0x40);
    do_pageType(field_0x3e);
    Z2GetSpeechMgr2()->setString(pReference->getCharSoundInfo().data, pReference->getCharSoundInfo().field_0x40c, mSeSpeaker, mSeMood);
}

void jmessage_tMeasureProcessor::do_character(int iCharacter) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    JUTFont* font = pReference->getFont();

    if (field_0x44 == 0) {
        if (font->isLeadByte_ShiftJIS(iCharacter)) {
            if (!isOutfontKanjiCode(iCharacter)) {
                u16 character2Bytes = change1ByteTo2Bytes(iCharacter);
                pReference->setCharactor(changeKataToHira(character2Bytes));
            }
        } else if (iCharacter < 0x8800 && !isOutfontKanjiCode(iCharacter)) {
            pReference->setCharactor(changeKataToHira(iCharacter));
        }
    }

    if (iCharacter == '\n') {
        mTotalLineCnt++;
        field_0x3e++;

        if (field_0x44 != 0) {
            pReference->setLineArrange(mTotalLineCnt, 1);
            field_0x44--;
        }

        field_0x48 = 0;

        if (field_0x49 != 0) {
            field_0x49++;
        } else {
            field_0x49 = 1;
        }

        if (mSelectType == 0 && field_0x3e >= mPageLineMax) {
            pReference->setPageLine(field_0x40, mPageLineMax - (field_0x49 - 1));
            pReference->setPageLineMax(field_0x40, field_0x3e);
            do_pageType(mPageLineMax - (field_0x49 - 1));

            if (field_0x46 == 0) {
                field_0x40++;
            }

            field_0x3e = 0;
            mPageLineMax = pReference->getLineMax();
            field_0x49 = 0;

            if (field_0x46 == 0 && field_0x38 > 1.0f && field_0x48 == 0) {
                field_0x48 = 1;
                mPageLineMax--;
                JUT_ASSERT(1637, mPageLineMax > 0);
                pReference->setPageType(field_0x40, 2);
            }
        }

        if (mTotalLineCnt >= ((4)*10)) {
            JUT_ASSERT(1650, mTotalLineCnt < ((4)*10));
        }
    } else {
        if (field_0x49 != 0) {
            field_0x49 = 0;
        }

        pReference->addLineLength(mTotalLineCnt, field_0x38 * font->getWidth(iCharacter) / (f32)font->getCellWidth(), 1.0f);

        if (field_0x46 != 0) {
            f32 var_f31 = pReference->getSelFontSize() / (f32)font->getCellWidth();
            pReference->addSelLength(field_0x46 - 1, (var_f31 * (field_0x38 * font->getWidth(iCharacter))) + pReference->getSelCharSpace());
        }

        if (field_0x38 > 1.0f) {
            if (field_0x3e == 1 && pReference->getPageType(field_0x40) == 2) {
                pReference->setPageType(field_0x40, 4);
                pReference->setLineScale(mTotalLineCnt, pReference->getLineScale(0));
            } else if (field_0x3e == 2 && pReference->getPageType(field_0x40) == 4) {
                pReference->setPageType(field_0x40, 5);
                pReference->setLineScale(mTotalLineCnt, pReference->getLineScale(0));
            }
        }
    }
}

static void dummyString() {
    DEAD_STRING("'");
    DEAD_STRING("s");
}

bool jmessage_tMeasureProcessor::do_tag(u32 i_tag, void const* i_data, u32 i_size) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    switch (i_tag & 0xFF0000) {
    case MSGTAG_GROUP(1):
    case MSGTAG_GROUP(2):
        return true;
    case MSGTAG_GROUP(255):
        switch (i_tag) {
        case MSGTAG_COLOR:
            return true;
        case MSGTAG_SCALE: {
            u16 scale = *(u16*)i_data;
            pReference->setLineScale(mTotalLineCnt, scale);
            do_scale(scale / 100.0f);
            return true;
        }
        case MSGTAG_RUBY:
        case 0xFFFF02: {
            do_rubyset(i_data, i_size);

            if (field_0x46 != 0) {
                if (mSelectType == 2) {
                    if (!pReference->isSelectRubyFlag(field_0x46 - 1)) {
                        pReference->onSelectRubyFlag(field_0x46 - 1);
                    }
                } else if (mSelectType == 3) {
                    if (!pReference->isSelectRubyFlag(field_0x46 - 1)) {
                        pReference->onSelectRubyFlag(field_0x46 - 1);
                    }
                }
            }
            return true;
        }
        }
    case MSGTAG_GROUP(0):
        switch (i_tag) {
        case MSGTAG_INSTANT:
            pReference->setCharAllAlphaRate(1.0f);
            return true;
        case MSGTAG_TYPE:
        case MSGTAG_BOXATMOST:
        case MSGTAG_UNK_6:
        case MSGTAG_PAUSE:
        case MSGTAG_BOXATLEAST:
        case MSGTAG_LINE_DOWN:
            return true;
        case MSGTAG_CURRENT_LETTER_PAGE: {
            char buffer[4];
            int number = dComIfGp_getMessageCountNumber() / 100;
            sprintf(buffer, "%d", number);
            push_word(buffer);
            return true;
        }
        case MSGTAG_MAX_LETTER_PAGE: {
            char buffer[4];
            int number = dComIfGp_getMessageCountNumber() % 100;
            sprintf(buffer, "%d", number);
            push_word(buffer);
            return true;
        }
        case MSGTAG_UNK_3:
        case MSGTAG_AUTOBOX:
            if (*(u16*)i_data != 0) {
                dMsgObject_c* objectPtr = pReference->mpObjectPtr;  // fakematch, should be getObjectPtr
                objectPtr->onAutoMessageFlag();
            }
            return true;
        case MSGTAG_DEMOBOX:
            pReference->setDemoFrame(*(u32*)i_data);
            return true;
        case MSGTAG_PLAYER_NAME: {
            if (field_0x49 != 0) {
                field_0x49 = 0;
            }

            char player_name[100];
            strcpy(player_name, dComIfGs_getPlayerName());
            push_word(player_name);
            return true;
        }
        case MSGTAG_HORSE_NAME: {
            if (field_0x49 != 0) {
                field_0x49 = 0;
            }

            char horse_name[100];
            strcpy(horse_name, dComIfGs_getHorseName());
            push_word(horse_name);
            return true;
        }
        case MSGTAG_UNK_53:
            if (field_0x49 != 0) {
                field_0x49 = 0;
            }

            stack_pushCurrent(dMsgObject_getWord());
            return true;
        case MSGTAG_SCENT_NAME: {
            if (field_0x49 != 0) {
                field_0x49 = 0;
            }

            dMsgObject_c* objectPtr = pReference->getObjectPtr();
            const char* smellname = objectPtr->getSmellName();
            stack_pushCurrent(smellname);
            return true;
        }
        case MSGTAG_PORTAL_NAME: {
            if (field_0x49 != 0) {
                field_0x49 = 0;
            }

            dMsgObject_c* objectPtr = pReference->getObjectPtr();
            const char* portalname = objectPtr->getPortalName();
            stack_pushCurrent(portalname);
            return true;
        }
        case MSGTAG_BOMB_NAME: {
            if (field_0x49 != 0) {
                field_0x49 = 0;
            }

            dMsgObject_c* objectPtr = pReference->getObjectPtr();
            OS_REPORT("ボムカーソル位置 %d\n", dMsgObject_getMsgObjectClass()->getSelectCursorPos());

            if (!pReference->isSelectSetCancelFlag()) {
                pReference->onBombNameUseFlag();
            }

            const char* bombname = objectPtr->getBombName();
            stack_pushCurrent(bombname);
            return true;
        }
        case MSGTAG_INSECT_NAME: {
            if (field_0x49 != 0) {
                field_0x49 = 0;
            }

            dMsgObject_c* objectPtr = pReference->getObjectPtr();
            const char* insectname = objectPtr->getInsectName();
            stack_pushCurrent(insectname);
            return true;
        }
        case MSGTAG_LETTER_NAME: {
            if (field_0x49 != 0) {
                field_0x49 = 0;
            }

            dMsgObject_c* objectPtr = pReference->getObjectPtr();
            const char* lettername = objectPtr->getLetterName();
            stack_pushCurrent(lettername);
            return true;
        }
        case MSGTAG_SELECT_2WAY:
            JUT_ASSERT(1916, mSelectType != 0);

            if (field_0x46 == 0) {
                if (field_0x3e > 0) {
                    pReference->setPageLine(field_0x40, field_0x3e);
                    pReference->setPageLineMax(field_0x40, field_0x3e);
                    do_pageType(field_0x3e);
                    field_0x3e = 0;
                    mPageLineMax = pReference->getLineMax();
                    field_0x49 = 0;
                }

                mSelectType = 2;
                field_0x44 = 1;
                field_0x46 = 1;
            }

            if (*(u8*)i_data == 1) {
                pReference->setSelectPos(field_0x47);
            }

            field_0x47++;
            field_0x46++;
            pReference->setLineArrange(mTotalLineCnt, 1);
            return true;
        case MSGTAG_SELECT_3WAY:
            JUT_ASSERT(1944, mSelectType != 0);

            if (field_0x46 == 0) {
                if (field_0x3e > 0) {
                    pReference->setPageLine(field_0x40, field_0x3e);
                    pReference->setPageLineMax(field_0x40, field_0x3e);
                    do_pageType(field_0x3e);
                    field_0x3e = 0;
                    mPageLineMax = pReference->getLineMax();
                    field_0x49 = 0;
                }

                mSelectType = 3;
                field_0x44 = 2;
            }

            if (*(u8*)i_data == 1) {
                pReference->setSelectPos(field_0x47);
            }

            field_0x47++;
            field_0x46++;
            pReference->setLineArrange(mTotalLineCnt, 1);
            return true;
        case MSGTAG_INLINE_2_FIRST:
        case MSGTAG_INLINE_3_FIRST:
            pReference->setSelectPos(field_0x47);
        case MSGTAG_INLINE_2_NEXT:
        case MSGTAG_INLINE_3_NEXT:
            field_0x47++;
            pReference->setLineArrange(mTotalLineCnt, 0);
            do_space(0);
            return true;
        case MSGTAG_AWAIT_CHOICE:
            mSelectType = 1;
            field_0x38 = 1.0f;
            do_character('\n');
            stack_pushCurrent(pReference->getSelMsgPtr());
            return true;
        case MSGTAG_UNK_33:
            do_name1();
            return true;
        case MSGTAG_ABTN:
        case MSGTAG_BBTN:
        case MSGTAG_CSTICK:
        case MSGTAG_LBTN:
        case MSGTAG_RBTN:
        case MSGTAG_XBTN:
        case MSGTAG_YBTN:
        case MSGTAG_ZBTN:
        case MSGTAG_DPAD:
        case MSGTAG_STICK_CROSS:
        case MSGTAG_LEFT_ARROW:
        case MSGTAG_RIGHT_ARROW:
        case MSGTAG_UP_ARROW:
        case MSGTAG_DOWN_ARROW:
        case MSGTAG_STICK_UP:
        case MSGTAG_STICK_DOWN:
        case MSGTAG_STICK_LEFT:
        case MSGTAG_STICK_RIGHT:
        case MSGTAG_STICK_VERTICAL:
        case MSGTAG_STICK_HORIZONTAL:
        case MSGTAG_RED_TARGET:
        case MSGTAG_YELLOW_TARGET:
        case MSGTAG_ABTN_STAR:
        case MSGTAG_WHITE_TARGET:
        case MSGTAG_WARP_ICON:
        case MSGTAG_XYBTN:
        case MSGTAG_YXBTN:
        case MSGTAG_HEART:
        case MSGTAG_QUAVER:
            do_space(i_tag);
            return true;
        case MSGTAG_BOMB_BAG_ICON:
            do_space(i_tag);
            if (!pReference->isSelectSetCancelFlag()) {
                dMsgObject_c* objectPtr = pReference->getObjectPtr();
                u8 type = *(u8*)i_data & 0xFF;
                objectPtr->addSelectBomBag(type - 1);
            }
            return true;
        case MSGTAG_INPUT_VALUE:
            for (int i = 0; i < 4; i++) {
                do_space(0);
            }
            return true;
        case MSGTAG_ACKNOWLEDGE:
            return true;
        case MSGTAG_BOMB_NUM: {
            char buffer[40];
            dMsgObject_c* objectPtr = pReference->getObjectPtr();
            u8 sel_bomb_num = objectPtr->getSelectBombNum();
            dMsgUnit_setTag(7, sel_bomb_num, buffer);
            push_word(buffer);
            return true;
        }
        case MSGTAG_BOMB_PRICE: {
            char buffer[40];
            dMsgObject_c* objectPtr = pReference->getObjectPtr();
            s16 sel_bomb_price = objectPtr->getSelectBombPrice();
            dMsgUnit_setTag(1, sel_bomb_price, buffer);
            push_word(buffer);
            return true;
        }
        case MSGTAG_BOMB_MAX: {
            u8 bomb_type = fpcNm_ITEM_NORMAL_BOMB;

            const u8 type = *(u8*)i_data;
            if (type == 1) {
                bomb_type = fpcNm_ITEM_WATER_BOMB;
            } else if (type == 2) {
                bomb_type = fpcNm_ITEM_POKE_BOMB;
            }

            char buffer[40];
            dMsgUnit_setTag(7, dComIfGs_getBombMax(bomb_type), buffer);
            push_word(buffer);
            return true;
        }
        case MSGTAG_ARROW_MAX: {
            char buffer[40];
            dMsgUnit_setTag(0, dComIfGs_getArrowMax(), buffer);
            push_word(buffer);
            return true;
        }
        }
        return true;
    case MSGTAG_GROUP(4):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_GLYPH_DOLLARSIGN:
            push_word(changeCodeToChar('$'));
            return true;
        case MSGTAG_GLYPH_BACKSLASH:
            push_word(changeCodeToChar('\\'));
            return true;
        case MSGTAG_GLYPH_ATMARK:
            push_word(changeCodeToChar('@'));
            return true;
        case MSGTAG_GLYPH_SHARP:
            push_word(changeCodeToChar(0x81F2));
            return true;
        case MSGTAG_GLYPH_FLAT:
            push_word(changeCodeToChar(0x81F3));
            return true;
        case MSGTAG_GLYPH_SQRT:
            push_word(changeCodeToChar(0x81E3));
            return true;
        case MSGTAG_GLYPH_PERCENT:
            push_word(changeCodeToChar(0x876C));
            return true;
        case MSGTAG_GLYPH_HECTARE:
            push_word(changeCodeToChar(0x8766));
            return true;
        case MSGTAG_GLYPH_ARE:
            push_word(changeCodeToChar(0x8765));
            return true;
        case MSGTAG_GLYPH_LITRE:
            push_word(changeCodeToChar(0x8767));
            return true;
        case MSGTAG_GLYPH_WATT:
            push_word(changeCodeToChar(0x8768));
            return true;
        case MSGTAG_GLYPH_CALORIE:
            push_word(changeCodeToChar(0x8769));
            return true;
        case MSGTAG_GLYPH_DOLLAR:
            push_word(changeCodeToChar(0x876A));
            return true;
        case MSGTAG_GLYPH_CENT:
            push_word(changeCodeToChar(0x876B));
            return true;
        }
        return false;
    case MSGTAG_GROUP(5): {
        char buffer[40];
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_TIME_INFO: {
            const u8 type = *(u8*)i_data;
            if (type == 0) {
                dMsgUnit_setTag(4, dMeter2Info_getMsgTimeMs(), buffer);
            } else if (type == 2) {
                dMsgUnit_setTag(4, dComIfGs_getRaceGameTime(), buffer);
            } else {
                dMsgUnit_setTag(4, dMeter2Info_getTimeMs(), buffer);
            }
            break;
        }
        case 1:
            dMsgUnit_setTag(0, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case 2:
            dMsgUnit_setTag(1, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_FUNDRAISE_REMAIN: {
            s16 fund_raise_val = *(u32*)i_data;
            dMsgObject_c* objectPtr = pReference->getObjectPtr();
            objectPtr->setFundRaisingValue(fund_raise_val);

            dMsgUnit_setTag(10, fund_raise_val - dMsgObject_getFundRaising(), buffer);
            break;
        }
        case MSGTAG_NEW_LETTER_NUM:
            dMsgUnit_setTag(11, dMeter2Info_getNewLetterNum(), buffer);
            break;
        case MSGTAG_POE_NUM:
            dMsgUnit_setTag(12, getPohNum(), buffer);
            break;
        case MSGTAG_BALLOON_SCORE: {
            int number;
            u8 type = *(u8*)i_data & 0xFF;
            if (type == 0) {
                number = dComIfGp_getMessageCountNumber();
            } else {
                number = dComIfGs_getBalloonScore();
            }

            dMsgUnit_setTag(13, number, buffer);
            break;
        }
        case MSGTAG_INSECT_INFO: {
            u32 insect_num;
            if (*(u8*)i_data == 0) {
                insect_num = dComIfGs_checkGetInsectNum();
            } else {
                insect_num = 24 - dComIfGs_checkGetInsectNum();
            }

            dMsgUnit_setTag(2, insect_num, buffer);
            break;
        }
        case 4:
            dMsgUnit_setTag(3, 0, buffer);
            break;
        case 5:
            dMsgUnit_setTag(0x10000, dComIfGp_getNeedLightDropNum(), buffer);
            break;
        case 6:
            dMsgUnit_setTag(6, 20 - dComIfGs_getEventReg(0xFF1F), buffer);
            break;
        case MSGTAG_FISH_LENGTH:
            dMsgUnit_setTag(9, (u32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_FISH_COUNT:
            dMsgUnit_setTag(14, (u32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_ROLLGOAL_LV:
            dMsgUnit_setTag(0x10001, (u32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_RIVER_POINTS:
            dMsgUnit_setTag(8, dMeter2Info_getNowCount(), buffer);
            break;
        default:
            return false;
        }
        push_word(buffer);
        return true;
    }
    case MSGTAG_GROUP(6): {
        char buffer[40];
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_PLAYER_GENITIV:
            #if VERSION == VERSION_GCN_PAL
            if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_GERMAN) {
                setPlayerName(buffer, 1);
            } else {
                setPlayerName(buffer, 0);
            }
            #endif
            push_word(buffer);
            return true;
        case MSGTAG_HORSE_GENITIV:
            #if VERSION == VERSION_GCN_PAL
            if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_GERMAN) {
                setHorseName(buffer, 1);
            } else {
                setHorseName(buffer, 0);
            }
            #endif
            push_word(buffer);
            return true;
        case MSGTAG_MALE_ICON:
            push_word(changeCodeToChar(CHAR_CODE_MALE_ICON));
            return true;
        case MSGTAG_FEMALE_ICON:
            push_word(changeCodeToChar(CHAR_CODE_FEMALE_ICON));
            return true;
        case MSGTAG_STAR_ICON:
            push_word(changeCodeToChar(CHAR_CODE_STAR_ICON));
            return true;
        case MSGTAG_REFMARK:
            push_word(changeCodeToChar(CHAR_CODE_REFMARK));
            return true;
        case MSGTAG_THIN_LEFT_ARROW:
            push_word(changeCodeToChar(CHAR_CODE_THIN_LEFT_ARROW));
            return true;
        case MSGTAG_THIN_RIGHT_ARROW:
            push_word(changeCodeToChar(CHAR_CODE_THIN_RIGHT_ARROW));
            return true;
        case MSGTAG_THIN_UP_ARROW:
            push_word(changeCodeToChar(CHAR_CODE_THIN_UP_ARROW));
            return true;
        case MSGTAG_THIN_DOWN_ARROW:
            push_word(changeCodeToChar(CHAR_CODE_THIN_DOWN_ARROW));
            return true;
        case MSGTAG_BULLET:
        case MSGTAG_BULLET_SPACE:
            do_space(i_tag);
            return true;
        }
    }
    case MSGTAG_GROUP(3):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_WII_ABTN:
        case MSGTAG_WII_BBTN:
        case MSGTAG_WII_HOMEBTN:
        case MSGTAG_WII_MINUSBTN:
        case MSGTAG_WII_PLUSBTN:
        case MSGTAG_WII_1BTN:
        case MSGTAG_WII_2BTN:
        case MSGTAG_WII_DPAD_ITEM:
        case MSGTAG_WII_DPAD_UP:
        case MSGTAG_WII_DPAD_DOWN:
        case MSGTAG_WII_DPAD_HORIZONTAL:
        case MSGTAG_WII_DPAD_RIGHT:
        case MSGTAG_WII_DPAD_LEFT:
        case MSGTAG_WII_WIIMOTE:
        case MSGTAG_WII_RETICULE:
        case MSGTAG_WII_NUNCHUK:
        case MSGTAG_WII_WIIMOTE2:
        case MSGTAG_WII_FAIRY:
        case MSGTAG_WII_CBTN:
        case MSGTAG_WII_ZBTN:
            do_space(i_tag);
            return true;
        case MSGTAG_WII_MSGID_OVERRIDE:
            pReference->setRevoMessageID(*(u32*)i_data);
            return true;
        }
    }

    return false;
}

void jmessage_tMeasureProcessor::do_scale(f32 i_scale) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    field_0x38 = i_scale * pReference->getDistanceScale();

    if (field_0x38 > 1.0f && field_0x48 == 0) {
        field_0x48 = 1;
        mPageLineMax--;
        JUT_ASSERT(0x930, mPageLineMax > 0);

#if REGION_JPN
        if (field_0x3e == 0) {
            pReference->setPageType(field_0x40, 2);
        }
#else
        if (field_0x3e == 0) {
            pReference->setPageType(field_0x40, 2);
        } else if (field_0x3e == 2 && mPageLineMax == 3) {
            if (pReference->getPageType(field_0x40) == 4) {
                pReference->setPageType(field_0x40, 5);
            } else {
                pReference->setPageType(field_0x40, 8);
            }
        }
#endif
        else {
            pReference->setPageType(field_0x40, 3);
            if (field_0x3e == 1 && pReference->getPageType(field_0x40) == 2) {
                pReference->setPageType(field_0x40, 4);
            }
        }
    }
}

void jmessage_tMeasureProcessor::do_space(u32 i_tag) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    f32 var_f31 = field_0x38;

    if (field_0x44 == 0) {
        u16 character = 0xFFFF;
        switch (i_tag & 0xFF0000) {
        case MSGTAG_GROUP(0):
            switch (i_tag) {
            case MSGTAG_ABTN:
                character = 0xFFE0;
                break;
            case MSGTAG_BBTN:
                character = 0xFFE1;
                break;
            case MSGTAG_CSTICK:
                character = 0xFFE2;
                break;
            case MSGTAG_LBTN:
                character = 0xFFE3;
                break;
            case MSGTAG_RBTN:
                character = 0xFFE4;
                break;
            case MSGTAG_XBTN:
                character = 0xFFE5;
                break;
            case MSGTAG_YBTN:
                character = 0xFFE6;
                break;
            case MSGTAG_ZBTN:
                character = 0xFFE7;
                break;
            case MSGTAG_DPAD:
                character = 0xFFE8;
                break;
            case MSGTAG_STICK_CROSS:
                character = 0xFFE9;
                break;
            case MSGTAG_LEFT_ARROW:
                character = 0xFFEA;
                break;
            case MSGTAG_RIGHT_ARROW:
                character = 0xFFEB;
                break;
            case MSGTAG_UP_ARROW:
                character = 0xFFEC;
                break;
            case MSGTAG_DOWN_ARROW:
                character = 0xFFED;
                break;
            case MSGTAG_STICK_UP:
                character = 0xFFEE;
                break;
            case MSGTAG_STICK_DOWN:
                character = 0xFFF0;
                break;
            case MSGTAG_STICK_LEFT:
                character = 0xFFF1;
                break;
            case MSGTAG_STICK_RIGHT:
                character = 0xFFF2;
                break;
            case MSGTAG_STICK_VERTICAL:
                character = 0xFFF3;
                break;
            case MSGTAG_STICK_HORIZONTAL:
                character = 0xFFF4;
                break;
            case MSGTAG_HEART:
                character = 0xFFF5;
                break;
            case MSGTAG_QUAVER:
                character = 0xFFF6;
                break;
            }

            pReference->setCharactor(character);
            break;
        case MSGTAG_GROUP(3):
            switch (i_tag & 0xFF00FFFF) {
            case MSGTAG_WII_HOMEBTN:
            case MSGTAG_WII_MINUSBTN:
            case MSGTAG_WII_PLUSBTN:
                pReference->setCharactor(character);
                break;
            case MSGTAG_WII_DPAD_ITEM:
            case MSGTAG_WII_DPAD_UP:
            case MSGTAG_WII_DPAD_DOWN:
            case MSGTAG_WII_DPAD_HORIZONTAL:
            case MSGTAG_WII_DPAD_RIGHT:
            case MSGTAG_WII_DPAD_LEFT:
            case MSGTAG_WII_WIIMOTE:
            case MSGTAG_WII_RETICULE:
                pReference->setCharactor(character);
                break;
            case MSGTAG_WII_NUNCHUK:
            case MSGTAG_WII_WIIMOTE2:
            case MSGTAG_WII_FAIRY:
            case MSGTAG_WII_CBTN:
            case MSGTAG_WII_ZBTN:
            case MSGTAG_WII_1BTN:
            case MSGTAG_WII_2BTN:
            case MSGTAG_WII_ABTN:
            case MSGTAG_WII_BBTN:
                pReference->setCharactor(character);
            }
            break;
        case MSGTAG_GROUP(6):
            switch (i_tag) {
            case MSGTAG_BULLET:
            case MSGTAG_BULLET_SPACE:
                pReference->setCharactor(character);
                break;
            }
            break;
        }
    }

    if (field_0x49 != 0) {
        field_0x49 = 0;
    }

    pReference->addLineLength(mTotalLineCnt, var_f31, 1.0f);
    if (field_0x46 != 0) {
        pReference->addSelLength(field_0x46 - 1, var_f31);
    }
}

void jmessage_tMeasureProcessor::do_pageType(int param_0) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    if (param_0 == 1) {
        if (pReference->getPageType(field_0x40) == 2) {
            pReference->setPageType(field_0x40, 1);
        }
    } else if (param_0 == 3) {
        if (pReference->getPageType(field_0x40) == 2) {
            pReference->setPageType(field_0x40, 6);
        } else if (pReference->getPageType(field_0x40) == 3) {
            pReference->setPageType(field_0x40, 7);
        }
    }
}

void jmessage_tMeasureProcessor::do_name1() {
    const char* name = dComIfGs_getPlayerName();
#if REGION_JPN
    int c = (((char)name[0] & 0xFF) << 8) | ((char)name[1] & 0xFF);
    // if first character is hiragana or katakana
    if ((c >= 0x829F && c <= 0x82F1) || (c >= 0x8340 && c <= 0x8396)) {
        push_word(changeCodeToChar(c));
    }
#endif
}

void jmessage_tMeasureProcessor::do_rubyset(void const* i_data, u32 i_size) {
    if (field_0x44 == 0) {
        u8 length = i_size - 1;
        u8* pRuby = (u8*)i_data;
        int index = 0;

        pRuby++;

        jmessage_tReference* pReference = (jmessage_tReference*)getReference();
        while (index < length) {
            u8 bytes[3];
            bytes[0] = pRuby[index++];
            bytes[1] = pRuby[index++];
            bytes[2] = 0;

            int character = (((s8)bytes[0] & 0xFF) << 8) | ((s8)bytes[1] & 0xFF);
            pReference->setCharactor(changeKataToHira(character));
        }
    }
}

void jmessage_tMeasureProcessor::push_word(char* i_word) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    strcpy(pReference->getWord(field_0x4b), i_word);
    stack_pushCurrent(pReference->getWord(field_0x4b));
    field_0x4b++;
}

jmessage_tSequenceProcessor::jmessage_tSequenceProcessor(jmessage_tReference const* pReference,
                                                         jmessage_tControl* pControl)
    : JMessage::TSequenceProcessor(pReference, pControl),
      mMeasureProcessor(pReference)
{
    field_0xa8 = 1;
    field_0xa4 = field_0xa8;
    field_0xa6 = 0;
    field_0xad = 0;
    mMouthCheck = 0;
    field_0xb1 = 0;
    field_0xae = 0;
    mForceForm = 0xFF;
    field_0xb2 = 0;
    field_0xaa = 0;
    field_0xb5 = 0;
    field_0xac = 0;
    field_0xb3 = 0;
    field_0xb4 = 0;
}

void jmessage_tSequenceProcessor::do_reset() {}

void jmessage_tSequenceProcessor::do_begin(void const* pEntry, char const* pszText) {
    mpEntry = pEntry;
    mpText = pszText;

    if (((JMSMesgEntry_c*)pEntry)->fuki_kind == 8) {
        field_0xa8 = g_MsgObject_HIO_c.mDisplaySpeedSpirit;
    } else {
        field_0xa8 = g_MsgObject_HIO_c.mDisplaySpeed;
    }

    field_0xa4 = field_0xa8;
    field_0xa6 = 0;
    field_0xad = 0;
    mMouthCheck = 0;
    field_0xb1 = 0;
    field_0xb2 = 0;
    field_0xaa = 0;
    field_0xac = 0;
    field_0xb4 = ((JMSMesgEntry_c*)pEntry)->se_speaker;
    field_0xb3 = ((JMSMesgEntry_c*)pEntry)->se_mood;

    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    pReference->resetCharCnt();
    pReference->setNowColorType(0);
    pReference->setTopColorType(0);
    pReference->setNowWordCount(0);
    pReference->setTopWordCount(0);
    pReference->setBatchColorFlag(0);
    pReference->setTopTagScale(0);
    pReference->setNowTagScale(0);
    pReference->calcDistance();

    dComIfGp_setMesgAnimeAttrInfo(((JMSMesgEntry_c*)pEntry)->base_anm_id);
    dComIfGp_setMesgFaceAnimeAttrInfo(((JMSMesgEntry_c*)pEntry)->face_anm_id);

    if (dComIfGp_isHeapLockFlag() == 2) {
        pReference->setFukiPosType(1);
    } else {
        if (dComIfGp_isHeapLockFlag() == 3) {
            pReference->setFukiPosType(0);
        } else {
            pReference->setFukiPosType(((JMSMesgEntry_c*)pEntry)->fuki_pos_type);
        }
    }

    pReference->setFukiKind(((JMSMesgEntry_c*)pEntry)->fuki_kind);

    if (dMsgObject_getMsgOutputType() != 0xFF) {
        pReference->setForm(dMsgObject_getMsgOutputType());
    } else {
        pReference->setForm(((JMSMesgEntry_c*)pEntry)->output_type);
    }

    pReference->setArrange(((JMSMesgEntry_c*)pEntry)->unk_0xd);
    pReference->setForm(((JMSMesgEntry_c*)pEntry)->unk_0xd);
    pReference->setMsgID(((JMSMesgEntry_c*)pEntry)->message_id);

    if (((JMSMesgEntry_c*)pEntry)->event_label_id != 0) {
        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[((JMSMesgEntry_c*)pEntry)->event_label_id]);
    }

    dMsgObject_c* objectPtr = pReference->getObjectPtr();
    objectPtr->talkStartInit();
    mMeasureProcessor.process_messageEntryText(this, pEntry, pszText);

    if (pReference->isBombNameUseFlag()) {
        pReference->offBombNameUseFlag();
        calcStringLength();
    }

    f32 tboxWidth = pReference->getSelLength(0);
    if (tboxWidth < pReference->getSelLength(1)) {
        tboxWidth = pReference->getSelLength(1);
    }

    if (tboxWidth < pReference->getSelLength(2)) {
        tboxWidth = pReference->getSelLength(2);
    }

    tboxWidth += 10.0f;

    if (tboxWidth > pReference->getSelTBoxWidth()) {
        pReference->setSelTBoxWidth(tboxWidth);
    }

    if (dMsgObject_getMsgOutputType() != 0xFF) {
        field_0xae = dMsgObject_getMsgOutputType();
    } else {
        field_0xae = ((JMSMesgEntry_c*)pEntry)->output_type;
    }

    if (mForceForm != 0xFF) {
        field_0xae = mForceForm;
    }

    pReference->setAddCharAlpha(255.0f);
    pReference->setAddCharAllAlphaRate(1.0f);
    pReference->setCharAllAlphaRate(1.0f);

    if (field_0xae == 3) {
        pReference->setAddCharAllAlphaRate(1.0f / (1.0f + g_MsgObject_HIO_c.mBatchDisplayWeight_3));
        pReference->setCharAllAlphaRate(0.0f);
    } else if (field_0xae == 8) {
        pReference->setAddCharAllAlphaRate(1.0f / (1.0f + g_MsgObject_HIO_c.mBatchDisplayWeight_8));
        pReference->setCharAllAlphaRate(0.0f);
    } else if (field_0xae == 9) {
        pReference->setAddCharAllAlphaRate(1.0f / (1.0f + g_MsgObject_HIO_c.mBatchDisplayWeight_9));
        pReference->setCharAllAlphaRate(0.0f);
    } else if (field_0xae == 5) {
        field_0xa4 = g_MsgObject_HIO_c.mCharDisplayWeight_5;
        pReference->setAddCharAlpha(255.0f / (1.0f + field_0xa4));
    }

    pReference->resetReference();
    field_0xb5 = 0;
}

void jmessage_tSequenceProcessor::do_end() {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    if (dMsgObject_getSelectWordFlag() != 0) {
        pReference->setSelectNum(dMsgObject_getSelectWordFlag());
        pReference->setSelectType(0);
        pReference->setSelectPos(0);
    }

    mMouthCheck = 0;

    if (pReference->getSelectNum() == 2) {
        if (pReference->getSelectType() == 0) {
            pReference->setStopFlag(2);
        } else {
            pReference->setStopFlag(4);
        }
    } else {
        if (pReference->getSelectNum() == 3) {
            if (pReference->getSelectType() == 0) {
                if (!pReference->isMidona()) {
                    pReference->setStopFlag(3);
                }
            } else {
                pReference->setStopFlag(5);
            }
        } else {
            if (pReference->getSendFlag() == 5) {
                pReference->setStopFlag(6);
            } else {
                pReference->setStopFlag(7);
            }
        }
    }

    if (dComIfGp_checkMesgBgm()) {
        dComIfGp_setMesgBgmOff();
    }

    if (mForceForm != 0xFF) {
        mForceForm = 0xFF;
    }

    if (pReference->isLightBatchFlag()) {
        pReference->allsetCharCountBuffer();
    }

    pReference->setNowLightCount(0xFF);
}

bool jmessage_tSequenceProcessor::do_isReady() {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    #if DEBUG
    if (pReference->getRevoMessageID() != 0) {
        return 0;
    }
    #endif

    if (dComIfGp_checkMesgBgm()) {
        bool isItemMusicPlaying = true;
        if (mDoAud_checkPlayingSubBgmFlag() != Z2BGM_ITEM_GET &&
            mDoAud_checkPlayingSubBgmFlag() != Z2BGM_ITEM_GET_MINI)
        {
            dComIfGp_setMesgBgmOff();
            isItemMusicPlaying = false;
        }

        if (isItemMusicPlaying) {
            return false;
        }
    }

    if (pReference->isButtonTagStopFlag()) {
        if (mDoCPd_c::getTrigA(PAD_1)) {
            pReference->offButtonTagStopFlag();
            pReference->onLightBatchFlag();
        }
        return false;
    }

    if (pReference->isKanban() || pReference->isPlaceName() || pReference->isStaffRoll() ||
        pReference->isBossName() || pReference->isSaveSeq() || pReference->isHowl())
    {
        if (pReference->getStatus() == 2 || pReference->getStatus() == 6 || pReference->getStatus() == 11 ||
            pReference->getStatus() == 5)
        {
            if (pReference->getLineCount() < pReference->getNowPageLineMax() || pReference->getStatus() == 11) {
                field_0xae = 1;
                field_0xa4 = 0;
                pReference->onBatchFlag();
                pReference->setCharCnt(D_MSG_CLASS_CHAR_CNT_MAX);
                field_0xa4 = field_0xa8;
                return true;
            }

            if (field_0xaa == pReference->getPageEndCount()) {
                return true;
            }

            pReference->setStopFlag(1);
            field_0xaa++;
        }
    } else if (pReference->isBook()) {
        if (pReference->getStatus() == 2 ||
            pReference->getStatus() == 6)
        {
            if (pReference->getLineCount() < pReference->getNowPageLineMax()) {
                field_0xae = 1;
                field_0xa4 = 0;
                pReference->onBatchFlag();
                pReference->setCharCnt(D_MSG_CLASS_CHAR_CNT_MAX);
                return true;
            }
            
            if (pReference->getPageEndCount() == pReference->getPageNum()) {
                return true;
            }

            pReference->setStopFlag(1);
            field_0xaa++;
        }
    } else if ((pReference->getStatus() == 6 || pReference->getStatus() == 11 ||
                (pReference->getStatus() == 8 || pReference->getStatus() == 9) && pReference->isMidona()) ||
               (pReference->getStatus() == 2 && pReference->isMidona()))
    {
        if (pReference->getLineCount() < pReference->getNowPageLineMax()) {
            switch (field_0xae) {
            case 0:
            case 5:
            case 6:
                if (mDoCPd_c::getTrigA(PAD_1) || field_0xb2 != 0) {
                    field_0xa4 = 0;
                    pReference->onBatchFlag();
                    pReference->setCharCnt(D_MSG_CLASS_CHAR_CNT_MAX);
                }
                break;
            case 1:
            case 3:
            case 8:
            case 9:
                field_0xa4 = 0;
                pReference->onBatchFlag();
                pReference->setCharCnt(D_MSG_CLASS_CHAR_CNT_MAX);
                break;
            case 2:
                if (field_0xb2 != 0) {
                    field_0xa4 = 0;
                }
                break;
            }

            if (field_0xa4 > 0) {
                field_0xa4--;
                return false;
            }

            field_0xa6++;
#if REGION_JPN
            if (field_0xa6 >= 1) {
#else
            if (field_0xa6 >= 2) {
#endif
                field_0xa4 = field_0xa8;
                field_0xa6 = 0;
            }

            if (field_0xb1 != 0) {
                field_0xb1 = 0;
            } else if (mMouthCheck == 0) {
                mMouthCheck = 1;
            }

            if (field_0xae == 5) {
                field_0xa4 = g_MsgObject_HIO_c.mCharDisplayWeight_5;
            }

            return true;
        }

        if (pReference->getPageEndCount() == pReference->getPageNum()) {
            return true;
        }

        pReference->setStopFlag(1);
        mMouthCheck = 0;
        field_0xaa++;
    }

    return false;
}

void jmessage_tSequenceProcessor::do_character(int iCharacter) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    if (field_0xac == 0) {
        if (JUTFont::isLeadByte_ShiftJIS(iCharacter)) {
            if (!isOutfontKanjiCode(iCharacter)) {
                u16 character2Bytes = change1ByteTo2Bytes(iCharacter);
                pReference->addCharactor(changeKataToHira(character2Bytes));
            }
        } else if (iCharacter < 0x8800 && !isOutfontKanjiCode(iCharacter)) {
            pReference->addCharactor(changeKataToHira(iCharacter));
        }

        pReference->addNowLightCount();
    }

    if (field_0xb3 != 0) {
        cXyz actorPos = pReference->getActorPos();
        messageSePlay(field_0xb4, field_0xb3, &actorPos);
        field_0xb3 = 0;
    }

    switch(iCharacter) {
    case '!':
        mMouthCheck = 0;
        field_0xb1 = 1;
        break;
    case '\n':
        pReference->addLineCount();
        pReference->addEndLineCount();
        break;
    default:
        if ((0x8140 <= iCharacter && iCharacter <= 0x81FC) || (0x839F <= iCharacter && iCharacter <= 0x879C)) {
            mMouthCheck = 0;
            field_0xb1 = 1;
        }
        break;
    }
}

bool jmessage_tSequenceProcessor::do_tag(u32 i_tag, void const* i_data, u32 i_size) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    switch (i_tag & 0xFF0000) {
    case MSGTAG_GROUP(1): {
        cXyz pos = pReference->getActorPos();
        messageSePlay(field_0xb4, (i_tag & 0xFFFF) & 0xFF, &pos);
        return true;
    }
    case MSGTAG_GROUP(2):
        dComIfGp_setMesgCameraTagInfo((i_tag & 0xFFFF) & 0xFF);
        return true;
    case MSGTAG_GROUP(255):
        switch (i_tag) {
        case MSGTAG_COLOR:
            pReference->setNowColorType(*(u8*)i_data & 0xFF);
            return true;
        case MSGTAG_SCALE:
            pReference->setNowTagScale(*(u16*)i_data & 0xFFFF);
            return true;
        case MSGTAG_RUBY:
        case 0xFFFF02:
            do_rubyset(i_data, i_size);
            return true;
        }
    case MSGTAG_GROUP(0):
        switch (i_tag) {
        case MSGTAG_PLAYER_NAME:
        case MSGTAG_HORSE_NAME:
            push_word();
            return true;
        case MSGTAG_UNK_53:
            JMessage::TSequenceProcessor::stack_pushCurrent(dMsgObject_getWord());
            return true;
        case MSGTAG_SCENT_NAME: {
            JMessage::TSequenceProcessor::stack_pushCurrent(pReference->getObjectPtr()->getSmellName());
            return true;
        }
        case MSGTAG_PORTAL_NAME: {
            JMessage::TSequenceProcessor::stack_pushCurrent(pReference->getObjectPtr()->getPortalName());
            return true;
        }
        case MSGTAG_BOMB_NAME: {
            JMessage::TSequenceProcessor::stack_pushCurrent(pReference->getObjectPtr()->getBombName());
            return true;
        }
        case MSGTAG_INSECT_NAME: {
            JMessage::TSequenceProcessor::stack_pushCurrent(pReference->getObjectPtr()->getInsectName());
            return true;
        }
        case MSGTAG_LETTER_NAME: {
            JMessage::TSequenceProcessor::stack_pushCurrent(pReference->getObjectPtr()->getLetterName());
            return true;
        }
        case MSGTAG_LINE_DOWN:
            return true;
        case MSGTAG_CURRENT_LETTER_PAGE:
        case MSGTAG_MAX_LETTER_PAGE:
            push_word();
            return true;
        case MSGTAG_INSTANT:
            field_0xb2 = 1;
            pReference->setBatchColorFlag(1);
            return true;
        case MSGTAG_TYPE:
            field_0xb2 = 0;
            return true;
        case MSGTAG_UNK_3:
        case MSGTAG_AUTOBOX:
            pReference->setSendTimer(*(u16*)i_data);
            if (pReference->getSendTimer() == 0) {
                pReference->setSendFlag(3);
            } else {
                pReference->setSendFlag(1);
            }
            return true;
        case MSGTAG_BOXATMOST:
            pReference->setSendTimer(*(u16*)i_data);
            pReference->setSendFlag(2);
            return true;
        case MSGTAG_BOXATLEAST:
            dMeter2Info_setMsgKeyWaitTimer(*(u16*)i_data);
            pReference->setSendTimer(*(u16*)i_data);
            pReference->setSendFlag(6);
            return true;
        case MSGTAG_DEMOBOX:
            pReference->setDemoFrame(*(u32*)i_data);
            pReference->setSendFlag(4);
            return true;
        case MSGTAG_UNK_6:
            field_0xa8 = *(u16*)i_data;
            field_0xa4 = field_0xa8;
            return true;
        case MSGTAG_PAUSE:
            field_0xa4 = *(u16*)i_data;
            mMouthCheck = 0;
            return true;
        case MSGTAG_SELECT_2WAY:
            if (pReference->getSelectNum() == 0) {
                pReference->setSelectNum(2);
                pReference->setSelectType(0);
                field_0xb2 = 1;
            }

            if (*(u8*)i_data == 1) {
                pReference->setSelectPos(field_0xad);
            }

            field_0xad++;
            field_0xac = 1;
            return true;
        case MSGTAG_SELECT_3WAY:
            if (pReference->getSelectNum() == 0) {
                pReference->setSelectNum(3);
                pReference->setSelectType(0);
                field_0xb2 = 1;
            }

            if (*(u8*)i_data == 1) {
                pReference->setSelectPos(field_0xad);
            }

            field_0xad++;
            field_0xac = 1;
            return true;
        case MSGTAG_INLINE_2_NEXT:
            if (pReference->getSelectNum() != 2 || pReference->getSelectType() != 1) {
                pReference->setSelectNum(2);
                pReference->setSelectType(1);
            }

            field_0xb2 = 1;
            return true;
        case MSGTAG_INLINE_2_FIRST:
            if (pReference->getSelectNum() != 2 || pReference->getSelectType() != 1) {
                pReference->setSelectNum(2);
                pReference->setSelectType(1);
                pReference->setSelectPos(0);
            } else {
                pReference->setSelectPos(1);
            }

            field_0xb2 = 1;
            return true;
        case MSGTAG_INLINE_3_NEXT:
            if (pReference->getSelectNum() != 3 || pReference->getSelectType() != 1) {
                pReference->setSelectNum(3);
                pReference->setSelectType(1);
            }

            field_0xad++;
            field_0xb2 = 1;
            return true;
        case MSGTAG_INLINE_3_FIRST:
            if (pReference->getSelectNum() != 3 || pReference->getSelectType() != 1) {
                pReference->setSelectNum(3);
                pReference->setSelectType(1);
            }

            pReference->setSelectPos(field_0xad);
            field_0xb2 = 1;
            return true;
        case MSGTAG_AWAIT_CHOICE:
            do_character('\n');
            JMessage::TSequenceProcessor::stack_pushCurrent(pReference->getSelMsgPtr());
            return true;
        case MSGTAG_UNK_33:
            do_name1();
            return true;
        case MSGTAG_ABTN:
        case MSGTAG_BBTN:
        case MSGTAG_CSTICK:
        case MSGTAG_LBTN:
        case MSGTAG_RBTN:
        case MSGTAG_XBTN:
        case MSGTAG_YBTN:
        case MSGTAG_ZBTN:
        case MSGTAG_DPAD:
        case MSGTAG_STICK_CROSS:
        case MSGTAG_LEFT_ARROW:
        case MSGTAG_RIGHT_ARROW:
        case MSGTAG_UP_ARROW:
        case MSGTAG_DOWN_ARROW:
        case MSGTAG_STICK_UP:
        case MSGTAG_STICK_DOWN:
        case MSGTAG_STICK_LEFT:
        case MSGTAG_STICK_RIGHT:
        case MSGTAG_STICK_VERTICAL:
        case MSGTAG_STICK_HORIZONTAL:
        case MSGTAG_RED_TARGET:
        case MSGTAG_YELLOW_TARGET:
        case MSGTAG_ABTN_STAR:
        case MSGTAG_WHITE_TARGET:
        case MSGTAG_WARP_ICON:
        case MSGTAG_XYBTN:
        case MSGTAG_YXBTN:
        case MSGTAG_BOMB_BAG_ICON:
        case MSGTAG_HEART:
        case MSGTAG_QUAVER:
            do_space(i_tag);
            return true;
        case MSGTAG_INPUT_VALUE: {
            pReference->decideOutFontRupeeColor(0);
            dMsgObject_c* object_p = pReference->getObjectPtr();

            object_p->setInputValue(0);
            object_p->onInputFlag();
            pReference->resetInputFigure();
            pReference->setSendFlag(5);

            if (*(u32*)i_data == 1) {
                dComIfGs_onTmpBit(dSv_event_tmp_flag_c::tempBitLabels[80]);
            }
            return true;
        }
        case MSGTAG_ACKNOWLEDGE:
            pReference->onButtonTagStopFlag();
            return true;
        case MSGTAG_BOMB_NUM: {
            char buffer[40];
            dMsgObject_c* objectPtr = pReference->getObjectPtr();

            u8 sel_bomb_num = objectPtr->getSelectBombNum();
            dMsgUnit_setTag(7, sel_bomb_num, buffer);
            strcpy((char*)pReference->getWord(field_0xb5), buffer);
            push_word();
            return true;
        }
        case MSGTAG_BOMB_PRICE: {
            char buffer[40];
            dMsgObject_c* objectPtr = pReference->getObjectPtr();

            s16 sel_bomb_price = objectPtr->getSelectBombPrice();
            dMsgUnit_setTag(1, sel_bomb_price, buffer);
            strcpy((char*)pReference->getWord(field_0xb5), buffer);
            push_word();
            return true;
        }
        case MSGTAG_BOMB_MAX:
        case MSGTAG_ARROW_MAX:
            push_word();
            return true;
        }
    case MSGTAG_GROUP(4):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_GLYPH_DOLLARSIGN:
        case MSGTAG_GLYPH_BACKSLASH:
        case MSGTAG_GLYPH_ATMARK:
        case MSGTAG_GLYPH_SHARP:
        case MSGTAG_GLYPH_FLAT:
        case MSGTAG_GLYPH_SQRT:
        case MSGTAG_GLYPH_PERCENT:
        case MSGTAG_GLYPH_HECTARE:
        case MSGTAG_GLYPH_ARE:
        case MSGTAG_GLYPH_LITRE:
        case MSGTAG_GLYPH_WATT:
        case MSGTAG_GLYPH_CALORIE:
        case MSGTAG_GLYPH_DOLLAR:
        case MSGTAG_GLYPH_CENT:
            push_word();
            return true;
        }
    case MSGTAG_GROUP(5):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_TIME_INFO:
        case 1:
        case 2:
        case MSGTAG_INSECT_INFO:
        case 4:
        case 5:
        case 6:
        case MSGTAG_RIVER_POINTS:
        case MSGTAG_FISH_LENGTH:
        case MSGTAG_FUNDRAISE_REMAIN:
        case MSGTAG_NEW_LETTER_NUM:
        case MSGTAG_POE_NUM:
        case MSGTAG_BALLOON_SCORE:
        case MSGTAG_FISH_COUNT:
        case MSGTAG_ROLLGOAL_LV:
            push_word();
            return true;
        }
    case MSGTAG_GROUP(6):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_PLAYER_GENITIV:
        case MSGTAG_HORSE_GENITIV:
        case MSGTAG_MALE_ICON:
        case MSGTAG_FEMALE_ICON:
        case MSGTAG_STAR_ICON:
        case MSGTAG_REFMARK:
        case MSGTAG_THIN_LEFT_ARROW:
        case MSGTAG_THIN_RIGHT_ARROW:
        case MSGTAG_THIN_UP_ARROW:
        case MSGTAG_THIN_DOWN_ARROW:
            push_word();
            return true;
        case MSGTAG_BULLET:
        case MSGTAG_BULLET_SPACE:
            do_space(i_tag);
            return true;
        }
    case MSGTAG_GROUP(3):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_WII_ABTN:
        case MSGTAG_WII_BBTN:
        case MSGTAG_WII_HOMEBTN:
        case MSGTAG_WII_MINUSBTN:
        case MSGTAG_WII_PLUSBTN:
        case MSGTAG_WII_1BTN:
        case MSGTAG_WII_2BTN:
        case MSGTAG_WII_DPAD_ITEM:
        case MSGTAG_WII_DPAD_UP:
        case MSGTAG_WII_DPAD_DOWN:
        case MSGTAG_WII_DPAD_HORIZONTAL:
        case MSGTAG_WII_DPAD_RIGHT:
        case MSGTAG_WII_DPAD_LEFT:
        case MSGTAG_WII_WIIMOTE:
        case MSGTAG_WII_RETICULE:
        case MSGTAG_WII_NUNCHUK:
        case MSGTAG_WII_WIIMOTE2:
        case MSGTAG_WII_FAIRY:
        case MSGTAG_WII_CBTN:
        case MSGTAG_WII_ZBTN:
            do_space(i_tag);
            return true;
        case MSGTAG_WII_MSGID_OVERRIDE:
            return true;
        }
    }

    return false;
}

bool jmessage_tSequenceProcessor::do_jump_isReady() {
    return true;
}

void jmessage_tSequenceProcessor::do_name1() {
    const char* name = dComIfGs_getPlayerName();
#if REGION_JPN
    int c = (((char)name[0] & 0xFF) << 8) | ((char)name[1] & 0xFF);
    // if first character is hiragana or katakana
    if ((c >= 0x829F && c <= 0x82F1) || (c >= 0x8340 && c <= 0x8396)) {
        push_word();
    }
#endif
}

void jmessage_tSequenceProcessor::do_space(u32 i_tag) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    pReference->addNowLightCount();

    if (field_0xac == 0) {
        u16 character = 0xFFFF;
        switch (i_tag) {
        case MSGTAG_ABTN:
            character = 0xFFE0;
            break;
        case MSGTAG_BBTN:
            character = 0xFFE1;
            break;
        case MSGTAG_CSTICK:
            character = 0xFFE2;
            break;
        case MSGTAG_LBTN:
            character = 0xFFE3;
            break;
        case MSGTAG_RBTN:
            character = 0xFFE4;
            break;
        case MSGTAG_XBTN:
            character = 0xFFE5;
            break;
        case MSGTAG_YBTN:
            character = 0xFFE6;
            break;
        case MSGTAG_ZBTN:
            character = 0xFFE7;
            break;
        case MSGTAG_DPAD:
            character = 0xFFE8;
            break;
        case MSGTAG_STICK_CROSS:
            character = 0xFFE9;
            break;
        case MSGTAG_LEFT_ARROW:
            character = 0xFFEA;
            break;
        case MSGTAG_RIGHT_ARROW:
            character = 0xFFEB;
            break;
        case MSGTAG_UP_ARROW:
            character = 0xFFEC;
            break;
        case MSGTAG_DOWN_ARROW:
            character = 0xFFED;
            break;
        case MSGTAG_STICK_UP:
            character = 0xFFEE;
            break;
        case MSGTAG_STICK_DOWN:
            character = 0xFFF0;
            break;
        case MSGTAG_STICK_LEFT:
            character = 0xFFF1;
            break;
        case MSGTAG_STICK_RIGHT:
            character = 0xFFF2;
            break;
        case MSGTAG_STICK_VERTICAL:
            character = 0xFFF3;
            break;
        case MSGTAG_STICK_HORIZONTAL:
            character = 0xFFF4;
            break;
        case MSGTAG_HEART:
            character = 0xFFF5;
            break;
        case MSGTAG_QUAVER:
            character = 0xFFF6;
        }
    
        pReference->addCharactor(character);
    }
}

void jmessage_tSequenceProcessor::do_rubyset(void const* i_data, u32 i_size) {
    if (field_0xac == 0) {
        u8 length = i_size - 1;
        u8* pRuby = (u8*)i_data;
        int index = 0;
        pRuby++;
    
        jmessage_tReference* pReference = (jmessage_tReference*)JMessage::TSequenceProcessor::getReference();
        while (index < length) {
            u8 bytes[3];
            bytes[0] = pRuby[index++];
            bytes[1] = pRuby[index++];
            bytes[2] = 0;

            int character = (((s8)bytes[0] & 0xFF) << 8) | ((s8)bytes[1] & 0xFF);
            pReference->addCharactor(changeKataToHira(character));
        }
    }
}

void jmessage_tSequenceProcessor::push_word() {
    jmessage_tReference* pReference = (jmessage_tReference*)JMessage::TSequenceProcessor::getReference();
    JMessage::TSequenceProcessor::stack_pushCurrent(pReference->getWord(field_0xb5));
    field_0xb5++;
    pReference->setNowWordCount(field_0xb5);
}

void jmessage_tSequenceProcessor::messageSePlay(u8 i_speaker, u8 i_mood, cXyz* i_pos) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    dMsgObject_c* pObject = pReference->getObjectPtr();

    if (i_pos != NULL && pObject->isNoDemoFlag()) {
        if (dComIfGp_roomControl_getStayNo() != 0) {
            s8 reverb = dComIfGp_getReverb(dComIfGp_roomControl_getStayNo());
            OS_REPORT("message se play1===>%d\n", i_mood);
            Z2GetSpeechMgr2()->playOneShotVoice(i_speaker, i_mood, i_pos, reverb);
        } else {
            OS_REPORT("message se play2===>%d\n", i_mood);
            Z2GetSpeechMgr2()->playOneShotVoice(i_speaker, i_mood, i_pos, 0);
        }
    } else {
        if (dComIfGp_roomControl_getStayNo() != 0) {
            s8 reverb = dComIfGp_getReverb(dComIfGp_roomControl_getStayNo());
            OS_REPORT("message se play3===>%d\n", i_mood);
            Z2GetSpeechMgr2()->playOneShotVoice(i_speaker, i_mood, NULL, reverb);
        } else {
            OS_REPORT("message se play4===>%d\n", i_mood);
            Z2GetSpeechMgr2()->playOneShotVoice(i_speaker, i_mood, NULL, 0);
        }
    }
}

void jmessage_tSequenceProcessor::do_jump(void const* pEntry, char const* pszText) {}

void jmessage_tSequenceProcessor::calcStringLength() {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    pReference->onSelectSetCancelFlag();
    mMeasureProcessor.process_messageEntryText(this, mpEntry, mpText);
    pReference->offSelectSetCancelFlag();
}

jmessage_tRenderingProcessor::jmessage_tRenderingProcessor(jmessage_tReference const* pReference) : JMessage::TRenderingProcessor(pReference) {
    mpOutFont = NULL;
    mCharInfoPtr = NULL;
    mpCharInfoCnt = 0;
    field_0x44 = 0.0f;
    field_0x48 = field_0x4c = 0.0f;
    mTextInitPosX = mTextInitPosY = 0.0f;
    mTextInitScaleX = mTextInitScaleY = 1.0f;

    for (int i = 0; i < 3; i++) {
        mSelTextInitPosX[i] = mSelTextInitPosY[i] = 0.0f;
    }

    field_0x11c = 0;
    field_0x128 = 0.0f;
    field_0x12c = 0.0f;
    field_0x130 = 0.0f;
    field_0x134 = 0.0f;
    field_0x14c = 0;
    field_0x140 = 0;
    field_0x142 = 0;
    field_0x144 = 0;
    field_0x146 = 0;
    field_0x148 = 0;
    field_0x14a = 0;
    field_0x13c = 0;
    field_0x13e = 0;
    field_0x184[0] = field_0x184[1] = field_0x184[2] = 0;
    field_0x14d = 0;
    field_0x14e = 0;

    mCCColor = 0xFFFFFFFF;
    mGCColor = 0xFFFFFFFF;

    field_0x14f = 0;
    mColorNo = 0;
    field_0x151 = 0;
    field_0x138 = 0.0f;

    for (int i = 0; i < ARRAY_SIZE(field_0x7c); i++) {
        field_0x7c[i] = 0.0f;
    }
}

void jmessage_tRenderingProcessor::do_reset() {}

static void dummyString2() {
    DEAD_STRING("");
}

void jmessage_tRenderingProcessor::do_begin(void const* pEntry, char const* pszText) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    field_0x128 = 0.0f;
    field_0x12c = 0.0f;
    field_0x130 = 0.0f;
    field_0x134 = 0.0f;
    field_0x14c = 0;
    field_0x142 = 0;
    field_0x144 = 0;
    field_0x146 = 0;
    field_0x148 = 0;
    field_0x14a = 0;
    field_0x13c = 0;
    field_0x13e = 0;
    field_0x138 = 0.0f;
    field_0x184[0] = field_0x184[1] = field_0x184[2] = 0;

    if (mCharInfoPtr != NULL) {
        mpCharInfoCnt = (s16*)((u8*)mCharInfoPtr + 0x1068);
        *mpCharInfoCnt = 0;
    }

    field_0x14d = 0;
    field_0x140 = 0;
    field_0x44 = 1.0f;
    field_0x14e = 0;

    mCCColor = 0xFFFFFFFF;
    mGCColor = 0xFFFFFFFF;

    field_0x14f = pReference->getTopWordCount();
    field_0x187 = 0;
    field_0x48 = mTextInitPosX;
    field_0x4c = mTextInitPosY;
    field_0x151 = 0;

    mpOutFont->initialize();

    for (int i = 0; i < ARRAY_SIZE(field_0x7c); i++) {
        field_0x7c[i] = 0.0f;
    }

    field_0x11c = 0;
    strcpy(pReference->getTextPtr(), "");
    strcpy(pReference->getTextSPtr(), "");
    strcpy(pReference->getRubyPtr(), "");

    for (int i = 0; i < 3; i++) {
        strcpy(pReference->getSelTextPtr(i), "");
        strcpy(pReference->getSelRubyPtr(i), "");
    }

    if (1.0f != pReference->getDistanceScale()) {
        do_scale(field_0x44);
    }

    do_color(pReference->getTopColorType());
    pReference->resetDrawLightCount();

    do_widthcenter();
    do_heightcenter();

    field_0x13c = pReference->getTopTagScale();
    if (field_0x13c != 0) {
        do_scale(field_0x13c / 100.0f);
    }
}

void jmessage_tRenderingProcessor::do_end() {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    if (dMsgObject_getSelectWordFlag() != 0) {
        for (int i = 0; i < dMsgObject_getSelectWordFlag(); i++) {
            char buffer[200];
            strcpy(buffer, dMsgObject_getSelectWord(i));

            if (pReference->getSelectNum() == 2) {
                strcat(pReference->getSelTextPtr(i + 1), buffer);
            } else if (pReference->getSelectNum() == 3) {
                strcat(pReference->getSelTextPtr(i), buffer);
            }
        }
    }

    u8 batchColorFlag = pReference->getBatchColorFlag();
    if (batchColorFlag != 0) {
        batchColorFlag--;
        pReference->setBatchColorFlag(batchColorFlag);
    }
}

void jmessage_tRenderingProcessor::do_character(int i_character) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    JUTFont* pFont = pReference->getFont();

    char local_d4[3];
    local_d4[2] = 0;

    if ((i_character & 0xFF00) != 0) {
        local_d4[0] = ((i_character & 0xFF00) >> 8);
        local_d4[1] = i_character & 0xFF;
    } else {
        local_d4[0] = i_character & 0xFF;
        local_d4[1] = 0;
    }

    if (i_character == 10) {
        field_0x142++;

        if (field_0x14e != 0) {
            field_0x48 = mSelTextInitPosX[field_0x14e - 1];
        } else {
            field_0x48 = mTextInitPosX;
        }

        field_0x4c = field_0x4c + pReference->getLineSpace();
        if (field_0x140 != 0 && pReference->getNowPageType() != 4 && pReference->getNowPageType() != 5)
        {
            if (field_0x44 > 1.0f) {
                field_0x44 = 1.0f;
            }
            field_0x140 = 0;
        }
    
        do_strcat(local_d4, true, true, false);
        field_0x12c = 0.0f;

        if (field_0x14e != 0) {
            do_rubystrcat(local_d4, pReference->getSelRubyPtr(field_0x14e - 1), pReference->getSelRubyCharSpace(), 0.0f);
        } else {
            do_rubystrcat(local_d4, pReference->getRubyPtr(), pReference->getRubyCharSpace(), 0.0f);
            pReference->addDrawLightCount();
        }

        do_widthcenter();
        do_heightcenter();
    } else {
        f32 var_f31;
        if (field_0x187 == 0) {
            var_f31 = pReference->getFontSizeX() / pFont->getCellWidth();
            if (1.0f != field_0x44) {
                s16 iVar9 = 0.5f + field_0x44 * pReference->getFontSizeX();
                var_f31 = (f32)iVar9 / (f32)pFont->getCellWidth();
            }
            field_0x7c[field_0x142] = var_f31 * pFont->getOffset(i_character);
            field_0x48 += field_0x7c[field_0x142];
            field_0x187 = 1;
        }
        
        if (field_0x14e != 0) {
            var_f31 = pReference->getSelFontSize() / pFont->getCellWidth();
            field_0x48 += (var_f31 * (field_0x44 * pFont->getWidth(i_character)) + pReference->getSelCharSpace());
        } else {
            pReference->addDrawLightCount();
            var_f31 = pReference->getFontSizeX() / pFont->getCellWidth();

            if (checkCharInfoCharactor(i_character) && mCharInfoPtr != NULL) {
                f32 var_f29 = (f32)pFont->getWidth() / pFont->getCellWidth();
                f32 var_f28 = var_f31 * (field_0x44 * pFont->getWidth(i_character));
                f32 var_f27 = var_f29 * (field_0x44 * pReference->getFontSizeY());

                f32 var_f30 = 1.0f;
                if (i_character == 0x8145) {
                    var_f30 = 0.3f;
                }

                f32 var_f26 = mTextInitPosY + mTextInitScaleY * (field_0x4c - mTextInitPosY);
                f32 var_f25 = mTextInitPosX + mTextInitScaleX * (field_0x48 - mTextInitPosX);
                getCharInfo(0.5f + (var_f25 + (0.5f * var_f28)),
                            field_0x138 + (var_f26 + 0.5f * var_f27),
                            field_0x44 * var_f30, field_0x44 * var_f30, 1.0f);
            }

            f32 var_f24 = pReference->getCharSpace();
            var_f31 = pReference->getFontSizeX() / pFont->getCellWidth();
            if (1.0f != field_0x44) {
                s16 iVar9 = 0.5f + field_0x44 * pReference->getFontSizeX();
                var_f31 = (f32)iVar9 / (f32)pFont->getCellWidth();
            }
            field_0x48 += var_f24 + (var_f31 * pFont->getWidth(i_character));
        }

        if (field_0x14c != 0) {
            --field_0x14c;
            if (field_0x14c == 0) {
                if (field_0x14e != 0) {
                    f32 var_f23 = (field_0x48 - mSelTextInitPosX[field_0x14e - 1] - pReference->getSelCharSpace());
                    do_rubystrcat(field_0x152, pReference->getSelRubyPtr(field_0x14e - 1),
                                  pReference->getSelRubyCharSpace(), var_f23);
                } else {
                    f32 var_f22 = (field_0x48 - mTextInitPosX) - pReference->getCharSpace();
                    do_rubystrcat(field_0x152, pReference->getRubyPtr(), pReference->getRubyCharSpace(),
                                  var_f22);
                }
            }
        }

        do_strcat(local_d4, true, true, false);
    }
}

bool jmessage_tRenderingProcessor::do_tag(u32 i_tag, void const* i_data, u32 i_size) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    switch(i_tag & 0xFF0000) {
    case MSGTAG_GROUP(1):
    case MSGTAG_GROUP(2):
        return 1;
    case MSGTAG_GROUP(255):
        switch (i_tag) {
        case MSGTAG_COLOR:
            do_color(*(u8*)i_data & 0xFF);
            return 1;
        case MSGTAG_SCALE:
            field_0x13c = *(u16*)i_data & 0xFFFF;
            do_scale(field_0x13c / 100.0f);
            return 1;
        case MSGTAG_RUBY:
        case 0xFFFF02:
            do_rubyset(i_data, i_size);
            return 1;
        }
    case MSGTAG_GROUP(0):
        switch (i_tag) {
        case MSGTAG_PLAYER_NAME:
        case MSGTAG_HORSE_NAME:
            push_word();
            return 1;
        case MSGTAG_UNK_53:
            stack_pushCurrent(dMsgObject_getWord());
            field_0x151 = dMsgObject_getSelectWordFlag();
            return 1;
        case MSGTAG_SCENT_NAME:
            stack_pushCurrent(pReference->getObjectPtr()->getSmellName());
            return 1;
        case MSGTAG_PORTAL_NAME:
            stack_pushCurrent(pReference->getObjectPtr()->getPortalName());
            return 1;
        case MSGTAG_BOMB_NAME:
            stack_pushCurrent(pReference->getObjectPtr()->getBombName());
            return 1;
        case MSGTAG_INSECT_NAME:
            stack_pushCurrent(pReference->getObjectPtr()->getInsectName());
            return 1;
        case MSGTAG_LETTER_NAME:
            stack_pushCurrent(pReference->getObjectPtr()->getLetterName());
            return 1;
        case MSGTAG_LINE_DOWN:
            do_linedown(*(u16*)i_data);
            return 1;
        case MSGTAG_CURRENT_LETTER_PAGE:
        case MSGTAG_MAX_LETTER_PAGE:
                push_word();
                return 1;
        case MSGTAG_SELECT_2WAY:
            if (field_0x14e == 0) {
                field_0x14e = 1;
            }
        case MSGTAG_SELECT_3WAY:
            field_0x14e++;
            field_0x11c = 0;
            field_0x48 = mSelTextInitPosX[field_0x14e - 1];
            do_selwidthcenter(field_0x14e - 1);
            return 1;
        case MSGTAG_INLINE_2_NEXT:
        case MSGTAG_INLINE_2_FIRST:
            do_arrow2();
            return 1;
        case MSGTAG_INLINE_3_NEXT:
        case MSGTAG_INLINE_3_FIRST:
            do_arrow2();
            return 1;
        case MSGTAG_AWAIT_CHOICE:
            do_character('\n');
            stack_pushCurrent(pReference->getSelMsgPtr());
            return 1;
        case MSGTAG_UNK_33:
            do_name1();
            return 1;
        case MSGTAG_ABTN:
            do_outfont(0, 0);
            return 1;
        case MSGTAG_BBTN:
            do_outfont(1, 0);
            return 1;
        case MSGTAG_CSTICK:
            do_outfont(2, 0);
            return 1;
        case MSGTAG_LBTN:
            do_outfont(3, 0);
            return 1;
        case MSGTAG_RBTN:
            do_outfont(4, 0);
            return 1;
        case MSGTAG_XBTN:
            do_outfont(5, 0);
            return 1;
        case MSGTAG_YBTN:
            do_outfont(6, 0);
            return 1;
        case MSGTAG_ZBTN:
            do_outfont(7, 0);
            return 1;
        case MSGTAG_DPAD:
            do_outfont(8, 0);
            return 1;
        case MSGTAG_STICK_CROSS:
            do_outfont(9, 0);
            return 1;
        case MSGTAG_LEFT_ARROW:
            do_outfont(10, 0);
            return 1;
        case MSGTAG_RIGHT_ARROW:
            do_outfont(11, 0);
            return 1;
        case MSGTAG_UP_ARROW:
            do_outfont(12, 0);
            return 1;
        case MSGTAG_DOWN_ARROW:
            do_outfont(13, 0);
            return 1;
        case MSGTAG_STICK_UP:
            do_outfont(14, 0);
            return 1;
        case MSGTAG_STICK_DOWN:
            do_outfont(15, 0);
            return 1;
        case MSGTAG_STICK_LEFT:
            do_outfont(16, 0);
            return 1;
        case MSGTAG_STICK_RIGHT:
            do_outfont(17, 0);
            return 1;
        case MSGTAG_STICK_VERTICAL:
            do_outfont(18, 0);
            return 1;
        case MSGTAG_STICK_HORIZONTAL:
            do_outfont(19, 0);
            return 1;
        case MSGTAG_RED_TARGET:
            do_outfont(20, 0);
            return 1;
        case MSGTAG_YELLOW_TARGET:
            do_outfont(21, 0);
            return 1;
        case MSGTAG_INPUT_VALUE: {
            do_outfont(30, 0);
            dMsgObject_c* pMsgObject = pReference->mpObjectPtr;
            do_numset(pMsgObject->getInputValue());
            return 1;
        }
        case MSGTAG_ACKNOWLEDGE:
            return 1;
        case MSGTAG_ABTN_STAR:
            do_outfont(23, 0);
            return 1;
        case MSGTAG_WHITE_TARGET:
            do_outfont(24, 0);
            return 1;
        case MSGTAG_WARP_ICON:
            do_outfont(25, 0);
            return 1;
        case MSGTAG_XYBTN: {
            dMsgObject_c* pMsgObject = pReference->getObjectPtr();
            if (pMsgObject->getItemEquipButton() == 0) {
                do_outfont(5, 0);
            } else {
                do_outfont(6, 0);
            }
            return 1;
        }
        case MSGTAG_YXBTN:
            if (pReference->getObjectPtr()->getItemEquipButton() == 0) {
                do_outfont(6, 0);
            } else {
                do_outfont(5, 0);
            }
            return 1;
        case MSGTAG_BOMB_BAG_ICON:
            do_outfont(41, 0);
            return 1;
        case MSGTAG_HEART:
            do_outfont(27, 0);
            return 1;
        case MSGTAG_QUAVER:
            do_outfont(28, 0);
            return 1;
        case MSGTAG_BOMB_NUM: {
            char buffer[40];
            u8 bombNum = pReference->getObjectPtr()->getSelectBombNum();
            dMsgUnit_setTag(7, bombNum, buffer);
            strcpy(pReference->getWord(field_0x14f), buffer);
            push_word();
            return 1;
        }
        case MSGTAG_BOMB_PRICE: {
            char buffer[40];
            s16 bombPrice = pReference->getObjectPtr()->getSelectBombPrice();
            dMsgUnit_setTag(1, bombPrice, buffer);
            strcpy(pReference->getWord(field_0x14f), buffer);
            push_word();
            return 1;
        }
        case MSGTAG_BOMB_MAX:
        case MSGTAG_ARROW_MAX:
            push_word();
            return 1;
        case MSGTAG_INSTANT:
        case MSGTAG_TYPE:
        case MSGTAG_UNK_3:
        case MSGTAG_AUTOBOX:
        case MSGTAG_BOXATMOST:
        case MSGTAG_UNK_6:
        case MSGTAG_PAUSE:
        case MSGTAG_DEMOBOX:
        case MSGTAG_BOXATLEAST:
            return 1;
        }
    case MSGTAG_GROUP(4):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_GLYPH_DOLLARSIGN:
        case MSGTAG_GLYPH_BACKSLASH:
        case MSGTAG_GLYPH_ATMARK:
        case MSGTAG_GLYPH_SHARP:
        case MSGTAG_GLYPH_FLAT:
        case MSGTAG_GLYPH_SQRT:
        case MSGTAG_GLYPH_PERCENT:
        case MSGTAG_GLYPH_HECTARE:
        case MSGTAG_GLYPH_ARE:
        case MSGTAG_GLYPH_LITRE:
        case MSGTAG_GLYPH_WATT:
        case MSGTAG_GLYPH_CALORIE:
        case MSGTAG_GLYPH_DOLLAR:
        case MSGTAG_GLYPH_CENT:
            push_word();
            return 1;
        }
    case MSGTAG_GROUP(5):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_TIME_INFO:
        case 1:
        case 2:
        case MSGTAG_INSECT_INFO:
        case 4:
        case 5:
        case 6:
        case MSGTAG_RIVER_POINTS:
        case MSGTAG_FISH_LENGTH:
        case MSGTAG_FUNDRAISE_REMAIN:
        case MSGTAG_NEW_LETTER_NUM:
        case MSGTAG_POE_NUM:
        case MSGTAG_BALLOON_SCORE:
        case MSGTAG_FISH_COUNT:
        case MSGTAG_ROLLGOAL_LV:
            push_word();
            return 1;
        }
    case MSGTAG_GROUP(6):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_PLAYER_GENITIV:
        case MSGTAG_HORSE_GENITIV:
        case MSGTAG_MALE_ICON:
        case MSGTAG_FEMALE_ICON:
        case MSGTAG_STAR_ICON:
        case MSGTAG_REFMARK:
        case MSGTAG_THIN_LEFT_ARROW:
        case MSGTAG_THIN_RIGHT_ARROW:
        case MSGTAG_THIN_UP_ARROW:
        case MSGTAG_THIN_DOWN_ARROW:
            push_word();
            return 1;
        case MSGTAG_BULLET:
            do_outfont(42, 0);
            return 1;
        case MSGTAG_BULLET_SPACE:
            do_outfont(43, 0);
            return 1;
        }
    case MSGTAG_GROUP(3):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_WII_ABTN:
            do_outfont(0, 0);
            return 1;
        case MSGTAG_WII_BBTN:
            do_outfont(51, 0);
            return 1;
        case MSGTAG_WII_HOMEBTN:
            do_outfont(52, 0);
            return 1;
        case MSGTAG_WII_MINUSBTN:
            do_outfont(53, 0);
            return 1;
        case MSGTAG_WII_PLUSBTN:
            do_outfont(54, 0);
            return 1;
        case MSGTAG_WII_1BTN:
            do_outfont(63, 0);
            return 1;
        case MSGTAG_WII_2BTN:
            do_outfont(64, 0);
            return 1;
        case MSGTAG_WII_DPAD_ITEM:
            do_outfont(50, 0);
            return 1;
        case MSGTAG_WII_DPAD_UP:
            do_outfont(49, 0);
            return 1;
        case MSGTAG_WII_DPAD_DOWN:
            do_outfont(65, 0);
            return 1;
        case MSGTAG_WII_DPAD_HORIZONTAL:
            do_outfont(66, 0);
            return 1;
        case MSGTAG_WII_DPAD_RIGHT:
            do_outfont(67, 0);
            return 1;
        case MSGTAG_WII_DPAD_LEFT:
            do_outfont(68, 0);
            return 1;
        case MSGTAG_WII_WIIMOTE:
            if (dComIfGs_getOptPointer() == 0) {
                do_outfont(9, 0);
            } else {
                do_outfont(44, 0);
            }
            return 1;
        case MSGTAG_WII_RETICULE:
            do_outfont(69, 0);
            return 1;
        case MSGTAG_WII_NUNCHUK:
            do_outfont(45, 0);
            return 1;
        case MSGTAG_WII_WIIMOTE2:
            do_outfont(44, 0);
            return 1;
        case MSGTAG_WII_FAIRY:
            do_outfont(62, 0);
            return 1;
        case MSGTAG_WII_CBTN:
            do_outfont(2, 0);
            return 1;
        case MSGTAG_WII_ZBTN:
            do_outfont(7, 0);
            return 1;
        case MSGTAG_WII_MSGID_OVERRIDE:
            return 1;
        default:
            break;
        }
        break;
    }

    return 0;
}

void jmessage_tRenderingProcessor::resetRendering() {
    if (mCharInfoPtr == NULL) return;

    for (int i = 0; i < 150; i++) {
        mCharInfoPtr[i].field_0x14 = 0.0f;
        mCharInfoPtr[i].field_0x19 = 0;
    }
    *mpCharInfoCnt = 0;
}

void jmessage_tRenderingProcessor::do_widthcenter() {
    field_0x187 = 0;
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    s16 sVar3 = field_0x142 + pReference->getStartLineCount();
    if (pReference->getLineArrange(sVar3) == 1 || pReference->getLineArrange(sVar3) == 2) {
        return;
    }
    
    f32 fVar6 = 0.5f + (pReference->getTBoxWidth() - getLineLength(field_0x142)) / 2;
    if (fVar6 >= 1.0f) {
        char buffer[16];
        snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CR[%d]", (int)fVar6);
        do_strcat(buffer, false, true, true);
        field_0x48 += fVar6;
    }
}

void jmessage_tRenderingProcessor::do_selwidthcenter(int param_1) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    f32 fVar5 = 0.5f + (pReference->getSelTBoxWidth() - pReference->getSelLength(param_1)) / 2;
    if (fVar5 >= 1.0f) {
        char buffer[16];
        snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CR[%d]", (int)fVar5);
        do_strcat(buffer, false, true, false);
        field_0x48 += fVar5;
    }
}

void jmessage_tRenderingProcessor::do_heightcenter() {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    field_0x138 = 0.0f;

    f32 var_f31;
    if (field_0x142 == 0) {
        var_f31 = (pReference->getTBoxHeight() - (pReference->getFontSizeY() + (pReference->getLineSpace() * (pReference->getLineMax() - 1)))) / 2;
    } else {
        var_f31 = 0.0f;
    }

    switch (pReference->getNowPageType()) {
    case 0:
        if (field_0x142 == 0) {
            int nowPageLine = pReference->getNowPageLine();
            if (pReference->getLineMax() != (s16)nowPageLine) {
                var_f31 += pReference->getLineSpace() * (0.5f * (pReference->getLineMax() - (s16)nowPageLine));
            }
        }
        break;
    case 1: {
        if (field_0x142 == 0) {
            int nowPageLine = pReference->getNowPageLine();
            if (nowPageLine == 1) {
                field_0x138 = pReference->getLineSpace() * (0.5f * (pReference->getLineMax() - (s16)nowPageLine));
                var_f31 += field_0x138;
            }
        }
        break;
    }
#if REGION_JPN
    case 2:
        if ((s8)pReference->getLineMax() == 3) {
            int nowPageLine = pReference->getNowPageLine();
            field_0x138 = pReference->getLineSpace() * (0.5f * (pReference->getLineMax() - (s16)nowPageLine));
            var_f31 += field_0x138;
            break;
        }
        if (field_0x142 == 0) {
            field_0x138 = pReference->getLineSpace();
            var_f31 += field_0x138;
        } else if (field_0x142 == 1) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }
        break;
    case 3: {
        if (field_0x142 == 1) {
            int nowPageLine = pReference->getNowPageLine();
            field_0x138 = pReference->getLineSpace() * (0.5f * (pReference->getLineMax() - (s16)nowPageLine));
            var_f31 += field_0x138;
            break;
        }
        break;
    }
    case 4: {
        if (field_0x142 == 1) {
            var_f31 += 0.5f * pReference->getLineSpace();
        }

        f32 sp8 = pReference->getLineScale(field_0x142) / 100.0f;
        f32 dVar15 = (pReference->getLineSpace() - (sp8 * pReference->getFontSizeY())) / 2;
        field_0x138 += dVar15;
        var_f31 += dVar15;
        break;
    }
    case 5: {
        if (field_0x142 >= 1) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }

        f32 sp8 = pReference->getLineScale(field_0x142) / 100.0f;
        f32 dVar15 = (pReference->getLineSpace() - (sp8 * pReference->getFontSizeY())) / 2;
        field_0x138 += dVar15;
        var_f31 += dVar15;

        break;
    }
    case 6:
        if (field_0x142 <= 1) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }
        break;
    case 7:
        if (field_0x142 == 1 || field_0x142 == 2) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }
        break;
    case 8:
        if (field_0x142 == 2) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }
#else
    case 2:
        if (field_0x142 == 0) {
            field_0x138 = pReference->getLineSpace();
            var_f31 += field_0x138;
        } else {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }
        break;
    case 3: {
        int nowPageLine = pReference->getNowPageLine();
        field_0x138 = 0.5f * pReference->getLineSpace();
        var_f31 += field_0x138;
        break;
    }
    case 4: {
        if (field_0x142 == 0) {
            int nowPageLine = pReference->getNowPageLine();
            field_0x138 = pReference->getLineSpace() * (0.5f * (pReference->getLineMax() - (s16)nowPageLine));
            var_f31 += field_0x138;
        }

        if (field_0x142 == 1) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }

        f32 spC = pReference->getLineScale(field_0x142) / 100.0f;
        f32 dVar15 = ((pReference->getLineSpace() - (spC * pReference->getFontSizeY())) / 2);
        field_0x138 += dVar15;
        var_f31 += dVar15;
        break;
    }
    case 5: {
        if (field_0x142 >= 1) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }

        f32 sp8 = pReference->getLineScale(field_0x142) / 100.0f;
        f32 dVar15 = (pReference->getLineSpace() - (sp8 * pReference->getFontSizeY())) / 2;
        field_0x138 += dVar15;
        var_f31 += dVar15;
        break;
    }
    case 6:
        if (field_0x142 <= 1) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }
        break;
    case 7:
        if (field_0x142 == 1 || field_0x142 == 2) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }
        break;
    case 8:
        if (field_0x142 == 2) {
            field_0x138 = 0.5f * pReference->getLineSpace();
            var_f31 += field_0x138;
        }
#endif
    }

    if (field_0x142 == 0) {
        field_0x138 = 0.0f;
    }

    if (0.0f != var_f31) {
        field_0x4c += var_f31;

        char buffer[16];
        snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CD[%d]", (int)var_f31);
        do_strcat(buffer, false, true, true);

        if (field_0x14e != 0) {
            do_rubystrcat(buffer, pReference->getSelRubyPtr(field_0x14e - 1), pReference->getSelRubyCharSpace(), 0.0f);
        } else {
            do_rubystrcat(buffer, pReference->getRubyPtr(), pReference->getRubyCharSpace(), 0.0f);
        }
    }
}

void jmessage_tRenderingProcessor::do_color(u8 i_colorNo) {
    jmessage_tReference* reference_p = (jmessage_tReference*)getReference();

    mColorNo = i_colorNo;
    mCCColor = getFontCCColorTable(i_colorNo, reference_p->getFukiKind());
    mGCColor = getFontGCColorTable(i_colorNo, reference_p->getFukiKind());

    char buffer[40];
    sprintf(buffer,
            "\x1B"
            "CC[%08x]"
            "\x1B"
            "GC[%08x]",
            mCCColor, mGCColor);
    do_strcat(buffer, false, false, false);
}

void jmessage_tRenderingProcessor::do_scale(f32 param_1) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    f32 dVar10 = (param_1 * pReference->getDistanceScale());
    int scale_x = 0.5f + (dVar10 * pReference->getFontSizeX());
    int iVar2 = 0.5f + (dVar10 * pReference->getFontSizeY());
    int uVar6 = pReference->getNowPageType() - 4 == 0;
    int scale_y = iVar2;

    if (field_0x44 != dVar10) {
        int iVar2 = 0.5f + (field_0x44 * pReference->getFontSizeY());
        s16 sVar5 = ((s16)scale_y - (s16)iVar2) / 2;
        field_0x44 = dVar10;
        field_0x140 += sVar5;
        do_transY(sVar5, true);
        field_0x13e = sVar5;
    }

    char buffer[32];
    sprintf(buffer,
            "\x1B"
            "FX[%d]"
            "\x1B"
            "FY[%d]",
            (s16)scale_x, (s16)scale_y);
    do_strcat(buffer, false, true, false);
}

void jmessage_tRenderingProcessor::do_linedown(s16 param_0) {
    char buffer[16];

    sprintf(buffer, "\x1B" "CD[%d]", param_0);
    do_strcat(buffer, false, true, false);
}

void jmessage_tRenderingProcessor::do_transY(s16 i_transY, bool unused) {
    if (i_transY != 0) {
        char buffer0[16];
        char buffer1[16];

        if (i_transY < 0) {
            sprintf(buffer0, "\x1B" "CU[%d]", -i_transY);
            sprintf(buffer1, "\x1B" "CD[%d]", -i_transY);
        } else {
            sprintf(buffer0, "\x1B" "CD[%d]", i_transY);
            sprintf(buffer1, "\x1B" "CU[%d]", i_transY);
        }

        field_0x4c -= i_transY;

        do_strcat(buffer0, false, true, true);
        jmessage_tReference* pReference = (jmessage_tReference*)getReference();
        do_rubystrcat(buffer1, pReference->getRubyPtr(), pReference->getRubyCharSpace(), 0.0f);
    }
}

void jmessage_tRenderingProcessor::do_outfont(u8 i_iconNo, u32 i_color) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    JUTFont* pFont = pReference->getFont();  // fakematch
    f32 cellWidth = pFont->getCellWidth();
    f32 width = pFont->getWidth() / cellWidth;

    int var_r28 = 1;
    f32 sizeX = (24.0f * field_0x44 * mTextInitScaleX);
    f32 sizeY = (24.0f * field_0x44 * mTextInitScaleY);
    f32 posX = (mTextInitPosX + mTextInitScaleX * (field_0x48 - mTextInitPosX));
    f32 posY = (mTextInitPosY + mTextInitScaleY * (field_0x4c - mTextInitPosY));

    u32 color;
    if (i_color == 0) {
        color = mCCColor;
    } else {
        color = i_color;
    }

    if (i_iconNo == 30) {
        mpOutFont->setRupeeColor(pReference->getOutFontRupeeColor());
    }

    mpOutFont->drawFont(NULL, i_iconNo, posX - field_0x7c[field_0x142], posY, sizeX, sizeY, color, 0xFF);
    pReference->addDrawLightCount();

    if (mCharInfoPtr != NULL) {
        getCharInfo(0.5f + (posX + (0.5f * sizeX)), field_0x138 + (posY + (0.5f * sizeY)),
                    field_0x44, field_0x44, 1.0f);
    }

    f32 spacing = 0.5f + ((field_0x44 * (sizeX * var_r28)) + pReference->getCharSpace());
    field_0x48 += (int)spacing;

    char buffer[16];
    snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CR[%d]", (int)spacing);
    do_strcat(buffer, false, true, false);
}

void jmessage_tRenderingProcessor::do_arrow2() {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    JUTFont* pFont = pReference->getFont();  // fakematch
    f32 cellWidth = pFont->getCellWidth();
    f32 dVar11 = pFont->getWidth() / cellWidth;

    if (field_0x14d == pReference->getSelectPos()) {
        mpOutFont->drawFont(NULL, 29,
                            (mTextInitPosX + mTextInitScaleX * (field_0x48 - mTextInitPosX)) - field_0x7c[field_0x142],
                            field_0x4c, (dVar11 * (field_0x44 * pReference->getFontSizeX())),
                            dVar11 * (field_0x44 * pReference->getFontSizeY()), mCCColor, 0xFF);
    }

    f32 spacing = 0.5f + ((field_0x44 * pReference->getFontSizeX()) + pReference->getCharSpace());
    field_0x48 += (int)spacing;

    char buffer[16];
    snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CR[%d]", (int)spacing);
    do_strcat(buffer, false, true, false);
    field_0x14d++;
}

f32 jmessage_tRenderingProcessor::getLineLength(int i_lineNo) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    int lineNo = i_lineNo + (s16)pReference->getStartLineCount();
    f32 var_f31 = pReference->getStrLength(lineNo) * pReference->getFontSizeX();
    f32 var_f30 = pReference->getSpaceLength(lineNo) * pReference->getCharSpace();
    return var_f31 + var_f30;
}

void jmessage_tRenderingProcessor::do_strcat(char* i_str, bool param_2, bool param_3, bool param_4) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    if (pReference->isCharSend() || !param_2) {
        field_0x11c += strlen(i_str);
        if (field_0x14e != 0) {
            if (field_0x11c < 50) {
                strcat(pReference->getSelTextPtr(field_0x14e - 1), i_str);
            } else {
                JUT_WARN(5316, "%s", "TextBox Alloc Byte Over!!");
            }
        } else if (field_0x11c < D_MSG_CLASS_CHAR_CNT_MAX) {
            if (param_2) {
                field_0x146++;
                if (pReference->getBatchColorFlag() != 0) {
                    pReference->setCharCnt(field_0x146);
                } else if (field_0x146 > pReference->getCharCnt() || (field_0x146 == pReference->getCharCnt() && pReference->getCharAlpha() < 255.0f))
                {
                    if (field_0x146 > pReference->getCharCnt()) {
                        pReference->setCharAlpha(0.0f);
                        pReference->setCharCnt(field_0x146);
                    }

                    if (pReference->getCharAlpha() < 255.0f) {
                        pReference->addCharAlpha();
                        if (field_0x148 != 0) {
                            char* textPtr = pReference->getTextPtr();
                            textPtr[field_0x148] = 0;
                            strcat(textPtr, field_0x184);
                        }

                        if (field_0x14a != 0) {
                            char* textPtr = pReference->getTextSPtr();
                            textPtr[field_0x14a] = 0;
                            strcat(textPtr, field_0x184);
                        }


                        u32 charColor = (mCCColor & 0xFFFFFF00) | ((int)pReference->getCharAlpha() & 0xFF);
                        u32 gradColor = (mGCColor & 0xFFFFFF00) | ((int)pReference->getCharAlpha() & 0xFF);
                        char buffer[36];
                        sprintf(buffer, "\x1b" "CC[%08x]" "\x1b" "GC[%08x]", charColor, gradColor);

                        int length = 0;
                        length = strlen(buffer);

                        if (field_0x11c + length < D_MSG_CLASS_CHAR_CNT_MAX) {
                            field_0x148 = strlen(pReference->getTextPtr());
                            field_0x14a = strlen(pReference->getTextSPtr());

                            strcpy(field_0x184, i_str);
                            strcat(pReference->getTextPtr(), buffer);
                            strcat(pReference->getTextSPtr(), buffer);
                        } else {
                            JUT_WARN(5362, "%s", "TextBox Alloc Byte Over!!");
                        }
                    }
                }
            } else if (param_4) {
                field_0x148 = 0;
                field_0x14a = 0;
            }
            
            strcat(pReference->getTextPtr(), i_str);
            if (param_3) {
                strcat(pReference->getTextSPtr(), i_str);
            }
        } else {
            JUT_WARN(5380, "%s", "TextBox Alloc Byte Over!!");
        }
    }
}

void jmessage_tRenderingProcessor::do_rubyset(void const* i_data, u32 i_size) {
    jmessage_tReference* pReference = (jmessage_tReference*) getReference();
    JUTFont* pFont = pReference->getFont();

    u8 mRubyCnt = i_size - 1;
    u8* pRuby = (u8*)i_data;
    int index = 0;

    JUT_ASSERT(5405, mRubyCnt < (50));
    field_0x14c = *(u8*)i_data & 0xFF;
    pRuby++;

    field_0x152[0] = 0;
    field_0x128 = 0.0f;

    while (index < mRubyCnt) {
        u8 buffer[3];
        buffer[0] = pRuby[index++];
        buffer[1] = pRuby[index++];
        buffer[2] = 0;
        strcat(field_0x152, (const char*)buffer);
    
        int character = (((char)buffer[0] & 0xFF) << 8) | ((char)buffer[1] & 0xFF);
        if (field_0x14e != 0) {
            field_0x128 +=
                (pReference->getSelRubyCharSpace() +
                 pFont->getWidth(character) * (pReference->getSelRubySize() / pFont->getCellWidth()));
        } else {
            field_0x128 += (pReference->getRubyCharSpace() +
                            pFont->getWidth(character) * (pReference->getRubySize() / pFont->getCellWidth()));
        }
    }

    if (field_0x14e != 0) {
        field_0x128 -= pReference->getSelRubyCharSpace();
        field_0x130 = field_0x48 - mSelTextInitPosX[field_0x14e - 1];
    } else {
        field_0x128 -= pReference->getRubyCharSpace();
        field_0x130 = field_0x48 - mTextInitPosX;
    }
}

void jmessage_tRenderingProcessor::do_rubystrcat(char* i_src, char* i_dst, f32 i_charSpace, f32 param_4) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    if (pReference->isCharSend()) {
        if (0.0f != param_4) {
            field_0x134 = param_4;
            f32 cursor_trans = (0.5f * (field_0x130 + field_0x134) - 0.5f * field_0x128) - field_0x12c;
            if (cursor_trans >= 1.0f) {
                char buffer[16];
                snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CR[%d]", (int)cursor_trans);
                strcat(i_dst, buffer);
                field_0x12c += (int)cursor_trans;
            } else if (cursor_trans <= -1.0f) {
                char buffer[16];
                snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CL[%d]", (int)-cursor_trans);
                strcat(i_dst, buffer);
                field_0x12c += (int)cursor_trans;
            }
            field_0x12c += field_0x128 + i_charSpace;
        }

        strcat(i_dst, i_src);
    }
}

void jmessage_tRenderingProcessor::do_name1() {
    const char* name = dComIfGs_getPlayerName();
#if REGION_JPN
    int c = (((char)name[0] & 0xFF) << 8) | ((char)name[1] & 0xFF);
    // if first character is hiragana or katakana
    if ((c >= 0x829F && c <= 0x82F1) || (c >= 0x8340 && c <= 0x8396)) {
        push_word();
    }
#endif
}

void jmessage_tRenderingProcessor::do_numset(s16 i_num) {
    if (9999 < i_num) {
        i_num = 9999;
    }

    if (i_num < 0) {
        i_num = 0;
    }

    u32 color = 0xFF0000FF;
    jmessage_tReference* pReference = (jmessage_tReference*) getReference();
    if (pReference->getInputFigure() == 3) {
        do_outfont(getOutFontNumberType(i_num / 1000), color);
    } else {
        do_outfont(getOutFontNumberType(i_num / 1000), 0);
    }

    if (pReference->getInputFigure() == 2) {
        do_outfont(getOutFontNumberType(i_num % 1000 / 100), color);
    } else {
        do_outfont(getOutFontNumberType(i_num % 1000 / 100), 0);
    }

    if (pReference->getInputFigure() == 1) {
        do_outfont(getOutFontNumberType(i_num % 100 / 10), color);
    } else {
        do_outfont(getOutFontNumberType(i_num % 100 / 10), 0);
    }

    if (pReference->getInputFigure() == 0) {
        do_outfont(getOutFontNumberType(i_num % 10), color);
    } else {
        do_outfont(getOutFontNumberType(i_num % 10), 0);
    }
}

void jmessage_tRenderingProcessor::push_word() {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    stack_pushCurrent(pReference->getWord(field_0x14f));
    field_0x14f++;
}

void jmessage_tRenderingProcessor::getCharInfo(f32 param_1, f32 param_2, f32 param_3, f32 param_4, f32 param_5) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();

    if (pReference->isLightSend()) {
        if (*mpCharInfoCnt >= 150) {
            JUT_WARN(5608, "%s", "CharInfo stack over!!");
            JUT_ASSERT(5609, *mpCharInfoCnt < (150));
        }
        
        mCharInfoPtr[*mpCharInfoCnt].field_0x00 = param_1;
        mCharInfoPtr[*mpCharInfoCnt].field_0x04 = param_2;
        mCharInfoPtr[*mpCharInfoCnt].field_0x08 = param_3;
        mCharInfoPtr[*mpCharInfoCnt].field_0x0c = param_4;
        mCharInfoPtr[*mpCharInfoCnt].field_0x10 = param_5;
        mCharInfoPtr[*mpCharInfoCnt].mColorNo = mColorNo;

        if (!pReference->isBatchFlag() && mCharInfoPtr[*mpCharInfoCnt].field_0x19 == 0) {
            mCharInfoPtr[*mpCharInfoCnt].field_0x19 = 1;
        }

        (*mpCharInfoCnt)++;
    }
}

jmessage_string_tReference::jmessage_string_tReference() {
    mPanePtr = NULL;
    mRubyPanePtr = NULL;
    mpFont = mDoExt_getMesgFont();

    for (int i = 0; i < ARRAY_SIZE(mLineLength); i++) {
        mLineLength[i] = 0.0f;
        mOutfontLength[i] = 0.0f;
    }

    resetCharactor();
}

jmessage_string_tReference::~jmessage_string_tReference() {
}

void jmessage_string_tReference::init(J2DTextBox* panePtr, J2DTextBox* runyPanePtr,
                                          JUTFont* font, COutFont_c* outFontPtr, u8 flags) {
    mPanePtr = panePtr;
    mRubyPanePtr = runyPanePtr;
    mOutFontPtr = outFontPtr;
    mLineCount = 0;
    mLineMax = D_MSG_CLASS_LINE_MAX;
    mNowPage = 0;
    mFlags = flags;
    if (font != NULL) {
        mpFont = font;
    }

    for (int i = 0; i < ARRAY_SIZE(mLineLength); i++) {
        mLineLength[i] = 0.0f;
        mOutfontLength[i] = 0.0f;
    }

    if (mOutFontPtr != NULL) {
        mOutFontPtr->initialize();
    }
}

void jmessage_string_tReference::setCharactor(u16 param_0) {}

void jmessage_string_tReference::addCharactor(u16 param_0) {}

void jmessage_string_tReference::resetCharactor() {}

f32 jmessage_string_tReference::getLineLength(int lineNo) {
    if (lineNo < 0) {
        return 0.0f;
    }

    if (lineNo < mLineMax) {
        return mLineLength[lineNo];
    }

    return 0.0f;
}

void jmessage_string_tReference::addLineLength(int lineNo, f32 length) {
    if (lineNo >= 0 && lineNo < mLineMax)  {
        mLineLength[lineNo] += length;
    }
}

f32 jmessage_string_tReference::getOutfontLength(int param_0) {
    if (param_0 < 0) return 0.0f;
    if (param_0 < mLineMax) {
        return mOutfontLength[param_0];
    }
    return 0.0f;
}

void jmessage_string_tReference::setOutfontLength(int param_0, f32 param_1) {
    if (param_0 < 0) return;
    if (param_0 >= mLineMax) return;
    mOutfontLength[param_0] = param_1;
}

void jmessage_string_tReference::clearOutfontLength(int param_0) {
    if (param_0 < 0) return;
    if (param_0 >= mLineMax) return;

    if (mOutfontLength[param_0]) {
        mOutfontLength[param_0] = 0.0f;
    }
}

s16 jmessage_string_tReference::getLineCountNowPage() {
    s16 rv = -1;
    s32 iVar4 = mNowPage * mLineMax;
    s32 uVar1 = iVar4 + mLineMax;
    if (mLineCount >= iVar4 && mLineCount < uVar1) {
        rv = mLineCount - iVar4;
    }

    return rv;
}

jmessage_string_tControl::jmessage_string_tControl() {}

jmessage_string_tMeasureProcessor::jmessage_string_tMeasureProcessor(jmessage_string_tReference const* pReference) : JMessage::TRenderingProcessor(pReference) {
    mpReference = (jmessage_string_tReference*)getReference();
    mpReference->resetCharactor();
}

void jmessage_string_tMeasureProcessor::do_begin(void const* pEntry, char const* pszText) {
    (void)pEntry;
    (void)pszText;
    mpReference->setLineCount(0);
    mpReference->resetCharactor();
}

void jmessage_string_tMeasureProcessor::do_end() {
    mpReference->addLineCount();
}

void jmessage_string_tMeasureProcessor::do_character(int iCharacter) {
    JUTFont* pFont = mpReference->getFont();
    if (JUTFont::isLeadByte_ShiftJIS(iCharacter)) {
        if (!isOutfontKanjiCode(iCharacter)) {
            u16 character2Bytes = change1ByteTo2Bytes(iCharacter);
            mpReference->setCharactor(changeKataToHira(character2Bytes));
        }
    } else if (iCharacter < 0x8800 && !isOutfontKanjiCode(iCharacter)) {
        mpReference->setCharactor(changeKataToHira(iCharacter));
    }

    f32 charSpace;
    J2DTextBox::TFontSize fontSize;
    if (mpReference->getPanePtr() != NULL) {
        charSpace = mpReference->getPanePtr()->getCharSpace();
        mpReference->getPanePtr()->getFontSize(fontSize);
    } else {
        charSpace = 0.0f;
        fontSize.mSizeY = fontSize.mSizeX = 20.0f;
    }

    if (iCharacter == '\n') {
        mpReference->addLineCount();
    } else {
        f32 finalLength = charSpace + fontSize.mSizeX * ((f32)pFont->getWidth(iCharacter) / pFont->getCellWidth());
        mpReference->addLineLength(mpReference->getLineCountNowPage(), finalLength);
        mpReference->clearOutfontLength(mpReference->getLineCountNowPage());
    }
}

bool jmessage_string_tMeasureProcessor::do_tag(u32 i_tag, void const* i_data, u32 i_size) {
    f32 var_f30;
    J2DTextBox::TFontSize fontsize;
    if (mpReference->getPanePtr() != NULL) {
        var_f30 = ((J2DTextBox*)mpReference->getPanePtr())->getCharSpace();
        ((J2DTextBox*)mpReference->getPanePtr())->getFontSize(fontsize);
    } else {
        var_f30 = 0.0f;
        fontsize.mSizeX = fontsize.mSizeY = 20.0f;
    }

    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    switch(i_tag & 0xFF0000) {
    case MSGTAG_GROUP(255):
        switch(i_tag) {
        case MSGTAG_RUBY:
        case 0xFFFF02:
            do_rubyset(i_data, i_size);
            break;
        }
        break;
    case MSGTAG_GROUP(0):
        switch (i_tag) {
        case MSGTAG_PLAYER_NAME:
            stack_pushCurrent(dComIfGs_getPlayerName());
            break;
        case MSGTAG_HORSE_NAME:
            stack_pushCurrent(dComIfGs_getHorseName());
            break;
        case MSGTAG_UNK_53:
            stack_pushCurrent(dMsgObject_getWord());
            break;
        case MSGTAG_SCENT_NAME:
            stack_pushCurrent(dMsgObject_getMsgObjectClass()->getSmellName());
            break;
        case MSGTAG_PORTAL_NAME:
            stack_pushCurrent(dMsgObject_getMsgObjectClass()->getPortalName());
            break;
        case MSGTAG_BOMB_NAME:
            stack_pushCurrent(dMsgObject_getMsgObjectClass()->getBombName());
            break;
        case MSGTAG_INSECT_NAME:
            stack_pushCurrent(dMsgObject_getMsgObjectClass()->getInsectName());
            break;
        case MSGTAG_LETTER_NAME:
            stack_pushCurrent(dMsgObject_getMsgObjectClass()->getLetterName());
            break;
        case MSGTAG_CURRENT_LETTER_PAGE: {
            char buffer[4];
            sprintf(buffer, "%d", dComIfGp_getMessageCountNumber() / 100);
            stack_pushCurrent(buffer);
            break;
        }
        case MSGTAG_MAX_LETTER_PAGE: {
            char buffer[4];
            sprintf(buffer, "%d", dComIfGp_getMessageCountNumber() % 100);
            stack_pushCurrent(buffer);
            break;
        }
        case MSGTAG_ABTN:
        case MSGTAG_BBTN:
        case MSGTAG_CSTICK:
        case MSGTAG_LBTN:
        case MSGTAG_RBTN:
        case MSGTAG_XBTN:
        case MSGTAG_YBTN:
        case MSGTAG_ZBTN:
        case MSGTAG_DPAD:
        case MSGTAG_STICK_CROSS:
        case MSGTAG_LEFT_ARROW:
        case MSGTAG_RIGHT_ARROW:
        case MSGTAG_UP_ARROW:
        case MSGTAG_DOWN_ARROW:
        case MSGTAG_STICK_UP:
        case MSGTAG_STICK_DOWN:
        case MSGTAG_STICK_LEFT:
        case MSGTAG_STICK_RIGHT:
        case MSGTAG_STICK_VERTICAL:
        case MSGTAG_STICK_HORIZONTAL:
        case MSGTAG_RED_TARGET:
        case MSGTAG_YELLOW_TARGET:
        case MSGTAG_ABTN_STAR:
        case MSGTAG_WHITE_TARGET:
        case MSGTAG_WARP_ICON:
        case MSGTAG_XYBTN:
        case MSGTAG_YXBTN:
        case MSGTAG_BOMB_BAG_ICON:
        case MSGTAG_HEART:
        case MSGTAG_QUAVER: {
            mpReference->setCharactor(0xFFFF);
            f32 length = fontsize.mSizeX + var_f30;
            mpReference->addLineLength(mpReference->getLineCountNowPage(), length);
            mpReference->setOutfontLength(mpReference->getLineCountNowPage(), length);
            break;
        }
        case MSGTAG_INPUT_VALUE: {
            int i = 0;
            fontsize.mSizeX += var_f30;
            for (; i < 4; i++) {
                mpReference->addLineLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
                mpReference->setOutfontLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
            }
            break;
        }
        case MSGTAG_BOMB_NUM: {
            char buffer[40];
            u8 bombNum = dMsgObject_getMsgObjectClass()->getSelectBombNum();
            dMsgUnit_setTag(7, bombNum, buffer);
            stack_pushCurrent(buffer);
            mpReference->clearOutfontLength(mpReference->getLineCountNowPage());
            break;
        }
        case MSGTAG_BOMB_PRICE: {
            char buffer[40];
            s16 bombPrice = dMsgObject_getMsgObjectClass()->getSelectBombPrice();
            dMsgUnit_setTag(1, bombPrice, buffer);
            stack_pushCurrent(buffer);
            mpReference->clearOutfontLength(mpReference->getLineCountNowPage());
            break;
        }
        case MSGTAG_BOMB_MAX: {
            u8 bombType = fpcNm_ITEM_NORMAL_BOMB;
            if ((*(u8*)i_data) == 1) {
                bombType = fpcNm_ITEM_WATER_BOMB;
            } else if ((*(s8*)i_data & 0xFFU) == 2) {
                bombType = fpcNm_ITEM_POKE_BOMB;
            }

            char buffer[40];
            dMsgUnit_setTag(7, dComIfGs_getBombMax(bombType), buffer);
            stack_pushCurrent(buffer);
            mpReference->clearOutfontLength(mpReference->getLineCountNowPage());
            break;
        }
        case MSGTAG_ARROW_MAX: {
            char buffer[40];
            dMsgUnit_setTag(0, dComIfGs_getArrowMax(), buffer);
            stack_pushCurrent(buffer);
            mpReference->clearOutfontLength(mpReference->getLineCountNowPage());
            break;
        }
        }
        break;
    case MSGTAG_GROUP(4):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_GLYPH_DOLLARSIGN:
            stack_pushCurrent(changeCodeToChar(0x24));
            break;
        case MSGTAG_GLYPH_BACKSLASH:
            stack_pushCurrent(changeCodeToChar(0x5C));
            break;
        case MSGTAG_GLYPH_ATMARK:
            stack_pushCurrent(changeCodeToChar(0x40));
            break;
        case MSGTAG_GLYPH_SHARP:
            stack_pushCurrent(changeCodeToChar(0x81F2));
            break;
        case MSGTAG_GLYPH_FLAT:
            stack_pushCurrent(changeCodeToChar(0x81F3));
            break;
        case MSGTAG_GLYPH_SQRT:
            stack_pushCurrent(changeCodeToChar(0x81E3));
            break;
        case MSGTAG_GLYPH_PERCENT:
            stack_pushCurrent(changeCodeToChar(0x876C));
            break;
        case MSGTAG_GLYPH_HECTARE:
            stack_pushCurrent(changeCodeToChar(0x8766));
            break;
        case MSGTAG_GLYPH_ARE:
            stack_pushCurrent(changeCodeToChar(0x8765));
            break;
        case MSGTAG_GLYPH_LITRE:
            stack_pushCurrent(changeCodeToChar(0x8767));
            break;
        case MSGTAG_GLYPH_WATT:
            stack_pushCurrent(changeCodeToChar(0x8768));
            break;
        case MSGTAG_GLYPH_CALORIE:
            stack_pushCurrent(changeCodeToChar(0x8769));
            break;
        case MSGTAG_GLYPH_DOLLAR:
            stack_pushCurrent(changeCodeToChar(0x876A));
            break;
        case MSGTAG_GLYPH_CENT:
            stack_pushCurrent(changeCodeToChar(0x876B));
            break;
        }
        mpReference->clearOutfontLength(mpReference->getLineCountNowPage());
        break;
    case MSGTAG_GROUP(5): {
        char buffer[40];
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_TIME_INFO:
            if (*(u8*)i_data == 0) {
                dMsgUnit_setTag(4, dMeter2Info_getMsgTimeMs(), buffer);
            } else if ((*(s8*)i_data & 0xFFU) == 2) {
                dMsgUnit_setTag(4, dComIfGs_getRaceGameTime(), buffer);
            } else {
                dMsgUnit_setTag(4, dMeter2Info_getTimeMs(), buffer);
            }
            break;
        case 1:
            dMsgUnit_setTag(0, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case 2:
            dMsgUnit_setTag(1, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_FUNDRAISE_REMAIN: {
            s16 uVar9 = *(int*)i_data;
            dMsgObject_getMsgObjectClass()->setFundRaisingValue(uVar9);
            dMsgUnit_setTag(10, uVar9 - dMsgObject_getFundRaising(), buffer);
            break;
        }
        case MSGTAG_NEW_LETTER_NUM:
            dMsgUnit_setTag(0xb, dMeter2Info_getNewLetterNum(), buffer);
            break;
        case MSGTAG_POE_NUM:
            dMsgUnit_setTag(0xc, getPohNum(), buffer);
            break;
        case MSGTAG_BALLOON_SCORE: {
            int local_15c;
            if (*(u8*)i_data == 0) {
                local_15c = dComIfGp_getMessageCountNumber();
            } else {
                local_15c = dComIfGs_getBalloonScore();
            }
            dMsgUnit_setTag(0xd, local_15c, buffer);
            break;
        }
        case MSGTAG_INSECT_INFO: {
            int insectNum;
            if (*(u8*)i_data == 0) {
                insectNum = dComIfGs_checkGetInsectNum();
            } else {
                insectNum = 0x18 - dComIfGs_checkGetInsectNum();
            }
            dMsgUnit_setTag(2, insectNum, buffer);
            break;
        }
        case 4:
            dMsgUnit_setTag(3, 0, buffer);
            break;
        case 5:
            dMsgUnit_setTag(0x10000, dComIfGp_getNeedLightDropNum(), buffer);
            break;
        case 6:
            dMsgUnit_setTag(6, 0x14 - dComIfGs_getEventReg(0xff1f), buffer);
            break;
        case MSGTAG_FISH_LENGTH:
            dMsgUnit_setTag(9, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_FISH_COUNT:
            dMsgUnit_setTag(0xe, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_ROLLGOAL_LV:
            dMsgUnit_setTag(0x10001, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_RIVER_POINTS:
            dMsgUnit_setTag(8, dMeter2Info_getNowCount(), buffer);
            break;
        default:
            return false;
        }

        stack_pushCurrent(buffer);
        mpReference->clearOutfontLength(mpReference->getLineCountNowPage());
        break;
    }
    case MSGTAG_GROUP(6): {
        char buffer[40];
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_PLAYER_GENITIV:
            #if VERSION == VERSION_GCN_PAL
            if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_GERMAN) {
                setPlayerName(buffer, 1);
            } else {
                setPlayerName(buffer, 0);
            }
            #endif
            // @bug buffer is uninitialized
            stack_pushCurrent(buffer);
            break;
        case MSGTAG_HORSE_GENITIV:
            #if VERSION == VERSION_GCN_PAL
            if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_GERMAN) {
                setHorseName(buffer, 1);
            } else {
                setHorseName(buffer, 0);
            }
            #endif
            stack_pushCurrent(buffer);
            break;
        case MSGTAG_MALE_ICON:
            stack_pushCurrent(changeCodeToChar(CHAR_CODE_MALE_ICON));
            break;
        case MSGTAG_FEMALE_ICON:
            stack_pushCurrent(changeCodeToChar(CHAR_CODE_FEMALE_ICON));
            break;
        case MSGTAG_STAR_ICON:
            stack_pushCurrent(changeCodeToChar(CHAR_CODE_STAR_ICON));
            break;
        case MSGTAG_REFMARK:
            stack_pushCurrent(changeCodeToChar(CHAR_CODE_REFMARK));
            break;
        case MSGTAG_THIN_LEFT_ARROW:
            stack_pushCurrent(changeCodeToChar(CHAR_CODE_THIN_LEFT_ARROW));
            break;
        case MSGTAG_THIN_RIGHT_ARROW:
            stack_pushCurrent(changeCodeToChar(CHAR_CODE_THIN_RIGHT_ARROW));
            break;
        case MSGTAG_THIN_UP_ARROW:
            stack_pushCurrent(changeCodeToChar(CHAR_CODE_THIN_UP_ARROW));
            break;
        case MSGTAG_THIN_DOWN_ARROW:
            stack_pushCurrent(changeCodeToChar(CHAR_CODE_THIN_DOWN_ARROW));
            break;
        case MSGTAG_BULLET:
        case MSGTAG_BULLET_SPACE:
            fontsize.mSizeX += var_f30;
            mpReference->addLineLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
            mpReference->setOutfontLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
        }
        mpReference->clearOutfontLength(mpReference->getLineCountNowPage());
        break;
    case MSGTAG_GROUP(3):
        switch(i_tag & 0xFF00FFFF) {
        case MSGTAG_WII_ABTN:
        case MSGTAG_WII_BBTN:
        case MSGTAG_WII_1BTN:
        case MSGTAG_WII_2BTN:
        case MSGTAG_WII_NUNCHUK:
        case MSGTAG_WII_WIIMOTE2:
        case MSGTAG_WII_FAIRY:
        case MSGTAG_WII_CBTN:
        case MSGTAG_WII_ZBTN:
            mpReference->setCharactor(0xFFFF);
            fontsize.mSizeX += var_f30;
            mpReference->addLineLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
            mpReference->setOutfontLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
            break;
        case MSGTAG_WII_HOMEBTN:
        case MSGTAG_WII_MINUSBTN:
        case MSGTAG_WII_PLUSBTN:
            mpReference->setCharactor(0xFFFF);
            fontsize.mSizeX += var_f30;
            mpReference->addLineLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
            mpReference->setOutfontLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
            break;
        case MSGTAG_WII_DPAD_ITEM:
        case MSGTAG_WII_DPAD_UP:
        case MSGTAG_WII_DPAD_DOWN:
        case MSGTAG_WII_DPAD_HORIZONTAL:
        case MSGTAG_WII_DPAD_RIGHT:
        case MSGTAG_WII_DPAD_LEFT:
        case MSGTAG_WII_WIIMOTE:
        case MSGTAG_WII_RETICULE:
            mpReference->setCharactor(0xFFFF);
            fontsize.mSizeX += var_f30;
            mpReference->addLineLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
            mpReference->setOutfontLength(mpReference->getLineCountNowPage(), fontsize.mSizeX);
            break;
        case MSGTAG_WII_MSGID_OVERRIDE:
            break;
        }
        break;
    }
    }

    return true;
}

void jmessage_string_tMeasureProcessor::do_rubyset(void const* i_data, u32 i_size) {
    u8 len = i_size - 1;
    u8* pRuby = (u8*)i_data;
    int i = 0;
    pRuby++;

    while (i < len) {
        u8 bytes[3];
        bytes[0] = pRuby[i++];
        bytes[1] = pRuby[i++];
        bytes[2] = 0;

        int character = (((char)bytes[0] & 0xFF) << 8) | ((char)bytes[1] & 0xFF);
        mpReference->setCharactor(changeKataToHira(character));
    }
}

/* 80230A08-80230A5C 22B348 0054+00 0/0 1/1 0/0 .text
 * __ct__34jmessage_string_tSequenceProcessorFPC26jmessage_string_tReferenceP24jmessage_string_tControl
 */
jmessage_string_tSequenceProcessor::jmessage_string_tSequenceProcessor(
    jmessage_string_tReference const* pReference, jmessage_string_tControl* pControl) :
    JMessage::TSequenceProcessor(pReference, pControl),
    mMeasureProcessor(pReference)
    {}

void jmessage_string_tSequenceProcessor::do_reset() {}

void jmessage_string_tSequenceProcessor::do_begin(void const* pEntry, char const* pszText) {
    mpSeqReference = (jmessage_string_tReference*)JMessage::TSequenceProcessor::getReference();
    mMeasureProcessor.process_messageEntryText(this, pEntry, pszText);
}

void jmessage_string_tSequenceProcessor::do_end() {}

bool jmessage_string_tSequenceProcessor::do_isReady() {
    return true;
}

void jmessage_string_tSequenceProcessor::do_character(int iCharacter) {}

bool jmessage_string_tSequenceProcessor::do_tag(u32 i_tag, void const* i_data, u32 i_size) {
    switch (i_tag & 0xFF0000) {
    case MSGTAG_GROUP(0):
        switch(i_tag) {
        case MSGTAG_BOXATLEAST:
            dMeter2Info_setMsgKeyWaitTimer(*(u16*)i_data);
            break;
        }
        break;
    }
    return true;
}

bool jmessage_string_tSequenceProcessor::do_jump_isReady() {
    return true;
}

void jmessage_string_tSequenceProcessor::do_jump(void const* pEntry, char const* pszText) {}

jmessage_string_tRenderingProcessor::jmessage_string_tRenderingProcessor(
    jmessage_string_tReference const* pReference)
    : JMessage::TRenderingProcessor(pReference) {
    mpReference = (jmessage_string_tReference*)getReference();
    do_reset();
}

s16 jmessage_string_tRenderingProcessor::getLineCountNowPage() {
    s16 rv = -1;
    s32 iVar4 = mpReference->getNowPage() * mpReference->getLineMax();
    s32 uVar1 = iVar4 + mpReference->getLineMax();
    if (field_0x552 >= iVar4 && field_0x552 < uVar1) {
        rv = field_0x552 - iVar4;
    }
    return rv;
}

void jmessage_string_tRenderingProcessor::do_reset() {
    field_0x44 = 0.0f;
    field_0x3c = 0.0f;
    field_0x40 = 0.0f;
    field_0x48 = 0.0f;
    field_0x4c = 0.0f;
    field_0x50 = 0.0f;
    
    field_0x54[0] = 0;
    field_0x254[0] = 0;
    field_0x454[0] = 0;
    field_0x54e = 0;
    field_0x550 = 0;
    field_0x554 = 0;
    field_0x552 = 0;
    field_0x486[0] = 0;
}

void jmessage_string_tRenderingProcessor::do_begin(void const* pEntry, char const* pszText) {
    do_reset();
    do_widthcenter();
    do_heightcenter();
}

void jmessage_string_tRenderingProcessor::do_end() {
    if (mpReference->getPanePtr() != NULL) {
        strcpy(mpReference->getPanePtr()->getStringPtr(), field_0x54);
    }

    if (mpReference->getRubyPanePtr() != NULL) {
        strcpy(mpReference->getRubyPanePtr()->getStringPtr(), field_0x254);
    }
}

void jmessage_string_tRenderingProcessor::do_character(int iCharacter) {
    JUTFont* pFont = mpReference->getFont();
    if (JUTFont::isLeadByte_ShiftJIS(iCharacter)) {
        if (!isOutfontKanjiCode(iCharacter)) {
            u16 character2Bytes = change1ByteTo2Bytes(iCharacter);
            mpReference->addCharactor(changeKataToHira(character2Bytes));
        }
    } else {
        if (iCharacter < 0x8800 && !isOutfontKanjiCode(iCharacter)) {
            mpReference->addCharactor(changeKataToHira(iCharacter));
        }
    }

    f32 dVar12;
    J2DTextBox::TFontSize fontSize;
    if (mpReference->getPanePtr() != NULL) {
        dVar12 = ((J2DTextBox*)mpReference->getPanePtr())->getCharSpace();
        ((J2DTextBox*)mpReference->getPanePtr())->getFontSize(fontSize);
    } else {
        dVar12 = 0.0f;
        fontSize.mSizeX = fontSize.mSizeY = 20.0f;
    }

    char local_a6[3];
    local_a6[0] = local_a6[1] = local_a6[2] = 0;
    if ((iCharacter & 0xFF00) != 0) {
        local_a6[0] = ((int)(iCharacter & 0xFF00) >> 8);
        local_a6[1] = iCharacter & 0xFF;
    } else {
        local_a6[0] = iCharacter & 0xFF;
        local_a6[1] = 0;
    }

    if (iCharacter == '\n') {
        do_strcat(local_a6);
        do_rubystrcat(local_a6);
        field_0x552++;
        field_0x48 = 0.0f;
        field_0x4c = 0.0f;
        if (mpReference->getPanePtr() != NULL) {
            field_0x50 += ((J2DTextBox*)mpReference->getPanePtr())->getLineSpace();
        }
        do_widthcenter();
    } else {
        f32 fVar2;
        f32 fVar1 = dVar12 + fontSize.mSizeX * (pFont->getWidth(iCharacter) /  (f32)pFont->getCellWidth());
        field_0x4c += fVar1;
        if (field_0x554 != 0) {
            field_0x40 += fVar1;
            field_0x554--;
            if (field_0x554 == 0) {
                field_0x40 -= dVar12;
                fVar2 = (field_0x3c + 0.5f * (field_0x40 - field_0x44));
                f32 fVar3 = fVar2 - field_0x48;
                char acStack_98[16];
                if (fVar3 >= 1.0f) {
                    snprintf(acStack_98, sizeof(acStack_98) - 1, "\x1B" "CR[%d]", (int)fVar3);
                    do_rubystrcat(acStack_98);
                } else {
                    if (fVar3 <= -1.0f) {
                        snprintf(acStack_98, sizeof(acStack_98) - 1, "\x1B" "CL[%d]", (int)-fVar3);
                        do_rubystrcat(acStack_98);
                    } else {
                        fVar3 = 0.0f;
                    }
                }

                if (mpReference->getRubyPanePtr() != NULL) {
                    field_0x48 += ((J2DTextBox*)mpReference->getRubyPanePtr())->getCharSpace() + ((int)fVar3 + field_0x44);
                } else {
                    field_0x48 += (int)fVar3 + field_0x44;
                }
                do_rubystrcat(field_0x454);
            }
        }
        do_strcat(local_a6);
    }
}

bool jmessage_string_tRenderingProcessor::do_tag(u32 i_tag, void const* i_data, u32 i_size) {
    jmessage_tReference* pReference = (jmessage_tReference*)getReference();
    switch(i_tag & 0xFF0000) {
    case MSGTAG_GROUP(255):
        switch(i_tag) {
        case MSGTAG_COLOR:
            do_color(*(u8*)i_data & 0xFF);
            break;
        case MSGTAG_SCALE:
            do_scale(*(u16*)i_data / 100.0f);
            break;
        case MSGTAG_RUBY:
        case 0xFFFF02:
            do_rubyset(i_data, i_size);
            break;
        }
        break;
    case MSGTAG_GROUP(0):
        switch (i_tag) {
        case MSGTAG_PLAYER_NAME:
            push_word(dComIfGs_getPlayerName());
            break;
        case MSGTAG_HORSE_NAME:
            push_word(dComIfGs_getHorseName());
            break;
        case MSGTAG_UNK_53:
            push_word(dMsgObject_getWord());
            break;
        case MSGTAG_SCENT_NAME:
            push_word(dMsgObject_getMsgObjectClass()->getSmellName());
            break;
        case MSGTAG_PORTAL_NAME:
            push_word(dMsgObject_getMsgObjectClass()->getPortalName());
            break;
        case MSGTAG_BOMB_NAME:
            push_word(dMsgObject_getMsgObjectClass()->getBombName());
            break;
        case MSGTAG_INSECT_NAME:
            push_word(dMsgObject_getMsgObjectClass()->getInsectName());
            break;
        case MSGTAG_LETTER_NAME:
            push_word(dMsgObject_getMsgObjectClass()->getLetterName());
            break;
        case MSGTAG_LINE_DOWN:
            do_linedown(*(u16*)i_data);
            break;
        case MSGTAG_CURRENT_LETTER_PAGE: {
            char buffer[4];
            sprintf(buffer, "%d", dComIfGp_getMessageCountNumber() / 100);
            push_word(buffer);
            break;
        }
        case MSGTAG_MAX_LETTER_PAGE: {
            char buffer[4];
            sprintf(buffer, "%d", dComIfGp_getMessageCountNumber() % 100);
            push_word(buffer);
            break;
        }
        case MSGTAG_ABTN:
            do_outfont(0);
            break;
        case MSGTAG_BBTN:
            do_outfont(1);
            break;
        case MSGTAG_CSTICK:
            do_outfont(2);
            break;
        case MSGTAG_LBTN:
            do_outfont(3);
            break;
        case MSGTAG_RBTN:
            do_outfont(4);
            break;
        case MSGTAG_XBTN:
            do_outfont(5);
            break;
        case MSGTAG_YBTN:
            do_outfont(6);
            break;
        case MSGTAG_ZBTN:
            do_outfont(7);
            break;
        case MSGTAG_DPAD:
            do_outfont(8);
            break;
        case MSGTAG_STICK_CROSS:
            do_outfont(9);
            break;
        case MSGTAG_LEFT_ARROW:
            do_outfont(10);
            break;
        case MSGTAG_RIGHT_ARROW:
            do_outfont(11);
            break;
        case MSGTAG_UP_ARROW:
            do_outfont(12);
            break;
        case MSGTAG_DOWN_ARROW:
            do_outfont(13);
            break;
        case MSGTAG_STICK_UP:
            do_outfont(14);
            break;
        case MSGTAG_STICK_DOWN:
            do_outfont(15);
            break;
        case MSGTAG_STICK_LEFT:
            do_outfont(16);
            break;
        case MSGTAG_STICK_RIGHT:
            do_outfont(17);
            break;
        case MSGTAG_STICK_VERTICAL:
            do_outfont(18);
            break;
        case MSGTAG_STICK_HORIZONTAL:
            do_outfont(19);
            break;
        case MSGTAG_RED_TARGET:
            do_outfont(20);
            break;
        case MSGTAG_YELLOW_TARGET:
            do_outfont(21);
            break;
        case MSGTAG_INPUT_VALUE:
            do_outfont(30);
            do_numset(dMsgObject_getMsgObjectClass()->getInputValue());
            break;
        case MSGTAG_ABTN_STAR:
            do_outfont(23);
            break;
        case MSGTAG_WHITE_TARGET:
            do_outfont(24);
            break;
        case MSGTAG_WARP_ICON:
            do_outfont(25);
            break;
        case MSGTAG_XYBTN:
            if (dMsgObject_getItemEquipButton() == 0) {
                do_outfont(5);
            } else {
                do_outfont(6);
            }
            break;
        case MSGTAG_YXBTN:
            if (dMsgObject_getItemEquipButton() == 0) {
                do_outfont(6);
            } else {
                do_outfont(5);
            }
            break;
        case MSGTAG_BOMB_BAG_ICON:
            do_outfont(41);
            break;
        case MSGTAG_HEART:
            do_outfont(27);
            break;
        case MSGTAG_QUAVER:
            do_outfont(28);
            break;
        case MSGTAG_BOMB_NUM: {
            char buffer[40];
            u8 num = dMsgObject_getMsgObjectClass()->getSelectBombNum();
            dMsgUnit_setTag(7, num, buffer);
            push_word(buffer);
            break;
        }
        case MSGTAG_BOMB_PRICE: {
            char buffer[40];
            s16 price = dMsgObject_getMsgObjectClass()->getSelectBombPrice();
            dMsgUnit_setTag(1, price, buffer);
            push_word(buffer);
            break;
        }
        case MSGTAG_BOMB_MAX: {
            u8 bombType = fpcNm_ITEM_NORMAL_BOMB;
            if ((*(u8*)i_data) == 1) {
                bombType = fpcNm_ITEM_WATER_BOMB;
            } else if ((*(s8*)i_data & 0xFFU) == 2) {
                bombType = fpcNm_ITEM_POKE_BOMB;
            }

            char buffer[40];
            dMsgUnit_setTag(7, dComIfGs_getBombMax(bombType), buffer);
            push_word(buffer);
            break;
        }
        case MSGTAG_ARROW_MAX: {
            char buffer[40];
            dMsgUnit_setTag(0, dComIfGs_getArrowMax(), buffer);
            push_word(buffer);
            break;
        }
        }
        break;
    case MSGTAG_GROUP(4):
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_GLYPH_DOLLARSIGN:
            push_word(changeCodeToChar(0x24));
            break;
        case MSGTAG_GLYPH_BACKSLASH:
            push_word(changeCodeToChar(0x5C));
            break;
        case MSGTAG_GLYPH_ATMARK:
            push_word(changeCodeToChar(0x40));
            break;
        case MSGTAG_GLYPH_SHARP:
            push_word(changeCodeToChar(0x81F2));
            break;
        case MSGTAG_GLYPH_FLAT:
            push_word(changeCodeToChar(0x81F3));
            break;
        case MSGTAG_GLYPH_SQRT:
            push_word(changeCodeToChar(0x81E3));
            break;
        case MSGTAG_GLYPH_PERCENT:
            push_word(changeCodeToChar(0x876C));
            break;
        case MSGTAG_GLYPH_HECTARE:
            push_word(changeCodeToChar(0x8766));
            break;
        case MSGTAG_GLYPH_ARE:
            push_word(changeCodeToChar(0x8765));
            break;
        case MSGTAG_GLYPH_LITRE:
            push_word(changeCodeToChar(0x8767));
            break;
        case MSGTAG_GLYPH_WATT:
            push_word(changeCodeToChar(0x8768));
            break;
        case MSGTAG_GLYPH_CALORIE:
            push_word(changeCodeToChar(0x8769));
            break;
        case MSGTAG_GLYPH_DOLLAR:
            push_word(changeCodeToChar(0x876A));
            break;
        case MSGTAG_GLYPH_CENT:
            push_word(changeCodeToChar(0x876B));
            break;
        }
        break;
    case MSGTAG_GROUP(5): {
        char buffer[40];
        switch (i_tag & 0xFF00FFFF) { 
        case MSGTAG_TIME_INFO:
            if (*(u8*)i_data == 0) {
                dMsgUnit_setTag(4, dMeter2Info_getMsgTimeMs(), buffer);
            } else if ((*(s8*)i_data & 0xFFU) == 2) {
                dMsgUnit_setTag(4, dComIfGs_getRaceGameTime(), buffer);
            } else {
                dMsgUnit_setTag(4, dMeter2Info_getTimeMs(), buffer);
            }
            break;
        case 1:
            dMsgUnit_setTag(0, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case 2:
            dMsgUnit_setTag(1, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_FUNDRAISE_REMAIN: {
            s16 uVar9 = *(int*)i_data;
            dMsgObject_getMsgObjectClass()->setFundRaisingValue(uVar9);
            dMsgUnit_setTag(10, uVar9 - dMsgObject_getFundRaising(), buffer);
            break;
        }
        case MSGTAG_NEW_LETTER_NUM:
            dMsgUnit_setTag(0xb, dMeter2Info_getNewLetterNum(), buffer);
            break;
        case MSGTAG_POE_NUM:
            dMsgUnit_setTag(0xc, getPohNum(), buffer);
            break;
        case MSGTAG_BALLOON_SCORE:{
            int local_15c;
            if (*(u8*)i_data == 0) {
                local_15c = dComIfGp_getMessageCountNumber();
            } else {
                local_15c = dComIfGs_getBalloonScore();
            }
            dMsgUnit_setTag(0xd, local_15c, buffer);
            break;
        }
        case MSGTAG_INSECT_INFO: {
            int insectNum;
            if (*(u8*)i_data == 0) {
                insectNum = dComIfGs_checkGetInsectNum();
            } else {
                insectNum = 24 - dComIfGs_checkGetInsectNum();
            }
            dMsgUnit_setTag(2, insectNum, buffer);
            break;
        }
        case 4:
            dMsgUnit_setTag(3, 0, buffer);
            break;
        case 5:
            dMsgUnit_setTag(0x10000, dComIfGp_getNeedLightDropNum(), buffer);
            break;
        case 6:
            dMsgUnit_setTag(6, 20 - dComIfGs_getEventReg(0xFF1F), buffer);
            break;
        case MSGTAG_FISH_LENGTH:
            dMsgUnit_setTag(9, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_FISH_COUNT:
            dMsgUnit_setTag(0xe, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_ROLLGOAL_LV:
            dMsgUnit_setTag(0x10001, (s32)dComIfGp_getMessageCountNumber(), buffer);
            break;
        case MSGTAG_RIVER_POINTS:
            dMsgUnit_setTag(8, dMeter2Info_getNowCount(), buffer);
            break;
        default:
            return 0;
        }
        push_word(buffer);
        break;
    }
    case MSGTAG_GROUP(6): {
        char buffer[40];
        switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_PLAYER_GENITIV:
            #if VERSION == VERSION_GCN_PAL
            if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_GERMAN) {
                setPlayerName(buffer, 1);
            } else {
                setPlayerName(buffer, 0);
            }
            #endif
            // @bug buffer is uninitialized
            push_word(buffer);
            break;
        case MSGTAG_HORSE_GENITIV:
            #if VERSION == VERSION_GCN_PAL
            if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_GERMAN) {
                setHorseName(buffer, 1);
            } else {
                setHorseName(buffer, 0);
            }
            #endif
            push_word(buffer);
            break;
        case MSGTAG_MALE_ICON:
            push_word(changeCodeToChar(CHAR_CODE_MALE_ICON));
            break;
        case MSGTAG_FEMALE_ICON:
            push_word(changeCodeToChar(CHAR_CODE_FEMALE_ICON));
            break;
        case MSGTAG_STAR_ICON:
            push_word(changeCodeToChar(CHAR_CODE_STAR_ICON));
            break;
        case MSGTAG_REFMARK:
            push_word(changeCodeToChar(CHAR_CODE_REFMARK));
            break;
        case MSGTAG_THIN_LEFT_ARROW:
            push_word(changeCodeToChar(CHAR_CODE_THIN_LEFT_ARROW));
            break;
        case MSGTAG_THIN_RIGHT_ARROW:
            push_word(changeCodeToChar(CHAR_CODE_THIN_RIGHT_ARROW));
            break;
        case MSGTAG_THIN_UP_ARROW:
            push_word(changeCodeToChar(CHAR_CODE_THIN_UP_ARROW));
            break;
        case MSGTAG_THIN_DOWN_ARROW:
            push_word(changeCodeToChar(CHAR_CODE_THIN_DOWN_ARROW));
            break;
        case MSGTAG_BULLET:
            do_outfont(42);
            break;
        case MSGTAG_BULLET_SPACE:
            do_outfont(43);
        }
        break;
    }
    case MSGTAG_GROUP(3):
    switch (i_tag & 0xFF00FFFF) {
        case MSGTAG_WII_ABTN:
            do_outfont(0);
            break;
        case MSGTAG_WII_BBTN:
            do_outfont(51);
            break;
        case MSGTAG_WII_HOMEBTN:
            do_outfont(52);
            break;
        case MSGTAG_WII_MINUSBTN:
            do_outfont(53);
            break;
        case MSGTAG_WII_PLUSBTN:
            do_outfont(54);
            break;
        case MSGTAG_WII_1BTN:
            do_outfont(63);
            break;
        case MSGTAG_WII_2BTN:
            do_outfont(64);
            break;
        case MSGTAG_WII_DPAD_ITEM:
            do_outfont(50);
            break;
        case MSGTAG_WII_DPAD_UP:
            do_outfont(49);
            break;
        case MSGTAG_WII_DPAD_DOWN:
            do_outfont(65);
            break;
        case MSGTAG_WII_DPAD_HORIZONTAL:
            do_outfont(66);
            break;
        case MSGTAG_WII_DPAD_RIGHT:
            do_outfont(67);
            break;
        case MSGTAG_WII_DPAD_LEFT:
            do_outfont(68);
            break;
        case MSGTAG_WII_WIIMOTE:
            if (dComIfGs_getOptPointer() == 0) {
                do_outfont(9);
            } else {
                do_outfont(44);
            }
            break;
        case MSGTAG_WII_RETICULE:
            do_outfont(69);
            break;
        case MSGTAG_WII_NUNCHUK:
            do_outfont(45);
            break;
        case MSGTAG_WII_WIIMOTE2:
            do_outfont(44);
            break;
        case MSGTAG_WII_FAIRY:
            do_outfont(62);
            break;
        case MSGTAG_WII_CBTN:
            do_outfont(2);
            break;
        case MSGTAG_WII_ZBTN:
            do_outfont(7);
            break;
        }
        break;
    }
 
    return 1;
}

void jmessage_string_tRenderingProcessor::do_widthcenter() {
    J2DPane* pane;
    f32 scale;
    f32 width;
    f32 lineLength;
    f32 dVar9;
    char buffer[16];
    int outFontLen;

    if (getLineCountNowPage() >= 0 && mpReference->getPanePtr() != NULL) {
        if (mpReference->isFlag(1) == 0) {
            if (mpReference->getPanePtr()->getHBinding() != HBIND_CENTER) {
                return;
            }
        }

        pane = mpReference->getPanePtr();
        scale = 1.0f;
        scale = mDoGph_gInf_c::getScale();

        #if VERSION == VERSION_SHIELD_DEBUG 
        for (; pane != NULL; pane = pane->getParentPane()) {
            if (pane->getUserInfo() == 'n_43') {
                scale = 1.0f;
                break;
            }
        }
        #endif

        width = scale * mpReference->getPanePtr()->getWidth();
        lineLength = mpReference->getLineLength(getLineCountNowPage());
        dVar9 = (0.5f + (width - lineLength) / 2);

        if (mpReference->getPanePtr()->getHBinding() == HBIND_CENTER) {
            snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CL[%d]", (int)mpReference->getOutfontLength(getLineCountNowPage()));
            do_strcat(buffer);
        }

        if (dVar9 >= 1.0f) {
            field_0x4c += (int)dVar9;
            if (mpReference->getPanePtr()->getHBinding() != HBIND_CENTER) {
                snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CR[%d]", (int)dVar9);
                do_strcat(buffer);
            }
        }
    }
}

void jmessage_string_tRenderingProcessor::do_heightcenter() {
    f32 height;
    J2DTextBox::TFontSize fontSize;
    f32 lineSpace;
    f32 dVar11;
    f32 dVar9;
    char buffer[16];

    if (mpReference->getLineCountNowPage() >= 0 && mpReference->getPanePtr() != NULL) {
        if (mpReference->isFlag(2) == 0) {
            if (mpReference->getPanePtr()->getHBinding() != HBIND_CENTER)
                return;
        }

        height = mpReference->getPanePtr()->getHeight();
        mpReference->getPanePtr()->getFontSize(fontSize);
        lineSpace = mpReference->getPanePtr()->getLineSpace();
        dVar11 = ((lineSpace * mpReference->getLineCountNowPage()) - (lineSpace - fontSize.mSizeY));
        dVar9 = 0.5f + (height - dVar11) / 2;

        if (dVar9 >= 1.0f) {
            field_0x50 += dVar9;
            if (mpReference->getPanePtr()->getHBinding() != HBIND_CENTER) {
                snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CD[%d]", (int)dVar9);
                do_strcat(buffer);
                do_rubystrcat(buffer);
            }
        }
    }
}

void jmessage_string_tRenderingProcessor::do_strcat(char* i_str) {
    if (getLineCountNowPage() >= 0) {
        field_0x54e += strlen(i_str);
        if (field_0x54e < ARRAY_SIZE(field_0x54)) {
            strcat(field_0x54, i_str);
        } else {
            JUT_WARN(7531, "%s", "Message Alloc Byte Over!!");
        }
    }
}

void jmessage_string_tRenderingProcessor::do_rubyset(void const* i_data, u32 i_size) {
    if (getLineCountNowPage() >= 0) {
        if (mpReference->getRubyPanePtr() != NULL) {
            JUTFont* pFont = mpReference->getFont();
            f32 charSpace = mpReference->getRubyPanePtr()->getCharSpace();
            J2DTextBox::TFontSize fontSize;
            mpReference->getRubyPanePtr()->getFontSize(fontSize);

            u8 m_rubycount = i_size - 1;
            u8* pRuby = (u8*)i_data;

            int i = 0;
            JUT_ASSERT(7565, m_rubycount < (50));
            field_0x554 = *(u8*)i_data & 0xFF;
            pRuby++;

            field_0x454[0] = 0;
            field_0x44 = 0.0f;

            while (i < (int)m_rubycount) {
                u8 bytes[3];
                bytes[0] = pRuby[i++];
                bytes[1] = pRuby[i++];
                bytes[2] = 0;
                strcat(field_0x454, (const char*)bytes);

                int character = (((char)bytes[0] & 0xFF) << 8) | ((char)bytes[1] & 0xFF);
                field_0x44 += charSpace + fontSize.mSizeX * ((f32)pFont->getWidth(character) / pFont->getCellWidth());
                mpReference->addCharactor(changeKataToHira(character));
            }

            field_0x44 -= charSpace;
            field_0x3c = field_0x4c;
            field_0x40 = 0.0f;
        }
    }
}

void jmessage_string_tRenderingProcessor::do_rubystrcat(char* i_str) {
    if (getLineCountNowPage() >= 0) {
        field_0x550 += strlen(i_str);
        if (field_0x550 < ARRAY_SIZE(field_0x254)) {
            strcat(field_0x254, i_str);
        } else {
            JUT_WARN(7613, "%s", "Message Alloc Byte Over!!");
        }
    }
}

void jmessage_string_tRenderingProcessor::do_outfont(u8 i_iconNo) {
    if (mpReference->getPanePtr() != NULL) {
        f32 charSpace = mpReference->getPanePtr()->getCharSpace();
        f32 dVar13 = 0.0f;
        int r28 = 1;

        J2DTextBox::TFontSize fontSize;
        mpReference->getPanePtr()->getFontSize(fontSize);

        if (mpReference->getOutFontPtr() != NULL) {
            J2DPane* pane = mpReference->getPanePtr();
            f32 paneBounds0 = pane->getBounds().i.x;
            f32 paneBounds1 = pane->getBounds().i.y;
            f32 scales[2];
            scales[0] = mpReference->getPanePtr()->getScaleX();
            scales[1] = mpReference->getPanePtr()->getScaleX();

            for (pane = pane->getParentPane(); pane != NULL; pane = pane->getParentPane()) {
                paneBounds0 += pane->getBounds().i.x;
                paneBounds1 += pane->getBounds().i.y;
            }

            f32 stack_2c = fontSize.mSizeX;
            f32 stack_28 = fontSize.mSizeY;
            f32 posX = 0.0f;
            f32 posY = 0.0f;
            if (mpReference->isFlag(4)) {
                f32 width = mpReference->getPanePtr()->getWidth();
                f32 lineLen = mpReference->getLineLength(getLineCountNowPage());
                posX = 0.5f + (width - lineLen) / 2;
            }

            if (mpReference->isFlag(8)) {
                f32 height = mpReference->getPanePtr()->getHeight();
                J2DTextBox::TFontSize fontSize;
                mpReference->getPanePtr()->getFontSize(fontSize);

                f32 lineSpace = mpReference->getPanePtr()->getLineSpace();
                f32 stack_c = (lineSpace * mpReference->getLineCountNowPage() - (lineSpace - fontSize.mSizeY));
                posY = 0.5f + (height - stack_c) / 2;
            }

            mpReference->getOutFontPtr()->drawFont(mpReference->getPanePtr(), i_iconNo, field_0x4c + posX, field_0x50 + posY,
                                                   fontSize.mSizeX, fontSize.mSizeY, 0xFFFFFFFF, 0xFF);
            dVar13 = (0.5f + (charSpace + fontSize.mSizeX * r28));
        }

        field_0x4c += (int) dVar13;
        char buffer[16];
        snprintf(buffer, sizeof(buffer) - 1, "\x1B" "CR[%d]", (int)dVar13);
        do_strcat(buffer);
        mpReference->addCharactor(0xFFFF);
    }
}

void jmessage_string_tRenderingProcessor::do_color(u8 i_colorNo) {
    u32 ccColor;
    u32 gcColor;
    if (i_colorNo == 0) {
        ccColor = ((jmessage_string_tReference*)getReference())->getCCColor();
        gcColor = ((jmessage_string_tReference*)getReference())->getGCColor();
    } else {
        ccColor = getFontCCColorTable(i_colorNo, 0);
        gcColor = getFontGCColorTable(i_colorNo, 0);
    }

    char buffer[32];
    sprintf(buffer, "\x1b" "CC[%08x]" "\x1b" "GC[%08x]", ccColor, gcColor);
    do_strcat(buffer);
}

void jmessage_string_tRenderingProcessor::do_scale(f32 i_scale) {
    J2DTextBox::TFontSize fontSize;
    mpReference->getPanePtr()->getFontSize(fontSize);
    s16 scaleX = 0.5f + fontSize.mSizeX * i_scale;
    s16 scaleY = 0.5f + fontSize.mSizeY * i_scale;

    char buffer[32];
    sprintf(buffer, "\x1b" "FX[%d]" "\x1b" "FY[%d]", scaleX, scaleY);
    do_strcat(buffer);
}

void jmessage_string_tRenderingProcessor::do_linedown(s16 i_lineNo) {
    char buffer[16];
    sprintf(buffer, "\x1B" "CD[%d]", i_lineNo);
    do_strcat(buffer);
}

void jmessage_string_tRenderingProcessor::do_numset(s16 i_num) {
    if (9999 < i_num) {
        i_num = 9999;
    }
    if (i_num < 0) {
        i_num = 0;
    }

    do_outfont(getOutFontNumberType(i_num / 1000));
    do_outfont(getOutFontNumberType(i_num % 1000 / 100));
    do_outfont(getOutFontNumberType(i_num % 100 / 10));
    do_outfont(getOutFontNumberType(i_num % 10));
}

void jmessage_string_tRenderingProcessor::push_word(char const* i_word) {
    strcpy(field_0x486, i_word);
    stack_pushCurrent(field_0x486);
}
