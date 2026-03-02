#ifndef JAIAUDIBLE_H
#define JAIAUDIBLE_H

#include "JSystem/JGeometry.h"

struct JASSoundParams;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAIAudible {
public:
    virtual ~JAIAudible();
    virtual JASSoundParams* getOuterParams(int) = 0;
    virtual void calc() = 0;

    JGeometry::TVec3<float>& getPos() { return mPos; }
    void setPos(const JGeometry::TVec3<float>& other) { mPos.set(other); }

    /* 0x04 */ JGeometry::TVec3<float> mPos;
};

#endif /* JAIAUDIBLE_H */
