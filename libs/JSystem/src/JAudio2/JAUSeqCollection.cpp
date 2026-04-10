#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUSeqCollection.h"
#include "JSystem/JUtility/JUTAssert.h"

JAUSeqCollection::JAUSeqCollection() {
    mHeader = NULL;
}

void JAUSeqCollection::init(void const* param_0) {
    mHeader = (const JAUSeqCollectionData*)param_0;
    if (mHeader->mMagic1 != 0x53 || mHeader->mMagic2 != 0x43) {
        mHeader = NULL;
        return;
    }

    mNumSoundCategories = mHeader->mNumSoundCategories;
    mSectionSize = mHeader->mSectionSize;
    mTableOffsets = mHeader->mTableOffsets;

}

bool JAUSeqCollection::getSeqData(int param_0, int param_1, JAISeqData* param_2) {
    if (param_0 >= mNumSoundCategories) {
        return false;
    }
    u32 r29 = mTableOffsets[param_0];
    BE(u32)* puVar2 = (BE(u32)*)((u8*)mHeader + r29);
    if (param_1 >= puVar2[0]) {
        return false;
    }
    param_2->set((void*)mHeader, puVar2[param_1 + 1]);
    return true;
}

bool JAUSeqCollection::getSeqDataRegion(JAISeqDataRegion* param_0) {
    if (isValid()) {
        param_0->addr = (u8*)mHeader;
        param_0->size = mSectionSize;
        return true;
    }
    return false;
}

JAUSeqDataMgr_SeqCollection::JAUSeqDataMgr_SeqCollection() {
    user_ = NULL;
}

bool JAUSeqDataMgr_SeqCollection::setSeqDataUser(JAISeqDataUser* param_0) {
    JUT_ASSERT(60, user_ == NULL);
    user_ = param_0;
    return true;
}

int JAUSeqDataMgr_SeqCollection::releaseSeqData() {
    if (user_) {
        JAISeqDataRegion region;
        getSeqDataRegion(&region);
        int result = user_->releaseSeqData(region);
        return result;
    }
    return 2;
}

JAUSeqDataMgr_SeqCollection::SeqDataReturnValue JAUSeqDataMgr_SeqCollection::getSeqData(JAISoundID param_0, JAISeqData* param_1) {
    if (!isValid()) {
        return SeqDataReturnValue_0;
    }
    if (JAUSeqCollection::getSeqData(param_0.id_.info.type.parts.groupID, param_0.id_.info.waveID, param_1)) {
        return SeqDataReturnValue_2;
    }
    return SeqDataReturnValue_0;
}

JAUSeqDataMgr_SeqCollection::~JAUSeqDataMgr_SeqCollection() {}
