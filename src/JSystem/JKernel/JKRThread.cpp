#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTConsole.h"
#include <cstdio>
#include "global.h"
#include <stdint.h>

JSUList<JKRThread> JKRThread::sThreadList(0);

void* JKRIdleThread::sThread;

JKRThreadSwitch* JKRThreadSwitch::sManager;

u32 JKRThreadSwitch::sTotalCount;

u64 JKRThreadSwitch::sTotalStart;

JKRThreadSwitch_PreCallback JKRThreadSwitch::mUserPreCallback;

JKRThreadSwitch_PostCallback JKRThreadSwitch::mUserPostCallback;

JKRThread::JKRThread(u32 stack_size, int message_count, int param_3) : mThreadListLink(this) {
    JKRHeap* heap = JKRHeap::findFromRoot(this);
    if (heap == NULL) {
        heap = JKRGetSystemHeap();
    }

    setCommon_heapSpecified(heap, stack_size, param_3);
    setCommon_mesgQueue(mHeap, message_count);
}

JKRThread::JKRThread(JKRHeap* heap, u32 stack_size, int message_count, int param_4)
    : mThreadListLink(this) {
    if (heap == NULL) {
        heap = JKRGetCurrentHeap();
    }

    setCommon_heapSpecified(heap, stack_size, param_4);
    setCommon_mesgQueue(mHeap, message_count);
}

JKRThread::JKRThread(OSThread* thread, int message_count) : mThreadListLink(this) {
    mHeap = NULL;
    mThreadRecord = thread;
    mStackSize = (uintptr_t)thread->stackEnd - (uintptr_t)thread->stackBase;
    mStackMemory = thread->stackBase;

    setCommon_mesgQueue(JKRGetSystemHeap(), message_count);
}

JKRThread::~JKRThread() {
    sThreadList.remove(&mThreadListLink);

    if (mHeap) {
        if (OSIsThreadTerminated(mThreadRecord) == FALSE) {
            OSDetachThread(mThreadRecord);
            OSCancelThread(mThreadRecord);
        }
        JKRFreeToHeap(mHeap, mStackMemory);
        JKRFreeToHeap(mHeap, mThreadRecord);
    }
    JKRFree(mMesgBuffer);
}

void JKRThread::setCommon_mesgQueue(JKRHeap* heap, int message_count) {
    mMessageCount = message_count;
    mMesgBuffer = (OSMessage*)JKRAllocFromHeap(heap, mMessageCount * sizeof(OSMessage), 0);
    JUT_ASSERT(130, mMesgBuffer);

    OSInitMessageQueue(&mMessageQueue, mMesgBuffer, mMessageCount);
    sThreadList.append(&mThreadListLink);

    mCurrentHeap = NULL;
    mCurrentHeapError = NULL;
}

void JKRThread::setCommon_heapSpecified(JKRHeap* heap, u32 stack_size, int param_3) {
    mHeap = heap;
    mStackSize = stack_size & 0xffffffe0;
    mStackMemory = JKRAllocFromHeap(mHeap, mStackSize, 0x20);
    JUT_ASSERT(164, mStackMemory);
    mThreadRecord = (OSThread*)JKRAllocFromHeap(mHeap, sizeof(OSThread), 0x20);
    JUT_ASSERT(168, mThreadRecord);

    OSCreateThread(mThreadRecord, start, this, (u8*)mStackMemory + mStackSize, mStackSize, param_3, 1);
}

void* JKRThread::start(void* thread) {
    return ((JKRThread*)thread)->run();
}

JKRThread* JKRThread::searchThread(OSThread* thread) {
    for (JSUListIterator<JKRThread> iterator = getList().getFirst(); iterator != getList().getEnd(); ++iterator) {
        if (iterator->getThreadRecord() == thread) {
            return iterator.getObject();
        }
    }

    return NULL;
}

static void dummy1(JKRThread* thread, JKRThread::TLoad* load) {
    load->getId();
    load->isValid();
    thread->getLoadInfo();
}

JKRThreadSwitch::JKRThreadSwitch(JKRHeap* param_0) {
    mHeap = param_0;
    OSSetSwitchThreadCallback(JKRThreadSwitch::callback);
    this->field_0xC = 0;
    this->field_0x10 = 1;
    this->field_0x18 = 0;
    sTotalCount = 0;
    sTotalStart = 0;
    this->field_0x20 = 0;
    this->field_0x24 = 0;
    mSetNextHeap = true;
}

JKRThreadSwitch* JKRThreadSwitch::createManager(JKRHeap* heap) {
    JUT_ASSERT(343, sManager == NULL);

    if (!heap) {
        heap = JKRGetCurrentHeap();
    }

    sManager = new (heap, 0) JKRThreadSwitch(heap);
    return sManager;
}

static void dummy2(JKRThread::TLoad* load) {
    load->setValid(false);
    load->setId(0);
}

JKRThread* JKRThreadSwitch::enter(JKRThread* thread, int thread_id) {
    if (!thread) {
        return NULL;
    }

    JKRThread* found_thread = JKRThread::searchThread(thread->getThreadRecord());
    if (found_thread) {
        thread = found_thread;
    }

    JKRThread::TLoad* loadInfo = thread->getLoadInfo();
    loadInfo->clear();
    loadInfo->setValid(true);
    loadInfo->setId(thread_id);
    return thread;
}

static void dummyStrings() {
    DEAD_STRING("on");
    DEAD_STRING("off");
    DEAD_STRING("JKRThread:%x  OSThread:%x  Load:ID:%d  (%s)\n");
}

