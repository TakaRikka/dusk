#ifndef JSUPPORT_H
#define JSUPPORT_H

#include <dolphin/dolphin.h>
#include <stdint.h>

/**
* @ingroup jsystem-jsupport
* 
*/
template <typename T>
T* JSUConvertOffsetToPtr(const void* ptr, uintptr_t offset) {
    return offset == 0 ? NULL : (T*)((intptr_t)ptr + (intptr_t)offset);
}

/**
* @ingroup jsystem-jsupport
* 
*/
template <typename T>
T* JSUConvertOffsetToPtr(const void* ptr, const void* offset) {
    T* ret;
    if (offset == NULL) {
        ret = NULL;
    } else {
        ret = (T*)((intptr_t)ptr + (intptr_t)offset);
    }
    return ret;
}

inline u8 JSULoNibble(u8 param_0) { return param_0 & 0x0f; }
inline u8 JSUHiNibble(u8 param_0) {return param_0 >> 4; }

inline u8 JSULoByte(u16 in) {
    return in & 0xff;
}

inline u8 JSUHiByte(u16 in) {
    return in >> 8;
}

inline u16 JSUHiHalf(u32 in) {
    return (in >> 16);
}

inline u16 JSULoHalf(u32 param_0) {return param_0; }

#endif
