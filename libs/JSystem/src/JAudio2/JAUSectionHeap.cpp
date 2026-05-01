#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUSectionHeap.h"
#include "JSystem/JAudio2/JASBNKParser.h"
#include "JSystem/JAudio2/JASBankTable.h"
#include "JSystem/JAudio2/JASBasicWaveBank.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JASResArcLoader.h"
#include "JSystem/JAudio2/JASVoiceBank.h"
#include "JSystem/JAudio2/JASWSParser.h"
#include "JSystem/JAudio2/JAUSeqCollection.h"
#include "JSystem/JAudio2/JAUSoundInfo.h"
#include "JSystem/JAudio2/JAUSoundTable.h"
#include "JSystem/JAudio2/JAUStreamFileTable.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include <dvd.h>
#include <cstring>

namespace {
    class TPushCurrentHeap {
    public:
        TPushCurrentHeap(JKRHeap* heap) { mHeap = JKRSetCurrentHeap(heap); }
        ~TPushCurrentHeap() { JKRSetCurrentHeap(mHeap); }

    private:
        JKRHeap* mHeap;
    };

    class TStreamDataMgr : public JAIStreamDataMgr {
    public:
        TStreamDataMgr(const void* param_0) {
            JAUStreamFileTable stack_14;
            stack_14.init(param_0);
            if (!stack_14.isValid()) {
                mNumStreamFiles = 0;
                mStreamFileDVDEntryNums = NULL;
                return;
            }
            mNumStreamFiles = stack_14.getNumFiles();
            mStreamFileDVDEntryNums = JKR_NEW_ARRAY(s32, mNumStreamFiles);
            if (!mStreamFileDVDEntryNums) {
                mNumStreamFiles = 0;
                return;
            }
            for (u32 i = 0; i < mNumStreamFiles; i++) {
                mStreamFileDVDEntryNums[i] = DVDConvertPathToEntrynum(stack_14.getFilePath(i));
            }
        }
        virtual s32 getStreamFileEntry(JAISoundID id) {
            u32 short_id = id.id_.info.waveID;
            if (short_id >= mNumStreamFiles) {
                return -1;
            }
            return mStreamFileDVDEntryNums[short_id];
        }

        bool isValid() { return mNumStreamFiles; }

        u32 mNumStreamFiles;
        s32* mStreamFileDVDEntryNums;
    };
}

namespace {
    bool JKRSolidHeap_isEmpty(JKRSolidHeap* pHeap) {
        u32 beforeSize = pHeap->getFreeSize();
        pHeap->freeAll();
        return pHeap->getFreeSize() - beforeSize == 0;
    }
}

JAUSection::TSectionData::TSectionData() {
    resetRegisteredWaveBankTables();
    resetRegisteredBankTables();
    mBstDst = NULL;
    mBstnDst = NULL;
    field_0x80 = NULL;
    mWaveBankMemoryUsage = 0;
    mBankMemoryUsage = 0;
}

void JAUSection::TSectionData::resetRegisteredBankTables() {
    for (u32 i = 0; i < 255; i++) {
        registeredBankTables.reset(i);
    }
}

void JAUSection::TSectionData::resetRegisteredWaveBankTables() {
    for (u32 i = 0; i < 255; i++) {
        registeredWaveBankTables.reset(i);
    }
}

JAUSection::JAUSection(JAUSectionHeap* param_0, u32 param_1, s32 param_2)
            : JSULink<JAUSection>(this), field_0x28(param_1), sectionHeap_(param_0) {
    buildingBankTable_ = NULL;
    field_0x2c = 1;
    data_.field_0x98 = param_2;
    if (this != sectionHeap_) {
        data_.field_0x00.setSeqDataArchive(sectionHeap_->sectionHeapData_.seqDataBlocks.getSeqDataArchive());
    }
}

void JAUSection::finishBuild() {
    JUT_ASSERT(143, isOpen());
    JUT_ASSERT(144, isBuilding());
    {
        TPushCurrentHeap push(getHeap_());
    }
    data_.field_0x98 -= getHeap_()->getFreeSize();
    getHeap_()->freeTail();
    field_0x2c = 0;
}

void JAUSection::dispose() {
    JUT_ASSERT(267, ! data_.registeredBankTables.any());
    JUT_ASSERT(268, ! data_.registeredWaveBankTables.any());
    if (data_.mBstDst) {
        sectionHeap_->sectionHeapData_.soundTable->~JAUSoundTable();
        sectionHeap_->sectionHeapData_.soundTable = NULL;
    }
    if ((data_.mBstnDst)) {
        sectionHeap_->sectionHeapData_.soundNameTable->~JAUSoundNameTable();
        sectionHeap_->sectionHeapData_.soundNameTable = NULL;
    }
}

