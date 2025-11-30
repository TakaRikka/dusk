#ifndef JASDVDTHREAD_H
#define JASDVDTHREAD_H

#include "dolphin/types.h"

class JASTaskThread;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASDvd {
public:
    static JASTaskThread* getThreadPointer();
    static bool createThread(s32 priority, int msgCount, u32 stackSize);

    static JASTaskThread* sThread;
};

#endif /* JASDVDTHREAD_H */
