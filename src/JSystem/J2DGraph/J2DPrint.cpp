#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DPrint.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTFont.h"
#include <stdio.h>
#include <stdlib.h>

char* J2DPrint::mStrBuff;

static bool sStrBufInitialized;

size_t J2DPrint::mStrBuffSize;

static u8 data_8045158C[4];

f32 J2DPrint_print_alpha_va(J2DPrint* pPrint, u8 alpha, const char* fmt, va_list args) {
    pPrint->initchar();
    s32 length = vsnprintf(J2DPrint::mStrBuff, J2DPrint::mStrBuffSize, fmt, args);

    J2DPrint::TSize size;
    if (length < 0) {
        return 0.0f;
    } else if (length >= J2DPrint::mStrBuffSize) {
        length = J2DPrint::mStrBuffSize - 1;
        data_8045158C[0] = 1;
    }

    pPrint->parse((u8*)J2DPrint::mStrBuff, length, 0x80000000 - 1, NULL, size, alpha, true);
    return size.field_0x0;
}

J2DPrint::J2DPrint(JUTFont* pFont, JUtility::TColor charColor, JUtility::TColor gradColor) {
    private_initiate(pFont, 0.0f, 0.0f, charColor, gradColor, 0, 0xFFFFFFFF, true);
}

/* 802F4394-802F4420 2EECD4 008C+00 0/0 6/6 0/0 .text
 * __ct__8J2DPrintFP7JUTFontffQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColor
 */
J2DPrint::J2DPrint(JUTFont* pFont, f32 charSpacing, f32 lineSpacing, JUtility::TColor charColor,
                   JUtility::TColor gradColor, JUtility::TColor blackColor, JUtility::TColor whiteColor) {
    private_initiate(pFont, charSpacing, lineSpacing, charColor, gradColor, blackColor, whiteColor, false);
}

J2DPrint::~J2DPrint() {}

void J2DPrint::initiate() {
    if (mFont != NULL) {
        mFont->setGX(mBlackColor, mWhiteColor);
    }
}

/* 802F4468-802F4658 2EEDA8 01F0+00 1/1 0/0 0/0 .text
 * private_initiate__8J2DPrintFP7JUTFontffQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorb
 */
void J2DPrint::private_initiate(JUTFont* pFont, f32 charSpacing, f32 lineSpacing, JUtility::TColor charColor,
                                JUtility::TColor gradColor, JUtility::TColor blackColor,
                                JUtility::TColor whiteColor, bool param_7) {
    if (mStrBuff == NULL) {
        setBuffer(0x400);
    }

    mFont = pFont;
    mCharSpacing = charSpacing;
    mLineSpacing = 32.0f;

    if (mFont != NULL) {
        mLineSpacing = param_7 == false ? lineSpacing : mFont->getLeading();
    }

    field_0x5a = 1;
    locate(0.0f, 0.0f);

    mCharColor = charColor;
    mGradColor = gradColor;
    mBlackColor = blackColor;
    mWhiteColor = whiteColor;

    if (mFont == NULL) {
        field_0x58 = 80;
    } else {
        field_0x58 = mFont->getWidth() << 2;
    }

    if (mFont != NULL) {
        setFontSize();
        mFont->setGX(mBlackColor, mWhiteColor);
    }

    initchar();
}

u8* J2DPrint::setBuffer(size_t size) {
    JUT_ASSERT(335, size > 0);

    u8* u8Buff = (u8*)mStrBuff;
    if (sStrBufInitialized) {
        delete mStrBuff;
    }

    mStrBuff = new (JKRGetSystemHeap(), 0) char[size];
    mStrBuffSize = size;
    sStrBufInitialized = true;
    return u8Buff;
}

void J2DPrint::setFontSize() {
    if (mFont != NULL) {
        mFontSizeX = mFont->getCellWidth();
        mFontSizeY = mFont->getCellHeight();
    }
}

