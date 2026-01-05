#ifndef D_METER_D_METER2_INFO_H
#define D_METER_D_METER2_INFO_H

#include "SSystem/SComponent/c_xyz.h"

class CPaneMgr;
class J2DTextBox;
struct JMSMesgEntry_c;
class JUTFont;

struct dMenu_LetterData {
    u16 mSubject;
    u16 mName;
    u16 mText;
    u16 mEventFlag;
};

struct dMenu_Letter {
    static u16 getLetterSubject(int idx) { return letter_data[idx].mSubject; }
    static u16 getLetterName(int idx) { return letter_data[idx].mName; }
    static u16 getLetterText(int idx) { return letter_data[idx].mText; }
    static u16 getLetterEventFlag(int idx) { return letter_data[idx].mEventFlag; }

    static dMenu_LetterData letter_data[64];
};

class dMw_c;
class dMeter2_c;
class J2DPicture;
class dMeterMap_c;

enum dMeter2Info_warpStatus {
    /* 0x3 */ WARP_STATUS_DECIDED_e = 3,
};

class dMeter2Info_c {
public:
    class WarpInfo_c {
    public:
        /* 0x00 */ char mStageName[8];
        /* 0x08 */ cXyz mPosition;
        /* 0x14 */ s16 mAngle;
        /* 0x16 */ u8 mRoomNo;
        /* 0x17 */ u8 unk_0x17;
        /* 0x18 */ u8 mPlayerNo;
    };

    dMeter2Info_c();
    virtual ~dMeter2Info_c();

    void init();
    int setFloatingMessage(u16 i_msgID, s16 i_msgTimer, bool i_wakuVisible);
    int setFloatingFlow(u16 i_flowID, s16 i_msgTimer, bool i_wakuVisible);
    bool isFloatingMessageVisible();
    s16 decFloatingMessageTimer();
    void resetFloatingMessage();
    void decMsgKeyWaitTimer();
    void getString(u32 i_stringID, char* o_string, JMSMesgEntry_c* i_msgEntry);
    void getStringKana(u32 i_stringID, char* o_string, JMSMesgEntry_c* i_msgEntry);
    void getStringKanji(u32 i_stringID, char* o_string, JMSMesgEntry_c* i_msgEntry);
    f32 getStringLength(J2DTextBox* i_textbox, char* i_string);
    f32 getStringLength(JUTFont* i_font, f32 param_2, f32 param_3, char* i_string);
    void onDirectUseItem(int);
    BOOL isDirectUseItem(int);
    int setMeterString(s32 i_string);
    void resetWarpStatus();
    void warpInProc();
    void warpOutProc();
    void resetMeterString();
    void setWarpInfo(const char* i_stageName, const cXyz& i_position, s16 i_angle,
                     u8 i_roomNo, u8 param_4, u8 i_warpPlayerNo);
    u8 getItemType(u8 i_itemNo);
    int readItemTexture(u8 i_itemNo, void* i_texBuf1, J2DPicture* i_pic1, void* i_texBuf2,
                        J2DPicture* i_pic2, void* i_texBuf3, J2DPicture* i_pic3,
                        void* i_texBuf4, J2DPicture* i_pic4, int param_9);
    void setItemColor(u8 i_itemNo, J2DPicture* i_pic1, J2DPicture* i_pic2,
                      J2DPicture* i_pic3, J2DPicture* i_pic4);
    s16 get2ndTexture(u8 i_itemType);
    s16 get3rdTexture(u8 i_itemType);
    s16 get4thTexture(u8 i_itemType);
    void set1stColor(u8 i_itemType, J2DPicture* i_pic);
    void set2ndColor(u8 i_itemType, J2DPicture* i_pic);
    void set3rdColor(u8 i_itemType, J2DPicture* i_pic);
    void set4thColor(u8 i_itemType, J2DPicture* i_pic);
    void setHotSpringTimer(u8 i_slotNo);
    void decHotSpringTimer();
    void changeWater(u8 i_slotNo);
    void setMiniGameItem(u8 i_minigameFlag);
    void resetMiniGameItem(bool i_saveItem);
    void setMiniGameCount(s8 i_count);
    void setSaveStageName(const char* i_stageName);

    void setWindowStatus(u8 i_status) {
        unk_0xba = mWindowStatus;
        mWindowStatus = i_status;
    }

