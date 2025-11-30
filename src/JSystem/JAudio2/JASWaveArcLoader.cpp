#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASWaveArcLoader.h"
#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JAudio2/JASTaskThread.h"
#include "JSystem/JAudio2/JASMutex.h"
#include "JSystem/JKernel/JKRDvdAramRipper.h"
#include "cstring.h"
#include "dolphin/os.h"
#include <stdint.h>

JASHeap* JASWaveArcLoader::sAramHeap;

JASHeap* JASWaveArcLoader::getRootHeap() {
    if (JASWaveArcLoader::sAramHeap) {
        return JASWaveArcLoader::sAramHeap;
    }
    return JASKernel::getAramHeap();
}

char JASWaveArcLoader::sCurrentDir[DIR_MAX] = "/AudioRes/Waves/";

void JASWaveArcLoader::setCurrentDir(char const* dir) {
    ASSERT(std::strlen(dir) < DIR_MAX - 1);
    strcpy(sCurrentDir, dir);
    u32 len = strlen(sCurrentDir);
    if (sCurrentDir[len - 1] != '/') {
        ASSERT(len + 1 < DIR_MAX);
        sCurrentDir[len] = '/';
        sCurrentDir[len + 1] = '\0';
    }
}

char* JASWaveArcLoader::getCurrentDir() {
    return sCurrentDir;
}

JASWaveArc::JASWaveArc() : mHeap(this) {
    _48 = 0;
    mStatus = 0;
    mEntryNum = -1;
    mFileLength = 0;
    _58 = 0;
    _5a = 0;
    OSInitMutex(&mMutex);
}

JASWaveArc::~JASWaveArc() {}

bool JASWaveArc::loadSetup(u32 param_0) {
    JASMutexLock mutexLock(&mMutex);
    if (_58 != param_0) {
        return false;
    }
    if (mStatus != 1) {
        return false;
    }
    _48 = 1;
    mStatus = 2;
    return true;
}

bool JASWaveArc::eraseSetup() {
    JASMutexLock mutexLock(&mMutex);
    if (mStatus == 0) {
        return false;
    }
    if (mStatus == 1) {
        mStatus = 0;
        return false;
    }
    _48 = 0;
    mStatus = 0;
    return true;
}

void JASWaveArc::loadToAramCallback(void* this_) {
    loadToAramCallbackParams* tmp = (loadToAramCallbackParams*)this_;
    JASWaveArc* wavArc = tmp->mWavArc;
    JKRAramBlock* block = JKRDvdAramRipper::loadToAram(tmp->mEntryNum, tmp->mBase,
                                                       EXPAND_SWITCH_UNKNOWN0, 0, 0, NULL);
    if (block == NULL) {
        // "loadToAram Failed"
        return;
    }
    wavArc->_5a--;
    if (wavArc->loadSetup(tmp->_c)) {
        wavArc->onLoadDone();
    }
}

bool JASWaveArc::sendLoadCmd() {
    JASMutexLock mutexLock(&mMutex);
    _48 = 0;
    mStatus = 1;
    void* base = mHeap.getBase();
    loadToAramCallbackParams commandInfo;
    commandInfo.mWavArc = this;
    commandInfo.mEntryNum = mEntryNum;
    commandInfo.mBase = (uintptr_t)mHeap.mBase;
    commandInfo._c = ++_58;

    _5a++;

    JASTaskThread* thread = JASDvd::getThreadPointer();
    int status = thread->sendCmdMsg(loadToAramCallback, &commandInfo, 0x10);
    if (status == 0) {
        // "sendCmdMsg loadToAramCallback Failed"
        mHeap.free();
        return false;
    }
    return true;
}

bool JASWaveArc::load(JASHeap* heap) {
    if (mEntryNum < 0) {
        return false;
    }
    JASMutexLock mutexLock(&mMutex);
    if (mStatus != 0) {
        return false;
    }
    if (heap == NULL) {
        heap = JASWaveArcLoader::getRootHeap();
    }
    bool result = mHeap.alloc(heap, mFileLength);
    if (result == false) {
        return false;
    }
    return sendLoadCmd();
}

bool JASWaveArc::loadTail(JASHeap* heap) {
    if (mEntryNum < 0) {
        return false;
    }
    JASMutexLock mutexLock(&mMutex);
    if (mStatus != 0) {
        return false;
    }
    if (heap == NULL) {
        heap = JASWaveArcLoader::getRootHeap();
    }
    bool result = mHeap.allocTail(heap, mFileLength);
    if (result == false) {
        return false;
    }
    return sendLoadCmd();
}

bool JASWaveArc::erase() {
    return mHeap.free();
}

void JASWaveArc::onDispose() {
    if (eraseSetup()) {
        onEraseDone();
    }
}

void JASWaveArc::setEntryNum(s32 entryNum) {
    DVDFileInfo fileInfo;
    if (entryNum < 0) {
        return;
    }
    BOOL openStatus = DVDFastOpen(entryNum, &fileInfo);
    if (openStatus == FALSE) {
        return;
    }
    mFileLength = fileInfo.length;
    DVDClose(&fileInfo);
    mEntryNum = entryNum;
}

void JASWaveArc::setFileName(char const* fileName) {
    char* currentDir = JASWaveArcLoader::getCurrentDir();
    size_t length = strlen(currentDir);
    length = length + strlen(fileName);
    JKRHeap* systemHeap = JASKernel::getSystemHeap();
    char* fileString = new (systemHeap, -4) char[length + 1];
    strcpy(fileString, currentDir);
    strcat(fileString, fileName);
    fileString[length] = '\0';
    int entryNum = DVDConvertPathToEntrynum(fileString);
    delete[] fileString;
    if (entryNum < 0) {
        return;
    }
    setEntryNum(entryNum);
}
