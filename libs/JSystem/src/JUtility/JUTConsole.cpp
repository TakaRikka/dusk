#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTDirectPrint.h"
#include "JSystem/JUtility/JUTVideo.h"
#include <vi.h>
#include <cstdio>
#include "global.h"

JUTConsoleManager* JUTConsoleManager::sManager;

JUTConsole* JUTConsole::create(unsigned int param_0, unsigned int maxLines, JKRHeap* pHeap) {
    JUTConsoleManager* pManager = JUTConsoleManager::getManager();
    JUT_ASSERT(33, pManager != NULL);

    u8* buffer = (u8*)JKRAllocFromHeap(pHeap, getObjectSizeFromBufferSize(param_0, maxLines), 0);

    JUTConsole* console = JKR_NEW_ARGS (buffer) JUTConsole(param_0, maxLines, true);
    console->mBuf = buffer + sizeof(JUTConsole);
    console->clear();

    pManager->appendConsole(console);
    return console;
}

JUTConsole* JUTConsole::create(unsigned int param_0, void* buffer, u32 bufferSize) {
    JUTConsoleManager* pManager = JUTConsoleManager::getManager();
    JUT_ASSERT(59, pManager != NULL);

    JUT_ASSERT(62, ( (uintptr_t)buffer & 0x3 ) == 0);

    u32 maxLines = getLineFromObjectSize(bufferSize, param_0);

    JUTConsole* console = JKR_NEW_ARGS (buffer) JUTConsole(param_0, maxLines, false);
    console->mBuf = (u8*)buffer + sizeof(JUTConsole);
    console->clear();

    pManager->appendConsole(console);
    return console;
}

JUTConsole::JUTConsole(unsigned int param_0, unsigned int maxLines, bool param_2) {
    field_0x2c = param_2;
    field_0x20 = param_0;
    mMaxLines = maxLines;

    mPositionX = 30;
    mPositionY = 50;
    mHeight = 20;

    if (mHeight > mMaxLines) {
        mHeight = mMaxLines;
    }

    mFont = NULL;
    mVisible = true;
    field_0x69 = false;
    field_0x6a = false;
    field_0x6b = false;
    mOutput = 1;

    field_0x5c = JUtility::TColor(0, 0, 0, 100);
    field_0x60 = JUtility::TColor(0, 0, 0, 230);
    field_0x64 = 8;
}

static void dummystring1() {
    UNUSED("console != 0");
}

JUTConsole::~JUTConsole() {
    JUT_ASSERT(154, JUTConsoleManager::getManager());
    JUTConsoleManager::getManager()->removeConsole(this);
}

size_t JUTConsole::getObjectSizeFromBufferSize(unsigned int param_0, unsigned int maxLines) {
    size_t result = (param_0 + 2) * maxLines + sizeof(JUTConsole);
    return result;
}

size_t JUTConsole::getLineFromObjectSize(u32 bufferSize, unsigned int param_1) {
    bufferSize -= sizeof(JUTConsole);
    size_t result = (bufferSize) / (param_1 + 2);
    return result;
}

void JUTConsole::clear() {
    field_0x30 = 0;
    field_0x34 = 0;
    field_0x38 = 0;
    field_0x3c = 0;

    for (u32 i = 0; i < mMaxLines; i++) {
        setLineAttr(i, 0);
    }

    setLineAttr(0, -1);
    *getLinePtr(0) = 0;
}

