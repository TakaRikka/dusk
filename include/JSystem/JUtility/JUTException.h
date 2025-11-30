#ifndef JUTEXCEPTION_H
#define JUTEXCEPTION_H

#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "__va_arg.h"
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include "global.h"

typedef struct _GXRenderModeObj GXRenderModeObj;
typedef struct OSContext OSContext;
class JUTDirectPrint;

/**
* @ingroup jsystem-jutility
* 
*/
class JUTExternalFB {
public:
    JUTExternalFB(_GXRenderModeObj*, GXGamma, void*, u32);

private:
    /* 0x00 */ _GXRenderModeObj* mRenderMode;
    /* 0x04 */ u32 mSize;
    /* 0x08 */ u32 field_0x08;
    /* 0x0C */ u16 field_0x0C;
    /* 0x0E */ u16 mGamma;
    /* 0x10 */ bool field_0x10;
    /* 0x11 */ u8 field_[3];
};

STATIC_ASSERT(sizeof(JUTExternalFB) == 0x14);

#define JUT_PRINT_GPR 1
#define JUT_PRINT_GPR_MAP 2
#define JUT_PRINT_SRR0_MAP 4
#define JUT_PRINT_FLOAT 8
#define JUT_PRINT_STACK 16

/**
* @ingroup jsystem-jutility
* 
*/

typedef void (*JUTExceptionUserCallback)(u16, OSContext*, u32, u32);

class JUTException : public JKRThread {
public:
    enum EInfoPage {
        EINFO_PAGE_GPR = 1,
        EINFO_PAGE_FLOAT = 2,
        EINFO_PAGE_STACK = 3,
        EINFO_PAGE_GPR_MAP = 4,
        EINFO_PAGE_SSR0_MAP = 5,
    };

    class JUTExMapFile {
    public:
        JUTExMapFile(char* path) : mLink(this) { mPath = path; }

    public:
        /* 0x00 */ char* mPath;
        /* 0x04 */ JSULink<JUTExMapFile> mLink;
        /* 0x14 */
    };

    JUTException(JUTDirectPrint*);
    virtual ~JUTException();

    void showFloatSub(int, f32);
    void showFloat(OSContext*);
    void showStack(OSContext*);
    void showMainInfo(u16, OSContext*, u32, u32);
    void showGPR(OSContext*);
    bool showMapInfo_subroutine(u32, bool);
    void showGPRMap(OSContext*);
    void showSRR0Map(OSContext*);
    void printDebugInfo(JUTException::EInfoPage, OSError, OSContext*, u32, u32);
    bool isEnablePad() const;
    bool readPad(u32*, u32*);
    void printContext(u16, OSContext*, u32, u32);
    void createFB();

    virtual void* run();

    static JUTException* create(JUTDirectPrint*);
    static void errorHandler(OSError, OSContext*, u32, u32);
    static void panic_f_va(char const*, int, char const*, va_list);
    static void panic_f(char const*, int, char const*, ...);
    static void setFPException(u32);
    static bool searchPartialModule(u32, u32*, u32*, u32*, u32*);
    static JUTExceptionUserCallback setPreUserCallback(JUTExceptionUserCallback);
    static JUTExceptionUserCallback setPostUserCallback(JUTExceptionUserCallback);
    static void appendMapFile(char const*);
    static bool queryMapAddress(char*, u32, s32, u32*, u32*, char*, u32, bool, bool);
    static bool queryMapAddress_single(char*, u32, s32, u32*, u32*, char*, u32, bool,
                                                      bool);
    static void createConsole(void*, u32);
    static void waitTime(s32);

    static JUTException* getManager() { return sErrorManager; }
    static JUTConsole* getConsole() { return sConsole; }

    JUTExternalFB* getFrameMemory() const { return mFrameMemory; }

    void setTraceSuppress(u32 param_0) { mTraceSuppress = param_0; }
    void setGamePad(JUTGamePad* gamePad) {
        mGamePad = gamePad;
        mGamePadPort = JUTGamePad::EPortInvalid;
    }

    static void setMapFile(const char* map) {
        appendMapFile(map);
    }
    static void panic(const char* file, int line, const char* msg) {
        panic_f(file, line, "%s", msg);
    }

private:
    static OSMessageQueue sMessageQueue;
    static const char* sCpuExpName[17];
    static JSUList<JUTException::JUTExMapFile> sMapFileList;
    static OSMessage sMessageBuffer[1];
    static JUTException* sErrorManager;
    static JUTExceptionUserCallback sPreUserCallback;
    static JUTExceptionUserCallback sPostUserCallback;
    static void* sConsoleBuffer;
    static u32 sConsoleBufferSize;
    static JUTConsole* sConsole;
    static u32 msr;
    static u32 fpscr;

private:
    /* 0x7C */ JUTExternalFB* mFrameMemory;
    /* 0x80 */ JUTDirectPrint* mDirectPrint;
    /* 0x84 */ JUTGamePad* mGamePad;
    /* 0x88 */ JUTGamePad::EPadPort mGamePadPort;
    /* 0x8C */ s32 mPrintWaitTime0;
    /* 0x90 */ s32 mPrintWaitTime1;
    /* 0x94 */ u32 mTraceSuppress;
    /* 0x98 */ u32 field_0x98;
    /* 0x9C */ u32 mPrintFlags;
    /* 0xA0 */ u32 mStackPointer;
};

STATIC_ASSERT(sizeof(JUTException) == 0xA4);

/**
* @ingroup jsystem-jutility
* 
*/
struct JUTWarn {
    JUTWarn& operator<<(const char*) { return *this; }
    JUTWarn& operator<<(long) { return *this; }
};

#endif /* JUTEXCEPTION_H */
