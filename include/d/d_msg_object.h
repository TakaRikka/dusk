#ifndef D_MSG_D_MSG_OBJECT_H
#define D_MSG_D_MSG_OBJECT_H

#include "JSystem/JMessage/control.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_msg.h"

class COutFont_c;
class J2DTextBox;
class JUTFont;
class dMsgScrnBase_c;
class dMsgString_c;
struct jmessage_tReference;
struct jmessage_tRenderingProcessor;
struct jmessage_tSequenceProcessor;
struct jmessage_tControl;

class dMsgObject_c : public msg_class {
public:
    int _create(msg_class*);
    int _execute();
    int _draw();
    int _delete();
    void setMessageIndex(u32, u32, bool);
    void setMessageIndexDemo(u32, bool);
    u32 getMessageIndex(u32);
    u32 getRevoMessageIndex(u32);
    u32 getMessageIndexAlways(u32);
    u32 getMessageIDAlways(u32);
    s16 getMessageGroup(u32);
    void waitProc();
    void openProc();
    void outnowProc();
    void outwaitProc();
    void stopProc();
    void continueProc();
    void selectProc();
    void selectEndProc();
    void inputProc();
    void finishProc();
    void endProc();
    void deleteProc();
    void textmodeProc();
    void talkStartInit();
    void fukiPosCalc(bool);
    void setStatusLocal(u16);
    u16 getStatusLocal();
    void delete_screen(bool);
    u8 isSend();
    void readMessageGroupLocal(mDoDvdThd_mountXArchive_c**);
    void changeFlowGroupLocal(s32);
    void demoMessageGroupLocal();
    void endFlowGroupLocal();
    void changeGroupLocal(s16);
    bool getStringLocal(u32, J2DTextBox*, J2DTextBox*, JUTFont*, COutFont_c*, char*,
                                       char*, char*, s16*);
    bool isGetItemMessage();
    bool isKanbanMessage();
    bool isHowlMessage();
    bool isMidonaMessage();
    bool isMidonaNormalTalkWait();
    bool isMidonaTalkWait();
    bool isPlaceMessage();
    bool isBossMessage();
    bool isBookMessage();
    bool isStaffMessage();
    bool isSaveMessage();
    bool isTalkMessage();
    const char* getSmellName();
    const char* getPortalName();
    const char* getBombName();
    const char* getInsectName();
    const char* getLetterName();
    u8 getSelectBombNum();
    void onMsgSendControlLocal();
    void offMsgSendControlLocal();
    int isMsgSendControlLocal();
    void onMsgSendLocal();
    void offMsgSendLocal();
    int isMsgSendLocal();
    bool isMouthCheckLocal();
    int getBombBuyPriceLocal(u8);
    void addSelectBomBag(u8);
    void resetSelectBomBag();
    u8 getSelectBombBagIDLocal();
    s16 getSelectBombPriceLocal();
    void setEquipBombInfoLocal();
    void updateEquipBombInfoLocal();
    static void setShopWaitTimer(u8);
    void setSelectWordFlagLocal(u8);
    bool isHowlHearingModeLocal();
    static bool isCameraCancelFlag();
    static void onCameraCancelFlag();
    static void offCameraCancelFlag();
    static bool isKillMessageFlag();
    static void onKillMessageFlag();
    static void setKillMessageFlag();
    void setKillMessageFlagLocal();
    static void setTalkPartner(fopAc_ac_c*);
    static void setNowTalkFlowNo(s16);
    static s16 getNowTalkFlowNo();
    static void setDemoMessage(u32);
    static void setTalkHeap(void*);
    void setTalkHeapLocal(void*);
    static void* getTalkHeap();
    bool isDraw();
    void setButtonStatusLocal();
    void setDemoMessageLocal(u32);
    u16 getSmellTypeMessageIDLocal();
    void setSmellTypeLocal(u8);
    u8 getSelectCursorPosLocal();
    void setSelectCursorPosLocal(u8);
    void setTalkActorLocal(fopAc_ac_c*);
    static void readMessageGroup(mDoDvdThd_mountXArchive_c**);
    static void changeFlowGroup(s32);
    static void demoMessageGroup();
    static void endFlowGroup();
    static void changeGroup(s16);
    static bool getString(u32, J2DTextBox*, J2DTextBox*, JUTFont*, COutFont_c*,
                                         char*, char*, char*, s16*);
    static void* getMsgDtPtr();
    static void setProcessID(fpc_ProcID);
    static msg_class* getActor();
    static fopAc_ac_c* getpTalkActor();
    static int getIdx();
    static u16 getNodeIdx();
    static void setStatus(u16);
    static u16 getStatus();
    static dMsgScrnBase_c* getScrnDrawPtr();
    static void setTalkActor(fopAc_ac_c*);
    static void onMsgSendControl();
    static void offMsgSendControl();
    static int isMsgSendControl();
    static void onMsgSend();
    static void offMsgSend();
    static int isMsgSend();
    static bool isMouthCheck();
    static u32 getMessageID();
    static u16 getSmellTypeMessageID();
    static void setSmellType(u8);
    static u8 getSelectCursorPos();
    static void setSelectCursorPos(u8);
    static void setPortalMessageID(u16);
    static void setInsectItemNo(u8);
    static void setLetterNameID(u16);
    static void setArrowNum(u8);
    static void setMsgOutputType(u8);
    static u8 getMsgOutputType();
    static const char* getWord();
    static const char* getSelectWord(int);
    static void setSelectWordFlag(u8);
    static u8 getSelectWordFlag();
    static bool isHowlHearingMode();
    static u8 getSelectBombBagID();
    static s16 getSelectBombPrice();
    static void setEquipBombInfo();
    static u8 getItemEquipButton();
    static void setSelectCancelPos(u8);

