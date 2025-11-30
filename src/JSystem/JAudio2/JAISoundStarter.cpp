#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISoundStarter.h"
#include "JSystem/JAudio2/JAISoundHandles.h"

JAISoundStarter::JAISoundStarter(bool setInstance) :
    JASGlobalInstance<JAISoundStarter>(setInstance) {}

JAISoundStarter::~JAISoundStarter() {}

bool JAISoundStarter::startLevelSound(JAISoundID id, JAISoundHandle* handlePtr, const JGeometry::TVec3<f32>* posPtr) {
    JUT_ASSERT(20, handlePtr);
    if (*handlePtr) {
        if ((*handlePtr)->getID() == id) {
            (*handlePtr)->updateLifeTime(1);
            if (posPtr != NULL) {
                (*handlePtr)->setPos(*posPtr);
            }
            return true;
        }
    }

    bool rt = startSound(id, handlePtr, posPtr);
    if (rt && *handlePtr) {
        (*handlePtr)->setLifeTime(1, false);
    }
    return rt;
}