    u8& getDirectUseItem() { return mDirectUseItem; }
    dMeterMap_c* getMeterMapClass() { return mMeterMap; }
    void setMenuWindowClass(dMw_c* i_menu) { mMenuWindowClass = i_menu; }
    dMw_c* getMenuWindowClass() { return mMenuWindowClass; }
    CPaneMgr* getMeterItemPanePtr(s32 i_idx) { return mMeterItemPanePtr[i_idx]; }
    void offUseButton(int i_buttonBit) { mUseButton &= ~(u16)i_buttonBit; }
    u16 getOilGaugeBackUp() { return mOilGaugeBackUp; }
    void setOilGaugeBackUp(u16 arg0) { mOilGaugeBackUp = arg0; }
    u8 getInsectSelectType() { return mInsectSelectType; }
    void setInsectSelectType(u8 i_type) { mInsectSelectType = i_type; }
    u8 getWarpStatus() { return mWarpStatus; }
    void setWarpStatus(u8 i_status) { mWarpStatus = i_status; }
    u8 getWarpRoomNo() { return mWarpInfo.mRoomNo; }
    u8 getWarpPlayerNo() { return mWarpInfo.mPlayerNo; }
    cXyz& getWarpPos() { return mWarpInfo.mPosition; }
    const char* getWarpStageName() { return mWarpInfo.mStageName; }
    void setNowCount(u8 i_count) { mNowCount = i_count; }
    void setMaxCount(u8 i_count) { mMaxCount = i_count; }
    void allUseButton() { mUseButton = 0xFFFF; }
    bool isUseButton(int i_buttonBit) { return i_buttonBit & mUseButton; }
    void setMeterMapClass(dMeterMap_c* i_map) { mMeterMap = i_map; }
    void resetGameStatus() { mGameStatus = 0; }
    void onGameStatus(int i_status) { mGameStatus |= (u16)i_status; }
    void setMapStatus(u8 i_status) { mMapStatus = i_status; }
    u8 getMapStatus() { return mMapStatus; }
    void setPauseStatus(u8 i_status) { mPauseStatus = i_status; }
    void resetPauseStatus() { mPauseStatus = 0; }
    u8 getPauseStatus() { return mPauseStatus; }
    bool isGameStatus(int i_status) { return (mGameStatus & (u16)i_status) != 0 ? true : false; }
    bool isTouchKeyCheck(int i_status) { return mTouchKeyCheck & (u16)(1 << i_status) ? true : false; }

