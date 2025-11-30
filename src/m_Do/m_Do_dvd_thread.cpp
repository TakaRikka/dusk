/**
 * m_Do_dvd_thread.cpp
 * DVD Thread Manager
*/

#include "m_Do/m_Do_dvd_thread.h"
#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_ext.h"

s32 mDoDvdThd::main(void* param_0) {
    { JKRThread jkrThread(OSGetCurrentThread(), 0); }
    mDoExt_getAssertHeap()->becomeCurrentHeap();
    static_cast<mDoDvdThd_param_c*>(param_0)->mainLoop();
    return 0;
}

OSThread mDoDvdThd::l_thread;

mDoDvdThdStack mDoDvdThd::l_threadStack;

mDoDvdThd_param_c mDoDvdThd::l_param;

#if DEBUG
u8 mDoDvdThd::verbose;
u8 mDoDvdThd::DVDLogoMode;
bool mDoDvdThd::SyncWidthSound;
u8 mDoDvdThd::Report_DVDRead;
#else
bool mDoDvdThd::SyncWidthSound;
#endif

u8 sDefaultDirection;

#pragma push
#pragma force_active on
static u8 padding[0x18];
#pragma pop

void mDoDvdThd::create(s32 param_0) {
    OSCreateThread(&l_thread, (void*(*)(void*))mDoDvdThd::main, &l_param,
                   l_threadStack.stack + sizeof(l_threadStack), sizeof(l_threadStack), param_0, 1);
    OSResumeThread(&l_thread);
}

void mDoDvdThd::suspend() {
    OSSuspendThread(&l_thread);
}

static s32 my_DVDConvertPathToEntrynum(char const* param_0) {
    return DVDConvertPathToEntrynum(param_0);
}

mDoDvdThd_command_c::~mDoDvdThd_command_c() {}

mDoDvdThd_param_c::mDoDvdThd_param_c() {
    OSInitMessageQueue(&mMessageQueue, &mMessageQueueMessages, 1);
    OSInitMutex(&mMutext);
    cLs_Create(&mNodeList);
}

void mDoDvdThd_param_c::kick() {
    OSSendMessage(&mMessageQueue, NULL, OS_MESSAGE_NOBLOCK);
}

s32 mDoDvdThd_param_c::waitForKick() {
    return OSReceiveMessage(&mMessageQueue, NULL, OS_MESSAGE_BLOCK);
}

mDoDvdThd_command_c* mDoDvdThd_param_c::getFirstCommand() {
    return (mDoDvdThd_command_c*)mNodeList.mpHead;
}

void mDoDvdThd_param_c::addition(mDoDvdThd_command_c* pCommand) {
    OSLockMutex(&mMutext);
    cLs_Addition(&mNodeList, pCommand);
    OSUnlockMutex(&mMutext);
    this->kick();
}

void mDoDvdThd_param_c::cut(mDoDvdThd_command_c* param_0) {
    OSLockMutex(&mMutext);
    cLs_SingleCut(param_0);
    OSUnlockMutex(&mMutext);
    this->kick();
}

static void cb(void* param_0) {
    mDoDvdThd_command_c** pCmd = (mDoDvdThd_command_c**)param_0;
    s32 result = (*pCmd)->execute();
    if (result != 1) {
        OSReport_Error("mDoDvdThd_param_c::mainLoop() コマンドの実行が失敗しました。\n");
    }
}

void mDoDvdThd_param_c::mainLoop() {
    while (this->waitForKick() != 0) {
        while (mDoDvdThd_command_c* command = this->getFirstCommand()) {
            this->cut(command);
            if (mDoDvdThd::SyncWidthSound) {
                JASDvd::getThreadPointer()->sendCmdMsg(cb, &command, 4);
            } else {
                cb(&command);
            }
        }
    }
}

mDoDvdThd_command_c::mDoDvdThd_command_c() {
    mIsDone = false;
    cNd_ForcedClear(this);
}

mDoDvdThd_callback_c::~mDoDvdThd_callback_c() {}

mDoDvdThd_callback_c::mDoDvdThd_callback_c(mDoDvdThd_callback_func pFunc, void* pData) {
    mFunction = pFunc;
    mData = pData;
    mResult = NULL;
}

