/**
 * m_Do_dvd_thread.cpp
 * DVD Thread Manager
*/

#include "m_Do/m_Do_dvd_thread.h"
#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_ext.h"

s32 mDoDvdThd::main(void* param_0) {
    JKRThread(OSGetCurrentThread(), 0);
    JKRSetCurrentHeap(mDoExt_getAssertHeap());
    mDoDvdThd_param_c* param = static_cast<mDoDvdThd_param_c*>(param_0);
    param->mainLoop();
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
OSBootInfo* mDoDvdHack::BootInfo;
mDoDvdHack::FSTEntry* mDoDvdHack::FstStart;
const char* mDoDvdHack::FstStringStart;
u32 mDoDvdHack::MaxEntryNum;
mDoDvdHack::Manager mDoDvdHack::Manager::sManager;
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
    OS_REPORT("DVD読み込みスレッドを停止しました\n");
    OSSuspendThread(&l_thread);
}

#if DEBUG
void mDoDvdHack::__DVDFSInit() {
    BootInfo = (OSBootInfo*)OSPhysicalToCached(0);
    FstStart = (FSTEntry*)BootInfo->FSTLocation;
    if (FstStart) {
        MaxEntryNum = FstStart->nextEntryOrLength;
        FstStringStart = (char*)FstStart + (MaxEntryNum* sizeof(FSTEntry));
    }
}

const char* mDoDvdHack::EntryToName(s32 entry) {
    if (entry < 0) {
        return "*ERROR*";
    }
    if (entry == 0) {
        return "/";
    }
    return FstStringStart + (FstStart[entry].isDirAndStringOff & ~0xff000000);
}

const char* mDoDvdHack::ConvertEntrynumToName(s32 entry) {
    __DVDFSInit();
    return EntryToName(entry);
}
#endif

static void dummy1() {
    OS_REPORT(__FILE__);
    OS_REPORT("DVDConvertEntrynumToPath: specified entrynum(%d) is out of range  ");
    OS_REPORT("DVDConvertEntrynumToPath: maxlen should be more than 1 (%d is specified)");
}


static s32 my_DVDConvertPathToEntrynum(char const* path) {
    s32 entrynum = DVDConvertPathToEntrynum(path);
#if DEBUG
    if (entrynum < 0) {
        BOOL connected = mDoCPd_c::isConnect(2);
        if (connected) {
            JUT_WARN(437, "can\'t open:[%s]\n", path);
        }
    }
#endif
    return entrynum;
}

mDoDvdThd_command_c::~mDoDvdThd_command_c() {
    if (!mIsDone) {
        OS_REPORT_ERROR("mDoDvdThd_command_c::~mDoDvdThd_command_c() 完了していないコマンドがデストラクトされました\nきっとカタストロフィが発生することでしょう\n");
    }
}

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
    mDoDvdThd_command_c* pCmd = *(mDoDvdThd_command_c**)param_0;
    s32 result = pCmd->execute();
    if (result != 1) {
        OSReport_Error("mDoDvdThd_param_c::mainLoop() コマンドの実行が失敗しました。\n");
    }
}

void mDoDvdThd_param_c::mainLoop() {
    mDoDvdThd_command_c* command;
    while (this->waitForKick() != 0) {
        while (command = this->getFirstCommand()) {
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
        if (mDoDvdThd::DVDLogoMode) {
            OS_REPORT("\x1b[34m<DVD> callback %08x %08x %08x\n\x1b[m", callCmd, pFunc, pData);
        }
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
            if (mDoDvdThd::DVDLogoMode) {
                OS_REPORT("\x1b[34m<DVD> mountArchive(%d:%s)\n\x1b[m", mountArcCmd->mEntryNumber, pArchivePath);
            }
        }
    }
    return mountArcCmd;
}

