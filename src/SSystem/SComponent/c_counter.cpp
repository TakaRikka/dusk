/**
 * c_counter.cpp
 *
 */

#include "SSystem/SComponent/c_counter.h"

counter_class g_Counter;

void cCt_Counter(int resetCounter1) {
    if (resetCounter1 == 1) {
        g_Counter.mCounter1 = 0;
    } else {
        g_Counter.mCounter1++;
    }

    g_Counter.mCounter0++;
}
