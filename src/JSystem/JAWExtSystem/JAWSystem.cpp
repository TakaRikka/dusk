#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAWExtSystem/JAWSystem.h"

JAWSystemInterface* JAWSystemInterface::sInstance;

JAWSystemInterface::JAWSystemInterface() {
    mHeap = NULL;
    sInstance = this;
}

JKRHeap* JAWSystemInterface::getCurrentHeap() const {
    if (mHeap) {
        return mHeap;
    }
    return JKRGetCurrentHeap();
}
