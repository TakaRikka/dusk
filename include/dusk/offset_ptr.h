#ifndef DUSK_OFFSET_PTR_H
#define DUSK_OFFSET_PTR_H

#if TARGET_PC

#include "global.h"
#include "endian.h"

struct OffsetPtr {
    // Top bit is used to store "already relocated" flag as a guard thing.
    BE<s32> value;

    bool setBase(void* base);
    bool isRelocated();

    template<typename T>
    explicit operator T*() const {
        s32 swapped = value;
        if (swapped == 0) {
            // This shouldn't be able to happen but the original offsetting code has the safeguard.
            return nullptr;
        }

        // Because we use the top (31st) bit as an "already relocated" flag,
        // we need to make sure to check the 30th bit to see if we're actually negative.
        // And cut off the 31st bit if we're supposed to be positive.
        // Effective range this gives us is still like a gigabyte in both directions,
        // so we'll be fine on that front.
        s32 realOffset = (swapped & 0x4000'0000) ? swapped : (swapped & 0x7FFF'FFFF);
        return (T*)POINTER_ADD(this, realOffset);
    }
};

template<typename T>
struct OffsetPtrT {
    OffsetPtr value;

    bool setBase(void* base) {
        return value.setBase(base);
    }
    bool isRelocated() {
        return value.isRelocated();
    }

    T* operator->() {
        return (T*) value;
    }

    operator T*() const {
        return (T*) value;
    }

    template<typename TOther>
    explicit operator TOther*() const {
        return (TOther*) value;
    }
};


#define OFFSET_PTR(T) OffsetPtrT<T>
#define OFFSET_PTR_RAW OffsetPtr
#else
#define OFFSET_PTR(T) T*
#define OFFSET_PTR_RAW u32
#endif

#endif  // DUSK_OFFSET_PTR_H
