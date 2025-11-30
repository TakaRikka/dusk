#ifndef STD_VECTOR_H
#define STD_VECTOR_H

#include "JSystem/JGadget/std-memory.h"
#include <algorithm.h>
#include <msl_memory.h>
#include <stdint.h>

namespace JGadget {
namespace vector {
u32 extend_default(u32, u32, u32);
};

typedef u32 (*extendFunc)(u32, u32, u32);

template <typename T, typename Allocator = JGadget::TAllocator<T> >
struct TVector {
    struct TDestructed_deallocate_ {
        TDestructed_deallocate_(JGadget::TAllocator<T>& allocator, T* ptr) {
            mAllocator = &allocator;
            mPtr = ptr;
        }

        ~TDestructed_deallocate_() { mAllocator->deallocate(mPtr, 0); }

        void set(T* ptr) { mPtr = ptr; }

        Allocator* mAllocator;
        T* mPtr;
    };

    TVector(Allocator const& allocator) {
        mAllocator = allocator;
        pBegin_ = NULL;
        pEnd_ = pBegin_;
        mCapacity = 0;
        pfnExtend_ = JGadget::vector::extend_default;
    }

    ~TVector() {
        clear();
        mAllocator.deallocate(pBegin_, 0);
    }

    void insert(T* pos, u32 count, const T& val) {
        if (count != 0) {
            T* ptr = Insert_raw(pos, count);
            if (ptr == end()) {
                /* JGadget_outMessage sp120(JGadget_outMessage::warning, __FILE__, 0x141);
                sp120 << "can't allocate memory"; */
            } else {
                std::uninitialized_fill_n(ptr, count, val);
            }
        }
    }

    T* Insert_raw(T* pIt, u32 pCount) {
        JUT_ASSERT(446, (pBegin_ <= pIt) && (pIt <= pEnd_));

        T* const pFirst = pIt;

        if (pCount == 0) {
            return pIt;
        }

        if (pCount + size() <= mCapacity) {
            void** newEnd = pFirst + pCount;

            if (newEnd < pEnd_) {
                void** pOverwrittenValues = pEnd_ - pCount;
                std::uninitialized_copy(pOverwrittenValues, pEnd_, pEnd_);
                std::copy_backward(pFirst, pOverwrittenValues, pEnd_);
                DestroyElement_(pFirst, newEnd);

                pEnd_ += pCount;
                return pIt;
            } else {
                std::uninitialized_copy(pFirst, pEnd_, newEnd);
                DestroyElement_(pFirst, pEnd_);

                pEnd_ += pCount;
                return pIt;
            }
        }

        u32 newSize = GetSize_extend_(pCount);

        void** newDataPointer = mAllocator.allocate(newSize, 0);
        if (!newDataPointer) {
            return end();
        }

        TDestructed_deallocate_ destructionDeallocator(mAllocator, newDataPointer);

        void** const endOfCopy = std::uninitialized_copy(pBegin_, pFirst, newDataPointer);
        std::uninitialized_copy(pFirst, pEnd_, endOfCopy + pCount);

        DestroyElement_all_();
        destructionDeallocator.set(pBegin_);

        pEnd_ = newDataPointer + (pEnd_ - pBegin_ + pCount);
        pBegin_ = newDataPointer;
        mCapacity = newSize;

        return endOfCopy;
    }

    T* insert(T* pos, const T& val) {
        u32 diff = (int)((uintptr_t)pos - (uintptr_t)begin()) / 4;
        insert(pos, 1, val);
        return pBegin_ + diff;
    }

    T* begin() const { return pBegin_; }

    T* end() const { return pEnd_; }

    u32 size() const {
        if (pBegin_ == 0) {
            return 0;
        }
        return (int)((uintptr_t)pEnd_ - (uintptr_t)pBegin_) / 4;
    }

    u32 capacity() { return mCapacity; }

    u32 GetSize_extend_(u32 count) {
        JUT_ASSERT(0x22B, pfnExtend_!=NULL);

        u32 oldSize = size();
        u32 neededNewSpace = oldSize + count;
        u32 extendedSize = pfnExtend_(capacity(), oldSize, count);

        return neededNewSpace > extendedSize ? neededNewSpace : extendedSize;
    }

    void DestroyElement_(T* start, T* end) {
        for (; start != end; start++) {
            mAllocator.destroy(start);
        }
    }

    void DestroyElement_all_() { DestroyElement_(pBegin_, pEnd_); }

    T* erase(T* start, T* end) {
        T* vectorEnd = pEnd_;
        T* ppvVar3 = std::copy(end, vectorEnd, start);
        DestroyElement_(ppvVar3, pEnd_);
        pEnd_ = ppvVar3;
        return start;
    }

    void clear() { erase(begin(), end()); }

    /* 0x00 */ Allocator mAllocator;
    /* 0x04 */ T* pBegin_;
    /* 0x08 */ T* pEnd_;
    /* 0x0C */ u32 mCapacity;
    /* 0x10 */ extendFunc pfnExtend_;
};

struct TVector_pointer_void : public TVector<void*, TAllocator<void*> > {
    TVector_pointer_void(const JGadget::TAllocator<void*>& allocator);
    TVector_pointer_void(u32, void* const&,
                         const JGadget::TAllocator<void*>& allocator);

    ~TVector_pointer_void();

    void insert(void**, void* const&);
    void** erase(void**, void**);

    void clear() { erase(begin(), end()); }
    void push_back(void* const& value) { insert(end(), (void* const&)value); }
};

template <typename T>
struct TVector_pointer : TVector_pointer_void {
    TVector_pointer(const TAllocator<void*>& allocator) : TVector_pointer_void(allocator) {}
    ~TVector_pointer() {}

    const T* begin() const { return (const T*)TVector_pointer_void::begin(); }
    T* begin() { return (T*)TVector_pointer_void::begin(); }

    const T* end() const { return (const T*)TVector_pointer_void::end(); }
    T* end() { return (T*)TVector_pointer_void::end(); }

    void push_back(const T& ref) {
        static_cast<TVector_pointer_void*>(this)->push_back((void* const&)ref);
    }
};

}  // namespace JGadget

#endif /* STD_VECTOR_H */
