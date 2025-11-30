#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTDirectPrint.h"
#include "stdio.h"
#include <dolphin/os.h>
#include "global.h"

JUTDirectPrint* JUTDirectPrint::sDirectPrint;

JUTDirectPrint::JUTDirectPrint() {
    changeFrameBuffer(NULL, 0, 0);
    setCharColor(0xff, 0xff, 0xff);
}

JUTDirectPrint* JUTDirectPrint::start() {
    if (!sDirectPrint) {
        sDirectPrint = new JUTDirectPrint();
    }

    return sDirectPrint;
}

void JUTDirectPrint::erase(int x, int y, int width, int height) {
    if (!this->field_0x00) {
        return;
    }

    if (400 < mFrameBufferWidth) {
        x = x << 1;
        width = width << 1;
    }

    if (300 < mFrameBufferHeight) {
        y = y << 1;
        height = height << 1;
    }

    u16* pixel = mFrameBuffer + mStride * y + x;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            *pixel = 0x1080;
            pixel = pixel + 1;
        }

        pixel += mStride - width;
    }
}

u8 JUTDirectPrint::sAsciiTable[128] = {
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0xFD, 0xFE, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x29, 0x64, 0x65, 0x66, 0x2B, 0x67, 0x68, 0x25, 0x26, 0x69, 0x2A, 0x6A, 0x27, 0x2C, 0x6B,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x24, 0x6C, 0x6D, 0x6E, 0x6F, 0x28,
    0x70, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x71, 0x72, 0x73, 0x74, 0x75,
    0xFF, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B,
    0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x76, 0x77, 0x78, 0x79, 0x7A,
};

u32 JUTDirectPrint::sFontData[64] = {
    0x70871C30, 0x8988A250, 0x88808290, 0x88830C90, 0x888402F8, 0x88882210, 0x71CF9C10, 0xF9CF9C70,
    0x8208A288, 0xF200A288, 0x0BC11C78, 0x0A222208, 0x8A222208, 0x71C21C70, 0x23C738F8, 0x5228A480,
    0x8A282280, 0x8BC822F0, 0xFA282280, 0x8A28A480, 0x8BC738F8, 0xF9C89C08, 0x82288808, 0x82088808,
    0xF2EF8808, 0x82288888, 0x82288888, 0x81C89C70, 0x8A08A270, 0x920DA288, 0xA20AB288, 0xC20AAA88,
    0xA208A688, 0x9208A288, 0x8BE8A270, 0xF1CF1CF8, 0x8A28A220, 0x8A28A020, 0xF22F1C20, 0x82AA0220,
    0x82492220, 0x81A89C20, 0x8A28A288, 0x8A28A288, 0x8A289488, 0x8A2A8850, 0x894A9420, 0x894AA220,
    0x70852220, 0xF8011000, 0x08020800, 0x10840400, 0x20040470, 0x40840400, 0x80020800, 0xF8011000,
    0x70800000, 0x88822200, 0x08820400, 0x108F8800, 0x20821000, 0x00022200, 0x20800020, 0x00000000,
};

u32 JUTDirectPrint::sFontData2[77] = {
    0x51421820, 0x53E7A420, 0x014A2C40, 0x01471000, 0x0142AA00, 0x03EAA400, 0x01471A78, 0x00000000,
    0x50008010, 0x20010820, 0xF8020040, 0x20420820, 0x50441010, 0x00880000, 0x00070E00, 0x01088840,
    0x78898820, 0x004A8810, 0x788A8810, 0x01098808, 0x00040E04, 0x70800620, 0x11400820, 0x12200820,
    0x10001020, 0x10000820, 0x100F8820, 0x70000620, 0x60070000, 0x110F82A0, 0x12AA8AE0, 0x084F92A0,
    0x100FBE1C, 0x10089008, 0x60070808, 0x00000000, 0x02000200, 0x7A078270, 0x8BC81E88, 0x8A2822F8,
    0x9A282280, 0x6BC79E78, 0x30000000, 0x48080810, 0x41E80000, 0x422F1830, 0xFBE88810, 0x40288890,
    0x43C89C60, 0x81000000, 0x81000000, 0x990F3C70, 0xA10AA288, 0xE10AA288, 0xA10AA288, 0x98CAA270,
    0x00000000, 0x00000020, 0xF1EF1E20, 0x8A28A0F8, 0x8A281C20, 0xF1E80220, 0x80283C38, 0x00000000,
    0x00000000, 0x8A28B688, 0x8A2A8888, 0x8A2A8878, 0x894A8808, 0x788536F0, 0x00000000, 0x00000000,
    0xF8000000, 0x10000000, 0x20000000, 0x40000000, 0xF8000000,
};

static u32 twiceBit[4] = {
    0,
    3,
    12,
    15,
};

