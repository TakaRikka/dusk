#ifndef JSUOUTPUTSTREAM_H
#define JSUOUTPUTSTREAM_H

#include "JSystem/JSupport/JSUIosBase.h"

/**
* @ingroup jsystem-jsupport
* 
*/
class JSUOutputStream : public JSUIosBase {
public:
    JSUOutputStream() {}
    virtual ~JSUOutputStream();

    virtual s32 skip(s32, s8);
	virtual s32 writeData(const void*, s32) = 0;

    s32 write(const void*, s32);
	void write(const char*);

#ifdef _MSVC_LANG
    JSUOutputStream& operator<<(uintptr_t param_0) {
        write(&param_0, sizeof(uintptr_t));
        return *this;
    }
#endif

	JSUOutputStream& operator<<(u32 param_0) {
        write(&param_0, sizeof(u32));
        return *this;
    }

    JSUOutputStream& operator<<(s32 param_0) {
        write(&param_0, sizeof(s32));
        return *this;
    }

    JSUOutputStream& operator<<(s16 param_0) {
        write(&param_0, sizeof(s16));
        return *this;
    }

    JSUOutputStream& operator<<(u16 param_0) {
        write(&param_0, sizeof(u16));
        return *this;
    }

    JSUOutputStream& operator<<(u8 param_0) {
        write(&param_0, sizeof(u8));
        return *this;
    }

    JSUOutputStream& operator<<(const char* param_0) {
        write(param_0);
        return *this;
    }
};  // Size = 0x8

#endif /* JSUOUTPUTSTREAM_H */
