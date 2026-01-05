#ifndef JGADGET_STD_STREAMBUF_H
#define JGADGET_STD_STREAMBUF_H

#include <dolphin/types.h>
#include <string>

namespace JGadget {
template <typename T>
struct TTrait_char {};

template <>
struct TTrait_char<char> {
    static int eof() { return -1; }
    static int to_int_type(char value) { return (int)value; }
    static char to_char_type(int value) { return (char)value; }
    static bool eq_int_type(int a, int b) { return a == b; }
    static size_t length(const char* sz) { return strlen(sz); }
};

class TStreamBuffer {
public:
    TStreamBuffer() {
        pBase_get_ = NULL;
        pEnd_get_ = NULL;
        pCurrent_get_ = NULL;
        pBase_put_ = NULL;
        pEnd_put_ = NULL;
        pCurrent_put_ = NULL;
    }

    virtual ~TStreamBuffer() = 0;

    int sputc(char param_0) {
        if (pCurrent_put_ < pEnd_put_) {
            *pCurrent_put_ = param_0;
            pCurrent_put_++;
            return TTrait_char<char>::to_int_type(param_0);
        } else {
            return overflow(TTrait_char<char>::to_int_type(param_0));
        }
    }

    int sgetc();

    int sbumpc() {
        if (pCurrent_get_ < pEnd_get_) {
            int var_r29 = TTrait_char<char>::to_int_type(*pCurrent_get_);
            pCurrent_get_++;
            return var_r29;
        }

        return uflow();
    }

    int snextc() {
        int var_r31 = TTrait_char<char>::eof();
        return TTrait_char<char>::eq_int_type(sbumpc(), var_r31) ? var_r31 : sgetc();
    }

    virtual void setbuf(char*, s32);
    virtual s32 sync();
    virtual int underflow();
    virtual int uflow();
    virtual s32 xsputn(const char*, s32);
    virtual int overflow(int);

    /* 0x04 */ char* pBase_get_;
    /* 0x08 */ char* pEnd_get_;
    /* 0x0C */ char* pCurrent_get_;
    /* 0x10 */ char* pBase_put_;
    /* 0x14 */ char* pEnd_put_;
    /* 0x18 */ char* pCurrent_put_;
};
}

#endif /* JGADGET_STD_STREAMBUF_H */
