#ifndef JKRDISPOSER_H
#define JKRDISPOSER_H

#include "JSystem/JSupport/JSUList.h"

class JKRHeap;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRDisposer {
public:
    JKRDisposer();
    virtual ~JKRDisposer();

public:
    /* 0x00 */  // vtable
    /* 0x04 */ JKRHeap* mHeap;
    /* 0x08 */ JSULink<JKRDisposer> mLink;
    /* 0x18 */
};

#endif /* JKRDISPOSER_H */
