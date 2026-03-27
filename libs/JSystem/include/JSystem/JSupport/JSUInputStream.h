#ifndef JSUINPUTSTREAM_H
#define JSUINPUTSTREAM_H

#include "JSystem/JSupport/JSUIosBase.h"
#include "dusk/endian.h"

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
        BE<u32> val;
        this->read(&val, sizeof(val));
        return val;
    }

    u32 read32b() {
        BE<u32> val;
        this->read(&val, sizeof(val));
        return val;
    }

    s32 readS32() {
        BE<s32> val;
        this->read(&val, sizeof(val));
        return val;
    }

    s16 readS16() {
        BE<s16> val;
        this->read(&val, sizeof(val));
        return val;
    }

    u16 readU16() {
        BE<u16> val;
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
        BE<u16> val;
        this->read(&val, sizeof(val));
        return val;
    }

    JSUInputStream& operator>>(u32& dest) {
        read(&dest, 4);
        be_swap(dest);
        return *this;
    }

    JSUInputStream& operator>>(u16& dest) {
        read(&dest, 2);
        be_swap(dest);
        return *this;
    }

    JSUInputStream& operator>>(u8& dest) {
        read(&dest, 1);
        return *this;
    }

    JSUInputStream& operator>>(s16& dest) {
        read(&dest, 2);
        be_swap(dest);
        return *this;
    }

    JSUInputStream& operator>>(char* dest) {
        read(dest);
        return *this;
    }

    s32 read(bool& val) { return read(&val, sizeof(bool)); }

    s32 read(u8& val) { return read(&val, sizeof(u8)); }

    s32 read(u32& param_0) {
        auto ret = read(&param_0, 4);
        be_swap(param_0);
        return ret;
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
