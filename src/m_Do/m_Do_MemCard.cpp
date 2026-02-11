/**
 * m_Do_MemCard.cpp
 * Memory Card Control
 */

#include <dolphin/card.h>
#include "m_Do/m_Do_MemCard.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_MemCardRWmng.h"
#include "m_Do/m_Do_Reset.h"

#if PLATFORM_WII || PLATFORM_SHIELD
#include <revolution/nand.h>
#include <revolution/sc.h>
#endif

#define SLOT_A 0

#define CHECKSPACE_RESULT_READY    0
#define CHECKSPACE_RESULT_INSSPACE 1
#define CHECKSPACE_RESULT_NOENT    2
#define CHECKSPACE_RESULT_ERROR    3

#if PLATFORM_WII
s32 my_CARDOpen(s32 chan, const char* fileName, CARDFileInfo* fileInfo) {
    CARDStat stat;
    DVDDiskID* diskID = DVDGetCurrentDiskID();
    
    for (int i = 0; i < CARD_MAX_FILE; i++) {
        s32 ret = CARDGetStatus(chan, i, &stat);
        if (ret == CARD_RESULT_READY) {
            if (memcmp(stat.gameName, "GZ2E", 4) == 0 &&
                memcmp(stat.company, diskID->company, 2) == 0 &&
                strncmp(stat.fileName, fileName, 32) == 0)
            {
                return CARDFastOpen(chan, i, fileInfo);
            }
        } else if (ret != CARD_RESULT_NOFILE) {
            return ret;
        }
    }

    return CARD_RESULT_NOFILE;
}
#endif

#if PLATFORM_WII
#define CARD_OPEN my_CARDOpen
#else
#define CARD_OPEN CARDOpen
#endif

#if PLATFORM_WII
#define NAND_OPEN  NANDSafeOpen
#define NAND_CLOSE NANDSafeClose
#elif PLATFORM_SHIELD
#define NAND_OPEN  NANDSimpleSafeOpen
#define NAND_CLOSE NANDSimpleSafeClose
#endif

mDoMemCd_Ctrl_c::mDoMemCd_Ctrl_c() {}

#if PLATFORM_GCN
#define STACK_SIZE 0x1000
#else
#define STACK_SIZE 0x2000
#endif

static u8 MemCardStack[STACK_SIZE];

static OSThread MemCardThread;

void mDoMemCd_Ctrl_c::ThdInit() {
    #if !PLATFORM_SHIELD
    CARDInit();
    #endif

    mCopyToPos = 0;
    mProbeStat = 2;
    mCardState = CARD_STATE_NO_CARD_e;
    mCardCommand = COMM_NONE_e;
    mChannel = SLOT_A;

    OSInitMutex(&mMutex);
    OSInitCond(&mCond);
    OSCreateThread(&MemCardThread, (void*(*)(void*))mDoMemCd_main, NULL, MemCardStack + sizeof(MemCardStack),
                   sizeof(MemCardStack), OSGetThreadPriority(OSGetCurrentThread()) + 1, 1);
    OSResumeThread(&MemCardThread);

    // "Memory Card Thread Init\n"
    OS_REPORT("メモリーカードスレッド起動\n");
}

void mDoMemCd_Ctrl_c::main() {
    do {
        OSLockMutex(&mMutex);
        while (mCardCommand == COMM_NONE_e) {
            OSWaitCond(&mCond, &mMutex);
        }
        OSUnlockMutex(&mMutex);

        switch (mCardCommand) {
        #if PLATFORM_GCN || PLATFORM_WII
        case COMM_RESTORE_e:
            restore();
            break;
        case COMM_STORE_e:
            store();
            break;
        case COMM_FORMAT_e:
            format();
            break;
        case COMM_ATTACH_e:
            attach();
            break;
        case COMM_DETACH_e:
            detach();
            break;
        #elif PLATFORM_SHIELD
        case COMM_RESTORE_e:
        case COMM_STORE_e:
        case COMM_FORMAT_e:
        case COMM_ATTACH_e:
        case COMM_DETACH_e:
            break;
        #endif

        #if PLATFORM_WII || PLATFORM_SHIELD
        case COMM_RESTORE_NAND_e:
            restoreNAND();
            break;
        case COMM_STORE_NAND_e:
            storeNAND();
            break;
        case COMM_STORE_SETUP_NAND_e:
            storeSetUpNAND();
            break;
        #endif
        }

        OSLockMutex(&mMutex);
        mCardCommand = COMM_NONE_e;
        OSUnlockMutex(&mMutex);
    } while (true);
}

