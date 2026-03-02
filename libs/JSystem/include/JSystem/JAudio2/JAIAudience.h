#ifndef JAIAUDIENCE_H
#define JAIAUDIENCE_H

#include "JSystem/JGeometry.h"

class JAIAudible;
class JAISoundID;
struct JASSoundParams;

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAIAudience {
    virtual ~JAIAudience();
    virtual JAIAudible* newAudible(JGeometry::TVec3<f32> const&, JAISoundID,
                                   JGeometry::TVec3<f32> const*, u32) = 0;
    virtual int getMaxChannels() = 0;
    virtual void deleteAudible(JAIAudible*) = 0;
    virtual u32 calcPriority(JAIAudible*) = 0;
    virtual void mixChannelOut(JASSoundParams const&, JAIAudible*, int) = 0;
};

#endif /* JAIAUDIENCE_H */