void J2DPrint::locate(f32 cursorH, f32 cursorV) {
    field_0x24 = cursorH;
    field_0x28 = cursorV;
    mCursorH = cursorH;
    mCursorV = cursorV;
    field_0x34 = 0.0f;
}

f32 J2DPrint::print(f32 cursorH, f32 cursorV, u8 alpha, char const* fmt, ...) {
    locate(cursorH, cursorV);

    va_list args;
    va_start(args, fmt);
    f32 var_f31 = print_va(alpha, fmt, args);
    va_end(args);

    return var_f31;
}

void J2DPrint::printReturn(char const* pString, f32 param_1, f32 param_2,
                           J2DTextBoxHBinding hBind, J2DTextBoxVBinding vBind, f32 param_5,
                           f32 param_6, u8 alpha) {
    if (mFont != NULL) {
        initchar();
        field_0x24 = mCursorH;
        field_0x28 = mCursorV;

        size_t length = strlen(pString);
        if (length >= mStrBuffSize) {
            length = mStrBuffSize - 1;
            data_8045158C[0] = 1;
        }

        f32 var_f26 = mLineSpacing;

        u16 local_2b0[260];
        TSize size;
        f32 var_f31 = parse((u8*)pString, length, param_1, local_2b0, size, alpha, false);
        f32 dVar12 = mFont->getAscent()*(mFontSizeY / mFont->getCellHeight());
        var_f31 += dVar12;

        switch (vBind) {
        case VBIND_TOP:
            break;
        case VBIND_BOTTOM:
            param_6 += (s32)(param_2 - var_f31 - 0.5f);
            break;
        case VBIND_CENTER:
            param_6 += (s32)(param_2 - var_f31 - 0.5f) / 2;
        default:
            break;
        }

        for (int i = 0; local_2b0[i] != 0xFFFF; i++) {
            switch (hBind) {
            case HBIND_LEFT:
                local_2b0[i] = 0;
                break;
            case HBIND_RIGHT:
                local_2b0[i] = param_1 - local_2b0[i];
                break;
            case HBIND_CENTER:
                local_2b0[i] = (param_1 - local_2b0[i]) / 2;
                break;
            }
        }

        initchar();

        mCursorH += param_5;
        mCursorV += param_6 + dVar12;
        field_0x24 = mCursorH;
        field_0x28 = mCursorV;
        parse((u8*)pString, length, param_1, local_2b0, size, alpha, true);
    }
}