    void setShopWaitTimerLocal(u8 timer) { mShopWaitTimer = timer; }
    void setSelectCancelPosLocal(u8 pos) { mSelectCancelPos = pos; }
    u8 getItemEquipButtonLocal() { return mItemEquipButton; }
    s16 getInputValueLocal() { return mInputValue; }
    void setInputValueLocal(s16 i_inputValue) { mInputValue = i_inputValue; }
    s16 getNowTotalPriceLocal() { return mNowTotalPrice; }
    void setNowTotalPriceLocal(s16 i_nowTotalPrice) { mNowTotalPrice = i_nowTotalPrice; }
    s16 getNowTotalPaymentLocal() { return mNowTotalPayment; }
    void setNowTotalPaymentLocal(s16 i_nowTotalPayment) { mNowTotalPayment = i_nowTotalPayment; }
    void onInputFlagLocal() { mInputFlag = 1; }
    void offInputFlagLocal() { mInputFlag = 0; }
    void offPaymentFlagLocal() { mPaymentFlag = false; }
    void offAutoMessageFlagLocal() { mAutoMessageFlag = 0; }
    void onAutoMessageFlagLocal() { mAutoMessageFlag = 1; }
    void setFundRaisingValueLocal(s16 i_value) { mFundRaisingValue = i_value; }
    u16 getPortalMessageIDLocal() { return mPortalMessageID; }
    bool isAutoMessageFlagLocal() { return mAutoMessageFlag; }
    bool isPaymentFlagLocal() { return mPaymentFlag; }
    void onPaymentFlagLocal() { mPaymentFlag = true; }
    s16 getNowTalkFlowNoLocal() { return mNowTalkFlowNo; }
    void setNowTalkFlowNoLocal(s16 param_0) { mNowTalkFlowNo = param_0; }
    void setTalkPartnerLocal(fopAc_ac_c* actor) { mpTalkPartner = actor; }
    void onCameraCancelFlagLocal() { mCameraCancelFlag = true; }
    void offCameraCancelFlagLocal() { mCameraCancelFlag = false; }
    void onKillMessageFlagLocal() { mKillMessageFlag = true; }
    void setProcessIDLocal(fpc_ProcID id) { mProcessID = id; }
    void setPortalMessageIDLocal(u16 msgID) { mPortalMessageID = msgID; }
    void setInsectItemNoLocal(u8 itemNo) { mInsectItemNo = itemNo; }
    void setLetterNameIDLocal(u16 nameID) { mLetterNameID = nameID; }
    void setArrowNumLocal(u8 arrowNum) { mArrowNum = arrowNum; }
    void setMsgOutputTypeLocal(u8 outputType) { mMsgOutputType = outputType; }
    void addNowTotalPrice(s16 price) { addNowTotalPriceLocal(price); }
    void addNowTotalPriceLocal(s16 price) { mNowTotalPrice += price; }
    void addNowTotalPayment(s16 price) { addNowTotalPaymentLocal(price); }
    void addNowTotalPaymentLocal(s16 price) { mNowTotalPayment += price; }
    void setBombBagIDLocal(int idx, u8 id) { mBombBagID[idx] = id; }
    void setBombMessageIDLocal(int idx, u16 id) { mBombMessageID[idx] = id; }
    void setBombNumLocal(int idx, u8 id) { mBombNum[idx] = id; }