s32 mDoDvdThd_mountArchive_c::execute() {
#if PLATFORM_GCN
    bool isZeldaHeap = false;
    bool isGameHeap = false;
    bool isArcHeap = false;
    bool isJ2DHeap = false;
#endif
    JKRHeap* heap = mHeap != NULL ? mHeap : mDoExt_getArchiveHeap();
    JKRMemArchive* memArchive = NULL;
#if DEBUG
    OSTime time1 = OSGetTime();
#endif
    while (true) {
#if PLATFORM_GCN
        if (heap == mDoExt_getArchiveHeapPtr()) {
            isArcHeap = true;
        } else if (heap == mDoExt_getZeldaHeap()) {
            isZeldaHeap = true;
        } else if (heap == mDoExt_getGameHeap()) {
            isGameHeap = true;
        } else if (heap == mDoExt_getJ2dHeap()) {
            isJ2DHeap = true;
        }
#endif
        if (mMountDirection == 0) {
            memArchive =
                new (heap, 0) JKRMemArchive(mEntryNumber, JKRArchive::MOUNT_DIRECTION_HEAD);
        } else {
            memArchive =
                new (heap, -4) JKRMemArchive(mEntryNumber, JKRArchive::MOUNT_DIRECTION_TAIL);
        }
        if (memArchive != NULL && memArchive->isMounted()) {
            mArchive = memArchive;
#if DEBUG
            OSTime time2 = OSGetTime();
            u32 msec = OS_TICKS_TO_MSEC(time2 - time1);
            if (mArchive && mDoDvdThd::verbose) {
                OS_REPORT("mDoDvdThd_mountArchive_c::execute: mArchive=%08x mArcHeader=%08x mMountDirection=%d Time=%4dms %s\n", mArchive, mArchive->mArcHeader, mMountDirection, msec, mDoDvdHack::ConvertEntrynumToName(mEntryNumber));
            }
#endif
            break;
        }
        OSReport_Error("mDoDvdThd_mountArchive_c::execute マウント失敗\n");
        OS_REPORT_ERROR("Name = %s\n", mDoDvdHack::ConvertEntrynumToName(mEntryNumber));
        if (memArchive) {
            delete memArchive;
        }
        memArchive = NULL;
#if PLATFORM_GCN
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
#else
        if (heap != mDoExt_getZeldaHeap()) {
            OSReport_Error("mDoDvdThd_mountArchive_c::execute システムヒープで再チャレンジ！\n");
            heap = mDoExt_getZeldaHeap();
        } else {
            OSReport_FatalError("mDoDvdThd_mountArchive_c::execute ヒープが致命的に足りません！\n");
            break;
        }
#endif
    }
    mIsDone = true;
    return mArchive != NULL;
}

mDoDvdThd_mountAramArchive_c::~mDoDvdThd_mountAramArchive_c() {}

static void dummy_mDoDvdThd_mountAramArchive_c__create() {
    OS_REPORT("<DVD> mountAramArchive(%d:%s)\n");
}

s32 mDoDvdThd_mountAramArchive_c::execute() {
    JKRHeap* heap = mDoExt_getArchiveHeap();
    BOOL result = FALSE;
    if (!mArchive) {
        if (mMountDirection == 0) {
            mArchive = new (heap, 0x20) JKRAramArchive();
        } else {
            mArchive = new (heap, -0x20) JKRAramArchive();
        }
#if DEBUG
        if (mDoDvdThd::verbose) {
            OS_REPORT("mDoDvdThd_mountAramArchive_c::execute mArchive=%08x size=%08x\n", mArchive, JKRGetMemBlockSize(heap, mArchive));
        }
#endif
        JUT_ASSERT(880, mArchive != NULL);
    }
    if (mArchive) {
        JKRArchive::EMountDirection mountDirection = mMountDirection == 0 ? JKRArchive::MOUNT_DIRECTION_HEAD : JKRArchive::MOUNT_DIRECTION_TAIL;
#if DEBUG
        OSTime time1 = OSGetTime();
#endif
        result = mArchive->mountFixed(mEntryNum, mountDirection);
#if DEBUG
        OSTime time2 = OSGetTime();
        u32 msec = OS_TICKS_TO_MSEC(time2 - time1);
        if (mDoDvdThd::verbose) {
            OS_REPORT("mDoDvdThd_mountAramArchive_c::execute: mArchive=%08x ->mAramCache=%08x ->mFileSystem=%08x %s\n", mArchive, mArchive->mBlock, mArchive->mDvdFile, mDoDvdHack::ConvertEntrynumToName(mEntryNum));
        }
#endif
    }
    mIsDone = true;
    return result;
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
            if (mDoDvdThd::DVDLogoMode) {
                OS_WARNING("<DVD> mountXArchive(%d:%s)\n", mountXArcCmd->mEntryNum, pArchivePath);
            }
        }
    }
    return mountXArcCmd;
}

