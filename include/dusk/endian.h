#ifndef DOLPHIN_ENDIAN_H
#define DOLPHIN_ENDIAN_H

#include "dolphin/types.h"

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
    #else
        #define BSWAP16(x) __builtin_bswap16(x)
        #define BSWAP32(x) __builtin_bswap32(x)
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
    operator T() const;
};

template<>
inline BE<u16>::operator u16() const {
    return RES_U16(inner);
}

template<>
inline BE<s16>::operator s16() const {
    return RES_S16(inner);
}

template<>
inline BE<u32>::operator u32() const {
    return RES_U32(inner);
}

template<>
inline BE<s32>::operator s32() const {
    return RES_S32(inner);
}

#define BE(T) BE<T>
#else
#define BE(T) T
#endif


#endif // DOLPHIN_ENDIAN_H