f32 J2DPrint::parse(const u8* pString, int length, int param_2, u16* param_3,
                    TSize& size, u8 alpha, bool param_6) {
    if (mFont == NULL) {
        return 0.0f;
    }

    const u8* pStringStart = pString;
    u16 someIndex = 0;

    f32 prevCursorH = mCursorH;
    f32 prevCursorV = mCursorV;
    f32 f31 = prevCursorH;
    f32 local_a8 = prevCursorV;
    int iCharacter = *(pString++);

    f32 f29 = mCursorH;
    f32 local_ac = mCursorH;
    f32 local_b0 = mCursorV;
    f32 local_b4 = mCursorV;

    JUtility::TColor local_b8 = field_0x8;
    JUtility::TColor local_bc = field_0xc;
    f32 local_c0;

    local_b8.a = local_b8.a * alpha / 0xFF;
    local_bc.a = local_bc.a * alpha / 0xFF;
    mFont->setGradColor(local_b8, field_0x22 ? local_bc : local_b8);

    do {
        bool b2ByteCharacter = false;
        bool r25;
        if (mFont->isLeadByte(iCharacter)) {
            iCharacter = (iCharacter << 8) | *(pString++);
            b2ByteCharacter = true;
        }

        if (iCharacter == 0 || ((uintptr_t)pString - (uintptr_t)pStringStart) > length) {
            if (!param_6 && param_3 != NULL) {
                param_3[someIndex] = 0.5f + f31;
            }
            someIndex++;
            break;
        } else {
            r25 = true;
            local_c0 = mCursorH;
            if (iCharacter < ' ') {
                if (iCharacter == '\x1B') {
                    u16 code = doEscapeCode(&pString, alpha);
                    if (code == 'HM') {
                        if (!param_6 && param_3 != NULL) {
                            param_3[someIndex] = 0.5f + f31;
                        }

                        mCursorH = prevCursorH;
                        mCursorV = prevCursorV;

                        someIndex++;
                        if (someIndex == 0x100) {
                            break;
                        }

                        f31 = 0.0f;
                    }

                    if (code != 0) {
                        r25 = false;
                    }
                } else {
                    doCtrlCode(iCharacter);
                    r25 = false;
                    if (iCharacter == 10) {
                        if (!param_6 && param_3 != NULL) {
                            param_3[someIndex] = 0.5f + f31;
                        }

                        someIndex++;
                        if (someIndex == 0x100) {
                            break;
                        }

                        f31 = 0.0f;
                    }
                }
            } else if (b2ByteCharacter && ((uintptr_t)pString - (uintptr_t)pStringStart > (u32)length)) {
                if (!param_6 && param_3 != NULL) {
                    param_3[someIndex] = 0.5f + f31;
                }
                someIndex++;
                break;
            } else {
                if (mFont->isFixed()) {
                    field_0x34 = mFont->getFixedWidth();
                } else {
                    JUTFont::TWidth width;
                    mFont->getWidthEntry(iCharacter, &width);
                    field_0x34 = width.field_0x1;
                }

                field_0x34 *= mScaleX / mFont->getCellWidth();

                f32 fVar6 = ((mCursorH + field_0x34) - field_0x24);
                fVar6 = 10000.0f * fVar6;
                f32 local_c8 = s32(fVar6) / 10000.0f;
                if (local_c8 > param_2 && mCursorH > prevCursorH) {
                    u32 characterSize;
                    if (b2ByteCharacter) {
                        characterSize = 2;
                    } else {
                        characterSize = 1;
                    }

                    pString -= characterSize;
                    mCursorV += field_0x14;

                    if (!param_6 && (param_3 != NULL)) {
                        param_3[someIndex] = 0.5f + f31;
                    }

                    someIndex++;
                    if (someIndex == 0x100) {
                        break;
                    }

                    mCursorH = field_0x24;
                    f31 = 0.0f;
                    r25 = false;
                } else {
                    if (param_6) {
                        if (param_3 != NULL) {
                            mFont->drawChar_scale(mCursorH + (f32)(s16)param_3[someIndex], mCursorV, (s32)mScaleX, (s32)mScaleY, iCharacter, true);
                        } else {
                            mFont->drawChar_scale(mCursorH, mCursorV, (s32)mScaleX, (s32)mScaleY, iCharacter, true);
                        }
                    }
                    mCursorH += field_0x34;
                }
            }
        }

        if (r25) {
            if (mCursorH - prevCursorH > f31) {
                f31 = mCursorH - prevCursorH;
            }
            mCursorH += field_0x10;
            field_0x34 += field_0x10;
            
            f32 local_cc = (mScaleY / mFont->getHeight()) * mFont->getDescent();
            if (local_a8 < mCursorV + local_cc) {
                local_a8 = mCursorV + local_cc;
            }
            if (mCursorH > local_ac) {
                local_ac = mCursorH;
            }
            if (mCursorH < f29) {
                f29 = mCursorH;
            }
            if (local_c0 < f29) {
                f29 = local_c0;
            }
            if (mCursorV > local_b4) {
                local_b4 = mCursorV;
            }
            if (mCursorV < local_b0) {
                local_b0 = mCursorV;
            }
        }

        iCharacter = *(pString++);
    } while (true);

    if (param_3 != NULL) {
        param_3[someIndex] = 0xFFFF;
    }

    size.field_0x0 = local_ac - f29;
    size.field_0x4 = local_b4 - local_b0 + mFont->getLeading();

    if (!param_6) {
        mCursorH = prevCursorH;
        mCursorV = prevCursorV;
    }

    return local_a8 - prevCursorV;
}

