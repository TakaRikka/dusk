#ifndef JAWSYSTEM_H
#define JAWSYSTEM_H

#include "JSystem/JKernel/JKRHeap.h"

class JAWWindow;

class JAWSystemInterface {
public:
    JAWSystemInterface();
    virtual BOOL registWindow(u32, JAWWindow*, int, int) = 0;
    virtual BOOL destroyWindow(u32, JAWWindow*) = 0;
    virtual void setForegroundWindow(JAWWindow*) = 0;

    /* 0x04 */ JKRHeap* mHeap;

    JKRHeap* getCurrentHeap() const;

    static JAWSystemInterface* sInstance;
};

#endif /* JAWSYSTEM_H */
