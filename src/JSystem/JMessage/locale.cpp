#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JMessage/locale.h"

int JMessage::locale::parseCharacter_ShiftJIS(const char** ppszText) {
    int c = *(u8*)*ppszText & 0xFF;
    (*ppszText)++;

    if (isLeadByte_ShiftJIS(c)) {
        c <<= 8;
        c |= *(u8*)*ppszText & 0xFF;
        (*ppszText)++;
    }

    return c;
}

int JMessage::locale::parseCharacter_UTF8(const char** ppszText) {
    int c = *(u8*)*ppszText & 0xFF;
    (*ppszText)++;

    if (c & 0x80) {
        if ((c & 0xE0) == 0xC0) {
            c &= 0x1F;
            c <<= 6;
            c |= (*(u8*)*ppszText & 0xFF) & 0x3F;
            (*ppszText)++;
        } else if ((c & 0xF0) == 0xE0) {
            c &= 0xF;
            c <<= 6;
            c |= (*(u8*)*ppszText & 0xFF) & 0x3F;
            (*ppszText)++;

            c <<= 6;
            c |= (*(u8*)*ppszText & 0xFF) & 0x3F;
            (*ppszText)++;
        } else if ((c & 0xF8) == 0xF0) {
            c &= 0x7;
            c <<= 6;
            c |= (*(u8*)*ppszText & 0xFF) & 0x3F;
            (*ppszText)++;

            c <<= 6;
            c |= (*(u8*)*ppszText & 0xFF) & 0x3F;
            (*ppszText)++;

            c <<= 6;
            c |= (*(u8*)*ppszText & 0xFF) & 0x3F;
            (*ppszText)++;
        } else {
            c = -1;
        }
    }

    return c;
}
