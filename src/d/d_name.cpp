#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_name.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "m_Do/m_Do_audio.h"
#include "m_Do/m_Do_controller_pad.h"
#include "stdio.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "f_op/f_op_msg_mng.h"

static const char* l_mojiHira[65] = {
    "あ", "い", "う", "え", "お", "か", "き", "く", "け", "こ", "さ", "し", "す",
    "せ", "そ", "た", "ち", "つ", "て", "と", "な", "に", "ぬ", "ね", "の", "は",
    "ひ", "ふ", "へ", "ほ", "ま", "み", "む", "め", "も", "や", "　", "ゆ", "　",
    "よ", "ら", "り", "る", "れ", "ろ", "わ", "　", "を", "　", "ん", "ぁ", "ぃ",
    "ぅ", "ぇ", "ぉ", "ゃ", "　", "ゅ", "　", "ょ", "っ", "　", "ー", "゛", "゜",
};

static const char* l_mojiHira2[65] = {
    "￥", "￥", "￥", "￥", "￥", "が", "ぎ", "ぐ", "げ", "ご", "ざ", "じ", "ず",
    "ぜ", "ぞ", "だ", "ぢ", "づ", "で", "ど", "￥", "￥", "￥", "￥", "￥", "ば",
    "び", "ぶ", "べ", "ぼ", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
};

static const char* l_mojiHira3[65] = {
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "ぱ",
    "ぴ", "ぷ", "ぺ", "ぽ", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
};

static const char* l_mojikata[65] = {
    "ア", "イ", "ウ", "エ", "オ", "カ", "キ", "ク", "ケ", "コ", "サ", "シ", "ス",
    "セ", "ソ", "タ", "チ", "ツ", "テ", "ト", "ナ", "ニ", "ヌ", "ネ", "ノ", "ハ",
    "ヒ", "フ", "ヘ", "ホ", "マ", "ミ", "ム", "メ", "モ", "ヤ", "　", "ユ", "　",
    "ヨ", "ラ", "リ", "ル", "レ", "ロ", "ワ", "　", "ヲ", "　", "ン", "ァ", "ィ",
    "ゥ", "ェ", "ォ", "ャ", "　", "ュ", "　", "ョ", "ッ", "　", "ー", "゛", "゜",
};

static const char* l_mojikata2[65] = {
    "￥", "￥", "ヴ", "￥", "￥", "ガ", "ギ", "グ", "ゲ", "ゴ", "ザ", "ジ", "ズ",
    "ゼ", "ゾ", "ダ", "ヂ", "ヅ", "デ", "ド", "￥", "￥", "￥", "￥", "￥", "バ",
    "ビ", "ブ", "ベ", "ボ", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
};

static const char* l_mojikata3[65] = {
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "パ",
    "ピ", "プ", "ペ", "ポ", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
};

static const char* l_mojiEisu[65] = {
    "A", "N", "a", "n", "1", "B", "O", "b", "o", "2", "C", "P", "c", "p", "3", "D", "Q",
    "d", "q", "4", "E", "R", "e", "r", "5", "F", "S", "f", "s", "6", "G", "T", "g", "t",
    "7", "H", "U", "h", "u", "8", "I", "V", "i", "v", "9", "J", "W", "j", "w", "0", "K",
    "X", "k", "x", ",", "L", "Y", "l", "y", ".", "M", "Z", "m", "z", " ",
};

#if REGION_PAL
static char* l_mojiEisuPal_1[65] = {
    "A", "N", "AA", "BB", "1", "B", "O", "CC", "DD", "2", "C", "P", "EE", "FF", "3", "D", "Q",
    "GG", "HH", "4", "E", "R", "II", "JJ", "5", "F", "S", "KK", "LL", "6", "G", "T", "MM", "NN",
    "7", "H", "U", "OO", "PP", "8", "I", "V", "QQ", "RR", "9", "J", "W", "SS", "TT", "0", "K",
    "X", "UU", "VV", ",", "L", "Y", "WW", "XX", ".", "M", "Z", "YY", "ZZ", " ",
};

static char* l_mojiEisuPal_2[65] = {
    "a", "n", "aa", "bb", "1", "b", "o", "cc", "dd", "2", "c", "p", "ee", "ff", "3", "d", "q",
    "gg", "hh", "4", "e", "r", "ii", "jj", "5", "f", "s", "kk", "ll", "6", "g", "t", "mm",
    "nn", "7", "h", "u", "oo", "pp", "8", "i", "v", "qq", "rr", "9", "j", "w", "ss", "tt", "0",
    "k", "x", "uu", "vv", ",", "l", "y", "ww", "xx", ".", "m", "z", "yy", "zz", " ",
};
#endif

static dNm_HIO_c g_nmHIO;

typedef void (dName_c::*selProcFunc)(void);
static selProcFunc SelProc[9] = {
    &dName_c::MojiSelect,     &dName_c::MojiSelectAnm,  &dName_c::MojiSelectAnm2,
    &dName_c::MojiSelectAnm3, &dName_c::MenuSelect,     &dName_c::MenuSelectAnm,
    &dName_c::MenuSelectAnm2, &dName_c::MenuSelectAnm3, &dName_c::Wait
};

dNm_HIO_c::dNm_HIO_c() {
    mMenuScale = 1.3f;
    mSelCharScale = 1.4f;
    field_0x10 = 10;
}

dName_c::dName_c(J2DPane* pane) {
    nameIn.field_0xc = pane;
    _create();
    init();
}

dName_c::~dName_c() {
    delete stick;
    delete nameIn.NameInScr;
    mDoExt_removeMesgFont();

    for (int i = 0; i < 8; i++) {
        delete mNameCursor[i];
    }

    for (int i = 0; i < 65; i++) {
        delete mMojiIcon[i];
    }

    for (int i = 0; i < 4; i++) {
        if (mMenuIcon[i] != NULL) {
            delete mMenuIcon[i];
        }
    }

    delete mCursorColorKey;
    delete mCursorTexKey;
    delete mSelIcon;
    archive->removeResourceAll();
}

void dName_c::_create() {
    stick = new STControl(5, 2, 2, 1, 0.9f, 0.5f, 0, 0x800);
    stick->setFirstWaitTime(5);
    nameIn.font = mDoExt_getMesgFont();
    g_nmHIO.field_0x4 = -1;
    screenSet();

    mNextNameStr[0] = 0;
    mCursorDelay = 1;
    displayInit();
}

void dName_c::init() {
    mCurPos = 0;
    mLastCurPos = 0;
    nameCursorMove();
    mLastCurPos = mCurPos;

    for (int i = 0; i < 4; i++) {
        field_0x30c[i][2] = 0;
    }

    mCharColumn = 0;
    mCharRow = 0;
    mPrevColumn = 0;
    mPrevRow = 0;
    field_0x30c[0][0] = 0;
    field_0x30c[0][1] = 0;
    field_0x30c[0][2] = 1;

    selectCursorMove();
    mSelProc = PROC_MOJI_SELECT;
    field_0x2ac = mSelProc;
    field_0x2ad = mSelProc;
    field_0x2ae = field_0x2ac;
    #if REGION_PAL || REGION_JPN
    mMojiSet = MOJI_HIRA;
    #else
    mMojiSet = MOJI_EIGO;
    #endif
    mPrevMojiSet = 255;
    #if REGION_PAL || REGION_JPN
    mSelMenu = MENU_HIRA;
    mPrevSelMenu = MENU_HIRA;
    #else
    mSelMenu = MENU_END;
    mPrevSelMenu = MENU_END;
    #endif
    mojiListChange();
}