void JUTConsole::doDraw(JUTConsole::EConsoleType consoleType) const {
    f32 font_yOffset;
    s32 changeLine_1;
    s32 changeLine_2;

    if (mVisible && (mFont != NULL || consoleType == CONSOLE_TYPE_2)) {
        if (mHeight != 0) {
            bool spA = consoleType == CONSOLE_TYPE_0 ? true : false;
            int spA4 = spA ? 1 : 0;
            spA4 = 0;
            font_yOffset = 2.0f + mFontSizeY;

            if (consoleType != CONSOLE_TYPE_2) {
                if (JUTGetVideoManager() == NULL) {
                    J2DOrthoGraph ortho(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f);
                    ortho.setPort();
                } else {
                    J2DOrthoGraph ortho(0.0f, 0.0f, JUTGetVideoManager()->getFbWidth(),
                                        JUTGetVideoManager()->getEfbHeight(), -1.0f, 1.0f);
                    ortho.setPort();
                }

                J2DFillBox(mPositionX - 2, (int)(mPositionY - font_yOffset),
                           (int)((mFontSizeX * field_0x20) + 4.0f),
                           (int)(font_yOffset * (mHeight + spA4)), spA ? field_0x60 : field_0x5c);
                mFont->setGX();

                if (spA) {
                    if (((diffIndex(field_0x30, field_0x38) - (int)mHeight) + 1) <= 0) {
                        mFont->setCharColor(JUtility::TColor(255, 255, 255, 255));
                    } else if (field_0x30 == field_0x34) {
                        mFont->setCharColor(JUtility::TColor(255, 230, 230, 255));
                    } else {
                        mFont->setCharColor(JUtility::TColor(230, 230, 255, 255));
                    }
                } else {
                    mFont->setCharColor(JUtility::TColor(230, 230, 230, 255));
                }
            } else {
                JUTDirectPrint::getManager()->erase(mPositionX - 3, mPositionY - 2,
                                                    (field_0x20 * 6) + 6,
                                                    (int)(font_yOffset * mHeight) + 4);
                JUTDirectPrint::getManager()->setCharColor(JUtility::TColor(255, 255, 255, 255));
            }

            s32 curLine = field_0x30;
            s32 yFactor = 0;
            u8* linePtr;

            while (true) {
                linePtr = getLinePtr(curLine);
                u8 sp9 = linePtr[-1];
                if (sp9 == 0) {
                    break;
                }

                if (consoleType != CONSOLE_TYPE_2) {
                    mFont->drawString_scale(mPositionX, ((yFactor * font_yOffset) + mPositionY),
                                            mFontSizeX, mFontSizeY, (char*)linePtr, true);
                } else {
                    JUTDirectPrint::getManager()->drawString(
                        mPositionX, ((yFactor * font_yOffset) + mPositionY), (char*)linePtr);
                }
                curLine = nextIndex(curLine);
                yFactor++;

                if (yFactor >= mHeight || curLine == field_0x34) {
                    break;
                }
            }

            if (spA4 != 0) {
                f32 f31 = mPositionX;
                int sp94 = mHeight * font_yOffset + mPositionY;
                mFont->setCharColor(JUtility::TColor(0xff, mVisible ? 0xff : 200, 0xc8, 0xff));
                mFont->drawString_scale((int)f31, sp94, mFontSizeX, mFontSizeY, "X", TRUE);
                f31 += mFontSizeX;
                mFont->drawString_scale((int)f31, sp94, mFontSizeX, mFontSizeY,
                                        mVisible ? "[ON]" : "[OFF]", TRUE);
                f31 += (int)(mFontSizeX * 6.0f);
                if (this == NULL)  // ????
                {
                    mFont->setCharColor(JUtility::TColor(0xff, 0xff, 0x64, 0xff));
                    mFont->drawString_scale((int)(f31 - mFontSizeX), mFontSizeX, mFontSizeY, sp94,
                                            "*", TRUE);
                }
                mFont->setCharColor(JUtility::TColor(0xc8, 0xc8, 0xc8, 0xff));
                char spA8[] = "S----------E";
                char spB8[0x20];
                int sp90 = diffIndex(field_0x34, field_0x38) + 1;
                int sp8C = diffIndex(field_0x34, field_0x30);
                int sp88 = 0;
                int sp84;
                if (sp90 <= mHeight) {
                    sp84 = 9;
                    sp88 = 1;
                } else {
                    sp84 = (sp8C * 9) / (int)(sp90 - mHeight);
                }
                spA8[sp84 + 1] = 'O';
                mFont->drawString_scale((int)f31, sp94, mFontSizeX, mFontSizeY, spA8, TRUE);
                f31 += mFontSizeX * 13.0f;
                if (sp88) {
                    sprintf(spB8, "ALL");
                } else {
                    f32 f29 = sp8C / (f32)(sp90 - mHeight);
                    sprintf(spB8, "%3d%%(%dL)", (int)(100.0 * f29), sp90);
                }
                mFont->drawString_scale(f31, sp94, mFontSizeX, mFontSizeY, spB8, TRUE);
            }
        }
    }
}

void JUTConsole::print_f(char const* fmt, ...) {
    UNUSED(fmt);

    va_list args;
    va_start(args, fmt);
    JUTConsole::print_f_va(fmt, args);
    va_end(args);
}

