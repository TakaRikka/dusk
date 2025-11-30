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

#define SLOT_A 0

#define CHECKSPACE_RESULT_READY    0
#define CHECKSPACE_RESULT_INSSPACE 1
#define CHECKSPACE_RESULT_NOENT    2
#define CHECKSPACE_RESULT_ERROR    3

mDoMemCd_Ctrl_c::mDoMemCd_Ctrl_c() {}

static u8 MemCardStack[0x1000];

static OSThread MemCardThread;

void mDoMemCd_Ctrl_c::ThdInit() {
    CARDInit();
    mCopyToPos = 0;
    mProbeStat = 2;
    mCardState = CARD_STATE_NO_CARD_e;
    mCardCommand = COMM_NONE_e;
    mChannel = SLOT_A;

    OSInitMutex(&mMutex);
    OSInitCond(&mCond);

    int priority = OSGetThreadPriority(OSGetCurrentThread());
    OSCreateThread(&MemCardThread, (void*(*)(void*))mDoMemCd_main, NULL, MemCardStack + sizeof(MemCardStack),
                   sizeof(MemCardStack), priority + 1, 1);
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

void mDoMemCd_Ctrl_c::restore() {
    CARDFileInfo file;
    field_0x1fc8 = 0;

    s32 ret = CARDOpen(mChannel, "gczelda2", &file);
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

void mDoMemCd_Ctrl_c::store() {
    CARDFileInfo file;
    s32 ret;
    field_0x1fc8 = 0;

    if (mCardState == CARD_STATE_NO_FILE_e) {
        ret = CARDCreate(mChannel, "gczelda2", SAVEDATA_FILE_SIZE, &file);
        if (ret == CARD_RESULT_READY || ret == CARD_RESULT_EXIST) {
            mCardState = CARD_STATE_READY_e;
        } else {
            setCardState(ret);
        }
    }

    if (mCardState == CARD_STATE_READY_e) {
        ret = CARDOpen(mChannel, "gczelda2", &file);
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

        OS_REPORT("CARD Save Sync ret:%d stat:%d\n", ret, mCardState);
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

    s32 ret = CARDOpen(mChannel, "gczelda2", &file);
    if (ret == CARD_RESULT_READY) {
        CARDClose(&file);
        return TRUE;
    } else {
        setCardState(ret);
        return FALSE;
    }
}

// 
s32 mDoMemCd_Ctrl_c::checkspace() {
    s32 bytesNotUsed, filesNotUsed;
    s32 result = CARDFreeBlocks(mChannel, &bytesNotUsed, &filesNotUsed);

    if (result != CARD_RESULT_READY) {
        setCardState(result);
        return CHECKSPACE_RESULT_ERROR;  
    }

    if (bytesNotUsed < SAVEDATA_FILE_SIZE) {
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

mDoMemCd_Ctrl_c g_mDoMemCd_control;

static int mDoMemCd_main(void*) {
    { JKRThread thread(OSGetCurrentThread(), 0); }

    mDoExt_getAssertHeap()->becomeCurrentHeap();

    g_mDoMemCd_control.main();
    return 0;
}