s32 mDoDvdThd_mountXArchive_c::execute() {
#if DEBUG
    OSTime time1 = OSGetTime();
#endif
    JKRArchive::EMountDirection mountDirection =
        mMountDirection == 0 ? JKRArchive::MOUNT_DIRECTION_HEAD : JKRArchive::MOUNT_DIRECTION_TAIL;
    JKRHeap* heap = mHeap != NULL ? mHeap : mDoExt_getArchiveHeap();
    s32 result = 0;
    mArchive = JKRArchive::mount(mEntryNum, mMountMode, heap, mountDirection);
    result = mArchive != NULL;
#if DEBUG
    if (result) {
        OSTime time2 = OSGetTime();
        u32 msec = OS_TICKS_TO_MSEC(time2 - time1);
        if (mDoDvdThd::verbose) {
            OS_REPORT("mDoDvdThd_mountXArchive_c::execute: mArchive=%08x %s\n", mArchive, mDoDvdHack::ConvertEntrynumToName(mEntryNum));
        }
    }
#endif
    mIsDone = true;
    return result;
}

mDoDvdThd_getResource_c::~mDoDvdThd_getResource_c() {}

static void dummy_mDoDvdThd_getResource_c__create() {
    OS_REPORT("<DVD> getResource(%x,%d)\n");
    OSReport("mDoDvdThd_getResource_c::create() クラス生成に失敗\n");
    OSReport("mDoDvdThd_getResource_c::create() リソース取得に失敗\n");
}

s32 mDoDvdThd_getResource_c::execute() {
#if DEBUG
    OSTime time1 = OSGetTime();
#endif
    JUT_ASSERT(1080, mArchive);
    mResource = mArchive->getResource(mResourceId);
#if DEBUG
    OSTime time2 = OSGetTime();
    u32 msec = OS_TICKS_TO_MSEC(time2 - time1);
    if (mArchive && mDoDvdThd::verbose) {
        OS_REPORT("mDoDvdThd_getResource_c::execute: mResouce=%08x  Time=%4dms\n", mResource, msec);
    }
#endif
    if (!mResource) {
        OSReport_Error("mDoDvdThd_getResource_c::execute() リソース取得に失敗\n");
    }
    mIsDone = true;
    return mResource != NULL;
}

static void dummy2() {
    OS_REPORT("mEntryNum != -1");
    OS_REPORT("mDoDvdThd_toMainRam_c(%d:%s)\n");
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
            if (mDoDvdThd::DVDLogoMode) {
                OS_WARNING("<DVD> toMainRam(%d:%s)\n", toMainRAMCmd->mEntryNum, pArchivePath);
            }
        }
    }
    return toMainRAMCmd;
}

mDoDvdThd_toMainRam_c::~mDoDvdThd_toMainRam_c() {}

s32 mDoDvdThd_toMainRam_c::execute() {
#if DEBUG
    OSTime time1 = OSGetTime();
#endif
    JKRHeap* heap;
    if (mHeap != NULL) {
        heap = mHeap;
    } else {
        heap = mDoExt_getArchiveHeap();
    }
    JKRDvdRipper::EAllocDirection allocDir = mMountDirection == 0 ?
                                                 JKRDvdRipper::ALLOC_DIRECTION_FORWARD :
                                                 JKRDvdRipper::ALLOC_DIRECTION_BACKWARD;
    mData = JKRDvdToMainRam(mEntryNum, NULL, EXPAND_SWITCH_UNKNOWN1, 0, heap, allocDir,
                                        0, NULL, NULL);
    if (mData != NULL) {
        mDataSize = heap->getSize(mData);
    }
#if DEBUG
    OSTime time2 = OSGetTime();
    u32 msec = OS_TICKS_TO_MSEC(time2 - time1);
    if (mDoDvdThd::verbose) {
        OS_REPORT("mDoDvdThd_toMainRam_c::execute: mMemAddress=%08x mMemSize=%08x %s\n", mData, mDataSize, mDoDvdHack::ConvertEntrynumToName(mEntryNum));
    }
#endif
    mIsDone = true;
    return mData != NULL;
}