    u8 getSelectPushFlag() { return mSelectPushFlag; }
    u8 getSelectCancelPos() { return mSelectCancelPos; }
    s16 getInputValue() { return getInputValueLocal(); }
    void setInputValue(s16 i_inputValue) { setInputValueLocal(i_inputValue); }
    s16 getNowTotalPrice() { return getNowTotalPriceLocal(); }
    void setNowTotalPrice(s16 i_nowTotalPrice) { setNowTotalPriceLocal(i_nowTotalPrice); }
    s16 getNowTotalPayment() { return getNowTotalPaymentLocal(); }
    void setNowTotalPayment(s16 i_nowTotalPayment) { setNowTotalPaymentLocal(i_nowTotalPayment); }
    void onInputFlag() { onInputFlagLocal(); }
    void offPaymentFlag() { offPaymentFlagLocal(); }
    void offAutoMessageFlag() { offAutoMessageFlagLocal(); }
    void onAutoMessageFlag() { onAutoMessageFlagLocal(); }
    void setFundRaisingValue(s16 i_value) { setFundRaisingValueLocal(i_value); }
    s16 getFundRaisingValue() { return getFundRaisingValueLocal(); }
    s16 getFundRaisingValueLocal() { return mFundRaisingValue; }
    u16 getPortalMessageID() { return getPortalMessageIDLocal(); }
    bool isAutoMessageFlag() { return isAutoMessageFlagLocal(); }
    bool isPaymentFlag() { return isPaymentFlagLocal(); }
    u8 getFukiKind() { return mFukiKind; }
    bool isKillMessageFlagLocal() { return mKillMessageFlag; }
    void offKillMessageFlagLocal() { mKillMessageFlag = false; }
    void* getTalkHeapLocal() { return mpTalkHeap; }
    u8 getSelectBomBag(int idx) { return getSelectBombBagLocal(idx); }
    u8 getSelectBombBagLocal(int idx) { return mSelectBombBag[idx]; }
    u8 getBombNumLocal(int idx) { return mBombNum[idx]; }
    bool isCameraCancelFlagLocal() { return mCameraCancelFlag; }
    void* getMsgDtPtrLocal() { return mpMsgDt; }
    msg_class* getActorLocal() { return field_0x100; }
    fopAc_ac_c* getpTalkActorLocal() { return mpTalkActor; }
    int getIdxLocal() { return mIdx; }
    u16 getNodeIdxLocal() { return mNodeIdx; }
    dMsgScrnBase_c* getScrnDrawPtrLocal() { return mpScrnDraw; }
    u32 getMessageIDLocal() { return mMessageID; }
    u8 getMsgOutputTypeLocal() { return mMsgOutputType; }
    const char* getWordLocal() { return mWord; }
    const char* getSelectWordLocal(int idx) { return mSelectWord[idx]; }
    u8 getSelectWordFlagLocal() { return mSelectWordFlag; }
    u16 getLetterNameIDLocal() { return mLetterNameID; }
    u16 getInsectItemNoLocal() { return mInsectItemNo; }
    u16 getBombMessageIDLocal(int idx) { return mBombMessageID[idx]; }
    u8 getBombBagIDLocal(int idx) { return mBombBagID[idx]; }
    u8 getArrowNumLocal() { return mArrowNum; }
    u8 isNoDemoFlag() { return mNoDemoFlag; }

    static void setWord(const char* i_word);
    void setWordLocal(const char* i_word) {
        strcpy(mWord, i_word);
    }

    static void setSelectWord(int i_no, const char* i_word);
    void setSelectWordLocal(int i_no, const char* i_word) {
        strcpy(mSelectWord[i_no], i_word);
    }

    jmessage_tSequenceProcessor* getSequenceProcessor() { return mpSeqProc; }

