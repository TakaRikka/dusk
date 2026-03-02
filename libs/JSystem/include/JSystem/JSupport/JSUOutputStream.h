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

#define JSU_OUTPUTSTREAM_OPERATOR(T) \
    JSUOutputStream& operator<<(T val) { \
        write(&val, sizeof(T)); \
        return *this; \
    }

    JSU_OUTPUTSTREAM_OPERATOR(signed char)
    JSU_OUTPUTSTREAM_OPERATOR(unsigned char)
    JSU_OUTPUTSTREAM_OPERATOR(signed short)
    JSU_OUTPUTSTREAM_OPERATOR(unsigned short)
    JSU_OUTPUTSTREAM_OPERATOR(int)
    JSU_OUTPUTSTREAM_OPERATOR(unsigned int)
    JSU_OUTPUTSTREAM_OPERATOR(signed long)
    JSU_OUTPUTSTREAM_OPERATOR(unsigned long)
    JSU_OUTPUTSTREAM_OPERATOR(signed long long)
    JSU_OUTPUTSTREAM_OPERATOR(unsigned long long)

#undef JSU_OUTPUTSTREAM_OPERATOR

    JSUOutputStream& operator<<(const char* param_0) {
        write(param_0);
        return *this;
    }
};  // Size = 0x8

#endif /* JSUOUTPUTSTREAM_H */
