/**
 * DynamicLink.cpp
 * REL Linking Manager
 */

#include "DynamicLink.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRFileCache.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "stdio.h"
#include "m_Do/m_Do_dvd_thread.h"
#include "m_Do/m_Do_ext.h"

DynamicModuleControlBase* DynamicModuleControlBase::mFirst;

DynamicModuleControlBase* DynamicModuleControlBase::mLast;

DynamicModuleControlBase::~DynamicModuleControlBase() {
    force_unlink();
    if (mPrev != NULL) {
        mPrev->mNext = mNext;
    }
    if (mNext != NULL) {
        mNext->mPrev = mPrev;
    }
    if (mFirst == this) {
        mFirst = mNext;
    }
    if (mLast == this) {
        mLast = mPrev;
    }
    mNext = NULL;
    mPrev = NULL;
}

DynamicModuleControlBase::DynamicModuleControlBase() {
    mLinkCount = 0;
    mDoLinkCount = 0;
    mNext = NULL;
    if (mFirst == NULL) {
        mFirst = this;
    }
    mPrev = mLast;
    if (mPrev != NULL) {
        mPrev->mNext = this;
    }
    mLast = this;
}

BOOL DynamicModuleControlBase::link() {
    if (mLinkCount == 0) {
        do_load();
        if (do_link() == false) {
            return false;
        }
        if (mDoLinkCount < 0xFFFF) {
            mDoLinkCount++;
        }
    }
    if (mLinkCount < 0xFFFF) {
        mLinkCount++;
    }
    return true;
}

BOOL DynamicModuleControlBase::unlink() {
    if (mLinkCount != 0) {
        mLinkCount--;
        if (mLinkCount == 0) {
            do_unlink();
            do_unload();
        }
    } else {
        OSReport_Warning("%08x DynamicModuleControlBase::unlink() mLinkCount id already zero.\n",
                         this);
    }
    return true;
}

BOOL DynamicModuleControlBase::load_async() {
    if (mLinkCount == 0) {
        return do_load_async();
    }
    return true;
}

bool DynamicModuleControlBase::force_unlink() {
    if (mLinkCount != 0) {
        mLinkCount = 0;
        do_unlink();
    }
    return true;
}

void DynamicModuleControlBase::dump() {
    u16 doLinkCount;
    u16 linkCount;
    DynamicModuleControlBase* current = getFirstClass();
    size_t totalSize = 0;
    JUTReportConsole_f("\nDynamicModuleControlBase::dump()\n");
    JUTReportConsole_f("Do  Lnk Size      Name\n");
    while (current != NULL) {
        doLinkCount = current->mDoLinkCount;
        linkCount = current->mLinkCount;
        if (doLinkCount != 0 || linkCount != 0) {
            u32 size = current->getModuleSize();
            const char* name = current->getModuleName();
            if (size < 0xFFFFFFFF) {
                name = (name != NULL) ? name : "(Null)";
                const char* type = current->getModuleTypeString();
                JUTReportConsole_f("%3d%3d%5.1f %05x %-4s %-24s ", doLinkCount, linkCount,
                                   size / 1024.0f, size, type, name);
                totalSize = totalSize + size;
            } else {
                name = (name != NULL) ? name : "(Null)";
                const char* type = current->getModuleTypeString();
                JUTReportConsole_f("%3d%3d ???? ????? %-4s %-24s ", doLinkCount, linkCount, type,
                                   name);
            }
            current->dump2();
            JUTReportConsole_f("\n");
        }
        current = current->getNextClass();
    }
    JUTReportConsole_f("TotalSize %6.2f %06x\n\n", totalSize / 1024.0f, totalSize);
}

DynamicModuleControl::DynamicModuleControl(char const* name) {
    mModule = NULL;
    mBss = NULL;
    unk_24 = 0;
    mName = name;
    mResourceType = 0;
    unk_33 = 0;
    mChecksum = 0;
    mSize = 0;
    mAsyncLoadCallback = NULL;
}

u32 DynamicModuleControl::sAllocBytes;