void JKRThreadSwitch::callback(OSThread* current, OSThread* next) {
    if (mUserPreCallback) {
        (*mUserPreCallback)(current, next);
    }

    sTotalCount = sTotalCount + 1;

    JKRHeap* next_heap = NULL;
    for (JSUListIterator<JKRThread> iterator = JKRThread::getList().getFirst(); iterator != JKRThread::getList().getEnd(); ++iterator) {
        JKRThread* thread = iterator.getObject();

        if (thread->getThreadRecord() == current) {
            thread->setCurrentHeap(JKRHeap::getCurrentHeap());
            if (thread->getLoadInfo()->isValid()) {
                thread->getLoadInfo()->addCurrentCost();
            }
        }

        if (thread->getThreadRecord() == next) {
            if (thread->getLoadInfo()->isValid()) {
                thread->getLoadInfo()->setCurrentTime();
                thread->getLoadInfo()->incCount();
            }

            if (sManager->mSetNextHeap) {
                next_heap = thread->getCurrentHeap();
                if (!next_heap) {
                    next_heap = JKRHeap::getCurrentHeap();
                } else if (JKRHeap::getRootHeap()->isSubHeap(next_heap)) {
                    continue;
                #if PLATFORM_WII || PLATFORM_SHIELD
                } else if (JKRHeap::getRootHeap2()->isSubHeap(next_heap)) {
                    continue;
                #endif
                } else {
                    switch (thread->getCurrentHeapError()) {
                    case 0:
                        JUT_PANIC(508, "JKRThreadSwitch: currentHeap destroyed.");
                        break;
                    case 1:
                        JUTWarningConsole("JKRThreadSwitch: currentHeap destroyed.\n");
                        next_heap = JKRHeap::getCurrentHeap();
                        break;
                    case 2:
                        next_heap = JKRHeap::getCurrentHeap();
                        break;
                    case 3:
                        next_heap = JKRHeap::getSystemHeap();
                        break;
                    }
                }
            }
        }
    }

    if (next_heap) {
        next_heap->becomeCurrentHeap();
    }

    if (mUserPostCallback) {
        (*mUserPostCallback)(current, next);
    }
}

void dummy3(JKRThreadSwitch* threadSw, JKRThreadName_* name) {
    threadSw->draw(name);
}

void JKRThreadSwitch::draw(JKRThreadName_* thread_name_list, JUTConsole* console) {
    const char* print_0 = " total: switch:%3d  time:%d(%df)\n";
    const char* print_1 = " -------------------------------------\n";

    if (console) {
        console->clear();
        console->print_f(print_0, getTotalCount(), (int)this->field_0x18, this->field_0x10);
        console->print(print_1);
    } else {
        OS_REPORT(print_0, getTotalCount(), (int)this->field_0x18, this->field_0x10);
        OS_REPORT(print_1);
    }

    for (JSUListIterator<JKRThread> iterator = JKRThread::getList().getFirst(); iterator != JKRThread::getList().getEnd(); ++iterator) {
        JKRThread* thread = iterator.getObject();
        JKRThread::TLoad* loadInfo = thread->getLoadInfo();

        if (loadInfo->isValid()) {
            char* thread_print_name = NULL;
            if (thread_name_list) {
                JKRThreadName_* thread_name = thread_name_list;
                for (; thread_name->name; thread_name++) {
                    if (thread_name->id == loadInfo->getId()) {
                        thread_print_name = thread_name->name;
                        break;
                    }
                }
            }

            if (!thread_print_name) {
                char buffer[16];
                sprintf(buffer, "%d", loadInfo->getId());
                thread_print_name = buffer;
            }

            u32 switch_count = loadInfo->getCount();
            u32 cost = loadInfo->getCost();
            f32 cost_per_0x18 = loadInfo->getCost() / f32(this->field_0x18);

            u32 cost_int = (u32)(cost_per_0x18 * 100.0f);
            u32 cost_float = (u32)(cost_per_0x18 * 1000.0f) % 10;
            if (console) {
                console->print_f(" [%10s] switch:%5d  cost:%2d.%d%%\n", thread_print_name,
                                 switch_count, cost_int, cost_float);
            } else {
                OS_REPORT(" [%10s] switch:%5d  cost:%2d.%d%%\n", thread_print_name, switch_count,
                         cost_int, cost_float);

            }
        }
    }
}

static void dummy4(JKRTask* thread, JSULink<JKRTask>* link) {
    thread->getStack();
    delete link;
}

JKRTask::~JKRTask() {
    sTaskList.remove(&mTaskLink);
}

void* JKRTask::run() {
    struct TaskMessage {
        void (*field_0x0)(int);
        int field_0x4;
        void* field_0x8;
    };
    OSInitFastCast();
    while (true) {
        TaskMessage* msg = (TaskMessage*)waitMessageBlock();
        if (msg->field_0x0) {
            msg->field_0x0(msg->field_0x4);
            check();
            if (field_0x94) {
                OSSendMessage(field_0x94, msg->field_0x8, OS_MESSAGE_NOBLOCK);
            }
        }
        msg->field_0x0 = NULL;
    }
}

int JKRTask::check() {
    int result = 0;
    u8* ptr = (u8*)JKRThread::getStack();
    JUT_ASSERT(1033, *((u32*)ptr) == 0xDEADBABE);
    ptr += 4;
    result += 4;
    while (*ptr++ == 0xDE) { result++; }
    return result;
}

#if !PLATFORM_GCN
static void dummy(JKRIdleThread* thread) {
    thread->run();
    delete thread;
    thread->destroy();
}
#endif

#pragma push
#pragma force_active on
JSUList<JKRTask> JKRTask::sTaskList;
#pragma pop

#pragma push
#pragma force_active on
u8 JKRTask::sEndMesgQueue[32];
#pragma pop
