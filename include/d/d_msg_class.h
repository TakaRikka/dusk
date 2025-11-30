#ifndef D_MSG_D_MSG_CLASS_H
#define D_MSG_D_MSG_CLASS_H

#include "JSystem/JMessage/control.h"
#include "JSystem/JMessage/JMessage.h"
#include "SSystem/SComponent/c_xyz.h"

#if REGION_JPN
#define D_MSG_CLASS_PAGE_CNT_MAX 30
#define D_MSG_CLASS_CHAR_CNT_MAX 0x210
#define D_MSG_CLASS_LINE_MAX 9
#else
#define D_MSG_CLASS_PAGE_CNT_MAX 40
#define D_MSG_CLASS_CHAR_CNT_MAX 0x200
#define D_MSG_CLASS_LINE_MAX 12
#endif

class JMSMesgEntry_c {
public:
    /* 0x0 */ u32 string_offset;

    // Attributes
    /* 0x04 */ u16 message_id;
    /* 0x06 */ u16 event_label_id;
    /* 0x08 */ u8 se_speaker;
    /* 0x09 */ u8 fuki_kind;
    /* 0x0A */ u8 output_type;
    /* 0x0B */ u8 fuki_pos_type;
    /* 0x0C */ u8 unk_0xc;
    /* 0x0D */ u8 unk_0xd;
    /* 0x0E */ u8 se_mood;
    /* 0x0F */ u8 camera_id;
    /* 0x10 */ u8 base_anm_id;
    /* 0x11 */ u8 face_anm_id;
    /* 0x12 */ u16 unk_0x12;
};

class JMSMesgInfo_c {
public:
    /* 0x00 */ bmg_section_t header;  // section header
    /* 0x08 */ u16 entry_num;         // number of entries in this section
    /* 0x0A */ u16 entry_size;        // size of an entry
    /* 0x0C */ u8 padding[4];         // padding
    /* 0x10 */ JMSMesgEntry_c entries[0];
};

class COutFont_c;
class J2DTextBox;
class JUTFont;

class STControl;
class dMsgObject_c;
struct jmessage_tReference : public JMessage::TReference {
    jmessage_tReference();
    void calcDistance();
    u8 getLineMax();
    bool isKanban();
    bool isPlaceName();
    bool isBossName();
    bool isSaveSeq();
    bool isBook();
    bool isStaffRoll();
    bool isHowl();
    bool isMidona();
    void resetReference();
    void pageSend();
    void selectMessage();
    void inputNumber();
    char* getWord(int);
    void resetWord();
    void setCharactor(u16);
    void addCharactor(u16);
    void resetCharactor();
    void shiftCharCountBuffer();
    void resetCharCountBuffer();
    void allsetCharCountBuffer();
    bool isCharSend();
    bool isLightSend();
    bool isLightEnd();
    void decideOutFontRupeeColor(int);
    cXyz getActorPos() { return mActorPos; }

    void setActorPos(cXyz pos) { mActorPos = pos; }

    bool isSelectSetCancelFlag() {
        return mSelectSetCancelFlag ? true : false;
    }

    BOOL isSelectRubyFlag(int i_flag) {
        BOOL var_r31;
        if (mSelectRubyFlag & (u8)(1 << i_flag)) {
            var_r31 = true;
        } else {
            var_r31 = false;
        }
        return var_r31;
    }

    bool isBatchFlag() { return mBatchFlag ? true : false; }
    bool isLightBatchFlag() { return mLightBatchFlag ? true : false; }

