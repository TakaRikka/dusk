#ifndef POINTER_H
#define POINTER_H

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
    TPointer_delete(T* ptr) : TPointer<T>::TPointer(ptr) {}
    ~TPointer_delete() {
        delete this->mPtr;
    }
};

}

#endif
