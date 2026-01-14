#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JGadget/define.h"

#define MSL_USE_INLINES 1
#include <cctype>

JGadget_outMessage::JGadget_outMessage(MessageFunc fn, const char* file, int line) {
    mMsgFunc = fn;
    mFile = file;
    mLine = line;

    mWrite_p = mBuffer;
    *mWrite_p = 0;
}

JGadget_outMessage::~JGadget_outMessage() {
    for (u8* p = (u8*)mBuffer; p != (u8*)mWrite_p; p++) {
        char c = *p;
        if (!isprint(c) && !isspace(c)) {
            *p = '_';
        }
    }

    mMsgFunc(mFile, mLine, mBuffer);
}

JGadget_outMessage& JGadget_outMessage::operator<<(const char* sz) {
    JUT_ASSERT(99, sz!=NULL);

    while (*sz != 0 && mWrite_p < mBuffer + (BUFFER_SIZE - 1)) {
        *mWrite_p = *sz;
        mWrite_p++;
        sz++;
    }

    *mWrite_p = 0;
    return *this;
}

JGadget_outMessage& JGadget_outMessage::operator<<(char c) {
    char sz[2];
    sz[0] = c;
    sz[1] = 0;

    return *this << sz;
}

JGadget_outMessage& JGadget_outMessage::operator<<(s32 value) {
    char sz[64];
    snprintf(sz, 64, "%d", value);

    return *this << sz;
}

JGadget_outMessage& JGadget_outMessage::operator<<(u32 value) {
    char sz[64];
    snprintf(sz, 64, "%u", value);

    return *this << sz;
}

JGadget_outMessage& JGadget_outMessage::operator<<(const void* data) {
    char sz[64];
    snprintf(sz, 64, "%p", data);

    return *this << sz;
}

void JGadget_outMessage::warning(const char* file, int line, const char* message) {
    JUTAssertion::setWarningMessage(3, (char*)file, line, message);
}