JKRArchive* DynamicModuleControl::sArchive;

JKRFileCache* DynamicModuleControl::sFileCache;

static const char* baseString = "Base";

JKRArchive* DynamicModuleControl::mountCallback(void* param_0) {
    JKRExpHeap* heap = mDoExt_getArchiveHeap();
    sFileCache = JKRFileCache::mount("/rel/Final/Release", heap, NULL);
    sArchive = JKRArchive::mount("RELS.arc", JKRArchive::MOUNT_COMP, heap,
                                 JKRArchive::MOUNT_DIRECTION_HEAD);
    if (sArchive == NULL) {
        // "Mount failure, but if the archive isn't created, it was too slow %s\n"
        OSReport_Warning("マウント失敗ですが単にアーカイブを作ってないだけなら遅いだけです %s\n",
                         "RELS.arc");
    }
    return sArchive;
}

bool DynamicModuleControl::initialize() {
    sFileCache = NULL;
    sAllocBytes = 0;
    sArchive = NULL;
    mountCallback(NULL);
    return true;
}

bool DynamicModuleControl::callback(void* moduleControlPtr) {
    DynamicModuleControl* moduleControl = (DynamicModuleControl*)moduleControlPtr;
    return moduleControl->do_load();
}

static u32 calcSum2(u16 const* data, u32 size) {
    u32 sum = 0;
    while (size > 0) {
        sum = sum + *data;
        size = size - 2;
        data++;
    }
    return sum;
}

bool DynamicModuleControl::do_load() {
    if (mModule != NULL) {
        return true;
    }
    JKRExpHeap* heap = mDoExt_getArchiveHeap();
    s32 i = 0;
    while (true) {
        if (mModule != NULL) {
            JKRFree(mModule);
            mModule = NULL;
        }
        char buffer[64];
        snprintf(buffer, 64, "%s.rel", mName);
        if (mModule == NULL && sArchive != NULL) {
            if (mModule == NULL) {
                mModule = (OSModuleHeader*)JKRGetResource('MMEM', buffer, sArchive);
                if (mModule != NULL) {
                    mResourceType = 1;
                }
            }
            if (mModule == NULL) {
                mModule = (OSModuleHeader*)JKRGetResource('AMEM', buffer, sArchive);
                if (mModule != NULL) {
                    mResourceType = 2;
                }
            }
            if (mModule == NULL) {
                mModule = (OSModuleHeader*)JKRGetResource('DMEM', buffer, sArchive);
                if (mModule != NULL) {
                    mResourceType = 3;
                }
            }
        }
        if (mModule != NULL) {
            mSize = sArchive->getExpandedResSize(mModule);
            JKRDetachResource(mModule, NULL);
        } else {
            if (mModule == NULL) {
                snprintf(buffer, 64, "/rel/Final/Release/%s.rel", mName);
                mModule = (OSModuleHeader*)JKRDvdToMainRam(
                    buffer, NULL, EXPAND_SWITCH_UNKNOWN1, NULL, heap,
                    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, NULL, NULL);
                if (mModule != NULL) {
                    mSize = 0;
                    mResourceType = 7;
                }
            }
            if (mModule == NULL && sFileCache != NULL) {
                mModule = (OSModuleHeader*)sFileCache->getResource('rels', buffer);
                if (mModule != NULL) {
                    mSize = 0;
                    mResourceType = 11;
                    JKRDetachResource(mModule, NULL);
                }
            }
        }
        if (mModule == NULL) {
            // "DynamicModuleControl::do_load() Resource load failure [%s]\n"
            OSReport_Error(
                "\x44\x79\x6E\x61\x6D\x69\x63\x4D\x6F\x64\x75\x6C\x65\x43\x6F\x6E\x74\x72\x6F\x6C"
                "\x3A\x3A\x64\x6F\x5F\x6C\x6F\x61\x64\x28\x29\x20\x83\x8A\x83\x5C\x81\x5B\x83\x58"
                "\x93\xC7\x82\xDD\x8D\x9E\x82\xDD\x8E\xB8\x94\x73\x20\x5B\x25\x73\x5D\x0A",
                mName);
            return false;
        }
        if (mSize > 0) {
            u32 sum = calcSum2((u16*)mModule, mSize);
            if (unk_33 == 0) {
                mChecksum = sum;
                unk_33++;
            } else {
                u32 newsum = sum & 0xFFFF;
                if (newsum != mChecksum) {
                    OSReport_Error(
                        // "DynamicModuleControl::do_load() Checksum Error %04x %04x[%s]\n"
                        "DynamicModuleControl::do_load() チェックサムエラー %04x %04x[%s]\n",
                        newsum, mChecksum, mName);
                    unk_33 = 0;
                    i++;
                    if (i >= 3) {
                        return false;
                    }
                } else {
                    if (unk_33 < 0xFF) {
                        unk_33++;
                    }
                    break;
                }
            }
        } else {
            if (unk_33 < 0xFF) {
                unk_33++;
            }
            break;
        }
    }
    return true;
}