JAUSoundTable* JAUSection::newSoundTable(void const* bst, u32 param_1, bool param_2) {
    JUT_ASSERT(285, isOpen());
    JUT_ASSERT(286, isBuilding());
    JUT_ASSERT(287, bst);
    JUT_ASSERT(288, asSectionHeap() == this);
    JUT_ASSERT(289, sectionHeap_->sectionHeapData_.soundTable == NULL);
    {
        TPushCurrentHeap push(getHeap_());
        void const* bstDst = bst;
        if (param_1) {
            bstDst = newCopy(bst, param_1, 4);
            JUT_ASSERT(296, bstDst);
        }
        JAUSoundTable* soundTable = JKR_NEW JAUSoundTable(param_2);
        JUT_ASSERT(299, soundTable);
        soundTable->init(bstDst);
        sectionHeap_->sectionHeapData_.soundTable = soundTable;
        data_.mBstDst = bstDst;
    }
    return sectionHeap_->sectionHeapData_.soundTable;
}

JAUSoundNameTable* JAUSection::newSoundNameTable(void const* bstn, u32 param_1, bool param_2) {
    JUT_ASSERT(315, isOpen());
    JUT_ASSERT(316, isBuilding());
    JUT_ASSERT(317, bstn);
    JUT_ASSERT(318, asSectionHeap() == this);
    JUT_ASSERT(319, sectionHeap_->sectionHeapData_.soundNameTable == NULL);
    {
        TPushCurrentHeap push(getHeap_());
        void const* bstnDst = bstn;
        if (param_1) {
            bstnDst = newCopy(bstn, param_1, 4);
            JUT_ASSERT(326, bstnDst);
        }
        JAUSoundNameTable* soundNameTable = JKR_NEW JAUSoundNameTable(param_2);
        JUT_ASSERT(329, soundNameTable);
        soundNameTable->init(bstnDst);
        sectionHeap_->sectionHeapData_.soundNameTable = soundNameTable;
        data_.mBstnDst = bstnDst;
    }
    return sectionHeap_->sectionHeapData_.soundNameTable;
}

JAIStreamDataMgr* JAUSection::newStreamFileTable(void const* param_0, bool param_1) {
    JUT_ASSERT(345, asSectionHeap() == this);
    JUT_ASSERT(346, isOpen());
    JUT_ASSERT(347, isBuilding());
    JUT_ASSERT(348, sectionHeap_->sectionHeapData_.streamDataMgr_ == NULL);
    {
        TPushCurrentHeap push(getHeap_());
        JAIStreamDataMgr* r28 = NULL;
        if (param_1) {
            TStreamDataMgr* r26 = JKR_NEW TStreamDataMgr(param_0);
            if (r26->isValid()) {
                r28 = r26;
            }
        } else {
            JAUStreamDataMgr_StreamFileTable* r25 = JKR_NEW JAUStreamDataMgr_StreamFileTable();
            r25->init(param_0);
            if (r25->isValid()) {
                r28 = r25;
            }
        }
        sectionHeap_->sectionHeapData_.streamDataMgr_ = r28;
    }
    return sectionHeap_->sectionHeapData_.streamDataMgr_;
}

JAISeqDataMgr* JAUSection::newSeSeqCollection(void const* bsc, u32 param_1) {
    JUT_ASSERT(398, asSectionHeap() == this);
    JUT_ASSERT(399, sectionHeap_->sectionHeapData_.seSeqDataMgr_ == NULL);
    if (param_1) {
        bsc = newCopy(bsc, param_1, 4);
    }
    JUT_ASSERT(405, bsc);
    {
        TPushCurrentHeap push(getHeap_());
        JAUSeqDataMgr_SeqCollection* seSeqDataMgr = JKR_NEW JAUSeqDataMgr_SeqCollection();
        JUT_ASSERT(409, seSeqDataMgr);
        seSeqDataMgr->init(bsc);
        sectionHeap_->sectionHeapData_.seSeqDataMgr_ = seSeqDataMgr;
        data_.field_0x80 = seSeqDataMgr;
    }
    return sectionHeap_->sectionHeapData_.seSeqDataMgr_;
}

