#ifndef JAISOUNDINFO_H
#define JAISOUNDINFO_H

#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JAudio2/JASGadget.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundInfo : public JASGlobalInstance<JAISoundInfo> {
    JAISoundInfo(bool);
    virtual int getSoundType(JAISoundID) const = 0;
    virtual int getCategory(JAISoundID) const = 0;
    virtual u32 getPriority(JAISoundID) const = 0;
    virtual void getSeInfo(JAISoundID, JAISe*) const = 0;
    virtual void getSeqInfo(JAISoundID, JAISeq*) const = 0;
    virtual void getStreamInfo(JAISoundID, JAIStream*) const = 0;
    virtual ~JAISoundInfo();
};

#endif /* JAISOUNDINFO_H */
