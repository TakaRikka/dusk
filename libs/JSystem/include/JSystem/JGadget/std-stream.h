#ifndef JGADGET_STD_STREAM_H
#define JGADGET_STD_STREAM_H

#include "JSystem/JGadget/std-streambuf.h"
#include "global.h"

namespace JGadget {
class TStream_base {
public:
    TStream_base() {}
    virtual ~TStream_base() = 0;

    void setf(u32);
    void setf(u32, u32);
    void width(s32);

    s32 precision() const {
        return precision_;
    }

    s32 width() const {
        return width_;
    }

    u32 flags() const {
        return flags_;
    }

    void Init_() {
        flags_ = skipws|dec;
        width_ = 0;
        precision_ = 6;
    }

    static const u32 skipws = 0x400000;
    static const u32 dec = 0x1;

private:
    /* 0x4 */ u32 flags_;
    /* 0x8 */ s32 width_;
    /* 0xC */ s32 precision_;
};

class TStream : public TStream_base {
public:
    TStream();
    virtual ~TStream() = 0;

    void init(TStreamBuffer* psb);
    bool fail() const;
    bool good() const;
    void setstate(u8 state);
    void clear(u8 state);
    TStreamBuffer* rdbuf() const;
    void fill(char);
    static char widen(char);
    static char narrow(char, char);

    char fill() const {
        return fill_;
    }

private:
    /* 0x10 */ u8 state_;
    /* 0x14 */ TStreamBuffer* rdbuf_;
    /* 0x18 */ char fill_;
};

class TInputStream {
public:
    struct sentry {
        sentry(TInputStream& stream, bool);

        operator bool() const { return _0x0; }

        /* 0x0 */ bool _0x0;
    };

    int get();

    TStream* _0x0;
    virtual ~TInputStream();

private:
    /* 0x08 */ int field_0x8;
};

class TOutputStream {
public:
    struct sentry {
        sentry(TOutputStream& stream) {
            _0x0 = &stream;
            
            _0x4 = stream._0x0->good();
            if (!_0x4) {
                stream._0x0->setstate(2);
            }
        }

        ~sentry() {
            if (!_0x0->_0x0->fail() && (_0x0->_0x0->flags() & 2)) {
                _0x0->flush();
            }
        }

        operator bool() const { return _0x4; }

        /* 0x0 */ TOutputStream* _0x0;
        /* 0x4 */ bool _0x4;
    };

    struct TBufferIterator {
        TBufferIterator(TStreamBuffer* psb) {
            buffer_ = psb;
        }

        void operator=(char param_0) {
            if (buffer_ != NULL && TTrait_char<char>::eq_int_type(buffer_->sputc(param_0), TTrait_char<char>::eof())) {
                buffer_ = NULL;
            }
        }

        TBufferIterator& operator*() {
            return *this;
        }

        void operator++() {}

        bool failed() const { return buffer_ == NULL; }

        TStreamBuffer* buffer_;
    };

    TOutputStream& operator<<(const char*);
    TOutputStream& operator<<(char);
    TOutputStream& operator<<(s32);
    TOutputStream& operator<<(u32);
    TOutputStream& operator<<(bool);
    TOutputStream& operator<<(double);

    void flush();
    TBufferIterator& Put_CString_prefixed_(const char*, u32, const char*, u32);
    TBufferIterator& Put_longInt_(u32, bool);
    TBufferIterator& Put(double);

    TBufferIterator& Put(const char* param_0, u32 param_1) {
        return Put_CString_prefixed_(param_0, param_1, NULL, 0);
    }

    TBufferIterator& Put(s32 param_0) {
        return Put_longInt_(param_0, true);
    }

    TBufferIterator& Put(u32 param_0) {
        return Put_longInt_(param_0, false);
    }

    TBufferIterator& Put(bool param_0) {
        u32 flags = _0x0->flags();
        const TCString_* s = &saaosz_bool_[!(flags & 8) ? 0 : 1][param_0 ? 1 : 0];
        return Put_CString_prefixed_(s->sz, s->len, NULL, 0);
    }

    TStream* _0x0;
    virtual ~TOutputStream();
    
    struct TCString_ {
        const char* sz;
        u32 len;
    };
    
    struct saoCaseNumeral_struct {
        const char _0[16];
        TCString_ _1; // 0x10
        TCString_ _2; // 0x18
        TCString_ _3; // 0x20
    };

    static const saoCaseNumeral_struct saoCaseNumeral_[2];
    static const TCString_ saoszPrefix_sign_[3];
    static const TCString_ saaosz_bool_[2][2];
    static const TCString_ soszPrefix_oct_;

    static const saoCaseNumeral_struct& getCaseNumeral_(u32 flags) {
        return saoCaseNumeral_[!(flags & 4) ? 0 : 1];
    }

private:
    /* 0x08 */ int field_0x8;
};

}

#endif /* JGADGET_STD_STREAM_H */
