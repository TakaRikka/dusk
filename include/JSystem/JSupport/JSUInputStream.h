#ifndef JSUINPUTSTREAM_H
#define JSUINPUTSTREAM_H

#include "JSystem/JSupport/JSUIosBase.h"

/**
* @ingroup jsystem-jsupport
* 
*/
class JSUInputStream : public JSUIosBase {
public:
    JSUInputStream() {}
    virtual ~JSUInputStream();

    /* vt[3] */ virtual s32 getAvailable() const = 0;
    /* vt[4] */ virtual s32 skip(s32);
    /* vt[5] */ virtual u32 readData(void*, s32) = 0;

    u32 readU32() {
        u32 val;
        this->read(&val, sizeof(val));
        return val;
    }

    u32 read32b() {
        u32 val;
        this->read(&val, sizeof(val));
        return val;
    }

    s32 readS32() {
        s32 val;
        this->read(&val, sizeof(val));
        return val;
    }

    s16 readS16() {
        s16 val;
        this->read(&val, sizeof(val));
        return val;
    }

    u16 readU16() {
        u16 val;
        this->read(&val, sizeof(val));
        return val;
    }

    u8 readU8() {
        u8 val;
        this->read(&val, sizeof(val));
        return val;
    }

    u8 read8b() {
        u8 val;
        this->read(&val, sizeof(val));
        return val;
    }

    u16 read16b() {
        u16 val;
        this->read(&val, sizeof(val));
        return val;
    }

    JSUInputStream& operator>>(u32& dest) {
        read(&dest, 4);
        return *this;
    }

    JSUInputStream& operator>>(u16& dest) {
        read(&dest, 2);
        return *this;
    }

    JSUInputStream& operator>>(u8& dest) {
        read(&dest, 1);
        return *this;
    }

    JSUInputStream& operator>>(s16& dest) {
        read(&dest, 2);
        return *this;
    }

    JSUInputStream& operator>>(char* dest) {
        read(dest);
        return *this;
    }

    s32 read(u32& param_0) {
        return read(&param_0, 4);
    }

    // TODO: return value probably wrong
    s32 read(void*, s32);
    char* read(char*);
};  // Size = 0x8

// move?
/**
* @ingroup jsystem-jsupport
* 
*/
template <typename T>
T* JSUConvertOffsetToPtr(const void*, const void*);

#endif /* JSUINPUTSTREAM_H */