u8* JAUSection::newStaticSeqDataBlock_(JAISoundID param_0, u32 size) {
    JUT_ASSERT(419, isOpen());
    JUT_ASSERT(420, isBuilding());
    JUT_ASSERT(421, size);
    {
        TPushCurrentHeap push(getHeap_());
        JAUSeqDataBlock* seqDataBlock = JKR_NEW JAUSeqDataBlock();
        if (!seqDataBlock) {
            return NULL;
        }
        JSULink<JAUSeqDataBlock>* link = JKR_NEW JSULink<JAUSeqDataBlock>(seqDataBlock);
        if (!link) {
            JUT_WARN(432, "%s", "created UNUSED object in Heap\n");
            return NULL;
        }
        u8* r28 = JKR_NEW_ARRAY_ARGS(u8, size, 0x20);
        if (!r28) {
            JUT_WARN(438, "%s", "created UNUSED object in Heap\n");
            return NULL;
        }
        seqDataBlock->region.addr = r28;
        seqDataBlock->region.size = size;
        seqDataBlock->field_0x10 = param_0;
        JASCriticalSection cs;
        if(data_.field_0x00.appendDynamicSeqDataBlock(seqDataBlock)) {
            data_.field_0x28.append(link);
            return r28;
        }
    }
    return NULL;
}

bool JAUSection::newStaticSeqData(JAISoundID param_0, void const* param_1, u32 size) {
    {
        u8* r30 = newStaticSeqDataBlock_(param_0, size);
        if (r30) {
            memcpy(r30, param_1, size);
            return true;
        }
    }
    return false;
}

bool JAUSection::newStaticSeqData(JAISoundID param_0) {
    JKRArchive* seqArchive = data_.field_0x00.getSeqDataArchive();
    JUT_ASSERT(481, seqArchive);
    JAUSoundInfo* soundInfo = JASGlobalInstance<JAUSoundInfo>::getInstance();
    if (!soundInfo) {
        JUT_WARN(485, "%s","cannot JAUSoundInfo::getInstance().");
        return false;
    }
    u16 r26 = soundInfo->getBgmSeqResourceID(param_0);
    u32 r25 = JASResArcLoader::getResSize(seqArchive, r26);
    u8* r24 = newStaticSeqDataBlock_(0xffffffff, r25);
    if (r24) {
        data_.field_0x00.loadDynamicSeq(param_0, true, sectionHeap_->sectionHeapData_.seqDataUser);
        return true;
    }
    return false;
}

void* JAUSection::newCopy(void const* param_0, u32 param_1, s32 param_2) {
    JUT_ASSERT(516, isOpen());
    JUT_ASSERT(517, isBuilding());
    u8* r31 = JKR_NEW_ARRAY_ARGS(u8, param_1, getHeap_(), param_2);
    if (r31) {
        memcpy(r31, param_0, param_1);
    }
    return r31;
}

static bool dummy_string(int i) {
    std::bitset<1> x;
    return x.test(i);
}

JASWaveBank* JAUSection::newWaveBank(u32 bank_no, void const* param_1) {
    JUT_ASSERT(528, isOpen());
    JUT_ASSERT(529, isBuilding());
    {
        TPushCurrentHeap push(getHeap_());
        s32 previousFree = getHeap_()->getFreeSize();
        JASWaveBank* waveBank = JASWSParser::createWaveBank(param_1, getHeap_());
        if (waveBank) {
            JUT_ASSERT(536, sectionHeap_->getWaveBankTable().getWaveBank( bank_no ) == NULL);
            sectionHeap_->getWaveBankTable().registWaveBank(bank_no, waveBank);
            data_.registeredWaveBankTables.set(bank_no, true);
            data_.mWaveBankMemoryUsage += previousFree - getHeap_()->getFreeSize();
            return waveBank;
        }
    }
    return NULL;

}

bool JAUSection::loadWaveArc(u32 param_0, u32 param_1) {
    if (data_.registeredWaveBankTables.test(param_0)) {
        JASWaveBank* waveBank = sectionHeap_->getWaveBankTable().getWaveBank(param_0);
        if (waveBank) {
            for (u32 i = 0; i < waveBank->getArcCount(); i++) {
                if (param_1 & 1 << i) {
                    JASWaveArc* waveArc = waveBank->getWaveArc(i);
                    waveArc->load(NULL);
                }
            }
            return true;
        }
    }
    return false;
}

