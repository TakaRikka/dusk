#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISoundHandles.h"

JAISoundHandle* JAISoundHandles::getHandleSoundID(JAISoundID id) {
    for (int i = 0; i < numHandles_; i++) {
        if (handle_[i].isSoundAttached()) {
            if (handle_[i]->getID() == id) {
                return &handle_[i];
            }
        }
    }

    return NULL;
}

JAISoundHandle* JAISoundHandles::getFreeHandle() {
    for (int i = 0; i < numHandles_; i++) {
        if (!handle_[i].isSoundAttached()) {
            return &handle_[i];
        }
    }

    return NULL;
}
