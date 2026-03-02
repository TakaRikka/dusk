#ifndef JUTFADER_H
#define JUTFADER_H

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/TColor.h"

/**
* @ingroup jsystem-jutility
* 
*/
class JUTFader {
public:
    enum EStatus {
        UNKSTATUS_M1 = -1,
        UNKSTATUS_0 = 0,
    };

    JUTFader(int, int, int, int, JUtility::TColor);
    void control();
    void setStatus(JUTFader::EStatus, int);

    virtual ~JUTFader();
    virtual bool startFadeIn(int);
    virtual bool startFadeOut(int);
    virtual void draw();

    s32 getStatus() const { return mStatus; }
    void setColor(JUtility::TColor color) { mColor.set(color); }

    /* 0x04 */ s32 mStatus;
    /* 0x08 */ u16 field_0x8;
    /* 0x0A */ u16 field_0xa;
    /* 0x0C */ JUtility::TColor mColor;
    /* 0x10 */ JGeometry::TBox2<f32> mBox;
    /* 0x20 */ int mEStatus;
    /* 0x24 */ u32 field_0x24;
};

#endif /* JUTFADER_H */