BOOL DynamicModuleControl::do_load_async() {
    if (mAsyncLoadCallback == NULL) {
        if (mModule != NULL) {
            return true;
        }
        mAsyncLoadCallback = mDoDvdThd_callback_c::create((mDoDvdThd_callback_func)DynamicModuleControl::callback, this);
        if (mAsyncLoadCallback == NULL) {
            OSReport_Error(
                // "DynamicModuleControl::do_load_async() async load callback entry failure [%s]\n"
                "DynamicModuleControl::do_load_async() 非同期読み込みコールバック登録失敗 [%s]\n",
                mName);
        }
    }
    if (mAsyncLoadCallback != NULL && mAsyncLoadCallback->sync()) {
        mAsyncLoadCallback->destroy();
        mAsyncLoadCallback = NULL;
        return true;
    } else {
        return false;
    }
}

bool DynamicModuleControl::do_unload() {
    if (mModule != NULL) {
        JKRFree(mModule);
        mModule = NULL;
    }
    return true;
}

void DynamicModuleControl::dump2() {
    if (mModule != NULL) {
        OSSectionInfo* section = (OSSectionInfo*)mModule->info.sectionInfoOffset;
        OSSectionInfo* section2 = section + 1;
        u32 offset = section2->offset & ~(1);
        OSReport("%08x-%08x %08x %08x", mModule, offset, offset + section2->size);
    }
}

