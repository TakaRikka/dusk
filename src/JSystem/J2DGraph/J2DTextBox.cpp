#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/J2DGraph/J2DPrint.h"
#include "JSystem/JSupport/JSURandomInputStream.h"
#include "JSystem/JUtility/JUTResFont.h"

J2DTextBox::J2DTextBox()
    : mFont(NULL), mCharColor(), mGradientColor(), mStringPtr(NULL), mWhiteColor(), mBlackColor() {
    initiate(NULL, NULL, 0, HBIND_LEFT, VBIND_TOP);
}

J2DTextBox::J2DTextBox(J2DPane* p_pane, JSURandomInputStream* p_stream, JKRArchive* p_archive)
    : mFont(NULL), mCharColor(), mGradientColor(), mStringPtr(NULL), mWhiteColor(), mBlackColor() {
    private_readStream(p_pane, p_stream, p_archive);
}

J2DTextBox::J2DTextBox(J2DPane* p_pane, JSURandomInputStream* p_stream, u32 param_2,
                       J2DMaterial* p_material)
    : mFont(NULL), mCharColor(), mGradientColor(), mStringPtr(NULL), mWhiteColor(), mBlackColor() {
    J2DTextBoxInfo info;

    int startPos = p_stream->getPosition();

    J2DTbxBlockHeader header;
    p_stream->read(&header, 8);
    mKind = 'TBX1';

    int panHeaderPos = p_stream->getPosition();
    J2DTbxBlockHeader panHeader;
    p_stream->peek(&panHeader, 8);
    makePaneExStream(p_pane, p_stream);
    p_stream->seek(panHeaderPos + panHeader.mSize, JSUStreamSeekFrom_SET);

    p_stream->read(&info, 0x20);
    J2DMaterial* mat = NULL;

    if (info.mMaterialNum != 0xFFFF) {
        mat = &p_material[info.mMaterialNum];

        if (mat != NULL) {
            if (mat->getTevBlock() != NULL) {
                JUTFont* font = mat->getTevBlock()->getFont();

                if (font != NULL) {
                    mFont = font;
                    mat->getTevBlock()->setFontUndeleteFlag();
                }
            }
            mAlpha = mat->getColorBlock()->getMatColor(0)->a;
        }
    }

    mCharSpacing = info.mCharSpace;
    mLineSpacing = info.mLineSpace;
    mFontSizeX = info.mFontSizeX;
    mFontSizeY = info.mFontSizeY;
    mFlags = (info.mHBind << 2) | info.mVBind;
    mCharColor = JUtility::TColor(info.mCharColor);
    mGradientColor = JUtility::TColor(info.mGradColor);
    setConnectParent(info.mConnected);

    u16 strLength = 0;
    if (!(param_2 & 0x2000000)) {
        strLength = info.field_0x1c;
        if ((s16)info.field_0x1c == -1) {
            strLength = info.field_0x1e + 1;
        }
    }

    mStringLength = 0;
    mStringPtr = NULL;

    if (strLength != 0) {
        mStringPtr = new char[strLength];
    }

    if (mStringPtr != NULL) {
        mStringLength = strLength;
        int temp_r0 = (u16)strLength - 1;
        u16 var_r26_2 = info.field_0x1e;

        if (temp_r0 < var_r26_2) {
            var_r26_2 = (u16)temp_r0;
        }

        p_stream->peek(mStringPtr, var_r26_2);
        mStringPtr[var_r26_2] = 0;
    }

    p_stream->skip(info.field_0x1e);
    p_stream->seek(startPos + header.mSize, JSUStreamSeekFrom_SET);
    mBlackColor = JUtility::TColor(0);
    mWhiteColor = JUtility::TColor(0xFFFFFFFF);

    if (mat != NULL && mat->getTevBlock() != NULL) {
        if (mat->getTevBlock()->getTevStageNum() != 1) {
            J2DGXColorS10* color0p = mat->getTevBlock()->getTevColor(0);
            GXColorS10 color0;
            color0.r = color0p->r;
            color0.g = color0p->g;
            color0.b = color0p->b;
            color0.a = color0p->a;

            J2DGXColorS10* color1p = mat->getTevBlock()->getTevColor(1);
            GXColorS10 color1;
            color1.r = color1p->r;
            color1.g = color1p->g;
            color1.b = color1p->b;
            color1.a = color1p->a;

            mBlackColor = JUtility::TColor(((u8)color0.r << 0x18) | ((u8)color0.g << 0x10) |
                                           ((u8)color0.b << 8) | (u8)color0.a);
            mWhiteColor = JUtility::TColor(((u8)color1.r << 0x18) | ((u8)color1.g << 0x10) |
                                           ((u8)color1.b << 8) | (u8)color1.a);
        }
    }

    field_0x10c = 0.0f;
    field_0x110 = 0.0f;
    mTextFontOwned = true;
}

/* 802FFBC4-802FFC58 2FA504 0094+00 0/0 1/1 0/0 .text
 * __ct__10J2DTextBoxFUxRCQ29JGeometry8TBox2<f>PC7ResFONTPCcs18J2DTextBoxHBinding18J2DTextBoxVBinding
 */