    void setAddCharAllAlphaRate(f32 i_alphaRate) { mAddCharAllAlphaRate = i_alphaRate; }
    void setCharAllAlphaRate(f32 i_alphaRate) { mCharAllAlphaRate = i_alphaRate; }
    void resetNowLightCount() { mNowLightCount = 0; }
    void resetDrawLightCount() { mDrawLightCount = 0; }
    void setBatchColorFlag(u8 i_flag) { mBatchColorFlag = i_flag; }
    void setSendTimer(s16 i_timer) { mSendTimer = i_timer; }
    void setSendFlag(u8 i_flag) { mSendFlag = i_flag; }
    void setSelectNum(u8 i_num) { mSelectNum = i_num; }
    void setSelectType(u8 i_type) { mSelectType = i_type; }
    void resetInputFigure() { mInputFigure = 0; }
    void onButtonTagStopFlag() { mButtonTagStopFlag = 1; }
    void setDemoFrame(u32 i_frame) { mDemoFrame = i_frame; }
    void setTopColorType(u8 i_colorType) { mTopColorType = i_colorType; }
    void setNowColorType(u8 i_colorType) { mNowColorType = i_colorType; }
    void setTopTagScale(u16 i_tagScale) { mTopTagScale = i_tagScale; }
    void setNowTagScale(u16 i_tagScale) { mNowTagScale = i_tagScale; }
    void setRevoMessageID(u32 i_msgID) { mRevoMessageID = i_msgID; }
    void setPageLine(int i_no, s8 i_pageLine) { mPageLine[i_no] = i_pageLine; }
    void setPageLineMax(int i_no, s8 i_pageLineMax) { mPageLineMax[i_no] = i_pageLineMax; }
    void setPageType(int i_no, u8 i_pageType) { mPageType[i_no] = i_pageType; }
    void setLineArrange(int i_no, u8 i_arrange) { mLineArrange[i_no] = i_arrange; }
    void setLineScale(int i_no, u16 i_lineScale) { mLineScale[i_no] = i_lineScale; }
    void setSelLength(int i_no, f32 i_selLength) { mSelLength[i_no] = i_selLength; }
    void setTopWordCount(s8 wordCount) { mTopWordCount = wordCount; }
    void setNowWordCount(s8 wordCount) { mNowWordCount = wordCount; }
    void offSelectRubyFlag(int i_flag) { mSelectRubyFlag &= ~(u8)(1 << i_flag); }
    void setPageEndCount(s16 i_endCount) { mPageEndCount = i_endCount; }
    void onBombNameUseFlag() { mBombNameUseFlag = 1; }
    void onSelectRubyFlag(int i_flag) { mSelectRubyFlag |= (u8)(1 << i_flag); }
    void setpStatus(u16* status) { mpStatus = status; }
    void setObjectPtr(dMsgObject_c* ptr) { mpObjectPtr = ptr; }
    void setCountBackUp() { mCharactor.mCountBackUp = mCharactor.field_0x40e; }
    void setSelMsgPtr(char* ptr) { mpSelMsgPtr = ptr; }
    void setPageNum(s16 pageNum) { mPageNum = pageNum; }
    void setStopFlag(u8 flag) { mStopFlag = flag; }
    void setStatus(u16 status) { *mpStatus = status; }
    void setFont(JUTFont* font) { mpFont = font; }
    void setFontSizeX(f32 x) { mFontSizeX = x; }
    void setFontSizeY(f32 y) { mFontSizeY = y; }
    void setRubySize(f32 size) { mRubySize = size; }
    void setTBoxWidth(f32 width) { mTBoxWidth = width; }
    void setTBoxHeight(f32 height) { mTBoxHeight = height; }
    void setLineSpace(f32 space) { mLineSpace = space; }
    void setCharSpace(f32 space) { mCharSpace = space; }
    void setRubyCharSpace(f32 space) { mRubyCharSpace = space; }
    void setSelFontSize(f32 size) { mSelFontSize = size; }
    void setSelRubySize(f32 size) { mSelRubySize = size; }
    void setSelTBoxWidth(f32 width) { mSelTBoxWidth = width; }
    void setSelCharSpace(f32 space) { mSelCharSpace = space; }
    void setSelRubyCharSpace(f32 space) { mSelRubyCharSpace = space; }
    void decSendTimer() { mSendTimer--; }
    void resetCharCnt() { mCharCnt = 0; }
    void setFukiPosType(u8 type) { mFukiPosType = type; }
    void setFukiKind(u8 kind) { mFukiKind = kind; }
    void setForm(u8 form) { mForm = form; }
    void setArrange(u8 arrange) { mArrange = arrange; }
    void setMsgID(u16 id) { mMsgID = id; }
    void offBombNameUseFlag() { mBombNameUseFlag = false; }
    void setNowLightCount(u8 count) { mNowLightCount = count; }
    void setCharCnt(s16 cnt) { mCharCnt = cnt; }
    void offButtonTagStopFlag() { mButtonTagStopFlag = false; }
    void onLightBatchFlag() { mLightBatchFlag = true; }
    void addNowLightCount() { mNowLightCount++; }
    void addLineCount() { mLineCount++; }
    void addEndLineCount() { mEndLineCount++; }
    void onSelectSetCancelFlag() { mSelectSetCancelFlag = true; }
    void offSelectSetCancelFlag() { mSelectSetCancelFlag = false; }
    void addDrawLightCount() { mDrawLightCount++; }
    void setCharAlpha(f32 alpha) { mCharAlpha = alpha; }

    void addCharAlpha() {
        mCharAlpha += mAddCharAlpha;
        if (mCharAlpha > 255.0f) {
            mCharAlpha = 255.0f;
        }
    }

    void setLineLength(int i_no, f32 i_strLen, f32 i_spaceLen) {
        mStrLength[i_no] = i_strLen;
        mSpaceLength[i_no] = i_spaceLen;
    }

    void setSelectPos(u8 i_pos) {
        if (!isSelectSetCancelFlag()) {
            mSelectPos = i_pos;
        }
    }

    void addCharAllAlphaRate() {
        mCharAllAlphaRate += mAddCharAllAlphaRate;
        if (mCharAllAlphaRate > 1.0f) {
            mCharAllAlphaRate = 1.0f;
        }
    }