    /* 0x0FC */ int field_0xfc;
    /* 0x100 */ msg_class* field_0x100;
    /* 0x104 */ dMsgScrnBase_c* mpScrnDraw;
    /* 0x108 */ dMsgString_c* mpMsgString;
    /* 0x10C */ COutFont_c* mpOutFont;
    /* 0x110 */ JMessage::TResourceContainer* mpResCont;
    /* 0x114 */ jmessage_tControl* mpCtrl;
    /* 0x118 */ jmessage_tReference* mpRefer;
    /* 0x11C */ jmessage_tSequenceProcessor* mpSeqProc;
    /* 0x120 */ jmessage_tRenderingProcessor* mpRenProc;
    /* 0x124 */ JMessage::TParse* field_0x124;
    /* 0x128 */ void* mpMsgRes;
    /* 0x12C */ int field_0x12c;
    /* 0x130 */ void* mpMsgDt;
    /* 0x134 */ u8 field_0x134[4];
    /* 0x138 */ fopAc_ac_c* mpTalkActor;
    /* 0x13C */ fopAc_ac_c* field_0x13c;
    /* 0x140 */ fopAc_ac_c* mpTalkPartner;
    /* 0x144 */ JKRExpHeap* mpTalkHeap;
    /* 0x148 */ JKRHeap* field_0x148;
    /* 0x14C */ int mProcessID;
    /* 0x150 */ f32 field_0x150;
    /* 0x154 */ u32 mMessageID;
    /* 0x158 */ u32 field_0x158;
    /* 0x15C */ u32 field_0x15c;
    /* 0x160 */ int mIdx;
    /* 0x164 */ u16 mNodeIdx;
    /* 0x166 */ u16 field_0x166;
    /* 0x168 */ u16 field_0x168;
    /* 0x16A */ s16 field_0x16a;
    /* 0x16C */ s16 field_0x16c;
    /* 0x16E */ s16 field_0x16e;
    /* 0x170 */ s16 mNowTalkFlowNo;
    /* 0x172 */ s16 field_0x172;
    /* 0x174 */ s16 mInputValue;
    /* 0x176 */ s16 mFundRaisingValue;
    /* 0x178 */ u16 field_0x178;
    /* 0x17A */ s16 mNowTotalPrice;
    /* 0x17C */ s16 mNowTotalPayment;
    /* 0x17E */ s16 field_0x17e;
    /* 0x180 */ u16 mPortalMessageID;
    /* 0x182 */ u16 mBombMessageID[3];
    /* 0x188 */ u16 mLetterNameID;
    /* 0x18A */ u8 mBombNum[3];
    /* 0x18D */ u8 mBombBagID[3];
    /* 0x190 */ u8 mSelectBombBag[3];
    /* 0x193 */ u8 field_0x193;
    /* 0x194 */ u8 mInsectItemNo;
    /* 0x195 */ u8 mMsgSendControl;
    /* 0x196 */ u8 field_0x196;
    /* 0x197 */ u8 field_0x197;
    /* 0x198 */ u8 field_0x198;
    /* 0x199 */ u8 field_0x199;
    /* 0x19A */ u8 mFukiKind;
    /* 0x19B */ u8 field_0x19b;
    /* 0x19C */ u8 mFlowChk;
    /* 0x19D */ u8 field_0x19d;
    /* 0x19E */ bool mCameraCancelFlag;
    /* 0x19F */ u8 mInputFlag;
    /* 0x1A0 */ u8 mSmellType;
    /* 0x1A1 */ u8 mSelectCancelPos;
    /* 0x1A2 */ u8 mSelectPushFlag;
    /* 0x1A3 */ u8 field_0x1a3;
    /* 0x1A4 */ u8 mItemEquipButton;
    /* 0x1A5 */ u8 mArrowNum;
    /* 0x1A6 */ char mWord[200];
    /* 0x26E */ char mSelectWord[3][200];
    /* 0x4C6 */ u8 mSelectWordFlag;
    /* 0x4C7 */ u8 field_0x4c7;
    /* 0x4C8 */ u8 mMsgOutputType;
    /* 0x4C9 */ u8 mShopWaitTimer;
    /* 0x4CA */ u8 field_0x4ca;
    /* 0x4CB */ u8 mNoDemoFlag;
    /* 0x4CC */ u8 field_0x4cc;
    /* 0x4CD */ u8 field_0x4cd;
    /* 0x4CE */ u8 field_0x4ce;
    /* 0x4CF */ u8 mAutoMessageFlag;
    /* 0x4D0 */ bool mPaymentFlag;
    /* 0x4D1 */ u8 field_0x4d1;
    /* 0x4D2 */ bool mKillMessageFlag;
    /* 0x4D3 */ u8 mEquipBombInfo;
    /* 0x4D4 */ u8 field_0x4d4;
};

s16 dMsgObject_getGroupID();
void dMsgObject_setFundRaising(u16 param_0);
u16 dMsgObject_getFundRaising();
void dMsgObject_addOffering(s16 param_0);
u16 dMsgObject_getOffering();
void dMsgObject_addTotalPrice(s16 param_0);
u16 dMsgObject_getTotalPrice();
void dMsgObject_setTotalPrice(u16 param_0);
u16 dMsgObject_getTotalPayment();
void dMsgObject_setTotalPayment(u16 param_0);

inline dMsgObject_c* dMsgObject_getMsgObjectClass() {
    return dComIfGp_getMsgObjectClass();
}

inline void dMsgObject_demoMessageGroup() {
    dMsgObject_c::demoMessageGroup();
}

inline bool dMsgObject_isTalkNowCheck() {
    return dMsgObject_getMsgObjectClass()->getStatus() != 1 ? true : false;
}

inline bool dMsgObject_isKillMessageFlag() {
    return dMsgObject_c::isKillMessageFlag();
}

inline bool dMsgObject_isCameraCancelFlag() {
    return dMsgObject_c::isCameraCancelFlag();
}

inline u32 dMsgObject_getMessageID() {
    return dMsgObject_c::getMessageID();
}

inline u16 dMsgObject_getSmellTypeMessageID() {
    return dMsgObject_getMsgObjectClass()->getSmellTypeMessageID();
}