    void setMapKeyDirection(u16 i_direction) { mMapKeyDirection = i_direction; }
    bool isSub2DStatus(int i_flag) { return (mSub2DStatus & (u16)(1 << i_flag)) != 0 ? true : false; }
    void offMenuInForce(int i_flag) { unk_0x98 &= ~(1 << i_flag); }
    bool isMenuInForce(int i_flag) { return unk_0x98 & (1 << i_flag); }
    u16 getMapKeyDirection() { return mMapKeyDirection; }
    u8 getWindowStatus() { return mWindowStatus; }
    void setMsgResource(void* i_res) { mMsgResource = i_res; }
    void setStageMsgResource(void* i_res) { mStageMsgResource = i_res; }
    void setMsgUnitResource(void* i_res) { mMsgUnitResource = i_res; }
    void* getMsgResource() { return mMsgResource; }
    void* getStageMsgResource() { return mStageMsgResource; }
    void* getMsgUnitResource() { return mMsgUnitResource; }
    u8 getLightDropGetFlag(int i_no) { return mLightDropGetFlag[i_no]; }
    u32 getMeterStringType() { return mMeterString; }
    void setMeterClass(dMeter2_c* i_meter) { mMeterClass = i_meter; }
    void onLifeGaugeSE() { mLifeGaugeSE = 1; }
    void offLifeGaugeSE() { mLifeGaugeSE = 0; }
    u8 getLifeGaugeSE() { return mLifeGaugeSE; }
    const char* getSaveStageName() { return mSaveStageName; }
    void onShopTalkFlag() { mShopTalkFlag = true; }
    void offShopTalkFlag() { mShopTalkFlag = false; }
    void setLightDropGetFlag(int i_no, u8 i_flag) { mLightDropGetFlag[i_no] = i_flag; }
    u8 getRentalBombBag() { return mRentalBombBagIdx; }
    void setTableMapRegionNo(u8 i_regionNo) { mTableMapRegionNo = i_regionNo; }
    void setGoldWolfMapType(u8 i_mapType) { mGoldWolfMapType = i_mapType; }
    dMeter2_c* getMeterClass() { return mMeterClass; }
    u8 getMiniGameItemSetFlag() { return mMiniGameItemSetFlag; }
    s16 getMsgKeyWaitTimer() { return mMsgKeyWaitTimer; }
    u8 getGameOverType() { return mGameOverType; }
    void setGameOverType(u8 i_gameoverType) { mGameOverType = i_gameoverType; }
    void setMsgKeyWaitTimer(s16 i_waitTimer) { mMsgKeyWaitTimer = i_waitTimer; }
    s32 getMsgTimeMs() { return mMsgTimeMs; }
    void setMsgTimeMs(s32 i_msgTime) { mMsgTimeMs = i_msgTime; }
    void setTimeMs(s32 i_time) { mTimeMs = i_time; }
    s32 getTimeMs() { return mTimeMs; }
    u8 getNowCount() { return mNowCount; }
    int getMaxCount() { return mMaxCount; }
    void setScopeZoomPointer(u8 param_0) { mScopeZoomPointer = param_0; }
    u8 getItemExplainWindowStatus() { return mItemExplainWindowStatus; }
    void setItemExplainWindowStatus(u8 i_value) { mItemExplainWindowStatus = i_value; }
    void resetDirectUseItem() { mDirectUseItem = 0; }
    u16 getFloatingFlowID() { return mFloatingFlowID; }
    bool isFloatingMessageWakuVisible() { return mFloatingMessageWakuVisible; }
    void onBlinkButton(int i_flag) { mBlinkButton |= i_flag; }
    bool isBlinkButton(int i_flag) { return (mBlinkButton & (u16)i_flag) != 0 ? true : false; }
    void resetBlinkButton() { mBlinkButton = 0; }
    s16 getFloatingMessageTimer() { return mFloatingMessageTimer; }
    u16 getFloatingMessageID() { return mFloatingMessageID; }
    s16 getHorseLifeCount() { return mHorseLifeCount; }
    void setMeterItemPanePtr(int i_no, CPaneMgr* i_pane) { mMeterItemPanePtr[i_no] = i_pane; }
    bool isShopTalkFlag() { return mShopTalkFlag != false; }
    u8 getCollectCursorPosX() { return mCollectCursorPosX; }
    u8 getCollectCursorPosY() { return mCollectCursorPosY; }
    void setCollectCursorPosXY(u8 x, u8 y) {
        mCollectCursorPosX = x;
        mCollectCursorPosY = y;
    }
    void setMapDrugFlag(bool i_flag) { mMapDrugFlag = i_flag; }
    void offTempBit(int i_bit) { mTempBits &= ~(1 << i_bit); }
    void onTempBit(int i_bit) { mTempBits |= (1 << i_bit); }
    bool isTempBit(int i_bit) { return mTempBits & (1 << i_bit); }
    void offSub2DStatus(int i_bit) { mSub2DStatus &= ~(1 << i_bit); }
    void onSub2DStatus(int i_bit) { mSub2DStatus |= 1 << i_bit; }
    void set2DWidth(f32 i_width) { m2DWidth = i_width; }
    void set2DHeight(f32 i_height) { m2DHeight = i_height; }
    void set2DPosH(f32 i_posH) { m2DPosH = i_posH; }
    void set2DPosV(f32 i_posV) { m2DPosV = i_posV; }
    void onUseButton(int i_button) { mUseButton |= i_button; }
    f32 get2DWidth() { return m2DWidth; }
    f32 get2DHeight() { return m2DHeight; }
    u8 getTableMapRegionNo() { return mTableMapRegionNo; }
    u8 getGoldWolfMapType() { return mGoldWolfMapType; }
    bool isWindowAccept(int param_0) { return mWindowAccept & (1 << param_0); }
    void setHorseLifeCount(s16 i_count) { mHorseLifeCount = i_count; }
    void resetTouchKeyCheck() { mTouchKeyCheck = 0; }
    void resetWindowAccept() { mWindowAccept = 0xFFFF; }
    void onWindowAccept(int param_0) { mWindowAccept |= (u16)(1 << param_0); }
    void offWindowAccept(int param_0) { mWindowAccept &= ~(u16)(1 << param_0); }
    void onMenuInForce(int param_0) { unk_0x98 |= (u16)(1 << param_0); }

public:
    /* 0x04 */ u8 unk_0x4[4];
    /* 0x08 */ u64 unk_0x8;
    /* 0x10 */ void* mMsgResource;
    /* 0x14 */ void* mStageMsgResource;
    /* 0x18 */ void* mMsgUnitResource;
    /* 0x1C */ dMeter2_c* mMeterClass;
    /* 0x20 */ dMeterMap_c* mMeterMap;
    /* 0x24 */ dMw_c* mMenuWindowClass;
    /* 0x28 */ CPaneMgr* mMeterItemPanePtr[4];
    /* 0x38 */ WarpInfo_c mWarpInfo;
    /* 0x54 */ f32 unk_0x54;
    /* 0x58 */ f32 unk_0x58;
    /* 0x5C */ f32 unk_0x5c;
    /* 0x60 */ f32 unk_0x60;
    /* 0x64 */ f32 unk_0x64;
    /* 0x68 */ f32 unk_0x68;
    /* 0x6C */ f32 unk_0x6c;
    /* 0x70 */ f32 m2DWidth;
    /* 0x74 */ f32 m2DHeight;
    /* 0x78 */ f32 m2DPosH;
    /* 0x7C */ f32 m2DPosV;
    /* 0x80 */ f32 unk_0x80;
    /* 0x84 */ s32 mTimeMs;
    /* 0x88 */ s32 mMsgTimeMs;
    /* 0x8C */ s32 mMeterString;
    /* 0x90 */ u32 mTempBits;
    /* 0x94 */ s16 mMsgKeyWaitTimer;
    /* 0x96 */ s16 mHorseLifeCount;
    /* 0x98 */ u16 unk_0x98;
    /* 0x9A */ u16 mHotSpringTimer[4];
    /* 0xA2 */ u16 mSub2DStatus;
    /* 0xA4 */ u16 mFloatingFlowID;
    /* 0xA6 */ u16 mFloatingMessageID;
    /* 0xA8 */ s16 mFloatingMessageTimer;
    /* 0xAA */ u16 mMapKeyDirection;
    /* 0xAC */ u16 mBlinkButton;
    /* 0xAE */ u16 mUseButton;
    /* 0xB0 */ u16 mGameStatus;
    /* 0xB2 */ u16 mTouchKeyCheck;
    /* 0xB4 */ u16 mWindowAccept;
    /* 0xB6 */ u16 mOilGaugeBackUp;
    /* 0xB8 */ u8 mDirectUseItem;
    /* 0xB9 */ u8 mWindowStatus;  // 0: normal play, 2: item wheel, 3: pause menu, 4: map, 10: pause menu submenus, 11: map in dungeon
    /* 0xBA */ u8 unk_0xba;
    /* 0xBB */ u8 mMaxCount;
    /* 0xBC */ u8 mNowCount;
    /* 0xBD */ bool mShopTalkFlag;
    /* 0xBE */ u8 unk_0xbe;
    /* 0xBF */ u8 mMapStatus;
    /* 0xC0 */ u8 mWarpStatus;
    /* 0xC1 */ u8 mPauseStatus;
    /* 0xC2 */ u8 mGameOverType;
    /* 0xC3 */ u8 mInsectSelectType;
    /* 0xC4 */ u8 mSaveSelItemIdx[4];
    /* 0xC8 */ u8 mSaveMixItemIdx[4];
    /* 0xCC */ u8 mSaveBombNum;
    /* 0xCD */ u8 mSaveArrowNum;
    /* 0xCE */ u8 mSaveBowItem;
    /* 0xCF */ u8 mSaveBombItem;
    /* 0xD0 */ u8 mSaveSelItemIdxMG[4];
    /* 0xD4 */ u8 mSaveMixItemIdxMG[4];
    /* 0xD8 */ u8 mSaveBombNumMG;
    /* 0xD9 */ u8 mSaveArrowNumMG;
    /* 0xDA */ u8 mSaveBowItemMG;
    /* 0xDB */ u8 mSaveBombItemMG;
    /* 0xDC */ u8 mRentalBombBagIdx;
    /* 0xDD */ u8 mMiniGameItemSetFlag;  // 1: rented in game, 3: rented not in game
    /* 0xDE */ s8 mMiniGameCount;
    /* 0xDF */ u8 mCollectCursorPosX;
    /* 0xE0 */ u8 mCollectCursorPosY;
    /* 0xE1 */ u8 mTableMapRegionNo;
    /* 0xE2 */ u8 mGoldWolfMapType;
    /* 0xE3 */ u8 mLightDropGetFlag[3];
    /* 0xE6 */ u8 mLifeGaugeSE;
    /* 0xE7 */ u8 mScopeZoomPointer;
    /* 0xE8 */ u8 mItemExplainWindowStatus;
    /* 0xE9 */ char mSaveStageName[8];
    /* 0xF1 */ bool mFloatingMessageWakuVisible;
    /* 0xF2 */ bool mMapDrugFlag;
    /* 0xF3 */ u8 unk_0xf3[5];
};