void JUTDirectPrint::drawChar(int position_x, int position_y, int ch) {
    int codepoint = (100 <= ch) ? ch - 100 : ch;
    int col_index = (codepoint % 5) * 6;
    int row_index = (codepoint / 5) * 7;

    const u32 *font_data = (100 > ch) ? sFontData + row_index : sFontData2 + row_index;

    int scale_x = (mFrameBufferWidth < 400) ? 1 : 2;
    int scale_y = (mFrameBufferHeight < 300) ? 1 : 2;

    u16 *pixel = mFrameBuffer + mStride * position_y * scale_y + position_x * scale_x;
    for (int y = 0; y < 7; y++)
    {
        u32 data = *font_data << col_index;
        font_data += 1;

        if (scale_x == 1)
        {
            data = (data & 0xfc000000) >> 1;
        }
        else
        {
            u32 a = twiceBit[(data >> 26) & 3];
            u32 b = twiceBit[(data >> 28) & 3] << 4;
            u32 c = twiceBit[(data >> 30) & 3] << 8;
            data = (a | b | c) << 19;
        }

        for (int x = 0; x < scale_x * 6; x += 2)
        {
            u16 value;

            value = (((data & 0x40000000) ? mCharColor_Y : 0) |
                        ((data & 0x80000000) ? mCharColor_Cb4 : 32) +
                        ((data & 0x40000000) ? mCharColor_Cb2 : 64) +
                        ((data & 0x20000000) ? mCharColor_Cb4 : 32));
            pixel[0] = value;
            if (scale_y > 1)
                pixel[mStride] = value;

            value = (((data & 0x20000000) ? mCharColor_Y : 0) |
                        ((data & 0x40000000) ? mCharColor_Cr4 : 32) +
                        ((data & 0x20000000) ? mCharColor_Cr2 : 64) +
                        ((data & 0x10000000) ? mCharColor_Cr4 : 32));
            pixel[1] = value;
            if (scale_y > 1)
                pixel[1 + mStride] = value;

            pixel += 2;
            data <<= 2;
           
        }

        pixel += mStride * scale_y - 6 * scale_x;
    }
}

void JUTDirectPrint::changeFrameBuffer(void* frameBuffer, u16 width, u16 height) {
    this->field_0x00 = frameBuffer;
    mFrameBuffer = (u16*)frameBuffer;
    mFrameBufferWidth = width;
    mFrameBufferHeight = height;
    mStride = ALIGN_NEXT(width & 0xFFFF, 16);
    mFrameBufferSize = (u32)mStride * (u32)mFrameBufferHeight * 2;
}

void JUTDirectPrint::printSub(u16 position_x, u16 position_y, char const* format, va_list args,
                              bool clear) {
    char buffer[256];
    if (!mFrameBuffer) {
        return;
    }

    int buffer_length = vsnprintf(buffer, ARRAY_SIZEU(buffer), format, args);
    u16 x = position_x;
    if (buffer_length > 0) {
        if (clear) {
            erase(position_x - 6, position_y - 3, (buffer_length + 2) * 6, 0xd);
        }

        char* ptr = buffer;
        for (; 0 < buffer_length; buffer_length--, ptr++) {
            int codepoint = sAsciiTable[*ptr & 0x7f];
            if (codepoint == 0xfe) {
                position_x = x;
                position_y += 7;
            } else if (codepoint == 0xfd) {
                s32 current_position = (int)position_x;
                s32 tab = (current_position - x + 0x2f) % 0x30;
                position_x = current_position + 0x30 - tab;
            } else {
                if (codepoint != 0xff) {
                    drawChar(position_x, position_y, codepoint);
                }
                position_x += 6;
            }
        }
    }

    DCStoreRange(mFrameBuffer, mFrameBufferSize);
}

void JUTDirectPrint::print(u16 position_x, u16 position_y, char const* format, ...) {
    if (mFrameBuffer) {
        va_list args;
        va_start(args, format);
        printSub(position_x, position_y, format, args, true);
        va_end(args);
    }
}

void JUTDirectPrint::drawString(u16 position_x, u16 position_y, char* text) {
    drawString_f(position_x, position_y, "%s", text);
}

void JUTDirectPrint::drawString_f(u16 position_x, u16 position_y, char const* format, ...) {
    if (mFrameBuffer) {
        va_list args;
        va_start(args, format);
        printSub(position_x, position_y, format, args, false);
        va_end(args);
    }
}

void JUTDirectPrint::setCharColor(JUtility::TColor color) {
    setCharColor(color.r, color.g, color.b);
}

void JUTDirectPrint::setCharColor(u8 r, u8 g, u8 b) {
    mCharColor.set(r, g, b, 0xFF);

    int Cb = -0.148 * (int)r - 0.291 * (int)g + 0.439 * (int)b + 128;
    int Cr = 0.439 * (int)r - 0.368 * (int)g - 0.071 * (int)b + 128;
    int Y = 0.257 * (int)r + 0.504 * (int)g + 0.098 * (int)b + 16;

    mCharColor_Y = Y << 8;

    mCharColor_Cb = Cb;
    mCharColor_Cb2 = (Cb >> 1) & 0x7fff;
    mCharColor_Cb4 = (Cb >> 2) & 0x3fff;
    mCharColor_Cr = Cr;
    mCharColor_Cr2 = (Cr >> 1) & 0x7fff;
    mCharColor_Cr4 = (Cr >> 2) & 0x3fff;
}