void JUTConsole::print(char const* str) {
    if (mOutput & 2) {
        OS_REPORT("%s", str);
    }

    if (mOutput & 1) {
        u8* src = (u8*)const_cast<char*>(str); // needs to be non-const to match debug
        u8* dst = (u8*)getLinePtr(field_0x38) + field_0x3c;
        while (*src != 0) {
            if (field_0x6a && field_0x34 == nextIndex(field_0x38)) {
                break;
            }
            if (*src == '\n') {
                src++;
                field_0x3c = field_0x20;
            } else if (*src == '\t') {
                src++;
                while (field_0x3c < field_0x20) {
                    *dst++ = ' ';
                    field_0x3c++;
                    if (field_0x3c % field_0x64 == 0) {
                        break;
                    }
                }
            } else if (mFont && mFont->isLeadByte(*src)) {
                if (field_0x3c + 1 < field_0x20) {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    field_0x3c++;
                    field_0x3c++;
                } else {
                    *dst++ = '\0';
                    field_0x3c++;
                }
            } else {
                *dst++ = *src++;
                field_0x3c++;
            }

            if (field_0x3c < field_0x20) {
                continue;
            }
            *dst = '\0';
            field_0x38 = nextIndex(field_0x38);
            field_0x3c = 0;
            setLineAttr(field_0x38, 0xff);
            dst = getLinePtr(field_0x38);
            *dst = '\0';
            int local_28 = diffIndex(field_0x30, field_0x38);
            if (local_28 == mHeight) {
                field_0x30 = nextIndex(field_0x30);
            }
            if (field_0x38 == field_0x34) {
                field_0x34 = nextIndex(field_0x34);
            }
            if (field_0x38 == field_0x30) {
                field_0x30 = nextIndex(field_0x30);
            }

            if (field_0x6b) {
                break;
            }
        }
        *dst = '\0';
    }
}

extern "C" void JUTConsole_print_f_va_(JUTConsole* console, const char* fmt, va_list args) {
    JUT_ASSERT(563, console!=NULL);

    char buf[1024];
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    console->print(buf);
}

void JUTConsole::dumpToTerminal(unsigned int param_0) {
    if (param_0 == 0) {
        return;
    }

    int r29 = field_0x34;
    if (param_0 != -1) {
        r29 = field_0x38;
        for (; param_0; param_0--) {
            int r25 = prevIndex(r29);
            if (getLineAttr(r25) == 0) {
                break;
            }
            r29 = r25;
            if (r29 == field_0x34) {
                break;
            }
        }
    }

    int r27 = 0;
    OS_REPORT("\n:::dump of console[%x]--------------------------------\n", this);

    while (true) {
        u8* r28 = getLinePtr(r29);
        u8 r24 = r28[-1];
        if (r24 == 0) {
            break;
        }
        if (field_0x69) {
            OSReport("[%03d] %s\n", r27, r28);
        } else {
            OSReport("%s\n", r28);
        }
        r29 = nextIndex(r29);
        r27++;
        if (r29 == field_0x34) {
            break;
        }
    };

    OS_REPORT(":::dump of console[%x] END----------------------------\n", this);
}

static void dummyStrings2()
{
    UNUSED("console != this && console != 0");
    UNUSED("\n:::dump of console[%x]----------------\n");
    UNUSED(":::dump of console[%x] END------------\n");
    UNUSED("sManager == 0");
    UNUSED("consoleManager != 0 && sManager == consoleManager");
}

void JUTConsole::scroll(int scrollAmnt) {
    if (scrollAmnt < 0) {
        int diff = diffIndex(field_0x34, field_0x30);
        if (scrollAmnt < -diff) {
            scrollAmnt = -diff;
        }
    } else if (scrollAmnt > 0) {
        if (diffIndex(field_0x34, field_0x38) + 1 <= mHeight) {
            scrollAmnt = 0;
        } else {
            int r27 = diffIndex(field_0x30, field_0x38) - mHeight + 1;
            if (scrollAmnt > r27) {
                scrollAmnt = r27;
            }
        }
    }

    field_0x30 += scrollAmnt;
    if (field_0x30 < 0) {
        field_0x30 += mMaxLines;
    }

    if (field_0x30 >= (u32)mMaxLines) {
        field_0x30 -= mMaxLines;
    }
}

int JUTConsole::getUsedLine() const {
    int result = diffIndex(field_0x34, field_0x38);
    return result;
}

int JUTConsole::getLineOffset() const {
    int result = diffIndex(field_0x34, field_0x30);
    return result;
}

JUTConsoleManager::JUTConsoleManager() {
    mActiveConsole = NULL;
    mDirectConsole = NULL;
}

JUTConsoleManager* JUTConsoleManager::createManager(JKRHeap* pHeap) {
    JUT_ASSERT(0x39c, sManager == NULL);

    if (pHeap == NULL) {
        pHeap = JKRGetCurrentHeap();
    }

    sManager = JKR_NEW_ARGS (pHeap, 0) JUTConsoleManager();
    return sManager;
}

