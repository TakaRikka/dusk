#ifndef JAUSECTIONHEAP_H
#define JAUSECTIONHEAP_H

#include "JSystem/JAudio2/JAUBankTable.h"
#include "JSystem/JAudio2/JAUSeqDataBlockMgr.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "bitset.h"

class JAISeqData;
class JAISeqDataMgr;
class JAISeqDataUser;
class JAIStreamDataMgr;
class JASVoiceBank;
class JAUBankTableLink;
class JAUDisposer_;
class JAUSectionHeap;
class JAUSoundTable;
class JAUSoundNameTable;
class JKRArchive;
class JKRSolidHeap;

/**
 * @ingroup jsystem-jaudio
 *
 */
class JAUSection : public JKRDisposer, protected JSULink<JAUSection> {
public:
    struct TSectionData {
        TSectionData();
        void resetRegisteredBankTables();
        void resetRegisteredWaveBankTables();

        /* 0x00 */ JAUDynamicSeqDataBlocks field_0x00;
        /* 0x28 */ JSUList<JAUSeqDataBlock> field_0x28;
        /* 0x34 */ std::bitset<255> registeredWaveBankTables;
        /* 0x54 */ std::bitset<255> registeredBankTables;
        /* 0x74 */ JAUBankTableDictionary field_0x74;
        /* 0x80 */ JAISeqDataMgr* field_0x80;
        /* 0x84 */ const void* mBstDst;
        /* 0x88 */ const void* mBstnDst;
        /* 0x8C */ JSUList<JAUDisposer_> field_0x8c;
        /* 0x98 */ s32 field_0x98;
        /* 0x9C */ int field_0x9c;
        /* 0xA0 */ int field_0xa0;
    };

    JAUSection(JAUSectionHeap*, u32, s32);
    void finishBuild();
    virtual void dispose();
    JAUSoundTable* newSoundTable(void const*, u32, bool);
    JAUSoundNameTable* newSoundNameTable(void const*, u32, bool);
    JAIStreamDataMgr* newStreamFileTable(void const*, bool);
    JAISeqDataMgr* newSeSeqCollection(void const*, u32);
    u8* newStaticSeqDataBlock_(JAISoundID, u32);
    bool newStaticSeqData(JAISoundID, void const*, u32);
    bool newStaticSeqData(JAISoundID);
    void* newCopy(void const*, u32, s32);
    JASWaveBank* newWaveBank(u32, void const*);
    bool loadWaveArc(u32, u32);
    JASBank* newBank(void const*, u32);
    JASVoiceBank* newVoiceBank(u32, u32);
    bool beginNewBankTable(u32, u32);
    JAUBankTable* endNewBankTable();
    virtual ~JAUSection() {}

    bool isBuilding() const { return field_0x2c; }
    bool isOpen() const;
    JAUSectionHeap* asSectionHeap() { return (JAUSection*)sectionHeap_ == this ? sectionHeap_ : NULL; }
    const TSectionData& getSectionData() const { return data_; }

    JKRHeap* getHeap_();

    /* 0x28 */ u32 field_0x28;
    /* 0x2C */ bool field_0x2c;
    /* 0x30 */ JAUSectionHeap* sectionHeap_;
    /* 0x34 */ JAUBankTableLink* buildingBankTable_;
    /* 0x38 */ TSectionData data_;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAUSectionHeap : public JAUSection, public JASGlobalInstance<JAUSectionHeap>, public JAISeqDataMgr {
public:
    struct TSectionHeapData {
        TSectionHeapData();

        /* 0x000 */ JAUWaveBankTable waveBankTable;
        /* 0x404 */ JAISeqDataUser* seqDataUser;
        /* 0x408 */ JAUDynamicSeqDataBlocks seqDataBlocks;
        /* 0x430 */ JAISeqDataMgr* seSeqDataMgr_;
        /* 0x434 */ JAIStreamDataMgr* streamDataMgr_;
        /* 0x438 */ JAUSoundTable* soundTable;
        /* 0x43C */ JAUSoundNameTable* soundNameTable;
    };

    void setSeqDataArchive(JKRArchive*);
    bool loadDynamicSeq(JAISoundID, bool);
    u32 releaseIdleDynamicSeqDataBlock();
    JAUSectionHeap(JKRSolidHeap*, bool, s32);
    JAUSection* getOpenSection();
    bool setSeqDataUser(JAISeqDataUser*);
    bool newDynamicSeqBlock(u32);
    SeqDataReturnValue getSeqData(JAISoundID, JAISeqData*);
    int releaseSeqData();

    JAISeqDataMgr* getSeqSeqDataMgr() { return this; }
    JAISeqDataMgr* getSeSeqDataMgr() { return sectionHeapData_.seSeqDataMgr_; }
    JAIStreamDataMgr* getStreamDataMgr() { return sectionHeapData_.streamDataMgr_; }
    TSectionHeapData const& getSectionHeapData() const { return sectionHeapData_; }
    JAUWaveBankTable& getWaveBankTable() { return sectionHeapData_.waveBankTable; }
    JKRHeap* getHeap() {
        if (getOpenSection()->isBuilding() != 0) {
            return mHeap;
        } 
        return NULL;
    }

    /* 0xE0 */ JKRHeap* mHeap;
    /* 0xE4 */ int field_0xe4;
    /* 0xE8 */ JSUList<JAUSection> mSectionList;
    /* 0xF4 */ TSectionHeapData sectionHeapData_;
};

inline JKRHeap* JAUSection::getHeap_() { return sectionHeap_->mHeap; }
inline bool JAUSection::isOpen() const { return this == sectionHeap_->getOpenSection(); }

JAUSectionHeap* JAUNewSectionHeap(bool);

#endif /* JAUSECTIONHEAP_H */