void J2DPrint::doCtrlCode(int iCharacter) {
    switch (iCharacter) {
    case '\b':
        mCursorH -= field_0x34;
        field_0x34 = 0.0f;
        break;
    case '\t':
        if (field_0x20 > 0) {
            f32 fVar1 = mCursorH;
            mCursorH = field_0x20 + field_0x20 * ((int)mCursorH / field_0x20);
            field_0x34 = mCursorH - fVar1;
        }
        break;
    case '\n':
        field_0x34 = 0.0f;
        mCursorH = field_0x24;
        mCursorV += field_0x14;
        break;
    case '\r':
        field_0x34 = 0.0f;
        mCursorH = field_0x24;
        break;
    case '\x1C':
        mCursorH += 1.0f;
        break;
    case '\x1D':
        mCursorH -= 1.0f;
        break;
    case '\x1E':
        mCursorV -= 1.0f;
        break;
    case '\x1F':
        mCursorV += + 1.0f;
    }
}

u16 J2DPrint::doEscapeCode(const u8** ppu8String, u8 alpha) {
    const u8* pStringStart = *ppu8String;
    u16 code = 0;

    u16 codeLower;
    for (int i = 0; i < 2; i++) {
        if (mFont->isLeadByte(**ppu8String)) {
            codeLower = ((**ppu8String) << 8) | (*ppu8String)[1];
            (*ppu8String) += 2;
        } else {
            codeLower = **ppu8String;
            (*ppu8String)++;
        }

        if (codeLower >= 0x80 || codeLower < 0x20) {
            *ppu8String = pStringStart;
            return 0;
        }

        code = (code << 8) | codeLower;
    }

    JUtility::TColor local_40 = field_0x8;
    JUtility::TColor local_44 = field_0xc;
    f32 scaleNumber;

    switch(code) {
    case 'CU':  // Cursor Up
        mCursorV -= getNumberF32(ppu8String, 1.0f, 0.0f, 10);
        break;
    case 'CD':  // Cursor Down
        mCursorV += getNumberF32(ppu8String, 1.0f, 0.0f, 10);
        break;
    case 'CL':  // Cursor Left
        mCursorH -= getNumberF32(ppu8String, 1.0f, 0.0f, 10);
        break;
    case 'CR':  // Cursor Right
        mCursorH += getNumberF32(ppu8String, 1.0f, 0.0f, 10);
        break;
    case 'LU':  // Line Up?
        mCursorV -= field_0x14;
        break;
    case 'LD':  // Line Down?
        mCursorV += field_0x14;
        break;
    case 'ST': {
        s32 number = getNumberS32(ppu8String, field_0x20, field_0x20, 10);
        if (number > 0) {
            field_0x20 = number;
        }
        break;
    }
    case 'CC': {  // Character Color
        field_0x8 = getNumberS32(ppu8String, (u32)mCharColor, (u32)field_0x8, 16);
        local_40 = field_0x8;
        local_40.a = local_40.a * alpha / 0xff;
        local_44.a = local_44.a * alpha / 0xff;

        mFont->setGradColor(local_40, field_0x22 != 0 ? local_44 : local_40);
        break;
    }
    case 'GC': {  // Gradient Color
        field_0xc = getNumberS32(ppu8String, (u32)mGradColor, (u32)field_0xc, 16);
        local_44 = field_0xc;
        local_40.a = local_40.a * alpha / 0xFF;
        local_44.a = local_44.a * alpha / 0xFF;

        mFont->setGradColor(local_40, field_0x22 != 0 ? local_44 : local_40);
        break;
    }
    case 'FX': {  // Font Scale X
        scaleNumber = getNumberF32(ppu8String, mFontSizeX, mScaleX, 10);
        if (scaleNumber >= 0) {
            mScaleX = scaleNumber;
        }
        break;
    }
    case 'FY': {  // Font Scale Y
        scaleNumber = getNumberF32(ppu8String, mFontSizeY, mScaleY, 10);
        if (scaleNumber >= 0) {
            mScaleY = scaleNumber;
        }
        break;
    }
    case 'SH':  // Space Horizontal?
        field_0x10 = getNumberF32(ppu8String, mCharSpacing, field_0x10, 10);
        break;
    case 'SV':  // Space Vertical?
        field_0x14 = getNumberF32(ppu8String, mLineSpacing, field_0x14, 10);
        break;
    case 'GM': {
        field_0x22 = getNumberS32(ppu8String, field_0x22 == 0, field_0x22, 10);
        local_40.a = local_40.a * alpha / 0xFF;
        local_44.a = local_44.a * alpha / 0xFF;

        mFont->setGradColor(local_40, field_0x22 != 0 ? local_44 : local_40);
        break;
    }
    case 'HM':
        break;
    default:
        *ppu8String = pStringStart;
        code = 0;
        break;
    }

    return code;
}

