#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTDirectPrint.h"
#include "JSystem/JUtility/JUTVideo.h"
#include <dolphin/vi.h>
#include <stdio.h>
#include "global.h"

JUTConsoleManager* JUTConsoleManager::sManager;

JUTConsole* JUTConsole::create(unsigned int param_0, unsigned int maxLines, JKRHeap* pHeap) {
    JUTConsoleManager* pManager = JUTConsoleManager::getManager();
    JUT_ASSERT(33, pManager != NULL);

    u8* buffer = (u8*)JKRAllocFromHeap(pHeap, getObjectSizeFromBufferSize(param_0, maxLines), 0);

    JUTConsole* console = new (buffer) JUTConsole(param_0, maxLines, true);
    console->mBuf = buffer + sizeof(JUTConsole);
    console->clear();

    pManager->appendConsole(console);
    return console;
}

JUTConsole* JUTConsole::create(unsigned int param_0, void* buffer, u32 bufferSize) {
    JUTConsoleManager* pManager = JUTConsoleManager::getManager();
    JUT_ASSERT(59, pManager != NULL);

    JUT_ASSERT(62, ( (u32)buffer & 0x3 ) == 0);
    u32 maxLines = getLineFromObjectSize(bufferSize, param_0);

    JUTConsole* console = new (buffer) JUTConsole(param_0, maxLines, false);
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

    field_0x5c.set(0, 0, 0, 100);
    field_0x60.set(0, 0, 0, 230);
    field_0x64 = 8;
}

JUTConsole::~JUTConsole() {
    JUT_ASSERT(154, JUTConsoleManager::getManager());
    JUTConsoleManager::getManager()->removeConsole(this);
}

size_t JUTConsole::getObjectSizeFromBufferSize(unsigned int param_0, unsigned int maxLines) {
    return (param_0 + 2) * maxLines + sizeof(JUTConsole);
}

