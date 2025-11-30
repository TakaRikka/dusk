#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUSeqCollection.h"
#include "JSystem/JUtility/JUTAssert.h"

JAUSeqCollection::JAUSeqCollection() {
    field_0x8 = NULL;
}

void JAUSeqCollection::init(void const* param_0) {
    field_0x8 = (const JAUSeqCollectionData*)param_0;
    if (field_0x8->field_0x0 != 0x53 || field_0x8->field_0x1 != 0x43) {
        field_0x8 = NULL;
        return;
    }

    field_0x0 = field_0x8->field_0x2;
    field_0xc = field_0x8->field_0x4;
    field_0x4 = &field_0x8->field_0x8;

}

bool JAUSeqCollection::getSeqData(int param_0, int param_1, JAISeqData* param_2) {
    if (param_0 >= field_0x0) {
        return false;
    }
    u32* puVar2 = (u32*)((u8*)field_0x8 + field_0x4[param_0]);
    if (param_1 >= puVar2[0]) {
        return false;
    }
    param_2->set((void*)field_0x8, puVar2[param_1 + 1]);
    return true;
}

bool JAUSeqCollection::getSeqDataRegion(JAISeqDataRegion* param_0) {
    if (isValid()) {
        param_0->addr = (u8*)field_0x8;
        param_0->size = field_0xc;
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
        return user_->releaseSeqData(region);
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
