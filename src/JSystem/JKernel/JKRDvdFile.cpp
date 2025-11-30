#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRDvdFile.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTException.h"

JSUList<JKRDvdFile> JKRDvdFile::sDvdList;

JKRDvdFile::JKRDvdFile() : mDvdLink(this) {
    initiate();
}

JKRDvdFile::JKRDvdFile(const char* name) : mDvdLink(this) {
    initiate();
    bool result = open(name);
    mIsAvailable = result;
    // weird code. doesn't match without this, maybe remains from assert or something?
    if (mIsAvailable)
        return;
    else
        return;
}

JKRDvdFile::JKRDvdFile(s32 entryNum) : mDvdLink(this) {
    initiate();
    bool result = open(entryNum);
    mIsAvailable = result;
    // weird code. doesn't match without this, maybe remains from assert or something?
    if (mIsAvailable)
        return;
    else
        return;
}

JKRDvdFile::~JKRDvdFile() {
    close();
}

void JKRDvdFile::initiate(void) {
    mDvdFile = this;
    OSInitMutex(&mMutex1);
    OSInitMutex(&mMutex2);
    OSInitMessageQueue(&mMessageQueue2, &mMessage2, 1);
    OSInitMessageQueue(&mMessageQueue1, &mMessage1, 1);
    mOSThread = NULL;
    field_0x50 = 0;
    field_0x58 = 0;
}

bool JKRDvdFile::open(const char* name) {
    if (!mIsAvailable) {
        mIsAvailable = DVDOpen(name, &mFileInfo);
        if (mIsAvailable) {
            getDvdList().append(&mDvdLink);
            getStatus();
        }
    }
    return mIsAvailable;
}

bool JKRDvdFile::open(s32 entryNum) {
    if (!mIsAvailable) {
        mIsAvailable = DVDFastOpen(entryNum, &mFileInfo);
        if (mIsAvailable) {
            getDvdList().append(&mDvdLink);
            getStatus();
        }
    }
    return mIsAvailable;
}

void JKRDvdFile::close() {
    if (mIsAvailable) {
        s32 result = DVDClose(&mFileInfo);
        if (result != 0) {
            mIsAvailable = false;
            getDvdList().remove(&mDvdLink);
        } else {
            JUTException::panic(__FILE__, 213, "cannot close DVD file\n");
        }
    }
}

s32 JKRDvdFile::readData(void* param_1, s32 length, s32 param_3) {
    /* clang-format off */
    // The assert condition gets stringified as "( length & 0x1f ) == 0", 
    // with out disabling clang-format the spaces in the condition will  
    // get removed and the string will be incorrect.
    JUT_ASSERT(238, ( length & 0x1f ) == 0);
    /* clang-format on */

    OSLockMutex(&mMutex1);
    if (mOSThread) {
        OSUnlockMutex(&mMutex1);
        return -1;
    }

    mOSThread = OSGetCurrentThread();

    s32 result = -1;
    s32 readAsyncResult =
        DVDReadAsyncPrio(&mFileInfo, param_1, length, param_3, JKRDvdFile::doneProcess, 2);
    if (readAsyncResult) {
        result = sync();
    }

    mOSThread = NULL;
    OSUnlockMutex(&mMutex1);

    return result;
}

s32 JKRDvdFile::writeData(void const* param_0, s32 length, s32 param_2) {
    /* clang-format off */
    JUT_ASSERT(344, ( length & 0x1f ) == 0);
    /* clang-format on */
    return -1;
}

s32 JKRDvdFile::sync(void) {
    OSMessage message;
    OSLockMutex(&mMutex1);
    OSReceiveMessage(&mMessageQueue2, &message, 1);
    mOSThread = NULL;
    OSUnlockMutex(&mMutex1);
    return (int)message;
}

void JKRDvdFile::doneProcess(s32 id, DVDFileInfo* fileInfo) {
    // fileInfo->field_0x3c looks like some kind of user pointer?
    JKRDvdFile* dvdFile = *(JKRDvdFile**)((u8*)fileInfo + 0x3c);
    OSSendMessage(&dvdFile->mMessageQueue2, (OSMessage)id, OS_MESSAGE_NOBLOCK);
}

s32 JKRDvdFile::getFileSize(void) const {
    return mFileInfo.length;
}