    void addLineLength(int param_0, f32 param_1, f32 param_2) {
        mStrLength[param_0] += param_1;
        mSpaceLength[param_0] += param_2;
    }

    void addSelLength(int param_0, f32 param_1) {
        mSelLength[param_0] += param_1;
    }

    dMsgObject_c* getObjectPtr() { return mpObjectPtr; }
    u8 getForm() { return mForm; }
    u8 getNowLightCount() { return mNowLightCount; }
    u8 getCharCountBuffer(int i_no) { return mCharCountBuffer[i_no]; }
    u8 getDrawLightCount() { return mDrawLightCount; }
    u8 getFukiKind() { return mFukiKind; }
    s16 getSendTimer() { return mSendTimer; }
    u8 getSelectNum() { return mSelectNum; }
    u8 getSelectType() { return mSelectType; }
    const char* getSelMsgPtr() { return mpSelMsgPtr; }
    f32 getDistanceScale() { return mDistanceScale; }
    f32 getFontSizeX() { return mFontSizeX; }
    f32 getFontSizeY() { return mFontSizeY; }
    s16 getStartLineCount() { return mStartLineCount; }
    f32 getStrLength(int param_0) { return mStrLength[param_0]; }
    f32 getSpaceLength(int param_0) { return mSpaceLength[param_0]; }
    f32 getCharSpace() { return mCharSpace; }
    u8 getPageType(int i_no) { return mPageType[i_no]; }
    u8 getInputFigure() { return mInputFigure; }
    JUTFont* getFont() { return mpFont; }
    f32 getSelRubySize() { return mSelRubySize; }
    f32 getSelRubyCharSpace() { return mSelRubyCharSpace; }
    f32 getRubySize() { return mRubySize; }
    f32 getRubyCharSpace() { return mRubyCharSpace; }
    char* getSelTextPtr(int idx) { return mSelText[idx]; }
    char* getSelRubyPtr(int idx) { return mSelRuby[idx]; }
    char* getTextPtr() { return mText; }
    char* getTextSPtr() { return mTextS; }
    char* getRubyPtr() { return mRuby; }
    u8 getSelectRubyFlag() { return mSelectRubyFlag; }
    f32 getSelTBoxWidth() { return mSelTBoxWidth; }
    u8 getSelectPos() { return mSelectPos; }
    u16 getMsgID() { return mMsgID; }
    bool isButtonTagStopFlag() { return mButtonTagStopFlag ? true : false; }
    u8 getStopFlag() { return mStopFlag; }
    u8 getSendFlag() { return mSendFlag; }
    u8 getFukiPosType() { return mFukiPosType; }
    u16 getStatus() { return *mpStatus; }
    u8 getArrange() { return mArrange; }
    u8 getLineArrange(int idx) { return mLineArrange[idx]; }
    f32 getSelFontSize() { return mSelFontSize; }
    f32 getSelCharSpace() { return mSelCharSpace; }
    u16 getLineScale(int i_no) { return mLineScale[i_no]; }
    bool isBombNameUseFlag() { return mBombNameUseFlag ? true : false; }
    f32 getSelLength(int idx) { return mSelLength[idx]; }
    void setAddCharAlpha(f32 alpha) { mAddCharAlpha = alpha; }
    s8 getNowPageLineMax() { return mPageLineMax[field_0x5d2]; }
    s16 getLineCount() { return mLineCount; }
    void onBatchFlag() { mBatchFlag = true; }
    s16 getPageNum() { return mPageNum; }
    s16 getPageEndCount() { return mPageEndCount; }
    s8 getTopWordCount() { return mTopWordCount; }
    u8 getTopColorType() { return mTopColorType; }
    u16 getTopTagScale() { return mTopTagScale; }
    u8 getBatchColorFlag() { return mBatchColorFlag; }
    f32 getLineSpace() { return mLineSpace; }
    u8 getNowPageType() { return mPageType[field_0x5d2]; }
    f32 getTBoxWidth() { return mTBoxWidth;}
    f32 getTBoxHeight() { return mTBoxHeight;}
    s8 getNowPageLine() { return mPageLine[field_0x5d2]; }
    u8 getOutFontRupeeColor() { return mOutFontRupeeColor; }
    f32 getCharAlpha() { return mCharAlpha; }
    s16 getCharCnt() { return mCharCnt; }

    struct CharSoundInfo {
        u16 data[D_MSG_CLASS_CHAR_CNT_MAX];
        s16 field_0x40c;
        s16 field_0x40e;
        s16 mCountBackUp;
    };
    CharSoundInfo getCharSoundInfo() { return mCharactor; }
    u32 getDemoFrame() { return mDemoFrame; }
    u32 getRevoMessageID() { return mRevoMessageID; }
    f32 getCharAllAlphaRate() { return mCharAllAlphaRate; }

    virtual ~jmessage_tReference();

