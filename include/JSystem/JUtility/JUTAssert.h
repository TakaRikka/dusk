#ifndef JUTASSERT_H
#define JUTASSERT_H

#include <dolphin/os.h>

#if DEBUG
#if __MWERKS__

#define JUT_SHOW_ASSERT(LINE, COND) JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, LINE, #COND)

#define JUT_ASSERT(LINE, COND) \
    (COND) ? (void)0 : (JUT_SHOW_ASSERT(LINE, COND), OSPanic(__FILE__, LINE, "Halt"));

#define JUT_ASSERT_MSG(LINE, COND, MSG) \
    (COND) ? (void)0 : (JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, LINE, MSG), OSPanic(__FILE__, LINE, "Halt"));

#define JUT_ASSERT_MSG_F(LINE, COND, MSG, ...) \
    (COND) ? (void)0 : (JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, LINE, MSG, __VA_ARGS__), OSPanic(__FILE__, LINE, "Halt"));

#define J3D_PANIC(LINE, COND, MSG) ((COND) != 0 || (OSPanic(__FILE__, LINE, MSG), 0));

#define JUT_PANIC(LINE, TEXT)                                                                      \
    JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, LINE, TEXT);                    \
    OSPanic(__FILE__, LINE, "Halt");

#define JUT_PANIC_F(LINE, MSG, ...)                                                               \
    JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, LINE, MSG, __VA_ARGS__);        \
    OSPanic(__FILE__, LINE, "Halt");

#define JUT_WARN_DEVICE(LINE, DEVICE, ...)                                                                        \
    JUTAssertion::setWarningMessage_f(DEVICE, __FILE__, LINE, __VA_ARGS__);    \

#define JUT_LOG(LINE, ...)                                                                         \
    JUTAssertion::setLogMessage_f(JUTAssertion::getSDevice(), __FILE__, LINE, __VA_ARGS__)

#define JUT_SET_CONFIRM(LINE, COND)                                                                \
    JUTAssertion::setConfirmMessage(JUTAssertion::getSDevice(), __FILE__, LINE, COND, #COND)

#else

#define JUT_SHOW_ASSERT(LINE, COND) JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, __LINE__, #COND)

#define JUT_ASSERT(LINE, COND) \
    (COND) ? (void)0 : (JUT_SHOW_ASSERT(__LINE__, COND), OSPanic(__FILE__, __LINE__, "Halt"));

#define JUT_ASSERT_MSG(LINE, COND, MSG) \
    (COND) ? (void)0 : (JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, __LINE__, MSG), OSPanic(__FILE__, __LINE__, "Halt"));

#define JUT_ASSERT_MSG_F(LINE, COND, MSG, ...) \
    (COND) ? (void)0 : (JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, __LINE__, MSG, __VA_ARGS__), OSPanic(__FILE__, __LINE__, "Halt"));

#define J3D_PANIC(LINE, COND, MSG) ((COND) != 0 || (OSPanic(__FILE__, __LINE__, MSG), 0));

#define JUT_PANIC(LINE, TEXT)                                                                      \
JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, __LINE__, TEXT);                    \
    OSPanic(__FILE__, __LINE__, "Halt");

#define JUT_PANIC_F(LINE, MSG, ...)                                                               \
JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, __LINE__, MSG, __VA_ARGS__);        \
    OSPanic(__FILE__, __LINE__, "Halt");

#define JUT_WARN_DEVICE(LINE, DEVICE, ...)                                                                        \
    JUTAssertion::setWarningMessage_f(DEVICE, __FILE__, __LINE__, __VA_ARGS__);    \

#define JUT_LOG(LINE, ...)                                                                         \
    JUTAssertion::setLogMessage_f(JUTAssertion::getSDevice(), __FILE__, __LINE__, __VA_ARGS__)

#define JUT_SET_CONFIRM(LINE, COND)                                                                \
    JUTAssertion::setConfirmMessage(JUTAssertion::getSDevice(), __FILE__, __LINE__, COND, #COND)

#endif
#define JUT_WARN(LINE, ...)                                                                        \
    JUT_WARN_DEVICE(__LINE__, JUTAssertion::getSDevice(), __VA_ARGS__)

#define JUT_CONFIRM(LINE, COND)                                                                    \
    JUT_SET_CONFIRM(LINE, COND)
#else
#define JUT_ASSERT(...) (void)0;
#define JUT_ASSERT_MSG(...) (void)0;
#define JUT_ASSERT_MSG_F(...) (void)0;
#define J3D_PANIC(...) (void)0;
#define JUT_PANIC(...)
#define JUT_PANIC_F(...)
#define JUT_WARN_DEVICE(...)
#define JUT_WARN(...)
#define JUT_LOG(...)
#define JUT_CONFIRM(...)
#endif

namespace JUTAssertion {
    void create();
    u32 flush_subroutine();
    void flushMessage();
    void flushMessage_dbPrint();
    void setVisible(bool);
    void setMessageCount(int);

    u32 getSDevice();
    void showAssert_f(u32 device, const char* file, int line, const char* msg, ...);
    void showAssert_f_va(u32 device, const char* file, int line, const char* msg, va_list args);
    void setWarningMessage_f(u32 device, char * file, int line, const char * fmt, ...);
    void setWarningMessage_f_va(u32 device, const char* file, int line, const char* msg, va_list args);
    void setLogMessage_f(u32 device, char* file, int line, const char* fmt, ...);
    void setConfirmMessage(u32 param_1, char* file, int line, bool param_4, const char* msg);
    
    inline void showAssert(u32 device, const char* file, int line, const char* msg) {
        showAssert_f(device, file, line, "%s", msg);
    }

    inline void setWarningMessage(u32 device, char* file, int line, const char* msg) {
        setWarningMessage_f(device, file, line, "%s", msg);
    }
};

#endif /* JUTASSERT_H */
