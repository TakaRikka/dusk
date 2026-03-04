#ifndef POINTER_H
#define POINTER_H

#include "JSystem/JKernel/JKRHeap.h"

namespace JGadget {

template<class T>
class TPointer {
public:
    TPointer(T* ptr) : mPtr(ptr) {}
    ~TPointer() {}
    void set(T* ptr) { mPtr = ptr; }
    T* mPtr;
};

template<class T>
class TPointer_delete : public TPointer<T> {
public:
#ifdef __MWERKS__
    TPointer_delete(T* ptr) : TPointer(ptr) {}
    ~TPointer_delete() {
        JKR_DELETE(mPtr);
    }
#else
    TPointer_delete(T* ptr) : TPointer<T>(ptr) {}
    ~TPointer_delete() {
        JKR_DELETE(this->mPtr);
    }
#endif
};

}

#endif