    /* 0x0008 */ STControl* mpStick;
    /* 0x000C */ CharSoundInfo mCharactor;
    /* 0x0414 */ cXyz mActorPos;
    /* 0x0420 */ f32 mFontSizeX;
    /* 0x0424 */ f32 mFontSizeY;
    /* 0x0428 */ f32 mRubySize;
    /* 0x042C */ f32 mTBoxWidth;
    /* 0x0430 */ f32 mTBoxHeight;
    /* 0x0434 */ f32 mLineSpace;
    /* 0x0438 */ f32 mCharSpace;
    /* 0x043C */ f32 mRubyCharSpace;
    /* 0x0440 */ f32 mSelFontSize;
    /* 0x0444 */ f32 mSelRubySize;
    /* 0x0448 */ f32 mSelTBoxWidth;
    /* 0x044C */ f32 mSelCharSpace;
    /* 0x0450 */ f32 mSelRubyCharSpace;
    /* 0x0454 */ f32 mDistanceScale;
    /* 0x0458 */ f32 mAddCharAllAlphaRate;
    /* 0x045C */ f32 mCharAllAlphaRate;
    /* 0x0460 */ f32 mAddCharAlpha;
    /* 0x0464 */ f32 mCharAlpha;
    /* 0x0468 */ f32 mStrLength[D_MSG_CLASS_PAGE_CNT_MAX];
    /* 0x0508 */ f32 mSpaceLength[D_MSG_CLASS_PAGE_CNT_MAX];
    /* 0x05A8 */ f32 mSelLength[3];
    /* 0x05B4 */ u32 mDemoFrame;
    /* 0x05B8 */ u32 mRevoMessageID;
    /* 0x05BC */ u16* mpStatus;
    /* 0x05C0 */ dMsgObject_c* mpObjectPtr;
    /* 0x05C4 */ char* mpSelMsgPtr;
    /* 0x05C8 */ JUTFont* mpFont;
    /* 0x05CC */ s16 mStartLineCount;
    /* 0x05CE */ s16 mEndLineCount;
    /* 0x05D0 */ s16 mLineCount;
    /* 0x05D2 */ s16 field_0x5d2;
    /* 0x05D4 */ s16 mPageEndCount;
    /* 0x05D6 */ s16 mSendTimer;
    /* 0x05D8 */ s16 mCharCnt;
    /* 0x05DA */ s16 mPageNum;
    /* 0x05DC */ u8 mNowLightCount;
    /* 0x05DD */ u8 mDrawLightCount;
    /* 0x05DE */ u16 mMsgID;
    /* 0x05E0 */ u16 mLineScale[D_MSG_CLASS_PAGE_CNT_MAX];
    /* 0x0630 */ u16 mTopTagScale;
    /* 0x0632 */ u16 mNowTagScale;
    /* 0x0634 */ char mWord[10][100];
    /* 0x0A1C */ char mText[D_MSG_CLASS_CHAR_CNT_MAX];
    /* 0x0C1C */ char mTextS[D_MSG_CLASS_CHAR_CNT_MAX];
    /* 0x0E1C */ char mRuby[D_MSG_CLASS_CHAR_CNT_MAX];
    /* 0x101C */ char mSelText[3][50];
    /* 0x10B2 */ char mSelRuby[3][80];
    /* 0x11A2 */ s8 mPageLine[D_MSG_CLASS_PAGE_CNT_MAX];
    /* 0x11CA */ s8 mPageLineMax[D_MSG_CLASS_PAGE_CNT_MAX];
    /* 0x11F2 */ u8 mPageType[D_MSG_CLASS_PAGE_CNT_MAX];
    /* 0x121A */ u8 mLineArrange[D_MSG_CLASS_PAGE_CNT_MAX];
    /* 0x1242 */ u8 mSelectNum;
    /* 0x1243 */ u8 mSelectType;
    /* 0x1244 */ u8 mSelectPos;
    /* 0x1245 */ u8 mFukiPosType;
    /* 0x1246 */ u8 mFukiKind;
    /* 0x1247 */ u8 mForm;
    /* 0x1248 */ u8 mArrange;
    /* 0x1249 */ u8 mNowColorType;
    /* 0x124A */ u8 mTopColorType;
    /* 0x124B */ bool mButtonTagStopFlag;
    /* 0x124C */ u8 mSendFlag;
    /* 0x124D */ u8 mStopFlag;
    /* 0x124E */ u8 mSelectRubyFlag;
    /* 0x124F */ u8 mInputFigure;
    /* 0x1250 */ u8 mOutFontRupeeColor;
    /* 0x1251 */ s8 mNowWordCount;
    /* 0x1252 */ s8 mTopWordCount;
    /* 0x1253 */ u8 mCharCountBuffer[31];
    /* 0x1272 */ bool mLightBatchFlag;
    /* 0x1273 */ bool mBatchFlag;
    /* 0x1274 */ bool mSelectSetCancelFlag;
    /* 0x1275 */ bool mBombNameUseFlag;
    /* 0x1276 */ u8 mBatchColorFlag;
};  // Size: 0x1278

