#ifndef J2DTEXTBOX_H
#define J2DTEXTBOX_H

#include "JSystem/J2DGraph/J2DMaterial.h"
#include "JSystem/J2DGraph/J2DPane.h"

class J2DMaterial;
class JUTFont;

enum J2DTextBoxVBinding {
    /* 0x0 */ VBIND_CENTER,
    /* 0x1 */ VBIND_BOTTOM,
    /* 0x2 */ VBIND_TOP
};

enum J2DTextBoxHBinding {
    /* 0x0 */ HBIND_CENTER,
    /* 0x1 */ HBIND_RIGHT,
    /* 0x2 */ HBIND_LEFT
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DTbxBlockHeader {
    /* 0x00 */ u32 mTag;
    /* 0x04 */ s32 mSize;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DTextBoxInfo {
    /* 0x00 */ u16 field_0x0;
    /* 0x02 */ u16 field_0x2;
    /* 0x04 */ u16 mMaterialNum;
    /* 0x06 */ s16 mCharSpace;
    /* 0x08 */ s16 mLineSpace;
    /* 0x0A */ u16 mFontSizeX;
    /* 0x0C */ u16 mFontSizeY;
    /* 0x0E */ u8 mHBind;
    /* 0x0F */ u8 mVBind;
    /* 0x10 */ u32 mCharColor;
    /* 0x14 */ u32 mGradColor;
    /* 0x18 */ u8 mConnected;
    /* 0x19 */ char field_0x19[3];
    /* 0x1C */ u16 field_0x1c;
    /* 0x1E */ u16 field_0x1e;
};  // Size: 0x20

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTextBox : public J2DPane {
public:
    struct TFontSize {
        /* 0x0 */ f32 mSizeX;
        /* 0x4 */ f32 mSizeY;
    };

    virtual ~J2DTextBox();
    virtual u16 getTypeID() const;
    virtual void resize(f32, f32);
    virtual bool setConnectParent(bool);
    virtual void drawSelf(f32, f32, Mtx*);
    virtual void drawSelf(f32, f32);
    virtual bool isUsed(ResTIMG const*);
    virtual bool isUsed(ResFONT const*);
    virtual void rewriteAlpha();
    virtual void draw(f32, f32);
    virtual void draw(f32, f32, f32, J2DTextBoxHBinding);
    virtual void setFont(JUTFont*);
    virtual JUTFont* getFont() const { return mFont; }
    virtual bool setBlack(JUtility::TColor);
    virtual bool setWhite(JUtility::TColor white) { mWhiteColor = white; return true; }
    virtual bool setBlackWhite(JUtility::TColor black, JUtility::TColor white) {
        mBlackColor = black;
        mWhiteColor = white;
        return true;
    }
    virtual JUtility::TColor getBlack() const {
        return mBlackColor;
    }
    virtual JUtility::TColor getWhite() const {
        return mWhiteColor;
    }
    virtual J2DMaterial* getMaterial() const { return NULL; }

    J2DTextBox(u64, JGeometry::TBox2<f32> const&, ResFONT const*, char const*, s16,
                              J2DTextBoxHBinding, J2DTextBoxVBinding);
    J2DTextBox(J2DPane*, JSURandomInputStream*, JKRArchive*);
    J2DTextBox(J2DPane*, JSURandomInputStream*, u32, J2DMaterial*);
    J2DTextBox();
    void initiate(ResFONT const*, char const*, s16, J2DTextBoxHBinding,
                                 J2DTextBoxVBinding);
    void private_readStream(J2DPane*, JSURandomInputStream*, JKRArchive*);
    char* getStringPtr() const;
    s32 setString(s16, char const*, ...);
    s32 setString(char const*, ...);

    void setFontSize(f32 sizeX, f32 sizeY) {
        f32 x;
        if (sizeX > 0.0f) {
            x = sizeX;
        } else {
            x = 0.0f;
        }
        mFontSizeX = x;

        f32 y;
        if (sizeY > 0.0f) {
            y = sizeY;
        } else {
            y = 0.0f;
        }
        mFontSizeY = y;
    }

    void setFontSize(TFontSize size) { setFontSize(size.mSizeX, size.mSizeY); }
    void getFontSize(TFontSize& size) const {
        size.mSizeX = mFontSizeX;
        size.mSizeY = mFontSizeY;
    }

    void setFontColor(JUtility::TColor i_charColor, JUtility::TColor i_GradientColor) {
        mCharColor = i_charColor;
        mGradientColor = i_GradientColor;
    }

    void setCharColor(JUtility::TColor i_charColor) {
        mCharColor.set(i_charColor);
    }

    void setGradColor(JUtility::TColor i_GradientColor) {
        mGradientColor.set(i_GradientColor);
    }

    void setCharSpace(f32 space) { mCharSpacing = space; }
    f32 getCharSpace() const { return mCharSpacing; }
    void setLineSpace(f32 space) { mLineSpacing = space; }
    f32 getLineSpace() const { return mLineSpacing; }

    J2DTextBoxVBinding getVBinding() const {
        return (J2DTextBoxVBinding)(mFlags & 3);
    }

    J2DTextBoxHBinding getHBinding() const {
        return (J2DTextBoxHBinding)((mFlags >> 2) & 3);
    }

    JUtility::TColor getCharColor() { return mCharColor; }
    JUtility::TColor getGradColor() { return mGradientColor; }
    u16 getStringAllocByte() const { return mStringLength; }

// private:
    /* 0x0100 */ JUTFont* mFont;
    /* 0x0104 */ JUtility::TColor mCharColor;
    /* 0x0108 */ JUtility::TColor mGradientColor;
    /* 0x010C */ f32 field_0x10c;
    /* 0x0110 */ f32 field_0x110;
    /* 0x0114 */ f32 mCharSpacing;
    /* 0x0118 */ f32 mLineSpacing;
    /* 0x011C */ f32 mFontSizeX;
    /* 0x0120 */ f32 mFontSizeY;
    /* 0x0124 */ char* mStringPtr;
    /* 0x0128 */ JUtility::TColor mWhiteColor;
    /* 0x012C */ JUtility::TColor mBlackColor;
    /* 0x0130 */ u8 mFlags;
    /* 0x0131 */ bool mTextFontOwned;
    /* 0x0132 */ u16 mStringLength;
#if PLATFORM_GCN
    /* 0x0134 */ u8 field_0x134[4];
#endif
};  // Size: 0x138

#endif /* J2DTEXTBOX_H */
