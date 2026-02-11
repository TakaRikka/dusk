#ifndef JKRTHREAD_H
#define JKRTHREAD_H

#include "JSystem/JKernel/JKRHeap.h"
#include <dolphin/os.h>
#include <dolphin/os.h>

/**
 * @ingroup jsystem-jkernel
 * 
 */
struct JKRThreadName_ {
    s32 id;
    char* name;
};

class JUTConsole;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRThread : public JKRDisposer {
public:
    class TLoad {
    public:
        TLoad() {
            clear();
            mValid = false;
            mThreadId = 0;
        }

        bool isValid() const { return mValid; }
        u32 getCost() const { return mCost; }
        u32 getCount() const { return mSwitchCount; }
        s32 getId() const { return mThreadId; }

        void setValid(bool valid) { mValid = valid; }
        void setId(s32 id) { mThreadId = id; }
        void setCurrentTime() { mLastTick = OSGetTick(); }

        void resetCost() { mCost = 0; }
        void resetCount() { mSwitchCount = 0; }

        void incCount() { mSwitchCount++; }
        void addCurrentCost() { mCost = mCost + (OSGetTick() - mLastTick); }

        void clear() {
            resetCount();
            resetCost();
            mLastTick = 0;
        }

    private:
        /* 0x00 */ bool mValid;
        /* 0x01 */ u8 padding_0x61[3];
        /* 0x04 */ u32 mCost;
        /* 0x08 */ u32 mSwitchCount;
        /* 0x0C */ OSTick mLastTick;
        /* 0x10 */ s32 mThreadId;
    };

    JKRThread(u32 stack_size, int message_count, int param_3);
    JKRThread(JKRHeap* heap, u32 stack_size, int message_count, int param_4);
    JKRThread(OSThread* thread, int message_count);
    virtual ~JKRThread();

    /* vt[03] */ virtual void* run() { return NULL; }

    void setCommon_mesgQueue(JKRHeap* heap, int message_count);
    void setCommon_heapSpecified(JKRHeap* heap, u32 stack_size, int param_3);

    OSThread* getThreadRecord() const { return mThreadRecord; }
    void* getStack() const { return mStackMemory; }
    TLoad* getLoadInfo() { return &mLoadInfo; }
    JKRHeap* getCurrentHeap() const { return mCurrentHeap; }
    s32 getCurrentHeapError() const { return mCurrentHeapError; }

    void setCurrentHeap(JKRHeap* heap) {
        mCurrentHeap = heap ? heap : JKRHeap::getCurrentHeap();
    }

    void resume() { OSResumeThread(mThreadRecord); }
    BOOL sendMessage(OSMessage message) {
        return OSSendMessage(&mMessageQueue, message, OS_MESSAGE_NOBLOCK);
    }
    void sendMessageBlock(OSMessage message) {
        OSSendMessage(&mMessageQueue, message, OS_MESSAGE_BLOCK);
    }
    OSMessage waitMessage() {
        OSMessage message;
        OSReceiveMessage(&mMessageQueue, &message, OS_MESSAGE_NOBLOCK);
        return message;
    }

    OSMessage waitMessage(BOOL* received) {
        OSMessage message;
        BOOL rv = OSReceiveMessage(&mMessageQueue, &message, OS_MESSAGE_NOBLOCK);
        if (received) {
            *received = rv;
        }
        return message;
    }
    OSMessage waitMessageBlock() {
        OSMessage message;
        OSReceiveMessage(&mMessageQueue, &message, OS_MESSAGE_BLOCK);
        return message;
    }
    void jamMessageBlock(OSMessage message) {
        OSJamMessage(&mMessageQueue, message, OS_MESSAGE_BLOCK);
    }

private:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRDisposer
    /* 0x18 */ JSULink<JKRThread> mThreadListLink;
    /* 0x28 */ JKRHeap* mHeap;
    /* 0x2C */ OSThread* mThreadRecord;
    /* 0x30 */ OSMessageQueue mMessageQueue;
    /* 0x50 */ OSMessage* mMesgBuffer;
    /* 0x54 */ s32 mMessageCount;
    /* 0x58 */ void* mStackMemory;
    /* 0x5C */ u32 mStackSize;
    /* 0x60 */ TLoad mLoadInfo;
    /* 0x74 */ JKRHeap* mCurrentHeap;
    /* 0x78 */ s32 mCurrentHeapError;

public:
    static void* start(void* param_1);
    static JKRThread* searchThread(OSThread* thread);
    static JSUList<JKRThread>& getList() { return (JSUList<JKRThread>&)sThreadList; }

    static JSUList<JKRThread> sThreadList;
    // static u8 sThreadList[12];
};

class JKRIdleThread : public JKRThread {
public:
    virtual void destroy() {}
    virtual ~JKRIdleThread() { sThread = NULL; }
    virtual void* run() { while (true); }

    static void* sThread;
};

typedef void (*JKRThreadSwitch_PreCallback)(OSThread* current, OSThread* next);
typedef void (*JKRThreadSwitch_PostCallback)(OSThread* current, OSThread* next);

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRThreadSwitch {
public:
    JKRThreadSwitch(JKRHeap*);
    virtual void draw(JKRThreadName_* param_1, JUTConsole* param_2);
    virtual void draw(JKRThreadName_* thread_name_list) { draw(thread_name_list, NULL); }
    virtual ~JKRThreadSwitch() {}

    static JKRThreadSwitch* createManager(JKRHeap* heap);

    JKRThread* enter(JKRThread* param_1, int param_2);
    static void callback(OSThread* param_1, OSThread* param_2);

    static JKRThreadSwitch* getManager() { return sManager; }
    static u32 getTotalCount() { return sTotalCount; }

private:
    static JKRThreadSwitch* sManager;
    static u32 sTotalCount;
    static u64 sTotalStart;
    static JKRThreadSwitch_PreCallback mUserPreCallback;
    static JKRThreadSwitch_PostCallback mUserPostCallback;

private:
    /* 0x00 */  // vtable
    /* 0x04 */ JKRHeap* mHeap;
    /* 0x08 */ bool mSetNextHeap;
    /* 0x09 */ u8 field_0x9[3];
    /* 0x0C */ u32 field_0xC;
    /* 0x10 */ u32 field_0x10;
    /* 0x14 */ u8 field_0x14[4];
    /* 0x18 */ s64 field_0x18;
    /* 0x20 */ u32 field_0x20;
    /* 0x24 */ u32 field_0x24;
};

class JKRTask : public JKRThread {
public:
    virtual ~JKRTask();
    virtual void* run();

    int check();

    static JSUList<JKRTask> sTaskList;
    static u8 sEndMesgQueue[32];

    /* 0x7C */ JSULink<JKRTask> mTaskLink;
    /* 0x8C */ u8 field_0x8c[0x94 - 0x8C];
    /* 0x94 */ OSMessageQueue* field_0x94;
};

#endif /* JKRTHREAD_H */