struct jmessage_tMeasureProcessor : public JMessage::TRenderingProcessor {
    jmessage_tMeasureProcessor(jmessage_tReference const* pReference);
    void do_scale(f32);
    void do_space(u32);
    void do_pageType(int);
    void do_name1();
    void do_rubyset(void const*, u32);
    void push_word(char*);

    virtual void do_begin(void const* pEntry, char const* pszText);
    virtual void do_end();
    virtual void do_character(int iCharacter);
    virtual bool do_tag(u32 uTag, void const* pData, u32 uSize);

    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ s16 mTotalLineCnt;
    /* 0x3E */ s16 field_0x3e;
    /* 0x40 */ s16 field_0x40;
    /* 0x42 */ u16 mSeMood;
    /* 0x44 */ u8 field_0x44;
    /* 0x45 */ u8 mSelectType;
    /* 0x46 */ u8 field_0x46;
    /* 0x47 */ u8 field_0x47;
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ s8 field_0x49;
    /* 0x4A */ s8 mPageLineMax;
    /* 0x4B */ s8 field_0x4b;
    /* 0x4C */ u8 mSeSpeaker;
    /* 0x4D */ u8 field_0x4d;
};  // Size: 0x50

struct jmessage_tControl : public JMessage::TControl {
    jmessage_tControl();

    virtual ~jmessage_tControl();
};

struct jmessage_tSequenceProcessor : public JMessage::TSequenceProcessor {
    jmessage_tSequenceProcessor(jmessage_tReference const* pReference, jmessage_tControl* pControl);
    void do_name1();
    void do_space(u32);
    void do_rubyset(void const*, u32);
    void push_word();
    void messageSePlay(u8, u8, cXyz*);
    void calcStringLength();

    virtual void do_reset();
    virtual void do_begin(void const* pEntry, char const* pszText);
    virtual void do_end();
    virtual void do_character(int iCharacter);
    virtual bool do_tag(u32 uTag, void const* pData, u32 uSize);
    virtual bool do_isReady();
    virtual bool do_jump_isReady();
    virtual void do_jump(void const* pEntry, char const* pszText);

    u8 getMouthCheck() { return mMouthCheck; }
    void setForceForm(u8 forceForm) { mForceForm = forceForm; }

    /* 0x4C */ jmessage_tMeasureProcessor mMeasureProcessor;
    /* 0x9C */ const void* mpEntry;
    /* 0xA0 */ const char* mpText;
    /* 0xA4 */ s16 field_0xa4;
    /* 0xA6 */ s16 field_0xa6;
    /* 0xA8 */ s16 field_0xa8;
    /* 0xAA */ s16 field_0xaa;
    /* 0xAC */ u8 field_0xac;
    /* 0xAD */ u8 field_0xad;
    /* 0xAE */ u8 field_0xae;
    /* 0xAF */ u8 mForceForm;
    /* 0xB0 */ u8 mMouthCheck;
    /* 0xB1 */ u8 field_0xb1;
    /* 0xB2 */ u8 field_0xb2;
    /* 0xB3 */ u8 field_0xb3;
    /* 0xB4 */ u8 field_0xb4;
    /* 0xB5 */ s8 field_0xb5;
};

struct CharInfo_c {
    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ u8 mColorNo;
    /* 0x19 */ u8 field_0x19;
};

struct jmessage_tRenderingProcessor : public JMessage::TRenderingProcessor {
    jmessage_tRenderingProcessor(jmessage_tReference const* pReference);
    void resetRendering();
    void do_widthcenter();
    void do_selwidthcenter(int);
    void do_heightcenter();
    void do_color(u8);
    void do_scale(f32);
    void do_linedown(s16);
    void do_transY(s16, bool);
    void do_outfont(u8, u32);
    void do_arrow2();
    f32 getLineLength(int);
    void do_strcat(char*, bool, bool, bool);
    void do_rubyset(void const*, u32);
    void do_rubystrcat(char*, char*, f32, f32);
    void do_name1();
    void do_numset(s16);
    void push_word();
    void getCharInfo(f32, f32, f32, f32, f32);

    virtual void do_reset();
    virtual void do_begin(void const* pEntry, char const* pszText);
    virtual void do_end();
    virtual void do_character(int iCharacter);
    virtual bool do_tag(u32 uTag, void const* pData, u32 uSize);

    void setTextInitPos(float x, float y) {
        mTextInitPosX = x;
        mTextInitPosY = y;
    }

    void setTextScale(float x, float y) {
        mTextInitScaleX = x;
        mTextInitScaleY = y;
    }

    void setSelTextInitPos(int idx, float x, float y) {
        mSelTextInitPosX[idx] = x;
        mSelTextInitPosY[idx] = y;
    }

    void setTextInitOffsetPos(f32 offset) { mTextInitOffsetPos = offset; }
    void setOutFont(COutFont_c* font) { mpOutFont = font; }
    void setCharInfoPtr(CharInfo_c* info) { mCharInfoPtr = info; }

