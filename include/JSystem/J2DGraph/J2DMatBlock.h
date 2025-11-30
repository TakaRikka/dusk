#ifndef J2DMATBLOCK_H
#define J2DMATBLOCK_H

#include "JSystem/J2DGraph/J2DTevs.h"
#include "JSystem/JUtility/TColor.h"

class JUTFont;
class JUTPalette;
class JUTResFont;
class JUTTexture;
struct ResFONT;
struct ResTIMG;
struct ResTLUT;

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DGXColorS10 : public GXColorS10 {
    J2DGXColorS10() {}
    J2DGXColorS10(const J2DGXColorS10& other) {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
    }
    J2DGXColorS10(const GXColorS10& other) {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
    }

    J2DGXColorS10& operator=(const GXColorS10& other) {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
        return *this;
    }
};

struct J2DTevSwapModeInfo;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTevBlock {
public:
    virtual void initialize() {}
    virtual void setGX() {}
    virtual void loadTexture(_GXTexMapID, u32) {}
    virtual u32 getType() = 0;
    virtual u8 getMaxStage() = 0;
    virtual void setTexNo(u32, u16) {}
    virtual u32 getTexNo(u32) const { return 0xFFFF; }
    virtual void setFontNo(u16) {}
    virtual u16 getFontNo() const { return 0xFFFF; }
    virtual void setTevOrder(u32, J2DTevOrder) {}
    virtual J2DTevOrder* getTevOrder(u32) { return NULL; }
    virtual void setTevColor(u32, J2DGXColorS10) {}
    virtual J2DGXColorS10* getTevColor(u32) { return NULL; }
    virtual void setTevKColor(u32, JUtility::TColor) {}
    virtual JUtility::TColor* getTevKColor(u32) { return NULL; }
    virtual void setTevKColorSel(u32, u8) {}
    virtual u8 getTevKColorSel(u32) { return 0; }
    virtual void setTevKAlphaSel(u32, u8) {}
    virtual u8 getTevKAlphaSel(u32) { return 0; }
    virtual void setTevStageNum(u8) {}
    virtual u8 getTevStageNum() const { return 1; }
    virtual void setTevStage(u32, J2DTevStage) {}
    virtual J2DTevStage* getTevStage(u32) { return NULL; }
    virtual void setTevSwapModeInfo(u32, J2DTevSwapModeInfo) {}
    virtual void setTevSwapModeTable(u32, J2DTevSwapModeTable) {}
    virtual J2DTevSwapModeTable* getTevSwapModeTable(u32) { return NULL; }
    virtual void setIndTevStage(u32, J2DIndTevStage) {}
    virtual J2DIndTevStage* getIndTevStage(u32) { return NULL; }
    virtual bool insertTexture(u32, ResTIMG const*) { return false; }
    virtual bool insertTexture(u32, ResTIMG const*, JUTPalette*) { return false; }
    virtual bool insertTexture(u32, JUTTexture*) { return false; }
    virtual bool setTexture(u32, ResTIMG const*) { return false; }
    virtual bool setTexture(u32, JUTTexture*) { return false; }
    virtual bool removeTexture(u32) { return false; }
    virtual bool setFont(ResFONT*) { return false; }
    virtual bool setFont(JUTFont*) { return false; }
    virtual bool setPalette(u32, ResTLUT const*) { return false; }
    virtual bool prepareTexture(u8) { return false; }
    virtual JUTTexture* getTexture(u32);
    virtual JUTPalette* getPalette(u32);
    virtual JUTFont* getFont() { return NULL; }
    virtual void shiftDeleteFlag(u8, bool) {}
    virtual void setUndeleteFlag(u8) {}
    virtual void setFontUndeleteFlag() {}
    virtual ~J2DTevBlock() {}
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTevBlock1 : public J2DTevBlock {
public:
    J2DTevBlock1();

    virtual void initialize();
    virtual void setGX();
    virtual void loadTexture(_GXTexMapID, u32);
    virtual u32 getType() { return 'TVB1'; }
    virtual u8 getMaxStage() { return 1; }
    virtual void setTexNo(u32 index, u16 texNo) { mTexNo[index] = texNo; }
    virtual u32 getTexNo(u32 index) const { return mTexNo[index]; }
    virtual void setFontNo(u16 fontNo) { mFontNo = fontNo; }
    virtual u16 getFontNo() const { return mFontNo; }
    virtual void setTevOrder(u32 index, J2DTevOrder order) {
        mTevOrder[index] = order;
    }
    virtual J2DTevOrder* getTevOrder(u32 index) { return &mTevOrder[index]; }
    virtual void setTevColor(u32 index, J2DGXColorS10 color) {
        mTevColor[index] = color;
    }
    virtual J2DGXColorS10* getTevColor(u32 index) { return &mTevColor[index]; }
    virtual void setTevKColor(u32 index, JUtility::TColor color) {
        mTevKColor[index] = color;
    }
    virtual JUtility::TColor* getTevKColor(u32 index) { return &mTevKColor[index]; }
    virtual void setTevKColorSel(u32 index, u8 sel) { mTevKColorSel[index] = sel; }
    virtual u8 getTevKColorSel(u32 index) { return mTevKColorSel[index]; }
    virtual void setTevKAlphaSel(u32 index, u8 sel) { mTevKAlphaSel[index] = sel; }
    virtual u8 getTevKAlphaSel(u32 index) { return mTevKAlphaSel[index]; }
    virtual void setTevStageNum(u8 num) {}
    virtual u8 getTevStageNum() const { return 1; }
    virtual void setTevStage(u32 index, J2DTevStage stage) {
        mTevStage[index] = stage;
    }
    virtual J2DTevStage* getTevStage(u32 index) { return &mTevStage[index]; }
    virtual void setTevSwapModeInfo(u32 index, J2DTevSwapModeInfo info) {
        mTevStage[index].setTevSwapModeInfo(info);
    }
    virtual void setTevSwapModeTable(u32 index, J2DTevSwapModeTable table) {
        mTevSwapModeTable[index] = table;
    }
    virtual J2DTevSwapModeTable* getTevSwapModeTable(u32 index) {
        return &mTevSwapModeTable[index];
    }
    virtual void setIndTevStage(u32 index, J2DIndTevStage stage) {
        mIndTevStage[index] = stage;
    }
    virtual J2DIndTevStage* getIndTevStage(u32 index) {
        return &mIndTevStage[index];
    }
    virtual bool insertTexture(u32 index, ResTIMG const* p_timg) {
        return insertTexture(index, p_timg, NULL);
    }
    virtual bool insertTexture(u32, ResTIMG const*, JUTPalette*);
    virtual bool insertTexture(u32, JUTTexture*);
    virtual bool setTexture(u32, ResTIMG const*);
    virtual bool setTexture(u32, JUTTexture*);
    virtual bool removeTexture(u32);
    virtual bool setFont(ResFONT*);
    virtual bool setFont(JUTFont*);
    virtual bool setPalette(u32, ResTLUT const*);
    virtual bool prepareTexture(u8);
    virtual JUTTexture* getTexture(u32 index) {
        return index >= 1 ? NULL : mTexture[index];
    }
    virtual JUTPalette* getPalette(u32 index) {
        return index >= 1 ? NULL : mPalette[index];
    }
    virtual JUTFont* getFont() { return mFont; }
    virtual void shiftDeleteFlag(u8, bool);
    virtual void setUndeleteFlag(u8 flag) { mUndeleteFlag &= flag; }
    virtual void setFontUndeleteFlag() { mUndeleteFlag &= 0x7F; }
    virtual ~J2DTevBlock1();

private:
    /* 0x004 */ u16 mTexNo[1];
    /* 0x006 */ u16 mFontNo;
    /* 0x008 */ J2DTevOrder mTevOrder[1];
    /* 0x00C */ J2DGXColorS10 mTevColor[4];
    /* 0x02C */ J2DTevStage mTevStage[1];
    /* 0x034 */ JUtility::TColor mTevKColor[4];
    /* 0x044 */ u8 mTevKColorSel[1];
    /* 0x045 */ u8 mTevKAlphaSel[1];
    /* 0x046 */ J2DTevSwapModeTable mTevSwapModeTable[4];
    /* 0x04C */ J2DIndTevStage mIndTevStage[1];
    /* 0x050 */ JUTTexture* mTexture[1];
    /* 0x054 */ JUTPalette* mPalette[1];
    /* 0x058 */ JUTFont* mFont;
    /* 0x05C */ u8 mUndeleteFlag;
};  // Size: 0x60

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTevBlock2 : public J2DTevBlock {
public:
    J2DTevBlock2();

    virtual void initialize();
    virtual void setGX();
    virtual void loadTexture(_GXTexMapID, u32);
    virtual u32 getType() { return 'TVB2'; }
    virtual u8 getMaxStage() { return 2; }
    virtual void setTexNo(u32 index, u16 texNo) { mTexNo[index] = texNo; }
    virtual u32 getTexNo(u32 index) const { return mTexNo[index]; }
    virtual void setFontNo(u16 fontNo) { mFontNo = fontNo; }
    virtual u16 getFontNo() const { return mFontNo; }
    virtual void setTevOrder(u32 index, J2DTevOrder order) {
        mTevOrder[index] = order;
    }
    virtual J2DTevOrder* getTevOrder(u32 index) { return &mTevOrder[index]; }
    virtual void setTevColor(u32 index, J2DGXColorS10 color) {
        mTevColor[index] = color;
    }
    virtual J2DGXColorS10* getTevColor(u32 index) { return &mTevColor[index]; }
    virtual void setTevKColor(u32 index, JUtility::TColor color) {
        mTevKColor[index] = color;
    }
    virtual JUtility::TColor* getTevKColor(u32 index) { return &mTevKColor[index]; }
    virtual void setTevKColorSel(u32 index, u8 sel) { mTevKColorSel[index] = sel; }
    virtual u8 getTevKColorSel(u32 index) { return mTevKColorSel[index]; }
    virtual void setTevKAlphaSel(u32 index, u8 sel) { mTevKAlphaSel[index] = sel; }
    virtual u8 getTevKAlphaSel(u32 index) { return mTevKAlphaSel[index]; }
    virtual void setTevStageNum(u8 num) { mTevStageNum = num; }
    virtual u8 getTevStageNum() const { return mTevStageNum; }
    virtual void setTevStage(u32 index, J2DTevStage stage) {
        mTevStage[index] = stage;
    }
    virtual J2DTevStage* getTevStage(u32 index) { return &mTevStage[index]; }
    virtual void setTevSwapModeInfo(u32 index, J2DTevSwapModeInfo info) {
        mTevStage[index].setTevSwapModeInfo(info);
    }
    virtual void setTevSwapModeTable(u32 index, J2DTevSwapModeTable table) {
        mTevSwapModeTable[index] = table;
    }
    virtual J2DTevSwapModeTable* getTevSwapModeTable(u32 index) {
        return &mTevSwapModeTable[index];
    }
    virtual void setIndTevStage(u32 index, J2DIndTevStage stage) {
        mIndTevStage[index] = stage;
    }
    virtual J2DIndTevStage* getIndTevStage(u32 index) {
        return &mIndTevStage[index];
    }
    virtual bool insertTexture(u32 index, ResTIMG const* p_timg) {
        return insertTexture(index, p_timg, NULL);
    }
    virtual bool insertTexture(u32, ResTIMG const*, JUTPalette*);
    virtual bool insertTexture(u32, JUTTexture*);
    virtual bool setTexture(u32, ResTIMG const*);
    virtual bool setTexture(u32, JUTTexture*);
    virtual bool removeTexture(u32);
    virtual bool setFont(ResFONT*);
    virtual bool setFont(JUTFont*);
    virtual bool setPalette(u32, ResTLUT const*);
    virtual bool prepareTexture(u8);
    virtual JUTTexture* getTexture(u32 index) {
        return index >= 2 ? NULL : mTexture[index];
    }
    virtual JUTPalette* getPalette(u32 index) {
        return index >= 2 ? NULL : mPalette[index];
    }
    virtual JUTFont* getFont() { return mFont; }
    virtual void shiftDeleteFlag(u8, bool);
    virtual void setUndeleteFlag(u8 flag) { mUndeleteFlag &= flag; }
    virtual void setFontUndeleteFlag() { mUndeleteFlag &= 0x7F; }
    virtual ~J2DTevBlock2();

private:
    /* 0x004 */ u16 mTexNo[2];
    /* 0x008 */ u16 mFontNo;
    /* 0x00A */ J2DTevOrder mTevOrder[2];
    /* 0x012 */ J2DGXColorS10 mTevColor[4];
    /* 0x032 */ u8 mTevStageNum;
    /* 0x033 */ J2DTevStage mTevStage[2];
    /* 0x043 */ u8 field_0x43;
    /* 0x044 */ JUtility::TColor mTevKColor[4];
    /* 0x054 */ u8 mTevKColorSel[2];
    /* 0x056 */ u8 mTevKAlphaSel[2];
    /* 0x058 */ J2DTevSwapModeTable mTevSwapModeTable[4];
    /* 0x05C */ J2DIndTevStage mIndTevStage[2];
    /* 0x064 */ JUTTexture* mTexture[2];
    /* 0x06C */ JUTPalette* mPalette[2];
    /* 0x074 */ JUTFont* mFont;
    /* 0x078 */ u8 mUndeleteFlag;
};  // Size: 0x7C

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTevBlock4 : public J2DTevBlock {
public:
    J2DTevBlock4();

    virtual void initialize();
    virtual void setGX();
    virtual void loadTexture(_GXTexMapID, u32);
    virtual u32 getType() { return 'TVB4'; }
    virtual u8 getMaxStage() { return 4; }
    virtual void setTexNo(u32 index, u16 texNo) { mTexNo[index] = texNo; }
    virtual u32 getTexNo(u32 index) const { return mTexNo[index]; }
    virtual void setFontNo(u16 fontNo) { mFontNo = fontNo; }
    virtual u16 getFontNo() const { return mFontNo; }
    virtual void setTevOrder(u32 index, J2DTevOrder order) {
        mTevOrder[index] = order;
    }
    virtual J2DTevOrder* getTevOrder(u32 index) { return &mTevOrder[index]; }
    virtual void setTevColor(u32 index, J2DGXColorS10 color) {
        mTevColor[index] = color;
    }
    virtual J2DGXColorS10* getTevColor(u32 index) { return &mTevColor[index]; }
    virtual void setTevKColor(u32 index, JUtility::TColor color) {
        mTevKColor[index] = color;
    }
    virtual JUtility::TColor* getTevKColor(u32 index) { return &mTevKColor[index]; }
    virtual void setTevKColorSel(u32 index, u8 sel) { mTevKColorSel[index] = sel; }
    virtual u8 getTevKColorSel(u32 index) { return mTevKColorSel[index]; }
    virtual void setTevKAlphaSel(u32 index, u8 sel) { mTevKAlphaSel[index] = sel; }
    virtual u8 getTevKAlphaSel(u32 index) { return mTevKAlphaSel[index]; }
    virtual void setTevStageNum(u8 num) { mTevStageNum = num; }
    virtual u8 getTevStageNum() const { return mTevStageNum; }
    virtual void setTevStage(u32 index, J2DTevStage stage) {
        mTevStage[index] = stage;
    }
    virtual J2DTevStage* getTevStage(u32 index) { return &mTevStage[index]; }
    virtual void setTevSwapModeInfo(u32 index, J2DTevSwapModeInfo info) {
        mTevStage[index].setTevSwapModeInfo(info);
    }
    virtual void setTevSwapModeTable(u32 index, J2DTevSwapModeTable table) {
        mTevSwapModeTable[index] = table;
    }
    virtual J2DTevSwapModeTable* getTevSwapModeTable(u32 index) {
        return &mTevSwapModeTable[index];
    }
    virtual void setIndTevStage(u32 index, J2DIndTevStage stage) {
        mIndTevStage[index] = stage;
    }
    virtual J2DIndTevStage* getIndTevStage(u32 index) {
        return &mIndTevStage[index];
    }
    virtual bool insertTexture(u32 index, ResTIMG const* p_timg) {
        return insertTexture(index, p_timg, NULL);
    }
    virtual bool insertTexture(u32, ResTIMG const*, JUTPalette*);
    virtual bool insertTexture(u32, JUTTexture*);
    virtual bool setTexture(u32, ResTIMG const*);
    virtual bool setTexture(u32, JUTTexture*);
    virtual bool removeTexture(u32);
    virtual bool setFont(ResFONT*);
    virtual bool setFont(JUTFont*);
    virtual bool setPalette(u32, ResTLUT const*);
    virtual bool prepareTexture(u8);
    virtual JUTTexture* getTexture(u32 index) {
        return index >= 4 ? NULL : mTexture[index];
    }
    virtual JUTPalette* getPalette(u32 index) {
        return index >= 4 ? NULL : mPalette[index];
    }
    virtual JUTFont* getFont() { return mFont; }
    virtual void shiftDeleteFlag(u8, bool);
    virtual void setUndeleteFlag(u8 flag) { mUndeleteFlag &= flag; }
    virtual void setFontUndeleteFlag() { mUndeleteFlag &= 0x7F; }
    virtual ~J2DTevBlock4();

private:
    /* 0x004 */ u16 mTexNo[4];
    /* 0x00C */ u16 mFontNo;
    /* 0x00E */ J2DTevOrder mTevOrder[4];
    /* 0x01E */ J2DGXColorS10 mTevColor[4];
    /* 0x03E */ u8 mTevStageNum;
    /* 0x03F */ J2DTevStage mTevStage[4];
    /* 0x05F */ u8 field_0x5f;
    /* 0x060 */ JUtility::TColor mTevKColor[4];
    /* 0x070 */ u8 mTevKColorSel[4];
    /* 0x074 */ u8 mTevKAlphaSel[4];
    /* 0x078 */ J2DTevSwapModeTable mTevSwapModeTable[4];
    /* 0x07C */ J2DIndTevStage mIndTevStage[4];
    /* 0x08C */ JUTTexture* mTexture[4];
    /* 0x09C */ JUTPalette* mPalette[4];
    /* 0x0AC */ JUTFont* mFont;
    /* 0x0B0 */ u8 mUndeleteFlag;
};  // Size: 0xB4

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTevBlock8 : public J2DTevBlock {
public:
    J2DTevBlock8();

    virtual void initialize();
    virtual void setGX();
    virtual void loadTexture(_GXTexMapID, u32);
    virtual u32 getType() { return 'TVB8'; }
    virtual u8 getMaxStage() { return 8; }
    virtual void setTexNo(u32 index, u16 texNo) { mTexNo[index] = texNo; }
    virtual u32 getTexNo(u32 index) const { return mTexNo[index]; }
    virtual void setFontNo(u16 fontNo) { mFontNo = fontNo; }
    virtual u16 getFontNo() const { return mFontNo; }
    virtual void setTevOrder(u32 index, J2DTevOrder order) {
        mTevOrder[index] = order;
    }
    virtual J2DTevOrder* getTevOrder(u32 index) { return &mTevOrder[index]; }
    virtual void setTevColor(u32 index, J2DGXColorS10 color) {
        mTevColor[index] = color;
    }
    virtual J2DGXColorS10* getTevColor(u32 index) { return &mTevColor[index]; }
    virtual void setTevKColor(u32 index, JUtility::TColor color) {
        mTevKColor[index] = color;
    }
    virtual JUtility::TColor* getTevKColor(u32 index) { return &mTevKColor[index]; }
    virtual void setTevKColorSel(u32 index, u8 sel) { mTevKColorSel[index] = sel; }
    virtual u8 getTevKColorSel(u32 index) { return mTevKColorSel[index]; }
    virtual void setTevKAlphaSel(u32 index, u8 sel) { mTevKAlphaSel[index] = sel; }
    virtual u8 getTevKAlphaSel(u32 index) { return mTevKAlphaSel[index]; }
    virtual void setTevStageNum(u8 num) { mTevStageNum = num; }
    virtual u8 getTevStageNum() const { return mTevStageNum; }
    virtual void setTevStage(u32 index, J2DTevStage stage) {
        mTevStage[index] = stage;
    }
    virtual J2DTevStage* getTevStage(u32 index) { return &mTevStage[index]; }
    virtual void setTevSwapModeInfo(u32 index, J2DTevSwapModeInfo info) {
        mTevStage[index].setTevSwapModeInfo(info);
    }
    virtual void setTevSwapModeTable(u32 index, J2DTevSwapModeTable table) {
        mTevSwapModeTable[index] = table;
    }
    virtual J2DTevSwapModeTable* getTevSwapModeTable(u32 index) {
        return &mTevSwapModeTable[index];
    }
    virtual void setIndTevStage(u32 index, J2DIndTevStage stage) {
        mIndTevStage[index] = stage;
    }
    virtual J2DIndTevStage* getIndTevStage(u32 index) {
        return &mIndTevStage[index];
    }
    virtual bool insertTexture(u32 index, ResTIMG const* p_timg) {
        return insertTexture(index, p_timg, NULL);
    }
    virtual bool insertTexture(u32, ResTIMG const*, JUTPalette*);
    virtual bool insertTexture(u32, JUTTexture*);
    virtual bool setTexture(u32, ResTIMG const*);
    virtual bool setTexture(u32, JUTTexture*);
    virtual bool removeTexture(u32);
    virtual bool setFont(ResFONT*);
    virtual bool setFont(JUTFont*);
    virtual bool setPalette(u32, ResTLUT const*);
    virtual bool prepareTexture(u8);
    virtual JUTTexture* getTexture(u32 index) {
        return index >= 8 ? NULL : mTexture[index];
    }
    virtual JUTPalette* getPalette(u32 index) {
        return index >= 8 ? NULL : mPalette[index];
    }
    virtual JUTFont* getFont() { return mFont; }
    virtual void shiftDeleteFlag(u8, bool);
    virtual void setUndeleteFlag(u8 flag) { mUndeleteFlag &= flag; }
    virtual void setFontUndeleteFlag() { mFontUndeleteFlag = false; }
    virtual ~J2DTevBlock8();

private:
    /* 0x004 */ u16 mTexNo[8];
    /* 0x014 */ u16 mFontNo;
    /* 0x016 */ J2DTevOrder mTevOrder[8];
    /* 0x036 */ J2DGXColorS10 mTevColor[4];
    /* 0x056 */ u8 mTevStageNum;
    /* 0x057 */ J2DTevStage mTevStage[8];
    /* 0x097 */ u8 field_0x97;
    /* 0x098 */ JUtility::TColor mTevKColor[4];
    /* 0x0A8 */ u8 mTevKColorSel[8];
    /* 0x0B0 */ u8 mTevKAlphaSel[8];
    /* 0x0B8 */ J2DTevSwapModeTable mTevSwapModeTable[4];
    /* 0x0BC */ J2DIndTevStage mIndTevStage[8];
    /* 0x0DC */ JUTTexture* mTexture[8];
    /* 0x0FC */ JUTPalette* mPalette[8];
    /* 0x11C */ JUTFont* mFont;
    /* 0x120 */ u8 mUndeleteFlag;
    /* 0x121 */ bool mFontUndeleteFlag;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTevBlock16 : public J2DTevBlock {
public:
    J2DTevBlock16();

    virtual void initialize();
    virtual void setGX();
    virtual void loadTexture(_GXTexMapID, u32);
    virtual u32 getType() { return 'TV16'; }
    virtual u8 getMaxStage() { return 16; }
    virtual void setTexNo(u32 index, u16 texNo) { mTexNo[index] = texNo; }
    virtual u32 getTexNo(u32 index) const { return mTexNo[index]; }
    virtual void setFontNo(u16 fontNo) { mFontNo = fontNo; }
    virtual u16 getFontNo() const { return mFontNo; }
    virtual void setTevOrder(u32 index, J2DTevOrder order) {
        mTevOrder[index] = order;
    }
    virtual J2DTevOrder* getTevOrder(u32 index) { return &mTevOrder[index]; }
    virtual void setTevColor(u32 index, J2DGXColorS10 color) {
        mTevColor[index] = color;
    }
    virtual J2DGXColorS10* getTevColor(u32 index) { return &mTevColor[index]; }
    virtual void setTevKColor(u32 index, JUtility::TColor color) {
        mTevKColor[index] = color;
    }
    virtual JUtility::TColor* getTevKColor(u32 index) { return &mTevKColor[index]; }
    virtual void setTevKColorSel(u32 index, u8 sel) { mTevKColorSel[index] = sel; }
    virtual u8 getTevKColorSel(u32 index) { return mTevKColorSel[index]; }
    virtual void setTevKAlphaSel(u32 index, u8 sel) { mTevKAlphaSel[index] = sel; }
    virtual u8 getTevKAlphaSel(u32 index) { return mTevKAlphaSel[index]; }
    virtual void setTevStageNum(u8 num) { mTevStageNum = num; }
    virtual u8 getTevStageNum() const { return mTevStageNum; }
    virtual void setTevStage(u32 index, J2DTevStage stage) {
        mTevStage[index] = stage;
    }
    virtual J2DTevStage* getTevStage(u32 index) { return &mTevStage[index]; }
    virtual void setTevSwapModeInfo(u32 index, J2DTevSwapModeInfo info) {
        mTevStage[index].setTevSwapModeInfo(info);
    }
    virtual void setTevSwapModeTable(u32 index, J2DTevSwapModeTable table) {
        mTevSwapModeTable[index] = table;
    }
    virtual J2DTevSwapModeTable* getTevSwapModeTable(u32 index) {
        return &mTevSwapModeTable[index];
    }
    virtual void setIndTevStage(u32 index, J2DIndTevStage stage) {
        mIndTevStage[index] = stage;
    }
    virtual J2DIndTevStage* getIndTevStage(u32 index) {
        return &mIndTevStage[index];
    }
    virtual bool insertTexture(u32 index, ResTIMG const* p_timg) {
        return insertTexture(index, p_timg, NULL);
    }
    virtual bool insertTexture(u32, ResTIMG const*, JUTPalette*);
    virtual bool insertTexture(u32, JUTTexture*);
    virtual bool setTexture(u32, ResTIMG const*);
    virtual bool setTexture(u32, JUTTexture*);
    virtual bool removeTexture(u32);
    virtual bool setFont(ResFONT*);
    virtual bool setFont(JUTFont*);
    virtual bool setPalette(u32, ResTLUT const*);
    virtual bool prepareTexture(u8);
    virtual JUTTexture* getTexture(u32 index) {
        return index >= 8 ? NULL : mTexture[index];
    }
    virtual JUTPalette* getPalette(u32 index) {
        return index >= 8 ? NULL : mPalette[index];
    }
    virtual JUTFont* getFont() { return mFont; }
    virtual void shiftDeleteFlag(u8, bool);
    virtual void setUndeleteFlag(u8 flag) { mUndeleteFlag &= flag; }
    virtual void setFontUndeleteFlag() { mFontUndeleteFlag = false; }
    virtual ~J2DTevBlock16();

public:
    /* 0x004 */ u16 mTexNo[8];
    /* 0x014 */ u16 mFontNo;
    /* 0x016 */ J2DTevOrder mTevOrder[16];
    /* 0x056 */ J2DGXColorS10 mTevColor[4];
    /* 0x076 */ u8 mTevStageNum;
    /* 0x077 */ J2DTevStage mTevStage[16];
    /* 0x0F7 */ u8 field_0xf7;
    /* 0x0F8 */ JUtility::TColor mTevKColor[4];
    /* 0x108 */ u8 mTevKColorSel[16];
    /* 0x118 */ u8 mTevKAlphaSel[16];
    /* 0x128 */ J2DTevSwapModeTable mTevSwapModeTable[4];
    /* 0x12C */ J2DIndTevStage mIndTevStage[16];
    /* 0x16C */ JUTTexture* mTexture[8];
    /* 0x18C */ JUTPalette* mPalette[8];
    /* 0x1AC */ JUTFont* mFont;
    /* 0x1B0 */ u8 mUndeleteFlag;
    /* 0x1B1 */ bool mFontUndeleteFlag;
};  // Size: 0x1B4

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DAlphaCompInfo {
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 field_0x1;
    /* 0x2 */ u8 mRef0;
    /* 0x3 */ u8 mRef1;
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u8 field_0x6;
    /* 0x7 */ u8 field_0x7;
};

inline u16 J2DCalcAlphaCmp(s32 param_1, u32 param_2, u32 param_3) {
    return ((param_1) << 5) | ((param_2 & 0xff) << 3) | (param_3 & 0xff);
}

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DAlphaComp {
    J2DAlphaComp() {
        mAlphaCmp = j2dDefaultAlphaCmp;
        mRef0 = 0;
        mRef1 = 0;
    }
    J2DAlphaComp(const J2DAlphaCompInfo& info) {
        mAlphaCmp = J2DCalcAlphaCmp(info.field_0x0, info.mRef0, info.mRef1);
        mRef0 = info.field_0x1;
        mRef1 = info.field_0x4;
    }
    u8 getComp0() { return mAlphaCmp >> 5 & 7; }
    u8 getRef0() { return mRef0; }
    u8 getOp() { return mAlphaCmp >> 3 & 3; }
    u8 getComp1() { return mAlphaCmp & 7; }
    u8 getRef1() { return mRef1; }

    /* 0x0 */ u16 mAlphaCmp;
    /* 0x2 */ u8 mRef0;
    /* 0x3 */ u8 mRef1;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DBlendInfo {
    void operator=(J2DBlendInfo const& other) {
        mType = other.mType;
        mSrcFactor = other.mSrcFactor;
        mDstFactor = other.mDstFactor;
        mOp = other.mOp;
    }

    /* 0x0 */ u8 mType;
    /* 0x1 */ u8 mSrcFactor;
    /* 0x2 */ u8 mDstFactor;
    /* 0x3 */ u8 mOp;
};

extern const J2DBlendInfo j2dDefaultBlendInfo;

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DBlend {
    J2DBlend() { mBlendInfo = j2dDefaultBlendInfo; }
    J2DBlend(const J2DBlendInfo& info) { mBlendInfo = info; }
    void setBlendInfo(const J2DBlendInfo& info) { mBlendInfo = info; }
    u8 getType() { return mBlendInfo.mType; }
    u8 getSrcFactor() { return mBlendInfo.mSrcFactor; }
    u8 getDstFactor() { return mBlendInfo.mDstFactor; }
    u8 getOp() { return mBlendInfo.mOp; }

    /* 0x0 */ J2DBlendInfo mBlendInfo;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DPEBlock {
public:
    J2DPEBlock() { initialize(); }

    void initialize();
    void setGX();
    void setAlphaComp(J2DAlphaComp comp) { mAlphaComp = comp; }
    void setBlend(J2DBlend blend) { mBlend = blend; }
    void setDither(u8 dither) { mDither = dither; }

private:
    /* 0x0 */ J2DAlphaComp mAlphaComp;
    /* 0x4 */ J2DBlend mBlend;
    /* 0x8 */ u8 mDither;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DIndBlock {
public:
    virtual void initialize() {}
    virtual void setGX() {}
    virtual u32 getType() = 0;
    virtual void setIndTexStageNum(u8) {}
    virtual u8 getIndTexStageNum() const { return 0; }
    virtual void setIndTexOrder(u32, J2DIndTexOrder) {}
    virtual J2DIndTexOrder* getIndTexOrder(u32) { return NULL; }
    virtual void setIndTexMtx(u32, J2DIndTexMtx) {}
    virtual J2DIndTexMtx* getIndTexMtx(u32) { return NULL; }
    virtual void setIndTexCoordScale(u32, J2DIndTexCoordScale) {}
    virtual J2DIndTexCoordScale* getIndTexCoordScale(u32) { return NULL; }
    virtual ~J2DIndBlock() {}
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DIndBlockFull : public J2DIndBlock {
public:
    J2DIndBlockFull() { initialize(); }

    virtual void initialize();
    virtual void setGX();
    virtual u32 getType() { return 'IBLF'; }
    virtual void setIndTexStageNum(u8 num) { mIndTexStageNum = num; }
    virtual u8 getIndTexStageNum() const { return mIndTexStageNum; }
    virtual void setIndTexOrder(u32 index, J2DIndTexOrder order) {
        mIndTexOrder[index] = order;
    }
    virtual J2DIndTexOrder* getIndTexOrder(u32 index) {
        return &mIndTexOrder[index];
    }
    virtual void setIndTexMtx(u32 index, J2DIndTexMtx mtx) {
        mIndTexMtx[index] = mtx;
    }
    virtual J2DIndTexMtx* getIndTexMtx(u32 index) { return &mIndTexMtx[index]; }
    virtual void setIndTexCoordScale(u32 index, J2DIndTexCoordScale scale) {
        mTexCoordScale[index] = scale;
    }
    virtual J2DIndTexCoordScale* getIndTexCoordScale(u32 index) {
        return &mTexCoordScale[index];
    }
    virtual ~J2DIndBlockFull() {}

private:
    /* 0x04 */ u8 mIndTexStageNum;
    /* 0x05 */ J2DIndTexOrder mIndTexOrder[4];
    /* 0x10 */ J2DIndTexMtx mIndTexMtx[3];
    /* 0x64 */ J2DIndTexCoordScale mTexCoordScale[4];
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DIndBlockNull : public J2DIndBlock {
public:
    J2DIndBlockNull() {}

    virtual void setGX() {}
    virtual u32 getType() { return 'IBLN'; }
    virtual ~J2DIndBlockNull() {}
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTexGenBlock {
private:
    /* 0x00 */ u32 mTexGenNum;
    /* 0x04 */ J2DTexCoord mTexGenCoord[8];
    /* 0x24 */ J2DTexMtx* mTexMtx[8];

public:
    J2DTexGenBlock() { initialize(); }

    void initialize();
    void setGX();
    void setTexMtx(u32, J2DTexMtx&);
    void getTexMtx(u32, J2DTexMtx&);

    u32 getTexGenNum() const { return mTexGenNum; }
    void setTexGenNum(u32 num) { mTexGenNum = num; }
    void setTexCoord(u32 i, J2DTexCoord coord) { mTexGenCoord[i] = coord; }
    void setTexCoord(u32 i, const J2DTexCoord* coord) { mTexGenCoord[i] = *coord; }
    void setTexMtx(u32 i, J2DTexMtx* mtx) { mTexMtx[i] = mtx; }
    J2DTexMtx& getTexMtx(u32 i) { return *mTexMtx[i]; }
    J2DTexCoord& getTexCoord(u32 i) { return mTexGenCoord[i]; }

    virtual ~J2DTexGenBlock();
};  // Size: 0x48

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DColorBlock {
private:
    /* 0x00 */ JUtility::TColor mMatColor[2];
    /* 0x08 */ u8 mColorChanNum;
    /* 0x0A */ J2DColorChan mColorChan[4];
    /* 0x12 */ u8 mCullMode;

public:
    J2DColorBlock() { initialize(); }

    void initialize();
    void setGX();

    virtual ~J2DColorBlock() {}

    JUtility::TColor* getMatColor(u32 i) { return &mMatColor[i]; }
    J2DColorChan* getColorChan(u32 i) { return &mColorChan[i]; }
    void setCullMode(u8 mode) { mCullMode = mode; }
    void setColorChanNum(u8 num) { mColorChanNum = num; }
    void setMatColor(u32 i, JUtility::TColor color) { mMatColor[i] = color; }
    void setColorChan(u32 i, const J2DColorChan& color) { mColorChan[i] = color; }
};

#endif /* J2DMATBLOCK_H */
