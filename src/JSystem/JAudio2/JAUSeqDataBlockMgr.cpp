//
// JAUSeqDataBlockMgr
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUSeqDataBlockMgr.h"
#include "JSystem/JAudio2/JAUSoundInfo.h"
#include "JSystem/JAudio2/JASResArcLoader.h"
#include "dolphin/types.h"

JAUSeqDataBlock::JAUSeqDataBlock() : field_0x0(this) {}

u8* JAUSeqDataBlocks::getSeqData(JAISoundID param_0) {
    if (param_0.isAnonymous()) {
        return NULL;
    }
    JSULink<JAUSeqDataBlock>* link;
    for (link = getFirst(); link != NULL;
            link = link->getNext())
    {
        if ((u32)link->getObject()->field_0x10 == (u32)param_0) {
            return link->getObject()->region.addr;
        }
    }
    return NULL;
}

JSULink<JAUSeqDataBlock>* JAUSeqDataBlocks::seekFreeBlock(u32 size) {
    u32 currentBlockSize = 0xffffffff;
    JSULink<JAUSeqDataBlock>* rv = NULL;
    for (JSULink<JAUSeqDataBlock>* link = getFirst(); link != NULL; link = link->getNext())
    {
        if (link->getObject()->field_0x10.isAnonymous()) {
            u32 blockSize = link->getObject()->region.size;
            if (blockSize >= size && blockSize < currentBlockSize) {
                rv = link;
                currentBlockSize = blockSize;
            }
        }
    }
    if (rv != NULL) {
        return rv;
    }
    return NULL;
}

bool JAUSeqDataBlocks::append(JSULink<JAUSeqDataBlock>* param_0) {
    return JSUList::append(param_0);
}

bool JAUSeqDataBlocks::remove(JSULink<JAUSeqDataBlock>* param_0) {
    return JSUList::remove(param_0);
}

bool JAUSeqDataBlocks::hasFailedBlock(JAISoundID param_0) {
    JSULink<JAUSeqDataBlock>* link;
    for (link = getFirst(); link != NULL; link = link->getNext())
    {
        if (link->getObject()->field_0x10 == param_0) {
            link->getObject()->field_0x10.setAnonymous();
            return true;
        }
    }
    return false;
}

JAUDynamicSeqDataBlocks::JAUDynamicSeqDataBlocks() {
    seqDataArchive_ = NULL;
}

void JAUDynamicSeqDataBlocks::setSeqDataArchive(JKRArchive* param_0) {
    JUT_ASSERT(104, seqDataArchive_ == NULL);
    seqDataArchive_ = param_0;
}

s32 JAUDynamicSeqDataBlocks::getSeqData(JAISoundID param_0, JAISeqDataUser* param_1,
                                        JAISeqData* param_2, bool param_3) {
    rearrangeLoadingSeqs_();
    if (mFreeBlocks.hasFailedBlock(param_0)) {
        return 0;
    }

    if (field_0xc.getSeqData(param_0)) {
        return 1;
    }

    u8* seqData = mLoadedBlocks.getSeqData(param_0);
    if (seqData != NULL) {
        param_2->field_0x0 = seqData;
        param_2->field_0x4 = 0;
        return 2;
    }

    if (loadDynamicSeq(param_0, param_3, param_1)) {
        return 1;
    }
    return 0;
}

bool JAUDynamicSeqDataBlocks::appendDynamicSeqDataBlock(JAUSeqDataBlock* seqDataBlock) {
    JUT_ASSERT(135, ( reinterpret_cast < u32 > ( seqDataBlock->region.addr ) & 0x1f ) == 0);
    rearrangeLoadingSeqs_();
    bool result;
    if (seqDataBlock->field_0x10.isAnonymous()) {
        result = mFreeBlocks.append(&seqDataBlock->field_0x0);
    } else {
        result = mLoadedBlocks.append(&seqDataBlock->field_0x0);
    }
    JUT_ASSERT(144, result);
    return 1;
}

static void JAUDynamicSeqDataBlocks_receiveLoaded_(u32 param_0, u32 param_1) {
    JSULink<JAUSeqDataBlock>* link = (JSULink<JAUSeqDataBlock>*)param_1;
    JAUSeqDataBlock* seqDataBlock = link->getObject();
    if (param_0 != NULL) {
        seqDataBlock->field_0x1c = 2;
    } else {
        seqDataBlock->field_0x1c = 0;
    }
}