    /* 0x038 */ COutFont_c* mpOutFont;
    /* 0x03C */ CharInfo_c* mCharInfoPtr;
    /* 0x040 */ s16* mpCharInfoCnt;
    /* 0x044 */ f32 field_0x44;
    /* 0x048 */ f32 field_0x48;
    /* 0x04C */ f32 field_0x4c;
    /* 0x050 */ f32 mTextInitPosX;
    /* 0x054 */ f32 mTextInitPosY;
    /* 0x058 */ f32 mTextInitScaleX;
    /* 0x05C */ f32 mTextInitScaleY;
    /* 0x060 */ f32 mSelTextInitPosX[3];
    /* 0x06C */ f32 mSelTextInitPosY[3];
    /* 0x078 */ f32 mTextInitOffsetPos;
    /* 0x07C */ f32 field_0x7c[D_MSG_CLASS_PAGE_CNT_MAX];
    /* 0x11C */ int field_0x11c;
    /* 0x120 */ u32 mCCColor;
    /* 0x124 */ u32 mGCColor;
    /* 0x128 */ f32 field_0x128;
    /* 0x12C */ f32 field_0x12c;
    /* 0x130 */ f32 field_0x130;
    /* 0x134 */ f32 field_0x134;
    /* 0x138 */ f32 field_0x138;
    /* 0x13C */ u16 field_0x13c;
    /* 0x13E */ s16 field_0x13e;
    /* 0x140 */ s16 field_0x140;
    /* 0x142 */ s16 field_0x142;
    /* 0x144 */ u16 field_0x144;
    /* 0x146 */ s16 field_0x146;
    /* 0x148 */ u16 field_0x148;
    /* 0x14A */ u16 field_0x14a;
    /* 0x14C */ u8 field_0x14c;
    /* 0x14D */ u8 field_0x14d;
    /* 0x14E */ u8 field_0x14e;
    /* 0x14F */ s8 field_0x14f;
    /* 0x150 */ u8 mColorNo;
    /* 0x151 */ u8 field_0x151;
    /* 0x152 */ char field_0x152[0x184 - 0x152];
    /* 0x184 */ char field_0x184[3];
    /* 0x187 */ u8 field_0x187;
};

struct jmessage_string_tControl : public JMessage::TControl {
    jmessage_string_tControl();
};

struct jmessage_string_tReference : public JMessage::TReference {
    jmessage_string_tReference();
    void init(J2DTextBox*, J2DTextBox*, JUTFont*, COutFont_c*, u8);
    void setCharactor(u16);
    void addCharactor(u16);
    void resetCharactor();
    f32 getLineLength(int);
    void addLineLength(int, f32);
    f32 getOutfontLength(int);
    void setOutfontLength(int, f32);
    void clearOutfontLength(int);
    s16 getLineCountNowPage();
    J2DTextBox* getPanePtr() { return mPanePtr; }
    J2DTextBox* getRubyPanePtr() { return mRubyPanePtr; }
    u32 getCCColor() { return mCCColor; }
    u32 getGCColor() { return mGCColor; }
    u8 getLineMax() { return mLineMax; }
    u8 getNowPage() { return mNowPage; }
    JUTFont* getFont() { return mpFont; }
    s16 getLineCount() { return mLineCount; }
    void setLineCount(s16 lineCount) { mLineCount = lineCount; }
    void addLineCount() { mLineCount++; }
    BOOL isFlag(u8 flag) { 
        BOOL rv;
        if (mFlags & flag) {
            rv = 1;
        } else {
            rv = 0;
        }
        return rv;
    }
    void setColor(u32 ccColor, u32 gcColor) {
        mCCColor = ccColor;
        mGCColor = gcColor;
    }
    void setNowPage(u8 nowPage) { mNowPage = nowPage; }
    void setLineMax(u8 lineMax) { mLineMax = lineMax; }
    COutFont_c* getOutFontPtr() { return mOutFontPtr; }

    virtual ~jmessage_string_tReference();

    /* 0x08 */ J2DTextBox* mPanePtr;
    /* 0x0C */ J2DTextBox* mRubyPanePtr;
    /* 0x10 */ COutFont_c* mOutFontPtr;
    /* 0x14 */ JUTFont* mpFont;
    /* 0x18 */ f32 mLineLength[D_MSG_CLASS_LINE_MAX];
    /* 0x3C */ f32 mOutfontLength[D_MSG_CLASS_LINE_MAX];
    /* 0x78 */ u32 mCCColor;
    /* 0x7C */ u32 mGCColor;
    /* 0x80 */ s16 mLineCount;
    /* 0x82 */ u8 mLineMax;
    /* 0x83 */ u8 mNowPage;
    /* 0x84 */ u8 mFlags;
};

struct jmessage_string_tMeasureProcessor : public JMessage::TRenderingProcessor {
    jmessage_string_tMeasureProcessor(jmessage_string_tReference const* pReference);
    void do_rubyset(void const*, u32);