void mDoMemCd_Ctrl_c::update() {
    if (mDoRst::isReset()) {
        OSLockMutex(&mMutex);
        mCardCommand = COMM_DETACH_e;
        mProbeStat = 3;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    } else if (getStatus(0) != 14) {
        #if PLATFORM_GCN || PLATFORM_WII
        if (CARDProbe(SLOT_A) && getStatus(0) == 0) {
            OSLockMutex(&mMutex);
            mProbeStat = 0;
            mCardState = CARD_STATE_13_e;
            mCardCommand = COMM_ATTACH_e;
            OSUnlockMutex(&mMutex);
            OSSignalCond(&mCond);
        } else if (!CARDProbe(SLOT_A) && getStatus(0) != 0) {
            OSLockMutex(&mMutex);
            mProbeStat = 1;
            mCardState = CARD_STATE_13_e;
            mCardCommand = COMM_DETACH_e;
            OSUnlockMutex(&mMutex);
            OSSignalCond(&mCond);
        }
        #endif
    }
}

void mDoMemCd_Ctrl_c::load() {
    if (OSTryLockMutex(&mMutex)) {
        field_0x1fc8 = 0;
        mCardCommand = COMM_RESTORE_e;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

#if !PLATFORM_SHIELD
void mDoMemCd_Ctrl_c::restore() {
    CARDFileInfo file;
    field_0x1fc8 = 0;

    s32 ret = CARD_OPEN(mChannel, "gczelda2", &file);
    OS_REPORT("\x1b[43;30mCret=%d\n\x1b[m", ret);
    if (ret == CARD_RESULT_READY) {
        s32 ret2 = mDoMemCdRWm_Restore(&file, this, sizeof(mData));
        OS_REPORT("\x1b[43;30mret2=%d\n\x1b[m", ret2);
        if (ret2 == CARD_RESULT_READY) {
            mCardState = CARD_STATE_READ_e;
        } else {
            setCardState(ret);
        }
        CARDClose(&file);
    } else {
        setCardState(ret);
    }

    field_0x1fc8 = 1;
}
#endif

s32 mDoMemCd_Ctrl_c::LoadSync(void* i_buffer, u32 i_size, u32 i_position) {
    int ret = 0;

    if (field_0x1fc8 == 0) {
        return 0;
    }

    if (OSTryLockMutex(&mMutex)) {
        if (mCardState == CARD_STATE_READ_e) {
            memcpy(i_buffer, &mData[i_position], i_size);
            mCardState = CARD_STATE_READY_e;
            ret = 1;
        } else {
            mCardState = CARD_STATE_NO_FILE_e;
            ret = 2;
        }
        OSUnlockMutex(&mMutex);
    }

    return ret;
}

void mDoMemCd_Ctrl_c::save(void* i_buffer, u32 i_size, u32 i_position) {
    if (OSTryLockMutex(&mMutex)) {
        memcpy(&mData[i_position], i_buffer, i_size);
        field_0x1fc8 = 0;
        mCardCommand = COMM_STORE_e;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

#if !PLATFORM_SHIELD
void mDoMemCd_Ctrl_c::store() {
    CARDFileInfo file;
    s32 ret;
    field_0x1fc8 = 0;

    if (mCardState == CARD_STATE_NO_FILE_e) {
        #if PLATFORM_GCN
        ret = CARDCreate(mChannel, "gczelda2", CARD_FILE_SIZE, &file);
        #else
        ret = CARDCreate(mChannel, "zeldaTp.dat", CARD_FILE_SIZE, &file);
        #endif
        if (ret == CARD_RESULT_READY || ret == CARD_RESULT_EXIST) {
            mCardState = CARD_STATE_READY_e;
        } else {
            setCardState(ret);
        }
    }

    if (mCardState == CARD_STATE_READY_e) {
        ret = CARD_OPEN(mChannel, "gczelda2", &file);
        if (ret == CARD_RESULT_READY) {
            ret = mDoMemCdRWm_Store(&file, this, sizeof(mData));
            if (ret != CARD_RESULT_READY) {
                setCardState(ret);
                OS_REPORT("CARD Write ERR ret:%d\n", ret);
            } else {
                mCardState = CARD_STATE_WRITE_e;
                OS_REPORT("CARD Write OK ret:%d stat:%d\n", ret, mCardState);
            }
            CARDClose(&file);
        } else {
            setCardState(ret);
        }
    } else {
        setCardState(ret);
    }

    field_0x1fc8 = 1;
}
#endif

s32 mDoMemCd_Ctrl_c::SaveSync() {
    int ret = 0;

    if (field_0x1fc8 == 0) {
        return 0;
    }

    if (OSTryLockMutex(&mMutex)) {
        if (mCardState == CARD_STATE_WRITE_e) {
            mCardState = CARD_STATE_READY_e;
            ret = 1;
        } else if (mCardState == CARD_STATE_READY_e) {
            ret = 0;
        } else {
            ret = 2;
        }

        OSUnlockMutex(&mMutex);
    }

    return ret;
}

u32 mDoMemCd_Ctrl_c::getStatus(u32) {
    u32 status;
    if (OSTryLockMutex(&mMutex)) {
        switch (mCardState) {
        case CARD_STATE_READY_e:
            status = 2;
            break;
        case CARD_STATE_NO_FILE_e:
            status = 1;
            break;
        case CARD_STATE_READ_e:
            status = 3;
            break;
        case CARD_STATE_WRITE_e:
            status = 4;
            break;
        case CARD_STATE_FORMAT_e:
            status = 5;
            break;
        case CARD_STATE_NO_CARD_e:
            status = 0;
            break;
        case CARD_STATE_WRONG_ENCODING_e:
            status = 7;
            break;
        case CARD_STATE_BROKEN_e:
            status = 6;
            break;
        case CARD_STATE_INSSPACE_e:
            status = 11;
            break;
        case CARD_STATE_NOENT_e:
            status = 12;
            break;
        case CARD_STATE_WRONG_DEVICE_e:
            status = 9;
            break;
        case CARD_STATE_WRONG_SECTORSIZE_e:
            status = 10;
            break;
        case CARD_STATE_FATAL_ERROR_e:
            status = 8;
            break;
        case CARD_STATE_13_e:
            status = 14;
            break;
        }

        OSUnlockMutex(&mMutex);
        return status;
    }

    return 14;
}

void mDoMemCd_Ctrl_c::command_format() {
    if (OSTryLockMutex(&mMutex)) {
        mCardCommand = COMM_FORMAT_e;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

#if !PLATFORM_SHIELD
void mDoMemCd_Ctrl_c::format() {
    field_0x1fc8 = 0;

    s32 ret = CARDFormat(mChannel);
    if (OSTryLockMutex(&mMutex)) {
        if (ret == CARD_RESULT_READY) {
            mCardState = CARD_STATE_FORMAT_e;
        } else {
            setCardState(ret);
        }

        field_0x1fc8 = 1;
        OSUnlockMutex(&mMutex);
    }
}
#endif

s32 mDoMemCd_Ctrl_c::FormatSync() {
    int ret = 0;

    if (field_0x1fc8 == 0) {
        return 0;
    }

    if (OSTryLockMutex(&mMutex)) {
        if (mCardState != CARD_STATE_13_e) {
            if (mCardState == CARD_STATE_FORMAT_e) {
                mCardState = CARD_STATE_NO_FILE_e;
                ret = 1;
            } else {
                ret = 2;
            }
        }
        OSUnlockMutex(&mMutex);
    }

    return ret;
}

#if !PLATFORM_SHIELD
void mDoMemCd_Ctrl_c::attach() {
    s32 mem_size;
    s32 sector_size;

    s32 ret = CARDProbeEx(mChannel, &mem_size, &sector_size);
    if (ret == CARD_RESULT_NOCARD) {
        mCardState = CARD_STATE_NO_CARD_e;
    } else if (ret == CARD_RESULT_FATAL_ERROR) {
        mCardState = CARD_STATE_FATAL_ERROR_e;
    } else if (ret == CARD_RESULT_WRONGDEVICE) {
        mCardState = CARD_STATE_WRONG_DEVICE_e;
    } else if (sector_size != SECTOR_SIZE) {
        mCardState = CARD_STATE_WRONG_SECTORSIZE_e;
    } else {
        if (mount()) {
            if (loadfile()) {
                mCardState = CARD_STATE_READY_e;
            } else {
                switch (checkspace()) {
                case CHECKSPACE_RESULT_READY:
                    mCardState = CARD_STATE_NO_FILE_e;
                    break;
                case CHECKSPACE_RESULT_INSSPACE:
                    mCardState = CARD_STATE_INSSPACE_e;
                    break;
                case CHECKSPACE_RESULT_NOENT:
                    mCardState = CARD_STATE_NOENT_e;
                    break;
                case CHECKSPACE_RESULT_ERROR:
                    break;
                }
            }
        }
    }
}

void mDoMemCd_Ctrl_c::command_attach() {
    if (OSTryLockMutex(&mMutex)) {
        mCardState = CARD_STATE_13_e;
        mCardCommand = COMM_ATTACH_e;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

void mDoMemCd_Ctrl_c::detach() {
    CARDUnmount(mChannel);
    mCardState = CARD_STATE_NO_CARD_e;
}

static u8 MemCardWorkArea0[CARD_WORKAREA_SIZE] ATTRIBUTE_ALIGN(32);

s32 mDoMemCd_Ctrl_c::mount() {
    s32 result = CARDMount(mChannel, MemCardWorkArea0, NULL);

    switch (result) {
    case CARD_RESULT_IOERROR:
    case CARD_RESULT_FATAL_ERROR:
        mCardState = CARD_STATE_FATAL_ERROR_e;
        return FALSE;
    case CARD_RESULT_NOCARD:
        mCardState = CARD_STATE_NO_CARD_e;
        return FALSE;
    case CARD_RESULT_BROKEN:
    case CARD_RESULT_READY:
        switch (CARDCheck(mChannel)) {
        case CARD_RESULT_READY:
            return TRUE;
        case CARD_RESULT_BROKEN:
            mCardState = CARD_STATE_BROKEN_e;
            return FALSE;
        case CARD_RESULT_IOERROR:
            mCardState = CARD_STATE_FATAL_ERROR_e;
            return FALSE;
        case CARD_RESULT_NOCARD:
            mCardState = CARD_STATE_NO_CARD_e;
            return FALSE;
        case CARD_RESULT_ENCODING:
            mCardState = CARD_STATE_WRONG_ENCODING_e;
            return FALSE;
        }
        break;
    case CARD_RESULT_ENCODING:
        mCardState = CARD_STATE_WRONG_ENCODING_e;
        return FALSE;
    }

    return FALSE;
}

s32 mDoMemCd_Ctrl_c::loadfile() {
    CARDFileInfo file;

    s32 ret = CARD_OPEN(mChannel, "gczelda2", &file);
    if (ret == CARD_RESULT_READY) {
        CARDClose(&file);
        return TRUE;
    } else {
        setCardState(ret);
        return FALSE;
    }
}

s32 mDoMemCd_Ctrl_c::checkspace() {
    s32 bytesNotUsed, filesNotUsed;
    s32 result = CARDFreeBlocks(mChannel, &bytesNotUsed, &filesNotUsed);

    if (result != CARD_RESULT_READY) {
        setCardState(result);
        return CHECKSPACE_RESULT_ERROR;  
    }

    if (bytesNotUsed < CARD_FILE_SIZE) {
        return CHECKSPACE_RESULT_INSSPACE;
    }

    if (filesNotUsed < 1) {
        return CHECKSPACE_RESULT_NOENT;
    }
    
    return CHECKSPACE_RESULT_READY;
}

void mDoMemCd_Ctrl_c::setCardState(s32 i_result) {
    switch (i_result) {
    case CARD_RESULT_IOERROR:
    case CARD_RESULT_FATAL_ERROR:
        mCardState = CARD_STATE_FATAL_ERROR_e;
        break;
    case CARD_RESULT_NOCARD:
        mCardState = CARD_STATE_NO_CARD_e;
        break;
    case CARD_RESULT_BROKEN:
    case CARD_RESULT_READY:
        if (CARDCheck(mChannel) != CARD_RESULT_READY) {
            mCardState = CARD_STATE_BROKEN_e;
        }
        break;
    case CARD_RESULT_ENCODING:
        mCardState = CARD_STATE_WRONG_ENCODING_e;
        break;
    case CARD_RESULT_NOFILE:
        mCardState = CARD_STATE_NO_FILE_e;
        break;
    }
}
#endif

#if PLATFORM_WII || PLATFORM_SHIELD
void mDoMemCd_Ctrl_c::loadNAND() {
    if (OSTryLockMutex(&mMutex)) {
        field_0x1fc8 = 0;
        mCardCommand = COMM_RESTORE_NAND_e;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

void mDoMemCd_Ctrl_c::restoreNAND() {
    NANDFileInfo file;
    s32 ret, ret2;

    field_0x1fc8 = 0;    

    ret = NANDOpen("zeldaTp.dat", &file, NAND_ACCESS_RW);
    OS_REPORT("\x1b[43;30mCret=%d\n\x1b[m", ret);
    if (ret == NAND_RESULT_OK) {
        ret2 = mDoMemCdRWm_RestoreNAND(&file, this, sizeof(mData));
        OS_REPORT("\x1b[43;30mret2=%d\n\x1b[m", ret2);
        if (ret2 == NAND_RESULT_OK) {
            mNandState = NAND_STATE_READ_e;
        } else {
            setNandState(ret);
        }
        NANDClose(&file);
    } else {
        setNandState(ret);
    }

    field_0x1fc8 = 1;
}

s32 mDoMemCd_Ctrl_c::LoadSyncNAND(void* i_buffer, u32 i_size, u32 i_position) {
    int ret = 0;

    if (field_0x1fc8 == 0) {
        return 0;
    }

    if (OSTryLockMutex(&mMutex)) {
        if (mNandState == NAND_STATE_READ_e) {
            memcpy(i_buffer, &mData[i_position], i_size);
            mNandState = NAND_STATE_READY_e;
            ret = 1;
        } else {
            ret = 2;
        }
        OSUnlockMutex(&mMutex);
    }

    return ret;
}

void mDoMemCd_Ctrl_c::saveNAND(void* i_buffer, u32 i_size, u32 i_position) {
    if (OSTryLockMutex(&mMutex)) {
        memcpy(&mData[i_position], i_buffer, i_size);
        field_0x1fc8 = 0;
        mCardCommand = COMM_STORE_NAND_e;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

static u8 l_safeCopyBuf[0x4000];

void mDoMemCd_Ctrl_c::storeNAND() {
    NANDFileInfo file;
    s32 ret;
    field_0x1fc8 = 0;

    if (mNandState == NAND_STATE_NO_FILE_e) {
        ret = NANDCreate("zeldaTp.dat", NAND_PERM_RUSR | NAND_PERM_WUSR | NAND_PERM_RGRP, 0);
        if (ret == NAND_RESULT_OK) {
            mNandState = NAND_STATE_READY_e;
        } else {
            setNandState(ret);
        }
    }

    if (mNandState == NAND_STATE_READY_e) {
        u8 sp8 = 1;
        ret = NANDCreate("banner.bin", NAND_PERM_RUSR | NAND_PERM_WUSR | NAND_PERM_RGRP, 0);
        printf("NAND bannerFile Create ret:%d\n", ret);
        if (ret == NAND_RESULT_OK || ret == NAND_RESULT_EXISTS) {
            ret = NAND_OPEN("banner.bin", &file, NAND_ACCESS_RW, l_safeCopyBuf, sizeof(l_safeCopyBuf));
            if (ret == NAND_RESULT_OK) {
                ret = mDoMemCdRWm_StoreBannerNAND(&file);
                if (ret == NAND_RESULT_OK) {
                    printf("NAND bannerFile Write OK ret:%d\n", ret);
                } else {
                    setNandState(ret);
                    printf("NAND bannerFile Write ERR ret:%d\n", ret);
                }
                NAND_CLOSE(&file);
            } else {
                setNandState(ret);
            }
        } else {
            setNandState(ret);
        }

        if (ret == NAND_RESULT_OK) {
            ret = NAND_OPEN("zeldaTp.dat", &file, NAND_ACCESS_RW, l_safeCopyBuf, sizeof(l_safeCopyBuf));
            if (ret == NAND_RESULT_OK) {
                ret = mDoMemCdRWm_StoreNAND(&file, this, sizeof(mData));
                if (ret == NAND_RESULT_OK) {
                    mNandState = NAND_STATE_WRITE_e;
                    printf("NAND Write OK ret:%d stat:%d\n", ret, mNandState);
                } else {
                    setNandState(ret);
                    printf("NAND Write ERR ret:%d\n", ret);
                }
                NAND_CLOSE(&file);
            } else {
                setNandState(ret);
            }
        }
    } else {
        setNandState(ret);
    }

    field_0x1fc8 = 1;
}

s32 mDoMemCd_Ctrl_c::SaveSyncNAND() {
    int ret = 0;

    if (field_0x1fc8 == 0) {
        return 0;
    }

    if (OSTryLockMutex(&mMutex)) {
        if (mNandState == NAND_STATE_WRITE_e) {
            mNandState = NAND_STATE_READY_e;
            ret = 1;
        } else {
            ret = 2;
        }

        printf("NAND Save Sync ret:%d stat:%d\n", ret, mNandState);
        OSUnlockMutex(&mMutex);
    }

    return ret;
}

void mDoMemCd_Ctrl_c::storeSetUpNAND() {
    field_0x1fc8 = 0;
    
    while ((int)SCCheckStatus() != 0) {}

    #if PLATFORM_WII
    if (!SCFlush()) {
        mNandState = NAND_STATE_WRITE_e;
        printf("== 本体設定Write OK ==\n");
    } else {
        mNandState = NAND_STATE_FATAL_ERROR_e;
        printf("== 本体設定Write ERR ==\n");
    }
    #else
    mNandState = NAND_STATE_WRITE_e;
    #endif

    field_0x1fc8 = 1;
}

void m_Do_MemCard_cpp__dummyString() {
    DEAD_STRING("本体設定 Save Sync ret:%d stat:%d\n");
}

void mDoMemCd_Ctrl_c::setNandState(s32 i_result) {
    switch (i_result) {
    case NAND_RESULT_CORRUPT:
        mNandState = NAND_STATE_BROKEN_e;
        break;
    case NAND_RESULT_MAXBLOCKS:
        mNandState = NAND_STATE_INSSPACE_e;
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_MAXFD:
        mNandState = NAND_STATE_NOENT_e;
        break;
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_AUTHENTICATION:
        mNandState = NAND_STATE_AUTHENTICATION_e;
        break;
    case NAND_RESULT_NOEXISTS:
        mNandState = NAND_STATE_NO_FILE_e;
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
        mNandState = NAND_STATE_FATAL_ERROR_e;
        break;
    case -7:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        break;
    }
}

u32 mDoMemCd_Ctrl_c::getStatusNAND() {
    u32 status;
    if (OSTryLockMutex(&mMutex)) {
        switch (mNandState) {
        case NAND_STATE_READY_e:
            status = 1;
            break;
        case NAND_STATE_NO_FILE_e:
            status = 0;
            break;
        case NAND_STATE_READ_e:
            status = 2;
            break;
        case NAND_STATE_WRITE_e:
            status = 3;
            break;
        case NAND_STATE_FORMAT_e:
            status = 6;
            break;
        case NAND_STATE_AUTHENTICATION_e:
            status = 4;
            break;
        case NAND_STATE_BROKEN_e:
            status = 5;
            break;
        case NAND_STATE_INSSPACE_e:
            status = 7;
            break;
        case NAND_STATE_NOENT_e:
            status = 8;
            break;
        case NAND_STATE_FATAL_ERROR_e:
            status = 9;
            break;
        case NAND_STATE_10_e:
            status = 10;
            break;
        }

        OSUnlockMutex(&mMutex);
        return status;
    }

    return 10;
}

s32 mDoMemCd_Ctrl_c::chekNANDFile() {
    NANDFileInfo file;

    s32 ret = NANDOpen("zeldaTp.dat", &file, NAND_ACCESS_RW);
    if (ret == NAND_RESULT_OK) {
        mNandState = NAND_STATE_READY_e;
        NANDClose(&file);
        return 1;
    }

    switch (checkspaceNAND()) {
    case CHECKSPACE_RESULT_READY:
        mNandState = NAND_STATE_NO_FILE_e;
        break;
    case CHECKSPACE_RESULT_INSSPACE:
        mNandState = NAND_STATE_INSSPACE_e;
        break;
    case CHECKSPACE_RESULT_NOENT:
        mNandState = NAND_STATE_NOENT_e;
        break;
    case CHECKSPACE_RESULT_ERROR:
        setNandState(ret);
        break;
    }

    return 0;
}

s32 mDoMemCd_Ctrl_c::checkspaceNAND() {
    u32 answer = -1;
    s32 ret;

    s32 result = NANDCheck(3, 2, &answer);
    if (result != NAND_RESULT_OK) {
        setNandState(result);
        return CHECKSPACE_RESULT_ERROR;  
    }

    if (answer == 0) {
        ret = 0;
    } else if (answer & 5) {
        ret = 1;
    } else if (answer & 10) {
        ret = 2;
    }
    
    return ret;
}
#endif

mDoMemCd_Ctrl_c g_mDoMemCd_control;

static int mDoMemCd_main(void*) {
    JKRThread(OSGetCurrentThread(), 0);

    JKRSetCurrentHeap(mDoExt_getAssertHeap());

    g_mDoMemCd_control.main();
    return 0;
}