size_t JUTConsole::getLineFromObjectSize(u32 bufferSize, unsigned int param_1) {
    return (bufferSize - sizeof(JUTConsole)) / (param_1 + 2);
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
            bool temp_r30 = consoleType == CONSOLE_TYPE_0;
            font_yOffset = 2.0f + mFontSizeY;

            if (consoleType != CONSOLE_TYPE_2) {
                if (JUTVideo::getManager() == NULL) {
                    J2DOrthoGraph ortho(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f);
                    ortho.setPort();
                } else {
                    J2DOrthoGraph ortho(0.0f, 0.0f, JUTVideo::getManager()->getFbWidth(),
                                        JUTVideo::getManager()->getEfbHeight(), -1.0f, 1.0f);
                    ortho.setPort();
                }

                const JUtility::TColor* color;
                if (temp_r30) {
                    color = &field_0x60;
                } else {
                    color = &field_0x5c;
                }

                J2DFillBox(mPositionX - 2, (int)(mPositionY - font_yOffset),
                           (int)((mFontSizeX * field_0x20) + 4.0f), (int)(font_yOffset * mHeight),
                           *color);
                mFont->setGX();

                if (temp_r30) {
                    s32 s = (diffIndex(field_0x30, field_0x38) - mHeight) + 1;
                    if (s <= 0) {
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

            char* linePtr;
            s32 curLine = field_0x30;
            s32 yFactor = 0;

            do {
                linePtr = (char*)getLinePtr(curLine);

                if ((u8)linePtr[-1] != NULL) {
                    if (consoleType != CONSOLE_TYPE_2) {
                        mFont->drawString_scale(mPositionX, ((yFactor * font_yOffset) + mPositionY),
                                                mFontSizeX, mFontSizeY, linePtr, true);
                    } else {
                        JUTDirectPrint::getManager()->drawString(
                            mPositionX, ((yFactor * font_yOffset) + mPositionY), linePtr);
                    }

                    changeLine_1 = curLine + 1;
                    yFactor += 1;
                    changeLine_2 = changeLine_1 & ~(-((s32)mMaxLines <= (s32)changeLine_1));
                    curLine = changeLine_2;
                } else {
                    break;
                }
            } while (yFactor < mHeight && changeLine_2 != field_0x34);
        }
    }
}

void JUTConsole::print_f(char const* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    JUTConsole_print_f_va_(this, fmt, args);
    va_end(args);
}

void JUTConsole::print(char const* str) {
    if (mOutput & 2) {
        OS_REPORT("%s", str);
    }

    if (mOutput & 1) {
        const u8* r29 = (const u8*)str;
        u8* r28 = getLinePtr(field_0x38) + field_0x3c;
        while (*r29) {
            if (field_0x6a && field_0x34 == nextIndex(field_0x38)) {
                break;
            }
            if (*r29 == '\n') {
                r29++;
                field_0x3c = field_0x20;
            } else if (*r29 == '\t') {
                r29++;
                while (field_0x3c < field_0x20) {
                    *(r28++) = ' ';
                    field_0x3c++;
                    if (field_0x3c % field_0x64 == 0) {
                        break;
                    }
                }
            } else if (mFont && mFont->isLeadByte(*r29)) {
                if (field_0x3c + 1 < field_0x20) {
                    *(r28++) = *(r29++);
                    *(r28++) = *(r29++);
                    field_0x3c++;
                    field_0x3c++;
                } else {
                    *(r28++) = 0;
                    field_0x3c++;
                }
            } else {
                *(r28++) = *(r29++);
                field_0x3c++;
            }

            if (field_0x3c < field_0x20) {
                continue;
            }
            *r28 = 0;
            field_0x38 = nextIndex(field_0x38);
            field_0x3c = 0;
            setLineAttr(field_0x38, 0xff);
            r28 = getLinePtr(field_0x38);
            *r28 = 0;
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
        *r28 = 0;
    }
}

extern "C" void JUTConsole_print_f_va_(JUTConsole* console, const char* fmt, va_list args) {
    JUT_ASSERT(563, console!=NULL);

    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    console->print(buf);
}

void JUTConsole::dumpToTerminal(unsigned int param_0) {
    if (param_0 == 0) {
        return;
    }

    int r29 = field_0x34;
    if (param_0 != -1) {
        r29 = field_0x38;
        for (int i = 0; i != param_0; i++) {
            int r25 = prevIndex(r29);
            if (getLineAttr(r25) == 0) {
                break;
            }
            r29 = r25;
            if (r25 == field_0x34) {
                break;
            }
        }
    }

    int r27 = 0;
    OS_REPORT("\n:::dump of console[%x]--------------------------------\n", this);

    do {
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
    } while (r29 != field_0x34);

    OS_REPORT(":::dump of console[%x] END----------------------------\n", this);
}

void JUTConsole::scroll(int scrollAmnt) {
    if (scrollAmnt < 0) {
        int diff = diffIndex(field_0x34, field_0x30);
        if (scrollAmnt < -diff) {
            scrollAmnt = -diff;
        }
    } else {
        if (scrollAmnt > 0) {
            int diff = diffIndex(field_0x34, field_0x38);
            if (diff + 1 <= mHeight) {
                scrollAmnt = 0;
            } else {
                diff = diffIndex(field_0x30, field_0x38);
                if (scrollAmnt > (int)(diff - mHeight) + 1) {
                    scrollAmnt = (int)(diff - mHeight) + 1;
                }
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
    return diffIndex(field_0x34, field_0x38);
}

int JUTConsole::getLineOffset() const {
    return diffIndex(field_0x34, field_0x30);
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

    JUTConsoleManager* manager = new (pHeap, 0) JUTConsoleManager();
    sManager = manager;
    return manager;
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
    JUT_ASSERT(982, sManager != NULL && console != NULL);
    JUT_ASSERT(985, soLink_.Find( console ) != soLink_.end());

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
            do {
                new_count = VIGetRetraceCount();
            } while (retrace_count == new_count);
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

    if (JUTGetReportConsole() == NULL) {
        vsnprintf(buf, sizeof(buf), fmt, args);
    } else if (JUTGetReportConsole()->getOutput() &
               (JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT))
    {
        vsnprintf(buf, sizeof(buf), fmt, args);
        JUTGetReportConsole()->print(buf);
    }
}

extern "C" void JUTReportConsole_f(const char* fmt, ...) {
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

    if (JUTGetWarningConsole() == NULL) {
        vsnprintf(buf, sizeof(buf), fmt, args);
        OSReport("%s", buf);
    } else if (JUTGetWarningConsole()->getOutput() &
               (JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT))
    {
        vsnprintf(buf, sizeof(buf), fmt, args);
        JUTGetWarningConsole()->print(buf);
    }
}

void JUTWarningConsole_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    JUTReportConsole_f_va(fmt, args);
    va_end(args);
}

void JUTWarningConsole(const char* message) {
    JUTReportConsole_f("%s", message);
}