inline void dMsgObject_readMessageGroup(mDoDvdThd_mountXArchive_c** param_0) {
    dMsgObject_getMsgObjectClass()->readMessageGroup(param_0);
}

inline void dMsgObject_setDemoMessage(u32 param_0) {
    dMsgObject_getMsgObjectClass()->setDemoMessage(param_0);
}

inline void dMsgObject_offMsgSendControl() {
    dMsgObject_getMsgObjectClass()->offMsgSendControl();
}

inline void dMsgObject_onMsgSendControl() {
    dMsgObject_getMsgObjectClass()->onMsgSendControl();
}

inline int dMsgObject_isMsgSendControl() {
    return dMsgObject_getMsgObjectClass()->isMsgSendControl();
}

inline int dMsgObject_isMsgSend() {
    return dMsgObject_getMsgObjectClass()->isMsgSend();
}

inline void dMsgObject_offMsgSend() {
    dMsgObject_getMsgObjectClass()->offMsgSend();
}

inline const char* dMsgObject_getWord() {
    return dMsgObject_c::getWord();
}

inline void dMsgObject_setShopWaitTimer(u8 timer) {
    dMsgObject_c::setShopWaitTimer(timer);
}

inline void dMsgObject_changeFlowGroup(s32 flow) {
    dMsgObject_getMsgObjectClass()->changeFlowGroup(flow);
}

inline void dMsgObject_setTalkActor(fopAc_ac_c* actor) {
    if (dMsgObject_getMsgObjectClass() != NULL) {
        dMsgObject_getMsgObjectClass()->setTalkActor(actor);
    }
}

inline bool dMsgObject_getString(u32 param_0, J2DTextBox* param_1, J2DTextBox* param_2,
                                 JUTFont* param_3, COutFont_c* param_4, char* param_5,
                                 char* param_6, char* param_7, s16* param_8) {
    return dMsgObject_getMsgObjectClass()->getString(param_0, param_1, param_2, param_3, param_4, param_5, param_6,
                                   param_7, param_8);
}

inline void dMsgObject_onCameraCancelFlag() {
    dMsgObject_c::onCameraCancelFlag();
}

inline void dMsgObject_offCameraCancelFlag() {
    dMsgObject_c::offCameraCancelFlag();
}

inline void dMsgObject_onKillMessageFlag() {
    dMsgObject_c::onKillMessageFlag();
}

inline void dMsgObject_setKillMessageFlag() {
    dMsgObject_c::setKillMessageFlag();
}

inline void dMsgObject_endFlowGroup() {
    dMsgObject_getMsgObjectClass()->endFlowGroup();
}

inline void dMsgObject_setSmellType(u8 type) {
    dMsgObject_c::setSmellType(type);
}

inline u8 dMsgObject_getSelectCursorPos() {
    return dMsgObject_getMsgObjectClass()->getSelectCursorPos();
}

inline void dMsgObject_setTalkPartner(fopAc_ac_c* actor) {
    dMsgObject_getMsgObjectClass()->setTalkPartner(actor);
}

inline void dMsgObject_setSelectWordFlag(u8 flag) {
    dMsgObject_getMsgObjectClass()->setSelectWordFlag(flag);
}

inline u8 dMsgObject_getSelectWordFlag() {
    return dMsgObject_getMsgObjectClass()->getSelectWordFlag();
}

inline const char* dMsgObject_getSelectWord(int idx) {
    return dMsgObject_getMsgObjectClass()->getSelectWord(idx);
}

inline u8 dMsgObject_getItemEquipButton() {
    return dMsgObject_getMsgObjectClass()->getItemEquipButton();
}

inline void dMsgObject_setNowTalkFlowNo(s16 nowTalkFlowNo) {
    dMsgObject_c::setNowTalkFlowNo(nowTalkFlowNo);
}

inline void dMsgObject_setEquipBombInfo() {
    dMsgObject_c::setEquipBombInfo();
}

inline bool dMsgObject_isHowlHearingMode() {
    return dMsgObject_c::isHowlHearingMode();
}

inline void dMsgObject_onMsgSend() {
    dMsgObject_getMsgObjectClass()->onMsgSend();
}

inline bool dMsgObject_isFukidashiCheck() {
    return dMsgObject_getMsgObjectClass()->getScrnDrawPtr() == NULL ? false : true;
}

inline void* dMsgObject_getTalkHeap() {
    return dMsgObject_c::getTalkHeap();
}

inline void dMsgObject_setTalkHeap(void* heap) {
    dMsgObject_c::setTalkHeap(heap);
}

inline u8 dMsgObject_getMsgOutputType() {
    return dMsgObject_getMsgObjectClass()->getMsgOutputType();
}

inline void dMsgObject_setMsgOutputType(u8 outputType) {
    dMsgObject_getMsgObjectClass()->setMsgOutputType(outputType);
}

