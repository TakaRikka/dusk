#ifndef STD_VECTOR_H
#define STD_VECTOR_H

#include "JSystem/JGadget/define.h"
#include "JSystem/JGadget/std-memory.h"
#include <algorithm>
#include <memory>
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

    typedef T* iterator;
    typedef const T* const_iterator;

    TVector(Allocator const& allocator) {
#if PLATFORM_GCN
        mAllocator = allocator;
#endif
        pBegin_ = NULL;
        pEnd_ = pBegin_;
        mCapacity = 0;
        pfnExtend_ = JGadget::vector::extend_default;
    }

    ~TVector() {
#if DEBUG
        Confirm();
#endif
        clear();
        JGADGET_ASSERTWARN(250, size()==0);
        mAllocator.deallocate(pBegin_, 0);
    }

    void insert(T* pos, u32 count, const T& val) {
        if (count == 0) {
            return;
        }

        T* ptr = Insert_raw(pos, count);
        if (ptr == end()) {
            JGADGET_WARNMSG(321, "can't allocate memory");
        } else {
            std::uninitialized_fill_n(ptr, count, val);
        }
    }

    T* Insert_raw(T* pFirst, u32 pCount) {
        T* const pIt = pFirst;
        JUT_ASSERT(446, (pBegin_<=pIt)&&(pIt<=pEnd_));

        if (pCount == 0) {
            return pFirst;
        }

        if (pCount + size() <= mCapacity) {
            void** newEnd = pIt + pCount;

            if (newEnd < pEnd_) {
                void** pOverwrittenValues = pEnd_ - pCount;
                std::uninitialized_copy(pOverwrittenValues, pEnd_, pEnd_);
                std::copy_backward(pIt, pOverwrittenValues, pEnd_);
                DestroyElement_(pIt, newEnd);

                pEnd_ += pCount;
                return pFirst;
            } else {
                std::uninitialized_copy(pIt, pEnd_, newEnd);
                DestroyElement_(pIt, pEnd_);

                pEnd_ += pCount;
                return pFirst;
            }
        }

        u32 newSize = GetSize_extend_(pCount);

        void** newDataPointer = mAllocator.allocate(newSize, 0);
        if (!newDataPointer) {
            return end();
        }

        TDestructed_deallocate_ destructionDeallocator(mAllocator, newDataPointer);

        void** const endOfCopy = std::uninitialized_copy(pBegin_, pIt, newDataPointer);
        std::uninitialized_copy(pIt, pEnd_, endOfCopy + pCount);

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

    iterator begin() { return pBegin_; }
    const_iterator begin() const { return pBegin_; }
    iterator end() { return pEnd_; }
    const_iterator end() const { return pEnd_; }

    u32 size() const {
        if (pBegin_ == 0) {
            return 0;
        }
        return (int)((uintptr_t)pEnd_ - (uintptr_t)pBegin_) / 4;
    }

    u32 capacity() const { return mCapacity; }

    bool Confirm() const {
        if (size() > mCapacity) {
            JGADGET_WARNMSG(507, "illegal size");
            return false;
        }
        return true;
    }

    u32 GetSize_extend_(u32 count) const {
        JUT_ASSERT(555, pfnExtend_!=NULL);

        u32 oldSize = size();
        u32 neededNewSpace = oldSize + count;
        u32 extendedSize = pfnExtend_(capacity(), oldSize, count);

        return neededNewSpace > extendedSize ? neededNewSpace : extendedSize;
    }

    void DestroyElement_(T* pFirst, T* pLast) {
        JUT_ASSERT(536, (pBegin_<=pFirst)&&(pFirst<=pEnd_));
        JUT_ASSERT(537, (pBegin_<=pLast)&&(pLast<=pEnd_));
        T* it = pFirst;
        for (; it != pLast; it++) {
            mAllocator.destroy(it);
        }
    }

    void DestroyElement_all_() { DestroyElement_(pBegin_, pEnd_); }

    T* erase(T* pFirst, T* pLast) {
        iterator pItFirst = pFirst;
        iterator pItLast = pLast;
        JUT_ASSERT(347, (pBegin_<=pItFirst)&&(pItFirst<=pEnd_))
        JUT_ASSERT(348, (pBegin_<=pItLast)&&(pItLast<=pEnd_));
        JUT_ASSERT(349, pItFirst<=pItLast);
        T* vectorEnd = pEnd_; // DEBUG NONMATCHING
        T* ppvVar3 = std::copy(pItLast, vectorEnd, pItFirst);
        DestroyElement_(ppvVar3, pEnd_);
        pEnd_ = ppvVar3;
        return pFirst;
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

    typedef T* iterator;
    typedef const T* const_iterator;

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
