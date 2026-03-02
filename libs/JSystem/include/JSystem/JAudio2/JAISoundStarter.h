#ifndef JAISOUNDSTARTER_H
#define JAISOUNDSTARTER_H

#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JAudio2/JASGadget.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundStarter : public JASGlobalInstance<JAISoundStarter> {
    JAISoundStarter(bool setInstance);

    virtual ~JAISoundStarter();
    virtual bool startSound(JAISoundID id, JAISoundHandle* handlePtr, const JGeometry::TVec3<f32>* posPtr) = 0;
    bool startLevelSound(JAISoundID id, JAISoundHandle* handlePtr, const JGeometry::TVec3<f32>* posPtr);
};

#endif /* JAISOUNDSTARTER_H */
