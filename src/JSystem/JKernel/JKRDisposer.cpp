#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JKernel/JKRHeap.h"

JKRDisposer::JKRDisposer() : mLink(this) {
    mHeap = JKRHeap::findFromRoot(this);
    if (mHeap) {
        mHeap->appendDisposer(this);
    }
}

JKRDisposer::~JKRDisposer() {
    JKRHeap* heap = mHeap;
    if (heap) {
        heap->removeDisposer(this);
    }
}
