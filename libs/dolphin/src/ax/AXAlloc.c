#include <dolphin/dolphin.h>
#include <dolphin/ax.h>

#include "__ax.h"

static AXVPB* __AXStackHead[AX_PRIORITY_STACKS];
static AXVPB* __AXStackTail[AX_PRIORITY_STACKS];

static AXVPB* __AXCallbackStack;

static u32 __AXCheckStacks(void) {
    u32 i;
    u32 voices;
    AXVPB* voice;

    voices = 0;
    for (i = 0; i < 32; i++) {
        voice = __AXStackHead[i];
        while (voice != 0) {
            voices++;
            if (voices > 64) {
                return 0;
            }

            voice = voice->next;
        }
    }

    return 1;
}

AXVPB* __AXGetStackHead(u32 priority) {
    ASSERTLINE(97, priority < AX_PRIORITY_STACKS);
    return __AXStackHead[priority];
}

void __AXServiceCallbackStack(void) {
    AXVPB* p;

    for (p = __AXPopCallbackStack(); p; p = __AXPopCallbackStack()) {
        if (p->priority != 0) {
            if (p->callback) {
                p->callback(p);
            }
    
            __AXRemoveFromStack(p);
            __AXPushFreeStack(p);
        }
    }
}

void __AXInitVoiceStacks(void) {
    u32 i;

    __AXCallbackStack = NULL;
    for (i = 0; i < AX_PRIORITY_STACKS; i++) {
        __AXStackHead[i] = __AXStackTail[i] = 0;
    }
}

void __AXAllocInit(void) {
#if DEBUG
    OSReport("Initializing AXAlloc code module\n");
#endif
    __AXInitVoiceStacks();
}

void __AXAllocQuit(void) {
#if DEBUG
    OSReport("Shutting down AXAlloc code module\n");
#endif
    __AXInitVoiceStacks();
}

void __AXPushFreeStack(AXVPB* p) {
    ASSERTLINE(168, p->priority);
    p->next = __AXStackHead[0];
    __AXStackHead[0] = p;
    p->priority = 0;
}

AXVPB* __AXPopFreeStack(void) {
    AXVPB* p;

    p = (void*)(u32)&__AXStackHead[0]->next;
    if (p) {
        __AXStackHead[0] = p->next;
    }
    return p;
}

void __AXPushCallbackStack(AXVPB* p) {
    p->next1 = __AXCallbackStack;
    __AXCallbackStack = p;
}

AXVPB* __AXPopCallbackStack(void) {
    AXVPB* p;

    p = (void*)(u32)&__AXCallbackStack[0];
    if (p) {
        __AXCallbackStack = p->next1;
    }
    return p;
}

void __AXRemoveFromStack(AXVPB* p) {
    u32 i;
    AXVPB* head;
    AXVPB* tail;

    ASSERTLINE(219, p->priority);

    i = p->priority;
    head = __AXStackHead[i];
    tail = __AXStackTail[i];
    if (head == tail) {
        __AXStackHead[i] = __AXStackTail[i] = 0;
        return;
    }

    if (p == head) {
        __AXStackHead[i] = p->next;
        __AXStackHead[i]->prev = 0;
        return;
    }

    if (p == tail) {
        __AXStackTail[i] = p->prev;
        __AXStackTail[i]->next = 0;
        return;
    }

    head = p->prev;
    tail = p->next;
    head->next = tail;
    tail->prev = head;
}

void __AXPushStackHead(AXVPB* p, u32 priority) {
    ASSERTLINE(261, priority);
    ASSERTLINE(262, priority < AX_PRIORITY_STACKS);

    p->next = __AXStackHead[priority];
    p->prev = 0;

    if (p->next) {
        __AXStackHead[priority]->prev = p;
        __AXStackHead[priority] = p;
    } else {
        __AXStackTail[priority] = p;
        __AXStackHead[priority] = p;
    }

    p->priority = priority;
}

AXVPB* __AXPopStackFromBottom(u32 priority) {
    AXVPB* p;

    ASSERTLINE(287, priority);
    ASSERTLINE(288, priority < AX_PRIORITY_STACKS);

    p = NULL;
    if (__AXStackHead[priority]) {
        if (__AXStackHead[priority] == __AXStackTail[priority]) {
            p = __AXStackHead[priority];
            __AXStackHead[priority] = __AXStackTail[priority] = 0;
        } else if (__AXStackTail[priority]) {
            p = __AXStackTail[priority];
            __AXStackTail[priority] = p->prev;
            __AXStackTail[priority]->next = 0;
        }
    }

    return p;
}

void AXFreeVoice(AXVPB* p) {
    BOOL old;
    
    ASSERTLINE(322, p);
    ASSERTMSGLINE(326, p->priority != 0, "Calling AXFreeVoice() for voice that is already free\n");

    old = OSDisableInterrupts();
    __AXRemoveFromStack(p);
    if (p->pb.state == 1) {
        p->depop = 1;
    }
    __AXSetPBDefault(p);
    __AXPushFreeStack(p);

    ASSERTMSGLINE(343, __AXCheckStacks() != 0, "Voice list is trashed!\n");
    OSRestoreInterrupts(old);
}

AXVPB* AXAcquireVoice(u32 priority, void (*callback)(void*), u32 userContext) {
    BOOL old;
    AXVPB* p;
    u32 i;

    ASSERTLINE(361, priority);
    ASSERTLINE(362, priority < AX_PRIORITY_STACKS);

    old = OSDisableInterrupts();
    p = __AXPopFreeStack();
    if (p == 0) {
        for (i = 1; i < priority; i++) {
            p = __AXPopStackFromBottom(i);
            if (p) {
                if (p->pb.state == 1) {
                    p->depop = 1;
                }
                if (p->callback != 0) {
                    p->callback(p);
                }
                break;
            }
        }
    }

    if (p) {
        __AXPushStackHead(p, priority);
        p->callback = callback;
        p->userContext = userContext;
        __AXSetPBDefault(p);
    }

    ASSERTMSGLINE(410, __AXCheckStacks() != 0, "Voice list is trashed!\n");
    OSRestoreInterrupts(old);
    return p;
}

void AXSetVoicePriority(AXVPB* p, u32 priority) {
    BOOL old;

    ASSERTLINE(424, p);
    ASSERTMSGLINE(428, p->priority != 0, "Calling AXSetVoicePriority() for voice that is already free\n");
    ASSERTMSGLINE(433, priority, "Do not set voice priority to 0, use AXFreeVoice() to free voice\n");
    ASSERTLINE(435, priority < AX_PRIORITY_STACKS);

    old = OSDisableInterrupts();
    __AXRemoveFromStack(p);
    __AXPushStackHead(p, priority);

    ASSERTMSGLINE(442, __AXCheckStacks() != 0, "Voice list is trashed!\n");
    OSRestoreInterrupts(old);
}
