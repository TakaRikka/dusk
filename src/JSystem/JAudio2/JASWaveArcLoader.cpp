#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASWaveArcLoader.h"
#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JAudio2/JASTaskThread.h"
#include "JSystem/JAudio2/JASMutex.h"
#include "JSystem/JKernel/JKRDvdAramRipper.h"
#include <string>
#include <dolphin/os.h>
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
    JUT_ASSERT(40, std::strlen(dir) < DIR_MAX - 1);
    strcpy(sCurrentDir, dir);
    u32 len = strlen(sCurrentDir);
    if (sCurrentDir[len - 1] != '/') {
        JUT_ASSERT(45, len + 1 < DIR_MAX);
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
    if (JKRDvdAramRipper::loadToAram(tmp->mEntryNum, tmp->mBase, EXPAND_SWITCH_UNKNOWN0, 0, 0, NULL) == NULL) {
        JUT_WARN(129, "%s", "loadToAram Failed");
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
    loadToAramCallbackParams commandInfo;
    commandInfo.mWavArc = this;
    commandInfo.mEntryNum = mEntryNum;
    commandInfo.mBase = (uintptr_t)mHeap.getBase();
    commandInfo._c = ++_58;

    _5a++;

    if (JASDvd::getThreadPointer()->sendCmdMsg(loadToAramCallback, &commandInfo, 0x10) == 0) {
        JUT_WARN(193, "%s", "sendCmdMsg loadToAramCallback Failed");
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
    if (mHeap.alloc(heap, mFileLength) == false) {
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
    if (mHeap.allocTail(heap, mFileLength) == false) {
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
    if (DVDFastOpen(entryNum, &fileInfo) == FALSE) {
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
    char* path = new (JASKernel::getSystemHeap(), -4) char[length + 1];
    JUT_ASSERT(322, path);
    strcpy(path, currentDir);
    strcat(path, fileName);
    path[length] = '\0';
    int entryNum = DVDConvertPathToEntrynum(path);
    delete[] path;
    if (entryNum < 0) {
        return;
    }
    setEntryNum(entryNum);
}