    virtual void do_begin(void const* pEntry, char const* pszText);
    virtual void do_end();
    virtual void do_character(int iCharacter);
    virtual bool do_tag(u32 uTag, void const* pData, u32 uSize);

    /* 0x38 */ jmessage_string_tReference* mpReference;
};

struct jmessage_string_tSequenceProcessor : public JMessage::TSequenceProcessor {
    jmessage_string_tSequenceProcessor(jmessage_string_tReference const* pReference,
                                                      jmessage_string_tControl* pControl);

    virtual void do_reset();
    virtual void do_begin(void const* pEntry, char const* pszText);
    virtual void do_end();
    virtual void do_character(int iCharacter);
    virtual bool do_tag(u32 uTag, void const* pData, u32 uSize);
    virtual bool do_isReady();
    virtual bool do_jump_isReady();
    virtual void do_jump(void const* pEntry, char const* pszText);
    
    /* 0x4C */ jmessage_string_tMeasureProcessor mMeasureProcessor;
    /* 0x88 */ jmessage_string_tReference* mpSeqReference;
};

struct jmessage_string_tRenderingProcessor : public JMessage::TRenderingProcessor {
    jmessage_string_tRenderingProcessor(jmessage_string_tReference const* pReference);
    s16 getLineCountNowPage();
    void do_widthcenter();
    void do_heightcenter();
    void do_strcat(char*);
    void do_rubyset(void const*, u32);
    void do_rubystrcat(char*);
    void do_outfont(u8);
    void do_color(u8);
    void do_scale(f32);
    void do_linedown(s16);
    void do_numset(s16);
    void push_word(char const*);

    virtual void do_reset();
    virtual void do_begin(void const* pEntry, char const* pszText);
    virtual void do_end();
    virtual void do_character(int iCharacter);
    virtual bool do_tag(u32 uTag, void const* pData, u32 uSize);

    char* getString() { return field_0x54; }

    /* 0x038 */ jmessage_string_tReference* mpReference;
    /* 0x03C */ f32 field_0x3c;
    /* 0x040 */ f32 field_0x40;
    /* 0x044 */ f32 field_0x44;
    /* 0x048 */ f32 field_0x48;
    /* 0x04C */ f32 field_0x4c;
    /* 0x050 */ f32 field_0x50;
    /* 0x054 */ char field_0x54[D_MSG_CLASS_CHAR_CNT_MAX];
    /* 0x254 */ char field_0x254[D_MSG_CLASS_CHAR_CNT_MAX];
    /* 0x454 */ char field_0x454[0x486 - 0x454];
    /* 0x486 */ char field_0x486[0x20]; // Unknown length
    /* 0x4A6 */ u8 field_0x4a6[0x54e - 0x4a6];
    /* 0x54E */ s16 field_0x54e;
    /* 0x550 */ s16 field_0x550;
    /* 0x552 */ s16 field_0x552;
    /* 0x554 */ u8 field_0x554;
};

#define MSGTAG_GROUP(g) (g << 16)

// Group 0
#define MSGTAG_PLAYER_NAME         0
#define MSGTAG_INSTANT             1
#define MSGTAG_TYPE                2
#define MSGTAG_UNK_3               3  // appears the same as autobox
#define MSGTAG_AUTOBOX             4
#define MSGTAG_BOXATMOST           5
#define MSGTAG_UNK_6               6
#define MSGTAG_PAUSE               7
#define MSGTAG_SELECT_2WAY         8
#define MSGTAG_SELECT_3WAY         9
#define MSGTAG_ABTN                10
#define MSGTAG_BBTN                11
#define MSGTAG_CSTICK              12
#define MSGTAG_LBTN                13
#define MSGTAG_RBTN                14
#define MSGTAG_XBTN                15
#define MSGTAG_YBTN                16
#define MSGTAG_ZBTN                17
#define MSGTAG_DPAD                18
#define MSGTAG_STICK_CROSS         19
#define MSGTAG_LEFT_ARROW          20
#define MSGTAG_RIGHT_ARROW         21
#define MSGTAG_UP_ARROW            22
#define MSGTAG_DOWN_ARROW          23
#define MSGTAG_STICK_UP            24
#define MSGTAG_STICK_DOWN          25
#define MSGTAG_STICK_LEFT          26
#define MSGTAG_STICK_RIGHT         27
#define MSGTAG_STICK_VERTICAL      28
#define MSGTAG_STICK_HORIZONTAL    29
#define MSGTAG_INLINE_2_NEXT       30
#define MSGTAG_INLINE_2_FIRST      31
#define MSGTAG_AWAIT_CHOICE        32
#define MSGTAG_UNK_33              33  // calls "do_name1" but the function does nothing
#define MSGTAG_HORSE_NAME          34
#define MSGTAG_RED_TARGET          35
#define MSGTAG_YELLOW_TARGET       36
#define MSGTAG_INPUT_VALUE         37
#define MSGTAG_ACKNOWLEDGE         38
#define MSGTAG_ABTN_STAR           39
#define MSGTAG_DEMOBOX             40
#define MSGTAG_SCENT_NAME          41
#define MSGTAG_WHITE_TARGET        42
#define MSGTAG_PORTAL_NAME         43
#define MSGTAG_WARP_ICON           44
#define MSGTAG_BOMB_NAME           45
#define MSGTAG_XYBTN               46
#define MSGTAG_YXBTN               47
#define MSGTAG_BOMB_BAG_ICON       48
#define MSGTAG_BOMB_NUM            49
#define MSGTAG_BOMB_PRICE          50
#define MSGTAG_INLINE_3_NEXT       51
#define MSGTAG_INLINE_3_FIRST      52
#define MSGTAG_UNK_53              53
#define MSGTAG_BOXATLEAST          54
#define MSGTAG_BOMB_MAX            55
#define MSGTAG_ARROW_MAX           56
#define MSGTAG_HEART               57
#define MSGTAG_QUAVER              58
#define MSGTAG_INSECT_NAME         59
#define MSGTAG_LETTER_NAME         60
#define MSGTAG_LINE_DOWN           61
#define MSGTAG_CURRENT_LETTER_PAGE 62
#define MSGTAG_MAX_LETTER_PAGE     63