extern dMeter2Info_c g_meter2_info;

void dMeter2Info_setSword(u8 i_itemId, bool i_offItemBit);
void dMeter2Info_setCloth(u8 i_clothId, bool i_offItemBit);
void dMeter2Info_setShield(u8 i_itemId, bool i_offItemBit);
void dMeter2Info_set2DVibration();
void dMeter2Info_set2DVibrationM();
void dMeter2Info_offUseButton(int pButton);
bool dMeter2Info_is2DActiveTouchArea();
u8 dMeter2Info_getRecieveLetterNum();
u8 dMeter2Info_getPixel(f32 i_posX, f32 i_posY, f32 param_2, f32 param_3, f32 i_sizeX,
                          f32 i_sizeY, struct ResTIMG const* i_resTimg);
const char* dMeter2Info_getPlusTextureName();
const char* dMeter2Info_getNumberTextureName(int i_num);
int dMeter2Info_recieveLetter();
u8 dMeter2Info_getNewLetterNum();
int dMeter2Info_setNewLetterSender();
bool dMeter2Info_isItemOpenCheck();
bool dMeter2Info_isMapOpenCheck();
s16 dMeter2Info_getNowLifeGauge();

#if WIDESCREEN_SUPPORT
void dMeter2Info_onWide2D();
void dMeter2Info_offWide2D();
#endif

