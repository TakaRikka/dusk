#ifndef JAISOUNDCHILD_H
#define JAISOUNDCHILD_H

#include "JSystem/JAudio2/JAISoundParams.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"

struct JASTrack;

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundChild : JASPoolAllocObject<JAISoundChild> {
    void init();
    void mixOut(JASTrack*);
    void calc();

    JAISoundChild() { init(); }

    /* 0x00 */ JAISoundParamsMove mMove;
    /* 0x50 */ JASSoundParams mParams;
};  // Size: 0x64

#endif /* JAISOUNDCHILD_H */
