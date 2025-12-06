#ifndef DEFINE_H
#define DEFINE_H

#include "dolphin/types.h"

#ifdef __cplusplus
extern "C" {

class JGadget_outMessage {
public:
    typedef void (*MessageFunc)(const char*, int, const char*);

    static void warning(const char*, int, const char*);

    JGadget_outMessage(MessageFunc fn, const char* file, int line);
    ~JGadget_outMessage();

    JGadget_outMessage& operator<<(int param_1) { return *this << (s32)param_1; }
    JGadget_outMessage& operator<<(u16);
    JGadget_outMessage& operator<<(unsigned int);
    JGadget_outMessage& operator<<(u8 param_1) { return *this << (char)param_1; }
    JGadget_outMessage& operator<<(const char* str);
    JGadget_outMessage& operator<<(char);
    JGadget_outMessage& operator<<(s32);
    JGadget_outMessage& operator<<(u32);
    JGadget_outMessage& operator<<(const void*);

private:
    MessageFunc mMsgFunc;
    char mBuffer[256];
    char* mWrite_p;
    char* mFile;
    int mLine;
};

#ifdef DEBUG

#define JGADGET_ASSERTWARN(line, COND) \
    ((COND)) || (JGadget_outMessage(JGadget_outMessage::warning, __FILE__, line) << #COND, false);

#define JGADGET_WARNMSG(line, msg)                                            \
        JGadget_outMessage(JGadget_outMessage::warning, __FILE__, line) << msg, false;

#define JGADGET_WARNMSG1(line, msg, arg)                                      \
        JGadget_outMessage out(JGadget_outMessage::warning, __FILE__, line);  \
        out << msg << (arg);

#define JGADGET_WARNMSG4(line, msg, arg1, arg2, arg3, arg4)                   \
        JGadget_outMessage out(JGadget_outMessage::warning, __FILE__, line);  \
        out << msg << (arg1) << (arg2) << (arg3) << (arg4);

#define JGADGET_EXITWARN(cond)                                                                     \
    if (!(cond)) {                                                                                 \
        false;                                                                                     \
        return false;                                                                              \
    }
#else
#define JGADGET_ASSERTWARN(line, COND) (void)0
#define JGADGET_WARNMSG(line, msg) (void)0
#define JGADGET_WARNMSG1(line, msg, arg) (void)0
#define JGADGET_WARNMSG4(line, msg, arg1, arg2, arg3, arg4) (void)0
#endif
}
#endif

#endif
