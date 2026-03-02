#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAWExtSystem/JAWExtSystem.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/JAWExtSystem/JAWGraphContext.h"
#include "JSystem/JAWExtSystem/JAWWindow.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include <algorithm>

JGadget::TList<JAWWindow*> JAWExtSystem::sPage[128];
JAWExtSystem::TSystemInterface JAWExtSystem::sInterface;
s32 JAWExtSystem::sCurrentPage = ARRAY_SIZE(sPage) - 1;
u8 JAWExtSystem::lbl_80748E44;

BOOL JAWExtSystem::registWindow(u32 param_1, JAWWindow* param_2, int param_3, int param_4) {
    TCurrentHeap heap(sInterface);
    if (param_1 >= ARRAY_SIZE(sPage)) {
        return FALSE;
    }
    param_2->move(param_3, param_4);
    sPage[param_1].push_back(param_2);
    if (sCurrentPage > param_1) {
        sCurrentPage = param_1;
    }
    return TRUE;
}

BOOL JAWExtSystem::destroyWindow(u32 param_1, JAWWindow* param_2) {
    TCurrentHeap heap(sInterface);
    if (param_1 >= ARRAY_SIZE(sPage)) {
        return FALSE;
    }
    JGadget::TList<JAWWindow*>* page = &sPage[param_1];
    JGadget::TList<JAWWindow*>::iterator it = std::find(page->begin(), page->end(), param_2);
    if (it == page->end()) {
        return FALSE;
    }
    page->erase(it);
    return TRUE;
}

void JAWExtSystem::nextPage() {
    int page = sCurrentPage;
    do {
        sCurrentPage++;
        if (sCurrentPage >= ARRAY_SIZE(sPage)) {
            sCurrentPage = 0;
        }
        if (page == sCurrentPage) {
            break;
        }
    } while (sPage[sCurrentPage].empty());
}

void JAWExtSystem::prevPage() {
    int page = sCurrentPage;
    do {
        sCurrentPage--;
        if (sCurrentPage < 0) {
            sCurrentPage = ARRAY_SIZE(sPage) - 1;
        }
        if (page == sCurrentPage) {
            break;
        }
    } while (sPage[sCurrentPage].empty());
}

void JAWExtSystem::nextWindow() {
    JGadget::TList<JAWWindow*>* page = &sPage[sCurrentPage];
    JGadget::TList<JAWWindow*>::iterator begin = page->begin();
    if (begin == page->end()) {
        return;
    }
    JAWWindow* window = *begin;
    page->erase(begin);
    page->push_back(window);
}

void JAWExtSystem::prevWindow() {
    JGadget::TList<JAWWindow*>* page = &sPage[sCurrentPage];
    JGadget::TList<JAWWindow*>::iterator end = page->end();
    end--;
    if (end == page->end()) {
        return;
    }
    JAWWindow* window = *end;
    page->erase(end);
    page->push_front(window);
}

void JAWExtSystem::draw() {
    TCurrentHeap heap(sInterface);
    if (!JAWGraphContext::lbl_8074CD30) {
        return;
    }
    J2DOrthoGraph ortho(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f);
    ortho.setOrtho(0.0f, -20.0f, 640.0f, 520.0f, -1.0, 1.0f);
    ortho.setPort();
    JGadget::TList<JAWWindow*>* page = &sPage[sCurrentPage];
    JGadget::TIterator_reverse<JGadget::TList<JAWWindow*>::iterator> it;
    for (it = page->rbegin(); it != page->rend(); it++) {
        JAWWindow** window = &*it;
        (*window)->initIf();
        (*window)->setAlpha(lbl_80748E44);
        (*window)->draw(0, 0, &ortho);
    }
    for (int i = 0; i < ARRAY_SIZE(sPage); i++) {
        JGadget::TList<JAWWindow*>* page = &sPage[i];
        JGadget::TIterator_reverse<JGadget::TList<JAWWindow*>::iterator> it;
        for (it = page->rbegin(); it != page->rend(); it++) {
            JAWWindow** window = &*it;
            (*window)->frameWork();
        }
    }
    GXSetScissor(0, 0, 640, 480);
}

void JAWExtSystem::padProc(const JUTGamePad& pad) {
    TCurrentHeap heap(sInterface);
    if (pad.getAnalogR() && pad.testTrigger(PAD_BUTTON_MENU)) {
        JAWGraphContext::lbl_8074CD30 ^= true;
    }
    if (!JAWGraphContext::lbl_8074CD30) {
        return;
    }
    JGadget::TList<JAWWindow*>* page = &sPage[sCurrentPage];
    JGadget::TList<JAWWindow*>::iterator begin = page->begin();
    if (begin == page->end()) {
        return;
    }
    JAWWindow* window = *begin;
    if (pad.getAnalogR()) {
        f32 f31 = pad.getAnalogL() * 0.1f + 8.0f;
        int r25 = f31 * pad.getMainStickX();
        int r28 = -int(f31 * pad.getMainStickY());
        if (pad.testButton(PAD_BUTTON_X)) {
            window->addSize(r25, r28);
        } else if (pad.testButton(PAD_BUTTON_B)) {
            int r29 = lbl_80748E44;
            r29 += (r28 > 0 ? -2 : r28 < 0 ? 2 : 0) * (pad.getAnalogL() ? 2 : 1);
            if (r29 < 0) {
                r29 = 0;
            }
            if (r29 >= 256) {
                r29 = 255;
            }
            lbl_80748E44 = r29;
        } else {
            window->addPosition(r25, r28);
        }
        if (pad.testTrigger(PAD_BUTTON_RIGHT)) {
            nextPage();
        }
        if (pad.testTrigger(PAD_BUTTON_LEFT)) {
            prevPage();
        }
        if (pad.testTrigger(PAD_BUTTON_UP)) {
            prevWindow();
        }
        if (pad.testTrigger(PAD_BUTTON_DOWN)) {
            nextWindow();
        }
    } else {
        window->padProc(pad);
    }
}