mDoDvdThd_callback_c* mDoDvdThd_callback_c::create(mDoDvdThd_callback_func pFunc, void* pData) {
    mDoDvdThd_callback_c* callCmd =
        new (mDoExt_getCommandHeap(), -4) mDoDvdThd_callback_c(pFunc, pData);
    if (callCmd != NULL) {
        mDoDvdThd::l_param.addition(callCmd);
    }
    return callCmd;
}

s32 mDoDvdThd_callback_c::execute() {
    mResult = mFunction(mData);
    mIsDone = true;
    return mResult != NULL;
}

mDoDvdThd_mountArchive_c::~mDoDvdThd_mountArchive_c() {}

mDoDvdThd_mountArchive_c::mDoDvdThd_mountArchive_c(u8 param_0) {
    mMountDirection = param_0;
    mEntryNumber = -1;
    mArchive = NULL;
    mHeap = NULL;
    if (param_0 == 0) {
        mMountDirection = sDefaultDirection;
    }
}

mDoDvdThd_mountArchive_c* mDoDvdThd_mountArchive_c::create(char const* pArchivePath,
                                                           u8 mountDirection, JKRHeap* pHeap) {
    mDoDvdThd_mountArchive_c* mountArcCmd =
        new (mDoExt_getCommandHeap(), -4) mDoDvdThd_mountArchive_c(mountDirection);
    if (mountArcCmd != NULL) {
        mountArcCmd->mEntryNumber = my_DVDConvertPathToEntrynum(pArchivePath);
        if (mountArcCmd->mEntryNumber == -1) {
            mountArcCmd->mIsDone = true;
            delete mountArcCmd;
            mountArcCmd = NULL;
        } else {
            mountArcCmd->mHeap = pHeap;
            mDoDvdThd::l_param.addition(mountArcCmd);
        }
    }
    return mountArcCmd;
}

s32 mDoDvdThd_mountArchive_c::execute() {
    bool isZeldaHeap = false;
    bool isGameHeap = false;
    bool isArcHeap = false;
    bool isJ2DHeap = false;
    JKRHeap* heap = mHeap != NULL ? mHeap : mDoExt_getArchiveHeap();
    JKRMemArchive* memArchive;
    while (true) {
        if (heap == mDoExt_getArchiveHeapPtr()) {
            isArcHeap = true;
        } else if (heap == mDoExt_getZeldaHeap()) {
            isZeldaHeap = true;
        } else if (heap == mDoExt_getGameHeap()) {
            isGameHeap = true;
        } else if (heap == mDoExt_getJ2dHeap()) {
            isJ2DHeap = true;
        }
        if (mMountDirection == 0) {
            memArchive =
                new (heap, 0) JKRMemArchive(mEntryNumber, JKRArchive::MOUNT_DIRECTION_HEAD);
        } else {
            memArchive =
                new (heap, -4) JKRMemArchive(mEntryNumber, JKRArchive::MOUNT_DIRECTION_TAIL);
        }
        if (memArchive != NULL && memArchive->isMounted()) {
            mArchive = memArchive;
            break;
        }
        OSReport_Error("mDoDvdThd_mountArchive_c::execute マウント失敗\n");
        if (memArchive) {
            delete memArchive;
        }
        if (!isZeldaHeap) {
            OSReport_Error("mDoDvdThd_mountArchive_c::execute ゼルダヒープで再チャレンジ！\n");
            heap = mDoExt_getZeldaHeap();
        } else if (!isArcHeap) {
            OSReport_Error("mDoDvdThd_mountArchive_c::execute アーカイブヒープで再チャレンジ！\n");
            heap = mDoExt_getArchiveHeapPtr();
        } else if (!isGameHeap) {
            OSReport_Error("mDoDvdThd_mountArchive_c::execute ゲームヒープで再チャレンジ！\n");
            heap = mDoExt_getGameHeap();
        } else if (!isJ2DHeap) {
            OSReport_Error("mDoDvdThd_mountArchive_c::execute J2Dヒープで再チャレンジ！\n");
            heap = mDoExt_getJ2dHeap();
        } else {
            OSReport_FatalError("mDoDvdThd_mountArchive_c::execute ヒープが致命的に足りません！\n");
            break;
        }
    }
    mIsDone = true;
    return mArchive != NULL;
}

mDoDvdThd_mountXArchive_c::~mDoDvdThd_mountXArchive_c() {}

