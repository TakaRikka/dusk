#ifndef M_DO_M_DO_DVD_THREAD_H
#define M_DO_M_DO_DVD_THREAD_H

#include "JSystem/JKernel/JKRArchive.h"
#include <dolphin/os.h>
#include <dolphin/os.h>
#include "f_pc/f_pc_node.h"

#define mDoDvd_MOUNT_DIRECTION_HEAD 0
#define mDoDvd_MOUNT_DIRECTION_TAIL 1

class JKRHeap;
class JKRMemArchive;

typedef void* (*mDoDvdThd_callback_func)(void*);

class mDoDvdThd_command_c : public node_class {
public:
    /* 0x0C */ bool mIsDone;
    /* 0x10  vtable*/
public:
    virtual ~mDoDvdThd_command_c();
    mDoDvdThd_command_c();
    inline s32 sync() { return mIsDone; }
    inline void destroy() { delete this; }
    virtual s32 execute() = 0;
};  // Size = 0x14

class mDoDvdThd_toMainRam_c : public mDoDvdThd_command_c {
public:
    mDoDvdThd_toMainRam_c(u8);
    static mDoDvdThd_toMainRam_c* create(char const*, u8, JKRHeap*);
    virtual ~mDoDvdThd_toMainRam_c();
    virtual s32 execute();

    void* getMemAddress() const { return mData; }

private:
    /* 0x14 */ u8 mMountDirection;
    /* 0x18 */ s32 mEntryNum;
    /* 0x1C */ void* mData;
    /* 0x20 */ s32 mDataSize;
    /* 0x24 */ JKRHeap* mHeap;
};  // Size = 0x28

class mDoDvdThd_param_c {
public:
    mDoDvdThd_param_c();
    void kick();
    s32 waitForKick();
    mDoDvdThd_command_c* getFirstCommand();
    void addition(mDoDvdThd_command_c*);
    void cut(mDoDvdThd_command_c*);
    void mainLoop();

private:
    /* 0x00 */ OSMessageQueue mMessageQueue;
    /* 0x20 */ void* mMessageQueueMessages;
    /* 0x24 */ node_list_class mNodeList;
    /* 0x30 */ OSMutex mMutext;
};  // Size = 0x48

class mDoDvdThd_mountXArchive_c : public mDoDvdThd_command_c {
public:
    virtual ~mDoDvdThd_mountXArchive_c();
    mDoDvdThd_mountXArchive_c(u8 mountDirection, JKRArchive::EMountMode mountMode);
    static mDoDvdThd_mountXArchive_c* create(char const*, u8, JKRArchive::EMountMode,
                                                            JKRHeap*);
    virtual s32 execute();

    JKRArchive* getArchive() const { return mArchive; }

private:
    /* 0x14 */ u8 mMountDirection;
    /* 0x18 */ s32 mEntryNum;
    /* 0x1C */ JKRArchive* mArchive;
    /* 0x20 */ JKRArchive::EMountMode mMountMode;
    /* 0x24 */ JKRHeap* mHeap;
};  // Size = 0x28

class mDoDvdThd_mountArchive_c : public mDoDvdThd_command_c {
public:
    virtual ~mDoDvdThd_mountArchive_c();
    mDoDvdThd_mountArchive_c(u8);
    static mDoDvdThd_mountArchive_c* create(char const*, u8, JKRHeap*);
    virtual s32 execute();

    JKRMemArchive* getArchive() const { return mArchive; }
    JKRHeap* getHeap() const { return mHeap; }

private:
    /* 0x14 */ u8 mMountDirection;
    /* 0x18 */ s32 mEntryNumber;
    /* 0x1C */ JKRMemArchive* mArchive;
    /* 0x20 */ JKRHeap* mHeap;
};  // Size = 0x24

class mDoDvdThd_callback_c : public mDoDvdThd_command_c {
public:
    virtual ~mDoDvdThd_callback_c();
    mDoDvdThd_callback_c(mDoDvdThd_callback_func, void*);
    static mDoDvdThd_callback_c* create(mDoDvdThd_callback_func, void*);
    virtual s32 execute();

private:
    /* 0x14 */ mDoDvdThd_callback_func mFunction;
    /* 0x18 */ void* mData;
    /* 0x1C */ void* mResult;
};

struct mDoDvdThdStack {
    u8 stack[4096];
} ATTRIBUTE_ALIGN(16);

struct mDoDvdThd {
    static s32 main(void*);
    static void create(s32);
    static void suspend();

    static OSThread l_thread;
    static mDoDvdThdStack l_threadStack;
    static mDoDvdThd_param_c l_param;

    static u8 verbose;
    static u8 DVDLogoMode;
    static bool SyncWidthSound;
    static u8 Report_DVDRead;
};

#endif /* M_DO_M_DO_DVD_THREAD_H */