JASBank* JAUSection::newBank(void const* param_0, u32 param_1) {
    JUT_ASSERT(647, isOpen());
    JUT_ASSERT(648, isBuilding());
    JASWaveBank* waveBank = sectionHeap_->getWaveBankTable().getWaveBank(param_1);
    JUT_ASSERT(650, waveBank != NULL);
    {
        TPushCurrentHeap push(getHeap_());
        u32 bank_no = JASBNKParser::getBankNumber(param_0);
        s32 previousFree = getHeap_()->getFreeSize();
        JASBank* bank = JASBNKParser::createBank(param_0, getHeap_());
        if (bank) {
            if (buildingBankTable_) {
                JUT_ASSERT(660, buildingBankTable_->getBank( bank_no ) == NULL);
                buildingBankTable_->registBank(bank_no, bank);
            } else {
                JUT_ASSERT(665, JASDefaultBankTable::getInstance() ->getBank( bank_no ) == NULL);
                JASDefaultBankTable::getInstance()->registBank(bank_no, bank);
                data_.registeredBankTables.set(bank_no, true);
            }
            bank->assignWaveBank(waveBank);
            data_.mBankMemoryUsage += previousFree - getHeap_()->getFreeSize();
            return bank;
        }
    }
    return 0;
}

JASVoiceBank* JAUSection::newVoiceBank(u32 bank_no, u32 param_1) {
    {
        JUT_ASSERT(685, isOpen());
        JUT_ASSERT(686, isBuilding());
        JASWaveBank* waveBank = sectionHeap_->getWaveBankTable().getWaveBank(param_1);
        JUT_ASSERT(688, waveBank != NULL);
        TPushCurrentHeap push(getHeap_()); 
        JASBank* voiceBank = JKR_NEW JASVoiceBank();
        if (voiceBank) {
            if (buildingBankTable_) {
                JUT_ASSERT(696, buildingBankTable_->getBank( bank_no ) == NULL);
                buildingBankTable_->registBank(bank_no, voiceBank);
            } else {
                JUT_ASSERT(701, JASDefaultBankTable::getInstance() ->getBank( bank_no ) == NULL);
                JASDefaultBankTable::getInstance()->registBank(bank_no, voiceBank);
                data_.registeredBankTables.set(bank_no, true);
            }
            voiceBank->assignWaveBank(waveBank);
            return (JASVoiceBank*)voiceBank;
        }
    }
    return NULL;
}

bool JAUSection::beginNewBankTable(u32 param_0, u32 param_1) {
    JUT_ASSERT(714, isOpen());
    JUT_ASSERT(715, isBuilding());
    JUT_ASSERT(716, buildingBankTable_ == NULL);
    JAUBankTableLink* bankTableLink = NULL;
    {
        TPushCurrentHeap push(getHeap_());
        JASBank** r26 = JKR_NEW_ARRAY(JASBank*, param_1);
        if (r26) {
            bankTableLink = JKR_NEW JAUBankTableLink(param_0, r26, param_1);
            if (bankTableLink) {
                buildingBankTable_ = bankTableLink;
            } else {
                JUT_WARN(730, "%s","created UNUSED object in Heap\n");
            }
        } else {
            JUT_WARN(735, "%s","created UNUSED object in Heap\n");
        }
    }
    return bankTableLink;
}

JAUBankTable* JAUSection::endNewBankTable() {
    JAUBankTable* r29 = NULL;
    if (buildingBankTable_) {
        data_.field_0x74.appendBankTable(buildingBankTable_);
        buildingBankTable_ = NULL;
        r29 = buildingBankTable_;
    }
    return r29;
}

JAUSectionHeap::TSectionHeapData::TSectionHeapData() {
    seSeqDataMgr_ = NULL;
    streamDataMgr_ = NULL;
    soundTable = NULL;
    soundNameTable = NULL;
    seqDataUser = NULL;
}

void JAUSectionHeap::setSeqDataArchive(JKRArchive* param_0) {
    sectionHeapData_.seqDataBlocks.setSeqDataArchive(param_0);
    JSULink<JAUSection>* link;
    for (link = mSectionList.getFirst(); link; link = link->getNext()) {
        link->getObject()->data_.field_0x00.setSeqDataArchive(param_0);
    }
}

bool JAUSectionHeap::loadDynamicSeq(JAISoundID param_0, bool param_1) {
    return sectionHeapData_.seqDataBlocks.loadDynamicSeq(param_0, param_1, sectionHeapData_.seqDataUser);
}