inline void dMsgObject_setPortalMessageID(u16 id) {
    dMsgObject_getMsgObjectClass()->setPortalMessageID(id);
}

inline void dMsgObject_setWord(const char* i_word) {
    dMsgObject_getMsgObjectClass()->setWord(i_word);
}

inline void dMsgObject_setSelectWord(int i_no, const char* i_word) {
    dMsgObject_getMsgObjectClass()->setSelectWord(i_no, i_word);
}

inline u8* dMsgObject_getMsgDtPtr() {
    return (u8*)dMsgObject_getMsgObjectClass()->getMsgDtPtr();
}

inline void dMsgObject_setSelectCancelPos(u8 param_0) {
    dMsgObject_getMsgObjectClass()->setSelectCancelPos(param_0);
}

inline void dMsgObject_setSelectCursorPos(u8 param_0) {
    dMsgObject_getMsgObjectClass()->setSelectCursorPos(param_0);
}

inline u8 dMsgObject_getSelectBombBagID() {
    return dMsgObject_getMsgObjectClass()->getSelectBombBagID();
}

inline s16 dMsgObject_getSelectBombPrice() {
    return dMsgObject_getMsgObjectClass()->getSelectBombPrice();
}

inline bool dMsgObject_isMouthCheck() {
    return dMsgObject_getMsgObjectClass()->isMouthCheck();
}

inline void dMsgObject_setInsectItemNo(u8 i_insectItemNo) {
    dMsgObject_getMsgObjectClass()->setInsectItemNo(i_insectItemNo);
}

inline bool dMsgObject_isSelectTalkNowCheck() {
    return dMsgObject_getMsgObjectClass()->getStatus() != 8 || (dMsgObject_getMsgObjectClass()->getStatus() != 9 && dMsgObject_getMsgObjectClass()->getStatus() != 20);
}

class dMsgObject_HowlHIO_c {
public:
    dMsgObject_HowlHIO_c();
    virtual ~dMsgObject_HowlHIO_c() {}

    /* 0x04 */ s32 field_0x4;
    /* 0x08 */ JUtility::TColor mHowlLineStartCol;
    /* 0x0C */ JUtility::TColor mHowlLineEndCol;
    /* 0x10 */ f32 mHighlightScale;
    /* 0x14 */ JUtility::TColor mHighlightMoyaR0;
    /* 0x18 */ JUtility::TColor mHighlightMoyaR1;
    /* 0x1C */ JUtility::TColor mHighlightMoyabsR0;
    /* 0x20 */ JUtility::TColor mHighlightMoyabsR1;
    /* 0x24 */ JUtility::TColor mHighlightPikaR0;
    /* 0x28 */ JUtility::TColor mHighlightPikaR1;
    /* 0x2C */ f32 mAnmSpeed;
    /* 0x30 */ JUtility::TColor mGuideUpperBlack;
    /* 0x34 */ JUtility::TColor mGuideUpperWhite;
    /* 0x38 */ JUtility::TColor mGuideMidBlack;
    /* 0x3C */ JUtility::TColor mGuideMidWhite;
    /* 0x40 */ JUtility::TColor mGuideLowerBlack;
    /* 0x44 */ JUtility::TColor mGuideLowerWhite;
    /* 0x48 */ JUtility::TColor mAnswerUpperBlack;
    /* 0x4C */ JUtility::TColor mAnswerUpperWhite;
    /* 0x50 */ JUtility::TColor mAnswerMidBlack;
    /* 0x54 */ JUtility::TColor mAnswerMidWhite;
    /* 0x58 */ JUtility::TColor mAnswerLowerBlack;
    /* 0x5C */ JUtility::TColor mAnswerLowerWhite;
    /* 0x60 */ JUtility::TColor mDotBlack;
    /* 0x64 */ JUtility::TColor mDotWhite;
    /* 0x68 */ f32 mDotScale;
    /* 0x6C */ f32 mDrawInterval;
    /* 0x70 */ f32 mLineThickness;
    /* 0x74 */ f32 mCrossPosX;
    /* 0x78 */ f32 mCrossPosY;
    /* 0x7C */ f32 mCrossScale;
    /* 0x80 */ f32 mWindowPosX;
    /* 0x84 */ f32 mWindowPosY;
    /* 0x88 */ f32 mWindowScale;
    /* 0x8C */ f32 mInputStartPoint;
    /* 0x90 */ f32 mGuideUpperAlpha;
    /* 0x94 */ f32 mGuideMidAlpha;
    /* 0x98 */ f32 mGuideLowerAlpha;
    /* 0x9C */ f32 mAnswerUpperAlpha;
    /* 0xA0 */ f32 mAnswerMidAlpha;
    /* 0xA4 */ f32 mAnswerLowerAlpha;
    /* 0xA8 */ f32 mLineSpaceOffset;
    /* 0xAC */ u16 mWaveformThickness;
    /* 0xAE */ u16 mWaveformLineLen;
    /* 0xB0 */ s16 mWaveformDotLen;
};

