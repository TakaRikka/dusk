#ifndef JAWEXTSYSTEM_H
#define JAWEXTSYSTEM_H

#include "JSystem/JAWExtSystem/JAWSystem.h"
#include "JSystem/JGadget/std-list.h"

class JAWWindow;
class JUTGamePad;

namespace JAWExtSystem {
    BOOL registWindow(u32, JAWWindow*, int, int);
    BOOL destroyWindow(u32, JAWWindow*);
    void nextPage();
    void prevPage();
    void nextWindow();
    void prevWindow();
    void draw();
    void padProc(const JUTGamePad&);

    class TSystemInterface : public JAWSystemInterface {
    public:
        TSystemInterface() {}
        virtual BOOL registWindow(u32 param_1, JAWWindow* param_2, int param_3, int param_4) {
            return JAWExtSystem::registWindow(param_1, param_2, param_3, param_4);
        }
        virtual BOOL destroyWindow(u32 param_1, JAWWindow* param_2) {
            return JAWExtSystem::destroyWindow(param_1, param_2);
        }
        virtual void setForegroundWindow(JAWWindow*) {}
    };

    class TCurrentHeap {
    public:
        TCurrentHeap(const TSystemInterface& interface) {
            heap = JKRGetCurrentHeap();
            JKRSetCurrentHeap(interface.getCurrentHeap());
        }
        ~TCurrentHeap() { JKRSetCurrentHeap(heap); }

        /* 0x00 */ JKRHeap* heap;
    };

    extern JGadget::TList<JAWWindow*> sPage[128];
    extern TSystemInterface sInterface;
    extern s32 sCurrentPage;
    extern u8 lbl_80748E44;
}

#endif  /* JAWEXTSYSTEM_H */