inline void dMeter2Info_Initialize() {
    g_meter2_info.init();
}

inline dMeterMap_c* dMeter2Info_getMeterMapClass() {
    return g_meter2_info.getMeterMapClass();
}

inline dMw_c* dMeter2Info_getMenuWindowClass() {
    return g_meter2_info.getMenuWindowClass();
}

inline void dMeter2Info_setWindowStatus(u8 i_status) {
    g_meter2_info.setWindowStatus(i_status);
}

inline CPaneMgr* dMeter2Info_getMeterItemPanePtr(s32 i_idx) {
    return g_meter2_info.getMeterItemPanePtr(i_idx);
}

inline void dMeter2Info_getString(u32 i_stringID, char* o_string, JMSMesgEntry_c* i_msgEntry) {
    g_meter2_info.getString(i_stringID, o_string, i_msgEntry);
}

inline void dMeter2Info_getStringKanji(u32 i_stringID, char* o_string, JMSMesgEntry_c* i_msgEntry) {
    g_meter2_info.getStringKanji(i_stringID, o_string, i_msgEntry);
}

inline void dMeter2Info_getStringKana(u32 i_stringID, char* o_string, JMSMesgEntry_c* i_msgEntry) {
    g_meter2_info.getStringKana(i_stringID, o_string, i_msgEntry);
}

inline f32 dMeter2Info_getStringLength(JUTFont* i_font, f32 param_2, f32 param_3, char* i_string) {
    return g_meter2_info.getStringLength(i_font, param_2, param_3, i_string);
}

inline f32 dMeter2Info_getStringLength(J2DTextBox* i_textbox, char* i_string) {
    return g_meter2_info.getStringLength(i_textbox, i_string);
}

inline void dMeter2Info_setHotSpringTimer(u8 i_slotNo) {
    g_meter2_info.setHotSpringTimer(i_slotNo);
}

inline u16 dMeter2Info_getOilGaugeBackUp() {
    return g_meter2_info.getOilGaugeBackUp();
}

inline void dMeter2Info_setOilGaugeBackUp(u16 param_1) {
    g_meter2_info.setOilGaugeBackUp(param_1);
}

inline void dMeter2Info_setSaveStageName(const char* i_stageName) {
    g_meter2_info.setSaveStageName(i_stageName);
}

inline u8 dMeter2Info_getInsectSelectType() {
    return g_meter2_info.getInsectSelectType();
}

inline void dMeter2Info_setInsectSelectType(u8 i_type) {
    g_meter2_info.setInsectSelectType(i_type);
}

inline u8 dMeter2Info_getWarpStatus() {
    return g_meter2_info.getWarpStatus();
}

inline void dMeter2Info_setWarpStatus(u8 i_status) {
    return g_meter2_info.setWarpStatus(i_status);
}

inline int dMeter2Info_readItemTexture(u8 i_itemNo, void* i_texBuf1, J2DPicture* i_pic1, void* i_texBuf2,
                                       J2DPicture* i_pic2, void* i_texBuf3, J2DPicture* i_pic3,
                                       void* i_texBuf4, J2DPicture* i_pic4, int param_9) {
    return g_meter2_info.readItemTexture(i_itemNo, i_texBuf1, i_pic1, i_texBuf2, i_pic2, i_texBuf3,
                                         i_pic3, i_texBuf4, i_pic4, param_9);
}

inline void dMeter2Info_setItemColor(u8 i_itemNo, J2DPicture* i_pic1, J2DPicture* i_pic2,
                                     J2DPicture* i_pic3, J2DPicture* i_pic4) {
    g_meter2_info.setItemColor(i_itemNo, i_pic1, i_pic2, i_pic3, i_pic4);
}

inline u8 dMeter2Info_getWarpRoomNo() {
    return g_meter2_info.getWarpRoomNo();
}

inline u8 dMeter2Info_getWarpPlayerNo() {
    return g_meter2_info.getWarpPlayerNo();
}

inline cXyz& dMeter2Info_getWarpPos() {
    return g_meter2_info.getWarpPos();
}

inline const char* dMeter2Info_getWarpStageName() {
    return g_meter2_info.getWarpStageName();
}

inline void dMeter2Info_setNowCount(u8 i_count) {
    g_meter2_info.setNowCount(i_count);
}

inline void dMeter2Info_setMaxCount(u8 i_count) {
    g_meter2_info.setMaxCount(i_count);
}

