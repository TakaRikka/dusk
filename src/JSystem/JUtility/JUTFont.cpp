/**
 * JUTFont.cpp
 * JUtility - Font Management
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTFont.h"

JUTFont::JUTFont() : mColor1(), mColor2(), mColor3(), mColor4() {
    mValid = false;
}

void JUTFont::initialize_state() {
    setCharColor(JUtility::TColor());
    mFixed = false;
    mFixedWidth = 0;
    mValid = false;
}

void JUTFont::setCharColor(JUtility::TColor col1) {
    mColor1 = col1;
    mColor2 = col1;
    mColor3 = col1;
    mColor4 = col1;
}

void JUTFont::setGradColor(JUtility::TColor col1, JUtility::TColor col2) {
    mColor1 = col1;
    mColor2 = col1;
    mColor3 = col2;
    mColor4 = col2;
}

f32 JUTFont::drawString_size_scale(f32 a1, f32 a2, f32 a3, f32 a4, const char* str, u32 usz,
                                   bool a7) {
    f32 temp = a1;

    for (; usz > 0; usz--, str++) {
        u32 c = *(u8*)str;
        u32 b = c;
        if (isLeadByte(b)) {
            str++;
            b <<= 8;
            b |= *(u8*)str;
            usz--;
        }

        a1 += drawChar_scale(a1, a2, a3, a4, b, a7);
        a7 = 1;
    }

    return a1 - temp;
}
