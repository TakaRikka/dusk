#ifndef JASCRITICALSECTION_H
#define JASCRITICALSECTION_H

#include <os.h>

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASCriticalSection {
public:
    JASCriticalSection() { mInterruptState = OSDisableInterrupts(); };
    ~JASCriticalSection() { OSRestoreInterrupts(mInterruptState); };

private:
    u32 mInterruptState;
};

#endif /* JASCRITICALSECTION_H */