inline BOOL dMeter2Info_isDirectUseItem(int param_0) {
    return g_meter2_info.isDirectUseItem(param_0);
}

inline bool dMeter2Info_isUseButton(int i_buttonBit) {
    return g_meter2_info.isUseButton(i_buttonBit);
}

inline void dMeter2Info_setMeterMapClass(dMeterMap_c* i_map) {
    g_meter2_info.setMeterMapClass(i_map);
}

inline void dMeter2Info_decHotSpringTimer() {
    g_meter2_info.decHotSpringTimer();
}

inline void dMeter2Info_allUseButton() {
    g_meter2_info.allUseButton();
}

inline void dMeter2Info_offUseButton(int i_buttonBit) {
    g_meter2_info.offUseButton(i_buttonBit);
}

inline void dMeter2Info_resetGameStatus() {
    g_meter2_info.resetGameStatus();
}

inline void dMeter2Info_onGameStatus(int i_status) {
    g_meter2_info.onGameStatus(i_status);
}

inline void dMeter2Info_setMapStatus(u8 i_status) {
    g_meter2_info.setMapStatus(i_status);
}

inline u8 dMeter2Info_getMapStatus() {
    return g_meter2_info.getMapStatus();
}

inline void dMeter2Info_setPauseStatus(u8 i_status) {
    g_meter2_info.setPauseStatus(i_status);
}

inline void dMeter2Info_resetPauseStatus() {
    g_meter2_info.resetPauseStatus();
}

inline u8 dMeter2Info_getPauseStatus() {
    return g_meter2_info.getPauseStatus();
}

inline bool dMeter2Info_isGameStatus(int i_status) {
    return g_meter2_info.isGameStatus(i_status);
}

inline bool dMeter2Info_isTouchKeyCheck(int i_status) {
    return g_meter2_info.isTouchKeyCheck(i_status);
}

inline void dMeter2Info_setMapKeyDirection(u16 i_direction) {
    g_meter2_info.setMapKeyDirection(i_direction);
}

inline bool dMeter2Info_isSub2DStatus(int i_flag) {
    return g_meter2_info.isSub2DStatus(i_flag);
}

inline void dMeter2Info_offMenuInForce(int i_flag) {
    g_meter2_info.offMenuInForce(i_flag);
}

inline bool dMeter2Info_isMenuInForce(int i_flag) {
    return g_meter2_info.isMenuInForce(i_flag);
}

inline void dMeter2Info_setMenuWindowClass(dMw_c* i_menu) {
    g_meter2_info.setMenuWindowClass(i_menu);
}

inline void dMeter2Info_resetWarpStatus() {
    g_meter2_info.resetWarpStatus();
}

inline u16 dMeter2Info_getMapKeyDirection() {
    return g_meter2_info.getMapKeyDirection();
}

inline u8 dMeter2Info_getWindowStatus() {
    return g_meter2_info.getWindowStatus();
}

inline void dMeter2Info_setMsgResource(void* i_res) {
    g_meter2_info.setMsgResource(i_res);
}

inline void dMeter2Info_setStageMsgResource(void* i_res) {
    g_meter2_info.setStageMsgResource(i_res);
}

inline void dMeter2Info_setMsgUnitResource(void* i_res) {
    g_meter2_info.setMsgUnitResource(i_res);
}

inline void* dMeter2Info_getMsgResource() {
    return g_meter2_info.getMsgResource();
}

inline void* dMeter2Info_getStageMsgResource() {
    return g_meter2_info.getStageMsgResource();
}

inline void* dMeter2Info_getMsgUnitResource() {
    return g_meter2_info.getMsgUnitResource();
}

inline u8 dMeter2Info_getLightDropGetFlag(int i_no) {
    return g_meter2_info.getLightDropGetFlag(i_no);
}

inline s32 dMeter2Info_getMeterStringType() {
    return g_meter2_info.getMeterStringType();
}

inline void dMeter2Info_setMeterClass(dMeter2_c* i_meter) {
    g_meter2_info.setMeterClass(i_meter);
}

inline void dMeter2Info_onLifeGaugeSE() {
    g_meter2_info.onLifeGaugeSE();
}

inline void dMeter2Info_offLifeGaugeSE() {
    g_meter2_info.offLifeGaugeSE();
}

inline u8 dMeter2Info_getLifeGaugeSE() {
    return g_meter2_info.getLifeGaugeSE();
}

inline const char* dMeter2Info_getSaveStageName() {
    return g_meter2_info.getSaveStageName();
}

inline void dMeter2Info_onShopTalkFlag() {
    g_meter2_info.onShopTalkFlag();
}

inline void dMeter2Info_setLightDropGetFlag(int i_no, u8 i_flag) {
    g_meter2_info.setLightDropGetFlag(i_no, i_flag);
}

