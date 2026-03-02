#ifndef VECTOR_H
#define VECTOR_H

#include "JSystem/JGadget/std-memory.h"
#include "types.h"

namespace JGadget {

namespace vector {

u32 extend_default(u32 arg1, u32 arg2, u32 arg3);

typedef u32 (*ExtendFunc)(u32, u32, u32);

}  // namespace vector

template <typename T, template <class> class Allocator>
struct TVector {
    TVector(Allocator<T> alloc) {
        _0 = NULL;
        pBegin_ = _0;
        _c = NULL;
        extend = vector::extend_default;
    }

    inline u32 size() const {
        if (pBegin_ == NULL) {
            return 0;
        }

        return ((int)pEnd_ - (int)pBegin_) / 4;
    }

    void **const begin() const { return pBegin_; }
    void **const end() const { return pEnd_; }
    void** begin() { return pBegin_; }
    void** end() { return pEnd_; }
    // void erase(void** arg1, void** arg2) {}

    void** _0;
    void** pBegin_;
    void** pEnd_;
    u32 _c;
    vector::ExtendFunc extend;
};

struct TVector_pointer_void : TVector<void*, TAllocator> {
    TVector_pointer_void(JGadget::TAllocator<void*> const&);
    ~TVector_pointer_void();
    void erase(void**, void**);
    void insert(void**, void* const&);

    void clear() { erase(begin(), end()); }
    void push_back(void* const& ref) { insert(end(), ref); }
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
        static_cast<TVector_pointer_void*>(this)->push_back(ref);
    }
};

}  // namespace JGadget

#endif /* VECTOR_H */