mDoDvdThd_mountXArchive_c::mDoDvdThd_mountXArchive_c(u8 mountDirection, JKRArchive::EMountMode mountMode) {
    mMountDirection = mountDirection;
    mEntryNum = -1;
    mArchive = NULL;
    mMountMode = mountMode;
    mHeap = NULL;
    if (mountDirection == 0) {
        mMountDirection = sDefaultDirection;
    }
}

mDoDvdThd_mountXArchive_c* mDoDvdThd_mountXArchive_c::create(char const* pArchivePath,
                                                             u8 mountDirection,
                                                             JKRArchive::EMountMode mountMode,
                                                             JKRHeap* pHeap) {
    mDoDvdThd_mountXArchive_c* mountXArcCmd =
        new (mDoExt_getCommandHeap(), -4) mDoDvdThd_mountXArchive_c(mountDirection, mountMode);
    if (mountXArcCmd != NULL) {
        mountXArcCmd->mEntryNum = my_DVDConvertPathToEntrynum(pArchivePath);
        if (mountXArcCmd->mEntryNum == -1) {
            mountXArcCmd->mIsDone = true;
            delete mountXArcCmd;
            mountXArcCmd = NULL;
        } else {
            mountXArcCmd->mHeap = pHeap;
            mDoDvdThd::l_param.addition(mountXArcCmd);
        }
    }
    return mountXArcCmd;
}

s32 mDoDvdThd_mountXArchive_c::execute() {
    JKRArchive::EMountDirection mountDirection =
        mMountDirection == 0 ? JKRArchive::MOUNT_DIRECTION_HEAD : JKRArchive::MOUNT_DIRECTION_TAIL;
    JKRHeap* heap = mHeap != NULL ? mHeap : mDoExt_getArchiveHeap();
    mArchive = JKRArchive::mount(mEntryNum, mMountMode, heap, mountDirection);
    s32 result = mArchive != NULL;
    mIsDone = true;
    return result;
}

mDoDvdThd_toMainRam_c::mDoDvdThd_toMainRam_c(u8 param_0) {
    mMountDirection = param_0;
    if (param_0 == 0) {
        mMountDirection = sDefaultDirection;
    }
}

mDoDvdThd_toMainRam_c* mDoDvdThd_toMainRam_c::create(char const* pArchivePath, u8 mountDirection,
                                                     JKRHeap* pHeap) {
    mDoDvdThd_toMainRam_c* toMainRAMCmd =
        new (mDoExt_getCommandHeap(), -4) mDoDvdThd_toMainRam_c(mountDirection);
    if (toMainRAMCmd != NULL) {
        toMainRAMCmd->mEntryNum = my_DVDConvertPathToEntrynum(pArchivePath);
        if (toMainRAMCmd->mEntryNum == -1) {
            toMainRAMCmd->mIsDone = true;
            delete toMainRAMCmd;
            toMainRAMCmd = NULL;
        } else {
            toMainRAMCmd->mHeap = pHeap;
            mDoDvdThd::l_param.addition(toMainRAMCmd);
        }
    }
    return toMainRAMCmd;
}

mDoDvdThd_toMainRam_c::~mDoDvdThd_toMainRam_c() {}

s32 mDoDvdThd_toMainRam_c::execute() {
    JKRHeap* heap;
    if (mHeap != NULL) {
        heap = mHeap;
    } else {
        heap = mDoExt_getArchiveHeap();
    }
    JKRDvdRipper::EAllocDirection allocDir = mMountDirection == 0 ?
                                                 JKRDvdRipper::ALLOC_DIRECTION_FORWARD :
                                                 JKRDvdRipper::ALLOC_DIRECTION_BACKWARD;
    mData = JKRDvdRipper::loadToMainRAM(mEntryNum, NULL, EXPAND_SWITCH_UNKNOWN1, 0, heap, allocDir,
                                        0, NULL, NULL);
    if (mData != NULL) {
        mDataSize = heap->getSize(mData);
    }
    mIsDone = true;
    return mData != NULL;
}

static void dummy() {
    OSReport("mDoDvdThd_getResource_c::create() クラス生成に失敗\n");
    OSReport("mDoDvdThd_getResource_c::create() リソース取得に失敗\n");
    OSReport("mDoDvdThd_getResource_c::execute() リソース取得に失敗\n");
}