void dName_c::initial() {
    displayInit();

    if (mNextNameStr[0] != 0) {
        NameStrSet();
        mNextNameStr[0] = 0;
    }

    #if REGION_PAL || REGION_JPN
    if (mSelProc == PROC_MOJI_SELECT) {
        mMenuIcon[mMojiSet]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
        mMenuText[mMojiSet]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
        if (mPrevMojiSet != 255) {
            mMenuIcon[mPrevMojiSet]->scale(1.0f, 1.0f);
            mMenuText[mPrevMojiSet]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
        }
    }
    #endif
}

void dName_c::showIcon() {
    Vec pos;

    switch (mSelProc) {
    case PROC_MOJI_SELECT:
        if (mCharColumn != 255 && mCharRow != 255) {
            pos = mMojiIcon[mCharRow + mCharColumn * 5]->getGlobalVtxCenter(false, 0);
            mSelIcon->setPos(pos.x, pos.y, mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr(),
                             true);
            ((J2DTextBox*)mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr())
                ->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
            mSelIcon->setAlphaRate(1.0f);
        }
        break;
    case PROC_MENU_SELECT:
        if (mSelMenu != 255) {
            int menu_i = getMenuPosIdx(mSelMenu);

            pos = mMenuIcon[menu_i]->getGlobalVtxCenter(false, 0);
            mSelIcon->setPos(pos.x, pos.y, mMenuIcon[menu_i]->getPanePtr(), true);
            mMenuText[menu_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
            mSelIcon->setAlphaRate(1.0f);
        }
        break;
    }
}

void dName_c::_move() {
    stick->checkTrigger();
    (this->*SelProc[mSelProc])();

    #if REGION_PAL || REGION_JPN
    if (mDoCPd_c::getTrigY(PAD_1)) {
        mDoAud_seStart(Z2SE_SY_DUMMY, 0, 0, 0);
        mPrevMojiSet = mMojiSet;
        mMojiSet++;
        #if REGION_JPN
        if (mMojiSet > MOJI_EIGO) {
        #else
        if (mMojiSet > MOJI_KATA) {
        #endif
            mMojiSet = MOJI_HIRA;
        }
        mojiListChange();
    } else {
    #endif
    #if REGION_JPN
    if (mDoCPd_c::getTrigX(PAD_1)) {
        if (mCurPos != 0) {
            if (mojiChange(mCurPos - 1) == 1) {
                mDoAud_seStart(Z2SE_SY_DUMMY, 0, 0, 0);
            } else {
                mDoAud_seStart(Z2SE_SYS_ERROR, 0, 0, 0);
            }
        }
    } else {
    #endif
    if (mDoCPd_c::getTrigRight(PAD_1)) {
        // BUG: this check only fails if the cursor is at exactly 7
        // setMoji allows the cursor to reach 8, which is out of bounds here
        if (mCurPos != 7) {
            mDoAud_seStart(Z2SE_SY_DUMMY, 0, 0, 0);
            mLastCurPos = mCurPos;
            mCurPos++;
            nameCursorMove();
        }
    } else if (mDoCPd_c::getTrigLeft(PAD_1)) {
        if (mCurPos != 0) {
            mDoAud_seStart(Z2SE_SY_DUMMY, 0, 0, 0);
            mLastCurPos = mCurPos;
            mCurPos--;
            nameCursorMove();
        }
    } else if (mDoCPd_c::getTrigB(PAD_1)) {
        if (mCurPos == 0) {
            mDoAud_seStart(Z2SE_SY_MENU_BACK, 0, 0, 0);
            field_0x2ac = mSelProc;
            field_0x2ae = mSelProc;
            mSelProc = PROC_WAIT;
            mIsInputEnd = true;
        } else {
            backSpace();
        }
    } else if (mDoCPd_c::getTrigStart(PAD_1)) {
        #if REGION_PAL
        if ((mSelProc != PROC_MENU_SELECT || mSelMenu != MENU_EIGO) &&
            (mSelProc == PROC_MENU_SELECT || mSelProc == PROC_MOJI_SELECT))
        {
        #else
        if ((mSelProc != PROC_MENU_SELECT || mSelMenu != MENU_END) &&
            (mSelProc == PROC_MENU_SELECT || mSelProc == PROC_MOJI_SELECT))
        {
        #endif
            mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, 0, 0, 0);
            mPrevSelMenu = mSelMenu;
            #if REGION_PAL
            mSelMenu = MENU_EIGO;
            #else
            mSelMenu = MENU_END;
            #endif

            switch (mSelProc) {
            case PROC_MOJI_SELECT:
                mPrevColumn = mCharColumn;
                mPrevRow = mCharRow;
                MojiSelectAnmInit();
                mSelProc = PROC_MOJI_SEL_ANM2;
                break;
            case PROC_MENU_SELECT:
                MenuSelectAnmInit();
                mSelProc = PROC_MENU_SEL_ANM;
                break;
            }
        }
    }
    #if REGION_JPN
    }
    #endif
    #if REGION_PAL || REGION_JPN
    }
    #endif

    cursorAnm();
}

int dName_c::nameCheck() {
    for (int i = 8, len = 7; i > 0; i--) {
        #if REGION_JPN
        // '　' (full-width space)
        if (mChrInfo[len].mCharacter != ' ' && mChrInfo[len].mCharacter != '\x81\x40') {
        #else
        if (mChrInfo[len].mCharacter != ' ') {
        #endif
            return len + 1;
        }
        len--;
    }

    return 0;
}

void dName_c::playNameSet(int nameLength) {
    char* str = mInputStr;

    for (int i = 0; i < nameLength; i++) {
        #if REGION_JPN
        if (mChrInfo[i].mMojiSet == 2) {
            *str = mChrInfo[i].mCharacter;
            str += 1;
        } else {
            *(u16*)str = mChrInfo[i].mCharacter;
            str += 2;
        }
        #else
        *str = mChrInfo[i].mCharacter;
        str++;
        #endif
    }

    *str = 0;
}

void dName_c::cursorAnm() {
    mCurColAnmF += 2;
    if (mCurColAnmF >= mCursorColorKey->getFrameMax()) {
        mCurColAnmF -= mCursorColorKey->getFrameMax();
    }
    mCursorColorKey->setFrame(mCurColAnmF);

    mCurTexAnmF += 2;
    if (mCurTexAnmF >= mCursorTexKey->getFrameMax()) {
        mCurTexAnmF -= mCursorTexKey->getFrameMax();
    }
    mCursorTexKey->setFrame(mCurTexAnmF);

    nameIn.NameInScr->animation();
}

void dName_c::Wait() {}

void dName_c::MojiSelect() {
    if (mDoCPd_c::getTrigA(PAD_1)) {
        selectMojiSet();
    } else if (stick->checkRightTrigger()) {
        mDoAud_seStart(Z2SE_SY_NAME_CURSOR, 0, 0, 0);
        mPrevColumn = mCharColumn;
        mPrevRow = mCharRow;
        mCharColumn++;

        if (mCharColumn > 12) {
            mCharColumn = 0;
        }
        MojiSelectAnmInit();
        mSelProc = PROC_MOJI_SEL_ANM;
    } else if (stick->checkLeftTrigger()) {
        mDoAud_seStart(Z2SE_SY_NAME_CURSOR, 0, 0, 0);
        mPrevColumn = mCharColumn;
        mPrevRow = mCharRow;

        if (mCharColumn == 0) {
            mCharColumn = 12;
        } else {
            mCharColumn--;
        }
        MojiSelectAnmInit();
        mSelProc = PROC_MOJI_SEL_ANM;
    } else if (stick->checkUpTrigger()) {
        mPrevColumn = mCharColumn;
        mPrevRow = mCharRow;
        MojiSelectAnmInit();

        if (mCharRow == 0) {
            mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, 0, 0, 0);
            menuCursorPosSet();
            mSelProc = PROC_MOJI_SEL_ANM2;
        } else {
            mDoAud_seStart(Z2SE_SY_NAME_CURSOR, 0, 0, 0);
            mCharRow--;
            mSelProc = PROC_MOJI_SEL_ANM;
        }
    } else if (stick->checkDownTrigger()) {
        mPrevColumn = mCharColumn;
        mPrevRow = mCharRow;
        MojiSelectAnmInit();
        mCharRow++;

        if (mCharRow > 4) {
            mCharRow = 4;
            mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, 0, 0, 0);
            menuCursorPosSet();
            mSelProc = PROC_MOJI_SEL_ANM2;
        } else {
            mDoAud_seStart(Z2SE_SY_NAME_CURSOR, 0, 0, 0);
            mSelProc = PROC_MOJI_SEL_ANM;
        }
    }
}

void dName_c::MojiSelectAnmInit() {
    mSelIcon->setAlphaRate(0.0f);
    mMojiIcon[mPrevRow + mPrevColumn * 5]->scaleAnimeStart(0);
    ((J2DTextBox*)mMojiIcon[mPrevRow + mPrevColumn * 5]->getPanePtr())
        ->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
}

void dName_c::MojiSelectAnm() {
    if (mMojiIcon[mPrevRow + mPrevColumn * 5]->scaleAnime(mCursorDelay, g_nmHIO.mSelCharScale,
                                                          1.0f, 0) == 1)
    {
        selectCursorMove();
        mSelProc = PROC_MOJI_SELECT;
        field_0x2ad = mSelProc;
    }
}

void dName_c::MojiSelectAnm2() {
    if (mMojiIcon[mPrevRow + mPrevColumn * 5]->scaleAnime(mCursorDelay, g_nmHIO.mSelCharScale,
                                                          1.0f, 0) == 1)
    {
        menuCursorMove2();
        mSelProc = PROC_MENU_SELECT;
        field_0x2ad = mSelProc;
    }
}

void dName_c::MojiSelectAnm3() {}

int dName_c::mojiChange(u8 idx) {
    if (mChrInfo[idx].field_0x3 == 0 || mChrInfo[idx].mMojiSet == MOJI_EIGO ||
        mChrInfo[idx].mCharacter == '　')
    {
        return 0;
    }

    if (mChrInfo[idx].mColumn == 4 || mChrInfo[idx].mColumn == 6 || mChrInfo[idx].mColumn == 8 ||
        mChrInfo[idx].mColumn == 9)
    {
        return 0;
    }

    switch (mChrInfo[idx].mColumn) {
    case 0:
    case 10: {
        if (mChrInfo[idx].mCharacter == 'ウ' || mChrInfo[idx].mCharacter == 'ゥ' ||
            mChrInfo[idx].mCharacter == 'ヴ')
        {
            mChrInfo[idx].mCharacter++;

            if (mChrInfo[idx].mCharacter == 'ェ') {
                mChrInfo[idx].mCharacter = 'ヴ';
            }

            if (mChrInfo[idx].mCharacter == 'ヵ') {
                mChrInfo[idx].mCharacter = 'ゥ';
            }
        } else {
            int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'ァ' : 'ぁ';

            if ((mChrInfo[idx].mCharacter - c) % 2) {
                --mChrInfo[idx].mCharacter;
            } else {
                ++mChrInfo[idx].mCharacter;
            }
        }
        break;
    }
    case 1: {
        int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'カ' : 'か';
        c = ((mChrInfo[idx].mCharacter - c) % 2);

        int c2 = c + 1;
        mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
        break;
    }
    case 2: {
        int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'サ' : 'さ';
        c = ((mChrInfo[idx].mCharacter - c) % 2);

        int c2 = c + 1;
        mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
        break;
    }
    case 3:
    case 12: {
        if (mChrInfo[idx].mCharacter != (u32)0x815b) {
            if (mChrInfo[idx].mCharacter <= (mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'ヂ' : 'ぢ')) {
                int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'タ' : 'た';
                c = ((mChrInfo[idx].mCharacter - c) % 2);

                int c2 = c + 1;
                mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
            } else if (mChrInfo[idx].mCharacter <=
                           (mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'ド' : 'ど') &&
                       mChrInfo[idx].mCharacter >=
                           (mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'テ' : 'て'))
            {
                int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'テ' : 'て';
                c = ((mChrInfo[idx].mCharacter - c) % 2);

                int c2 = c + 1;
                mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
            } else {
                int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'ッ' : 'っ';
                int c2 = (mChrInfo[idx].mCharacter - c) % 3;

                int ivar2 = c2 + 1;
                if (ivar2 > 2) {
                    ivar2 = 0;
                }

                mChrInfo[idx].mCharacter = ivar2 + (mChrInfo[idx].mCharacter - c2);
            }
        }
        break;
    }
    case 5: {
        int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'ハ' : 'は';
        int c2 = (mChrInfo[idx].mCharacter - c) % 3;

        int ivar2 = c2 + 1;
        if (ivar2 > 2) {
            ivar2 = 0;
        }

        mChrInfo[idx].mCharacter = ivar2 + (mChrInfo[idx].mCharacter - c2);
        break;
    }
    case 7:
    case 11: {
        int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? 'ャ' : 'ゃ';
        c = ((mChrInfo[idx].mCharacter - c) % 2);

        int c2 = c + 1;
        mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
        break;
    }
    }

    setNameText();
    return 1;
}

void dName_c::selectMojiSet() {
    #if REGION_JPN
    int moji = getMoji();
    if (moji != -1) {
        if (moji == '゛' || moji == '゜') {
            if (mCurPos != 0) {
                if (checkDakuon(moji, mCurPos - 1) == 1) {
                    mDoAud_seStart(Z2SE_SY_NAME_INPUT, NULL, 0, 0);
                    setDakuon(moji, mCurPos - 1);
                } else {
                    mDoAud_seStart(Z2SE_SYS_ERROR, NULL, 0, 0);
                }
            }
        } else {
            setMoji(moji);
        }
    }
    setNameText();
    #else
    setMoji(getMoji());
    setNameText();
    #endif
}

#if REGION_JPN
int dName_c::checkDakuon(int param_0, u8 param_1) {
    if (mChrInfo[param_1].mMojiSet == MOJI_EIGO) {
        return 0;
    }

    if (param_1 == 0 && mChrInfo[param_1].field_0x3 == 0) {
        return 0;
    }

    if (param_0 == '゜' && mChrInfo[param_1].mColumn != 5) {
        return 0;
    }

    if (param_0 == '゛' &&
        (mChrInfo[param_1].mCharacter == 'ウ' || mChrInfo[param_1].mCharacter == 'ヴ'))
    {
        return 1;
    }

    if (param_0 == '゛' && mChrInfo[param_1].mColumn != 1 && mChrInfo[param_1].mColumn != 2 &&
        mChrInfo[param_1].mColumn != 3 && mChrInfo[param_1].mColumn != 5)
    {
        return 0;
    }

    return 1;
}

int dName_c::setDakuon(int param_1, u8 param_2) {
    int c;

    if (param_1 == '゛') {
        switch (mChrInfo[param_2].mColumn) {
        case 0: {
            c = -1;
            if (mChrInfo[param_2].mCharacter == 'ウ' || mChrInfo[param_2].mCharacter == 'ヴ') {
                c = 4;
                mChrInfo[param_2].mCharacter = 'ヴ';
            }
            break;
        }
        case 1: {
            int c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'カ' : 'か';
            c = (mChrInfo[param_2].mCharacter - c2) % 2;
            break;
        }
        case 2: {
            int c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'サ' : 'さ';
            c = (mChrInfo[param_2].mCharacter - c2) % 2;
            break;
        }
        case 3: {
            int c2;
            if (mChrInfo[param_2].mCharacter <=
                ((mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'ヂ' : 'ぢ')))
            {
                c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'タ' : 'た';
                c = (mChrInfo[param_2].mCharacter - c2) % 2;
            } else {
                if (mChrInfo[param_2].mCharacter <= (mChrInfo[param_2].mMojiSet != 0 ? 'ド' : 'ど'))
                {
                    if (mChrInfo[param_2].mCharacter >=
                        (mChrInfo[param_2].mMojiSet != 0 ? 'テ' : 'て'))
                    {
                        c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'テ' : 'て';
                        c = (mChrInfo[param_2].mCharacter - c2) % 2;
                        break;
                    }
                }

                c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'ッ' : 'っ';
                c = (mChrInfo[param_2].mCharacter - c2) % 3;
                if (c == 2) {
                    c = 1;
                } else if (c == 1) {
                    c = 0;
                } else if (c == 0) {
                    c = 3;
                }
            }
            break;
        }
        case 5: {
            int c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'ハ' : 'は';
            c = (mChrInfo[param_2].mCharacter - c2) % 3;
            break;
        }
        }

        if (c != 1) {
            if (c == 2) {
                mChrInfo[param_2].mCharacter -= 1;
            } else if (c == 0) {
                mChrInfo[param_2].mCharacter += 1;
            } else if (c == 3) {
                mChrInfo[param_2].mCharacter += 2;
            }

            setNameText();

            return 1;
        }
    } else if (param_1 == '゜') {
        int c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'ハ' : 'は';
        c = (mChrInfo[param_2].mCharacter - c2) % 3;
        if (c != 2) {
            mChrInfo[param_2].mCharacter = mChrInfo[param_2].mCharacter + (2 - c);
            setNameText();

            return 1;
        }
    }

    return 0;
}
#endif

int dName_c::getMoji() {
    int result = -1;
    const char* moji;

    #if REGION_PAL
    switch (mMojiSet) {
    case MOJI_HIRA:
        moji = l_mojiEisuPal_1[mCharRow + mCharColumn * 5];
        break;
    case MOJI_KATA:
        moji = l_mojiEisuPal_2[mCharRow + mCharColumn * 5];
        break;
    }
    #else
    switch (mMojiSet) {
    case MOJI_HIRA:
        moji = l_mojiHira[mCharRow + mCharColumn * 5];
        break;
    case MOJI_KATA:
        moji = l_mojikata[mCharRow + mCharColumn * 5];
        break;
    case MOJI_EIGO:
        moji = l_mojiEisu[mCharRow + mCharColumn * 5];
        break;
    }
    #endif

    #if REGION_JPN
    if (*(u8*)moji >> 4 == 0x8 || *(u8*)moji >> 4 == 0x9) {
        result = *(u16*)moji;
    } else {
        result = *(char*)moji;
    }
    #else
    result = *moji;
    #endif

    return result;
}

void dName_c::setMoji(int moji) {
    if (mCurPos == 8 || nameCheck() == 8) {
        mDoAud_seStart(Z2SE_SYS_ERROR, NULL, 0, 0);
    } else {
        mDoAud_seStart(Z2SE_SY_NAME_INPUT, NULL, 0, 0);

        s32 notEmpty = false;
        for (int i = mCurPos; i < 8; i++) {
            #if REGION_JPN
            // '　' (full-width space)
            if (mChrInfo[i].mCharacter != '\x81\x40') {
            #else
            if (mChrInfo[i].mCharacter != ' ') {
            #endif
                notEmpty = true;
                break;
            }
        }

        if (notEmpty) {
            #if REGION_JPN
            // '　' (full-width space)
            if (mChrInfo[7].mCharacter == '\x81\x40') {
            #else
            if (mChrInfo[7].mCharacter == ' ') {
            #endif
                for (int i = 6; i >= mCurPos; i--) {
                    mChrInfo[i + 1] = mChrInfo[i];
                }

                mChrInfo[mCurPos].mColumn = mCharColumn;
                mChrInfo[mCurPos].mRow = mCharRow;
                mChrInfo[mCurPos].mMojiSet = mMojiSet;
                mChrInfo[mCurPos].field_0x3 = 1;
                #if REGION_PAL
                mChrInfo[mCurPos].mCharacter = moji & 0xFF;
                #else
                mChrInfo[mCurPos].mCharacter = moji;
                #endif

                if (mCurPos != 8) {
                    mLastCurPos = mCurPos;
                    mCurPos++;
                    nameCursorMove();
                }
            }
        } else {
            mChrInfo[mCurPos].mColumn = mCharColumn;
            mChrInfo[mCurPos].mRow = mCharRow;
            mChrInfo[mCurPos].mMojiSet = mMojiSet;
            mChrInfo[mCurPos].field_0x3 = 1;
            #if REGION_PAL
            mChrInfo[mCurPos].mCharacter = moji & 0xFF;
            #else
            mChrInfo[mCurPos].mCharacter = moji;
            #endif

            if (mCurPos != 8) {
                mLastCurPos = mCurPos;
                mCurPos++;
                nameCursorMove();
            }
        }
    }
}


void dName_c::setNameText() {
    for (int i = 0; i < 8; i++) {
        //"\x1bCD\x1bCR\x1bCC[000000]\x1bGM[0]%c\x1bHM\x1bCC[ffffff]\x1bGM[0]%c"
        //"\x1bCD\x1bCR\x1bCC[000000]\x1bGM[0]%c%c\x1bHM\x1bCC[ffffff]\x1bGM[0]%c%c"
        if (mChrInfo[i].field_0x3 != 0) {
            #if REGION_JPN
            if (mChrInfo[i].mMojiSet == 2) {
            #endif
                sprintf(mNameText[i],
                        "\x1b"
                        "CD\x1b"
                        "CR\x1b"
                        "CC[000000]\x1bGM[0]%c\x1bHM\x1b"
                        "CC[ffffff]\x1bGM[0]%c",
                        #if REGION_PAL
                        (u8)mChrInfo[i].mCharacter & 0xFF,
                        (u8)mChrInfo[i].mCharacter & 0xFF
                        #else
                        (u8)mChrInfo[i].mCharacter,
                        (u8)mChrInfo[i].mCharacter
                        #endif
                );
            #if REGION_JPN
            } else {
                sprintf(mNameText[i],
                        "\x1b"
                        "CD\x1b"
                        "CR\x1b"
                        "CC[000000]\x1bGM[0]%c%c\x1bHM\x1b"
                        "CC[ffffff]\x1bGM[0]%c%c",
                        (mChrInfo[i].mCharacter & 0xff00) >> 8,
                        (mChrInfo[i].mCharacter & 0xff),
                        (mChrInfo[i].mCharacter & 0xff00) >> 8,
                        (mChrInfo[i].mCharacter & 0xff)
                );
            }
            #endif
        }
    }
}

void dName_c::nameCursorMove() {
    if (mCurPos <= 8) {
        u8 position = mCurPos;

        if (position > 7) {
            position = 7;
        }

        if (mLastCurPos != 255 && mLastCurPos < 8) {
            mNameCursor[mLastCurPos]->hide();
        }

        mNameCursor[position]->show();
    }
}

void dName_c::selectCursorMove() {
    int idx;
    #if REGION_PAL
    if (mCharColumn < 3) {
        idx = 0;
    } else if (mCharColumn < 6) {
        idx = 1;
    } else if (mCharColumn >= 6) {
        idx = 2;
    }
    #elif REGION_JPN
    if (mCharColumn < 3) {
        idx = 0;
    } else if (mCharColumn < 6) {
        idx = 1;
    } else if (mCharColumn < 8) {
        idx = 2;
    } else if (mCharColumn >= 8) {
        idx = 3;
    }
    #else
    idx = 3;
    #endif
    field_0x30c[idx][0] = mCharColumn;
    field_0x30c[idx][1] = mCharRow;
    field_0x30c[idx][2] = 1;

    mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr()->scale(g_nmHIO.mSelCharScale,
                                                               g_nmHIO.mSelCharScale);
    ((J2DTextBox*)mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr())
        ->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));

    Vec pos = mMojiIcon[mCharRow + mCharColumn * 5]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
}

void dName_c::menuCursorPosSet() {
    mPrevSelMenu = mSelMenu;
    #if REGION_PAL
    if (mCharColumn < 3) {
        mSelMenu = MENU_HIRA;
    } else if (mCharColumn < 6) {
        mSelMenu = MENU_KATA;
    } else if (mCharColumn >= 6) {
        mSelMenu = MENU_EIGO;
    }
    #elif REGION_JPN
    if (mCharColumn < 3) {
        mSelMenu = MENU_HIRA;
        return;
    }
    if (mCharColumn < 6) {
        mSelMenu = MENU_KATA;
        return;
    }
    if (mCharColumn < 8) {
        mSelMenu = MENU_EIGO;
        return;
    }
    if (mCharColumn >= 8) {
        mSelMenu = MENU_END;
        return;
    }
    #else
    mSelMenu = MENU_END;
    #endif
}

void dName_c::MenuSelect() {
    #if REGION_PAL || REGION_JPN
    if (stick->checkRightTrigger()) {
        mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0);
        mPrevSelMenu = mSelMenu;
        mSelMenu++;
        #if REGION_PAL
        if (mSelMenu > MENU_EIGO) {
        #else
        if (mSelMenu > MENU_END) {
        #endif
            mSelMenu = MENU_HIRA;
        }
        MenuSelectAnmInit();
        mSelProc = PROC_MENU_SEL_ANM;
    } else if (stick->checkLeftTrigger()) {
        mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0);
        mPrevSelMenu = mSelMenu;
        if (mSelMenu == MENU_HIRA) {
            #if REGION_JPN
            mSelMenu = MENU_END;
            #else
            mSelMenu = MENU_EIGO;
            #endif
        } else {
            mSelMenu--;
        }
        MenuSelectAnmInit();
        mSelProc = PROC_MENU_SEL_ANM;
    } else {
    #else
    if (!stick->checkRightTrigger() && !stick->checkLeftTrigger()) {
    #endif
        if (stick->checkUpTrigger()) {
            mDoAud_seStart(Z2SE_SY_NAME_CURSOR, NULL, 0, 0);
            mPrevSelMenu = mSelMenu;
            selectCursorPosSet(4);
            MenuSelectAnmInit();
            mSelProc = PROC_MENU_SEL_ANM2;
        } else if (stick->checkDownTrigger()) {
            mDoAud_seStart(Z2SE_SY_NAME_CURSOR, NULL, 0, 0);
            mPrevSelMenu = mSelMenu;
            selectCursorPosSet(0);
            MenuSelectAnmInit();
            mSelProc = PROC_MENU_SEL_ANM2;
        } else if (mDoCPd_c::getTrigA(PAD_1)) {
            #if REGION_PAL
            if (mSelMenu == MENU_EIGO) {
            #else
            if (mSelMenu == MENU_END) {
            #endif
                if (nameCheck() != 0) {
                    mDoAud_seStart(Z2SE_SY_NAME_OK, NULL, 0, 0);
                } else {
                    mDoAud_seStart(Z2SE_SYS_ERROR, NULL, 0, 0);
                }
            } else {
                mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
            }
            menuAbtnSelect();
        } else if (mDoCPd_c::getTrigStart(PAD_1)) {
            #if REGION_PAL
            if (mSelMenu == MENU_EIGO) {
            #else
            if (mSelMenu == MENU_END) {
            #endif
                if (nameCheck() != 0) {
                    mDoAud_seStart(Z2SE_SY_NAME_OK, NULL, 0, 0);
                } else {
                    mDoAud_seStart(Z2SE_SYS_ERROR, NULL, 0, 0);
                }
                menuAbtnSelect();
            }
        }
    }
}

void dName_c::MenuSelectAnmInit() {
    mSelIcon->setAlphaRate(0.0f);

    int prevMenu_i = getMenuPosIdx(mPrevSelMenu);
    mMenuIcon[prevMenu_i]->scaleAnimeStart(0);
}

void dName_c::MenuSelectAnm() {
    int prevMenu_i = getMenuPosIdx(mPrevSelMenu);

    if (mMenuIcon[prevMenu_i]->scaleAnime(mCursorDelay, g_nmHIO.mMenuScale, 1.0f, 0) == 1) {
        mMenuText[prevMenu_i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
        menuCursorMove();
        mSelProc = PROC_MENU_SELECT;
        field_0x2ad = mSelProc;
    }
}

void dName_c::MenuSelectAnm2() {
    int prevMenu_i = getMenuPosIdx(mPrevSelMenu);
    int mojiSet_i = getMenuPosIdx(mMojiSet);

    bool canMove = true;
    if (prevMenu_i != mojiSet_i) {
        canMove = mMenuIcon[prevMenu_i]->scaleAnime(mCursorDelay, g_nmHIO.mMenuScale, 1.0f, 0);
    }

    if (canMove == true) {
        if (prevMenu_i != mojiSet_i) {
            mMenuText[prevMenu_i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
            #if REGION_PAL || REGION_JPN
            mMenuIcon[mojiSet_i]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
            mMenuText[mojiSet_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
            #endif
        }
        selectCursorMove();
        mSelProc = PROC_MOJI_SELECT;
        field_0x2ad = mSelProc;
    }
}

void dName_c::MenuSelectAnm3() {}

void dName_c::menuAbtnSelect() {
    switch (mSelMenu) {
    case MENU_HIRA:
    case MENU_KATA:
    #if !REGION_PAL
    case MENU_EIGO:
    #endif
        if (mSelMenu != mMojiSet) {
            mPrevMojiSet = mMojiSet;
            mMojiSet = mSelMenu;
            mojiListChange();
        }
        break;
    #if REGION_PAL
    case MENU_EIGO:
    #else
    case MENU_END:
    #endif
        int nameNum = nameCheck();
        if (nameNum != 0) {
            playNameSet(nameNum);
            field_0x2ac = mSelProc;
            field_0x2ae = mSelProc;
            mSelProc = PROC_WAIT;
            mIsInputEnd = 2;
        } else {
            mDoAud_seStart(Z2SE_SY_DUMMY, NULL, 0, 0);
        }
        break;
    }
}

void dName_c::backSpace() {
    if (mCurPos != 0) {
        mDoAud_seStart(Z2SE_SY_NAME_DELETE, NULL, 0, 0);

        #if REGION_JPN
        // '　' (full-width space)
        if (mCurPos == 8 && mChrInfo[7].mCharacter != '\x81\x40') {
        #else
        if (mCurPos == 8 && mChrInfo[7].mCharacter != ' ') {
        #endif
            mChrInfo[7].mColumn = 7;
            mChrInfo[7].mRow = 1;
            #if REGION_PAL || REGION_JPN
            mChrInfo[7].mMojiSet = MOJI_HIRA;
            #else
            mChrInfo[7].mMojiSet = MOJI_EIGO;
            #endif
            mChrInfo[7].field_0x3 = 1;
            #if REGION_JPN
            // '　' (full-width space)
            mChrInfo[7].mCharacter = '\x81\x40';
            #else
            mChrInfo[7].mCharacter = ' ';
            #endif
        } else {
            for (int i = mCurPos - 1; i < 7; i++) {
                mChrInfo[i] = mChrInfo[i + 1];
            }
            mChrInfo[7].mColumn = 7;
            mChrInfo[7].mRow = 1;
            #if REGION_PAL || REGION_JPN
            mChrInfo[7].mMojiSet = MOJI_HIRA;
            #else
            mChrInfo[7].mMojiSet = MOJI_EIGO;
            #endif
            mChrInfo[7].field_0x3 = 1;
            #if REGION_JPN
            // '　' (full-width space)
            mChrInfo[7].mCharacter = '\x81\x40';
            #else
            mChrInfo[7].mCharacter = ' ';
            #endif
        }

        setNameText();
        mLastCurPos = mCurPos;
        mCurPos--;
        nameCursorMove();
    }
}

void dName_c::mojiListChange() {
    #if REGION_PAL
    char** mojiSet;

    switch (mMojiSet) {
    case MOJI_HIRA:
        mojiSet = l_mojiEisuPal_1;
        break;
    case MOJI_KATA:
        mojiSet = l_mojiEisuPal_2;
        break;
    }
    #else
    const char** mojiSet;

    switch (mMojiSet) {
    case MOJI_HIRA:
        mojiSet = l_mojiHira;
        break;
    case MOJI_KATA:
        mojiSet = l_mojikata;
        break;
    case MOJI_EIGO:
        mojiSet = l_mojiEisu;
        break;
    }
    #endif

    char buf[74];
    for (int i = 0; i < 65; i++) {
        strcpy(buf, "\x1B"
                    "CD"
                    "\x1B"
                    "CR"
                    "\x1B"
                    "CC[000000]"
                    "\x1B"
                    "GM[0]");
        strcat(buf, mojiSet[i]);
        strcat(buf, "\x1B"
                    "HM"
                    "\x1B"
                    "CC[ffffff]"
                    "\x1B"
                    "GM[0]");
        strcat(buf, mojiSet[i]);
        strcpy(mMojiText[i], buf);
    }

    #if REGION_PAL || REGION_JPN
    if (mSelProc == PROC_MOJI_SELECT) {
        mMenuIcon[mMojiSet]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
        mMenuText[mMojiSet]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
        if (mPrevMojiSet != 255) {
            mMenuIcon[mPrevMojiSet]->scale(1.0f, 1.0f);
            mMenuText[mPrevMojiSet]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
        }
    }
    #endif
}

void dName_c::menuCursorMove() {
    int menu_i = getMenuPosIdx(mSelMenu);
    mMenuIcon[menu_i]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
    mMenuText[menu_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));

    Vec pos = mMenuIcon[menu_i]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mMenuIcon[menu_i]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
}

void dName_c::menuCursorMove2() {
    int menu_i = getMenuPosIdx(mSelMenu);
    int mojiSet_i = getMenuPosIdx(mMojiSet);

    if (menu_i != mojiSet_i) {
        mMenuIcon[menu_i]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
        mMenuText[menu_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
        #if REGION_PAL || REGION_JPN
        mMenuIcon[mojiSet_i]->scale(1.0f, 1.0f);
        mMenuText[mojiSet_i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
        #endif
    }

    Vec pos = mMenuIcon[menu_i]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mMenuIcon[menu_i]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
}

void dName_c::selectCursorPosSet(int row) {
    if (field_0x30c[mSelMenu][2] == 1) {
        mCharColumn = field_0x30c[mSelMenu][0];
        mCharRow = row;
    } else {
        switch (mSelMenu) {
        case MENU_HIRA:
            mCharColumn = 0;
            break;
        case MENU_KATA:
            mCharColumn = 3;
            break;
        case MENU_EIGO:
            #if REGION_PAL
            mCharColumn = 8;
            #else
            mCharColumn = 6;
            #endif
            break;
        #if !REGION_PAL
        case MENU_END:
            mCharColumn = 8;
            break;
        #endif
        }

        mCharRow = row;
        field_0x30c[mSelMenu][0] = mCharColumn;
        field_0x30c[mSelMenu][1] = mCharRow;
        field_0x30c[mSelMenu][2] = 1;
    }
}

void dName_c::_draw() {
    dComIfGd_set2DOpa(&nameIn);
    dComIfGd_set2DOpa(mSelIcon);
}

void dName_c::screenSet() {
    static u64 l_cur0TagName[8] = {
        's_00', 's_01', 's_02', 's_03', 's_04', 's_05', 's_06', 's_07',
    };
    static u64 l_cur1TagName[8] = {
        's_0r', 's_01r', 's_02r', 's_03r', 's_04r', 's_05r', 's_06r', 's_07r',
    };
#if REGION_JPN
    static u64 l_menu_icon_tag[4] = {
        'j_hira_n',
        'j_kata_n',
        'j_eigo_n',
        'j_end_n',
    };
    static u64 l_menu_tag[5][3] = {
        'm_hira_0',  'm_hira_1',  'm_hira_s',  'm_kata_0', 'm_kata_1', 'm_kata_s',
        'm_eigo_0', 'm_eigo_1', 'm_eigo_s', 'j_end_0', 'j_end_1', 'j_end_s',
    };
    static u32 l_menu_msg[4] = {
        0x386,
        0x387,
        0x388,
        0x38A,
    };
#else
    static u64 l_menu_icon_tag[4] = {
        'p_ABC_n',
        'p_abc_n',
        'j_eigo_n',
        'p_end_n',
    };
    static u64 l_menu_tag[5][3] = {
        'p_ABC_0',  'p_ABC_1',  'p_ABC_2',  'p_abc_0', 'p_abc_1', 'p_abc_2',
        'm_eigo_0', 'm_eigo_1', 'm_eigo_2', 'p_end_0', 'p_end_1', 'p_end_2',
    };
    static u32 l_menu_msg[4] = {
        0x38B,
        0x38C,
        0x388,
        0x38E,
    };
#endif
    static u64 l_tagName[65] = {
        'm_00_0', 'm_00_1', 'm_00_2', 'm_00_3', 'm_00_4', 'm_01_0', 'm_01_1', 'm_01_2', 'm_01_3',
        'm_01_4', 'm_02_0', 'm_02_1', 'm_02_2', 'm_02_3', 'm_02_4', 'm03_0',  'm03_1',  'm03_2',
        'm03_3',  'm03_4',  'm_04_0', 'm_04_1', 'm_04_2', 'm_04_3', 'm_04_4', 'm_05_0', 'm_05_1',
        'm_05_2', 'm_05_3', 'm_05_4', 'm_06_0', 'm_06_1', 'm_06_2', 'm_06_3', 'm_06_4', 'm_07_0',
        'm_07_1', 'm_07_2', 'm_07_3', 'm_07_4', 'm_08_0', 'm_08_1', 'm_08_2', 'm_08_3', 'm_08_4',
        'm_09_0', 'm_09_1', 'm_09_2', 'm_09_3', 'm_09_4', 'm_10_0', 'm_10_1', 'm_10_2', 'm_10_3',
        'm_10_4', 'm_11_0', 'm_11_1', 'm_11_2', 'm_11_3', 'm_11_4', 'm12_0',  'm12_1',  'm12_2',
        'm12_3',  'm12_4',
    };
    static u64 l_nameTagName[8] = {
        'name_00', 'name_01', 'name_02', 'name_03', 'name_04', 'name_05', 'name_06', 'name_07',
    };
    static u64 l_nameCurTagName[8] = {
        's__n_00', 's__n_01', 's__n_02', 's__n_03', 's__n_04', 's__n_05', 's__n_06', 's__n_07',
    };

    nameIn.NameInScr = new J2DScreen();
    JUT_ASSERT(0, nameIn.NameInScr != NULL);

    archive = dComIfGp_getNameResArchive();
    nameIn.NameInScr->setPriority("zelda_player_name.blo", 0x100000, archive);
    dPaneClass_showNullPane(nameIn.NameInScr);
    nameIn.field_0x10 = nameIn.NameInScr->search('name_n');

    void* bpk = JKRGetNameResource("zelda_player_name.bpk", archive);
    JUT_ASSERT(0, bpk != NULL);
    mCursorColorKey = (J2DAnmColorKey*)J2DAnmLoaderDataBase::load(bpk);
    mCursorColorKey->searchUpdateMaterialID(nameIn.NameInScr);

    void* btk = JKRGetNameResource("zelda_player_name.btk", archive);
    JUT_ASSERT(0, btk != NULL);
    mCursorTexKey = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mCursorTexKey->searchUpdateMaterialID(nameIn.NameInScr);

    J2DPane* panes0[8];
    J2DPane* panes1[8];
    for (int i = 0; i < 8; i++) {
        panes0[i] = nameIn.NameInScr->search(l_cur0TagName[i]);
        panes1[i] = nameIn.NameInScr->search(l_cur1TagName[i]);

        panes0[i]->setAnimation(mCursorTexKey);
        panes0[i]->setAnimation(mCursorColorKey);
        panes1[i]->setAnimation(mCursorTexKey);
        panes1[i]->setAnimation(mCursorColorKey);
    }

    #if REGION_JPN
    nameIn.NameInScr->search('pal_n')->hide();
    mMenuPane = nameIn.NameInScr->search('jpn_n');
    mMenuPane->show();

    nameIn.NameInScr->search('p_ABC_n')->scale(0.0f, 0.0f);
    nameIn.NameInScr->search('p_abc_n')->scale(0.0f, 0.0f);
    nameIn.NameInScr->search('p_end_n')->scale(0.0f, 0.0f);
    #else
    nameIn.NameInScr->search('jpn_n')->hide();
    mMenuPane = nameIn.NameInScr->search('pal_n');
    mMenuPane->show();

    nameIn.NameInScr->search('j_hira_n')->scale(0.0f, 0.0f);
    nameIn.NameInScr->search('j_kata_n')->scale(0.0f, 0.0f);
    nameIn.NameInScr->search('j_eigo_n')->scale(0.0f, 0.0f);
    nameIn.NameInScr->search('j_end_n')->scale(0.0f, 0.0f);
    #endif

    J2DTextBox* menuPane[3];
    for (int i = 0; i < 4; i++) {
        #if !REGION_JPN
        if (i == 2) {
            mMenuIcon[i] = NULL;
            mMenuText[i] = NULL;
        } else {
        #endif
            mMenuIcon[i] = new CPaneMgr(nameIn.NameInScr, l_menu_icon_tag[i], 1, NULL);

            char buf[16];
            fopMsgM_messageGet(buf, l_menu_msg[i]);

            for (int j = 0; j < 3; j++) {
                menuPane[j] = (J2DTextBox*)nameIn.NameInScr->search(l_menu_tag[i][j]);

                if (j == 0) {
                    mMenuText[i] = menuPane[j];
                }

                menuPane[j]->setFont(nameIn.font);
                menuPane[j]->setString(buf);
            }
        #if !REGION_JPN
        }
        #endif
    }

    #if !(REGION_PAL || REGION_JPN)
    mMenuIcon[0]->hide();
    mMenuIcon[1]->hide();
    #endif
    mMojiPane = nameIn.NameInScr->search('moji_n');

    for (u32 i = 0; i < 65; i++) {
        mMojiIcon[i] = new CPaneMgr(nameIn.NameInScr, l_tagName[i], 2, NULL);
        ((J2DTextBox*)mMojiIcon[i]->getPanePtr())->setFont(nameIn.font);
        ((J2DTextBox*)mMojiIcon[i]->getPanePtr())->setString(72, "");
        mMojiText[i] = ((J2DTextBox*)mMojiIcon[i]->getPanePtr())->getStringPtr();
    }

    J2DPane* nameTagPane[8];
    for (int i = 0; i < 8; i++) {
        mNameCursor[i] = new CPaneMgrAlpha(nameIn.NameInScr, l_nameCurTagName[i], 2, NULL);
        nameTagPane[i] = nameIn.NameInScr->search(l_nameTagName[i]);
        ((J2DTextBox*)nameTagPane[i])->setFont(nameIn.font);
        ((J2DTextBox*)nameTagPane[i])->setString(72, "");
        ((J2DTextBox*)nameTagPane[i])->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
        #if REGION_PAL
        ((J2DTextBox*)nameTagPane[i])->resize(24.0f, 23.0f);
        #endif
        mNameText[i] = ((J2DTextBox*)nameTagPane[i])->getStringPtr();
    }

    #if REGION_PAL
    int idx = 2;

    static u8 palMoji00[13] = {
        0xC0, 0xC1, 0xC2, 0xC4, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE,
    };
    static u8 palMoji01[13] = {
        0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD6, 0x8C, 0xD9, 0xDA, 0xDB, 0xDC, 0x2D,
    };
    static u8 palMoji10[13] = {
        0xE0, 0xE1, 0xE2, 0xE4, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE,
    };
    static u8 palMoji11[13] = {
        0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF6, 0x9C, 0xF9, 0xFA, 0xFB, 0xFC, 0xDF,
    };

    for (int i = 0; i < 13; i++, idx += 5) {
        l_mojiEisuPal_1[idx][0] = palMoji00[i];
        l_mojiEisuPal_1[idx][1] = 0;

        l_mojiEisuPal_1[idx + 1][0] = palMoji01[i];
        l_mojiEisuPal_1[idx + 1][1] = 0;

        l_mojiEisuPal_2[idx][0] = palMoji10[i];
        l_mojiEisuPal_2[idx][1] = 0;

        l_mojiEisuPal_2[idx + 1][0] = palMoji11[i];
        l_mojiEisuPal_2[idx + 1][1] = 0;
    }
    #endif

    mCharColumn = 0;
    mCharRow = 0;

    mSelIcon = new dSelect_cursor_c(0, 1.0f, NULL);
    JUT_ASSERT(0, mSelIcon != NULL);
    mSelIcon->setParam(0.82f, 0.77f, 0.05f, 0.4f, 0.4f);

    Vec pos = mMojiIcon[mCharRow + mCharColumn * 5]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr(), true);
    mSelIcon->setAlphaRate(0.0f);
}


void dName_c::displayInit() {
    mSelIcon->setAlphaRate(0.0f);
    mCurColAnmF = 0;
    mCurTexAnmF = 0;
    mSelProc = field_0x2ac;
    field_0x2ad = field_0x2ae;

    for (int i = 0; i < 65; i++) {
        ((J2DTextBox*)mMojiIcon[i]->getPanePtr())
            ->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
    }

    for (int i = 0; i < 4; i++) {
        if (mMenuText[i] != NULL) {
            mMenuText[i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
        }
    }

    for (int i = 0; i < 8; i++) {
        mNameCursor[i]->hide();
        mChrInfo[i].mColumn = 7;
        mChrInfo[i].mRow = 1;
        #if REGION_PAL || REGION_JPN
        mChrInfo[i].mMojiSet = MOJI_HIRA;
        #else
        mChrInfo[i].mMojiSet = MOJI_EIGO;
        #endif
        mChrInfo[i].field_0x3 = 1;
        #if REGION_JPN
        // '　' (full-width space)
        mChrInfo[i].mCharacter = '\x81\x40';
        #else
        mChrInfo[i].mCharacter = ' ';
        #endif
    }

    mIsInputEnd = false;
}

void dName_c::NameStrSet() {
    char* moji = mNextNameStr;

    int i = 0;
    while (*moji != 0) {
        #if REGION_PAL
        mChrInfo[i].mCharacter = static_cast<u8>(*moji);

        for (int j = 0; j < 65; j++) {
            if (mChrInfo[i].mCharacter == *(u8*)l_mojiEisuPal_1[j] ||
                mChrInfo[i].mCharacter == *(u16*)l_mojiEisuPal_2[j])
            {
                mChrInfo[i].mColumn = j / 5;
                mChrInfo[i].mRow = j % 5;
                mChrInfo[i].mMojiSet = MOJI_HIRA;
                break;
            }
        }
        moji++;
        i++;
        #else
        if (*(u8*)moji >> 4 == 8 || *(u8*)moji >> 4 == 9) {
            mChrInfo[i].mCharacter = *(u16*)moji;

            for (int j = 0; j < 65; j++) {
                if (mChrInfo[i].mCharacter == *(u16*)l_mojiHira[j] ||
                    mChrInfo[i].mCharacter == *(u16*)l_mojiHira2[j] ||
                    mChrInfo[i].mCharacter == *(u16*)l_mojiHira3[j])
                {
                    mChrInfo[i].mColumn = j / 5;
                    mChrInfo[i].mRow = j % 5;
                    mChrInfo[i].mMojiSet = MOJI_HIRA;
                    break;
                } else if (mChrInfo[i].mCharacter == *(u16*)l_mojikata[j] ||
                           mChrInfo[i].mCharacter == *(u16*)l_mojikata2[j] ||
                           mChrInfo[i].mCharacter == *(u16*)l_mojikata3[j])
                {
                    mChrInfo[i].mColumn = j / 5;
                    mChrInfo[i].mRow = j % 5;
                    mChrInfo[i].mMojiSet = MOJI_KATA;
                    break;
                }
            }
            moji += 2;
            i++;
        } else {
            mChrInfo[i].mCharacter = *moji;

            for (int j = 0; j < 65; j++) {
                if (mChrInfo[i].mCharacter == *(u8*)l_mojiEisu[j]) {
                    mChrInfo[i].mColumn = j / 5;
                    mChrInfo[i].mRow = j % 5;
                    mChrInfo[i].mMojiSet = MOJI_EIGO;
                    break;
                }
            }
            moji++;
            i++;
        }
        #endif
    }

    mLastCurPos = mCurPos;
    mCurPos = i;
    setNameText();
    nameCursorMove();
}

s32 dName_c::getMenuPosIdx(u8 selPos) {
    s32 result;
    switch (selPos) {
    case 0:
        result = 0;
        break;
    case 1:
        result = 1;
        break;
    case 2:
        #if REGION_PAL
        result = 3;
        #else
        result = 2;
        #endif
        break;
    #if !REGION_PAL
    case 3:
        result = 3;
        break;
    #endif
    }
    return result;
    //!@bug UB: uninitialized default return
}

void dDlst_NameIN_c::draw() {
    if (field_0xc != NULL) {
        Mtx m;
        MtxP global_mtx = (MtxP)&field_0xc->getGlbMtx()[0][0];  // fake match?

        MTXScale(m, (field_0xc->getWidth() / field_0x10->getWidth()),
                 (field_0xc->getHeight() / field_0x10->getHeight()), 1.0f);
        MTXConcat(global_mtx, m, global_mtx);
        field_0x10->setMtx(global_mtx);
    }

    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    NameInScr->draw(0.0f, 0.0f, graf_ctx);
}
