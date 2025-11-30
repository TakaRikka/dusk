#ifndef JUTRESOURCE_H
#define JUTRESOURCE_H

#include "dolphin/types.h"

class JKRArchive;
class JSUInputStream;

/**
* @ingroup jsystem-jutility
* 
*/
class JUTResReference {
private:
    /* 0x001 */ u8 mType;
    /* 0x002 */ u8 mNameLength;
    /* 0x003 */ char mName[0x100];

public:
    enum ResType {
        RESTYPE_Null = 0,
        RESTYPE_Unk1 = 1,
        RESTYPE_Unk2 = 2,
        RESTYPE_Unk3 = 3,
        RESTYPE_Unk4 = 4,
    };
    JUTResReference() { mType = 0; }
    void* getResource(JSUInputStream*, u32, JKRArchive*);
    void* getResource(void const*, u32, JKRArchive*);
    void* getResource(u32, JKRArchive*);
};
// only rough size known due to getPointer__7J2DPaneFP20JSURandomInputStreamUlP10JKRArchive

#endif /* JUTRESOURCE_H */