BOOL DynamicModuleControl::do_link() {
    OSGetTime();
    if (mModule == NULL) {
        do_load();
    }
    if (mModule != NULL) {
        ASSERT(mModule->info.sectionInfoOffset < 0x80000000);
        ASSERT((u32)mModule + mModule->fixSize < 0x82000000);
        OSGetTime();
        OSGetTime();
        if (mModule->info.version >= 3) {
            u32 fixSizePtr;
            u32 fixSize = mModule->fixSize;
            u32 fixSize2 = (fixSize + 0x1f) & ~0x1f;
            fixSizePtr = (u32)mModule + fixSize2;
            s32 size = JKRGetMemBlockSize(NULL, mModule);
            if (size < 0) {
                void* bss = JKRAlloc(mModule->bssSize, 0x20);
                if (bss == NULL) {
                    // "BSS Memory allocation failed\n"
                    OSReport_Error("BSSメモリ確保失敗\n", bss);
                    goto error;
                }
                mBss = bss;
                BOOL linkResult = OSLink(&mModule->info, bss);
                if (linkResult == FALSE) {
                    // "link failed\n"
                    OSReport_Error("リンク失敗\n");
                    goto error;
                }
            } else {
                if (fixSize2 + mModule->bssSize < size) {
                    BOOL linkResult = OSLinkFixed(&mModule->info, (void*)fixSizePtr);
                    if (linkResult == FALSE) {
                        // "link failed\n"
                        OSReport_Error("リンク失敗\n");
                        goto error;
                    }
                    s32 result = JKRResizeMemBlock(NULL, mModule, fixSize2 + mModule->bssSize);
                    if (result < 0) {
                        // "Module size (resize) failed\n"
                        OSReport_Error("モジュールリサイズ(縮小)失敗\n");
                    }
                } else {
                    s32 result = JKRResizeMemBlock(NULL, mModule, fixSize2 + mModule->bssSize);
                    if (result > 0) {
                        BOOL linkResult = OSLinkFixed(&mModule->info, (void*)fixSizePtr);
                        if (linkResult == FALSE) {
                            // "link failed\n"
                            OSReport_Error("リンク失敗\n");
                            goto error;
                        }
                    } else {
                        void* bss = JKRAlloc(mModule->bssSize, 0x20);
                        if (bss == NULL) {
                            // "BSS Memory allocation failure [%x]\n"
                            OSReport_Error("BSSメモリ確保失敗 [%x]\n", mModule->bssSize);
                            goto error;
                        }
                        mBss = bss;
                        BOOL linkResult = OSLinkFixed(&mModule->info, bss);
                        if (linkResult == FALSE) {
                            // "link failed\n"
                            OSReport_Error("リンク失敗\n");
                            goto error;
                        }
                        s32 result = JKRResizeMemBlock(NULL, mModule, fixSize);
                        if (result < 0) {
                            // "Module size (resize) failed\n"
                            OSReport_Error("モジュールリサイズ(縮小)失敗\n");
                        }
                    }
                }
            }
        }
        OSGetTime();
        sAllocBytes = sAllocBytes + getModuleSize();
        OSGetTime();
        unk_24 = ((u32(*)())mModule->prolog)();
        OSGetTime();
        return TRUE;
    }

error:
    unk_33 = 0;
    if (mBss != NULL) {
        JKRHeap::free(mBss, NULL);
        mBss = NULL;
    }
    if (mModule != NULL) {
        JKRHeap::free(mModule, NULL);
        mModule = NULL;
    }
    return FALSE;
}

bool DynamicModuleControl::do_unlink() {
    OSTime time1 = OSGetTime();
    ((void (*)())mModule->epilog)();
    OSTime time2 = OSGetTime();
    BOOL unklink = OSUnlink(&mModule->info);
    OSTime time3 = OSGetTime();
    if (unklink == FALSE) {
        // "Unlink failed mModule=%08x mBss=%08x\n"
        OSReport_Error("アンリンク失敗 mModule=%08x mBss=%08x\n", mModule, mBss);
        return false;
    }
    sAllocBytes = sAllocBytes - getModuleSize();
    if (mBss != NULL) {
        JKRFree(mBss);
        mBss = NULL;
    }
    do_unload();
    return true;
}

int DynamicModuleControl::getModuleSize() const {
    if (mModule != NULL) {
        u32 size = JKRGetMemBlockSize(NULL, mModule);
        if (mBss != NULL) {
            JKRGetMemBlockSize(NULL, mBss);
        }
        return size + mModule->bssSize;
    } else {
        return 0;
    }
}

const char* DynamicModuleControl::getModuleTypeString() const {
    static const char* strings[4] = {"????", "MEM", "ARAM", "DVD"};
    return strings[mResourceType & 3];
}

extern "C" void ModuleProlog() {
    /* empty function */
}

extern "C" void ModuleEpilog() {
    /* empty function */
}

extern "C" void ModuleUnresolved() {
    // "\nError: Unlinked function was called.\n"
    OSReport_Error("\nError: リンクされていない関数が呼び出されました.\n");
    OSReport_Error("Address:      Back Chain    LR Save\n");
    u32 i = 0;
    u32* stackPtr = (u32*)OSGetStackPointer();
    while ((stackPtr != NULL) && ((u32)stackPtr != 0xFFFFFFFF) && (i++ < 0x10)) {
        OSReport_Error("0x%08x:   0x%08x    0x%08x\n", stackPtr, *stackPtr, *(stackPtr + 1));
        stackPtr = (u32*)*stackPtr;
    }
    OSReport_Error("\n");
}

extern "C" void ModuleConstructorsX(void (**ctors)()) {
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
}

extern "C" void ModuleDestructorsX(void (**dtors)()) {
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}