bool JAUDynamicSeqDataBlocks::loadDynamicSeq(JAISoundID param_0, bool param_1,
                                             JAISeqDataUser* param_2) {
    {
        if (seqDataArchive_ == NULL) {
            JUT_WARN(192, "%s", "cannot get sequence data archive.");
            return false;
        }
        JAUSoundInfo* soundInfo = JASGlobalInstance<JAUSoundInfo>::getInstance();
        if (soundInfo == NULL) {
            JUT_WARN(198, "%s", "cannot JAUSoundInfo::getInstance().");
            return false;
        }
        u16 resourceId = soundInfo->getBgmSeqResourceID(param_0);
        size_t resSize = JASResArcLoader::getResSize(seqDataArchive_, resourceId);
        JSULink<JAUSeqDataBlock>* link = mFreeBlocks.seekFreeBlock(resSize);
        if (link == NULL) {
            if (param_1) {
                link = &releaseIdleDynamicSeqDataBlock_(param_2, resSize)->field_0x0;
                if (link == NULL) {
                    return false;
                }
            } else {
                return false;
            }
        }
        mFreeBlocks.remove(link);
        link->getObject()->field_0x10 = param_0;
        link->getObject()->field_0x1c = 1;
        field_0xc.append(link);
        JASResArcLoader::loadResourceAsync(
            seqDataArchive_, resourceId,
            link->getObject()->region.addr, link->getObject()->region.size,
            JAUDynamicSeqDataBlocks_receiveLoaded_, (u32)link);
    }
    return true;
}

u32 JAUDynamicSeqDataBlocks::releaseIdleDynamicSeqDataBlock(JAISeqDataUser* param_0) {
    rearrangeLoadingSeqs_();
    u32 size = 0;
    JSULink<JAUSeqDataBlock>* nextLink;
    for (JSULink<JAUSeqDataBlock>* link = mLoadedBlocks.getFirst(); link != NULL; ) {
        nextLink = link->getNext();
        JAUSeqDataBlock* seqDataBlock = link->getObject();
        if (param_0 == NULL || !param_0->isUsingSeqData(seqDataBlock->region)) {
            mLoadedBlocks.remove(link);
            JAUSeqDataBlock* seqDataBlock = link->getObject();
            seqDataBlock->field_0x10.setAnonymous();
            if (size < seqDataBlock->region.size) {
                size = seqDataBlock->region.size;
            }
            mFreeBlocks.append(link);
        }
        link = nextLink;
    }
    return size;
}

JAUSeqDataBlock*
JAUDynamicSeqDataBlocks::releaseIdleDynamicSeqDataBlock_(JAISeqDataUser* param_0, u32 param_1) {
    rearrangeLoadingSeqs_();
    u32 minSize = 0xffffffff;
    JSULink<JAUSeqDataBlock>* foundBlock = NULL;
    JSULink<JAUSeqDataBlock>* link = mLoadedBlocks.getFirst();
    JSULink<JAUSeqDataBlock>* nextLink;
    while (link != NULL) {
        nextLink = link->getNext();
        JAUSeqDataBlock* seqDataBlock = link->getObject();
        if (param_0 == NULL || !param_0->isUsingSeqData(seqDataBlock->region)) {
            u32 size = seqDataBlock->region.size;
            if (size >= param_1 && size < minSize) {
                foundBlock = link;
                minSize = size;
            }
        }
        link = nextLink;
    }
    if (foundBlock != NULL) {
        mLoadedBlocks.remove(foundBlock);
        JAUSeqDataBlock* seqDataBlock = foundBlock->getObject();
        seqDataBlock->field_0x10.setAnonymous();
        mFreeBlocks.append(foundBlock);
        return foundBlock->getObject();
    }
    return NULL;
}

void JAUDynamicSeqDataBlocks::rearrangeLoadingSeqs_() {
    JSULink<JAUSeqDataBlock>* link = field_0xc.getFirst();
    JSULink<JAUSeqDataBlock>* nextLink;
    while (link != NULL) {
        nextLink = link->getNext();
        JAUSeqDataBlock* seqDataBlock = link->getObject();
        if (seqDataBlock->field_0x1c == 0) {
            field_0xc.remove(link);
            mFreeBlocks.append(link);
        } else if (seqDataBlock->field_0x1c == 2) {
            field_0xc.remove(link);
            mLoadedBlocks.append(link);
        }
        link = nextLink;
    }
}
