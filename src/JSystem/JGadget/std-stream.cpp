#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JGadget/std-stream.h"
#include "JSystem/JGadget/define.h"
#include <ctype>
#include <float.h>

namespace JGadget {
namespace {
    namespace floatingpoint_ {
        int classify(double x) {
            int fp = __fpclassifyd(x);
            switch (fp) {
            case FP_ZERO:
                return 1;
            case FP_SUBNORMAL:
                return 2;
            case FP_NORMAL:
                return 3;
            case FP_INFINITE:
                return 4;
            case FP_QNAN:
                return 5;
            }

            return 0;
        }
    }
}

TStream_base::~TStream_base() {}
TStream::~TStream() {}

void TStream::init(TStreamBuffer* psb) {
    Init_();
    state_ = 0;
    rdbuf_ = psb;
    fill_ = widen(' ');

    JUT_ASSERT(126, rdbuf()==psb);
    JUT_ASSERT(127, flags()==(skipws|dec));
    JUT_ASSERT(128, width()==0);
    JUT_ASSERT(129, precision()==6);
    JUT_ASSERT(130, fill()==' ');
}

// NONMATCHING
TInputStream::sentry::sentry(TInputStream& stream, bool param_1) {
    _0x0 = stream._0x0->good();
    if (!_0x0) {
        stream._0x0->setstate(2);
        return;
    }

    if (!param_1 && (stream._0x0->flags() & TStream_base::dec)) {
        TStreamBuffer* rdbuf = stream._0x0->rdbuf();
        int var_r28 = rdbuf->sgetc();

        while (true) {
            var_r28 = rdbuf->snextc();

            if (TTrait_char<char>::eq_int_type(var_r28, TTrait_char<char>::eof())) {
                break;
            }

            if (!isspace(TTrait_char<char>::to_char_type(var_r28))) {
                break;
            }
        }
    }

    _0x0 = stream._0x0->good();
}

// NONMATCHING
TInputStream::~TInputStream() {}

int TInputStream::get() {
    int var_r29;
    int var_r28 = TTrait_char<char>::eof();
    field_0x8 = 0;

    var_r29 = var_r28;
    sentry entry(*this, true);
    if (entry) {
        TStreamBuffer* var_r26 = _0x0->rdbuf();
        int var_r27 = 0;

        var_r29 = var_r26->sbumpc();
        if (TTrait_char<char>::eq_int_type(var_r29, var_r28)) {
            var_r27 |= 0x3;
        } else {
            field_0x8++;
        }
    }

    return var_r29;
}

// NONMATCHING
TOutputStream::~TOutputStream() {}

void TOutputStream::flush() {}

// NONMATCHING
TOutputStream& TOutputStream::operator<<(const char* param_0) {
    sentry entry(*this);
    if (entry) {
        TBufferIterator& sp8 = Put(param_0, TTrait_char<char>::length(param_0));
        if (TBufferIterator(sp8).failed()) {
            _0x0->setstate(6);
        }
    }

    return *this;
}

// NONMATCHING
TOutputStream& TOutputStream::operator<<(char param_0) {
    char sz[2];
    sz[0] = param_0;

    sentry entry(*this);
    if (entry) {
        TBufferIterator& sp8 = Put(sz, 1);
        if (TBufferIterator(sp8).failed()) {
            _0x0->setstate(6);
        }
    }

    return *this;
}

// NONMATCHING
TOutputStream& TOutputStream::operator<<(s32 param_0) {
    sentry entry(*this);
    if (entry) {
        TBufferIterator& sp8 = Put(param_0);
        if (TBufferIterator(sp8).failed()) {
            _0x0->setstate(6);
        }
    }

    return *this;
}

// NONMATCHING
TOutputStream& TOutputStream::operator<<(u32 param_0) {
    sentry entry(*this);
    if (entry) {
        TBufferIterator& sp8 = Put(param_0);
        if (TBufferIterator(sp8).failed()) {
            _0x0->setstate(6);
        }
    }

    return *this;
}

// NONMATCHING
TOutputStream& TOutputStream::operator<<(bool param_0) {
    sentry entry(*this);
    if (entry) {
        TBufferIterator& sp8 = Put(param_0);
        if (TBufferIterator(sp8).failed()) {
            _0x0->setstate(6);
        }
    }

    return *this;
}

// NONMATCHING
TOutputStream& TOutputStream::operator<<(double param_0) {
    sentry entry(*this);
    if (entry) {
        TBufferIterator& sp8 = Put(param_0);
        if (TBufferIterator(sp8).failed()) {
            _0x0->setstate(6);
        }
    }

    return *this;
}

// NONMATCHING
TOutputStream::TBufferIterator& TOutputStream::Put(double param_0) {
    u32 flags = _0x0->flags();
    
    const saoCaseNumeral_struct& sp24 = getCaseNumeral_(flags);

    const TCString_* var_r28 = &saoszPrefix_sign_[0];
    if (param_0 < 0.0f) {
        var_r28 = &saoszPrefix_sign_[2];
        param_0 = -param_0;
    } else if (flags & 0x1000) {
        var_r28 = &saoszPrefix_sign_[1];
    }

    TCString_ sp30;
    int sp1C = floatingpoint_::classify(param_0);
    switch (sp1C) {
    case 5:
        sp30 = sp24._3;
        break;
    case 4:
        sp30 = sp24._2;
        break;
    default:
        char sp28[8];
        char* var_r30 = sp28;

        *var_r30 = '%';
        if (flags & 0x800) {
            *++var_r30 = '#';
        }
        *++var_r30 = '.';
        *++var_r30 = '*';

        static const char pcFormat[] = "gGffeE";
        const char* var_r29 = pcFormat;

        u32 sp18 = flags & 0xF0000000;
        switch (sp18) {
        case 0:
            break;
        case 0x10000000:
            var_r29 += 2;
            break;
        case 0x20000000:
            var_r29 += 4;
            break;
        default:
            JGADGET_WARNMSG1(580, "illegal float-field : ", sp18);
            bool sp9 = false;
        }

        if (flags & 4) {
            var_r29++;
        }

        *++var_r30 = *var_r29;
        *++var_r30 = 0;

        char sp38[64];
        int sp14 = snprintf(sp38, 64, sp28, param_0, _0x0->precision());
        sp30.sz = sp38;
        
        int sp10;
        bool sp8 = sp14 >= 0 && (u32)sp14 < 64;
        if (sp8) {
            sp10 = sp14;
        } else {
            JUT_ASSERT(590, false);
            int spC = 0x3F;
            sp10 = spC;
        }

        sp30.len = sp10;
    }

    return Put_CString_prefixed_(sp30.sz, sp30.len, var_r28->sz, var_r28->len);
}

TOutputStream::TBufferIterator& TOutputStream::Put_CString_prefixed_(const char* param_0, u32 param_1, const char* param_2, u32 param_3) {
    TBufferIterator sp8(_0x0->rdbuf());
    if (sp8.failed()) {
        return sp8;
    }

    u32 flags = _0x0->flags();
    char fill = _0x0->fill();
    s32 width = _0x0->width();

    s32 var_r26 = param_1 + param_3;
    s32 var_r31 = 0;
    if (width > var_r26) {
        var_r31 = width - var_r26;
    }

    if (!(flags & 0x50000)) {
        while (var_r31 != 0) {
            *sp8 = fill;
            var_r31--;
            ++sp8;
        }
    }

    while (param_3 != 0) {
        *sp8 = *param_2;
        param_3--;
        param_2++;
        ++sp8;
    }

    if (flags & 0x40000) {
        while (var_r31 != 0) {
            *sp8 = fill;
            var_r31--;
            ++sp8;
        }
    }

    while (param_1 != 0) {
        *sp8 = *param_0;
        param_1--;
        param_0++;
        ++sp8;
    }

    if (flags & 0x10000) {
        while (var_r31 != 0) {
            *sp8 = fill;
            var_r31--;
            ++sp8;
        }
    }

    _0x0->width(0);
    return sp8;
}

// NONMATCHING - stack
TOutputStream::TBufferIterator& TOutputStream::Put_longInt_(u32 param_0, bool param_1) {
    bool spC = false;

    char buf[33];
    char* pbufBegin = buf;
    char* pbufEnd = &buf[32];
    char* pbuf = pbufEnd;

    u32 flags = _0x0->flags();
    
    const saoCaseNumeral_struct& sp20 = getCaseNumeral_(flags);
    const char* sp1C = sp20._0;
    int sp18 = 10;

    const TCString_* var_r29 = &saoszPrefix_sign_[0];

    u32 sp14 = flags & 0xFC00000;
    switch (sp14) {
    case 0x400000:
        if (param_1) {
            int sp10 = param_0;
            if (sp10 < 0) {
                var_r29 = &saoszPrefix_sign_[2];
                param_0 = -sp10;
            } else if (flags & 0x1000) {
                var_r29 = &saoszPrefix_sign_[1];
            }
        }
        break;
    case 0x800000:
        sp18 = 0x10;
        if (param_0 != 0 && (flags & 0x400)) {
            var_r29 = &sp20._1;
        }
        break;
    case 0x1000000:
        sp18 = 8;
        if (param_0 != 0 && (flags & 0x400)) {
            var_r29 = &soszPrefix_oct_;
        }
        break;
    default:
        JGADGET_WARNMSG1(737, "illegal base-field : ", sp14);
        bool spA = false;
    }

    if (param_0 == 0) {
        pbuf--;
        *pbuf = TStream::widen('0');
    } else {
        do {
            pbuf--;
            *pbuf = TStream::widen(sp1C[param_0 % sp18]);
            param_0 /= sp18;
        } while (param_0 != 0);
    }

    JUT_ASSERT(737, (pbufBegin<=pbuf)&&(pbuf<pbufEnd));
    return Put_CString_prefixed_(pbuf, pbufEnd - pbuf, var_r29->sz, var_r29->len);
}

const TOutputStream::saoCaseNumeral_struct TOutputStream::saoCaseNumeral_[2] = {
    {
        {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'},
        {"0x", 2},
        {"inf", 3},
        {"nan", 3},
    },
    {
        {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'},
        {"0X", 2},
        {"INF", 3},
        {"NAN", 3},
    },
};

const TOutputStream::TCString_ TOutputStream::saoszPrefix_sign_[3] = {
    {"", 0},
    {"+", 1},
    {"-", 1},
};

const TOutputStream::TCString_ TOutputStream::saaosz_bool_[2][2] = {
    {{"0", 1}, {"1", 1}},
    {{"false", 5}, {"true", 4}},
};

const TOutputStream::TCString_ TOutputStream::soszPrefix_oct_ = {"0", 1};

}