J2DTextBox::J2DTextBox(u64 tag, JGeometry::TBox2<f32> const& bounds, ResFONT const* p_font,
                       char const* string, s16 strLength, J2DTextBoxHBinding hBind,
                       J2DTextBoxVBinding vBind)
    : J2DPane(tag, bounds), mFont(NULL), mCharColor(), mGradientColor(), mStringPtr(NULL),
      mWhiteColor(), mBlackColor() {
    initiate(p_font, string, strLength, hBind, vBind);
}

void J2DTextBox::initiate(ResFONT const* p_font, char const* string, s16 length,
                          J2DTextBoxHBinding hBind, J2DTextBoxVBinding vBind) {
    if (p_font != NULL) {
        mFont = new JUTResFont(p_font, NULL);
    }

    mCharColor.set(0xFFFFFFFF);
    mGradientColor.set(0xFFFFFFFF);
    mBlackColor = JUtility::TColor(0);
    mWhiteColor = JUtility::TColor(0xFFFFFFFF);

    mFlags = (hBind << 2) | vBind;
    mStringLength = 0;
    mStringPtr = NULL;

    if (string != NULL && length != 0) {
        u32 len = strlen(string);
        u16 stringLen = length;

        if (length == -1) {
            if (len >= 0xFFFF) {
                len = 0xFFFF - 1;
            }
            stringLen = len + 1;
        }

        mStringPtr = new char[stringLen];

        if (stringLen != 0 && mStringPtr != NULL) {
            strncpy(mStringPtr, string, stringLen - 1);
            mStringPtr[stringLen - 1] = 0;
            mStringLength = stringLen;
        }
    }

    field_0x10c = 0.0f;
    field_0x110 = 0.0f;
    mCharSpacing = 0.0f;

    if (mFont == NULL) {
        mLineSpacing = 0.0f;
        mFontSizeX = 0.0f;
        mFontSizeY = 0.0f;
    } else {
        mLineSpacing = mFont->getLeading();
        mFontSizeX = mFont->getWidth();
        mFontSizeY = mFont->getHeight();
    }

    mKind = 'TBX1';
    mTextFontOwned = true;
}

void J2DTextBox::private_readStream(J2DPane* p_pane, JSURandomInputStream* p_stream,
                                    JKRArchive* p_archive) {
    int position = p_stream->getPosition();

    J2DTbxBlockHeader header;
    p_stream->read(&header, 8);
    mKind = header.mTag;

    makePaneStream(p_pane, p_stream);
    u8 spA = p_stream->readU8();

    ResFONT* fontPtr = (ResFONT*)getPointer(p_stream, 'FONT', p_archive);
    if (fontPtr != NULL) {
        mFont = new JUTResFont(fontPtr, NULL);
    }

    mCharColor.set(p_stream->read32b());
    mGradientColor.set(p_stream->read32b());
    mFlags = p_stream->read8b();
    mCharSpacing = p_stream->readS16();
    mLineSpacing = p_stream->readS16();
    mFontSizeX = p_stream->read16b();
    mFontSizeY = p_stream->read16b();

    s16 stringLen = p_stream->read16b();
    mStringPtr = new char[stringLen + 1];

    if (mStringPtr != NULL) {
        p_stream->read(mStringPtr, stringLen);
        mStringPtr[stringLen] = 0;
        mStringLength = stringLen + 1;
    } else {
        p_stream->skip(stringLen);
        mStringLength = 0;
    }

    spA -= 10;
    if (spA != 0) {
        u8 sp8 = p_stream->read8b();
        if (sp8 != 0) {
            setConnectParent(true);
        }

        spA--;
    }

    mBlackColor = JUtility::TColor(0);
    mWhiteColor = JUtility::TColor(0xFFFFFFFF);

    if (spA != 0) {
        mBlackColor.set(p_stream->read32b());
        spA--;
    }

    if (spA != 0) {
        mWhiteColor.set(p_stream->read32b());
    }

    field_0x10c = 0.0f;
    field_0x110 = 0.0f;
    p_stream->seek(position + header.mSize, JSUStreamSeekFrom_SET);
    mTextFontOwned = true;
}

J2DTextBox::~J2DTextBox() {
    if (mTextFontOwned) {
        delete mFont;
    }

    delete[] mStringPtr;
}

void J2DTextBox::setFont(JUTFont* p_font) {
    if (p_font) {
        if (mTextFontOwned) {
            delete mFont;
        }
        mFont = p_font;
        mTextFontOwned = false;
    }
}

void J2DTextBox::draw(f32 posX, f32 posY) {
    Mtx m;

    if (isVisible()) {
        J2DPrint print(mFont, mCharSpacing, mLineSpacing, mCharColor, mGradientColor, mBlackColor,
                       mWhiteColor);
        print.setFontSize(mFontSizeX, mFontSizeY);
        makeMatrix(posX, posY, 0.0f, 0.0f);

        GXLoadPosMtxImm(mPositionMtx, GX_PNMTX0);
        GXSetCurrentMtx(0);
        GXSetNumIndStages(0);
        for (int i = 0; i < 16; i++) {
            GXSetTevDirect((GXTevStageID)i);
        }
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);

        if (mStringPtr != NULL) {
            print.print(0.0f, 0.0f, mAlpha, "%s", mStringPtr);
        }
        MTXIdentity(m);
        GXLoadPosMtxImm(m, 0);
    }
}

