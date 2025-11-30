#ifndef JASTASKTHREAD_H
#define JASTASKTHREAD_H

#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"

typedef void (*JASThreadCallback)(void*);

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASTaskThread : public JKRThread {
private:
    struct JASThreadCallStack {
        /* 0x00 */ JASThreadCallback callback_;
        /* 0x04 */ u32 msgType_;
        /* 0x08 */ union {
            u8 buffer[0x400];
            void* bufferPtr;
        } msg;
    };
public:
    typedef JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable> ThreadMemPool;

    JASTaskThread(int priority, int msgCount, u32 stackSize);
    void* allocCallStack(JASThreadCallback callback, void* msg);
    void* allocCallStack(JASThreadCallback callback, const void* msg, u32 msgSize);
    int sendCmdMsg(JASThreadCallback callback, void* msg);
    int sendCmdMsg(JASThreadCallback callback, const void* msg, u32 msgSize);
    void pause(bool);

    virtual ~JASTaskThread();
    virtual void* run();

    /* 0x7C */ OSThreadQueue threadQueue_;
    /* 0x84 */ bool field_0x84;
};

#endif /* JASTASKTHREAD_H */