// Group 3
#define MSGTAG_WII_MSGID_OVERRIDE  0
#define MSGTAG_WII_ABTN            1
#define MSGTAG_WII_BBTN            2
#define MSGTAG_WII_HOMEBTN         3
#define MSGTAG_WII_MINUSBTN        4
#define MSGTAG_WII_PLUSBTN         5
#define MSGTAG_WII_1BTN            6
#define MSGTAG_WII_2BTN            7
#define MSGTAG_WII_DPAD_ITEM       8
#define MSGTAG_WII_DPAD_UP         9
#define MSGTAG_WII_DPAD_DOWN       10
#define MSGTAG_WII_DPAD_HORIZONTAL 11
#define MSGTAG_WII_DPAD_RIGHT      12
#define MSGTAG_WII_DPAD_LEFT       13
#define MSGTAG_WII_WIIMOTE         14
#define MSGTAG_WII_RETICULE        15
#define MSGTAG_WII_NUNCHUK         16
#define MSGTAG_WII_WIIMOTE2        17
#define MSGTAG_WII_FAIRY           18
#define MSGTAG_WII_CBTN            19
#define MSGTAG_WII_ZBTN            20

// Group 4
#define MSGTAG_GLYPH_DOLLARSIGN 0
#define MSGTAG_GLYPH_BACKSLASH  1
#define MSGTAG_GLYPH_ATMARK     2
#define MSGTAG_GLYPH_SHARP      3
#define MSGTAG_GLYPH_FLAT       4
#define MSGTAG_GLYPH_SQRT       5
#define MSGTAG_GLYPH_PERCENT    6
#define MSGTAG_GLYPH_HECTARE    7
#define MSGTAG_GLYPH_ARE        8
#define MSGTAG_GLYPH_LITRE      9
#define MSGTAG_GLYPH_WATT       10
#define MSGTAG_GLYPH_CALORIE    11
#define MSGTAG_GLYPH_DOLLAR     12
#define MSGTAG_GLYPH_CENT       13

// Group 5
#define MSGTAG_TIME_INFO        0
#define MSGTAG_INSECT_INFO      3
#define MSGTAG_RIVER_POINTS     7
#define MSGTAG_FISH_LENGTH      8
#define MSGTAG_FUNDRAISE_REMAIN 9
#define MSGTAG_NEW_LETTER_NUM   10
#define MSGTAG_POE_NUM          11
#define MSGTAG_BALLOON_SCORE    12
#define MSGTAG_FISH_COUNT       13
#define MSGTAG_ROLLGOAL_LV      14

// Group 6
#define MSGTAG_PLAYER_GENITIV   0
#define MSGTAG_HORSE_GENITIV    1
#define MSGTAG_MALE_ICON        2
#define MSGTAG_FEMALE_ICON      3
#define MSGTAG_STAR_ICON        4
#define MSGTAG_REFMARK          5
#define MSGTAG_THIN_LEFT_ARROW  6
#define MSGTAG_THIN_RIGHT_ARROW 7
#define MSGTAG_THIN_UP_ARROW    8
#define MSGTAG_THIN_DOWN_ARROW  9
#define MSGTAG_BULLET           10
#define MSGTAG_BULLET_SPACE     11

// Group 255
#define MSGTAG_COLOR (MSGTAG_GROUP(255) | 0)
#define MSGTAG_SCALE (MSGTAG_GROUP(255) | 1)
#define MSGTAG_RUBY  (MSGTAG_GROUP(255) | 2)

#endif /* D_MSG_D_MSG_CLASS_H */