void J2DTextBox::draw(f32 posX, f32 posY, f32 param_2, J2DTextBoxHBinding hBind) {
    Mtx m;

    if (isVisible()) {
        J2DPrint print(mFont, mCharSpacing, mLineSpacing, mCharColor, mGradientColor, mBlackColor,
                       mWhiteColor);
        print.setFontSize(mFontSizeX, mFontSizeY);
        makeMatrix(posX, posY, 0.0f, 0.0f);

        GXLoadPosMtxImm(mPositionMtx, GX_PNMTX0);
        GXSetCurrentMtx(0);
        GXSetNumIndStages(0);
        for (int i = 0; i < 16; i++) {
            GXSetTevDirect((GXTevStageID)i);
        }
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);

        if (mStringPtr != NULL) {
            print.printReturn(mStringPtr, param_2, 0.0f, hBind, VBIND_TOP, 0.0f, -mFontSizeY,
                              mAlpha);
        }
        MTXIdentity(m);
        GXLoadPosMtxImm(m, 0);
    }
}

char* J2DTextBox::getStringPtr() const {
    return mStringPtr;
}

s32 J2DTextBox::setString(char const* string, ...) {
    va_list args;
    va_start(args, string);

    delete[] mStringPtr;

    u32 len = strlen(string);

    if (len >= 0xFFFF) {
        len = 0xFFFF - 1;
    }

    mStringLength = 0;
    char* tmp = new char[len + 1];
    mStringPtr = tmp;

    if (mStringPtr) {
        mStringLength = len + 1;
        strcpy(mStringPtr, string);
    }

    va_end(args);
    return len;
}

s32 J2DTextBox::setString(s16 length, char const* string, ...) {
    va_list args;
    va_start(args, string);

    delete[] mStringPtr;
    mStringPtr = NULL;

    u32 len = strlen(string);
    u16 stringLen = length;

    if (length == -1) {
        if (len >= 0xFFFF) {
            len = 0xFFFF - 1;
        }
        stringLen = len + 1;
    }

    mStringLength = 0;

    if (stringLen != 0) {
        mStringPtr = new char[stringLen];
    }

    if (mStringPtr != NULL) {
        strncpy(mStringPtr, string, stringLen - 1);
        mStringPtr[stringLen - 1] = 0;
        mStringLength = stringLen;
    }

    va_end(args);
    return len;
}

bool J2DTextBox::setConnectParent(bool connected) {
    if (getPaneTree()->getParent() == NULL) {
        return false;
    }

    if (getPaneTree()->getParent()->getObject()->getTypeID() != 0x11) {
        return false;
    }

    mConnected = connected;
    return connected;
}

void J2DTextBox::drawSelf(f32 param_0, f32 param_1) {
    Mtx identity;
    MTXIdentity(identity);

    drawSelf(param_0, param_1, &identity);
}

void J2DTextBox::drawSelf(f32 param_0, f32 param_1, Mtx* p_mtx) {
    Mtx m;

    J2DPrint print(mFont, mCharSpacing, mLineSpacing, mCharColor, mGradientColor, mBlackColor,
                   mWhiteColor);
    print.setFontSize(mFontSizeX, mFontSizeY);
    MTXConcat(*p_mtx, mGlobalMtx, m);

    GXLoadPosMtxImm(m, GX_PNMTX0);
    GXSetNumIndStages(0);
    for (int i = 0; i < 16; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);

    print.locate(param_0 + mBounds.i.x, param_1 + mBounds.i.y);
    if (mStringPtr != NULL) {
        print.printReturn(mStringPtr, mBounds.getWidth() + 0.0001f, mBounds.getHeight(),
                          getHBinding(), getVBinding(), field_0x10c, field_0x110, mColorAlpha);
    }
}

void J2DTextBox::resize(f32 x, f32 y) {
    if (mConnected && getPaneTree() != NULL && getPaneTree()->getParent() != NULL) {
        J2DPane* obj = getPaneTree()->getParent()->getObject();

        if (obj->getTypeID() == 0x11) {
            f32 obj_x = obj->getWidth() + (x - getWidth());
            f32 obj_y = obj->getHeight() + (y - getHeight());
            obj->resize(obj_x, obj_y);
            return;
        }
    }

    J2DPane::resize(x, y);
}

bool J2DTextBox::isUsed(ResFONT const* p_font) {
    if (mFont != NULL && mFont->getResFont() == p_font) {
        return true;
    }

    return J2DPane::isUsed(p_font);
}

u16 J2DTextBox::getTypeID() const {
    return 19;
}

bool J2DTextBox::isUsed(ResTIMG const* p_timg) {
    return J2DPane::isUsed(p_timg);
}

void J2DTextBox::rewriteAlpha() {
    /* empty function */
}
