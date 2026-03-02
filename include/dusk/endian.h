#ifndef DOLPHIN_ENDIAN_H
#define DOLPHIN_ENDIAN_H

#include <bit>

#include "dolphin/types.h"
#include "dolphin/mtx.h"

// Platform detection - Little Endian targets
#if defined(_WIN32) || defined(__x86_64__) || defined(__i386__) || defined(__aarch64__) || defined(_M_X64) || defined(_M_IX86)
    #define TARGET_LITTLE_ENDIAN 1
#else
    #define TARGET_LITTLE_ENDIAN 0
#endif

#if TARGET_LITTLE_ENDIAN
    #ifdef _MSC_VER
        #include <stdlib.h>
        #define BSWAP16(x) _byteswap_ushort(x)
        #define BSWAP32(x) _byteswap_ulong(x)
        #define BSWAP64(x) _byteswap_uint64(x)
    #else
        #define BSWAP16(x) __builtin_bswap16(x)
        #define BSWAP32(x) __builtin_bswap32(x)
        #define BSWAP64(x) __builtin_bswap64(x)
    #endif
#else
    #define BSWAP16(x) (x)
    #define BSWAP32(x) (x)
#endif

// Big-Endian to Host conversion
inline u16 be16(u16 val) { return BSWAP16(val); }
inline s16 be16s(s16 val) { return (s16)BSWAP16((u16)val); }
inline u32 be32(u32 val) { return BSWAP32(val); }
inline s32 be32s(s32 val) { return (s32)BSWAP32((u32)val); }
inline u64 be64(u64 val) { return BSWAP64(val); }
inline s64 be64s(s64 val) { return (s64)BSWAP64((u64)val); }

#ifdef TARGET_PC
// Helper wrappers so code below reads nicely:
static inline u16 RES_U16(u16 v) {
    return be16(v);
}
static inline s16 RES_S16(s16 v) {
    return be16s(v);
}
static inline u32 RES_U32(u32 v) {
    return be32(v);
}
static inline s32 RES_S32(s32 v) {
    return be32s(v);
}
static inline u64 RES_U64(u64 v) {
    return be64(v);
}
static inline s64 RES_S64(s64 v) {
    return be64s(v);
}
static inline f32 RES_F32(f32 v) {
    return std::bit_cast<f32, s32>(RES_S32(std::bit_cast<s32, f32>(v)));
}
#else
// On GameCube host-endian == file-endian, these are no-ops (keep as macros to allow compile in
// original code paths)
#define RES_U16(x) (x)
#define RES_S16(x) (x)
#define RES_U32(x) (x)
#define RES_S32(x) (x)
#endif

#ifdef TARGET_PC

/*
 * Declares a big-endian integer type.
 */
template<class T>
struct BE {
    T inner;
    BE() = default;
    BE(const T& from) {
        inner = swap(from);
    }

    // post-ops
    T operator--(int) {
        T orig = inner;
        *this -= 1;
        return swap(orig);
    }

    T operator++(int) {
        T orig = inner;
        *this += 1;
        return swap(orig);
    }

    operator T() const {
        return swap(inner);
    }

    T host[[nodiscard]]() const {
        return swap(inner);
    }

    static T swap[[nodiscard]](T val);
};

#define BIN_ASSIGN_OP(op)                          \
                                                   \
template<typename TA, typename TB>                 \
constexpr BE<TA>& operator op(BE<TA>& a, TB b) {   \
    TA aCopy = a;                                  \
    aCopy op b;                                    \
    a = aCopy;                                     \
    return a;                                      \
}

BIN_ASSIGN_OP(&=);
BIN_ASSIGN_OP(|=);
BIN_ASSIGN_OP(+=);
BIN_ASSIGN_OP(-=);
BIN_ASSIGN_OP(/=);

#undef BIN_ASSIGN_OP


template<>
inline u16 BE<u16>::swap(u16 val) {
    return RES_U16(val);
}

template<>
inline s16 BE<s16>::swap(s16 val) {
    return RES_S16(val);
}

template<>
inline u32 BE<u32>::swap(u32 val) {
    return RES_U32(val);
}

template<>
inline s32 BE<s32>::swap(s32 val) {
    return RES_S32(val);
}

template<>
inline s64 BE<s64>::swap(s64 val) {
    return RES_S64(val);
}

template<>
inline u64 BE<u64>::swap(u64 val) {
    return RES_U64(val);
}

template<>
inline f32 BE<f32>::swap(f32 val) {
    return RES_F32(val);
}

template<>
inline S16Vec BE<S16Vec>::swap(S16Vec val) {
    return {
        BE<s16>::swap(val.x),
        BE<s16>::swap(val.y),
        BE<s16>::swap(val.z),
    };
}

template<>
struct BE<Vec> {
    BE<f32> x;
    BE<f32> y;
    BE<f32> z;

    BE() = default;
    BE(f32 x, f32 y, f32 z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    BE(const Vec& from) {
        x = from.x;
        y = from.y;
        z = from.z;
    }

    operator Vec() const {
        return { x, y, z };
    }

    static Vec swap(Vec val) {
        return {
            BE<f32>::swap(val.x),
            BE<f32>::swap(val.y),
            BE<f32>::swap(val.z),
        };
    }
};

template <>
struct BE<Mtx44> {
    BE<f32> contents[4][4];

    auto& operator[](int x) const {
        return contents[x];
    }
};

template<typename T>
void be_swap(T& val) {
    val = BE<T>::swap(val);
}

template<typename T, u32 N>
void be_swap(T (& val)[N]) {
    for (u32 i = 0; i < N; i++) {
        be_swap(val[i]);
    }
    val = BE<T>::swap(val);
}

template<>
inline void be_swap(Mtx44& val) {
    for (auto & x : val) {
        for (float & y : x) {
            be_swap(y);
        }
    }
}

#define BE(T) BE<T>
#define BE_HOST(T) (T.host())
#else
#define BE(T) T
#define BE_HOST(T) (T)
#endif


#endif // DOLPHIN_ENDIAN_H