inline u8 dMeter2Info_getRentalBombBag() {
    return g_meter2_info.getRentalBombBag();
}

inline u8 dMeter2Info_getMiniGameItemSetFlag() {
    return g_meter2_info.getMiniGameItemSetFlag();
}

inline void dMeter2Info_setMiniGameItem(u8 i_minigameFlag) {
    g_meter2_info.setMiniGameItem(i_minigameFlag);
}

inline void dMeter2Info_resetMiniGameItem(bool i_saveItem) {
    g_meter2_info.resetMiniGameItem(i_saveItem);
}

inline void dMeter2Info_setTableMapRegionNo(u8 i_regionNo) {
    g_meter2_info.setTableMapRegionNo(i_regionNo);
}

inline void dMeter2Info_setGoldWolfMapType(u8 i_mapType) {
    g_meter2_info.setGoldWolfMapType(i_mapType);
}

inline void dMeter2Info_changeWater(u8 i_slotNo) {
    g_meter2_info.changeWater(i_slotNo);
}

inline void dMeter2Info_warpInProc() {
    g_meter2_info.warpInProc();
}

inline void dMeter2Info_warpOutProc() {
    g_meter2_info.warpOutProc();
}

inline dMeter2_c* dMeter2Info_getMeterClass() {
    return g_meter2_info.getMeterClass();
}

inline s16 dMeter2Info_getMsgKeyWaitTimer() {
    return g_meter2_info.getMsgKeyWaitTimer();
}

inline void dMeter2Info_onDirectUseItem(int param_0) {
    g_meter2_info.onDirectUseItem(param_0);
}

inline void dMeter2Info_setFloatingFlow(u16 i_flowID, s16 i_msgTimer, bool i_wakuVisible) {
    g_meter2_info.setFloatingFlow(i_flowID, i_msgTimer, i_wakuVisible);
}

inline u8 dMeter2Info_getGameOverType() {
    return g_meter2_info.getGameOverType();
}

inline void dMeter2Info_setGameOverType(u8 i_gameoverType) {
    g_meter2_info.setGameOverType(i_gameoverType);
}

inline void dMeter2Info_setMsgKeyWaitTimer(s16 i_waitTimer) {
    g_meter2_info.setMsgKeyWaitTimer(i_waitTimer);
}

inline s32 dMeter2Info_getMsgTimeMs() {
    return g_meter2_info.getMsgTimeMs();
}

inline s32 dMeter2Info_getTimeMs() {
    return g_meter2_info.getTimeMs();
}

inline void dMeter2Info_setMsgTimeMs(s32 i_msgTime) {
    g_meter2_info.setMsgTimeMs(i_msgTime);
}

inline void dMeter2Info_setTimeMs(s32 i_time) {
    g_meter2_info.setTimeMs(i_time);
}

inline u8 dMeter2Info_getNowCount() {
    return g_meter2_info.getNowCount();
}

inline int dMeter2Info_getMaxCount() {
    return g_meter2_info.getMaxCount();
}

inline void dMeter2Info_setScopeZoomPointer(u8 param_0) {
    g_meter2_info.setScopeZoomPointer(param_0);
}

inline void dMeter2Info_decMsgKeyWaitTimer() {
    g_meter2_info.decMsgKeyWaitTimer();
}

inline bool dMeter2Info_isFloatingMessageVisible() {
    return g_meter2_info.isFloatingMessageVisible();
}

inline u8 dMeter2Info_getItemExplainWindowStatus() {
    return g_meter2_info.getItemExplainWindowStatus();
}

inline void dMeter2Info_setItemExplainWindowStatus(u8 i_value) {
    g_meter2_info.setItemExplainWindowStatus(i_value);
}

inline void dMeter2Info_resetDirectUseItem() {
    g_meter2_info.resetDirectUseItem();
}

inline u16 dMeter2Info_getFloatingFlowID() {
    return g_meter2_info.getFloatingFlowID();
}

inline u8 dMeter2Info_getCollectCursorPosX() {
    return g_meter2_info.getCollectCursorPosX();
}

inline u8 dMeter2Info_getCollectCursorPosY() {
    return g_meter2_info.getCollectCursorPosY();
}

inline void dMeter2Info_setCollectCursorPosXY(u8 x, u8 y) {
    g_meter2_info.setCollectCursorPosXY(x, y);
}

inline void dMeter2Info_onBlinkButton(int i_flag) {
    g_meter2_info.onBlinkButton(i_flag);
}

inline bool dMeter2Info_isFloatingMessageWakuVisible() {
    return g_meter2_info.isFloatingMessageWakuVisible();
}

inline bool dMeter2Info_isBlinkButton(int i_flag) {
    return g_meter2_info.isBlinkButton(i_flag);
}

