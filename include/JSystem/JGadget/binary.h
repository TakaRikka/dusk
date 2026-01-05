#ifndef JGADGET_BINARY_H
#define JGADGET_BINARY_H

#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JGadget/search.h"

namespace JGadget {
namespace binary {

struct TEBit {
    u32 value;
};

const void* parseVariableUInt_16_32_following(const void* pu16, u32* pu32First, u32* pu32Second, TEBit* tebit);

inline bool isPower2(unsigned int arg0) {
    return arg0 != 0 && (arg0 & arg0 - 1) == 0;
}

inline u32 align_roundUp(unsigned int arg0, unsigned int uAlign) {
    JUT_ASSERT(98, isPower2(uAlign));
    return (arg0 + uAlign - 1) & ~(uAlign - 1);
}

struct TParseData {
    TParseData(const void* pContent) : raw(pContent) {}

    const void* getRaw() const { return raw; }
    void setRaw(const void* p) { raw = p; }

    /* 0x0 */ const void* raw;
};

template <int S>
struct TParseData_aligned : public TParseData {
    TParseData_aligned(const void* pContent) : TParseData(pContent) {}
    void setRaw(const void* p) {
        /* if ((u32)p % S != 0) {
            JUTWarn w;
            w << "misaligned : " << (u32)p;
        } */
        static_cast<TParseData*>(this)->setRaw(p);
    }
};

// Base for header and/or block parsing
struct TParse_header_block {
    virtual ~TParse_header_block() = 0;

    virtual bool parseHeader_next(const void** ppData_inout, u32* puBlock_out, u32 arg2) = 0;
    virtual bool parseBlock_next(const void** ppData_inout, u32* puData_out, u32 arg2) = 0;

    bool parse_next(const void** ppData_inout, u32 a2);

    bool parse(const void* ppData_inout, u32 a2) {
        return parse_next(&ppData_inout, a2);
    }
};

template <typename T>
struct TParseValue_raw_ {
    typedef T ParseType;
    static T parse(const void* data) { return (T)*(T*)data; }
};

template <typename T>
struct TParseValue_raw : public TParseValue_raw_<T> {
    typedef TParseValue_raw_<T> InnerParseValueClass;
};

template <typename T>
struct TParseValue_endian_big_ : public TParseValue_raw_<T> {
    static T parse(const void* data) { return TParseValue_raw_<T>::parse(data); }
};

template <class Parser>
struct TParseValue : public Parser {
    static typename Parser::ParseType parse(const void* data) { return Parser::parse(data); }

    static typename Parser::ParseType parse(const void* data, s32 advanceNum) {
        return Parser::parse(advance(data, advanceNum));
    }

    static const void* advance(const void* data, s32 advanceNum) {
        return (char*)data + (advanceNum * sizeof(typename Parser::ParseType));
    }
};

template<class Parser, int size>
struct TValueIterator
        : public JGadget::TIterator<
            std::random_access_iterator_tag,
            typename Parser::ParseType,
            ptrdiff_t,
            typename Parser::ParseType*,
            typename Parser::ParseType&
        >
    {
    typedef typename Parser::ParseType ValueType;

    TValueIterator(const void* begin) {
        mBegin = reinterpret_cast<const char*>(begin);
    }

    const void* get() const { return mBegin; }

    typename Parser::ParseType operator*() const {
        return TParseValue<typename Parser::InnerParseValueClass>::parse(get());
    }

    TValueIterator& operator++() {
        mBegin += size;
        return *this;
    }

    const TValueIterator operator++(int) {
        TValueIterator old(*this);
        ++(*this);
        return old;
    }

    TValueIterator& operator+=(s32 n) {
        mBegin += size * n;
        return *this;
    }

    TValueIterator& operator--() {
        mBegin -= size;
        return *this;
    }

    char const* mBegin;
};

template<typename T>
struct TValueIterator_raw : public TValueIterator<TParseValue_raw<T>, sizeof(T)> {
    TValueIterator_raw(const void* begin) : TValueIterator<TParseValue_raw<T>, sizeof(T)>(begin) {}

    friend bool operator==(TValueIterator<TParseValue_raw<T>, sizeof(T)> a, TValueIterator<TParseValue_raw<T>, sizeof(T)> b) {
        return a.mBegin == b.mBegin;
    }

    friend bool operator!=(TValueIterator<TParseValue_raw<T>, sizeof(T)> a, TValueIterator<TParseValue_raw<T>, sizeof(T)> b) {
        return !operator==(a, b);
    }

    friend TValueIterator<TParseValue_raw<T>, sizeof(T)> operator+(TValueIterator<TParseValue_raw<T>, sizeof(T)> a, s32 b) {
        TValueIterator<TParseValue_raw<T>, sizeof(T)> it = a;
        it += b;
        return it;
    }
};

template <typename T>
struct TParseValue_misaligned_ : public TParseValue_raw_<T> {
    typedef T ParseType;
    static T parse(const void* data) { return TParseValue_raw_<T>::parse(data); }
};

template <typename T>
struct TParseValue_misaligned : public TParseValue_raw_<T> {
    typedef TParseValue_misaligned_<T> InnerParseValueClass;
};

template<typename T>
struct TValueIterator_misaligned : public TValueIterator<TParseValue_misaligned<T>, sizeof(T)> {
    TValueIterator_misaligned(const void* begin) : TValueIterator<TParseValue_misaligned<T>, sizeof(T)>(begin) {}

    friend bool operator==(TValueIterator<TParseValue_misaligned<T>, sizeof(T)> a, TValueIterator<TParseValue_misaligned<T>, sizeof(T)> b) {
        return a.mBegin == b.mBegin;
    }

    friend bool operator!=(TValueIterator<TParseValue_misaligned<T>, sizeof(T)> a, TValueIterator<TParseValue_misaligned<T>, sizeof(T)> b) {
        return !operator==(a, b);
    }

    friend TValueIterator<TParseValue_misaligned<T>, sizeof(T)> operator+(TValueIterator<TParseValue_misaligned<T>, sizeof(T)> a, s32 b) {
        TValueIterator<TParseValue_misaligned<T>, sizeof(T)> it(a);
        it += b;
        return it;
    }
};

}  // namespace binary
}  // namespace JGadget

#endif /* JGADGET_BINARY_H */