class dMsgObject_HIO_c {
public:
    dMsgObject_HIO_c();
    virtual ~dMsgObject_HIO_c() {}

    /* 0x004 */ s8 field_0x4;
    /* 0x008 */ f32 mBoxMidna_fontSizeX;
    /* 0x00C */ f32 field_0xc;
    /* 0x010 */ f32 mBoxMidna_fontSizeY;
    /* 0x014 */ f32 field_0x14;
    /* 0x018 */ f32 mBoxTalkAlphaP;
    /* 0x01C */ f32 mBoxNaviAlphaP;
    /* 0x020 */ f32 mBoxMidnaAlphaP;
    /* 0x024 */ f32 mBoxItemAlphaP;
    /* 0x028 */ f32 mBoxStoneAlphaP;
    /* 0x02C */ f32 mBoxWoodAlphaP;
    /* 0x030 */ f32 mBoxBookAlphaP;
    /* 0x034 */ f32 mBoxWolfAlphaP;
    /* 0x038 */ f32 mSubtitleAlphaP;
    /* 0x03C */ f32 field_0x3c;
    /* 0x040 */ f32 mBoxNaviAnmSpeed;
    /* 0x044 */ f32 field_0x44;
    /* 0x048 */ f32 mBoxItemAnmSpeed;
    /* 0x04C */ f32 field_0x4c;
    /* 0x050 */ f32 field_0x50;
    /* 0x054 */ f32 field_0x54;
    /* 0x058 */ f32 mBoxWolfAnmSpeed;
    /* 0x05C */ f32 mBoxItemHaloAlpha;
    /* 0x060 */ f32 mBoxNaviHaloAlpha;
    /* 0x064 */ f32 mBoxMidnaHaloAlpha;
    /* 0x068 */ f32 mBoxWolfHaloAlpha;
    /* 0x06C */ f32 mBoxTalkHaloAlpha;
    /* 0x070 */ f32 mBoxTalkScaleX;
    /* 0x074 */ f32 mBoxNaviScaleX;
    /* 0x078 */ f32 mBoxMidnaScaleX;
    /* 0x07C */ f32 mBoxItemScaleX;
    /* 0x080 */ f32 mBoxStoneScaleX;
    /* 0x084 */ f32 mBoxWoodScaleX;
    /* 0x088 */ f32 mBoxBookScaleX;
    /* 0x08C */ f32 mBoxWolfScaleX;
    /* 0x090 */ f32 mStageTitleScaleX;
    /* 0x094 */ f32 mSubtitleScaleX;
    /* 0x098 */ f32 mBossNameScaleX;
    /* 0x09C */ f32 mBoxTalkScaleY;
    /* 0x0A0 */ f32 mBoxNaviScaleY;
    /* 0x0A4 */ f32 mBoxMidnaScaleY;
    /* 0x0A8 */ f32 mBoxItemScaleY;
    /* 0x0AC */ f32 mBoxStoneScaleY;
    /* 0x0B0 */ f32 mBoxWoodScaleY;
    /* 0x0B4 */ f32 mBoxBookScaleY;
    /* 0x0B8 */ f32 mBoxWolfScaleY;
    /* 0x0BC */ f32 mStageTitleScaleY;
    /* 0x0C0 */ f32 mSubtitleScaleY;
    /* 0x0C4 */ f32 mBossNameScaleY;
    /* 0x0C8 */ f32 mBoxItemSizeX;
    /* 0x0CC */ f32 mBoxItemSizeY;
    /* 0x0D0 */ f32 mBoxItemPosX;
    /* 0x0D4 */ f32 mBoxItemPosY;
    /* 0x0D8 */ f32 mBoxItemTextSizeX;
    /* 0x0DC */ f32 mBoxItemTextSizeY;
    /* 0x0E0 */ f32 mBoxItemTextPosX;
    /* 0x0E4 */ f32 mBoxItemTextPosY;
    /* 0x0E8 */ JUtility::TColor
        mBoxStartBlack[2][10];  // talk, navi, midna, item, stone, wood, book, wolf,
    /* 0x110 */ //JUtility::TColor mBoxEndBlack[10];
    /* 0x138 */ JUtility::TColor mBoxStartWhite[2][10];
    /* 0x160 */ //JUtility::TColor mBoxEndWhite[10];
    /* 0x188 */ JUtility::TColor mSelMsgInactiveCol;
    /* 0x18C */ f32 mStageTitleCharPosX;
    /* 0x190 */ f32 mBossNameCharPosX;
    /* 0x194 */ f32 mStageTitleCharPosY;
    /* 0x198 */ f32 mBossNameCharPosY;
    /* 0x19C */ f32 mStageTitleCharSizeX;
    /* 0x1A0 */ f32 mBossNameCharSizeX;
    /* 0x1A4 */ f32 mStageTitleCharSizeY;
    /* 0x1A8 */ f32 mBossNameCharSizeY;
    /* 0x1AC */ f32 mStageTitleBasePosX;
    /* 0x1B0 */ f32 mBossNameBasePosX;
    /* 0x1B4 */ f32 mStageTitleBasePosY;
    /* 0x1B8 */ f32 mBossNameBasePosY;
    /* 0x1BC */ f32 mStageTitleBaseSizeX;
    /* 0x1C0 */ f32 mBossNameBaseSizeX;
    /* 0x1C4 */ f32 mStageTitleBaseSizeY;
    /* 0x1C8 */ f32 mBossNameBaseSizeY;
    /* 0x1CC */ f32 mStageTitleBaseAlpha;
    /* 0x1D0 */ f32 mBossNameBaseAlpha;
    /* 0x1D4 */ f32 mChoicePos[3][10];  // down, mid, top
    /* 0x24C */ f32
        mBoxPos[3][10];  // down, mid, top    // talk, navi, midna, item, stone, wood, book, wolf,
    /* 0x2C4 */ f32 mPortalIconScale;
    /* 0x2C8 */ f32 mTextPosX;
    /* 0x2CC */ f32 mTextPosY;
    /* 0x2D0 */ f32 mPikariScale;
    /* 0x2D4 */ JUtility::TColor mPikariMoyaR0;
    /* 0x2D8 */ JUtility::TColor mPikariMoyaR1;
    /* 0x2DC */ JUtility::TColor mPikariMoyabsR0;
    /* 0x2E0 */ JUtility::TColor mPikariMoyabsR1;
    /* 0x2E4 */ JUtility::TColor mPikariPikaR0;
    /* 0x2E8 */ JUtility::TColor mPikariPikaR1;
    /* 0x2EC */ f32 mPikariAnmSpeed;
    /* 0x2F0 */ s16 mPikariHaloDelay_spirit;
    /* 0x2F2 */ u8 mStageTitleDisplayType;
    /* 0x2F4 */ s16 mMsgIndex;
    /* 0x2F6 */ s16 mFlowIndex;
    /* 0x2F8 */ s16 mSaveSeqMsgIndex;
    /* 0x2FA */ s16 mSelWaitFrame;
    /* 0x2FC */ s16 mBoxAppearBound;
    /* 0x2FE */ s16 mBoxAppearFrame;
    /* 0x300 */ s16 mWaitFrame;
    /* 0x302 */ s16 mLightAppearFrame;
    /* 0x304 */ s16 field_0x304;
    /* 0x306 */ s16 mStageTitleFadeIn;
    /* 0x308 */ s16 mStageTitleFadeOut;
    /* 0x30A */ s16 mBossNameFadeIn;
    /* 0x30C */ s16 mBossNameFadeOut;
    /* 0x30E */ u16 mStageTitleDisplayTime;
    /* 0x310 */ u16 mBossNameDisplayTime;
    /* 0x312 */ s16 mBatchDisplayWeight_3;
    /* 0x314 */ s16 mCharDisplayWeight_5;
    /* 0x316 */ s16 mBatchDisplayWeight_8;
    /* 0x318 */ s16 mBatchDisplayWeight_9;
    /* 0x31A */ s16 mHaloDelayFrame;
    /* 0x31C */ s16 mHaloDelayFrameSpirit;
    /* 0x31E */ bool mSaveSeqMsgDebug;
    /* 0x31F */ u8 mMsgDebug;
    /* 0x320 */ bool mTextColorDebug;
    /* 0x321 */ u8
        mTextColorUpperR[9];  // Default, Red, Green, Blue, Yellow, L.Blue, Purple, Grey, Orange,
    /* 0x32A */ u8 mTextColorUpperG[9];
    /* 0x333 */ u8 mTextColorUpperB[9];
    /* 0x33C */ u8
        mTextColorLowerR[9];  // Default, Red, Green, Blue, Yellow, L.Blue, Purple, Grey, Orange,
    /* 0x345 */ u8 mTextColorLowerG[9];
    /* 0x34E */ u8 mTextColorLowerB[9];
    /* 0x357 */ u8 mDisplaySpeed;
    /* 0x358 */ u8 mDisplaySpeedSpirit;
    /* 0x359 */ u8 mMessageDisplay;
    /* 0x35C */ dMsgObject_HowlHIO_c mHowlHIO;
};

extern dMsgObject_HIO_c g_MsgObject_HIO_c;

#endif /* D_MSG_D_MSG_OBJECT_H */
