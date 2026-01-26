#ifndef C_COUNTER_H
#define C_COUNTER_H

#include <dolphin/types.h>

struct counter_class {
    u32 mCounter0;
    s32 mCounter1;
    u32 mTimer;
};

extern counter_class g_Counter;

void cCt_Counter(int resetCounter1);

inline void cCt_execCounter() {
    g_Counter.mTimer++;
}

#endif /* C_COUNTER_H */