u32 JAUSectionHeap::releaseIdleDynamicSeqDataBlock() {
    return sectionHeapData_.seqDataBlocks.releaseIdleDynamicSeqDataBlock(sectionHeapData_.seqDataUser);
}

static JAUSectionHeap* JAUNewSectionHeap(JKRSolidHeap* heap, bool param_1) {
    JUT_ASSERT(809, JKRSolidHeap_isEmpty( heap ));
    TPushCurrentHeap push(heap);
    s32 r29 = heap->getFreeSize();
    JAUSectionHeap* sectionHeap = JKR_NEW JAUSectionHeap(heap, param_1, r29);
    return sectionHeap;
}

JAUSectionHeap* JAUNewSectionHeap(bool param_0) {
    s32 freeSize = JASDram->getFreeSize();
    JKRSolidHeap* sectionHeap = JKRCreateSolidHeap(freeSize, JASDram, true);
    JKRHEAP_NAME(sectionHeap, "sectionHeap");
    JUT_ASSERT(821, sectionHeap);
    return JAUNewSectionHeap(sectionHeap, param_0);
}

JAUSectionHeap::JAUSectionHeap(JKRSolidHeap* param_0, bool param_1, s32 param_2)
        : JAUSection(this, 0, param_2), JASGlobalInstance<JAUSectionHeap>(param_1), mHeap(param_0) {
    field_0xe4 = 0;
    mSectionList.append(this);
}

JAUSection* JAUSectionHeap::getOpenSection() {
    return mSectionList.getLast()->getObject();
}

JAUSection* JAUSectionHeap::getSection(int param_0) {
    JSULink<JAUSection>* link = mSectionList.getFirst();
    while (param_0 > 0) {
        JSULink<JAUSection>* link2 = link->getNext();
        if (!link2) {
            return NULL;
        }
        link = link2;
        param_0--;
    }
    return link->getObject();
}

bool JAUSectionHeap::setSeqDataUser(JAISeqDataUser* param_0) {
    sectionHeapData_.seqDataUser = param_0;
    return true;
}

bool JAUSectionHeap::newDynamicSeqBlock(u32 size) {
    JUT_ASSERT(937, isOpen());
    JUT_ASSERT(938, isBuilding());
    JUT_ASSERT(939, sectionHeap_ == this);
    {
        TPushCurrentHeap push(getHeap_());
        JAUSeqDataBlock * seqDataBlock = JKR_NEW JAUSeqDataBlock();
        if (!seqDataBlock) {
            return false;
        }
        JSULink<JAUSeqDataBlock> * link = JKR_NEW JSULink<JAUSeqDataBlock>(seqDataBlock);
        if (!link) {
            JUT_WARN(950, "%s", "created UNUSED object in Heap\n");
            return false;
        }
        u8* r25 = JKR_NEW_ARRAY_ARGS(u8, size, 0x20);
        if (!r25) {
            JUT_WARN(956, "%s", "created UNUSED object in Heap\n");
            return false;
        }
        seqDataBlock->region.addr = r25;
        seqDataBlock->region.size = size;
        seqDataBlock->field_0x10.setAnonymous();
        JASCriticalSection cs;
        if (sectionHeap_->sectionHeapData_.seqDataBlocks.appendDynamicSeqDataBlock(seqDataBlock)) {
            return true;
        }
        JUT_WARN(968, "%s","created UNUSED object in Heap\n");
    }
    return false;
}

JAISeqDataMgr::SeqDataReturnValue JAUSectionHeap::getSeqData(JAISoundID param_0, JAISeqData* param_1) {
    const int JAI_ASYNC_RESULT_RETRY = 1;
    for (JSULink<JAUSection> * link = mSectionList.getFirst(); link; link = link->getNext()) {
        s32 result = link->getObject()->data_.field_0x00.getSeqData(param_0, sectionHeapData_.seqDataUser, param_1, false);
        JUT_ASSERT(994, result != JAI_ASYNC_RESULT_RETRY);
        if (result == 2) {
            return JAISeqDataMgr::SeqDataReturnValue_2;
        }
    }
    return (SeqDataReturnValue)sectionHeapData_.seqDataBlocks.getSeqData(param_0, sectionHeapData_.seqDataUser, param_1, true);
}

int JAUSectionHeap::releaseSeqData() {
    JUT_WARN(1005, "%s", "JAUSectionHeap cannot release SeqData all at once. Please pop Section.\n");
    return 0;
}
