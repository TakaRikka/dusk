#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "stdio.h"

JUTDbPrint::JUTDbPrint(JUTFont* pFont, JKRHeap* pHeap) {
    mFont = pFont;
    mFirst = NULL;
    mHeap = pHeap != NULL ? pHeap : JKRHeap::getCurrentHeap();
    mColor.set(255, 255, 255, 255);
    mVisible = true;
}

JUTDbPrint* JUTDbPrint::sDebugPrint;

JUTDbPrint* JUTDbPrint::start(JUTFont* pFont, JKRHeap* pHeap) {
    if (sDebugPrint == NULL) {
        if (pHeap == NULL) {
            pHeap = JKRHeap::getCurrentHeap();
        }
        sDebugPrint = new JUTDbPrint(pFont, pHeap);
    }

    return sDebugPrint;
}

JUTFont* JUTDbPrint::changeFont(JUTFont* pFont) {
    JUTFont* old = mFont;
    if (pFont != NULL) {
        mFont = pFont;
    }
    return old;
}

void JUTDbPrint::enter(int param_0, int param_1, int param_2, char const* param_3, int param_4) {
    if (param_4 > 0) {
        unk_print* ptr = static_cast<unk_print*>(JKRAllocFromHeap(mHeap, param_4 + 0x10, -4));
        if (ptr != NULL) {
            ptr->unk_0x04 = param_0;
            ptr->unk_0x06 = param_1;
            ptr->unk_0x08 = param_2;
            ptr->unk_0x0A = param_4;
            strcpy(ptr->unk_0x0C, param_3);
            ptr->mNext = mFirst;
            mFirst = ptr;
        }
    }
}

void JUTDbPrint::flush() {
    this->flush(0, 0, JUTVideo::getManager()->getFbWidth(), JUTVideo::getManager()->getEfbHeight());
}

void JUTDbPrint::flush(int param_0, int param_1, int param_2, int param_3) {
    // weird cast
    unk_print* curPtr = (unk_print*)&mFirst;
    unk_print* cur = mFirst;
    if (mFont != NULL && cur != NULL) {
        J2DOrthoGraph g(param_0, param_1, param_2, param_3, -1, 1);
        g.setPort();
        mFont->setGX();
        mFont->setCharColor(mColor);

        while (cur != NULL) {
            if (mVisible) {
                this->drawString(cur->unk_0x04, cur->unk_0x06, cur->unk_0x0A, (u8*)cur->unk_0x0C);
            }

            if (--cur->unk_0x08 <= 0) {
                unk_print* next = cur->mNext;
                JKRFreeToHeap(mHeap, cur);
                cur = next;
                curPtr->mNext = next;
            } else {
                curPtr = cur;
                cur = cur->mNext;
            }
        }
    }
}

void JUTDbPrint::drawString(int param_0, int param_1, int param_2, u8 const* param_3) {
    JUTFont* font = mFont;
    font->drawString_size_scale(param_0, param_1, font->getWidth(), font->getHeight(),
                                (const char*)param_3, param_2, true);
}

void JUTReport(int param_0, int param_1, char const* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buf[0x100];
    int ret = vsnprintf(buf, 0x100, fmt, args);
    va_end(args);

    if (ret < 0) {
        return;
    }
    JUTDbPrint::sDebugPrint->enter(param_0, param_1, 1, buf, ret < 0x100 ? ret : 0xFF);
}

void JUTReport(int param_0, int param_1, int param_2, char const* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buf[0x100];
    int ret = vsnprintf(buf, 0x100, fmt, args);
    va_end(args);

    if (ret < 0) {
        return;
    }
    JUTDbPrint::sDebugPrint->enter(param_0, param_1, param_2, buf, ret < 0x100 ? ret : 0xFF);
}