inline void dMeter2Info_resetBlinkButton() {
    g_meter2_info.resetBlinkButton();
}

inline void dMeter2Info_resetFloatingMessage() {
    g_meter2_info.resetFloatingMessage();
}

inline s16 dMeter2Info_decFloatingMessageTimer() {
    return g_meter2_info.decFloatingMessageTimer();
}

inline s16 dMeter2Info_getFloatingMessageTimer() {
    return g_meter2_info.getFloatingMessageTimer();
}

inline u16 dMeter2Info_getFloatingMessageID() {
    return g_meter2_info.getFloatingMessageID();
}

inline s16 dMeter2Info_getHorseLifeCount() {
    return g_meter2_info.getHorseLifeCount();
}

inline void dMeter2Info_setMeterString(s32 i_string) {
    g_meter2_info.setMeterString(i_string);
}

inline void dMeter2Info_resetMeterString() {
    g_meter2_info.resetMeterString();
}

inline void dMeter2Info_setMeterItemPanePtr(int i_no, CPaneMgr* i_pane) {
    g_meter2_info.setMeterItemPanePtr(i_no, i_pane);
}

inline bool dMeter2Info_isShopTalkFlag() {
    return g_meter2_info.isShopTalkFlag();
}

inline void dMeter2Info_setMapDrugFlag(bool i_flag) {
    g_meter2_info.setMapDrugFlag(i_flag);
}

inline bool dMeter2Info_isTempBit(int i_bit) {
    return g_meter2_info.isTempBit(i_bit);
}

inline void dMeter2Info_offSub2DStatus(int i_bit) {
    g_meter2_info.offSub2DStatus(i_bit);
}

inline void dMeter2Info_onSub2DStatus(int i_bit) {
    g_meter2_info.onSub2DStatus(i_bit);
}

inline void dMeter2Info_set2DWidth(f32 i_width) {
    g_meter2_info.set2DWidth(i_width);
}

inline void dMeter2Info_set2DHeight(f32 i_height) {
    g_meter2_info.set2DHeight(i_height);
}

inline void dMeter2Info_set2DPosH(f32 i_posH) {
    g_meter2_info.set2DPosH(i_posH);
}

inline void dMeter2Info_set2DPosV(f32 i_posV) {
    g_meter2_info.set2DPosV(i_posV);
}

inline void dMeter2Info_offShopTalkFlag() {
    g_meter2_info.offShopTalkFlag();
}

inline void dMeter2Info_onUseButton(int i_button) {
    g_meter2_info.onUseButton(i_button);
}

inline f32 dMeter2Info_get2DWidth() {
    return g_meter2_info.get2DWidth();
}

inline f32 dMeter2Info_get2DHeight() {
    return g_meter2_info.get2DHeight();
}

inline void dMeter2Info_setWarpInfo(const char* i_stageName, const cXyz& i_position, s16 i_angle,
                                    u8 i_roomNo, u8 param_4, u8 i_warpPlayerNo) {
    g_meter2_info.setWarpInfo(i_stageName, i_position, i_angle, i_roomNo, param_4, i_warpPlayerNo);
}

inline u8 dMeter2Info_getTableMapRegionNo() {
    return g_meter2_info.getTableMapRegionNo();
}

inline u8 dMeter2Info_getGoldWolfMapType() {
    return g_meter2_info.getGoldWolfMapType();
}

inline bool dMeter2Info_isWindowAccept(int param_0) {
    return g_meter2_info.isWindowAccept(param_0);
}

inline void dMeter2Info_setHorseLifeCount(s16 i_count) {
    g_meter2_info.setHorseLifeCount(i_count);
}

inline void dMeter2Info_offTempBit(int i_bit) {
    g_meter2_info.offTempBit(i_bit);
}

inline void dMeter2Info_onTempBit(int i_bit) {
    g_meter2_info.onTempBit(i_bit);
}

inline bool dMeter2Info_isNextStage(const char*, s16, s16, s16) {
    return false;
}

inline void dMeter2Info_setFloatingMessage(u16 i_msgID, s16 i_msgTimer, bool i_wakuVisible) {
    g_meter2_info.setFloatingMessage(i_msgID, i_msgTimer, i_wakuVisible);
}

inline void dMeter2Info_setMiniGameCount(s8 i_count) {
    g_meter2_info.setMiniGameCount(i_count);
}

inline void dMeter2Info_onWindowAccept(int param_0) {
    g_meter2_info.onWindowAccept(param_0);
}

inline void dMeter2Info_offWindowAccept(int param_0) {
    g_meter2_info.offWindowAccept(param_0);
}

inline void dMeter2Info_onMenuInForce(int param_0) {
    g_meter2_info.onMenuInForce(param_0);
}

#endif /* D_METER_D_METER2_INFO_H */