void JUTConsoleManager::appendConsole(JUTConsole* console) {
    JUT_ASSERT(961, sManager != NULL && console != NULL);

    JUT_ASSERT(964, soLink_.Find( console ) == soLink_.end());
    soLink_.Push_back(console);

    if (mActiveConsole == NULL) {
        mActiveConsole = console;
    }
}

void JUTConsoleManager::removeConsole(JUTConsole* console) {
    JUT_ASSERT(984, sManager != NULL && console != NULL);
    JUT_ASSERT(987, soLink_.Find( console ) != soLink_.end());

    if (mActiveConsole == console) {
        if (soLink_.size() <= 1) {
            mActiveConsole = NULL;
        } else {
            mActiveConsole = console != &soLink_.back() ? soLink_.Element_toValue(console->mListNode.getNext()) : &soLink_.front();
        }
    }

    if (JUTGetWarningConsole() == console)
        JUTSetWarningConsole(NULL);
    if (JUTGetReportConsole() == console)
        JUTSetReportConsole(NULL);

    soLink_.Remove(console);
}

void JUTConsoleManager::draw() const {
    ConsoleList::const_iterator iter = soLink_.begin();
    ConsoleList::const_iterator end = soLink_.end();

    for (; iter != end; ++iter) {
        const JUTConsole* const console = &(*iter);
        if (console != mActiveConsole)
            console->doDraw(JUTConsole::CONSOLE_TYPE_1);
    }

    if (mActiveConsole != NULL)
        mActiveConsole->doDraw(JUTConsole::CONSOLE_TYPE_0);
}

void JUTConsoleManager::drawDirect(bool waitRetrace) const {
    if (mDirectConsole != NULL) {
        if (waitRetrace) {
            s32 interrupt_status = OSEnableInterrupts();
            u32 retrace_count = VIGetRetraceCount();
            u32 new_count;
            while (retrace_count == VIGetRetraceCount()){
                // nop
            }
            OSRestoreInterrupts(interrupt_status);
        }
        mDirectConsole->doDraw(JUTConsole::CONSOLE_TYPE_2);
    }
}

void JUTConsoleManager::setDirectConsole(JUTConsole* console) {
    if (mDirectConsole != NULL) {
        appendConsole(mDirectConsole);
    }

    if (console != NULL) {
        removeConsole(console);
    }
    mDirectConsole = console;
}

static JUTConsole* sReportConsole;

extern "C" void JUTSetReportConsole(JUTConsole* console) {
    sReportConsole = console;
}

extern "C" JUTConsole* JUTGetReportConsole() {
    return sReportConsole;
}

static JUTConsole* sWarningConsole;

extern "C" void JUTSetWarningConsole(JUTConsole* console) {
    sWarningConsole = console;
}

extern "C" JUTConsole* JUTGetWarningConsole() {
    return sWarningConsole;
}

extern "C" void JUTReportConsole_f_va(const char* fmt, va_list args) {
    char buf[256];
    int len;

    if (JUTGetReportConsole() == NULL) {
        len = vsnprintf(buf, sizeof(buf), fmt, args);
        OS_REPORT("%s", buf);
    } else if (JUTGetReportConsole()->getOutput() &
               (JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT))
    {
        len = vsnprintf(buf, sizeof(buf), fmt, args);
        JUTGetReportConsole()->print(buf);
    }
}

extern "C" void JUTReportConsole_f(const char* fmt, ...) {
    UNUSED(fmt);

    va_list args;
    va_start(args, fmt);
    JUTReportConsole_f_va(fmt, args);
    va_end(args);
}

static void dummyString() {
    DEAD_STRING("\n:::dump of console[%x]----------------\n");
    DEAD_STRING(":::dump of console[%x] END------------\n");
}

void JUTReportConsole(const char* message) {
    JUTReportConsole_f("%s", message);
}

void JUTWarningConsole_f_va(const char* fmt, va_list args) {
    char buf[256];
    int len;

    if (JUTGetWarningConsole() == NULL) {
        len = vsnprintf(buf, sizeof(buf), fmt, args);
        OSReport("%s", buf);
    } else if (JUTGetWarningConsole()->getOutput() &
               (JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT))
    {
        len = vsnprintf(buf, sizeof(buf), fmt, args);
        JUTGetWarningConsole()->print(buf);
    }
}

void JUTWarningConsole_f(const char* fmt, ...) {
    UNUSED(fmt);

    va_list args;
    va_start(args, fmt);
    JUTReportConsole_f_va(fmt, args);
    va_end(args);
}

void JUTWarningConsole(const char* message) {
    JUTReportConsole_f("%s", message);
}