void J2DPrint::initchar() {
    field_0x8 = mCharColor;
    field_0xc = mGradColor;
    field_0x10 = mCharSpacing;
    field_0x14 = mLineSpacing;
    field_0x22 = field_0x5a;
    field_0x20 = field_0x58;
    mScaleX = mFontSizeX;
    mScaleY = mFontSizeY;
}

s32 J2DPrint::getNumberS32(const u8** ppu8String, s32 defaultValue, s32 errorValue, int base) {
    const u8* pStringStart = *ppu8String;
    if (**ppu8String != '[') {
        return defaultValue;
    }

    (*ppu8String)++;

    s32 number = 0;
    char* pEnd;
    if (base == 10) {
        number = strtol((char*)*ppu8String, &pEnd, base);
    } else if (base == 16) {
        number = strtoul((char*)*ppu8String, &pEnd, base);
        if ((uintptr_t)pEnd - (uintptr_t)*ppu8String != 8) {
            if ((uintptr_t)pEnd - (uintptr_t)*ppu8String == 6) {
                number = (number << 8) | 0xFF;
            } else {
                *ppu8String = pStringStart;
                return errorValue;
            }
        }
    }

    if (pEnd[0] != ']') {
        *ppu8String = pStringStart;
        return errorValue;
    } else {
        if ((char*)*ppu8String == pEnd) {
            *ppu8String = (const u8*)pEnd + 1;
            return defaultValue;
        } else {
            *ppu8String = (const u8*)pEnd + 1;
        }
    }

    return number;
}

f32 J2DPrint::getNumberF32(const u8** ppu8String, f32 defaultValue, f32 errorValue, int base) {
    const u8* pStringStart = *ppu8String;
    if (**ppu8String != '[') {
        return defaultValue;
    }

    (*ppu8String)++;

    s32 number = 0;
    char* pEnd;
    if (base == 10) {
        number = strtol((char*)*ppu8String, &pEnd, base);
    } else if (base == 16) {
        number = strtoul((char*)*ppu8String, &pEnd, base);
        if ((uintptr_t)pEnd - (uintptr_t)*ppu8String != 8) {
            if ((uintptr_t)pEnd - (uintptr_t)*ppu8String == 6) {
                number = (number << 8) | 0xFF;
            } else {
                *ppu8String = pStringStart;
                return errorValue;
            }
        }
    }

    if (pEnd[0] != ']') {
        *ppu8String = pStringStart;
        return errorValue;
    } else {
        if ((char*)*ppu8String == pEnd) {
            *ppu8String = (const u8*)pEnd + 1;
            return defaultValue;
        } else {
            *ppu8String = (const u8*)pEnd + 1;
        }
    }

    return number;
}
